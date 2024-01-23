
-- Version 1.18.0
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
INSERT INTO t_statetransitions_tables (table_name) VALUES ('t_circulationsqueue') ;
INSERT INTO t_statetransitions_tables (table_name, flow_id) VALUES ('t_datastoragequeue', 5) ;
INSERT INTO t_statetransitions_tables (table_name, flow_id) VALUES ('t_datastoragequeue', 2) ;
INSERT INTO t_statetransitions_tables (table_name, flow_id) VALUES ('t_datastoragequeue', 4) ;
INSERT INTO t_statetransitions_tables (table_name, flow_id) VALUES ('t_datastoragequeue', 3) ;







/* ------------------------------
	t_statetransitions_allowed
*/ ------------------------------

-- t_circulationsqueue
----------------------
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'STANDBY'),
	(SELECT id FROM t_stati WHERE status = 'MEDIA_WAIT')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'STANDBY'),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING')) ; 
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'STANDBY'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;  
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'STANDBY'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ; 
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'STANDBY'),
	(SELECT id FROM t_stati WHERE status = 'ABORTED')) ; 
	
	  
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'MEDIA_WAIT'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;
	

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'COMPLETED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'MEDIA_ACKNOWLEDGE')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'TRANSFERRING')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'RECOVERABLEERROR')) ;		
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;		
	

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'TRANSFERRING'),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'TRANSFERRING'),
	(SELECT id FROM t_stati WHERE status = 'MEDIA_ACKNOWLEDGE')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'TRANSFERRING'),
	(SELECT id FROM t_stati WHERE status = 'COMPLETED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'TRANSFERRING'),
	(SELECT id FROM t_stati WHERE status = 'RECOVERABLEERROR')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'TRANSFERRING'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;
		
	

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'MEDIA_ACKNOWLEDGE'),
	(SELECT id FROM t_stati WHERE status = 'COMPLETED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'MEDIA_ACKNOWLEDGE'),
	(SELECT id FROM t_stati WHERE status = 'RECOVERABLEERROR')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_dsqueue_x_cirqueue'),
	(SELECT id FROM t_stati WHERE status = 'MEDIA_ACKNOWLEDGE'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;
	

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'RECOVERABLEERROR'),
	(SELECT id FROM t_stati WHERE status = 'MEDIA_WAIT')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'RECOVERABLEERROR'),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'RECOVERABLEERROR'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'RECOVERABLEERROR'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;
		

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'ERROR'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;
	
	
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING'),
	(SELECT id FROM t_stati WHERE status = 'TRANSFERRING')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING'),
	(SELECT id FROM t_stati WHERE status = 'COMPLETED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING'),
	(SELECT id FROM t_stati WHERE status = 'RECOVERABLEERROR')) ;	
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;	



-- t_datastoragequeue IF TYPE IS RETIEVE
----------------------------------------
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 5),
	(SELECT id FROM t_stati WHERE status = 'ERROR'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;	
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 5),
	(SELECT id FROM t_stati WHERE status = 'NOTFOUNDERROR'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;
	
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 5),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'DOWNLOADING')) ;
	
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 5),
	(SELECT id FROM t_stati WHERE status = 'DOWNLOADING'),
	(SELECT id FROM t_stati WHERE status = 'DISTRIBUTION')) ;	
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 5),
	(SELECT id FROM t_stati WHERE status = 'DOWNLOADING'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;	
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 5),
	(SELECT id FROM t_stati WHERE status = 'DOWNLOADING'),
	(SELECT id FROM t_stati WHERE status = 'COMPLETED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 5),
	(SELECT id FROM t_stati WHERE status = 'DOWNLOADING'),
	(SELECT id FROM t_stati WHERE status = 'LTA_DOWNLOADING')) ;		
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 5),
	(SELECT id FROM t_stati WHERE status = 'DOWNLOADING'),
	(SELECT id FROM t_stati WHERE status = 'NOTFOUNDERROR')) ;
			
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 5),
	(SELECT id FROM t_stati WHERE status = 'DISTRIBUTION'),
	(SELECT id FROM t_stati WHERE status = 'COMPLETED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 5),
	(SELECT id FROM t_stati WHERE status = 'DISTRIBUTION'),
	(SELECT id FROM t_stati WHERE status = 'DOWNLOADING')) ;		
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 5),
	(SELECT id FROM t_stati WHERE status = 'DISTRIBUTION'),
	(SELECT id FROM t_stati WHERE status = 'NOTFOUNDERROR')) ;	
		
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 5),
	(SELECT id FROM t_stati WHERE status = 'LTA_DOWNLOADING'),
	(SELECT id FROM t_stati WHERE status = 'LTA_PROCESSING')) ;
	
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 5),
	(SELECT id FROM t_stati WHERE status = 'LTA_PROCESSING'),
	(SELECT id FROM t_stati WHERE status = 'LTA_DOWNLOADING')) ;				
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 5),
	(SELECT id FROM t_stati WHERE status = 'LTA_PROCESSING'),
	(SELECT id FROM t_stati WHERE status = 'DISTRIBUTION')) ;	
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 5),
	(SELECT id FROM t_stati WHERE status = 'LTA_PROCESSING'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;	
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 5),
	(SELECT id FROM t_stati WHERE status = 'LTA_PROCESSING'),
	(SELECT id FROM t_stati WHERE status = 'COMPLETED')) ;	
	


-- t_datastoragequeue IF TYPE IS ARCHIVE
----------------------------------------	
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 2),
	(SELECT id FROM t_stati WHERE status = 'ERROR'),
	(SELECT id FROM t_stati WHERE status = 'UPLOADING')) ;	
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 2),
	(SELECT id FROM t_stati WHERE status = 'UPLOADING'),
	(SELECT id FROM t_stati WHERE status = 'UPLOADED')) ;		
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 2),
	(SELECT id FROM t_stati WHERE status = 'UPLOADING'),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING')) ;		
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 2),
	(SELECT id FROM t_stati WHERE status = 'UPLOADED'),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING')) ;			
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 2),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING'),
	(SELECT id FROM t_stati WHERE status = 'COMPLETED')) ;			


-- t_datastoragequeue IF TYPE IS LTA_ARCHIVE
--------------------------------------------
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 4),
	(SELECT id FROM t_stati WHERE status = 'ERROR'),
	(SELECT id FROM t_stati WHERE status = 'UPLOADING')) ;	
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 4),
	(SELECT id FROM t_stati WHERE status = 'UPLOADING'),
	(SELECT id FROM t_stati WHERE status = 'UPLOADED')) ;		
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 4),
	(SELECT id FROM t_stati WHERE status = 'UPLOADING'),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING')) ;		
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 4),
	(SELECT id FROM t_stati WHERE status = 'UPLOADED'),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING')) ;			
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 4),
	(SELECT id FROM t_stati WHERE status = 'PROCESSING'),
	(SELECT id FROM t_stati WHERE status = 'COMPLETED')) ;	




-- t_datastoragequeue IF TYPE IS DELETE
---------------------------------------
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 3),
	(SELECT id FROM t_stati WHERE status = 'ERROR'),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE')) ;		
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 3),
	(SELECT id FROM t_stati WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_stati WHERE status = 'DELETING')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_datastoragequeue' AND flow_id = 3),
	(SELECT id FROM t_stati WHERE status = 'DELETING'),
	(SELECT id FROM t_stati WHERE status = 'COMPLETED')) ;	
					
	







---------------------------------------------------------------------------------
------------------------- TRIGGER FUNCTION MODIFICATION -------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	tf_circulationstatus
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_circulationstatus() RETURNS TRIGGER AS $$
BEGIN
  	PERFORM p_statetransitions_check('t_circulationsqueue', 1, OLD.globalstatus_id, NEW.globalstatus_id);
	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;


/* ------------------------------
	tf_datastoragequeuetrans
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_datastoragequeuetrans() RETURNS TRIGGER AS $$
BEGIN
	IF (SELECT datastoragetype FROM t_datastoragetypes WHERE id = NEW.flow_id) = 'RETRIEVE' THEN
  	PERFORM p_statetransitions_check('t_datastoragequeue', 5, OLD.globalstatus_id, NEW.globalstatus_id);
	END IF ;
	
	IF (SELECT datastoragetype FROM t_datastoragetypes WHERE id = NEW.flow_id) = 'ARCHIVE'THEN
  	PERFORM p_statetransitions_check('t_datastoragequeue', 2, OLD.globalstatus_id, NEW.globalstatus_id);
	END IF ;
	
	IF (SELECT datastoragetype FROM t_datastoragetypes WHERE id = NEW.flow_id) = 'LTA_ARCHIVE' THEN
  	PERFORM p_statetransitions_check('t_datastoragequeue', 4, OLD.globalstatus_id, NEW.globalstatus_id);
	END IF ;
	
	IF (SELECT datastoragetype FROM t_datastoragetypes WHERE id = NEW.flow_id) = 'DELETE' THEN
  	PERFORM p_statetransitions_check('t_datastoragequeue', 3, OLD.globalstatus_id, NEW.globalstatus_id);
	END IF ;
	
	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
------------------------------- DROP OLD OBJECTS --------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	p_dsqueuexcirqueue_statetrans
*/ ------------------------------
DROP FUNCTION IF EXISTS p_circulationstatetransition (BIGINT, SMALLINT, SMALLINT) ;


/* ------------------------------
	p_datastoragequeue_statetrans
*/ ------------------------------
DROP FUNCTION IF EXISTS p_datastoragequeue_statetrans (BIGINT, SMALLINT, SMALLINT, SMALLINT) ;








--------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ---------------------------------
--------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'circulation-1.18.0' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'circulation-1.18.0',
		'2017-02-24',
		6,
		20,
		'CIRCULATION_1-18-0',
		'Added new state transitions for t_circulationsqueue and t_datastoragequeue. Changed state transition check using p_statetransitions_check. Dropped p_circulationstatetransition and p_datastoragequeue_statetrans procedure.'
) ;


