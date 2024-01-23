
-- Version 1.8.2
-- POSTGRES








---------------------------------------------------------------------------------
----------------------------- TRIGGERS MODIFICATION -----------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_circulationsqueue
*/ ------------------------------
DROP TRIGGER IF EXISTS tr_circulationsqueue_update1 ON t_circulationsqueue ;








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
		'circulation-1.8.2',
		'2014-01-23',
		6,
		20,
		'CIRCULATION_1-8-2',
		'Dropped trigger tr_circulationsqueue_update1 to disable state transition control'
) ;


