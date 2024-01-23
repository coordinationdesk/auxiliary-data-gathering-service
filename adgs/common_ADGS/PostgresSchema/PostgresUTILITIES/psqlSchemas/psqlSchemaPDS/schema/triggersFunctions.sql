/*

 Exprivia S.p.A.
 Via Della Bufalotta, 378 - 00139 Roma - Italy
 http://www.exprivia.it

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia S.p.A.;
 The contents of this file may not be disclosed to third parties, copied
 or duplicated in any form, in whole or in part, without the prior written
 permission of Exprivia S.p.A.

*/





-------------------------------------------------------------------------------
--                                                                           --
--                             TRIGGER FUNCTIONS                             --
--                                                                           --                                                                           --
-------------------------------------------------------------------------------

/* --------------------------
	tf_events
*/ --------------------------
CREATE OR REPLACE FUNCTION tf_events() RETURNS TRIGGER AS $$

BEGIN
	--  NOT IMPLEMENTED IN THIS SCHEMA
	--  To implement this procedure you have to install the BROADCAST schema v.1.0.0 or major
	
	RETURN NULL ;
	
END ;
$$ LANGUAGE plpgsql ;




/* -------------------------------
	tf_status_transiction_check
*/ -------------------------------
CREATE OR REPLACE FUNCTION tf_status_transiction_check() RETURNS TRIGGER AS $$

BEGIN
	PERFORM p_statetransitions_check(TG_TABLE_NAME::VARCHAR, 1, OLD.status_id, NEW.status_id) ;
	
	RETURN NEW ;
	
END ;
$$ LANGUAGE plpgsql ;




/* ------------------------------
	tf_inserteventtableitem
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_inserteventtableitem() RETURNS TRIGGER AS $$
DECLARE
  	status INTEGER ;
BEGIN
  	status := p_events(TG_ARGV[0], TG_ARGV[1]) ;
  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;




/* --------------------------
	tf_inventory_ins_bfr
*/ --------------------------
CREATE OR REPLACE FUNCTION tf_inventory_ins_bfr() RETURNS TRIGGER AS $$

BEGIN
	IF NEW.unique_id IS NULL  THEN
		NEW.unique_id := NEW.filename || NEW.fileversion ;
	END IF ;
	
	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;




/* ------------------------------
	tf_inventoryinsevent
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_inventoryinsevent() RETURNS TRIGGER AS $$

BEGIN
	PERFORM p_inventory_manager(NEW.id_inv, NEW.filename, NEW.fileversion, NULL, NEW.validityflag, NEW.filetype, NEW.fileclass, NEW.orderid, NEW.satelliteid, NEW.sensorid, NEW.mission, NEW.validitystart, NEW.validitystop) ;
	PERFORM p_insert_inventoryevents(NEW.id_inv, NEW.filename, NEW.fileversion, NEW.filetype, NULL, NEW.validityflag, 'I') ;
	
	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;




/* ------------------------------
	tf_inventoryupdevent
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_inventoryupdevent() RETURNS TRIGGER AS $$

BEGIN
	PERFORM p_inventory_manager(NEW.id_inv, NEW.filename, NEW.fileversion, OLD.validityflag, NEW.validityflag, NEW.filetype, NEW.fileclass, NEW.orderid, NEW.satelliteid, NEW.sensorid, NEW.mission, NEW.validitystart, NEW.validitystop) ;
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
	tf_inventorydelete
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_inventorydelete() RETURNS TRIGGER AS $$

BEGIN
	PERFORM p_insert_inventoryevents(NULL, OLD.filename, OLD.fileversion, OLD.filetype, 'f', 'f', 'D') ;
	
  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;




/* ------------------------------
	tf_inventorysmmanager
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_inventorysmmanager() RETURNS TRIGGER AS $$

BEGIN
	IF (TG_OP = 'INSERT') 
	THEN
		PERFORM p_insert_inventoryevents(NEW.inv_id, NEW.storagemanid) ;
		PERFORM p_insert_inv_data_restored(NEW.id, NEW.inv_id, NEW.storagemanid) ;
		RETURN NEW ;
	END IF ;
	
	IF (TG_OP = 'UPDATE') 
	THEN
		PERFORM p_insert_inventoryevents(NEW.inv_id, NEW.storagemanid) ;
		RETURN NEW ;
	END IF ;
	
	IF (TG_OP = 'DELETE') 
	THEN 
		PERFORM p_insert_inventoryevents(OLD.inv_id, OLD.storagemanid) ;
		
		IF (SELECT count(*) FROM t_inventorysm ism
			INNER JOIN t_storagemanager stm ON ism.storagemanid = stm.id
			WHERE ism.inv_id = OLD.inv_id AND stm.syncdownload = 't') = 0 
		THEN
			DELETE FROM t_sm_sync_missingactions WHERE inv_id = OLD.inv_id AND operation = 'UPLOAD' ;
		END IF ;
		
		RETURN OLD ;
	END IF ;
	
	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;




/* -------------------------------
	tf_ordersqueue_statcheck
*/ -------------------------------
CREATE OR REPLACE FUNCTION tf_ordersqueue_statcheck() RETURNS TRIGGER AS $$

DECLARE
  	preStatId INT ;
	postStatId INT ;
	errMsg VARCHAR(128) ;

BEGIN
  	IF (TG_ARGV[0] = 'INS') THEN
		IF (NEW.status != 'ABORTED') AND (NEW.status != 'ELIGIBLE') AND (NEW.status != 'STANDBY') THEN
			errMsg := 'tf_ordersqueue_statcheck: the first status can be only ABORTED or ELIGIBLE or STANDBY' ;
			RAISE EXCEPTION '%', errMsg ;
		END IF ;
	END IF ;
	
	IF (TG_ARGV[0] = 'UPD') THEN
		preStatId := (SELECT id FROM t_stati WHERE status = OLD.status) ;
		postStatId := (SELECT id FROM t_stati WHERE status = NEW.status) ;
		
		IF postStatId IS NULL THEN
			errMsg := 'tf_ordersqueue_statcheck: ' || NEW.status || ' status does not exist.' ;
			RAISE EXCEPTION '%', errMsg ;
		END IF ;
		
		PERFORM p_statetransitions_check('t_ordersqueue', 1, preStatId, postStatId);
	END IF ;
	
  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;




/* ------------------------------
	tf_ordersqueue_manager
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_ordersqueue_manager() RETURNS TRIGGER AS $$

BEGIN
	IF (TG_OP = 'INSERT') THEN
		PERFORM p_orderstatetransitions_mgr(NEW.orderid, NULL, NEW.status) ;
		PERFORM p_orders_manager(NEW.orderid, NEW.status, NEW.status) ;
		RETURN NEW ;
	END IF ;
	
	IF (TG_OP = 'UPDATE') THEN
		PERFORM p_orderstatetransitions_mgr(NEW.orderid, OLD.status, NEW.status) ;
		PERFORM p_orders_manager(OLD.orderid, OLD.status, NEW.status) ;
		PERFORM p_orderqueuecheckchanges(OLD.orderid, OLD.status, NEW.status, OLD.processorname, NEW.processorname, OLD.processorversion, NEW.processorversion, OLD.processorconfiguration, NEW.processorconfiguration, OLD.starttime, NEW.starttime, OLD.stoptime, NEW.stoptime, OLD.ordertype, NEW.ordertype, OLD.satelliteid, NEW.satelliteid, OLD.mission, NEW.mission) ;
		RETURN NEW ;
	END IF ;
	
END ;
$$ LANGUAGE plpgsql ;




/* ------------------------------
	tf_smqueue_checkchanges
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_smqueue_checkchanges() RETURNS TRIGGER AS $$

DECLARE
  	preStatId INT ;
	postStatId INT ;
	errMsg VARCHAR(128) ;

BEGIN
  	IF (TG_ARGV[0] = 'INS') THEN
		IF NEW.status != 'STANDBY' THEN
			errMsg := 'tf_smqueue_checkchanges: the first status must be STANDBY' ;
			RAISE EXCEPTION '%', errMsg ;
		END IF ;
	END IF ;
	
	IF (TG_ARGV[0] = 'UPD') THEN
		preStatId := (SELECT id FROM t_stati WHERE status = OLD.status) ;
		postStatId := (SELECT id FROM t_stati WHERE status = NEW.status) ;
		
		IF postStatId IS NULL THEN
			errMsg := 'tf_smqueue_checkchanges: ' || NEW.status || ' status does not exist.' ;
			RAISE EXCEPTION '%', errMsg ;
		END IF ;
		
		PERFORM p_statetransitions_check('t_smqueue', 1, preStatId, postStatId);
	END IF ;
	
  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;




/* ------------------------------
	tf_smactionsinsert
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_smactionsinsert() 
	RETURNS TRIGGER AS $$
BEGIN
	IF NEW.packageid IS NOT NULL THEN
		RAISE EXCEPTION 'tf_smactionsInsert: Cannot insert a new record having a packageid';
	END IF;
	
  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;




/* ------------------------------
	tf_smactions_upd_bfr
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_smactions_upd_bfr() 
	RETURNS TRIGGER AS $$
BEGIN
	IF (OLD.packageid IS NOT NULL AND NEW.packageid IS NOT NULL) AND (OLD.packageid != NEW.packageid) THEN
		RAISE EXCEPTION 'tf_smactions_upd_bfr: Cannot assign a new packageid in a record having an old packageid';
	END IF;
	
	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;




/* ------------------------------
	tf_smactions_upd_aft
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_smactions_upd_aft() 
	RETURNS TRIGGER AS $$
BEGIN
	PERFORM p_smactionsupdate(NEW.filename, NEW.fileversion, NEW.packageid) ;
	
  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;




/* ------------------------------
	tf_cartsinvtrmgr
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_cartsinvtrmgr() RETURNS TRIGGER AS $$

BEGIN
  	PERFORM p_cartsinvcheck(NEW.id, NEW.cart, NEW.subreq, NEW.distsubreq, NEW.ordsubreq, NEW.stndsubreq ) ;
	
  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;




/* ------------------------------
	tf_pendinginvinsert
*/ ------------------------------
CREATE FUNCTION tf_pendinginvinsert() RETURNS TRIGGER AS $$
DECLARE
  	status INTEGER ;
BEGIN
	status := p_postinvqueue_insert(NEW.filename, NEW.fileversion) ;
  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;




/* ------------------------------
	tf_filetype_insert
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_filetype_insert() RETURNS TRIGGER AS $$

BEGIN
	INSERT INTO t_filetypes_last_inv (filetype) VALUES (NEW.filetype) ;
	
	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;




/* -------------------------------
	tf_distributionqueue_statcheck
*/ -------------------------------
CREATE OR REPLACE FUNCTION tf_distributionqueue_statcheck() RETURNS TRIGGER AS $$

DECLARE
	preStatId INT ;
	postStatId INT ;
	errMsg VARCHAR(128) ;
	
BEGIN
	IF (TG_OP = 'UPDATE') THEN
		preStatId := (SELECT id FROM t_stati WHERE status = OLD.status) ;
		postStatId := (SELECT id FROM t_stati WHERE status = NEW.status) ;
		
		IF postStatId IS NULL THEN
			errMsg := 'tf_distributionqueue_statcheck: ' || NEW.status || ' status does not exist.' ;
			RAISE EXCEPTION '%', errMsg ;
		END IF ;
		
		PERFORM p_statetransitions_check('t_distributionqueue', 1, preStatId, postStatId);
	END IF ;
	
	IF (NEW.status = 'ELIGIBLE') THEN
		NEW.retrycounter := 0 ;
	END IF ;
	
	RETURN NEW ;
	
END ;
$$ LANGUAGE plpgsql ;




/* ------------------------------
	tf_invdistributionsupdate
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_invdistributionsupdate() RETURNS TRIGGER AS $$

BEGIN
	NEW.last_update := now()::TIMESTAMP ;
	
  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;




/* ------------------------------
	tf_distributionitems_manager
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_distributionitems_manager() RETURNS TRIGGER AS $$

BEGIN
	IF (TG_OP = 'INSERT') THEN
		PERFORM p_inv_dist_counter_by_items(NEW.filename, NEW.fileversion, 'INS') ;
		RETURN NEW ;
	END IF ;
	
	IF (TG_OP = 'UPDATE') THEN
		PERFORM p_PurgeDistributionWaitingItems(NEW.id, 'distributionitem_id') ;
		RETURN NEW ;
	END IF ;
	
	IF (TG_OP = 'DELETE') THEN
		PERFORM p_inv_dist_counter_by_items(OLD.filename, OLD.fileversion, 'DEL') ;
		RETURN OLD ;
	END IF ;
	
END ;
$$ LANGUAGE plpgsql ;




/* ------------------------------
	tf_distributionqueue_manager
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_distributionqueue_manager() RETURNS TRIGGER AS $$

BEGIN
	IF (TG_OP = 'INSERT') THEN
		PERFORM p_cartsinv_update(NEW.packageid, NEW.status);
		RETURN NEW ;
	END IF ;
	
	IF (TG_OP = 'UPDATE') THEN
		PERFORM p_cartsinv_update(NEW.packageid, NEW.status);
		PERFORM p_inv_dist_counter_by_queue(NEW.packageid, OLD.status, NEW.status, 'UPD') ;
		PERFORM p_PurgeDistributionWaitingItems(NEW.packageid, 'package_id') ;
		RETURN NEW ;
	END IF ;
	
	IF (TG_OP = 'DELETE') THEN
		PERFORM p_inv_dist_counter_by_queue(OLD.packageid, OLD.status, NULL, 'DEL') ;
		RETURN OLD ;
	END IF ;
	
END ;
$$ LANGUAGE plpgsql ;




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


