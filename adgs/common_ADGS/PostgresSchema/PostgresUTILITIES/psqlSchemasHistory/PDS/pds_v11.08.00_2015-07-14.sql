
-- Version 11.8.0
-- POSTGRES








--------------------------------------------------------------------------------
-------------------------------- TABLES CREATION -------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_ordersqueueblacklist
*/ ------------------------------
CREATE TABLE t_ordersqueueblacklist (
    id SERIAL NOT NULL,
    orderid INTEGER NOT NULL,
	jobresponsible VARCHAR(64) NOT NULL
) ;

ALTER TABLE t_ordersqueueblacklist
	ADD CONSTRAINT pk_t_ordersqueueblacklist
		PRIMARY KEY (id) ;

ALTER TABLE t_ordersqueueblacklist 
	ADD CONSTRAINT uq_ordersqueueblacklist01
		UNIQUE (orderid, jobresponsible) ;

ALTER TABLE t_ordersqueueblacklist
	ADD CONSTRAINT fk_t_ordersqueueblacklist1
		FOREIGN KEY (orderid)
			REFERENCES t_ordersqueue (orderid)
				ON DELETE CASCADE ;

CREATE INDEX ix_t_ordersqueueblacklist01 ON t_ordersqueueblacklist USING BTREE (orderid) ;
CREATE INDEX ix_t_ordersqueueblacklist02 ON t_ordersqueueblacklist USING BTREE (jobresponsible) ;








--------------------------------------------------------------------------------
--------------------------------- TABLES INSERT --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_stati
*/ ------------------------------
INSERT INTO t_stati (id, status) VALUES (39, 'PARTIALLY_TRANSFERRED') ;








--------------------------------------------------------------------------------
------------------------------ PROCEDURES CHANGES ------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	p_inventory_manager
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_inventory_manager(
	fname VARCHAR(128), 
	fversion VARCHAR(32),
	prevalflag BOOLEAN,
	postvalflag BOOLEAN,
	ftype VARCHAR(16),
	fclass VARCHAR(8),
	orderid INTEGER,
	in_satellite SMALLINT, 
	in_sensor SMALLINT, 
	in_mission SMALLINT,
	valstart TIMESTAMP(6),
	valstop TIMESTAMP(6)) RETURNS INTEGER AS $$
	
DECLARE
  evnt VARCHAR(3) ;
  evnttype VARCHAR(8) ;
  
BEGIN
	-- Check for satellite-sensor-mission relation
  	PERFORM p_checksatsensmiss(in_satellite, in_sensor, in_mission) ;
	
  	-- If is called after an insert
  	IF prevalflag IS NULL THEN
		evnt := 'INS' ;
		evnttype := 'INSFTYPE' ;
		INSERT INTO t_pendinginventory (filename, fileversion) VALUES (fname, fversion) ;
	-- If is called after an update
	ELSE
		evnt := 'UPD' ;
		evnttype := 'UPDFTYPE' ;
	END IF ;
	
	-- If validity flag is, or became, TRUE
  	IF ((prevalflag IS NULL AND postvalflag = 't') OR (prevalflag <> postvalflag AND postvalflag = 't')) THEN
  	  	PERFORM p_processinventory(fname, fversion, postvalflag, ftype, fclass, orderid, in_satellite, valstart, valstop) ;
		UPDATE t_filetypes_last_inv SET last_valid = NOW() WHERE filetype = ftype ;
  	END IF ;

	----------------------------------------------------------------------------
	-- BROADCAST SECTION
	----------------------------------------------------------------------------
  	PERFORM p_events('t_inventory', evnt) ;
	PERFORM p_events(ftype, evnttype) ;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;




/* ------------------------------
	P_PURGEWAITINGORDERS
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_purgewaitingorders(
	ftype VARCHAR(16), 
	ckstart TIMESTAMP(6), 
	ckstop TIMESTAMP(6)) RETURNS INTEGER AS $$

DECLARE
	row RECORD ;

BEGIN
	FOR row IN (
		SELECT DISTINCT orderid
		FROM t_waitingorders WHERE 
			-- orders no waiting any longer due the presence of the new ineventory ftype
			(triggertype = 'FILETYPEMATCH' AND filetype = ftype)
			-- orders no waiting any longer due the covering by the new inventoy
			OR ((triggertype = 'INTERSECT' OR triggertype = 'COVER') AND filetype = ftype AND ckstart <= starttime AND ckstop >= stoptime)
			-- and the intersections
			OR (triggertype = 'INTERSECT' AND filetype = ftype AND ( NOT ( ckstart >= stoptime OR starttime >= ckstop))) 
	) LOOP
		
		-- delete records from table
		DELETE FROM t_waitingorders WHERE orderid = row.orderid ;
		-- write event
		PERFORM p_events(row.orderid::VARCHAR, 'ORDNOWAI');	
		
	END LOOP ;
	
	RETURN 0;
END ;
$$ LANGUAGE plpgsql ;




/* ------------------------------
	P_POSTINVQUEUE_MANAGER
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_postinvqueue_manager(
	in_secs INTEGER) RETURNS INTEGER AS $$

DECLARE
	p_secs INTERVAL;
	p_date TIMESTAMP(6);

BEGIN
	p_secs := (SELECT in_secs * '1 second'::interval);
	p_date := (SELECT now() - p_secs) ;
	
	PERFORM p_purgewaitingorders(
		pq.filetype,
		pq.validitystart,
		pq.validitystop)
	FROM 
		t_postinvqueue pq
	WHERE 
		pq.tstamp < p_date ;
	
	DELETE FROM t_postinvqueue WHERE tstamp < p_date;
		
	RETURN 0;
	
END ;
$$ LANGUAGE plpgsql ;








--------------------------------------------------------------------------------
--------------------------- TRIGGER FUNCTION CREATION --------------------------
--------------------------------------------------------------------------------

/* --------------------------
	tf_inventory_ins_bfr
*/ --------------------------
CREATE OR REPLACE FUNCTION tf_inventory_ins_bfr() RETURNS TRIGGER AS $$

BEGIN
	IF NEW.unique_id IS NULL  THEN
		NEW.unique_id := NEW.filename || NEW.fileversion ;
	END IF ;
	
	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;








--------------------------------------------------------------------------------
--------------------------- TRIGGER FUNCTION CHANGES ---------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	tf_inventoryinsevent
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_inventoryinsevent() RETURNS TRIGGER AS $$

BEGIN
	PERFORM p_inventory_manager(NEW.filename, NEW.fileversion, NULL, NEW.validityflag, NEW.filetype, NEW.fileclass, NEW.orderid, NEW.satelliteid, NEW.sensorid, NEW.mission, NEW.validitystart, NEW.validitystop) ;
	PERFORM p_insert_inventoryevents(NEW.id_inv, NEW.filename, NEW.fileversion, NEW.filetype, NULL, NEW.validityflag, 'I') ;
	
	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;


/* ------------------------------
	tf_inventoryupdevent
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_inventoryupdevent() RETURNS TRIGGER AS $$

BEGIN
	PERFORM p_inventory_manager(NEW.filename, NEW.fileversion, OLD.validityflag, NEW.validityflag, NEW.filetype, NEW.fileclass, NEW.orderid, NEW.satelliteid, NEW.sensorid, NEW.mission, NEW.validitystart, NEW.validitystop) ;
	PERFORM p_insert_inventoryevents(NEW.id_inv, NULL, NULL, NEW.filetype, OLD.validityflag, NEW.validityflag, 'U') ;
	
	IF ((NEW.validityflag != OLD.validityflag) AND (NEW.validityflag = 't')) THEN
		PERFORM p_purgewaitingorders(NEW.filetype, NEW.validitystart, NEW.validitystop) ;
	END IF;
	
	IF OLD.qualityinfo != NEW.qualityinfo THEN
		PERFORM p_purgewaitingorders(NEW.filetype, NEW.validitystart, NEW.validitystop) ;
	END IF ;
	
	IF (OLD.qualityinfo = 'DUP' AND (NEW.qualityinfo = '' OR NEW.qualityinfo IS NULL) AND NEW.validityflag = 't') THEN
		PERFORM p_generatetmporder('t', OLD.filename, OLD.fileversion, OLD.filetype, OLD.fileclass, OLD.satelliteid) ;
	END IF ;
	
	IF OLD.qualified != NEW.qualified THEN
		PERFORM p_purgewaitingorders(NEW.filetype, NEW.validitystart, NEW.validitystop) ;
	END IF ;
	
  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;


/* ------------------------------
	tf_inventorydelete
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_inventorydelete() RETURNS TRIGGER AS $$

BEGIN
	PERFORM p_insert_inventoryevents(NULL, OLD.filename, OLD.fileversion, OLD.filetype, 'f', 'f', 'D') ;
	
  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;








--------------------------------------------------------------------------------
-------------------------------- TRIGGER CHANGES -------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_inventory
*/ ------------------------------
DROP TRIGGER IF EXISTS tr_inventoryinsert01 ON t_inventory ;
CREATE TRIGGER tr_inventory_ins_bfr BEFORE INSERT
	ON t_inventory
		FOR EACH ROW EXECUTE PROCEDURE tf_inventory_ins_bfr() ;

DROP TRIGGER IF EXISTS tr_inventoryinsert ON t_inventory ;
CREATE TRIGGER tr_inventory_ins_aft AFTER INSERT
	ON t_inventory
		FOR EACH ROW EXECUTE PROCEDURE tf_inventoryinsevent() ;

DROP TRIGGER IF EXISTS tr_inventoryupdate ON t_inventory ;
CREATE TRIGGER tr_inventory_upd_aft AFTER UPDATE
	ON t_inventory
		FOR EACH ROW EXECUTE PROCEDURE tf_inventoryupdevent() ;

DROP TRIGGER IF EXISTS tr_inventorydelete ON t_inventory ;
CREATE TRIGGER tr_inventory_del_aft AFTER DELETE
	ON t_inventory
		FOR EACH ROW EXECUTE PROCEDURE tf_inventorydelete() ;








--------------------------------------------------------------------------------
--------------------------------- OBJECTS DROP ---------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	tr_inventory_quality_update
*/ ------------------------------
DROP TRIGGER IF EXISTS tr_inventory_quality_update ON t_inventory ;

/* --------------------------
	tf_inventory_uniqueid
*/ --------------------------
DROP FUNCTION IF EXISTS tf_inventory_uniqueid() ;

/* ------------------------------
	tf_inventory_quality_update
*/ ------------------------------
DROP FUNCTION tf_inventory_quality_update() ;

/* ------------------------------
	p_filetypes_last_inv_update
*/ ------------------------------
DROP FUNCTION IF EXISTS p_filetypes_last_inv_update(VARCHAR) ;

/* ------------------------------
	P_PURGEWAITINGORDERS_ACT
*/ ------------------------------
DROP FUNCTION IF EXISTS p_purgewaitingorders_act(VARCHAR, VARCHAR, TIMESTAMP, TIMESTAMP) ;

/* ------------------------------
	P_PURGEWAITINGORDERS_FIX
*/ ------------------------------
DROP FUNCTION IF EXISTS p_purgewaitingorders_fix(VARCHAR) ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'pds-11.8.0' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-11.8.0',
		'2015-07-14',
		3,
		2,
		'PDS_11-8-0',
		'Added table t_ordersqueueblacklist. Added p_purgewaitingorders calling on quality change in t_inventory. Changed procedures p_inventory_manager, p_purgewaitingorders and p_postinvqueue_manager. Optimized inventory triggers functions. Removed tr_inventory_quality_update, tf_inventory_uniqueid, tf_inventory_quality_update, p_filetypes_last_inv_update, p_purgewaitingorders_act and p_purgewaitingorders_fix.'
) ;


