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
--	 	  	  	  	  	  	  BROADCAST TRIGGERS	 	  	  	  	  	  	 --
--                                                                           --
-------------------------------------------------------------------------------

/* ------------------------------
	t_acsisminvrelation
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_acsisminvrelation_broadcast_insert
			AFTER INSERT ON t_acsisminvrelation
				REFERENCING NEW TABLE AS t_acsisminvrelation_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_acsisminvrelation_broadcast_update
			AFTER UPDATE ON t_acsisminvrelation
				REFERENCING OLD TABLE AS t_acsisminvrelation_update_old NEW TABLE AS t_acsisminvrelation_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_acsisminvrelation_broadcast_delete
			AFTER DELETE ON t_acsisminvrelation
				REFERENCING OLD TABLE AS t_acsisminvrelation_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_acsltainvrelation
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_acsltainvrelation_broadcast_insert
			AFTER INSERT ON t_acsltainvrelation
				REFERENCING NEW TABLE AS t_acsltainvrelation_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_acsltainvrelation_broadcast_update
			AFTER UPDATE ON t_acsltainvrelation
				REFERENCING OLD TABLE AS t_acsltainvrelation_update_old NEW TABLE AS t_acsltainvrelation_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_acsltainvrelation_broadcast_delete
			AFTER DELETE ON t_acsltainvrelation
				REFERENCING OLD TABLE AS t_acsltainvrelation_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_algorithm
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_algorithm_broadcast_insert
			AFTER INSERT ON t_algorithm
				REFERENCING NEW TABLE AS t_algorithm_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_algorithm_broadcast_update
			AFTER UPDATE ON t_algorithm
				REFERENCING OLD TABLE AS t_algorithm_update_old NEW TABLE AS t_algorithm_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_algorithm_broadcast_delete
			AFTER DELETE ON t_algorithm
				REFERENCING OLD TABLE AS t_algorithm_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_anomaly_types
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_anomaly_types_broadcast_insert
			AFTER INSERT ON t_anomaly_types
				REFERENCING NEW TABLE AS t_anomaly_types_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_anomaly_types_broadcast_update
			AFTER UPDATE ON t_anomaly_types
				REFERENCING OLD TABLE AS t_anomaly_types_update_old NEW TABLE AS t_anomaly_types_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_anomaly_types_broadcast_delete
			AFTER DELETE ON t_anomaly_types
				REFERENCING OLD TABLE AS t_anomaly_types_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_applications
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_applications_broadcast_insert
			AFTER INSERT ON t_applications
				REFERENCING NEW TABLE AS t_applications_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_applications_broadcast_update
			AFTER UPDATE ON t_applications
				REFERENCING OLD TABLE AS t_applications_update_old NEW TABLE AS t_applications_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_applications_broadcast_delete
			AFTER DELETE ON t_applications
				REFERENCING OLD TABLE AS t_applications_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_applicationevents
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_applicationevents_broadcast_insert
			AFTER INSERT ON t_applicationevents
				REFERENCING NEW TABLE AS t_applicationevents_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_applicationevents_broadcast_update
			AFTER UPDATE ON t_applicationevents
				REFERENCING OLD TABLE AS t_applicationevents_update_old NEW TABLE AS t_applicationevents_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_applicationevents_broadcast_delete
			AFTER DELETE ON t_applicationevents
				REFERENCING OLD TABLE AS t_applicationevents_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_applicationstatus
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_applicationstatus_broadcast_insert
			AFTER INSERT ON t_applicationstatus
				REFERENCING NEW TABLE AS t_applicationstatus_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_applicationstatus_broadcast_update
			AFTER UPDATE ON t_applicationstatus
				REFERENCING OLD TABLE AS t_applicationstatus_update_old NEW TABLE AS t_applicationstatus_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_applicationstatus_broadcast_delete
			AFTER DELETE ON t_applicationstatus
				REFERENCING OLD TABLE AS t_applicationstatus_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_cartsinv
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_cartsinv_broadcast_insert
			AFTER INSERT ON t_cartsinv
				REFERENCING NEW TABLE AS t_cartsinv_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_cartsinv_broadcast_update
			AFTER UPDATE ON t_cartsinv
				REFERENCING OLD TABLE AS t_cartsinv_update_old NEW TABLE AS t_cartsinv_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_cartsinv_broadcast_delete
			AFTER DELETE ON t_cartsinv
				REFERENCING OLD TABLE AS t_cartsinv_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_completed_orders
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_completed_orders_broadcast_insert
			AFTER INSERT ON t_completed_orders
				REFERENCING NEW TABLE AS t_completed_orders_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_completed_orders_broadcast_update
			AFTER UPDATE ON t_completed_orders
				REFERENCING OLD TABLE AS t_completed_orders_update_old NEW TABLE AS t_completed_orders_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_completed_orders_broadcast_delete
			AFTER DELETE ON t_completed_orders
				REFERENCING OLD TABLE AS t_completed_orders_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_configurationschemas
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_configurationschemas_broadcast_insert
			AFTER INSERT ON t_configurationschemas
				REFERENCING NEW TABLE AS t_configurationschemas_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_configurationschemas_broadcast_update
			AFTER UPDATE ON t_configurationschemas
				REFERENCING OLD TABLE AS t_configurationschemas_update_old NEW TABLE AS t_configurationschemas_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_configurationschemas_broadcast_delete
			AFTER DELETE ON t_configurationschemas
				REFERENCING OLD TABLE AS t_configurationschemas_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_configurationspaces
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_configurationspaces_broadcast_insert
			AFTER INSERT ON t_configurationspaces
				REFERENCING NEW TABLE AS t_configurationspaces_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_configurationspaces_broadcast_update
			AFTER UPDATE ON t_configurationspaces
				REFERENCING OLD TABLE AS t_configurationspaces_update_old NEW TABLE AS t_configurationspaces_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_configurationspaces_broadcast_delete
			AFTER DELETE ON t_configurationspaces
				REFERENCING OLD TABLE AS t_configurationspaces_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_configurationstylesheets
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_configurationstylesheets_broadcast_insert
			AFTER INSERT ON t_configurationstylesheets
				REFERENCING NEW TABLE AS t_configurationstylesheets_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_configurationstylesheets_broadcast_update
			AFTER UPDATE ON t_configurationstylesheets
				REFERENCING OLD TABLE AS t_configurationstylesheets_update_old NEW TABLE AS t_configurationstylesheets_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_configurationstylesheets_broadcast_delete
			AFTER DELETE ON t_configurationstylesheets
				REFERENCING OLD TABLE AS t_configurationstylesheets_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_distributionformat
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_distributionformat_broadcast_insert
			AFTER INSERT ON t_distributionformat
				REFERENCING NEW TABLE AS t_distributionformat_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_distributionformat_broadcast_update
			AFTER UPDATE ON t_distributionformat
				REFERENCING OLD TABLE AS t_distributionformat_update_old NEW TABLE AS t_distributionformat_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_distributionformat_broadcast_delete
			AFTER DELETE ON t_distributionformat
				REFERENCING OLD TABLE AS t_distributionformat_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_distributionitems
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_distributionitems_broadcast_insert
			AFTER INSERT ON t_distributionitems
				REFERENCING NEW TABLE AS t_distributionitems_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_distributionitems_broadcast_update
			AFTER UPDATE ON t_distributionitems
				REFERENCING OLD TABLE AS t_distributionitems_update_old NEW TABLE AS t_distributionitems_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_distributionitems_broadcast_delete
			AFTER DELETE ON t_distributionitems
				REFERENCING OLD TABLE AS t_distributionitems_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_distributionqueue
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_distributionqueue_broadcast_insert
			AFTER INSERT ON t_distributionqueue
				REFERENCING NEW TABLE AS t_distributionqueue_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_distributionqueue_broadcast_update
			AFTER UPDATE ON t_distributionqueue
				REFERENCING OLD TABLE AS t_distributionqueue_update_old NEW TABLE AS t_distributionqueue_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_distributionqueue_broadcast_delete
			AFTER DELETE ON t_distributionqueue
				REFERENCING OLD TABLE AS t_distributionqueue_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_distributionrules
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_distributionrules_broadcast_insert
			AFTER INSERT ON t_distributionrules
				REFERENCING NEW TABLE AS t_distributionrules_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_distributionrules_broadcast_update
			AFTER UPDATE ON t_distributionrules
				REFERENCING OLD TABLE AS t_distributionrules_update_old NEW TABLE AS t_distributionrules_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_distributionrules_broadcast_delete
			AFTER DELETE ON t_distributionrules
				REFERENCING OLD TABLE AS t_distributionrules_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_filetypes
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_filetypes_broadcast_insert
			AFTER INSERT ON t_filetypes
				REFERENCING NEW TABLE AS t_filetypes_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_filetypes_broadcast_update
			AFTER UPDATE ON t_filetypes
				REFERENCING OLD TABLE AS t_filetypes_update_old NEW TABLE AS t_filetypes_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_filetypes_broadcast_delete
			AFTER DELETE ON t_filetypes
				REFERENCING OLD TABLE AS t_filetypes_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_inventory
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_inventory_broadcast_insert
			AFTER INSERT ON t_inventory
				REFERENCING NEW TABLE AS t_inventory_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_inventory_broadcast_update
			AFTER UPDATE ON t_inventory
				REFERENCING OLD TABLE AS t_inventory_update_old NEW TABLE AS t_inventory_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_inventory_broadcast_delete
			AFTER DELETE ON t_inventory
				REFERENCING OLD TABLE AS t_inventory_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_inventoryanomalies
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_inventoryanomalies_broadcast_insert
			AFTER INSERT ON t_inventoryanomalies
				REFERENCING NEW TABLE AS t_inventoryanomalies_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_inventoryanomalies_broadcast_update
			AFTER UPDATE ON t_inventoryanomalies
				REFERENCING OLD TABLE AS t_inventoryanomalies_update_old NEW TABLE AS t_inventoryanomalies_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_inventoryanomalies_broadcast_delete
			AFTER DELETE ON t_inventoryanomalies
				REFERENCING OLD TABLE AS t_inventoryanomalies_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_inventorycomments
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_inventorycomments_broadcast_insert
			AFTER INSERT ON t_inventorycomments
				REFERENCING NEW TABLE AS t_inventorycomments_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_inventorycomments_broadcast_update
			AFTER UPDATE ON t_inventorycomments
				REFERENCING OLD TABLE AS t_inventorycomments_update_old NEW TABLE AS t_inventorycomments_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_inventorycomments_broadcast_delete
			AFTER DELETE ON t_inventorycomments
				REFERENCING OLD TABLE AS t_inventorycomments_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_inventorygaps
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_inventorygaps_broadcast_insert
			AFTER INSERT ON t_inventorygaps
				REFERENCING NEW TABLE AS t_inventorygaps_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_inventorygaps_broadcast_update
			AFTER UPDATE ON t_inventorygaps
				REFERENCING OLD TABLE AS t_inventorygaps_update_old NEW TABLE AS t_inventorygaps_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_inventorygaps_broadcast_delete
			AFTER DELETE ON t_inventorygaps
				REFERENCING OLD TABLE AS t_inventorygaps_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_inventorysm
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_inventorysm_broadcast_insert
			AFTER INSERT ON t_inventorysm
				REFERENCING NEW TABLE AS t_inventorysm_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_inventorysm_broadcast_update
			AFTER UPDATE ON t_inventorysm
				REFERENCING OLD TABLE AS t_inventorysm_update_old NEW TABLE AS t_inventorysm_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_inventorysm_broadcast_delete
			AFTER DELETE ON t_inventorysm
				REFERENCING OLD TABLE AS t_inventorysm_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_ordersinput
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_ordersinput_broadcast_insert
			AFTER INSERT ON t_ordersinput
				REFERENCING NEW TABLE AS t_ordersinput_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_ordersinput_broadcast_update
			AFTER UPDATE ON t_ordersinput
				REFERENCING OLD TABLE AS t_ordersinput_update_old NEW TABLE AS t_ordersinput_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_ordersinput_broadcast_delete
			AFTER DELETE ON t_ordersinput
				REFERENCING OLD TABLE AS t_ordersinput_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_ordersqueue
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_ordersqueue_broadcast_insert
			AFTER INSERT ON t_ordersqueue
				REFERENCING NEW TABLE AS t_ordersqueue_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_ordersqueue_broadcast_update
			AFTER UPDATE ON t_ordersqueue
				REFERENCING OLD TABLE AS t_ordersqueue_update_old NEW TABLE AS t_ordersqueue_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_ordersqueue_broadcast_delete
			AFTER DELETE ON t_ordersqueue
				REFERENCING OLD TABLE AS t_ordersqueue_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_orderstatetransitions
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_orderstatetransitions_broadcast_insert
			AFTER INSERT ON t_orderstatetransitions
				REFERENCING NEW TABLE AS t_orderstatetransitions_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_orderstatetransitions_broadcast_update
			AFTER UPDATE ON t_orderstatetransitions
				REFERENCING OLD TABLE AS t_orderstatetransitions_update_old NEW TABLE AS t_orderstatetransitions_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_orderstatetransitions_broadcast_delete
			AFTER DELETE ON t_orderstatetransitions
				REFERENCING OLD TABLE AS t_orderstatetransitions_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_receptionrules
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_receptionrules_broadcast_insert
			AFTER INSERT ON t_receptionrules
				REFERENCING NEW TABLE AS t_receptionrules_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_receptionrules_broadcast_update
			AFTER UPDATE ON t_receptionrules
				REFERENCING OLD TABLE AS t_receptionrules_update_old NEW TABLE AS t_receptionrules_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_receptionrules_broadcast_delete
			AFTER DELETE ON t_receptionrules
				REFERENCING OLD TABLE AS t_receptionrules_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_repositories
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_repositories_broadcast_insert
			AFTER INSERT ON t_repositories
				REFERENCING NEW TABLE AS t_repositories_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_repositories_broadcast_update
			AFTER UPDATE ON t_repositories
				REFERENCING OLD TABLE AS t_repositories_update_old NEW TABLE AS t_repositories_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_repositories_broadcast_delete
			AFTER DELETE ON t_repositories
				REFERENCING OLD TABLE AS t_repositories_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_smactions
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_smactions_broadcast_insert
			AFTER INSERT ON t_smactions
				REFERENCING NEW TABLE AS t_smactions_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_smactions_broadcast_update
			AFTER UPDATE ON t_smactions
				REFERENCING OLD TABLE AS t_smactions_update_old NEW TABLE AS t_smactions_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_smactions_broadcast_delete
			AFTER DELETE ON t_smactions
				REFERENCING OLD TABLE AS t_smactions_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_smqueue
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_smqueue_broadcast_insert
			AFTER INSERT ON t_smqueue
				REFERENCING NEW TABLE AS t_smqueue_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_smqueue_broadcast_update
			AFTER UPDATE ON t_smqueue
				REFERENCING OLD TABLE AS t_smqueue_update_old NEW TABLE AS t_smqueue_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_smqueue_broadcast_delete
			AFTER DELETE ON t_smqueue
				REFERENCING OLD TABLE AS t_smqueue_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_temporarydistribution
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_temporarydistribution_broadcast_insert
			AFTER INSERT ON t_temporarydistribution
				REFERENCING NEW TABLE AS t_temporarydistribution_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_temporarydistribution_broadcast_update
			AFTER UPDATE ON t_temporarydistribution
				REFERENCING OLD TABLE AS t_temporarydistribution_update_old NEW TABLE AS t_temporarydistribution_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_temporarydistribution_broadcast_delete
			AFTER DELETE ON t_temporarydistribution
				REFERENCING OLD TABLE AS t_temporarydistribution_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_temporaryorders
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_temporaryorders_broadcast_insert
			AFTER INSERT ON t_temporaryorders
				REFERENCING NEW TABLE AS t_temporaryorders_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_temporaryorders_broadcast_update
			AFTER UPDATE ON t_temporaryorders
				REFERENCING OLD TABLE AS t_temporaryorders_update_old NEW TABLE AS t_temporaryorders_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_temporaryorders_broadcast_delete
			AFTER DELETE ON t_temporaryorders
				REFERENCING OLD TABLE AS t_temporaryorders_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_users
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_users_broadcast_insert
			AFTER INSERT ON t_users
				REFERENCING NEW TABLE AS t_users_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_users_broadcast_update
			AFTER UPDATE ON t_users
				REFERENCING OLD TABLE AS t_users_update_old NEW TABLE AS t_users_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_users_broadcast_delete
			AFTER DELETE ON t_users
				REFERENCING OLD TABLE AS t_users_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_waitingorders
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_waitingorders_broadcast_insert
			AFTER INSERT ON t_waitingorders
				REFERENCING NEW TABLE AS t_waitingorders_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events('ORDWAIT');

		CREATE TRIGGER tr_waitingorders_broadcast_update
			AFTER UPDATE ON t_waitingorders
				REFERENCING OLD TABLE AS t_waitingorders_update_old NEW TABLE AS t_waitingorders_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_waitingorders_broadcast_delete
			AFTER DELETE ON t_waitingorders
				REFERENCING OLD TABLE AS t_waitingorders_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;
