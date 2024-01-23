/*

Copyright 2010-2020, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.it A.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S. Databases schema tables script $

	$Id$

	$Author$
	
*/





--------------------------------------------------------------------------------
--
--    	  	  	  	  	  	  	  TABLES
--
--------------------------------------------------------------------------------


/* ------------------------------
	t_blockedcirculationsqueue
*/ ------------------------------
CREATE TABLE t_blockedcirculationsqueue (
    id BIGSERIAL NOT NULL,
    startdate TIMESTAMP(6),
	enddate TIMESTAMP(6),
	priority INTEGER DEFAULT 0 NOT NULL,
	remark VARCHAR(512),
	status_id INT2 DEFAULT 16 NOT NULL,
	creationdate TIMESTAMP(6) DEFAULT NOW(),
	modificationdate TIMESTAMP(6) DEFAULT NOW(),
	fromurl VARCHAR(512) NOT NULL,
	tourl VARCHAR(512) NOT NULL,
	relay_host_id INTEGER,
	relay_activity_id BIGINT,
	retries INT2 DEFAULT 0 NOT NULL,
	removesource BOOLEAN DEFAULT 'f' NOT NULL,
	circulationmode_id SMALLINT NOT NULL,
	host_id INTEGER,
	circulationpolicy_id INTEGER,
	dest_host_id INTEGER,
	error VARCHAR(512),
	tempnamevalue VARCHAR(64),
	tempname_id SMALLINT,
	nextretrydate TIMESTAMP(6) DEFAULT NOW(),
	dest_protocol_id SMALLINT,
	media_circulation_id INTEGER,
	media_insertionmode_id SMALLINT DEFAULT 1 NOT NULL,
	localtransfermode_id SMALLINT DEFAULT 1 NOT NULL
) ;
ALTER TABLE t_blockedcirculationsqueue 
	ADD CONSTRAINT pk_t_blockedcirculationsqueue
		PRIMARY KEY (id) ;



/* ------------------------------
	t_circ_callbacks
*/ ------------------------------
CREATE TABLE t_circ_callbacks (
	id SERIAL NOT NULL,
	circ_queue_id BIGINT NOT NULL,
	url VARCHAR(512) NOT NULL,
	external_id BIGINT NOT NULL
) ;
ALTER TABLE t_circ_callbacks 
	ADD CONSTRAINT pk_t_circ_callbacks
		PRIMARY KEY (id) ;



/* ------------------------------
	t_circulationactivehosts
*/ ------------------------------
CREATE TABLE t_circulationactivehosts (
	host_id INTEGER NOT NULL,
	tstamp TIMESTAMP(6) DEFAULT NOW() NOT NULL
) ;
ALTER TABLE t_circulationactivehosts 
	ADD CONSTRAINT pk_t_circulationactivehosts
		PRIMARY KEY (host_id) ;



/* ------------------------------
	t_circulationmodes
*/ ------------------------------
CREATE TABLE t_circulationmodes (
	id SMALLINT NOT NULL,
	circulationmode VARCHAR(16) NOT NULL
) ;
ALTER TABLE t_circulationmodes 
	ADD CONSTRAINT pk_t_circulationmodes
		PRIMARY KEY (id) ;



/* ------------------------------
	t_circulationpolicies
*/ ------------------------------
CREATE TABLE t_circulationpolicies (
    id SERIAL NOT NULL,
	filetype_id INTEGER NOT NULL,
	circulator_node_id INTEGER NOT NULL,
	priority INTEGER NOT NULL DEFAULT 0,
	source_id INTEGER NOT NULL,
	destination_id INTEGER NOT NULL,
	removesource BOOLEAN DEFAULT 'f' NOT NULL,
	agent_node_id INTEGER,
	localtransfermode_id SMALLINT,
	status_id SMALLINT DEFAULT 43 NOT NULL,
	circ_policy_name VARCHAR(64)
) ;
ALTER TABLE t_circulationpolicies 
	ADD CONSTRAINT pk_t_circulationpolicies
		PRIMARY KEY (id) ;



/* ------------------------------
   t_circulationpolicies_monitor
*/ ------------------------------
CREATE TABLE t_circulationpolicies_monitor (
	id SERIAL NOT NULL,
	circulationpolicy_id INTEGER NOT NULL,
	monitoringflag BOOLEAN NOT NULL
);
ALTER TABLE t_circulationpolicies_monitor 
	ADD CONSTRAINT pk_t_circulationpolicies_monitor 
		PRIMARY KEY(id);



/* ------------------------------
	t_circulation_remove_queue
*/ ------------------------------
CREATE TABLE t_circulation_remove_queue (
    id BIGSERIAL NOT NULL,
	url VARCHAR(512) NOT NULL,
	host_id INTEGER
) ;
ALTER TABLE t_circulation_remove_queue 
	ADD CONSTRAINT pk_t_circulation_remove_queue
		PRIMARY KEY (id) ; 



/* ------------------------------
	t_circulationsmedia
*/ ------------------------------
CREATE TABLE t_circulationsmedia (
	id SERIAL NOT NULL,
	media_tag VARCHAR(128) NOT NULL,
	status_id SMALLINT NOT NULL,
	tstamp TIMESTAMP(6) DEFAULT now() NOT NULL,
	mount_point VARCHAR(1024) NOT NULL,
	perc_copied_files INTEGER DEFAULT 0 NOT NULL,
	source BOOLEAN DEFAULT 't' NOT NULL
) ;
ALTER TABLE t_circulationsmedia 
	ADD CONSTRAINT pk_t_circulationsmedia
		PRIMARY KEY (id) ;



/* ------------------------------
	t_circulationsmedia_insmode 
*/ ------------------------------
CREATE TABLE t_circulationsmedia_insmode (
	id SMALLINT NOT NULL,
	mode VARCHAR(24) NOT NULL
) ;
ALTER TABLE t_circulationsmedia_insmode 
	ADD CONSTRAINT pk_t_circulationsmedia_insmode
		PRIMARY KEY (id) ;



/* ------------------------------
	t_circulationsqueue
*/ ------------------------------
CREATE TABLE t_circulationsqueue (
    id BIGSERIAL NOT NULL,
    startdate TIMESTAMP(6),
	enddate TIMESTAMP(6),
	priority INTEGER DEFAULT 0 NOT NULL,
	remark VARCHAR(512),
	status_id INT2 DEFAULT 16 NOT NULL,
	creationdate TIMESTAMP(6) DEFAULT NOW(),
	modificationdate TIMESTAMP(6) DEFAULT NOW(),
	fromurl VARCHAR(512) NOT NULL,
	tourl VARCHAR(512) NOT NULL,
	relay_host_id INTEGER,
	relay_activity_id BIGINT,
	retries INT2 DEFAULT 0 NOT NULL,
	removesource BOOLEAN DEFAULT 'f' NOT NULL,
	circulationmode_id SMALLINT NOT NULL,
	host_id INTEGER,
	circulationpolicy_id INTEGER,
	dest_host_id INTEGER,
	error VARCHAR(512),
	tempnamevalue VARCHAR(64),
	tempname_id SMALLINT,
	nextretrydate TIMESTAMP(6) DEFAULT NOW(),
	dest_protocol_id SMALLINT,
	media_circulation_id INTEGER,
	media_insertionmode_id SMALLINT DEFAULT 1 NOT NULL,
	localtransfermode_id SMALLINT DEFAULT 1 NOT NULL,
	streamer_id INTEGER
) ;
ALTER TABLE t_circulationsqueue 
	ADD CONSTRAINT pk_t_circulationsqueue
		PRIMARY KEY (id) ;



/* ------------------------------
	t_circulationsqueuedwh
*/ ------------------------------
CREATE TABLE t_circulationsqueuedwh (
    id BIGSERIAL NOT NULL,
    startdate TIMESTAMP(6),
	enddate TIMESTAMP(6),
	priority INTEGER DEFAULT 0 NOT NULL,
	remark VARCHAR(512),
	status_id INT2 DEFAULT 16 NOT NULL,
	creationdate TIMESTAMP(6) DEFAULT NOW(),
	modificationdate TIMESTAMP(6) DEFAULT NOW(),
	fromurl VARCHAR(512) NOT NULL,
	tourl VARCHAR(512) NOT NULL,
	relay_host_id INTEGER,
	relay_activity_id BIGINT,
	retries INT2 DEFAULT 0 NOT NULL,
	removesource BOOLEAN DEFAULT 'f' NOT NULL,
	circulationmode_id SMALLINT NOT NULL,
	host_id INTEGER,
	circulationpolicy_id INTEGER,
	dest_host_id INTEGER,
	error VARCHAR(512),
	total_time DOUBLE PRECISION,
	datasize BIGINT,
	circulation_id BIGINT DEFAULT 0 NOT NULL,
	source_db_host VARCHAR(256) DEFAULT 'UNKNOWN' NOT NULL
) ;
ALTER TABLE t_circulationsqueuedwh 
	ADD CONSTRAINT pk_t_circulationsqueuedwh
		PRIMARY KEY (id) ;



/* ------------------------------
	t_circulationstats
*/ ------------------------------
CREATE TABLE t_circulationstats (
  id BIGSERIAL NOT NULL,
  creationdate TIMESTAMP(6) DEFAULT now(),
  from_host_id INTEGER,
  to_host_id INTEGER,
  name_lookup_time DOUBLE PRECISION,
  connection_time DOUBLE PRECISION,
  app_connection_time DOUBLE PRECISION,
  pre_transf_time DOUBLE PRECISION,
  start_transf_time DOUBLE PRECISION,
  total_time DOUBLE PRECISION,
  redirect_time DOUBLE PRECISION,
  circulationsqueue_id BIGINT,
  datasize BIGINT,
  unkn_from_host VARCHAR(256),
  unkn_to_host VARCHAR(256)
) ;
ALTER TABLE t_circulationstats 
	ADD CONSTRAINT pk_t_circulationstats
		PRIMARY KEY (id) ;



/* ------------------------------
	t_datastoragequeue
*/ ------------------------------
CREATE TABLE t_datastoragequeue (
	id BIGSERIAL NOT NULL,
	status_id INT2 NOT NULL,
	type_id INT2 NOT NULL,
	client_name VARCHAR(256),
	md5 CHAR(32),
	startdate TIMESTAMP(6) DEFAULT NOW(),
	enddate TIMESTAMP(6),
	check_date TIMESTAMP(6) DEFAULT now() NOT NULL,
	flowtag_id INT DEFAULT 1 NOT NULL
) ;
ALTER TABLE t_datastoragequeue 
	ADD CONSTRAINT pk_t_datastoragequeue
		PRIMARY KEY (id) ;



/* ------------------------------
	t_datastoragetypes
*/ ------------------------------
CREATE TABLE t_datastoragetypes (
	id INT2 NOT NULL,
	datastoragetype VARCHAR(16) NOT NULL
) ;
ALTER TABLE t_datastoragetypes 
	ADD CONSTRAINT pk_t_datastoragetypes
		PRIMARY KEY (id) ;



/* ------------------------------
	t_dsqueue_x_cirqueue
*/ ------------------------------
CREATE TABLE t_dsqueue_x_cirqueue (
	id BIGSERIAL NOT NULL,
	datastoragequeue_id BIGINT NOT NULL,
	circulation_id BIGINT,
	inv_filename VARCHAR(128) NOT NULL,
	error VARCHAR(512),
	parent VARCHAR(128),
	globalstatus_id INT2 DEFAULT 7 NOT NULL,
	priority INTEGER DEFAULT 0 NOT NULL,
	tourl VARCHAR(512),
	fromurl VARCHAR(512),
	circ_status_id INT2,
	circ_status_mod_date TIMESTAMP(6) DEFAULT NOW() NOT NULL,
	localtransfermode_id SMALLINT DEFAULT 1 NOT NULL,
	filequeue_id BIGSERIAL
) ;
ALTER TABLE t_dsqueue_x_cirqueue 
	ADD CONSTRAINT pk_t_dsqueue_x_cirqueue
		PRIMARY KEY (id) ;



/* ------------------------------
	t_flowtags
*/ ------------------------------
CREATE TABLE t_flowtags (
	id serial NOT NULL, 
	flowtagname VARCHAR(32) NOT NULL
);
ALTER TABLE t_flowtags 
	ADD CONSTRAINT pk_t_flowtags 
		PRIMARY KEY (id);



/* ------------------------------
	t_knownhosts_limits
*/ ------------------------------
CREATE TABLE t_knownhosts_limits (
	id SERIAL NOT NULL,
	circulator_node_id INTEGER NOT NULL,
	dest_host_id INTEGER NOT NULL,
	max_upload_slots INTEGER NOT NULL
) ;
ALTER TABLE t_knownhosts_limits 
	ADD CONSTRAINT pk_t_knownhosts_limits
		PRIMARY KEY (id) ;



/* ------------------------------
	t_securityzones
*/ ------------------------------
CREATE TABLE t_securityzones (
	id SERIAL NOT NULL,
	ip_cidr VARCHAR(32) NOT NULL,
	level INTEGER NOT NULL,
	securityzone VARCHAR(32)
) ;
ALTER TABLE t_securityzones 
	ADD CONSTRAINT pk_t_securityzones
		PRIMARY KEY (id) ;



/* ------------------------------
	t_securityzonerelays
*/ ------------------------------
CREATE TABLE t_securityzonerelays (
	id SERIAL NOT NULL,
	host_id INTEGER NOT NULL,
	seczone_id INTEGER NOT NULL
) ;
ALTER TABLE t_securityzonerelays 
	ADD CONSTRAINT pk_t_securityzonerelays
		PRIMARY KEY (id) ;



/* ------------------------------
	t_streamers
*/ ------------------------------
CREATE TABLE t_streamers (
    id BIGSERIAL NOT NULL,
	streamtype VARCHAR(16) NOT NULL
) ;
ALTER TABLE t_streamers 
	ADD CONSTRAINT pk_t_streamers
		PRIMARY KEY (id) ;


