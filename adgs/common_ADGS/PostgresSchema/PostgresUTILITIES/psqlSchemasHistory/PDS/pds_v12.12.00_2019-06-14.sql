
-- Version 12.12.00
-- POSTGRES








--------------------------------------------------------------------------------
------------------------------ PROCEDURE CREATION ------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	p_PurgeDistributionWaitingItems
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_PurgeDistributionWaitingItems(
	id_to_remove BIGINT,
	field_to_considerate VARCHAR(32)) RETURNS INTEGER AS $$

DECLARE
	sqlCmd VARCHAR(256);
	
BEGIN
	sqlCmd = 'DELETE FROM t_distributionwaitingitems WHERE ' || field_to_considerate || ' = ' || id_to_remove ;
	RAISE NOTICE '%', sqlCmd;
	EXECUTE sqlCmd ;
	
	RETURN 0;
	
END ;
$$ LANGUAGE plpgsql ;








--------------------------------------------------------------------------------
-------------------------- TRIGGERS FUNCTIONS CHANGES --------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	tf_inventoryupdevent
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_inventoryupdevent() RETURNS TRIGGER AS $$

BEGIN
	PERFORM p_inventory_manager(NEW.filename, NEW.fileversion, OLD.validityflag, NEW.validityflag, NEW.filetype, NEW.fileclass, NEW.orderid, NEW.satelliteid, NEW.sensorid, NEW.mission, NEW.validitystart, NEW.validitystop) ;
	PERFORM p_insert_inventoryevents(NEW.id_inv, NULL, NULL, NEW.filetype, OLD.validityflag, NEW.validityflag, 'U') ;
	
	IF (NEW.validityflag != OLD.validityflag) THEN
		IF (NEW.validityflag = 't') THEN
			PERFORM p_purgewaitingorders(NEW.filetype, NEW.validitystart, NEW.validitystop) ;
		ELSE
			PERFORM p_PurgeDistributionWaitingItems(NEW.id_inv, 'inventory_id') ;
		END IF;
	END IF;
	
	IF OLD.qualityinfo != NEW.qualityinfo THEN
		PERFORM p_purgewaitingorders(NEW.filetype, NEW.validitystart, NEW.validitystop) ;
	END IF ;
	
	IF (OLD.qualityinfo = 'DUP' AND (NEW.qualityinfo = '' OR NEW.qualityinfo IS NULL) AND NEW.validityflag = 't') THEN
		PERFORM p_generatetmporder('t', OLD.filename, OLD.fileversion, OLD.filetype, OLD.fileclass, OLD.satelliteid) ;
	END IF ;
	
	IF OLD.qualified != NEW.qualified THEN
		PERFORM p_purgewaitingorders(NEW.filetype, NEW.validitystart, NEW.validitystop) ;
	END IF ;
	
  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	tf_distributionqueue_manager
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_distributionqueue_manager() RETURNS TRIGGER AS $$

BEGIN
	IF (TG_ARGV[0] = 'INS') THEN
		PERFORM p_events('t_distributionqueue', 'INS') ;
		PERFORM p_cartsinv_update(NEW.packageid, NEW.status);
		RETURN NEW ;
	END IF ;
	
	IF (TG_ARGV[0] = 'UPD') THEN
		PERFORM p_events('t_distributionqueue', 'UPD') ;
		PERFORM p_cartsinv_update(NEW.packageid, NEW.status);
		PERFORM p_inv_dist_counter_by_queue(NEW.packageid, OLD.status, NEW.status, 'UPD') ;
		PERFORM p_PurgeDistributionWaitingItems(NEW.packageid, 'package_id') ;
		RETURN NEW ;
	END IF ;
	
	IF (TG_ARGV[0] = 'DEL') THEN
		PERFORM p_inv_dist_counter_by_queue(OLD.packageid, OLD.status, NULL, 'DEL') ;
		PERFORM p_events('t_distributionqueue', 'DEL') ;
		RETURN OLD ;
	END IF ;
	
END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	tf_distributionitems_manager
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_distributionitems_manager() RETURNS TRIGGER AS $$

BEGIN
	IF (TG_ARGV[0] = 'INS') THEN
		PERFORM p_inv_dist_counter_by_items(NEW.filename, NEW.fileversion, 'INS') ;
		PERFORM p_events('t_distributionitems', 'INS') ;
		RETURN NEW ;
	END IF ;
	
	IF (TG_ARGV[0] = 'UPD') THEN
		PERFORM p_events('t_distributionitems', 'UPD') ;
		PERFORM p_PurgeDistributionWaitingItems(NEW.id, 'distributionitem_id') ;
		RETURN NEW ;
	END IF ;
	
	IF (TG_ARGV[0] = 'DEL') THEN
		PERFORM p_inv_dist_counter_by_items(OLD.filename, OLD.fileversion, 'DEL') ;
		PERFORM p_events('t_distributionitems', 'DEL') ;
		RETURN OLD ;
	END IF ;
	
END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'pds-12.12.00' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-12.12.00',
		'2019-06-14',
		3,
		2,
		'PDS_12-12-0',
		'Added new trigger functions and stored procedure to purge t_distributionwaitingitems'
) ;


