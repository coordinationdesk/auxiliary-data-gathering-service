
-- Version 1.14.1
-- POSTGRES








--------------------------------------------------------------------------------
-------------------------------- TABLES CHANGES --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_datastoragequeue
*/ ------------------------------
ALTER TABLE t_datastoragequeue ADD startdate TIMESTAMP(6) DEFAULT NOW() ;
UPDATE t_datastoragequeue SET startdate = NULL ;








--------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ---------------------------------
--------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'circulation-1.14.1' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'circulation-1.14.1',
		'2016-06-30',
		6,
		20,
		'CIRCULATION_1-14-1',
		'Added field startdate on table t_datastoragequeue'
) ;


