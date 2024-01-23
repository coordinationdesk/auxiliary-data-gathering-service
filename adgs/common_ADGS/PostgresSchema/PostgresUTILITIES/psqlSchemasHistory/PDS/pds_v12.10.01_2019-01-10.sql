
-- Version 12.10.01
-- POSTGRES








--------------------------------------------------------------------------------
-------------------------------- TABLES INSERT ---------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_tmpnamestrategy
*/ ------------------------------
INSERT INTO t_tmpnamestrategy (id_tmpnamestrategy, tempname) VALUES (4, 'TSTAMP') ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'pds-12.10.01' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-12.10.01',
		'2019-01-10',
		3,
		2,
		'PDS_12-10-1',
		'Added TSTAMP value to t_tmpnamestrategy table'
) ;


