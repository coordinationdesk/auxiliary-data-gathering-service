
-- Version 1.17.0
-- POSTGRES





-- #############################################################################
-- ##################                                         ##################
-- ##################                WARNING!!!               ##################
-- ##################   This patch can be installed only on   ##################
-- ##################       PDS 12.0.0 version or major       ##################
-- ##################                                         ##################
-- #############################################################################





--------------------------------------------------------------------------------
-------------------------------- TABLES CREATION -------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
    t_circulationpolicies_monitor
*/ ------------------------------
CREATE TABLE t_circulationpolicies_monitor (
	id SERIAL NOT NULL,
	circulationpolicy_id INTEGER NOT NULL,
	monitoringflag BOOLEAN NOT NULL
);

ALTER TABLE t_circulationpolicies_monitor 
	ADD CONSTRAINT pk_t_circulationpolicies_monitor 
		PRIMARY KEY(id);

ALTER TABLE t_circulationpolicies_monitor 
	ADD CONSTRAINT uq_t_circulationpolicies_monitor01
		UNIQUE (circulationpolicy_id);

ALTER TABLE t_circulationpolicies_monitor 
	ADD CONSTRAINT fk_t_circulationpolicies_monitor01
		FOREIGN KEY (circulationpolicy_id) 
			REFERENCES t_circulationpolicies (id) 
				ON DELETE CASCADE;

COMMENT ON TABLE t_circulationpolicies_monitor IS 'Contains the list of circulation policies to be monitored';
COMMENT ON COLUMN t_circulationpolicies_monitor.id IS 'Record identifier (Primary Key)';
COMMENT ON COLUMN t_circulationpolicies_monitor.circulationpolicy_id IS 'Circulation policy identifier. References the t_circulationpolicies table';
COMMENT ON COLUMN t_circulationpolicies_monitor.monitoringflag IS 'Used for the monitoring of the circulation policy';








--------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ---------------------------------
--------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'circulation-1.17.0' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'circulation-1.17.0',
		'2016-10-12',
		6,
		20,
		'CIRCULATION_1-17-0',
		'Added new table t_circulationpolicies_monitor'
) ;


