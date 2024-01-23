
-- Version 11.0.0
-- POSTGRES










---------------------------------------------------------------------------------
------------------------------ FUNCTIONS CREATION -------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	f_get_tasktable_id
*/ ------------------------------
CREATE OR REPLACE FUNCTION f_get_tasktable_id(tt_name VARCHAR(64)) RETURNS INT AS $$
DECLARE
	res INT ;
BEGIN

  	res := (SELECT id_tasktable FROM t_tasktables WHERE name = tt_name) ;
  	RETURN res ;

	EXCEPTION WHEN OTHERS THEN
	   RETURN -1 ;

END ;
$$ LANGUAGE plpgsql ;


/* ------------------------------
	f_compare_int_values
*/ ------------------------------
CREATE OR REPLACE FUNCTION f_compare_int_values(val1 INT, val2 INT) RETURNS BOOLEAN AS $$
BEGIN
	IF (val1 IS NULL AND val2 IS NULL) THEN
		RETURN TRUE ;
	ELSIF (val1 = val2) THEN
		RETURN TRUE ;
	ELSE
		RETURN FALSE ;
	END IF ;
END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
------------------------------ TABLES MODIFICATION ------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_configurationstylesheets
*/ ------------------------------
ALTER TABLE t_configurationstylesheets ADD id SERIAL NOT NULL ;
ALTER TABLE t_configurationstylesheets DROP CONSTRAINT pk_t_configurationstylesheets ;
ALTER TABLE t_configurationstylesheets 
	ADD CONSTRAINT pk_t_configurationstylesheets
		PRIMARY KEY (id) ;
ALTER TABLE t_configurationstylesheets 
	ADD CONSTRAINT uq_configurationstylesheets01
		UNIQUE (name, version) ;


/* ------------------------------
	t_inventory
*/ ------------------------------
CREATE INDEX ix_t_inventory13 ON t_inventory USING BTREE (filename) ;


/* ------------------------------
	t_inv_relations
*/ ------------------------------
ALTER TABLE t_inv_relations ALTER COLUMN rel_type_id DROP DEFAULT ;


/* ------------------------------
	t_distributionqueue
*/ ------------------------------
ALTER TABLE t_distributionqueue ADD retrycounter INTEGER DEFAULT 0 NOT NULL ;
ALTER TABLE t_distributionqueue ADD delserviceparam TEXT ;
ALTER TABLE t_distributionqueue DROP COLUMN format CASCADE ;
ALTER TABLE t_distributionqueue ADD format INTEGER ;
ALTER TABLE t_distributionqueue ADD lastattempt TIMESTAMP(6) ;
 

/* ------------------------------
	t_distributionitems
*/ ------------------------------
ALTER TABLE t_distributionitems ADD distributionrule_id INTEGER NOT NULL ;
ALTER TABLE t_distributionitems DROP COLUMN format CASCADE ;
ALTER TABLE t_distributionitems ADD format INTEGER ;
ALTER TABLE t_distributionitems ADD disseminationpackingalgo_id INTEGER DEFAULT 1 NOT NULL;
ALTER TABLE t_distributionitems ALTER COLUMN disseminationpackingalgo_id DROP DEFAULT ;


/* ------------------------------
	t_ondemanddistribution
*/ ------------------------------
ALTER TABLE t_ondemanddistribution DROP COLUMN format CASCADE ;
ALTER TABLE t_ondemanddistribution ADD format INTEGER ;
ALTER TABLE t_ondemanddistribution ADD disseminationpackingalgo_id INTEGER DEFAULT 1 NOT NULL;
ALTER TABLE t_ondemanddistribution ALTER COLUMN disseminationpackingalgo_id DROP DEFAULT ;


/* ------------------------------
	t_temporarydistribution
*/ ------------------------------
ALTER TABLE t_temporarydistribution DROP COLUMN format CASCADE ;
ALTER TABLE t_temporarydistribution ADD format INTEGER ;
ALTER TABLE t_temporarydistribution ADD disseminationpackingalgo_id INTEGER DEFAULT 1 NOT NULL;
ALTER TABLE t_temporarydistribution ALTER COLUMN disseminationpackingalgo_id DROP DEFAULT ;


/* ------------------------------
	t_fileclasses
*/ ------------------------------
-- DROP DEPENDING OBJECTS
ALTER TABLE t_inventory DROP CONSTRAINT fk_t_inventory1 ; 
ALTER TABLE t_ordersgeneration DROP CONSTRAINT fk_t_ordersgeneration5 ;
ALTER TABLE t_invrollingpolicies DROP CONSTRAINT fk_t_invrollingpolicies4 ;

-- CHANGING TABLE COLUMNS
ALTER TABLE t_fileclasses ADD id_fileclass SERIAL NOT NULL ;

ALTER TABLE t_fileclasses DROP CONSTRAINT pk_t_fileclasses ;
ALTER TABLE t_fileclasses
	ADD CONSTRAINT pk_t_fileclasses
		PRIMARY KEY (id_fileclass) ;

ALTER TABLE t_fileclasses 
	ADD CONSTRAINT uq_fileclasses01
		UNIQUE (fileclass) ;

ALTER TABLE t_fileclasses DROP CONSTRAINT uq_fileclasses ;

-- RECREATE DEPENDING OBJECTS
ALTER TABLE t_inventory
	ADD CONSTRAINT fk_t_inventory1
		FOREIGN KEY (fileclass)
			REFERENCES t_fileclasses (fileclass)
				ON DELETE CASCADE ;
ALTER TABLE t_ordersgeneration
	ADD CONSTRAINT fk_t_ordersgeneration5
		FOREIGN KEY (fileclass)
			REFERENCES t_fileclasses (fileclass)
				ON DELETE CASCADE ;
ALTER TABLE t_invrollingpolicies
	ADD CONSTRAINT fk_t_invrollingpolicies4
		FOREIGN KEY (fileclass)
			REFERENCES t_fileclasses (fileclass)
				ON DELETE CASCADE ;


/* ------------------------------
	t_mediatypes
*/ ------------------------------

-- DROP DEPENDING OBJECTS
ALTER TABLE t_distributionitems DROP CONSTRAINT fk_t_distributionitems2 ;
ALTER TABLE t_ondemanddistribution DROP CONSTRAINT fk_ondemand_mediatype ;
ALTER TABLE t_mediacapformat DROP CONSTRAINT fk_t_mediacapformat ;
ALTER TABLE t_distributionpolicies DROP CONSTRAINT fk_t_distributionpolicies2 ;
ALTER TABLE t_temporarydistribution DROP CONSTRAINT fk_t_temporarydistribution2 ;

-- CHANGING TABLE COLUMNS
ALTER TABLE t_mediatypes ADD id_mediatype SERIAL NOT NULL ;
ALTER TABLE t_mediatypes ALTER COLUMN id_mediatype TYPE INT2 ;
DROP SEQUENCE IF EXISTS t_mediatypes_id_mediatype_seq CASCADE ;

UPDATE t_mediatypes SET id_mediatype = 1 WHERE mediatype = 'CD' ;
UPDATE t_mediatypes SET id_mediatype = 2 WHERE mediatype = 'DVD' ;
UPDATE t_mediatypes SET id_mediatype = 3 WHERE mediatype = 'FTP' ;
UPDATE t_mediatypes SET id_mediatype = 4 WHERE mediatype = 'LTO' ;

ALTER TABLE t_mediatypes DROP CONSTRAINT pk_t_mediatypes ;
ALTER TABLE t_mediatypes
	ADD CONSTRAINT pk_t_mediatypes
		PRIMARY KEY (id_mediatype) ;

ALTER TABLE t_mediatypes 
	ADD CONSTRAINT uq_mediatypes01
		UNIQUE (mediatype) ;

-- RECREATE DEPENDING OBJECTS
ALTER TABLE t_distributionitems
	ADD CONSTRAINT fk_t_distributionitems2
		FOREIGN KEY (mediatype)
			REFERENCES t_mediatypes (mediatype)
				ON DELETE CASCADE ;
ALTER TABLE t_ondemanddistribution 
	ADD CONSTRAINT fk_t_ondemanddistribution4 
		FOREIGN KEY (mediatype)
			REFERENCES t_mediatypes (mediatype) 
				ON DELETE CASCADE ;
ALTER TABLE t_mediacapformat
	ADD CONSTRAINT fk_t_mediacapformat1
		FOREIGN KEY (mediatype)
			REFERENCES t_mediatypes (mediatype)
				ON DELETE CASCADE ;
ALTER TABLE t_distributionpolicies
	ADD CONSTRAINT fk_t_distributionpolicies2
		FOREIGN KEY (mediatype)
			REFERENCES t_mediatypes (mediatype)
				ON DELETE CASCADE ;
ALTER TABLE t_temporarydistribution
	ADD CONSTRAINT fk_t_temporarydistribution2
		FOREIGN KEY (mediatype)
			REFERENCES t_mediatypes	(mediatype)
				ON DELETE CASCADE ;


/* ------------------------------
	t_users
*/ ------------------------------

-- DROP DEPENDING OBJECTS
ALTER TABLE t_distributionitems DROP CONSTRAINT fk_t_distributionitems3 ;
ALTER TABLE t_ondemanddistribution DROP CONSTRAINT fk_t_ondemanddistribution2 ;
ALTER TABLE t_distributionpolicies DROP CONSTRAINT fk_t_distributionpolicies3 ;
ALTER TABLE t_receptionpolicies DROP CONSTRAINT fk_t_receptionpolicies2 ;
ALTER TABLE t_temporarydistribution DROP CONSTRAINT fk_t_temporarydistribution3 ;

-- CHANGING TABLE COLUMNS
ALTER TABLE t_users ADD id_user SERIAL NOT NULL ;

ALTER TABLE t_users DROP CONSTRAINT pk_t_users ;
ALTER TABLE t_users
	ADD CONSTRAINT pk_t_users
		PRIMARY KEY (id_user) ;

ALTER TABLE t_users 
	ADD CONSTRAINT uq_users01
		UNIQUE (userid) ;

-- RECREATE DEPENDING OBJECTS
ALTER TABLE t_distributionitems
	ADD CONSTRAINT fk_t_distributionitems3
		FOREIGN KEY (userid)
			REFERENCES t_users (userid)
			  	ON DELETE CASCADE ;
ALTER TABLE t_ondemanddistribution
	ADD CONSTRAINT fk_t_ondemanddistribution2
		FOREIGN KEY (userid)
			REFERENCES t_users (userid)
				ON DELETE CASCADE ;
ALTER TABLE t_distributionpolicies
	ADD CONSTRAINT fk_t_distributionpolicies3
		FOREIGN KEY (userid)
			REFERENCES t_users (userid)
				ON DELETE CASCADE ;
ALTER TABLE t_receptionpolicies
	ADD CONSTRAINT fk_t_receptionpolicies2
		FOREIGN KEY (userid)
			REFERENCES t_users (userid)
				ON DELETE CASCADE ;
ALTER TABLE t_temporarydistribution
	ADD CONSTRAINT fk_t_temporarydistribution3
		FOREIGN KEY (userid)
			REFERENCES t_users (userid)
				ON DELETE CASCADE ;


/* ------------------------------
	t_filetypes
*/ ------------------------------

-- DROP DEPENDING OBJECTS
ALTER TABLE t_distributionpolicies DROP CONSTRAINT fk_t_distributionpolicies4 ;
ALTER TABLE t_receptionpolicies DROP CONSTRAINT fk_t_receptionpolicies3 ;
ALTER TABLE t_inventory DROP CONSTRAINT fk_t_inventory5 ;
ALTER TABLE t_ordersgeneration DROP CONSTRAINT fk_t_ordersgeneration2 ;
ALTER TABLE t_invrollingpolicies DROP CONSTRAINT fk_t_invrollingpolicies1 ;
ALTER TABLE t_waitingorders DROP CONSTRAINT fk_t_waitingorders2 ;
ALTER TABLE t_storagepriority DROP CONSTRAINT fk_t_storagepriority2 ;
ALTER TABLE t_classifiedfiletypes DROP CONSTRAINT fk_t_classifiedfiletypes ;
ALTER TABLE t_inventoryeventsconf DROP CONSTRAINT fk_t_inventsconf_ftype ;
ALTER TABLE t_storagegroup DROP CONSTRAINT fk_storagegroup_ftype ;
ALTER TABLE t_filetypes_x_satellites DROP CONSTRAINT fk_t_filetypesxsatellites01 ;
ALTER TABLE t_filetypes_stylesheets DROP CONSTRAINT fk_filetypes_stylesheets01 ;
ALTER TABLE t_filetypescomp DROP CONSTRAINT fk_filetypescomp01 ;
ALTER TABLE t_filetypescomp DROP CONSTRAINT fk_filetypescomp02 ;
ALTER TABLE t_filetypes_last_inv DROP CONSTRAINT fk_filetypes_last_inv01 ;
   -- plugin schemas
ALTER TABLE t_circulationpolicies DROP CONSTRAINT fk_circulationpolicies01 ;
ALTER TABLE t_inventorylinksconf DROP CONSTRAINT fk_invlinkconf_ftype1 ;
ALTER TABLE t_inventorylinksconf DROP CONSTRAINT fk_invlinkconf_ftype2 ;

-- CHANGING TABLE COLUMNS
ALTER TABLE t_filetypes ADD id_filetype SERIAL NOT NULL ;

ALTER TABLE t_filetypes DROP CONSTRAINT pk_t_filetypes ;
ALTER TABLE t_filetypes
	ADD CONSTRAINT pk_t_filetypes
		PRIMARY KEY (id_filetype) ;

ALTER TABLE t_filetypes 
	ADD CONSTRAINT uq_filetypes01
		UNIQUE (filetype) ;

ALTER TABLE t_filetypes 
	ADD CONSTRAINT ck_filetypes01 
		CHECK ((sngext IS NULL AND hdrext IS NULL AND dblext IS NULL) 
			OR (sngext IS NULL AND hdrext IS NOT NULL AND dblext IS NOT NULL AND ( hdrext != dblext)) 
			OR (sngext IS NOT NULL AND ((hdrext IS NULL AND dblext IS NULL) OR ( hdrext != dblext)))) ;

ALTER TABLE t_filetypes DROP COLUMN userfiletype ;
ALTER TABLE t_filetypes DROP COLUMN generateheader ;
ALTER TABLE t_filetypes DROP COLUMN headergenerator ;
ALTER TABLE t_filetypes DROP COLUMN inventoryexecutable ;
ALTER TABLE t_filetypes DROP COLUMN package ;
ALTER TABLE t_filetypes DROP COLUMN onlinelatencytime ;
ALTER TABLE t_filetypes DROP COLUMN headertemplateprefix ;
ALTER TABLE t_filetypes DROP COLUMN storage ;
ALTER TABLE t_filetypes DROP COLUMN overalllifetime ;
ALTER TABLE t_filetypes DROP COLUMN preinventoryexecutable ;
ALTER TABLE t_filetypes DROP COLUMN storeontape ;
ALTER TABLE t_filetypes DROP COLUMN inventoryexecopts ;
ALTER TABLE t_filetypes DROP COLUMN preinventoryexecopts ;

ALTER TABLE t_filetypes ALTER COLUMN headerseparation SET DEFAULT 'f' ;

-- RECREATE DEPENDING OBJECTS
ALTER TABLE t_distributionpolicies
	ADD CONSTRAINT fk_t_distributionpolicies4
		FOREIGN KEY (filetype)
			REFERENCES t_filetypes (filetype)
				ON DELETE CASCADE ;
ALTER TABLE t_receptionpolicies
	ADD CONSTRAINT fk_t_receptionpolicies3
		FOREIGN KEY (filetype)
			REFERENCES t_filetypes (filetype)
				ON DELETE CASCADE ;
ALTER TABLE t_inventory
	ADD CONSTRAINT fk_t_inventory5
		FOREIGN KEY (filetype)
			REFERENCES t_filetypes (filetype)
				ON DELETE CASCADE ;
ALTER TABLE t_ordersgeneration
	ADD CONSTRAINT fk_t_ordersgeneration2
		FOREIGN KEY (filetype)
			REFERENCES t_filetypes (filetype)
				ON DELETE CASCADE ;
ALTER TABLE t_invrollingpolicies
 	 ADD CONSTRAINT fk_t_invrollingpolicies1
		FOREIGN KEY (filetype)
			REFERENCES t_filetypes (filetype)
				ON DELETE CASCADE ;
ALTER TABLE t_waitingorders
	ADD CONSTRAINT fk_t_waitingorders2
		FOREIGN KEY (filetype)
			REFERENCES t_filetypes (filetype)
				ON DELETE CASCADE ;
ALTER TABLE t_storagepriority
	ADD CONSTRAINT fk_t_storagepriority2
		FOREIGN KEY (filetype)
			REFERENCES t_filetypes (filetype)
			ON DELETE CASCADE ;
ALTER TABLE t_classifiedfiletypes
	ADD CONSTRAINT FK_t_classifiedfiletypes
		FOREIGN KEY (filetype)
			REFERENCES t_filetypes (filetype)
				ON DELETE CASCADE ;
ALTER TABLE t_inventoryeventsconf 
	ADD CONSTRAINT fk_t_inventsconf_ftype
		FOREIGN KEY (filetype)
			REFERENCES t_filetypes (filetype) 
				ON DELETE CASCADE ;
ALTER TABLE t_storagegroup 
	ADD CONSTRAINT fk_storagegroup_ftype 
		FOREIGN KEY (filetype)
			REFERENCES t_filetypes (filetype) 
				ON DELETE CASCADE ;
ALTER TABLE t_filetypes_x_satellites 
	ADD CONSTRAINT fk_t_filetypesxsatellites01 
		FOREIGN KEY (filetype)
			REFERENCES t_filetypes (filetype) 
				ON DELETE CASCADE ;
ALTER TABLE t_filetypes_stylesheets 
	ADD CONSTRAINT fk_filetypes_stylesheets01
		FOREIGN KEY (filetype)
			REFERENCES t_filetypes (filetype) 
				ON DELETE CASCADE ;
ALTER TABLE t_filetypescomp 
	ADD CONSTRAINT fk_filetypescomp01
		FOREIGN KEY (pfiletype) 
			REFERENCES t_filetypes (filetype) 
				ON DELETE CASCADE ;
ALTER TABLE t_filetypescomp 
	ADD CONSTRAINT fk_filetypescomp02
		FOREIGN KEY (cfiletype) 
			REFERENCES t_filetypes (filetype) 
				ON DELETE CASCADE ;
ALTER TABLE t_filetypes_last_inv 
	ADD CONSTRAINT fk_filetypes_last_inv01 
		FOREIGN KEY (filetype) 
			REFERENCES t_filetypes (filetype) 
				ON DELETE CASCADE ;
-- plugin schemas
ALTER TABLE t_circulationpolicies 
	ADD CONSTRAINT fk_circulationpolicies01
		FOREIGN KEY (filetype_id) 
			REFERENCES t_filetypes (filetype) 
				ON DELETE CASCADE ;
ALTER TABLE t_inventorylinksconf 
	ADD CONSTRAINT fk_invlinkconf_ftype1
		FOREIGN KEY (orig_filetype)
			REFERENCES t_filetypes (filetype) 
				ON DELETE CASCADE ;
ALTER TABLE t_inventorylinksconf 
	ADD CONSTRAINT fk_invlinkconf_ftype2
		FOREIGN KEY (dest_filetype)
			REFERENCES t_filetypes (filetype) 
				ON DELETE CASCADE ;


/* ------------------------------
	t_usermodes
*/ ------------------------------
ALTER TABLE t_usermodes ALTER COLUMN id TYPE INT2 ;
UPDATE t_usermodes SET id = 1 WHERE mode = 'NOMINAL' ;
UPDATE t_usermodes SET id = 2 WHERE mode = 'TEST' ;
UPDATE t_usermodes SET id = 3 WHERE mode = 'DEBUG' ;

ALTER TABLE t_usermodes 
	ADD CONSTRAINT uq_usermodes01
		UNIQUE (mode) ;

DROP SEQUENCE IF EXISTS t_usermodes_id_seq CASCADE ;


/* ------------------------------
	t_distributionalgo
*/ ------------------------------
ALTER TABLE t_distributionalgo ALTER COLUMN id TYPE INT2 ;
UPDATE t_distributionalgo SET id = 1 WHERE name = 'ALWAYS' ;
UPDATE t_distributionalgo SET id = 2 WHERE name = 'QUALIFIED' ;
UPDATE t_distributionalgo SET id = 3 WHERE name = 'REDISTRIBUTION' ;
UPDATE t_distributionalgo SET id = 4 WHERE name = 'QUALIFYREPORT' ;
UPDATE t_distributionalgo SET id = 5 WHERE name = 'PARTIALCVRG' ;
UPDATE t_distributionalgo SET id = 6 WHERE name = 'TOTALCVRG' ;
INSERT INTO t_distributionalgo (id, name, on_demand) VALUES (7, 'DATASTRIPCONDITIONED', 'f') ;

DROP SEQUENCE IF EXISTS t_distributionalgo_id_seq CASCADE ;


/* ------------------------------
	t_satrelations
*/ ------------------------------
INSERT INTO t_satrelations VALUES (38, 1, 2) ;
INSERT INTO t_satrelations VALUES (38, 1, 20) ;
INSERT INTO t_satrelations VALUES (39, 1, 2) ;
INSERT INTO t_satrelations VALUES (39, 1, 20) ;


/* ------------------------------
	t_inv_relation_types
*/ ------------------------------
INSERT INTO t_inv_relation_types (id, relation_type) VALUES (3, 'CHILD_TO_PARENT') ;








---------------------------------------------------------------------------------
-------------------------------- TABLES CREATION --------------------------------
---------------------------------------------------------------------------------

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

ALTER TABLE t_disseminationpackingalgos 
	ADD CONSTRAINT uq_disseminationpackingalgos01
		UNIQUE (factorykey) ;

INSERT INTO t_disseminationpackingalgos (factorykey) VALUES ('DummyValue') ;
INSERT INTO t_disseminationpackingalgos (factorykey) VALUES ('DateSize') ;
INSERT INTO t_disseminationpackingalgos (factorykey) VALUES ('OneItem') ;
INSERT INTO t_disseminationpackingalgos (factorykey) VALUES ('OneItemPlusRpt') ;


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

ALTER TABLE t_discoveryplugins 
	ADD CONSTRAINT uq_discoveryplugins01
		UNIQUE (name) ;

INSERT INTO t_discoveryplugins (name) VALUES ('DummyPlugin') ;


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

ALTER TABLE t_filetypescomps 
	ADD CONSTRAINT ck_filetypescomps01 
		CHECK (pfiletype != cfiletype ) ;

ALTER TABLE t_filetypescomps 
	ADD CONSTRAINT uq_filetypescomps01 
		UNIQUE (pfiletype, cfiletype) ;

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

CREATE INDEX ix_t_filetypescomps01 ON t_filetypescomps USING BTREE (pfiletype) ;
CREATE INDEX ix_t_filetypescomps02 ON t_filetypescomps USING BTREE (cfiletype) ;


/* ------------------------------
	t_tasktables
*/ ------------------------------
CREATE TABLE t_tasktables (
    id_tasktable SERIAL NOT NULL,
	name VARCHAR(64) NOT NULL,
	tasktable TEXT,
	description VARCHAR(256)
) ;
ALTER TABLE t_tasktables
	ADD CONSTRAINT pk_t_tasktables
		PRIMARY KEY (id_tasktable) ;

ALTER TABLE t_tasktables 
	ADD CONSTRAINT uq_tasktables01
		UNIQUE (name) ;

--INSERT INTO t_tasktables (name, description) VALUES ('defaultReception', '') ;
--INSERT INTO t_tasktables (name, description) VALUES ('defaultDistribFormat', '') ;
--INSERT INTO t_tasktables (name, description) VALUES ('defaultDelivery', '') ;
--INSERT INTO t_tasktables (name, description) VALUES ('nopDeliveryCheck', '') ;


/* ------------------------------
	t_repositories
*/ ------------------------------
CREATE TABLE t_repositories (
    id_repository SERIAL NOT NULL,
	description VARCHAR(256) NOT NULL,
	protocol VARCHAR(8)NOT NULL,
	hostname VARCHAR(256) NOT NULL,
	username VARCHAR(64) NOT NULL,
	password VARCHAR(256),
	remotepath VARCHAR(1024) DEFAULT '' NOT NULL
) ;
ALTER TABLE t_repositories
	ADD CONSTRAINT pk_t_repositories
		PRIMARY KEY (id_repository) ;

ALTER TABLE t_repositories 
	ADD CONSTRAINT uq_repositories01
		UNIQUE (description) ;


/* ------------------------------
	t_receptionrules
*/ ------------------------------
CREATE TABLE t_receptionrules (
    id_receptionrule SERIAL NOT NULL,
    filetype_id INTEGER NOT NULL,
	repository_id INTEGER NOT NULL,
	user_id INTEGER NOT NULL,
	usermode_id INTEGER NOT NULL,
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
	successnotificationaddress TEXT
) ;
ALTER TABLE t_receptionrules
	ADD CONSTRAINT pk_t_receptionrules
		PRIMARY KEY (id_receptionrule) ;

ALTER TABLE t_receptionrules
	ADD CONSTRAINT ck_receptionrules01
		CHECK (pollingperiod_sec > 0) ;

ALTER TABLE t_receptionrules
	ADD CONSTRAINT ck_receptionrules02
		CHECK ((retryperiod_sec > 0) AND (retryperiod_sec < floor(pollingperiod_sec/connretries))) ;

CREATE UNIQUE INDEX uq_receptionrules01 
	ON t_receptionrules (filetype_id, repository_id, user_id, usermode_id)
		WHERE active = 't' ;

CREATE UNIQUE INDEX uq_receptionrules02 
	ON t_receptionrules (filetype_id, localpath)
		WHERE active = 't' ;

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

CREATE INDEX ix_t_receptionrules01 ON t_receptionrules USING BTREE (filetype_id) ;
CREATE INDEX ix_t_receptionrules02 ON t_receptionrules USING BTREE (repository_id) ;
CREATE INDEX ix_t_receptionrules03 ON t_receptionrules USING BTREE (user_id) ;
CREATE INDEX ix_t_receptionrules04 ON t_receptionrules USING BTREE (usermode_id) ;
CREATE INDEX ix_t_receptionrules05 ON t_receptionrules USING BTREE (reception_tt_id) ;
CREATE INDEX ix_t_receptionrules06 ON t_receptionrules USING BTREE (discoveryplugin_id) ;


/* ------------------------------
	t_receptionruleshist
*/ ------------------------------
CREATE TABLE t_receptionruleshist (
    id_receptionrulehist SERIAL NOT NULL,
	receptionrule_id INTEGER NOT NULL,
	filename VARCHAR(1024) NOT NULL,
	tstamp TIMESTAMP NOT NULL,
	status_id INT2 NOT NULL
) ;
ALTER TABLE t_receptionruleshist
	ADD CONSTRAINT pk_t_receptionruleshist
		PRIMARY KEY (id_receptionrulehist) ;

ALTER TABLE t_receptionruleshist 
	ADD CONSTRAINT uq_receptionruleshist01
		UNIQUE (receptionrule_id, filename) ;

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

CREATE INDEX ix_t_receptionruleshist01 ON t_receptionruleshist USING BTREE (receptionrule_id) ;
CREATE INDEX ix_t_receptionruleshist02 ON t_receptionruleshist USING BTREE (filename) ;
CREATE INDEX ix_t_receptionruleshist03 ON t_receptionruleshist USING BTREE (status_id) ;


/* ------------------------------
	t_receptionruleslock
*/ ------------------------------
CREATE TABLE t_receptionruleslock (
    receptionrule_id INTEGER NOT NULL,
    responsible VARCHAR(128) NOT NULL,
    tstamp TIMESTAMP(6) DEFAULT NOW() NOT NULL
) ;
ALTER TABLE t_receptionruleslock
	ADD CONSTRAINT pk_t_receptionruleslock
		PRIMARY KEY (receptionrule_id) ;

ALTER TABLE t_receptionruleslock
	ADD CONSTRAINT fk_receptionruleslock01
		FOREIGN KEY (receptionrule_id)
			REFERENCES t_receptionrules (id_receptionrule)
				ON DELETE CASCADE ;

CREATE INDEX ix_t_receptionruleslock01 ON t_receptionruleslock USING BTREE (receptionrule_id) ;



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

ALTER TABLE t_distributionrules
	ADD CONSTRAINT ck_distributionrules01
		CHECK (retryperiod_sec > 0) ;

CREATE UNIQUE INDEX uq_distributionrules01 
	ON t_distributionrules (ondemand, filetype_id, mediatype_id, user_id, usermode_id, repository_id, format_tt_id) 
		WHERE active = 't' AND fileclass_id IS NULL;

CREATE UNIQUE INDEX uq_distributionrules02 
	ON t_distributionrules (ondemand, filetype_id, mediatype_id, user_id, usermode_id, repository_id, format_tt_id, fileclass_id) 
		WHERE active = 't' AND fileclass_id IS NOT NULL;

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

CREATE INDEX ix_t_storagerollingthreshold01 ON t_storagerollingthresholds USING BTREE (action_id) ;


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

ALTER TABLE t_cleanupalgos 
	ADD CONSTRAINT uq_cleanupalgos01
		UNIQUE (name) ;

INSERT INTO t_cleanupalgos (id_cleanupalgo, name) VALUES (1, 'QUOTA') ;
INSERT INTO t_cleanupalgos (id_cleanupalgo, name) VALUES (2, 'AGING') ;



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

ALTER TABLE t_cleanuprules
	ADD CONSTRAINT ck_cleanuprules01
		CHECK (hiquota_mb >= lowquota_mb) ;

ALTER TABLE t_cleanuprules
	ADD CONSTRAINT ck_cleanuprules02
		CHECK (lowquota_mb >= 0) ;

ALTER TABLE t_cleanuprules
	ADD CONSTRAINT ck_cleanuprules03
		CHECK (age_h > 0) ;

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

CREATE INDEX ix_t_cleanuprules01 ON t_cleanuprules USING BTREE (repository_id) ;
CREATE INDEX ix_t_cleanuprules02 ON t_cleanuprules USING BTREE (cleanupalgo_id) ;








---------------------------------------------------------------------------------
--------------------------- FOREIGNKEYS MODIFICATION ----------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_distributionqueue
*/ ------------------------------
ALTER TABLE t_distributionqueue
	ADD CONSTRAINT fk_t_distributionqueue1
		FOREIGN KEY (format)
			REFERENCES t_tasktables (id_tasktable)
				ON DELETE CASCADE ;
CREATE INDEX ix_t_distributionqueue02 ON t_distributionqueue USING BTREE (format) ;


/* ------------------------------
	t_distributionitems
*/ ------------------------------
ALTER TABLE t_distributionitems
	ADD CONSTRAINT fk_t_distributionitems5
		FOREIGN KEY (format)
			REFERENCES t_tasktables (id_tasktable)
				ON DELETE CASCADE ;
CREATE INDEX ix_t_distributionitems05 ON t_distributionitems USING BTREE (format) ;

ALTER TABLE t_distributionitems 
	ADD CONSTRAINT fk_t_distributionitems7
		FOREIGN KEY (distributionrule_id)
			REFERENCES t_distributionrules (id_distributionrule) 
				ON DELETE CASCADE ;
CREATE INDEX ix_t_distributionitems07 ON t_distributionitems USING BTREE (distributionrule_id) ;

ALTER TABLE t_distributionitems 
	ADD CONSTRAINT fk_t_distributionitems8
		FOREIGN KEY (disseminationpackingalgo_id)
			REFERENCES t_disseminationpackingalgos (id_disseminationpackingalgo) 
				ON DELETE CASCADE ;
CREATE INDEX ix_t_distributionitems08 ON t_distributionitems USING BTREE (disseminationpackingalgo_id) ;


/* ------------------------------
	t_temporarydistribution
*/ ------------------------------
ALTER TABLE t_temporarydistribution DROP CONSTRAINT fk_t_temporarydistribution1 ;
ALTER TABLE t_temporarydistribution
	ADD CONSTRAINT fk_t_temporarydistribution1
		FOREIGN KEY (policy)
			REFERENCES t_distributionrules (id_distributionrule)
				ON DELETE CASCADE ;
CREATE INDEX ix_t_temporarydistribution01 ON t_temporarydistribution USING BTREE (policy) ;

ALTER TABLE t_temporarydistribution
	ADD CONSTRAINT fk_t_temporarydistribution5
		FOREIGN KEY	(format)
			REFERENCES t_tasktables (id_tasktable)
				ON DELETE CASCADE ;
CREATE INDEX ix_t_temporarydistribution02 ON t_temporarydistribution USING BTREE (format) ;

ALTER TABLE t_temporarydistribution DROP CONSTRAINT fk_tempdist_cart ;
ALTER TABLE t_temporarydistribution 
	ADD CONSTRAINT fk_t_temporarydistribution7
		FOREIGN KEY (cartid)
			REFERENCES t_cartsref (id) 
				ON DELETE CASCADE ;
CREATE INDEX ix_t_temporarydistribution03 ON t_temporarydistribution USING BTREE (cartid) ;

ALTER TABLE t_temporarydistribution 
	ADD CONSTRAINT fk_t_temporarydistribution8
		FOREIGN KEY (disseminationpackingalgo_id)
			REFERENCES t_disseminationpackingalgos (id_disseminationpackingalgo) 
				ON DELETE CASCADE ;
CREATE INDEX ix_t_temporarydistribution04 ON t_temporarydistribution USING BTREE (disseminationpackingalgo_id) ;


/* ------------------------------
	t_distpoldataset
*/ ------------------------------
ALTER TABLE t_distpoldataset DROP CONSTRAINT fk_t_distpoldataset1 ;
ALTER TABLE t_distpoldataset
	ADD CONSTRAINT fk_t_distpoldataset1
		FOREIGN KEY (policy)
			REFERENCES t_distributionrules (id_distributionrule)
				ON DELETE CASCADE ;
CREATE INDEX ix_t_distpoldataset01 ON t_distpoldataset USING BTREE (policy) ;
CREATE INDEX ix_t_distpoldataset02 ON t_distpoldataset USING BTREE (dataset) ;

/* ------------------------------
	t_distpoloriginator
*/ ------------------------------
ALTER TABLE t_distpoloriginator DROP CONSTRAINT fk_t_distpoloriginator1 ;
ALTER TABLE t_distpoloriginator
	ADD CONSTRAINT fk_t_distpoloriginator1
		FOREIGN KEY (policy)
			REFERENCES t_distributionrules (id_distributionrule)
				ON DELETE CASCADE ;
CREATE INDEX ix_t_distpoloriginator01 ON t_distpoloriginator USING BTREE (policy) ;
CREATE INDEX ix_t_distpoloriginator02 ON t_distpoloriginator USING BTREE (originator) ;


/* ------------------------------
	t_ondemanddistribution
*/ ------------------------------
ALTER TABLE t_ondemanddistribution
	ADD CONSTRAINT fk_t_ondemanddistribution3
		FOREIGN KEY (format)
			REFERENCES t_tasktables (id_tasktable)
				ON DELETE CASCADE ;

ALTER TABLE t_ondemanddistribution 
	ADD CONSTRAINT fk_t_ondemanddistribution5
		FOREIGN KEY (disseminationpackingalgo_id)
			REFERENCES t_disseminationpackingalgos (id_disseminationpackingalgo) 
				ON DELETE CASCADE ;

CREATE INDEX ix_t_ondemanddistribution01 ON t_ondemanddistribution USING BTREE (orderid) ;
CREATE INDEX ix_t_ondemanddistribution02 ON t_ondemanddistribution USING BTREE (userid) ;
CREATE INDEX ix_t_ondemanddistribution03 ON t_ondemanddistribution USING BTREE (format) ;
CREATE INDEX ix_t_ondemanddistribution04 ON t_ondemanddistribution USING BTREE (mediatype) ;
CREATE INDEX ix_t_ondemanddistribution05 ON t_ondemanddistribution USING BTREE (disseminationpackingalgo_id) ;


/* ------------------------------
	t_standingdistribution
*/ ------------------------------
ALTER TABLE t_standingdistribution DROP CONSTRAINT fk_stendingdistribution01 ;
ALTER TABLE t_standingdistribution 
	ADD CONSTRAINT fk_stendingdistribution01 
		FOREIGN KEY (distpoliciesid)
			REFERENCES t_distributionrules (id_distributionrule) 
				ON DELETE CASCADE ;
CREATE INDEX ix_t_standingdistribution01 ON t_standingdistribution USING BTREE (distpoliciesid) ;
CREATE INDEX ix_t_standingdistribution02 ON t_standingdistribution USING BTREE (cartsid) ;


/* ------------------------------
	t_receptionpolicieslock
*/ ------------------------------
ALTER TABLE t_receptionpolicieslock DROP CONSTRAINT fk_t_receptionpolicieslock ;
ALTER TABLE t_receptionpolicieslock
	ADD CONSTRAINT fk_t_receptionpolicieslock1
		FOREIGN KEY (receptionpolicyid)
			REFERENCES t_receptionrules (id_receptionrule)
				ON DELETE CASCADE ;
CREATE INDEX ix_t_receptionpolicieslock01 ON t_receptionpolicieslock USING BTREE (receptionpolicyid) ;








---------------------------------------------------------------------------------
------------------------------ PROCEDURES CREATION ------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	p_events
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_events(
	in_table VARCHAR(64), 
	in_action VARCHAR(16)) RETURNS INTEGER AS $$
	
DECLARE
	event_file VARCHAR(64) ;
	ret INTEGER;
	
BEGIN
	IF (SELECT count(*) FROM t_dbparameters WHERE parameter = 'broadcast_events_path') > 0 THEN
		event_file := trim((SELECT value_str FROM t_dbparameters WHERE parameter = 'broadcast_events_path')) || '/PDS_events' ;
	ELSE
		event_file := '/usr/acs/tmp/PDS_events' ;
	END IF ;
	
	ret := f_acs_traceEvent(event_file, in_table, in_action) ;
	
	RETURN ret;
	
END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	p_inventory_manager
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_inventory_manager(
	fname VARCHAR(128), 
	fversion VARCHAR(32),
	prevalflag BOOLEAN,
	postvalflag BOOLEAN,
	ftype VARCHAR(16),
	fclass VARCHAR(8),
	orderid INTEGER,
	in_satellite SMALLINT, 
	in_sensor SMALLINT, 
	in_mission SMALLINT,
	valstart TIMESTAMP(6),
	valstop TIMESTAMP(6)) RETURNS INTEGER AS $$
	
DECLARE
  err INTEGER ;
  evnt VARCHAR(3) ;
  evnttype VARCHAR(8) ;
  
BEGIN
	-- Check for satellite-sensor-mission relation
  	err := p_checksatsensmiss(in_satellite, in_sensor, in_mission) ;
	
	-- If is called after an insert
  	IF prevalflag IS NULL THEN
		evnt := 'INS' ;
		evnttype := 'INSFTYPE' ;
		INSERT INTO t_pendinginventory (filename, fileversion) VALUES (fname, fversion) ;
	-- If is called after an update
	ELSE
		evnt := 'UPD' ;
		evnttype := 'UPDFTYPE' ;
	END IF ;
	
	-- If validity flag is, or became, TRUE
  	IF ((prevalflag IS NULL AND postvalflag = 't') OR (prevalflag <> postvalflag AND postvalflag = 't')) THEN
  	  	err := p_processinventory(fname, fversion, postvalflag, ftype, fclass, orderid, in_satellite, valstart, valstop) ;
  	END IF ;

	----------------------------------------------------------------------------
	-- BROADCAST SECTION
	----------------------------------------------------------------------------
  	err := p_events('t_inventory', evnt) ;
	err := p_events(ftype, evnttype) ;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	p_orders_manager
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_orders_manager(
	in_orderid INTEGER, 
	pre_status VARCHAR(32), 
	post_status VARCHAR(32),
	event VARCHAR(8)) RETURNS INTEGER AS $$
	
DECLARE
  	cnt INTEGER ;
  	rec RECORD ;
  	err INTEGER ;
	evn INTEGER ;
	p_extordid VARCHAR(128);
	p_subreq INT;
	p_cartid INT;
	cartsinv_status VARCHAR(32);
	ex_msg VARCHAR(128);
	
BEGIN
	-- Set error message
	ex_msg := 'p_orders_manager: orderid ' || in_orderid || ' - ' || pre_status || ' to ' || post_status || ' invalid order state transition.';
	
	
	-- No status
	IF post_status IS NULL THEN
		RETURN 0;
	END IF ;
	
	
	-- First insert
	IF pre_status IS NULL THEN
		IF post_status = 'ELIGIBLE' THEN
			INSERT INTO t_orderstatetransitions (orderid, status) VALUES (in_orderid, post_status) ;
			evn := p_events('t_orderstatetransitions', 'NUL2ELI') ;
			RETURN evn ;
		ELSIF post_status = 'STANDBY' THEN
			INSERT INTO t_orderstatetransitions (orderid, status) VALUES (in_orderid, post_status) ;
			evn := p_events('t_orderstatetransitions', 'NUL2STA') ;
			RETURN evn ;
		ELSIF post_status = 'ABORTED' THEN
			INSERT INTO t_orderstatetransitions (orderid, status) VALUES (in_orderid, post_status) ;
			evn := p_events('t_orderstatetransitions', 'NUL2ABO') ;
			RETURN evn ;
		ELSE
			RAISE EXCEPTION '%', ex_msg ;
		END IF ;
	END IF ;
	
	
	-- Status changes
	IF post_status = 'ELIGIBLE' THEN
		IF pre_status = 'ERROR' THEN
			INSERT INTO t_orderstatetransitions (orderid, status) VALUES (in_orderid, post_status) ;
			evn := p_events('t_orderstatetransitions', 'ERR2ELI') ;	
		ELSIF pre_status = 'RECOVERABLEERROR' THEN
			INSERT INTO t_orderstatetransitions (orderid, status) VALUES (in_orderid, post_status) ;
			evn := p_events('t_orderstatetransitions', 'REC2ELI') ;
		ELSIF pre_status = 'STANDBY' THEN
			INSERT INTO t_orderstatetransitions (orderid, status) VALUES (in_orderid, post_status) ;
			evn := p_events('t_orderstatetransitions', 'STA2ELI') ;
		ELSIF pre_status = 'ELIGIBLE' THEN
		-- The following transition is logged only the first time to avoid too much negligible transitions
		-- ELIGIBLE->PRECONDCHEDCK->ELIGIBLE...
			SELECT INTO rec COUNT(*) FROM t_orderstatetransitions WHERE orderid = in_orderid ;
			cnt := rec.count;
			IF cnt = 0 THEN -- the first time
				-- insert item in state transition table
				INSERT INTO t_orderstatetransitions (orderid, status) VALUES (in_orderid, post_status) ;
			END IF ;
			RETURN 0 ;	
		ELSIF pre_status = 'PRECONDITIONCHECK' THEN 
			RETURN 0 ;
		ELSE 
			RAISE EXCEPTION '%', ex_msg ;
		END IF ;
	
	ELSIF post_status = 'PRECONDITIONCHECK' THEN
		IF pre_status = 'ELIGIBLE' THEN
			RETURN 0 ;
		-- The following transition is not logged to avoid too much negligible transitions ELIGIBLE->PRECONDCHEDCK->ELIGIBLE...
		-- INSERT INTO t_orderstatetransitions(orderid ,status) values ( in_orderid,post_status);
		ELSE 
			RAISE EXCEPTION '%', ex_msg ;
		END IF ;

	ELSIF post_status = 'DOWNLOADING' THEN
		IF pre_status = 'PRECONDITIONCHECK' THEN
			INSERT INTO t_orderstatetransitions (orderid, status) VALUES (in_orderid, post_status) ;
			evn := p_events('t_orderstatetransitions', 'PRE2DOW') ;
		ELSE 
			RAISE EXCEPTION '%', ex_msg ;
		END IF ;
	
	ELSIF post_status = 'PROCESSING' THEN
		IF pre_status = 'DOWNLOADING' THEN
			INSERT INTO t_orderstatetransitions (orderid, status) VALUES (in_orderid, post_status) ;
			evn := p_events('t_orderstatetransitions', 'DOW2PRO') ;
		ELSE 
			RAISE EXCEPTION '%', ex_msg ;
		END IF ;
	
	ELSIF post_status = 'ERROR' THEN
		IF pre_status = 'DOWNLOADING' THEN
			INSERT INTO t_orderstatetransitions (orderid, status) VALUES (in_orderid, post_status) ;
			evn := p_events('t_orderstatetransitions', 'DOW2ERR') ;	
		ELSIF pre_status = 'STANDBY' THEN
			INSERT INTO t_orderstatetransitions (orderid, status) VALUES (in_orderid, post_status) ;
			evn := p_events('t_orderstatetransitions', 'STA2ERR') ;	
		ELSIF pre_status = 'PROCESSING' THEN
			INSERT INTO t_orderstatetransitions (orderid, status) VALUES (in_orderid, post_status) ;
			evn := p_events('t_orderstatetransitions', 'PRO2ERR') ;
		ELSE 
			RAISE EXCEPTION '%', ex_msg ;
		END IF ;
	
	ELSIF post_status = 'RECOVERABLEERROR' THEN
		IF pre_status = 'PROCESSING' THEN
			INSERT INTO t_orderstatetransitions (orderid, status) VALUES (in_orderid, post_status) ;
			evn := p_events('t_orderstatetransitions', 'PRO2REC') ;
		ELSE 
			RAISE EXCEPTION '%', ex_msg ;
		END IF ;
	
	ELSIF post_status = 'COMPLETED' THEN
		IF pre_status = 'PROCESSING'  THEN
			INSERT INTO t_orderstatetransitions (orderid, status) VALUES (in_orderid, post_status) ;
			evn := p_events('t_orderstatetransitions', 'PRO2COM') ;
			err := p_completed_order_distribution(in_orderid) ;
		ELSIF pre_status = 'ERROR' THEN
			INSERT INTO t_orderstatetransitions (orderid, status) VALUES (in_orderid, post_status) ;
			evn := p_events('t_orderstatetransitions', 'ERR2COM') ;
			err := p_completed_order_distribution(in_orderid) ;
		ELSE 
			RAISE EXCEPTION '%', ex_msg ;
		END IF ;
	
	ELSIF post_status = 'ABORTED' THEN
		INSERT INTO t_orderstatetransitions (orderid, status) VALUES (in_orderid, post_status) ;
		evn := p_events('t_orderstatetransitions', '2ABO') ;
	
	ELSE
		RAISE EXCEPTION '%', ex_msg ;
	END IF ;
	
	
	p_extordid := (SELECT externalorderid FROM t_ordersqueue WHERE orderid = in_orderid);
	p_subreq := (SELECT subreq FROM t_ordersqueue WHERE orderid = in_orderid);
	
	IF p_extordid IS NOT NULL AND p_subreq IS NOT NULL THEN
		p_cartid := (SELECT id FROM t_cartsref AS cr
						INNER JOIN t_ordersqueue AS oq
							ON oq.externalorderid = cr.extref
						WHERE oq.orderid = in_orderid);
		
		IF post_status = 'COMPLETED' THEN
			cartsinv_status := 'PROCESSED';
		ELSE
			cartsinv_status := post_status ;
		END IF ;
		
		UPDATE t_cartsinv SET status = cartsinv_status WHERE cart = p_cartid AND ordsubreq = p_subreq ;	
		err := p_carts_update_status(p_cartid);
	END IF ;

	err := p_events('t_ordersqueue', event) ;
	RETURN 0 ;

END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	p_distribution_manager
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_distribution_manager(
	in_packageid INT,
	pre_status VARCHAR(32), 
	post_status VARCHAR(32)) RETURNS INTEGER AS $$

DECLARE
	ret INT;
	ex_msg VARCHAR(128);

BEGIN
	-- state transition :
	---------------------------------------------------------------------------------
	-- COMPLETED -> ABORTED
	-- DISTRIBUTION -> COMPLETED | WAITINGACK | ERROR | RECOVERABLEERROR | ABORTED
	-- DOWNLOADING -> DOWNLOADING | DISTRIBUTION | ERROR | RECOVERABLEERROR | ABORTED
	-- ELIGIBLE -> ELIGIBLE | DOWNLOADING | SUSPENDED | ABORTED
	-- SUSPENDED -> ELIGIBLE | ERROR | RECOVERABLEERROR | ABORTED
	-- TRANSIENT -> TRANSIENT | ELIGIBLE | ERROR | RECOVERABLEERROR | ABORTED
	-- RECOVERABLEERROR -> DOWNLOADING | SUSPENDED | ERROR | ABORTED
	-- WAITINGACK -> WAITINGACK | COMPLETED | ERROR | RECOVERABLEERROR | ABORTED
	-- ERROR -> ELIGIBLE | ABORTED
	---------------------------------------------------------------------------------
	
	
	-- Set error message
	ex_msg := 'p_distribution_manager: id ' || in_packageid || ' - ' || pre_status || ' to ' || post_status || ' invalid state transition.';
	
	
	-- Empty status
  	IF post_status IS NULL THEN
  	  	RETURN 0 ;
  	END IF ;

	
	-- No status changes
  	IF pre_status = post_status THEN
  	  	IF post_status = 'TRANSIENT' OR pre_status = 'ELIGIBLE' OR pre_status = 'DOWNLOADING' OR pre_status = 'WAITINGACK' THEN
  	  	  	ret := p_cartsinv_update(in_packageid, post_status);
			RETURN ret ;
  	  	ELSE
  	  	  	RAISE EXCEPTION '%', ex_msg ;
  	  	END IF ;
  	END IF ;


	-- Status changes
  	IF post_status = 'ELIGIBLE' 
  	  	AND (pre_status = 'TRANSIENT' OR pre_status = 'SUSPENDED' OR pre_status = 'ERROR')  THEN
  	  	  	ret := p_cartsinv_update(in_packageid, post_status);
			RETURN ret ;
	
	ELSIF post_status = 'SUSPENDED' 
  	  	AND (pre_status = 'ELIGIBLE' OR pre_status = 'RECOVERABLEERROR') THEN
  	  	  	ret := p_cartsinv_update(in_packageid, post_status);
			RETURN ret ;
	
  	ELSIF post_status = 'DOWNLOADING' 
  	  	AND (pre_status = 'ELIGIBLE' OR pre_status = 'RECOVERABLEERROR') THEN
  	  	  	ret := p_cartsinv_update(in_packageid, post_status);
			RETURN ret ;
	
	ELSIF post_status = 'DISTRIBUTION' 
  	  	AND pre_status = 'DOWNLOADING' THEN
  	  	  	ret := p_cartsinv_update(in_packageid, post_status);
			RETURN ret ;
	
	ELSIF post_status = 'COMPLETED' 
  	  	AND (pre_status = 'DISTRIBUTION' OR pre_status = 'WAITINGACK') THEN
  	  	  	ret := p_cartsinv_update(in_packageid, post_status);
			RETURN ret ;
	
	ELSIF post_status = 'WAITINGACK' 
  	  	AND pre_status = 'DISTRIBUTION' THEN
  	  	  	ret := p_cartsinv_update(in_packageid, post_status);
			RETURN ret ;
	
	ELSIF post_status = 'RECOVERABLEERROR' 
  	  	AND (pre_status = 'DOWNLOADING' OR pre_status = 'DISTRIBUTION' OR pre_status = 'SUSPENDED' OR pre_status = 'TRANSIENT' OR pre_status = 'WAITINGACK') THEN
  	  	  	ret := p_cartsinv_update(in_packageid, post_status);
			RETURN ret ;
	
	ELSIF post_status = 'ERROR' 
  	  	AND (pre_status = 'DOWNLOADING' OR pre_status = 'DISTRIBUTION' OR pre_status = 'SUSPENDED' OR pre_status = 'TRANSIENT' OR pre_status = 'WAITINGACK' OR pre_status = 'RECOVERABLEERROR') THEN
  	  	  	ret := p_cartsinv_update(in_packageid, post_status);
			RETURN ret ;
	
	ELSIF post_status = 'ABORTED' THEN
  	  	ret := p_cartsinv_update(in_packageid, post_status);
		RETURN ret ;
	
	ELSE
  	  	 RAISE EXCEPTION '%', ex_msg ;
	END IF ;
	
  	RETURN ret ;
	
END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
---------------------------- PROCEDURES MODIFICATION ----------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	p_iorepository
*/ ------------------------------
-- Changed only p_inserteventtableitem to p_events
CREATE OR REPLACE FUNCTION p_iorepository(
	action VARCHAR(8), 
	in_satellite SMALLINT, 
	in_sensor SMALLINT, 
	in_mission SMALLINT) RETURNS INTEGER AS $$
	
DECLARE
  	err INTEGER ;
	
BEGIN
	err := p_checksatsensmiss(in_satellite, in_sensor, in_mission) ;
	err := p_events('t_iorepository', action) ;

  	RETURN 0 ;
END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	p_purgewaitingorders_fix
*/ ------------------------------
-- Changed only p_inserteventtableitem to p_events
CREATE OR REPLACE FUNCTION p_purgewaitingorders_fix(calledby VARCHAR(8)) RETURNS INTEGER AS $$
	
DECLARE
  	tmpOrdId VARCHAR(16);
	evn INTEGER;
	row RECORD ;
	
BEGIN
	
	IF (calledby = 'PURGEWA') THEN
		-- delete records from table
		DELETE FROM t_waitingorders WHERE orderid IN (SELECT DISTINCT orderid FROM waitordtorembypurge) ; 

		FOR row IN (SELECT DISTINCT orderid::VARCHAR(16) FROM waitordtorembypurge) LOOP
			tmpOrdId := row.orderid ;
			evn := p_events(tmpOrdId, 'ORDNOWAI');
		END LOOP ;
		
	END IF;
	
	IF (calledby = 'POSTINV') THEN
		-- delete records from table
		DELETE FROM t_waitingorders WHERE orderid IN (SELECT DISTINCT orderid FROM waitordtorembypost) ; 

		FOR row IN (SELECT DISTINCT orderid::VARCHAR(16) FROM waitordtorembypost) LOOP
			tmpOrdId := row.orderid ;
			evn := p_events(tmpOrdId, 'ORDNOWAI');
		END LOOP ;
	END IF;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	P_INSERT_TEMPORARY_DISTRIBUTION
*/ ------------------------------
DROP FUNCTION p_insert_temporary_distribution(TIMESTAMP,VARCHAR,VARCHAR,VARCHAR,VARCHAR,INT,INT,INT,INT) ;
CREATE OR REPLACE FUNCTION p_insert_temporary_distribution(
	in_duedate TIMESTAMP(6),
	in_fname VARCHAR(128),
	in_fversion VARCHAR(32),
	in_mtype VARCHAR(16),
	in_format INT,
	in_algo INT,
	in_ordid INT,
	in_cartid INT,
	in_subreq INT,
	in_disspackalgo INTEGER)RETURNS INTEGER AS $$

BEGIN

	INSERT INTO t_temporarydistribution (
		duedate,
		filename,
		fileversion,
		mediatype,
		userid,
		format,
		algo,
		policy,
		orderid,
		cartid,
		subreq,
		disseminationpackingalgo_id )
	VALUES (
		in_duedate,
		in_fname,
		in_fversion,
		in_mtype,
		'ANONYMOUS',
		in_format,
		in_algo,
		(SELECT max(id_distributionrule) FROM t_distributionrules WHERE user_id = (
			SELECT id_user FROM t_users WHERE userid = 'ANONYMOUS')),
		in_ordid,
		in_cartid,
		in_subreq,
		in_disspackalgo
	) ;

	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	p_distribute_item
*/ ------------------------------
DROP FUNCTION p_distribute_item(VARCHAR,VARCHAR,VARCHAR,VARCHAR,VARCHAR,VARCHAR,INTEGER) ;
CREATE OR REPLACE FUNCTION p_distribute_item(
	in_fname VARCHAR(128),
	in_fversion VARCHAR(32),
	in_ftypeid INTEGER,
	in_userid INTEGER,
	in_fileclassid INTEGER,
	in_format INTEGER,
	in_orderid INTEGER,
	in_duedays INTEGER) RETURNS INTEGER AS $$
	
DECLARE
	p_extordid 	VARCHAR(128);
	p_cartid   	INT;
	p_subreq   	INT;
	p_totsubreq	INT;
	p_chng_add	INT;
	
BEGIN
	p_chng_add := NULL ;
	
	p_extordid := (SELECT externalorderid FROM t_ordersqueue WHERE orderid = in_orderid);
	p_subreq := (SELECT subreq FROM t_ordersqueue WHERE orderid = in_orderid);
	
	IF p_extordid IS NOT NULL AND p_subreq IS NOT NULL 
	THEN
		p_cartid := (SELECT id FROM t_cartsref AS cr
				INNER JOIN t_ordersqueue AS oq
				ON oq.externalorderid = cr.extref
				WHERE oq.orderid = in_orderid);

		p_totsubreq := (SELECT max(c.distsubreq) +1 
				FROM t_cartsinv c 
				WHERE c.cart = p_cartid);
	ELSE
		p_cartid := NULL;
		p_totsubreq := NULL;
	END IF;


--	the input inventory file is distributed to each user who declared an interest on that file-type.
--	the distribution is done according to the active field in the distribution policy record
	
	INSERT INTO t_temporarydistribution (
		duedate, 
		filename, 
		fileversion, 
		mediatype, 
		userid, 
		format, 
		algo, 
		policy, 
		orderid, 
		cartid, 
		subreq,
		disseminationpackingalgo_id )
	SELECT DISTINCT
		CASE
			WHEN in_duedays IS NULL THEN (NOW()::DATE + dr.maxdaytodeliver) + NOW()::TIME(6)
			ELSE (NOW()::DATE + in_duedays) + NOW()::TIME(6)
		END,
		in_fname, 
		in_fversion,
		mt.mediatype,
		us.userid,
		dr.format_tt_id, 
		dr.distributionalgo_id, 
		dr.id_distributionrule, 
		in_orderid, 
		p_cartid, 
		p_totsubreq,
		dr.disseminationpackingalgo_id
	FROM t_distributionrules dr
		INNER JOIN t_users us
			ON dr.user_id = us.id_user
		INNER JOIN t_mediatypes mt
			ON dr.mediatype_id = mt.id_mediatype
	WHERE
		dr.user_id = in_userid
		AND (dr.filetype_id = in_ftypeid OR dr.filetype_id IN (SELECT pfiletype FROM t_filetypescomps WHERE cfiletype = in_ftypeid)) 
		AND dr.active = 't'
		AND dr.ondemand = 'f'
		AND f_compare_int_values(dr.format_tt_id, in_format) 
		AND (f_compare_int_values(dr.fileclass_id, in_fileclassid) OR dr.fileclass_id IS NULL)
		AND dr.usermode_id = (SELECT mode FROM t_users WHERE id_user = in_userid)
	RETURNING 
		id INTO p_chng_add ;
	
	
	IF p_chng_add IS NOT NULL THEN
		UPDATE t_cartsinv SET distsubreq = p_totsubreq WHERE cart = p_cartid AND ordsubreq = p_subreq ;
	END IF ;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	P_DISTRIBUTE_ITEM_ON_DEMAND
*/ ------------------------------
DROP FUNCTION p_distribute_item_on_demand(VARCHAR,VARCHAR,VARCHAR,VARCHAR,VARCHAR,VARCHAR,INTEGER) ;
CREATE OR REPLACE FUNCTION p_distribute_item_on_demand(
	in_fname VARCHAR(128),
	in_fversion VARCHAR(32),
	in_ftypeid INTEGER,
	in_userid INTEGER,
	in_fileclassid INTEGER,
	in_format INTEGER,
	in_orderid INTEGER) RETURNS INTEGER AS $$
	
DECLARE
	p_extordid		VARCHAR(128);
	p_cartid		INT;
	p_subreq		INT;
	p_totsubreq		INT;
	p_ondem_media	VARCHAR(16);
	p_chng_add		INT;
	
BEGIN
	p_chng_add := NULL ;

	p_extordid := (SELECT externalorderid FROM t_ordersqueue WHERE orderid = in_orderid);
	p_subreq := (SELECT subreq FROM t_ordersqueue WHERE orderid = in_orderid);
	p_ondem_media := (SELECT od.mediatype FROM t_ondemanddistribution od WHERE od.orderid = in_orderid AND od.userid = (SELECT userid FROM t_users WHERE id_user = in_userid)) ;
	
	
	IF p_extordid IS NOT NULL AND p_subreq IS NOT NULL 
	THEN
		p_cartid := (SELECT id FROM t_cartsref AS cr
					INNER JOIN t_ordersqueue AS oq
						ON oq.externalorderid = cr.extref
					WHERE oq.orderid = in_orderid);

		p_totsubreq := (SELECT max(c.distsubreq) +1 
					FROM t_cartsinv c 
					WHERE c.cart = p_cartid);
	ELSE
		p_cartid := NULL;
		p_totsubreq := NULL;
	END IF;


--	the input inventory file is distributed to each user who declared an interest on that file-type.
--	the distribution is done according to the active field in the distribution policy record
	
	INSERT INTO t_temporarydistribution (
		duedate, 
		filename, 
		fileversion, 
		mediatype, 
		userid, 
		format, 
		algo, 
		policy, 
		orderid, 
		cartid, 
		subreq,
		disseminationpackingalgo_id )
	SELECT DISTINCT
		(NOW()::DATE + dr.maxdaytodeliver) + NOW()::TIME(6),
		in_fname, 
		in_fversion,
		CASE 
			WHEN p_ondem_media IS NULL THEN mt.mediatype
			ELSE p_ondem_media
		END,
		us.userid,
		dr.format_tt_id, 
		dr.distributionalgo_id, 
		dr.id_distributionrule, 
		in_orderid, 
		p_cartid, 
		p_totsubreq,
		dr.disseminationpackingalgo_id
	FROM t_distributionrules dr
		INNER JOIN t_users us
			ON dr.user_id = us.id_user
		INNER JOIN t_mediatypes mt
			ON dr.mediatype_id = mt.id_mediatype
	WHERE
		dr.user_id = in_userid
		AND (dr.filetype_id = in_ftypeid OR dr.filetype_id IN (SELECT pfiletype FROM t_filetypescomps WHERE cfiletype = in_ftypeid)) 
		AND dr.active = 't'
		AND dr.ondemand = 't'
		AND f_compare_int_values(dr.format_tt_id, in_format) 
		AND (f_compare_int_values(dr.fileclass_id, in_fileclassid) OR dr.fileclass_id IS NULL)
		AND dr.usermode_id = (SELECT mode FROM t_users WHERE id_user = in_userid)
		AND (p_ondem_media IS NULL OR dr.distributionalgo_id IN (SELECT id FROM t_distributionalgo WHERE on_demand = 't'))
	RETURNING 
		id INTO p_chng_add ;
	
	
	IF p_chng_add IS NOT NULL THEN
		UPDATE t_cartsinv SET distsubreq = p_totsubreq WHERE cart = p_cartid AND ordsubreq = p_subreq ;
	END IF ;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;




/* ------------------------------
	P_DISTRIBUTE_ORDER_INVENTORY
*/ ------------------------------
DROP FUNCTION p_distribute_order_inventory(INTEGER,VARCHAR,VARCHAR) ;
CREATE OR REPLACE FUNCTION p_distribute_order_inventory(
	in_orderid INTEGER, 
	in_fileclassid INTEGER) RETURNS INTEGER AS $$
	
BEGIN
--	this is the standard way to distribute the output of an order:
--	each inventory is distributed to users who defined a valid distribution rule for it

	IF in_orderid IS NOT NULL THEN
  	  	
		PERFORM p_distribute_item(
			iv.filename,
			iv.fileversion,
			dr.filetype_id,
			dr.user_id,
			in_fileclassid,
			dr.format_tt_id,
			iv.orderid,
			NULL)
		FROM 
			t_inventory AS iv
			INNER JOIN (SELECT id_filetype, filetype FROM t_filetypes) AS ft
				ON ft.filetype = iv.filetype
			INNER JOIN t_distributionrules AS dr
				ON (dr.filetype_id = ft.id_filetype 
					OR dr.filetype_id IN (SELECT pfiletype FROM t_filetypescomps WHERE cfiletype = ft.id_filetype))
		WHERE
			iv.orderid = in_orderid
			AND dr.active = 't'
			AND dr.ondemand = 'f'
			AND ((f_compare_int_values(dr.fileclass_id, in_fileclassid) OR dr.fileclass_id IS NULL))
		;
  	END IF ;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	p_processinventory
*/ ------------------------------
DROP FUNCTION p_processinventory(VARCHAR,VARCHAR,BOOLEAN,VARCHAR,VARCHAR,INTEGER,SMALLINT,TIMESTAMP,TIMESTAMP);
CREATE OR REPLACE FUNCTION p_processinventory(
	fname VARCHAR(128), 
	fversion VARCHAR(32),
	valflag BOOLEAN,
	ftype VARCHAR(16),
	fclass VARCHAR(8),
	in_orderid INTEGER,
	in_satellite SMALLINT, 
	valstart TIMESTAMP(6),
	valstop TIMESTAMP(6)
) RETURNS INTEGER AS $$

DECLARE
  	err   	   INTEGER ;
	ftype_id   INTEGER ;
	fclass_id  INTEGER ;
	
BEGIN
	---------------------------------------------------------------------------------
	-- The procedure assumes the triplet satellite/sensor/mission is already verified
	---------------------------------------------------------------------------------
	
	-- return without processing if the inventory was not pending
  	IF ((SELECT COUNT(*) FROM t_pendinginventory WHERE filename = fname AND fileversion = fversion) = 0) THEN
  	  	RETURN 0 ;
  	END IF ;
	
  	-- remove pending so nobody will try the same
  	DELETE FROM t_pendinginventory WHERE filename = fname AND fileversion = fversion ;
	
	-----------------------------------------------------------------------------
	-- TEMPORARY ORDER GENERATION SECTION
	-----------------------------------------------------------------------------
	err := p_generateTmpOrder('t', fname, fversion, ftype, fclass, in_satellite) ;
	
	-----------------------------------------------------------------------------
	-- DISTRIBUTION SECTION
	-----------------------------------------------------------------------------
	IF in_orderid IS NULL THEN
		ftype_id := (SELECT id_filetype FROM t_filetypes WHERE filetype = ftype) ;
		fclass_id := (SELECT id_fileclass FROM t_fileclasses WHERE fileclass = fclass) ;
			
		PERFORM 
			p_distribute_item(fname, fversion, ftype_id, dr.user_id, dr.fileclass_id, dr.format_tt_id, in_orderid, NULL)
		FROM 
			t_distributionrules AS dr
			INNER JOIN t_users AS us
				ON dr.user_id = us.id_user AND dr.usermode_id = us.mode
		WHERE (dr.filetype_id = ftype_id OR dr.filetype_id IN (SELECT pfiletype FROM t_filetypescomps WHERE cfiletype = ftype_id)) 
			AND dr.active = 't' 
			AND dr.ondemand = 'f'
			AND (dr.fileclass_id = fclass_id OR dr.fileclass_id IS NULL) ;
	END IF ;
	
	-----------------------------------------------------------------------------
	-- ORDER'S BLACK LIST SECTION
	-----------------------------------------------------------------------------
	err :=  p_purgewaitingorders(ftype, valstart, valstop) ;

	-----------------------------------------------------------------------------
	-- INVENTORY LINKS SECTION
	-----------------------------------------------------------------------------
	err := p_inventorylinks_check(fname, fversion, ftype, in_orderid) ;

RETURN 0;

END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	P_COMPLETED_ORDER_DISTRIBUTION
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_completed_order_distribution(in_orderid INTEGER) RETURNS INTEGER AS $$

DECLARE
  	row_odd RECORD ;
  	p_fileclassid INTEGER ;
	
	-- the procedure distributes the output from a completed order
	
BEGIN
	p_fileclassid := (SELECT id_fileclass FROM t_fileclasses WHERE fileclass = (SELECT ordertype FROM t_ordersqueue WHERE orderid = in_orderid)) ;
	
	--
	--	check if the users are already assigned to that order looking into t_ondemanddistribution
	--
	IF ((SELECT COUNT(*) FROM t_ondemanddistribution WHERE orderid = in_orderid) = 0) THEN
	--
	--	no predefined user : proceed with the configured ones
	--
		PERFORM p_distribute_order_inventory(in_orderid, p_fileclassid) ;
		RETURN 0 ;
	END IF ;
	
	--
	--	a set of users was found: distribute items to that users according to their distribution policies
	--
	FOR row_odd IN (
		SELECT 
			us.id_user AS user_id, 
			odd.format AS format_id
		FROM 
			t_ondemanddistribution odd 
			INNER JOIN t_users us
				ON odd.userid = us.userid
		WHERE 
			odd.orderid = in_orderid) LOOP

  	  	PERFORM p_distribute_item_on_demand(
			inv.filename, 
			inv.fileversion, 
			ft.id_filetype, 
			row_odd.user_id, 
			p_fileclassid, 
			row_odd.format_id, 
			in_orderid)
		FROM 
			t_inventory inv 
			INNER JOIN t_filetypes ft
				ON inv.filetype = ft.filetype
		WHERE 
			inv.orderid = in_orderid
		;
		
  	END LOOP ;
	
	RETURN 0 ;
END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	p_generatetmporder
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_generatetmporder(
	in_active BOOLEAN,
	fname VARCHAR(128),
	fversion VARCHAR(32),
	ftype VARCHAR(16),
	fclass VARCHAR(8),
	in_satellite SMALLINT) RETURNS INTEGER AS $$
	
BEGIN
	
	INSERT INTO t_temporaryorders (
		filename, 
		fileversion, 
		filetype, 
		ordertype, 
		processorname,
		processorversion, 
		algorithm, 
		processorconfiguration, 
		priority,
		troubleshooting, 
		processingtimewindow, 
		ordersat)
  	SELECT
		fname, 
		fversion, 
		ftype, 
		og.orderType, 
		og.processorName,
		og.processorVersion, 
		og.algorithm, 
		og.processorconfiguration, 
		og.priority,
		og.troubleshooting, 
		og.processingtimewindow, 
		in_satellite
	FROM 
		t_ordersgeneration og
	WHERE 
		og.filetype = ftype 
		AND og.active = in_active 
		AND (og.fileclass IS NULL OR og.fileclass = fclass)
	;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	p_generatetmporderpname
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_generatetmporderpname(
	in_active BOOLEAN,
	fname VARCHAR(128),
	fversion VARCHAR(32),
	ftype VARCHAR(16),
	fclass VARCHAR(8),
	in_satellite SMALLINT) RETURNS INTEGER AS $$
	
BEGIN
	
	INSERT INTO t_temporaryorders (
		filename, 
		fileversion, 
		filetype, 
		ordertype, 
		processorname,
		processorversion, 
		algorithm, 
		processorconfiguration, 
		priority,
		troubleshooting, 
		processingtimewindow, 
		ordersat)
  	SELECT
		fname, 
		fversion, 
		ftype, 
		og.orderType, 
		og.processorName,
		og.processorVersion, 
		og.algorithm, 
		og.processorconfiguration, 
		og.priority,
		og.troubleshooting, 
		og.processingtimewindow, 
		in_satellite
	FROM 
		t_ordersgeneration og
	WHERE 
		og.filetype = ftype 
		AND og.active = in_active 
		AND (og.fileclass IS NULL OR og.fileclass = fclass)
		AND og.processorname IN (SELECT pname FROM tempPnames)
	;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;



---------------------------
--- p_smStateTransition ---
---------------------------
CREATE OR REPLACE FUNCTION p_smStateTransition(
	in_packageid INT, 
	pre_status VARCHAR(32), 
	post_status VARCHAR(32)) RETURNS INTEGER AS $$

DECLARE
	ex_msg VARCHAR(128);

BEGIN

-- State Transition (from -> to | or):
------------------------------------------------
-- STANDBY -> ELIGIBLE
-- ELIGIBLE -> PREPARING | SUBMITTED
-- PREPARING -> SUBMITTED | ERROR
-- SUBMITTED -> ACKNOWLEDGED | COMPLETED | ERROR
-- ACKNOWLEDGED -> UPLOADING | ERROR
-- UPLOADING -> COMPLETED | ERROR
-- COMPLETED -> to nothing
-- ERROR -> ELIGIBLE
------------------------------------------------

	-- Set error message
	ex_msg := 'p_smStateTransition: id ' || in_packageid || ' - ' || pre_status || ' to ' || post_status || ' invalid sm state transition.';
	
	
	-- Empty status
	IF post_status IS NULL THEN
		RETURN 0;
	END IF ;
	
	
	-- New insert
	IF pre_status IS NULL AND post_status = 'STANDBY' THEN
		RETURN 0;
	END IF ;


	-- No changes status
	IF pre_status = post_status THEN
		RETURN 0;
	END IF ;
	

	-- Changing status
	IF post_status = 'ELIGIBLE'
		AND (pre_status = 'STANDBY' OR pre_status = 'ERROR') THEN
			RETURN 0 ;

	ELSIF post_status = 'PREPARING'
		AND pre_status = 'ELIGIBLE' THEN
			RETURN 0 ;
	
	ELSIF post_status = 'SUBMITTED'
		AND (pre_status = 'ELIGIBLE' OR pre_status = 'PREPARING') THEN
			RETURN 0 ;
	
	ELSIF post_status = 'ACKNOWLEDGED'
		AND pre_status = 'SUBMITTED' THEN
			RETURN 0 ;
	
	ELSIF post_status = 'UPLOADING' 
		AND pre_status = 'ACKNOWLEDGED' THEN
			RETURN 0 ;
	
	ELSIF post_status = 'COMPLETED' 
		AND (pre_status = 'UPLOADING' OR pre_status = 'SUBMITTED') THEN
			RETURN 0 ;
	
	ELSIF post_status = 'ERROR' 
		AND (pre_status = 'PREPARING' OR pre_status = 'SUBMITTED' OR pre_status = 'ACKNOWLEDGED' OR pre_status = 'UPLOADING') THEN
			RETURN 0 ;
	
	ELSE
		RAISE EXCEPTION '%', ex_msg ;
	
	END IF ;	

	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	P_REMOVE_OBSOLETE_ACTIONS
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_remove_obsolete_actions(
	fname VARCHAR(128), 
	fversion VARCHAR(32)) RETURNS INTEGER AS $$

DECLARE
	row RECORD;
	
BEGIN

	FOR row IN (
		SELECT 
			sm.packageid AS pkgid,
			sq.status AS stat, 
			sq.operation AS oper
		FROM 
			t_smactions sm
			INNER JOIN t_smqueue sq
				ON sq.packageid = sm.packageid
		WHERE sm.filename = fname 
			AND sm.fileversion = fversion 
			AND sm.packageid IS NOT NULL 
			AND sm.operation = 'UPLOAD') LOOP
	
		IF row.stat = 'ELIGIBLE' AND row.oper = 'UPLOAD' OR row.stat = 'ERROR' AND row.oper = 'UPLOAD' THEN
			UPDATE t_smactions SET packageid = NULL WHERE packageid = row.pkgid ;
			DELETE FROM t_smqueue WHERE packageid = row.pkgid ;
		END IF ;
		
	END LOOP ;
	
	DELETE FROM t_smactions WHERE filename = fname AND fileversion = fversion AND operation = 'UPLOAD' ;
	
	RETURN 0 ;
END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	P_SMACTIONSUPDATE
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_smactionsupdate(
	fname VARCHAR(128), 
	fversion VARCHAR(32), 
	pkid INT) RETURNS INTEGER AS $$
	
BEGIN

	IF (SELECT status FROM t_smqueue WHERE packageid = pkid) = 'COMPLETED' THEN
		
		PERFORM
			p_purgewaitingorders(i.filetype, i.validitystart, i.validitystop)
		FROM 
			t_inventory i
		WHERE 
			filename = fname 
			AND fileversion = fversion ;
		
	END IF;

	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	P_POSTINQUEUE_INSERT
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_postinvqueue_insert(
	in_fname VARCHAR(128), 
	in_fversion VARCHAR(32)) RETURNS INTEGER AS $$
	
BEGIN
	
	INSERT INTO t_postinvqueue (
		filename,
		fileversion,
		filetype,
		validitystart,
		validitystop)
	SELECT 
		i.filename,
		i.fileversion,
		i.filetype,
		i.validitystart,
		i.validitystop
	FROM 
		t_inventory i
	WHERE 
		filename = in_fname 
		AND fileversion = in_fversion ;
	
	RETURN 0;
	
END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	P_POSTINVQUEUE_MANAGER
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_postinvqueue_manager(
	in_secs INTEGER) RETURNS INTEGER AS $$

DECLARE
	p_secs INTERVAL;
	p_date TIMESTAMP(6);
	fcall  INTEGER;
	row    RECORD;

BEGIN
	p_secs := (SELECT in_secs * '1 second'::interval);
	p_date := (SELECT now() - p_secs) ;
	
	CREATE TEMP TABLE temppostinvqueue (
		ftype VARCHAR(16), 
		valstart TIMESTAMP(6), 
		valstop TIMESTAMP(6)) ;
	
	INSERT INTO temppostinvqueue(
		SELECT filetype, validitystart, validitystop
			FROM t_postinvqueue
				WHERE tstamp < p_date);
	
	IF ((SELECT count(*) FROM temppostinvqueue) > 0) THEN
		CREATE TEMP TABLE waitordtorembypost (orderid INT) ;
		
		PERFORM
			p_purgewaitingorders_act('POSTINV', tp.ftype, tp.valstart, tp.valstop) 
		FROM
			temppostinvqueue tp ;
		
		fcall := p_purgewaitingorders_fix('POSTINV');
				
		DROP TABLE waitordtorembypost ;
		DELETE FROM t_postinvqueue WHERE tstamp < p_date;
	END IF;
	
	DROP TABLE temppostinvqueue;
	
	RETURN 0;
	
END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	p_smqueueevent
*/ ------------------------------
-- Changed only p_inserteventtableitem to p_events
CREATE OR REPLACE FUNCTION p_smqueueevent(
	in_packageid INTEGER, 
	pre_status VARCHAR(32), 
	post_status VARCHAR(32), 
	operation VARCHAR(8)) RETURNS INTEGER AS $$
	
DECLARE
  	err INTEGER ;
	
BEGIN
	err := p_smstatetransition(in_packageid, pre_status, post_status) ;
	err := p_events('t_smqueue', operation) ;
	
  	RETURN 0 ;
END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
--------------------------- TRIGGER FUNCTIONS CREATION --------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	tf_inventory_quality_update
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_inventory_quality_update() RETURNS TRIGGER AS $$

DECLARE
  	status INTEGER ;

BEGIN	
	IF (OLD.qualityinfo = 'DUP' AND (NEW.qualityinfo = '' OR NEW.qualityinfo IS NULL) AND NEW.validityflag = 't') THEN
		status := p_generatetmporder('t', OLD.filename, OLD.fileversion, OLD.filetype, OLD.fileclass, OLD.satelliteid) ;
	END IF ;
	
	RETURN NEW ;

END ;
$$ LANGUAGE plpgsql ;


/* ------------------------------
	tf_align_filetypescomps
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_align_filetypescomps() RETURNS TRIGGER AS $$

DECLARE
	new_pfiletype_id INTEGER ;
	new_cfiletype_id INTEGER ;
	old_pfiletype_id INTEGER ;
	old_cfiletype_id INTEGER ;
	
BEGIN	
	IF (TG_ARGV[0] = 'INS') THEN
		new_pfiletype_id := (SELECT id_filetype FROM t_filetypes WHERE filetype = NEW.pfiletype) ;
		new_cfiletype_id := (SELECT id_filetype FROM t_filetypes WHERE filetype = NEW.cfiletype) ;
		INSERT INTO t_filetypescomps (pfiletype, cfiletype) VALUES (new_pfiletype_id, new_cfiletype_id) ;
		
		RETURN NEW ;
	END IF ;
	
	IF (TG_ARGV[0] = 'UPD') THEN
		new_pfiletype_id := (SELECT id_filetype FROM t_filetypes WHERE filetype = NEW.pfiletype) ;
		old_pfiletype_id := (SELECT id_filetype FROM t_filetypes WHERE filetype = OLD.pfiletype) ;
		new_cfiletype_id := (SELECT id_filetype FROM t_filetypes WHERE filetype = NEW.cfiletype) ;
		old_cfiletype_id := (SELECT id_filetype FROM t_filetypes WHERE filetype = OLD.cfiletype) ;
		UPDATE t_filetypescomps SET pfiletype = new_pfiletype_id, cfiletype = new_cfiletype_id
			WHERE pfiletype = old_pfiletype_id AND  cfiletype = old_cfiletype_id ;
		
		RETURN NEW ;
	END IF ;
	
	IF (TG_ARGV[0] = 'DEL') THEN
		old_pfiletype_id := (SELECT id_filetype FROM t_filetypes WHERE filetype = OLD.pfiletype) ;
		old_cfiletype_id := (SELECT id_filetype FROM t_filetypes WHERE filetype = OLD.cfiletype) ;
		DELETE FROM t_filetypescomps WHERE pfiletype = old_pfiletype_id AND cfiletype = old_cfiletype_id ;
		
		RETURN OLD ;
	END IF ;
END ;
$$ LANGUAGE plpgsql ;


/* ------------------------------
	tf_upd_distributionqueue_retry
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_upd_distributionqueue_retry() RETURNS TRIGGER AS $$

BEGIN
  	IF (NEW.status = 'ELIGIBLE') THEN
		NEW.retrycounter := 0 ;
	END IF ;
	
	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
------------------------- TRIGGER FUNCTIONS MODIFICATION ------------------------
---------------------------------------------------------------------------------

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


/* ------------------------------
	tf_ins_p_distributionstate
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_ins_p_distributionstate() RETURNS TRIGGER AS $$
DECLARE
  	status INTEGER ;
BEGIN
  	status := p_distribution_manager(NEW.packageid, NEW.status, NEW.status) ;
  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;


/* ------------------------------
	tf_upd_p_distributionstate
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_upd_p_distributionstate() RETURNS TRIGGER AS $$
DECLARE
  	res INTEGER ;
BEGIN
  	res := p_distribution_manager(OLD.packageid, OLD.status, NEW.status) ;
	PERFORM p_events('t_distributionqueue', 'UPD') ;
  	
	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;


/* ------------------------------
	tf_inventoryinsevent
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_inventoryinsevent() RETURNS TRIGGER AS $$

DECLARE
  	status INTEGER ;
	
BEGIN
	status := p_inventory_manager(NEW.filename, NEW.fileversion, NULL, NEW.validityflag, NEW.filetype, NEW.fileclass, NEW.orderid, NEW.satelliteid, NEW.sensorid, NEW.mission, NEW.validitystart, NEW.validitystop) ;
	
	IF NEW.validityflag = 't' THEN
		status := p_filetypes_last_inv_update(NEW.filetype) ;
	END IF;			
  	
  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;


/* ------------------------------
	tf_inventoryupdevent
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_inventoryupdevent() RETURNS TRIGGER AS $$

DECLARE
  	status INTEGER ;

BEGIN
	status := p_inventory_manager(NEW.filename, NEW.fileversion, OLD.validityflag, NEW.validityflag, NEW.filetype, NEW.fileclass, NEW.orderid, NEW.satelliteid, NEW.sensorid, NEW.mission, NEW.validitystart, NEW.validitystop) ;
	
	status := p_insert_inventoryevents(NEW.filename, NEW.fileversion, OLD.validityflag, NEW.validityflag) ;
	
	IF ((NEW.validityflag != OLD.validityflag) AND (NEW.validityflag = 't')) THEN
		status := p_filetypes_last_inv_update(OLD.filetype) ;
	END IF;	
	
  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	tf_ordersqueueevents
*/ ------------------------------
DROP TRIGGER tr_ordersqueueinsert ON t_ordersqueue;
DROP TRIGGER tr_ordersqueueupdate ON t_ordersqueue;
DROP FUNCTION tf_ordersqueueeventandcheckchanges();

CREATE OR REPLACE FUNCTION tf_ordersqueueevents() RETURNS TRIGGER AS $$
DECLARE
  	status INTEGER ;
BEGIN
  	IF (TG_ARGV[0] = 'INS') THEN
	  status := p_orders_manager(NEW.orderid, NULL, NEW.status, 'INS') ;
	ELSIF (TG_ARGV[0] = 'UPD') THEN
	  status := p_orders_manager(OLD.orderid, OLD.status, NEW.status, 'UPD') ;
	  status := p_orderqueuecheckchanges(OLD.orderid, OLD.status, NEW.status, OLD.processorname, NEW.processorname, OLD.processorversion, NEW.processorversion, OLD.processorconfiguration, NEW.processorconfiguration, OLD.starttime, NEW.starttime, OLD.stoptime, NEW.stoptime, OLD.ordertype, NEW.ordertype, OLD.satelliteid, NEW.satelliteid, OLD.mission, NEW.mission) ;
  	ELSE
	  RETURN NEW ;
	END IF ;

  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;


/* ------------------------------
	tf_distributionitemsevent
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_distributionitemsevent() RETURNS TRIGGER AS $$
DECLARE
  	status INTEGER ;
	evttype VARCHAR[10] ;
BEGIN
  	status := p_events(TG_ARGV[0], TG_ARGV[1]) ;
	IF (TG_ARGV[1] = 'INS') THEN
	  evttype := 'INSERT' ;
	ELSIF (TG_ARGV[1] = 'UPD') THEN
	  evttype := 'UPDATE' ;
	ELSE
	  evttype := TG_ARGV[1] ;
	END IF ;

  	--status := p_insert_inventoryevents(NEW.filename, NEW.fileversion, evttype, NEW.validityflag) ;

  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;


/* ------------------------------
	tf_waitingordersinsert
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_waitingordersinsert() RETURNS TRIGGER AS $$
DECLARE
  	status INTEGER ;
BEGIN
  	status := p_events(NEW.orderid::VARCHAR(64), TG_ARGV[0]) ;
  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;


/* ------------------------------
	tf_smactionsupdate
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_smactionsupdate() RETURNS TRIGGER AS $$
DECLARE
  	status INTEGER ;
BEGIN
  	status := p_smactionsupdate(NEW.filename, NEW.fileversion, NEW.packageid) ;
	status := p_events('t_smactions', 'UPD') ;
	
  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;


/* ------------------------------
	tf_cartsinvtrmgr
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_cartsinvtrmgr() RETURNS TRIGGER AS $$

DECLARE
  	status INTEGER ;

BEGIN
  	status := p_cartsinvcheck(NEW.id, NEW.cart, NEW.subreq, NEW.distsubreq, NEW.ordsubreq, NEW.stndsubreq ) ;
	
	IF (TG_ARGV[0] = 'INS') THEN
		status := p_events('t_cartsinv' ,'INS') ;
	END IF ;
	
	IF (TG_ARGV[0] = 'UPD') THEN
		status := p_events('t_cartsinv' ,'UPD') ;
  	END IF ;

  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;


/* ------------------------------
	tf_filetype_insert
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_filetype_insert() RETURNS TRIGGER AS $$
DECLARE
  	status INTEGER ;
BEGIN
	INSERT INTO t_filetypes_last_inv (filetype) VALUES (NEW.filetype) ;
	status := p_events('t_filetypes', 'INS') ;
  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;













---------------------------------------------------------------------------------
----------------------------- TRIGGER MODIFICATION ------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_ordersqueue
*/ ------------------------------
CREATE TRIGGER tr_ordersqueueinsert AFTER INSERT
  ON t_ordersqueue
  	FOR EACH ROW 
		EXECUTE PROCEDURE tf_ordersqueueevents('INS') ;

CREATE TRIGGER tr_ordersqueueupdate AFTER UPDATE
  ON t_ordersqueue
  	FOR EACH ROW 
		EXECUTE PROCEDURE tf_ordersqueueevents('UPD');


/* ------------------------------
	t_distributionqueue
*/ ------------------------------
DROP TRIGGER IF EXISTS tr_distributionqueueupdate1 ON t_distributionqueue ;
DROP TRIGGER IF EXISTS tr_distributionqueueupdate2 ON t_distributionqueue ;

CREATE TRIGGER tr_distributionqueueupdate1 BEFORE UPDATE
	ON t_distributionqueue
		FOR EACH ROW EXECUTE PROCEDURE tf_upd_distributionqueue_retry() ;

CREATE TRIGGER tr_distributionqueueupdate2 AFTER UPDATE
	ON t_distributionqueue
		FOR EACH ROW EXECUTE PROCEDURE tf_upd_p_distributionstate() ;








---------------------------------------------------------------------------------
------------------------------- TRIGGERS CREATION -------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_inventory
*/ ------------------------------
CREATE TRIGGER tr_inventory_quality_update AFTER UPDATE OF qualityinfo
	ON t_inventory 
		FOR EACH ROW EXECUTE PROCEDURE tf_inventory_quality_update() ;


/* ------------------------------
	t_filetypescomp
*/ ------------------------------
CREATE TRIGGER tr_filetypescompinsert AFTER INSERT
	ON t_filetypescomp 
		FOR EACH ROW EXECUTE PROCEDURE tf_align_filetypescomps('INS') ;

CREATE TRIGGER tr_filetypescompupdate AFTER UPDATE
	ON t_filetypescomp 
		FOR EACH ROW EXECUTE PROCEDURE tf_align_filetypescomps('UPD') ;

CREATE TRIGGER tr_filetypescompdelete BEFORE DELETE
	ON t_filetypescomp 
		FOR EACH ROW EXECUTE PROCEDURE tf_align_filetypescomps('DEL') ;


/* ------------------------------
	t_receptionrules
*/ ------------------------------
CREATE TRIGGER tr_receptionrulesinsert AFTER INSERT
	ON t_receptionrules 
		FOR EACH ROW EXECUTE PROCEDURE tf_inserteventtableitem('t_receptionrules' ,'INS' ) ;

CREATE TRIGGER tr_receptionrulesupdate AFTER UPDATE
	ON t_receptionrules 
		FOR EACH ROW EXECUTE PROCEDURE tf_inserteventtableitem('t_receptionrules' ,'UPD' ) ;








---------------------------------------------------------------------------------
------------------------------------- VIEWS -------------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	v_distribution
*/ ------------------------------
CREATE OR REPLACE VIEW v_distribution (
	packageid, 
	status, 
	distributeddate, 
	jobresponsible, 
	creationdate, 
	mediatype, 
	userid)
AS SELECT DISTINCT 
	dq.packageid, 
	dq.status, 
	dq.distributeddate, 
	dq.jobresponsible, 
	dq.creationdate, 
	di.mediatype, 
	di.userid,
	di.distributionrule_id
FROM 
	t_distributionqueue dq 
	INNER JOIN t_distributionitems di
		ON dq.packageid = di.packageid
;


/* ------------------------------
	v_distribution
*/ ------------------------------
CREATE OR REPLACE VIEW v_distributionrules (
	filetype, 
	username, 
	usermode,
	mediatype,
	repository_host,
	repository_prot,
	distributionalgo, 
	tasktable,
	fileclass,
	active)
AS SELECT  
	ft.filetype,
	us.userid,
	um.mode,
	mt.mediatype,
	rp.hostname,
	rp.protocol,
	da.name,
	tt.name,
	fc.fileclass,
	dr.active 
FROM 
	t_distributionrules dr 
	INNER JOIN t_filetypes ft
		ON dr.filetype_id = ft.id_filetype
	INNER JOIN t_users us
		ON dr.user_id = us.id_user
	INNER JOIN t_usermodes um
		ON dr.usermode_id = um.id
	INNER JOIN t_mediatypes mt
		ON dr.mediatype_id = mt.id_mediatype
	INNER JOIN t_repositories rp
		ON dr.repository_id = rp.id_repository
	INNER JOIN t_distributionalgo da
		ON dr.distributionalgo_id = da.id
	INNER JOIN t_tasktables tt
		ON dr.format_tt_id = tt.id_tasktable
	LEFT JOIN t_fileclasses fc
		ON dr.fileclass_id = fc.id_fileclass	
ORDER BY
	dr.filetype_id,
	dr.user_id
;








---------------------------------------------------------------------------------
---------------------------------- TABLES DROP ----------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_storagelocation
*/ ------------------------------
DROP TABLE t_storagelocation CASCADE ;

/* ------------------------------
	t_ordergenerationsatmatrix
*/ ------------------------------
DROP TABLE t_ordergenerationsatmatrix CASCADE ;








---------------------------------------------------------------------------------
-------------------------------- PROCEDURES DROP --------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	p_inserteventtableitem
*/ ------------------------------
DROP FUNCTION p_inserteventtableitem(VARCHAR,VARCHAR) ;

/* ------------------------------
	p_distribute_standard_inventory
*/ ------------------------------
DROP FUNCTION p_distribute_standard_inventory(VARCHAR,VARCHAR,INTEGER,VARCHAR) ;

/* ------------------------------
	p_distributionstate
*/ ------------------------------
DROP FUNCTION p_distributionstate(INT,VARCHAR,VARCHAR);

/* ------------------------------
	p_distribute_item_wdate
*/ ------------------------------
DROP FUNCTION p_distribute_item_wdate(VARCHAR,VARCHAR,VARCHAR,VARCHAR,VARCHAR,VARCHAR,INTEGER,INT2) ;

/* ------------------------------
	p_insertinventory
*/ ------------------------------
DROP FUNCTION p_insertinventory(VARCHAR,VARCHAR,BOOLEAN,VARCHAR,VARCHAR,INTEGER,SMALLINT,SMALLINT,SMALLINT,TIMESTAMP,TIMESTAMP) ;

/* ------------------------------
	p_inventory_update
*/ ------------------------------
DROP FUNCTION p_inventory_update(BOOLEAN,VARCHAR,VARCHAR,BOOLEAN,VARCHAR,VARCHAR,INTEGER,SMALLINT,SMALLINT, SMALLINT,TIMESTAMP,TIMESTAMP) ;

/* ------------------------------
	p_orderqueueevent
*/ ------------------------------
DROP FUNCTION p_orderqueueevent(INTEGER,VARCHAR,VARCHAR,VARCHAR) ;

/* ------------------------------
	p_orderstatetransition
*/ ------------------------------
DROP FUNCTION p_orderstatetransition (INTEGER,VARCHAR,VARCHAR) ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------
INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-11.0.0',
		'2013-03-13',
		3,
		2,
		'PDS_11-0-0',
		'Added new tables t_tasktables, t_repositories, t_receptionrules and t_receptionruleshist. Modified tables t_mediatypes, t_users and t_filetypes. Dropped table t_storagelocation.'
) ;


