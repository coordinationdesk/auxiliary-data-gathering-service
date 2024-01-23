
-- Version 8.3.0
-- POSTGRES








---------------------------------------------------------------------------------
------------------------------ TABLES MODIFICATION ------------------------------
---------------------------------------------------------------------------------

--------------------------
--- T_DISTRIBUTIONALGO ---
--------------------------
ALTER TABLE t_distributionalgo ADD on_demand BOOLEAN DEFAULT 't' NOT NULL ;
UPDATE t_distributionalgo SET on_demand = 'f' WHERE name = 'PARTIALCVRG' ;
UPDATE t_distributionalgo SET on_demand = 'f' WHERE name = 'TOTALCVRG' ;

------------------------------
--- T_ONDEMANDDISTRIBUTION ---
------------------------------
ALTER TABLE t_ondemanddistribution ADD mediatype VARCHAR(16) ;
ALTER TABLE t_ondemanddistribution 
	ADD CONSTRAINT fk_ondemand_mediatype 
		FOREIGN KEY (mediatype)
			REFERENCES t_mediatypes (mediatype) 
				ON DELETE CASCADE ;

------------------
--- T_CARTSINV ---
------------------
ALTER TABLE t_cartsinv 
	ADD CONSTRAINT q_cartsubreq 
		UNIQUE (cart, subreq);

----------------
--- T_SMTYPE ---
----------------
INSERT INTO t_smtype ("type") VALUES ('STAFSM') ;
INSERT INTO t_smtype ("type") VALUES ('AMSSM') ;
INSERT INTO t_smtype ("type") VALUES ('SatStore') ;








---------------------------------------------------------------------------------
------------------------------ PROCEDURES CREATION ------------------------------
---------------------------------------------------------------------------------

-----------------------
--- P_CARTSINVCHECK ---
-----------------------
CREATE FUNCTION p_cartsinvcheck(
	pre_id INT, 
	pre_cart INT, 
	pre_subreq INT, 
	pre_distsubreq INT, 
	pre_procsubreq INT, 
	pre_stndsubreq INT
) RETURNS INTEGER AS $$

BEGIN

	IF (pre_distsubreq != 0) THEN 
		IF ( ( SELECT count(*) FROM t_cartsinv WHERE id != pre_id  AND cart = pre_cart AND pre_distsubreq = distsubreq ) > 0 ) THEN 
			RAISE EXCEPTION 'distribution subrequest already present for cart in t_cartsinv ' ;
			RETURN 1;
		END IF ;
	
	END IF ;

	IF (pre_procsubreq != 0) THEN 
		IF ( ( SELECT count(*) FROM t_cartsinv WHERE id != pre_id  AND cart = pre_cart AND pre_procsubreq = ordsubreq ) > 0 ) THEN 
			RAISE EXCEPTION 'processing subrequest already present for cart in t_cartsinv' ;
			RETURN 1;
		END IF ;
	
	END IF ;

	IF (pre_stndsubreq != 0) THEN 
		IF ( ( SELECT count(*) FROM t_cartsinv WHERE id != pre_id  AND cart = pre_cart AND pre_stndsubreq = stndsubreq ) > 0 ) THEN 
			RAISE EXCEPTION 'standing subrequest already present for cart in t_cartsinv ' ;
			RETURN 1;
		END IF ;
	
	END IF ;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
---------------------------- PROCEDURES MODIFICATION ----------------------------
---------------------------------------------------------------------------------

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
	p_extordid 	  VARCHAR(32);
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




/* ------------------------------
	P_COMPLETED_ORDER_DISTRIBUTION
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_completed_order_distribution(in_orderid INTEGER) RETURNS INTEGER AS $$

DECLARE
  	row_odd	    RECORD ;
  	row_inv	    RECORD ;
	p_ordertype VARCHAR(8) ;
	dueUsers    INTEGER ;
	err   	    INTEGER ;
	
	--
	-- the procedure distributes the output from a completed order
	--
	
BEGIN

	p_ordertype := (SELECT ot.ordertype FROM t_ordersqueue ot WHERE ot.orderid = in_orderid) ;
	--
	--	check if the users are already assigned to that order looking into t_ondemanddistribution
	--
	dueUsers := (SELECT COUNT(*) FROM t_ondemanddistribution WHERE "orderid" = in_orderid) ;
	IF dueUsers = 0 THEN
	--
	--	no preDEFINEd user : proceed with the configured ones
	--
		err := p_distribute_order_inventory(in_orderid, p_ordertype, NULL) ;
		RETURN 0 ;
	END IF ;
	--
	--	a set of users was found: distribute items to that users according to their distribution policies
	--
	FOR row_odd IN (
		SELECT odd.userid, odd.format 
		FROM t_ondemanddistribution odd 
		WHERE odd.orderid=in_orderid) LOOP

  	  	FOR row_inv IN (
			SELECT inv.filename, inv.fileversion, inv.filetype 
			FROM t_inventory inv 
			WHERE inv.orderid=in_orderid) LOOP

			err := p_distribute_item_on_demand(row_inv.filename, row_inv.fileversion, row_inv.filetype, row_odd.userid, p_ordertype, row_odd.format, in_orderid) ;
		END LOOP ;
  	END LOOP ;
	
	RETURN 0 ;
END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
-------------------------- TRIGGERS FUNCTIONS CREATION --------------------------
---------------------------------------------------------------------------------

/***************************************************************************/
/* Function callable from triggers tr_cartsinvinsert/update to wrap up the */
/* p_cartsinvcheck and p_inserteventtableitem fucntions.                   */
/* The first argument of the function called by the trigger is used to     */
/* check if the even is an insert or an update                             */
/***************************************************************************/
CREATE FUNCTION tf_cartsinvtrmgr() RETURNS TRIGGER AS $$

DECLARE
  	status INTEGER ;

BEGIN
  	status := p_cartsinvcheck(NEW.id, NEW.cart, NEW.subreq, NEW.distsubreq, NEW.ordsubreq, NEW.stndsubreq ) ;
	
	IF (TG_ARGV[0] = 'INS') THEN
		status := p_inserteventtableitem('t_cartsinv' ,'INS') ;
	END IF ;
	
	IF (TG_ARGV[0] = 'UPD') THEN
		status := p_inserteventtableitem('t_cartsinv' ,'UPD') ;
  	END IF ;

  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
----------------------------- TRIGGERS MODIFICATION -----------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_cartsinv
*/ ------------------------------
DROP TRIGGER tr_cartsinvinsert ON t_cartsinv;
CREATE TRIGGER tr_cartsinvinsert AFTER INSERT
	ON t_cartsinv
		FOR EACH ROW 
			EXECUTE PROCEDURE tf_cartsinvtrmgr('INS') ;

DROP TRIGGER tr_cartsinvupdate  ON t_cartsinv;
CREATE TRIGGER tr_cartsinvupdate AFTER UPDATE
	ON t_cartsinv
		FOR EACH ROW 
			EXECUTE PROCEDURE tf_cartsinvtrmgr('UPD');








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
		'8.3.0',
		'2009-07-20',
		3,
		1,
		'SchemaVersion_8-3-0',
		'Altered tables t_cartsinv, t_distributionalgo and t_ondemanddistribution. Added procedure p_cartsinvcheck. Modified procedures p_distribute_item, p_distribute_item_on_demand and p_completed_order_distribution.'
) ;
