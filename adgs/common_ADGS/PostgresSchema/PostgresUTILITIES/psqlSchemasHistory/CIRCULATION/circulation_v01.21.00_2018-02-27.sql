
-- Version 1.21.0
-- POSTGRES








--------------------------------------------------------------------------------
-------------------------------- TABLES INSERT ---------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_statetransitions_allowed
*/ ------------------------------

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 5),
	(SELECT id FROM t_stati WHERE status = 'NOTFOUNDERROR'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 5),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;
	
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 5),
	(SELECT id FROM t_stati WHERE status = 'DOWNLOADING'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 5),
	(SELECT id FROM t_stati WHERE status = 'DISTRIBUTION'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;	

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 5),
	(SELECT id FROM t_stati WHERE status = 'LTA_DOWNLOADING'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;



INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 2),
	(SELECT id FROM t_stati WHERE status = 'UPLOADING'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;		

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 2),
	(SELECT id FROM t_stati WHERE status = 'UPLOADED'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;			

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 2),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;			



INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 4),
	(SELECT id FROM t_stati WHERE status = 'UPLOADING'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;		

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 4),
	(SELECT id FROM t_stati WHERE status = 'UPLOADED'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;			

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 4),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;			



INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 3),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 3),
	(SELECT id FROM t_stati WHERE status = 'DELETING'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;	





	
--------------------------------------------------------------------------------
-------------------------------- TRIGGER CHANGES -------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	tf_datastoragequeuetrans
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_datastoragequeuetrans() RETURNS TRIGGER AS $$
BEGIN
	IF (SELECT datastoragetype FROM t_datastoragetypes WHERE id = NEW.type_id) = 'ARCHIVE'THEN
  		PERFORM p_statetransitions_check('t_datastoragequeue', (SELECT id FROM t_statetransitions_flows WHERE flow_type = 'ARCHIVE'), OLD.status_id, NEW.status_id);
	END IF ;
	
	IF (SELECT datastoragetype FROM t_datastoragetypes WHERE id = NEW.type_id) = 'DELETE' THEN
  		PERFORM p_statetransitions_check('t_datastoragequeue', (SELECT id FROM t_statetransitions_flows WHERE flow_type = 'DELETE'), OLD.status_id, NEW.status_id);
	END IF ;
	
	IF (SELECT datastoragetype FROM t_datastoragetypes WHERE id = NEW.type_id) = 'LTA_ARCHIVE' THEN
  		PERFORM p_statetransitions_check('t_datastoragequeue', (SELECT id FROM t_statetransitions_flows WHERE flow_type = 'LTA_ARCHIVE'), OLD.status_id, NEW.status_id);
	END IF ;
	
	IF (SELECT datastoragetype FROM t_datastoragetypes WHERE id = NEW.type_id) = 'RETRIEVE' THEN
  		PERFORM p_statetransitions_check('t_datastoragequeue', (SELECT id FROM t_statetransitions_flows WHERE flow_type = 'RETRIEVE'), OLD.status_id, NEW.status_id);
	END IF ;
	
	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;


DROP TRIGGER IF EXISTS tr_datastoragequeue_update1 ON t_datastoragequeue ;
CREATE TRIGGER tr_datastoragequeue_update1
  BEFORE UPDATE OF status_id
  ON t_datastoragequeue
  FOR EACH ROW
  EXECUTE PROCEDURE tf_datastoragequeuetrans();







--------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ---------------------------------
--------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'circulation-1.21.0' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'circulation-1.21.0',
		'2018-02-27',
		6,
		20,
		'CIRCULATION_1-21-0',
		'Fixed bug on tf_datastoragequeuetrans'
) ;

