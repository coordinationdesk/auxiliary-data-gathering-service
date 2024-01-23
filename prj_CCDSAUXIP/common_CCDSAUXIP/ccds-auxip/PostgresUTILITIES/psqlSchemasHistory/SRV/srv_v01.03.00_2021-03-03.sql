
-- SRV Version 1.3.0
-- POSTGRES





-- Moved LTADL tables and TRANSACTION tables into indipendent schemas





--------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ---------------------------------
--------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'srv-01.03.00' ;

INSERT INTO t_schema (
	version,
	modificationdate,
	modificationtype,
	schematype,
	cvstag,
	description )
VALUES  (
	'srv-01.03.00',
	'2021-03-03',
	6,
	35,
	'SRV_1-3-0',
	'Moved LTADL tables and TRANSACTION tables into indipendent schemas'
) ;



