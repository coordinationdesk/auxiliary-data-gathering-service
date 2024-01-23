
-- Version 11.4.0
-- POSTGRES








---------------------------------------------------------------------------------
------------------------------ PROCEDURES CREATION ------------------------------
---------------------------------------------------------------------------------
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








---------------------------------------------------------------------------------
---------------------------- PROCEDURES MODIFICATION ----------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	P_INSERT_INVENTORYEVENTS (called by t_inventory)
*/ ------------------------------
DROP FUNCTION p_insert_inventoryevents(VARCHAR,VARCHAR,BOOLEAN,BOOLEAN) ;
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








---------------------------------------------------------------------------------
------------------------ TRIGGERS FUNCTIONS MODIFICATION ------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	tf_inventoryinsevent
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_inventoryinsevent() RETURNS TRIGGER AS $$

DECLARE
  	status INTEGER ;
	
BEGIN
	status := p_inventory_manager(NEW.filename, NEW.fileversion, NULL, NEW.validityflag, NEW.filetype, NEW.fileclass, NEW.orderid, NEW.satelliteid, NEW.sensorid, NEW.mission, NEW.validitystart, NEW.validitystop) ;
	status := p_insert_inventoryevents(NEW.id_inv, NEW.filename, NEW.fileversion, NEW.filetype, NULL, NEW.validityflag, 'I') ;
	
	IF NEW.validityflag = 't' THEN
		status := p_filetypes_last_inv_update(NEW.filetype) ;
	END IF;			
  	
  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;

/* ------------------------------
	tf_inventoryupdevent
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_inventoryupdevent() RETURNS TRIGGER AS $$

DECLARE
  	status INTEGER ;

BEGIN
	status := p_inventory_manager(NEW.filename, NEW.fileversion, OLD.validityflag, NEW.validityflag, NEW.filetype, NEW.fileclass, NEW.orderid, NEW.satelliteid, NEW.sensorid, NEW.mission, NEW.validitystart, NEW.validitystop) ;
	
	status := p_insert_inventoryevents(NEW.id_inv, NULL, NULL, NEW.filetype, OLD.validityflag, NEW.validityflag, 'U') ;
	
	IF ((NEW.validityflag != OLD.validityflag) AND (NEW.validityflag = 't')) THEN
		status := p_filetypes_last_inv_update(OLD.filetype) ;
	END IF;	
	
  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
-------------------------- TRIGGERS FUNCTIONS CREATION --------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	tf_inventorydelete
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_inventorydelete() RETURNS TRIGGER AS $$

DECLARE
  	status INTEGER ;

BEGIN
	status := p_insert_inventoryevents(NULL, OLD.filename, OLD.fileversion, OLD.filetype, 'f', 'f', 'D') ;
	
  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;


/* ------------------------------
	tf_inventorysmmanager
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_inventorysmmanager() RETURNS TRIGGER AS $$

DECLARE
  	status INTEGER ;

BEGIN	
	status := p_events('t_inventorysm', TG_ARGV[0]) ;
	
	IF (TG_ARGV[0] = 'DEL') THEN 
		status := p_insert_inventoryevents(OLD.inv_id, OLD.storagemanid) ;
		RETURN OLD ;
	ELSE
		status := p_insert_inventoryevents(NEW.inv_id, NEW.storagemanid) ;
		RETURN NEW ;
	END IF ;
END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
------------------------------- TRIGGERS CREATION -------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_inventory
*/ ------------------------------
CREATE TRIGGER tr_inventorydelete AFTER DELETE
	ON t_inventory
		FOR EACH ROW EXECUTE PROCEDURE tf_inventorydelete() ;








---------------------------------------------------------------------------------
----------------------------- TRIGGERS MODIFICATIONS ----------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_inventorysm
*/ ------------------------------
DROP TRIGGER tr_inventorysminsert ON t_inventorysm ;
DROP TRIGGER tr_inventorysmupdate ON t_inventorysm ;

CREATE TRIGGER tr_inventorysminsert AFTER INSERT
	ON t_inventorysm 
		FOR EACH ROW EXECUTE PROCEDURE tf_inventorysmmanager('INS') ;

CREATE TRIGGER tr_inventorysmupdate AFTER UPDATE
	ON t_inventorysm 
		FOR EACH ROW EXECUTE PROCEDURE tf_inventorysmmanager('UPD') ;

CREATE TRIGGER tr_inventorysmdelete AFTER DELETE
	ON t_inventorysm 
		FOR EACH ROW EXECUTE PROCEDURE tf_inventorysmmanager('DEL') ;








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
		'pds-11.4.0',
		'2013-09-09',
		3,
		2,
		'PDS_11-4-0',
		'Moved (and modified) p_insert_inventoryevents procedure from USDFWA to PDS. Added new p_insert_inventoryevents for inventorysm trigger. Added new triggers and triggers function on inventory delete.'
) ;


