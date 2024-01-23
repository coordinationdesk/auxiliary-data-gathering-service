
-- Version 01.03.02
-- POSTGRES





-------------------------------------------------------------------------------
-------------------------------- TABLES INSERT --------------------------------
-------------------------------------------------------------------------------

/* ------------------------------
	t_statetransitions_allowed
*/ ------------------------------

-- t_ltadl_retrievequeue
----------------------
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_retrievequeue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'SUBMITTED'),
	(SELECT id FROM t_srv_statuses WHERE status = 'NOTFOUNDERROR')) ;








-------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS --------------------------------
-------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'ltadl-01.03.02' ;

INSERT INTO t_schema (
	version,
	modificationdate,
	modificationtype,
	schematype,
	cvstag,
	description )
VALUES  (
	'ltadl-01.03.02',
	'2021-09-08',
	6,
	37,
	'LTADL_1-3-2',
	'Added new status transitions (LTA-299)'
) ;


