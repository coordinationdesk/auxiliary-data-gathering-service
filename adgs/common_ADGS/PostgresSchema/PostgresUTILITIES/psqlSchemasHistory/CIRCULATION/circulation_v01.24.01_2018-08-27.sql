
-- Version 1.24.1
-- POSTGRES





--------------------------------------------------------------------------------
-------------------------------- TABLES INSERT --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_flowtags
*/ ------------------------------
INSERT INTO t_flowtags (flowtagname) VALUES ('PROCESSING');
INSERT INTO t_flowtags (flowtagname) VALUES ('DAGS_REQUEST');
INSERT INTO t_flowtags (flowtagname) VALUES ('OTHER');
INSERT INTO t_flowtags (flowtagname) VALUES ('DPC_ARCHIVE');
INSERT INTO t_flowtags (flowtagname) VALUES ('AI2AI_ARCHIVE');
INSERT INTO t_flowtags (flowtagname) VALUES ('OTHER_ARCHIVE');








--------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ---------------------------------
--------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'circulation-1.24.1' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'circulation-1.24.1',
		'2018-08-27',
		6,
		20,
		'CIRCULATION_1-24-1',
		'Added new values into t_flowtags'
) ;


