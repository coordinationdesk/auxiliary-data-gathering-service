
-- Version 13.05.01
-- POSTGRES








--------------------------------------------------------------------------------
-------------------------------- TABLES CHANGES --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_operation_requests 
*/ ------------------------------
ALTER TABLE t_operation_requests RENAME COLUMN contet_type TO content_type ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'pds-13.05.01' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-13.05.01',
		'2021-09-29',
		3,
		2,
		'PDS_13-5-1',
		'Renamed column contet_type TO content_type on t_operation_requests table'
) ;


