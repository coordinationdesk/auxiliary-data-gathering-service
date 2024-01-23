
-- Version 1.0.4
-- POSTGRES







--------------------------------------------------------------------------------
-------------------------------- TABLES CHANGES --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_receptionrules
*/ ------------------------------
ALTER TABLE t_crgroups ALTER COLUMN mailaddresses TYPE VARCHAR(1024) ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'cr-1.0.4' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'cr-1.0.4',
		'2016-08-03',
		5,
		24,
		'CR_1-0-4',
		'Changed type TEXT to VARCHAR on t_crgroups'
) ;


