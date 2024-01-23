
-- Version 11.6.3
-- POSTGRES








---------------------------------------------------------------------------------
------------------------------ TABLES MODIFICATION ------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_repositories
*/ ------------------------------
ALTER TABLE t_repositories ADD allowedciphers VARCHAR(256) DEFAULT '' NOT NULL ;








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
		'pds-11.6.3',
		'2014-04-17',
		3,
		2,
		'PDS_11-6-3',
		'Added new field allowedciphers on t_repositories.'
) ;


