
-- Version 01.05.03
-- POSTGRES





-------------------------------------------------------------------------------
-------------------------------- TABLES INSERT --------------------------------
-------------------------------------------------------------------------------

/* ------------------------------
	t_statetransitions_allowed
*/ ------------------------------

-- t_ltadl_filequeue
----------------------
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_filequeue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'ONLINE'),
	(SELECT id FROM t_srv_statuses WHERE status = 'RECOVERABLEERROR')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_filequeue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'BOOKED'),
	(SELECT id FROM t_srv_statuses WHERE status = 'RECOVERABLEERROR')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_filequeue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'TRANSFERRING'),
	(SELECT id FROM t_srv_statuses WHERE status = 'RECOVERABLEERROR')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_filequeue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'RECOVERABLEERROR'),
	(SELECT id FROM t_srv_statuses WHERE status = 'BOOKED')) ;








-------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS --------------------------------
-------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'ltadl-01.05.03' ;

INSERT INTO t_schema (
	version,
	modificationdate,
	modificationtype,
	schematype,
	cvstag,
	description )
VALUES  (
	'ltadl-01.05.03',
	'2021-11-25',
	6,
	37,
	'LTADL_1-5-3',
	'Added new status transitions for t_ltadl_filequeue (S2PDGS-581)'
) ;


