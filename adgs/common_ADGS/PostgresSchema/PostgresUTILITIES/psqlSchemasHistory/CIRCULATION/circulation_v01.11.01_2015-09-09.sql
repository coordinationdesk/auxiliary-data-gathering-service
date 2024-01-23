
-- Version 1.11.1
-- POSTGRES








--------------------------------------------------------------------------------
-------------------------------- TABLES CHANGES --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_circulationsmedia 
*/ ------------------------------
ALTER TABLE t_circulationsmedia ADD source BOOLEAN DEFAULT 't' NOT NULL ;
ALTER TABLE t_circulationsmedia DROP CONSTRAINT uq_circulationsmedia01 ;
ALTER TABLE t_circulationsmedia 
	ADD CONSTRAINT uq_circulationsmedia01
		UNIQUE (media_tag, source) ;


/* ------------------------------
	t_circulationsqueue
*/ ------------------------------
ALTER TABLE t_circulationsqueue ALTER COLUMN dest_protocol_id DROP NOT NULL ;








--------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ---------------------------------
--------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'circulation-1.11.1' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'circulation-1.11.1',
		'2015-09-09',
		6,
		20,
		'CIRCULATION_1-11-1',
		'Added field source on table t_circulationsmedia. Removed not null constraint to dest_protocol_id on t_circulationsqueue.'
) ;


