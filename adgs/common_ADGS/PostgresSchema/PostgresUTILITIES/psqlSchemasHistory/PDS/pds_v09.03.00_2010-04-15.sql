
-- Version 9.3.0
-- POSTGRES








---------------------------------------------------------------------------------
------------------------------ TABLES MODIFICATION ------------------------------
---------------------------------------------------------------------------------

------------------
--- T_CARTSREF ---
------------------
ALTER TABLE t_cartsref ALTER COLUMN extref TYPE VARCHAR(128);







---------------------------------------------------------------------------------
---------------------------- PROCEDURES MODIFICATION ----------------------------
---------------------------------------------------------------------------------

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
	p_extordid VARCHAR(128);
	p_subreq INT;
	p_cartid INT;
	cartsinv_status VARCHAR(32);
	
BEGIN

	IF post_status IS NULL THEN
		RETURN 0;
	END IF ;

	IF pre_status IS NULL THEN
		IF post_status = 'ELIGIBLE' THEN
			INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
			evn := p_inserteventtableitem('t_orderstatetransitions', 'NUL2ELI') ;
			RETURN evn ;
		ELSIF post_status = 'STANDBY' THEN
			INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
			evn := p_inserteventtableitem('t_orderstatetransitions', 'NUL2STA') ;
			RETURN evn ;
		ELSIF post_status = 'ABORTED' THEN
			INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
			evn := p_inserteventtableitem('t_orderstatetransitions', '2ABO') ;
			RETURN evn ;
		ELSE
			RAISE EXCEPTION 'Invalid Order State Transition' ;
		END IF ;
	END IF ;

-- state control
	IF post_status = 'ELIGIBLE' THEN
		IF pre_status = 'ERROR' THEN
			INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
			evn := p_inserteventtableitem('t_orderstatetransitions', 'ERR2ELI') ;	
		ELSIF pre_status = 'RECOVERABLEERROR' THEN
			INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
			evn := p_inserteventtableitem('t_orderstatetransitions', 'REC2ELI') ;
		ELSIF pre_status = 'STANDBY' THEN
			INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
			evn := p_inserteventtableitem('t_orderstatetransitions', 'STA2ELI') ;
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
			evn := p_inserteventtableitem('t_orderstatetransitions', 'PRE2DOW') ;
		ELSE 
			RAISE EXCEPTION 'Invalid Order State Transition' ;
		END IF ;
	ELSIF post_status = 'PROCESSING' THEN
		IF pre_status = 'DOWNLOADING' THEN
			INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
			evn := p_inserteventtableitem('t_orderstatetransitions', 'DOW2PRO') ;
		ELSE 
			RAISE EXCEPTION 'Invalid Order State Transition' ;
		END IF ;
	ELSIF post_status = 'ERROR' THEN
		IF pre_status = 'DOWNLOADING' THEN
			INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
			evn := p_inserteventtableitem('t_orderstatetransitions', 'DOW2ERR') ;	
		ELSIF pre_status = 'STANDBY' THEN
			INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
			evn := p_inserteventtableitem('t_orderstatetransitions', 'STA2ERR') ;	
		ELSIF pre_status = 'PROCESSING' THEN
			INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
			evn := p_inserteventtableitem('t_orderstatetransitions', 'PRO2ERR') ;
		ELSE 
			RAISE EXCEPTION 'Invalid Order State Transition' ;
		END IF ;
	ELSIF post_status = 'RECOVERABLEERROR' THEN
		IF pre_status = 'PROCESSING' THEN
			INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
			evn := p_inserteventtableitem('t_orderstatetransitions', 'PRO2REC') ;
		ELSE 
			RAISE EXCEPTION 'Invalid Order State Transition' ;
		END IF ;
	ELSIF post_status = 'COMPLETED' THEN
		IF pre_status = 'PROCESSING'  THEN
			INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
			evn := p_inserteventtableitem('t_orderstatetransitions', 'PRO2COM') ;
			err := p_completed_order_distribution(in_orderid) ;
		ELSIF pre_status = 'ERROR' THEN
			INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
			evn := p_inserteventtableitem('t_orderstatetransitions', 'ERR2COM') ;
			err := p_completed_order_distribution(in_orderid) ;
		ELSE 
			RAISE EXCEPTION 'Invalid Order State Transition' ;
		END IF ;
	ELSIF post_status = 'ABORTED' THEN
		INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
		evn := p_inserteventtableitem('t_orderstatetransitions', '2ABO') ;
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
		
		IF post_status = 'COMPLETED' THEN
			cartsinv_status := 'PROCESSED';
		ELSE
			cartsinv_status := post_status ;
		END IF ;
		
		UPDATE t_cartsinv SET status = cartsinv_status WHERE cart = p_cartid AND ordsubreq = p_subreq ;	
		err := p_carts_update_status(p_cartid);
	END IF ;

	RETURN 0 ;

END ;
$$ LANGUAGE plpgsql ;




/* ------------------------------
	P_DISTRIBUTE_ITEM
*/ ------------------------------
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
	p_extordid 	VARCHAR(128);
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

		p_totsubreq := (SELECT max(c.distsubreq) +1 
				FROM t_cartsinv c 
				WHERE c.cart = p_cartid);
		
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
						  	WHERE dp.userid = in_userid 
								AND (dp.filetype = in_filetype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = in_filetype)) 
								AND dp.active = 't' 
								AND dp.ordertype IS NULL 
								AND dp.usermode = (SELECT mode FROM t_users WHERE userid = in_userid)) LOOP

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
				  	  	  	WHERE dp.userid = in_userid 
								AND (dp.filetype = in_filetype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = in_filetype)) 
								AND dp.active = 't' 
								AND (dp.ordertype = in_ordertype OR dp.ordertype IS NULL) 
								AND dp.usermode = (SELECT mode FROM t_users WHERE userid = in_userid)) LOOP

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
  	  	  	  	  	  	  	WHERE dp.userid=in_userid 
								AND (dp.filetype = in_filetype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = in_filetype)) 
								AND dp.active = 't' 
								AND dp.format = in_format 
								AND dp.ordertype IS NULL 
								AND dp.usermode = (SELECT mode FROM t_users WHERE userid = in_userid)) LOOP

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
				  	  	  	WHERE dp.userid=in_userid 
								AND (dp.filetype = in_filetype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = in_filetype)) 
								AND dp.active = 't' 
								AND dp.format = in_format 
								AND (dp.ordertype = in_ordertype OR dp.ordertype IS NULL) 
								AND dp.usermode = (SELECT mode FROM t_users WHERE userid = in_userid)) LOOP

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




/* ------------------------------
	P_DISTRIBUTE_ITEM_ON_DEMAND
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_distribute_item_on_demand(
	in_fname VARCHAR(128),
	in_fversion VARCHAR(8),
	in_filetype VARCHAR(16),
	in_userid VARCHAR(32),
	in_ordertype VARCHAR(8),
	in_format VARCHAR(8),
	in_orderid INTEGER) RETURNS INTEGER AS $$
	
DECLARE
	newdate    	  TIMESTAMP(6);
	tmpdate    	  DATE ;
	tmptime    	  TIME(6) ;
	row        	  RECORD ;
	p_extordid 	  VARCHAR(128);
	p_cartid   	  INT;
	p_subreq   	  INT;
	p_totsubreq	  INT;
	p_ondem_media VARCHAR(16);
	
BEGIN

	p_extordid := (SELECT externalorderid FROM t_ordersqueue WHERE orderid = in_orderid);
	p_subreq := (SELECT subreq FROM t_ordersqueue WHERE orderid = in_orderid);
	p_ondem_media := (SELECT od.mediatype FROM t_ondemanddistribution AS od WHERE od.orderid = in_orderid);
	
	
	IF p_extordid IS NOT NULL AND p_subreq IS NOT NULL 
	THEN
		p_cartid := (SELECT id FROM t_cartsref AS cr
				INNER JOIN t_ordersqueue AS oq
				ON oq.externalorderid = cr.extref
				WHERE oq.orderid = in_orderid);

		p_totsubreq := (SELECT max(c.distsubreq) +1 
				FROM t_cartsinv c 
				WHERE c.cart = p_cartid);
		
		UPDATE t_cartsinv SET distsubreq = p_totsubreq WHERE cart = p_cartid AND ordsubreq = p_subreq ;
	ELSE
		p_cartid := NULL;
		p_totsubreq := NULL;
	END IF;


--	the input inventory file is distributed to each user who declared an interest on that file-type.
--	the distribution is done according to the active field in the distribution policy record
	IF in_format IS NULL THEN
		IF p_ondem_media IS NOT NULL THEN
			IF in_ordertype IS NULL THEN
				FOR row IN (SELECT DISTINCT dp.id, dp.mediatype, dp.deliverfrequency, dp.format, dp.algo
			  	  	  		  FROM t_distributionpolicies dp
						  		WHERE dp.userid=in_userid 
									AND (dp.filetype = in_filetype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = in_filetype)) 
									AND dp.ordertype IS NULL 
									AND dp.usermode = (SELECT mode FROM t_users WHERE userid = in_userid)
									AND dp.algo IN (SELECT id from t_distributionalgo WHERE on_demand = 't')) LOOP

						tmpdate := NOW() ;
						tmptime := NOW() ;
						newdate := (tmpdate + row.deliverfrequency) + tmptime ;

						INSERT INTO t_temporarydistribution 
						  ("duedate", "filename", "fileversion", "mediatype", "userid", "format", "algo", "policy", "orderid", "cartid", "subreq")
						VALUES
						  (newdate, in_fname, in_fversion, p_ondem_media, in_userid, row.format, row.algo, row.id, in_orderid, p_cartid, p_totsubreq) ;
				END LOOP ;
			ELSE
				-- No format but order type
				FOR row IN (SELECT DISTINCT dp.id, dp.mediatype, dp.deliverfrequency, dp.format, dp.algo
				  	  		  FROM t_distributionpolicies dp
				  	  	  		WHERE dp.userid=in_userid 
									AND (dp.filetype = in_filetype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = in_filetype)) 
									AND (dp.ordertype = in_ordertype OR dp.ordertype IS NULL) 
									AND dp.usermode = (SELECT mode FROM t_users WHERE userid = in_userid)
									AND dp.algo IN (SELECT id from t_distributionalgo WHERE on_demand = 't')) LOOP

						tmpdate := NOW() ;
						tmptime := NOW() ;
						newdate := (tmpdate + row.deliverfrequency) + tmptime ;

						INSERT INTO t_temporarydistribution
						  ("duedate", "filename", "fileversion", "mediatype", "userid", "format", "algo", "policy", "orderid", "cartid", "subreq")
						VALUES 
						  (newdate, in_fname, in_fversion, p_ondem_media, in_userid, row.format, row.algo, row.id, in_orderid, p_cartid, p_totsubreq) ;
				END LOOP ;
			END IF ;
		ELSE
			IF in_ordertype IS NULL THEN
				FOR row IN (SELECT DISTINCT dp.id, dp.mediatype, dp.deliverfrequency, dp.format, dp.algo
			  	  	  		  FROM t_distributionpolicies dp
						  		WHERE dp.userid=in_userid 
									AND (dp.filetype = in_filetype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = in_filetype)) 
									AND dp.ordertype IS NULL 
									AND dp.usermode = (SELECT mode FROM t_users WHERE userid = in_userid)) LOOP

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
				  	  	  		WHERE dp.userid=in_userid 
									AND (dp.filetype = in_filetype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = in_filetype)) 
									AND (dp.ordertype = in_ordertype OR dp.ordertype IS NULL) 
									AND dp.usermode = (SELECT mode FROM t_users WHERE userid = in_userid)) LOOP

						tmpdate := NOW() ;
						tmptime := NOW() ;
						newdate := (tmpdate + row.deliverfrequency) + tmptime ;

						INSERT INTO t_temporarydistribution
						  ("duedate", "filename", "fileversion", "mediatype", "userid", "format", "algo", "policy", "orderid", "cartid", "subreq")
						VALUES 
						  (newdate, in_fname, in_fversion, row.mediatype, in_userid, row.format, row.algo, row.id, in_orderid, p_cartid, p_totsubreq) ;
				END LOOP ;
			END IF ;
		END IF ;
	ELSE
		IF p_ondem_media IS NOT NULL THEN
			IF in_ordertype IS NULL THEN

				-- Format defined in input
				FOR row IN (SELECT DISTINCT dp.id, dp.mediatype, dp.deliverfrequency, dp.algo
  	  	  	  	  	  		  FROM t_distributionpolicies dp
  	  	  	  	  	  	  		WHERE dp.userid=in_userid 
									AND (dp.filetype = in_filetype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = in_filetype)) 
									--AND dp.format = in_format 
									AND dp.ordertype IS NULL 
									AND dp.usermode = (SELECT mode FROM t_users WHERE userid = in_userid)
									AND dp.algo IN (SELECT id from t_distributionalgo WHERE on_demand = 't')) LOOP

						tmpdate := NOW() ;
						tmptime := NOW() ;
						newdate := (tmpdate + row.deliverfrequency) + tmptime ;

						INSERT INTO t_temporarydistribution
						  ("duedate", "filename", "fileversion", "mediatype", "userid", "format", "algo", "policy", "orderid", "cartid", "subreq")
							VALUES
  	  	  	  	  		  (newdate, in_fname, in_fversion, p_ondem_media, in_userid, in_format, row.algo, row.id, in_orderid, p_cartid, p_totsubreq) ;
				END LOOP ;
			ELSE
				-- Format and order type defined in input
				FOR row IN (SELECT DISTINCT dp.id, dp.mediatype, dp.deliverfrequency, dp.algo
				  	  		  FROM t_distributionpolicies dp
				  	  	  		WHERE dp.userid=in_userid 
									AND (dp.filetype = in_filetype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = in_filetype)) 
									--AND dp.format = in_format 
									AND (dp.ordertype = in_ordertype OR dp.ordertype IS NULL) 
									AND dp.usermode = (SELECT mode FROM t_users WHERE userid = in_userid)
									AND dp.algo IN (SELECT id from t_distributionalgo WHERE on_demand = 't')) LOOP

						tmpdate := NOW() ;
						tmptime := NOW() ;
						newdate := (tmpdate + row.deliverfrequency) + tmptime ;

						INSERT INTO t_temporarydistribution
						  ("duedate", "filename", "fileversion", "mediatype", "userid", "format", "algo", "policy", "orderid", "cartid", "subreq")
						VALUES
						  (newdate, in_fname, in_fversion, p_ondem_media, in_userid, in_format, row.algo, row.id, in_orderid, p_cartid, p_totsubreq) ;
				END LOOP ;
			END IF ;
		ELSE
			IF in_ordertype IS NULL THEN

				-- Format defined in input
				FOR row IN (SELECT DISTINCT dp.id, dp.mediatype, dp.deliverfrequency, dp.algo
  	  	  	  	  	  		  FROM t_distributionpolicies dp
  	  	  	  	  	  	  		WHERE dp.userid=in_userid 
									AND (dp.filetype = in_filetype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = in_filetype)) 
									AND dp.format = in_format 
									AND dp.ordertype IS NULL 
									AND dp.usermode = (SELECT mode FROM t_users WHERE userid = in_userid)) LOOP

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
				  	  	  		WHERE dp.userid=in_userid 
									AND (dp.filetype = in_filetype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = in_filetype)) 
									AND dp.format = in_format 
									AND (dp.ordertype = in_ordertype OR dp.ordertype IS NULL) 
									AND dp.usermode = (SELECT mode FROM t_users WHERE userid = in_userid)) LOOP

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
	END IF ;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;






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
		'pds-9.3.0',
		'2010-04-15',
		3,
		1,
		'SchemaVersion_9-3-0',
		'Modified table t_cartsref and procedures p_orderstatetransition, p_distribute_item and p_distribute_item_on_demand.'
) ;
