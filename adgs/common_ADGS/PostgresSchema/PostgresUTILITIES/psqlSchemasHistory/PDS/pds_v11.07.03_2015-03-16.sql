
-- Version 11.7.3
-- POSTGRES








---------------------------------------------------------------------------------
-------------------------------- TABLES CREATION --------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_inventoryextra
*/ ------------------------------
CREATE TABLE t_inventoryextra (
	inv_id BIGINT NOT NULL,
	extra TEXT
) ;

ALTER TABLE t_inventoryextra
	ADD CONSTRAINT pk_t_inventoryextra
		PRIMARY KEY (inv_id) ;

ALTER TABLE t_inventoryextra
	ADD CONSTRAINT fk_t_inventoryextra
		FOREIGN KEY (inv_id)
			REFERENCES t_inventory (id_inv)
				ON DELETE CASCADE ;








---------------------------------------------------------------------------------
--------------------------------- TABLE CHANGES ---------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_inventory
*/ ------------------------------
ALTER TABLE t_inventory ADD qualityvalue DOUBLE PRECISION ;

CREATE INDEX ix_t_inventory18 ON t_inventory USING BTREE (generationtime) ;
CREATE INDEX ix_t_inventory19 ON t_inventory USING BTREE (startorbitnumber) ;
CREATE INDEX ix_t_inventory20 ON t_inventory USING BTREE (originator) ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'pds-11.7.3' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-11.7.3',
		'2015-03-16',
		3,
		2,
		'PDS_11-7-3',
		'Added table t_inventoryextra. Added field qualityvalue on t_inventory.'
) ;


