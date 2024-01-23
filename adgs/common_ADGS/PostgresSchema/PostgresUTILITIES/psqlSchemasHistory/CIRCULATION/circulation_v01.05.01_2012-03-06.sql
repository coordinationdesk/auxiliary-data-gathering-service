
-- Version 1.5.1
-- POSTGRES





---------------------------------------------------------------------------------
-------------------------------- TABLES CREATION --------------------------------
---------------------------------------------------------------------------------

------------------------------
-- t_circulationstats
------------------------------
ALTER TABLE t_circulationpolicies ADD priority INTEGER NOT NULL DEFAULT 0 ;

------------------------------
-- t_circulationstats
------------------------------
ALTER TABLE t_circulationstats DROP CONSTRAINT fk_circulationstats01 ;
ALTER TABLE t_circulationstats ALTER COLUMN id TYPE BIGSERIAL ;
ALTER TABLE t_circulationstats DROP COLUMN speed ;
ALTER TABLE t_circulationstats ADD circulationsqueue_id BIGINT ;
ALTER TABLE t_circulationstats ADD datasize BIGINT ;
ALTER TABLE t_circulationstats 
	ADD CONSTRAINT fk_circulationstats01
		FOREIGN KEY (circulationsqueue_id) REFERENCES t_circulationsqueue (id) 
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
		'circulation-1.5.1',
		'2012-03-06',
		6,
		20,
		'CIRCULATION_1-5-1',
		'Modified t_circulationpolicies and t_circulationstats tables.'
) ;


