
-- Version 1.6.5
-- POSTGRES








---------------------------------------------------------------------------------
------------------------------ TABLES MODIFICATION ------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_circulationpolicies
*/ ------------------------------
ALTER TABLE t_circulationpolicies ADD tempnamevalue VARCHAR(64) ;


/* ------------------------------
	t_circulationsqueue
*/ ------------------------------
ALTER TABLE t_circulationsqueue ADD tempnamevalue VARCHAR(64) ;


/* ------------------------------
	t_circulationusers
*/ ------------------------------
ALTER TABLE t_circulationusers ADD tempnamevalue VARCHAR(64) ;
ALTER TABLE t_circulationusers ADD tempnamemode_id INT2 ;
ALTER TABLE t_circulationusers 
	ADD CONSTRAINT fk_circulationusers03
		FOREIGN KEY (tempnamemode_id) REFERENCES t_circulationtempnames (id) 
			ON DELETE CASCADE ;







---------------------------------------------------------------------------------
--------------------------------- TABLES UPDATE ---------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_circulationtempnames
*/ ------------------------------
UPDATE t_circulationtempnames SET tempname = 'PREFIX' WHERE tempname = UPPER('prefix_dot') ;
UPDATE t_circulationtempnames SET tempname = 'SUFFIX' WHERE tempname = UPPER('suffix_lock') ;
UPDATE t_circulationtempnames SET tempname = 'TMPDIR' WHERE tempname = UPPER('tmp_dir') ;








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
		'circulation-1.6.5',
		'2013-03-21',
		6,
		20,
		'CIRCULATION_1-6-5',
		'Added field tempnamevalue on t_circulationpolicies, t_circulationsqueue and t_circulationusers tables. Changed values into t_circulationtempnames.'
) ;
