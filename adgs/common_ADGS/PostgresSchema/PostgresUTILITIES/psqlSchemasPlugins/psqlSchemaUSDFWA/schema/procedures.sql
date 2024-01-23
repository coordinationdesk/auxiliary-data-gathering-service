/*

Copyright 2010-2020, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S. Databases schema procedures script $

	$Id$

	$Author$

	$Log$
*/





--------------------------------------------------------------------------------
--
--    	  	  	  	  	  	  	  PROCEDURES
--
--------------------------------------------------------------------------------

------------------------------------------------
--- P_INVENTORYLINKS_INSERT (for order algo) ---
------------------------------------------------
CREATE FUNCTION p_inventorylinks_insert (
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
CREATE FUNCTION p_inventorylinks_insert (
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


