
-- Version 11.12.03
-- POSTGRES









---------------------------------------------------------------------------------
--------------------------------- TABLE CHANGES ---------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_temporaryorders
*/ ------------------------------
ALTER TABLE t_temporaryorders ADD COLUMN emittime TIMESTAMP(6);



---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'pds-11.12.03' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-11.12.03',
		'2016-01-21',
		3,
		2,
		'PDS_11-12-3',
		'Added the new field emittime in the t_temporaryorders table'
) ;


