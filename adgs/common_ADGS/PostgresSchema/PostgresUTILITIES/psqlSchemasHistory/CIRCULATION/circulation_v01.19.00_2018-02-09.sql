
-- Version 1.19.0
-- POSTGRES





--------------------------------------------------------------------------------
-------------------------------- TABLES CHANGES --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_circulationsqueuedwh
*/ ------------------------------
CREATE INDEX ix_t_circulationsqueuedwh07 ON t_circulationsqueuedwh USING BTREE (modificationdate) ;
CREATE INDEX ix_t_circulationsqueuedwh08 ON t_circulationsqueuedwh USING BTREE (host_id) ;




--------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ---------------------------------
--------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'circulation-1.19.0' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'circulation-1.19.0',
		'2018-02-09',
		6,
		20,
		'CIRCULATION_1-19-0',
		'Added new indexes on t_circulationsqueuedwh'
) ;

