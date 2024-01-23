
-- Version 11.6.7
-- POSTGRES








---------------------------------------------------------------------------------
--------------------------------- TABLE CHANGES ---------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_stati
*/ ------------------------------
INSERT INTO t_stati (id, status) VALUES (38, 'LTA_PROCESSING') ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------
DELETE FROM t_schema WHERE version = 'pds-11.6.7' ;
INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-11.6.7',
		'2014-10-24',
		3,
		2,
		'PDS_11-6-7',
		'Added new status.'
) ;


