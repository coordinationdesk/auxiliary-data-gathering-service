
-- Version 01.03.01
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
	(SELECT id FROM t_srv_statuses WHERE status = 'RUNNING'),
	(SELECT id FROM t_srv_statuses WHERE status = 'RUNNING')) ;


-- t_ltadl_filequeue
----------------------
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_filequeue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'OFFLINE'),
	(SELECT id FROM t_srv_statuses WHERE status = 'ERROR')) ;








-------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS --------------------------------
-------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'ltadl-01.03.01' ;

INSERT INTO t_schema (
	version,
	modificationdate,
	modificationtype,
	schematype,
	cvstag,
	description )
VALUES  (
	'ltadl-01.03.01',
	'2021-07-12',
	6,
	37,
	'LTADL_1-3-1',
	'Added new status transitions (LTA-299)'
) ;


