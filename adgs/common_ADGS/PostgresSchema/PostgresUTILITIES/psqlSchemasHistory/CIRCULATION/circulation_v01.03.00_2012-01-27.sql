
-- Version 1.3.0
-- POSTGRES





---------------------------------------------------------------------------------
-------------------------------- TABLES CREATION --------------------------------
---------------------------------------------------------------------------------

------------------------------
-- t_knownhosts_limits
------------------------------
CREATE TABLE t_knownhosts_limits (
	circulator_node_id INTEGER NOT NULL,
	dest_host_id INTEGER NOT NULL,
	max_upload_slots INTEGER NOT NULL
) ;
ALTER TABLE t_knownhosts_limits 
	ADD CONSTRAINT pk_t_knownhosts_limits
		PRIMARY KEY (circulator_node_id,dest_host_id) ;

ALTER TABLE t_knownhosts_limits
  ADD CONSTRAINT ck_knownhosts_limits01
  	CHECK (max_upload_slots > 0) ;

ALTER TABLE t_knownhosts_limits 
	ADD CONSTRAINT fk_knownhosts_limits01
		FOREIGN KEY (circulator_node_id) REFERENCES t_knownhosts (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_knownhosts_limits 
	ADD CONSTRAINT fk_knownhosts_limits02
		FOREIGN KEY (dest_host_id) REFERENCES t_knownhosts (id) 
			ON DELETE CASCADE ;








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
		'circulation-1.3.0',
		'2012-01-27',
		6,
		20,
		'CIRCULATION_1-3-0',
		'Added table t_knownhosts_limits.'
) ;


