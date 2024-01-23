
-- Version 11.3.0
-- POSTGRES








---------------------------------------------------------------------------------
-------------------------------- TABLES CREATION --------------------------------
---------------------------------------------------------------------------------

/* --------------------------
	t_inv_client
*/ --------------------------
CREATE TABLE t_inv_client (
    id_inv_client SERIAL NOT NULL,
    name VARCHAR(128) NOT NULL
) ;
ALTER TABLE t_inv_client
	ADD CONSTRAINT pk_t_inv_client
		PRIMARY KEY (id_inv_client) ;

ALTER TABLE t_inv_client 
	ADD CONSTRAINT uq_inv_client01
		UNIQUE (name) ;



/* --------------------------
	t_inv_downloads
*/ --------------------------
CREATE TABLE t_inv_downloads (
    inv_id BIGINT NOT NULL,
	client_id INTEGER NOT NULL,
    downloadtime TIMESTAMP(6) DEFAULT NOW() NOT NULL
) ;
ALTER TABLE t_inv_downloads
	ADD CONSTRAINT pk_t_inv_downloads
		PRIMARY KEY (inv_id, client_id) ;

ALTER TABLE t_inv_downloads 
	ADD CONSTRAINT fk_inv_downloads01 
		FOREIGN KEY (inv_id) 
			REFERENCES t_inventory (id_inv) 
				ON DELETE CASCADE ;

ALTER TABLE t_inv_downloads 
	ADD CONSTRAINT fk_inv_downloads02 
		FOREIGN KEY (client_id) 
			REFERENCES t_inv_client (id_inv_client) 
				ON DELETE CASCADE ;

CREATE INDEX ix_t_inv_downloads01 ON t_inv_downloads USING BTREE (inv_id) ;
CREATE INDEX ix_t_inv_downloads02 ON t_inv_downloads USING BTREE (client_id) ;








---------------------------------------------------------------------------------
----------------------------- TABLES MODIFICATIONS ------------------------------
---------------------------------------------------------------------------------

/* --------------------------
	t_inventorysm
*/ --------------------------
ALTER TABLE t_inventorysm ADD stored_date TIMESTAMP(6) DEFAULT now() NOT NULL ;

ALTER TABLE t_inventorysm ADD inv_id BIGINT ;
UPDATE t_inventorysm SET inv_id = subq.id_inv FROM (
	SELECT id_inv, filename, fileversion FROM t_inventory) AS subq
		WHERE t_inventorysm.filename = subq.filename AND t_inventorysm.fileversion = subq.fileversion ;
ALTER TABLE t_inventorysm ALTER COLUMN inv_id SET NOT NULL ;

ALTER TABLE t_inventorysm DROP CONSTRAINT fk_t_inventorysm1 ;
ALTER TABLE t_inventorysm
	ADD CONSTRAINT fk_t_inventorysm1
		FOREIGN KEY (inv_id)
			REFERENCES t_inventory (id_inv)
				ON DELETE CASCADE ;

ALTER TABLE t_inventorysm DROP COLUMN filename ;
ALTER TABLE t_inventorysm DROP COLUMN fileversion ;

ALTER TABLE t_inventorysm
	ADD CONSTRAINT uq_inventorysm01
		UNIQUE (inv_id, storagemanid) ;

DROP INDEX ix_invsm_storagemanid ;
CREATE INDEX ix_t_inventorysm01 ON t_inventorysm USING BTREE (inv_id);
CREATE INDEX ix_t_inventorysm02 ON t_inventorysm USING BTREE (storagemanid);







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
		'pds-11.3.0',
		'2013-09-11',
		3,
		2,
		'PDS_11-3-0',
		'Added new tables t_inv_client and t_inv_downloads. Added inv_id and stored_date fields and removed filename and fileversion to t_inventorysm.'
) ;


