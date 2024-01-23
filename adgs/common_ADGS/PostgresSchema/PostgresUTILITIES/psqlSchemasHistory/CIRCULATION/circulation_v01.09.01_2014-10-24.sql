
-- Version 1.9.1
-- POSTGRES








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
		ELSIF pre_stat = 'DISTRIBUTION' AND (post_stat = 'COMPLETED' OR post_stat = 'DOWNLOADING') THEN
			RETURN 0 ;
		ELSIF pre_stat = 'LTA_DOWNLOADING' AND post_stat = 'LTA_PROCESSING' THEN
			RETURN 0 ;
		ELSIF pre_stat = 'LTA_PROCESSING' AND (post_stat = 'LTA_DOWNLOADING' OR post_stat = 'DISTRIBUTION' OR post_stat = 'ERROR') THEN
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
		ELSIF pre_stat = 'UPLOADING' AND (post_stat = 'UPLOADED' OR post_stat = 'PROCESSING') THEN
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
		'circulation-1.9.1',
		'2014-10-24',
		6,
		20,
		'CIRCULATION_1-9-1',
		'Added new allowed transitions on p_datastoragequeue_statetrans.'
) ;


