
-- Version 1.1.0
-- POSTGRES





-- ########################################################################### --
-- ##################                                        ################# --
-- ##################                WARNING!!!              ################# --
-- ##################  This schema can be installed only on  ################# --
-- ##################      PDS 10.0.0 version or major       ################# --
-- ##################                                        ################# --
-- ########################################################################### --





---------------------------------------------------------------------------------
-------------------------------- TABLES CREATION --------------------------------
---------------------------------------------------------------------------------

------------------------------
-- t_dsqueue_x_cirqueue
------------------------------
CREATE TABLE t_dsqueue_x_cirqueue (
	datastoragequeue_id BIGINT NOT NULL,
	circulation_id BIGINT NOT NULL,
	inv_filename VARCHAR(128) NOT NULL
) ;
ALTER TABLE t_dsqueue_x_cirqueue 
	ADD CONSTRAINT pk_t_dsqueue_x_cirqueue
		PRIMARY KEY (datastoragequeue_id, circulation_id) ;

ALTER TABLE t_dsqueue_x_cirqueue 
	ADD CONSTRAINT fk_dsqueue_x_cirqueue01
		FOREIGN KEY (datastoragequeue_id) REFERENCES t_datastoragequeue (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_dsqueue_x_cirqueue 
	ADD CONSTRAINT fk_dsqueue_x_cirqueue02
		FOREIGN KEY (circulation_id) REFERENCES t_circulationsqueue (id) 
			ON DELETE CASCADE ;







---------------------------------------------------------------------------------
------------------------------ TABLES MODIFICATION ------------------------------
---------------------------------------------------------------------------------

------------------------------
-- t_datastoragequeue
------------------------------
ALTER TABLE t_datastoragequeue DROP CONSTRAINT fk_datastoragequeue01 ;
ALTER TABLE t_datastoragequeue DROP COLUMN circulation_id ;
ALTER TABLE t_datastoragequeue DROP COLUMN filename ;
ALTER TABLE t_datastoragequeue DROP COLUMN prodname ;
ALTER TABLE t_datastoragequeue DROP COLUMN url ;








---------------------------------------------------------------------------------
---------------------------- PROCEDURES MODIFICATION ----------------------------
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
-- STANDBY -> PROCESSING | ELIGIBLE / ERROR
-- ELIGIBLE -> PROCESSING | TRANSFERRING | RECOVERABLE ERROR | ERROR
-- TRANSFERRING -> PROCESSING | COMPLETED | RECOVERABLE ERROR | ERROR
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
		IF post_stat = 'PROCESSING' OR post_stat = 'ELIGIBLE' OR post_stat = 'ERROR' THEN
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
		IF post_stat = 'PROCESSING' OR post_stat = 'COMPLETED' OR post_stat = 'RECOVERABLEERROR'OR post_stat = 'ERROR'  THEN
			RETURN 0 ;
		ELSE 
			RAISE EXCEPTION '%', ex_msg ;
			RETURN 1 ;
		END IF ;
	
	ELSIF pre_stat = 'RECOVERABLEERROR' THEN
		IF post_stat = 'PROCESSING' OR post_stat = 'ELIGIBLE' OR post_stat = 'ERROR' THEN
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

BEGIN
	ds_id := (SELECT datastoragequeue_id FROM t_dsqueue_x_cirqueue WHERE circulation_id = in_cq_id);
	cq_stat := (SELECT status FROM t_stati WHERE id = in_cq_stat_id) ;
	
	IF cq_stat = 'ERROR' THEN
		UPDATE t_datastoragequeue SET status_id = in_cq_stat_id WHERE id = ds_id ;
	END IF;
	
	IF cq_stat = 'COMPLETED' THEN
		dxc_count := (SELECT count(*) FROM t_dsqueue_x_cirqueue WHERE datastoragequeue_id = ds_id);	
		cq_stat_count := (SELECT count(*) FROM t_dsqueue_x_cirqueue dxc
							INNER JOIN t_circulationsqueue cq ON dxc.circulation_id = cq.id
							WHERE dxc.datastoragequeue_id = ds_id AND cq.status_id = in_cq_stat_id);
		
		IF dxc_count = cq_stat_count THEN
			UPDATE t_datastoragequeue SET status_id = in_cq_stat_id 
			WHERE id = ds_id AND type_id = (
				SELECT id FROM t_datastoragetypes WHERE datastoragetype = 'RETRIEVE');

			UPDATE t_datastoragequeue SET status_id = (
				 SELECT id FROM t_stati WHERE status = 'UPLOADED')
			WHERE id = ds_id AND type_id = (
				SELECT id FROM t_datastoragetypes WHERE datastoragetype = 'ARCHIVE');
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
		'circulation-1.1.0',
		'2011-11-08',
		6,
		20,
		'CIRCULATION_1-1-0',
		'Created table t_dsqueue_x_cirqueue and changed procedures p_circulationstatetransition and p_datastoragequeue_update.'
) ;


