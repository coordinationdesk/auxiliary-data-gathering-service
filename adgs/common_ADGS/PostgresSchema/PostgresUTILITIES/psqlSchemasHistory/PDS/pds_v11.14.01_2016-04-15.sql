
-- Version 11.14.01
-- POSTGRES








--------------------------------------------------------------------------------
-------------------------------- TABLES CHANGES --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_smactions
*/ ------------------------------
CREATE INDEX ix_t_smactions01 ON t_smactions USING BTREE (inv_id) ;
CREATE INDEX ix_t_smactions02 ON t_smactions USING BTREE (smid) ;
CREATE INDEX ix_t_smactions03 ON t_smactions USING BTREE (packageid) ;

/* ------------------------------
	t_smqueue
*/ ------------------------------
CREATE INDEX ix_t_smqueue01 ON t_smqueue USING BTREE (smid) ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'pds-11.14.01' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-11.14.01',
		'2016-04-15',
		3,
		2,
		'PDS_11-14-1',
		'Added indexes on t_smactions and t_smqueue tables.'
) ;


