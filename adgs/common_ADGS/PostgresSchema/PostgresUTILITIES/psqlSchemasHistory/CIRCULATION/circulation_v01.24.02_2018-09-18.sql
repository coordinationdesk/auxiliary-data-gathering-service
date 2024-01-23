
-- Version 1.24.2
-- POSTGRES





--------------------------------------------------------------------------------
-------------------------------- TABLES CHANGES --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_circulationpolicies
*/ ------------------------------
ALTER TABLE t_circulationpolicies ADD circ_policy_name VARCHAR(64) ;








--------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ---------------------------------
--------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'circulation-1.24.2' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'circulation-1.24.2',
		'2018-09-18',
		6,
		20,
		'CIRCULATION_1-24-2',
		'Added circ_policy_name field on t_circulationpolicies table'
) ;


