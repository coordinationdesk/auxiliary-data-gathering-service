
-- Version 01.02.00
-- POSTGRES





-------------------------------------------------------------------------------
------------------------------- TABLES CREATION -------------------------------
-------------------------------------------------------------------------------

/* ------------------------------
	t_ltadl_retrievequeue
*/ ------------------------------
CREATE TABLE t_ltadl_retrievequeue (
    id SERIAL NOT NULL,
	filename VARCHAR(256),
	filter_query TEXT,
	jobresponsible VARCHAR(64) NOT NULL,
	status_id BIGINT NOT NULL,
	destination_url VARCHAR(1024) NOT NULL,
	sel_algo_id BIGINT NOT NULL,
	lta_id BIGINT NOT NULL,
	filequeue_id BIGINT,
	batchqueue_id BIGINT,
	creationdate TIMESTAMP(3) DEFAULT now(),
	startdate TIMESTAMP(3), 
	modificationdate TIMESTAMP(3) NOT NULL DEFAULT now(),
	completiondate TIMESTAMP(3), 
	remote_retriever_url VARCHAR(1024)	
);
ALTER TABLE t_ltadl_retrievequeue 
	ADD CONSTRAINT pk_t_ltadl_retrievequeue
		PRIMARY KEY (id) ;

ALTER TABLE t_ltadl_retrievequeue 
	ADD CONSTRAINT ck_ltadl_retrievequeue01 
		CHECK ((filename IS NOT NULL AND filter_query IS NOT NULL) OR (filename IS NULL AND filter_query IS NULL)) ;

ALTER TABLE t_ltadl_retrievequeue 
	ADD CONSTRAINT fk_ltadl_retrievequeue01
		FOREIGN KEY (status_id) 
			REFERENCES t_srv_statuses (id) 
				ON DELETE CASCADE ;

ALTER TABLE t_ltadl_retrievequeue 
	ADD CONSTRAINT fk_ltadl_retrievequeue02
		FOREIGN KEY (sel_algo_id) 
			REFERENCES t_ltadl_aip_sel_algos (id) 
				ON DELETE CASCADE ;

ALTER TABLE t_ltadl_retrievequeue 
	ADD CONSTRAINT fk_ltadl_retrievequeue03
		FOREIGN KEY (lta_id) 
			REFERENCES t_ltadl_remote_lta (id) 
				ON DELETE CASCADE ;

ALTER TABLE t_ltadl_retrievequeue 
	ADD CONSTRAINT fk_ltadl_retrievequeue04
		FOREIGN KEY (filequeue_id) 
			REFERENCES t_ltadl_filequeue (id) 
				ON DELETE CASCADE ;

ALTER TABLE t_ltadl_retrievequeue 
	ADD CONSTRAINT fk_ltadl_retrievequeue05
		FOREIGN KEY (batchqueue_id) 
			REFERENCES t_ltadl_batchqueue (id) 
				ON DELETE CASCADE ;

CREATE INDEX ix_t_ltadl_retrievequeue01 ON t_ltadl_retrievequeue USING BTREE (status_id) ;
CREATE INDEX ix_t_ltadl_retrievequeue02 ON t_ltadl_retrievequeue USING BTREE (sel_algo_id) ;
CREATE INDEX ix_t_ltadl_retrievequeue03 ON t_ltadl_retrievequeue USING BTREE (lta_id) ;
CREATE INDEX ix_t_ltadl_retrievequeue04 ON t_ltadl_retrievequeue USING BTREE (filequeue_id) ;
CREATE INDEX ix_t_ltadl_retrievequeue05 ON t_ltadl_retrievequeue USING BTREE (batchqueue_id) ;
CREATE INDEX ix_t_ltadl_retrievequeue06 ON t_ltadl_retrievequeue USING BTREE (modificationdate) ;








-------------------------------------------------------------------------------
------------------------------- TABLES CHANGES --------------------------------
-------------------------------------------------------------------------------

/* ------------------------------
	t_ltadl_remote_lta
*/ ------------------------------
ALTER TABLE t_ltadl_remote_lta ADD attributes JSONB ;








-------------------------------------------------------------------------------
-------------------------------- TABLES INSERT --------------------------------
-------------------------------------------------------------------------------

/* ------------------------------
	t_statetransitions_tables
*/ ------------------------------
INSERT INTO t_statetransitions_tables (table_name) VALUES ('t_ltadl_retrievequeue') ;



/* ------------------------------
	t_statetransitions_allowed
*/ ------------------------------

-- t_ltadl_retrievequeue
----------------------
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_retrievequeue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_srv_statuses WHERE status = 'BOOKED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_retrievequeue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_srv_statuses WHERE status = 'ERROR')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_retrievequeue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'BOOKED'),
	(SELECT id FROM t_srv_statuses WHERE status = 'ERROR')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_retrievequeue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'BOOKED'),
	(SELECT id FROM t_srv_statuses WHERE status = 'IN_PROGRESS')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_retrievequeue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'IN_PROGRESS'),
	(SELECT id FROM t_srv_statuses WHERE status = 'ELIGIBLE')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_retrievequeue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'IN_PROGRESS'),
	(SELECT id FROM t_srv_statuses WHERE status = 'COMPLETED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_retrievequeue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'IN_PROGRESS'),
	(SELECT id FROM t_srv_statuses WHERE status = 'ERROR')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_retrievequeue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'ERROR'),
	(SELECT id FROM t_srv_statuses WHERE status = 'ELIGIBLE')) ;








--------------------------------------------------------------------------------
-------------------------- TRIGGER FUNCTIONS CREATION --------------------------
--------------------------------------------------------------------------------

/* --------------------------
	tf_ltadl_retrievequeue_reset_fields
*/ --------------------------
CREATE OR REPLACE FUNCTION tf_ltadl_retrievequeue_reset_fields() RETURNS TRIGGER AS $$

BEGIN
	IF (OLD.status_id = (SELECT id FROM t_srv_statuses WHERE status = 'ERROR') AND NEW.status_id = (SELECT id FROM t_srv_statuses WHERE status = 'ELIGIBLE')) THEN
		UPDATE t_ltadl_retrievequeue SET 
			jobresponsible = NULL,
			startdate = NULL,
			modificationdate = now(),
			completiondate = NULL,
			remote_retriever_url = NULL,
			filequeue_id = NULL,
			batchqueue_id = NULL 
		WHERE id = OLD.id ;
	END IF ;
	
	RETURN NEW ;
	
END ;
$$ LANGUAGE plpgsql ;








-------------------------------------------------------------------------------
------------------------------- TRIGGER CREATION ------------------------------
-------------------------------------------------------------------------------

/* ------------------------------
	t_ltadl_retrievequeue 
*/ ------------------------------
CREATE TRIGGER tr_ltadl_retrievequeue_reset_fields AFTER UPDATE OF status_id
	ON t_ltadl_retrievequeue
		FOR EACH ROW EXECUTE PROCEDURE tf_ltadl_retrievequeue_reset_fields() ;








-------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS --------------------------------
-------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'ltadl-01.02.00' ;

INSERT INTO t_schema (
	version,
	modificationdate,
	modificationtype,
	schematype,
	cvstag,
	description )
VALUES  (
	'ltadl-01.02.00',
	'2021-04-15',
	6,
	37,
	'LTADL_1-2-0',
	'Added table t_ltadl_retrievequeue'
) ;


