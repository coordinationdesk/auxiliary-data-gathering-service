
-- Version 12.06.00
-- POSTGRES








--------------------------------------------------------------------------------
-------------------------------- TABLES CHANGES --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_inventory
*/ ------------------------------
ALTER TABLE t_inventory ALTER COLUMN qualityinfo TYPE VARCHAR(72) ;




--------------------------------------------------------------------------------
-------------------------------- INDEX CREATION --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_cartsinv
*/ ------------------------------
CREATE INDEX ix_t_cartsinv01 ON t_cartsinv USING BTREE (filename, fileversion) ;
CREATE INDEX ix_t_cartsinv02 ON t_cartsinv USING BTREE (cart) ;


/* ------------------------------
	t_filetypes_last_inv
*/ ------------------------------
CREATE INDEX ix_t_filetypes_last_inv01 ON t_filetypes_last_inv USING BTREE (filetype) ;


/* ------------------------------
	t_filetypes_x_satellites
*/ ------------------------------
CREATE INDEX ix_t_filetypes_x_satellites01 ON t_filetypes_x_satellites USING BTREE (filetype) ;
CREATE INDEX ix_t_filetypes_x_satellites02 ON t_filetypes_x_satellites USING BTREE (satellite_id) ;


/* --------------------------
	t_inventorydataset
*/ --------------------------
CREATE INDEX ix_t_inventorydataset02 ON t_inventorydataset USING BTREE (measdatasetid) ;


/* ------------------------------
	t_inventoryeventsconf
*/ ------------------------------
CREATE INDEX ix_t_inventoryeventsconf01 ON t_inventoryeventsconf USING BTREE (filetype) ;


/* ------------------------------
	t_invsiterelation
*/ ------------------------------
CREATE INDEX ix_t_invsiterelation02 ON t_invsiterelation USING BTREE (site) ;


/* ------------------------------
	t_ordersattachments
*/ ------------------------------
CREATE INDEX ix_t_ordersattachments02 ON t_ordersattachments USING BTREE (attachtype) ;


/* --------------------------
	t_sites
*/ --------------------------
CREATE INDEX ix_t_sites01 ON t_sites USING BTREE (parent) ;
CREATE INDEX ix_t_sites02 ON t_sites USING BTREE (type) ;


/* ------------------------------
	t_storagegroup
*/ ------------------------------
CREATE INDEX ix_t_storagegroup01 ON t_storagegroup USING BTREE (filetype) ;
CREATE INDEX ix_t_storagegroup02 ON t_storagegroup USING BTREE (storagemanager) ;








--------------------------------------------------------------------------------
--------------------------------- TABLE INSERT ---------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_satellites
*/ ------------------------------
INSERT INTO t_satellites (satelliteid, satellitename, satelliteacronym) VALUES (45, 'EarthCARE', 'ECA') ;

/* ------------------------------
	t_sensors
*/ ------------------------------
INSERT INTO t_sensors VALUES (78, 'ATLID', 'Atmospheric Lidar' );
INSERT INTO t_sensors VALUES (79, 'BBR', 'Broad Band Radiometer' );
INSERT INTO t_sensors VALUES (80, 'CPR', 'Cloud Profiling Radar' );


/* ------------------------------
	t_satrelations
*/ ------------------------------
----- EarthCARE -----
INSERT INTO t_satrelations VALUES (45, 1, 78) ;
INSERT INTO t_satrelations VALUES (45, 1, 79) ;
INSERT INTO t_satrelations VALUES (45, 1, 80) ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'pds-12.06.00' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-12.06.00',
		'2017-11-06',
		3,
		2,
		'PDS_12-6-0',
		'Added missing indexes on foreign keys. Added new EarthCARE-A satellite, sensors and relations'
) ;


