
-- Version 8.1.0
-- POSTGRES








---------------------------------------------------------------------------------
------------------------------- TABLES ALTERATION -------------------------------
---------------------------------------------------------------------------------

------------------
--- T_CARTSINV ---
------------------
ALTER TABLE t_cartsinv RENAME COLUMN subreqstart TO subreq ;
ALTER TABLE t_cartsinv RENAME COLUMN subreqstop TO distsubreq ;
ALTER TABLE t_cartsinv ALTER COLUMN distsubreq DROP NOT NULL;
ALTER TABLE t_cartsinv ADD ordsubreq INT ;
ALTER TABLE t_cartsinv ADD stndsubreq INT ;
ALTER TABLE t_cartsinv ADD status VARCHAR(32) DEFAULT 'ELIGIBLE' NOT NULL ;
ALTER TABLE t_cartsinv ADD handled BOOLEAN DEFAULT 'f' NOT NULL ;


------------------
--- T_CARTREF ---
------------------
ALTER TABLE t_cartsref ADD stndreqs INT DEFAULT 0 NOT NULL ;


------------------------------
--- T_STANDINGDISTRIBUTION ---
------------------------------
ALTER TABLE t_standingdistribution ADD subrequest INT DEFAULT 1 NOT NULL ;
ALTER TABLE t_standingdistribution ALTER COLUMN subrequest DROP DEFAULT ;
ALTER TABLE t_standingdistribution DROP CONSTRAINT uq_stendingdistribution01;
ALTER TABLE t_standingdistribution 
	ADD CONSTRAINT uq_stendingdistribution01
		UNIQUE (distpoliciesid, validitystart, validitystop, cartsid, subrequest) ;








---------------------------------------------------------------------------------
------------------------------ PROCEDURES CREATION ------------------------------
---------------------------------------------------------------------------------

-------------------------
--- P_CARTSINV_UPDATE ---
-------------------------
CREATE FUNCTION p_cartsinv_update(
	in_packageid INTEGER, 
	in_status VARCHAR(32)
) RETURNS INTEGER AS $$

DECLARE
	p_cartid INTEGER;
	ret INTEGER;

BEGIN
	ret := 0 ;
	p_cartid := (SELECT DISTINCT cartid FROM t_distributionitems WHERE packageid = in_packageid);
	
	IF p_cartid IS NOT NULL THEN		
		UPDATE t_cartsinv SET status = in_status WHERE cart = p_cartid AND distsubreq IN (SELECT subreq FROM t_distributionitems WHERE packageid = in_packageid);
		ret := p_carts_update_status(p_cartid);
	END IF ;
	
	RETURN ret;
	
END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
---------------------------- PROCEDURES MODIFICATION ----------------------------
---------------------------------------------------------------------------------

---------------------------
--- P_DISTRIBUTIONSTATE ---
---------------------------
CREATE OR REPLACE FUNCTION p_distributionstate(
	in_packageid INT,
	pre_status VARCHAR(32), 
	post_status VARCHAR(32)) RETURNS INTEGER AS $$

DECLARE
	ret INT;

BEGIN
	
	ret := 0 ;
	
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
	-- ABORTED


  	IF post_status IS NULL THEN
  	  	RETURN 0 ;
  	END IF ;


  	IF pre_status = post_status THEN
  	  	IF post_status = 'TRANSIENT' OR pre_status = 'ELIGIBLE' OR pre_status = 'DOWNLOADING' THEN
  	  	  	ret := p_cartsinv_update(in_packageid, post_status);
			RETURN ret ;
  	  	ELSE
  	  	  	RAISE EXCEPTION 'p_distributionstate: Invalid Distribution State Transition' ;
  	  	END IF ;
  	END IF ;


-- state control
  	IF post_status = 'ELIGIBLE' THEN
  	  	IF pre_status = 'TRANSIENT' OR pre_status = 'ERROR'  THEN
  	  	  	ret := p_cartsinv_update(in_packageid, post_status);
			RETURN ret ;
  	  	ELSE
  	  	  	RAISE EXCEPTION 'p_distributionstate: Invalid Distribution State Transition' ;
  	  	END IF ;
	
  	ELSIF post_status = 'DOWNLOADING' THEN
  	  	IF pre_status = 'ELIGIBLE' THEN
  	  	  	ret := p_cartsinv_update(in_packageid, post_status);
			RETURN ret ;
  	  	ELSE 
  	  	  	RAISE EXCEPTION 'p_distributionstate: Invalid Distribution State Transition' ;
  	  	END IF ;

	ELSIF post_status = 'DISTRIBUTION' THEN
  	  	IF pre_status = 'DOWNLOADING' THEN
  	  	  	ret := p_cartsinv_update(in_packageid, post_status);
			RETURN ret ;
  	  	ELSE 
  	  	  	RAISE EXCEPTION 'p_distributionstate: Invalid Distribution State Transition' ;
  	  	END IF ;

	ELSIF post_status = 'COMPLETED' THEN
  	  	IF pre_status = 'DISTRIBUTION' THEN
  	  	  	ret := p_cartsinv_update(in_packageid, post_status);
			RETURN ret ;
  	  	ELSE 
  	  	  	RAISE EXCEPTION 'p_distributionstate: Invalid Distribution State Transition' ;
  	  	END IF ;

	ELSIF post_status = 'ERROR' THEN
  	  	IF pre_status = 'DOWNLOADING' OR pre_status = 'DISTRIBUTION' THEN
  	  	  	ret := p_cartsinv_update(in_packageid, post_status);
			RETURN ret ;
  	  	ELSE 
  	  	  	RAISE EXCEPTION 'p_distributionstate: Invalid Distribution State Transition' ;
  	  	END IF ;
	
	ELSIF post_status = 'ABORTED' THEN
  	  	ret := p_cartsinv_update(in_packageid, post_status);
		RETURN ret ;
	
	ELSE
  	  	 RAISE EXCEPTION 'p_distributionstate: Invalid Distribution State Transition' ;
	END IF ;
	
  	RETURN ret ;
	
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
			RETURN evn ;
		ELSIF post_status = 'STANDBY' THEN
			INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
			evn := p_inserteventtableitem('NUL2STA', 't_orderstatetransitions') ;
			RETURN evn ;
		ELSIF post_status = 'ABORTED' THEN
			INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
			evn := p_inserteventtableitem('2ABO', 't_orderstatetransitions') ;
			RETURN evn ;
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
		-- INSERT INTO t_orderstatetransitions(orderid ,status) values ( in_orderid,post_status);
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
	ELSIF post_status = 'ABORTED' THEN
		INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
		evn := p_inserteventtableitem('2ABO', 't_orderstatetransitions') ;
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
		
		UPDATE t_cartsinv SET status = post_status WHERE cart = p_cartid AND ordsubreq = p_subreq ;	
		err := p_carts_update_status(p_cartid);
	END IF ;

	RETURN 0 ;

END ;
$$ LANGUAGE plpgsql ;



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
	newdate    	TIMESTAMP(6);
	tmpdate    	DATE ;
	tmptime    	TIME(6) ;
	row        	RECORD ;
	p_extordid 	VARCHAR(32);
	p_cartid   	INT;
	p_subreq   	INT;
	p_totsubreq	INT;
	
BEGIN

	p_extordid := (SELECT externalorderid FROM t_ordersqueue WHERE orderid = in_orderid);
	p_subreq := (SELECT subreq FROM t_ordersqueue WHERE orderid = in_orderid);
	
	IF p_extordid IS NOT NULL AND p_subreq IS NOT NULL 
	THEN
		p_cartid := (SELECT id FROM t_cartsref AS cr
				INNER JOIN t_ordersqueue AS oq
				ON oq.externalorderid = cr.extref
				WHERE oq.orderid = in_orderid);

		p_totsubreq := (SELECT oq.subreq + cr.distreqs 
				FROM t_ordersqueue AS oq
				INNER JOIN t_cartsref AS cr
				ON oq.externalorderid = cr.extref
				WHERE oq.orderid = in_orderid);
		
		UPDATE t_cartsinv SET distsubreq = p_totsubreq WHERE cart = p_cartid AND ordsubreq = p_subreq ;
		
	ELSE
		p_cartid := NULL;
		p_totsubreq := NULL;
	END IF;


--	the input inventory file is distributed to each user who declared an interest on that file-type.
--	the distribution is done according to the active field in the distribution policy record
	IF in_format IS NULL THEN
		IF in_ordertype IS NULL THEN
			FOR row IN (SELECT DISTINCT dp.id, dp.mediatype, dp.deliverfrequency, dp.format, dp.algo
			  	  	  	  FROM t_distributionpolicies dp
						  	WHERE dp.userid=in_userid AND
							  	  dp.filetype = in_filetype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = in_filetype) AND
								  dp.active='t' AND
								  dp.ordertype IS NULL AND
								  dp.usermode = (SELECT mode FROM t_users WHERE userid = in_userid)) LOOP

					tmpdate := NOW() ;
					tmptime := NOW() ;
					newdate := (tmpdate + row.deliverfrequency) + tmptime ;

					INSERT INTO t_temporarydistribution 
					  ("duedate", "filename", "fileversion", "mediatype", "userid", "format", "algo", "policy", "orderid", "cartid", "subreq")
					VALUES
					  (newdate, in_fname, in_fversion, row.mediatype, in_userid, row.format, row.algo, row.id, in_orderid, p_cartid, p_totsubreq) ;
			END LOOP ;
		ELSE
			-- No format but order type
			FOR row IN (SELECT DISTINCT dp.id, dp.mediatype, dp.deliverfrequency, dp.format, dp.algo
				  	  	  FROM t_distributionpolicies dp
				  	  	  	WHERE dp.userid=in_userid AND
				  	  	  	  	  dp.filetype = in_filetype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = in_filetype) AND
					  	  	  	  dp.active='t' AND
					  	  	  	  (dp.ordertype=in_ordertype OR dp.ordertype IS NULL) AND
								  dp.usermode = (SELECT mode FROM t_users WHERE userid = in_userid)) LOOP

					tmpdate := NOW() ;
					tmptime := NOW() ;
					newdate := (tmpdate + row.deliverfrequency) + tmptime ;

					INSERT INTO t_temporarydistribution
					  ("duedate", "filename", "fileversion", "mediatype", "userid", "format", "algo", "policy", "orderid", "cartid", "subreq")
					VALUES 
					  (newdate, in_fname, in_fversion, row.mediatype, in_userid, row.format, row.algo, row.id, in_orderid, p_cartid, p_totsubreq) ;
			END LOOP ;
		END IF ;
	ELSE
		IF in_ordertype IS NULL THEN

			-- Format defined in input
			FOR row IN (SELECT DISTINCT dp.id, dp.mediatype, dp.deliverfrequency, dp.algo
  	  	  	  	  	  	  FROM t_distributionpolicies dp
  	  	  	  	  	  	  	WHERE dp.userid=in_userid AND
							  	  dp.filetype = in_filetype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = in_filetype) AND
								  dp.active='t' AND
								  dp.format=in_format AND
								  dp.ordertype IS NULL AND
								  dp.usermode = (SELECT mode FROM t_users WHERE userid = in_userid)) LOOP

					tmpdate := NOW() ;
					tmptime := NOW() ;
					newdate := (tmpdate + row.deliverfrequency) + tmptime ;

					INSERT INTO t_temporarydistribution
					  ("duedate", "filename", "fileversion", "mediatype", "userid", "format", "algo", "policy", "orderid", "cartid", "subreq")
						VALUES
  	  	  	  	  	  (newdate, in_fname, in_fversion, row.mediatype, in_userid, in_format, row.algo, row.id, in_orderid, p_cartid, p_totsubreq) ;
			END LOOP ;
		ELSE
			-- Format and order type defined in input
			FOR row IN (SELECT DISTINCT dp.id, dp.mediatype, dp.deliverfrequency, dp.algo
				  	  	  FROM t_distributionpolicies dp
				  	  	  	WHERE dp.userid=in_userid AND
							  	  dp.filetype = in_filetype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = in_filetype) AND
								  dp.active='t' AND
								  dp.format=in_format AND
								  (dp.ordertype=in_ordertype OR dp.ordertype IS NULL) AND
								  dp.usermode = (SELECT mode FROM t_users WHERE userid = in_userid)) LOOP

					tmpdate := NOW() ;
					tmptime := NOW() ;
					newdate := (tmpdate + row.deliverfrequency) + tmptime ;

					INSERT INTO t_temporarydistribution
					  ("duedate", "filename", "fileversion", "mediatype", "userid", "format", "algo", "policy", "orderid", "cartid", "subreq")
					VALUES
					  (newdate, in_fname, in_fversion, row.mediatype, in_userid, in_format, row.algo, row.id, in_orderid, p_cartid, p_totsubreq) ;
			END LOOP ;
		END IF ;
	END IF ;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
------------------------------- TRIGGERS CREATION -------------------------------
---------------------------------------------------------------------------------

------------------
--- t_cartsinv ---
------------------
CREATE TRIGGER tr_cartsinvinsert AFTER INSERT
  ON t_cartsinv EXECUTE PROCEDURE tf_inserteventtableitem('t_cartsinv','INS') ;

CREATE TRIGGER tr_cartsinvupdate AFTER UPDATE
  ON t_cartsinv EXECUTE PROCEDURE tf_inserteventtableitem('t_cartsinv', 'UPD') ;









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
		'8.1.0',
		'2009-06-17',
		3,
		1,
		'SchemaVersion_8-1-0',
		'Modified tables t_cartsinv, t_cartsref and t_standingdistribution. Modified procedures p_distribute_item, p_distributionstate and p_orderstatetransition. Added trigger on t_cartsinv'
) ;
