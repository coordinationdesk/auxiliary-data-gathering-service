/*

 Exprivia S.p.A.
 Via Della Bufalotta, 378 - 00139 Roma - Italy
 http://www.exprivia.it

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia S.p.A.;
 The contents of this file may not be disclosed to third parties, copied
 or duplicated in any form, in whole or in part, without the prior written
 permission of Exprivia S.p.A.

*/





-------------------------------------------------------------------------------
--                                                                           --
--                                 PROCEDURES                                --
--                                                                           --
-------------------------------------------------------------------------------


/* ------------------------------
	FAKE
*/ ------------------------------
CREATE FUNCTION p_fake() RETURNS TRIGGER AS $$
BEGIN
	RETURN NULL;
END ;
$$ LANGUAGE plpgsql ;





/* ------------------------------
	p_events (old EventsDBlade)
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





/* ------------------------------
	p_events (pg_notify Broadcast)
*/ ------------------------------
CREATE OR REPLACE PROCEDURE p_events(
	payload TEXT) AS $$
	
BEGIN
	--  NOT IMPLEMENTED IN THIS SCHEMA
	--  To implement this procedure you have to install the BROADCAST schema v.1.0.0 or major
	
END ;
$$ LANGUAGE plpgsql ;





/* ------------------------------
	p_application_events
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_application_events(
	in_appname VARCHAR(64), 
	in_evn_type VARCHAR(64),
	in_evn_desc VARCHAR(256)) RETURNS INTEGER AS $$
	
DECLARE
	p_app_id INTEGER := NULL ;
	p_exp_days INTEGER := NULL ;
	
BEGIN
	-- ##### APPLICATION SECTION ##### --
	-- Get the id of the application
	SELECT id_application INTO p_app_id FROM t_applications WHERE app_name = in_appname ;
	
	-- If the application does not exist
	IF p_app_id IS NULL THEN
		-- insert new application into t_applications and get is id
		INSERT INTO t_applications (app_name) VALUES (in_appname) RETURNING id_application INTO p_app_id ;
	END IF ;
	
	-- ##### EVENT SECTION ##### --
	-- Insert new event into t_applicationevents
	INSERT INTO t_applicationevents (
		application_id,
		tstamp,
		event_type,
		event_description)
	VALUES (
		p_app_id,
		now(),
		in_evn_type,
		in_evn_desc) ;
	
	-- ##### CLEANING SECTION ##### --
	-- Clean t_applicationevents removing events olther than app_even_expir_days (t_dbparameters)
	
	-- get the value of app_even_expir_days
	SELECT value_int INTO p_exp_days FROM t_dbparameters WHERE parameter = 'app_even_expir_days' ;
	
	-- if the value is not setted
	IF p_exp_days IS NULL THEN
		-- raise error message
		RAISE EXCEPTION 'p_application_events: Cannot retrieve value for app_even_expir_days db_parameter.' ;
	END IF ;
	-- else... delete
	DELETE FROM t_applicationevents WHERE
		application_id = p_app_id
		AND tstamp < (now() - CAST(p_exp_days || 'days' AS interval)) ;
		
	RETURN 0;
	
END ;
$$ LANGUAGE plpgsql ;




/* ------------------------------
	p_distribute_item
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_distribute_item(
	in_fname VARCHAR(128),
	in_fversion VARCHAR(32),
	in_ftypeid INTEGER,
	in_userid INTEGER,
	in_fileclassid INTEGER,
	in_format INTEGER,
	in_orderid INTEGER,
	in_duedays INTEGER) RETURNS INTEGER AS $$
	
DECLARE
	p_extordid 	VARCHAR(128);
	p_cartid   	INT;
	p_subreq   	INT;
	p_totsubreq	INT;
	p_chng_add	INT;
	
BEGIN
	p_chng_add := NULL ;
	
	p_extordid := (SELECT externalorderid FROM t_ordersqueue WHERE orderid = in_orderid);
	p_subreq := (SELECT subreq FROM t_ordersqueue WHERE orderid = in_orderid);
	
	IF p_extordid IS NOT NULL AND p_subreq IS NOT NULL 
	THEN
		p_cartid := (SELECT id FROM t_cartsref AS cr
				INNER JOIN t_ordersqueue AS oq
				ON oq.externalorderid = cr.extref
				WHERE oq.orderid = in_orderid);

		p_totsubreq := (SELECT max(c.distsubreq) +1 
				FROM t_cartsinv c 
				WHERE c.cart = p_cartid);
	ELSE
		p_cartid := NULL;
		p_totsubreq := NULL;
	END IF;


--	the input inventory file is distributed to each user who declared an interest on that file-type.
--	the distribution is done according to the active field in the distribution policy record
	
	INSERT INTO t_temporarydistribution (
		duedate, 
		filename, 
		fileversion, 
		mediatype, 
		userid, 
		format, 
		algo, 
		policy, 
		orderid, 
		cartid, 
		subreq,
		disseminationpackingalgo_id )
	SELECT DISTINCT
		CASE
			WHEN in_duedays IS NULL THEN (NOW()::DATE + dr.maxdaytodeliver) + NOW()::TIME(6)
			ELSE (NOW()::DATE + in_duedays) + NOW()::TIME(6)
		END,
		in_fname, 
		in_fversion,
		mt.mediatype,
		us.userid,
		dr.format_tt_id, 
		dr.distributionalgo_id, 
		dr.id_distributionrule, 
		in_orderid, 
		p_cartid, 
		p_totsubreq,
		dr.disseminationpackingalgo_id
	FROM t_distributionrules dr
		INNER JOIN t_users us
			ON dr.user_id = us.id_user
		INNER JOIN t_mediatypes mt
			ON dr.mediatype_id = mt.id_mediatype
	WHERE
		dr.user_id = in_userid
		AND (dr.filetype_id = in_ftypeid OR dr.filetype_id IN (SELECT pfiletype FROM t_filetypescomps WHERE cfiletype = in_ftypeid)) 
		AND dr.active = 't'
		AND dr.ondemand = 'f'
		AND (CASE WHEN in_format IS NOT NULL 
				THEN dr.format_tt_id = in_format 
				ELSE dr.format_tt_id = dr.format_tt_id 
			END)
		AND (f_compare_int_values(dr.fileclass_id, in_fileclassid) OR dr.fileclass_id IS NULL)
		AND dr.usermode_id = (SELECT mode FROM t_users WHERE id_user = in_userid)
	;
	
	GET DIAGNOSTICS p_chng_add = ROW_COUNT ;
		
	IF p_chng_add > 0 THEN
		UPDATE t_cartsinv SET distsubreq = p_totsubreq WHERE cart = p_cartid AND ordsubreq = p_subreq ;
	END IF ;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;





/* ------------------------------
	P_DISTRIBUTE_ITEM_ON_DEMAND
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_distribute_item_on_demand(
	in_fname VARCHAR(128),
	in_fversion VARCHAR(32),
	in_ftypeid INTEGER,
	in_userid INTEGER,
	in_fileclassid INTEGER,
	in_format INTEGER,
	in_orderid INTEGER) RETURNS INTEGER AS $$
	
DECLARE
	p_extordid		VARCHAR(128);
	p_cartid		INT;
	p_subreq		INT;
	p_totsubreq		INT;
	p_ondem_media	VARCHAR(16);
	p_chng_add		INT;
	
BEGIN
	p_chng_add := NULL ;

	p_extordid := (SELECT externalorderid FROM t_ordersqueue WHERE orderid = in_orderid);
	p_subreq := (SELECT subreq FROM t_ordersqueue WHERE orderid = in_orderid);
	p_ondem_media := (SELECT od.mediatype FROM t_ondemanddistribution od WHERE od.orderid = in_orderid AND od.userid = (SELECT userid FROM t_users WHERE id_user = in_userid)) ;
	
	
	IF p_extordid IS NOT NULL AND p_subreq IS NOT NULL 
	THEN
		p_cartid := (SELECT id FROM t_cartsref AS cr
					INNER JOIN t_ordersqueue AS oq
						ON oq.externalorderid = cr.extref
					WHERE oq.orderid = in_orderid);

		p_totsubreq := (SELECT max(c.distsubreq) +1 
					FROM t_cartsinv c 
					WHERE c.cart = p_cartid);
	ELSE
		p_cartid := NULL;
		p_totsubreq := NULL;
	END IF;


--	the input inventory file is distributed to each user who declared an interest on that file-type.
--	the distribution is done according to the active field in the distribution policy record
	
	INSERT INTO t_temporarydistribution (
		duedate, 
		filename, 
		fileversion, 
		mediatype, 
		userid, 
		format, 
		algo, 
		policy, 
		orderid, 
		cartid, 
		subreq,
		disseminationpackingalgo_id )
	SELECT DISTINCT
		(NOW()::DATE + dr.maxdaytodeliver) + NOW()::TIME(6),
		in_fname, 
		in_fversion,
		CASE 
			WHEN p_ondem_media IS NULL THEN mt.mediatype
			ELSE p_ondem_media
		END,
		us.userid,
		dr.format_tt_id, 
		dr.distributionalgo_id, 
		dr.id_distributionrule, 
		in_orderid, 
		p_cartid, 
		p_totsubreq,
		dr.disseminationpackingalgo_id
	FROM t_distributionrules dr
		INNER JOIN t_users us
			ON dr.user_id = us.id_user
		INNER JOIN t_mediatypes mt
			ON dr.mediatype_id = mt.id_mediatype
	WHERE
		dr.user_id = in_userid
		AND (dr.filetype_id = in_ftypeid OR dr.filetype_id IN (SELECT pfiletype FROM t_filetypescomps WHERE cfiletype = in_ftypeid)) 
		AND dr.active = 't'
		AND dr.ondemand = 't'
		AND (CASE WHEN in_format IS NOT NULL 
				THEN dr.format_tt_id = in_format 
				ELSE dr.format_tt_id = dr.format_tt_id 
			END)
		AND (f_compare_int_values(dr.fileclass_id, in_fileclassid) OR dr.fileclass_id IS NULL)
		AND dr.usermode_id = (SELECT mode FROM t_users WHERE id_user = in_userid)
		AND (p_ondem_media IS NULL OR dr.distributionalgo_id IN (SELECT id FROM t_distributionalgo WHERE on_demand = 't'))
	;
	
	GET DIAGNOSTICS p_chng_add = ROW_COUNT ;
		
	IF p_chng_add > 0 THEN
		UPDATE t_cartsinv SET distsubreq = p_totsubreq WHERE cart = p_cartid AND ordsubreq = p_subreq ;
	END IF ;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;





/* ------------------------------
	P_DISTRIBUTE_ORDER_INVENTORY
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_distribute_order_inventory(
	in_orderid INTEGER, 
	in_fileclassid INTEGER) RETURNS INTEGER AS $$
	
BEGIN
--	this is the standard way to distribute the output of an order:
--	each inventory is distributed to users who defined a valid distribution rule for it

	IF in_orderid IS NOT NULL THEN
  	  	
		PERFORM p_distribute_item(
			iv.filename,
			iv.fileversion,
			dr.filetype_id,
			dr.user_id,
			(SELECT id_fileclass FROM t_fileclasses WHERE t_fileclasses.fileclass = iv.fileclass),
			dr.format_tt_id,
			iv.orderid,
			NULL)
		FROM 
			t_inventory AS iv
			INNER JOIN (SELECT id_filetype, filetype FROM t_filetypes) AS ft
				ON ft.filetype = iv.filetype
			INNER JOIN t_distributionrules AS dr
				ON (dr.filetype_id = ft.id_filetype 
					OR dr.filetype_id IN (SELECT pfiletype FROM t_filetypescomps WHERE cfiletype = ft.id_filetype))
		WHERE
			iv.orderid = in_orderid
			AND dr.active = 't'
			AND dr.ondemand = 'f'
			AND ((f_compare_int_values(dr.fileclass_id, (SELECT id_fileclass FROM t_fileclasses WHERE t_fileclasses.fileclass = iv.fileclass)) OR dr.fileclass_id IS NULL))
		;
  	END IF ;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;





/* ------------------------------
	p_completed_order_distribution
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_completed_order_distribution(in_orderid INTEGER) RETURNS INTEGER AS $$

DECLARE
  	row_odd RECORD ;
  	
	-- the procedure distributes the output from a completed order
	
BEGIN
	--
	--	check if the users are already assigned to that order looking into t_ondemanddistribution
	--
	IF ((SELECT COUNT(*) FROM t_ondemanddistribution WHERE orderid = in_orderid) = 0) THEN
	--
	--	no predefined user : proceed with the configured ones
	--
		PERFORM p_distribute_order_inventory(in_orderid, NULL) ;
		RETURN 0 ;
	END IF ;
	
	--
	--	a set of users was found: distribute items to that users according to their distribution policies
	--
	FOR row_odd IN (
		SELECT 
			us.id_user AS user_id, 
			odd.format AS format_id
		FROM 
			t_ondemanddistribution odd 
			INNER JOIN t_users us
				ON odd.userid = us.userid
		WHERE 
			odd.orderid = in_orderid) LOOP

  	  	PERFORM p_distribute_item_on_demand(
			inv.filename, 
			inv.fileversion, 
			ft.id_filetype, 
			row_odd.user_id, 
			(SELECT id_fileclass FROM t_fileclasses WHERE t_fileclasses.fileclass = inv.fileclass),
			row_odd.format_id, 
			in_orderid)
		FROM 
			t_inventory inv 
			INNER JOIN t_filetypes ft
				ON inv.filetype = ft.filetype
		WHERE 
			inv.orderid = in_orderid
		;
		
  	END LOOP ;
	
	RETURN 0 ;
END ;
$$ LANGUAGE plpgsql ;





/* ------------------------------
	p_checksatsensmiss
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_checksatsensmiss(
	in_sat SMALLINT, 
	in_sen SMALLINT, 
	in_mis SMALLINT) RETURNS INTEGER AS $$
	
DECLARE
	cnt INTEGER ;
	
BEGIN
	cnt := 0 ;
	
	IF in_sat IS NULL AND in_sen IS NULL AND in_mis IS NULL THEN
		RETURN 0 ;
	END IF ;
		
	IF in_sen IS NULL AND in_mis IS NULL THEN
		SELECT INTO cnt COUNT(*) FROM t_satrelations WHERE satelliteid = in_sat ;
	ELSIF in_sat IS NULL AND in_mis IS NULL THEN
		SELECT INTO cnt COUNT(*) FROM t_satrelations WHERE sensorid = in_sen ;
	ELSIF in_sat IS NULL AND in_sen IS NULL THEN
		SELECT INTO cnt COUNT(*) FROM t_satrelations WHERE mission = in_mis ;
	ELSIF in_mis IS NULL THEN
		SELECT INTO cnt COUNT(*) FROM t_satrelations WHERE satelliteid = in_sat AND sensorid = in_sen ;
	ELSIF in_sat IS NULL THEN
		SELECT INTO cnt COUNT(*) FROM t_satrelations WHERE sensorid = in_sen AND mission = in_mis ;
	ELSIF in_sen IS NULL THEN
		SELECT INTO cnt COUNT(*) FROM t_satrelations WHERE satelliteid = in_sat AND mission = in_mis ;
	ELSE
		SELECT INTO cnt COUNT(*) FROM t_satrelations WHERE satelliteid = in_sat AND sensorid = in_sen AND mission = in_mis ;
	END IF ;
	
	IF (cnt = 0) THEN
		RAISE EXCEPTION 'p_checksatsensmiss: Inconsistent Satellite/Sensor/Mission' ;
	END IF ;
	
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
	p_generatetmporder
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_generatetmporder(
	in_active BOOLEAN,
	fname VARCHAR(128),
	fversion VARCHAR(32),
	ftype VARCHAR(16),
	fclass VARCHAR(8),
	in_satellite SMALLINT) RETURNS INTEGER AS $$
	
BEGIN
	
	INSERT INTO t_temporaryorders (
		filename, 
		fileversion, 
		filetype, 
		ordertype, 
		processorname,
		processorversion, 
		algorithm, 
		processorconfiguration, 
		priority,
		troubleshooting, 
		processingtimewindow, 
		ordersat)
  	SELECT
		fname, 
		fversion, 
		ftype, 
		og.orderType, 
		og.processorName,
		og.processorVersion, 
		og.algorithm, 
		og.processorconfiguration, 
		og.priority,
		og.troubleshooting, 
		og.processingtimewindow, 
		in_satellite
	FROM 
		t_ordersgeneration og
	WHERE 
		og.filetype = ftype 
		AND og.active = in_active 
		AND (og.fileclass IS NULL OR og.fileclass = fclass)
	;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;






/* ------------------------------
	p_generatetmporderpname2
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_generatetmporderpname2(
	in_active BOOLEAN,
	fname VARCHAR(128),
	fversion VARCHAR(32),
	ftype VARCHAR(16),
	fclass VARCHAR(8),
	in_satellite SMALLINT,
	tab_name VARCHAR(30),
	fld_name VARCHAR(30)) RETURNS INTEGER AS $$

DECLARE
	sql_cmd VARCHAR(4000) ;
	
BEGIN
	sql_cmd := '' ;
	sql_cmd := sql_cmd || 'INSERT INTO t_temporaryorders (' ;
	sql_cmd := sql_cmd || 	'filename, ' ;
	sql_cmd := sql_cmd || 	'fileversion, ' ;
	sql_cmd := sql_cmd || 	'filetype, ' ;
	sql_cmd := sql_cmd || 	'ordertype, ' ;
	sql_cmd := sql_cmd || 	'processorname, ' ;
	sql_cmd := sql_cmd || 	'processorversion, '  ;
	sql_cmd := sql_cmd || 	'algorithm, ' ;
	sql_cmd := sql_cmd || 	'processorconfiguration, ' ;
	sql_cmd := sql_cmd || 	'priority, ' ;
	sql_cmd := sql_cmd || 	'troubleshooting, ' ;
	sql_cmd := sql_cmd || 	'processingtimewindow, ' ;
	sql_cmd := sql_cmd || 	'ordersat) ' ;
	sql_cmd := sql_cmd || 'SELECT ' ;
	sql_cmd := sql_cmd ||	'''' || fname || ''', ' ;
	sql_cmd := sql_cmd ||	'''' || fversion || ''', ' ;
	sql_cmd := sql_cmd ||	'''' || ftype || ''', ' ;
	sql_cmd := sql_cmd ||	'og.orderType, ' ;
	sql_cmd := sql_cmd ||	'og.processorName, ' ;
	sql_cmd := sql_cmd ||	'og.processorVersion, ' ;
	sql_cmd := sql_cmd ||	'og.algorithm, ' ;
	sql_cmd := sql_cmd ||	'og.processorconfiguration, ' ;
	sql_cmd := sql_cmd ||	'og.priority, ' ;
	sql_cmd := sql_cmd ||	'og.troubleshooting, ' ;
	sql_cmd := sql_cmd ||	'og.processingtimewindow, ' ;
	sql_cmd := sql_cmd ||	in_satellite || ' ' ;
	sql_cmd := sql_cmd || 'FROM ' ;
	sql_cmd := sql_cmd ||	't_ordersgeneration og ' ;
	sql_cmd := sql_cmd || 'WHERE ' ;
	sql_cmd := sql_cmd ||	'og.filetype = ' || '''' || ftype || ''' ' ;
	sql_cmd := sql_cmd ||	'AND og.active = BOOL(' || '''' || in_active || ''') ' ;
	sql_cmd := sql_cmd ||	'AND (og.fileclass IS NULL OR og.fileclass = ' || '''' || fclass || ''') ' ;
	sql_cmd := sql_cmd ||	'AND og.processorname IN (SELECT ' || fld_name || ' FROM ' || tab_name || ') ' ;
	
	EXECUTE sql_cmd ;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;




/* ------------------------------
	P_PROCESSINVENTORY
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_processinventory(
	fname VARCHAR(128), 
	fversion VARCHAR(32),
	valflag BOOLEAN,
	ftype VARCHAR(16),
	fclass VARCHAR(8),
	in_orderid INTEGER,
	in_satellite SMALLINT, 
	valstart TIMESTAMP(6),
	valstop TIMESTAMP(6)
) RETURNS INTEGER AS $$

DECLARE
  	err INTEGER ;
	ftype_id INTEGER ;
	fclass_id INTEGER ;
	row RECORD;
	
BEGIN
	---------------------------------------------------------------------------------
	-- The procedure assumes the triplet satellite/sensor/mission is already verified
	---------------------------------------------------------------------------------
	
	-- return without processing if the inventory was not pending
  	IF ((SELECT COUNT(*) FROM t_pendinginventory WHERE filename = fname AND fileversion = fversion) = 0) THEN
  	  	RETURN 0 ;
  	END IF ;
	
  	-- remove pending so nobody will try the same
  	DELETE FROM t_pendinginventory WHERE filename = fname AND fileversion = fversion ;
	
	-----------------------------------------------------------------------------
	-- TEMPORARY ORDER GENERATION SECTION
	-----------------------------------------------------------------------------
	err := p_generateTmpOrder('t', fname, fversion, ftype, fclass, in_satellite) ;
	
	-----------------------------------------------------------------------------
	-- DISTRIBUTION SECTION
	-----------------------------------------------------------------------------
	IF in_orderid IS NULL THEN
		ftype_id := (SELECT id_filetype FROM t_filetypes WHERE filetype = ftype) ;
		fclass_id := (SELECT id_fileclass FROM t_fileclasses WHERE fileclass = fclass) ;
				
		FOR row IN (
			SELECT DISTINCT
				dr.user_id,
				dr.format_tt_id
			FROM 
				t_distributionrules AS dr
				INNER JOIN t_users AS us
					ON dr.user_id = us.id_user AND dr.usermode_id = us.mode
			WHERE (dr.filetype_id = ftype_id OR dr.filetype_id IN (SELECT pfiletype FROM t_filetypescomps WHERE cfiletype = ftype_id)) 
				AND dr.active = 't' 
				AND dr.ondemand = 'f'
				AND (dr.fileclass_id = fclass_id OR dr.fileclass_id IS NULL)) LOOP
				
			PERFORM p_distribute_item(fname, fversion, ftype_id, row.user_id, fclass_id, row.format_tt_id, in_orderid, NULL) ;
				
		END LOOP ;
	END IF ;
	
	-----------------------------------------------------------------------------
	-- ORDER'S BLACK LIST SECTION
	-----------------------------------------------------------------------------
	err :=  p_purgewaitingorders(ftype, valstart, valstop) ;

	-----------------------------------------------------------------------------
	-- INVENTORY LINKS SECTION
	-----------------------------------------------------------------------------
	err := p_inventorylinks_check(fname, fversion, ftype, in_orderid) ;

	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;





/* ------------------------------
	P_INSERT_INVENTORYEVENTS (called by t_inventory)
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_insert_inventoryevents(
	in_invid BIGINT, -- used for update events
	fname VARCHAR(128), -- used for insert or delete events
	fversion VARCHAR(32), -- used for insert or delete events
	ftype VARCHAR(16),
	pre_flag BOOLEAN,
	post_flag BOOLEAN,
	operation CHAR(1)) RETURNS INTEGER AS $$
	
DECLARE 
	p_fname VARCHAR(128);
	p_fversion VARCHAR(32);
	p_event VARCHAR(16);
	sync_cnt INTEGER ;
	async_cnt INTEGER ;

BEGIN
	-- Check the last input value validity
	IF operation NOT IN ('I','U','D') THEN
		RAISE EXCEPTION 'p_insert_inventoryevents (inventory): Invalid operation input value.' ;
		RETURN 1 ;
	END IF ;
	
	-- If there aren't configured filetypes to check... do nothing
	IF (SELECT count(*) FROM t_inventoryeventsconf WHERE filetype = ftype) = 0 THEN
		RETURN 0 ;
	END IF ;
	
	-- If is an insert operation write 'INSERT' and exit
	IF operation = 'I' THEN
		INSERT INTO t_inventoryevents (filename, fileversion, event ) VALUES (fname, fversion, 'INSERT');	
		RETURN 0;
	END IF ;
	
	-- If is a delete operation write 'DELETE' and exit
	IF operation = 'D' THEN
		INSERT INTO t_inventoryevents (filename, fileversion, event ) VALUES (fname, fversion, 'DELETE');	
		RETURN 0;
	END IF ;
	
	-- Get inventoru filename and fileversion
	SELECT filename, fileversion INTO p_fname, p_fversion FROM t_inventory WHERE id_inv = in_invid ;
	
	-- Flags Table:
	--/-----------------------------------------------------------/
	--| pre_flag | post_flag | event                              |
	--|----------|-----------|------------------------------------|
	--|   false  |   true    | NOT AVAILABLE or OFFLINE or ONLINE |
	--|   false  |   false   | return  (do nothing)               |
	--|   true   |   true    | UPDATE                             |
	--|   true   |   false   | INVALIDATED                        |
	--|----------|-----------|------------------------------------|

	IF (pre_flag = 'f' AND post_flag = 't') THEN
		-- Count if there are synchronous storages linked to this inventory
		SELECT count(*) INTO sync_cnt FROM t_inventorysm im INNER JOIN t_storagemanager sm ON im.storagemanid = sm.id
			WHERE im.inv_id = in_invid AND sm.syncdownload = 't' ;
		-- Count if there are asynchronous storages linked to this inventory
		SELECT count(*) INTO async_cnt FROM t_inventorysm im INNER JOIN t_storagemanager sm ON im.storagemanid = sm.id
			WHERE im.inv_id = in_invid AND sm.syncdownload = 'f' ;
		
		IF (async_cnt > 0 AND sync_cnt = 0) THEN
			p_event := 'OFFLINE' ;
		ELSIF sync_cnt > 0 THEN
			p_event := 'ONLINE' ;
		ELSE
			p_event := 'NOT AVAILABLE';
		END IF ;
	END IF ;

	IF (pre_flag = 'f' AND post_flag = 'f') THEN
		RETURN 0 ;
	END IF ;

	IF (pre_flag = 't' AND post_flag = 't') THEN
		p_event := 'UPDATE';
	END IF ;
	
	IF (pre_flag = 't' AND post_flag = 'f') THEN
		p_event := 'INVALIDATED';
	END IF ;
	

	INSERT INTO t_inventoryevents (
		filename,
		fileversion,
		event ) 
	VALUES (
		p_fname, 
		p_fversion,
		p_event );

	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;




/* ------------------------------
	P_INSERT_INVENTORYEVENTS (called by t_inventorysm)
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_insert_inventoryevents(
	in_invid BIGINT,
	in_storageid INTEGER) RETURNS INTEGER AS $$
	
DECLARE 
	p_fname VARCHAR(128);
	p_fversion VARCHAR(32);
	p_valflag BOOLEAN;
	p_event VARCHAR(16);
	sync_cnt INTEGER ;
	async_cnt INTEGER ;

BEGIN
	-- If there aren't configured filetypes to check... return
	IF (SELECT count(*) FROM t_inventoryeventsconf WHERE filetype = (SELECT filetype FROM t_inventory WHERE id_inv = in_invid)) = 0 THEN
		RETURN 0 ;
	END IF ;
	
	-- Get inventoru filename and fileversion
	SELECT filename, fileversion, validityflag INTO p_fname, p_fversion, p_valflag FROM t_inventory WHERE id_inv = in_invid ;
	
	-- If the validity flag is FALSE... return
	IF p_valflag = 'f' THEN
		RETURN 0 ;
	END IF ;
	
	-- Count if there are synchronous storages linked to this inventory
	SELECT count(*) INTO sync_cnt FROM t_inventorysm im INNER JOIN t_storagemanager sm ON im.storagemanid = sm.id
		WHERE im.inv_id = in_invid AND sm.syncdownload = 't' ;
	-- Count if there are asynchronous storages linked to this inventory
	SELECT count(*) INTO async_cnt FROM t_inventorysm im INNER JOIN t_storagemanager sm ON im.storagemanid = sm.id
		WHERE im.inv_id = in_invid AND sm.syncdownload = 'f' ;
	
	IF (async_cnt > 0 AND sync_cnt = 0) THEN
		p_event := 'OFFLINE' ;
	ELSIF sync_cnt > 0 THEN
		p_event := 'ONLINE' ;
	ELSE
		p_event := 'NOT AVAILABLE';
	END IF ;
	
	INSERT INTO t_inventoryevents (
		filename,
		fileversion,
		event ) 
	VALUES (
		p_fname, 
		p_fversion,
		p_event );

	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;





/* ------------------------------
	P_ORDERQUEUECHECKCHANGES
*/ ------------------------------
CREATE FUNCTION p_orderqueuecheckchanges(
		pre_orderid INT, 
		pre_status VARCHAR(32), 
		post_status VARCHAR(32), 
		pre_processorname VARCHAR(32),
		post_processorname VARCHAR(32),
		pre_processorversion VARCHAR(8),
		post_processorversion VARCHAR(8),
		pre_processorconfiguration VARCHAR(8),
		post_processorconfiguration VARCHAR(8),
		pre_starttime TIMESTAMP(6),
		post_starttime TIMESTAMP(6),
		pre_stoptime TIMESTAMP(6),
		post_stoptime TIMESTAMP(6),
		pre_ordertype VARCHAR(8),
		post_ordertype VARCHAR(8),
		pre_satelliteid SMALLINT,
		post_satelliteid SMALLINT,
		pre_mission SMALLINT,
		post_mission SMALLINT) RETURNS INTEGER AS $$
	
BEGIN

	IF (pre_status != post_status) THEN
		RETURN 0;
	END IF;
	
	IF (pre_processorname != post_processorname OR
	    pre_processorversion != post_processorversion OR
	    pre_processorconfiguration != post_processorconfiguration OR
	    pre_starttime != post_starttime OR
	    pre_stoptime != post_stoptime OR
	    pre_ordertype != post_ordertype OR
	    pre_satelliteid != post_satelliteid OR
	    pre_mission != post_mission) 
	THEN
		DELETE FROM t_waitingorders WHERE orderid = pre_orderid;
	END IF;
	
	RETURN 0 ;
		
END ;
$$ LANGUAGE plpgsql ;





/* ------------------------------
	P_COMPLETED_ORDER
*/ ------------------------------
CREATE FUNCTION p_completed_order(in_orderid INTEGER) RETURNS INTEGER AS $$
BEGIN
	INSERT INTO t_completed_orders VALUES (in_orderid);
	RETURN 0;
END ;
$$ LANGUAGE plpgsql ;





/* ------------------------------
	P_GENERATETMPORDERPNAME
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_generatetmporderpname(
	in_active BOOLEAN,
	fname VARCHAR(128),
	fversion VARCHAR(32),
	ftype VARCHAR(16),
	fclass VARCHAR(8),
	in_satellite SMALLINT) RETURNS INTEGER AS $$
	
BEGIN
	
	INSERT INTO t_temporaryorders (
		filename, 
		fileversion, 
		filetype, 
		ordertype, 
		processorname,
		processorversion, 
		algorithm, 
		processorconfiguration, 
		priority,
		troubleshooting, 
		processingtimewindow, 
		ordersat)
  	SELECT
		fname, 
		fversion, 
		ftype, 
		og.orderType, 
		og.processorName,
		og.processorVersion, 
		og.algorithm, 
		og.processorconfiguration, 
		og.priority,
		og.troubleshooting, 
		og.processingtimewindow, 
		in_satellite
	FROM 
		t_ordersgeneration og
	WHERE 
		og.filetype = ftype 
		AND og.active = in_active 
		AND (og.fileclass IS NULL OR og.fileclass = fclass)
		AND og.processorname IN (SELECT pname FROM tempPnames)
	;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;





/* ------------------------------
	p_remove_obsolete_actions
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_remove_obsolete_actions(
	fname VARCHAR(128), 
	fversion VARCHAR(32)) RETURNS INTEGER AS $$

DECLARE
	row RECORD;
	
BEGIN
	FOR row IN (
		SELECT 
			sma.packageid AS pkgid,
			smq.status AS stat, 
			smq.operation AS oper
		FROM 
			t_smactions sma
			INNER JOIN t_smqueue smq
				ON smq.packageid = sma.packageid
		WHERE 
			sma.filename = fname 
			AND sma.fileversion = fversion 
			AND sma.packageid IS NOT NULL 
			AND sma.operation = 'UPLOAD') LOOP
	
		IF (row.stat = 'ELIGIBLE' AND row.oper = 'UPLOAD') OR (row.stat = 'ERROR' AND row.oper = 'UPLOAD') THEN
			UPDATE t_smactions SET packageid = NULL WHERE packageid = row.pkgid ;
			DELETE FROM t_smqueue WHERE packageid = row.pkgid ;
		END IF ;
		
	END LOOP ;
	
	DELETE FROM t_smactions WHERE filename = fname AND fileversion = fversion AND operation = 'UPLOAD' ;
	
	DELETE FROM t_smqueue WHERE packageid IN (
		SELECT sq.packageid FROM t_smqueue sq
		LEFT JOIN t_smactions sa ON sq.packageid = sa.packageid
		WHERE sq.status = 'COMPLETED' AND sa.packageid IS NULL) ;

	RETURN 0 ;
END ;
$$ LANGUAGE plpgsql ;





/* ------------------------------
	P_SMACTIONSUPDATE
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_smactionsupdate(
	fname VARCHAR(128), 
	fversion VARCHAR(32), 
	pkid INT) RETURNS INTEGER AS $$
	
BEGIN

	IF (SELECT status FROM t_smqueue WHERE packageid = pkid) = 'COMPLETED' THEN
		
		PERFORM
			p_purgewaitingorders(i.filetype, i.validitystart, i.validitystop)
		FROM 
			t_inventory i
		WHERE 
			filename = fname 
			AND fileversion = fversion ;
		
	END IF;

	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;





/* ------------------------------
	P_INSERT_TEMPORARY_DISTRIBUTION
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_insert_temporary_distribution(
	in_duedate TIMESTAMP(6),
	in_fname VARCHAR(128),
	in_fversion VARCHAR(32),
	in_mtype VARCHAR(16),
	in_format INT,
	in_algo INT,
	in_ordid INT,
	in_cartid INT,
	in_subreq INT,
	in_disspackalgo INTEGER)RETURNS INTEGER AS $$

BEGIN

	INSERT INTO t_temporarydistribution (
		duedate,
		filename,
		fileversion,
		mediatype,
		userid,
		format,
		algo,
		policy,
		orderid,
		cartid,
		subreq,
		disseminationpackingalgo_id )
	VALUES (
		in_duedate,
		in_fname,
		in_fversion,
		in_mtype,
		'ANONYMOUS',
		in_format,
		in_algo,
		(SELECT max(id_distributionrule) FROM t_distributionrules WHERE user_id = (
			SELECT id_user FROM t_users WHERE userid = 'ANONYMOUS')),
		in_ordid,
		in_cartid,
		in_subreq,
		in_disspackalgo
	) ;

	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;





/* ------------------------------
	P_CARTS_UPDATE_STATUS
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_carts_update_status(in_cartid INT) RETURNS INTEGER AS $$

DECLARE
	p_extorderid VARCHAR(32);
	ord_total INT;
	dis_total INT;
	mid_status VARCHAR(32) ;

BEGIN
	
	p_extorderid := (SELECT extref FROM t_cartsref WHERE id = in_cartid ) ;
	
	ord_total := (SELECT count(*) FROM t_ordersqueue WHERE externalorderid = p_extorderid AND subreq IS NOT NULL);
	dis_total := (SELECT count(*) FROM t_distributionitems WHERE cartid = in_cartid);


	-- ORDERS ERROR
	IF (SELECT count(status) FROM t_ordersqueue WHERE externalorderid = p_extorderid AND subreq IS NOT NULL AND status = 'ERROR') > 0 
	THEN
		UPDATE t_cartsref SET status = 'ERROR' WHERE id = in_cartid ;
		RETURN 0;
	END IF;
	
	-- ORDERS ABORTED
	IF (SELECT count(status) FROM t_ordersqueue WHERE externalorderid = p_extorderid AND subreq IS NOT NULL AND status = 'ABORTED') > 0 
	THEN
		UPDATE t_cartsref SET status = 'ABORTED' WHERE id = in_cartid ;
		RETURN 0;
	END IF;
	
	IF (ord_total = 0) THEN
	-- No orders, so act as every order has completed
		mid_status := 'COMPLETED' ;

	-- ORDERS ELIGIBLE
	ELSIF (SELECT count(status) FROM t_ordersqueue WHERE externalorderid = p_extorderid AND subreq IS NOT NULL AND status = 'ELIGIBLE') < ord_total 
	THEN
		-- ORDERS COMPLETED
		IF (SELECT count(status) FROM t_ordersqueue WHERE externalorderid = p_extorderid AND status = 'COMPLETED') < ord_total 
		THEN
			UPDATE t_cartsref SET status = 'PROGRESS' WHERE id = in_cartid ;
			RETURN 0;
		ELSE	
			mid_status := 'COMPLETED' ;
		END IF ;
	ELSE 
		mid_status := 'ELIGIBLE' ;
	END IF ;



	-- DISTRIBUTION ERROR
	IF (SELECT count(di.id) FROM t_distributionitems AS di
		INNER JOIN t_distributionqueue AS dq ON di.packageid = dq.packageid
		WHERE di.cartid = in_cartid AND dq.status = 'ERROR') > 0 
	THEN
		UPDATE t_cartsref SET status = 'ERROR' WHERE id = in_cartid ;
		RETURN 0;
	END IF;

	-- DISTRIBUTION ELIGIBLE
	IF (SELECT count(di.id) FROM t_distributionitems AS di
		INNER JOIN t_distributionqueue AS dq ON di.packageid = dq.packageid
		WHERE di.cartid = in_cartid AND dq.status = 'ELIGIBLE') < dis_total 
	THEN
		-- DISTRIBUTION COMPLETED
		IF (SELECT count(di.id) FROM t_distributionitems AS di
			INNER JOIN t_distributionqueue AS dq ON di.packageid = dq.packageid
			WHERE di.cartid = in_cartid AND dq.status = 'COMPLETED') < dis_total
		THEN
			UPDATE t_cartsref SET status = 'PROGRESS' WHERE id = in_cartid ;
			RETURN 0;
		ELSE
			UPDATE t_cartsref SET status = mid_status WHERE id = in_cartid ;
			RETURN 0;
		END IF;
	ELSE 
		UPDATE t_cartsref SET status = 'ELIGIBLE' WHERE id = in_cartid ;
		RETURN 0;
	END IF ;

	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;





/* ------------------------------
	P_INVENTORYLINKS_CHECK
*/ ------------------------------
CREATE FUNCTION p_inventorylinks_check (
	fname VARCHAR(128), 
	fversion VARCHAR(32),
	ftype VARCHAR(16),
	ordid INTEGER
) RETURNS INTEGER AS $$

BEGIN
	
	--  NOT IMPLEMENTED IN THIS SCHEMA
	--  To implement this procedure you have to install USDFWA 2.0 schema.
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;





/* ------------------------------
	P_CARTSINV_UPDATE
*/ ------------------------------
CREATE FUNCTION p_cartsinv_update(
	in_packageid INTEGER, 
	in_status VARCHAR(32)
) RETURNS INTEGER AS $$

DECLARE
	p_cartid INTEGER;
	ret INTEGER;

BEGIN
	ret := 0 ;
	p_cartid := (SELECT DISTINCT cartid FROM t_distributionitems WHERE packageid = in_packageid);
	
	IF p_cartid IS NOT NULL THEN		
		UPDATE t_cartsinv SET status = in_status WHERE cart = p_cartid AND distsubreq IN (SELECT subreq FROM t_distributionitems WHERE packageid = in_packageid);
		ret := p_carts_update_status(p_cartid);
	END IF ;
	
	RETURN ret;
	
END ;
$$ LANGUAGE plpgsql ;





/* ------------------------------
	P_CARTSINVCHECK
*/ ------------------------------
CREATE FUNCTION p_cartsinvcheck(
	pre_id INT, 
	pre_cart INT, 
	pre_subreq INT, 
	pre_distsubreq INT, 
	pre_procsubreq INT, 
	pre_stndsubreq INT
) RETURNS INTEGER AS $$

BEGIN

	IF (pre_distsubreq != 0) THEN 
		IF ( ( SELECT count(*) FROM t_cartsinv WHERE id != pre_id  AND cart = pre_cart AND pre_distsubreq = distsubreq ) > 0 ) THEN 
			RAISE EXCEPTION 'distribution subrequest already present for cart in t_cartsinv ' ;
			RETURN 1;
		END IF ;
	
	END IF ;

	IF (pre_procsubreq != 0) THEN 
		IF ( ( SELECT count(*) FROM t_cartsinv WHERE id != pre_id  AND cart = pre_cart AND pre_procsubreq = ordsubreq ) > 0 ) THEN 
			RAISE EXCEPTION 'processing subrequest already present for cart in t_cartsinv' ;
			RETURN 1;
		END IF ;
	
	END IF ;

	IF (pre_stndsubreq != 0) THEN 
		IF ( ( SELECT count(*) FROM t_cartsinv WHERE id != pre_id  AND cart = pre_cart AND pre_stndsubreq = stndsubreq ) > 0 ) THEN 
			RAISE EXCEPTION 'standing subrequest already present for cart in t_cartsinv ' ;
			RETURN 1;
		END IF ;
	
	END IF ;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;





/* ------------------------------
	P_POSTINQUEUE_INSERT
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_postinvqueue_insert(
	in_fname VARCHAR(128), 
	in_fversion VARCHAR(32)) RETURNS INTEGER AS $$
	
BEGIN
	
	INSERT INTO t_postinvqueue (
		filename,
		fileversion,
		filetype,
		validitystart,
		validitystop)
	SELECT 
		i.filename,
		i.fileversion,
		i.filetype,
		i.validitystart,
		i.validitystop
	FROM 
		t_inventory i
	WHERE 
		filename = in_fname 
		AND fileversion = in_fversion ;
	
	RETURN 0;
	
END ;
$$ LANGUAGE plpgsql ;





/* ------------------------------
	P_POSTINVQUEUE_MANAGER
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_postinvqueue_manager(
	in_secs INTEGER) RETURNS INTEGER AS $$

DECLARE
	p_secs INTERVAL;
	p_date TIMESTAMP(6);

BEGIN
	p_secs := (SELECT in_secs * '1 second'::interval);
	p_date := (SELECT now() - p_secs) ;
	
	PERFORM p_purgewaitingorders(
		pq.filetype,
		pq.validitystart,
		pq.validitystop)
	FROM 
		t_postinvqueue pq
	WHERE 
		pq.tstamp < p_date ;
	
	DELETE FROM t_postinvqueue WHERE tstamp < p_date;
		
	RETURN 0;
	
END ;
$$ LANGUAGE plpgsql ;





/* ------------------------------
	P_PURGEEXPIREDWAITINGORDERS
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_purgeexpiredwaitingorders() RETURNS INTEGER AS $$

BEGIN
        DELETE FROM t_waitingorders WHERE expirationdate < NOW() AND expirationdate IS NOT NULL;

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





/* ------------------------------
	p_orders_manager
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_orders_manager(
	in_orderid INTEGER, 
	pre_status VARCHAR(32), 
	post_status VARCHAR(32)) RETURNS INTEGER AS $$
	
DECLARE
  	p_extordid VARCHAR(128);
	p_subreq INT;
	p_cartid INT;
	cartsinv_status VARCHAR(32);
	
BEGIN
	p_extordid := (SELECT externalorderid FROM t_ordersqueue WHERE orderid = in_orderid);
	p_subreq := (SELECT subreq FROM t_ordersqueue WHERE orderid = in_orderid);
	
	IF p_extordid IS NOT NULL AND p_subreq IS NOT NULL THEN
		p_cartid := (SELECT id FROM t_cartsref AS cr INNER JOIN t_ordersqueue AS oq ON oq.externalorderid = cr.extref WHERE oq.orderid = in_orderid);
		
		IF post_status = 'COMPLETED' THEN
			cartsinv_status := 'PROCESSED';
		ELSE
			cartsinv_status := post_status ;
		END IF ;
		
		UPDATE t_cartsinv SET status = cartsinv_status WHERE cart = p_cartid AND ordsubreq = p_subreq ;	
		PERFORM p_carts_update_status(p_cartid);
	END IF ;
	
	RETURN 0 ;

END ;
$$ LANGUAGE plpgsql ;





/* ------------------------------
	p_inv_dist_counter_by_items 
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_inv_dist_counter_by_items(
	in_fname VARCHAR(128),
	in_fversion VARCHAR(32),
	operation CHAR(3)) RETURNS INTEGER AS $$
	
DECLARE
	p_invid BIGINT ;
	
BEGIN
	-- Check operation input value validity
	IF operation NOT IN ('INS','DEL') THEN
		RAISE EXCEPTION 'p_inv_distributions_by_items: Invalid operation input value (INS/DEL).' ;
		RETURN 1 ;
	END IF ;
	
	-- Get the inventory ID
	SELECT id_inv INTO p_invid FROM t_inventory WHERE filename = in_fname AND fileversion = in_fversion ;
	
	IF operation = 'DEL' THEN
		UPDATE t_inv_distributions SET retain_counter = (retain_counter - 1) WHERE inv_id = p_invid AND retain_counter > 0 ;
	END IF ;
	
	IF operation = 'INS' THEN
		IF (SELECT count(*) FROM t_inv_distributions WHERE inv_id = p_invid) > 0 THEN
			UPDATE t_inv_distributions SET retain_counter = (retain_counter + 1) WHERE inv_id = p_invid ;
		ELSE
			INSERT INTO t_inv_distributions (inv_id, retain_counter) VALUES (p_invid, 1) ;
		END IF ;
	END IF ;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;




/* ------------------------------
	p_inv_dist_counter_by_queue 
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_inv_dist_counter_by_queue(
	in_packid INTEGER,
	in_pre_status VARCHAR(32),
	in_post_status VARCHAR(32),
	operation CHAR(3)) RETURNS INTEGER AS $$
	
BEGIN
	-- Check operation input value validity
	IF operation NOT IN ('UPD','DEL') THEN
		RAISE EXCEPTION 'p_inv_distributions_by_queue: Invalid operation input value (UPD/DEL).' ;
		RETURN 1 ;
	END IF ;
	
	IF operation = 'DEL' OR (operation = 'UPD' AND (in_pre_status != in_post_status AND in_post_status = 'COMPLETED')) THEN
		UPDATE 
			t_inv_distributions 
		SET 
			retain_counter = (retain_counter - 1) 
		WHERE 
			retain_counter > 0
			AND inv_id IN (
		SELECT
			iv.id_inv
		FROM
			t_inventory iv
			INNER JOIN t_distributionitems di
				ON iv.filename = di.filename AND iv.fileversion = di.fileversion
			INNER JOIN t_distributionqueue dq
				ON dq.packageid = di.packageid
		WHERE
			dq.packageid = in_packid );
	END IF ;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;




/* ------------------------------
	p_insert_test_inventories
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_insert_test_inventories(
	in_fname VARCHAR(128), -- name of file that will be created (the version will be invcreased automatically)
	in_ftype VARCHAR(16), -- name of filetype (must be exist). If you set NULL will be used random filetypes
	in_files_number INTEGER -- number of files you want to create (cannot be zero)
) RETURNS INTEGER AS $$

DECLARE 
	p_ftype VARCHAR(16);

BEGIN	
	--INPUT CHECK
	-- Check the name value
	IF in_fname IS NULL THEN
		RAISE EXCEPTION 'p_insert_test_inventories: in_fname input value cannot be NULL.' ;
		RETURN 1 ;
	END IF ;
	-- Check the files number value
	IF in_files_number <= 0 THEN
		RAISE EXCEPTION 'p_insert_test_inventories: in_files_number input value must be greater than 0.' ;
		RETURN 1 ;
	END IF ;
	
	-- INSERT OPERATIONS
	FOR counter IN 1..in_files_number LOOP
		-- Set filetype
		IF in_ftype IS NOT NULL THEN
			p_ftype := in_ftype ;
		ELSE
			-- If is not setted will be selected a random value
			SELECT filetype INTO p_ftype FROM t_filetypes ORDER BY random() LIMIT 1 ;
		END IF;
		
		-- Insert into t_inventory
		INSERT INTO t_inventory(
			filename, 
			fileversion, 
			generationtime, 
			validitystart, 
			validitystop, 
			validityflag, 
			validationdate, 
			filetype, 
			fileclass)
		VALUES (
			in_fname,
			(SELECT -- select the version of file and increase it
				CASE 
					WHEN max(fileversion) IS NULL THEN '0001'
					ELSE trim(to_char(to_number(max(fileversion), '9999') + 1, '0000')) 
				END 
			FROM t_inventory WHERE filename = in_fname),
			now(),
			now(),
			now() + interval '1' day,
			'f',
			now(),
			p_ftype,
			'TEST' );
	END LOOP ;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;




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





/* ------------------------------
	p_statetransitions_check
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_statetransitions_check(
	in_tname VARCHAR(30),
	in_flw INT,
	in_pre_st INT, 
	in_pst_st INT) RETURNS INTEGER AS $$

DECLARE
	tbl_id INTEGER;
	pre_st_name VARCHAR(32);
	pst_st_name VARCHAR(32);
	no_table_ex_msg TEXT;
	no_trans_ex_msg TEXT;

BEGIN
	-- Retrieve the id of the table using the table name and the flow
	SELECT id INTO tbl_id FROM t_statetransitions_tables WHERE table_name = in_tname AND flow_id = in_flw ;
	
	-- Retrieve status name by id (just for error message)
	SELECT status INTO pre_st_name FROM t_stati WHERE id = in_pre_st ;
	SELECT status INTO pst_st_name FROM t_stati WHERE id = in_pst_st ;
	
	
	-- Set no table error messages
	IF in_flw = 1 THEN
		no_table_ex_msg := 'p_StateTransitions_Check: table ' || in_tname || ' not regestired into t_statetransitions_tables table.' ; 
	ELSE
		no_table_ex_msg := 'p_StateTransitions_Check: table ' || in_tname || ' in flow ' || in_flw || ' not regestired into t_statetransitions_tables table.' ; 
	END IF ;
	
	-- Set no transition error messages
	IF in_flw = 1 THEN
		no_trans_ex_msg := 'p_StateTransitions_Check: table ' || in_tname || ' - ' || pre_st_name || ' to ' || pst_st_name || ' invalid state transition.' ; 
	ELSE
		no_trans_ex_msg := 'p_StateTransitions_Check: table ' || in_tname || ' in flow ' || in_flw || ' - ' || pre_st_name || ' to ' || pst_st_name || ' invalid state transition.' ; 
	END IF ;
	
	
	-- Check for registered table
	IF tbl_id IS NULL THEN
		RAISE EXCEPTION '%', no_table_ex_msg ;
	END IF ;
	
	-- Check for allowed transition
	IF (SELECT count(*) FROM t_statetransitions_allowed WHERE table_id = tbl_id AND prev_state_id = in_pre_st AND post_state_id = in_pst_st) = 0 THEN
		RAISE EXCEPTION '%', no_trans_ex_msg ;
	END IF ;	
	
	
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
	p_move_seq_to_max
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_move_seq_to_max(
	tb_name VARCHAR(64)) RETURNS INTEGER AS $$

DECLARE
	fld_name VARCHAR(64) ;
	row_cnt INTEGER ;
	
BEGIN
	-- Get, if exists, the name of serial column
	SELECT column_name INTO fld_name FROM information_schema.columns
		WHERE table_name = tb_name AND column_default SIMILAR TO '%(nexval|_seq|regclass)%';

	-- Check if the table contains some rows 
	EXECUTE 'SELECT count(*) FROM ' || tb_name INTO row_cnt ;

	-- If the table has a serial column and has some records, ...
	IF (fld_name IS NOT NULL) AND (row_cnt > 0) THEN
		-- update the start value of the sequence to the max value contained into the table 
		EXECUTE 'SELECT pg_catalog.setval(' || '''' || tb_name || '_' || fld_name || '_seq' || '''' || ', (SELECT MAX(' || fld_name || ') FROM ' || tb_name || '), true)' ;
	END IF ;
	
	RETURN 0;
	
END ;
$$ LANGUAGE plpgsql ;




/* ------------------------------
	p_insert_inv_data_restored
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_insert_inv_data_restored(
	ismId INTEGER,
	invId BIGINT,
	smId INTEGER) RETURNS INTEGER AS $$

DECLARE
	isSync BOOLEAN ;
	asynchCount INTEGER ;
	parentsAsynchCount INTEGER ;
	insertedRows INTEGER := 0 ;
	
BEGIN
	-- Get the syncupload type of the SM for the new record to
	-- check if the new record has been inserted into a SYNCH repository
	SELECT s.syncupload INTO isSync 
	FROM t_storagemanager s INNER JOIN t_inventorysm i ON i.storagemanid = s.id
	WHERE i.id = ismId ;
	
	IF isSync IS TRUE THEN
		
		-- Check if there are other records having the same inv_id into asynch repositories
		SELECT count(*) INTO asynchCount FROM t_inventorysm i INNER JOIN t_storagemanager s ON i.storagemanid = s.id 
		WHERE i.inv_id = invId AND s.syncupload IS FALSE ;
		
		-- Check if there are other "parents" records having the same inv_id into asynch repositories
		SELECT count(*) INTO parentsAsynchCount FROM t_inventorysm i INNER JOIN t_storagemanager s ON i.storagemanid = s.id 
		WHERE i.inv_id = (SELECT inv_id2 FROM t_inv_relations WHERE inv_id1 = invId) AND s.syncupload IS FALSE ;
		
		-- If there is another record (or his parent) into an asynch repository insert a record into t_inv_data_restored
		IF (asynchCount > 0 OR parentsAsynchCount > 0) THEN
			INSERT INTO t_inv_data_restored (
				id,
				sm_id,
				inv_id,
				parent_inv_id,
				filetype_id,
				size)
			SELECT 
				ismId,
				smId,
				invId,
				r.inv_id2,
				f.id_filetype,
				CASE WHEN i.datasize IS NULL THEN 0 ELSE i.datasize END
			FROM
				t_inventory i
				LEFT JOIN t_inv_relations r ON i.id_inv = r.inv_id1
				INNER JOIN t_filetypes f ON i.filetype = f.filetype
			WHERE
				i.id_inv = invId ;
			
			GET DIAGNOSTICS insertedRows = ROW_COUNT ;
			
		END IF ;
	END IF ;
	
	RETURN insertedRows ;
	
END ;
$$ LANGUAGE plpgsql ;




/* ------------------------------
	p_PurgeDistributionWaitingItems
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_PurgeDistributionWaitingItems(
	id_to_remove BIGINT,
	field_to_considerate VARCHAR(32)) RETURNS INTEGER AS $$

DECLARE
	sqlCmd VARCHAR(256);
	
BEGIN
	sqlCmd = 'DELETE FROM t_distributionwaitingitems WHERE ' || field_to_considerate || ' = ' || id_to_remove ;
	RAISE NOTICE '%', sqlCmd;
	EXECUTE sqlCmd ;
	
	RETURN 0;
	
END ;
$$ LANGUAGE plpgsql ;





