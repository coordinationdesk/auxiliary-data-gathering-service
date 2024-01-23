
-- Version 12.01.00
-- POSTGRES








--------------------------------------------------------------------------------
-------------------------------- TABLES CHANGES --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_smactions
*/ ------------------------------
CREATE INDEX ix_t_smactions06 ON t_smactions USING BTREE (operation) ;
CREATE INDEX ix_t_smactions07 ON t_smactions USING BTREE (enddate) ;








--------------------------------------------------------------------------------
--------------------------------- TABLES INSERT --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_statetransitions_allowed
*/ ------------------------------
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_ordersqueue'),
	(SELECT id FROM t_stati WHERE status = 'PRECONDITIONCHECK'),
	(SELECT id FROM t_stati WHERE status = 'ERROR')) ;


/* ------------------------------
	t_dbparameters
*/ ------------------------------
INSERT INTO t_dbparameters 
	(parameter, parametertype_id, value_bool, param_description) 
VALUES 
	('order_transition_table_enabled', 4, 't', 'TRUE if the new system to chek the status transition is installed') ;


/* ------------------------------
	t_stati
*/ ------------------------------
INSERT INTO t_stati (id, status) VALUES (40, 'TO_DELETE') ;








--------------------------------------------------------------------------------
---------------------------- PROCEDURES MODIFICATION ---------------------------
--------------------------------------------------------------------------------

/* -------------------------------
	p_orderstatetransitions_mgr
*/ -------------------------------
CREATE OR REPLACE FUNCTION p_orderstatetransitions_mgr(
	in_ordid INTEGER, 
	in_pre_stat VARCHAR(32), 
	in_post_stat VARCHAR(32)) RETURNS INTEGER AS $$
	
DECLARE
  	prefix VARCHAR(4);
	comp_stat VARCHAR(8);
	
BEGIN
	IF in_pre_stat IS NULL THEN
		prefix = 'NUL' ;
	ELSIF in_pre_stat IS NOT NULL AND in_post_stat = 'ABORTED'  THEN
		prefix = '' ;
	ELSE
		prefix = SUBSTRING(in_pre_stat FROM 0 FOR 4) ;
	END IF ;
	
	comp_stat := prefix || '2' || SUBSTRING(in_post_stat FROM 0 FOR 4) ;
		
	IF comp_stat = 'ERR2COM' OR comp_stat = 'PRO2COM' THEN
		PERFORM p_completed_order(in_ordid) ;
	END IF ;
	
	-- The following transition is not logged to avoid too much negligible transitions ELIGIBLE->PRECONDCHEDCK->ELIGIBLE
	IF comp_stat = 'PRE2ELI' OR comp_stat = 'ELI2PRE' THEN
		RETURN 0 ;
	END IF ;
	
	-- The following transition is logged only the first time to avoid too much negligible transitions ELIGIBLE->PRECONDCHEDCK->ELIGIBLE
	IF (comp_stat = 'ELI2ELI') THEN 
		IF ((SELECT COUNT(*) FROM t_orderstatetransitions WHERE orderid = in_ordid) = 0) THEN
			INSERT INTO t_orderstatetransitions (orderid, status) VALUES (in_ordid, in_post_stat) ;
		END IF ;
		RETURN 0 ;
	END IF ;
	
	-- Otherwise
	INSERT INTO t_orderstatetransitions (orderid, status) VALUES (in_ordid, in_post_stat) ;
	PERFORM p_events('t_orderstatetransitions', comp_stat) ;
	
	RETURN 0;
	
END ;
$$ LANGUAGE plpgsql ;








--------------------------------------------------------------------------------
------------------------------- TRIGGERS CHANGES -------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_ordersqueue
*/ ------------------------------
DROP TRIGGER IF EXISTS tr_ordersqueue_upd_bfr ON t_ordersqueue ;
CREATE TRIGGER tr_ordersqueue_upd_bfr BEFORE UPDATE OF status
	ON t_ordersqueue
		FOR EACH ROW EXECUTE PROCEDURE tf_ordersqueue_statcheck('UPD') ;


/* ------------------------------
	t_smqueue
*/ ------------------------------
DROP TRIGGER IF EXISTS tr_smqueue_upd_bfr ON t_smqueue ;
CREATE TRIGGER tr_smqueue_upd_bfr BEFORE UPDATE OF status
	ON t_smqueue
		FOR EACH ROW EXECUTE PROCEDURE tf_smqueue_checkchanges('UPD') ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'pds-12.01.00' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-12.01.00',
		'2017-01-12',
		3,
		2,
		'PDS_12-1-0',
		'Added indexes on t_smactions table. Added new allowed status transition on t_ordersqueue. Changed the method to create the "status change string" on p_orderstatetransitions_mgr'
) ;


