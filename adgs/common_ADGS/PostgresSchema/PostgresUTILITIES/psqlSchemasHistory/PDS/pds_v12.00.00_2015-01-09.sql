
-- Version 12.0.0
-- POSTGRES



-- #############################################################################
-- ##################                                          #################
-- ##################                 WARNING!!!               #################
-- ##################      Before install this schema the      #################
-- ##################  PL/Python - Python Procedural Language  #################
-- ##################                is needed                 #################
-- ##################                                          #################
-- #############################################################################

/*
	How to install the plpythonu language:
	- Be sure that the specific contrib is installed on the machine
	- on Postgres 9.0 or minor execute sql command:
		CREATE OR REPLACE LANGUAGE plpythonu ;
	- on Postgres 9.1 or major execute sql command:
		CREATE EXTENSION IF NOT EXISTS plpythonu ;
*/







--------------------------------------------------------------------------------
------------------------------ EXTENSION CREATION ------------------------------
--------------------------------------------------------------------------------

-- CREATE OR REPLACE LANGUAGE plpythonu ;
CREATE EXTENSION IF NOT EXISTS plpythonu ;








--------------------------------------------------------------------------------
------------------------------ FUNCTIONS CREATION ------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	f_get_text_from_file
*/ ------------------------------
CREATE OR REPLACE FUNCTION f_get_text_from_file(fname TEXT)
	RETURNS TEXT AS $$

try:
	f = open(fname, "r")
except IOError:
	print 'ERROR!!! Cannot open file:', fname
	raise

dataTxt = f.read()
f.close()

return dataTxt

$$ LANGUAGE plpythonu;



/* ------------------------------
	f_interval_to_seconds
*/ ------------------------------
CREATE OR REPLACE FUNCTION f_interval_to_seconds(
	in_intrv INTERVAL
) RETURNS DOUBLE PRECISION AS $$

DECLARE
	p_sec DOUBLE PRECISION ;

BEGIN
	p_sec := EXTRACT(EPOCH  FROM in_intrv) ;
	
	RETURN p_sec ;
END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	f_interval_to_seconds_diff
*/ ------------------------------
CREATE OR REPLACE FUNCTION f_interval_to_seconds_diff(
	beg_ts TIMESTAMP(6),
	end_ts TIMESTAMP(6)
) RETURNS DOUBLE PRECISION AS $$

DECLARE
	p_sec DOUBLE PRECISION ;

BEGIN
	p_sec := EXTRACT(EPOCH  FROM (end_ts - beg_ts)) ;
	
	RETURN p_sec ;
END ;
$$ LANGUAGE plpgsql ;








--------------------------------------------------------------------------------
-------------------------------- TABLES CREATION -------------------------------
--------------------------------------------------------------------------------

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

ALTER TABLE t_applications 
	ADD CONSTRAINT uq_applications01
		UNIQUE (app_name) ;


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

ALTER TABLE t_applicationevents 
	ADD CONSTRAINT uq_applicationevents01
		UNIQUE (application_id, tstamp) ;

ALTER TABLE t_applicationevents 
	ADD CONSTRAINT fk_applicationevents0l
		FOREIGN KEY (application_id)
			REFERENCES t_applications (id_application)
				ON DELETE CASCADE ;

CREATE INDEX ix_t_applicationevents01 ON t_applicationevents USING BTREE (application_id) ;
CREATE INDEX ix_t_applicationevents02 ON t_applicationevents USING BTREE (tstamp) ;


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

ALTER TABLE t_localtransfermodes 
	ADD CONSTRAINT uq_localtransfermodes01
		UNIQUE (localtransfermode) ;

COMMENT ON TABLE t_localtransfermodes IS 'Contains the list of possible local data transfer methods';
COMMENT ON COLUMN t_localtransfermodes.id IS 'Record identifier (Primary Key)';
COMMENT ON COLUMN t_localtransfermodes.localtransfermode IS 'The type of transfer mode (eg: COPY, HARDLINK...)';

INSERT INTO t_localtransfermodes (id, localtransfermode) VALUES (1, 'COPY') ;
INSERT INTO t_localtransfermodes (id, localtransfermode) VALUES (2, 'HARDLINK') ;
INSERT INTO t_localtransfermodes (id, localtransfermode) VALUES (3, 'SYMLINK') ;


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

ALTER TABLE t_statetransitions_flows 
	ADD CONSTRAINT uq_statetransitions_flows01
		UNIQUE (flow_type) ;

COMMENT ON TABLE t_statetransitions_flows IS 'Contains the list of possible data flows for tables where the status change is checked';
COMMENT ON COLUMN t_statetransitions_flows.id IS 'Record identifier (Primary Key)';
COMMENT ON COLUMN t_statetransitions_flows.flow_type IS 'The type of flow (eg: RETRIEVE, ARCHIVE...). "none" where there are no different flows';


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

ALTER TABLE t_statetransitions_tables 
	ADD CONSTRAINT uq_statetransitions_tables01
		UNIQUE (table_name, flow_id) ;

ALTER TABLE t_statetransitions_tables 
	ADD CONSTRAINT fk_statetransitions_tables01
		FOREIGN KEY (flow_id)
			REFERENCES t_statetransitions_flows (id)
				ON DELETE CASCADE ;

CREATE INDEX ix_t_statetransitions_tables01 ON t_statetransitions_tables USING BTREE (flow_id) ;

COMMENT ON TABLE t_statetransitions_tables IS 'Contains the list of tables having the "status" field to analyze';
COMMENT ON COLUMN t_statetransitions_tables.id IS 'Record identifier (Primary Key)';
COMMENT ON COLUMN t_statetransitions_tables.table_name IS 'The name of the table';
COMMENT ON COLUMN t_statetransitions_tables.flow_id IS 'This field is used when in a table there may be the same status transitions, but in different flows. The default is 1 that mean "none". References the t_statetransitions_flows table';


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

ALTER TABLE t_statetransitions_allowed 
	ADD CONSTRAINT uq_statetransitions_allowed01
		UNIQUE (table_id, prev_state_id, post_state_id) ;

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

CREATE INDEX ix_t_statetrans_allowed01 ON t_statetransitions_allowed USING BTREE (table_id) ;
CREATE INDEX ix_t_statetrans_allowed02 ON t_statetransitions_allowed USING BTREE (prev_state_id) ;
CREATE INDEX ix_t_statetrans_allowed03 ON t_statetransitions_allowed USING BTREE (post_state_id) ;

COMMENT ON TABLE t_statetransitions_allowed IS 'Contains the list of admitted transitions for each table';
COMMENT ON COLUMN t_statetransitions_allowed.id IS 'Record identifier (Primary Key)';
COMMENT ON COLUMN t_statetransitions_allowed.table_id IS 'Is the identifier of the table to which the status change is associated. References the t_statetransitions_tables table';
COMMENT ON COLUMN t_statetransitions_allowed.prev_state_id IS 'Is the identifier to the previous value of the changing status. References the t_stati table';
COMMENT ON COLUMN t_statetransitions_allowed.post_state_id IS 'Is the identifier to the new value of the changing status. References the t_stati table';








--------------------------------------------------------------------------------
-------------------------------- TABLES CHANGES --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_ordersattachments
*/ ------------------------------
ALTER TABLE t_ordersattachments ALTER COLUMN attachname DROP NOT NULL ;
ALTER TABLE t_ordersattachments ALTER COLUMN attachname DROP DEFAULT ;


/* ------------------------------
	t_repositories
*/ ------------------------------
ALTER TABLE t_repositories ALTER COLUMN password DROP NOT NULL ;
ALTER TABLE t_repositories ALTER COLUMN password DROP DEFAULT ;

ALTER TABLE t_repositories ALTER COLUMN remotepath DROP DEFAULT ;

ALTER TABLE t_repositories ALTER COLUMN allowedciphers DROP NOT NULL ;
ALTER TABLE t_repositories ALTER COLUMN allowedciphers DROP DEFAULT ;


/* ------------------------------
	t_receptionrules
*/ ------------------------------
ALTER TABLE t_receptionrules ALTER COLUMN successnotificationaddress TYPE VARCHAR(1024) ;


/* ------------------------------
	t_smactions
*/ ------------------------------
CREATE INDEX ix_t_smactions04 ON t_smactions USING BTREE (filename) ;
CREATE INDEX ix_t_smactions05 ON t_smactions USING BTREE (fileversion) ;


/* ------------------------------
	t_smqueue
*/ ------------------------------
CREATE INDEX ix_t_smqueue02 ON t_smqueue USING BTREE (status) ;








---------------------------------------------------------------------------------
--------------------------------- TABLES INSERT ---------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_satellites
*/ ------------------------------
INSERT INTO t_satellites (satelliteid, satellitename, satelliteacronym) VALUES (43, 'Csg', 'CSGS') ;


/* ------------------------------
	t_satrelations
*/ ------------------------------
-- CosmoSG 1
INSERT INTO t_satrelations VALUES (43, 1, 18) ;
-- CosmoSG 2
INSERT INTO t_satrelations VALUES (43, 2, 18) ;


/* ------------------------------
	t_statetransitions_flows
*/ ------------------------------
INSERT INTO t_statetransitions_flows (id, flow_type) VALUES (1, 'none') ;
INSERT INTO t_statetransitions_flows (id, flow_type) VALUES (2, 'ARCHIVE') ;
INSERT INTO t_statetransitions_flows (id, flow_type) VALUES (3, 'DELETE') ;
INSERT INTO t_statetransitions_flows (id, flow_type) VALUES (4, 'LTA_ARCHIVE') ;
INSERT INTO t_statetransitions_flows (id, flow_type) VALUES (5, 'RETRIEVE') ;


/* ------------------------------
	t_statetransitions_tables
*/ ------------------------------
INSERT INTO t_statetransitions_tables (table_name) VALUES ('t_distributionqueue') ;
INSERT INTO t_statetransitions_tables (table_name) VALUES ('t_ordersqueue') ;
INSERT INTO t_statetransitions_tables (table_name) VALUES ('t_smqueue') ;


/* ------------------------------
	t_statetransitions_allowed
*/ ------------------------------

-- t_distributionqueue
----------------------
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_distributionqueue'),
	(SELECT id FROM t_stati WHERE status = 'COMPLETED'),
	(SELECT id FROM t_stati WHERE status = 'ABORTED')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_distributionqueue'),
	(SELECT id FROM t_stati WHERE status = 'DISTRIBUTION'),
	(SELECT id FROM t_stati WHERE status = 'COMPLETED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_distributionqueue'),
	(SELECT id FROM t_stati WHERE status = 'DISTRIBUTION'),
	(SELECT id FROM t_stati WHERE status = 'WAITINGACK')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_distributionqueue'),
	(SELECT id FROM t_stati WHERE status = 'DISTRIBUTION'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_distributionqueue'),
	(SELECT id FROM t_stati WHERE status = 'DISTRIBUTION'),
	(SELECT id FROM t_stati WHERE status = 'RECOVERABLEERROR')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_distributionqueue'),
	(SELECT id FROM t_stati WHERE status = 'DISTRIBUTION'),
	(SELECT id FROM t_stati WHERE status = 'ABORTED')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_distributionqueue'),
	(SELECT id FROM t_stati WHERE status = 'DOWNLOADING'),
	(SELECT id FROM t_stati WHERE status = 'DOWNLOADING')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_distributionqueue'),
	(SELECT id FROM t_stati WHERE status = 'DOWNLOADING'),
	(SELECT id FROM t_stati WHERE status = 'DISTRIBUTION')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_distributionqueue'),
	(SELECT id FROM t_stati WHERE status = 'DOWNLOADING'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_distributionqueue'),
	(SELECT id FROM t_stati WHERE status = 'DOWNLOADING'),
	(SELECT id FROM t_stati WHERE status = 'RECOVERABLEERROR')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_distributionqueue'),
	(SELECT id FROM t_stati WHERE status = 'DOWNLOADING'),
	(SELECT id FROM t_stati WHERE status = 'ABORTED')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_distributionqueue'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_distributionqueue'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'DOWNLOADING')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_distributionqueue'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'SUSPENDED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_distributionqueue'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'ABORTED')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_distributionqueue'),
	(SELECT id FROM t_stati WHERE status = 'SUSPENDED'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_distributionqueue'),
	(SELECT id FROM t_stati WHERE status = 'SUSPENDED'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_distributionqueue'),
	(SELECT id FROM t_stati WHERE status = 'SUSPENDED'),
	(SELECT id FROM t_stati WHERE status = 'RECOVERABLEERROR')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_distributionqueue'),
	(SELECT id FROM t_stati WHERE status = 'SUSPENDED'),
	(SELECT id FROM t_stati WHERE status = 'ABORTED')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_distributionqueue'),
	(SELECT id FROM t_stati WHERE status = 'TRANSIENT'),
	(SELECT id FROM t_stati WHERE status = 'TRANSIENT')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_distributionqueue'),
	(SELECT id FROM t_stati WHERE status = 'TRANSIENT'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_distributionqueue'),
	(SELECT id FROM t_stati WHERE status = 'TRANSIENT'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_distributionqueue'),
	(SELECT id FROM t_stati WHERE status = 'TRANSIENT'),
	(SELECT id FROM t_stati WHERE status = 'RECOVERABLEERROR')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_distributionqueue'),
	(SELECT id FROM t_stati WHERE status = 'TRANSIENT'),
	(SELECT id FROM t_stati WHERE status = 'ABORTED')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_distributionqueue'),
	(SELECT id FROM t_stati WHERE status = 'RECOVERABLEERROR'),
	(SELECT id FROM t_stati WHERE status = 'DOWNLOADING')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_distributionqueue'),
	(SELECT id FROM t_stati WHERE status = 'RECOVERABLEERROR'),
	(SELECT id FROM t_stati WHERE status = 'SUSPENDED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_distributionqueue'),
	(SELECT id FROM t_stati WHERE status = 'RECOVERABLEERROR'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_distributionqueue'),
	(SELECT id FROM t_stati WHERE status = 'RECOVERABLEERROR'),
	(SELECT id FROM t_stati WHERE status = 'ABORTED')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_distributionqueue'),
	(SELECT id FROM t_stati WHERE status = 'WAITINGACK'),
	(SELECT id FROM t_stati WHERE status = 'WAITINGACK')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_distributionqueue'),
	(SELECT id FROM t_stati WHERE status = 'WAITINGACK'),
	(SELECT id FROM t_stati WHERE status = 'COMPLETED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_distributionqueue'),
	(SELECT id FROM t_stati WHERE status = 'WAITINGACK'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_distributionqueue'),
	(SELECT id FROM t_stati WHERE status = 'WAITINGACK'),
	(SELECT id FROM t_stati WHERE status = 'RECOVERABLEERROR')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_distributionqueue'),
	(SELECT id FROM t_stati WHERE status = 'WAITINGACK'),
	(SELECT id FROM t_stati WHERE status = 'ABORTED')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_distributionqueue'),
	(SELECT id FROM t_stati WHERE status = 'ERROR'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_distributionqueue'),
	(SELECT id FROM t_stati WHERE status = 'ERROR'),
	(SELECT id FROM t_stati WHERE status = 'ABORTED')) ;


-- t_ordersqueue
----------------------
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ordersqueue'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ordersqueue'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'PRECONDITIONCHECK')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ordersqueue'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'ABORTED')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ordersqueue'),
	(SELECT id FROM t_stati WHERE status = 'ERROR'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ordersqueue'),
	(SELECT id FROM t_stati WHERE status = 'ERROR'),
	(SELECT id FROM t_stati WHERE status = 'COMPLETED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ordersqueue'),
	(SELECT id FROM t_stati WHERE status = 'ERROR'),
	(SELECT id FROM t_stati WHERE status = 'ABORTED')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ordersqueue'),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ordersqueue'),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING'),
	(SELECT id FROM t_stati WHERE status = 'RECOVERABLEERROR')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ordersqueue'),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING'),
	(SELECT id FROM t_stati WHERE status = 'COMPLETED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ordersqueue'),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING'),
	(SELECT id FROM t_stati WHERE status = 'ABORTED')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ordersqueue'),
	(SELECT id FROM t_stati WHERE status = 'DOWNLOADING'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ordersqueue'),
	(SELECT id FROM t_stati WHERE status = 'DOWNLOADING'),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ordersqueue'),
	(SELECT id FROM t_stati WHERE status = 'DOWNLOADING'),
	(SELECT id FROM t_stati WHERE status = 'ABORTED')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ordersqueue'),
	(SELECT id FROM t_stati WHERE status = 'PRECONDITIONCHECK'),
	(SELECT id FROM t_stati WHERE status = 'DOWNLOADING')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ordersqueue'),
	(SELECT id FROM t_stati WHERE status = 'PRECONDITIONCHECK'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ordersqueue'),
	(SELECT id FROM t_stati WHERE status = 'PRECONDITIONCHECK'),
	(SELECT id FROM t_stati WHERE status = 'ABORTED')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ordersqueue'),
	(SELECT id FROM t_stati WHERE status = 'RECOVERABLEERROR'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ordersqueue'),
	(SELECT id FROM t_stati WHERE status = 'RECOVERABLEERROR'),
	(SELECT id FROM t_stati WHERE status = 'ABORTED')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ordersqueue'),
	(SELECT id FROM t_stati WHERE status = 'STANDBY'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ordersqueue'),
	(SELECT id FROM t_stati WHERE status = 'STANDBY'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ordersqueue'),
	(SELECT id FROM t_stati WHERE status = 'STANDBY'),
	(SELECT id FROM t_stati WHERE status = 'ABORTED')) ;


-- t_smqueue
----------------------
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_smqueue'),
	(SELECT id FROM t_stati WHERE status = 'STANDBY'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_smqueue'),
	(SELECT id FROM t_stati WHERE status = 'STANDBY'),
	(SELECT id FROM t_stati WHERE status = 'STANDBY')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_smqueue'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'PREPARING')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_smqueue'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'SUBMITTED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_smqueue'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_smqueue'),
	(SELECT id FROM t_stati WHERE status = 'PREPARING'),
	(SELECT id FROM t_stati WHERE status = 'SUBMITTED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_smqueue'),
	(SELECT id FROM t_stati WHERE status = 'PREPARING'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_smqueue'),
	(SELECT id FROM t_stati WHERE status = 'PREPARING'),
	(SELECT id FROM t_stati WHERE status = 'PREPARING')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_smqueue'),
	(SELECT id FROM t_stati WHERE status = 'SUBMITTED'),
	(SELECT id FROM t_stati WHERE status = 'WAITINGACK')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_smqueue'),
	(SELECT id FROM t_stati WHERE status = 'SUBMITTED'),
	(SELECT id FROM t_stati WHERE status = 'ACKNOWLEDGED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_smqueue'),
	(SELECT id FROM t_stati WHERE status = 'SUBMITTED'),
	(SELECT id FROM t_stati WHERE status = 'COMPLETED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_smqueue'),
	(SELECT id FROM t_stati WHERE status = 'SUBMITTED'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_smqueue'),
	(SELECT id FROM t_stati WHERE status = 'SUBMITTED'),
	(SELECT id FROM t_stati WHERE status = 'SUBMITTED')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_smqueue'),
	(SELECT id FROM t_stati WHERE status = 'ACKNOWLEDGED'),
	(SELECT id FROM t_stati WHERE status = 'UPLOADING')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_smqueue'),
	(SELECT id FROM t_stati WHERE status = 'ACKNOWLEDGED'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_smqueue'),
	(SELECT id FROM t_stati WHERE status = 'ACKNOWLEDGED'),
	(SELECT id FROM t_stati WHERE status = 'ACKNOWLEDGED')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_smqueue'),
	(SELECT id FROM t_stati WHERE status = 'UPLOADING'),
	(SELECT id FROM t_stati WHERE status = 'COMPLETED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_smqueue'),
	(SELECT id FROM t_stati WHERE status = 'UPLOADING'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_smqueue'),
	(SELECT id FROM t_stati WHERE status = 'UPLOADING'),
	(SELECT id FROM t_stati WHERE status = 'UPLOADING')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_smqueue'),
	(SELECT id FROM t_stati WHERE status = 'WAITINGACK'),
	(SELECT id FROM t_stati WHERE status = 'WAITINGACK')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_smqueue'),
	(SELECT id FROM t_stati WHERE status = 'WAITINGACK'),
	(SELECT id FROM t_stati WHERE status = 'ACKNOWLEDGED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_smqueue'),
	(SELECT id FROM t_stati WHERE status = 'WAITINGACK'),
	(SELECT id FROM t_stati WHERE status = 'COMPLETED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_smqueue'),
	(SELECT id FROM t_stati WHERE status = 'WAITINGACK'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_smqueue'),
	(SELECT id FROM t_stati WHERE status = 'ERROR'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_smqueue'),
	(SELECT id FROM t_stati WHERE status = 'ERROR'),
	(SELECT id FROM t_stati WHERE status = 'WAITINGACK')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_smqueue'),
	(SELECT id FROM t_stati WHERE status = 'ERROR'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;








---------------------------------------------------------------------------------
--------------------------------- TABLES UPDATE ---------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_dbparameters
*/ ------------------------------
DELETE FROM t_dbparameters WHERE parameter = 'app_even_expir_days' ;
INSERT INTO t_dbparameters 
	(parameter, parametertype_id, value_int, param_description) 
VALUES 
	('app_even_expir_days', 1, 30, 'Represents the number of days after which an application event must be removed.') ;








---------------------------------------------------------------------------------
------------------------------ PROCEDURES CREATION ------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	p_application_events
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_application_events(
	in_appname VARCHAR(64), 
	in_evn_type VARCHAR(64),
	in_evn_desc VARCHAR(256)) RETURNS INTEGER AS $$
	
DECLARE
	p_app_id INTEGER := NULL ;
	p_exp_days INTEGER := NULL ;
	
BEGIN
	-- ##### APPLICATION SECTION ##### --
	-- Get the id of the application
	SELECT id_application INTO p_app_id FROM t_applications WHERE app_name = in_appname ;
	
	-- If the application does not exist
	IF p_app_id IS NULL THEN
		-- insert new application into t_applications and get is id
		INSERT INTO t_applications (app_name) VALUES (in_appname) RETURNING id_application INTO p_app_id ;
	END IF ;
	
	-- ##### EVENT SECTION ##### --
	-- Insert new event into t_applicationevents
	INSERT INTO t_applicationevents (
		application_id,
		tstamp,
		event_type,
		event_description)
	VALUES (
		p_app_id,
		now(),
		in_evn_type,
		in_evn_desc) ;
	
	-- ##### CLEANING SECTION ##### --
	-- Clean t_applicationevents removing events olther than app_even_expir_days (t_dbparameters)
	
	-- get the value of app_even_expir_days
	SELECT value_int INTO p_exp_days FROM t_dbparameters WHERE parameter = 'app_even_expir_days' ;
	
	-- if the value is not setted
	IF p_exp_days IS NULL THEN
		-- raise error message
		RAISE EXCEPTION 'p_application_events: Cannot retrieve value for app_even_expir_days db_parameter.' ;
	END IF ;
	-- else... delete
	DELETE FROM t_applicationevents WHERE
		application_id = p_app_id
		AND tstamp < (now() - CAST(p_exp_days || 'days' AS interval)) ;
		
	RETURN 0;
	
END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	p_statetransitions_check
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_statetransitions_check(
	in_tname VARCHAR(30),
	in_flw INT,
	in_pre_st INT, 
	in_pst_st INT) RETURNS INTEGER AS $$

DECLARE
	tbl_id INTEGER;
	pre_st_name VARCHAR(32);
	pst_st_name VARCHAR(32);
	no_table_ex_msg TEXT;
	no_trans_ex_msg TEXT;

BEGIN
	-- Retrieve the id of the table using the table name and the flow
	SELECT id INTO tbl_id FROM t_statetransitions_tables WHERE table_name = in_tname AND flow_id = in_flw ;
	
	-- Retrieve status name by id (just for error message)
	SELECT status INTO pre_st_name FROM t_stati WHERE id = in_pre_st ;
	SELECT status INTO pst_st_name FROM t_stati WHERE id = in_pst_st ;
	
	
	-- Set no table error messages
	IF in_flw = 1 THEN
		no_table_ex_msg := 'p_StateTransitions_Check: table ' || in_tname || ' not regestired into t_statetransitions_tables table.' ; 
	ELSE
		no_table_ex_msg := 'p_StateTransitions_Check: table ' || in_tname || ' in flow ' || in_flw || ' not regestired into t_statetransitions_tables table.' ; 
	END IF ;
	
	-- Set no transition error messages
	IF in_flw = 1 THEN
		no_trans_ex_msg := 'p_StateTransitions_Check: table ' || in_tname || ' - ' || pre_st_name || ' to ' || pst_st_name || ' invalid state transition.' ; 
	ELSE
		no_trans_ex_msg := 'p_StateTransitions_Check: table ' || in_tname || ' in flow ' || in_flw || ' - ' || pre_st_name || ' to ' || pst_st_name || ' invalid state transition.' ; 
	END IF ;
	
	
	-- Check for registered table
	IF tbl_id IS NULL THEN
		RAISE EXCEPTION '%', no_table_ex_msg ;
	END IF ;
	
	-- Check for allowed transition
	IF (SELECT count(*) FROM t_statetransitions_allowed WHERE table_id = tbl_id AND prev_state_id = in_pre_st AND post_state_id = in_pst_st) = 0 THEN
		RAISE EXCEPTION '%', no_trans_ex_msg ;
	END IF ;	
	
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;



/* -------------------------------
	p_orderstatetransitions_mgr
*/ -------------------------------
CREATE OR REPLACE FUNCTION p_orderstatetransitions_mgr(
	in_ordid INTEGER, 
	in_pre_stat VARCHAR(32), 
	in_post_stat VARCHAR(32)) RETURNS INTEGER AS $$
	
DECLARE
  	comp_stat VARCHAR(32);
	
BEGIN
	-- First insert
	IF in_pre_stat IS NULL AND in_post_stat = 'ELIGIBLE' THEN
		comp_stat := 'NUL2ELI' ;
	ELSIF in_pre_stat IS NULL AND in_post_stat = 'STANDBY' THEN
		comp_stat := 'NUL2STA' ;
	ELSIF in_pre_stat IS NULL AND in_post_stat = 'ABORTED' THEN
		comp_stat := 'NUL2ABO' ;
	
	-- From eligible to ..
	ELSIF in_pre_stat = 'ELIGIBLE' AND in_post_stat = 'ELIGIBLE' THEN
		comp_stat := 'ELI2ELI' ;
	ELSIF in_pre_stat = 'ELIGIBLE' AND in_post_stat = 'PRECONDITIONCHECK' THEN
		comp_stat := 'ELI2PRE' ;
	ELSIF in_pre_stat = 'ELIGIBLE' AND in_post_stat = 'ABORTED' THEN
		comp_stat := '2ABO' ;
	
	-- From error to ..
	ELSIF in_pre_stat = 'ERROR' AND in_post_stat = 'ELIGIBLE' THEN
		comp_stat := 'ERR2ELI' ;
	ELSIF in_pre_stat = 'ERROR' AND in_post_stat = 'COMPLETED' THEN
		comp_stat := 'ERR2COM' ;
		PERFORM p_completed_order(in_ordid) ;
	ELSIF in_pre_stat = 'ERROR' AND in_post_stat = 'ABORTED' THEN
		comp_stat := '2ABO' ;
	
	-- From processing to ..
	ELSIF in_pre_stat = 'PROCESSING' AND in_post_stat = 'ERROR' THEN
		comp_stat := 'PRO2ERR' ;
	ELSIF in_pre_stat = 'PROCESSING' AND in_post_stat = 'RECOVERABLEERROR' THEN
		comp_stat := 'PRO2REC' ;
	ELSIF in_pre_stat = 'PROCESSING' AND in_post_stat = 'COMPLETED' THEN
		comp_stat := 'PRO2COM' ;
		PERFORM p_completed_order(in_ordid) ;
	ELSIF in_pre_stat = 'PROCESSING' AND in_post_stat = 'ABORTED' THEN
		comp_stat := '2ABO' ;
	
	-- From downloading to ..
	ELSIF in_pre_stat = 'DOWNLOADING' AND in_post_stat = 'PROCESSING' THEN
		comp_stat := 'DOW2PRO' ;
	ELSIF in_pre_stat = 'DOWNLOADING' AND in_post_stat = 'ERROR' THEN
		comp_stat := 'DOW2ERR' ;
	ELSIF in_pre_stat = 'DOWNLOADING' AND in_post_stat = 'ABORTED' THEN
		comp_stat := '2ABO' ;
		
	-- From preconditioncheck to ..
	ELSIF in_pre_stat = 'PRECONDITIONCHECK' AND in_post_stat = 'ELIGIBLE' THEN
		comp_stat := 'PRE2ELI' ;
	ELSIF in_pre_stat = 'PRECONDITIONCHECK' AND in_post_stat = 'DOWNLOADING' THEN
		comp_stat := 'PRE2DOW' ;
	ELSIF in_pre_stat = 'PRECONDITIONCHECK' AND in_post_stat = 'ABORTED' THEN
		comp_stat := '2ABO' ;
	
	-- From recoverableerror to ..
	ELSIF in_pre_stat = 'RECOVERABLEERROR' AND in_post_stat = 'ELIGIBLE' THEN
		comp_stat := 'REC2ELI' ;
	ELSIF in_pre_stat = 'RECOVERABLEERROR' AND in_post_stat = 'ABORTED' THEN
		comp_stat := '2ABO' ;
	
	-- From standby to ..
	ELSIF in_pre_stat = 'STANDBY' AND in_post_stat = 'ELIGIBLE' THEN
		comp_stat := 'STA2ELI' ;
	ELSIF in_pre_stat = 'STANDBY' AND in_post_stat = 'ERROR' THEN
		comp_stat := 'STA2ERR' ;
	ELSIF in_pre_stat = 'STANDBY' AND in_post_stat = 'ABORTED' THEN
		comp_stat := '2ABO' ;
	
	-- .. otherwise
	ELSE
		RAISE EXCEPTION 'p_orderstatetransitions_mgr: Cannot map the status transition.' ;
	END IF ;
	
	
	-- The following transition is not logged to avoid too much negligible transitions ELIGIBLE->PRECONDCHEDCK->ELIGIBLE
	IF comp_stat = 'PRE2ELI' OR comp_stat = 'ELI2PRE' THEN
		RETURN 0 ;
	END IF ;
	
	-- The following transition is logged only the first time to avoid too much negligible transitions ELIGIBLE->PRECONDCHEDCK->ELIGIBLE
	IF (comp_stat = 'ELI2ELI') THEN 
		IF ((SELECT COUNT(*) FROM t_orderstatetransitions WHERE orderid = in_ordid) = 0) THEN
			INSERT INTO t_orderstatetransitions (orderid, status) VALUES (in_ordid, in_post_stat) ;
		END IF ;
		RETURN 0 ;
	END IF ;
	
	-- Otherwise
	INSERT INTO t_orderstatetransitions (orderid, status) VALUES (in_ordid, in_post_stat) ;
	PERFORM p_events('t_orderstatetransitions', comp_stat) ;
	
	RETURN 0;
	
END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	p_generatetmporderpname2
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_generatetmporderpname2(
	in_active BOOLEAN,
	fname VARCHAR(128),
	fversion VARCHAR(32),
	ftype VARCHAR(16),
	fclass VARCHAR(8),
	in_satellite SMALLINT,
	tab_name VARCHAR(30),
	fld_name VARCHAR(30)) RETURNS INTEGER AS $$

DECLARE
	sql_cmd VARCHAR(4000) ;
	
BEGIN
	sql_cmd := '' ;
	sql_cmd := sql_cmd || 'INSERT INTO t_temporaryorders (' ;
	sql_cmd := sql_cmd || 	'filename, ' ;
	sql_cmd := sql_cmd || 	'fileversion, ' ;
	sql_cmd := sql_cmd || 	'filetype, ' ;
	sql_cmd := sql_cmd || 	'ordertype, ' ;
	sql_cmd := sql_cmd || 	'processorname, ' ;
	sql_cmd := sql_cmd || 	'processorversion, '  ;
	sql_cmd := sql_cmd || 	'algorithm, ' ;
	sql_cmd := sql_cmd || 	'processorconfiguration, ' ;
	sql_cmd := sql_cmd || 	'priority, ' ;
	sql_cmd := sql_cmd || 	'troubleshooting, ' ;
	sql_cmd := sql_cmd || 	'processingtimewindow, ' ;
	sql_cmd := sql_cmd || 	'ordersat) ' ;
	sql_cmd := sql_cmd || 'SELECT ' ;
	sql_cmd := sql_cmd ||	'''' || fname || ''', ' ;
	sql_cmd := sql_cmd ||	'''' || fversion || ''', ' ;
	sql_cmd := sql_cmd ||	'''' || ftype || ''', ' ;
	sql_cmd := sql_cmd ||	'og.orderType, ' ;
	sql_cmd := sql_cmd ||	'og.processorName, ' ;
	sql_cmd := sql_cmd ||	'og.processorVersion, ' ;
	sql_cmd := sql_cmd ||	'og.algorithm, ' ;
	sql_cmd := sql_cmd ||	'og.processorconfiguration, ' ;
	sql_cmd := sql_cmd ||	'og.priority, ' ;
	sql_cmd := sql_cmd ||	'og.troubleshooting, ' ;
	sql_cmd := sql_cmd ||	'og.processingtimewindow, ' ;
	sql_cmd := sql_cmd ||	in_satellite || ' ' ;
	sql_cmd := sql_cmd || 'FROM ' ;
	sql_cmd := sql_cmd ||	't_ordersgeneration og ' ;
	sql_cmd := sql_cmd || 'WHERE ' ;
	sql_cmd := sql_cmd ||	'og.filetype = ' || '''' || ftype || ''' ' ;
	sql_cmd := sql_cmd ||	'AND og.active = BOOL(' || '''' || in_active || ''') ' ;
	sql_cmd := sql_cmd ||	'AND (og.fileclass IS NULL OR og.fileclass = ' || '''' || fclass || ''') ' ;
	sql_cmd := sql_cmd ||	'AND og.processorname IN (SELECT ' || fld_name || ' FROM ' || tab_name || ') ' ;
	
	EXECUTE sql_cmd ;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
---------------------------- PROCEDURES MODIFICATION ----------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	p_checksatsensmiss
*/ ------------------------------
DROP FUNCTION IF EXISTS p_checksatsensmiss(SMALLINT,SMALLINT,SMALLINT) ;

CREATE OR REPLACE FUNCTION p_checksatsensmiss(
	in_sat SMALLINT, 
	in_sen SMALLINT, 
	in_mis SMALLINT) RETURNS INTEGER AS $$
	
DECLARE
	cnt INTEGER ;
	
BEGIN
	cnt := 0 ;
	
	IF in_sat IS NULL AND in_sen IS NULL AND in_mis IS NULL THEN
		RETURN 0 ;
	END IF ;
		
	IF in_sen IS NULL AND in_mis IS NULL THEN
		SELECT INTO cnt COUNT(*) FROM t_satrelations WHERE satelliteid = in_sat ;
	ELSIF in_sat IS NULL AND in_mis IS NULL THEN
		SELECT INTO cnt COUNT(*) FROM t_satrelations WHERE sensorid = in_sen ;
	ELSIF in_sat IS NULL AND in_sen IS NULL THEN
		SELECT INTO cnt COUNT(*) FROM t_satrelations WHERE mission = in_mis ;
	ELSIF in_mis IS NULL THEN
		SELECT INTO cnt COUNT(*) FROM t_satrelations WHERE satelliteid = in_sat AND sensorid = in_sen ;
	ELSIF in_sat IS NULL THEN
		SELECT INTO cnt COUNT(*) FROM t_satrelations WHERE sensorid = in_sen AND mission = in_mis ;
	ELSIF in_sen IS NULL THEN
		SELECT INTO cnt COUNT(*) FROM t_satrelations WHERE satelliteid = in_sat AND mission = in_mis ;
	ELSE
		SELECT INTO cnt COUNT(*) FROM t_satrelations WHERE satelliteid = in_sat AND sensorid = in_sen AND mission = in_mis ;
	END IF ;
	
	IF (cnt = 0) THEN
		RAISE EXCEPTION 'p_checksatsensmiss: Inconsistent Satellite/Sensor/Mission' ;
	END IF ;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	p_completed_order_distribution
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_completed_order_distribution(in_orderid INTEGER) RETURNS INTEGER AS $$

DECLARE
  	row_odd RECORD ;
  	
	-- the procedure distributes the output from a completed order
	
BEGIN
	--
	--	check if the users are already assigned to that order looking into t_ondemanddistribution
	--
	IF ((SELECT COUNT(*) FROM t_ondemanddistribution WHERE orderid = in_orderid) = 0) THEN
	--
	--	no predefined user : proceed with the configured ones
	--
		PERFORM p_distribute_order_inventory(in_orderid, NULL) ;
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
			(SELECT id_fileclass FROM t_fileclasses WHERE t_fileclasses.fileclass = inv.fileclass),
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
	p_orders_manager
*/ ------------------------------
DROP FUNCTION IF EXISTS p_orders_manager(INTEGER,VARCHAR,VARCHAR,VARCHAR) ;

CREATE OR REPLACE FUNCTION p_orders_manager(
	in_orderid INTEGER, 
	pre_status VARCHAR(32), 
	post_status VARCHAR(32)) RETURNS INTEGER AS $$
	
DECLARE
  	p_extordid VARCHAR(128);
	p_subreq INT;
	p_cartid INT;
	cartsinv_status VARCHAR(32);
	
BEGIN
	p_extordid := (SELECT externalorderid FROM t_ordersqueue WHERE orderid = in_orderid);
	p_subreq := (SELECT subreq FROM t_ordersqueue WHERE orderid = in_orderid);
	
	IF p_extordid IS NOT NULL AND p_subreq IS NOT NULL THEN
		p_cartid := (SELECT id FROM t_cartsref AS cr INNER JOIN t_ordersqueue AS oq ON oq.externalorderid = cr.extref WHERE oq.orderid = in_orderid);
		
		IF post_status = 'COMPLETED' THEN
			cartsinv_status := 'PROCESSED';
		ELSE
			cartsinv_status := post_status ;
		END IF ;
		
		UPDATE t_cartsinv SET status = cartsinv_status WHERE cart = p_cartid AND ordsubreq = p_subreq ;	
		PERFORM p_carts_update_status(p_cartid);
	END IF ;
	
	RETURN 0 ;

END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	p_remove_obsolete_actions
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_remove_obsolete_actions(
	fname VARCHAR(128), 
	fversion VARCHAR(32)) RETURNS INTEGER AS $$

DECLARE
	row RECORD;
	
BEGIN
	FOR row IN (
		SELECT 
			sma.packageid AS pkgid,
			smq.status AS stat, 
			smq.operation AS oper
		FROM 
			t_smactions sma
			INNER JOIN t_smqueue smq
				ON smq.packageid = sma.packageid
		WHERE 
			sma.filename = fname 
			AND sma.fileversion = fversion 
			AND sma.packageid IS NOT NULL 
			AND sma.operation = 'UPLOAD') LOOP
	
		IF (row.stat = 'ELIGIBLE' AND row.oper = 'UPLOAD') OR (row.stat = 'ERROR' AND row.oper = 'UPLOAD') THEN
			UPDATE t_smactions SET packageid = NULL WHERE packageid = row.pkgid ;
			DELETE FROM t_smqueue WHERE packageid = row.pkgid ;
		END IF ;
		
	END LOOP ;
	
	DELETE FROM t_smactions WHERE filename = fname AND fileversion = fversion AND operation = 'UPLOAD' ;
	
	DELETE FROM t_smqueue WHERE packageid IN (
		SELECT sq.packageid FROM t_smqueue sq
		LEFT JOIN t_smactions sa ON sq.packageid = sa.packageid
		WHERE sq.status = 'COMPLETED' AND sa.packageid IS NULL) ;

	RETURN 0 ;
END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
--------------------------- TRIGGER FUNCTION CREATION ---------------------------
---------------------------------------------------------------------------------

/* -------------------------------
	tf_distributionqueue_statcheck
*/ -------------------------------
CREATE OR REPLACE FUNCTION tf_distributionqueue_statcheck() RETURNS TRIGGER AS $$

DECLARE
	preStatId INT ;
	postStatId INT ;
	errMsg VARCHAR(128) ;
	
BEGIN
	IF (TG_ARGV[0] = 'UPD') THEN
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
		IF (NEW.status != 'ABORTED') OR (NEW.status != 'ELIGIBLE') OR (NEW.status != 'STANDBY') THEN
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
	IF (TG_ARGV[0] = 'INS') THEN
		PERFORM p_orderstatetransitions_mgr(NEW.orderid, NULL, NEW.status) ;
		PERFORM p_orders_manager(NEW.orderid, NEW.status, NEW.status) ;
		PERFORM p_events('t_ordersqueue', 'INS') ;
		RETURN NEW ;
	END IF ;
	
	IF (TG_ARGV[0] = 'UPD') THEN
		PERFORM p_orderstatetransitions_mgr(NEW.orderid, OLD.status, NEW.status) ;
		PERFORM p_orders_manager(OLD.orderid, OLD.status, NEW.status) ;
		PERFORM p_orderqueuecheckchanges(OLD.orderid, OLD.status, NEW.status, OLD.processorname, NEW.processorname, OLD.processorversion, NEW.processorversion, OLD.processorconfiguration, NEW.processorconfiguration, OLD.starttime, NEW.starttime, OLD.stoptime, NEW.stoptime, OLD.ordertype, NEW.ordertype, OLD.satelliteid, NEW.satelliteid, OLD.mission, NEW.mission) ;
		PERFORM p_events('t_ordersqueue', 'UPD') ;
		RETURN NEW ;
	END IF ;
	
	IF (TG_ARGV[0] = 'DEL') THEN
		PERFORM p_events('t_ordersqueue', 'DEL') ;
		RETURN OLD ;
	END IF ;
END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
------------------------- TRIGGER FUNCTION MODIFICATION -------------------------
---------------------------------------------------------------------------------

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
	tf_distributionqueue_manager
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_distributionqueue_manager() RETURNS TRIGGER AS $$

DECLARE
  	res INTEGER ;
	
BEGIN
	IF (TG_ARGV[0] = 'INS') THEN
		PERFORM p_events('t_distributionqueue', 'INS') ;
		res := p_cartsinv_update(NEW.packageid, NEW.status);
		RETURN NEW ;
	END IF ;
	
	IF (TG_ARGV[0] = 'UPD') THEN
		PERFORM p_events('t_distributionqueue', 'UPD') ;
		res := p_cartsinv_update(NEW.packageid, NEW.status);
		res := p_inv_dist_counter_by_queue(NEW.packageid, OLD.status, NEW.status, 'UPD') ;
		RETURN NEW ;
	END IF ;
	
	IF (TG_ARGV[0] = 'DEL') THEN
		res := p_inv_dist_counter_by_queue(OLD.packageid, OLD.status, NULL, 'DEL') ;
		PERFORM p_events('t_distributionqueue', 'DEL') ;
		RETURN OLD ;
	END IF ;
	
END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
------------------------------- TRIGGER CREATION --------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_distributionqueue
*/ ------------------------------
DROP TRIGGER IF EXISTS tr_distributionqueueinsert1 ON t_distributionqueue ;
DROP TRIGGER IF EXISTS tr_distributionqueueinsert2 ON t_distributionqueue ;
DROP TRIGGER IF EXISTS tr_distributionqueueupdate1 ON t_distributionqueue ;
DROP TRIGGER IF EXISTS tr_distributionqueueupdate2 ON t_distributionqueue ;
DROP TRIGGER IF EXISTS tr_distributionqueuedelete1 ON t_distributionqueue ;

CREATE TRIGGER tr_distributionqueue_ins_bfr BEFORE INSERT
	ON t_distributionqueue
		FOR EACH ROW EXECUTE PROCEDURE tf_distributionqueue_statcheck('INS') ;

CREATE TRIGGER tr_distributionqueue_ins_aft AFTER INSERT
	ON t_distributionqueue 
		FOR EACH ROW EXECUTE PROCEDURE tf_distributionqueue_manager('INS');

CREATE TRIGGER tr_distributionqueue_upd_bfr BEFORE UPDATE OF status
	ON t_distributionqueue
		FOR EACH ROW EXECUTE PROCEDURE tf_distributionqueue_statcheck('UPD') ;

CREATE TRIGGER tr_distributionqueue_upd_aft AFTER UPDATE
	ON t_distributionqueue
		FOR EACH ROW EXECUTE PROCEDURE tf_distributionqueue_manager('UPD');

CREATE TRIGGER tr_distributionqueue_del_aft AFTER DELETE
	ON t_distributionqueue
		FOR EACH ROW EXECUTE PROCEDURE tf_distributionqueue_manager('DEL');


/* ------------------------------
	t_smqueue
*/ ------------------------------
DROP TRIGGER IF EXISTS tr_smqueueinsert ON t_smqueue ;
DROP TRIGGER IF EXISTS tr_smqueueupdate ON t_smqueue ;

CREATE TRIGGER tr_smqueue_ins_bfr BEFORE INSERT
	ON t_smqueue
		FOR EACH ROW EXECUTE PROCEDURE tf_smqueue_checkchanges('INS') ;

CREATE TRIGGER tr_smqueue_ins_aft AFTER INSERT
	ON t_smqueue 
		FOR EACH ROW EXECUTE PROCEDURE tf_inserteventtableitem('t_smqueue', 'INS') ;

CREATE TRIGGER tr_smqueue_upd_bfr BEFORE UPDATE
	ON t_smqueue
		FOR EACH ROW EXECUTE PROCEDURE tf_smqueue_checkchanges('UPD') ;

CREATE TRIGGER tr_smqueue_upd_aft AFTER UPDATE
	ON t_smqueue
		FOR EACH ROW EXECUTE PROCEDURE tf_inserteventtableitem('t_smqueue', 'UPD') ;

CREATE TRIGGER tr_smqueue_del_aft AFTER DELETE
	ON t_smqueue
		FOR EACH ROW EXECUTE PROCEDURE tf_inserteventtableitem('t_smqueue', 'DEL') ;


/* ------------------------------
	t_ordersqueue
*/ ------------------------------
DROP TRIGGER IF EXISTS tr_ordersqueueinsert ON t_ordersqueue ;
DROP TRIGGER IF EXISTS tr_ordersqueueupdate ON t_ordersqueue ;

CREATE TRIGGER tr_ordersqueue_ins_bfr BEFORE INSERT
	ON t_ordersqueue
		FOR EACH ROW EXECUTE PROCEDURE tf_ordersqueue_statcheck('INS') ;

CREATE TRIGGER tr_ordersqueue_ins_aft AFTER INSERT
	ON t_ordersqueue 
		FOR EACH ROW EXECUTE PROCEDURE tf_ordersqueue_manager('INS');

CREATE TRIGGER tr_ordersqueue_upd_bfr BEFORE UPDATE
	ON t_ordersqueue
		FOR EACH ROW EXECUTE PROCEDURE tf_ordersqueue_statcheck('UPD') ;

CREATE TRIGGER tr_ordersqueue_upd_aft AFTER UPDATE
	ON t_ordersqueue
		FOR EACH ROW EXECUTE PROCEDURE tf_ordersqueue_manager('UPD');

CREATE TRIGGER tr_ordersqueue_del_aft AFTER DELETE
	ON t_ordersqueue
		FOR EACH ROW EXECUTE PROCEDURE tf_ordersqueue_manager('DEL');








---------------------------------------------------------------------------------
--------------------------------- VIEW CREATION ---------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	v_state_transitions
*/ ------------------------------
CREATE OR REPLACE VIEW v_state_transitions (
	table_name, 
	flow, 
	prev_status,
	post_status)
AS SELECT 
	st.table_name, 
	sf.flow_type  AS flow,
	ss1.status AS pre_s,
	ss2.status AS pst_s
FROM 
	t_statetransitions_tables AS st
	INNER JOIN t_statetransitions_flows AS sf 
		ON st.flow_id = sf.id
	INNER JOIN t_statetransitions_allowed AS sa 
		ON st.id = sa.table_id
	INNER JOIN t_stati AS ss1 
		ON sa.prev_state_id = ss1.id
	INNER JOIN t_stati AS ss2 
		ON sa.post_state_id = ss2.id
ORDER BY 
	table_name, 
	flow, 
	pre_s,
	pst_s
;







---------------------------------------------------------------------------------
------------------------------- DROP OLD OBJECTS --------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	tf_ins_p_distributionstate
*/ ------------------------------
DROP FUNCTION IF EXISTS tf_ins_p_distributionstate() ;

/* ------------------------------
	tf_upd_distributionqueue_retry
*/ ------------------------------
DROP FUNCTION IF EXISTS tf_upd_distributionqueue_retry() ;

/* ------------------------------
	tf_smqueueeventandcheckchanges
*/ ------------------------------
DROP FUNCTION IF EXISTS tf_smqueueeventandcheckchanges() ;

/* ------------------------------
	tf_ordersqueueevents
*/ ------------------------------
DROP FUNCTION IF EXISTS tf_ordersqueueevents() ;

/* ------------------------------
	p_distribution_manager
*/ ------------------------------
DROP FUNCTION IF EXISTS p_distribution_manager(INT, VARCHAR, VARCHAR) ;

/* ------------------------------
	p_smqueueevent
*/ ------------------------------
DROP FUNCTION IF EXISTS p_smqueueevent(INTEGER, VARCHAR, VARCHAR, VARCHAR) ;

/* ------------------------------
	p_smStateTransition
*/ ------------------------------
DROP FUNCTION IF EXISTS p_smStateTransition(INT, VARCHAR, VARCHAR) ;

/* ------------------------------
	t_filetypescomp
*/ ------------------------------
DROP TABLE IF EXISTS t_filetypescomp CASCADE ;

/* ------------------------------
	t_distributionpolicieslock
*/ ------------------------------
INSERT INTO t_distributionruleslock (packageid, responsible, tstamp)
	SELECT packageid, responsible, tstamp FROM t_distributionpolicieslock ;

DROP TABLE IF EXISTS t_distributionpolicieslock CASCADE ;








--------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ---------------------------------
--------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'pds-12.00.00' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-12.00.00',
		'2015-10-27',
		3,
		2,
		'PDS_12-0-0',
		'Added phyton language and the phyton function f_get_text_from_file to load the content of a file into a text file. Added new tables t_applications, t_applicationevents, t_localtransfermodes, t_statetransitions_flows, t_statetransitions_tables and t_statetransitions_allowed. Removed t_distributionpolicieslock and t_filetypescomp tables. Added new procedures p_application_events, p_statetransitions_check and p_orderstatetransitions_mgr. Changed p_orders_manager, trigger and trigger functions that calls new procedures.'
) ;


