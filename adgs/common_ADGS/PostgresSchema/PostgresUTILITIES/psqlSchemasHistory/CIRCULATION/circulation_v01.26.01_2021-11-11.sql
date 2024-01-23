
-- Version 1.26.1
-- POSTGRES








--------------------------------------------------------------------------------
-------------------------------- TABLES CHANGES --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_datastoragequeue 
*/ ------------------------------
DROP INDEX IF EXISTS uq_datastoragequeue01 ;
CREATE UNIQUE INDEX uq_datastoragequeue01 ON t_datastoragequeue 
	USING btree (md5, type_id)
	WHERE status_id NOT IN (3, 8) AND md5 IS NOT NULL ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'circulation-1.26.1' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'circulation-1.26.1',
		'2021-11-11',
		6,
		20,
		'CIRCULATION_1-26-1',
		'Changed uq_datastoragequeue01 unique constraint (S3MARINE-538)'
) ;


