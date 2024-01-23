
-- Version 1.26.0
-- POSTGRES








-------------------------------------------------------------------------------
-------------------------------- TABLES CHANGES -------------------------------
-------------------------------------------------------------------------------

/* ------------------------------
        t_dsqueue_x_cirqueue
*/ ------------------------------
ALTER TABLE t_dsqueue_x_cirqueue ADD filequeue_id BIGSERIAL ;








-------------------------------------------------------------------------------
--------------------------------- CONCLUSIONS ---------------------------------
-------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'circulation-1.26.0' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'circulation-1.26.0',
		'2021-04-14',
		6,
		20,
		'CIRCULATION_1-26-0',
		'Added filequeue_id field on t_dsqueue_x_cirqueue table'
) ;


