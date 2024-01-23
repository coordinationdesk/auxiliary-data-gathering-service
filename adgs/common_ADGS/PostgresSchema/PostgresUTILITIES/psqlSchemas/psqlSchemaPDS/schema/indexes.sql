/*

Copyright 2010-2020, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S. Databases schema indexes script $

	$Id$

	$Author$

*/





--------------------------------------------------------------------------------
--
--                                  INDEXES
--
--------------------------------------------------------------------------------


/* ------------------------------
	t_acsisminvrelation
*/ ------------------------------
CREATE INDEX ix_t_acsisminvrelation01 ON t_acsisminvrelation USING BTREE (path);
CREATE INDEX ix_t_acsisminvrelation02 ON t_acsisminvrelation USING BTREE (medialabel);


/* ------------------------------
	t_applicationevents
*/ ------------------------------
CREATE INDEX ix_t_applicationevents01 ON t_applicationevents USING BTREE (application_id) ;
CREATE INDEX ix_t_applicationevents02 ON t_applicationevents USING BTREE (tstamp) ;


/* ------------------------------
	t_asyncsm_operations
*/ ------------------------------
CREATE INDEX ix_t_asyncsm_operations01 ON t_asyncsm_operations USING BTREE (operation_id) ;
CREATE INDEX ix_t_asyncsm_operations02 ON t_asyncsm_operations USING BTREE (sm_id) ;
CREATE INDEX ix_t_asyncsm_operations03 ON t_asyncsm_operations USING BTREE (status_id) ;


/* ------------------------------
	t_cartsinv
*/ ------------------------------
CREATE INDEX ix_t_cartsinv01 ON t_cartsinv USING BTREE (filename, fileversion) ;
CREATE INDEX ix_t_cartsinv02 ON t_cartsinv USING BTREE (cart) ;


/* ------------------------------
	t_cdp_feederqueue
*/ ------------------------------
CREATE INDEX ix_t_cdp_feederqueue01 ON t_cdp_feederqueue USING BTREE (creationdate);
CREATE INDEX ix_t_cdp_feederqueue02 ON t_cdp_feederqueue USING BTREE (retry_count);
CREATE INDEX ix_t_cdp_feederqueue03 ON t_cdp_feederqueue USING BTREE (retry_time);
CREATE INDEX ix_t_cdp_feederqueue04 ON t_cdp_feederqueue USING BTREE (status_id);
CREATE INDEX ix_t_cdp_feederqueue05 ON t_cdp_feederqueue USING BTREE (jobresponsible);
CREATE INDEX ix_t_cdp_feederqueue06 ON t_cdp_feederqueue USING BTREE (tempnamestrategy_id);


/* ------------------------------
	t_cdp_frame_grid
*/ ------------------------------
CREATE INDEX ix_t_cdp_frame_grid01 ON t_cdp_frame_grid USING BTREE (satelliteid) ;
CREATE INDEX ix_t_cdp_frame_grid02 ON t_cdp_frame_grid USING BTREE (filetype) ;
CREATE INDEX ix_t_cdp_frame_grid03 ON t_cdp_frame_grid USING BTREE (ordertype) ;
CREATE INDEX ix_t_cdp_frame_grid04 ON t_cdp_frame_grid USING BTREE (orbitNumber) ;
CREATE INDEX ix_t_cdp_frame_grid05 ON t_cdp_frame_grid USING BTREE (frameStart) ;
CREATE INDEX ix_t_cdp_frame_grid06 ON t_cdp_frame_grid USING BTREE (creationTime) ;


/* ------------------------------
	t_cdp_notification_queue
*/ ------------------------------
CREATE INDEX ix_t_cdp_notification_queue01 ON t_cdp_notification_queue USING BTREE (feederqueue_id);
CREATE INDEX ix_t_cdp_notification_queue02 ON t_cdp_notification_queue USING BTREE (creationdate);
CREATE INDEX ix_t_cdp_notification_queue03 ON t_cdp_notification_queue USING BTREE (retry_count);
CREATE INDEX ix_t_cdp_notification_queue04 ON t_cdp_notification_queue USING BTREE (retry_time);
CREATE INDEX ix_t_cdp_notification_queue05 ON t_cdp_notification_queue USING BTREE (status_id);
CREATE INDEX ix_t_cdp_notification_queue06 ON t_cdp_notification_queue USING BTREE (jobresponsible);


/* ------------------------------
	t_classifiedfiletypes
*/ ------------------------------
CREATE INDEX ix_t_classifiedfiletypes ON t_classifiedfiletypes USING BTREE (filetype) ;


/* ------------------------------
	t_cleanuprules 
*/ ------------------------------
CREATE INDEX ix_t_cleanuprules01 ON t_cleanuprules USING BTREE (repository_id) ;
CREATE INDEX ix_t_cleanuprules02 ON t_cleanuprules USING BTREE (cleanupalgo_id) ;


/* ------------------------------
	t_dbparameters
*/ ------------------------------
CREATE INDEX ix_t_dbparameters01 ON t_dbparameters USING BTREE (parametertype_id) ;


/* ------------------------------
	t_distpoldataset
*/ ------------------------------
CREATE INDEX ix_t_distpoldataset01 ON t_distpoldataset USING BTREE (policy) ;
CREATE INDEX ix_t_distpoldataset02 ON t_distpoldataset USING BTREE (dataset) ;


/* --------------------------
	t_distributionitems
*/ --------------------------
CREATE INDEX ix_t_distributionitems01 ON t_distributionitems USING BTREE (packageid) ;
CREATE INDEX ix_t_distributionitems02 ON t_distributionitems USING BTREE (filename, fileversion) ;
CREATE INDEX ix_t_distributionitems03 ON t_distributionitems USING BTREE (mediatype) ;
CREATE INDEX ix_t_distributionitems04 ON t_distributionitems USING BTREE (userid) ;
CREATE INDEX ix_t_distributionitems05 ON t_distributionitems USING BTREE (format) ;
CREATE INDEX ix_t_distributionitems06 ON t_distributionitems USING BTREE (cartid) ;
CREATE INDEX ix_t_distributionitems07 ON t_distributionitems USING BTREE (distributionrule_id) ;
CREATE INDEX ix_t_distributionitems08 ON t_distributionitems USING BTREE (disseminationpackingalgo_id) ;
CREATE INDEX ix_t_distributionitems09 ON t_distributionitems USING BTREE (distributeddate) ;
CREATE INDEX ix_t_distributionitems10 ON t_distributionitems USING BTREE (creationdate) ;


/* --------------------------
	t_distributionqueue
*/ --------------------------
CREATE INDEX ix_t_distributionqueue01 ON t_distributionqueue USING BTREE (status) ;
CREATE INDEX ix_t_distributionqueue02 ON t_distributionqueue USING BTREE (format) ;
CREATE INDEX ix_t_distributionqueue03 ON t_distributionqueue USING BTREE (creationdate) ;
CREATE INDEX ix_t_distributionqueue04 ON t_distributionqueue USING BTREE (distributeddate) ;


/* ------------------------------
	t_distributionrules
*/ ------------------------------
CREATE INDEX ix_t_distributionrules01 ON t_distributionrules USING BTREE (filetype_id) ;
CREATE INDEX ix_t_distributionrules02 ON t_distributionrules USING BTREE (mediatype_id) ;
CREATE INDEX ix_t_distributionrules03 ON t_distributionrules USING BTREE (repository_id) ;
CREATE INDEX ix_t_distributionrules04 ON t_distributionrules USING BTREE (user_id) ;
CREATE INDEX ix_t_distributionrules05 ON t_distributionrules USING BTREE (usermode_id) ;
CREATE INDEX ix_t_distributionrules06 ON t_distributionrules USING BTREE (distributionalgo_id) ;
CREATE INDEX ix_t_distributionrules07 ON t_distributionrules USING BTREE (format_tt_id) ;
CREATE INDEX ix_t_distributionrules08 ON t_distributionrules USING BTREE (delivery_tt_id) ;
CREATE INDEX ix_t_distributionrules09 ON t_distributionrules USING BTREE (delcheck_tt_id) ;
CREATE INDEX ix_t_distributionrules10 ON t_distributionrules USING BTREE (fileclass_id) ;
CREATE INDEX ix_t_distributionrules11 ON t_distributionrules USING BTREE (disseminationpackingalgo_id) ;


/* ------------------------------
	t_distruleoriginator
*/ ------------------------------
CREATE INDEX ix_t_distruleoriginator01 ON t_distruleoriginator USING BTREE (distributionrule_id) ;
CREATE INDEX ix_t_distruleoriginator02 ON t_distruleoriginator USING BTREE (originator_id) ;


/* ------------------------------
	t_distributionwaitingitems
*/ ------------------------------
CREATE INDEX ix_t_distributionwaitingitems1 ON t_distributionwaitingitems USING BTREE (distributionitem_id) ;
CREATE INDEX ix_t_distributionwaitingitems2 ON t_distributionwaitingitems USING BTREE (package_id) ;
CREATE INDEX ix_t_distributionwaitingitems3 ON t_distributionwaitingitems USING BTREE (inventory_id) ;


/* ------------------------------
	t_emitted_order_events
*/ ------------------------------
CREATE INDEX ix_t_emitted_order_events01 ON t_emitted_order_events USING BTREE (event_time) ;
CREATE INDEX ix_t_emitted_order_events02 ON t_emitted_order_events USING BTREE (processor_id, order_type) ;


/* ------------------------------
	t_filetypescomps
*/ ------------------------------
CREATE INDEX ix_t_filetypescomps01 ON t_filetypescomps USING BTREE (pfiletype) ;
CREATE INDEX ix_t_filetypescomps02 ON t_filetypescomps USING BTREE (cfiletype) ;


/* ------------------------------
	t_filetypes_last_inv
*/ ------------------------------
CREATE INDEX ix_t_filetypes_last_inv01 ON t_filetypes_last_inv USING BTREE (filetype) ;


/* ------------------------------
	t_filetypes_x_satellites
*/ ------------------------------
CREATE INDEX ix_t_filetypes_x_satellites01 ON t_filetypes_x_satellites USING BTREE (filetype) ;
CREATE INDEX ix_t_filetypes_x_satellites02 ON t_filetypes_x_satellites USING BTREE (satellite_id) ;


/* ------------------------------
	t_filetypes_x_migrations
*/ ------------------------------
CREATE INDEX ix_t_filetypesxmigrations01 ON t_filetypes_x_migrations USING BTREE (sm_id) ;
CREATE INDEX ix_t_filetypesxmigrations02 ON t_filetypes_x_migrations USING BTREE (filetype_id) ;
CREATE INDEX ix_t_filetypesxmigrations03 ON t_filetypes_x_migrations USING BTREE (migrationalgo_id) ;


/* ------------------------------
	t_inv_additional_params
*/ ------------------------------
CREATE INDEX ix_t_inv_additional_params01 ON t_inv_additional_params USING BTREE (inv_id) ;
CREATE INDEX ix_t_inv_additional_params02 ON t_inv_additional_params USING BTREE (timeliness_key_id) ;
CREATE INDEX ix_t_inv_additional_params03 ON t_inv_additional_params USING BTREE (data_take_id) ;


/* ------------------------------
	t_inv_data_restored
*/ ------------------------------
CREATE INDEX ix_t_inv_data_restored01 ON t_inv_data_restored USING BTREE (sm_id) ;
CREATE INDEX ix_t_inv_data_restored02 ON t_inv_data_restored USING BTREE (inv_id) ;
CREATE INDEX ix_t_inv_data_restored03 ON t_inv_data_restored USING BTREE (parent_inv_id) ;
CREATE INDEX ix_t_inv_data_restored04 ON t_inv_data_restored USING BTREE (filetype_id) ;
CREATE INDEX ix_t_inv_data_restored05 ON t_inv_data_restored USING BTREE (tstamp) ;


/* --------------------------
	t_inv_downloads
*/ --------------------------
CREATE INDEX ix_t_inv_downloads01 ON t_inv_downloads USING BTREE (inv_id) ;
CREATE INDEX ix_t_inv_downloads02 ON t_inv_downloads USING BTREE (client_id) ;


/* --------------------------
	t_inventory
*/ --------------------------
CREATE INDEX ix_t_inventory01 ON t_inventory USING BTREE (dir_id) ;
CREATE INDEX ix_t_inventory02 ON t_inventory USING BTREE (ref_dir_id) ;
CREATE INDEX ix_t_inventory03 ON t_inventory USING BTREE (orderid) ;
CREATE INDEX ix_t_inventory04 ON t_inventory USING BTREE (logical_id) ;
CREATE INDEX ix_t_inventory05 ON t_inventory USING BTREE (fileclass) ;
CREATE INDEX ix_t_inventory06 ON t_inventory USING BTREE (sensorid) ;
CREATE INDEX ix_t_inventory07 ON t_inventory USING BTREE (satelliteid) ;
CREATE INDEX ix_t_inventory08 ON t_inventory USING BTREE (acquisitionstationid) ;
CREATE INDEX ix_t_inventory09 ON t_inventory USING BTREE (filetype) ;
CREATE INDEX ix_t_inventory10 ON t_inventory USING BTREE (processingstationid) ;
CREATE INDEX ix_t_inventory11 ON t_inventory USING BTREE (qualified) ;
CREATE INDEX ix_t_inventory12 ON t_inventory USING BTREE (band) ;
CREATE INDEX ix_t_inventory13 ON t_inventory USING BTREE (filename) ;
CREATE INDEX ix_t_inventory14 ON t_inventory USING BTREE (validitystart) ;
CREATE INDEX ix_t_inventory15 ON t_inventory USING BTREE (validitystop) ;
CREATE INDEX ix_t_inventory16 ON t_inventory USING BTREE (inventorydate) ;
CREATE INDEX ix_t_inventory17 ON t_inventory USING BTREE (modificationdate) ;
CREATE INDEX ix_t_inventory18 ON t_inventory USING BTREE (generationtime) ;
CREATE INDEX ix_t_inventory19 ON t_inventory USING BTREE (startorbitnumber) ;
CREATE INDEX ix_t_inventory20 ON t_inventory USING BTREE (originator) ;
CREATE INDEX ix_t_inventory_jsonb ON t_inventory USING GIN (attributes);


/* --------------------------
	t_inventoryanomalies
*/ --------------------------
CREATE INDEX ix_t_invanomalies01 ON t_inventoryanomalies USING BTREE (anomaly_type_id) ;


/* ------------------------------
	t_inventorycomments
*/ ------------------------------
CREATE INDEX ix_t_inventorycomments01 ON t_inventorycomments USING BTREE (filename, fileversion) ;


/* ------------------------------
	t_inventoryeventsconf
*/ ------------------------------
CREATE INDEX ix_t_inventoryeventsconf01 ON t_inventoryeventsconf USING BTREE (filetype) ;


/* ------------------------------
	t_inventorydataset
*/ ------------------------------
CREATE INDEX ix_t_inventorydataset01 ON t_inventorydataset USING BTREE (filename, fileversion) ;
CREATE INDEX ix_t_inventorydataset02 ON t_inventorydataset USING BTREE (measdatasetid) ;


/* --------------------------
	t_inventorygaps
*/ --------------------------
CREATE INDEX ix_t_inventorygaps01 ON t_inventorygaps USING BTREE (filename, fileversion) ;
CREATE INDEX ix_t_inventorygaps02 ON t_inventorygaps USING BTREE (begin) ;
CREATE INDEX ix_t_inventorygaps03 ON t_inventorygaps USING BTREE ("end") ;


/* --------------------------
	t_inventorysm
*/ --------------------------
CREATE INDEX ix_t_inventorysm01 ON t_inventorysm USING BTREE (inv_id);
CREATE INDEX ix_t_inventorysm02 ON t_inventorysm USING BTREE (storagemanid);
CREATE INDEX ix_t_inventorysm03 ON t_inventorysm USING BTREE (packageid);


/* --------------------------
	t_inv_relations
*/ --------------------------
CREATE INDEX ix_t_inv_relations01 ON t_inv_relations USING BTREE (rel_type_id) ;
CREATE INDEX ix_t_inv_relations02 ON t_inv_relations USING BTREE (inv_id1) ;
CREATE INDEX ix_t_inv_relations03 ON t_inv_relations USING BTREE (inv_id2) ;
CREATE INDEX ix_t_inv_relations04 ON t_inv_relations USING BTREE (relation) ;


/* ------------------------------
	t_invrollingpolicies
*/ ------------------------------
CREATE INDEX ix_t_invrollingpolicies01 ON t_invrollingpolicies USING BTREE (filetype) ;
CREATE INDEX ix_t_invrollingpolicies02 ON t_invrollingpolicies USING BTREE (algorithm) ;
CREATE INDEX ix_t_invrollingpolicies03 ON t_invrollingpolicies USING BTREE (action) ;
CREATE INDEX ix_t_invrollingpolicies04 ON t_invrollingpolicies USING BTREE (fileclass) ;
CREATE INDEX ix_t_invrollingpolicies05 ON t_invrollingpolicies USING BTREE (storageid) ;


/* ------------------------------
	t_invspecialrollingpolicies
*/ ------------------------------
CREATE INDEX ix_t_invspecialrollingpol01 ON t_invspecialrollingpolicies USING BTREE (inventoryname, inventoryversion) ;
CREATE INDEX ix_t_invspecialrollingpol02 ON t_invspecialrollingpolicies USING BTREE (action) ;
CREATE INDEX ix_t_invspecialrollingpol03 ON t_invspecialrollingpolicies USING BTREE (storageid) ;


/* ------------------------------
	t_invsiterelation
*/ ------------------------------
CREATE INDEX ix_t_invsiterelation01 ON t_invsiterelation USING BTREE (filename, fileversion) ;
CREATE INDEX ix_t_invsiterelation02 ON t_invsiterelation USING BTREE (site) ;


/* ------------------------------
	t_mediacapformat
*/ ------------------------------
CREATE INDEX ix_t_mediacapformat01 ON t_mediacapformat USING BTREE (mediatype) ;


/* ------------------------------
	t_ondemanddistribution
*/ ------------------------------
CREATE INDEX ix_t_ondemanddistribution01 ON t_ondemanddistribution USING BTREE (orderid) ;
CREATE INDEX ix_t_ondemanddistribution02 ON t_ondemanddistribution USING BTREE (userid) ;
CREATE INDEX ix_t_ondemanddistribution03 ON t_ondemanddistribution USING BTREE (format) ;
CREATE INDEX ix_t_ondemanddistribution04 ON t_ondemanddistribution USING BTREE (mediatype) ;
CREATE INDEX ix_t_ondemanddistribution05 ON t_ondemanddistribution USING BTREE (disseminationpackingalgo_id) ;


/* ------------------------------
	t_ordersattachments
*/ ------------------------------
CREATE INDEX ix_t_ordersattachments01 ON t_ordersattachments USING BTREE (orderid) ;
CREATE INDEX ix_t_ordersattachments02 ON t_ordersattachments USING BTREE (attachtype) ;


/* --------------------------
	t_ordersgeneration
*/ --------------------------
CREATE INDEX ix_t_ordersgeneration01 ON t_ordersgeneration USING BTREE (processorname, processorversion) ;
CREATE INDEX ix_t_ordersgeneration02 ON t_ordersgeneration USING BTREE (ordertype) ;
CREATE INDEX ix_t_ordersgeneration03 ON t_ordersgeneration USING BTREE (algorithm) ;
CREATE INDEX ix_t_ordersgeneration04 ON t_ordersgeneration USING BTREE (filetype) ;
CREATE INDEX ix_t_ordersgeneration05 ON t_ordersgeneration USING BTREE (fileclass) ;


/* --------------------------
	t_ordersinput
*/ --------------------------
CREATE INDEX ix_t_ordersinput01 ON t_ordersinput USING BTREE (orderid) ;
CREATE INDEX ix_t_ordersinput02 ON t_ordersinput USING BTREE (filename, fileversion) ;


/* --------------------------
	t_ordersqueue
*/ --------------------------
CREATE INDEX ix_t_ordersqueue01 ON t_ordersqueue USING BTREE (status) ;
CREATE INDEX ix_t_ordersqueue02 ON t_ordersqueue USING BTREE (ordertype) ;
CREATE INDEX ix_t_ordersqueue03 ON t_ordersqueue USING BTREE (satelliteid) ;
CREATE INDEX ix_t_ordersqueue04 ON t_ordersqueue USING BTREE (mission) ;
CREATE INDEX ix_t_ordersqueue05 ON t_ordersqueue USING BTREE (processorname, processorversion) ;
CREATE INDEX ix_t_ordersqueue06 ON t_ordersqueue USING BTREE (creationdate) ;
CREATE INDEX ix_t_ordersqueue07 ON t_ordersqueue USING BTREE (starttime) ;
CREATE INDEX ix_t_ordersqueue08 ON t_ordersqueue USING BTREE (stoptime) ;
CREATE INDEX ix_t_ordersqueue09 ON t_ordersqueue USING BTREE (jobresponsible) ;
CREATE INDEX ix_t_ordersqueue10 ON t_ordersqueue USING BTREE (productionstoptime) ;


/* ------------------------------
	t_ordersqueueblacklist
*/ ------------------------------
CREATE INDEX ix_t_ordersqueueblacklist01 ON t_ordersqueueblacklist USING BTREE (orderid) ;
CREATE INDEX ix_t_ordersqueueblacklist02 ON t_ordersqueueblacklist USING BTREE (jobresponsible) ;


/* ------------------------------
	t_orderstatetransitions
*/ ------------------------------
CREATE INDEX ix_t_orderstatetransitions01 ON t_orderstatetransitions USING BTREE (orderid) ;


/* ------------------------------
	t_ordtriggeringfiles
*/ ------------------------------
CREATE INDEX ix_t_ordtriggeringfiles01 ON t_ordtriggeringfiles USING BTREE (filename, fileversion) ;
CREATE INDEX ix_t_ordtriggeringfiles02 ON t_ordtriggeringfiles USING BTREE (orderid) ;


/* ------------------------------
	t_proc_ftype_generation
*/ ------------------------------
CREATE INDEX ix_t_proc_ftype_generation01 ON t_proc_ftype_generation USING BTREE (proc_id) ;
CREATE INDEX ix_t_proc_ftype_generation02 ON t_proc_ftype_generation USING BTREE (ftype_id) ;


/* ------------------------------
	t_receptionrules
*/ ------------------------------
CREATE INDEX ix_t_receptionrules01 ON t_receptionrules USING BTREE (filetype_id) ;
CREATE INDEX ix_t_receptionrules02 ON t_receptionrules USING BTREE (repository_id) ;
CREATE INDEX ix_t_receptionrules03 ON t_receptionrules USING BTREE (user_id) ;
CREATE INDEX ix_t_receptionrules04 ON t_receptionrules USING BTREE (usermode_id) ;
CREATE INDEX ix_t_receptionrules05 ON t_receptionrules USING BTREE (reception_tt_id) ;
CREATE INDEX ix_t_receptionrules06 ON t_receptionrules USING BTREE (discoveryplugin_id) ;
CREATE INDEX ix_t_receptionrules07 ON t_receptionrules USING BTREE (satellite_id) ;
CREATE INDEX ix_t_receptionrules08 ON t_receptionrules USING BTREE (sensor_id) ;


/* ------------------------------
	t_receptionruleshist
*/ ------------------------------
CREATE INDEX ix_t_receptionruleshist01 ON t_receptionruleshist USING BTREE (receptionrule_id) ;
CREATE INDEX ix_t_receptionruleshist02 ON t_receptionruleshist USING BTREE (filename) ;
CREATE INDEX ix_t_receptionruleshist03 ON t_receptionruleshist USING BTREE (status_id) ;
CREATE INDEX ix_t_receptionruleshist04 ON t_receptionruleshist USING BTREE (filecreationtstamp) ;


/* ------------------------------
	t_receptionruleslock
*/ ------------------------------
CREATE INDEX ix_t_receptionruleslock01 ON t_receptionruleslock USING BTREE (receptionrule_id) ;


/* ------------------------------
	t_repositories
*/ ------------------------------
CREATE INDEX ix_t_repositories01 ON t_repositories USING BTREE (transferprotocol_id) ;
CREATE INDEX ix_t_repositories02 ON t_repositories USING BTREE (host_id) ;
CREATE INDEX ix_t_repositories03 ON t_repositories USING BTREE (tempname_id) ;


/* ------------------------------
	t_satrelations
*/ ------------------------------
CREATE INDEX ix_t_satrelations01 ON t_satrelations USING BTREE (satelliteid) ;
CREATE INDEX ix_t_satrelations02 ON t_satrelations USING BTREE (sensorid) ;


/* --------------------------
	t_sites
*/ --------------------------
CREATE INDEX ix_t_sites01 ON t_sites USING BTREE (parent) ;
CREATE INDEX ix_t_sites02 ON t_sites USING BTREE (type) ;


/* ------------------------------
	t_sm_sync_missingactions
*/ ------------------------------
CREATE INDEX ix_t_sm_sync_missingactions01 ON t_sm_sync_missingactions USING BTREE (sm_id) ;
CREATE INDEX ix_t_sm_sync_missingactions02 ON t_sm_sync_missingactions USING BTREE (inv_id) ;
CREATE INDEX ix_t_sm_sync_missingactions03 ON t_sm_sync_missingactions USING BTREE (invsm_id) ;


/* ------------------------------
	t_smactions
*/ ------------------------------
CREATE INDEX ix_t_smactions01 ON t_smactions USING BTREE (inv_id) ;
CREATE INDEX ix_t_smactions02 ON t_smactions USING BTREE (smid) ;
CREATE INDEX ix_t_smactions03 ON t_smactions USING BTREE (packageid) ;
CREATE INDEX ix_t_smactions04 ON t_smactions USING BTREE (filename) ;
CREATE INDEX ix_t_smactions05 ON t_smactions USING BTREE (fileversion) ;
CREATE INDEX ix_t_smactions06 ON t_smactions USING BTREE (operation) ;
CREATE INDEX ix_t_smactions07 ON t_smactions USING BTREE (enddate) ;


/* ------------------------------
	t_smqueue
*/ ------------------------------
CREATE INDEX ix_t_smqueue01 ON t_smqueue USING BTREE (smid) ;
CREATE INDEX ix_t_smqueue02 ON t_smqueue USING BTREE (status) ;


/* ------------------------------
	t_standingdistribution
*/ ------------------------------
CREATE INDEX ix_t_standingdistribution01 ON t_standingdistribution USING BTREE (distpoliciesid) ;
CREATE INDEX ix_t_standingdistribution02 ON t_standingdistribution USING BTREE (cartsid) ;


/* ------------------------------
	t_statetransitions_allowed
*/ ------------------------------
CREATE INDEX ix_t_statetrans_allowed01 ON t_statetransitions_allowed USING BTREE (table_id) ;
CREATE INDEX ix_t_statetrans_allowed02 ON t_statetransitions_allowed USING BTREE (prev_state_id) ;
CREATE INDEX ix_t_statetrans_allowed03 ON t_statetransitions_allowed USING BTREE (post_state_id) ;


/* ------------------------------
	t_statetransitions_tables
*/ ------------------------------
CREATE INDEX ix_t_statetransitions_tables01 ON t_statetransitions_tables USING BTREE (flow_id) ;


/* ------------------------------
	t_storagegroup
*/ ------------------------------
CREATE INDEX ix_t_storagegroup01 ON t_storagegroup USING BTREE (filetype) ;
CREATE INDEX ix_t_storagegroup02 ON t_storagegroup USING BTREE (storagemanager) ;


/* ------------------------------
	t_storagemanager
*/ ------------------------------
CREATE INDEX ix_t_storagemanager01 ON t_storagemanager USING BTREE (type) ;
CREATE INDEX ix_t_storagemanager02 ON t_storagemanager USING BTREE (migrationalgo_id) ;


/* ------------------------------
	t_storagerollingthresholds 
*/ ------------------------------
CREATE INDEX ix_t_storagerollingthreshold01 ON t_storagerollingthresholds USING BTREE (action_id) ;


/* ------------------------------
	t_tasktables
*/ ------------------------------
CREATE INDEX ix_t_tasktables01 ON t_tasktables USING BTREE (type_id) ;


/* ------------------------------
	t_temporarydistribution
*/ ------------------------------
CREATE INDEX ix_t_temporarydistribution01 ON t_temporarydistribution USING BTREE (filename, fileversion) ;
CREATE INDEX ix_t_temporarydistribution02 ON t_temporarydistribution USING BTREE (mediatype) ;
CREATE INDEX ix_t_temporarydistribution03 ON t_temporarydistribution USING BTREE (userid) ;
CREATE INDEX ix_t_temporarydistribution04 ON t_temporarydistribution USING BTREE (format) ;
CREATE INDEX ix_t_temporarydistribution05 ON t_temporarydistribution USING BTREE (algo) ;
CREATE INDEX ix_t_temporarydistribution06 ON t_temporarydistribution USING BTREE (policy) ;
CREATE INDEX ix_t_temporarydistribution07 ON t_temporarydistribution USING BTREE (cartid) ;
CREATE INDEX ix_t_temporarydistribution08 ON t_temporarydistribution USING BTREE (disseminationpackingalgo_id) ;


/* ------------------------------
	t_temporaryorders
*/ ------------------------------
CREATE INDEX ix_t_temporaryorders01 ON t_temporaryorders USING BTREE (filename, fileversion) ;
CREATE INDEX ix_t_temporaryorders02 ON t_temporaryorders USING BTREE (ordersat) ;


/* --------------------------
	t_users
*/ --------------------------
CREATE INDEX ix_t_users01 ON t_users USING BTREE (usertype) ;
CREATE INDEX ix_t_users02 ON t_users USING BTREE (mode) ;


/* --------------------------
	t_waitingorders
*/ --------------------------
CREATE INDEX ix_t_waitingorders01 ON t_waitingorders USING BTREE (orderid) ;
CREATE INDEX ix_t_waitingorders02 ON t_waitingorders USING BTREE (filetype) ;
CREATE INDEX ix_t_waitingorders03 ON t_waitingorders USING BTREE (triggertype) ;


