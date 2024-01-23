
-- Version 1.9.3
-- POSTGRES








---------------------------------------------------------------------------------
--------------------------------- TABLES INSERT ---------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_datastoragetypes
*/ ------------------------------
INSERT INTO t_datastoragetypes (id, datastoragetype) VALUES (4, 'LTA_ARCHIVE') ;








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
		ELSIF pre_stat = 'LTA_PROCESSING' AND (post_stat = 'LTA_DOWNLOADING' OR post_stat = 'DISTRIBUTION' OR post_stat = 'ERROR' OR post_stat = 'NOTFOUNDERROR') THEN
			RETURN 0 ;
		ELSE
			RAISE EXCEPTION '%', ex_msg ;
			RETURN 1 ;
		END IF;
	END IF;
	
	-- IF TYPE IS ARCHIVE OR LTA_ARCHIVE
	IF dsq_type IN ('ARCHIVE', 'LTA_ARCHIVE') THEN
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



/* ------------------------------
	p_datastoragequeue_update
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_datastoragequeue_update(
	in_cq_id BIGINT,
	in_cq_stat_id SMALLINT) RETURNS INTEGER AS $$

DECLARE
	ds_id BIGINT;
	cq_stat VARCHAR(32);
	dxc_count INT;
	cq_stat_count INT;
	row RECORD ;
	archive_type_id SMALLINT ;
	lta_archive_type_id SMALLINT ;
	
BEGIN
	cq_stat := (SELECT status FROM t_stati WHERE id = in_cq_stat_id) ;
	archive_type_id := (SELECT id FROM t_datastoragetypes WHERE datastoragetype = 'ARCHIVE') ;
	lta_archive_type_id := (SELECT id FROM t_datastoragetypes WHERE datastoragetype = 'LTA_ARCHIVE') ;
	
	FOR row IN
		(SELECT datastoragequeue_id FROM t_dsqueue_x_cirqueue WHERE circulation_id = in_cq_id) LOOP
	
		ds_id := row.datastoragequeue_id ;
	
		IF cq_stat = 'ERROR' THEN
			IF (SELECT type_id FROM t_datastoragequeue WHERE id = ds_id) IN (archive_type_id, lta_archive_type_id) THEN
				UPDATE t_dsqueue_x_cirqueue SET globalstatus_id = in_cq_stat_id WHERE datastoragequeue_id = ds_id AND circulation_id = in_cq_id ; 
			ELSE
				UPDATE t_datastoragequeue SET status_id = in_cq_stat_id WHERE id = ds_id ;
			END IF ;
		END IF;
	
		IF cq_stat = 'COMPLETED' THEN
			IF (SELECT type_id FROM t_datastoragequeue WHERE id = ds_id) IN (archive_type_id, lta_archive_type_id) THEN
				UPDATE t_dsqueue_x_cirqueue SET globalstatus_id = (
					SELECT id FROM t_stati WHERE status = 'UPLOADED')
						WHERE datastoragequeue_id = ds_id AND circulation_id = in_cq_id;
			ELSE
			
				dxc_count := (SELECT count(*) FROM t_dsqueue_x_cirqueue WHERE datastoragequeue_id = ds_id);	
				cq_stat_count := (SELECT count(*) FROM t_dsqueue_x_cirqueue dxc
									INNER JOIN t_circulationsqueue cq ON dxc.circulation_id = cq.id
										WHERE dxc.datastoragequeue_id = ds_id AND cq.status_id = in_cq_stat_id);
			
				IF dxc_count = cq_stat_count THEN
					UPDATE t_datastoragequeue SET status_id = in_cq_stat_id 
					WHERE id = ds_id AND type_id = (
						SELECT id FROM t_datastoragetypes WHERE datastoragetype = 'RETRIEVE');
				
					--UPDATE t_datastoragequeue SET status_id = (
					-- 	SELECT id FROM t_stati WHERE status = 'UPLOADED')
					--WHERE id = ds_id AND type_id = (
					--	SELECT id FROM t_datastoragetypes WHERE datastoragetype = 'ARCHIVE');
				END IF;
			END IF;
		END IF;
	END LOOP ;
	
	RETURN 0 ;
	
END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------
DELETE FROM t_schema WHERE version = 'circulation-1.9.3' ;
INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'circulation-1.9.3',
		'2014-12-15',
		6,
		20,
		'CIRCULATION_1-9-3',
		'Added new datastoragetype. Changed stored procedures for new datastoragetype.'
) ;


