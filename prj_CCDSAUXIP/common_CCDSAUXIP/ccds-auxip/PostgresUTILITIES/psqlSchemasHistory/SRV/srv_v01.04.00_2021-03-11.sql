
-- SRV Version 1.4.0
-- POSTGRES





-------------------------------------------------------------------------------
------------------------------- TABLES CREATION -------------------------------
-------------------------------------------------------------------------------

/* ------------------------------
	t_srv_catalogue_change_registry
*/ ------------------------------
CREATE TABLE t_srv_catalogue_change_registry (
    id BIGSERIAL NOT NULL,
    sensingStartDate TIMESTAMP(0) NOT NULL,
    modificationDate TIMESTAMP(0) NOT NULL DEFAULT NOW()::DATE,
	satellite_id SMALLINT NOT NULL
);
ALTER TABLE t_srv_catalogue_change_registry 
	ADD CONSTRAINT pk_t_srv_catalogue_change_registry
		PRIMARY KEY (id) ;

ALTER TABLE t_srv_catalogue_change_registry
	ADD CONSTRAINT uq_srv_catalogue_change_registry01
		UNIQUE (sensingStartDate, modificationDate, satellite_id) ;

ALTER TABLE t_srv_catalogue_change_registry
	ADD CONSTRAINT fk_srv_catalogue_change_registry01
		FOREIGN KEY (satellite_id) REFERENCES t_satellites (satelliteid)
			ON DELETE CASCADE ;

CREATE INDEX ix_t_srv_catalogue_change_registry01 ON t_srv_catalogue_change_registry USING BTREE (sensingStartDate) ;
CREATE INDEX ix_t_srv_catalogue_change_registry02 ON t_srv_catalogue_change_registry USING BTREE (modificationDate) ;
CREATE INDEX ix_t_srv_catalogue_change_registry03 ON t_srv_catalogue_change_registry USING BTREE (satellite_id) ;








-------------------------------------------------------------------------------
-------------------------------- TABLES CHANGES -------------------------------
-------------------------------------------------------------------------------

/* ------------------------------
	t_srv_receptionruleshist
*/ ------------------------------
CREATE INDEX ix_t_srv_receptionruleshist05 ON t_srv_receptionruleshist USING BTREE (evictiondate) ;








-------------------------------------------------------------------------------
------------------------------ PROCEDURES CHANGES -----------------------------
-------------------------------------------------------------------------------

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
		
	INSERT INTO t_srv_catalogue_change_registry (sensingStartDate, satellite_id)
		SELECT validitystart::DATE, satellite_id FROM t_srv_inventory WHERE id = in_inv_id AND satellite_id IS NOT NULL
	ON CONFLICT DO NOTHING ;
	
END;
$$ LANGUAGE PLPGSQL ;








-------------------------------------------------------------------------------
--------------------------- TRIGGER FUNCTION CHANGES --------------------------
-------------------------------------------------------------------------------

/* --------------------------
	tf_srv_bulkbatches_to_be_cancelled
*/ --------------------------
CREATE OR REPLACE FUNCTION tf_srv_bulkbatches_to_be_cancelled() RETURNS TRIGGER AS $$

DECLARE
	child_ord_id RECORD ;
	
BEGIN
	UPDATE t_srv_bulk_orders SET tobecancelled = TRUE
		WHERE id = NEW.bulk_id AND status_id IN (SELECT id FROM t_srv_statuses WHERE status IN ('CREATED','IN_PROGRESS')) ;
	
	IF (NEW.status_id = (SELECT id FROM t_srv_statuses WHERE status = 'QUEUED')) 
	THEN
		UPDATE t_srv_bulkbatches SET tobecancelled = TRUE WHERE id = NEW.id ;
	ELSE
		FOR child_ord_id IN (
			SELECT order_id FROM t_srv_bulkbatch_x_ordersqueue WHERE bulkbatch_id = NEW.id )
		LOOP
			CALL p_srv_del_order_link(child_ord_id.order_id) ;
		END LOOP ;
	END IF ;
	
	RETURN NEW ;
	
END ;
$$ LANGUAGE plpgsql ;



/* --------------------------
	tf_srv_pending_inv_operations
*/ --------------------------
CREATE OR REPLACE FUNCTION tf_srv_pending_inv_operations() RETURNS TRIGGER AS $$

BEGIN
	IF (TG_OP = 'DELETE') THEN
		INSERT INTO t_srv_pendinginv (
			inv_id, uuid, name, contenttype, contentlength, origindate, creationdate, modificationdate, checksum, validitystart, validitystop, footprint, validity, filetype_id, satellite_id, sensor_id, mission, evictiondate, productiontype, attributes, trace_id, operation) 
			SELECT OLD.*, 'DELETE' ;
		
		IF OLD.satellite_id IS NOT NULL THEN
			INSERT INTO t_srv_catalogue_change_registry (sensingStartDate, satellite_id) 
				VALUES (OLD.validitystart::DATE, OLD.satellite_id) 
				ON CONFLICT DO NOTHING ;
		END IF;
		
		RETURN OLD;
		
	ELSIF (TG_OP = 'UPDATE') THEN
		INSERT INTO t_srv_pendinginv (
			inv_id, uuid, name, contenttype, contentlength, origindate, creationdate, modificationdate, checksum, validitystart, validitystop, footprint, validity, filetype_id, satellite_id, sensor_id, mission, evictiondate, productiontype, attributes, trace_id, operation) 
			SELECT NEW.*, 'UPDATE' ;
		
		IF NEW.satellite_id IS NOT NULL THEN
			INSERT INTO t_srv_catalogue_change_registry (sensingStartDate, satellite_id) 
				VALUES (NEW.validitystart::DATE, NEW.satellite_id) 
				ON CONFLICT DO NOTHING ;
		END IF;
		
		RETURN NEW;
		
	ELSE
		RETURN NULL;
		
	END IF;
	
END ;
$$ LANGUAGE plpgsql ;








-------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS --------------------------------
-------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'srv-01.04.00' ;

INSERT INTO t_schema (
	version,
	modificationdate,
	modificationtype,
	schematype,
	cvstag,
	description )
VALUES  (
	'srv-01.04.00',
	'2021-03-11',
	6,
	35,
	'SRV_1-4-0',
	'Added new table t_srv_catalogue_change_registry. Changed tf_srv_bulkbatches_to_be_cancelled and tf_srv_pending_inv_operations (Task LTA-218 and LTA-228)'
) ;



