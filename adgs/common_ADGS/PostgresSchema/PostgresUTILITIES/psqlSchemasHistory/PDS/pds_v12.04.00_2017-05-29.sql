
-- Version 12.04.00
-- POSTGRES







/* ------------------------------
   t_statetransitions_allowed
*/ ------------------------------

DELETE FROM t_statetransitions_allowed WHERE table_id = '1' and prev_state_id in ('7', '15') ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_distributionqueue'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_distributionqueue'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'PRECONDITIONCHECK')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_distributionqueue'),
	(SELECT id FROM t_stati WHERE status = 'RECOVERABLEERROR'),
	(SELECT id FROM t_stati WHERE status = 'PRECONDITIONCHECK')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_distributionqueue'),
	(SELECT id FROM t_stati WHERE status = 'PRECONDITIONCHECK'),
	(SELECT id FROM t_stati WHERE status = 'DOWNLOADING')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_distributionqueue'),
	(SELECT id FROM t_stati WHERE status = 'PRECONDITIONCHECK'),
	(SELECT id FROM t_stati WHERE status = 'SUSPENDED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_distributionqueue'),
	(SELECT id FROM t_stati WHERE status = 'PRECONDITIONCHECK'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_distributionqueue'),
	(SELECT id FROM t_stati WHERE status = 'PRECONDITIONCHECK'),
	(SELECT id FROM t_stati WHERE status = 'RECOVERABLEERROR')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_distributionqueue'),
	(SELECT id FROM t_stati WHERE status = 'PRECONDITIONCHECK'),
	(SELECT id FROM t_stati WHERE status = 'ABORTED')) ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'pds-12.04.00' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-12.04.00',
		'2017-05-29',
		3,
		2,
		'PDS_12-4-0',
		'Added new status PRECONDITIONCHECK, modified status ELIGIBLE and RECOVERABLEERROR on t_statetransitions_allowed.'
) ;
