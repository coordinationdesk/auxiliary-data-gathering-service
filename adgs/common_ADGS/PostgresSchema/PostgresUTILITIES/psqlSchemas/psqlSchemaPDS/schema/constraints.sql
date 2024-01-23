/*

Copyright 2010-2020, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S.  Databases schema constraints script $

	$Id$

	$Author$

*/





--------------------------------------------------------------------------------
--
--    	  	  	  	  	  	  	 CONSTRAINTS
--
--------------------------------------------------------------------------------


-------------------------------------------------
--------------- CHECK CONSTRAINTS ---------------
-------------------------------------------------

/* ------------------------------
	t_cleanuprules 
*/ ------------------------------
ALTER TABLE t_cleanuprules
	ADD CONSTRAINT ck_cleanuprules01
		CHECK (hiquota_mb >= lowquota_mb) ;

ALTER TABLE t_cleanuprules
	ADD CONSTRAINT ck_cleanuprules02
		CHECK (lowquota_mb >= 0) ;

ALTER TABLE t_cleanuprules
	ADD CONSTRAINT ck_cleanuprules03
		CHECK (age_h > 0) ;


/* ------------------------------
	t_cdp_feederqueue
*/ ------------------------------
ALTER TABLE t_cdp_feederqueue 
	ADD CONSTRAINT ck_cdp_feederqueue01 
		CHECK ((cache_path IS NOT NULL AND s3_reference IS NULL AND inv_id IS NULL) 
			OR (cache_path IS NULL AND s3_reference IS NOT NULL AND inv_id IS NULL) 
			OR (cache_path IS NULL AND s3_reference IS NULL AND inv_id IS NOT NULL)) ;


/* ------------------------------
	t_distpoldataset
*/ ------------------------------
ALTER TABLE t_distpoldataset
  ADD CONSTRAINT ck_distpoldataset
  	CHECK (num > 0) ;


/* ------------------------------
	t_distributionrules
*/ ------------------------------
ALTER TABLE t_distributionrules
	ADD CONSTRAINT ck_distributionrules01
		CHECK (retryperiod_sec > 0) ;


/* ------------------------------
	t_filetypes
*/ ------------------------------
ALTER TABLE t_filetypes 
	ADD CONSTRAINT ck_filetypes01 
		CHECK ((sngext IS NULL AND hdrext IS NULL AND dblext IS NULL) 
			OR (sngext IS NULL AND hdrext IS NOT NULL AND dblext IS NOT NULL AND ( hdrext != dblext)) 
			OR (sngext IS NOT NULL AND ((hdrext IS NULL AND dblext IS NULL) OR ( hdrext != dblext)))) ;


/* ------------------------------
	t_filetypescomps
*/ ------------------------------
ALTER TABLE t_filetypescomps 
	ADD CONSTRAINT ck_filetypescomps01 
		CHECK (pfiletype != cfiletype ) ;


/* ------------------------------
	t_inventory
*/ ------------------------------
ALTER TABLE t_inventory
  ADD CONSTRAINT ck_inventory
  	CHECK (validitystart <= validitystop) ;

ALTER TABLE t_inventory 
	ADD CONSTRAINT ck_inventory02
		CHECK ((cloud_pctg >= 0 ) AND (cloud_pctg <= 100 )) ;


/* ------------------------------
	t_inventorydataset
*/ ------------------------------
ALTER TABLE t_inventorydataset
	ADD CONSTRAINT ck_invdataset_count
		CHECK (count > 0) ;


/* ------------------------------
	t_ordersqueue
*/ ------------------------------
ALTER TABLE t_ordersqueue
	ADD CONSTRAINT ck_ordersqueue
		CHECK (starttime <= stoptime) ;


/* ------------------------------
	t_receptionrules
*/ ------------------------------
ALTER TABLE t_receptionrules
	ADD CONSTRAINT ck_receptionrules01
		CHECK (pollingperiod_sec > 0) ;

ALTER TABLE t_receptionrules
	ADD CONSTRAINT ck_receptionrules02
		CHECK ((retryperiod_sec > 0) AND (retryperiod_sec < floor(pollingperiod_sec/connretries))) ;

ALTER TABLE t_receptionrules
	ADD CONSTRAINT ck_receptionrules03
		CHECK (php_timeout_sec > 90) ;

ALTER TABLE t_receptionrules
	ADD CONSTRAINT ck_receptionrules04
		CHECK ((satellite_id IS NULL AND (mission IS NOT NULL OR sensor_id IS NOT NULL)) != TRUE) ;

ALTER TABLE t_receptionrules
	ADD CONSTRAINT ck_receptionrules05
		CHECK ((mission IS NULL AND sensor_id IS NOT NULL) != TRUE) ;


/* ------------------------------
	t_repositories
*/ ------------------------------
ALTER TABLE t_repositories
	ADD CONSTRAINT ck_repositories01			
		CHECK (char_length(trim(tempname_value)) != 0) ;

ALTER TABLE t_repositories
	ADD CONSTRAINT ck_repositories02
		CHECK ((tempname_id != 2 AND (tempname_value = '..' OR tempname_value = '../')) != TRUE) ;

ALTER TABLE t_repositories
	ADD CONSTRAINT ck_repositories03
		CHECK ((tempname_id = 3 AND (tempname_value = '.' OR tempname_value = './')) != TRUE) ;


/* ------------------------------
	t_sm_sync_missingactions
*/ ------------------------------
ALTER TABLE t_sm_sync_missingactions
	ADD CONSTRAINT ck_sm_sync_missingactions01
		CHECK (operation IN ('UPLOAD', 'ERASE')) ;

ALTER TABLE t_sm_sync_missingactions 
	ADD CONSTRAINT ck_sm_sync_missingactions02 
		CHECK ((inv_id IS NULL AND invsm_id IS NOT NULL) OR (inv_id IS NOT NULL AND invsm_id IS NULL)) ;


/* ------------------------------
	t_standingdistribution
*/ ------------------------------
ALTER TABLE t_standingdistribution 
	ADD CONSTRAINT ck_stendingdistribution01 
		CHECK (validitystart <= validitystop ) ;


/* ------------------------------
	t_waitingorders
*/ ------------------------------
ALTER TABLE t_waitingorders
	ADD CONSTRAINT ck_waitingorders
		CHECK (stoptime >= starttime) ;

ALTER TABLE t_waitingorders
	ADD CONSTRAINT ck_waitingorders02
		CHECK ((triggertype = 'DELAY' AND expirationdate IS NULL) != TRUE) ;









-------------------------------------------------
--------------- UNIQUE CONSTRAINTS --------------
-------------------------------------------------

/* ------------------------------
	t_algorithm
*/ ------------------------------
ALTER TABLE t_algorithm 
	ADD CONSTRAINT uq_algorithm
		UNIQUE (algorithm) ;


/* ------------------------------
	t_anomaly_types
*/ ------------------------------
ALTER TABLE t_anomaly_types 
	ADD CONSTRAINT uq_anomaly_types01
		UNIQUE (anomaly_type) ;


/* ------------------------------
	t_applications
*/ ------------------------------
ALTER TABLE t_applications 
	ADD CONSTRAINT uq_applications01
		UNIQUE (app_name) ;


/* ------------------------------
	t_applicationstatus
*/ ------------------------------
ALTER TABLE t_applicationstatus 
	ADD CONSTRAINT uq_applicationstatus
		UNIQUE (application, instance) ;


/* ------------------------------
	t_applicationevents
*/ ------------------------------
ALTER TABLE t_applicationevents 
	ADD CONSTRAINT uq_applicationevents01
		UNIQUE (application_id, tstamp) ;


/* ------------------------------
	t_bands
*/ ------------------------------
ALTER TABLE t_bands 
	ADD CONSTRAINT uq_t_bands
		UNIQUE (name) ;


/* ------------------------------
	t_cdp_frame_grid
*/ ------------------------------
ALTER TABLE t_cdp_frame_grid 
	ADD CONSTRAINT uq_cdp_frame_grid01 
		UNIQUE (satelliteid, filetype, ordertype, orbitnumber, frameNumber) ;


/* ------------------------------
	t_cleanupalgos 
*/ ------------------------------
ALTER TABLE t_cleanupalgos 
	ADD CONSTRAINT uq_cleanupalgos01
		UNIQUE (name) ;


/* ------------------------------
	t_configurationschemas
*/ ------------------------------
ALTER TABLE t_configurationschemas 
	ADD CONSTRAINT uq_configurationschemas
		UNIQUE (namespace, version) ;


/* ------------------------------
	t_configurationspaces
*/ ------------------------------
ALTER TABLE t_configurationspaces 
	ADD CONSTRAINT uq_configurationspaces
		UNIQUE (namespace, version) ;


/* ------------------------------
	t_configurationstylesheets
*/ ------------------------------
ALTER TABLE t_configurationstylesheets 
	ADD CONSTRAINT uq_configurationstylesheets01
		UNIQUE (name, version) ;


/* ------------------------------
	t_dbstatus
*/ ------------------------------
ALTER TABLE t_dbstatus
  ADD CONSTRAINT uq_dbstatus1
  	UNIQUE (master) ;


/* ------------------------------
	t_discoveryplugins 
*/ ------------------------------
ALTER TABLE t_discoveryplugins 
	ADD CONSTRAINT uq_discoveryplugins01
		UNIQUE (name) ;


/* ------------------------------
	t_disseminationpackingalgos 
*/ ------------------------------
ALTER TABLE t_disseminationpackingalgos 
	ADD CONSTRAINT uq_disseminationpackingalgos01
		UNIQUE (factorykey) ;


/* ------------------------------
	t_distributionalgo
*/ ------------------------------
ALTER TABLE t_distributionalgo
  ADD CONSTRAINT uq_distalgo_name
  	UNIQUE (name) ;


/* ------------------------------
	t_distributionformat
*/ ------------------------------
ALTER TABLE t_distributionformat 
	ADD CONSTRAINT uq_distributionformat
		UNIQUE (format) ;


/* ------------------------------
	t_distributionrules
*/ ------------------------------
CREATE UNIQUE INDEX uq_distributionrules01 
	ON t_distributionrules (ondemand, filetype_id, mediatype_id, user_id, usermode_id, repository_id, format_tt_id) 
		WHERE active = 't' AND fileclass_id IS NULL;

CREATE UNIQUE INDEX uq_distributionrules02 
	ON t_distributionrules (ondemand, filetype_id, mediatype_id, user_id, usermode_id, repository_id, format_tt_id, fileclass_id) 
		WHERE active = 't' AND fileclass_id IS NOT NULL;


/* ------------------------------
	t_downlink_session_tracks
*/ ------------------------------
ALTER TABLE t_downlink_session_tracks 
	ADD CONSTRAINT uq_downlink_session_tracks01
		UNIQUE (session_id) ;


/* ------------------------------
	t_emitted_order_events
*/ ------------------------------
ALTER TABLE t_emitted_order_events 
	ADD CONSTRAINT uq_emitted_order_events01
		UNIQUE (event_time, event_tag, time_driven_start, time_driven_stop) ;


/* ------------------------------
	t_fileclasses
*/ ------------------------------
ALTER TABLE t_fileclasses 
	ADD CONSTRAINT uq_fileclasses01
		UNIQUE (fileclass) ;


/* ------------------------------
	t_filetypes
*/ ------------------------------
ALTER TABLE t_filetypes 
	ADD CONSTRAINT uq_filetypes01
		UNIQUE (filetype) ;


/* ------------------------------
	t_filetypescomps
*/ ------------------------------
ALTER TABLE t_filetypescomps 
	ADD CONSTRAINT uq_filetypescomps01 
		UNIQUE (pfiletype, cfiletype) ;


/* ------------------------------
	t_inv_client
*/ ------------------------------
ALTER TABLE t_inv_client 
	ADD CONSTRAINT uq_inv_client01
		UNIQUE (name) ;


/* ------------------------------
	t_inventory
*/ ------------------------------
ALTER TABLE t_inventory
	ADD CONSTRAINT uq_inventory01
		UNIQUE (filename, fileversion) ;

ALTER TABLE t_inventory
	ADD CONSTRAINT uq_inventory02
		UNIQUE (unique_id) ;


/* ------------------------------
	t_inventorysm
*/ ------------------------------
ALTER TABLE t_inventorysm
	ADD CONSTRAINT uq_inventorysm01
		UNIQUE (inv_id, storagemanid) ;


/* ------------------------------
	t_invspecialrollingpolicies
*/ ------------------------------
ALTER TABLE t_invspecialrollingpolicies
	ADD CONSTRAINT uq_invspecialrollingpolicies
		UNIQUE (inventoryname, inventoryversion, action, storageid) ;


/* ------------------------------
	t_knownhosts
*/ ------------------------------
ALTER TABLE t_knownhosts 
	ADD CONSTRAINT uq_knownhosts01
		UNIQUE (host_name) ;


/* ------------------------------
	t_localtransfermodes  
*/ ------------------------------
ALTER TABLE t_localtransfermodes 
	ADD CONSTRAINT uq_localtransfermodes01
		UNIQUE (localtransfermode) ;


/* ------------------------------
	t_measdataset
*/ ------------------------------
ALTER TABLE t_measdataset
	ADD CONSTRAINT uq_measdata_name
		UNIQUE (name) ;


/* ------------------------------
	t_mediatypes
*/ ------------------------------
ALTER TABLE t_mediatypes 
	ADD CONSTRAINT uq_mediatypes01
		UNIQUE (mediatype) ;


/* ------------------------------
	t_migrationalgos
*/ ------------------------------
ALTER TABLE t_migrationalgos 
	ADD CONSTRAINT uq_migrationalgos01
		UNIQUE (name) ;


/* ------------------------------
	t_ordersqueueblacklist
*/ ------------------------------
ALTER TABLE t_ordersqueueblacklist 
	ADD CONSTRAINT uq_ordersqueueblacklist01
		UNIQUE (orderid, jobresponsible) ;


/* ------------------------------
	t_proc_ftype_generation
*/ ------------------------------
ALTER TABLE t_proc_ftype_generation 
	ADD CONSTRAINT uq_proc_ftype_generation01
		UNIQUE (proc_id, ftype_id) ;


/* ------------------------------
	t_processors
*/ ------------------------------
ALTER TABLE t_processors 
	ADD CONSTRAINT uq_processors01
		UNIQUE (processorname, processorversion) ;


/* ------------------------------
	t_receptionrules
*/ ------------------------------
CREATE UNIQUE INDEX uq_receptionrules01 
	ON t_receptionrules (filetype_id, repository_id, user_id, usermode_id)
		WHERE active = 't' ;

CREATE UNIQUE INDEX uq_receptionrules02 
	ON t_receptionrules (filetype_id, localpath, COALESCE(hostname, 'DUMMY_VALUE_USED_FOR_UNIQUE_CONSTRAINT'), COALESCE(satellite_id, 0), COALESCE(mission, 0), COALESCE(sensor_id, 0))
		WHERE active = 't' ;


/* ------------------------------
	t_receptionruleshist
*/ ------------------------------
ALTER TABLE t_receptionruleshist 
	ADD CONSTRAINT uq_receptionruleshist01
		UNIQUE (receptionrule_id, filename) ;


/* ------------------------------
	t_repositories
*/ ------------------------------
ALTER TABLE t_repositories 
	ADD CONSTRAINT uq_repositories01
		UNIQUE (symbolicname) ;

ALTER TABLE t_repositories 
	ADD CONSTRAINT uq_repositories02
		UNIQUE (transferprotocol_id, host_id, username, remotepath) ;


/* ------------------------------
	t_satellites
*/ ------------------------------
ALTER TABLE t_satellites
	ADD CONSTRAINT uq_t_satellites1
		UNIQUE (satellitename) ;

ALTER TABLE t_satellites
	ADD CONSTRAINT uq_t_satellites2
		UNIQUE (satelliteacronym) ;


/* ------------------------------
	t_sensors
*/ ------------------------------
ALTER TABLE t_sensors
	ADD CONSTRAINT uq_t_sensors1
		UNIQUE (sensorname) ;


/* ------------------------------
	t_sm_sync_missingactions
*/ ------------------------------
ALTER TABLE t_sm_sync_missingactions
	ADD CONSTRAINT uq_sm_sync_missingactions1
		UNIQUE (operation, sm_id, inv_id) ;

ALTER TABLE t_sm_sync_missingactions
	ADD CONSTRAINT uq_sm_sync_missingactions2
		UNIQUE (operation, sm_id, invsm_id) ;


/* ------------------------------
	t_smtype
*/ ------------------------------
ALTER TABLE t_smtype
	ADD CONSTRAINT uq_smtype1
		UNIQUE (type) ;


/* ------------------------------
	t_sor_requests
*/ ------------------------------
ALTER TABLE t_sor_requests 
	ADD CONSTRAINT uq_sor_requests01
		UNIQUE (requestid) ;


/* ------------------------------
	t_statetransitions_allowed
*/ ------------------------------
ALTER TABLE t_statetransitions_allowed 
	ADD CONSTRAINT uq_statetransitions_allowed01
		UNIQUE (table_id, prev_state_id, post_state_id) ;


/* ------------------------------
	t_statetransitions_flows
*/ ------------------------------
ALTER TABLE t_statetransitions_flows 
	ADD CONSTRAINT uq_statetransitions_flows01
		UNIQUE (flow_type) ;


/* ------------------------------
	t_statetransitions_tables
*/ ------------------------------
ALTER TABLE t_statetransitions_tables 
	ADD CONSTRAINT uq_statetransitions_tables01
		UNIQUE (table_name, flow_id) ;


/* ------------------------------
	t_tasktables
*/ ------------------------------
ALTER TABLE t_tasktables 
	ADD CONSTRAINT uq_tasktables01
		UNIQUE (name) ;


/* ------------------------------
	t_tmpnamestrategy
*/ ------------------------------
ALTER TABLE t_tmpnamestrategy 
	ADD CONSTRAINT uq_tmpnamestrategy01
		UNIQUE (tempname) ;


/* ------------------------------
	t_users
*/ ------------------------------
ALTER TABLE t_users 
	ADD CONSTRAINT uq_users01
		UNIQUE (userid) ;


/* ------------------------------
	t_usermodes
*/ ------------------------------
ALTER TABLE t_usermodes 
	ADD CONSTRAINT uq_usermodes01
		UNIQUE (mode) ;





ALTER TABLE t_operationtypes 
	ADD CONSTRAINT uq_operationtypes01
		UNIQUE (type) ;


CREATE UNIQUE index uq_ordersgeneration
	ON t_ordersgeneration (processorname, processorversion, ordertype, algorithm, filetype, COALESCE(fileclass, 'DUMMY')) ;


ALTER TABLE t_rollingpolicyactions
	ADD CONSTRAINT uq_rollingpolicyact
		UNIQUE (action) ;


ALTER TABLE t_rollingpoliciesalgo
	ADD CONSTRAINT uq_rollingpoliciesalgo
		UNIQUE (name) ;


ALTER TABLE t_storagemanager
  ADD CONSTRAINT uq_storagemanager
  	UNIQUE (name) ;


ALTER TABLE t_stations
	ADD CONSTRAINT uq_t_stations01
		UNIQUE (stationname) ;


ALTER TABLE t_stations
	ADD CONSTRAINT uq_t_stations02
		UNIQUE (stationacronym) ;


ALTER TABLE t_waitingorders
	ADD CONSTRAINT uq_waitingorders
		UNIQUE (orderid, filetype, starttime, stoptime, triggertype) ;




ALTER TABLE t_qualifiedflag
	ADD CONSTRAINT uq_qualifiedflag
		UNIQUE (name) ;


ALTER TABLE t_originators
	ADD CONSTRAINT uq_originators
		UNIQUE (name) ;


ALTER TABLE t_sitetype 
	ADD CONSTRAINT uq_sitetype_name
		UNIQUE (name) ;


ALTER TABLE t_sites 
	ADD CONSTRAINT uq_sites_name 
		UNIQUE (name) ;


ALTER TABLE t_invsiterelation 
	ADD CONSTRAINT uq_invsiterelation 
		UNIQUE (filename, fileversion, site) ;


ALTER TABLE t_ordersattachmenttypes 
	ADD CONSTRAINT uq_ordersattachmenttypes
		UNIQUE (name) ;


ALTER TABLE t_smactions 
	ADD CONSTRAINT uq_smactions1
		UNIQUE (filename, fileversion, smid, operation, enddate) ;


ALTER TABLE t_pdsmodes 
	ADD CONSTRAINT uq_pdsmodes_appsub 
		UNIQUE (application, subsystem);


ALTER TABLE t_inventoryeventsconf 
	ADD CONSTRAINT uq_inveventsconf
		UNIQUE (filetype) ;


ALTER TABLE t_cartsinv 
	ADD CONSTRAINT q_cartsubreq 
		UNIQUE (cart, subreq);


ALTER TABLE t_cartsref 
	ADD CONSTRAINT uq_cartsref_extref
		UNIQUE (extref);


ALTER TABLE t_standingdistribution 
	ADD CONSTRAINT uq_stendingdistribution01
		UNIQUE (distpoliciesid, validitystart, validitystop, cartsid, subrequest) ;


ALTER TABLE t_storagegroup 
	ADD CONSTRAINT uq_storagegroup01 
		UNIQUE (filetype, storagemanager) ;


ALTER TABLE t_filetypes_x_migrations 
	ADD CONSTRAINT uq_filetypesxmigrations01
		UNIQUE (sm_id, filetype_id) ;


ALTER TABLE t_filetypes_x_satellites 
	ADD CONSTRAINT uq_t_filetypesxsatellites01 
		UNIQUE (filetype, satellite_id) ;


ALTER TABLE t_dbparametertypes 
	ADD CONSTRAINT uq_dbparametertypes01
		UNIQUE (parametertype) ;


ALTER TABLE t_dbparameters 
	ADD CONSTRAINT uq_dbparameters01
		UNIQUE (parameter) ;


ALTER TABLE t_transferprotocols 
	ADD CONSTRAINT uq_transferprotocols01
		UNIQUE (protocol);


ALTER TABLE t_inv_relation_types 
	ADD CONSTRAINT uq_inv_relation_types01
		UNIQUE (relation_type) ;


ALTER TABLE t_inv_relations 
	ADD CONSTRAINT uq_inv_relations01
		UNIQUE (inv_id1, inv_id2) ;


ALTER TABLE t_filetypes_last_inv 
	ADD CONSTRAINT uq_filetypes_last_inv01
		UNIQUE (filetype) ;


