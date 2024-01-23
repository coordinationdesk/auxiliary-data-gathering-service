
-- Version 9.0.0
-- POSTGRES








---------------------------------------------------------------------------------
-------------------------------- TABLES CREATION --------------------------------
---------------------------------------------------------------------------------

----------------------
--- T_POSTINVQUEUE ---
----------------------
CREATE TABLE t_postinvqueue (
    id SERIAL NOT NULL,
    filename VARCHAR(128) NOT NULL,
	fileversion VARCHAR(8) NOT NULL,
	filetype VARCHAR(16) NOT NULL,
	validitystart TIMESTAMP(6) NOT NULL,
	validitystop TIMESTAMP(6) NOT NULL,
	tstamp TIMESTAMP(6) DEFAULT NOW() NOT NULL
);
ALTER TABLE t_postinvqueue 
	ADD CONSTRAINT pk_t_postinvqueue 
		PRIMARY KEY (id);








---------------------------------------------------------------------------------
------------------------------- TABLES ALTERATION -------------------------------
---------------------------------------------------------------------------------

---------------------
--- T_INVENTORYSM ---
---------------------
CREATE INDEX ix_invsm_storagemanid
   ON t_inventorysm (storagemanid);


---------------------
--- T_SCHEMATYPES ---
---------------------
UPDATE t_schematypes SET schematype = 'PDS' WHERE schematype = 'PDS ONLY' ;


---------------------
--- T_ORDERSQUEUE ---
---------------------
ALTER TABLE t_ordersqueue ALTER COLUMN externalorderid TYPE VARCHAR(128);


--------------------
--- T_SATELLIETS ---
--------------------
UPDATE t_satellites SET satellitename = 'Oceansat' WHERE satellitename = 'Oceansat2';


----------------------
--- T_SATRELATIONS ---
----------------------
INSERT INTO t_satrelations VALUES (32, 1, 60) ;




---------------------------------------------------------------------------------
--------------------------------- VIEW CREATION ---------------------------------
---------------------------------------------------------------------------------

----------------------
--- V_SATRELATIONS ---
----------------------
CREATE VIEW v_satrelations (
	satellite, 
	mission, 
	sensor)
AS SELECT 	
	sa.satellitename AS satellite, 
	sr.mission AS mission, 
	se.sensorname AS sensor
FROM 
	t_satrelations AS sr
	INNER JOIN t_satellites AS sa 
		ON sr.satelliteid = sa.satelliteid
	INNER JOIN t_sensors AS se 
		ON sr.sensorid = se.sensorid
ORDER BY 
	satellite, 
	mission, 
	sensor ;








---------------------------------------------------------------------------------
------------------------------ PROCEDURES CREATION ------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	P_POSTINQUEUE_INSERT
*/ ------------------------------
CREATE FUNCTION p_postinvqueue_insert(
	in_fname VARCHAR(128), 
	in_fversion VARCHAR(8)) RETURNS INTEGER AS $$
	
DECLARE
	rec RECORD;
	
BEGIN

	SELECT INTO rec 
		filetype, 
		validitystart, 
		validitystop 
	FROM 
		t_inventory 
	WHERE 
		filename = in_fname 
		AND fileversion = in_fversion;
	
	
	INSERT INTO t_postinvqueue (
		filename,
		fileversion,
		filetype,
		validitystart,
		validitystop)
	VALUES (
		in_fname,
		in_fversion,
		rec.filetype,
		rec.validitystart,
		rec.validitystop) ;
	
	
	RETURN 0;
	
END ;
$$ LANGUAGE plpgsql ;




/* ------------------------------
	P_PURGEWAITINGORDERS_ACT
*/ ------------------------------
CREATE FUNCTION p_purgewaitingorders_act(
	calledby  VARCHAR(8), 
	ftype VARCHAR(16), 
	ckstart TIMESTAMP(6), 
	ckstop TIMESTAMP(6)) RETURNS INTEGER AS $$
	
BEGIN

	IF (calledby = 'PURGEWA') THEN
		
		-- prepare a list of orders no waiting any longer due the presence of the new ineventory ftype
  		INSERT INTO waitordtorembypurge 
			(SELECT DISTINCT wo.orderid
				FROM t_waitingorders wo
					WHERE wo.triggertype = 'FILETYPEMATCH' 
							AND wo.filetype = ftype) ;

		-- add to the previous list the orders no waiting any longer due the covering by the new inventoy
  		INSERT INTO waitordtorembypurge 
			(SELECT DISTINCT wo.orderid
				FROM t_waitingorders wo
					WHERE ( wo.triggertype = 'INTERSECT' OR wo.triggertype = 'COVER') 
							AND wo.filetype = ftype 
							AND ckstart <= wo.starttime 
							AND ckstop >= wo.stoptime) ;

		-- and now the intersections
  		INSERT INTO waitordtorembypurge 
			(SELECT DISTINCT wo.orderid
				FROM t_waitingorders wo
					WHERE wo.triggertype = 'INTERSECT' 
						AND wo.filetype = ftype 
						AND ( NOT ( ckstart >= wo.stoptime OR wo.starttime >= ckstop))) ;
	END IF;
	
	IF (calledby = 'POSTINV') THEN
		
		-- prepare a list of orders no waiting any longer due the presence of the new ineventory ftype
  		INSERT INTO waitordtorembypost 
			(SELECT DISTINCT wo.orderid
				FROM t_waitingorders wo
					WHERE wo.triggertype = 'FILETYPEMATCH' 
							AND wo.filetype = ftype) ;

		-- add to the previous list the orders no waiting any longer due the covering by the new inventoy
  		INSERT INTO waitordtorembypost 
			(SELECT DISTINCT wo.orderid
				FROM t_waitingorders wo
					WHERE ( wo.triggertype = 'INTERSECT' OR wo.triggertype = 'COVER') 
							AND wo.filetype = ftype 
							AND ckstart <= wo.starttime 
							AND ckstop >= wo.stoptime) ;

		-- and now the intersections
  		INSERT INTO waitordtorembypost 
			(SELECT DISTINCT wo.orderid
				FROM t_waitingorders wo
					WHERE wo.triggertype = 'INTERSECT' 
						AND wo.filetype = ftype 
						AND ( NOT ( ckstart >= wo.stoptime OR wo.starttime >= ckstop))) ;
	END IF;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;




/* ------------------------------
	P_PURGEWAITINGORDERS_FIX
*/ ------------------------------
CREATE FUNCTION p_purgewaitingorders_fix(calledby VARCHAR(8)) RETURNS INTEGER AS $$
	
DECLARE
  	tmpOrdId VARCHAR(16);
	evn INTEGER;
	row RECORD ;
	
BEGIN
	
	IF (calledby = 'PURGEWA') THEN
		-- delete records from table
		DELETE FROM t_waitingorders WHERE orderid IN (SELECT DISTINCT orderid FROM waitordtorembypurge) ; 

		FOR row IN (SELECT DISTINCT orderid::VARCHAR(16) FROM waitordtorembypurge) LOOP
			tmpOrdId := row.orderid ;
			evn := p_inserteventtableitem(tmpOrdId, 'ORDNOWAI');
		END LOOP ;
		
	END IF;
	
	IF (calledby = 'POSTINV') THEN
		-- delete records from table
		DELETE FROM t_waitingorders WHERE orderid IN (SELECT DISTINCT orderid FROM waitordtorembypost) ; 

		FOR row IN (SELECT DISTINCT orderid::VARCHAR(16) FROM waitordtorembypost) LOOP
			tmpOrdId := row.orderid ;
			evn := p_inserteventtableitem(tmpOrdId, 'ORDNOWAI');
		END LOOP ;
	END IF;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;




/* ------------------------------
	P_POSTINVQUEUE_MANAGER
*/ ------------------------------
CREATE FUNCTION p_postinvqueue_manager(
	in_secs INTEGER) RETURNS INTEGER AS $$

DECLARE
	p_secs INTERVAL;
	p_date TIMESTAMP(6);
	acall  INTEGER;
	fcall  INTEGER;
	row    RECORD;

BEGIN
	p_secs := (SELECT in_secs * '1 second'::interval);
	p_date := (SELECT now() - p_secs) ;
	
	CREATE TEMP TABLE temppostinvqueue (
		ftype VARCHAR(16), 
		valstart TIMESTAMP(6), 
		valstop TIMESTAMP(6)) ;
	
	INSERT INTO temppostinvqueue(
		SELECT filetype, validitystart, validitystop
			FROM t_postinvqueue
				WHERE tstamp < p_date);
	
	IF ((SELECT count(*) FROM temppostinvqueue) > 0) THEN
		CREATE TEMP TABLE waitordtorembypost (orderid INT) ;
		
		FOR row IN (SELECT * FROM temppostinvqueue) LOOP
			acall := p_purgewaitingorders_act('POSTINV', row.ftype, row.valstart, row.valstop) ;
		END LOOP ;

		fcall := p_purgewaitingorders_fix('POSTINV');
				
		DROP TABLE waitordtorembypost ;
		DELETE FROM t_postinvqueue WHERE tstamp < p_date;
	END IF;
	
	DROP TABLE temppostinvqueue;
	
	RETURN 0;
	
END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
---------------------------- PROCEDURES MODIFICATION ----------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	P_PURGEWAITINGORDERS
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_purgewaitingorders(
	ftype VARCHAR(16), 
	ckstart TIMESTAMP(6), 
	ckstop TIMESTAMP(6)) RETURNS INTEGER AS $$

DECLARE
	acall  INTEGER;
	fcall  INTEGER;

BEGIN
	
	CREATE TEMP TABLE waitordtorembypurge (orderid INT) ;
	
	acall := p_purgewaitingorders_act('PURGEWA', ftype, ckstart, ckstop);
	fcall := p_purgewaitingorders_fix('PURGEWA');
	
	DROP TABLE waitordtorembypurge ;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
-------------------------- TRIGGER FUNCTIONS CREATION ---------------------------
---------------------------------------------------------------------------------

/***********************************************************************/
/* Function callable from triggers tr_pendinginvinsert                 */
/* to wrap up the p_postinvqueue_insert fucntion.                      */
/***********************************************************************/
CREATE FUNCTION tf_pendinginvinsert() RETURNS TRIGGER AS $$
DECLARE
  	status INTEGER ;
BEGIN
	status := p_postinvqueue_insert(NEW.filename, NEW.fileversion) ;
  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
------------------------------- TRIGGERS CREATION -------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_pendinginventory
*/ ------------------------------
CREATE TRIGGER tr_pendinginvinsert AFTER INSERT
  ON t_pendinginventory 
  	FOR EACH ROW
  		EXECUTE PROCEDURE tf_pendinginvinsert() ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------
INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-9.0.0',
		'2009-09-18',
		3,
		1,
		'SchemaVersion_9-0-0',
		'Added table t_postinvqueue, procedure p_postinvqueue_insert, p_purgewaitingorders_act, p_purgewaitingorders_fix, p_postinvqueue_manager, trigger function tf_pendinginvinsert and trigger tr_pendinginvinsert, view v_satrelations and index ix_invsm_storagemanid. Modified procedure p_purgewaitingorders.'
) ;
