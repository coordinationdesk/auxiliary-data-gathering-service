
-- Version 01.03.00
-- POSTGRES





-------------------------------------------------------------------------------
------------------------------- TABLES CHANGES --------------------------------
-------------------------------------------------------------------------------

/* ------------------------------
	t_ltadl_filequeue
*/ ------------------------------
ALTER TABLE t_ltadl_filequeue ADD collapsed_to BIGINT ;
ALTER TABLE t_ltadl_filequeue ADD scheduled_download_time TIMESTAMP(3) DEFAULT NOW() ;

ALTER TABLE t_ltadl_filequeue 
	ADD CONSTRAINT fk_ltadl_filequeue04
		FOREIGN KEY (collapsed_to) 
			REFERENCES t_ltadl_filequeue (id) 
				ON DELETE CASCADE ;

CREATE INDEX ix_t_ltadl_filequeue06 ON t_ltadl_filequeue USING BTREE (collapsed_to) ;
CREATE INDEX ix_t_ltadl_filequeue07 ON t_ltadl_filequeue USING BTREE (scheduled_download_time) ;


/* ------------------------------
	t_ltadl_retrievequeue
*/ ------------------------------
ALTER TABLE t_ltadl_retrievequeue ADD list_tag VARCHAR(256) ;








-------------------------------------------------------------------------------
-------------------------------- TABLES INSERT --------------------------------
-------------------------------------------------------------------------------

/* ------------------------------
	t_srv_statuses
*/ ------------------------------
INSERT INTO t_srv_statuses (id, status) VALUES (30, 'COLLAPSED');


/* ------------------------------
	t_statetransitions_allowed
*/ ------------------------------

-- t_ltadl_filequeue
----------------------
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_filequeue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'OFFLINE'),
	(SELECT id FROM t_srv_statuses WHERE status = 'COLLAPSED')) ;

-- t_ltadl_batchqueue
----------------------
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_batchqueue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'ERROR'),
	(SELECT id FROM t_srv_statuses WHERE status = 'ERROR')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_batchqueue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'ERROR'),
	(SELECT id FROM t_srv_statuses WHERE status = 'RUNNING')) ;

-- t_ltadl_retrievequeue
----------------------
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_retrievequeue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'BOOKED'),
	(SELECT id FROM t_srv_statuses WHERE status = 'SUBMITTED')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_retrievequeue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'SUBMITTED'),
	(SELECT id FROM t_srv_statuses WHERE status = 'ONLINE')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_retrievequeue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'SUBMITTED'),
	(SELECT id FROM t_srv_statuses WHERE status = 'IN_PROGRESS')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_retrievequeue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'SUBMITTED'),
	(SELECT id FROM t_srv_statuses WHERE status = 'COMPLETED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_retrievequeue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'SUBMITTED'),
	(SELECT id FROM t_srv_statuses WHERE status = 'ELIGIBLE')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_retrievequeue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'SUBMITTED'),
	(SELECT id FROM t_srv_statuses WHERE status = 'ERROR')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_retrievequeue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'SUBMITTED'),
	(SELECT id FROM t_srv_statuses WHERE status = 'CANCELLED')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_retrievequeue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'ONLINE'),
	(SELECT id FROM t_srv_statuses WHERE status = 'IN_PROGRESS')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_retrievequeue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'ONLINE'),
	(SELECT id FROM t_srv_statuses WHERE status = 'COMPLETED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_retrievequeue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'ONLINE'),
	(SELECT id FROM t_srv_statuses WHERE status = 'ERROR')) ;

DELETE FROM t_statetransitions_allowed 
	WHERE table_id = (SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_retrievequeue') 
	AND prev_state_id = (SELECT id FROM t_srv_statuses WHERE status = 'IN_PROGRESS')
	AND post_state_id = (SELECT id FROM t_srv_statuses WHERE status = 'ELIGIBLE');
DELETE FROM t_statetransitions_allowed 
	WHERE table_id = (SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_retrievequeue') 
	AND prev_state_id = (SELECT id FROM t_srv_statuses WHERE status = 'BOOKED')
	AND post_state_id = (SELECT id FROM t_srv_statuses WHERE status = 'IN_PROGRESS');








--------------------------------------------------------------------------------
------------------------------ FUNCTIONS CREATION ------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	f_calculate_batch_status
*/ ------------------------------
CREATE OR REPLACE FUNCTION f_calculate_batch_status(in_batch_id BIGINT) RETURNS VARCHAR AS $$
DECLARE
	res INT ;
BEGIN
	IF (SELECT count(*) FROM t_ltadl_filequeue f INNER JOIN t_ltadl_batchqueue b ON f.ltadl_batch_id = b.id
		WHERE b.id = in_batch_id AND b.status_id = (SELECT id FROM t_srv_statuses WHERE status = 'ERROR')) > 0 THEN
		
		RETURN 'ERROR' ;
	
	ELSIF (SELECT count(*) FROM t_ltadl_filequeue f INNER JOIN t_ltadl_batchqueue b ON f.ltadl_batch_id = b.id
		WHERE b.id = in_batch_id AND b.status_id = (SELECT id FROM t_srv_statuses WHERE status = 'CANCELLED')) > 0 THEN
		
		RETURN 'CANCELLED' ;
		
	ELSIF (SELECT count(*) FROM t_ltadl_filequeue f INNER JOIN t_ltadl_batchqueue b ON f.ltadl_batch_id = b.id
		WHERE b.id = in_batch_id AND b.status_id IN (SELECT id FROM t_srv_statuses WHERE status IN ('BOOKED','TRANSFERRING'))) > 0 THEN
		
		RETURN 'RUNNING' ;
		
	ELSIF (SELECT count(*) FROM t_ltadl_filequeue WHERE ltadl_batch_id = in_batch_id) = (
		SELECT count(*) FROM t_ltadl_filequeue WHERE ltadl_batch_id = in_batch_id AND status_id = (SELECT id FROM t_srv_statuses WHERE status = 'COMPLETED')) THEN
		
		RETURN 'COMPLETED' ;
		
	ELSE
		RETURN 'SUBMITTED' ;
		
	END IF ;
	
END ;
$$ LANGUAGE plpgsql ;








--------------------------------------------------------------------------------
-------------------------- TRIGGER FUNCTIONS CREATION --------------------------
--------------------------------------------------------------------------------

/* --------------------------
	tf_ltadl_filequeue_update_batch_status
*/ --------------------------
CREATE OR REPLACE FUNCTION tf_ltadl_filequeue_update_batch_status() RETURNS TRIGGER AS $$

BEGIN
	UPDATE t_ltadl_batchqueue SET status_id = (
		SELECT id FROM t_srv_statuses WHERE status = (
			SELECT f_calculate_batch_status(NEW.ltadl_batch_id))) ;
	
	RETURN NEW ;
	
END ;
$$ LANGUAGE plpgsql ;








--------------------------------------------------------------------------------
------------------------------- TRIGGER CREATION -------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_ltadl_filequeue 
*/ ------------------------------
CREATE TRIGGER tr_ltadl_filequeue_update_status AFTER UPDATE OF status_id
	ON t_ltadl_filequeue
		FOR EACH ROW 
		WHEN (NEW.ltadl_batch_id IS NOT NULL)
		EXECUTE PROCEDURE tf_ltadl_filequeue_update_batch_status() ;








-------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS --------------------------------
-------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'ltadl-01.03.00' ;

INSERT INTO t_schema (
	version,
	modificationdate,
	modificationtype,
	schematype,
	cvstag,
	description )
VALUES  (
	'ltadl-01.03.00',
	'2021-05-20',
	6,
	37,
	'LTADL_1-3-0',
	'(LTA-299)'
) ;


