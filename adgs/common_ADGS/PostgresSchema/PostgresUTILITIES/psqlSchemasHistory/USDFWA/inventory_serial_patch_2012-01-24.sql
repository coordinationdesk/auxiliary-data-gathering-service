
-- Version "usdfwa_inv_serial_patch"
-- POSTGRES

------------------------------------------------------------------
-- ATTENZIONE!!!                                                --
-- Questa patch è necessaria solo se è stata eseguita la patch  --
-- pds_v10.3.0_2012-01-24.sql su un database pds già esistente  --
------------------------------------------------------------------








---------------------------------------------------------------------------------
------------------------------ TABLES MODIFICATION ------------------------------
---------------------------------------------------------------------------------

------------------------------
-- t_usdfwa
------------------------------
ALTER TABLE t_usdfwa 
	ADD CONSTRAINT fk_fname_fver
		FOREIGN KEY (filename, fileversion)
			REFERENCES t_inventory (filename, fileversion) 
				ON DELETE CASCADE ;

ALTER TABLE t_inventorylinks 
	ADD CONSTRAINT fk_invlinks_orig 
		FOREIGN KEY (orig_filename, orig_fileversion)
			REFERENCES t_inventory (filename, fileversion) 
				ON DELETE CASCADE ;

ALTER TABLE t_inventorylinks 
	ADD CONSTRAINT fk_invlinks_dest 
		FOREIGN KEY (dest_filename, dest_fileversion)
			REFERENCES t_inventory (filename, fileversion) 
				ON DELETE CASCADE ;






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
		'usdfwa_inv_serial_patch',
		'2012-01-24',
		6,
		5,
		NULL,
		'Recreated foreign keys to t_inventory table.'
) ;
