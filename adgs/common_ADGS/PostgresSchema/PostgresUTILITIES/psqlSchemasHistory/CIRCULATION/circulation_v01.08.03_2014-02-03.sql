
-- Version 1.8.3
-- POSTGRES








---------------------------------------------------------------------------------
----------------------------- TRIGGERS MODIFICATION -----------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_circulationsqueue
*/ ------------------------------
CREATE INDEX ix_t_circulationsqueue07 ON t_circulationsqueue USING BTREE (fromurl) ;
CREATE INDEX ix_t_circulationsqueue08 ON t_circulationsqueue USING BTREE (tourl) ;








---------------------------------------------------------------------------------
----------------------------- TRIGGERS MODIFICATION -----------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_circulationsqueue
*/ ------------------------------
DROP TRIGGER IF EXISTS tr_circulationsqueue_insert1 ON t_circulationsqueue ;








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
		'circulation-1.8.3',
		'2014-02-03',
		6,
		20,
		'CIRCULATION_1-8-3',
		'Added new indexes on t_circulationsqueue. Dropped trigger tr_circulationsqueue_insert1 to disable start status control.'
) ;


