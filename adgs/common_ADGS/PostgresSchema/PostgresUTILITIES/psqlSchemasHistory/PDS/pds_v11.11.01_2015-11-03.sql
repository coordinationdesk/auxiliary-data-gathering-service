
-- Version 11.11.01
-- POSTGRES








--------------------------------------------------------------------------------
--------------------------- TRIGGER FUNCTION CHANGES ---------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	tf_inventorysmmanager
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_inventorysmmanager() RETURNS TRIGGER AS $$

BEGIN
	PERFORM p_events('t_inventorysm', TG_ARGV[0]) ;
	
	IF (TG_ARGV[0] = 'DEL') 
	THEN 
		PERFORM p_insert_inventoryevents(OLD.inv_id, OLD.storagemanid) ;
		
		IF (SELECT count(*) FROM t_inventorysm ism
			INNER JOIN t_storagemanager stm ON ism.storagemanid = stm.id
			WHERE ism.inv_id = OLD.inv_id AND stm.syncdownload = 't') = 0 
		THEN
			DELETE FROM t_sm_sync_missingactions WHERE inv_id = OLD.inv_id AND operation = 'UPLOAD' ;
		END IF ;
		
		RETURN OLD ;
	ELSE
		PERFORM p_insert_inventoryevents(NEW.inv_id, NEW.storagemanid) ;
		RETURN NEW ;
	END IF ;
END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'pds-11.11.01' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-11.11.01',
		'2015-11-03',
		3,
		2,
		'PDS_11-11-1',
		'Changed tf_inventorysmmanager'
) ;


