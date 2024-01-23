
-- Version 11.7.0
-- POSTGRES








---------------------------------------------------------------------------------
-------------------------------- TABLES CREATION --------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_lta_operations
*/ ------------------------------
CREATE TABLE t_lta_operations (
    id_lta_operation SERIAL NOT NULL,
    pdi_id VARCHAR(136) NOT NULL,
	status VARCHAR(24) NOT NULL,
	status_reason TEXT,
    tstamp TIMESTAMP(6) NOT NULL
) ;
ALTER TABLE t_lta_operations
	ADD CONSTRAINT pk_t_lta_operations
		PRIMARY KEY (id_lta_operation) ;

COMMENT ON TABLE t_lta_operations IS 'Contains the list of the ingestions statuses';
COMMENT ON COLUMN t_lta_operations.id_lta_operation IS 'Record identifier (Primary Key)';
COMMENT ON COLUMN t_lta_operations.pdi_id IS 'Unique ID of the LTA Product. It is the ID of the PDI at datastrip level';
COMMENT ON COLUMN t_lta_operations.status IS 'The status in which the LTA product is';
COMMENT ON COLUMN t_lta_operations.status_reason IS 'Description of the reason why the product is in the status defined into the field "status"';
COMMENT ON COLUMN t_lta_operations.tstamp IS 'The timestamp of the instant when the product entered in the status specified into the field "status"';








---------------------------------------------------------------------------------
--------------------------------- TABLE CHANGES ---------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_dbparameters
*/ ------------------------------
ALTER TABLE t_invspecialrollingpolicies ADD consumable BOOLEAN DEFAULT 'f' NOT NULL ;


/* ------------------------------
	t_storagemanager
*/ ------------------------------
ALTER TABLE t_storagemanager ADD error_recovery_period INTEGER DEFAULT 60 ;


/* ------------------------------
	t_processors
*/ ------------------------------

-- DROP DEPENDING OBJECTS
ALTER TABLE t_ordersgeneration DROP CONSTRAINT fk_t_ordersgeneration1 ;
ALTER TABLE t_ordersqueue DROP CONSTRAINT fk_t_ordersqueue2 ;

-- CHANGING TABLE COLUMNS
ALTER TABLE t_processors ADD id_processor SERIAL NOT NULL ;
ALTER TABLE t_processors DROP CONSTRAINT pk_t_processors ;
ALTER TABLE t_processors
	ADD CONSTRAINT pk_t_processors
		PRIMARY KEY (id_processor) ;
ALTER TABLE t_processors 
	ADD CONSTRAINT uq_processors01
		UNIQUE (processorname, processorversion) ;

-- RECREATE DEPENDING OBJECTS
ALTER TABLE t_ordersgeneration
	ADD CONSTRAINT fk_t_ordersgeneration1
		FOREIGN KEY (processorname, processorversion)
			REFERENCES t_processors (processorname, processorversion)  
				ON DELETE CASCADE ;
ALTER TABLE t_ordersqueue
	ADD CONSTRAINT fk_t_ordersqueue2
		FOREIGN KEY (processorname, processorversion)
			REFERENCES t_processors (processorname, processorversion)
				ON DELETE CASCADE ;








---------------------------------------------------------------------------------
---------------------------- PROCEDURES MODIFCATIONS ----------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	P_REMOVE_OBSOLETE_ACTIONS
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_remove_obsolete_actions(
	fname VARCHAR(128), 
	fversion VARCHAR(32)) RETURNS INTEGER AS $$

DECLARE
	row RECORD;
	
BEGIN

	FOR row IN (
		SELECT 
			sma.packageid AS pkgid,
			smq.status AS stat, 
			smq.operation AS oper
		FROM 
			t_smactions sma
			INNER JOIN t_smqueue smq
				ON smq.packageid = sma.packageid
		WHERE 
			sma.filename = fname 
			AND sma.fileversion = fversion 
			AND sma.packageid IS NOT NULL 
			AND sma.operation = 'UPLOAD') LOOP
	
		IF row.stat = 'ELIGIBLE' AND row.oper = 'UPLOAD' OR row.stat = 'ERROR' AND row.oper = 'UPLOAD' THEN
			UPDATE t_smactions SET packageid = NULL WHERE packageid = row.pkgid ;
			DELETE FROM t_smqueue WHERE packageid = row.pkgid ;
		END IF ;
		
	END LOOP ;
	
	DELETE FROM t_smactions WHERE filename = fname AND fileversion = fversion AND operation = 'UPLOAD' ;
	DELETE FROM t_smqueue WHERE status = 'COMPLETED' AND packageid not in (
		SELECT DISTINCT packageid FROM t_smactions WHERE packageid IS NOT NULL) ;
	
	RETURN 0 ;
END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------
DELETE FROM t_schema WHERE version = 'pds-11.7.0' ;
INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-11.7.0',
		'2015-01-26',
		3,
		2,
		'PDS_11-7-0',
		'Added new table t_lta_operations. Added new field "consumable" on t_invpecialrollingpolicies and added new serial field on t_processors. Fixed bug on p_remove_obsolete_actions.'
) ;


