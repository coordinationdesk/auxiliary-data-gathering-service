
-- Version 1.6.2
-- POSTGRES








---------------------------------------------------------------------------------
---------------------------- PROCEDURES MODIFICATION ----------------------------
---------------------------------------------------------------------------------

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

BEGIN
	cq_stat := (SELECT status FROM t_stati WHERE id = in_cq_stat_id) ;
	
	FOR row IN
		(SELECT datastoragequeue_id FROM t_dsqueue_x_cirqueue WHERE circulation_id = in_cq_id) LOOP
	
		ds_id := row.datastoragequeue_id ;
	
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
	END LOOP ;
	
	RETURN 0 ;
	
END ;
$$ LANGUAGE plpgsql ;







---------------------------------------------------------------------------------
----------------------------- TRIGGERS MODIFICATION -----------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	p_datastoragequeue_update
*/ ------------------------------
DROP TRIGGER tr_circulationsqueue_update3 ON t_circulationsqueue ;

CREATE TRIGGER tr_circulationsqueue_update3 AFTER UPDATE OF status_id 
	ON t_circulationsqueue 
		FOR EACH ROW EXECUTE PROCEDURE tf_datastoragequeueupdate();







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
		'circulation-1.6.2',
		'2012-11-12',
		6,
		20,
		'CIRCULATION_1-6-2',
		'Changed procedure p_datastoragequeue_update and trigger tr_circulationsqueue_update3.'
) ;
