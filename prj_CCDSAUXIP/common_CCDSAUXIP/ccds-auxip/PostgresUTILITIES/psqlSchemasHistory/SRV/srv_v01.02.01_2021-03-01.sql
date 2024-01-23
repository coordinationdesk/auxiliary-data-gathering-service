
-- SRV Version 1.2.1
-- POSTGRES





BEGIN ;



--------------------------------------------------------------------------------
-------------------------------- TABLES CHANGES --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_srv_bulkbatch_x_ordersqueue
*/ ------------------------------
ALTER TABLE t_srv_bulkbatch_x_ordersqueue 
	ADD CONSTRAINT uq_srv_bulkbatch_x_ordersqueue01
		UNIQUE (bulkbatch_id, order_id) ;


/* ------------------------------
	t_srv_request_x_user
*/ ------------------------------
ALTER TABLE t_srv_request_x_user 
	ADD CONSTRAINT uq_srv_request_x_user01
		UNIQUE (user_id, order_id) ;








--------------------------------------------------------------------------------
------------------------------ FUNCTIONS CHANGES -------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	f_srv_create_order
*/ ------------------------------
CREATE OR REPLACE FUNCTION f_srv_create_order(
	inProductId BIGINT,
	inAipId INTEGER,
	inStatusId BIGINT,
	inPriority INTEGER,
	inEstimatedDate TIMESTAMP(3),
	inRetentionTime INTEGER,
	inUserId INTEGER,
	inBulkbatchId INTEGER
) RETURNS BIGINT AS $$

DECLARE
	foundOrderId BIGINT ;
	returnedOrderId BIGINT ;
	sqlCmd VARCHAR ;
	
BEGIN
	IF (inUserId IS NULL AND inBulkbatchId IS NULL) OR (inUserId IS NOT NULL AND inBulkbatchId IS NOT NULL) THEN
		RAISE EXCEPTION 'f_srv_create_order: UserId and BulkbatchId must be valorized, and only one per time' ;
	END IF ;
	
	SELECT id INTO foundOrderId FROM t_srv_ordersqueue WHERE product_id = inProductId AND target_aip_id = inAipId AND status_id != f_srv_get_status_id('ERROR');
	
	IF foundOrderId IS NOT NULL THEN
		BEGIN
			IF inUserId IS NOT NULL THEN
				INSERT INTO t_srv_request_x_user (user_id, order_id) VALUES (inUserId, foundOrderId) ON CONFLICT DO NOTHING ; 
			ELSE
				INSERT INTO t_srv_bulkbatch_x_ordersqueue (bulkbatch_id, order_id) VALUES (inBulkbatchId, foundOrderId) ON CONFLICT DO NOTHING ; 
			END IF ;
			
			UPDATE t_srv_ordersqueue SET estimated_date = inEstimatedDate, priority = CASE WHEN priority < inPriority THEN inPriority ELSE priority END WHERE id = foundOrderId ;
			
			IF (SELECT s.status FROM t_srv_statuses s INNER JOIN t_srv_ordersqueue q ON q.status_id = s.id WHERE q.id = foundOrderId) = 'COMPLETED' THEN
				sqlCmd := 'UPDATE t_srv_aip_content SET eviction_time = NOW() + INTERVAL ''' || inRetentionTime || ' SECOND'' WHERE order_id = ' || foundOrderId ;
				EXECUTE sqlCmd ;
			END IF ;
		
			RETURN foundOrderId ;
			
		EXCEPTION WHEN others THEN
			INSERT INTO t_srv_ordersqueue (product_id, status_id, target_aip_id, estimated_date, priority)
				VALUES (inProductId, inStatusId, inAipId, inEstimatedDate, inPriority)
				RETURNING id INTO returnedOrderId ;
				
			IF inUserId IS NOT NULL THEN
				INSERT INTO t_srv_request_x_user (user_id, order_id) VALUES (inUserId, returnedOrderId) ON CONFLICT DO NOTHING ; 
			ELSE
				INSERT INTO t_srv_bulkbatch_x_ordersqueue (bulkbatch_id, order_id) VALUES (inBulkbatchId, returnedOrderId) ON CONFLICT DO NOTHING ; 
			END IF ;
		
			RETURN returnedOrderId ;
		END ;
		
	ELSE
		INSERT INTO t_srv_ordersqueue (product_id, status_id, target_aip_id, estimated_date, priority)
			VALUES (inProductId, inStatusId, inAipId, inEstimatedDate, inPriority)
			RETURNING id INTO returnedOrderId ;
		
		IF inUserId IS NOT NULL THEN
			INSERT INTO t_srv_request_x_user (user_id, order_id) VALUES (inUserId, returnedOrderId) ON CONFLICT DO NOTHING ; 
		ELSE
			INSERT INTO t_srv_bulkbatch_x_ordersqueue (bulkbatch_id, order_id) VALUES (inBulkbatchId, returnedOrderId) ON CONFLICT DO NOTHING ; 
		END IF ;
		
		RETURN returnedOrderId ;
		
	END IF ;
		
END ;
$$ LANGUAGE plpgsql ;








--------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ---------------------------------
--------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'srv-01.02.01' ;

INSERT INTO t_schema (
	version,
	modificationdate,
	modificationtype,
	schematype,
	cvstag,
	description )
VALUES  (
	'srv-01.02.01',
	'2021-03-01',
	6,
	35,
	'SRV_1-2-1',
	'Added uq contraint on t_srv_bulkbatch_x_ordersqueue and t_srv_request_x_user'
) ;


COMMIT ;


