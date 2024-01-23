
-- Version 12.16.00
-- POSTGRES








--------------------------------------------------------------------------------
-------------------------------- TABLES CREATION -------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
        t_operation_requests
*/ ------------------------------
CREATE TABLE t_operation_requests (
	id SERIAL NOT NULL,
	object_id BIGINT NOT NULL,
	status VARCHAR(32) NOT NULL,
	type VARCHAR(32) NOT NULL,
	start_time TIMESTAMP(6) DEFAULT NOW() NOT NULL,
	stop_time TIMESTAMP(6),
	error_message TEXT,
	contet_type VARCHAR(32) NOT NULL
);
ALTER TABLE t_operation_requests
	ADD CONSTRAINT pk_t_operation_requests
		PRIMARY KEY (id) ;








--------------------------------------------------------------------------------
-------------------------------- TABLES CHANGES --------------------------------
--------------------------------------------------------------------------------

-- Remove all "WITH OIDS" string from creation statements of tables having OID








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'pds-12.16.00' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-12.16.00',
		'2021-02-02',
		3,
		2,
		'PDS_12-16-0',
		'Added new table t_operation_requests'
) ;


