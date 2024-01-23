
-- SRV Version 1.1.0
-- POSTGRES





BEGIN ;



--------------------------------------------------------------------------------
-------------------------------- TABLES CHANGES --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_srv_awsuploadqueue
*/ ------------------------------
ALTER TABLE t_srv_awsuploadqueue ADD hostname VARCHAR(256) NOT NULL DEFAULT 'dummy_value_for_schema_upgrade' ;
ALTER TABLE t_srv_awsuploadqueue ALTER COLUMN hostname DROP DEFAULT ;
ALTER TABLE t_srv_awsuploadqueue ADD aws_s3_upload_pool_size SMALLINT ;
ALTER TABLE t_srv_awsuploadqueue ADD aws_s3_upload_buffer_size BIGINT ;

CREATE INDEX ix_t_srv_awsuploadqueue03 ON t_srv_awsuploadqueue USING BTREE (hostname) ;


/* ------------------------------
	t_srv_inventory
*/ ------------------------------
ALTER TABLE t_srv_inventory ADD trace_id VARCHAR(160) ;


/* ------------------------------
	t_srv_pendinginv 
*/ ------------------------------
ALTER TABLE t_srv_pendinginv ADD trace_id VARCHAR(160) ;


/* ------------------------------
	t_srv_pending_notifications
*/ ------------------------------
ALTER TABLE t_srv_pending_notifications 
	DROP CONSTRAINT ck_srv_pending_notifications02 ;
ALTER TABLE t_srv_pending_notifications 
	ADD CONSTRAINT ck_srv_pending_notifications02 
		CHECK (((batchorder_uuid IS NOT NULL) AND (subscription_uuid IS NULL) AND (subscriptionevent IS NULL) AND (order_uuid IS NULL)) 
		OR ((batchorder_uuid IS NULL) AND (subscription_uuid IS NULL) AND (order_uuid IS NOT NULL)) 
		OR ((subscription_uuid IS NOT NULL) AND (batchorder_uuid IS NULL) AND (subscriptionevent IS NOT NULL)) 
		OR ((subscription_uuid IS NOT NULL) AND (batchorder_uuid IS NULL) AND ((subscriptionevent) = 'CREATED') AND (order_uuid IS NOT NULL))) ;


/* ------------------------------
	t_srv_receptionruleshist
*/ ------------------------------
CREATE INDEX ix_t_srv_receptionruleshist04 ON t_srv_receptionruleshist USING BTREE (filecreationtstamp) ;


/* ------------------------------
	t_ltadl_bulkorders
*/ ------------------------------
ALTER TABLE t_ltadl_bulkorders ADD destination_url VARCHAR(1024) NOT NULL DEFAULT 'dummy_value_for_schema_upgrade' ;
ALTER TABLE t_ltadl_bulkorders ALTER COLUMN destination_url DROP DEFAULT ;


/* ------------------------------
	t_ltadl_filequeue
*/ ------------------------------
ALTER TABLE t_ltadl_filequeue ADD lta_product_uuid VARCHAR(160) NOT NULL DEFAULT 'dummy_value_for_schema_upgrade' ;
ALTER TABLE t_ltadl_filequeue ALTER COLUMN lta_product_uuid DROP DEFAULT ; 








--------------------------------------------------------------------------------
------------------------------ PROCEDURES CHANGES ------------------------------
--------------------------------------------------------------------------------

/* --------------------------
	p_srv_successful_inventory
*/ --------------------------
CREATE OR REPLACE PROCEDURE p_srv_successful_inventory(
	in_inv_id BIGINT) AS $$
BEGIN
	INSERT INTO t_srv_pendinginv (
		inv_id, 
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
		trace_id,
		operation
	)
		SELECT *, 'INSERT' FROM t_srv_inventory WHERE id = in_inv_id ;
	
END;
$$ LANGUAGE PLPGSQL ;








--------------------------------------------------------------------------------
--------------------------- TRIGGER FUNCTION CHANGES ---------------------------
--------------------------------------------------------------------------------

/* --------------------------
	tf_srv_pending_inv_operations
*/ --------------------------
CREATE OR REPLACE FUNCTION tf_srv_pending_inv_operations() RETURNS TRIGGER AS $$

BEGIN
	IF (TG_OP = 'DELETE') THEN
		INSERT INTO t_srv_pendinginv (
			inv_id, uuid, name, contenttype, contentlength, origindate, creationdate, modificationdate, checksum, validitystart, validitystop, footprint, validity, filetype_id, satellite_id, sensor_id, mission, evictiondate, productiontype, attributes, trace_id, operation) 
			SELECT OLD.*, 'DELETE' ;
		RETURN OLD;
	ELSIF (TG_OP = 'UPDATE') THEN
		INSERT INTO t_srv_pendinginv (
			inv_id, uuid, name, contenttype, contentlength, origindate, creationdate, modificationdate, checksum, validitystart, validitystop, footprint, validity, filetype_id, satellite_id, sensor_id, mission, evictiondate, productiontype, attributes, trace_id, operation) 
			SELECT NEW.*, 'UPDATE' ;
		RETURN NEW;
	ELSIF (TG_OP = 'INSERT') THEN
		--INSERT INTO t_srv_pendinginv (
			--inv_id, uuid,	name, contenttype, contentlength, origindate, creationdate,	modificationdate, checksum,	validitystart, validitystop, footprint, validity, filetype_id, satellite_id, sensor_id, mission, evictiondate, productiontype, attributes, trace_id, operation) 
		--	SELECT NEW.*, 'INSERT' ;
		RETURN NULL;
	END IF;
	
END ;
$$ LANGUAGE plpgsql ;








--------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ---------------------------------
--------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'srv-01.01.00' ;

INSERT INTO t_schema (
	version,
	modificationdate,
	modificationtype,
	schematype,
	cvstag,
	description )
VALUES  (
	'srv-01.01.00',
	'2020-12-01',
	6,
	35,
	'SRV_1-1-0',
	'New implementations'
) ;


COMMIT ;


