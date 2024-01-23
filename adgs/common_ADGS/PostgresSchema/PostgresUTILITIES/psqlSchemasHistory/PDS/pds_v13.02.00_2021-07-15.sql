
-- Version 13.02.00
-- POSTGRES








--------------------------------------------------------------------------------
-------------------------------- TABLES CREATION -------------------------------
--------------------------------------------------------------------------------

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

ALTER TABLE t_cdp_feederqueue 
	ADD CONSTRAINT ck_cdp_feederqueue01 
		CHECK ((cache_path IS NOT NULL AND s3_reference IS NULL AND inv_id IS NULL) 
			OR (cache_path IS NULL AND s3_reference IS NOT NULL AND inv_id IS NULL) 
			OR (cache_path IS NULL AND s3_reference IS NULL AND inv_id IS NOT NULL)) ;

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

CREATE INDEX ix_t_cdp_feederqueue01 ON t_cdp_feederqueue USING BTREE (creationdate);
CREATE INDEX ix_t_cdp_feederqueue02 ON t_cdp_feederqueue USING BTREE (retry_count);
CREATE INDEX ix_t_cdp_feederqueue03 ON t_cdp_feederqueue USING BTREE (retry_time);
CREATE INDEX ix_t_cdp_feederqueue04 ON t_cdp_feederqueue USING BTREE (status_id);
CREATE INDEX ix_t_cdp_feederqueue05 ON t_cdp_feederqueue USING BTREE (jobresponsible);
CREATE INDEX ix_t_cdp_feederqueue06 ON t_cdp_feederqueue USING BTREE (tempnamestrategy_id);




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

CREATE INDEX ix_t_cdp_notification_queue01 ON t_cdp_notification_queue USING BTREE (feederqueue_id);
CREATE INDEX ix_t_cdp_notification_queue02 ON t_cdp_notification_queue USING BTREE (creationdate);
CREATE INDEX ix_t_cdp_notification_queue03 ON t_cdp_notification_queue USING BTREE (retry_count);
CREATE INDEX ix_t_cdp_notification_queue04 ON t_cdp_notification_queue USING BTREE (retry_time);
CREATE INDEX ix_t_cdp_notification_queue05 ON t_cdp_notification_queue USING BTREE (status_id);
CREATE INDEX ix_t_cdp_notification_queue06 ON t_cdp_notification_queue USING BTREE (jobresponsible);








-------------------------------------------------------------------------------
-------------------------------- TABLES INSERT --------------------------------
-------------------------------------------------------------------------------

/* ------------------------------
	t_statetransitions_tables
*/ ------------------------------
INSERT INTO t_statetransitions_tables (table_name) VALUES ('t_cdp_feederqueue') ;
INSERT INTO t_statetransitions_tables (table_name) VALUES ('t_cdp_notification_queue') ;


/* ------------------------------
	t_statetransitions_allowed
*/ ------------------------------

-- t_cdp_feederqueue
----------------------
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_cdp_feederqueue'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'PROGRESS')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_cdp_feederqueue'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'RECOVERABLEERROR')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_cdp_feederqueue'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_cdp_feederqueue'),
	(SELECT id FROM t_stati WHERE status = 'PROGRESS'),
	(SELECT id FROM t_stati WHERE status = 'COMPLETED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_cdp_feederqueue'),
	(SELECT id FROM t_stati WHERE status = 'PROGRESS'),
	(SELECT id FROM t_stati WHERE status = 'RECOVERABLEERROR')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_cdp_feederqueue'),
	(SELECT id FROM t_stati WHERE status = 'PROGRESS'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_cdp_feederqueue'),
	(SELECT id FROM t_stati WHERE status = 'COMPLETED'),
	(SELECT id FROM t_stati WHERE status = 'RECOVERABLEERROR')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_cdp_feederqueue'),
	(SELECT id FROM t_stati WHERE status = 'COMPLETED'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_cdp_feederqueue'),
	(SELECT id FROM t_stati WHERE status = 'RECOVERABLEERROR'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_cdp_feederqueue'),
	(SELECT id FROM t_stati WHERE status = 'ERROR'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;


-- t_cdp_notification_queue
----------------------
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_cdp_notification_queue'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'BOOKED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_cdp_notification_queue'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'RECOVERABLEERROR')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_cdp_notification_queue'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_cdp_notification_queue'),
	(SELECT id FROM t_stati WHERE status = 'BOOKED'),
	(SELECT id FROM t_stati WHERE status = 'PROGRESS')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_cdp_notification_queue'),
	(SELECT id FROM t_stati WHERE status = 'BOOKED'),
	(SELECT id FROM t_stati WHERE status = 'RECOVERABLEERROR')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_cdp_notification_queue'),
	(SELECT id FROM t_stati WHERE status = 'BOOKED'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_cdp_notification_queue'),
	(SELECT id FROM t_stati WHERE status = 'PROGRESS'),
	(SELECT id FROM t_stati WHERE status = 'COMPLETED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_cdp_notification_queue'),
	(SELECT id FROM t_stati WHERE status = 'PROGRESS'),
	(SELECT id FROM t_stati WHERE status = 'RECOVERABLEERROR')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_cdp_notification_queue'),
	(SELECT id FROM t_stati WHERE status = 'PROGRESS'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_cdp_notification_queue'),
	(SELECT id FROM t_stati WHERE status = 'COMPLETED'),
	(SELECT id FROM t_stati WHERE status = 'RECOVERABLEERROR')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_cdp_notification_queue'),
	(SELECT id FROM t_stati WHERE status = 'COMPLETED'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_cdp_notification_queue'),
	(SELECT id FROM t_stati WHERE status = 'RECOVERABLEERROR'),
	(SELECT id FROM t_stati WHERE status = 'BOOKED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_cdp_notification_queue'),
	(SELECT id FROM t_stati WHERE status = 'RECOVERABLEERROR'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_cdp_notification_queue'),
	(SELECT id FROM t_stati WHERE status = 'ERROR'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'pds-13.02.00' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-13.02.00',
		'2021-07-15',
		3,
		2,
		'PDS_13-2-0',
		'Added new tables t_cdp_feederqueue and t_cdp_notification_queue'
) ;


