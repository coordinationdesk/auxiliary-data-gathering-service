
-- Version 1.6.3
-- POSTGRES








---------------------------------------------------------------------------------
------------------------------ TABLES MODIFICATION ------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_distributionqueue
*/ ------------------------------
ALTER TABLE t_distributionqueue ADD circulationqueue_id BIGINT ;

ALTER TABLE t_distributionqueue
	ADD CONSTRAINT fk_distribution_to_circulation
		FOREIGN KEY (circulationqueue_id)
			REFERENCES t_circulationsqueue (id)
				ON DELETE CASCADE ;

CREATE INDEX ix_distribution_to_circulation ON t_distributionqueue USING BTREE (circulationqueue_id) ;







---------------------------------------------------------------------------------
---------------------------- PROCEDURES MODIFICATION ----------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	p_datastoragequeue_statetrans
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_datastoragequeue_statetrans(
	in_dsq_id BIGINT,
	in_dsq_type SMALLINT,
	in_pre_stat_id SMALLINT,
	in_post_stat_id SMALLINT) RETURNS INTEGER AS $$

DECLARE
	ex_msg VARCHAR(128);
	dsq_type VARCHAR(16);
	pre_stat VARCHAR(32);
	post_stat VARCHAR(32);
	
BEGIN
	-- Set stati and type from id
	dsq_type := (SELECT datastoragetype FROM t_datastoragetypes WHERE id = in_dsq_type) ;
	pre_stat := (SELECT status FROM t_stati WHERE id = in_pre_stat_id) ;
	post_stat := (SELECT status FROM t_stati WHERE id = in_post_stat_id) ;
	
	-- Set error message
	ex_msg := 'p_DatastorageQueue_StateTrans: id ' || in_dsq_id || ' - ' || pre_stat || ' to ' || post_stat || ' invalid state transition.';
	
	------------------------------
	--      Starting Chek       --
	------------------------------
	-- No status changes
	IF pre_stat = post_stat THEN
		RETURN 0;
	END IF ;
	
	-- status changes to ERROR
	-- (all stati can changes to error)
	IF post_stat = 'ERROR' THEN
		RETURN 0;
	END IF ;
	
	-- IF TYPE IS RETIEVE
	IF dsq_type = 'RETRIEVE' THEN
		IF (pre_stat = 'ERROR' OR pre_stat = 'NOTFOUNDERROR') AND post_stat = 'ELIGIBLE' THEN
			RETURN 0 ;
		ELSIF pre_stat = 'ELIGIBLE' AND post_stat = 'DOWNLOADING' THEN
			RETURN 0 ;
		ELSIF pre_stat = 'DOWNLOADING' AND (post_stat = 'DISTRIBUTION' OR post_stat = 'ELIGIBLE' OR post_stat = 'COMPLETED' OR post_stat = 'NOTFOUNDERROR' OR post_stat = 'LTA_DOWNLOADING') THEN
			RETURN 0 ;
		ELSIF pre_stat = 'DISTRIBUTION' AND post_stat = 'COMPLETED' THEN
			RETURN 0 ;
		ELSIF pre_stat = 'LTA_DOWNLOADING' AND post_stat = 'DOWNLOADING' THEN
			RETURN 0 ;
		ELSE
			RAISE EXCEPTION '%', ex_msg ;
			RETURN 1 ;
		END IF;
	END IF;
	
	-- IF TYPE IS ARCHIVE
	IF dsq_type = 'ARCHIVE' THEN
		IF pre_stat = 'ERROR' AND post_stat = 'UPLOADING' THEN
			RETURN 0 ;
		ELSIF pre_stat = 'UPLOADING' AND post_stat = 'UPLOADED' THEN
			RETURN 0 ;
		ELSIF pre_stat = 'UPLOADED' AND post_stat = 'PROCESSING' THEN
			RETURN 0 ;
		ELSIF pre_stat = 'PROCESSING' AND post_stat = 'COMPLETED' THEN
			RETURN 0 ;
		ELSE
			RAISE EXCEPTION '%', ex_msg ;
			RETURN 1 ;
		END IF;
	END IF;
	
	-- IF TYPE IS DELETE
	IF dsq_type = 'DELETE' THEN
		IF pre_stat = 'ERROR' AND post_stat = 'ELIGIBLE' THEN
			RETURN 0 ;
		ELSIF pre_stat = 'ELIGIBLE' AND post_stat = 'DELETING' THEN
			RETURN 0 ;
		ELSIF pre_stat = 'DELETING' AND post_stat = 'COMPLETED' THEN
			RETURN 0 ;
		ELSE
			RAISE EXCEPTION '%', ex_msg ;
			RETURN 1 ;
		END IF;
	END IF;
	
	RETURN 0 ;
	
END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	p_circulationStateTransition
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_circulationstatetransition(
	in_circ_id BIGINT,
	in_pre_stat_id INT2,
	in_post_stat_id INT2) RETURNS INTEGER AS $$
	
	
-- State Transition (from -> to | or):
------------------------------------------------
-- STANDBY -> MEDIA_WAIT | PROCESSING | ELIGIBLE / ERROR | ABORTED
-- MEDIA_WAIT -> ELIGIBLE
-- ELIGIBLE -> PROCESSING | TRANSFERRING | RECOVERABLE ERROR | ERROR
-- TRANSFERRING -> MEDIA_ACKNOLEDGE | PROCESSING | COMPLETED | RECOVERABLE ERROR | ERROR
-- MEDIA_ACKNOLEDGE -> COMPLETED | RECOVERABLE ERROR | ERROR
-- RECOVERABLE ERROR -> PROCESSING | ELIGIBLE | ERROR
-- ERROR -> ELIGIBLE
-- PROCESSING -> ELIGIBLE | TRANSFERRING | COMPLETED | RECOVERABLE ERROR | ERROR
------------------------------------------------

DECLARE
	ex_msg VARCHAR(128);
	pre_stat VARCHAR(32);
	post_stat VARCHAR(32);
	
BEGIN
	-- Set stati from id
	pre_stat := (SELECT status FROM t_stati WHERE id = in_pre_stat_id) ;
	post_stat := (SELECT status FROM t_stati WHERE id = in_post_stat_id) ;
	
	-- Set error message
	ex_msg := 'p_CirculationStateTransition: id ' || in_circ_id || ' - ' || pre_stat || ' to ' || post_stat || ' invalid state transition.';
	
	------------------------------
	--      Starting Chek       --
	------------------------------
	-- No status changes
	IF pre_stat = post_stat THEN
		RETURN 0 ;
	END IF ;
	
	-- Changing status
	IF pre_stat = 'STANDBY' THEN
		IF post_stat = 'MEDIA_WAIT' OR post_stat = 'PROCESSING' OR post_stat = 'ELIGIBLE' OR post_stat = 'ERROR' OR post_stat = 'ABORTED' THEN
			RETURN 0 ;
		ELSE 
			RAISE EXCEPTION '%', ex_msg ;
			RETURN 1 ;
		END IF ;

	ELSIF pre_stat = 'MEDIA_WAIT' THEN
		IF post_stat = 'ELIGIBLE' THEN
			RETURN 0 ;
		ELSE 
			RAISE EXCEPTION '%', ex_msg ;
			RETURN 1 ;
		END IF ;
	
	ELSIF pre_stat = 'ELIGIBLE' THEN
		IF post_stat = 'PROCESSING' OR post_stat = 'TRANSFERRING' OR post_stat = 'RECOVERABLEERROR' OR post_stat = 'ERROR'  THEN
			RETURN 0 ;
		ELSE 
			RAISE EXCEPTION '%', ex_msg ;
			RETURN 1 ;
		END IF ;
	
	ELSIF pre_stat = 'TRANSFERRING' THEN
		IF post_stat = 'PROCESSING' OR post_stat = 'MEDIA_ACKNOLEDGE' OR post_stat = 'COMPLETED' OR post_stat = 'RECOVERABLEERROR'OR post_stat = 'ERROR'  THEN
			RETURN 0 ;
		ELSE 
			RAISE EXCEPTION '%', ex_msg ;
			RETURN 1 ;
		END IF ;
	
	ELSIF pre_stat = 'MEDIA_ACKNOLEDGE' THEN
		IF post_stat = 'COMPLETED' OR post_stat = 'RECOVERABLEERROR' OR post_stat = 'ERROR' THEN
			RETURN 0 ;
		ELSE 
			RAISE EXCEPTION '%', ex_msg ;
			RETURN 1 ;
		END IF ;
	
	ELSIF pre_stat = 'RECOVERABLEERROR' THEN
		IF post_stat = 'MEDIA_WAIT' OR post_stat = 'PROCESSING' OR post_stat = 'ELIGIBLE' OR post_stat = 'ERROR' THEN
			RETURN 0 ;
		ELSE 
			RAISE EXCEPTION '%', ex_msg ;
			RETURN 1 ;
		END IF ;
	
	ELSIF pre_stat = 'ERROR' THEN
		IF post_stat = 'ELIGIBLE' THEN
			RETURN 0 ;
		ELSE 
			RAISE EXCEPTION '%', ex_msg ;
			RETURN 1 ;
		END IF ;
	
	ELSIF pre_stat = 'PROCESSING' THEN
		IF post_stat = 'ELIGIBLE' OR post_stat = 'TRANSFERRING' OR post_stat = 'COMPLETED' OR post_stat = 'RECOVERABLEERROR' OR post_stat = 'ERROR' THEN
			RETURN 0 ;
		ELSE 
			RAISE EXCEPTION '%', ex_msg ;
			RETURN 1 ;
		END IF ;
	
	ELSE
		RAISE EXCEPTION '%', ex_msg ;
		RETURN 1 ;
		
	END IF ;
	
	RETURN 0 ;

END ;
$$ LANGUAGE plpgsql ;




--------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------
INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'circulation-1.6.3',
		'2012-12-07',
		6,
		20,
		'CIRCULATION_1-6-3',
		'Added circulationqueue_id field on t_distributionqueue table. Fixed log messages on state transaction control procedures.'
) ;
