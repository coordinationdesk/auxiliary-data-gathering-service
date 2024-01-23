
-- Version 01.05.01
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
	(SELECT id FROM t_srv_statuses WHERE status = 'COMPLETED'),
	(SELECT id FROM t_srv_statuses WHERE status = 'COMPLETED')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_batchqueue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'CANCELLED'),
	(SELECT id FROM t_srv_statuses WHERE status = 'CANCELLED')) ;

DELETE FROM t_statetransitions_allowed 
	WHERE table_id = (SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_batchqueue') 
	AND prev_state_id = (SELECT id FROM t_srv_statuses WHERE status = 'SUBMITTED')
	AND post_state_id = (SELECT id FROM t_srv_statuses WHERE status = 'SUBMITTED');








-------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS --------------------------------
-------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'ltadl-01.05.01' ;

INSERT INTO t_schema (
	version,
	modificationdate,
	modificationtype,
	schematype,
	cvstag,
	description )
VALUES  (
	'ltadl-01.05.01',
	'2021-10-20',
	6,
	37,
	'LTADL_1-5-1',
	'Added new status transitions for t_ltadl_batchqueue'
) ;


