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
--                                   TABLES                                  --
--                                                                           --
-------------------------------------------------------------------------------


/* ------------------------------
	t_acshsminvrelation
*/ ------------------------------
CREATE TABLE t_acshsminvrelation (
    id INTEGER NOT NULL,
    externalreference VARCHAR(64) NOT NULL,
	signature VARCHAR(64)
) ;
ALTER TABLE t_acshsminvrelation
	ADD CONSTRAINT pk_t_acshsminvrelation
		PRIMARY KEY (id) ;



/* ------------------------------
	t_acsisminvrelation
*/ ------------------------------
CREATE TABLE t_acsisminvrelation (
    id INT NOT NULL,
    path VARCHAR(512) NOT NULL,
    signature VARCHAR(64),
	medialabel VARCHAR(128)
);
ALTER TABLE t_acsisminvrelation 
	ADD CONSTRAINT pk_t_acsisminvrel
		PRIMARY KEY (id);



/* ------------------------------
	t_acsltainvrelation
*/ ------------------------------
CREATE TABLE t_acsltainvrelation (
	id INTEGER NOT NULL,
	productid VARCHAR(512) NOT NULL,
	signature VARCHAR(64)
) ;
ALTER TABLE t_acsltainvrelation
	ADD CONSTRAINT pk_t_acsltainvrelation
		PRIMARY KEY (id) ;



/* ------------------------------
	t_algorithm
*/ ------------------------------
CREATE TABLE t_algorithm (
	id SERIAL NOT NULL,
	algorithm VARCHAR(32) NOT NULL,
	description VARCHAR(64)
) ;
ALTER TABLE t_algorithm
	ADD CONSTRAINT pk_t_algorithm
		PRIMARY KEY (id) ;



/* --------------------------
	t_anomaly_types
*/ --------------------------
CREATE TABLE t_anomaly_types (
	id SMALLINT NOT NULL,
	anomaly_type VARCHAR(24) NOT NULL
) ;
ALTER TABLE t_anomaly_types 
	ADD CONSTRAINT pk_t_anomaly_types
		PRIMARY KEY (id) ;



/* ------------------------------
	t_applications
*/ ------------------------------
CREATE TABLE t_applications (
    id_application SERIAL NOT NULL,
    app_name VARCHAR(64) NOT NULL,
    app_description TEXT
) ;
ALTER TABLE t_applications
	ADD CONSTRAINT pk_t_applications
		PRIMARY KEY (id_application) ;



/* ------------------------------
	t_applicationevents
*/ ------------------------------
CREATE TABLE t_applicationevents (
    id_applicationevent SERIAL NOT NULL,
	application_id INTEGER NOT NULL,
	tstamp TIMESTAMP(6) DEFAULT NOW() NOT NULL,
	event_type VARCHAR(64) NOT NULL,
	event_description VARCHAR(256)
) ;
ALTER TABLE t_applicationevents
	ADD CONSTRAINT pk_t_applicationevents
		PRIMARY KEY (id_applicationevent) ;



/* ------------------------------
	t_applicationstatus
*/ ------------------------------
CREATE TABLE t_applicationstatus (
	id SERIAL NOT NULL,
    application VARCHAR(64) NOT NULL,
    instance VARCHAR(8) NOT NULL,
    status OID
) ;
ALTER TABLE t_applicationstatus
	ADD CONSTRAINT pk_t_applicationstatus
		PRIMARY KEY (id) ;



/* ------------------------------
	t_asyncsm_operations
*/ ------------------------------
CREATE TABLE t_asyncsm_operations (
    id_asyncsm_operation SERIAL NOT NULL,
    filename VARCHAR(128) NOT NULL,
	fileversion VARCHAR(32) NOT NULL,
	unique_id VARCHAR(160) NOT NULL,
	tstamp TIMESTAMP(6) DEFAULT now() NOT NULL,
	operation_id SMALLINT NOT NULL,
	sm_id INTEGER NOT NULL,
	status_id SMALLINT NOT NULL,
	status_reason TEXT
) ;
ALTER TABLE t_asyncsm_operations
	ADD CONSTRAINT pk_t_asyncsm_operations
		PRIMARY KEY (id_asyncsm_operation) ;



/* ------------------------------
	t_bands
*/ ------------------------------
CREATE TABLE t_bands (
    id SERIAL NOT NULL,
    name VARCHAR(16) NOT NULL
);
ALTER TABLE t_bands 
	ADD CONSTRAINT pk_t_bands1
		PRIMARY KEY (id) ;



/* ------------------------------
	t_cartsinv
*/ ------------------------------
CREATE TABLE t_cartsinv (
    id SERIAL NOT NULL,
	filename VARCHAR(128),
	fileversion VARCHAR(32),
	cart INTEGER NOT NULL,
	subreq INTEGER NOT NULL,
	distsubreq INTEGER,
	ordsubreq INTEGER,
	stndsubreq INTEGER,
	status VARCHAR(32) DEFAULT 'ELIGIBLE' NOT NULL,
	handled BOOLEAN DEFAULT 'f' NOT NULL
);
ALTER TABLE t_cartsinv 
	ADD CONSTRAINT pk_t_cartsinv 
		PRIMARY KEY (id);



/* ------------------------------
	t_cartsref
*/ ------------------------------
CREATE TABLE t_cartsref (
    id SERIAL NOT NULL,
    extref VARCHAR(128) NOT NULL,
    data TEXT,
	totreqs INTEGER NOT NULL,
	procreqs INTEGER DEFAULT 0 NOT NULL ,
	distreqs INTEGER DEFAULT 0 NOT NULL ,
	status VARCHAR(32) NOT NULL,
	stndreqs INTEGER DEFAULT 0 NOT NULL,
	account VARCHAR(32)
);
ALTER TABLE t_cartsref 
	ADD CONSTRAINT pk_t_cartsref 
		PRIMARY KEY (id);



/* ------------------------------
	t_cdp_feederqueue
*/ ------------------------------
CREATE TABLE t_cdp_feederqueue (
    id SERIAL NOT NULL,
    uuid VARCHAR(160),
	checksum JSONB,
	uri VARCHAR(512) NOT NULL,
	cache_path VARCHAR(512),
	s3_reference TEXT,
	inv_id BIGINT,
	creationdate TIMESTAMP(6) DEFAULT NOW() NOT NULL,
	modificationdate TIMESTAMP(6),
	startdate TIMESTAMP(6),
	completiondate TIMESTAMP(6),
	retry_count INTEGER DEFAULT 0,
	retry_time TIMESTAMP(6),
	status_id SMALLINT NOT NULL,
	jobresponsible VARCHAR(64),
	destinationurl VARCHAR(512) NOT NULL,
	tempnamestrategy_id SMALLINT NOT NULL,
	tempnamevalue VARCHAR(24)
);
ALTER TABLE t_cdp_feederqueue 
	ADD CONSTRAINT pk_t_cdp_feederqueue
		PRIMARY KEY (id) ;



/* ------------------------------
	t_cdp_frame_grid
*/ ------------------------------
CREATE TABLE t_cdp_frame_grid (
    id SERIAL NOT NULL,
    satelliteid INTEGER NOT NULL,
	filetype VARCHAR(16) NOT NULL,
	ordertype VARCHAR(8) NOT NULL,
	orbitNumber INTEGER NOT NULL,
	frameStart TIMESTAMP(6) NOT NULL,
	frameNumber INTEGER NOT NULL,
	orderEmitted BOOLEAN NOT NULL DEFAULT FALSE,
	creationTime TIMESTAMP(6) NOT NULL DEFAULT NOW()
);
ALTER TABLE t_cdp_frame_grid 
	ADD CONSTRAINT pk_t_cdp_frame_grid
		PRIMARY KEY (id) ;



/* ------------------------------
	t_cdp_notification_queue
*/ ------------------------------
CREATE TABLE t_cdp_notification_queue (
    id SERIAL NOT NULL,
	feederqueue_id INTEGER NOT NULL,
    uuid VARCHAR(160),
	checksum JSONB,
	uri VARCHAR(512) NOT NULL,
	creationdate TIMESTAMP(6) DEFAULT NOW() NOT NULL,
	modificationdate TIMESTAMP(6),
	startdate TIMESTAMP(6),
	completiondate TIMESTAMP(6),
	retry_count INTEGER DEFAULT 0,
	retry_time TIMESTAMP(6),
	status_id SMALLINT NOT NULL,
	jobresponsible VARCHAR(64),
	notification_endpoint VARCHAR(64)
);
ALTER TABLE t_cdp_notification_queue 
	ADD CONSTRAINT pk_t_cdp_notification_queue
		PRIMARY KEY (id) ;



/* ------------------------------
	t_classifiedfiletypes
*/ ------------------------------
CREATE TABLE t_classifiedfiletypes (
    id       SERIAL NOT NULL,
    filetype VARCHAR(16) NOT NULL
) ;
ALTER TABLE t_classifiedfiletypes
  ADD CONSTRAINT pk_t_classifiedfiletypes
  	PRIMARY KEY (id) ;



/* ------------------------------
	t_cleanupalgos 
*/ ------------------------------
CREATE TABLE t_cleanupalgos (
	id_cleanupalgo INT2 NOT NULL,
	name VARCHAR(24) NOT NULL,
	description VARCHAR(128)
) ;
ALTER TABLE t_cleanupalgos
	ADD CONSTRAINT pk_t_cleanupalgos
		PRIMARY KEY (id_cleanupalgo) ;



/* ------------------------------
	t_cleanuprules 
*/ ------------------------------
CREATE TABLE t_cleanuprules (
	id_cleanuprule SERIAL NOT NULL,
	repository_id INTEGER NOT NULL,
	cleanupalgo_id INT2 NOT NULL,
	hiquota_mb BIGINT NOT NULL,
	lowquota_mb BIGINT NOT NULL,
	age_h INTEGER
) ;
ALTER TABLE t_cleanuprules
	ADD CONSTRAINT pk_t_cleanuprules
		PRIMARY KEY (id_cleanuprule) ;



/* ------------------------------
	t_completed_orders
*/ ------------------------------
CREATE TABLE t_completed_orders (
    order_id INT NOT NULL
);
ALTER TABLE t_completed_orders 
	ADD CONSTRAINT pk_t_completed_425
		PRIMARY KEY (order_id);



/* ------------------------------
	t_configurationschemas
*/ ------------------------------
CREATE TABLE t_configurationschemas (
	id SERIAL NOT NULL,
	namespace VARCHAR(64) NOT NULL,
    version VARCHAR(8) NOT NULL,
    schema TEXT
) ;
ALTER TABLE t_configurationschemas
	ADD CONSTRAINT pk_t_configurationschemas
		PRIMARY KEY (id) ;



/* ------------------------------
	t_configurationspaces
*/ ------------------------------
CREATE TABLE t_configurationspaces (
	id SERIAL NOT NULL,
    namespace VARCHAR(64) NOT NULL,
    version VARCHAR(8) NOT NULL,
    space TEXT
) ;
ALTER TABLE t_configurationspaces
	ADD CONSTRAINT pk_t_configurationspaces
		PRIMARY KEY (id) ;



/* ------------------------------
	t_configurationstylesheets
*/ ------------------------------
CREATE TABLE t_configurationstylesheets (
    id SERIAL NOT NULL,
	name VARCHAR(64) NOT NULL,
    version VARCHAR(8) NOT NULL,
    stylesheet TEXT
) ;
ALTER TABLE t_configurationstylesheets 
	ADD CONSTRAINT pk_t_configurationstylesheets
		PRIMARY KEY (id) ;



/* ------------------------------
	t_dbparameters
*/ ------------------------------
CREATE TABLE t_dbparameters (
	id SERIAL NOT NULL,
	parameter VARCHAR(32) NOT NULL,
	parametertype_id INT2 NOT NULL,
	value_int INTEGER,
	value_str VARCHAR(32),
	value_dttm TIMESTAMP(6),
	value_bool BOOLEAN,
	param_description TEXT
) ;
ALTER TABLE t_dbparameters 
	ADD CONSTRAINT pk_t_dbparameters
		PRIMARY KEY (id) ;



/* ------------------------------
	t_dbparametertypes
*/ ------------------------------
CREATE TABLE t_dbparametertypes (
	id INT2 NOT NULL,
	parametertype VARCHAR(16) NOT NULL
) ;
ALTER TABLE t_dbparametertypes 
	ADD CONSTRAINT pk_t_dbparametertypes
		PRIMARY KEY (id) ;



/* ------------------------------
	t_dbstatus
*/ ------------------------------
CREATE TABLE t_dbstatus (
    master BOOLEAN NOT NULL,
    modifycationdate TIMESTAMP(6) DEFAULT NOW()
) ;



/* ------------------------------
	t_discacheinvrelation
*/ ------------------------------
CREATE TABLE t_discacheinvrelation (
    id INTEGER NOT NULL,
    path VARCHAR(512) NOT NULL
) ;
ALTER TABLE t_discacheinvrelation
	ADD CONSTRAINT pk_t_discacheinvrelation
		PRIMARY KEY (id) ;



/* ------------------------------
	t_discoveryplugins 
*/ ------------------------------
CREATE TABLE t_discoveryplugins (
	id_discoveryplugin SERIAL NOT NULL,
	name VARCHAR(24) NOT NULL
) ;
ALTER TABLE t_discoveryplugins
	ADD CONSTRAINT pk_t_discoveryplugins
		PRIMARY KEY (id_discoveryplugin) ;



/* ------------------------------
	t_disseminationpackingalgos 
*/ ------------------------------
CREATE TABLE t_disseminationpackingalgos (
	id_disseminationpackingalgo SERIAL NOT NULL,
	factorykey VARCHAR(64) NOT NULL,
	description VARCHAR(128)
) ;
ALTER TABLE t_disseminationpackingalgos
	ADD CONSTRAINT pk_t_disseminationpackingalgos
		PRIMARY KEY (id_disseminationpackingalgo) ;



/* ------------------------------
	t_distpoldataset
*/ ------------------------------
CREATE TABLE t_distpoldataset (
	policy INTEGER NOT NULL,
	dataset INTEGER NOT NULL,
	num INTEGER NOT NULL
) ;
ALTER TABLE t_distpoldataset
	ADD CONSTRAINT pk_t_distpoldataset
		PRIMARY KEY (policy, dataset) ;



/* ------------------------------
	t_distributionalgo
*/ ------------------------------
CREATE TABLE t_distributionalgo (
    id INT2 NOT NULL,
    name VARCHAR(64) NOT NULL,
	on_demand BOOLEAN DEFAULT 't' NOT NULL
) ;
ALTER TABLE t_distributionalgo
	ADD CONSTRAINT pk_t_distributionalgo
		PRIMARY KEY (id) ;



/* ------------------------------
	t_distributionformat
*/ ------------------------------
CREATE TABLE t_distributionformat (
	id SERIAL NOT NULL,
    format VARCHAR(16) NOT NULL,
    description VARCHAR(64)
) ;
ALTER TABLE t_distributionformat
  ADD CONSTRAINT pk_t_distributionformat
  	PRIMARY KEY (id) ;



/* ------------------------------
	t_distributionitems
*/ ------------------------------
CREATE TABLE t_distributionitems (
    id SERIAL NOT NULL,
    duedate TIMESTAMP(6) NOT NULL,
    distributeddate TIMESTAMP(6),
    creationdate TIMESTAMP(6) DEFAULT NOW(),
    packageid INTEGER,
    filename VARCHAR(128) NOT NULL,
    fileversion VARCHAR(32) NOT NULL,
    mediatype VARCHAR(16) NOT NULL,
    userid VARCHAR(32) NOT NULL,
    format INTEGER,
	cartid INTEGER,
	subreq INTEGER,
	distributionrule_id INTEGER NOT NULL,
	disseminationpackingalgo_id INTEGER NOT NULL
) ;
ALTER TABLE t_distributionitems
	ADD CONSTRAINT pk_t_distributionitems
		PRIMARY KEY (id) ;



/* ------------------------------
	t_distributionqueue
*/ ------------------------------
CREATE TABLE t_distributionqueue (
    packageid SERIAL NOT NULL,
    status VARCHAR(32) NOT NULL,
    distributeddate TIMESTAMP(6),
    jobresponsible VARCHAR(64),
    creationdate TIMESTAMP(6) DEFAULT NOW() NOT NULL,
    format INTEGER,
	retrycounter INTEGER DEFAULT 0 NOT NULL,
	delserviceparam TEXT,
	lastattempt TIMESTAMP(6)
) ;
ALTER TABLE t_distributionqueue
	ADD CONSTRAINT pk_t_distributionqueue
		PRIMARY KEY (packageid) ;



/* ------------------------------
	t_distributionrules
*/ ------------------------------
CREATE TABLE t_distributionrules (
	id_distributionrule SERIAL NOT NULL, 
	active BOOLEAN DEFAULT 't' NOT NULL,
	filetype_id INTEGER NOT NULL,
	mediatype_id INT2 NOT NULL,
	maxdaytodeliver INTEGER DEFAULT 1 NOT NULL,
	failureaddresses VARCHAR(1024),
	successaddresses VARCHAR(1024),
	connretries INTEGER DEFAULT 1 NOT NULL,
	retryperiod_sec INTEGER NOT NULL,
	repository_id INTEGER NOT NULL,
	user_id INTEGER NOT NULL,
	usermode_id INT2 DEFAULT 1 NOT NULL,
	distributionalgo_id INT2 DEFAULT 1 NOT NULL,
	format_tt_id INTEGER DEFAULT f_get_tasktable_id('defaultDistribFormat') NOT NULL,
	delivery_tt_id INTEGER DEFAULT f_get_tasktable_id('defaultDelivery') NOT NULL,
	delcheck_tt_id INTEGER DEFAULT f_get_tasktable_id('nopDeliveryCheck') NOT NULL,
	fileclass_id INTEGER,
	disseminationpackingalgo_id INTEGER NOT NULL,
	ackcheckperiod_sec INTEGER DEFAULT 60 NOT NULL,
	parameters TEXT,
	priority INTEGER DEFAULT 1 NOT NULL,
	ondemand BOOLEAN DEFAULT 'f' NOT NULL
) ;
ALTER TABLE t_distributionrules
	ADD CONSTRAINT pk_t_distributionrules
		PRIMARY KEY (id_distributionrule) ;



/* ------------------------------
	t_distributionruleslock
*/ ------------------------------
CREATE TABLE t_distributionruleslock (
    packageid INTEGER NOT NULL,
	responsible VARCHAR(128) NOT NULL,
    tstamp TIMESTAMP(6) DEFAULT NOW() NOT NULL
) ;
ALTER TABLE t_distributionruleslock
  ADD CONSTRAINT pk_t_distributionruleslock
  	PRIMARY KEY (packageid) ;



/* ------------------------------
	t_distruleoriginator
*/ ------------------------------
CREATE TABLE t_distruleoriginator (
    distributionrule_id INTEGER NOT NULL,
    originator_id INTEGER NOT NULL
) ;
ALTER TABLE t_distruleoriginator
	ADD CONSTRAINT pk_t_distruleoriginator
		PRIMARY KEY (distributionrule_id, originator_id) ;



/* ------------------------------
	t_distributionwaitingitems
*/ ------------------------------
CREATE TABLE t_distributionwaitingitems (
	id SERIAL NOT NULL,
	distributionitem_id INTEGER NOT NULL,
	package_id INTEGER NOT NULL,
	inventory_id BIGINT NOT NULL,
	CONSTRAINT pk_t_distributionwaitingitems PRIMARY KEY (id)
) ;
ALTER TABLE t_distributionwaitingitems
	ADD CONSTRAINT fk_distributionwaitingitems01
		FOREIGN KEY (distributionitem_id)
			REFERENCES t_distributionitems (id)
				ON DELETE CASCADE ;



/* ------------------------------
	t_downlink_session_tracks
*/ ------------------------------
CREATE TABLE t_downlink_session_tracks (
	id SERIAL NOT NULL,
	session_id VARCHAR(64) NOT NULL,
	downlink_stop_time TIMESTAMP(6)
);
ALTER TABLE t_downlink_session_tracks
	ADD CONSTRAINT pk_t_downlink_session_tracks
		PRIMARY KEY (id) ;



/* ------------------------------
	t_emitted_order_events
*/ ------------------------------
CREATE TABLE t_emitted_order_events (
	id SERIAL NOT NULL,
	event_time TIMESTAMP NOT NULL,
	event_tag VARCHAR(64) NOT NULL,
	time_driven_start TIMESTAMP NOT NULL,
	time_driven_stop TIMESTAMP NOT NULL,
	processor_id INTEGER NOT NULL,
	order_type VARCHAR(8) NOT NULL,
	creation_date TIMESTAMP DEFAULT NOW()
) ;
ALTER TABLE t_emitted_order_events
	ADD CONSTRAINT pk_t_emitted_order_events
		PRIMARY KEY (id) ;



/* ------------------------------
	t_filetypescomps
*/ ------------------------------
CREATE TABLE t_filetypescomps (
    id SERIAL NOT NULL,
    pfiletype INTEGER NOT NULL,
    cfiletype INTEGER NOT NULL
);
ALTER TABLE t_filetypescomps 
	ADD CONSTRAINT pk_t_filetypescomps
		PRIMARY KEY (id) ;



/* --------------------------
	t_inv_client
*/ --------------------------
CREATE TABLE t_inv_client (
    id_inv_client SERIAL NOT NULL,
    name VARCHAR(128) NOT NULL
) ;
ALTER TABLE t_inv_client
	ADD CONSTRAINT pk_t_inv_client
		PRIMARY KEY (id_inv_client) ;



/* ------------------------------
	t_inv_additional_params
*/ ------------------------------
CREATE TABLE t_inv_additional_params (
	id BIGSERIAL NOT NULL,
	inv_id BIGINT NOT NULL,
	timeliness_key_id INTEGER,
	granule_number INTEGER,
	granule_position VARCHAR(24),
	dump_start TIMESTAMP(6),
	uuid VARCHAR(160) NOT NULL DEFAULT uuid_generate_v1(),
	data_take_id INTEGER,
	nextequatorxtime TIMESTAMP(6)
);
ALTER TABLE t_inv_additional_params
	ADD CONSTRAINT pk_t_inv_additional_params
		PRIMARY KEY (id) ;



/* ------------------------------
	t_inv_data_restored
*/ ------------------------------
CREATE TABLE t_inv_data_restored (
	id INTEGER NOT NULL,
	sm_id INTEGER NOT NULL,
	inv_id BIGINT NOT NULL,
	parent_inv_id BIGINT,
	filetype_id INTEGER NOT NULL,
	size BIGINT NOT NULL,
	tstamp TIMESTAMP DEFAULT NOW()
) ;
ALTER TABLE t_inv_data_restored
	ADD CONSTRAINT pk_t_inv_data_restored
		PRIMARY KEY (id) ;



/* --------------------------
	t_inv_distributions
*/ --------------------------
CREATE TABLE t_inv_distributions (
    inv_id BIGINT NOT NULL,
	retain_counter INTEGER NOT NULL,
    last_update TIMESTAMP(6) DEFAULT NOW() NOT NULL
) ;
ALTER TABLE t_inv_distributions
	ADD CONSTRAINT pk_t_inv_distributions
		PRIMARY KEY (inv_id) ;



/* --------------------------
	t_inv_downloads
*/ --------------------------
CREATE TABLE t_inv_downloads (
    inv_id BIGINT NOT NULL,
	client_id INTEGER NOT NULL,
    downloadtime TIMESTAMP(6) DEFAULT NOW() NOT NULL
) ;
ALTER TABLE t_inv_downloads
	ADD CONSTRAINT pk_t_inv_downloads
		PRIMARY KEY (inv_id, client_id) ;



/* ------------------------------
	t_inventorycomments
*/ ------------------------------
CREATE TABLE t_inventorycomments (
    id_inventorycomment BIGSERIAL NOT NULL,
	comment VARCHAR(128) NOT NULL,
    date TIMESTAMP(6) DEFAULT NOW() NOT NULL,
    author VARCHAR(32) NOT NULL,
    filename VARCHAR(128) NOT NULL,
    fileversion VARCHAR(32) NOT NULL
) ;
ALTER TABLE t_inventorycomments
	ADD CONSTRAINT pk_t_inventorycomments
		PRIMARY KEY (id_inventorycomment) ;



/* ------------------------------
	t_knownhosts
*/ ------------------------------
CREATE TABLE t_knownhosts (
	id SERIAL NOT NULL,
	ip_address VARCHAR(16),
	host_name VARCHAR(256)
) ;
ALTER TABLE t_knownhosts 
	ADD CONSTRAINT pk_t_knownhosts
		PRIMARY KEY (id) ;



/* ------------------------------
	t_localtransfermodes  
*/ ------------------------------
CREATE TABLE t_localtransfermodes (
    id SMALLINT NOT NULL,
    localtransfermode VARCHAR(30) NOT NULL
) ;
ALTER TABLE t_localtransfermodes
	ADD CONSTRAINT pk_t_localtransfermodes
		PRIMARY KEY (id) ;



/* ------------------------------
	t_mediacapformat
*/ ------------------------------
CREATE TABLE t_mediacapformat (
    format     VARCHAR(16) NOT NULL,
    mediatype  VARCHAR(16) NOT NULL,
    multiplier REAL DEFAULT 1.00000000 NOT NULL
) ;
ALTER TABLE t_mediacapformat
  ADD CONSTRAINT pk_t_mediacapformat
  	PRIMARY KEY (mediatype, format) ;



/* ------------------------------
	t_mediatypes
*/ ------------------------------
CREATE TABLE t_mediatypes (
    id_mediatype INT2 NOT NULL,
	mediatype VARCHAR(16) NOT NULL,
    uncompressedcapacity INTEGER NOT NULL
) ;
ALTER TABLE t_mediatypes
	ADD CONSTRAINT pk_t_mediatypes
		PRIMARY KEY (id_mediatype) ;



/* ------------------------------
	t_migrationalgos
*/ ------------------------------
CREATE TABLE t_migrationalgos (
	id_migrationalgo SERIAL NOT NULL,
	name VARCHAR(32) NOT NULL
) ;
ALTER TABLE t_migrationalgos
	ADD CONSTRAINT pk_t_migrationalgos
		PRIMARY KEY (id_migrationalgo) ;



/* ------------------------------
	t_objsminvrelation
*/ ------------------------------
CREATE TABLE t_objsminvrelation (
	id INTEGER NOT NULL,
	http_path VARCHAR(512) NOT NULL,
	s3_path VARCHAR(512) NOT NULL,
	signature VARCHAR(64)
);
ALTER TABLE t_objsminvrelation
	ADD CONSTRAINT pk_t_objsminvrelation
		PRIMARY KEY (id) ;



/* ------------------------------
	t_ondemanddistribution
*/ ------------------------------
CREATE TABLE t_ondemanddistribution (
	orderid INTEGER NOT NULL,
    userid VARCHAR(32) NOT NULL,
    format INTEGER,
	mediatype VARCHAR(16),
	disseminationpackingalgo_id INTEGER NOT NULL
) ;
ALTER TABLE t_ondemanddistribution
	ADD CONSTRAINT pk_t_ondemanddistribution
		PRIMARY KEY (orderid, userid) ;



/* ------------------------------
        t_operation_requests
*/ ------------------------------
CREATE TABLE t_operation_requests (
	id SERIAL NOT NULL,
	object_id BIGINT NOT NULL,
	status VARCHAR(32) NOT NULL,
	type VARCHAR(32) NOT NULL,
	start_time TIMESTAMP(6) DEFAULT NOW() NOT NULL,
	stop_time TIMESTAMP(6),
	error_message TEXT,
	content_type VARCHAR(32) NOT NULL
);
ALTER TABLE t_operation_requests
	ADD CONSTRAINT pk_t_operation_requests
		PRIMARY KEY (id) ;



/* ------------------------------
	t_operationtypes
*/ ------------------------------
CREATE TABLE t_operationtypes (
	id_operationtype INT2 NOT NULL,
	type VARCHAR(32) NOT NULL
) ;
ALTER TABLE t_operationtypes
	ADD CONSTRAINT pk_t_operationtypes
		PRIMARY KEY (id_operationtype) ;



/* ------------------------------
	t_orderstatetransitions
*/ ------------------------------
CREATE TABLE t_orderstatetransitions (
	id BIGSERIAL NOT NULL,
	orderid INTEGER NOT NULL,
	tstamp TIMESTAMP(6) DEFAULT NOW(),
	status VARCHAR(32) NOT NULL
) ;
ALTER TABLE t_orderstatetransitions
	ADD CONSTRAINT pk_t_orderstatetransitions
		PRIMARY KEY (id) ;



/* ------------------------------
	t_ordertypes
*/ ------------------------------
CREATE TABLE t_ordertypes (
	ordertype VARCHAR(8) NOT NULL,
	ordertypedescription VARCHAR(64) NOT NULL
) ;
ALTER TABLE t_ordertypes
  ADD CONSTRAINT pk_t_ordertypes
  	PRIMARY KEY (ordertype) ;



/* ------------------------------
	t_processors
*/ ------------------------------
CREATE TABLE t_processors (
    id_processor SERIAL NOT NULL,
	processorname VARCHAR(32) NOT NULL,
    processorversion VARCHAR(8) NOT NULL,
    description VARCHAR(64),
	baseline VARCHAR(16)
) ;
ALTER TABLE t_processors
	ADD CONSTRAINT pk_t_processors
		PRIMARY KEY (id_processor) ;



/* ------------------------------
	t_proc_ftype_generation
*/ ------------------------------
CREATE TABLE t_proc_ftype_generation (
    id INTEGER NOT NULL,
    proc_id INTEGER NOT NULL,
	ftype_id INTEGER NOT NULL
) ;
ALTER TABLE t_proc_ftype_generation
	ADD CONSTRAINT pk_t_proc_ftype_generation
		PRIMARY KEY (id) ;



/* ------------------------------
	t_rollingpolicyactions
*/ ------------------------------
CREATE TABLE t_rollingpolicyactions (
	id SERIAL NOT NULL,
	action VARCHAR(60) NOT NULL
) ;
ALTER TABLE t_rollingpolicyactions
	ADD CONSTRAINT pk_t_rollingpolicyactions
		PRIMARY KEY (id) ;



/* ------------------------------
	t_rollingpoliciesalgo
*/ ------------------------------
CREATE TABLE t_rollingpoliciesalgo (
	id SERIAL NOT NULL,
	name VARCHAR(60) NOT NULL
) ;
ALTER TABLE t_rollingpoliciesalgo
	ADD CONSTRAINT pk_t_rollingpoliciesalgo
		PRIMARY KEY (id) ;



/* ------------------------------
	t_sm_sync_missingactions
*/ ------------------------------
CREATE TABLE t_sm_sync_missingactions (
    id SERIAL NOT NULL,
	operation VARCHAR(16) NOT NULL,
	sm_id INTEGER NOT NULL,
	inv_id BIGINT,
	invsm_id INTEGER,
	tstamp TIMESTAMP(6) DEFAULT now() NOT NULL
) ;
ALTER TABLE t_sm_sync_missingactions
	ADD CONSTRAINT pk_t_sm_sync_missingactions
		PRIMARY KEY (id) ;



/* ------------------------------
	t_statetransitions_allowed
*/ ------------------------------
CREATE TABLE t_statetransitions_allowed (
    id SERIAL NOT NULL,
    table_id INT NOT NULL,
	prev_state_id INT2 NOT NULL,
    post_state_id INT2 NOT NULL
) ;
ALTER TABLE t_statetransitions_allowed
	ADD CONSTRAINT pk_t_statetransitions_allowed
		PRIMARY KEY (id) ;



/* ------------------------------
	t_statetransitions_flows
*/ ------------------------------
CREATE TABLE t_statetransitions_flows (
    id SMALLINT NOT NULL,
    flow_type VARCHAR(30) NOT NULL
) ;
ALTER TABLE t_statetransitions_flows
	ADD CONSTRAINT pk_t_statetransitions_flows
		PRIMARY KEY (id) ;



/* ------------------------------
	t_statetransitions_tables
*/ ------------------------------
CREATE TABLE t_statetransitions_tables (
    id SERIAL NOT NULL,
    table_name VARCHAR(30) NOT NULL,
    flow_id SMALLINT DEFAULT 1 NOT NULL
) ;
ALTER TABLE t_statetransitions_tables
	ADD CONSTRAINT pk_t_statetransitions_tables
		PRIMARY KEY (id) ;



/* ------------------------------
	t_stations
*/ ------------------------------
CREATE TABLE t_stations (
	stationid INT2 NOT NULL,
	stationacronym VARCHAR(4) NOT NULL,
	stationname VARCHAR(64) NOT NULL,
	position VARCHAR(60)
) ;
ALTER TABLE t_stations
	ADD CONSTRAINT pk_t_stations
		PRIMARY KEY (stationid) ;



/* ------------------------------
	t_temporaryorders
*/ ------------------------------
CREATE TABLE t_temporaryorders (
	id SERIAL NOT NULL,
	filename VARCHAR(128) NOT NULL,
	fileversion VARCHAR(32) NOT NULL,
	algorithm VARCHAR(32) NOT NULL,
	filetype VARCHAR(16) NOT NULL,
	ordertype VARCHAR(8) NOT NULL,
	processorname VARCHAR(32) NOT NULL,
	processorversion VARCHAR(8) NOT NULL,
	processorconfiguration VARCHAR(8) NOT NULL,
	priority INT2 NOT NULL,
	troubleshooting BOOLEAN NOT NULL,
	processingtimewindow INT2 NOT NULL,
	ordersat INT2,
	tstamp TIMESTAMP(6) DEFAULT NOW() NOT NULL,
	emittime TIMESTAMP(6),
	time_driven_start TIMESTAMP,
	time_driven_stop TIMESTAMP
) ;
ALTER TABLE t_temporaryorders
	ADD CONSTRAINT pk_t_temporaryorders
		PRIMARY KEY (id) ;



/* ------------------------------
	t_invspecialrollingpolicies
*/ ------------------------------
CREATE TABLE t_invspecialrollingpolicies (
	id SERIAL NOT NULL,
	inventoryname VARCHAR(128) NOT NULL,
	inventoryversion VARCHAR(32) NOT NULL,
	expirationdate TIMESTAMP(6),
	action INTEGER NOT NULL,
	storageid INT,
	consumable BOOLEAN DEFAULT 'f' NOT NULL
) ;
ALTER TABLE t_invspecialrollingpolicies
	ADD CONSTRAINT pk_t_invspecialrollingpolicies
		PRIMARY KEY (id) ;



/* ------------------------------
	t_fileclasses
*/ ------------------------------
CREATE TABLE t_fileclasses (
    id_fileclass SERIAL NOT NULL,
	fileclass VARCHAR(8) NOT NULL,
    description VARCHAR(64)
) ;
ALTER TABLE t_fileclasses
	ADD CONSTRAINT pk_t_fileclasses
		PRIMARY KEY (id_fileclass) ;



/* ------------------------------
	t_inventory
*/ ------------------------------
CREATE TABLE t_inventory (
	id_inv BIGSERIAL NOT NULL,
	filename VARCHAR(128) NOT NULL,
	fileversion VARCHAR(32) NOT NULL,
	unique_id VARCHAR(160) DEFAULT uuid_generate_v1() NOT NULL,
	source VARCHAR(128),
	sourceswversion VARCHAR(8),
	generationtime TIMESTAMP(6) NOT NULL,
	validitystart TIMESTAMP(6) NOT NULL,
	validitystop TIMESTAMP(6) NOT NULL,
	startorbitnumber INTEGER,
	stoporbitnumber INTEGER,
	geographiclocalization VARCHAR(32000),
	qualityinfo VARCHAR(72),
	validityflag BOOLEAN NOT NULL,
	validationdate TIMESTAMP(6) NOT NULL,
	headersize BIGINT,
	datasize BIGINT,
	inventorydate TIMESTAMP(6) DEFAULT NOW() NOT NULL,
	modificationdate TIMESTAMP(6) DEFAULT NOW() NOT NULL,
	filetype VARCHAR(16) NOT NULL,
	fileclass VARCHAR(8) NOT NULL,
	orderid INTEGER,
	sensorid INT2,
	acquisitionstationid INT2,
	processingstationid INT2,
	sensormode VARCHAR(16),
	phase VARCHAR(8),
	satelliteid INT2,
	ascendingflag BOOLEAN,
	equatorxlongitude FLOAT8,
	equatorxtime TIMESTAMP(6),
	mission INT2,
	qualified INTEGER DEFAULT 1 NOT NULL,
	originator INTEGER,
	qualifierfile VARCHAR(128),
	band INT,
	tdi INT2,
	cloud_pctg DOUBLE PRECISION,
	prodspec VARCHAR(128),
	dir_id BIGINT,
	ref_dir_id BIGINT,
	logical_id VARCHAR(256),
	qualityvalue DOUBLE PRECISION,
	md5 VARCHAR(32),
	attributes JSONB
) ;
ALTER TABLE t_inventory
	ADD CONSTRAINT pk_t_inventory
		PRIMARY KEY (id_inv) ;



/* ------------------------------
	t_inventoryevents
*/ ------------------------------
CREATE TABLE t_inventoryevents (
	id SERIAL NOT NULL,
	filename VARCHAR(128) NOT NULL,
    fileversion VARCHAR(32) NOT NULL,
	modificationdate TIMESTAMP(6) DEFAULT NOW() NOT NULL,
	event VARCHAR(16) NOT NULL
) ;
ALTER TABLE t_inventoryevents
	ADD CONSTRAINT pk_t_inventoryevents
		PRIMARY KEY (id) ;



/* ------------------------------
	t_inventoryextra
*/ ------------------------------
CREATE TABLE t_inventoryextra (
	inv_id BIGINT NOT NULL,
	extra TEXT
) ;
ALTER TABLE t_inventoryextra
	ADD CONSTRAINT pk_t_inventoryextra
		PRIMARY KEY (inv_id) ;



/* ------------------------------
	t_ordersgeneration
*/ ------------------------------
CREATE TABLE t_ordersgeneration (
    id                     SERIAL NOT NULL,
	processorname          VARCHAR(32) NOT NULL,
    processorversion       VARCHAR(8) NOT NULL,
    processorconfiguration VARCHAR(8) NOT NULL,
    ordertype              VARCHAR(8) NOT NULL,
    priority               INT2 NOT NULL,
    troubleshooting        BOOLEAN NOT NULL,
    processingtimewindow   INT2 NOT NULL,
    algorithm              VARCHAR(32) NOT NULL,
    filetype               VARCHAR(16) NOT NULL,
    active                 BOOLEAN DEFAULT 't' NOT NULL,
    fileclass              VARCHAR(8)
) ;
ALTER TABLE t_ordersgeneration
  ADD CONSTRAINT pk_t_ordersgeneration
  	PRIMARY KEY (id) ;



/* ------------------------------
	t_ordersinput
*/ ------------------------------
CREATE TABLE t_ordersinput (
    id SERIAL NOT NULL,
    filename VARCHAR(128) NOT NULL,
    fileversion VARCHAR(32) NOT NULL,
    orderid INTEGER NOT NULL
) ;
ALTER TABLE t_ordersinput
	ADD CONSTRAINT pk_t_ordersinput
		PRIMARY KEY (id) ;



/* ------------------------------
	t_invrollingpolicies
*/ ------------------------------
CREATE TABLE t_invrollingpolicies (
    id SERIAL NOT NULL,
    filetype VARCHAR(16) NOT NULL,
    latencyhours INTEGER NOT NULL,
    removeevendistribution BOOLEAN NOT NULL,
    removeevenused BOOLEAN NOT NULL,
    action INTEGER NOT NULL,
    algorithm INTEGER NOT NULL,
    parameters TEXT,
    priority INTEGER NOT NULL,
    fileclass VARCHAR(8),
	storageid INT,
	fetchrowslimit INTEGER,
	idlenumitems INTEGER,
	idlehours DOUBLE PRECISION,
	triggertime TIMESTAMP(6) NOT NULL DEFAULT NOW(),
	status TEXT
) ;
ALTER TABLE t_invrollingpolicies
	ADD CONSTRAINT pk_t_invrollingpolicies
		PRIMARY KEY (id) ;



/* ------------------------------
	t_filetypes
*/ ------------------------------
CREATE TABLE t_filetypes (
    id_filetype SERIAL NOT NULL,
	filetype VARCHAR(16) NOT NULL,
    description VARCHAR(64),
    nameregularexpression VARCHAR(128),
    headerseparation BOOLEAN DEFAULT 'f' NOT NULL,
    "group" VARCHAR(32) NOT NULL,
    invperiod INT,
	mngsngext BOOLEAN DEFAULT 't' NOT NULL,
	sngext VARCHAR(8),
	hdrext VARCHAR(8),
	dblext VARCHAR(8)
) ;
ALTER TABLE t_filetypes
	ADD CONSTRAINT pk_t_filetypes
		PRIMARY KEY (id_filetype) ;



/* ------------------------------
	t_ordersqueue
*/ ------------------------------
CREATE TABLE t_ordersqueue (
	orderid SERIAL NOT NULL,
	creationdate TIMESTAMP(6) NOT NULL,
	priority INT2 NOT NULL,
	status VARCHAR(32) NOT NULL,
	processorname VARCHAR(32) NOT NULL,
	processorversion VARCHAR(8) NOT NULL,
	processorconfiguration VARCHAR(8),
	troubleshooting BOOLEAN NOT NULL,
	starttime TIMESTAMP(6) NOT NULL,
	stoptime TIMESTAMP(6) NOT NULL,
	jobresponsible VARCHAR(64),
	productionstarttime TIMESTAMP(6),
	productionstoptime TIMESTAMP(6),
	processingtimewindow INT2 NOT NULL,
	processorid VARCHAR(16),
	ordertype VARCHAR(8) NOT NULL,
	externalorderid VARCHAR(128),
	satelliteid INT2,
	parameters TEXT,
	mission INT2,
	subreq INTEGER
) ;
ALTER TABLE t_ordersqueue
	ADD CONSTRAINT pk_t_ordersqueue
		PRIMARY KEY (orderid) ;



/* ------------------------------
	t_ordersqueueblacklist
*/ ------------------------------
CREATE TABLE t_ordersqueueblacklist (
    id SERIAL NOT NULL,
    orderid INTEGER NOT NULL,
	jobresponsible VARCHAR(64) NOT NULL
) ;
ALTER TABLE t_ordersqueueblacklist
	ADD CONSTRAINT pk_t_ordersqueueblacklist
		PRIMARY KEY (id) ;



/* ------------------------------
	t_orderwaittrigger
*/ ------------------------------
CREATE TABLE t_orderwaittrigger (
    triggertype VARCHAR(16) NOT NULL
) ;
ALTER TABLE t_orderwaittrigger
  ADD CONSTRAINT pk_t_orderwaittrigger
  	PRIMARY KEY (triggertype) ;



/* ------------------------------
	t_smtype
*/ ------------------------------
CREATE TABLE t_smtype(
    id   INT2 NOT NULL,
    type VARCHAR(16) NOT NULL
) ;
ALTER TABLE t_smtype
  ADD CONSTRAINT pk_t_smtype
  	PRIMARY KEY (id) ;



/* ------------------------------
	t_storagemanager
*/ ------------------------------
CREATE TABLE t_storagemanager (
	id SERIAL NOT NULL,
    type VARCHAR(16) NOT NULL,
    name VARCHAR(24) NOT NULL,
    description VARCHAR(64) NOT NULL,
    invrelationtable VARCHAR(32) NOT NULL,
    parameters TEXT,
	syncupload BOOLEAN DEFAULT 't' NOT NULL, 
	syncdownload BOOLEAN DEFAULT 't' NOT NULL,
	syncerase BOOLEAN DEFAULT 't' NOT NULL,
	allowdefaultcost BOOLEAN DEFAULT 't' NOT NULL,
	migrationalgo_id INTEGER,
	migrationalgo_par TEXT,
	error_recovery_period INTEGER DEFAULT 60
) ;
ALTER TABLE t_storagemanager
	ADD CONSTRAINT pk_t_storagemanager
		PRIMARY KEY (id) ;



/* ------------------------------
	t_inventorysm
*/ ------------------------------
CREATE TABLE t_inventorysm (
	id SERIAL NOT NULL,
	inv_id BIGINT,
	storagemanid INTEGER NOT NULL,
	packageid INTEGER,
	stored_date TIMESTAMP(6) DEFAULT now() NOT NULL,
	migration_report TEXT
) ;
ALTER TABLE t_inventorysm
	ADD CONSTRAINT pk_t_inventorysm
		PRIMARY KEY (id) ;



/* ------------------------------
	t_storagepriority
*/ ------------------------------
CREATE TABLE t_storagepriority (
    id INTEGER NOT NULL,
    filetype VARCHAR(16) NOT NULL,
    storecost INTEGER NOT NULL,
    retrievecost INTEGER NOT NULL,
    erasecost INTEGER NOT NULL,
	parameters TEXT
) ;
ALTER TABLE t_storagepriority
	ADD CONSTRAINT pk_t_storagepriority
		PRIMARY KEY (id, filetype) ;



/* ------------------------------
	t_inventorygaps
*/ ------------------------------
CREATE TABLE t_inventorygaps (
    inventorygapid SERIAL NOT NULL,
    filename VARCHAR(128) NOT NULL,
    fileversion VARCHAR(32) NOT NULL,
    begin TIMESTAMP(6) NOT NULL,
    "end" TIMESTAMP(6) NOT NULL
) ;
ALTER TABLE t_inventorygaps
	ADD CONSTRAINT pk_t_inventorygaps
		PRIMARY KEY (inventorygapid) ;



/* ------------------------------
	t_pendinginventory
*/ ------------------------------
CREATE TABLE t_pendinginventory (
    filename VARCHAR(128) NOT NULL,
    fileversion VARCHAR(32) NOT NULL
) ;
ALTER TABLE t_pendinginventory
  ADD CONSTRAINT pk_t_pendinginventory
  	PRIMARY KEY (filename, fileversion) ;



/* ------------------------------
	t_measdataset
*/ ------------------------------
CREATE TABLE t_measdataset (
    id SERIAL NOT NULL,
    name VARCHAR(64) NOT NULL
) ;
ALTER TABLE t_measdataset
	ADD CONSTRAINT pk_t_measdataset
		PRIMARY KEY (id) ;



/* ------------------------------
	t_inventorydataset
*/ ------------------------------
CREATE TABLE t_inventorydataset (
    filename      VARCHAR(128) NOT NULL,
    fileversion   VARCHAR(32) NOT NULL,
    measdatasetid INTEGER NOT NULL,
    count         INTEGER NOT NULL
) ;
ALTER TABLE t_inventorydataset
  ADD CONSTRAINT pk_t_inventorydataset
  	PRIMARY KEY (filename, fileversion, measdatasetid) ;



/* ------------------------------
	t_qualifiedflag
*/ ------------------------------
CREATE TABLE t_qualifiedflag (
    id   SERIAL NOT NULL,
    name VARCHAR(32) NOT NULL
) ;
ALTER TABLE t_qualifiedflag
  ADD CONSTRAINT pk_t_qualifiedflag
  	PRIMARY KEY (id) ;



/* ------------------------------
	t_originators
*/ ------------------------------
CREATE TABLE t_originators (
	id INT2 NOT NULL,
	name VARCHAR(64) NOT NULL
) ;
ALTER TABLE t_originators
	ADD CONSTRAINT pk_t_originators
		PRIMARY KEY (id) ;



/* ------------------------------
	t_ordtriggeringfiles
*/ ------------------------------
CREATE TABLE t_ordtriggeringfiles (
    id SERIAL NOT NULL,
    filename VARCHAR(128) NOT NULL,
    fileversion VARCHAR(32) NOT NULL,
    orderid INT NOT NULL
);
ALTER TABLE t_ordtriggeringfiles
	ADD CONSTRAINT pk_t_ordtriggeringfiles 
		PRIMARY KEY (id);



/* ------------------------------
	t_sitetype
*/ ------------------------------
CREATE TABLE t_sitetype (
    id SERIAL NOT NULL,
    name VARCHAR(32) NOT NULL
);
ALTER TABLE t_sitetype 
	ADD CONSTRAINT pk_t_sitetype 
		PRIMARY KEY (id) ;



/* ------------------------------
	t_sites
*/ ------------------------------
CREATE TABLE t_sites (
    id SERIAL NOT NULL,
    name VARCHAR(32) NOT NULL,
    parent INT,
    active BOOLEAN NOT NULL,
    type INT NOT NULL,
    details TEXT
);
ALTER TABLE t_sites 
	ADD CONSTRAINT pk_t_sites
		PRIMARY KEY (id) ;



/* ------------------------------
	t_invsiterelation
*/ ------------------------------
CREATE TABLE t_invsiterelation (
    id SERIAL NOT NULL,
    filename VARCHAR(128) NOT NULL,
    fileversion VARCHAR(32) NOT NULL,
    site INT NOT NULL
);
ALTER TABLE t_invsiterelation 
	ADD CONSTRAINT pk_t_invsiterelation
		PRIMARY KEY (id) ;



/* ------------------------------
	t_ordersattachmenttypes
*/ ------------------------------
CREATE TABLE t_ordersattachmenttypes (
    id SERIAL NOT NULL,
    name VARCHAR(16) NOT NULL
);
ALTER TABLE t_ordersattachmenttypes 
	ADD CONSTRAINT pk_t_ordersattachmenttypes
		PRIMARY KEY (id) ;



/* ------------------------------
	t_ordersattachments
*/ ------------------------------
CREATE TABLE t_ordersattachments (
    id SERIAL NOT NULL,
    orderid INT NOT NULL,
    attachtype INT NOT NULL,
    attachname VARCHAR(128),
    attachment OID
) ;
ALTER TABLE t_ordersattachments 
	ADD CONSTRAINT pk_t_ordersattachments
		PRIMARY KEY (id) ;



/* ------------------------------
	t_smactions
*/ ------------------------------
CREATE TABLE t_smactions (
    id SERIAL NOT NULL,
    inv_id BIGINT NOT NULL,
	filename VARCHAR(128) NOT NULL,
    fileversion VARCHAR(32) NOT NULL,
    unique_id VARCHAR(160) NOT NULL,
	smid INT NOT NULL,
    operation VARCHAR(16) NOT NULL,
    parameter OID,
    creationdate TIMESTAMP(6) DEFAULT NOW() NOT NULL,
    packageid INT,
	enddate TIMESTAMP(6) DEFAULT '3000-01-01 00:00:00'::TIMESTAMP(6)
) ;
ALTER TABLE t_smactions 
	ADD CONSTRAINT pk_t_smactions1
		PRIMARY KEY (id) ;



/* ------------------------------
	t_smqueue
*/ ------------------------------
CREATE TABLE t_smqueue (
    packageid SERIAL NOT NULL,
    status VARCHAR(16) NOT NULL,
    operation VARCHAR(16) NOT NULL,
    smid INT NOT NULL,
    creationdate TIMESTAMP(6) DEFAULT NOW() NOT NULL,
    parameter OID,
    startdate TIMESTAMP(6),
    stopdate TIMESTAMP(6),
    packagefilename VARCHAR(128),
	synchronous BOOLEAN,
	asynchifparams TEXT,
	extparams TEXT
) ;
ALTER TABLE t_smqueue 
	ADD CONSTRAINT pk_t_smqueue1 
		PRIMARY KEY (packageid) ;



/* ------------------------------
	t_pdsmodes
*/ ------------------------------
CREATE TABLE t_pdsmodes (
	id SERIAL NOT NULL,
	application VARCHAR(64) NOT NULL,
	subsystem VARCHAR(16) NOT NULL,
	mode VARCHAR(16) NOT NULL
);
ALTER TABLE t_pdsmodes
  ADD CONSTRAINT pk_t_pdsmodes
  	PRIMARY KEY (id) ;



/* ------------------------------
	t_stafinvrelation
*/ ------------------------------
CREATE TABLE t_stafinvrelation (
    id INT NOT NULL,
    remotepath VARCHAR(128) NOT NULL
);
ALTER TABLE t_stafinvrelation 
	ADD CONSTRAINT pk_t_stafinvrelation
		PRIMARY KEY (id) ;



/* ------------------------------
	t_inventoryeventsconf
*/ ------------------------------
CREATE TABLE t_inventoryeventsconf (
    id SERIAL NOT NULL,
    filetype VARCHAR(16) NOT NULL,
    conf TEXT
);
ALTER TABLE t_inventoryeventsconf 
	ADD CONSTRAINT pk_t_inventorye258
		PRIMARY KEY (id) ;



/* ------------------------------
	t_inventoryanomalies
*/ ------------------------------
CREATE TABLE t_inventoryanomalies (
    id SERIAL NOT NULL,
    filename VARCHAR(128) NOT NULL,
    fileversion VARCHAR(32) NOT NULL,
    tstamp TIMESTAMP(6) DEFAULT NOW() NOT NULL,
    message VARCHAR(256) NOT NULL,
	anomaly_type_id SMALLINT DEFAULT 1 NOT NULL
);
ALTER TABLE t_inventoryanomalies 
	ADD CONSTRAINT pk_t_inventorya266 
		PRIMARY KEY (id) ;



/* ------------------------------
	t_standingdistribution
*/ ------------------------------
CREATE TABLE t_standingdistribution (
    id SERIAL NOT NULL,
    distpoliciesid INTEGER NOT NULL,
    validitystart TIMESTAMP(6) NOT NULL,
    validitystop TIMESTAMP(6) NOT NULL,
    geographiclocalization VARCHAR(32000) NOT NULL,
    cartsid INTEGER,
    data TEXT,
	subrequest INTEGER NOT NULL
);
ALTER TABLE t_standingdistribution 
	ADD CONSTRAINT pk_standingdistribution 
		PRIMARY KEY (id) ;



/* ------------------------------
	t_storagegroup
*/ ------------------------------
CREATE TABLE t_storagegroup (
    id SERIAL NOT NULL,
    filetype VARCHAR(16) NOT NULL,
    storagemanager INT NOT NULL,
    groupname VARCHAR(64) NOT NULL
);
ALTER TABLE t_storagegroup 
	ADD CONSTRAINT pk_t_storagegroup 
		PRIMARY KEY (id);



/* ------------------------------
	t_postinvqueue
*/ ------------------------------
CREATE TABLE t_postinvqueue (
    id SERIAL NOT NULL,
    filename VARCHAR(128) NOT NULL,
	fileversion VARCHAR(32) NOT NULL,
	filetype VARCHAR(16) NOT NULL,
	validitystart TIMESTAMP(6) NOT NULL,
	validitystop TIMESTAMP(6) NOT NULL,
	tstamp TIMESTAMP(6) DEFAULT NOW() NOT NULL
);
ALTER TABLE t_postinvqueue 
	ADD CONSTRAINT pk_t_postinvqueue 
		PRIMARY KEY (id);



/* ------------------------------
	t_filetypes_x_migrations
*/ ------------------------------
CREATE TABLE t_filetypes_x_migrations (
	id_filetypemigration SERIAL NOT NULL,
	sm_id INTEGER NOT NULL,
	filetype_id INTEGER NOT NULL,
	migrationalgo_id INTEGER NOT NULL,
	parameters TEXT
) ;
ALTER TABLE t_filetypes_x_migrations
	ADD CONSTRAINT pk_t_filetypes_x_migrations
		PRIMARY KEY (id_filetypemigration) ;



/* ------------------------------
	t_filetypes_x_satellites
*/ ------------------------------
CREATE TABLE t_filetypes_x_satellites (
	id SERIAL NOT NULL,
	filetype VARCHAR(16) NOT NULL,
	satellite_id INT2 NOT NULL
) ;
ALTER TABLE t_filetypes_x_satellites 
	ADD CONSTRAINT pk_filetypes_x_satellites 
		PRIMARY KEY (id);



------------------------------
-- t_transferprotocols
------------------------------
CREATE TABLE t_transferprotocols (
	id INT2 NOT NULL,
	protocol VARCHAR(16) NOT NULL
) ;
ALTER TABLE t_transferprotocols 
	ADD CONSTRAINT pk_t_transferprotocols
		PRIMARY KEY (id);



/* --------------------------
	t_inv_relation_types
*/ --------------------------
CREATE TABLE t_inv_relation_types (
	id SMALLINT NOT NULL,
	relation_type VARCHAR(24) NOT NULL
) ;
ALTER TABLE t_inv_relation_types 
	ADD CONSTRAINT pk_t_inv_relation_types
		PRIMARY KEY (id) ;



/* --------------------------
	t_inv_relations
*/ --------------------------
CREATE TABLE t_inv_relations (
	id BIGSERIAL NOT NULL,
	inv_id1 BIGINT NOT NULL,
	inv_id2 BIGINT,
	rel_type_id SMALLINT NOT NULL,
	relation VARCHAR(256)
) ;
ALTER TABLE t_inv_relations 
	ADD CONSTRAINT pk_t_inv_relations
		PRIMARY KEY (id) ;



/* --------------------------
	t_filetypes_last_inv
*/ --------------------------
CREATE TABLE t_filetypes_last_inv (
	id SERIAL NOT NULL,
	filetype VARCHAR(16) NOT NULL,
	last_valid TIMESTAMP(6) DEFAULT NOW() NOT NULL
) ;
ALTER TABLE t_filetypes_last_inv 
	ADD CONSTRAINT pk_t_filetypes_last_inv
		PRIMARY KEY (id) ;



/* ------------------------------
	t_receptionrules
*/ ------------------------------
CREATE TABLE t_receptionrules (
    id_receptionrule SERIAL NOT NULL,
    filetype_id INTEGER NOT NULL,
	repository_id INTEGER NOT NULL,
	user_id INTEGER NOT NULL,
	usermode_id INT2 NOT NULL,
	priority INTEGER DEFAULT 1 NOT NULL,
	active BOOLEAN DEFAULT 't' NOT NULL,
	delete_on_remote BOOLEAN DEFAULT 'f' NOT NULL,
	generateheader BOOLEAN DEFAULT 'f' NOT NULL,
	reception_tt_id INTEGER DEFAULT f_get_tasktable_id('defaultReception') NOT NULL,
	tt_parameters TEXT,
	hostname VARCHAR(256),
	localpath VARCHAR(1024) NOT NULL,
	connretries INT2 DEFAULT 1 NOT NULL,
	pollingperiod_sec INTEGER NOT NULL,
	retryperiod_sec INTEGER NOT NULL,
	connfailureaddress VARCHAR(1024),
	procfailureaddress VARCHAR(1024),
	lastvisit TIMESTAMP DEFAULT NOW() NOT NULL,
	lastgood TIMESTAMP DEFAULT NOW() NOT NULL,
	retrycounter INT2 DEFAULT 0 NOT NULL,
	discoveryplugin_id INTEGER NOT NULL,
	successnotificationaddress VARCHAR(1024),
	php_timeout_sec INTEGER,
	satellite_id INT2,
	mission INT2,
	sensor_id INT2,
	timewindow_sec INTEGER
) ;
ALTER TABLE t_receptionrules
	ADD CONSTRAINT pk_t_receptionrules
		PRIMARY KEY (id_receptionrule) ;



/* ------------------------------
	t_receptionruleshist
*/ ------------------------------
CREATE TABLE t_receptionruleshist (
    id_receptionrulehist SERIAL NOT NULL,
	receptionrule_id INTEGER NOT NULL,
	filename VARCHAR(1024) NOT NULL,
	tstamp TIMESTAMP NOT NULL,
	status_id INT2 NOT NULL,
	filecreationtstamp TIMESTAMP(6),
	remoteuuid VARCHAR(160),
	evictiondate TIMESTAMP(6)
) ;
ALTER TABLE t_receptionruleshist
	ADD CONSTRAINT pk_t_receptionruleshist
		PRIMARY KEY (id_receptionrulehist) ;



/* ------------------------------
	t_receptionruleslock
*/ ------------------------------
CREATE TABLE t_receptionruleslock (
    receptionrule_id INTEGER NOT NULL,
    responsible VARCHAR(128) NOT NULL,
    tstamp TIMESTAMP(6) DEFAULT NOW() NOT NULL,
	processidentifier VARCHAR(128)
) ;
ALTER TABLE t_receptionruleslock
	ADD CONSTRAINT pk_t_receptionruleslock
		PRIMARY KEY (receptionrule_id) ;



/* ------------------------------
	t_repositories
*/ ------------------------------
CREATE TABLE t_repositories (
    id_repository SERIAL NOT NULL,
	symbolicname VARCHAR(64) NOT NULL,
	transferprotocol_id INTEGER NOT NULL,
	host_id INTEGER NOT NULL,
	username VARCHAR(32) NOT NULL,
	password VARCHAR(256),
	remotepath VARCHAR(1024) NOT NULL,
	tempname_id INT2 DEFAULT 1 NOT NULL,
	tempname_value VARCHAR(64) NOT NULL,
	allowedciphers VARCHAR(256),
	port SMALLINT,
	attributes JSONB
) ;
ALTER TABLE t_repositories
	ADD CONSTRAINT pk_t_repositories
		PRIMARY KEY (id_repository) ;



/* ------------------------------
	t_satellites
*/ ------------------------------
CREATE TABLE t_satellites ( 
    satelliteid INT2 NOT NULL,
	satelliteacronym VARCHAR(4) NOT NULL,
    satellitename VARCHAR(64) NOT NULL
) ;
ALTER TABLE t_satellites
	ADD CONSTRAINT pk_t_satellites
		PRIMARY KEY (satelliteid) ;



/* ------------------------------
	t_satrelations
*/ ------------------------------
CREATE TABLE t_satrelations ( 
    satelliteid INT2 NOT NULL,
    mission INT2 NOT NULL,
    sensorid INT2 NOT NULL
) ;
ALTER TABLE t_satrelations
	ADD CONSTRAINT pk_t_satrelations
		PRIMARY KEY (satelliteid,mission,sensorid) ;



/* ------------------------------
	t_sensors
*/ ------------------------------
CREATE TABLE t_sensors ( 
    sensorid INT2 NOT NULL,
    sensorname VARCHAR(64) NOT NULL,
	description VARCHAR(128)
) ;
ALTER TABLE t_sensors
	ADD CONSTRAINT pk_t_sensors
		PRIMARY KEY (sensorid) ;



/* ------------------------------
	t_serials
*/ ------------------------------
CREATE TABLE t_serials (
    table_name VARCHAR(30) NOT NULL,
	field_name VARCHAR(30) NOT NULL
) ;
ALTER TABLE t_serials
	ADD CONSTRAINT pk_t_serials
		PRIMARY KEY (table_name, field_name) ;



/* ------------------------------
	t_sor_requests
*/ ------------------------------
CREATE TABLE t_sor_requests (
	id_sor_request SERIAL NOT NULL,
	spacecraftid VARCHAR(24),
	requestid CHAR(8) NOT NULL,
	raiseddate TIMESTAMP(6) DEFAULT now() NOT NULL,
	originator VARCHAR(24),
	status VARCHAR(24) NOT NULL,
	statusupdated TIMESTAMP(6) DEFAULT now() NOT NULL,
	emailtitle VARCHAR(128) NOT NULL,
	emailbody TEXT
) ;
ALTER TABLE t_sor_requests
	ADD CONSTRAINT pk_t_sor_requests
		PRIMARY KEY (id_sor_request) ;



/* ------------------------------
	t_storagerollingthresholds 
*/ ------------------------------
CREATE TABLE t_storagerollingthresholds (
	storagemanager_id INTEGER NOT NULL,
	threshold_mb BIGINT NOT NULL,
	action_id INTEGER NOT NULL
) ;
ALTER TABLE t_storagerollingthresholds
	ADD CONSTRAINT pk_t_storagerollingthresholds
		PRIMARY KEY (storagemanager_id) ;



/* ------------------------------
	t_tasktables
*/ ------------------------------
CREATE TABLE t_tasktables (
    id_tasktable SERIAL NOT NULL,
	name VARCHAR(64) NOT NULL,
	tasktable TEXT,
	type_id INT2,
	description VARCHAR(256)
) ;
ALTER TABLE t_tasktables
	ADD CONSTRAINT pk_t_tasktables
		PRIMARY KEY (id_tasktable) ;



/* ------------------------------
	t_temporarydistribution
*/ ------------------------------
CREATE TABLE t_temporarydistribution (
    id SERIAL NOT NULL,
    duedate TIMESTAMP(6) NOT NULL,
    distributeddate TIMESTAMP(6),
    creationdate TIMESTAMP(6) DEFAULT NOW(),
    filename VARCHAR(128) NOT NULL,
    fileversion VARCHAR(32) NOT NULL,
    mediatype VARCHAR(16) NOT NULL,
    userid VARCHAR(32) NOT NULL,
    format INTEGER,
    algo INTEGER NOT NULL,
    tstamp TIMESTAMP(6) DEFAULT NOW(),
	policy INTEGER NOT NULL,
    orderid INTEGER,
    preselection BOOLEAN DEFAULT 'f' NOT NULL,
	cartid INTEGER,
	subreq INTEGER,
	disseminationpackingalgo_id INTEGER NOT NULL
) ;
ALTER TABLE t_temporarydistribution
	ADD CONSTRAINT pk_t_temporarydistribution
		PRIMARY KEY (id) ;



/* ------------------------------
	t_tmpnamestrategy
*/ ------------------------------
CREATE TABLE t_tmpnamestrategy (
    id_tmpnamestrategy INT2 NOT NULL,
	tempname VARCHAR(24) NOT NULL
) ;
ALTER TABLE t_tmpnamestrategy 
	ADD CONSTRAINT pk_t_tmpnamestrategy
		PRIMARY KEY (id_tmpnamestrategy) ;



/* ------------------------------
	t_usermodes
*/ ------------------------------
CREATE TABLE t_usermodes (
    id INT2 NOT NULL,
    mode VARCHAR(16) NOT NULL
);
ALTER TABLE t_usermodes 
	ADD CONSTRAINT pk_t_usermodes 
		PRIMARY KEY (id);



/* ------------------------------
	t_users
*/ ------------------------------
CREATE TABLE t_users (
    id_user SERIAL NOT NULL,
	userid VARCHAR(32) NOT NULL,
    firstname VARCHAR(32),
    lastname VARCHAR(32),
    title VARCHAR(8),
    organization VARCHAR(32),
    department VARCHAR(32),
    fax VARCHAR(24),
    phone VARCHAR(24),
    postaladdress VARCHAR(128),
    projectandprocessing VARCHAR(128),
    packagerequired BOOLEAN NOT NULL,
    compresspackage BOOLEAN,
    compressionmode VARCHAR(16),
    usertype VARCHAR(12) NOT NULL,
    packagemaxsize INTEGER,
    progmedianumber INTEGER NOT NULL,
    emailaddress VARCHAR(64),
    allqualified BOOLEAN DEFAULT 'f' NOT NULL,
	mode INT2 DEFAULT 1 NOT NULL
) ;
ALTER TABLE t_users
	ADD CONSTRAINT pk_t_users
		PRIMARY KEY (id_user) ;



/* ------------------------------
	t_usertypes
*/ ------------------------------
CREATE TABLE t_usertypes (
    usertype    VARCHAR(12) NOT NULL,
    description VARCHAR(64) NOT NULL
) ;
ALTER TABLE t_usertypes
  ADD CONSTRAINT pk_t_usertypes
  	PRIMARY KEY (usertype) ;



/* ------------------------------
	t_waitingorders
*/ ------------------------------
CREATE TABLE t_waitingorders (
    waitorderid SERIAL NOT NULL,
    orderid     INTEGER NOT NULL,
    filetype    VARCHAR(16) NOT NULL,
    starttime   TIMESTAMP(6) NOT NULL,
    stoptime    TIMESTAMP(6) NOT NULL,
    triggertype VARCHAR(16) NOT NULL,
	generationdate TIMESTAMP(6) DEFAULT NOW() NOT NULL,
	expirationdate TIMESTAMP(6)
) ;
ALTER TABLE t_waitingorders
  ADD CONSTRAINT pk_t_waitingorders
  	PRIMARY KEY (waitorderid) ;


