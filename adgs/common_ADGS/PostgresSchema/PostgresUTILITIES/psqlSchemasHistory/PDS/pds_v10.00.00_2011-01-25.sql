
-- Version 10.0.0
-- POSTGRES








---------------------------------------------------------------------------------
-------------------------------- TABLES CREATION --------------------------------
---------------------------------------------------------------------------------

------------------------------
-- t_dbparametertypes
------------------------------
CREATE TABLE t_dbparametertypes (
	id INT2 NOT NULL,
	parametertype VARCHAR(16) NOT NULL
) ;

ALTER TABLE t_dbparametertypes 
	ADD CONSTRAINT pk_t_dbparametertypes
		PRIMARY KEY (id) ;

ALTER TABLE t_dbparametertypes 
	ADD CONSTRAINT uq_dbparametertypes01
		UNIQUE (parametertype) ;

INSERT INTO t_dbparametertypes (id, parametertype) VALUES (1, 'INTEGER') ;
INSERT INTO t_dbparametertypes (id, parametertype) VALUES (2, 'STRING') ;
INSERT INTO t_dbparametertypes (id, parametertype) VALUES (3, 'DATETIME') ;
INSERT INTO t_dbparametertypes (id, parametertype) VALUES (4, 'BOOLEAN') ;



------------------------------
-- t_dbparameters
------------------------------
CREATE TABLE t_dbparameters (
	id SERIAL NOT NULL,
	parameter VARCHAR(32) NOT NULL,
	parametertype_id INT2 NOT NULL,
	value_int INTEGER,
	value_str VARCHAR(32),
	value_dttm TIMESTAMP(6),
	value_bool BOOLEAN
)  ;

ALTER TABLE t_dbparameters 
	ADD CONSTRAINT pk_t_dbparameters
		PRIMARY KEY (id) ;

ALTER TABLE t_dbparameters 
	ADD CONSTRAINT uq_dbparameters01
		UNIQUE (parameter) ;

ALTER TABLE t_dbparameters 
	ADD CONSTRAINT fk_dbparameters01 
		FOREIGN KEY (parametertype_id) 
			REFERENCES t_dbparametertypes (id) 
				ON DELETE CASCADE ;



------------------------------
-- t_stati
------------------------------
CREATE TABLE t_stati (
	id INT2 NOT NULL,
	status VARCHAR(32) NOT NULL
) ;

ALTER TABLE t_stati 
	ADD CONSTRAINT pk_t_stati
		PRIMARY KEY (id) ;

ALTER TABLE t_stati 
	ADD CONSTRAINT uq_stati01
		UNIQUE (status) ;

INSERT INTO t_stati (id, status) VALUES (1,  'ABORTED') ;
INSERT INTO t_stati (id, status) VALUES (2,  'ACKNOWLEDGED') ;
INSERT INTO t_stati (id, status) VALUES (3,  'COMPLETED') ;
INSERT INTO t_stati (id, status) VALUES (4,  'DISTRIBUTION') ;
INSERT INTO t_stati (id, status) VALUES (5,  'DOWNLOADING') ;
INSERT INTO t_stati (id, status) VALUES (6,  'DOWNLOADITEM') ;
INSERT INTO t_stati (id, status) VALUES (7,  'ELIGIBLE') ;
INSERT INTO t_stati (id, status) VALUES (8,  'ERROR') ;
INSERT INTO t_stati (id, status) VALUES (9,  'FATALERROR') ;
INSERT INTO t_stati (id, status) VALUES (10, 'PRECONDITIONCHECK') ;
INSERT INTO t_stati (id, status) VALUES (11, 'PREPARING') ;
INSERT INTO t_stati (id, status) VALUES (12, 'PROCESSED') ;
INSERT INTO t_stati (id, status) VALUES (13, 'PROCESSING') ;
INSERT INTO t_stati (id, status) VALUES (14, 'PROGRESS') ;
INSERT INTO t_stati (id, status) VALUES (15, 'RECOVERABLEERROR') ;
INSERT INTO t_stati (id, status) VALUES (16, 'STANDBY') ;
INSERT INTO t_stati (id, status) VALUES (17, 'SUBMITTED') ;
INSERT INTO t_stati (id, status) VALUES (18, 'SUSPENDED') ;
INSERT INTO t_stati (id, status) VALUES (19, 'TRANSIENT') ;
INSERT INTO t_stati (id, status) VALUES (20, 'TRANSFERRING') ;
INSERT INTO t_stati (id, status) VALUES (21, 'UPLOADED') ;
INSERT INTO t_stati (id, status) VALUES (22, 'UPLOADING') ;
INSERT INTO t_stati (id, status) VALUES (23, 'UPLOADITEM') ;
INSERT INTO t_stati (id, status) VALUES (25, 'DELETED') ;
INSERT INTO t_stati (id, status) VALUES (26, 'DELETING') ;



------------------------------
-- t_transferprotocols
------------------------------
CREATE TABLE t_transferprotocols (
	id INT2 NOT NULL,
	protocol VARCHAR(16) NOT NULL
) ;

ALTER TABLE t_transferprotocols 
	ADD CONSTRAINT pk_t_transferprotocols
		PRIMARY KEY (id);

ALTER TABLE t_transferprotocols 
	ADD CONSTRAINT uq_transferprotocols01
		UNIQUE (protocol);

INSERT INTO t_transferprotocols (id, protocol) VALUES (1, 'FTP') ;
INSERT INTO t_transferprotocols (id, protocol) VALUES (2, 'FTPS') ;
INSERT INTO t_transferprotocols (id, protocol) VALUES (3, 'SFTP') ;



/* ------------------------------
	t_discacheinvrelation
*/ ------------------------------
CREATE TABLE t_discacheinvrelation (
    id		INTEGER NOT NULL,
    path	VARCHAR(512) NOT NULL
) ;

ALTER TABLE t_discacheinvrelation
	ADD CONSTRAINT pk_t_discacheinvrelation
		PRIMARY KEY (id) ;

ALTER TABLE t_discacheinvrelation
  ADD CONSTRAINT fk_t_discacheinvrelation01
  	FOREIGN KEY (id)
  	  REFERENCES t_inventorysm (id)
  	  	ON DELETE CASCADE ;








---------------------------------------------------------------------------------
------------------------------ TABLES MODIFICATION ------------------------------
---------------------------------------------------------------------------------

------------------------------
-- t_filetypescomp
------------------------------
DELETE FROM t_filetypescomp WHERE cfiletype NOT IN (SELECT filetype FROM t_filetypes);
DELETE FROM t_filetypescomp WHERE pfiletype NOT IN (SELECT filetype FROM t_filetypes);

ALTER TABLE t_filetypescomp 
	ADD CONSTRAINT fk_filetypescomp01
		FOREIGN KEY (pfiletype) 
			REFERENCES t_filetypes (filetype) 
				ON DELETE CASCADE ;

ALTER TABLE t_filetypescomp 
	ADD CONSTRAINT fk_filetypescomp02
		FOREIGN KEY (cfiletype) 
			REFERENCES t_filetypes (filetype) 
				ON DELETE CASCADE ;

------------------------------
-- t_iorepository
------------------------------
ALTER TABLE t_iorepository ADD passive BOOLEAN DEFAULT 't' NOT NULL;

------------------------------
-- t_inventory
------------------------------
ALTER TABLE t_inventory ADD dir_id BIGINT;
ALTER TABLE t_inventory ADD ref_dir_id BIGINT;
ALTER TABLE t_inventory ALTER COLUMN fileversion TYPE VARCHAR(32);
CREATE INDEX ix_t_inventory01 ON t_inventory USING btree ("dir_id") ;
CREATE INDEX ix_t_inventory02 ON t_inventory USING btree ("ref_dir_id") ;
CREATE INDEX ix_t_inventory03 ON t_inventory USING btree ("orderid") ;

------------------------------
-- t_distributionitems
------------------------------
ALTER TABLE t_distributionitems ALTER COLUMN fileversion TYPE VARCHAR(32);

------------------------------
-- t_inventorycomments
------------------------------
ALTER TABLE t_inventorycomments ALTER COLUMN fileversion TYPE VARCHAR(32);

------------------------------
-- t_invspecialrollingpolicies
------------------------------
ALTER TABLE t_invspecialrollingpolicies ALTER COLUMN inventoryversion TYPE VARCHAR(32);

------------------------------
-- t_temporaryorders
------------------------------
ALTER TABLE t_temporaryorders ALTER COLUMN fileversion TYPE VARCHAR(32);

------------------------------
-- t_inventoryevents
------------------------------
ALTER TABLE t_inventoryevents ALTER COLUMN fileversion TYPE VARCHAR(32);

------------------------------
-- t_ordersinput
------------------------------
ALTER TABLE t_ordersinput ALTER COLUMN fileversion TYPE VARCHAR(32);

------------------------------
-- t_inventorysm
------------------------------
ALTER TABLE t_inventorysm ALTER COLUMN fileversion TYPE VARCHAR(32);

------------------------------
-- t_inventorygaps
------------------------------
ALTER TABLE t_inventorygaps ALTER COLUMN fileversion TYPE VARCHAR(32);

------------------------------
-- t_pendinginventory
------------------------------
ALTER TABLE t_pendinginventory ALTER COLUMN fileversion TYPE VARCHAR(32);

------------------------------
-- t_inventorydataset
------------------------------
ALTER TABLE t_inventorydataset ALTER COLUMN fileversion TYPE VARCHAR(32);

------------------------------
-- t_temporarydistribution
------------------------------
ALTER TABLE t_temporarydistribution ALTER COLUMN fileversion TYPE VARCHAR(32);

------------------------------
-- t_ordtriggeringfiles
------------------------------
ALTER TABLE t_ordtriggeringfiles ALTER COLUMN fileversion TYPE VARCHAR(32);

------------------------------
-- t_invsiterelation
------------------------------
ALTER TABLE t_invsiterelation ALTER COLUMN fileversion TYPE VARCHAR(32);

------------------------------
-- t_smactions
------------------------------
ALTER TABLE t_smactions ALTER COLUMN fileversion TYPE VARCHAR(32);

------------------------------
-- t_cartsinv
------------------------------
ALTER TABLE t_cartsinv ALTER COLUMN filename DROP NOT NULL;
ALTER TABLE t_cartsinv ALTER COLUMN fileversion DROP NOT NULL;
ALTER TABLE t_cartsinv ALTER COLUMN fileversion TYPE VARCHAR(32);

------------------------------
-- t_inventoryanomalies
------------------------------
ALTER TABLE t_inventoryanomalies ALTER COLUMN fileversion TYPE VARCHAR(32);

------------------------------
-- t_postinvqueue
------------------------------
ALTER TABLE t_postinvqueue ALTER COLUMN fileversion TYPE VARCHAR(32);

------------------------------
-- t_ordersqueue
------------------------------
ALTER TABLE t_ordersqueue ALTER COLUMN jobresponsible TYPE VARCHAR(64);

------------------------------
-- t_distributionqueue
------------------------------
DROP VIEW v_distribution;
ALTER TABLE t_distributionqueue ALTER COLUMN jobresponsible TYPE VARCHAR(64);
CREATE VIEW v_distribution (
	"packageid", 
	"status", 
	"distributeddate", 
	"jobresponsible", 
	"creationdate", 
	"mediatype", 
	"userid")
AS SELECT DISTINCT 
	x0.packageid, 
	x0.status, 
	x0.distributeddate, 
	x0.jobresponsible, 
	x0.creationdate, 
	x1.mediatype, 
	x1.userid
FROM 
	t_distributionqueue x0, 
	t_distributionitems x1
WHERE 
	(x0.packageid = x1.packageid) 
;








---------------------------------------------------------------------------------
--------------------------------- TABLES UPDATE ---------------------------------
---------------------------------------------------------------------------------

------------------------------
-- t_rollingpolicyactions
------------------------------
UPDATE t_rollingpolicyactions SET action = 'CLEANSTORAGE' WHERE action = 'INVALIDATE';
UPDATE t_rollingpolicyactions SET action = 'INVALIDATE' WHERE action = 'REMOVE';
INSERT INTO t_rollingpolicyactions (id, action) VALUES (4, 'REMOVE');

------------------------------
-- t_originators
------------------------------
INSERT INTO t_originators VALUES (40, 'OCADA-EDITOR');

------------------------------
-- t_satellites
------------------------------
INSERT INTO t_satellites VALUES(34,'Sentinel-1A', 'S1A') ;
INSERT INTO t_satellites VALUES(35,'Sentinel-1B', 'S1B') ;
INSERT INTO t_satellites VALUES(36,'Sentinel-2A', 'S2A') ;
INSERT INTO t_satellites VALUES(37,'Sentinel-2B', 'S2B') ;
INSERT INTO t_satellites VALUES(38,'Sentinel-3A', 'S3A') ;
INSERT INTO t_satellites VALUES(39,'Sentinel-3B', 'S3B') ;

------------------------------
-- t_sensros
------------------------------
INSERT INTO t_sensors VALUES (66,'MSI', 'Multi-Spectral Instrument');
INSERT INTO t_sensors VALUES (67,'OLCI', 'Ocean & Land Color Instrument');
INSERT INTO t_sensors VALUES (68,'SLSTR', 'Sea and Land Surface Temperature Radiometer');
INSERT INTO t_sensors VALUES (69,'SRAL', 'SAR Radar Altimeter');
INSERT INTO t_sensors VALUES (70,'MWR', 'MicroWave Radiometer');

UPDATE t_sensors SET description = 'MultiSpectral Scanner' WHERE sensorname = 'MSS';
UPDATE t_sensors SET description = 'Thematic Mapper' WHERE sensorname = 'TM';
UPDATE t_sensors SET description = 'Enhanced Thematic Mapper' WHERE sensorname = 'ETM';
UPDATE t_sensors SET description = 'Return Beam Vidicon' WHERE sensorname = 'RBV';
UPDATE t_sensors SET description = 'Multispectral Electronic Self-Scanning Radiometer' WHERE sensorname = 'MESSR';
UPDATE t_sensors SET description = 'Very Near Infrared Radiometer' WHERE sensorname = 'VNIR';
UPDATE t_sensors SET description = 'Short Wave Infrared Radiometer' WHERE sensorname = 'SWIR';

------------------------------
-- t_satrelations
------------------------------
----- SENTINEL -----
-- Sentinel-1A 1
INSERT INTO t_satrelations VALUES (34, 1, 18) ;
-- Sentinel-1B 1
INSERT INTO t_satrelations VALUES (35, 1, 18) ;
-- Sentinel-2A 1
INSERT INTO t_satrelations VALUES (36, 1, 66) ;
-- Sentinel-2B 1
INSERT INTO t_satrelations VALUES (37, 1, 66) ;
-- Sentinel-3A 1
INSERT INTO t_satrelations VALUES (38, 1, 67) ;
INSERT INTO t_satrelations VALUES (38, 1, 68) ;
INSERT INTO t_satrelations VALUES (38, 1, 69) ;
INSERT INTO t_satrelations VALUES (38, 1, 70) ;
-- Sentinel-3B 1
INSERT INTO t_satrelations VALUES (39, 1, 67) ;
INSERT INTO t_satrelations VALUES (39, 1, 68) ;
INSERT INTO t_satrelations VALUES (39, 1, 69) ;
INSERT INTO t_satrelations VALUES (39, 1, 70) ;








---------------------------------------------------------------------------------
---------------------------- PROCEDURES MODIFICATION ----------------------------
---------------------------------------------------------------------------------


/* ------------------------------
	P_DISTRIBUTE_ITEM
*/ ------------------------------
DROP FUNCTION p_distribute_item(character varying, character varying, character varying, character varying, character varying, character varying, integer) ;
CREATE OR REPLACE FUNCTION p_distribute_item(
	in_fname VARCHAR(128),
	in_fversion VARCHAR(32),
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
DROP FUNCTION p_distribute_item_wdate(character varying, character varying, character varying, character varying, character varying, character varying, integer, smallint);
CREATE OR REPLACE FUNCTION p_distribute_item_wdate(
	in_fname VARCHAR(128), 
	in_fversion VARCHAR(32), 
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
DROP FUNCTION p_distribute_item_on_demand(character varying, character varying, character varying, character varying, character varying, character varying, integer);
CREATE OR REPLACE FUNCTION p_distribute_item_on_demand(
	in_fname VARCHAR(128),
	in_fversion VARCHAR(32),
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
	P_DISTRIBUTE_STANDARD_INVENTORY
*/ ------------------------------
DROP FUNCTION p_distribute_standard_inventory(character varying, character varying, integer, character varying);
CREATE FUNCTION p_distribute_standard_inventory(
	in_fname VARCHAR(128),
	in_fversion VARCHAR(32),
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



/* ------------------------------
	P_DISTRIBUTE_ORDER_INVENTORY
*/ ------------------------------
DROP FUNCTION p_distribute_order_inventory(integer, character varying, character varying);
CREATE FUNCTION p_distribute_order_inventory(
	in_orderid INTEGER, 
	in_ordertype VARCHAR(32), 
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
	P_INSERTINVENTORY
*/ ------------------------------
DROP FUNCTION p_insertinventory(character varying, character varying, boolean, character varying, character varying, integer, smallint, smallint, smallint, timestamp without time zone, timestamp without time zone);
CREATE FUNCTION p_insertinventory(
	fname VARCHAR(128), 
	fversion VARCHAR(32),
	valflag BOOLEAN,
	ftype VARCHAR(16),
	fclass VARCHAR(8),
	orderid INTEGER,
	in_satellite SMALLINT, 
	in_sensor SMALLINT, 
	in_mission SMALLINT,
	valstart TIMESTAMP(6),
	valstop TIMESTAMP(6)) RETURNS INTEGER AS $$
	
DECLARE
  err INTEGER ;
  
BEGIN

  	err := p_checksatsensmiss(in_satellite,in_sensor,in_mission) ;

  	INSERT INTO t_pendinginventory ("filename", "fileversion") VALUES (fname, fversion) ;

  	IF valflag = 't' THEN
  	  	err := p_processinventory(fname, fversion, valflag, ftype, fclass, orderid, in_satellite, valstart, valstop) ;
  	END IF ;

-----------------------------------------------------------------------------
-- BROADCAST SECTION
-----------------------------------------------------------------------------

  	err := p_inserteventtableitem('t_inventory' ,'INS') ;
	err := p_inserteventtableitem(ftype, 'INSFTYPE') ;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	P_GENERATETMPORDER
*/ ------------------------------
DROP FUNCTION p_generatetmporder(boolean, character varying, character varying, character varying, character varying, smallint);
CREATE FUNCTION p_generatetmporder(
	in_active BOOLEAN,
	fname VARCHAR(128),
	fversion VARCHAR(32),
	ftype VARCHAR(16),
	fclass VARCHAR(8),
	in_satellite SMALLINT) RETURNS INTEGER AS $$
	
DECLARE
  	row         RECORD ;
  	satrow      RECORD ;
  	rec         RECORD ;
	satrowcount SMALLINT ;
	
BEGIN

-- TEMPORARY ORDER GENERATION SECTION

-- 		the matching rows are those having:
-- 		fileclass null or fileclass equal to the input one
-- 		same filetype of the input one and 
-- 		active flag equal to the input (in_active var) one

  	FOR row IN (SELECT og.id, og.orderType, og.processorName,
  	  	  	  	  	   og.processorVersion, og.algorithm, og.processorconfiguration,
  	  	  	  	  	   og.priority, og.troubleshooting, og.processingtimewindow 
  	  	  	  	  FROM t_ordersgeneration og
					  	WHERE og.filetype=ftype AND
						  	  og.active = in_active AND
							  (og.fileclass IS NULL OR og.fileclass = fclass)) LOOP

-- 		Check now if there are rows to map input satellite into output satellite related to the 
--		specific order generation entry (having same ogid).
--		There can be the following cases:
--		1 - There are no row.
--		    -> Emit only one temporary order with the same satellite id copied from the inventory item.
--		2 - There are one ore more row(s) (N) with: same satelliteid as the inventory.
--		    -> Emit N temporary order(s) copyng the satellite from the order satellite of each row.
--
--		Note that the inventory satellite id, as well order sat, can be NULL

  	  	SELECT INTO rec COUNT(*) FROM t_ordergenerationsatmatrix WHERE "ordergenerationid" = row.id ;
  	  	satrowcount := rec.count;

  	  	IF satrowcount > 0 THEN
-- 			Case 2, splitted because input satellite can be null

  	  	  	IF in_satellite IS NOT NULL THEN

  	  	  	  	FOR satrow IN (SELECT ogsm.ordersat 
  	  	  	  	  	  	  	  	 FROM t_ordersgenerationsatmatrix ogsm
  	  	  	  	  	  	  	  	   WHERE ogsm.ordergenerationid=row.id AND ogsm.inventorysat=in_satellite) LOOP
  	  	  	  	  	INSERT INTO t_temporaryorders
  	  	  	  	  	  ("filename", "fileversion", "filetype", "ordertype", "processorname",
					   "processorversion", "algorithm", "processorconfiguration", "priority",
					   "troubleshooting", "processingtimewindow", "ordersat")
  	  	  	  	  	VALUES
					  (fname, fversion, ftype, row.orderType, row.processorname,
					   row.processorversion, row.algorithm, row.processorconfiguration, row.priority,
					   row.troubleshooting, row.processingtimewindow, satrow.ordersat) ;
  	  	  	  	END LOOP;

  	  	  	ELSE

  	  	  	  	FOR satrow IN (SELECT ogsm.ordersat 
  	  	  	  	  	  	  	  	 FROM t_ordersgenerationsatmatrix ogsm
  	  	  	  	  	  	  	  	   WHERE ogsm.ordergenerationid=row.id AND ogsm.inventorysat IS NULL) LOOP
  	  	  	  	  	INSERT INTO t_temporaryorders
  	  	  	  	  	  ("filename", "fileversion", "filetype", "ordertype", "processorname",
					   "processorversion", "algorithm", "processorconfiguration", "priority",
					   "troubleshooting", "processingtimewindow", "ordersat")
  	  	  	  	  	VALUES
					  (fname, fversion, ftype, row.orderType, row.processorName,
					   row.processorVersion, row.algorithm, row.processorconfiguration, row.priority,
					   row.troubleshooting, row.processingtimewindow, satrow.ordersat) ;
  	  	  	  	END LOOP;

  	  	  	END IF ;

  	  	ELSE
--			Case 1: only one order no splitted because no where clause on input satellite

  	  	  	INSERT INTO t_temporaryorders 
  	  	  	  ("filename", "fileversion", "filetype", "ordertype", "processorname",
			   "processorversion", "algorithm", "processorconfiguration", "priority",
  	  	  	   "troubleshooting", "processingtimewindow", "ordersat")
  	  	  	VALUES
			  (fname, fversion, ftype, row.orderType, row.processorName,
			   row.processorVersion, row.algorithm, row.processorconfiguration, row.priority,
			   row.troubleshooting, row.processingtimewindow, in_satellite) ;
		END IF ;
	END LOOP ;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	P_GENERATETMPORDERPNAME
*/ ------------------------------
DROP FUNCTION p_generatetmporderpname(boolean, character varying, character varying, character varying, character varying, smallint);
CREATE FUNCTION p_generatetmporderpname(
	in_active BOOLEAN,
	fname VARCHAR(128),
	fversion VARCHAR(32),
	ftype VARCHAR(16),
	fclass VARCHAR(8),
	in_satellite SMALLINT) RETURNS INTEGER AS $$
	
DECLARE
  	row         RECORD ;
  	satrow      RECORD ;
  	rec         RECORD ;
	satrowcount SMALLINT ;
	
BEGIN

-- TEMPORARY ORDER GENERATION SECTION

-- 		the matching rows are those having:
-- 		fileclass null or fileclass equal to the input one
-- 		same filetype of the input one and 
-- 		active flag equal to the input (in_active var) one

  	FOR row IN (SELECT og.id, og.orderType, og.processorName,
  	  	  	  	  	   og.processorVersion, og.algorithm, og.processorconfiguration,
  	  	  	  	  	   og.priority, og.troubleshooting, og.processingtimewindow 
  	  	  	  	  FROM t_ordersgeneration og
					  	WHERE og.filetype=ftype 
								AND og.active = in_active 
								AND (og.fileclass IS NULL OR og.fileclass = fclass)
								AND og.processorname IN (SELECT pname FROM tempPnames)) LOOP

-- 		Check now if there are rows to map input satellite into output satellite related to the 
--		specific order generation entry (having same ogid).
--		There can be the following cases:
--		1 - There are no row.
--		    -> Emit only one temporary order with the same satellite id copied from the inventory item.
--		2 - There are one ore more row(s) (N) with: same satelliteid as the inventory.
--		    -> Emit N temporary order(s) copyng the satellite from the order satellite of each row.
--
--		Note that the inventory satellite id, as well order sat, can be NULL

  	  	SELECT INTO rec COUNT(*) FROM t_ordergenerationsatmatrix WHERE "ordergenerationid" = row.id ;
  	  	satrowcount := rec.count;

  	  	IF satrowcount > 0 THEN
-- 			Case 2, splitted because input satellite can be null

  	  	  	IF in_satellite IS NOT NULL THEN

  	  	  	  	FOR satrow IN (SELECT ogsm.ordersat 
  	  	  	  	  	  	  	  	 FROM t_ordersgenerationsatmatrix ogsm
  	  	  	  	  	  	  	  	   WHERE ogsm.ordergenerationid=row.id AND ogsm.inventorysat=in_satellite) LOOP
  	  	  	  	  	INSERT INTO t_temporaryorders
  	  	  	  	  	  ("filename", "fileversion", "filetype", "ordertype", "processorname",
					   "processorversion", "algorithm", "processorconfiguration", "priority",
					   "troubleshooting", "processingtimewindow", "ordersat")
  	  	  	  	  	VALUES
					  (fname, fversion, ftype, row.orderType, row.processorName,
					   row.processorVersion, row.algorithm, row.processorconfiguration, row.priority,
					   row.troubleshooting, row.processingtimewindow, satrow.ordersat) ;
  	  	  	  	END LOOP;

  	  	  	ELSE

  	  	  	  	FOR satrow IN (SELECT ogsm.ordersat 
  	  	  	  	  	  	  	  	 FROM t_ordersgenerationsatmatrix ogsm
  	  	  	  	  	  	  	  	   WHERE ogsm.ordergenerationid=row.id AND ogsm.inventorysat IS NULL) LOOP
  	  	  	  	  	INSERT INTO t_temporaryorders
  	  	  	  	  	  ("filename", "fileversion", "filetype", "ordertype", "processorname",
					   "processorversion", "algorithm", "processorconfiguration", "priority",
					   "troubleshooting", "processingtimewindow", "ordersat")
  	  	  	  	  	VALUES
					  (fname, fversion, ftype, row.orderType, row.processorname,
					   row.processorversion, row.algorithm, row.processorconfiguration, row.priority,
					   row.troubleshooting, row.processingtimewindow, satrow.ordersat) ;
  	  	  	  	END LOOP;

  	  	  	END IF ;

  	  	ELSE
--			Case 1: only one order no splitted because no where clause on input satellite

  	  	  	INSERT INTO t_temporaryorders 
  	  	  	  ("filename", "fileversion", "filetype", "ordertype", "processorname",
			   "processorversion", "algorithm", "processorconfiguration", "priority",
  	  	  	   "troubleshooting", "processingtimewindow", "ordersat")
  	  	  	VALUES
			  (fname, fversion, ftype, row.orderType, row.processorName,
			   row.processorVersion, row.algorithm, row.processorconfiguration, row.priority,
			   row.troubleshooting, row.processingtimewindow, in_satellite) ;
		END IF ;
	END LOOP ;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	P_PROCESSINVENTORY
*/ ------------------------------
DROP FUNCTION p_processinventory(character varying, character varying, boolean, character varying, character varying, integer, smallint, timestamp without time zone, timestamp without time zone);
CREATE FUNCTION p_processinventory(
	fname VARCHAR(128), 
	fversion VARCHAR(32),
	valflag BOOLEAN,
	ftype VARCHAR(16),
	fclass VARCHAR(8),
	orderid INTEGER,
	in_satellite SMALLINT, 
	valstart TIMESTAMP(6),
	valstop TIMESTAMP(6)
) RETURNS INTEGER AS $$

DECLARE
  	rec        RECORD ;
  	pendingcnt SMALLINT ;
	err   	   INTEGER ;
	
BEGIN

	---------------------------------------------------------------------------------
	-- The procedure assumes the triplet satellite/sensor/mission is already verified
	---------------------------------------------------------------------------------

	SELECT INTO rec COUNT(*) FROM t_pendinginventory p WHERE p.filename=fname AND p.fileversion=fversion ;
  	pendingcnt := rec.count;

  	-- return without processing if the inventory was not pending
  	IF pendingcnt = 0 THEN
  	  	RETURN 0 ;
  	END IF ;

  	-- remove pending so nobody will try the same
  	DELETE FROM t_pendinginventory WHERE "filename"=fname AND "fileversion"=fversion ;

	-----------------------------------------------------------------------------
	-- TEMPORARY ORDER GENERATION SECTION
	-----------------------------------------------------------------------------
	err := p_generateTmpOrder('t', fname, fversion, ftype, fclass, in_satellite) ;

	-----------------------------------------------------------------------------
	-- DISTRIBUTION SECTION
	-----------------------------------------------------------------------------
	err :=  p_distribute_standard_inventory(fname, fversion, orderid, ftype) ;

	-----------------------------------------------------------------------------
	-- ORDER'S BLACK LIST SECTION
	-----------------------------------------------------------------------------
	err :=  p_purgewaitingorders(ftype, valstart, valstop) ;

	-----------------------------------------------------------------------------
	-- INVENTORY LINKS SECTION
	-----------------------------------------------------------------------------
	err := p_inventorylinks_check(fname, fversion, ftype, orderid) ;

RETURN 0;

END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	P_INVENTORY_UPDTAE
*/ ------------------------------
DROP FUNCTION p_inventory_update(boolean, character varying, character varying, boolean, character varying, character varying, integer, smallint, smallint, smallint, timestamp without time zone, timestamp without time zone);
CREATE FUNCTION p_inventory_update(
	prevalflag BOOLEAN,
	fname VARCHAR(128), 
	fversion VARCHAR(32),
	valflag BOOLEAN,
	ftype VARCHAR(16),
	fclass VARCHAR(8),
	orderid INTEGER,
	in_satellite SMALLINT, 
	in_sensor SMALLINT, 
	in_mission SMALLINT,
	valstart TIMESTAMP(6),
	valstop TIMESTAMP(6)) RETURNS INTEGER AS $$
	
DECLARE
  err INTEGER ;
  
BEGIN

  	err := p_checksatsensmiss(in_satellite, in_sensor, in_mission) ;

  	IF (prevalflag <> valflag AND valflag = 't') THEN 
  	  	err := p_processinventory(fname, fversion, valflag, ftype, fclass, orderid, in_satellite, valstart, valstop) ;
  	  	err := p_inserteventtableitem('t_inventory', 'VALCHANGE') ;
  	END IF ;

  	err := p_inserteventtableitem('t_inventory', 'UPD') ;
  	err := p_inserteventtableitem(ftype, 'UPDFTYPE') ;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	P_INSERT_INVENTORYEVENTS
*/ ------------------------------
DROP FUNCTION p_insert_inventoryevents(character varying, character varying, boolean, boolean);
CREATE FUNCTION p_insert_inventoryevents(
	p_name VARCHAR(128), 
	p_version VARCHAR(32),
	pre_flag BOOLEAN,
	post_flag BOOLEAN) RETURNS INTEGER AS $$
	
DECLARE 
	p_event VARCHAR(16);

BEGIN

--  NOT NEEDED IN THIS SCHEMA
--  To implement this procedure install USDFWA schema.

	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	P_REMOVE_OBSOLETE_ACTIONS
*/ ------------------------------
DROP FUNCTION p_remove_obsolete_actions(character varying, character varying);
CREATE FUNCTION p_remove_obsolete_actions(
	fname VARCHAR(128), 
	fversion VARCHAR(32)) RETURNS INTEGER AS $$

DECLARE
	row        RECORD;
	rec        RECORD;
	pkg_id     INTEGER;
	p_status   VARCHAR(16);
	p_que_oper VARCHAR(16);

BEGIN

	FOR row IN
		(SELECT packageid 
		 FROM t_smactions
		 WHERE filename = fname AND fileversion = fversion AND packageid IS NOT NULL AND operation = 'UPLOAD') LOOP
		
		pkg_id := row.packageid ;
		
		SELECT INTO rec sq.status, sq.operation FROM t_smqueue AS sq WHERE sq.packageid = pkg_id ;
		
		p_status := rec.status ;
		p_que_oper := rec.operation ;
		
		IF p_status = 'ELIGIBLE' AND p_que_oper = 'UPLOAD' OR p_status = 'ERROR' AND p_que_oper = 'UPLOAD' THEN
			UPDATE t_smactions SET packageid = NULL WHERE packageid = pkg_id ;
			DELETE FROM t_smqueue WHERE packageid = pkg_id ;
		END IF ;
	END LOOP ;
	
	DELETE FROM t_smactions WHERE filename = fname AND fileversion = fversion AND operation = 'UPLOAD' ;
	
	RETURN 0 ;
END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	P_SMACTIONSUPDATE
*/ ------------------------------
DROP FUNCTION p_smactionsupdate(character varying, character varying, integer);
CREATE FUNCTION p_smactionsupdate(
	fname VARCHAR(128), 
	fversion VARCHAR(32), 
	pkid INT) RETURNS INTEGER AS $$
	
DECLARE 
	ftype VARCHAR(16) ;
	vstart TIMESTAMP(6);
	vstop TIMESTAMP(6);
	ret INTEGER;
	rec RECORD;
	
BEGIN

	IF (SELECT status FROM t_smqueue WHERE packageid = pkid) = 'COMPLETED' THEN
		
		SELECT INTO rec filetype, validitystart, validitystop FROM t_inventory WHERE filename = fname AND fileversion = fversion;
		ftype := rec.filetype ;
		vstart := rec.validitystart ;
		vstop := rec.validitystop ;
		
		ret := p_purgewaitingorders(ftype, vstart, vstop);
		
		RETURN ret;
		
	END IF;

	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	P_INSERT_TEMPORARY_DISTRIBUTION
*/ ------------------------------
DROP FUNCTION p_insert_temporary_distribution(timestamp without time zone, character varying, character varying, character varying, character varying, integer, integer, integer, integer);
CREATE FUNCTION p_insert_temporary_distribution(
	in_duedate TIMESTAMP(6),
	in_fname VARCHAR(128),
	in_fversion VARCHAR(32),
	in_mtype VARCHAR(16),
	in_format VARCHAR(16),
	in_algo INT,
	in_ordid INT,
	in_cartid INT,
	in_subreq INT)RETURNS INTEGER AS $$

BEGIN

	INSERT INTO "t_temporarydistribution" (
		"duedate",
		"filename",
		"fileversion",
		"mediatype",
		"userid",
		"format",
		"algo",
		"policy",
		"orderid",
		"cartid",
		"subreq" )
	VALUES (
		in_duedate,
		in_fname,
		in_fversion,
		in_mtype,
		'ANONYMOUS',
		in_format,
		in_algo,
		(SELECT max(id) FROM t_distributionpolicies WHERE userid = 'ANONYMOUS'),
		in_ordid,
		in_cartid,
		in_subreq
	) ;

	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	P_INVENTORYLINKS_CHECK
*/ ------------------------------
DROP FUNCTION p_inventorylinks_check(character varying, character varying, character varying, integer);
CREATE FUNCTION p_inventorylinks_check (
	fname VARCHAR(128), 
	fversion VARCHAR(32),
	ftype VARCHAR(16),
	ordid INTEGER
) RETURNS INTEGER AS $$

BEGIN
	
	--  NOT NEEDED IN THIS SCHEMA
	--  To implement this procedure install USDFWA 2.0 schema.
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	P_POSTINQUEUE_INSERT
*/ ------------------------------
DROP FUNCTION p_postinvqueue_insert(character varying, character varying);
CREATE FUNCTION p_postinvqueue_insert(
	in_fname VARCHAR(128), 
	in_fversion VARCHAR(32)) RETURNS INTEGER AS $$
	
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
	P_CARTS_UPDATE_STATUS
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_carts_update_status(in_cartid INT) RETURNS INTEGER AS $$

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
	
	-- ORDERS ABORTED
	IF (SELECT count(status) FROM t_ordersqueue WHERE externalorderid = p_extorderid AND subreq IS NOT NULL AND status = 'ABORTED') > 0 
	THEN
		UPDATE t_cartsref SET status = 'ABORTED' WHERE id = in_cartid ;
		RETURN 0;
	END IF;
	
	IF (ord_total = 0) THEN
	-- No orders, so act as every order has completed
		mid_status := 'COMPLETED' ;

	-- ORDERS ELIGIBLE
	ELSIF (SELECT count(status) FROM t_ordersqueue WHERE externalorderid = p_extorderid AND subreq IS NOT NULL AND status = 'ELIGIBLE') < ord_total 
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
------------------------------ FUNCTIONS CREATION -------------------------------
---------------------------------------------------------------------------------

------------------------------
-- f_delete_inventory_tree
------------------------------
CREATE OR REPLACE FUNCTION f_delete_inventory_tree(
	fname VARCHAR(128), 
	fversion VARCHAR(32))
RETURNS INTEGER AS $$

DECLARE
	res INT ;
	p_dirid INT8;
	
BEGIN
	res := 0 ;
	p_dirid := (SELECT dir_id FROM t_inventory WHERE filename = fname AND fileversion = fversion);
	
	IF ( p_dirid IS NULL)
	THEN
		DELETE FROM t_inventory WHERE filename = fname AND fileversion = fversion;
		GET DIAGNOSTICS res = ROW_COUNT;
	ELSE
		DELETE FROM t_inventory WHERE ref_dir_id = p_dirid OR dir_id = p_dirid;
		GET DIAGNOSTICS res = ROW_COUNT;
	END IF;

	RETURN res ;

END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
---------------------------- FUNCTIONS MODIFICATION -----------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	p_deleteinventory
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_deleteinventory(
	filename VARCHAR(128), 
	fileversion VARCHAR(32)) RETURNS INTEGER AS $$

BEGIN

  	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;







-- ############################################################################# --
-- ##################                                          ################# --
-- ##################                 WARNING!!!               ################# --
-- ##################  If USDFWA plugin is installed upgraded  ################# --
-- ##################       it to usdfwa-2.1.0 version         ################# --
-- ##################                                          ################# --
-- ############################################################################# --

-- ############################################################################# --
-- #################                                            ################ --
-- #################                  WARNING!!!                ################ --
-- #################  If BASELINE plugin is installed upgraded  ################ --
-- #################       it to baseline-1.1.0 version         ################ --
-- #################                                            ################ --
-- ############################################################################# --









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
		'pds-10.0.0',
		'2011-01-25',
		3,
		2,
		'PDS_10-0-0',
		'Added tables t_dbparametertypes, t_dbparameters, t_stati and t_transferprotocols. Added passive field on t_iorepository. Added new fields and changed fileversion size on t_inventory. Altered all table and procedures with fileversion reference. Added orders abortet on p_carts_update_status. Created function f_delete_inventory_tree.'
) ;
