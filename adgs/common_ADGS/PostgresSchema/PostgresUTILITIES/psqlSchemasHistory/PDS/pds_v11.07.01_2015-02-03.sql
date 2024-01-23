
-- Version 11.7.1
-- POSTGRES








---------------------------------------------------------------------------------
--------------------------------- TABLE CHANGES ---------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_qualifiedflag
*/ ------------------------------
INSERT INTO t_qualifiedflag ("name") VALUES ('PASSED') ;
INSERT INTO t_qualifiedflag ("name") VALUES ('DEGRADED') ;
INSERT INTO t_qualifiedflag ("name") VALUES ('FAILED') ;


/* ------------------------------
	t_stati
*/ ------------------------------
UPDATE t_stati SET status = 'MEDIA_ACKNOWLEDGE' WHERE status = 'MEDIA_ACKNOLEDGE' ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------
DELETE FROM t_schema WHERE version = 'pds-11.7.1' ;
INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-11.7.1',
		'2015-02-03',
		3,
		2,
		'PDS_11-7-1',
		'Added new qualified flags. Fixed bug on "MEDIA_ACKNOWLEDGE" status.'
) ;


