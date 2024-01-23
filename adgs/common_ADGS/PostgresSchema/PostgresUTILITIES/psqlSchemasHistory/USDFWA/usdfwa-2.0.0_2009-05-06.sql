
-- Version usdfwa-2.0
-- POSTGRES

--------------------------------------------------------------
-- ATTENZIONE!!!                                            --
-- Questa modifica è applicabile senza ulteriori interventi --
-- sul database solo per le versioni di schema del PDS 7.3  --
-- o superiori.                                             --
--------------------------------------------------------------








---------------------------------------------------------------------------------
-------------------------------- TABLES CREATION --------------------------------
---------------------------------------------------------------------------------

------------------------
--- T_INVENTORYLINKS ---
------------------------
CREATE TABLE "t_inventorylinks" (
    "id" SERIAL NOT NULL,
    "orig_filename" VARCHAR(128) NOT NULL,
    "orig_fileversion" VARCHAR(8) NOT NULL,
    "dest_filename" VARCHAR(128) NOT NULL,
    "dest_fileversion" VARCHAR(8) NOT NULL,
    "linkdate" TIMESTAMP(6) DEFAULT NOW() NOT NULL,
    "description" VARCHAR(64)
);
ALTER TABLE "t_inventorylinks" 
	ADD CONSTRAINT "pk_t_inventorylinks" 
		PRIMARY KEY ("id") ;

ALTER TABLE t_inventorylinks 
	ADD CONSTRAINT fk_invlinks_orig 
		FOREIGN KEY (orig_filename, orig_fileversion)
			REFERENCES t_inventory (filename, fileversion) 
				ON DELETE CASCADE ;

ALTER TABLE t_inventorylinks 
	ADD CONSTRAINT fk_invlinks_dest 
		FOREIGN KEY (dest_filename, dest_fileversion)
			REFERENCES t_inventory (filename, fileversion) 
				ON DELETE CASCADE ;

ALTER TABLE t_inventorylinks 
	ADD CONSTRAINT ck_invlinks_orig_dest
		CHECK ((orig_filename != dest_filename ) OR (orig_fileversion != dest_fileversion )) ;


----------------------------
--- T_INVENTORYLINKSCONF ---
----------------------------
CREATE TABLE t_inventorylinksconf (
    id SERIAL NOT NULL,
    orig_filetype VARCHAR(16) NOT NULL,
    dest_filetype VARCHAR(16) NOT NULL,
    algotype VARCHAR(24) NOT NULL,
    description VARCHAR(64)
);
ALTER TABLE t_inventorylinksconf 
	ADD CONSTRAINT pk_t_inventorylinksconf
		PRIMARY KEY (id) ;

ALTER TABLE t_inventorylinksconf 
	ADD CONSTRAINT fk_invlinkconf_ftype1
		FOREIGN KEY (orig_filetype)
			REFERENCES t_filetypes (filetype) 
				ON DELETE CASCADE ;

ALTER TABLE t_inventorylinksconf 
	ADD CONSTRAINT fk_invlinkconf_ftype2
		FOREIGN KEY (dest_filetype)
			REFERENCES t_filetypes (filetype) 
				ON DELETE CASCADE ;

ALTER TABLE t_inventorylinksconf 
	ADD CONSTRAINT uq_t_invlinksconf
		UNIQUE (orig_filetype, dest_filetype, algotype) ;







---------------------------------------------------------------------------------
------------------------------ PROCEDURES CREATION ------------------------------
---------------------------------------------------------------------------------

------------------------------------------------
--- P_INVENTORYLINKS_INSERT (for order algo) ---
------------------------------------------------
CREATE FUNCTION p_inventorylinks_insert (
	fname VARCHAR(128), 
	fversion VARCHAR(8),
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
CREATE FUNCTION p_inventorylinks_insert (
	fname VARCHAR(128), 
	fversion VARCHAR(8),
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








---------------------------------------------------------------------------------
---------------------------- PROCEDURES MODIFICATION ----------------------------
---------------------------------------------------------------------------------

------------------------------
--- P_INVENTORYLINKS_CHECK ---
------------------------------
CREATE OR REPLACE FUNCTION p_inventorylinks_check (
	fname VARCHAR(128), 
	fversion VARCHAR(8),
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
		description )
VALUES	(
		'usdfwa-2.0',
		'2009-05-08',
		3,
		11,
		'Added tables t_inventorylinks and t_inventorylinksconf and procedures p_inventorylinks_insert (for and time algos). Installed working version of p_inventorylinks_check procedure.'
) ;
