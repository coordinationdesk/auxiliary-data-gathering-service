
-- Version 8.0.0
-- POSTGRES








---------------------------------------------------------------------------------
-------------------------------- TABLES CREATION --------------------------------
---------------------------------------------------------------------------------

-------------------
--- T_USERMODES ---
-------------------
CREATE TABLE t_usermodes (
    id SERIAL NOT NULL,
    mode VARCHAR(16) NOT NULL
);
ALTER TABLE t_usermodes 
	ADD CONSTRAINT pk_t_usermodes 
		PRIMARY KEY (id);

INSERT INTO t_usermodes (id, mode) VALUES (1, 'NOMINAL') ;
INSERT INTO t_usermodes (id, mode) VALUES (2, 'TEST') ;
INSERT INTO t_usermodes (id, mode) VALUES (3, 'DEBUG') ;



----------------------
--- T_STORAGEGROUP ---
----------------------
CREATE TABLE t_storagegroup (
    id SERIAL NOT NULL,
    filetype VARCHAR(16) NOT NULL,
    storagemanager INT NOT NULL,
    groupname VARCHAR(64) NOT NULL
);
ALTER TABLE t_storagegroup 
	ADD CONSTRAINT pk_t_storagegroup 
		PRIMARY KEY (id);

ALTER TABLE t_storagegroup 
	ADD CONSTRAINT fk_storagegroup_ftype 
		FOREIGN KEY (filetype)
			REFERENCES t_filetypes (filetype) 
				ON DELETE CASCADE ;

ALTER TABLE t_storagegroup 
	ADD CONSTRAINT fk_storagegroup_storage
		FOREIGN KEY (storagemanager) 
			REFERENCES t_storagemanager (id) 
				ON DELETE CASCADE ;

ALTER TABLE t_storagegroup 
	ADD CONSTRAINT uq_storagegroup01 
		UNIQUE (filetype, storagemanager) ;








---------------------------------------------------------------------------------
------------------------------- TABLES ALTERATION -------------------------------
---------------------------------------------------------------------------------

---------------
--- T_USERS ---
---------------
ALTER TABLE t_users ADD mode INT DEFAULT 1 NOT NULL;

ALTER TABLE t_users 
	ADD CONSTRAINT fk_users_modes
		FOREIGN KEY (mode)
			REFERENCES t_usermodes (id) 
				ON DELETE CASCADE ;


---------------------------
--- T_RECEPTIONPOLICIES ---
---------------------------
ALTER TABLE t_receptionpolicies ADD usermode INT DEFAULT 1 NOT NULL;

ALTER TABLE t_receptionpolicies 
	ADD CONSTRAINT fk_receptpol_usermodes 
		FOREIGN KEY (usermode)
			REFERENCES t_usermodes (id) 
				ON DELETE CASCADE ;

ALTER TABLE t_receptionpolicies DROP CONSTRAINT uq_receptionpolicies;

ALTER TABLE t_receptionpolicies 
	ADD CONSTRAINT uq_receptpolicies01
		UNIQUE (filetype, repository, usermode);


------------------------------
--- T_DISTRIBUTIONPOLICIES ---
------------------------------
ALTER TABLE t_distributionpolicies ADD usermode INT DEFAULT 1 NOT NULL;

ALTER TABLE t_distributionpolicies 
	ADD CONSTRAINT fk_distpol_usermodes 
		FOREIGN KEY (usermode)
			REFERENCES t_usermodes (id) 
				ON DELETE CASCADE ;
	
ALTER TABLE t_distributionpolicies DROP CONSTRAINT uq_distributionpolicies;

ALTER TABLE t_distributionpolicies 
	ADD CONSTRAINT uq_distpolicies01 
		UNIQUE (filetype, mediatype, userid, ordertype, usermode);








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
					  (newdate, in_fname, in_fversion, row.mediatype, in_userid, row.format, row.algo, row.id, in_orderid, p_cartid, p_subreq) ;
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
					  (newdate, in_fname, in_fversion, row.mediatype, in_userid, row.format, row.algo, row.id, in_orderid, p_cartid, p_subreq) ;
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
  	  	  	  	  	  (newdate, in_fname, in_fversion, row.mediatype, in_userid, in_format, row.algo, row.id, in_orderid, p_cartid, p_subreq) ;
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
					  (newdate, in_fname, in_fversion, row.mediatype, in_userid, in_format, row.algo, row.id, in_orderid, p_cartid, p_subreq) ;
			END LOOP ;
		END IF ;
	END IF ;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;


-----------------------------------
--- P_DISTRIBUTE_ITEM_ON_DEMAND ---
-----------------------------------
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
							  	  dp.filetype = in_filetype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = in_filetype) AND
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
				  	  	  	  	  dp.filetype = in_filetype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = in_filetype) AND
					  	  	  	  (dp.ordertype=in_ordertype OR dp.ordertype IS NULL) AND
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
							  	  dp.filetype = in_filetype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = in_filetype) AND
								  dp.format=in_format AND
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
							  	  dp.filetype = in_filetype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = in_filetype) AND
								  dp.format=in_format AND
								  (dp.ordertype=in_ordertype OR dp.ordertype IS NULL) AND
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


---------------------------------------
--- P_DISTRIBUTE_STANDARD_INVENTORY ---
---------------------------------------
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
					  WHERE dp.filetype = ftype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = ftype)   
							AND dp.active='t' 
							AND dp.ordertype IS NULL) LOOP

  	  	  	err := p_distribute_item(in_fname, in_fversion, ftype, row.userid, NULL, row.format, in_orderid) ;

		END LOOP ;

  	END IF ;
	
	RETURN 0;	

END ;
$$ LANGUAGE plpgsql ;



------------------------------------
--- P_DISTRIBUTE_ORDER_INVENTORY ---
------------------------------------
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
								  	WHERE dp.filetype = row_inv.filetype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = row_inv.filetype) AND
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
								  	WHERE dp.filetype = row_inv.filetype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = row_inv.filetype) AND
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
								  	WHERE dp.filetype = row_inv.filetype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = row_inv.filetype) AND
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
								  	WHERE dp.filetype = row_inv.filetype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = row_inv.filetype) AND
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
		'8.0.0',
		'2009-06-12',
		3,
		1,
		'SchemaVersion_8-0-0',
		'Added tables t_usermodes and t_storagegroup. Modified tables t_users, t_receptionpolicies and t_distributionpolicies. Modified procedures p_distribute_item, p_distribute_item_on_demand, p_distribute_standard_inventory and p_distribute_order_inventory.'
) ;
