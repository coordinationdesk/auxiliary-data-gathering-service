
-- SRV Version 1.9.1
-- POSTGRES








--------------------------------------------------------------------------------
-------------------------------- TABLES CHANGES --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_srv_users
*/ ------------------------------
ALTER TABLE t_srv_users ADD service_alias VARCHAR(64) ;








--------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ---------------------------------
--------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'srv-01.09.01' ;

INSERT INTO t_schema (
	version,
	modificationdate,
	modificationtype,
	schematype,
	cvstag,
	description )
VALUES  (
	'srv-01.09.01',
	'2021-11-25',
	6,
	35,
	'SRV_1-9-1',
	'Added service_alias field on t_srv_users table'
) ;


