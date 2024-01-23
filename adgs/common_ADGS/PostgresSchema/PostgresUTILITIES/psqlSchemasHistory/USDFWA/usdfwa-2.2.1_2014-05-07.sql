
-- Version usdfwa-2.2.1
-- POSTGRES








---------------------------------------------------------------------------------
------------------------------------ INDEXES ------------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_usdfwa
*/ ------------------------------
CREATE INDEX ix_t_usdfwa01 ON t_usdfwa USING BTREE (filename, fileversion) ;


/* ------------------------------
	t_usdfwa_delivery_details
*/ ------------------------------
CREATE INDEX ix_t_usdfwa_del_det01 ON t_usdfwa_delivery_details USING BTREE (usdfwa_id) ;


/* ------------------------------
	t_inventorylinks
*/ ------------------------------
CREATE INDEX ix_t_inventorylinks01 ON t_inventorylinks USING BTREE (orig_filename, orig_fileversion) ;
CREATE INDEX ix_t_inventorylinks02 ON t_inventorylinks USING BTREE (dest_filename, dest_fileversion) ;


/* ------------------------------
	t_inventorylinksconf
*/ ------------------------------
CREATE INDEX ix_t_inventorylinksconf01 ON t_inventorylinksconf USING BTREE (orig_filetype) ;
CREATE INDEX ix_t_inventorylinksconf02 ON t_inventorylinksconf USING BTREE (dest_filetype) ;








---------------------------------------------------------------------------------
------------------------------------ COMMENTS -----------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_usdfwa
*/ ------------------------------
COMMENT ON TABLE t_usdfwa IS '';
COMMENT ON COLUMN t_usdfwa.id_usdfwa IS 'Record identifier (Primary Key)';
COMMENT ON COLUMN t_usdfwa.filename IS 'References filename on t_inventory table.';
COMMENT ON COLUMN t_usdfwa.fileversion IS 'References fileversion on t_inventory table.';
COMMENT ON COLUMN t_usdfwa.mmmcupdatefile IS '';
COMMENT ON COLUMN t_usdfwa.modificationdate IS '';
COMMENT ON SEQUENCE t_usdfwa_id_usdfwa_seq IS 'Used to generate the primary keys of the t_usdfwa table';


/* ------------------------------
	t_usdfwa_delivery_details
*/ ------------------------------
COMMENT ON TABLE t_usdfwa_delivery_details IS '';
COMMENT ON COLUMN t_usdfwa_delivery_details.id_delivery_details IS 'Record identifier (Primary Key)';
COMMENT ON COLUMN t_usdfwa_delivery_details.usdfwa_id IS '';
COMMENT ON COLUMN t_usdfwa_delivery_details.url IS '';
COMMENT ON COLUMN t_usdfwa_delivery_details.deliverydate IS '';
COMMENT ON COLUMN t_usdfwa_delivery_details.itemstatus IS '';
COMMENT ON COLUMN t_usdfwa_delivery_details.statusdetails IS '';
COMMENT ON SEQUENCE t_usdfwa_delivery_details_id_delivery_details_seq IS 'Used to generate the primary keys of the t_usdfwa_delivery_details table';


/* ------------------------------
	t_inventorylinks
*/ ------------------------------
COMMENT ON TABLE t_inventorylinks IS '';
COMMENT ON COLUMN t_inventorylinks.id IS 'Record identifier (Primary Key)';
COMMENT ON COLUMN t_inventorylinks.orig_filename IS 'References filename on t_inventory table.';
COMMENT ON COLUMN t_inventorylinks.orig_fileversion IS 'References fileversion on t_inventory table.';
COMMENT ON COLUMN t_inventorylinks.dest_filename IS 'References filename on t_inventory table.';
COMMENT ON COLUMN t_inventorylinks.dest_fileversion IS 'References fileversion on t_inventory table.';
COMMENT ON COLUMN t_inventorylinks.linkdate IS '';
COMMENT ON COLUMN t_inventorylinks.description IS '';
COMMENT ON SEQUENCE t_inventorylinks_id_seq IS 'Used to generate the primary keys of the t_inventorylinks table';


/* ------------------------------
	t_inventorylinksconf
*/ ------------------------------
COMMENT ON TABLE t_inventorylinksconf IS '';
COMMENT ON COLUMN t_inventorylinksconf.id IS 'Record identifier (Primary Key)';
COMMENT ON COLUMN t_inventorylinksconf.orig_filetype IS 'References filetype on t_filetypes table.';
COMMENT ON COLUMN t_inventorylinksconf.dest_filetype IS 'References filetype on t_filetypes table.';
COMMENT ON COLUMN t_inventorylinksconf.algotype IS '';
COMMENT ON COLUMN t_inventorylinksconf.description IS '';
COMMENT ON SEQUENCE t_inventorylinksconf_id_seq IS 'Used to generate the primary keys of the t_inventorylinksconf table';








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
		'usdfwa-2.2.1',
		'2014-05-07',
		6,
		5,
		'USDFWA_2-2-1',
		'Added foreignkeys indexes. Added comments on schema objects.'
) ;
