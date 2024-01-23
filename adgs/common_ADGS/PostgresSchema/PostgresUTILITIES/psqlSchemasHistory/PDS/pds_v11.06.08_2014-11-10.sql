
-- Version 11.6.8
-- POSTGRES








---------------------------------------------------------------------------------
--------------------------- TRIGGER FUNCTUION CHANGES ---------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	tf_smactionsupdate
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_smactionsupdate() 
	RETURNS TRIGGER AS $$
BEGIN
	IF OLD.packageid IS NOT NULL AND NEW.packageid IS NOT NULL THEN
		RAISE EXCEPTION 'tf_smactionsUpdate: Cannot assign a new packageid in a record having an old packageid';
	END IF;
	
	PERFORM p_smactionsupdate(NEW.filename, NEW.fileversion, NEW.packageid) ;
	PERFORM p_events('t_smactions', 'UPD') ;
	
  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
-------------------------- TRIGGER FUNCTUION CREATION ---------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	tf_smactionsinsert
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_smactionsinsert() 
	RETURNS TRIGGER AS $$
BEGIN
	IF NEW.packageid IS NOT NULL THEN
		RAISE EXCEPTION 'tf_smactionsInsert: Cannot insert a new record having a packageid';
	END IF;
	
	PERFORM p_events('t_smactions', 'INS') ;
	
  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
-------------------------------- TRIGGER CHANGES --------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_smactions
*/ ------------------------------
DROP TRIGGER tr_smactionsinsert ON t_smactions ;
CREATE TRIGGER tr_smactionsinsert AFTER INSERT
	ON t_smactions 
		FOR EACH ROW EXECUTE PROCEDURE tf_smactionsinsert() ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------
DELETE FROM t_schema WHERE version = 'pds-11.6.8' ;
INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-11.6.8',
		'2014-11-10',
		3,
		2,
		'PDS_11-6-8',
		'Added packageid control on t_smactions.'
) ;


