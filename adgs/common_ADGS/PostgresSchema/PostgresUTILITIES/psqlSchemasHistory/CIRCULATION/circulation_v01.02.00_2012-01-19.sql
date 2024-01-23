
-- Version 1.2.0
-- POSTGRES





---------------------------------------------------------------------------------
------------------------------ TABLES MODIFICATION ------------------------------
---------------------------------------------------------------------------------

------------------------------
-- t_knownhosts
------------------------------
ALTER TABLE t_knownhosts ALTER COLUMN ip_address DROP NOT NULL;
ALTER TABLE t_knownhosts ALTER COLUMN host_name TYPE VARCHAR(256);








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
		'circulation-1.2.0',
		'2012-01-19',
		6,
		20,
		'CIRCULATION_1-2-0',
		'Changed columns ip_address and host_name on table t_knownhosts.'
) ;


