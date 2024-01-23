
-- Version 01.01.00
-- POSTGRES





-------------------------------------------------------------------------------
------------------------------- TABLES CHANGES --------------------------------
-------------------------------------------------------------------------------

/* ------------------------------
	t_ltadl_filequeue
*/ ------------------------------
ALTER TABLE t_ltadl_filequeue ADD retries INTEGER NOT NULL DEFAULT 0 ;








-------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS --------------------------------
-------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'ltadl-01.01.00' ;

INSERT INTO t_schema (
	version,
	modificationdate,
	modificationtype,
	schematype,
	cvstag,
	description )
VALUES  (
	'ltadl-01.01.00',
	'2021-03-30',
	6,
	37,
	'LTADL_1-1-0',
	'Added retries field on t_ltadl_filequeue'
) ;


