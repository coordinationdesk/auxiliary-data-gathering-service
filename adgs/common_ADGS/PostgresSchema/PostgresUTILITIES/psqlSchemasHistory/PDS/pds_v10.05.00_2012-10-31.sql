
-- Version 10.5.0
-- POSTGRES








---------------------------------------------------------------------------------
-------------------------------- TABLES CREATION --------------------------------
---------------------------------------------------------------------------------

/* --------------------------
	t_anomaly_types
*/ --------------------------
CREATE TABLE t_anomaly_types (
	id SMALLINT NOT NULL,
	anomaly_type VARCHAR(24) NOT NULL
) ;
ALTER TABLE t_anomaly_types 
	ADD CONSTRAINT pk_t_anomaly_types
		PRIMARY KEY (id) ;

ALTER TABLE t_anomaly_types 
	ADD CONSTRAINT uq_anomaly_types01
		UNIQUE (anomaly_type) ;

INSERT INTO t_anomaly_types (id, anomaly_type) VALUES (1, 'DELAY') ;
INSERT INTO t_anomaly_types (id, anomaly_type) VALUES (2, 'CORRUPTION') ;


/* --------------------------
	t_filetypes_last_inv
*/ --------------------------
CREATE TABLE t_filetypes_last_inv (
	id SERIAL NOT NULL,
	filetype VARCHAR(16) NOT NULL,
	last_valid TIMESTAMP(6) DEFAULT NOW() NOT NULL
) ;
ALTER TABLE t_filetypes_last_inv 
	ADD CONSTRAINT pk_t_filetypes_last_inv
		PRIMARY KEY (id) ;

ALTER TABLE t_filetypes_last_inv 
	ADD CONSTRAINT uq_filetypes_last_inv01
		UNIQUE (filetype) ;

ALTER TABLE t_filetypes_last_inv 
	ADD CONSTRAINT fk_filetypes_last_inv01 
		FOREIGN KEY (filetype) 
			REFERENCES t_filetypes (filetype) 
				ON DELETE CASCADE ;

INSERT INTO t_filetypes_last_inv (filetype) SELECT filetype FROM t_filetypes ;








---------------------------------------------------------------------------------
------------------------------ TABLES MODIFICATION ------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_inventoryanomalies
*/ ------------------------------
ALTER TABLE t_inventoryanomalies ADD anomaly_type_id SMALLINT DEFAULT 1 NOT NULL;

ALTER TABLE t_inventoryanomalies 
	ADD CONSTRAINT fk_inventoryanomalies01 
		FOREIGN KEY (anomaly_type_id) 
			REFERENCES t_anomaly_types (id) 
				ON DELETE CASCADE ;

CREATE INDEX ix_t_invanomalies01 ON t_inventoryanomalies USING BTREE (anomaly_type_id) ;


/* ------------------------------
	t_receptionpolicies
*/ ------------------------------
ALTER TABLE t_receptionpolicies DROP CONSTRAINT uq_receptpolicies01 ;

ALTER TABLE t_receptionpolicies 
	ADD CONSTRAINT uq_receptpolicies01
		UNIQUE (filetype, repository, userid, usermode);


/* --------------------------
	t_inventory
*/ --------------------------
ALTER TABLE t_inventory DROP CONSTRAINT fk_inventory_qualified ;
ALTER TABLE t_inventory
	ADD CONSTRAINT fk_inventory7
		FOREIGN KEY	(qualified)
			REFERENCES t_qualifiedflag (id)
				ON DELETE CASCADE ;

ALTER TABLE t_inventory DROP CONSTRAINT fk_inventory_band ;
ALTER TABLE t_inventory 
	ADD CONSTRAINT fk_inventory8
		FOREIGN KEY (band)
			REFERENCES t_bands (id) 
				ON DELETE CASCADE ;

CREATE INDEX ix_t_inventory05 ON t_inventory USING BTREE (fileclass) ;
CREATE INDEX ix_t_inventory06 ON t_inventory USING BTREE (sensorid) ;
CREATE INDEX ix_t_inventory07 ON t_inventory USING BTREE (satelliteid) ;
CREATE INDEX ix_t_inventory08 ON t_inventory USING BTREE (acquisitionstationid) ;
CREATE INDEX ix_t_inventory09 ON t_inventory USING BTREE (filetype) ;
CREATE INDEX ix_t_inventory10 ON t_inventory USING BTREE (processingstationid) ;
CREATE INDEX ix_t_inventory11 ON t_inventory USING BTREE (qualified) ;
CREATE INDEX ix_t_inventory12 ON t_inventory USING BTREE (band) ;


/* --------------------------
	t_ordersinput
*/ --------------------------
CREATE INDEX ix_t_ordersinput01 ON t_ordersinput USING BTREE (orderid) ;
CREATE INDEX ix_t_ordersinput02 ON t_ordersinput USING BTREE (filename, fileversion) ;


/* --------------------------
	t_ordersqueue
*/ --------------------------
DROP INDEX i_ordersqueuemission ;
DROP INDEX i_ordersqueuestatus ;
CREATE INDEX ix_t_ordersqueue01 ON t_ordersqueue USING BTREE (status) ;
CREATE INDEX ix_t_ordersqueue02 ON t_ordersqueue USING BTREE (ordertype) ;
CREATE INDEX ix_t_ordersqueue03 ON t_ordersqueue USING BTREE (satelliteid) ;
CREATE INDEX ix_t_ordersqueue04 ON t_ordersqueue USING BTREE (mission) ;
CREATE INDEX ix_t_ordersqueue05 ON t_ordersqueue USING BTREE (processorname, processorversion) ;


/* --------------------------
	t_distributionqueue
*/ --------------------------
ALTER TABLE t_distributionqueue DROP CONSTRAINT fk_t_distributionqueue ;
ALTER TABLE t_distributionqueue
	ADD CONSTRAINT fk_t_distributionqueue1
		FOREIGN KEY (format)
			REFERENCES t_distributionformat (format)
				ON DELETE CASCADE ;
DROP INDEX i_distributionqueuestatus ;
CREATE INDEX ix_t_distributionqueue01 ON t_distributionqueue USING BTREE (status) ;
CREATE INDEX ix_t_distributionqueue02 ON t_distributionqueue USING BTREE (format) ;


/* --------------------------
	t_distributionitems
*/ --------------------------
ALTER TABLE t_distributionitems DROP CONSTRAINT fk_distitem_cart ;
ALTER TABLE t_distributionitems 
	ADD CONSTRAINT fk_t_distributionitems6
		FOREIGN KEY (cartid)
			REFERENCES t_cartsref (id) 
				ON DELETE CASCADE ;
CREATE INDEX ix_t_distributionitems01 ON t_distributionitems USING BTREE (packageid) ;
CREATE INDEX ix_t_distributionitems02 ON t_distributionitems USING BTREE (filename, fileversion) ;
CREATE INDEX ix_t_distributionitems03 ON t_distributionitems USING BTREE (mediatype) ;
CREATE INDEX ix_t_distributionitems04 ON t_distributionitems USING BTREE (userid) ;
CREATE INDEX ix_t_distributionitems05 ON t_distributionitems USING BTREE (format) ;
CREATE INDEX ix_t_distributionitems06 ON t_distributionitems USING BTREE (cartid) ;








---------------------------------------------------------------------------------
------------------------------ PROCEDURES CREATION ------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	p_filetypes_last_inv_update
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_filetypes_last_inv_update(
	ftype VARCHAR(16)) RETURNS INTEGER AS $$

BEGIN
	UPDATE t_filetypes_last_inv SET last_valid = NOW() WHERE filetype = ftype ;	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
--------------------------- TRIGGER FUNCTIONS CREATION --------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	tf_filetype_insert
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_filetype_insert() RETURNS TRIGGER AS $$
DECLARE
  	status INTEGER ;
BEGIN
	INSERT INTO t_filetypes_last_inv (filetype) VALUES (NEW.filetype) ;
	status := p_inserteventtableitem('t_filetypes', 'INS') ;
  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
------------------------- TRIGGER FUNCTIONS MODIFICATION ------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	tf_inventoryinsevent
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_inventoryinsevent() RETURNS TRIGGER AS $$

DECLARE
  	status INTEGER ;
	
BEGIN
	status := p_insertinventory(NEW.filename, NEW.fileversion, NEW.validityflag, NEW.filetype, NEW.fileclass, NEW.orderid, NEW.satelliteid, NEW.sensorid, NEW.mission, NEW.validitystart, NEW.validitystop) ;
	
	IF NEW.validityflag = 't' THEN
		status := p_filetypes_last_inv_update(NEW.filetype) ;
	END IF;			
  	
  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;


/* ------------------------------
	tf_inventoryupdevent
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_inventoryupdevent() RETURNS TRIGGER AS $$

DECLARE
  	status INTEGER ;

BEGIN
	status := p_inventory_update(OLD.validityflag, NEW.filename, NEW.fileversion, NEW.validityflag, NEW.filetype, NEW.fileclass, NEW.orderid, NEW.satelliteid, NEW.sensorid, NEW.mission, NEW.validitystart, NEW.validitystop) ;
	
	status := p_insert_inventoryevents(NEW.filename, NEW.fileversion, OLD.validityflag, NEW.validityflag) ;
	
	IF ((NEW.validityflag != OLD.validityflag) AND (NEW.validityflag = 't')) THEN
		status := p_filetypes_last_inv_update(OLD.filetype) ;
	END IF;	
	
  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;






---------------------------------------------------------------------------------
----------------------------------- TRIGGERS ------------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_filetypes
*/ ------------------------------
DROP TRIGGER tr_filetypesinsert ON t_filetypes ;

CREATE TRIGGER tr_filetypesinsert AFTER INSERT
  ON t_filetypes 
  	FOR EACH ROW
  		EXECUTE PROCEDURE tf_filetype_insert() ;








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
		'pds-10.5.0',
		'2012-10-31',
		3,
		2,
		'PDS_10-5-0',
		'Added new tables t_anomaly_types and t_filetypes_last_inv. Added field anomaly_type_id on t_inventoryanomalies and changed unique constraint on t_receptionpolicies. Added procedure p_filetypes_last_inv_update. Added trigger function tf_filetype_insert, modified trigger functions on t_inventory and modified trigger tr_filetypesinsert.'
) ;
