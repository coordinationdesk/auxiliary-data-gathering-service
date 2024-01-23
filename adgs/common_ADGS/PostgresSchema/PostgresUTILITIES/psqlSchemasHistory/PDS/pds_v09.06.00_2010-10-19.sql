
-- Version 9.6.0
-- POSTGRES








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
	p_extordid 	VARCHAR(128);
	p_cartid   	INT;
	p_subreq   	INT;
	p_totsubreq	INT;
	p_chng_add	BOOLEAN;
	
BEGIN
	p_chng_add := 'f';
	
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
					
					p_chng_add := 'f';
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
					
					p_chng_add := 'f';
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
					  					
					p_chng_add := 'f';
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
					  					
					p_chng_add := 'f';
			END LOOP ;
		END IF ;
	END IF ;
	
	IF p_chng_add = 't' THEN
		UPDATE t_cartsinv SET distsubreq = p_totsubreq WHERE cart = p_cartid AND ordsubreq = p_subreq ;
	END IF ;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;





/* ------------------------------
	P_DISTRIBUTE_ITEM_WDATE ---
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_distribute_item_wdate(
	in_fname VARCHAR(128), 
	in_fversion VARCHAR(8), 
	in_filetype VARCHAR(16), 
	in_userid VARCHAR(32), 
	in_ordertype VARCHAR(8), 
	in_format VARCHAR(16), 
	in_orderid INTEGER,
	in_duedays INT2) RETURNS INTEGER AS $$

DECLARE
	newdate    	TIMESTAMP(6);
	tmpdate    	DATE ;
	tmptime    	TIME(6) ;
	row        	RECORD ;
	p_extordid 	VARCHAR(128);
	p_cartid   	INT;
	p_subreq   	INT;
	p_totsubreq	INT;
	p_chng_add	BOOLEAN;
	
BEGIN
	p_chng_add := 'f';

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
	ELSE
		p_cartid := NULL;
		p_totsubreq := NULL;
	END IF;


--	the input inventory file is distributed to each user who declared an interest on that file-type.
--	the distribution is done according to the active field in the distribution policy record
	IF in_format IS NULL THEN
		IF in_ordertype IS NULL THEN
			FOR row IN (SELECT DISTINCT dp.id, dp.mediatype, dp.format, dp.algo
			  	  	  	  FROM t_distributionpolicies dp
						  	WHERE dp.userid = in_userid 
								AND (dp.filetype = in_filetype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = in_filetype)) 
								AND dp.active = 't' 
								AND dp.ordertype IS NULL 
								AND dp.usermode = (SELECT mode FROM t_users WHERE userid = in_userid)) LOOP

					tmpdate := NOW() ;
					tmptime := NOW() ;
					newdate := (tmpdate + in_duedays) + tmptime ;

					INSERT INTO t_temporarydistribution 
					  ("duedate", "filename", "fileversion", "mediatype", "userid", "format", "algo", "policy", "orderid", "cartid", "subreq")
					VALUES
					  (newdate, in_fname, in_fversion, row.mediatype, in_userid, row.format, row.algo, row.id, in_orderid, p_cartid, p_totsubreq) ;
					
					p_chng_add := 'f';
			END LOOP ;
		ELSE
			-- No format but order type
			FOR row IN (SELECT DISTINCT dp.id, dp.mediatype, dp.format, dp.algo
				  	  	  FROM t_distributionpolicies dp
				  	  	  	WHERE dp.userid = in_userid 
								AND (dp.filetype = in_filetype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = in_filetype)) 
								AND dp.active = 't' 
								AND (dp.ordertype = in_ordertype OR dp.ordertype IS NULL) 
								AND dp.usermode = (SELECT mode FROM t_users WHERE userid = in_userid)) LOOP

					tmpdate := NOW() ;
					tmptime := NOW() ;
					newdate := (tmpdate + in_duedays) + tmptime ;

					INSERT INTO t_temporarydistribution
					  ("duedate", "filename", "fileversion", "mediatype", "userid", "format", "algo", "policy", "orderid", "cartid", "subreq")
					VALUES 
					  (newdate, in_fname, in_fversion, row.mediatype, in_userid, row.format, row.algo, row.id, in_orderid, p_cartid, p_totsubreq) ;
					  					
					p_chng_add := 'f';
			END LOOP ;
		END IF ;
	ELSE
		IF in_ordertype IS NULL THEN

			-- Format defined in input
			FOR row IN (SELECT DISTINCT dp.id, dp.mediatype, dp.algo
  	  	  	  	  	  	  FROM t_distributionpolicies dp
  	  	  	  	  	  	  	WHERE dp.userid=in_userid 
								AND (dp.filetype = in_filetype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = in_filetype)) 
								AND dp.active = 't' 
								AND dp.format = in_format 
								AND dp.ordertype IS NULL 
								AND dp.usermode = (SELECT mode FROM t_users WHERE userid = in_userid)) LOOP

					tmpdate := NOW() ;
					tmptime := NOW() ;
					newdate := (tmpdate + in_duedays) + tmptime ;

					INSERT INTO t_temporarydistribution
					  ("duedate", "filename", "fileversion", "mediatype", "userid", "format", "algo", "policy", "orderid", "cartid", "subreq")
						VALUES
  	  	  	  	  	  (newdate, in_fname, in_fversion, row.mediatype, in_userid, in_format, row.algo, row.id, in_orderid, p_cartid, p_totsubreq) ;
					  					
					p_chng_add := 'f';
			END LOOP ;
		ELSE
			-- Format and order type defined in input
			FOR row IN (SELECT DISTINCT dp.id, dp.mediatype, dp.algo
				  	  	  FROM t_distributionpolicies dp
				  	  	  	WHERE dp.userid=in_userid 
								AND (dp.filetype = in_filetype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = in_filetype)) 
								AND dp.active = 't' 
								AND dp.format = in_format 
								AND (dp.ordertype = in_ordertype OR dp.ordertype IS NULL) 
								AND dp.usermode = (SELECT mode FROM t_users WHERE userid = in_userid)) LOOP

					tmpdate := NOW() ;
					tmptime := NOW() ;
					newdate := (tmpdate + in_duedays) + tmptime ;

					INSERT INTO t_temporarydistribution
					  ("duedate", "filename", "fileversion", "mediatype", "userid", "format", "algo", "policy", "orderid", "cartid", "subreq")
					VALUES
					  (newdate, in_fname, in_fversion, row.mediatype, in_userid, in_format, row.algo, row.id, in_orderid, p_cartid, p_totsubreq) ;
					  					
					p_chng_add := 'f';
			END LOOP ;
		END IF ;
	END IF ;
	
	IF p_chng_add = 't' THEN
		UPDATE t_cartsinv SET distsubreq = p_totsubreq WHERE cart = p_cartid AND ordsubreq = p_subreq ;
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
	newdate			TIMESTAMP(6);
	tmpdate			DATE ;
	tmptime			TIME(6) ;
	row				RECORD ;
	p_extordid		VARCHAR(128);
	p_cartid		INT;
	p_subreq		INT;
	p_totsubreq		INT;
	p_ondem_media	VARCHAR(16);
	p_chng_add		BOOLEAN;
	
BEGIN
	p_chng_add := 'f';

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
									
						p_chng_add := 'f';
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
						  									
						p_chng_add := 'f';
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
						  									
						p_chng_add := 'f';
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
						  									
						p_chng_add := 'f';
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
						  									
						p_chng_add := 'f';
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
						  									
						p_chng_add := 'f';
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
						  									
						p_chng_add := 'f';
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
						  									
						p_chng_add := 'f';
				END LOOP ;
			END IF ;
		END IF ;
	END IF ;
	
	IF p_chng_add = 't' THEN
		UPDATE t_cartsinv SET distsubreq = p_totsubreq WHERE cart = p_cartid AND ordsubreq = p_subreq ;
	END IF ;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;





/* ------------------------------
	p_intervalseconds
*/ ------------------------------
DROP FUNCTION p_intervalseconds(timestamp without time zone, timestamp without time zone);

CREATE OR REPLACE FUNCTION p_intervalseconds(start TIMESTAMP(6), stop TIMESTAMP(6)) RETURNS BIGINT AS $$
DECLARE
	res BIGINT ;
BEGIN

	res := EXTRACT(EPOCH FROM AGE(stop, start)) ;
    RETURN res;

	EXCEPTION WHEN OTHERS THEN
	   RETURN -99999999 ;

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
		'pds-9.6.0',
		'2010-10-19',
		3,
		1,
		'PDS_9-6-0',
		'Fixed bug on procedures p_distribute_item, p_distribute_item_on_demand, p_distribute_item_wdate and p_intervalseconds.'
) ;
