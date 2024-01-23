
-- Version 11.15.00
-- POSTGRES








--------------------------------------------------------------------------------
-------------------------------- TABLES CHANGES --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_inventory
*/ ------------------------------
ALTER TABLE t_inventory DROP COLUMN IF EXISTS data CASCADE ;
ALTER TABLE t_inventory DROP COLUMN IF EXISTS blobsize CASCADE ;
DROP TRIGGER IF EXISTS tr_oid_inventory ON t_inventory ;


/* ------------------------------
	t_inv_relations
*/ ------------------------------
ALTER TABLE t_inv_relations DROP CONSTRAINT IF EXISTS fk_inv_relations02 ;

ALTER TABLE t_inv_relations
	ADD CONSTRAINT fk_inv_relations02 FOREIGN KEY (inv_id2)
		REFERENCES t_inventory (id_inv) MATCH SIMPLE
			ON UPDATE NO ACTION ON DELETE SET NULL ;


/* ------------------------------
	t_asyncsm_operations
*/ ------------------------------
CREATE INDEX ix_t_asyncsm_operations01 ON t_asyncsm_operations USING BTREE (operation_id) ;
CREATE INDEX ix_t_asyncsm_operations02 ON t_asyncsm_operations USING BTREE (sm_id) ;
CREATE INDEX ix_t_asyncsm_operations03 ON t_asyncsm_operations USING BTREE (status_id) ;


/* ------------------------------
	t_invrollingpolicies
*/ ------------------------------
CREATE INDEX ix_t_invrollingpolicies01 ON t_invrollingpolicies USING BTREE (filetype) ;
CREATE INDEX ix_t_invrollingpolicies02 ON t_invrollingpolicies USING BTREE (algorithm) ;
CREATE INDEX ix_t_invrollingpolicies03 ON t_invrollingpolicies USING BTREE (action) ;
CREATE INDEX ix_t_invrollingpolicies04 ON t_invrollingpolicies USING BTREE (fileclass) ;
CREATE INDEX ix_t_invrollingpolicies05 ON t_invrollingpolicies USING BTREE (storageid) ;


/* ------------------------------
	t_invspecialrollingpolicies
*/ ------------------------------
CREATE INDEX ix_t_invspecialrollingpol01 ON t_invspecialrollingpolicies USING BTREE (inventoryname, inventoryversion) ;
CREATE INDEX ix_t_invspecialrollingpol02 ON t_invspecialrollingpolicies USING BTREE (action) ;
CREATE INDEX ix_t_invspecialrollingpol03 ON t_invspecialrollingpolicies USING BTREE (storageid) ;


/* ------------------------------
	t_mediacapformat
*/ ------------------------------
CREATE INDEX ix_t_mediacapformat01 ON t_mediacapformat USING BTREE (mediatype) ;


/* ------------------------------
	t_temporaryorders
*/ ------------------------------
CREATE INDEX ix_t_temporaryorders01 ON t_temporaryorders USING BTREE (filename, fileversion) ;
CREATE INDEX ix_t_temporaryorders02 ON t_temporaryorders USING BTREE (ordersat) ;








---------------------------------------------------------------------------------
------------------------------ PROCEDURES CREATION ------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	p_move_seq_to_max
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_move_seq_to_max(
	tb_name VARCHAR(64)) RETURNS INTEGER AS $$

DECLARE
	fld_name VARCHAR(64) ;
	row_cnt INTEGER ;
	
BEGIN
	-- Get, if exists, the name of serial column
	SELECT column_name INTO fld_name FROM information_schema.columns
		WHERE table_name = tb_name AND column_default SIMILAR TO '%(nexval|_seq|regclass)%';

	-- Check if the table contains some rows 
	EXECUTE 'SELECT count(*) FROM ' || tb_name INTO row_cnt ;

	-- If the table has a serial column and has some records, ...
	IF (fld_name IS NOT NULL) AND (row_cnt > 0) THEN
		-- update the start value of the sequence to the max value contained into the table 
		EXECUTE 'SELECT pg_catalog.setval(' || '''' || tb_name || '_' || fld_name || '_seq' || '''' || ', (SELECT MAX(' || fld_name || ') FROM ' || tb_name || '), true)' ;
	END IF ;
	
	RETURN 0;
	
END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'pds-11.14.02' ;
DELETE FROM t_schema WHERE version = 'pds-11.15.00' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-11.15.00',
		'2016-05-04',
		3,
		2,
		'PDS_11-15-0',
		'Removed blob and blobsize from t_inventory. Changed constraint fk_inv_relations02 to SET NULL. Added procedure p_move_seq_to_max'
) ;


