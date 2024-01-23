
-- Version 12.15.00
-- POSTGRES








--------------------------------------------------------------------------------
-------------------------------- TABLES CHANGES --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_invrollingpolicies
*/ ------------------------------
ALTER TABLE t_invrollingpolicies ADD fetchrowslimit INTEGER ;
ALTER TABLE t_invrollingpolicies ADD idlenumitems INTEGER ;
ALTER TABLE t_invrollingpolicies ADD idlehours DOUBLE PRECISION ;
ALTER TABLE t_invrollingpolicies ADD triggertime TIMESTAMP(6) NOT NULL DEFAULT NOW() ;
ALTER TABLE t_invrollingpolicies ADD status TEXT ;


/* ------------------------------
	t_invspecialrollingpolicies
*/ ------------------------------
ALTER TABLE t_invspecialrollingpolicies DROP CONSTRAINT uq_invspecialrollingpolicies ;
ALTER TABLE t_invspecialrollingpolicies
	ADD CONSTRAINT uq_invspecialrollingpolicies
		UNIQUE (inventoryname, inventoryversion, action, storageid) ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'pds-12.15.00' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-12.15.00',
		'2020-10-29',
		3,
		2,
		'PDS_12-15-0',
		'Added new fields on t_invrollingpolicies table'
) ;


