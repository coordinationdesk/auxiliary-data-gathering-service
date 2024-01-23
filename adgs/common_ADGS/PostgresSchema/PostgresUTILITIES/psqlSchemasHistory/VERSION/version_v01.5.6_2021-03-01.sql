
-- Version 1.5.6
-- POSTGRES








/* ------------------------------
        t_schematypes
*/ ------------------------------
INSERT INTO t_schematypes (id, schematype, description) VALUES (37, 'LTADL', NULL) ;
INSERT INTO t_schematypes (id, schematype, description) VALUES (38, 'TRANS', 'State transition plug-in schema') ;
INSERT INTO t_schematypes (id, schematype, description) VALUES (39, 'VRSN', 'Versioning schema') ;





-------------------------------------------------------------------------------
--------------------------------- CONCLUSIONS ---------------------------------
-------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'vrsn-01.05.06' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'vrsn-01.05.06',
		'2021-03-01',
		3,
		39,
		'VRSN_1-5-6',
		'Added new schema types'
) ;


