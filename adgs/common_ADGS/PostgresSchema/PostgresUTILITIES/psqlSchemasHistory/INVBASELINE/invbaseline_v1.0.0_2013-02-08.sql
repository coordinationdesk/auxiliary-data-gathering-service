
-- Version 1.0.0
-- POSTGRES



-- This schema can be installed only on PDS 10.3.0 or greater





---------------------------------------------------------------------------------
-------------------------------- TABLES CREATION --------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_inv_baselines
*/ ------------------------------
CREATE TABLE t_inv_baselines (
	id_inv_baseline SERIAL NOT NULL,
	baseline VARCHAR(24) NOT NULL
) ;

ALTER TABLE t_inv_baselines 
	ADD CONSTRAINT pk_t_inv_baselines
		PRIMARY KEY (id_inv_baseline) ;

ALTER TABLE t_inv_baselines 
	ADD CONSTRAINT uq_inventorybaselines01
		UNIQUE (baseline) ;



/* ------------------------------
	t_inv_x_baselines
*/ ------------------------------
CREATE TABLE t_inv_x_baselines (
	id_inv_x_baseline BIGSERIAL NOT NULL,
	inv_id BIGINT NOT NULL,
	inv_baseline_id INTEGER NOT NULL
) ;

ALTER TABLE t_inv_x_baselines 
	ADD CONSTRAINT pk_t_inv_x_baselines
		PRIMARY KEY (id_inv_x_baseline) ;

ALTER TABLE t_inv_x_baselines 
	ADD CONSTRAINT uq_inv_x_baselines01
		UNIQUE (inv_id, inv_baseline_id) ;

ALTER TABLE t_inv_x_baselines 
	ADD CONSTRAINT fk_inv_x_baselines01
		FOREIGN KEY (inv_id) REFERENCES t_inventory (id_inv) 
			ON DELETE CASCADE ;

ALTER TABLE t_inv_x_baselines 
	ADD CONSTRAINT fk_inv_x_baselines02
		FOREIGN KEY (inv_baseline_id) REFERENCES t_inv_baselines (id_inv_baseline) 
			ON DELETE CASCADE ;

CREATE INDEX ix_t_inv_x_baselines01 ON t_inv_x_baselines USING BTREE (inv_id) ;
CREATE INDEX ix_t_inv_x_baselines02 ON t_inv_x_baselines USING BTREE (inv_baseline_id) ;








---------------------------------------------------------------------------------
--------------------------------- TABLES INSERT ---------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_inv_baselines
*/ ------------------------------
INSERT INTO t_inv_baselines (baseline) VALUES ('DummyBaseline');

/* ------------------------------
	t_dbparameters
*/ ------------------------------
INSERT INTO t_dbparameters (parameter, parametertype_id, value_int) VALUES ('baseline_curr_val', 1, 1) ;








---------------------------------------------------------------------------------
------------------------------ PROCEDURES CREATION ------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	p_inv_x_baselines_insert
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_inv_x_baselines_insert(
	in_invid BIGINT) RETURNS INTEGER AS $$

DECLARE
	bsl_curr_val_id INTEGER ;
	excp_msg VARCHAR(128) ;
	
BEGIN
	-- Check if the baseline variable is setted
	IF ((SELECT count(*) FROM t_dbparameters WHERE parameter = 'baseline_curr_val') = 0) THEN
		RAISE EXCEPTION 'p_inv_x_baselines_insert: no value setted for baseline_curr_val into t_dbparameters' ;
	END IF;
	
	-- Get the id of the baseline setted
	bsl_curr_val_id := (SELECT value_int FROM t_dbparameters WHERE parameter = 'baseline_curr_val') ;
	
	-- Check if the id is setted a valid id
	IF ((SELECT count(*) FROM t_inv_baselines WHERE id_inv_baseline = bsl_curr_val_id) = 0) THEN
		excp_msg := 'p_inv_x_baselines_insert: ' || bsl_curr_val_id || ' - invalid baseline_curr_val value into t_dbparameters' ;
		RAISE EXCEPTION '%', excp_msg ;
	END IF ;
	
	-- Insert the relation into t_inv_x_baselines
	INSERT INTO t_inv_x_baselines (
		inv_id,
		inv_baseline_id )
	VALUES (
		in_invid,
		bsl_curr_val_id
	) ;
	
	RETURN 0 ;
END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
------------------------------- TRIGGER FUNCTIONS -------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	tf_filetype_insert
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_invxbaselines_insert() RETURNS TRIGGER AS $$
DECLARE
  	status INTEGER ;
BEGIN
	status := p_inv_x_baselines_insert(NEW.id_inv) ;
  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
------------------------------- TRIGGER CREATION --------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	tf_inventory_baselines
*/ ------------------------------
CREATE TRIGGER tf_inventory_baselines AFTER INSERT
	ON t_inventory 
		FOR EACH ROW EXECUTE PROCEDURE tf_invxbaselines_insert() ;








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
VALUES  (
	'invbaseline-1.0.0',
	'2013-02-08',
	5,
	23,
	'INVBASELINE_1-0-0',
	'Created invbaseline schema.'
) ;
