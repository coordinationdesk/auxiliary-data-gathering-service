
-- Version 1.8.4
-- POSTGRES








---------------------------------------------------------------------------------
------------------------------ TABLES MODIFICATION ------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_circulationstats
*/ ------------------------------
ALTER TABLE t_circulationstats ADD unkn_from_host VARCHAR(256) ;
ALTER TABLE t_circulationstats ADD unkn_to_host VARCHAR(256) ;

UPDATE t_circulationstats SET unkn_from_host = 'dummy' WHERE from_host_id IS NULL ;
UPDATE t_circulationstats SET unkn_to_host = 'dummy' WHERE to_host_id IS NULL ;

ALTER TABLE t_circulationstats
	ADD CONSTRAINT ck_circulationstats01			
		CHECK ((from_host_id IS NOT NULL AND unkn_from_host IS NULL) OR (from_host_id IS NULL AND unkn_from_host IS NOT NULL));

ALTER TABLE t_circulationstats
	ADD CONSTRAINT ck_circulationstats02			
		CHECK ((to_host_id IS NOT NULL AND unkn_to_host IS NULL) OR (to_host_id IS NULL AND unkn_to_host IS NOT NULL));








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
		'circulation-1.8.4',
		'2014-02-05',
		6,
		20,
		'CIRCULATION_1-8-4',
		'Added new fields and constraints to t_circulationstats.'
) ;


