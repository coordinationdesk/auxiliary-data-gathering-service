
-- Version 1.5.8
-- POSTGRES








/* ------------------------------
        t_schematypes
*/ ------------------------------
INSERT INTO t_schematypes (id, schematype, description) VALUES (41, 'MMO', 'MultiMission Ordering DB') ;





-------------------------------------------------------------------------------
--------------------------------- CONCLUSIONS ---------------------------------
-------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'vrsn-01.05.08' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'vrsn-01.05.08',
		'2021-06-16',
		3,
		39,
		'VRSN_1-5-8',
		'Added new schema types'
) ;


