
-- Version 11.12.01
-- POSTGRES








--------------------------------------------------------------------------------
-------------------------------- TABLES INSERT -------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_sensors
*/ ------------------------------
INSERT INTO t_sensors VALUES (71, 'SYNERGY', 'Synergy Products');
INSERT INTO t_sensors VALUES (72, 'GNSS', 'Global Navigation Satellite System');
INSERT INTO t_sensors VALUES (73, 'HKTM', 'House Keeping Telemetry');
INSERT INTO t_sensors VALUES (74, 'NAVATT', 'Navigation and Attitude data');

/* ------------------------------
	t_satrelations
*/ ------------------------------
INSERT INTO t_satrelations VALUES (40, 1, 71) ;
INSERT INTO t_satrelations VALUES (40, 1, 72) ;
INSERT INTO t_satrelations VALUES (40, 1, 73) ;
INSERT INTO t_satrelations VALUES (40, 1, 74) ;

INSERT INTO t_satrelations VALUES (40, 2, 71) ;
INSERT INTO t_satrelations VALUES (40, 2, 72) ;
INSERT INTO t_satrelations VALUES (40, 2, 73) ;
INSERT INTO t_satrelations VALUES (40, 2, 74) ;








--------------------------------------------------------------------------------
------------------------------ PROCEDURES CREATION -----------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	p_insert_test_ordersqueue
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_insert_test_ordersqueue(
	in_files_number INTEGER -- number of files you want to create (cannot be zero)
) RETURNS INTEGER AS $$

DECLARE 
	p_procname VARCHAR(32);
	p_procvers VARCHAR(8);

BEGIN
	--INPUT CHECK
	-- Check the files number value
	IF in_files_number <= 0 THEN
		RAISE EXCEPTION 'p_insert_test_ordersqueue: in_files_number input value must be greater than 0.' ;
		RETURN 1 ;
	END IF ;
	
	-- INSERT OPERATIONS
	FOR counter IN 1..in_files_number LOOP
		-- Set processor
		SELECT processorname, processorversion INTO p_procname, p_procvers FROM t_processors ORDER BY random() LIMIT 1 ;
		
		-- Insert into t_ordersqueue
		INSERT INTO t_ordersqueue(
			creationdate, 
			priority, 
			status,
			processorname,
			processorversion,
			troubleshooting,
			starttime,
			stoptime,
			processingtimewindow,
			ordertype)
		VALUES (
			now(),
			1,
			'ABORTED',
			p_procname,
			p_procvers,
			'FALSE',
			now(),
			now() + interval '1' day,
			1,
			(SELECT ordertype FROM t_ordertypes ORDER BY random() LIMIT 1) );
	END LOOP ;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'pds-11.12.01' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-11.12.01',
		'2015-12-11',
		3,
		2,
		'PDS_11-12-1',
		'Added new sensors and satrelations. Added test procedure p_insert_test_ordersqueue.'
) ;


