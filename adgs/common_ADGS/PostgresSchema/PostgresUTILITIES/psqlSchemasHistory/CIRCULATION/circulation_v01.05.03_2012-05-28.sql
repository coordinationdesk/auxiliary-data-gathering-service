
-- Version 1.5.3
-- POSTGRES








---------------------------------------------------------------------------------
--------------------------------- TABLES MODIFICATION ---------------------------------
---------------------------------------------------------------------------------
ALTER TABLE t_circulationsqueue ADD error VARCHAR(512) DEFAULT '' NOT NULL;
ALTER TABLE t_dsqueue_x_cirqueue ADD error VARCHAR(512) DEFAULT '' NOT NULL;








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
		'circulation-1.5.3',
		'2012-05-28',
		6,
		20,
		'CIRCULATION_1-5-3',
		'Added error field on t_circulationsqueue and t_dsqueue_x_cirqueue tables.'
) ;
