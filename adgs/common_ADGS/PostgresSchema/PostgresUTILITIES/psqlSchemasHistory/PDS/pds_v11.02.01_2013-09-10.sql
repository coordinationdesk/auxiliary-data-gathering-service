
-- Version 11.2.1
-- POSTGRES








DROP TABLE IF EXISTS db_values CASCADE ;
DROP TABLE IF EXISTS query_types CASCADE ;

-- Eliminare i files per il popolamento della dbvalues e della query_types!!!








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------
INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-11.2.1',
		'2013-09-10',
		3,
		2,
		'PDS_11-2-1',
		'Dropped dbvalues and query_types tables.'
) ;


