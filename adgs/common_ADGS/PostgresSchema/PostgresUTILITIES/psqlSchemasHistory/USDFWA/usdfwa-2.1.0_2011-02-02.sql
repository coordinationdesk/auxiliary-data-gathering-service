
-- Version usdfwa-2.0
-- POSTGRES

---------------------------------------------------------------
-- ATTENZIONE!!!                                             --
-- Questa modifica è applicabile senza ulteriori interventi  --
-- sul database solo per le versioni di schema del PDS 10.0  --
-- o superiori.                                              --
---------------------------------------------------------------








---------------------------------------------------------------------------------
------------------------------ TABLES MODIFICATION ------------------------------
---------------------------------------------------------------------------------

------------------------------
-- t_usdfwa
------------------------------
ALTER TABLE t_usdfwa ALTER COLUMN fileversion TYPE VARCHAR(32);

------------------------------
-- t_inventorylinks
------------------------------
ALTER TABLE t_inventorylinks ALTER COLUMN orig_fileversion TYPE VARCHAR(32);
ALTER TABLE t_inventorylinks ALTER COLUMN dest_fileversion TYPE VARCHAR(32);








---------------------------------------------------------------------------------
---------------------------- PROCEDURES MODIFICATION ----------------------------
---------------------------------------------------------------------------------

CREATE OR REPLACE FUNCTION p_insert_inventoryevents(
	p_name VARCHAR(128), 
	p_version VARCHAR(32),
	pre_flag BOOLEAN,
	post_flag BOOLEAN) RETURNS INTEGER AS $$
	
DECLARE 
	p_event VARCHAR(16);

BEGIN
	
	IF (SELECT count(*) FROM t_inventoryeventsconf WHERE filetype = (SELECT filetype FROM t_inventory WHERE filename = p_name AND fileversion = p_version)) = 0 THEN
		RETURN 0 ;
	END IF ;
	
	-- Flags Table:
	-----------------------------------
	--| pre_flag | post_flag | event  |
	--|----------|-----------|--------|
	--|   false  |   true    | INSERT |
	--|   false  |   false   | return |
	--|   true   |   true    | UPDATE |
	--|   true   |   false   | DELETE |
	--|----------|-----------|--------|

	IF (pre_flag = 'f' AND post_flag = 't') THEN
		p_event := 'INSERT';
	END IF ;

	IF (pre_flag = 'f' AND post_flag = 'f') THEN
		RETURN 0 ;
	END IF ;

	IF (pre_flag = 't' AND post_flag = 't') THEN
		p_event := 'UPDATE';
	END IF ;
	
	IF (pre_flag = 't' AND post_flag = 'f') THEN
		p_event := 'DELETE';
	END IF ;
	

	INSERT INTO t_inventoryevents (
		filename,
		fileversion,
		event ) 
	VALUES (
		p_name, 
		p_version,
		p_event
	);

	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;





------------------------------------------------
--- P_INVENTORYLINKS_INSERT (for order algo) ---
------------------------------------------------
CREATE OR REPLACE FUNCTION p_inventorylinks_insert (
	fname VARCHAR(128), 
	fversion VARCHAR(32),
	origftype VARCHAR(16),
	ordid INTEGER,
	algo VARCHAR(24)
) RETURNS INTEGER AS $$
	
DECLARE 
	p_destfname VARCHAR(128);
	p_destfvers VARCHAR(8);
	row RECORD ;
	
BEGIN

	FOR row IN (
		SELECT o.filename, o.fileversion
		FROM t_ordersinput AS o
		INNER JOIN t_inventory AS i ON i.orderid = o.orderid
		WHERE o.orderid = ordid AND i.filetype = origftype ) LOOP
		
			p_destfname := row.filename;
			p_destfvers := row.fileversion;

			INSERT INTO t_inventorylinks (orig_filename, orig_fileversion, dest_filename, dest_fileversion, description)
			VALUES ( fname, fversion, p_destfname, p_destfvers, 'linked by ' || algo || 'algorithm');
	END LOOP ;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;





------------------------------------------------
--- P_INVENTORYLINKS_INSERT (for temp algos) ---
------------------------------------------------
CREATE OR REPLACE FUNCTION p_inventorylinks_insert (
	fname VARCHAR(128), 
	fversion VARCHAR(32),
	destftype VARCHAR(16),
	algo VARCHAR(24)
) RETURNS INTEGER AS $$

DECLARE
	p_destfname VARCHAR(128);
	p_destfvers VARCHAR(8);
	p_start TIMESTAMP(6);
	p_stop TIMESTAMP(6);
	row RECORD ;

BEGIN

	p_start := (SELECT validitystart FROM t_inventory WHERE filename = fname AND fileversion = fversion) ;
	p_stop := (SELECT validitystop FROM t_inventory WHERE filename = fname AND fileversion = fversion) ;
	
	IF algo = 'LINKTEMPORIG' THEN
		FOR row IN (
			SELECT filename, fileversion
			FROM t_inventory
			WHERE validitystart >= p_start AND validitystop <= p_stop AND validityflag = 't' AND filetype = destftype
			GROUP BY filename, fileversion ) LOOP

				p_destfname := row.filename ;
				p_destfvers := row.fileversion ;
					
				INSERT INTO t_inventorylinks (orig_filename, orig_fileversion, dest_filename, dest_fileversion, description)
				VALUES ( fname, fversion, p_destfname, p_destfvers, 'linked by ' || algo || 'algorithm');

		END LOOP ;
	END IF ;
	
	IF algo = 'LINKTEMPDEST' THEN
		FOR row IN (
			SELECT filename, fileversion
			FROM t_inventory
			WHERE validitystart <= p_start AND validitystop >= p_stop AND validityflag = 't' AND filetype = destftype
			GROUP BY filename, fileversion ) LOOP
				
				p_destfname := row.filename ;
				p_destfvers := row.fileversion ;
			
				INSERT INTO t_inventorylinks (orig_filename, orig_fileversion, dest_filename, dest_fileversion, description)
				VALUES ( p_destfname, p_destfvers, fname, fversion, 'linked by ' || algo || 'algorithm');
				
		END LOOP ;
	END IF ;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;





------------------------------
--- P_INVENTORYLINKS_CHECK ---
------------------------------
CREATE OR REPLACE FUNCTION p_inventorylinks_check (
	fname VARCHAR(128), 
	fversion VARCHAR(32),
	ftype VARCHAR(16),
	ordid INTEGER
) RETURNS INTEGER AS $$
	
DECLARE
	p_oftype VARCHAR(16) ;
	p_dftype VARCHAR(16) ;
	p_algoty VARCHAR(24) ;
	row RECORD ;
	err INT ;
	
BEGIN

	FOR row IN (
		SELECT orig_filetype, dest_filetype, algotype
		FROM t_inventorylinksconf
		WHERE orig_filetype = ftype ) LOOP
			
			p_oftype := row.orig_filetype ;
			p_dftype := row.dest_filetype ;
			p_algoty := row.algotype ;
			
			IF (p_algoty = 'LINKORDER') THEN
				err := p_inventorylinks_insert(fname, fversion, p_dftype, ordid, p_algoty) ;
			END IF;
			
			IF (p_algoty = 'LINKTEMPORIG') THEN
				err := p_inventorylinks_insert(fname, fversion, p_dftype, p_algoty) ;
			END IF;
			
			IF (p_algoty = 'LINKTEMPDEST') THEN
				err := p_inventorylinks_insert(fname, fversion, p_dftype, p_algoty) ;
			END IF;
	END LOOP ;
	
	RETURN err;

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
		'usdfwa-2.1.0',
		'2011-02-02',
		6,
		5,
		'USDFWA_2-1-0',
		'Changed fileversion size on t_inventory and altered all table and procedures with fileversion reference.'
) ;
