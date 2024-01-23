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
--                                FOREIGN KEYS                               --
--                                                                           --
-------------------------------------------------------------------------------

/* ------------------------------
	t_acshsminvrelation
*/ ------------------------------
ALTER TABLE t_acshsminvrelation
	ADD CONSTRAINT fk_t_acshsminvrelation
		FOREIGN KEY (id)
			REFERENCES t_inventorysm (id)
				ON DELETE CASCADE ;



/* ------------------------------
	t_acsisminvrelation
*/ ------------------------------
ALTER TABLE t_acsisminvrelation 
	ADD CONSTRAINT fk_t_acsisminvrel
		FOREIGN KEY (id)
			REFERENCES t_inventorysm (id)
				ON DELETE CASCADE ;



/* ------------------------------
	t_acsltainvrelation
*/ ------------------------------
ALTER TABLE t_acsltainvrelation 
	ADD CONSTRAINT fk_acsltainvrelation1
		FOREIGN KEY (id)
			REFERENCES t_inventorysm (id) 
				ON DELETE CASCADE ;



/* ------------------------------
	t_applicationevents
*/ ------------------------------
ALTER TABLE t_applicationevents 
	ADD CONSTRAINT fk_applicationevents0l
		FOREIGN KEY (application_id)
			REFERENCES t_applications (id_application)
				ON DELETE CASCADE ;



/* ------------------------------
	t_asyncsm_operations
*/ ------------------------------
ALTER TABLE t_asyncsm_operations
	ADD CONSTRAINT fk_t_asyncsm_operations1
		FOREIGN KEY (operation_id)
			REFERENCES t_operationtypes (id_operationtype) ;

ALTER TABLE t_asyncsm_operations
	ADD CONSTRAINT fk_t_asyncsm_operations2
		FOREIGN KEY (sm_id)
			REFERENCES t_storagemanager (id) ;

ALTER TABLE t_asyncsm_operations
	ADD CONSTRAINT fk_t_asyncsm_operations3
		FOREIGN KEY (status_id)
			REFERENCES t_stati (id) ;



/* ------------------------------
	t_cdp_feederqueue
*/ ------------------------------
ALTER TABLE t_cdp_feederqueue
	ADD CONSTRAINT fk_cdp_feederqueue01
		FOREIGN KEY (status_id)
			REFERENCES t_stati (id)
				ON DELETE CASCADE ;

ALTER TABLE t_cdp_feederqueue
	ADD CONSTRAINT fk_cdp_feederqueue02
		FOREIGN KEY (tempnamestrategy_id)
			REFERENCES t_tmpnamestrategy (id_tmpnamestrategy)
				ON DELETE CASCADE ;


/* ------------------------------
	t_cdp_frame_grid
*/ ------------------------------
ALTER TABLE t_cdp_frame_grid 
	ADD CONSTRAINT fk_cdp_frame_grid01
		FOREIGN KEY (filetype) 
			REFERENCES t_filetypes (filetype) 
				ON DELETE CASCADE ;

ALTER TABLE t_cdp_frame_grid 
	ADD CONSTRAINT fk_cdp_frame_grid02
		FOREIGN KEY (ordertype) 
			REFERENCES t_ordertypes (ordertype) 
				ON DELETE CASCADE ;


/* ------------------------------
	t_cdp_notification_queue
*/ ------------------------------
ALTER TABLE t_cdp_notification_queue
	ADD CONSTRAINT fk_cdp_notification_queue01
		FOREIGN KEY (feederqueue_id)
			REFERENCES t_cdp_feederqueue (id)
				ON DELETE CASCADE ;

ALTER TABLE t_cdp_notification_queue
	ADD CONSTRAINT fk_cdp_notification_queue02
		FOREIGN KEY (status_id)
			REFERENCES t_stati (id)
				ON DELETE CASCADE ;


/* ------------------------------
	 t_classifiedfiletypes
*/ ------------------------------
ALTER TABLE t_classifiedfiletypes
	ADD CONSTRAINT FK_t_classifiedfiletypes
		FOREIGN KEY (filetype)
			REFERENCES t_filetypes (filetype)
				ON DELETE CASCADE ;



/* ------------------------------
	t_cleanuprules 
*/ ------------------------------
ALTER TABLE t_cleanuprules 
	ADD CONSTRAINT fk_cleanuprules1
		FOREIGN KEY (repository_id)
			REFERENCES t_repositories (id_repository) 
				ON DELETE CASCADE ;

ALTER TABLE t_cleanuprules 
	ADD CONSTRAINT fk_cleanuprules2
		FOREIGN KEY (cleanupalgo_id)
			REFERENCES t_cleanupalgos (id_cleanupalgo) 
				ON DELETE CASCADE ;



/* ------------------------------
	t_dbparameters
*/ ------------------------------
ALTER TABLE t_dbparameters 
	ADD CONSTRAINT fk_dbparameters01 
		FOREIGN KEY (parametertype_id) 
			REFERENCES t_dbparametertypes (id) 
				ON DELETE CASCADE ;



/* ------------------------------
	t_discacheinvrelation
*/ ------------------------------
ALTER TABLE t_discacheinvrelation
  ADD CONSTRAINT fk_t_discacheinvrelation01
  	FOREIGN KEY (id)
  	  REFERENCES t_inventorysm (id)
  	  	ON DELETE CASCADE ;



/* --------------------------
	t_distributionitems
*/ --------------------------
ALTER TABLE t_distributionitems
	ADD CONSTRAINT fk_t_distributionitems1
		FOREIGN KEY (packageid)
			REFERENCES t_distributionqueue (packageid)
				ON DELETE CASCADE ;

ALTER TABLE t_distributionitems
	ADD CONSTRAINT fk_t_distributionitems2
		FOREIGN KEY (mediatype)
			REFERENCES t_mediatypes (mediatype)
				ON DELETE CASCADE ;

ALTER TABLE t_distributionitems
	ADD CONSTRAINT fk_t_distributionitems3
		FOREIGN KEY (userid)
			REFERENCES t_users (userid)
			  	ON DELETE CASCADE ;

ALTER TABLE t_distributionitems
	ADD CONSTRAINT fk_t_distributionitems4
		FOREIGN KEY (filename, fileversion)
			REFERENCES t_inventory (filename, fileversion)
				ON DELETE CASCADE ;

ALTER TABLE t_distributionitems
	ADD CONSTRAINT fk_t_distributionitems5
		FOREIGN KEY (format)
			REFERENCES t_tasktables (id_tasktable)
				ON DELETE CASCADE ;

ALTER TABLE t_distributionitems 
	ADD CONSTRAINT fk_t_distributionitems6
		FOREIGN KEY (cartid)
			REFERENCES t_cartsref (id) 
				ON DELETE CASCADE ;

ALTER TABLE t_distributionitems 
	ADD CONSTRAINT fk_t_distributionitems7
		FOREIGN KEY (distributionrule_id)
			REFERENCES t_distributionrules (id_distributionrule) 
				ON DELETE CASCADE ;


ALTER TABLE t_distributionitems 
	ADD CONSTRAINT fk_t_distributionitems8
		FOREIGN KEY (disseminationpackingalgo_id)
			REFERENCES t_disseminationpackingalgos (id_disseminationpackingalgo) 
				ON DELETE CASCADE ;



/* --------------------------
	t_distributionqueue
*/ --------------------------
ALTER TABLE t_distributionqueue
	ADD CONSTRAINT fk_t_distributionqueue1
		FOREIGN KEY (format)
			REFERENCES t_tasktables (id_tasktable)
				ON DELETE CASCADE ;



/* ------------------------------
	t_distributionrules
*/ ------------------------------
ALTER TABLE t_distributionrules 
	ADD CONSTRAINT fk_distributionrules01
		FOREIGN KEY (filetype_id)
			REFERENCES t_filetypes (id_filetype) 
				ON DELETE CASCADE ;

ALTER TABLE t_distributionrules 
	ADD CONSTRAINT fk_distributionrules02
		FOREIGN KEY (mediatype_id)
			REFERENCES t_mediatypes (id_mediatype) 
				ON DELETE CASCADE ;

ALTER TABLE t_distributionrules 
	ADD CONSTRAINT fk_distributionrules03
		FOREIGN KEY (repository_id)
			REFERENCES t_repositories (id_repository) 
				ON DELETE CASCADE ;

ALTER TABLE t_distributionrules 
	ADD CONSTRAINT fk_distributionrules04
		FOREIGN KEY (user_id)
			REFERENCES t_users (id_user) 
				ON DELETE CASCADE ;

ALTER TABLE t_distributionrules 
	ADD CONSTRAINT fk_distributionrules05
		FOREIGN KEY (usermode_id)
			REFERENCES t_usermodes (id) 
				ON DELETE CASCADE ;

ALTER TABLE t_distributionrules 
	ADD CONSTRAINT fk_distributionrules06
		FOREIGN KEY (distributionalgo_id)
			REFERENCES t_distributionalgo (id) 
				ON DELETE CASCADE ;

ALTER TABLE t_distributionrules 
	ADD CONSTRAINT fk_distributionrules07
		FOREIGN KEY (format_tt_id)
			REFERENCES t_tasktables (id_tasktable) 
				ON DELETE CASCADE ;

ALTER TABLE t_distributionrules 
	ADD CONSTRAINT fk_distributionrules08
		FOREIGN KEY (delivery_tt_id)
			REFERENCES t_tasktables (id_tasktable) 
				ON DELETE CASCADE ;

ALTER TABLE t_distributionrules 
	ADD CONSTRAINT fk_distributionrules09
		FOREIGN KEY (delcheck_tt_id)
			REFERENCES t_tasktables (id_tasktable) 
				ON DELETE CASCADE ;

ALTER TABLE t_distributionrules 
	ADD CONSTRAINT fk_distributionrules10
		FOREIGN KEY (fileclass_id)
			REFERENCES t_fileclasses (id_fileclass) 
				ON DELETE CASCADE ;

ALTER TABLE t_distributionrules 
	ADD CONSTRAINT fk_distributionrules11
		FOREIGN KEY (disseminationpackingalgo_id)
			REFERENCES t_disseminationpackingalgos (id_disseminationpackingalgo) 
				ON DELETE CASCADE ;



/* ------------------------------
	t_distributionruleslock
*/ ------------------------------
ALTER TABLE t_distributionruleslock
  ADD CONSTRAINT fk_t_distributionruleslock01
  	FOREIGN KEY (packageid)
	  REFERENCES t_distributionqueue (packageid)
  	  	ON DELETE CASCADE ;



/* ------------------------------
	t_distributionwaitingitems
*/ ------------------------------
ALTER TABLE t_distributionwaitingitems
	ADD CONSTRAINT fk_distributionwaitingitems02
		FOREIGN KEY (package_id)
			REFERENCES t_distributionqueue (packageid)
				ON DELETE CASCADE ;

ALTER TABLE t_distributionwaitingitems
	ADD CONSTRAINT fk_distributionwaitingitems03
		FOREIGN KEY (inventory_id)
			REFERENCES t_inventory (id_inv)
				ON DELETE CASCADE ;



/* ------------------------------
	t_emitted_order_events
*/ ------------------------------
ALTER TABLE t_emitted_order_events
	ADD CONSTRAINT fk_emitted_order_events01
		FOREIGN KEY (processor_id)
			REFERENCES t_processors (id_processor)
				ON DELETE CASCADE ;

ALTER TABLE t_emitted_order_events
	ADD CONSTRAINT fk_emitted_order_events02
		FOREIGN KEY (order_type)
			REFERENCES t_ordertypes (ordertype)
				ON DELETE CASCADE ;



/* --------------------------
	t_filetypescomps
*/ --------------------------
ALTER TABLE t_filetypescomps 
	ADD CONSTRAINT fk_filetypescomps01
		FOREIGN KEY (pfiletype) 
			REFERENCES t_filetypes (id_filetype) 
				ON DELETE CASCADE ;

ALTER TABLE t_filetypescomps 
	ADD CONSTRAINT fk_filetypescomps02
		FOREIGN KEY (cfiletype) 
			REFERENCES t_filetypes (id_filetype) 
				ON DELETE CASCADE ;



/* ------------------------------
	t_inv_additional_params
*/ ------------------------------
ALTER TABLE t_inv_additional_params 
	ADD CONSTRAINT fk_inv_additional_params01
		FOREIGN KEY (inv_id)
			REFERENCES t_inventory (id_inv)
				ON DELETE CASCADE ;

ALTER TABLE t_inv_additional_params 
	ADD CONSTRAINT fk_inv_additional_params02
		FOREIGN KEY (timeliness_key_id)
			REFERENCES t_downlink_session_tracks (id)
				ON DELETE CASCADE ;



/* ------------------------------
	t_inv_data_restored
*/ ------------------------------
ALTER TABLE t_inv_data_restored
	ADD CONSTRAINT fk_inv_data_restored01
		FOREIGN KEY (id)
			REFERENCES t_inventorysm (id)
				ON DELETE CASCADE ;

ALTER TABLE t_inv_data_restored
	ADD CONSTRAINT fk_inv_data_restored02
		FOREIGN KEY (sm_id)
			REFERENCES t_storagemanager (id)
				ON DELETE CASCADE ;

ALTER TABLE t_inv_data_restored
	ADD CONSTRAINT fk_inv_data_restored03
		FOREIGN KEY (inv_id)
			REFERENCES t_inventory (id_inv)
				ON DELETE CASCADE ;

ALTER TABLE t_inv_data_restored
	ADD CONSTRAINT fk_inv_data_restored04
		FOREIGN KEY (parent_inv_id)
			REFERENCES t_inventory (id_inv)
				ON DELETE CASCADE ;

ALTER TABLE t_inv_data_restored
	ADD CONSTRAINT fk_inv_data_restored05
		FOREIGN KEY (filetype_id)
			REFERENCES t_filetypes (id_filetype)
				ON DELETE CASCADE ;



/* --------------------------
	t_inv_distributions
*/ --------------------------
ALTER TABLE t_inv_distributions 
	ADD CONSTRAINT fk_inv_distributions01 
		FOREIGN KEY (inv_id) 
			REFERENCES t_inventory (id_inv) 
				ON DELETE CASCADE ;



/* --------------------------
	t_inv_downloads
*/ --------------------------
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



/* --------------------------
	t_inventory
*/ --------------------------
ALTER TABLE t_inventory
	ADD CONSTRAINT fk_t_inventory1
		FOREIGN KEY (fileclass)
			REFERENCES t_fileclasses (fileclass)
				ON DELETE CASCADE ;

ALTER TABLE t_inventory
	ADD CONSTRAINT fk_t_inventory2
		FOREIGN KEY (sensorid)
			REFERENCES t_sensors (sensorid)
				ON DELETE CASCADE ;

ALTER TABLE t_inventory
	ADD CONSTRAINT fk_t_inventory3
		FOREIGN KEY (satelliteid)
			REFERENCES t_satellites (satelliteid)
				ON DELETE CASCADE ;

ALTER TABLE t_inventory
	ADD CONSTRAINT fk_t_inventory4
		FOREIGN KEY (acquisitionstationid)
			REFERENCES t_stations (stationid)
				ON DELETE CASCADE ;

ALTER TABLE t_inventory
	ADD CONSTRAINT fk_t_inventory5
		FOREIGN KEY (filetype)
			REFERENCES t_filetypes (filetype)
				ON DELETE CASCADE ;

ALTER TABLE t_inventory
	ADD CONSTRAINT fk_t_inventory6
		FOREIGN KEY (processingstationid)
			REFERENCES t_stations (stationid)
				ON DELETE CASCADE ;

ALTER TABLE t_inventory
	ADD CONSTRAINT fk_inventory7
		FOREIGN KEY	(qualified)
			REFERENCES t_qualifiedflag (id)
				ON DELETE CASCADE ;

ALTER TABLE t_inventory 
	ADD CONSTRAINT fk_inventory8
		FOREIGN KEY (band)
			REFERENCES t_bands (id) 
				ON DELETE CASCADE ;

ALTER TABLE t_inventory 
	ADD CONSTRAINT fk_inventory9 
		FOREIGN KEY (originator)
			REFERENCES t_originators (id) ;



/* --------------------------
	t_inventorycomments
*/ --------------------------
ALTER TABLE t_inventorycomments
	ADD CONSTRAINT fk_t_inventorycomments
		FOREIGN KEY (filename, fileversion)
			REFERENCES t_inventory (filename, fileversion)
				ON DELETE CASCADE ;



/* ------------------------------
	t_inventoryextra
*/ ------------------------------
ALTER TABLE t_inventoryextra
	ADD CONSTRAINT fk_t_inventoryextra
		FOREIGN KEY (inv_id)
			REFERENCES t_inventory (id_inv)
				ON DELETE CASCADE ;



/* ------------------------------
	t_objsminvrelation
*/ ------------------------------
ALTER TABLE t_objsminvrelation
	ADD CONSTRAINT fk_objsminvrelation01
		FOREIGN KEY (id)
			REFERENCES t_inventorysm (id)
				ON DELETE CASCADE;



/* --------------------------
	t_ondemanddistribution
*/ --------------------------
ALTER TABLE t_ondemanddistribution
	ADD CONSTRAINT fk_t_ondemanddistribution1
		FOREIGN KEY (orderid)
			REFERENCES t_ordersqueue (orderid)
				ON DELETE CASCADE ;

ALTER TABLE t_ondemanddistribution
	ADD CONSTRAINT fk_t_ondemanddistribution2
		FOREIGN KEY (userid)
			REFERENCES t_users (userid)
				ON DELETE CASCADE ;

ALTER TABLE t_ondemanddistribution
	ADD CONSTRAINT fk_t_ondemanddistribution3
		FOREIGN KEY (format)
			REFERENCES t_tasktables (id_tasktable)
				ON DELETE CASCADE ;

ALTER TABLE t_ondemanddistribution 
	ADD CONSTRAINT fk_t_ondemanddistribution4 
		FOREIGN KEY (mediatype)
			REFERENCES t_mediatypes (mediatype) 
				ON DELETE CASCADE ;

ALTER TABLE t_ondemanddistribution 
	ADD CONSTRAINT fk_t_ondemanddistribution5
		FOREIGN KEY (disseminationpackingalgo_id)
			REFERENCES t_disseminationpackingalgos (id_disseminationpackingalgo) 
				ON DELETE CASCADE ;



/* ------------------------------
	t_orderstatetransitions
*/ ------------------------------
ALTER TABLE t_orderstatetransitions
	ADD CONSTRAINT fk_t_orderstatetransitions
		FOREIGN KEY (orderid)
			REFERENCES t_ordersqueue (orderid)
				ON DELETE CASCADE ;



/* ------------------------------
	t_proc_ftype_generation
*/ ------------------------------
ALTER TABLE t_proc_ftype_generation
	ADD CONSTRAINT fk_proc_ftype_generation01
		FOREIGN KEY (proc_id)
			REFERENCES t_processors (id_processor)
				ON DELETE CASCADE ;

ALTER TABLE t_proc_ftype_generation
	ADD CONSTRAINT fk_proc_ftype_generation02
		FOREIGN KEY (ftype_id)
			REFERENCES t_filetypes (id_filetype)
				ON DELETE CASCADE ;



/* ------------------------------
	t_receptionrules
*/ ------------------------------
ALTER TABLE t_receptionrules 
	ADD CONSTRAINT fk_receptionrules1
		FOREIGN KEY (filetype_id)
			REFERENCES t_filetypes (id_filetype) 
				ON DELETE CASCADE ;

ALTER TABLE t_receptionrules 
	ADD CONSTRAINT fk_receptionrules2
		FOREIGN KEY (repository_id)
			REFERENCES t_repositories (id_repository) 
				ON DELETE CASCADE ;

ALTER TABLE t_receptionrules 
	ADD CONSTRAINT fk_receptionrules3
		FOREIGN KEY (user_id)
			REFERENCES t_users (id_user) 
				ON DELETE CASCADE ;

ALTER TABLE t_receptionrules 
	ADD CONSTRAINT fk_receptionrules4
		FOREIGN KEY (usermode_id)
			REFERENCES t_usermodes (id) 
				ON DELETE CASCADE ;

ALTER TABLE t_receptionrules 
	ADD CONSTRAINT fk_receptionrules5
		FOREIGN KEY (reception_tt_id)
			REFERENCES t_tasktables (id_tasktable) 
				ON DELETE CASCADE ;

ALTER TABLE t_receptionrules 
	ADD CONSTRAINT fk_receptionrules6
		FOREIGN KEY (discoveryplugin_id)
			REFERENCES t_discoveryplugins (id_discoveryplugin) 
				ON DELETE CASCADE ;

ALTER TABLE t_receptionrules 
	ADD CONSTRAINT fk_receptionrules7
		FOREIGN KEY (satellite_id)
			REFERENCES t_satellites (satelliteid) 
				ON DELETE CASCADE ;

ALTER TABLE t_receptionrules 
	ADD CONSTRAINT fk_receptionrules8
		FOREIGN KEY (sensor_id)
			REFERENCES t_sensors (sensorid) 
				ON DELETE CASCADE ;



/* ------------------------------
	t_receptionruleshist
*/ ------------------------------
ALTER TABLE t_receptionruleshist 
	ADD CONSTRAINT fk_receptionruleshist1
		FOREIGN KEY (receptionrule_id)
			REFERENCES t_receptionrules (id_receptionrule) 
				ON DELETE CASCADE ;

ALTER TABLE t_receptionruleshist 
	ADD CONSTRAINT fk_receptionruleshist2
		FOREIGN KEY (status_id)
			REFERENCES t_stati (id) 
				ON DELETE CASCADE ;



/* ------------------------------
	t_receptionruleslock
*/ ------------------------------
ALTER TABLE t_receptionruleslock
	ADD CONSTRAINT fk_receptionruleslock01
		FOREIGN KEY (receptionrule_id)
			REFERENCES t_receptionrules (id_receptionrule)
				ON DELETE CASCADE ;



/* ------------------------------
	t_satrelations
*/ ------------------------------
ALTER TABLE t_satrelations
	ADD CONSTRAINT fk_t_satrelations1
		FOREIGN KEY (satelliteid) REFERENCES t_satellites (satelliteid)
			ON DELETE CASCADE ;

ALTER TABLE t_satrelations
	ADD CONSTRAINT fk_t_satrelations2
		FOREIGN KEY (sensorid) REFERENCES t_sensors (sensorid)
			ON DELETE CASCADE ;



/* ------------------------------
	t_sm_sync_missingactions
*/ ------------------------------
ALTER TABLE t_sm_sync_missingactions
	ADD CONSTRAINT fk_sm_sync_missingactions01
		FOREIGN KEY (sm_id)
			REFERENCES t_storagemanager (id)
				ON DELETE CASCADE ;

ALTER TABLE t_sm_sync_missingactions
	ADD CONSTRAINT fk_sm_sync_missingactions02
		FOREIGN KEY (inv_id)
			REFERENCES t_inventory (id_inv)
				ON DELETE CASCADE ;

ALTER TABLE t_sm_sync_missingactions
	ADD CONSTRAINT fk_sm_sync_missingactions03
		FOREIGN KEY (invsm_id)
			REFERENCES t_inventorysm (id)
				ON DELETE CASCADE ;



/* ------------------------------
	t_statetransitions_allowed
*/ ------------------------------
ALTER TABLE t_statetransitions_allowed 
	ADD CONSTRAINT fk_statetransitions_allowed01
		FOREIGN KEY (table_id)
			REFERENCES t_statetransitions_tables (id)
				ON DELETE CASCADE ;

ALTER TABLE t_statetransitions_allowed 
	ADD CONSTRAINT fk_statetransitions_allowed02
		FOREIGN KEY (prev_state_id)
			REFERENCES t_stati (id)
				ON DELETE CASCADE ;

ALTER TABLE t_statetransitions_allowed 
	ADD CONSTRAINT fk_statetransitions_allowed03
		FOREIGN KEY (post_state_id)
			REFERENCES t_stati (id)
				ON DELETE CASCADE ;



/* ------------------------------
	t_statetransitions_tables
*/ ------------------------------
ALTER TABLE t_statetransitions_tables 
	ADD CONSTRAINT fk_statetransitions_tables01
		FOREIGN KEY (flow_id)
			REFERENCES t_statetransitions_flows (id)
				ON DELETE CASCADE ;



/* ------------------------------
	t_storagerollingthresholds 
*/ ------------------------------
ALTER TABLE t_storagerollingthresholds 
	ADD CONSTRAINT fk_storagerollingthresholds01
		FOREIGN KEY (storagemanager_id) 
			REFERENCES t_storagemanager (id) 
				ON DELETE CASCADE ;

ALTER TABLE t_storagerollingthresholds 
	ADD CONSTRAINT fk_storagerollingthresholds02
		FOREIGN KEY (action_id) 
			REFERENCES t_rollingpolicyactions (id) 
				ON DELETE CASCADE ;



/* ------------------------------
	t_tasktables
*/ ------------------------------
ALTER TABLE t_tasktables 
	ADD CONSTRAINT fk_tasktables1
		FOREIGN KEY (type_id)
			REFERENCES t_operationtypes (id_operationtype) 
				ON DELETE CASCADE ;



/* ------------------------------
	t_temporaryorders
*/ ------------------------------
ALTER TABLE t_temporaryorders
	ADD CONSTRAINT fk_t_temporaryorders1
		FOREIGN KEY (ordersat)
			REFERENCES t_satellites (satelliteid)
				ON DELETE CASCADE ;

ALTER TABLE t_temporaryorders
	ADD CONSTRAINT fk_t_temporaryorders2
		FOREIGN KEY (filename, fileversion)
			REFERENCES t_inventory (filename, fileversion)
			ON DELETE CASCADE ;



/* ------------------------------
	t_mediacapformat
*/ ------------------------------
ALTER TABLE t_mediacapformat
  ADD CONSTRAINT fk_t_mediacapformat1
  	FOREIGN KEY (mediatype)
	  REFERENCES t_mediatypes (mediatype)
	  	ON DELETE CASCADE ;



/* ------------------------------
	t_invspecialrollingpolicies
*/ ------------------------------
ALTER TABLE t_invspecialrollingpolicies
	ADD CONSTRAINT fk_t_invspecialrollingpolicies1
		FOREIGN KEY (inventoryname, inventoryversion)
			REFERENCES t_inventory (filename, fileversion)
				ON DELETE CASCADE ;

ALTER TABLE t_invspecialrollingpolicies
	ADD CONSTRAINT fk_t_invspecialrollingpolicies2
		FOREIGN KEY (action)
			REFERENCES t_rollingpolicyactions (id)
				ON DELETE CASCADE ;

ALTER TABLE t_invspecialrollingpolicies 
	ADD CONSTRAINT fk_invspecrollpol_storage
		FOREIGN KEY (storageid)
			REFERENCES t_storagemanager (id) 
				ON DELETE CASCADE ;



/* --------------------------
	t_ordersgeneration
*/ --------------------------
ALTER TABLE t_ordersgeneration
	ADD CONSTRAINT fk_t_ordersgeneration1
		FOREIGN KEY (processorname, processorversion)
			REFERENCES t_processors (processorname, processorversion)  
				ON DELETE CASCADE ;

ALTER TABLE t_ordersgeneration
	ADD CONSTRAINT fk_t_ordersgeneration2
		FOREIGN KEY (filetype)
			REFERENCES t_filetypes (filetype)
				ON DELETE CASCADE ;

ALTER TABLE t_ordersgeneration
	ADD CONSTRAINT fk_t_ordersgeneration3
		FOREIGN KEY (ordertype)
			REFERENCES t_ordertypes (ordertype)
				ON DELETE CASCADE ;

ALTER TABLE t_ordersgeneration
	ADD CONSTRAINT fk_t_ordersgeneration4
		FOREIGN KEY (algorithm)
			REFERENCES t_algorithm (algorithm)
				ON DELETE CASCADE ;

ALTER TABLE t_ordersgeneration
	ADD CONSTRAINT fk_t_ordersgeneration5
		FOREIGN KEY (fileclass)
			REFERENCES t_fileclasses (fileclass)
				ON DELETE CASCADE ;



/* --------------------------
	t_ordersinput
*/ --------------------------
ALTER TABLE t_ordersinput
	ADD CONSTRAINT fk_t_ordersinput1
		FOREIGN KEY (filename, fileversion)
			REFERENCES t_inventory (filename, fileversion)
				ON DELETE CASCADE ;

ALTER TABLE t_ordersinput
	ADD CONSTRAINT fk_t_ordersinput2
		FOREIGN KEY (orderid)
			REFERENCES t_ordersqueue (orderid)
				ON DELETE CASCADE ;



/* ------------------------------
	t_ordersqueueblacklist
*/ ------------------------------
ALTER TABLE t_ordersqueueblacklist
	ADD CONSTRAINT fk_t_ordersqueueblacklist1
		FOREIGN KEY (orderid)
			REFERENCES t_ordersqueue (orderid)
				ON DELETE CASCADE ;


/* --------------------------
	t_invrollingpolicies
*/ --------------------------
ALTER TABLE t_invrollingpolicies
	ADD CONSTRAINT fk_t_invrollingpolicies1
		FOREIGN KEY (filetype)
			REFERENCES t_filetypes (filetype)
				ON DELETE CASCADE ;

ALTER TABLE t_invrollingpolicies
	ADD CONSTRAINT fk_t_invrollingpolicies2
		FOREIGN KEY (algorithm)
			REFERENCES t_rollingpoliciesalgo (id)
				ON DELETE CASCADE ;

ALTER TABLE t_invrollingpolicies
	ADD CONSTRAINT fk_t_invrollingpolicies3
		FOREIGN KEY (action)
			REFERENCES t_rollingpolicyactions (id)
				ON DELETE CASCADE ;

ALTER TABLE t_invrollingpolicies
	ADD CONSTRAINT fk_t_invrollingpolicies4
		FOREIGN KEY (fileclass)
			REFERENCES t_fileclasses (fileclass)
				ON DELETE CASCADE ;

ALTER TABLE t_invrollingpolicies 
		ADD CONSTRAINT fk_invrollpol_storage
			FOREIGN KEY (storageid)
				REFERENCES t_storagemanager (id) 
					ON DELETE CASCADE ;



/* --------------------------
	t_ordersqueue
*/ --------------------------
ALTER TABLE t_ordersqueue
	ADD CONSTRAINT fk_t_ordersqueue1
		FOREIGN KEY (ordertype)
			REFERENCES t_ordertypes (ordertype)
				ON DELETE CASCADE ;

ALTER TABLE t_ordersqueue
	ADD CONSTRAINT fk_t_ordersqueue2
		FOREIGN KEY (processorname, processorversion)
			REFERENCES t_processors (processorname, processorversion)
				ON DELETE CASCADE ;

ALTER TABLE t_ordersqueue
	ADD CONSTRAINT fk_t_ordersqueue3
		FOREIGN KEY (satelliteid)
			REFERENCES t_satellites (satelliteid)
				ON DELETE CASCADE ;



/* ------------------------------
	t_storagemanager
*/ ------------------------------
ALTER TABLE t_storagemanager
	ADD CONSTRAINT fk_t_storagemanager1
		FOREIGN KEY (type)
			REFERENCES t_smtype (type)
				ON DELETE CASCADE ;

ALTER TABLE t_storagemanager
	ADD CONSTRAINT fk_t_storagemanager2
		FOREIGN KEY (migrationalgo_id)
			REFERENCES t_migrationalgos (id_migrationalgo)
				ON DELETE CASCADE ;



/* --------------------------
	t_inventorysm
*/ --------------------------
ALTER TABLE t_inventorysm
	ADD CONSTRAINT fk_t_inventorysm1
		FOREIGN KEY (inv_id)
			REFERENCES t_inventory (id_inv)
				ON DELETE CASCADE ;

ALTER TABLE t_inventorysm
	ADD CONSTRAINT fk_t_inventorysm2
		FOREIGN KEY (storagemanid)
			REFERENCES t_storagemanager (id)
				ON DELETE CASCADE ;



/* --------------------------
	t_storagepriority
*/ --------------------------
ALTER TABLE t_storagepriority
  ADD CONSTRAINT fk_t_storagepriority1
  	FOREIGN KEY (id)
  	  REFERENCES t_storagemanager (id)
  	  	ON DELETE CASCADE ;

ALTER TABLE t_storagepriority
  ADD CONSTRAINT fk_t_storagepriority2
  	FOREIGN KEY (filetype)
  	  REFERENCES t_filetypes (filetype)
  	  	ON DELETE CASCADE ;



/* --------------------------
	t_inventorygaps
*/ --------------------------
ALTER TABLE t_inventorygaps
  ADD CONSTRAINT fk_t_inventorygaps
  	FOREIGN KEY (filename, fileversion)
  	  REFERENCES t_inventory (filename, fileversion)
  	  	ON DELETE CASCADE ;



/* --------------------------
	t_pendinginventory
*/ --------------------------
ALTER TABLE t_pendinginventory
  ADD CONSTRAINT fk_t_pendinginventory
  	FOREIGN KEY (filename, fileversion)
  	  REFERENCES t_inventory (filename, fileversion)
  	  	ON DELETE CASCADE ;



/* --------------------------
	t_inventorydataset
*/ --------------------------
ALTER TABLE t_inventorydataset
  ADD CONSTRAINT fk_t_inventorydataset1
  	FOREIGN KEY (filename, fileversion)
  	  REFERENCES t_inventory (filename, fileversion)
  	  	ON DELETE CASCADE ;

ALTER TABLE t_inventorydataset
  ADD CONSTRAINT fk_t_inventorydataset2
  	FOREIGN KEY (measdatasetid)
  	  REFERENCES t_measdataset (id)
  	  	ON DELETE CASCADE ;



/* ------------------------------
	t_temporarydistribution
*/ ------------------------------
ALTER TABLE t_temporarydistribution
	ADD CONSTRAINT fk_t_temporarydistribution1
		FOREIGN KEY (policy)
			REFERENCES t_distributionrules (id_distributionrule)
				ON DELETE CASCADE ;

ALTER TABLE t_temporarydistribution
	ADD CONSTRAINT fk_t_temporarydistribution2
		FOREIGN KEY (mediatype)
			REFERENCES t_mediatypes	(mediatype)
				ON DELETE CASCADE ;

ALTER TABLE t_temporarydistribution
	ADD CONSTRAINT fk_t_temporarydistribution3
		FOREIGN KEY (userid)
			REFERENCES t_users (userid)
				ON DELETE CASCADE ;

ALTER TABLE t_temporarydistribution
	ADD CONSTRAINT fk_t_temporarydistribution4
		FOREIGN KEY (filename, fileversion)
			REFERENCES t_inventory (filename, fileversion)
				ON DELETE CASCADE ;

ALTER TABLE t_temporarydistribution
	ADD CONSTRAINT fk_t_temporarydistribution5
		FOREIGN KEY	(format)
			REFERENCES t_tasktables (id_tasktable)
				ON DELETE CASCADE ;

ALTER TABLE t_temporarydistribution
	ADD CONSTRAINT fk_t_temporarydistribution6
		FOREIGN KEY	(algo)
			REFERENCES t_distributionalgo (id)
				ON DELETE CASCADE ;

ALTER TABLE t_temporarydistribution 
	ADD CONSTRAINT fk_t_temporarydistribution7
		FOREIGN KEY (cartid)
			REFERENCES t_cartsref (id) 
				ON DELETE CASCADE ;

ALTER TABLE t_temporarydistribution 
	ADD CONSTRAINT fk_t_temporarydistribution8
		FOREIGN KEY (disseminationpackingalgo_id)
			REFERENCES t_disseminationpackingalgos (id_disseminationpackingalgo) 
				ON DELETE CASCADE ;



/* ------------------------------
	t_distpoldataset
*/ ------------------------------
ALTER TABLE t_distpoldataset
	ADD CONSTRAINT fk_t_distpoldataset1
		FOREIGN KEY (policy)
			REFERENCES t_distributionrules (id_distributionrule)
				ON DELETE CASCADE ;

ALTER TABLE t_distpoldataset
	ADD CONSTRAINT fk_t_distpoldataset2
		FOREIGN KEY (dataset)
			REFERENCES t_measdataset (id)
				ON DELETE CASCADE ;



/* ------------------------------
	t_distruleoriginator
*/ ------------------------------
ALTER TABLE t_distruleoriginator
	ADD CONSTRAINT fk_t_distruleoriginator1
		FOREIGN KEY (distributionrule_id)
			REFERENCES t_distributionrules (id_distributionrule)
				ON DELETE CASCADE ;

ALTER TABLE t_distruleoriginator
	ADD CONSTRAINT fk_t_distruleoriginator2
		FOREIGN KEY (originator_id)
			REFERENCES t_originators (id)
				ON DELETE CASCADE ;



/* ------------------------------
	t_completed_orders
*/ ------------------------------
ALTER TABLE t_completed_orders 
	ADD CONSTRAINT fk_t_completed_orders_id
		FOREIGN KEY (order_id)
			REFERENCES t_ordersqueue (orderid)
				ON DELETE CASCADE ;


/* ------------------------------
	t_ordtriggeringfiles
*/ ------------------------------
ALTER TABLE t_ordtriggeringfiles 
	ADD CONSTRAINT fk_ordtrigger_inventory 
		FOREIGN KEY (filename, fileversion)
			REFERENCES t_inventory (filename, fileversion) 
				ON DELETE CASCADE ;

ALTER TABLE t_ordtriggeringfiles 
	ADD CONSTRAINT fk_ordtrigger_ordqueue 
		FOREIGN KEY (orderid)
			REFERENCES t_ordersqueue (orderid) 
				ON DELETE CASCADE ;



ALTER TABLE t_sites 
	ADD CONSTRAINT fk_sites_parent
		FOREIGN KEY (parent)
			REFERENCES t_sites (id) 
				ON DELETE CASCADE  ;

ALTER TABLE t_sites 
	ADD CONSTRAINT fk_sites_sitetype
		FOREIGN KEY (type)
			REFERENCES t_sitetype (id) 
				ON DELETE CASCADE ;


ALTER TABLE t_invsiterelation 
	ADD CONSTRAINT fk_invsiterel_inventory
		FOREIGN KEY (filename, fileversion)
			REFERENCES t_inventory (filename, fileversion) 
				ON DELETE CASCADE ;


ALTER TABLE t_invsiterelation 
	ADD CONSTRAINT fk_invsiterel_sites
		FOREIGN KEY (site)
			REFERENCES t_sites (id) 
				ON DELETE CASCADE ;



/* ------------------------------
	t_ordersattachments
*/ ------------------------------
ALTER TABLE t_ordersattachments 
	ADD CONSTRAINT fk_ordattach_attachtype
		FOREIGN KEY (attachtype)
		REFERENCES t_ordersattachmenttypes (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_ordersattachments 
	ADD CONSTRAINT fk_ordattach_ordqueue
		FOREIGN KEY (orderid)
			REFERENCES t_ordersqueue (orderid) 
				ON DELETE CASCADE ;



/* ------------------------------
	t_smactions
*/ ------------------------------
ALTER TABLE t_smactions 
	ADD CONSTRAINT fk_smactions_storagemanager
		FOREIGN KEY (smid)
			REFERENCES t_storagemanager (id) 
				ON DELETE CASCADE  ;

ALTER TABLE t_smactions 
	ADD CONSTRAINT fk_smactions_smqueue
		FOREIGN KEY (packageid)
			REFERENCES t_smqueue (packageid) 
				ON DELETE CASCADE ;



/* ------------------------------
	t_smqueue
*/ ------------------------------
ALTER TABLE t_smqueue 
	ADD CONSTRAINT fk_smqueue_storagemanager 
		FOREIGN KEY (smid)
			REFERENCES t_storagemanager (id) 
				ON DELETE CASCADE ;



/* ------------------------------
	t_stafinvrelation
*/ ------------------------------
ALTER TABLE t_stafinvrelation 
	ADD CONSTRAINT fk_stafinvrel_invsm 
		FOREIGN KEY (id)
			REFERENCES t_inventorysm (id) 
				ON DELETE CASCADE ;



/* ------------------------------
	t_inventoryeventsconf
*/ ------------------------------
ALTER TABLE t_inventoryeventsconf 
	ADD CONSTRAINT fk_t_inventsconf_ftype
		FOREIGN KEY (filetype)
			REFERENCES t_filetypes (filetype) 
				ON DELETE CASCADE ;



/* ------------------------------
	t_cartsinv
*/ ------------------------------
ALTER TABLE t_cartsinv 
	ADD CONSTRAINT fk_cartsinv_fnamefver
		FOREIGN KEY (filename, fileversion)
			REFERENCES t_inventory (filename, fileversion) 
				ON DELETE CASCADE ;

ALTER TABLE t_cartsinv 
	ADD CONSTRAINT fk_cartsinv_cart 
		FOREIGN KEY (cart)
			REFERENCES t_cartsref (id) 
				ON DELETE CASCADE ;



/* ------------------------------
	t_standingdistribution
*/ ------------------------------
ALTER TABLE t_standingdistribution 
	ADD CONSTRAINT fk_stendingdistribution01 
		FOREIGN KEY (distpoliciesid)
			REFERENCES t_distributionrules (id_distributionrule) 
				ON DELETE CASCADE ;

ALTER TABLE t_standingdistribution 
	ADD CONSTRAINT fk_stendingdistribution02 
		FOREIGN KEY (cartsid)
			REFERENCES t_cartsref (id) 
				ON DELETE CASCADE ;



/* ------------------------------
	t_storagegroup
*/ ------------------------------
ALTER TABLE t_storagegroup 
	ADD CONSTRAINT fk_storagegroup_ftype 
		FOREIGN KEY (filetype)
			REFERENCES t_filetypes (filetype) 
				ON DELETE CASCADE ;

ALTER TABLE t_storagegroup 
	ADD CONSTRAINT fk_storagegroup_storage
		FOREIGN KEY (storagemanager) 
			REFERENCES t_storagemanager (id) 
				ON DELETE CASCADE ;



/* ------------------------------
	t_filetypes_x_migrations
*/ ------------------------------
ALTER TABLE t_filetypes_x_migrations 
	ADD CONSTRAINT fk_filetypesxmigrations1
		FOREIGN KEY (sm_id)
			REFERENCES t_storagemanager (id) 
				ON DELETE CASCADE ;

ALTER TABLE t_filetypes_x_migrations 
	ADD CONSTRAINT fk_filetypesxmigrations2
		FOREIGN KEY (filetype_id)
			REFERENCES t_filetypes (id_filetype) 
				ON DELETE CASCADE ;

ALTER TABLE t_filetypes_x_migrations 
	ADD CONSTRAINT fk_filetypesxmigrations3
		FOREIGN KEY (migrationalgo_id)
			REFERENCES t_migrationalgos (id_migrationalgo) 
				ON DELETE CASCADE ;



/* ------------------------------
	t_filetypes_x_satellites
*/ ------------------------------
ALTER TABLE t_filetypes_x_satellites 
	ADD CONSTRAINT fk_t_filetypesxsatellites01 
		FOREIGN KEY (filetype)
			REFERENCES t_filetypes (filetype) 
				ON DELETE CASCADE ;

ALTER TABLE t_filetypes_x_satellites 
	ADD CONSTRAINT fk_t_filetypesxsatellites02 
		FOREIGN KEY (satellite_id)
			REFERENCES t_satellites (satelliteid) 
				ON DELETE CASCADE ;



/* ------------------------------
	t_inv_relations
*/ ------------------------------
ALTER TABLE t_inv_relations 
	ADD CONSTRAINT fk_inv_relations01 
		FOREIGN KEY (inv_id1) 
			REFERENCES t_inventory (id_inv) 
				ON DELETE CASCADE ;

ALTER TABLE t_inv_relations
	ADD CONSTRAINT fk_inv_relations02 
		FOREIGN KEY (inv_id2)
			REFERENCES t_inventory (id_inv) MATCH SIMPLE
				ON UPDATE NO ACTION ON DELETE SET NULL ;

ALTER TABLE t_inv_relations 
	ADD CONSTRAINT fk_inv_relations03 
		FOREIGN KEY (rel_type_id) 
			REFERENCES t_inv_relation_types (id) 
				ON DELETE CASCADE ;



/* ------------------------------
	t_filetypes_last_inv
*/ ------------------------------
ALTER TABLE t_filetypes_last_inv 
	ADD CONSTRAINT fk_filetypes_last_inv01 
		FOREIGN KEY (filetype) 
			REFERENCES t_filetypes (filetype) 
				ON DELETE CASCADE ;



/* ------------------------------
	t_inventoryanomalies
*/ ------------------------------
ALTER TABLE t_inventoryanomalies 
	ADD CONSTRAINT fk_inventoryanomalies01 
		FOREIGN KEY (anomaly_type_id) 
			REFERENCES t_anomaly_types (id) 
				ON DELETE CASCADE ;



/* ------------------------------
	t_repositories
*/ ------------------------------
ALTER TABLE t_repositories
	ADD CONSTRAINT fk_t_repositories1
		FOREIGN KEY (transferprotocol_id)
			REFERENCES t_transferprotocols (id)
				ON DELETE CASCADE ;

ALTER TABLE t_repositories
	ADD CONSTRAINT fk_t_repositories2
		FOREIGN KEY (host_id)
			REFERENCES t_knownhosts (id)
				ON DELETE CASCADE ;

ALTER TABLE t_repositories
	ADD CONSTRAINT fk_t_repositories3
		FOREIGN KEY (tempname_id)
			REFERENCES t_tmpnamestrategy (id_tmpnamestrategy)
				ON DELETE CASCADE ;


/* --------------------------
	t_users
*/ --------------------------
ALTER TABLE t_users
	ADD CONSTRAINT fk_t_users
		FOREIGN KEY (usertype)
			REFERENCES t_usertypes (usertype)
				ON DELETE CASCADE ;

ALTER TABLE t_users 
	ADD CONSTRAINT fk_users_modes
		FOREIGN KEY (mode)
			REFERENCES t_usermodes (id) 
				ON DELETE CASCADE ;



/* --------------------------
	t_waitingorders
*/ --------------------------
ALTER TABLE t_waitingorders
	ADD CONSTRAINT fk_t_waitingorders1
		FOREIGN KEY (orderid)
			REFERENCES t_ordersqueue (orderid)
				ON DELETE CASCADE  ;

ALTER TABLE t_waitingorders
 	 ADD CONSTRAINT fk_t_waitingorders2
		FOREIGN KEY (filetype)
			REFERENCES t_filetypes (filetype)
				ON DELETE CASCADE ;

ALTER TABLE t_waitingorders
	ADD CONSTRAINT fk_t_waitingorders3
		FOREIGN KEY (triggertype) 
			REFERENCES t_orderwaittrigger (triggertype)
				ON DELETE CASCADE ;


