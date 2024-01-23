
-- Version 1.7.2
-- POSTGRES








---------------------------------------------------------------------------------
------------------------------ TABLES MODIFICATION ------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_circulationpolicies
*/ ------------------------------
ALTER TABLE t_circulationpolicies 
	ADD CONSTRAINT uq_circulationpolicies01
		UNIQUE (filetype_id,circulator_node_id,source_id,destination_id) ;

/* ------------------------------
	t_circulationsqueue
*/ ------------------------------
ALTER TABLE t_circulationsqueue DROP CONSTRAINT ck_circulationsqueue03 ;
ALTER TABLE t_circulationsqueue
	ADD CONSTRAINT ck_circulationsqueue03
		CHECK ((tempname_id = 3 AND (tempnamevalue = '.' OR tempnamevalue = './')) != TRUE) ;








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
		'circulation-1.7.2',
		'2013-10-04',
		6,
		20,
		'CIRCULATION_1-7-2',
		'Added unique constraint on t_circulationpolicies and changed check constraint on t_circulationsqueue.'
) ;


