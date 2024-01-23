
-- Version 10.5.2
-- POSTGRES








---------------------------------------------------------------------------------
------------------------------ TABLES MODIFICATION ------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_distributionpolicieslock
*/ ------------------------------
ALTER TABLE t_distributionpolicieslock ALTER COLUMN responsible TYPE VARCHAR(128) ;

/* ------------------------------
	t_receptionpolicieslock
*/ ------------------------------
ALTER TABLE t_receptionpolicieslock ALTER COLUMN responsible TYPE VARCHAR(128) ;

/* ------------------------------
	t_storagemanager
*/ ------------------------------
ALTER TABLE t_storagemanager ADD allowdefaultcost BOOLEAN DEFAULT 't' NOT NULL ;

/* ------------------------------
	t_stati
*/ ------------------------------
INSERT INTO t_stati (id, status) VALUES (31, 'LTA_DOWNLOADING') ;









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
		'pds-10.5.2',
		'2012-12-12',
		3,
		2,
		'PDS_10-5-2',
		'Added new field on t_storagemanager and new record in t_stati.'
) ;
