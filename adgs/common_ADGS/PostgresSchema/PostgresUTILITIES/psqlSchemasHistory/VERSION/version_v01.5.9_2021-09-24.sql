
-- Version 1.5.9
-- POSTGRES








/* ------------------------------
        t_schematypes
*/ ------------------------------
INSERT INTO t_schematypes (id, schematype, description) VALUES (42, 'BROADCAST', 'Broadcast Events Messages Schema') ;





-------------------------------------------------------------------------------
--------------------------------- CONCLUSIONS ---------------------------------
-------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'vrsn-01.05.09' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'vrsn-01.05.09',
		'2021-09-24',
		3,
		39,
		'VRSN_1-5-9',
		'Added new schema types'
) ;


