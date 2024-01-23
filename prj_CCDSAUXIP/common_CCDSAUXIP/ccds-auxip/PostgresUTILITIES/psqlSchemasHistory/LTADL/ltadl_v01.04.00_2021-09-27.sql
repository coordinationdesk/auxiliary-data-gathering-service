
-- Version 01.04.00
-- POSTGRES








-------------------------------------------------------------------------------
-------------------------------- TABLES INSERT --------------------------------
-------------------------------------------------------------------------------

/* ------------------------------
	t_statetransitions_allowed
*/ ------------------------------

-- t_ltadl_batchqueue
----------------------
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ltadl_batchqueue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'SUBMITTED'),
	(SELECT id FROM t_srv_statuses WHERE status = 'SUBMITTED')) ;








-------------------------------------------------------------------------------
------------------------------ FUNCTIONS CHANGES ------------------------------
-------------------------------------------------------------------------------

/* ------------------------------
	f_calculate_batch_status
*/ ------------------------------
CREATE OR REPLACE FUNCTION f_calculate_batch_status(
	in_batch_id BIGINT) 
RETURNS 
	VARCHAR AS $$

BEGIN
	DROP TABLE IF EXISTS tb1 ;
	CREATE TEMP TABLE tb1 AS 
	SELECT 
		id,
  		CASE
      		WHEN t.status_id = (SELECT id FROM t_srv_statuses WHERE status = 'COLLAPSED') THEN
          		(SELECT tsub.status_id FROM t_ltadl_filequeue tsub where tsub.id = t.collapsed_to)
      		ELSE t.status_id 
  		END AS stat_id
	FROM 
		t_ltadl_filequeue t
	WHERE 
		t.ltadl_batch_id = in_batch_id ;


	IF (SELECT count(*) FROM tb1 WHERE stat_id = (SELECT id FROM t_srv_statuses WHERE status = 'ERROR')) > 0 THEN
		RETURN 'ERROR' ;
	
	ELSIF (SELECT count(*) FROM tb1 WHERE stat_id = (SELECT id FROM t_srv_statuses WHERE status = 'CANCELLED')) > 0 THEN
		RETURN 'CANCELLED' ;
	
	ELSIF (SELECT count(*) FROM tb1) = (SELECT count(*) FROM tb1 WHERE stat_id = (SELECT id FROM t_srv_statuses WHERE status = 'COMPLETED')) THEN
		RETURN 'COMPLETED' ;
	
	ELSE
		RETURN 'RUNNING' ;
		
	END IF ;
	
END ;
$$ LANGUAGE plpgsql ;








-------------------------------------------------------------------------------
-------------------------- TRIGGER FUNCTIONS CHANGES --------------------------
-------------------------------------------------------------------------------

/* --------------------------
	tf_ltadl_filequeue_update_batch_status
*/ --------------------------
CREATE OR REPLACE FUNCTION tf_ltadl_filequeue_update_batch_status() RETURNS TRIGGER AS $$

BEGIN
	UPDATE t_ltadl_batchqueue SET status_id = (
		SELECT id FROM t_srv_statuses WHERE status = (
			SELECT f_calculate_batch_status(NEW.ltadl_batch_id)))
	WHERE 
		id = NEW.ltadl_batch_id ;
	
	RETURN NEW ;
	
END ;
$$ LANGUAGE plpgsql ;








-------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS --------------------------------
-------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'ltadl-01.04.00' ;

INSERT INTO t_schema (
	version,
	modificationdate,
	modificationtype,
	schematype,
	cvstag,
	description )
VALUES  (
	'ltadl-01.04.00',
	'2021-09-27',
	6,
	37,
	'LTADL_1-4-0',
	'Fixed f_calculate_batch_status function and tf_ltadl_filequeue_update_batch_status trigger function (LTA-370)'
) ;


