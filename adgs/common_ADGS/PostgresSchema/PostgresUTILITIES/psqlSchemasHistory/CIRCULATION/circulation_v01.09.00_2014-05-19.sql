
-- Version 1.9.0
-- POSTGRES








---------------------------------------------------------------------------------
------------------------------ TABLES MODIFICATION ------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_datastoragequeue
*/ ------------------------------
ALTER TABLE t_datastoragequeue ADD md5 CHAR(32) ;
ALTER TABLE t_datastoragequeue ADD enddate TIMESTAMP(6) ;
CREATE INDEX ix_t_datastoragequeue03 ON t_datastoragequeue USING BTREE (md5) ;


/* ------------------------------
	t_dsqueue_x_cirqueue
*/ ------------------------------
ALTER TABLE t_dsqueue_x_cirqueue ADD id BIGSERIAL NOT NULL ;
ALTER TABLE t_dsqueue_x_cirqueue ADD priority INTEGER DEFAULT 0 NOT NULL ;
ALTER TABLE t_dsqueue_x_cirqueue ADD tourl VARCHAR(512) ;

ALTER TABLE t_dsqueue_x_cirqueue DROP CONSTRAINT pk_t_dsqueue_x_cirqueue ;
ALTER TABLE t_dsqueue_x_cirqueue 
	ADD CONSTRAINT pk_t_dsqueue_x_cirqueue
		PRIMARY KEY (id) ;

ALTER TABLE t_dsqueue_x_cirqueue 
	ADD CONSTRAINT uq_dsqueuexcirqueue01
		UNIQUE (datastoragequeue_id, circulation_id) ;

ALTER TABLE t_dsqueue_x_cirqueue ALTER COLUMN circulation_id DROP NOT NULL ;
ALTER TABLE t_dsqueue_x_cirqueue DROP CONSTRAINT fk_dsqueue_x_cirqueue02 ;








---------------------------------------------------------------------------------
---------------------------- PROCEDURES MODIFICATION ----------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	p_dsqueuexcirqueue_statetrans
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_dsqueuexcirqueue_statetrans(
	in_pre_stat_id SMALLINT,
	in_post_stat_id SMALLINT) RETURNS INTEGER AS $$

DECLARE
	ex_msg VARCHAR(128);
	pre_stat VARCHAR(32);
	post_stat VARCHAR(32);
	
BEGIN
	-- Transitions allowed
	--------------------------------------------
	-- ABORTED -> ELIGIBLE
	-- COMPLETED -> ELIGIBLE
	-- DISTRIBUTION -> COMPLETED | ELIGIBLE | ERROR | RECOVERABLEERROR
	-- DOWNLOADING -> DISTRIBUTION | ERROR | RECOVERABLEERROR
	-- ELIGIBLE -> DOWNLOADING / UPLOADING | ABORTED
	-- ERROR -> ELIGIBLE | UPLOADED | UPLOADING
	-- PROCESSING -> COMPLETED | ERROR
	-- RECOVERABLEERROR -> ELIGIBLE
	-- UPLOADED -> PROCESSING | ERROR
	-- UPLOADING -> UPLOADED | ERROR
	--------------------------------------------
		
	-- Set stati from id
	pre_stat := (SELECT status FROM t_stati WHERE id = in_pre_stat_id) ;
	post_stat := (SELECT status FROM t_stati WHERE id = in_post_stat_id) ;
	
	-- Set error message
	ex_msg := 'p_DsqueueXCirqueue_StateTrans: globalstatus ' || pre_stat || ' to ' || post_stat || ' invalid transition.';
	
	------------------------------
	--      Starting Chek       --
	------------------------------
	IF pre_stat = 'ABORTED' AND post_stat = 'ELIGIBLE' THEN
		RETURN 0 ;
	ELSIF pre_stat = 'COMPLETED' AND post_stat = 'ELIGIBLE' THEN
		RETURN 0 ;
	ELSIF pre_stat = 'DISTRIBUTION' AND (post_stat = 'COMPLETED' OR post_stat = 'ELIGIBLE' OR post_stat = 'ERROR' OR post_stat = 'RECOVERABLEERROR') THEN
		RETURN 0 ;
	ELSIF pre_stat = 'DOWNLOADING' AND (post_stat = 'DISTRIBUTION' OR post_stat = 'ERROR' OR post_stat = 'RECOVERABLEERROR') THEN
		RETURN 0 ;
	ELSIF pre_stat = 'ELIGIBLE' AND (post_stat = 'DOWNLOADING' OR post_stat = 'UPLOADING' OR post_stat = 'ABORTED') THEN
		RETURN 0 ;
	ELSIF pre_stat = 'ERROR' AND (post_stat = 'ELIGIBLE' OR post_stat = 'UPLOADED' OR post_stat = 'UPLOADING') THEN
		RETURN 0 ;
	ELSIF pre_stat = 'PROCESSING' AND (post_stat = 'COMPLETED' OR post_stat = 'ERROR') THEN
		RETURN 0 ;
	ELSIF pre_stat = 'RECOVERABLEERROR' AND post_stat = 'ELIGIBLE' THEN
		RETURN 0 ;
	ELSIF pre_stat = 'UPLOADED' AND (post_stat = 'PROCESSING' OR post_stat = 'ERROR') THEN
		RETURN 0 ;
	ELSIF pre_stat = 'UPLOADING' AND (post_stat = 'UPLOADED' OR post_stat = 'ERROR') THEN
		RETURN 0 ;
	ELSE
		RAISE EXCEPTION '%', ex_msg ;
		RETURN 1 ;
	END IF;
	
	RETURN 0 ;
	
END ;
$$ LANGUAGE plpgsql ;



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
		'circulation-1.9.0',
		'2014-05-19',
		6,
		20,
		'CIRCULATION_1-9-0',
		'Added new fields on t_dsqueue_x_cirqueue. Added new allowed transitions on p_dsqueuexcirqueue_statetrans.'
) ;


