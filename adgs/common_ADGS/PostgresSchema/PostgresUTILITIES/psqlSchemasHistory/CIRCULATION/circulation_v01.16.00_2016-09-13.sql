
-- Version 1.16.0
-- POSTGRES





-- #############################################################################
-- ##################                                         ##################
-- ##################                WARNING!!!               ##################
-- ##################   This patch can be installed only on   ##################
-- ##################       PDS 12.0.0 version or major       ##################
-- ##################                                         ##################
-- #############################################################################




--------------------------------------------------------------------------------
-------------------------------- TABLES INSERT ---------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_statetransitions_tables
*/ ------------------------------
INSERT INTO t_statetransitions_tables (table_name) VALUES ('t_dsqueue_x_cirqueue') ;


/* ------------------------------
	t_statetransitions_allowed
*/ ------------------------------

-- t_dsqueue_x_cirqueue
----------------------
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'ABORTED'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;
 
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'COMPLETED'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'DISTRIBUTION'),
	(SELECT id FROM t_stati WHERE status = 'COMPLETED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'DISTRIBUTION'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'DISTRIBUTION'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'DOWNLOADING'),
	(SELECT id FROM t_stati WHERE status = 'DISTRIBUTION')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'DOWNLOADING'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'DOWNLOADING'),
	(SELECT id FROM t_stati WHERE status = 'LTA_DOWNLOADING')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'DOWNLOADING'),
	(SELECT id FROM t_stati WHERE status = 'NOTFOUNDERROR')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'DOWNLOADING')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'UPLOADING')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'ABORTED')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'ERROR'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'ERROR'),
	(SELECT id FROM t_stati WHERE status = 'UPLOADED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'ERROR'),
	(SELECT id FROM t_stati WHERE status = 'UPLOADING')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'LTA_DOWNLOADING'),
	(SELECT id FROM t_stati WHERE status = 'DISTRIBUTION')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'LTA_DOWNLOADING'),
	(SELECT id FROM t_stati WHERE status = 'NOTFOUNDERROR')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'LTA_DOWNLOADING'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING'),
	(SELECT id FROM t_stati WHERE status = 'COMPLETED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'UPLOADED'),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'UPLOADED'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'UPLOADING'),
	(SELECT id FROM t_stati WHERE status = 'UPLOADED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'UPLOADING'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;








---------------------------------------------------------------------------------
------------------------- TRIGGER FUNCTION MODIFICATION -------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	tf_datastoragequeuetrans
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_dsqueuexcirqueuetrans() RETURNS TRIGGER AS $$
BEGIN
  	PERFORM p_statetransitions_check('t_dsqueue_x_cirqueue', 1, OLD.globalstatus_id, NEW.globalstatus_id);
	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
------------------------------- DROP OLD OBJECTS --------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	p_dsqueuexcirqueue_statetrans
*/ ------------------------------
DROP FUNCTION IF EXISTS p_dsqueuexcirqueue_statetrans(SMALLINT,SMALLINT);








--------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ---------------------------------
--------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'circulation-1.16.0' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'circulation-1.16.0',
		'2016-09-13',
		6,
		20,
		'CIRCULATION_1-16-0',
		'Added new state transitions for t_dsqueue_x_cirqueue. Changed state transition check using p_statetransitions_check. Dropped p_dsqueuexcirqueue_statetrans procedure.'
) ;


