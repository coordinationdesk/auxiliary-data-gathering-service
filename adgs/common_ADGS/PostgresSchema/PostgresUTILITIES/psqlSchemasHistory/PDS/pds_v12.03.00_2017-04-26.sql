
-- Version 12.03.00
-- POSTGRES







/* ------------------------------
	t_distributionalgo
*/ ------------------------------
INSERT INTO t_distributionalgo (id, name, on_demand) VALUES (8, 'QCC_DISTR_CONDITIONED', 't') ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'pds-12.03.00' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-12.03.00',
		'2017-04-26',
		3,
		2,
		'PDS_12-3-0',
		'Added new value NAME on t_distributionalgo.'
) ;
