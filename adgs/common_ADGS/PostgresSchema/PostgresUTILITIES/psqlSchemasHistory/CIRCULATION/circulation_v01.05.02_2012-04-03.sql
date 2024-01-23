
-- Version 1.5.2
-- POSTGRES








---------------------------------------------------------------------------------
----------------------------- PROCEDURE MODIFICATION ----------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	p_circulationStateTransition
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_circulationstatetransition(
	in_circ_id BIGINT,
	in_pre_stat_id INT2,
	in_post_stat_id INT2) RETURNS INTEGER AS $$
	
	
-- State Transition (from -> to | or):
------------------------------------------------
-- STANDBY -> MEDIA_WAIT | PROCESSING | ELIGIBLE / ERROR
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
	-- Set error message
	ex_msg := 'p_CirculationStateTransition: id ' || in_circ_id || ' - invalid state transition.';
	
	-- Set stati from id
	pre_stat := (SELECT status FROM t_stati WHERE id = in_pre_stat_id) ;
	post_stat := (SELECT status FROM t_stati WHERE id = in_post_stat_id) ;
	
	
	------------------------------
	--      Starting Chek       --
	------------------------------
	-- No status changes
	IF pre_stat = post_stat THEN
		RETURN 0 ;
	END IF ;
	
	-- Changing status
	IF pre_stat = 'STANDBY' THEN
		IF post_stat = 'MEDIA_WAIT' OR post_stat = 'PROCESSING' OR post_stat = 'ELIGIBLE' OR post_stat = 'ERROR' THEN
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








---------------------------------------------------------------------------------
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
		'circulation-1.5.2',
		'2012-04-03',
		6,
		20,
		'CIRCULATION_1-5-2',
		'Added new status trasitions on p_circulationstatetransition procedure.'
) ;


