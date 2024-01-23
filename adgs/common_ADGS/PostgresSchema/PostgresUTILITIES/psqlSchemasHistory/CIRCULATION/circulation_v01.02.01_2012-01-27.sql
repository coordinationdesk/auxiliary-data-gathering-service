
-- Version 1.2.1
-- POSTGRES





---------------------------------------------------------------------------------
------------------------------ TABLES MODIFICATION ------------------------------
---------------------------------------------------------------------------------

------------------------------
-- t_circulationsqueue
------------------------------
ALTER TABLE t_circulationsqueue ADD dest_host_id INTEGER ;

ALTER TABLE t_circulationsqueue 
	ADD CONSTRAINT fk_circulationsqueue06
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
		'circulation-1.2.1',
		'2012-01-26',
		6,
		20,
		'CIRCULATION_1-2-1',
		'Added field dest_host_id on t_circulationsqueue.'
) ;


