
-- Version 11.12.02
-- POSTGRES








--------------------------------------------------------------------------------
------------------------------ PROCEDURES CHANGES ------------------------------
--------------------------------------------------------------------------------

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
	
	RETURN 0 ;

END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'pds-11.12.02' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-11.12.02',
		'2016-01-15',
		3,
		2,
		'PDS_11-12-2',
		'Fixed bun on p_distribute_order_inventory'
) ;


