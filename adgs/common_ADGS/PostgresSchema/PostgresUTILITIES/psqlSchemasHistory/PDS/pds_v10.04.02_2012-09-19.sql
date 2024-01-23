
-- Version 10.4.2
-- POSTGRES








---------------------------------------------------------------------------------
-------------------------------- TABLES CREATION --------------------------------
---------------------------------------------------------------------------------

/* --------------------------
	t_inv_relations
*/ --------------------------
ALTER TABLE t_inv_relations ALTER COLUMN inv_id2 DROP NOT NULL ;
CREATE INDEX ix_t_inv_relations02 ON t_inv_relations USING btree (inv_id1) ;
CREATE INDEX ix_t_inv_relations03 ON t_inv_relations USING btree (inv_id2) ;








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
		'pds-10.4.2',
		'2012-09-19',
		3,
		2,
		'PDS_10-4-2',
		'Added new indexes on t_inv_relations.'
) ;
