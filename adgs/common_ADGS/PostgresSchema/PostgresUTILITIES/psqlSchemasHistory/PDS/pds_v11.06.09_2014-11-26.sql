
-- Version 11.6.9
-- POSTGRES








---------------------------------------------------------------------------------
--------------------------------- TABLE CHANGES ---------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_dbparameters
*/ ------------------------------
ALTER TABLE t_dbparameters ADD param_description TEXT ;


/* ------------------------------
	t_inventorysm
*/ ------------------------------
ALTER TABLE t_inventorysm ADD migration_report TEXT ;

CREATE INDEX ix_t_inventorysm03 ON t_inventorysm USING BTREE (packageid);


/* ------------------------------
	t_invrollingpolicies
*/ ------------------------------
ALTER TABLE t_invrollingpolicies DROP CONSTRAINT uq_invrollingpolicies ;


/* ------------------------------
	t_rollingpolicyactions
*/ ------------------------------
INSERT INTO t_rollingpolicyactions VALUES (7, 'AUTOPARENT-REMOVECHILDREN') ;


/* ------------------------------
	t_rollingpolicyactions
*/ ------------------------------
INSERT INTO t_transferprotocols (id, protocol) VALUES (6, 'HTTP') ;
INSERT INTO t_transferprotocols (id, protocol) VALUES (7, 'HTTPS') ;








---------------------------------------------------------------------------------
------------------------------ OLD OBJECT REMOTION ------------------------------
---------------------------------------------------------------------------------
DROP TRIGGER tr_smactionsupdate ON t_smactions ;
DROP FUNCTION IF EXISTS tf_smactionsupdate() ;








---------------------------------------------------------------------------------
--------------------------- TRIGGER FUNCTUION CHANGES ---------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	tf_smactions_upd_bfr
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_smactions_upd_bfr() 
	RETURNS TRIGGER AS $$
BEGIN
	IF (OLD.packageid IS NOT NULL AND NEW.packageid IS NOT NULL) AND (OLD.packageid != NEW.packageid) THEN
		RAISE EXCEPTION 'tf_smactions_upd_bfr: Cannot assign a new packageid in a record having an old packageid';
	END IF;
	
	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;

/* ------------------------------
	tf_smactions_upd_aft
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_smactions_upd_aft() 
	RETURNS TRIGGER AS $$
BEGIN
	PERFORM p_smactionsupdate(NEW.filename, NEW.fileversion, NEW.packageid) ;
	PERFORM p_events('t_smactions', 'UPD') ;
	
  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
-------------------------------- TRIGGER CHANGES --------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_smactions
*/ ------------------------------
CREATE TRIGGER tr_smactions_upd_bfr BEFORE UPDATE
	ON t_smactions 
		FOR EACH ROW EXECUTE PROCEDURE tf_smactions_upd_bfr() ;

CREATE TRIGGER tr_smactions_upd_aft AFTER UPDATE
	ON t_smactions 
		FOR EACH ROW EXECUTE PROCEDURE tf_smactions_upd_aft() ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------
DELETE FROM t_schema WHERE version = 'pds-11.6.9' ;
INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-11.6.9',
		'2014-11-26',
		3,
		2,
		'PDS_11-6-9',
		'Added new filed migration_report on t_inventorysm table. Added new rollingpolicyaction and transfer protocols. Fixed bug and splitted into before and after the old tf_smactionsupdate.'
) ;


