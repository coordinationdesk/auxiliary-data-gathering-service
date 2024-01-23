
-- Version 11.7.2
-- POSTGRES








---------------------------------------------------------------------------------
--------------------------------- TABLE CHANGES ---------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_smactions
*/ ------------------------------
ALTER TABLE t_smactions ADD unique_id VARCHAR(160) ;
UPDATE t_smactions s SET unique_id = (
	SELECT unique_id FROM t_inventory i
		WHERE s.filename = i.filename AND s.fileversion = i.fileversion);
UPDATE t_smactions s SET unique_id = filename || '_' || fileversion WHERE unique_id IS NULL ;
ALTER TABLE t_smactions ALTER COLUMN unique_id SET NOT NULL;


/* ------------------------------
	t_lta_operations
*/ ------------------------------
DELETE FROM t_lta_operations WHERE 1 = 1 ;

ALTER TABLE t_lta_operations DROP status ;
ALTER TABLE t_lta_operations DROP status_reason ;
ALTER TABLE t_lta_operations DROP tstamp ;

ALTER TABLE t_lta_operations RENAME TO t_asyncsm_operations ;
ALTER TABLE t_asyncsm_operations RENAME COLUMN id_lta_operation TO id_asyncsm_operation ;
ALTER TABLE t_asyncsm_operations RENAME COLUMN pdi_id TO unique_id ;
ALTER TABLE t_asyncsm_operations ALTER COLUMN unique_id TYPE VARCHAR(160) ;
ALTER TABLE t_asyncsm_operations ADD filename VARCHAR(128) NOT NULL ;
ALTER TABLE t_asyncsm_operations ADD fileversion VARCHAR(32) NOT NULL ;
ALTER TABLE t_asyncsm_operations ADD tstamp TIMESTAMP(6) DEFAULT now() NOT NULL ;
ALTER TABLE t_asyncsm_operations ADD operation_id SMALLINT NOT NULL ;
ALTER TABLE t_asyncsm_operations ADD sm_id INTEGER NOT NULL ;
ALTER TABLE t_asyncsm_operations ADD status_id SMALLINT NOT NULL ;
ALTER TABLE t_asyncsm_operations ADD status_reason TEXT ;

ALTER TABLE t_asyncsm_operations
	ADD CONSTRAINT fk_t_asyncsm_operations1
		FOREIGN KEY (operation_id)
			REFERENCES t_operationtypes (id_operationtype) ;
ALTER TABLE t_asyncsm_operations
	ADD CONSTRAINT fk_t_asyncsm_operations2
		FOREIGN KEY (sm_id)
			REFERENCES t_storagemanager (id) ;
ALTER TABLE t_asyncsm_operations
	ADD CONSTRAINT fk_t_asyncsm_operations3
		FOREIGN KEY (status_id)
			REFERENCES t_stati (id) ;

ALTER TABLE t_asyncsm_operations DROP CONSTRAINT pk_t_lta_operations ;
ALTER TABLE t_asyncsm_operations
	ADD CONSTRAINT pk_t_asyncsm_operations
		PRIMARY KEY (id_asyncsm_operation) ;
		
ALTER SEQUENCE t_lta_operations_id_lta_operation_seq RENAME TO t_asyncsm_operations_id_asyncsm_operation ;

COMMENT ON COLUMN t_asyncsm_operations.tstamp IS 'The timestamp of the instant when the product entered in the status specified into the field "status"';
COMMENT ON COLUMN t_asyncsm_operations.operation_id IS 'Type of operation. It is a reference to t_operationtypes';
COMMENT ON COLUMN t_asyncsm_operations.sm_id IS 'The storage in which the LTA product is archived. It is a reference to t_storagemanager';
COMMENT ON COLUMN t_asyncsm_operations.status_id IS 'The status in which the LTA product is. It is a reference to t_stati';
COMMENT ON COLUMN t_asyncsm_operations.status_reason IS 'Description of the reason why the product is in the status defined into the field "status"';








---------------------------------------------------------------------------------
--------------------------------- TABLE INSERT ---------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_operationtypes
*/ ------------------------------
INSERT INTO t_operationtypes (id_operationtype, type) VALUES (4, 'UPLOAD') ;
INSERT INTO t_operationtypes (id_operationtype, type) VALUES (5, 'DOWNLOAD') ;
INSERT INTO t_operationtypes (id_operationtype, type) VALUES (6, 'EREASE') ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'pds-11.7.2' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-11.7.2',
		'2015-02-10',
		3,
		2,
		'PDS_11-7-2',
		'Renamed t_lta_operations to t_asyncsm_operations and changed some fields.'
) ;


