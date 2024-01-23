
-- Version 01.05.02
-- POSTGRES





-------------------------------------------------------------------------------
-------------------------------- TABLES INSERT --------------------------------
-------------------------------------------------------------------------------

/* ------------------------------
	t_statetransitions_allowed
*/ ------------------------------

-- t_ltadl_batchqueue
----------------------
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_batchqueue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'SUBMITTED'),
	(SELECT id FROM t_srv_statuses WHERE status = 'ERROR')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_batchqueue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'SUBMITTED'),
	(SELECT id FROM t_srv_statuses WHERE status = 'CANCELLED')) ;








-------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS --------------------------------
-------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'ltadl-01.05.02' ;

INSERT INTO t_schema (
	version,
	modificationdate,
	modificationtype,
	schematype,
	cvstag,
	description )
VALUES  (
	'ltadl-01.05.02',
	'2021-10-21',
	6,
	37,
	'LTADL_1-5-2',
	'Added new status transitions for t_ltadl_batchqueue'
) ;


