
-- Version 1.6.7
-- POSTGRES








---------------------------------------------------------------------------------
------------------------------ TABLES MODIFICATION ------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_circulationsqueue
*/ ------------------------------
ALTER TABLE t_circulationsqueue ADD tempname_id SMALLINT ;
ALTER TABLE t_circulationsqueue 
	ADD CONSTRAINT fk_circulationsqueue07
		FOREIGN KEY (tempname_id) REFERENCES t_circulationtempnames (id) 
			ON DELETE CASCADE ;
ALTER TABLE t_circulationsqueue
	ADD CONSTRAINT ck_circulationsqueue01			
		CHECK (char_length(trim(tempnamevalue)) != 0) ;
ALTER TABLE t_circulationsqueue
	ADD CONSTRAINT ck_circulationsqueue02
		CHECK ((tempname_id = 3 AND (tempnamevalue = '.' OR tempnamevalue = './' OR tempnamevalue = '..' OR tempnamevalue = '../')) != TRUE) ;


/* ------------------------------
	t_circulationpolicies
*/ ------------------------------
ALTER TABLE t_circulationpolicies ALTER COLUMN tempname_id SET DEFAULT 1 ;
ALTER TABLE t_circulationpolicies ALTER COLUMN tempnamevalue SET DEFAULT '.' ;

ALTER TABLE t_circulationpolicies ALTER COLUMN tempnamevalue SET NOT NULL ;
ALTER TABLE t_circulationpolicies
	ADD CONSTRAINT ck_circulationpolicies01			
		CHECK (char_length(trim(tempnamevalue)) != 0) ;
ALTER TABLE t_circulationpolicies
	ADD CONSTRAINT ck_circulationpolicies02
		CHECK ((tempname_id = 3 AND (tempnamevalue = '.' OR tempnamevalue = './' OR tempnamevalue = '..' OR tempnamevalue = '../')) != TRUE) ;

ALTER TABLE t_circulationpolicies ALTER COLUMN tempname_id DROP DEFAULT ;
ALTER TABLE t_circulationpolicies ALTER COLUMN tempnamevalue DROP DEFAULT ;


/* ------------------------------
	t_circulationusers
*/ ------------------------------
ALTER TABLE t_circulationusers
	ADD CONSTRAINT ck_circulationusers01			
		CHECK (char_length(trim(tempnamevalue)) != 0) ;
ALTER TABLE t_circulationusers
	ADD CONSTRAINT ck_circulationusers02
		CHECK ((tempnamemode_id = 3 AND (tempnamevalue = '.' OR tempnamevalue = './' OR tempnamevalue = '..' OR tempnamevalue = '../')) != TRUE) ;








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
		'circulation-1.6.7',
		'2013-06-12',
		6,
		20,
		'CIRCULATION_1-6-7',
		'Added tempname_id field on t_circulationsqueue. Added check constraints on t_circulationsqueue, t_circulationpolicies and t_circulationusers tables.'
) ;


