
-- Version 11.9.0
-- POSTGRES








--------------------------------------------------------------------------------
-------------------------------- TABLES CREATION -------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_sm_missing
*/ ------------------------------
CREATE TABLE t_sm_missing (
    id SERIAL NOT NULL,
    inv_id BIGINT NOT NULL,
	sm_id INTEGER NOT NULL,
	tstamp TIMESTAMP(6) DEFAULT now() NOT NULL
) ;

ALTER TABLE t_sm_missing
	ADD CONSTRAINT pk_t_sm_missing
		PRIMARY KEY (id) ;

ALTER TABLE t_sm_missing 
	ADD CONSTRAINT uq_sm_missing01
		UNIQUE (inv_id, sm_id) ;

ALTER TABLE t_sm_missing
	ADD CONSTRAINT fk_t_sm_missing1
		FOREIGN KEY (inv_id)
			REFERENCES t_inventory (id_inv)
				ON DELETE CASCADE ;

ALTER TABLE t_sm_missing
	ADD CONSTRAINT fk_t_sm_missing2
		FOREIGN KEY (sm_id)
			REFERENCES t_storagemanager (id)
				ON DELETE CASCADE ;

CREATE INDEX ix_t_sm_missing01 ON t_sm_missing USING BTREE (inv_id) ;
CREATE INDEX ix_t_sm_missing02 ON t_sm_missing USING BTREE (sm_id) ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'pds-11.9.0' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-11.9.0',
		'2015-09-24',
		3,
		2,
		'PDS_11-9-0',
		'Added table t_sm_missing.'
) ;


