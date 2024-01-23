
-- SRV Version 1.7.1
-- POSTGRES





-------------------------------------------------------------------------------
-------------------------------- TABLES UPDATE --------------------------------
-------------------------------------------------------------------------------

/* ------------------------------
	t_srv_statuses
*/ ------------------------------
-- Added new status 'COLLAPSED' into TRANSACTION schema








-------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS --------------------------------
-------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'srv-01.07.01' ;

INSERT INTO t_schema (
	version,
	modificationdate,
	modificationtype,
	schematype,
	cvstag,
	description )
VALUES  (
	'srv-01.07.01',
	'2021-05-20',
	6,
	35,
	'SRV_1-7-1',
	'Added new status (LTA-299)'
) ;



