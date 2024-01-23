
-- Version 10.5.1
-- POSTGRES








---------------------------------------------------------------------------------
------------------------------ TABLES MODIFICATION ------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_smtype
*/ ------------------------------
ALTER TABLE t_smtype ALTER COLUMN id TYPE INT2 ;
DROP SEQUENCE IF EXISTS t_smtype_id_seq CASCADE ;

UPDATE t_smtype SET id = 1 WHERE type = 'ACSHSM' ;
UPDATE t_smtype SET id = 2 WHERE type = 'ACSISM' ;
UPDATE t_smtype SET id = 3 WHERE type = 'STAFSM' ;
UPDATE t_smtype SET id = 4 WHERE type = 'AMSSM' ;
UPDATE t_smtype SET id = 5 WHERE type = 'SatStore' ;








---------------------------------------------------------------------------------
--------------------------------- TABLES INSERT ---------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_stati
*/ ------------------------------
INSERT INTO t_stati (id, status) VALUES (30, 'INVALID') ;

/* ------------------------------
	t_smtype
*/ ------------------------------
INSERT INTO t_smtype (id, type) VALUES (6,'DISCACHE');
INSERT INTO t_smtype (id, type) VALUES (7,'UMARF');
INSERT INTO t_smtype (id, type) VALUES (8,'ACSLTA');








---------------------------------------------------------------------------------
-------------------------------- TABLES CREATION --------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_acsltainvrelation
*/ ------------------------------
CREATE TABLE t_acsltainvrelation (
	id INTEGER NOT NULL,
	productid VARCHAR(512) NOT NULL,
	signature VARCHAR(64)
) ;
ALTER TABLE t_acsltainvrelation
	ADD CONSTRAINT pk_t_acsltainvrelation
		PRIMARY KEY (id) ;

ALTER TABLE t_acsltainvrelation 
	ADD CONSTRAINT fk_acsltainvrelation1
		FOREIGN KEY (id)
			REFERENCES t_inventorysm (id) 
				ON DELETE CASCADE ;








---------------------------------------------------------------------------------
----------------------------------- TRIGGERS ------------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_acsltainvrelation
*/ ------------------------------
CREATE TRIGGER tr_acsltainvrelationinsert AFTER INSERT
	ON t_acsltainvrelation EXECUTE PROCEDURE tf_inserteventtableitem('t_acsltainvrelation', 'INS') ;

CREATE TRIGGER tr_acsltainvrelationupdate AFTER UPDATE
	ON t_acsltainvrelation EXECUTE PROCEDURE tf_inserteventtableitem('t_acsltainvrelation', 'UPD') ;








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
		'pds-10.5.1',
		'2012-11-07',
		3,
		2,
		'PDS_10-5-1',
		'Added new table t_acsltainvrelation.'
) ;
