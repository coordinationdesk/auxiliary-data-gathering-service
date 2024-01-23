
-- Version 1.4.0
-- POSTGRES





---------------------------------------------------------------------------------
-------------------------------- TABLES CREATION --------------------------------
---------------------------------------------------------------------------------

------------------------------
-- t_circulationstats
------------------------------
CREATE TABLE t_circulationstats (
  id BIGINT NOT NULL,
  creationdate TIMESTAMP(6) DEFAULT now(),
  from_host_id INTEGER,
  to_host_id INTEGER,
  speed DOUBLE PRECISION,
  name_lookup_time DOUBLE PRECISION,
  connection_time DOUBLE PRECISION,
  app_connection_time DOUBLE PRECISION,
  pre_transf_time DOUBLE PRECISION,
  start_transf_time DOUBLE PRECISION,
  total_time DOUBLE PRECISION,
  redirect_time DOUBLE PRECISION
) ;
ALTER TABLE t_circulationstats 
	ADD CONSTRAINT pk_t_circulationstats
		PRIMARY KEY (id) ;

ALTER TABLE t_circulationstats 
	ADD CONSTRAINT fk_circulationstats01
		FOREIGN KEY (id) REFERENCES t_circulationsqueue (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_circulationstats 
	ADD CONSTRAINT fk_circulationstats02
		FOREIGN KEY (from_host_id) REFERENCES t_knownhosts (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_circulationstats 
	ADD CONSTRAINT fk_circulationstats03
		FOREIGN KEY (to_host_id) REFERENCES t_knownhosts (id) 
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
		'circulation-1.4.0',
		'2012-01-31',
		6,
		20,
		'CIRCULATION_1-4-0',
		'Added table t_circulationstats.'
) ;


