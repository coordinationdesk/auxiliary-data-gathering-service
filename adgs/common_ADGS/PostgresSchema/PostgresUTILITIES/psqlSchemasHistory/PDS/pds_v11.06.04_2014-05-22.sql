
-- Version 11.6.4
-- POSTGRES








---------------------------------------------------------------------------------
------------------------------ TABLES MODIFICATION ------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_distributionitems
*/ ------------------------------
CREATE INDEX ix_t_distributionitems09 ON t_distributionitems USING BTREE (distributeddate) ;
CREATE INDEX ix_t_distributionitems10 ON t_distributionitems USING BTREE (creationdate) ;


/* ------------------------------
	t_inventory
*/ ------------------------------
CREATE INDEX ix_t_inventory17 ON t_inventory USING BTREE (modificationdate) ;


/* --------------------------
	t_ordersqueue
*/ --------------------------
CREATE INDEX ix_t_ordersqueue06 ON t_ordersqueue USING BTREE (creationdate) ;
CREATE INDEX ix_t_ordersqueue07 ON t_ordersqueue USING BTREE (starttime) ;
CREATE INDEX ix_t_ordersqueue08 ON t_ordersqueue USING BTREE (stoptime) ;
CREATE INDEX ix_t_ordersqueue09 ON t_ordersqueue USING BTREE (jobresponsible) ;








---------------------------------------------------------------------------------
---------------------------- PROCEDURES MODIFICATION ----------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	p_orders_manager
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_orders_manager(
	in_orderid INTEGER, 
	pre_status VARCHAR(32), 
	post_status VARCHAR(32),
	event VARCHAR(8)) RETURNS INTEGER AS $$
	
DECLARE
  	cnt INTEGER ;
  	rec RECORD ;
  	err INTEGER ;
	evn INTEGER ;
	p_extordid VARCHAR(128);
	p_subreq INT;
	p_cartid INT;
	cartsinv_status VARCHAR(32);
	ex_msg VARCHAR(128);
	
BEGIN
	-- Set error message
	ex_msg := 'p_orders_manager: orderid ' || in_orderid || ' - ' || pre_status || ' to ' || post_status || ' invalid order state transition.';
	
	
	-- No status
	IF post_status IS NULL THEN
		RETURN 0;
	END IF ;
	
	
	-- First insert
	IF pre_status IS NULL THEN
		IF post_status = 'ELIGIBLE' THEN
			INSERT INTO t_orderstatetransitions (orderid, status) VALUES (in_orderid, post_status) ;
			evn := p_events('t_orderstatetransitions', 'NUL2ELI') ;
			RETURN evn ;
		ELSIF post_status = 'STANDBY' THEN
			INSERT INTO t_orderstatetransitions (orderid, status) VALUES (in_orderid, post_status) ;
			evn := p_events('t_orderstatetransitions', 'NUL2STA') ;
			RETURN evn ;
		ELSIF post_status = 'ABORTED' THEN
			INSERT INTO t_orderstatetransitions (orderid, status) VALUES (in_orderid, post_status) ;
			evn := p_events('t_orderstatetransitions', 'NUL2ABO') ;
			RETURN evn ;
		ELSE
			RAISE EXCEPTION '%', ex_msg ;
		END IF ;
	END IF ;
	
	
	-- Status changes
	IF post_status = 'ELIGIBLE' THEN
		IF pre_status = 'ERROR' THEN
			INSERT INTO t_orderstatetransitions (orderid, status) VALUES (in_orderid, post_status) ;
			evn := p_events('t_orderstatetransitions', 'ERR2ELI') ;	
		ELSIF pre_status = 'RECOVERABLEERROR' THEN
			INSERT INTO t_orderstatetransitions (orderid, status) VALUES (in_orderid, post_status) ;
			evn := p_events('t_orderstatetransitions', 'REC2ELI') ;
		ELSIF pre_status = 'STANDBY' THEN
			INSERT INTO t_orderstatetransitions (orderid, status) VALUES (in_orderid, post_status) ;
			evn := p_events('t_orderstatetransitions', 'STA2ELI') ;
		ELSIF pre_status = 'ELIGIBLE' THEN
		-- The following transition is logged only the first time to avoid too much negligible transitions
		-- ELIGIBLE->PRECONDCHEDCK->ELIGIBLE...
			SELECT INTO rec COUNT(*) FROM t_orderstatetransitions WHERE orderid = in_orderid ;
			cnt := rec.count;
			IF cnt = 0 THEN -- the first time
				-- insert item in state transition table
				INSERT INTO t_orderstatetransitions (orderid, status) VALUES (in_orderid, post_status) ;
			END IF ;
			RETURN 0 ;	
		ELSIF pre_status = 'PRECONDITIONCHECK' THEN 
			RETURN 0 ;
		ELSE 
			RAISE EXCEPTION '%', ex_msg ;
		END IF ;
	
	ELSIF post_status = 'PRECONDITIONCHECK' THEN
		IF pre_status = 'ELIGIBLE' THEN
			RETURN 0 ;
		-- The following transition is not logged to avoid too much negligible transitions ELIGIBLE->PRECONDCHEDCK->ELIGIBLE...
		-- INSERT INTO t_orderstatetransitions(orderid ,status) values ( in_orderid,post_status);
		ELSE 
			RAISE EXCEPTION '%', ex_msg ;
		END IF ;

	ELSIF post_status = 'DOWNLOADING' THEN
		IF pre_status = 'PRECONDITIONCHECK' THEN
			INSERT INTO t_orderstatetransitions (orderid, status) VALUES (in_orderid, post_status) ;
			evn := p_events('t_orderstatetransitions', 'PRE2DOW') ;
		ELSE 
			RAISE EXCEPTION '%', ex_msg ;
		END IF ;
	
	ELSIF post_status = 'PROCESSING' THEN
		IF pre_status = 'DOWNLOADING' THEN
			INSERT INTO t_orderstatetransitions (orderid, status) VALUES (in_orderid, post_status) ;
			evn := p_events('t_orderstatetransitions', 'DOW2PRO') ;
		ELSE 
			RAISE EXCEPTION '%', ex_msg ;
		END IF ;
	
	ELSIF post_status = 'ERROR' THEN
		IF pre_status = 'DOWNLOADING' THEN
			INSERT INTO t_orderstatetransitions (orderid, status) VALUES (in_orderid, post_status) ;
			evn := p_events('t_orderstatetransitions', 'DOW2ERR') ;	
		ELSIF pre_status = 'STANDBY' THEN
			INSERT INTO t_orderstatetransitions (orderid, status) VALUES (in_orderid, post_status) ;
			evn := p_events('t_orderstatetransitions', 'STA2ERR') ;	
		ELSIF pre_status = 'PROCESSING' THEN
			INSERT INTO t_orderstatetransitions (orderid, status) VALUES (in_orderid, post_status) ;
			evn := p_events('t_orderstatetransitions', 'PRO2ERR') ;
		ELSE 
			RAISE EXCEPTION '%', ex_msg ;
		END IF ;
	
	ELSIF post_status = 'RECOVERABLEERROR' THEN
		IF pre_status = 'PROCESSING' THEN
			INSERT INTO t_orderstatetransitions (orderid, status) VALUES (in_orderid, post_status) ;
			evn := p_events('t_orderstatetransitions', 'PRO2REC') ;
		ELSE 
			RAISE EXCEPTION '%', ex_msg ;
		END IF ;
	
	ELSIF post_status = 'COMPLETED' THEN
		IF pre_status = 'PROCESSING'  THEN
			INSERT INTO t_orderstatetransitions (orderid, status) VALUES (in_orderid, post_status) ;
			evn := p_events('t_orderstatetransitions', 'PRO2COM') ;
			err := p_completed_order(in_orderid) ;
		ELSIF pre_status = 'ERROR' THEN
			INSERT INTO t_orderstatetransitions (orderid, status) VALUES (in_orderid, post_status) ;
			evn := p_events('t_orderstatetransitions', 'ERR2COM') ;
			err := p_completed_order(in_orderid) ;
		ELSE 
			RAISE EXCEPTION '%', ex_msg ;
		END IF ;
	
	ELSIF post_status = 'ABORTED' THEN
		INSERT INTO t_orderstatetransitions (orderid, status) VALUES (in_orderid, post_status) ;
		evn := p_events('t_orderstatetransitions', '2ABO') ;
	
	ELSE
		RAISE EXCEPTION '%', ex_msg ;
	END IF ;
	
	
	p_extordid := (SELECT externalorderid FROM t_ordersqueue WHERE orderid = in_orderid);
	p_subreq := (SELECT subreq FROM t_ordersqueue WHERE orderid = in_orderid);
	
	IF p_extordid IS NOT NULL AND p_subreq IS NOT NULL THEN
		p_cartid := (SELECT id FROM t_cartsref AS cr
						INNER JOIN t_ordersqueue AS oq
							ON oq.externalorderid = cr.extref
						WHERE oq.orderid = in_orderid);
		
		IF post_status = 'COMPLETED' THEN
			cartsinv_status := 'PROCESSED';
		ELSE
			cartsinv_status := post_status ;
		END IF ;
		
		UPDATE t_cartsinv SET status = cartsinv_status WHERE cart = p_cartid AND ordsubreq = p_subreq ;	
		err := p_carts_update_status(p_cartid);
	END IF ;

	err := p_events('t_ordersqueue', event) ;
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
		'pds-11.6.4',
		'2014-05-22',
		3,
		2,
		'PDS_11-6-4',
		'Added ACQ-1099 indexes. Fixed bug on p_orders_manager (removed call to p_completed_order_distribution).'
) ;


