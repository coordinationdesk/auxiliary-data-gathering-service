
-- Version 8.3.1
-- POSTGRES








--------------------------------------------------------------------------------
---------------------------- PROCEDURES MODIFICATION ----------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	P_ORDERSTATETRANSITION
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_orderstatetransition(
	in_orderid INTEGER, 
	pre_status VARCHAR(32), 
	post_status VARCHAR(32)) RETURNS INTEGER AS $$
	
DECLARE
  	cnt INTEGER ;
  	rec RECORD ;
  	err INTEGER ;
	evn INTEGER ;
	p_extordid VARCHAR(32);
	p_subreq INT;
	p_cartid INT;
	
BEGIN

	IF post_status IS NULL THEN
		RETURN 0;
	END IF ;

	IF pre_status IS NULL THEN
		IF post_status = 'ELIGIBLE' THEN
			INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
			evn := p_inserteventtableitem('t_orderstatetransitions', 'NUL2ELI') ;
			RETURN evn ;
		ELSIF post_status = 'STANDBY' THEN
			INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
			evn := p_inserteventtableitem('t_orderstatetransitions', 'NUL2STA') ;
			RETURN evn ;
		ELSIF post_status = 'ABORTED' THEN
			INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
			evn := p_inserteventtableitem('t_orderstatetransitions', '2ABO') ;
			RETURN evn ;
		ELSE
			RAISE EXCEPTION 'Invalid Order State Transition' ;
		END IF ;
	END IF ;

-- state control
	IF post_status = 'ELIGIBLE' THEN
		IF pre_status = 'ERROR' THEN
			INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
			evn := p_inserteventtableitem('t_orderstatetransitions', 'ERR2ELI') ;	
		ELSIF pre_status = 'RECOVERABLEERROR' THEN
			INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
			evn := p_inserteventtableitem('t_orderstatetransitions', 'REC2ELI') ;
		ELSIF pre_status = 'STANDBY' THEN
			INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
			evn := p_inserteventtableitem('t_orderstatetransitions', 'STA2ELI') ;
		ELSIF pre_status = 'ELIGIBLE' THEN
		-- The following transition is logged only the first time to avoid too much negligible transitions
		-- ELIGIBLE->PRECONDCHEDCK->ELIGIBLE...
			SELECT INTO rec COUNT(*) FROM t_orderstatetransitions WHERE "orderid" = in_orderid ;
			cnt := rec.count;
			IF cnt = 0 THEN -- the first time
				-- insert item in state transition table
				INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
			END IF ;
			RETURN 0 ;	
		ELSIF pre_status = 'PRECONDITIONCHECK' THEN 
			RETURN 0 ;
		ELSE 
			RAISE EXCEPTION 'Invalid Order State Transition' ;
		END IF ;
	ELSIF post_status = 'PRECONDITIONCHECK' THEN
		IF pre_status = 'ELIGIBLE' THEN
			RETURN 0 ;
		-- The following transition is not logged to avoid too much negligible transitions ELIGIBLE->PRECONDCHEDCK->ELIGIBLE...
		-- INSERT INTO t_orderstatetransitions(orderid ,status) values ( in_orderid,post_status);
		ELSE 
			RAISE EXCEPTION 'Invalid Order State Transition' ;
		END IF ;

	ELSIF post_status = 'DOWNLOADING' THEN
		IF pre_status = 'PRECONDITIONCHECK' THEN
			INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
			evn := p_inserteventtableitem('t_orderstatetransitions', 'PRE2DOW') ;
		ELSE 
			RAISE EXCEPTION 'Invalid Order State Transition' ;
		END IF ;
	ELSIF post_status = 'PROCESSING' THEN
		IF pre_status = 'DOWNLOADING' THEN
			INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
			evn := p_inserteventtableitem('t_orderstatetransitions', 'DOW2PRO') ;
		ELSE 
			RAISE EXCEPTION 'Invalid Order State Transition' ;
		END IF ;
	ELSIF post_status = 'ERROR' THEN
		IF pre_status = 'DOWNLOADING' THEN
			INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
			evn := p_inserteventtableitem('t_orderstatetransitions', 'DOW2ERR') ;	
		ELSIF pre_status = 'STANDBY' THEN
			INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
			evn := p_inserteventtableitem('t_orderstatetransitions', 'STA2ERR') ;	
		ELSIF pre_status = 'PROCESSING' THEN
			INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
			evn := p_inserteventtableitem('t_orderstatetransitions', 'PRO2ERR') ;
		ELSE 
			RAISE EXCEPTION 'Invalid Order State Transition' ;
		END IF ;
	ELSIF post_status = 'RECOVERABLEERROR' THEN
		IF pre_status = 'PROCESSING' THEN
			INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
			evn := p_inserteventtableitem('t_orderstatetransitions', 'PRO2REC') ;
		ELSE 
			RAISE EXCEPTION 'Invalid Order State Transition' ;
		END IF ;
	ELSIF post_status = 'COMPLETED' THEN
		IF pre_status = 'PROCESSING'  THEN
			INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
			evn := p_inserteventtableitem('t_orderstatetransitions', 'PRO2COM') ;
			err := p_completed_order_distribution(in_orderid) ;
		ELSIF pre_status = 'ERROR' THEN
			INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
			evn := p_inserteventtableitem('t_orderstatetransitions', 'ERR2COM') ;
			err := p_completed_order_distribution(in_orderid) ;
		ELSE 
			RAISE EXCEPTION 'Invalid Order State Transition' ;
		END IF ;
	ELSIF post_status = 'ABORTED' THEN
		INSERT INTO t_orderstatetransitions ("orderid", "status") VALUES (in_orderid, post_status) ;
		evn := p_inserteventtableitem('t_orderstatetransitions', '2ABO') ;
	ELSE
		RAISE EXCEPTION 'Invalid Order State Transition' ;
	END IF ;
	
	p_extordid := (SELECT externalorderid FROM t_ordersqueue WHERE orderid = in_orderid);
	p_subreq := (SELECT subreq FROM t_ordersqueue WHERE orderid = in_orderid);
	
	IF p_extordid IS NOT NULL AND p_subreq IS NOT NULL THEN
		p_cartid := (SELECT id FROM t_cartsref AS cr
					INNER JOIN t_ordersqueue AS oq
					ON oq.externalorderid = cr.extref
					WHERE oq.orderid = in_orderid);
		
		UPDATE t_cartsinv SET status = post_status WHERE cart = p_cartid AND ordsubreq = p_subreq ;	
		err := p_carts_update_status(p_cartid);
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
		'8.3.1',
		'2009-09-16',
		4,
		1,
		'SchemaVersion_8-3-1',
		'Fixed inserteventtableitem call bug on p_orderstatetransition.'
) ;
