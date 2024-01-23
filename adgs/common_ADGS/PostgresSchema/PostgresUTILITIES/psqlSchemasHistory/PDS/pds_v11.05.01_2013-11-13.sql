
-- Version 11.5.1
-- POSTGRES








---------------------------------------------------------------------------------
--------------------------------- TABLES INSERT ---------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_originators
*/ ------------------------------
INSERT INTO t_originators ("id","name") VALUES (44,'MPC_');
INSERT INTO t_originators ("id","name") VALUES (45,'FOCC');


/* ------------------------------
	t_rollingpolicyactions
*/ ------------------------------
INSERT INTO t_rollingpolicyactions VALUES (6, 'REMOVEPARENT-REMOVECHILDREN') ;








---------------------------------------------------------------------------------
---------------------------- PROCEDURES MODIFICATION ----------------------------
---------------------------------------------------------------------------------

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
		'pds-11.5.1',
		'2013-11-13',
		3,
		2,
		'PDS_11-5-1',
		'Added new originators. Fixed bug on p_processinventory.'
) ;


