
-- Version 14.00.00
-- POSTGRES








-------------------------------------------------------------------------------
----------------------------- PROCEDURES CREATION -----------------------------
-------------------------------------------------------------------------------

-- *********************************************** --
--    WARNING !!!! Do NOT execute this command     --
-- if BROADCAST schema plugin is already installed --
-- *********************************************** --
/* ------------------------------
	p_events
*/ ------------------------------
CREATE OR REPLACE PROCEDURE p_events(
	payload TEXT) AS $$
	
BEGIN
	--  NOT IMPLEMENTED IN THIS SCHEMA
	--  To implement this procedure you have to install the BROADCAST schema v.1.0.0 or major
	
END ;
$$ LANGUAGE plpgsql ;
-- END OF WARNING --








-------------------------------------------------------------------------------
-------------------------------- TABLES CHANGES -------------------------------
-------------------------------------------------------------------------------

/* ------------------------------
	t_algorithm
*/ ------------------------------
ALTER TABLE t_algorithm ADD id SERIAL NOT NULL ;

ALTER TABLE t_ordersgeneration DROP CONSTRAINT fk_t_ordersgeneration4 ;
ALTER TABLE t_algorithm DROP CONSTRAINT pk_t_algorithm ;

ALTER TABLE t_algorithm
	ADD CONSTRAINT pk_t_algorithm
		PRIMARY KEY (id) ;
ALTER TABLE t_algorithm 
	ADD CONSTRAINT uq_algorithm
		UNIQUE (algorithm) ;

ALTER TABLE t_ordersgeneration
	ADD CONSTRAINT fk_t_ordersgeneration4
		FOREIGN KEY (algorithm)
			REFERENCES t_algorithm (algorithm)
				ON DELETE CASCADE ;


/* ------------------------------
	t_applicationstatus
*/ ------------------------------
ALTER TABLE t_applicationstatus ADD id SERIAL NOT NULL ;
ALTER TABLE t_applicationstatus	DROP CONSTRAINT pk_t_applicationstatus ;
ALTER TABLE t_applicationstatus
	ADD CONSTRAINT pk_applicationstatus
		PRIMARY KEY (id) ;
ALTER TABLE t_applicationstatus 
	ADD CONSTRAINT uq_applicationstatus
		UNIQUE (application, instance) ;


/* ------------------------------
	t_configurationschemas
*/ ------------------------------
ALTER TABLE t_configurationschemas ADD id SERIAL NOT NULL ;
ALTER TABLE t_configurationschemas DROP CONSTRAINT pk_t_configurationschemas ;
ALTER TABLE t_configurationschemas
	ADD CONSTRAINT pk_t_configurationschemas
		PRIMARY KEY (id) ;
ALTER TABLE t_configurationschemas 
	ADD CONSTRAINT uq_configurationschemas
		UNIQUE (namespace, version) ;


/* ------------------------------
	t_configurationspaces
*/ ------------------------------
ALTER TABLE t_configurationspaces ADD id SERIAL NOT NULL ;
ALTER TABLE t_configurationspaces DROP CONSTRAINT pk_t_configurationspaces ;
ALTER TABLE t_configurationspaces
	ADD CONSTRAINT pk_t_configurationspaces
		PRIMARY KEY (id) ;
ALTER TABLE t_configurationspaces 
	ADD CONSTRAINT uq_configurationspaces
		UNIQUE (namespace, version) ;


/* ------------------------------
	t_distributionformat
*/ ------------------------------
ALTER TABLE t_distributionformat ADD id SERIAL NOT NULL ;
ALTER TABLE t_distributionformat DROP CONSTRAINT pk_t_distributionformat ;
ALTER TABLE t_distributionformat
  ADD CONSTRAINT pk_t_distributionformat
  	PRIMARY KEY (id) ;
ALTER TABLE t_distributionformat 
	ADD CONSTRAINT uq_distributionformat
		UNIQUE (format) ;








-------------------------------------------------------------------------------
----------------------------- PROCEDURES CHANGES ------------------------------
-------------------------------------------------------------------------------

/* ------------------------------
	p_events
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_events(
	in_table VARCHAR(64), 
	in_action VARCHAR(16)) RETURNS INTEGER AS $$
	
DECLARE
	event_file VARCHAR(64) ;
	ret INTEGER;
	
BEGIN

	--  NOT IMPLEMENTED IN THIS SCHEMA
	--  To implement this procedure you have to install the Events DataBlade.
	
	-- *** WARNING!!! This function has been deprecated on PDS v.14.0.0 ***
	-- ***         and will be removed in the nex major release         ***

	RETURN 0;
	
END ;
$$ LANGUAGE plpgsql ;



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
	payload JSON ;
	
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
	
	-- Broadcast the event
	payload := JSON_BUILD_OBJECT(
			'serverip', inet_server_addr(),
			'clientip', inet_client_addr(),
			'user', current_user,
			'process', current_setting('application_name'),
			'pid', pg_backend_pid(),
			'database', current_database(),
			'subsystem', 'PDS',
			'table', 't_orderstatetransitions',
			'action', comp_stat,
			'affectedrows', 1,
			'id', in_ordid::VARCHAR );	
	CALL p_events(payload::TEXT) ;
	
	RETURN 0;
	
END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	p_purgewaitingorders
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_purgewaitingorders(
	ftype VARCHAR(16), 
	ckstart TIMESTAMP(6), 
	ckstop TIMESTAMP(6)) RETURNS INTEGER AS $$

DECLARE
	row RECORD ;
	payload JSON ;

BEGIN
	FOR row IN (
		SELECT DISTINCT orderid
		FROM t_waitingorders WHERE 
			-- orders no waiting any longer due the presence of the new ineventory ftype
			(triggertype = 'FILETYPEMATCH' AND filetype = ftype)
			-- orders no waiting any longer due the covering by the new inventoy
			OR ((triggertype = 'INTERSECT' OR triggertype = 'COVER') AND filetype = ftype AND ckstart <= starttime AND ckstop >= stoptime)
			-- and the intersections
			OR (triggertype = 'INTERSECT' AND filetype = ftype AND ( NOT ( ckstart >= stoptime OR starttime >= ckstop))) 
	) LOOP
		
		-- delete records from table
		DELETE FROM t_waitingorders WHERE orderid = row.orderid ;
		-- Broadcast the event
		payload := JSON_BUILD_OBJECT(
				'serverip', inet_server_addr(),
				'clientip', inet_client_addr(),
				'user', current_user,
				'process', current_setting('application_name'),
				'pid', pg_backend_pid(),
				'database', current_database(),
				'subsystem', 'PDS',
				'table', 't_waitingorders',
				'action', 'ORDNOWAI',
				'affectedrows', 1,
				'id', row.orderid::VARCHAR );	
		CALL p_events(payload::TEXT) ;
		
	END LOOP ;
	
	RETURN 0;
END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	p_inventory_manager
*/ ------------------------------
DROP FUNCTION IF EXISTS p_inventory_manager(VARCHAR, VARCHAR, BOOLEAN, BOOLEAN, VARCHAR, VARCHAR, INTEGER, SMALLINT, SMALLINT, SMALLINT, TIMESTAMP, TIMESTAMP);

CREATE OR REPLACE FUNCTION p_inventory_manager(
	invid BIGINT,
	fname VARCHAR(128), 
	fversion VARCHAR(32),
	prevalflag BOOLEAN,
	postvalflag BOOLEAN,
	ftype VARCHAR(16),
	fclass VARCHAR(8),
	orderid INTEGER,
	in_satellite SMALLINT, 
	in_sensor SMALLINT, 
	in_mission SMALLINT,
	valstart TIMESTAMP(6),
	valstop TIMESTAMP(6)) RETURNS INTEGER AS $$
	
DECLARE
	evnttype VARCHAR ;
	payload JSON ;
  
BEGIN
	-- Check for satellite-sensor-mission relation
	PERFORM p_checksatsensmiss(in_satellite, in_sensor, in_mission) ;
	
	-- If is called after an insert
	IF prevalflag IS NULL THEN
		evnttype := 'INSFTYPE' ;
		INSERT INTO t_pendinginventory (filename, fileversion) VALUES (fname, fversion) ;
	-- If is called after an update
	ELSE
		evnttype := 'UPDFTYPE' ;
	END IF ;
	
	-- If validity flag is, or became, TRUE
  	IF ((prevalflag IS NULL AND postvalflag = 't') OR (prevalflag <> postvalflag AND postvalflag = 't')) THEN
  	  	PERFORM p_processinventory(fname, fversion, postvalflag, ftype, fclass, orderid, in_satellite, valstart, valstop) ;
		UPDATE t_filetypes_last_inv SET last_valid = NOW() WHERE filetype = ftype ;
  	END IF ;

	----------------------------------------------------------------------------
	-- BROADCAST SECTION
	----------------------------------------------------------------------------
	payload := JSON_BUILD_OBJECT(
			'serverip', inet_server_addr(),
			'clientip', inet_client_addr(),
			'user', current_user,
			'process', current_setting('application_name'),
			'pid', pg_backend_pid(),
			'database', current_database(),
			'subsystem', 'PDS',
			'table', 't_inventory',
			'action', ftype || ': ' || evnttype,
			'affectedrows', 1,
			'id', invid::VARCHAR );	
	CALL p_events(payload::TEXT) ;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;








-------------------------------------------------------------------------------
-------------------------- TRIGGER FUNCTIONS CREATION -------------------------
-------------------------------------------------------------------------------

-- *********************************************** --
--    WARNING !!!! Do NOT execute this command     --
-- if BROADCAST schema plugin is already installed --
-- *********************************************** --
/* --------------------------
	tf_events
*/ --------------------------
CREATE OR REPLACE FUNCTION tf_events() RETURNS TRIGGER AS $$

BEGIN
	--  NOT IMPLEMENTED IN THIS SCHEMA
	--  To implement this procedure you have to install the BROADCAST schema v.1.0.0 or major
	
	RETURN NULL ;
	
END ;
$$ LANGUAGE plpgsql ;
-- END OF WARNING --








-------------------------------------------------------------------------------
-------------------------- TRIGGER FUNCTIONS CHANGES --------------------------
-------------------------------------------------------------------------------

/* ------------------------------
	tf_cartsinvtrmgr
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_cartsinvtrmgr() RETURNS TRIGGER AS $$

BEGIN
  	PERFORM p_cartsinvcheck(NEW.id, NEW.cart, NEW.subreq, NEW.distsubreq, NEW.ordsubreq, NEW.stndsubreq ) ;
	
  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	tf_distributionitems_manager
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_distributionitems_manager() RETURNS TRIGGER AS $$

BEGIN
	IF (TG_OP = 'INSERT') THEN
		PERFORM p_inv_dist_counter_by_items(NEW.filename, NEW.fileversion, 'INS') ;
		RETURN NEW ;
	END IF ;
	
	IF (TG_OP = 'UPDATE') THEN
		PERFORM p_PurgeDistributionWaitingItems(NEW.id, 'distributionitem_id') ;
		RETURN NEW ;
	END IF ;
	
	IF (TG_OP = 'DELETE') THEN
		PERFORM p_inv_dist_counter_by_items(OLD.filename, OLD.fileversion, 'DEL') ;
		RETURN OLD ;
	END IF ;
	
END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	tf_distributionqueue_manager
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_distributionqueue_manager() RETURNS TRIGGER AS $$

BEGIN
	IF (TG_OP = 'INSERT') THEN
		PERFORM p_cartsinv_update(NEW.packageid, NEW.status);
		RETURN NEW ;
	END IF ;
	
	IF (TG_OP = 'UPDATE') THEN
		PERFORM p_cartsinv_update(NEW.packageid, NEW.status);
		PERFORM p_inv_dist_counter_by_queue(NEW.packageid, OLD.status, NEW.status, 'UPD') ;
		PERFORM p_PurgeDistributionWaitingItems(NEW.packageid, 'package_id') ;
		RETURN NEW ;
	END IF ;
	
	IF (TG_OP = 'DELETE') THEN
		PERFORM p_inv_dist_counter_by_queue(OLD.packageid, OLD.status, NULL, 'DEL') ;
		RETURN OLD ;
	END IF ;
	
END ;
$$ LANGUAGE plpgsql ;



/* -------------------------------
	tf_distributionqueue_statcheck
*/ -------------------------------
CREATE OR REPLACE FUNCTION tf_distributionqueue_statcheck() RETURNS TRIGGER AS $$

DECLARE
	preStatId INT ;
	postStatId INT ;
	errMsg VARCHAR(128) ;
	
BEGIN
	IF (TG_OP = 'UPDATE') THEN
		preStatId := (SELECT id FROM t_stati WHERE status = OLD.status) ;
		postStatId := (SELECT id FROM t_stati WHERE status = NEW.status) ;
		
		IF postStatId IS NULL THEN
			errMsg := 'tf_distributionqueue_statcheck: ' || NEW.status || ' status does not exist.' ;
			RAISE EXCEPTION '%', errMsg ;
		END IF ;
		
		PERFORM p_statetransitions_check('t_distributionqueue', 1, preStatId, postStatId);
	END IF ;
	
	IF (NEW.status = 'ELIGIBLE') THEN
		NEW.retrycounter := 0 ;
	END IF ;
	
	RETURN NEW ;
	
END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	tf_filetype_insert
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_filetype_insert() RETURNS TRIGGER AS $$

BEGIN
	INSERT INTO t_filetypes_last_inv (filetype) VALUES (NEW.filetype) ;
	
	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	tf_inventoryinsevent
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_inventoryinsevent() RETURNS TRIGGER AS $$

BEGIN
	PERFORM p_inventory_manager(NEW.id_inv, NEW.filename, NEW.fileversion, NULL, NEW.validityflag, NEW.filetype, NEW.fileclass, NEW.orderid, NEW.satelliteid, NEW.sensorid, NEW.mission, NEW.validitystart, NEW.validitystop) ;
	PERFORM p_insert_inventoryevents(NEW.id_inv, NEW.filename, NEW.fileversion, NEW.filetype, NULL, NEW.validityflag, 'I') ;
	
	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	tf_inventorysmmanager
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_inventorysmmanager() RETURNS TRIGGER AS $$

BEGIN
	IF (TG_OP = 'INSERT') 
	THEN
		PERFORM p_insert_inventoryevents(NEW.inv_id, NEW.storagemanid) ;
		PERFORM p_insert_inv_data_restored(NEW.id, NEW.inv_id, NEW.storagemanid) ;
		RETURN NEW ;
	END IF ;
	
	IF (TG_OP = 'UPDATE') 
	THEN
		PERFORM p_insert_inventoryevents(NEW.inv_id, NEW.storagemanid) ;
		RETURN NEW ;
	END IF ;
	
	IF (TG_OP = 'DELETE') 
	THEN 
		PERFORM p_insert_inventoryevents(OLD.inv_id, OLD.storagemanid) ;
		
		IF (SELECT count(*) FROM t_inventorysm ism
			INNER JOIN t_storagemanager stm ON ism.storagemanid = stm.id
			WHERE ism.inv_id = OLD.inv_id AND stm.syncdownload = 't') = 0 
		THEN
			DELETE FROM t_sm_sync_missingactions WHERE inv_id = OLD.inv_id AND operation = 'UPLOAD' ;
		END IF ;
		
		RETURN OLD ;
	END IF ;
	
	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	tf_inventoryupdevent
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_inventoryupdevent() RETURNS TRIGGER AS $$

BEGIN
	PERFORM p_inventory_manager(NEW.id_inv, NEW.filename, NEW.fileversion, OLD.validityflag, NEW.validityflag, NEW.filetype, NEW.fileclass, NEW.orderid, NEW.satelliteid, NEW.sensorid, NEW.mission, NEW.validitystart, NEW.validitystop) ;
	PERFORM p_insert_inventoryevents(NEW.id_inv, NULL, NULL, NEW.filetype, OLD.validityflag, NEW.validityflag, 'U') ;
	
	IF (NEW.validityflag != OLD.validityflag) THEN
		IF (NEW.validityflag = 't') THEN
			PERFORM p_purgewaitingorders(NEW.filetype, NEW.validitystart, NEW.validitystop) ;
		ELSE
			PERFORM p_PurgeDistributionWaitingItems(NEW.id_inv, 'inventory_id') ;
		END IF;
	END IF;
	
	IF OLD.qualityinfo != NEW.qualityinfo THEN
		PERFORM p_purgewaitingorders(NEW.filetype, NEW.validitystart, NEW.validitystop) ;
	END IF ;
	
	IF (OLD.qualityinfo = 'DUP' AND (NEW.qualityinfo = '' OR NEW.qualityinfo IS NULL) AND NEW.validityflag = 't') THEN
		PERFORM p_generatetmporder('t', OLD.filename, OLD.fileversion, OLD.filetype, OLD.fileclass, OLD.satelliteid) ;
	END IF ;
	
	IF OLD.qualified != NEW.qualified THEN
		PERFORM p_purgewaitingorders(NEW.filetype, NEW.validitystart, NEW.validitystop) ;
	END IF ;
	
  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	tf_ordersqueue_manager
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_ordersqueue_manager() RETURNS TRIGGER AS $$

BEGIN
	IF (TG_OP = 'INSERT') THEN
		PERFORM p_orderstatetransitions_mgr(NEW.orderid, NULL, NEW.status) ;
		PERFORM p_orders_manager(NEW.orderid, NEW.status, NEW.status) ;
		RETURN NEW ;
	END IF ;
	
	IF (TG_OP = 'UPDATE') THEN
		PERFORM p_orderstatetransitions_mgr(NEW.orderid, OLD.status, NEW.status) ;
		PERFORM p_orders_manager(OLD.orderid, OLD.status, NEW.status) ;
		PERFORM p_orderqueuecheckchanges(OLD.orderid, OLD.status, NEW.status, OLD.processorname, NEW.processorname, OLD.processorversion, NEW.processorversion, OLD.processorconfiguration, NEW.processorconfiguration, OLD.starttime, NEW.starttime, OLD.stoptime, NEW.stoptime, OLD.ordertype, NEW.ordertype, OLD.satelliteid, NEW.satelliteid, OLD.mission, NEW.mission) ;
		RETURN NEW ;
	END IF ;
	
END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	tf_smactionsinsert
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_smactionsinsert() 
	RETURNS TRIGGER AS $$
BEGIN
	IF NEW.packageid IS NOT NULL THEN
		RAISE EXCEPTION 'tf_smactionsInsert: Cannot insert a new record having a packageid';
	END IF;
	
  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	tf_smactions_upd_aft
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_smactions_upd_aft() 
	RETURNS TRIGGER AS $$
BEGIN
	PERFORM p_smactionsupdate(NEW.filename, NEW.fileversion, NEW.packageid) ;
	
  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;







-------------------------------------------------------------------------------
------------------------------ TRIGGERS CREATION ------------------------------
-------------------------------------------------------------------------------

-- ***** BROADCAST SECTION ***** --
-- ***************************** --

/* ------------------------------
	t_acsisminvrelation
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_acsisminvrelation_broadcast_insert
			AFTER INSERT ON t_acsisminvrelation
				REFERENCING NEW TABLE AS t_acsisminvrelation_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_acsisminvrelation_broadcast_update
			AFTER UPDATE ON t_acsisminvrelation
				REFERENCING OLD TABLE AS t_acsisminvrelation_update_old NEW TABLE AS t_acsisminvrelation_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_acsisminvrelation_broadcast_delete
			AFTER DELETE ON t_acsisminvrelation
				REFERENCING OLD TABLE AS t_acsisminvrelation_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_acsltainvrelation
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_acsltainvrelation_broadcast_insert
			AFTER INSERT ON t_acsltainvrelation
				REFERENCING NEW TABLE AS t_acsltainvrelation_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_acsltainvrelation_broadcast_update
			AFTER UPDATE ON t_acsltainvrelation
				REFERENCING OLD TABLE AS t_acsltainvrelation_update_old NEW TABLE AS t_acsltainvrelation_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_acsltainvrelation_broadcast_delete
			AFTER DELETE ON t_acsltainvrelation
				REFERENCING OLD TABLE AS t_acsltainvrelation_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_algorithm
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_algorithm_broadcast_insert
			AFTER INSERT ON t_algorithm
				REFERENCING NEW TABLE AS t_algorithm_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_algorithm_broadcast_update
			AFTER UPDATE ON t_algorithm
				REFERENCING OLD TABLE AS t_algorithm_update_old NEW TABLE AS t_algorithm_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_algorithm_broadcast_delete
			AFTER DELETE ON t_algorithm
				REFERENCING OLD TABLE AS t_algorithm_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_anomaly_types
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_anomaly_types_broadcast_insert
			AFTER INSERT ON t_anomaly_types
				REFERENCING NEW TABLE AS t_anomaly_types_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_anomaly_types_broadcast_update
			AFTER UPDATE ON t_anomaly_types
				REFERENCING OLD TABLE AS t_anomaly_types_update_old NEW TABLE AS t_anomaly_types_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_anomaly_types_broadcast_delete
			AFTER DELETE ON t_anomaly_types
				REFERENCING OLD TABLE AS t_anomaly_types_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_applications
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_applications_broadcast_insert
			AFTER INSERT ON t_applications
				REFERENCING NEW TABLE AS t_applications_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_applications_broadcast_update
			AFTER UPDATE ON t_applications
				REFERENCING OLD TABLE AS t_applications_update_old NEW TABLE AS t_applications_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_applications_broadcast_delete
			AFTER DELETE ON t_applications
				REFERENCING OLD TABLE AS t_applications_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_applicationevents
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_applicationevents_broadcast_insert
			AFTER INSERT ON t_applicationevents
				REFERENCING NEW TABLE AS t_applicationevents_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_applicationevents_broadcast_update
			AFTER UPDATE ON t_applicationevents
				REFERENCING OLD TABLE AS t_applicationevents_update_old NEW TABLE AS t_applicationevents_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_applicationevents_broadcast_delete
			AFTER DELETE ON t_applicationevents
				REFERENCING OLD TABLE AS t_applicationevents_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_applicationstatus
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_applicationstatus_broadcast_insert
			AFTER INSERT ON t_applicationstatus
				REFERENCING NEW TABLE AS t_applicationstatus_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_applicationstatus_broadcast_update
			AFTER UPDATE ON t_applicationstatus
				REFERENCING OLD TABLE AS t_applicationstatus_update_old NEW TABLE AS t_applicationstatus_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_applicationstatus_broadcast_delete
			AFTER DELETE ON t_applicationstatus
				REFERENCING OLD TABLE AS t_applicationstatus_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_cartsinv
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_cartsinv_broadcast_insert
			AFTER INSERT ON t_cartsinv
				REFERENCING NEW TABLE AS t_cartsinv_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_cartsinv_broadcast_update
			AFTER UPDATE ON t_cartsinv
				REFERENCING OLD TABLE AS t_cartsinv_update_old NEW TABLE AS t_cartsinv_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_cartsinv_broadcast_delete
			AFTER DELETE ON t_cartsinv
				REFERENCING OLD TABLE AS t_cartsinv_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_completed_orders
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_completed_orders_broadcast_insert
			AFTER INSERT ON t_completed_orders
				REFERENCING NEW TABLE AS t_completed_orders_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_completed_orders_broadcast_update
			AFTER UPDATE ON t_completed_orders
				REFERENCING OLD TABLE AS t_completed_orders_update_old NEW TABLE AS t_completed_orders_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_completed_orders_broadcast_delete
			AFTER DELETE ON t_completed_orders
				REFERENCING OLD TABLE AS t_completed_orders_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_configurationschemas
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_configurationschemas_broadcast_insert
			AFTER INSERT ON t_configurationschemas
				REFERENCING NEW TABLE AS t_configurationschemas_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_configurationschemas_broadcast_update
			AFTER UPDATE ON t_configurationschemas
				REFERENCING OLD TABLE AS t_configurationschemas_update_old NEW TABLE AS t_configurationschemas_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_configurationschemas_broadcast_delete
			AFTER DELETE ON t_configurationschemas
				REFERENCING OLD TABLE AS t_configurationschemas_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_configurationspaces
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_configurationspaces_broadcast_insert
			AFTER INSERT ON t_configurationspaces
				REFERENCING NEW TABLE AS t_configurationspaces_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_configurationspaces_broadcast_update
			AFTER UPDATE ON t_configurationspaces
				REFERENCING OLD TABLE AS t_configurationspaces_update_old NEW TABLE AS t_configurationspaces_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_configurationspaces_broadcast_delete
			AFTER DELETE ON t_configurationspaces
				REFERENCING OLD TABLE AS t_configurationspaces_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_configurationstylesheets
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_configurationstylesheets_broadcast_insert
			AFTER INSERT ON t_configurationstylesheets
				REFERENCING NEW TABLE AS t_configurationstylesheets_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_configurationstylesheets_broadcast_update
			AFTER UPDATE ON t_configurationstylesheets
				REFERENCING OLD TABLE AS t_configurationstylesheets_update_old NEW TABLE AS t_configurationstylesheets_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_configurationstylesheets_broadcast_delete
			AFTER DELETE ON t_configurationstylesheets
				REFERENCING OLD TABLE AS t_configurationstylesheets_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_distributionformat
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_distributionformat_broadcast_insert
			AFTER INSERT ON t_distributionformat
				REFERENCING NEW TABLE AS t_distributionformat_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_distributionformat_broadcast_update
			AFTER UPDATE ON t_distributionformat
				REFERENCING OLD TABLE AS t_distributionformat_update_old NEW TABLE AS t_distributionformat_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_distributionformat_broadcast_delete
			AFTER DELETE ON t_distributionformat
				REFERENCING OLD TABLE AS t_distributionformat_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_distributionitems
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_distributionitems_broadcast_insert
			AFTER INSERT ON t_distributionitems
				REFERENCING NEW TABLE AS t_distributionitems_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_distributionitems_broadcast_update
			AFTER UPDATE ON t_distributionitems
				REFERENCING OLD TABLE AS t_distributionitems_update_old NEW TABLE AS t_distributionitems_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_distributionitems_broadcast_delete
			AFTER DELETE ON t_distributionitems
				REFERENCING OLD TABLE AS t_distributionitems_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_distributionqueue
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_distributionqueue_broadcast_insert
			AFTER INSERT ON t_distributionqueue
				REFERENCING NEW TABLE AS t_distributionqueue_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_distributionqueue_broadcast_update
			AFTER UPDATE ON t_distributionqueue
				REFERENCING OLD TABLE AS t_distributionqueue_update_old NEW TABLE AS t_distributionqueue_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_distributionqueue_broadcast_delete
			AFTER DELETE ON t_distributionqueue
				REFERENCING OLD TABLE AS t_distributionqueue_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_distributionrules
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_distributionrules_broadcast_insert
			AFTER INSERT ON t_distributionrules
				REFERENCING NEW TABLE AS t_distributionrules_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_distributionrules_broadcast_update
			AFTER UPDATE ON t_distributionrules
				REFERENCING OLD TABLE AS t_distributionrules_update_old NEW TABLE AS t_distributionrules_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_distributionrules_broadcast_delete
			AFTER DELETE ON t_distributionrules
				REFERENCING OLD TABLE AS t_distributionrules_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_filetypes
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_filetypes_broadcast_insert
			AFTER INSERT ON t_filetypes
				REFERENCING NEW TABLE AS t_filetypes_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_filetypes_broadcast_update
			AFTER UPDATE ON t_filetypes
				REFERENCING OLD TABLE AS t_filetypes_update_old NEW TABLE AS t_filetypes_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_filetypes_broadcast_delete
			AFTER DELETE ON t_filetypes
				REFERENCING OLD TABLE AS t_filetypes_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_inventory
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_inventory_broadcast_insert
			AFTER INSERT ON t_inventory
				REFERENCING NEW TABLE AS t_inventory_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_inventory_broadcast_update
			AFTER UPDATE ON t_inventory
				REFERENCING OLD TABLE AS t_inventory_update_old NEW TABLE AS t_inventory_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_inventory_broadcast_delete
			AFTER DELETE ON t_inventory
				REFERENCING OLD TABLE AS t_inventory_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_inventoryanomalies
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_inventoryanomalies_broadcast_insert
			AFTER INSERT ON t_inventoryanomalies
				REFERENCING NEW TABLE AS t_inventoryanomalies_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_inventoryanomalies_broadcast_update
			AFTER UPDATE ON t_inventoryanomalies
				REFERENCING OLD TABLE AS t_inventoryanomalies_update_old NEW TABLE AS t_inventoryanomalies_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_inventoryanomalies_broadcast_delete
			AFTER DELETE ON t_inventoryanomalies
				REFERENCING OLD TABLE AS t_inventoryanomalies_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_inventorycomments
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_inventorycomments_broadcast_insert
			AFTER INSERT ON t_inventorycomments
				REFERENCING NEW TABLE AS t_inventorycomments_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_inventorycomments_broadcast_update
			AFTER UPDATE ON t_inventorycomments
				REFERENCING OLD TABLE AS t_inventorycomments_update_old NEW TABLE AS t_inventorycomments_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_inventorycomments_broadcast_delete
			AFTER DELETE ON t_inventorycomments
				REFERENCING OLD TABLE AS t_inventorycomments_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_inventorygaps
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_inventorygaps_broadcast_insert
			AFTER INSERT ON t_inventorygaps
				REFERENCING NEW TABLE AS t_inventorygaps_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_inventorygaps_broadcast_update
			AFTER UPDATE ON t_inventorygaps
				REFERENCING OLD TABLE AS t_inventorygaps_update_old NEW TABLE AS t_inventorygaps_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_inventorygaps_broadcast_delete
			AFTER DELETE ON t_inventorygaps
				REFERENCING OLD TABLE AS t_inventorygaps_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_inventorysm
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_inventorysm_broadcast_insert
			AFTER INSERT ON t_inventorysm
				REFERENCING NEW TABLE AS t_inventorysm_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_inventorysm_broadcast_update
			AFTER UPDATE ON t_inventorysm
				REFERENCING OLD TABLE AS t_inventorysm_update_old NEW TABLE AS t_inventorysm_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_inventorysm_broadcast_delete
			AFTER DELETE ON t_inventorysm
				REFERENCING OLD TABLE AS t_inventorysm_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_ordersinput
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_ordersinput_broadcast_insert
			AFTER INSERT ON t_ordersinput
				REFERENCING NEW TABLE AS t_ordersinput_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_ordersinput_broadcast_update
			AFTER UPDATE ON t_ordersinput
				REFERENCING OLD TABLE AS t_ordersinput_update_old NEW TABLE AS t_ordersinput_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_ordersinput_broadcast_delete
			AFTER DELETE ON t_ordersinput
				REFERENCING OLD TABLE AS t_ordersinput_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_ordersqueue
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_ordersqueue_broadcast_insert
			AFTER INSERT ON t_ordersqueue
				REFERENCING NEW TABLE AS t_ordersqueue_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_ordersqueue_broadcast_update
			AFTER UPDATE ON t_ordersqueue
				REFERENCING OLD TABLE AS t_ordersqueue_update_old NEW TABLE AS t_ordersqueue_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_ordersqueue_broadcast_delete
			AFTER DELETE ON t_ordersqueue
				REFERENCING OLD TABLE AS t_ordersqueue_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_orderstatetransitions
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_orderstatetransitions_broadcast_insert
			AFTER INSERT ON t_orderstatetransitions
				REFERENCING NEW TABLE AS t_orderstatetransitions_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_orderstatetransitions_broadcast_update
			AFTER UPDATE ON t_orderstatetransitions
				REFERENCING OLD TABLE AS t_orderstatetransitions_update_old NEW TABLE AS t_orderstatetransitions_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_orderstatetransitions_broadcast_delete
			AFTER DELETE ON t_orderstatetransitions
				REFERENCING OLD TABLE AS t_orderstatetransitions_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_receptionrules
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_receptionrules_broadcast_insert
			AFTER INSERT ON t_receptionrules
				REFERENCING NEW TABLE AS t_receptionrules_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_receptionrules_broadcast_update
			AFTER UPDATE ON t_receptionrules
				REFERENCING OLD TABLE AS t_receptionrules_update_old NEW TABLE AS t_receptionrules_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_receptionrules_broadcast_delete
			AFTER DELETE ON t_receptionrules
				REFERENCING OLD TABLE AS t_receptionrules_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_repositories
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_repositories_broadcast_insert
			AFTER INSERT ON t_repositories
				REFERENCING NEW TABLE AS t_repositories_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_repositories_broadcast_update
			AFTER UPDATE ON t_repositories
				REFERENCING OLD TABLE AS t_repositories_update_old NEW TABLE AS t_repositories_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_repositories_broadcast_delete
			AFTER DELETE ON t_repositories
				REFERENCING OLD TABLE AS t_repositories_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_smactions
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_smactions_broadcast_insert
			AFTER INSERT ON t_smactions
				REFERENCING NEW TABLE AS t_smactions_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_smactions_broadcast_update
			AFTER UPDATE ON t_smactions
				REFERENCING OLD TABLE AS t_smactions_update_old NEW TABLE AS t_smactions_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_smactions_broadcast_delete
			AFTER DELETE ON t_smactions
				REFERENCING OLD TABLE AS t_smactions_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_smqueue
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_smqueue_broadcast_insert
			AFTER INSERT ON t_smqueue
				REFERENCING NEW TABLE AS t_smqueue_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_smqueue_broadcast_update
			AFTER UPDATE ON t_smqueue
				REFERENCING OLD TABLE AS t_smqueue_update_old NEW TABLE AS t_smqueue_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_smqueue_broadcast_delete
			AFTER DELETE ON t_smqueue
				REFERENCING OLD TABLE AS t_smqueue_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_temporarydistribution
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_temporarydistribution_broadcast_insert
			AFTER INSERT ON t_temporarydistribution
				REFERENCING NEW TABLE AS t_temporarydistribution_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_temporarydistribution_broadcast_update
			AFTER UPDATE ON t_temporarydistribution
				REFERENCING OLD TABLE AS t_temporarydistribution_update_old NEW TABLE AS t_temporarydistribution_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_temporarydistribution_broadcast_delete
			AFTER DELETE ON t_temporarydistribution
				REFERENCING OLD TABLE AS t_temporarydistribution_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_temporaryorders
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_temporaryorders_broadcast_insert
			AFTER INSERT ON t_temporaryorders
				REFERENCING NEW TABLE AS t_temporaryorders_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_temporaryorders_broadcast_update
			AFTER UPDATE ON t_temporaryorders
				REFERENCING OLD TABLE AS t_temporaryorders_update_old NEW TABLE AS t_temporaryorders_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_temporaryorders_broadcast_delete
			AFTER DELETE ON t_temporaryorders
				REFERENCING OLD TABLE AS t_temporaryorders_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_users
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_users_broadcast_insert
			AFTER INSERT ON t_users
				REFERENCING NEW TABLE AS t_users_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_users_broadcast_update
			AFTER UPDATE ON t_users
				REFERENCING OLD TABLE AS t_users_update_old NEW TABLE AS t_users_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_users_broadcast_delete
			AFTER DELETE ON t_users
				REFERENCING OLD TABLE AS t_users_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;


/* ------------------------------
	t_waitingorders
*/ ------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.routines WHERE routine_name = 'tf_events') = 1
	THEN
		CREATE TRIGGER tr_waitingorders_broadcast_insert
			AFTER INSERT ON t_waitingorders
				REFERENCING NEW TABLE AS t_waitingorders_insert_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events('ORDWAIT');

		CREATE TRIGGER tr_waitingorders_broadcast_update
			AFTER UPDATE ON t_waitingorders
				REFERENCING OLD TABLE AS t_waitingorders_update_old NEW TABLE AS t_waitingorders_update_new
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();

		CREATE TRIGGER tr_waitingorders_broadcast_delete
			AFTER DELETE ON t_waitingorders
				REFERENCING OLD TABLE AS t_waitingorders_delete_old
					FOR EACH STATEMENT EXECUTE PROCEDURE tf_events();
	END IF ;
END $$;

-- ************************************ --
-- ***** END OF BROADCAST SECTION ***** --








-------------------------------------------------------------------------------
------------------------------- TRIGGERS CHANGES ------------------------------
-------------------------------------------------------------------------------

/* ------------------------------
	t_cartsinv
*/ ------------------------------
DROP TRIGGER IF EXISTS tr_cartsinvinsert ON t_cartsinv ;
DROP TRIGGER IF EXISTS tr_cartsinvupdate ON t_cartsinv ;

CREATE TRIGGER tr_cartsinv_manager AFTER INSERT OR UPDATE
	ON t_cartsinv
		FOR EACH ROW EXECUTE PROCEDURE tf_cartsinvtrmgr() ;


/* ------------------------------
	t_distributionitems
*/ ------------------------------
DROP TRIGGER IF EXISTS tr_distributionitemsinsert ON t_distributionitems ;
DROP TRIGGER IF EXISTS tr_distributionitemsupdate ON t_distributionitems ;
DROP TRIGGER IF EXISTS tr_distributionitemsdelete ON t_distributionitems ;

CREATE TRIGGER tr_distributionitems_manager AFTER INSERT OR UPDATE OR DELETE
	ON t_distributionitems 
		FOR EACH ROW EXECUTE PROCEDURE tf_distributionitems_manager() ;


/* ------------------------------
	t_distributionqueue
*/ ------------------------------
DROP TRIGGER IF EXISTS tr_distributionqueue_ins_bfr ON t_distributionqueue ;
DROP TRIGGER IF EXISTS tr_distributionqueue_upd_bfr ON t_distributionqueue ;
DROP TRIGGER IF EXISTS tr_distributionqueue_ins_aft ON t_distributionqueue ;
DROP TRIGGER IF EXISTS tr_distributionqueue_upd_aft ON t_distributionqueue ;
DROP TRIGGER IF EXISTS tr_distributionqueue_del_aft ON t_distributionqueue ;

CREATE TRIGGER tr_distributionqueue_ins_bfr BEFORE INSERT
	ON t_distributionqueue
		FOR EACH ROW EXECUTE PROCEDURE tf_distributionqueue_statcheck() ;

CREATE TRIGGER tr_distributionqueue_upd_bfr BEFORE UPDATE OF status
	ON t_distributionqueue
		FOR EACH ROW EXECUTE PROCEDURE tf_distributionqueue_statcheck() ;

CREATE TRIGGER tr_distributionqueue_manager AFTER INSERT OR UPDATE OR DELETE
	ON t_distributionqueue 
		FOR EACH ROW EXECUTE PROCEDURE tf_distributionqueue_manager();


/* ------------------------------
	t_inventorysm
*/ ------------------------------
DROP TRIGGER IF EXISTS tr_inventorysminsert ON t_inventorysm ;
DROP TRIGGER IF EXISTS tr_inventorysmupdate ON t_inventorysm ;
DROP TRIGGER IF EXISTS tr_inventorysmdelete ON t_inventorysm ;

CREATE TRIGGER tr_inventorysm_manager AFTER INSERT OR UPDATE OR DELETE
	ON t_inventorysm 
		FOR EACH ROW EXECUTE PROCEDURE tf_inventorysmmanager() ;



/* ------------------------------
	t_ordersqueue
*/ ------------------------------
DROP TRIGGER IF EXISTS tr_ordersqueue_ins_aft ON t_ordersqueue ;
DROP TRIGGER IF EXISTS tr_ordersqueue_upd_aft ON t_ordersqueue ;
DROP TRIGGER IF EXISTS tr_ordersqueue_del_aft ON t_ordersqueue ;

CREATE TRIGGER tr_ordersqueue_manager AFTER INSERT OR UPDATE
	ON t_ordersqueue 
		FOR EACH ROW EXECUTE PROCEDURE tf_ordersqueue_manager();









-------------------------------------------------------------------------------
-------------------------------- TRIGGERS DROP --------------------------------
-------------------------------------------------------------------------------

/* ------------------------------
	t_acsltainvrelation
*/ ------------------------------
DROP TRIGGER IF EXISTS tr_acsltainvrelationinsert ON t_acsltainvrelation ;
DROP TRIGGER IF EXISTS tr_acsltainvrelationupdate ON t_acsltainvrelation ;


/* ------------------------------
	t_applicationstatus
*/ ------------------------------
DROP TRIGGER IF EXISTS tr_applicationstatusinsert ON t_applicationstatus ;
DROP TRIGGER IF EXISTS tr_applicationstatusupdate ON t_applicationstatus ;


/* ------------------------------
	t_completed_orders
*/ ------------------------------
DROP TRIGGER IF EXISTS tr_completedordersinsert ON t_completed_orders ;
DROP TRIGGER IF EXISTS tr_completedordersupdate ON t_completed_orders ;


/* ------------------------------
	t_configurationschemas
*/ ------------------------------
DROP TRIGGER IF EXISTS tr_configurationschemasinsert ON t_configurationschemas ;
DROP TRIGGER IF EXISTS tr_configurationschemasupdate ON t_configurationschemas ;


/* ------------------------------
	t_configurationspaces
*/ ------------------------------
DROP TRIGGER IF EXISTS tr_configurationspacesinsert ON t_configurationspaces ;
DROP TRIGGER IF EXISTS tr_configurationspacesupdate ON t_configurationspaces ;


/* ------------------------------
	t_configurationstylesheets
*/ ------------------------------
DROP TRIGGER IF EXISTS tr_configurationstylesheetsinsert ON t_configurationstylesheets ;
DROP TRIGGER IF EXISTS tr_configurationstylesheetsupdate ON t_configurationstylesheets ;


/* ------------------------------
	t_distributionrules
*/ ------------------------------
DROP TRIGGER IF EXISTS tr_distributionrulesinsert ON t_distributionrules  ;
DROP TRIGGER IF EXISTS tr_distributionrulesupdate ON t_distributionrules  ;


/* ------------------------------
	t_filetypes
*/ ------------------------------
DROP TRIGGER IF EXISTS tr_filetypesupdate ON t_filetypes ;


/* ------------------------------
	t_inventorycomments
*/ ------------------------------
DROP TRIGGER IF EXISTS tr_inventorycommentsinsert ON t_inventorycomments ;
DROP TRIGGER IF EXISTS tr_inventorycommentsupdate ON t_inventorycomments ;


/* ------------------------------
	t_inventorygaps
*/ ------------------------------
DROP TRIGGER IF EXISTS tr_inventorygapsinsert ON t_inventorygaps ;
DROP TRIGGER IF EXISTS tr_inventorygapsupdate ON t_inventorygaps ;


/* ------------------------------
	t_ordersinput
*/ ------------------------------
DROP TRIGGER IF EXISTS tr_ordersinputinsert ON t_ordersinput ;
DROP TRIGGER IF EXISTS tr_ordersinputupdate ON t_ordersinput ;


/* ------------------------------
	t_ordersqueue
*/ ------------------------------
DROP TRIGGER IF EXISTS tr_ordersqueue_del_aft ON t_ordersqueue ;


/* ------------------------------
	t_orderstatetransitions
*/ ------------------------------
DROP TRIGGER IF EXISTS tr_ordertransinsert ON t_orderstatetransitions ;
DROP TRIGGER IF EXISTS tr_ordertransupdate ON t_orderstatetransitions ;


/* ------------------------------
	t_receptionrules
*/ ------------------------------
DROP TRIGGER IF EXISTS tr_receptionrulesinsert ON t_receptionrules  ;
DROP TRIGGER IF EXISTS tr_receptionrulesupdate ON t_receptionrules ;


/* ------------------------------
	t_repositories
*/ ------------------------------
DROP TRIGGER IF EXISTS tr_repositoriesinsert ON t_repositories ;
DROP TRIGGER IF EXISTS tr_repositoriesupdate ON t_repositories ;


/* ------------------------------
	t_smqueue
*/ ------------------------------
DROP TRIGGER IF EXISTS tr_smqueue_ins_aft ON t_smqueue ;
DROP TRIGGER IF EXISTS tr_smqueue_upd_aft ON t_smqueue ;
DROP TRIGGER IF EXISTS tr_smqueue_del_aft ON t_smqueue ;


/* ------------------------------
	t_temporarydistribution
*/ ------------------------------
DROP TRIGGER IF EXISTS tr_temporarydistributioninsert ON t_temporarydistribution ;
DROP TRIGGER IF EXISTS tr_temporarydistributionupdate ON t_temporarydistribution ;


/* ------------------------------
	t_temporaryorders
*/ ------------------------------
DROP TRIGGER IF EXISTS tr_temporaryordersinsert ON t_temporaryorders ;
DROP TRIGGER IF EXISTS tr_temporaryordersupdate ON t_temporaryorders ;


/* ------------------------------
	t_users
*/ ------------------------------
DROP TRIGGER IF EXISTS tr_usersinsert ON t_users ;
DROP TRIGGER IF EXISTS tr_usersupdate ON t_users ;


/* ------------------------------
	t_waitingorders
*/ ------------------------------
DROP TRIGGER IF EXISTS tr_waitingordersinsert ON t_waitingorders ;
DROP TRIGGER IF EXISTS tr_waitingordersupdate ON t_waitingorders;








-------------------------------------------------------------------------------
---------------------------- TRIGGER FUNCTIONS DROP ---------------------------
-------------------------------------------------------------------------------

/* ------------------------------
	tf_distributionitemsevent
*/ ------------------------------
DROP FUNCTION IF EXISTS tf_distributionitemsevent() ;

/* ------------------------------
	tf_waitingordersinsert
*/ ------------------------------
DROP FUNCTION IF EXISTS tf_waitingordersinsert() ;








-------------------------------------------------------------------------------
--------------------------------- CONCLUSIONS ---------------------------------
-------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'pds-14.00.00' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-14.00.00',
		'2021-09-29',
		3,
		2,
		'PDS_14-0-0',
		'Changed broadcast system. Removed old EventsDBlade triggers and functions. Optimized stored procedures and trigger functions.'
) ;


