
-- Version 1.5.7
-- POSTGRES








/* ------------------------------
        t_schematypes
*/ ------------------------------
INSERT INTO t_schematypes (id, schematype, description) VALUES (40, 'COMMON', 'Common schema') ;





-------------------------------------------------------------------------------
--------------------------------- CONCLUSIONS ---------------------------------
-------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'vrsn-01.05.07' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'vrsn-01.05.07',
		'2021-03-03',
		3,
		39,
		'VRSN_1-5-7',
		'Added new schema types'
) ;


