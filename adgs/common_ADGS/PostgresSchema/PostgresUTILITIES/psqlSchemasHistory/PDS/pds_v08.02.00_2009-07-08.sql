
-- Version 8.2.0
-- POSTGRES








---------------------------------------------------------------------------------
------------------------------- TABLES POPULATION -------------------------------
---------------------------------------------------------------------------------

---------------------
--- T_SATRELATION ---
---------------------
INSERT INTO t_satrelations (satelliteid, mission, sensorid) VALUES (5, 1, 18);








---------------------------------------------------------------------------------
---------------------------- PROCEDURES MODIFICATION ----------------------------
---------------------------------------------------------------------------------

------------------------------
--- P_INSERTEVENTTABLEITEM ---
------------------------------
CREATE OR REPLACE FUNCTION p_inserteventtableitem(mytable VARCHAR(64), myaction VARCHAR(16)) RETURNS INTEGER AS $$
DECLARE
	ret INTEGER;
BEGIN
	ret := f_acs_traceEvent('/tmp/PDS_events', mytable, myaction) ;
	RETURN ret;
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
							  	  (dp.filetype = in_filetype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = in_filetype)) AND
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
				  	  	  	  	  (dp.filetype = in_filetype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = in_filetype)) AND
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
							  	  (dp.filetype = in_filetype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = in_filetype)) AND
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
							  	  (dp.filetype = in_filetype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = in_filetype)) AND
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
	newdate TIMESTAMP(6);
	tmpdate DATE ;
	tmptime TIME(6) ;
	row   	RECORD ;
	
BEGIN

--	the input inventory file is distributed to each user who declared an interest on that file-type.
--	the distribution is done according to the active field in the distribution policy record
	IF in_format IS NULL THEN
		IF in_ordertype IS NULL THEN
			FOR row IN (SELECT DISTINCT dp.id, dp.mediatype, dp.deliverfrequency, dp.format, dp.algo
			  	  	  	  FROM t_distributionpolicies dp
						  	WHERE dp.userid=in_userid AND
							  	  (dp.filetype = in_filetype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = in_filetype)) AND
								  dp.ordertype IS NULL AND
								  dp.usermode = (SELECT mode FROM t_users WHERE userid = in_userid)) LOOP

					tmpdate := NOW() ;
					tmptime := NOW() ;
					newdate := (tmpdate + row.deliverfrequency) + tmptime ;

					INSERT INTO t_temporarydistribution 
					  ("duedate", "filename", "fileversion", "mediatype", "userid", "format", "algo", "policy", "orderid")
					VALUES
					  (newdate, in_fname, in_fversion, row.mediatype, in_userid, row.format, row.algo, row.id, in_orderid) ;
			END LOOP ;
		ELSE
			-- No format but order type
			FOR row IN (SELECT DISTINCT dp.id, dp.mediatype, dp.deliverfrequency, dp.format, dp.algo
				  	  	  FROM t_distributionpolicies dp
				  	  	  	WHERE dp.userid=in_userid AND
				  	  	  	  	  (dp.filetype = in_filetype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = in_filetype)) AND
					  	  	  	  (dp.ordertype = in_ordertype OR dp.ordertype IS NULL) AND
								  dp.usermode = (SELECT mode FROM t_users WHERE userid = in_userid)) LOOP

					tmpdate := NOW() ;
					tmptime := NOW() ;
					newdate := (tmpdate + row.deliverfrequency) + tmptime ;

					INSERT INTO t_temporarydistribution
					  ("duedate", "filename", "fileversion", "mediatype", "userid", "format", "algo", "policy", "orderid")
					VALUES 
					  (newdate, in_fname, in_fversion, row.mediatype, in_userid, row.format, row.algo, row.id, in_orderid) ;
			END LOOP ;
		END IF ;
	ELSE
		IF in_ordertype IS NULL THEN

			-- Format defined in input
			FOR row IN (SELECT DISTINCT dp.id, dp.mediatype, dp.deliverfrequency, dp.algo
  	  	  	  	  	  	  FROM t_distributionpolicies dp
  	  	  	  	  	  	  	WHERE dp.userid=in_userid AND
							  	  (dp.filetype = in_filetype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = in_filetype)) AND
								  dp.format = in_format AND
								  dp.ordertype IS NULL AND
								  dp.usermode = (SELECT mode FROM t_users WHERE userid = in_userid)) LOOP

					tmpdate := NOW() ;
					tmptime := NOW() ;
					newdate := (tmpdate + row.deliverfrequency) + tmptime ;

					INSERT INTO t_temporarydistribution
					  ("duedate", "filename", "fileversion", "mediatype", "userid", "format", "algo", "policy", "orderid")
						VALUES
  	  	  	  	  	  (newdate, in_fname, in_fversion, row.mediatype, in_userid, in_format, row.algo, row.id, in_orderid) ;
			END LOOP ;
		ELSE
			-- Format and order type defined in input
			FOR row IN (SELECT DISTINCT dp.id, dp.mediatype, dp.deliverfrequency, dp.algo
				  	  	  FROM t_distributionpolicies dp
				  	  	  	WHERE dp.userid=in_userid AND
							  	  (dp.filetype = in_filetype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = in_filetype)) AND
								  dp.format=in_format AND
								  (dp.ordertype = in_ordertype OR dp.ordertype IS NULL) AND
								  dp.usermode = (SELECT mode FROM t_users WHERE userid = in_userid)) LOOP

					tmpdate := NOW() ;
					tmptime := NOW() ;
					newdate := (tmpdate + row.deliverfrequency) + tmptime ;

					INSERT INTO t_temporarydistribution
					  ("duedate", "filename", "fileversion", "mediatype", "userid", "format", "algo", "policy", "orderid")
					VALUES
					  (newdate, in_fname, in_fversion, row.mediatype, in_userid, in_format, row.algo, row.id, in_orderid) ;
			END LOOP ;
		END IF ;
	END IF ;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;





/* ------------------------------
	P_DISTRIBUTE_ORDER_INVENTORY
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_distribute_order_inventory(
	in_orderid INTEGER, 
	in_ordertype VARCHAR(8), 
	in_format VARCHAR(8)) RETURNS INTEGER AS $$
	
DECLARE
  	row_inv RECORD ;
  	row   	RECORD ;
  	err   	INTEGER ;
	
BEGIN
--
--	this is the standard way to distribute the output of an order:
--	each inventory is distributed to users who DEFINEd a valid distribution policy for it
--
	IF in_orderid IS NOT NULL THEN
  	  	FOR row_inv IN (SELECT inv.filename, inv.fileversion, inv.filetype
		  	  	  	  	  FROM t_inventory inv
						  	WHERE inv.orderid=in_orderid) LOOP
  	  	  	IF in_format IS NULL THEN
  	  	  	  	IF in_ordertype IS NULL	THEN
  	  	  	  	  	FOR row IN (SELECT DISTINCT dp.userid, dp.format
					  	  	  	  FROM t_distributionpolicies AS dp
								  INNER JOIN t_users AS us
										ON dp.userid = us.userid AND dp.usermode = us.mode
								  	WHERE (dp.filetype = row_inv.filetype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = row_inv.filetype)) AND
									  	  dp.active='t' AND
										  dp.ordertype IS NULL) LOOP

  	  	  	  	  	  	err := p_distribute_item(row_inv.filename,
						  	  	  	  	  	  	 row_inv.fileversion,
												 row_inv.filetype,
												 row.userid,
												 in_ordertype,
												 row.format,
												 in_orderid) ;

					END LOOP ; 
  	  	  	ELSE
  	  	  	  	  	FOR row IN (SELECT DISTINCT dp.userid, dp.format
					  	  	  	  FROM t_distributionpolicies AS dp
								  INNER JOIN t_users AS us
										ON dp.userid = us.userid AND dp.usermode = us.mode
								  	WHERE (dp.filetype = row_inv.filetype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = row_inv.filetype)) AND
									  	  dp.active='t' AND
										  (dp.ordertype=in_ordertype OR dp.ordertype IS NULL)) LOOP

  	  	  	  	  	  	err := p_distribute_item(row_inv.filename,
						  	  	  	  	  	  	 row_inv.fileversion,
												 row_inv.filetype,
												 row.userid,
												 in_ordertype,
												 row.format,
												 in_orderid) ;
					END LOOP ; 
  	  	  	  	END IF ;
  	  	  	ELSE
  	  	  	  	IF in_ordertype IS NULL THEN

  	  	  	  	  	FOR row IN (SELECT DISTINCT dp.userid
					  	  	  	  FROM t_distributionpolicies AS dp
								  INNER JOIN t_users AS us
										ON dp.userid = us.userid AND dp.usermode = us.mode
								  	WHERE (dp.filetype = row_inv.filetype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = row_inv.filetype)) AND
									  	  dp.active='t' AND
										  dp.format=in_format AND
										  dp.ordertype IS NULL) LOOP

  	  	  	  	  	  	err := p_distribute_item(row_inv.filename,
						  	  	  	  	  	  	 row_inv.fileversion,
												 row_inv.filetype,
												 row.userid,
												 in_ordertype,
												 in_format,
												 in_orderid) ;

					END LOOP ; 
  	  	  	  	ELSE
  	  	  	  	  	FOR row IN (SELECT DISTINCT dp.userid
					  	  	  	  FROM t_distributionpolicies AS dp
								  INNER JOIN t_users AS us
										ON dp.userid = us.userid AND dp.usermode = us.mode
								  	WHERE (dp.filetype = row_inv.filetype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = row_inv.filetype)) AND
									  	  dp.active='t' AND
										  dp.format=in_format AND
										  (dp.ordertype=in_ordertype OR dp.ordertype IS NULL)) LOOP

  	  	  	  	  	  	err := p_distribute_item(row_inv.filename,
						  	  	  	  	  	  	 row_inv.fileversion,
												 row_inv.filetype,
												 row.userid,
												 in_ordertype,
												 in_format,
												 in_orderid) ;
					END LOOP ; 
  	  	  	  	END IF ;
  	  	  	END IF ;
  	  	END LOOP ;
  	END IF ;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;





/* ------------------------------
	P_DISTRIBUTE_STANDARD_INVENTORY
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_distribute_standard_inventory(
	in_fname VARCHAR(128),
	in_fversion VARCHAR(8),
	in_orderid INTEGER,
	ftype VARCHAR(16)) RETURNS INTEGER AS $$
	
DECLARE
  	row RECORD ;
	err INTEGER ;
	
BEGIN
--
--	a standard inventory is the one who don't come from an order (orderid is null)
--	the distribution is done just looking into the t_distributionpolicy table
--	with the active field set to true
--
	IF in_orderid IS NULL THEN
		
  	  	FOR row IN (SELECT dp.userid, dp.format
	  	  	  	  	  FROM t_distributionpolicies AS dp
					  INNER JOIN t_users AS us
							ON dp.userid = us.userid AND dp.usermode = us.mode
					  WHERE (dp.filetype = ftype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = ftype))   
							AND dp.active = 't' 
							AND dp.ordertype IS NULL) LOOP

  	  	  	err := p_distribute_item(in_fname, in_fversion, ftype, row.userid, NULL, row.format, in_orderid) ;

		END LOOP ;

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
		'8.2.0',
		'2009-07-08',
		3,
		1,
		'SchemaVersion_8-2-0',
		'Added new satrelation. Modified procedures p_inserteventtableitem, p_distribute_item, p_distribute_item_on_demand, p_distribute_order_inventory and p_distribute_standard_inventory.'
) ;
