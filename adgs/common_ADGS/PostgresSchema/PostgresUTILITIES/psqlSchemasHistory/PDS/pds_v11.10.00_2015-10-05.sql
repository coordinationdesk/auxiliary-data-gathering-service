
-- Version 11.10.00
-- POSTGRES








--------------------------------------------------------------------------------
-------------------------------- TABLES CREATION -------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_sm_pendingerase
*/ ------------------------------
CREATE TABLE t_sm_pendingerase (
    id SERIAL NOT NULL,
    invsm_id INTEGER NOT NULL,
	tstamp TIMESTAMP(6) DEFAULT NOW() NOT NULL
) ;
ALTER TABLE t_sm_pendingerase
	ADD CONSTRAINT pk_t_sm_pendingerase
		PRIMARY KEY (id) ;

ALTER TABLE t_sm_pendingerase
	ADD CONSTRAINT fk_sm_pendingerase01
		FOREIGN KEY (invsm_id)
			REFERENCES t_inventorysm (id)
				ON DELETE CASCADE ;

CREATE INDEX ix_t_sm_pendingerase01 ON t_sm_pendingerase USING BTREE (invsm_id) ;

COMMENT ON TABLE t_sm_pendingerase IS 'Contains the list of the failed and still pending erase operations performed on synch storage managers';
COMMENT ON COLUMN t_sm_pendingerase.id IS 'Record identifier (Primary Key)';
COMMENT ON COLUMN t_sm_pendingerase.invsm_id IS 'Identifies to the file that was impossible to erase in a specific Storage Manager. References the t_inventorysm table';
COMMENT ON COLUMN t_sm_pendingerase.tstamp IS 'When the record has been inserted (default value NOW)';








--------------------------------------------------------------------------------
-------------------------------- TABLES CHANGES --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_inventorysm
*/ ------------------------------
ALTER TABLE t_inventorysm ALTER COLUMN inv_id DROP NOT NULL ;

/* ------------------------------
	t_inventorydataset
*/ ------------------------------
CREATE INDEX ix_t_inventorydataset01 ON t_inventorydataset USING BTREE (filename, fileversion) ;

/* ------------------------------
	t_inventorygaps
*/ ------------------------------
CREATE INDEX ix_t_inventorygaps01 ON t_inventorygaps USING BTREE (filename, fileversion) ;
DROP INDEX ix_t_inventorygaps1 ;
CREATE INDEX ix_t_inventorygaps02 ON t_inventorygaps USING BTREE (begin) ;
DROP INDEX ix_t_inventorygaps2 ;
CREATE INDEX ix_t_inventorygaps03 ON t_inventorygaps USING BTREE ("end") ;

/* ------------------------------
	t_invsiterelation
*/ ------------------------------
CREATE INDEX ix_t_invsiterelation01 ON t_invsiterelation USING BTREE (filename, fileversion) ;

/* ------------------------------
	t_ordersattachments
*/ ------------------------------
CREATE INDEX ix_t_ordersattachments01 ON t_ordersattachments USING BTREE (orderid) ;








--------------------------------------------------------------------------------
-------------------------- TRIGGER FUNCTIONS CREATION --------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	tf_inventorysm_check_invid
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_inventorysm_check_invid() RETURNS TRIGGER AS $$

BEGIN	
	IF NEW.inv_id IS NULL THEN 
		RAISE EXCEPTION 'tf_inventorysm_check_invid: Cannot insert a new record having null inv_id';
	END IF ;
	
	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;








--------------------------------------------------------------------------------
-------------------------- TRIGGER FUNCTIONS CREATION --------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_inventorysm
*/ ------------------------------
CREATE TRIGGER tr_inventorysm_ins_bfr BEFORE INSERT
	ON t_inventorysm 
		FOR EACH ROW EXECUTE PROCEDURE tf_inventorysm_check_invid('INS') ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'pds-11.10.00' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-11.10.00',
		'2015-10-05',
		3,
		2,
		'PDS_11-10-0',
		'Added table t_sm_pendingerase. Removed NOT NULL constrain on inv_id on t_inventorysm.'
) ;


