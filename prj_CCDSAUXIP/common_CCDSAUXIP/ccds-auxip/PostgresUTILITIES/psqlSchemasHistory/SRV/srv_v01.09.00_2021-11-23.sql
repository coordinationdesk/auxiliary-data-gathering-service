
-- SRV Version 1.9.0
-- POSTGRES








--------------------------------------------------------------------------------
------------------------------ EXTENSION CREATION ------------------------------
--------------------------------------------------------------------------------
CREATE EXTENSION IF NOT EXISTS pg_trgm;








-------------------------------------------------------------------------------
------------------------------- TABLES CREATION -------------------------------
-------------------------------------------------------------------------------

/* ------------------------------
        t_srv_inventory_dwh
*/ ------------------------------
CREATE TABLE t_srv_inventory_dwh (
	id BIGINT,
	uuid VARCHAR(160),
	name VARCHAR(1024),
	contenttype VARCHAR(256),
	contentlength BIGINT,
	origindate TIMESTAMP(3),
	creationdate TIMESTAMP(3),
	modificationdate TIMESTAMP(3),
	checksum JSONB,
	validitystart TIMESTAMP(3),
	validitystop TIMESTAMP(3),
	footprint GEOGRAPHY(GEOMETRY,4326),
	validity BOOLEAN,
	filetype_id INTEGER,
	satellite_id SMALLINT,
	sensor_id SMALLINT,
	mission SMALLINT,
	evictiondate TIMESTAMP(3),
	productiontype VARCHAR(36),
	attributes JSONB,
	trace_id VARCHAR(160)
) ;
ALTER TABLE t_srv_inventory_dwh
	ADD CONSTRAINT pk_t_srv_inventory_dwh
		PRIMARY KEY (id) ;

CREATE INDEX ix_t_srv_inventory_dwh01 ON t_srv_inventory_dwh USING BTREE (uuid) ;
CREATE INDEX ix_t_srv_inventory_dwh02 ON t_srv_inventory_dwh USING GIN (name GIN_TRGM_OPS) ;
CREATE INDEX ix_t_srv_inventory_dwh03 ON t_srv_inventory_dwh USING BTREE (origindate) ;
CREATE INDEX ix_t_srv_inventory_dwh04 ON t_srv_inventory_dwh USING BTREE (creationdate) ;
CREATE INDEX ix_t_srv_inventory_dwh05 ON t_srv_inventory_dwh USING BTREE (modificationdate) ;
CREATE INDEX ix_t_srv_inventory_dwh06 ON t_srv_inventory_dwh USING GIN (checksum) ;
CREATE INDEX ix_t_srv_inventory_dwh07 ON t_srv_inventory_dwh USING BTREE (validitystart) ;
CREATE INDEX ix_t_srv_inventory_dwh08 ON t_srv_inventory_dwh USING BTREE (validitystop) ;
CREATE INDEX ix_t_srv_inventory_dwh_spatial ON t_srv_inventory_dwh USING GIST (footprint) ;
CREATE INDEX ix_t_srv_inventory_dwh09 ON t_srv_inventory_dwh USING BTREE (filetype_id) ;
CREATE INDEX ix_t_srv_inventory_dwh10 ON t_srv_inventory_dwh USING BTREE (satellite_id) ;
CREATE INDEX ix_t_srv_inventory_dwh11 ON t_srv_inventory_dwh USING BTREE (sensor_id) ;
CREATE INDEX ix_t_srv_inventory_dwh12 ON t_srv_inventory_dwh USING BTREE (evictiondate) ;
CREATE INDEX ix_t_srv_inventory_dwh13 ON t_srv_inventory_dwh USING GIN (attributes) ;








--------------------------------------------------------------------------------
-------------------------- TRIGGER FUNCTIONS CREATION --------------------------
--------------------------------------------------------------------------------

/* --------------------------
	tf_populate_inventory_dwh
*/ --------------------------
CREATE OR REPLACE FUNCTION tf_populate_inventory_dwh() RETURNS TRIGGER AS $$

BEGIN
	INSERT INTO t_srv_inventory_dwh (
		id,
		uuid,
		name,
		contenttype,
		contentlength,
		origindate,
		creationdate,
		modificationdate,
		checksum,
		validitystart,
		validitystop,
		footprint,
		validity,
		filetype_id,
		satellite_id,
		sensor_id,
		mission,
		evictiondate,
		productiontype,
		attributes,
		trace_id)
	VALUES (
		NEW.id,
		NEW.uuid,
		NEW.name,
		NEW.contenttype,
		NEW.contentlength,
		NEW.origindate,
		NEW.creationdate,
		NEW.modificationdate,
		NEW.checksum,
		NEW.validitystart,
		NEW.validitystop,
		NEW.footprint,
		NEW.validity,
		NEW.filetype_id,
		NEW.satellite_id,
		NEW.sensor_id,
		NEW.mission,
		NEW.evictiondate,
		NEW.productiontype,
		NEW.attributes,
		NEW.trace_id 
	)
	ON CONFLICT (id) 
	DO 
	UPDATE SET
		uuid = NEW.uuid,
		name = NEW.name,
		contenttype = NEW.contenttype,
		contentlength = NEW.contentlength,
		origindate = NEW.origindate,
		creationdate = NEW.creationdate,
		modificationdate = NEW.modificationdate,
		checksum = NEW.checksum,
		validitystart = NEW.validitystart,
		validitystop = NEW.validitystop,
		footprint = NEW.footprint,
		validity = NEW.validity,
		filetype_id = NEW.filetype_id,
		satellite_id = NEW.satellite_id,
		sensor_id = NEW.sensor_id,
		mission = NEW.mission,
		evictiondate = NEW.evictiondate,
		productiontype = NEW.productiontype,
		attributes = NEW.attributes,
		trace_id = NEW.trace_id
	;
	
	RETURN NEW ;
	
END ;
$$ LANGUAGE plpgsql ;








--------------------------------------------------------------------------------
------------------------------- TRIGGER CREATION -------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_srv_inventory
*/ ------------------------------
CREATE TRIGGER tr_srv_inventory_populate_inventory_dwh AFTER INSERT OR UPDATE
	ON t_srv_inventory
		FOR EACH ROW EXECUTE PROCEDURE tf_populate_inventory_dwh() ;








-------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS --------------------------------
-------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'srv-01.09.00' ;

INSERT INTO t_schema (
	version,
	modificationdate,
	modificationtype,
	schematype,
	cvstag,
	description )
VALUES  (
	'srv-01.09.00',
	'2021-11-23',
	6,
	35,
	'SRV_1-9-0',
	'Added new table t_srv_inventory_dwh and trigger to populate it (CCDS-27)'
) ;



