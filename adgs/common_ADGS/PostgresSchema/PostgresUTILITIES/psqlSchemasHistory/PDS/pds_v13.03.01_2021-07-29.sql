
-- Version 13.03.01
-- POSTGRES








--------------------------------------------------------------------------------
-------------------------- TRIGGER FUNCTIONS CREATION --------------------------
--------------------------------------------------------------------------------

/* -------------------------------
	tf_status_transiction_check
*/ -------------------------------
CREATE OR REPLACE FUNCTION tf_status_transiction_check() RETURNS TRIGGER AS $$

BEGIN
	PERFORM p_statetransitions_check(TG_TABLE_NAME::VARCHAR, 1, OLD.status_id, NEW.status_id) ;
	
	RETURN NEW ;
	
END ;
$$ LANGUAGE plpgsql ;








--------------------------------------------------------------------------------
------------------------------- TRIGGER CREATION -------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_cdp_feederqueue
*/ ------------------------------
CREATE TRIGGER tr_cdp_feederqueue_upd_bfr BEFORE UPDATE OF status_id
	ON t_cdp_feederqueue
		FOR EACH ROW EXECUTE PROCEDURE tf_status_transiction_check() ;


/* ------------------------------
	t_cdp_notification_queue
*/ ------------------------------
CREATE TRIGGER tr_cdp_notification_queue_upd_bfr BEFORE UPDATE OF status_id
	ON t_cdp_notification_queue
		FOR EACH ROW EXECUTE PROCEDURE tf_status_transiction_check() ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'pds-13.03.01' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-13.03.01',
		'2021-07-29',
		3,
		2,
		'PDS_13-3-1',
		'Added trigger for status transition check on t_cdp_feederqueue and t_cdp_notification_queue tables'
) ;


