
-- Version 12.10.00
-- POSTGRES








--------------------------------------------------------------------------------
-------------------------------- TABLES CREATION -------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_inv_data_restored
*/ ------------------------------
CREATE TABLE t_inv_data_restored (
	id INTEGER NOT NULL,
	sm_id INTEGER NOT NULL,
	inv_id BIGINT NOT NULL,
	parent_inv_id BIGINT,
	filetype_id INTEGER NOT NULL,
	size BIGINT NOT NULL,
	tstamp TIMESTAMP DEFAULT NOW()
) ;

ALTER TABLE t_inv_data_restored
	ADD CONSTRAINT pk_t_inv_data_restored
		PRIMARY KEY (id) ;

ALTER TABLE t_inv_data_restored
	ADD CONSTRAINT fk_inv_data_restored01
		FOREIGN KEY (id)
			REFERENCES t_inventorysm (id)
				ON DELETE CASCADE ;

ALTER TABLE t_inv_data_restored
	ADD CONSTRAINT fk_inv_data_restored02
		FOREIGN KEY (sm_id)
			REFERENCES t_storagemanager (id)
				ON DELETE CASCADE ;

ALTER TABLE t_inv_data_restored
	ADD CONSTRAINT fk_inv_data_restored03
		FOREIGN KEY (inv_id)
			REFERENCES t_inventory (id_inv)
				ON DELETE CASCADE ;

ALTER TABLE t_inv_data_restored
	ADD CONSTRAINT fk_inv_data_restored04
		FOREIGN KEY (parent_inv_id)
			REFERENCES t_inventory (id_inv)
				ON DELETE CASCADE ;

ALTER TABLE t_inv_data_restored
	ADD CONSTRAINT fk_inv_data_restored05
		FOREIGN KEY (filetype_id)
			REFERENCES t_filetypes (id_filetype)
				ON DELETE CASCADE ;

CREATE INDEX ix_t_inv_data_restored01 ON t_inv_data_restored USING BTREE (sm_id) ;
CREATE INDEX ix_t_inv_data_restored02 ON t_inv_data_restored USING BTREE (inv_id) ;
CREATE INDEX ix_t_inv_data_restored03 ON t_inv_data_restored USING BTREE (parent_inv_id) ;
CREATE INDEX ix_t_inv_data_restored04 ON t_inv_data_restored USING BTREE (filetype_id) ;
CREATE INDEX ix_t_inv_data_restored05 ON t_inv_data_restored USING BTREE (tstamp) ;








--------------------------------------------------------------------------------
-------------------------------- TABLES CHANGES --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_receptionrules
*/ ------------------------------
DROP INDEX uq_receptionrules02 ;
CREATE UNIQUE INDEX uq_receptionrules02 
	ON t_receptionrules (filetype_id, localpath, COALESCE(hostname, 'DUMMY_VALUE_USED_FOR_UNIQUE_CONSTRAINT'), COALESCE(satellite_id, 0), COALESCE(mission, 0), COALESCE(sensor_id, 0))
		WHERE active = 't' ;








--------------------------------------------------------------------------------
-------------------------------- TABLES INSERT ---------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_operationtypes
*/ ------------------------------
INSERT INTO t_operationtypes (id_operationtype, type) VALUES (7, 'POSTPROC') ;


/* ------------------------------
	t_orderwaittrigger
*/ ------------------------------
INSERT INTO t_orderwaittrigger VALUES ('NEVER') ;


/* ------------------------------
	t_stati
*/ ------------------------------
INSERT INTO t_stati (id, status) VALUES (41, 'BLOCKED') ;
INSERT INTO t_stati (id, status) VALUES (42, 'DISABLED') ;
INSERT INTO t_stati (id, status) VALUES (43, 'ENABLED') ;
INSERT INTO t_stati (id, status) VALUES (44, 'BLOCKING') ;


/* ------------------------------
	t_statetransitions_flows
*/ ------------------------------
INSERT INTO t_statetransitions_flows (id, flow_type) VALUES (6, 'REPUBLISHING') ;
INSERT INTO t_statetransitions_flows (id, flow_type) VALUES (7, 'RETRIEVEONLY') ;








--------------------------------------------------------------------------------
------------------------------ PROCEDURE CREATION ------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	p_insert_inv_data_restored
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_insert_inv_data_restored(
	ismId INTEGER,
	invId BIGINT,
	smId INTEGER) RETURNS INTEGER AS $$

DECLARE
	isSync BOOLEAN ;
	asynchCount INTEGER ;
	parentsAsynchCount INTEGER ;
	insertedRows INTEGER := 0 ;
	
BEGIN
	-- Get the syncupload type of the SM for the new record to
	-- check if the new record has been inserted into a SYNCH repository
	SELECT s.syncupload INTO isSync 
	FROM t_storagemanager s INNER JOIN t_inventorysm i ON i.storagemanid = s.id
	WHERE i.id = ismId ;
	
	IF isSync IS TRUE THEN
		
		-- Check if there are other records having the same inv_id into asynch repositories
		SELECT count(*) INTO asynchCount FROM t_inventorysm i INNER JOIN t_storagemanager s ON i.storagemanid = s.id 
		WHERE i.inv_id = invId AND s.syncupload IS FALSE ;
		
		-- Check if there are other "parents" records having the same inv_id into asynch repositories
		SELECT count(*) INTO parentsAsynchCount FROM t_inventorysm i INNER JOIN t_storagemanager s ON i.storagemanid = s.id 
		WHERE i.inv_id = (SELECT inv_id2 FROM t_inv_relations WHERE inv_id1 = invId) AND s.syncupload IS FALSE ;
		
		-- If there is another record (or his parent) into an asynch repository insert a record into t_inv_data_restored
		IF (asynchCount > 0 OR parentsAsynchCount > 0) THEN
			INSERT INTO t_inv_data_restored (
				id,
				sm_id,
				inv_id,
				parent_inv_id,
				filetype_id,
				size)
			SELECT 
				ismId,
				smId,
				invId,
				r.inv_id2,
				f.id_filetype,
				CASE WHEN i.datasize IS NULL THEN 0 ELSE i.datasize END
			FROM
				t_inventory i
				LEFT JOIN t_inv_relations r ON i.id_inv = r.inv_id1
				INNER JOIN t_filetypes f ON i.filetype = f.filetype
			WHERE
				i.id_inv = invId ;
			
			GET DIAGNOSTICS insertedRows = ROW_COUNT ;
			
		END IF ;
	END IF ;
	
	RETURN insertedRows ;
	
END ;
$$ LANGUAGE plpgsql ;








--------------------------------------------------------------------------------
-------------------------- TRIGGERS FUNCTIONS CHANGES --------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	tf_inventorysmmanager
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_inventorysmmanager() RETURNS TRIGGER AS $$

BEGIN
	PERFORM p_events('t_inventorysm', TG_ARGV[0]) ;
	
	IF (TG_ARGV[0] = 'INS') 
	THEN
		PERFORM p_insert_inventoryevents(NEW.inv_id, NEW.storagemanid) ;
		PERFORM p_insert_inv_data_restored(NEW.id, NEW.inv_id, NEW.storagemanid) ;
		RETURN NEW ;
	END IF ;
	
	IF (TG_ARGV[0] = 'UPD') 
	THEN
		PERFORM p_insert_inventoryevents(NEW.inv_id, NEW.storagemanid) ;
		RETURN NEW ;
	END IF ;
	
	IF (TG_ARGV[0] = 'DEL') 
	THEN 
		PERFORM p_insert_inventoryevents(OLD.inv_id, OLD.storagemanid) ;
		
		IF (SELECT count(*) FROM t_inventorysm ism
			INNER JOIN t_storagemanager stm ON ism.storagemanid = stm.id
			WHERE ism.inv_id = OLD.inv_id AND stm.syncdownload = 't') = 0 
		THEN
			DELETE FROM t_sm_sync_missingactions WHERE inv_id = OLD.inv_id AND operation = 'UPLOAD' ;
		END IF ;
		
		RETURN OLD ;
	END IF ;
	
	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;







---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'pds-12.10.00' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-12.10.00',
		'2018-04-23',
		3,
		2,
		'PDS_12-10-0',
		'Created table t_inv_data_restored and p_insert_inv_data_restored. Changed tf_inventorysmmanager trigger function'
) ;


