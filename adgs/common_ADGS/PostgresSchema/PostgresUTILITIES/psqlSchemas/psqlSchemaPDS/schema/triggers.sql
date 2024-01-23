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
--                                  TRIGGERS                                 --
--                                                                           --                                                                           --
-------------------------------------------------------------------------------

/* ------------------------------
	t_cdp_feederqueue
*/ ------------------------------
CREATE TRIGGER tr_cdp_feederqueue_upd_bfr BEFORE UPDATE OF status_id
	ON t_cdp_feederqueue
		FOR EACH ROW EXECUTE PROCEDURE tf_status_transiction_check() ;



/* ------------------------------
	t_cdp_notification_queue
*/ ------------------------------
CREATE TRIGGER tr_cdp_notification_queue_upd_bfr BEFORE UPDATE OF status_id
	ON t_cdp_notification_queue
		FOR EACH ROW EXECUTE PROCEDURE tf_status_transiction_check() ;



/* ------------------------------
	t_distributionqueue
*/ ------------------------------
CREATE TRIGGER tr_distributionqueue_ins_bfr BEFORE INSERT
	ON t_distributionqueue
		FOR EACH ROW EXECUTE PROCEDURE tf_distributionqueue_statcheck() ;

CREATE TRIGGER tr_distributionqueue_upd_bfr BEFORE UPDATE OF status
	ON t_distributionqueue
		FOR EACH ROW EXECUTE PROCEDURE tf_distributionqueue_statcheck() ;

CREATE TRIGGER tr_distributionqueue_manager AFTER INSERT OR UPDATE OR DELETE
	ON t_distributionqueue 
		FOR EACH ROW EXECUTE PROCEDURE tf_distributionqueue_manager();



/* ------------------------------
	t_distributionitems
*/ ------------------------------
CREATE TRIGGER tr_distributionitems_manager AFTER INSERT OR UPDATE OR DELETE
	ON t_distributionitems 
		FOR EACH ROW EXECUTE PROCEDURE tf_distributionitems_manager() ;



/* ------------------------------
	t_inventory
*/ ------------------------------
--CREATE TRIGGER tr_inventory_ins_bfr BEFORE INSERT
--	ON t_inventory
--		FOR EACH ROW EXECUTE PROCEDURE tf_inventory_ins_bfr() ;

CREATE TRIGGER tr_inventory_ins_aft AFTER INSERT
	ON t_inventory
		FOR EACH ROW EXECUTE PROCEDURE tf_inventoryinsevent() ;

CREATE TRIGGER tr_inventory_upd_aft AFTER UPDATE
	ON t_inventory
		FOR EACH ROW EXECUTE PROCEDURE tf_inventoryupdevent() ;

CREATE TRIGGER tr_inventory_del_aft AFTER DELETE
	ON t_inventory
		FOR EACH ROW EXECUTE PROCEDURE tf_inventorydelete() ;



/* ------------------------------
	t_ordersqueue
*/ ------------------------------
CREATE TRIGGER tr_ordersqueue_ins_bfr BEFORE INSERT
	ON t_ordersqueue
		FOR EACH ROW EXECUTE PROCEDURE tf_ordersqueue_statcheck('INS') ;

CREATE TRIGGER tr_ordersqueue_upd_bfr BEFORE UPDATE OF status
	ON t_ordersqueue
		FOR EACH ROW EXECUTE PROCEDURE tf_ordersqueue_statcheck('UPD') ;

CREATE TRIGGER tr_ordersqueue_manager AFTER INSERT OR UPDATE
	ON t_ordersqueue 
		FOR EACH ROW EXECUTE PROCEDURE tf_ordersqueue_manager();



/* ------------------------------
	t_acshsminvrelation
*/ ------------------------------
CREATE TRIGGER tr_acshsminvrelationinsert AFTER INSERT
  ON t_acshsminvrelation EXECUTE PROCEDURE tf_inserteventtableitem('t_acshsminvrelation', 'INS') ;

CREATE TRIGGER tr_acshsminvrelationupdate AFTER UPDATE
  ON t_acshsminvrelation EXECUTE PROCEDURE tf_inserteventtableitem('t_acshsminvrelation', 'UPD') ;



/* ------------------------------
	t_filetypes
*/ ------------------------------
CREATE TRIGGER tr_filetypesinsert AFTER INSERT
	ON t_filetypes 
		FOR EACH ROW
			EXECUTE PROCEDURE tf_filetype_insert() ;



/* ------------------------------
	t_inventorysm
*/ ------------------------------
CREATE TRIGGER tr_inventorysm_ins_bfr BEFORE INSERT
	ON t_inventorysm 
		FOR EACH ROW EXECUTE PROCEDURE tf_inventorysm_check_invid('INS') ;

CREATE TRIGGER tr_inventorysm_manager AFTER INSERT OR UPDATE OR DELETE
	ON t_inventorysm 
		FOR EACH ROW EXECUTE PROCEDURE tf_inventorysmmanager() ;



/* ------------------------------
	t_smqueue
*/ ------------------------------
CREATE TRIGGER tr_smqueue_ins_bfr BEFORE INSERT
	ON t_smqueue
		FOR EACH ROW EXECUTE PROCEDURE tf_smqueue_checkchanges('INS') ;

CREATE TRIGGER tr_smqueue_upd_bfr BEFORE UPDATE OF status
	ON t_smqueue
		FOR EACH ROW EXECUTE PROCEDURE tf_smqueue_checkchanges('UPD') ;



/* ------------------------------
	t_smactions
*/ ------------------------------
CREATE TRIGGER tr_smactionsinsert AFTER INSERT
	ON t_smactions 
		FOR EACH ROW EXECUTE PROCEDURE tf_smactionsinsert() ;

CREATE TRIGGER tr_smactions_upd_bfr BEFORE UPDATE
	ON t_smactions 
		FOR EACH ROW EXECUTE PROCEDURE tf_smactions_upd_bfr() ;

CREATE TRIGGER tr_smactions_upd_aft AFTER UPDATE
	ON t_smactions 
		FOR EACH ROW EXECUTE PROCEDURE tf_smactions_upd_aft() ;



/* ------------------------------
	t_cartsinv
*/ ------------------------------
CREATE TRIGGER tr_cartsinv_manager AFTER INSERT OR UPDATE
	ON t_cartsinv
		FOR EACH ROW EXECUTE PROCEDURE tf_cartsinvtrmgr() ;



/* ------------------------------
	t_pendinginventory
*/ ------------------------------
CREATE TRIGGER tr_pendinginvinsert AFTER INSERT
  ON t_pendinginventory 
  	FOR EACH ROW
  		EXECUTE PROCEDURE tf_pendinginvinsert() ;



/* ------------------------------
	t_inv_distributions
*/ ------------------------------
CREATE TRIGGER tr_invdistributionsupdate BEFORE UPDATE
	ON t_inv_distributions
		FOR EACH ROW EXECUTE PROCEDURE tf_invdistributionsupdate() ;








--------------------------------------------------------------------------------
--
--	 	  	  	  	  	  	  BINARY FIELDS TRIGGERS
--
--------------------------------------------------------------------------------

/* ------------------------------
	t_applicationstatus
*/ ------------------------------
CREATE TRIGGER tr_oid_applicationstatus
	BEFORE DELETE OR UPDATE ON t_applicationstatus
		FOR EACH ROW EXECUTE PROCEDURE lo_manage("status");



/* ------------------------------
	t_ordersattachments
*/ ------------------------------
CREATE TRIGGER tr_oid_ordersattachments
	BEFORE DELETE OR UPDATE ON t_ordersattachments
		FOR EACH ROW EXECUTE PROCEDURE lo_manage("attachment");



/* ------------------------------
	t_smactions
*/ ------------------------------
CREATE TRIGGER tr_oid_smactions
	BEFORE DELETE OR UPDATE ON t_smactions
		FOR EACH ROW EXECUTE PROCEDURE lo_manage("parameter");



/* ------------------------------
	t_smqueue
*/ ------------------------------
CREATE TRIGGER tr_oid_smqueue
	BEFORE DELETE OR UPDATE ON t_smqueue
		FOR EACH ROW EXECUTE PROCEDURE lo_manage("parameter");


