
-- Version 1.7.1
-- POSTGRES








---------------------------------------------------------------------------------
------------------------------ TABLES MODIFICATION ------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_circulationpolicies
*/ ------------------------------
ALTER TABLE t_circulationpolicies ADD removesource BOOLEAN DEFAULT 'f' NOT NULL ;








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
		'circulation-1.7.1',
		'2013-09-30',
		6,
		20,
		'CIRCULATION_1-7-1',
		'Added removesource field on t_circulationpolicies.'
) ;


