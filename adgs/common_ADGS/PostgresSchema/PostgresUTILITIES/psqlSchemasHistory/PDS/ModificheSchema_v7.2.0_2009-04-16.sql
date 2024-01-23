
-- Version 7.2.0
-- POSTGRES








---------------------------------------------------------------------------------
------------------------------- TABLES ALTERATION -------------------------------
---------------------------------------------------------------------------------

-------------------------
--- T_TEMPORARYORDERS ---
-------------------------
ALTER TABLE t_temporaryorders ADD tstamp TIMESTAMP(6) DEFAULT NOW() NOT NULL;

-----------------------
--- T_QUALIFIEDFLAG ---
-----------------------
INSERT INTO t_qualifiedflag (name) VALUES ('QUALIFIED_PRODUCT') ;
INSERT INTO t_qualifiedflag (name) VALUES ('UNQUALIFIED_PRODUCT') ;

------------------------------
--- T_DISTRIBUTIONPOLICIES ---
------------------------------
ALTER TABLE t_distributionpolicies DROP CONSTRAINT uq_distributionpolicies;
ALTER TABLE "t_distributionpolicies"
  ADD CONSTRAINT uq_distributionpolicies
  	UNIQUE ("filetype", "mediatype", "userid", "ordertype") ;

-------------------
--- T_SMACTIONS ---
-------------------
ALTER TABLE t_smactions ALTER COLUMN enddate SET DEFAULT '3000-01-01 00:00:00.000000';








---------------------------------------------------------------------------------
------------------------------ PROCEDURES CREATION ------------------------------
---------------------------------------------------------------------------------

-----------------------------
--- P_CARTS_UPDATE_STATUS ---
-----------------------------
CREATE FUNCTION p_carts_update_status(in_cartid INT) RETURNS INTEGER AS $$

DECLARE
	p_extorderid VARCHAR(32);
	ord_total INT;
	dis_total INT;
	mid_status VARCHAR(32) ;

BEGIN
	
	p_extorderid := (SELECT extref FROM t_cartsref WHERE id = in_cartid ) ;
	
	ord_total := (SELECT count(*) FROM t_ordersqueue WHERE externalorderid = p_extorderid AND subreq IS NOT NULL);
	dis_total := (SELECT count(*) FROM t_distributionitems WHERE cartid = in_cartid);


	-- ORDERS ERROR
	IF (SELECT count(status) FROM t_ordersqueue WHERE externalorderid = p_extorderid AND subreq IS NOT NULL AND status = 'ERROR') > 0 
	THEN
		UPDATE t_cartsref SET status = 'ERROR' WHERE id = in_cartid ;
		RETURN 0;
	END IF;

	-- ORDERS ELIGIBLE
	IF (SELECT count(status) FROM t_ordersqueue WHERE externalorderid = p_extorderid AND subreq IS NOT NULL AND status = 'ELIGIBLE') < ord_total 
	THEN
		-- ORDERS COMPLETED
		IF (SELECT count(status) FROM t_ordersqueue WHERE externalorderid = p_extorderid AND status = 'COMPLETED') < ord_total 
		THEN
			UPDATE t_cartsref SET status = 'PROGRESS' WHERE id = in_cartid ;
			RETURN 0;
		ELSE	
			mid_status := 'COMPLETED' ;
		END IF ;
	ELSE 
		mid_status := 'ELIGIBLE' ;
	END IF ;



	-- DISTRIBUTION ERROR
	IF (SELECT count(di.id) FROM t_distributionitems AS di
		INNER JOIN t_distributionqueue AS dq ON di.packageid = dq.packageid
		WHERE di.cartid = in_cartid AND dq.status = 'ERROR') > 0 
	THEN
		UPDATE t_cartsref SET status = 'ERROR' WHERE id = in_cartid ;
		RETURN 0;
	END IF;

	-- DISTRIBUTION ELIGIBLE
	IF (SELECT count(di.id) FROM t_distributionitems AS di
		INNER JOIN t_distributionqueue AS dq ON di.packageid = dq.packageid
		WHERE di.cartid = in_cartid AND dq.status = 'ELIGIBLE') < dis_total 
	THEN
		-- DISTRIBUTION COMPLETED
		IF (SELECT count(di.id) FROM t_distributionitems AS di
			INNER JOIN t_distributionqueue AS dq ON di.packageid = dq.packageid
			WHERE di.cartid = in_cartid AND dq.status = 'COMPLETED') < dis_total
		THEN
			UPDATE t_cartsref SET status = 'PROGRESS' WHERE id = in_cartid ;
			RETURN 0;
		ELSE
			UPDATE t_cartsref SET status = mid_status WHERE id = in_cartid ;
			RETURN 0;
		END IF;
	ELSE 
		UPDATE t_cartsref SET status = 'ELIGIBLE' WHERE id = in_cartid ;
		RETURN 0;
	END IF ;

	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
---------------------------- PROCEDURES MODIFICATION ----------------------------
---------------------------------------------------------------------------------

-------------------------
--- P_DISTRIBUTE_ITEM ---
-------------------------
CREATE OR REPLACE FUNCTION p_distribute_item(
	in_fname VARCHAR(128),
	in_fversion VARCHAR(8),
	in_filetype VARCHAR(16),
	in_userid VARCHAR(32),
	in_ordertype VARCHAR(8),
	in_format VARCHAR(8),
	in_orderid INTEGER) RETURNS INTEGER AS $$
	
DECLARE
	newdate    TIMESTAMP(6);
	tmpdate    DATE ;
	tmptime    TIME(6) ;
	row        RECORD ;
	p_extordid VARCHAR(32);
	p_cartid   INT;
	p_subreq   INT;
	
BEGIN

	p_extordid := (SELECT externalorderid FROM t_ordersqueue WHERE orderid = in_orderid);
	p_subreq := (SELECT subreq FROM t_ordersqueue WHERE orderid = in_orderid);
	
	IF p_extordid IS NOT NULL AND p_subreq IS NOT NULL 
	THEN
		p_cartid := 	(SELECT id FROM t_cartsref AS cr
				INNER JOIN t_ordersqueue AS oq
				ON oq.externalorderid = cr.extref
				WHERE oq.orderid = in_orderid);

		p_subreq := (SELECT oq.subreq + cr.distreqs 
				FROM t_ordersqueue AS oq
				INNER JOIN t_cartsref AS cr
				ON oq.externalorderid = cr.extref
				WHERE oq.orderid = in_orderid);
	ELSE
		p_cartid := NULL;
		p_subreq := NULL;
	END IF;


--	the input inventory file is distributed to each user who declared an interest on that file-type.
--	the distribution is done according to the active field in the distribution policy record
	IF in_format IS NULL THEN
		IF in_ordertype IS NULL THEN
			FOR row IN (SELECT DISTINCT dp.id, dp.mediatype, dp.deliverfrequency, dp.format, dp.algo
			  	  	  	  FROM t_distributionpolicies dp
						  	WHERE dp.userid=in_userid AND
							  	  dp.filetype=in_filetype AND
								  dp.active='t' AND
								  dp.ordertype IS NULL) LOOP

					tmpdate := NOW() ;
					tmptime := NOW() ;
					newdate := (tmpdate + row.deliverfrequency) + tmptime ;

					INSERT INTO t_temporarydistribution 
					  ("duedate", "filename", "fileversion", "mediatype", "userid", "format", "algo", "policy", "orderid", "cartid", "subreq")
					VALUES
					  (newdate, in_fname, in_fversion, row.mediatype, in_userid, row.format, row.algo, row.id, in_orderid, p_cartid, p_subreq) ;
			END LOOP ;
		ELSE
			-- No format but order type
			FOR row IN (SELECT DISTINCT dp.id, dp.mediatype, dp.deliverfrequency, dp.format, dp.algo
				  	  	  FROM t_distributionpolicies dp
				  	  	  	WHERE dp.userid=in_userid AND
				  	  	  	  	  dp.filetype=in_filetype AND
					  	  	  	  dp.active='t' AND
					  	  	  	  (dp.ordertype=in_ordertype OR dp.ordertype IS NULL)) LOOP

					tmpdate := NOW() ;
					tmptime := NOW() ;
					newdate := (tmpdate + row.deliverfrequency) + tmptime ;

					INSERT INTO t_temporarydistribution
					  ("duedate", "filename", "fileversion", "mediatype", "userid", "format", "algo", "policy", "orderid", "cartid", "subreq")
					VALUES 
					  (newdate, in_fname, in_fversion, row.mediatype, in_userid, row.format, row.algo, row.id, in_orderid, p_cartid, p_subreq) ;
			END LOOP ;
		END IF ;
	ELSE
		IF in_ordertype IS NULL THEN

			-- Format defined in input
			FOR row IN (SELECT DISTINCT dp.id, dp.mediatype, dp.deliverfrequency, dp.algo
  	  	  	  	  	  	  FROM t_distributionpolicies dp
  	  	  	  	  	  	  	WHERE dp.userid=in_userid AND
							  	  dp.filetype=in_filetype AND
								  dp.active='t' AND
								  dp.format=in_format AND
								  dp.ordertype IS NULL) LOOP

					tmpdate := NOW() ;
					tmptime := NOW() ;
					newdate := (tmpdate + row.deliverfrequency) + tmptime ;

					INSERT INTO t_temporarydistribution
					  ("duedate", "filename", "fileversion", "mediatype", "userid", "format", "algo", "policy", "orderid", "cartid", "subreq")
						VALUES
  	  	  	  	  	  (newdate, in_fname, in_fversion, row.mediatype, in_userid, in_format, row.algo, row.id, in_orderid, p_cartid, p_subreq) ;
			END LOOP ;
		ELSE
			-- Format and order type defined in input
			FOR row IN (SELECT DISTINCT dp.id, dp.mediatype, dp.deliverfrequency, dp.algo
				  	  	  FROM t_distributionpolicies dp
				  	  	  	WHERE dp.userid=in_userid AND
							  	  dp.filetype=in_filetype AND
								  dp.active='t' AND
								  dp.format=in_format AND
								  (dp.ordertype=in_ordertype OR dp.ordertype IS NULL)) LOOP

					tmpdate := NOW() ;
					tmptime := NOW() ;
					newdate := (tmpdate + row.deliverfrequency) + tmptime ;

					INSERT INTO t_temporarydistribution
					  ("duedate", "filename", "fileversion", "mediatype", "userid", "format", "algo", "policy", "orderid", "cartid", "subreq")
					VALUES
					  (newdate, in_fname, in_fversion, row.mediatype, in_userid, in_format, row.algo, row.id, in_orderid, p_cartid, p_subreq) ;
			END LOOP ;
		END IF ;
	END IF ;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;



------------------------------
--- P_ORDERSTATETRANSITION ---
------------------------------
CREATE OR REPLACE FUNCTION p_orderstatetransition(
	in_orderid INTEGER, 
	pre_status VARCHAR(32), 
	post_status VARCHAR(32)) RETURNS INTEGER AS $$
	
DECLARE
  	cnt INTEGER ;
  	rec RECORD ;
  	err INTEGER ;
	evn INTEGER ;
	p_extordid VARCHAR(32);
	p_subreq INT;
	p_cartid INT;
	
BEGIN

	IF post_status IS NULL THEN
		RETURN 0;
	END IF ;

	IF pre_status IS NULL THEN
		IF post_status = 'ELIGIBLE' THEN
			INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
			evn := p_inserteventtableitem('NUL2ELI', 't_orderstatetransitions') ;
			RETURN 0 ;
		ELSIF post_status = 'STANDBY' THEN
			INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
			evn := p_inserteventtableitem('NUL2STA', 't_orderstatetransitions') ;
			RETURN 0 ;
		ELSE
			RAISE EXCEPTION 'Invalid Order State Transition' ;
		END IF ;
	END IF ;

-- state control
	IF post_status = 'ELIGIBLE' THEN
		IF pre_status = 'ERROR' THEN
			INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
			evn := p_inserteventtableitem('ERR2ELI', 't_orderstatetransitions') ;	
		ELSIF pre_status = 'RECOVERABLEERROR' THEN
			INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
			evn := p_inserteventtableitem('REC2ELI', 't_orderstatetransitions') ;
		ELSIF pre_status = 'STANDBY' THEN
			INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
			evn := p_inserteventtableitem('STA2ELI', 't_orderstatetransitions') ;
		ELSIF pre_status = 'ELIGIBLE' THEN
-- The following transition is logged only the first time to avoid too much negligible transitions
-- ELIGIBLE->PRECONDCHEDCK->ELIGIBLE...
			SELECT INTO rec COUNT(*) FROM t_orderstatetransitions WHERE "orderid" = in_orderid ;
			cnt := rec.count;
			IF cnt = 0 THEN -- the first time
				-- insert item in state transition table
				INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
			END IF ;
			RETURN 0 ;	
		ELSIF pre_status = 'PRECONDITIONCHECK' THEN 
			RETURN 0 ;
		ELSE 
			RAISE EXCEPTION 'Invalid Order State Transition' ;
		END IF ;
	ELSIF post_status = 'PRECONDITIONCHECK' THEN
		IF pre_status = 'ELIGIBLE' THEN
			RETURN 0 ;
-- The following transition is not logged to avoid too much negligible transitions ELIGIBLE->PRECONDCHEDCK->ELIGIBLE...
--		INSERT INTO t_orderstatetransitions(orderid ,status) values ( in_orderid,post_status);
		ELSE 
			RAISE EXCEPTION 'Invalid Order State Transition' ;
		END IF ;

	ELSIF post_status = 'DOWNLOADING' THEN
		IF pre_status = 'PRECONDITIONCHECK' THEN
			INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
			evn := p_inserteventtableitem('PRE2DOW', 't_orderstatetransitions') ;
		ELSE 
			RAISE EXCEPTION 'Invalid Order State Transition' ;
		END IF ;
	ELSIF post_status = 'PROCESSING' THEN
		IF pre_status = 'DOWNLOADING' THEN
			INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
			evn := p_inserteventtableitem('DOW2PRO', 't_orderstatetransitions') ;
		ELSE 
			RAISE EXCEPTION 'Invalid Order State Transition' ;
		END IF ;
	ELSIF post_status = 'ERROR' THEN
		IF pre_status = 'DOWNLOADING' THEN
			INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
			evn := p_inserteventtableitem('DOW2ERR', 't_orderstatetransitions') ;	
		ELSIF pre_status = 'STANDBY' THEN
			INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
			evn := p_inserteventtableitem('STA2ERR', 't_orderstatetransitions') ;	
		ELSIF pre_status = 'PROCESSING' THEN
			INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
			evn := p_inserteventtableitem('PRO2ERR', 't_orderstatetransitions') ;
		ELSE 
			RAISE EXCEPTION 'Invalid Order State Transition' ;
		END IF ;
	ELSIF post_status = 'RECOVERABLEERROR' THEN
		IF pre_status = 'PROCESSING' THEN
			INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
			evn := p_inserteventtableitem('PRO2REC', 't_orderstatetransitions') ;
		ELSE 
			RAISE EXCEPTION 'Invalid Order State Transition' ;
		END IF ;
	ELSIF post_status = 'COMPLETED' THEN
		IF pre_status = 'PROCESSING'  THEN
			INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
			evn := p_inserteventtableitem('PRO2COM', 't_orderstatetransitions') ;
			err := p_completed_order_distribution(in_orderid) ;
		ELSIF pre_status = 'ERROR' THEN
			INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
			evn := p_inserteventtableitem('ERR2COM', 't_orderstatetransitions') ;
			err := p_completed_order_distribution(in_orderid) ;
		ELSE 
			RAISE EXCEPTION 'Invalid Order State Transition' ;
		END IF ;
	ELSE
		RAISE EXCEPTION 'Invalid Order State Transition' ;
	END IF ;
	
	p_extordid := (SELECT externalorderid FROM t_ordersqueue WHERE orderid = in_orderid);
	p_subreq := (SELECT subreq FROM t_ordersqueue WHERE orderid = in_orderid);
	
	IF p_extordid IS NOT NULL AND p_subreq IS NOT NULL THEN
		p_cartid := (SELECT id FROM t_cartsref AS cr
					INNER JOIN t_ordersqueue AS oq
					ON oq.externalorderid = cr.extref
					WHERE oq.orderid = in_orderid);
		
		err := p_carts_update_status(p_cartid);
	END IF ;

	RETURN 0 ;

END ;
$$ LANGUAGE plpgsql ;




---------------------------
--- P_DISTRIBUTIONSTATE ---
---------------------------
CREATE OR REPLACE FUNCTION p_distributionstate(
	in_packageid INT,
	pre_status VARCHAR(32), 
	post_status VARCHAR(32)) RETURNS INTEGER AS $$

DECLARE
	p_cartid INT;
	p_subreq INT;
	err INT;

BEGIN
	-- SET DEBUG FILE TO '/tmp/test.trace';
	-- TRACE ON;

	-- state transition :
	-- TRANSIENT
	-- ELIGIBLE -> ELIGIBLE <- ERROR
	-- DOWNLOADING -> ERROR
	-- DISTRIBUTION -> ERROR
	-- COMPLETED
	-------------------------
	-- ERROR

  	IF post_status IS NULL THEN
  	  	RETURN 0 ;
  	END IF ;

  	IF pre_status = post_status THEN
  	  	IF post_status = 'TRANSIENT' OR pre_status = 'ELIGIBLE' OR pre_status = 'DOWNLOADING' THEN
  	  	  	RETURN 0 ;
  	  	ELSE
  	  	  	RAISE EXCEPTION 'p_distributionstate: Invalid Distribution State Transition' ;
  	  	END IF ;
  	END IF ;

-- state control
  	IF post_status = 'ELIGIBLE' THEN
  	  	IF pre_status = 'TRANSIENT' OR pre_status = 'ERROR'  THEN
  	  	  	RETURN 0 ;
  	  	ELSE
  	  	  	RAISE EXCEPTION 'p_distributionstate: Invalid Distribution State Transition' ;
  	  	END IF ;
	
  	ELSIF post_status = 'DOWNLOADING' THEN
  	  	IF pre_status = 'ELIGIBLE' THEN
  	  	  	RETURN 0 ;
  	  	ELSE 
  	  	  	RAISE EXCEPTION 'p_distributionstate: Invalid Distribution State Transition' ;
  	  	END IF ;

	ELSIF post_status = 'DISTRIBUTION' THEN
  	  	IF pre_status = 'DOWNLOADING' THEN
  	  	  	RETURN 0 ;
  	  	ELSE 
  	  	  	RAISE EXCEPTION 'p_distributionstate: Invalid Distribution State Transition' ;
  	  	END IF ;

	ELSIF post_status = 'COMPLETED' THEN
  	  	IF pre_status = 'DISTRIBUTION' THEN
  	  	  	RETURN 0 ;
  	  	ELSE 
  	  	  	RAISE EXCEPTION 'p_distributionstate: Invalid Distribution State Transition' ;
  	  	END IF ;

	ELSIF post_status = 'ERROR' THEN
  	  	IF pre_status = 'DOWNLOADING' OR pre_status = 'DISTRIBUTION' THEN
  	  	  	RETURN 0 ;
  	  	ELSE 
  	  	  	RAISE EXCEPTION 'p_distributionstate: Invalid Distribution State Transition' ;
  	  	END IF ;
	ELSE
  	  	 RAISE EXCEPTION 'p_distributionstate: Invalid Distribution State Transition' ;
	END IF ;
	
	p_cartid := (SELECT DISTINCT cartid FROM t_distributionitems WHERE packageid = in_packageid);
	p_subreq := (SELECT DISTINCT subreq FROM t_distributionitems WHERE packageid = in_packageid);
	
	IF p_cartid IS NOT NULL AND p_subreq IS NOT NULL THEN		
		err := p_carts_update_status(p_cartid);
	END IF ;

  	RETURN 0 ;
END ;
$$ LANGUAGE plpgsql ;




-----------------------------
--- P_CARTSTATETRANSITION ---
-----------------------------
DROP FUNCTION p_cartstatetransition(INTEGER,VARCHAR,VARCHAR);








---------------------------------------------------------------------------------
------------------------- TRIGGER FUNCTIONS MODIFICATION ------------------------
---------------------------------------------------------------------------------
CREATE OR REPLACE FUNCTION tf_ins_p_distributionstate() RETURNS TRIGGER AS $$
DECLARE
  	status INTEGER ;
BEGIN
  	status := p_distributionstate(NEW.packageid, NEW.status, NEW.status) ;
  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;


CREATE OR REPLACE FUNCTION tf_upd_p_distributionstate() RETURNS TRIGGER AS $$
DECLARE
  	status INTEGER ;
BEGIN
  	status := p_distributionstate(OLD.packageid, OLD.status, NEW.status) ;
  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------
INSERT INTO t_schema (
		"version", 
		"modificationdate",
		"modificationtype",
		"schematype",
		"cvstag",
		"description" )
VALUES	(
		'7.2.0',
		'2009-04-16',
		3,
		1,
		'SchemaVersion_7-2-0',
		'Altered table t_temporaryorders. Added procedure p_carts_update_status. Modified procedures p_distribute_item, p_orderstatetransition and p_distributionstate.'
) ;
