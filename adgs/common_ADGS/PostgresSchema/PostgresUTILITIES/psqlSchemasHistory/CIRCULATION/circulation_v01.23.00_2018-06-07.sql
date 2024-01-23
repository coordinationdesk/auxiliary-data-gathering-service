
-- Version 1.23.0
-- POSTGRES







--------------------------------------------------------------------------------
-------------------------------- TABLES CHANGES --------------------------------
--------------------------------------------------------------------------------


/* ------------------------------
	t_circ_callbacks
*/ ------------------------------


ALTER TABLE t_circ_callbacks 
	ADD CONSTRAINT uq_t_circ_callbacks01
		UNIQUE (circ_queue_id) ;







--------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ---------------------------------
--------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'circulation-1.23.0' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'circulation-1.23.0',
		'2018-06-07',
		6,
		20,
		'CIRCULATION_1-23-0',
		'Added new unique constraint on t_circ_callbacks'
) ;

