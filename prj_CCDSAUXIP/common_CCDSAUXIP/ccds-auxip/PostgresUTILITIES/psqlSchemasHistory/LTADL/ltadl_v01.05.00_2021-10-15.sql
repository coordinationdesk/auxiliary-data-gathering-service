
-- Version 01.05.00
-- POSTGRES





-------------------------------------------------------------------------------
------------------------------- TABLES CHANGES --------------------------------
-------------------------------------------------------------------------------

/* ------------------------------
	t_ltadl_retrievequeue
*/ ------------------------------
ALTER TABLE t_ltadl_retrievequeue ADD retry INTEGER NOT NULL DEFAULT 0 ;
ALTER TABLE t_ltadl_retrievequeue ADD retry_time TIMESTAMP(3) NOT NULL DEFAULT NOW() ;

CREATE INDEX ix_t_ltadl_retrievequeue07 ON t_ltadl_retrievequeue USING BTREE (retry_time) ;








-------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS --------------------------------
-------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'ltadl-01.05.00' ;

INSERT INTO t_schema (
	version,
	modificationdate,
	modificationtype,
	schematype,
	cvstag,
	description )
VALUES  (
	'ltadl-01.05.00',
	'2021-10-15',
	6,
	37,
	'LTADL_1-5-0',
	'Added new fields on t_ltadl_retrievequeue (LTA-388)'
) ;


