
-- Version 1.4.1
-- POSTGRES





---------------------------------------------------------------------------------
------------------------------ TABLES MODIFICATION ------------------------------
---------------------------------------------------------------------------------

------------------------------
-- t_knownhosts_limits
------------------------------
ALTER TABLE t_knownhosts_limits ADD id SERIAL NOT NULL;

ALTER TABLE t_knownhosts_limits DROP CONSTRAINT pk_t_knownhosts_limits;

ALTER TABLE t_knownhosts_limits 
	ADD CONSTRAINT pk_t_knownhosts_limits
		PRIMARY KEY (id) ;

ALTER TABLE t_knownhosts_limits 
	ADD CONSTRAINT uq_knownhosts_limits01
		UNIQUE (circulator_node_id,dest_host_id) ;








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
		'circulation-1.4.1',
		'2012-02-14',
		6,
		20,
		'CIRCULATION_1-4-1',
		'Added serial id on table t_knownhosts_limits.'
) ;


