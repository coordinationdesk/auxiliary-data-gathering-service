
-- Version 11.11.00
-- POSTGRES








--------------------------------------------------------------------------------
-------------------------------- TABLES CREATION -------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_sm_sync_missingactions
*/ ------------------------------
CREATE TABLE t_sm_sync_missingactions (
    id SERIAL NOT NULL,
	operation VARCHAR(16) NOT NULL,
	sm_id INTEGER NOT NULL,
	inv_id BIGINT,
	invsm_id INTEGER,
	tstamp TIMESTAMP(6) DEFAULT now() NOT NULL
) ;
ALTER TABLE t_sm_sync_missingactions
	ADD CONSTRAINT pk_t_sm_sync_missingactions
		PRIMARY KEY (id) ;

ALTER TABLE t_sm_sync_missingactions
	ADD CONSTRAINT ck_sm_sync_missingactions01
		CHECK (operation IN ('UPLOAD', 'ERASE')) ;

ALTER TABLE t_sm_sync_missingactions 
	ADD CONSTRAINT ck_sm_sync_missingactions02 
		CHECK ((inv_id IS NULL AND invsm_id IS NOT NULL) OR (inv_id IS NOT NULL AND invsm_id IS NULL)) ;

ALTER TABLE t_sm_sync_missingactions
	ADD CONSTRAINT uq_sm_sync_missingactions1
		UNIQUE (operation, sm_id, inv_id) ;

ALTER TABLE t_sm_sync_missingactions
	ADD CONSTRAINT uq_sm_sync_missingactions2
		UNIQUE (operation, sm_id, invsm_id) ;

ALTER TABLE t_sm_sync_missingactions
	ADD CONSTRAINT fk_sm_sync_missingactions01
		FOREIGN KEY (sm_id)
			REFERENCES t_storagemanager (id)
				ON DELETE CASCADE ;

ALTER TABLE t_sm_sync_missingactions
	ADD CONSTRAINT fk_sm_sync_missingactions02
		FOREIGN KEY (inv_id)
			REFERENCES t_inventory (id_inv)
				ON DELETE CASCADE ;

ALTER TABLE t_sm_sync_missingactions
	ADD CONSTRAINT fk_sm_sync_missingactions03
		FOREIGN KEY (invsm_id)
			REFERENCES t_inventorysm (id)
				ON DELETE CASCADE ;

CREATE INDEX ix_t_sm_sync_missingactions01 ON t_sm_sync_missingactions USING BTREE (sm_id) ;
CREATE INDEX ix_t_sm_sync_missingactions02 ON t_sm_sync_missingactions USING BTREE (inv_id) ;
CREATE INDEX ix_t_sm_sync_missingactions03 ON t_sm_sync_missingactions USING BTREE (invsm_id) ;

COMMENT ON TABLE t_sm_sync_missingactions IS 'Contains the list of the failed and still pending operations performed on synch storage managers';
COMMENT ON COLUMN t_sm_sync_missingactions.id IS 'Record identifier (Primary Key)';
COMMENT ON COLUMN t_sm_sync_missingactions.operation IS 'Identifies the type of the operation. Can be UPLOAD or ERASE';
COMMENT ON COLUMN t_sm_sync_missingactions.sm_id IS 'The ID of the storage manager on which the missing operation shall be performed. References the t_storagemanager table';
COMMENT ON COLUMN t_sm_sync_missingactions.inv_id IS 'Identifies the inventory id of the file to be UPLOADED into the storage manager. References the t_inventory table';
COMMENT ON COLUMN t_sm_sync_missingactions.invsm_id IS 'Identifies the file to be ERASED from the identified storage manager. References the t_inventorysm table';
COMMENT ON COLUMN t_sm_sync_missingactions.tstamp IS 'When the record has been inserted (default value NOW)';








--------------------------------------------------------------------------------
---------------------------------- TABLES DROP ---------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_sm_missing
*/ ------------------------------
DROP TABLE IF EXISTS t_sm_missing CASCADE ;

/* ------------------------------
	t_sm_pendingerase
*/ ------------------------------
DROP TABLE IF EXISTS t_sm_pendingerase CASCADE ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'pds-11.11.00' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-11.11.00',
		'2015-10-23',
		3,
		2,
		'PDS_11-11-0',
		'Added table t_sm_sync_missingactions and dropped tables t_sm_missing and t_sm_pendingerase.'
) ;


