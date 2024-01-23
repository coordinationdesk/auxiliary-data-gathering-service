
-- Version 1.0.2
-- POSTGRES







---------------------------------------------------------------------------------
------------------------------ PROCEDURES MODIFICATION ------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	p_cr_distribution
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_cr_distribution(
	in_auth VARCHAR(24),
	in_tstamp TIMESTAMP(6)) RETURNS INTEGER AS $$

DECLARE
	_row RECORD ;
	
BEGIN
	FOR _row IN (
		SELECT DISTINCT
			inv.filename,
			inv.fileversion,
			drl.filetype_id,
			drl.user_id,
			drl.fileclass_id,
			drl.format_tt_id
		FROM
			t_crtemporaries crt
			INNER JOIN t_inventory inv
				ON crt.inventory_id = inv.id_inv
			INNER JOIN t_crrelations crr
				ON crt.crgroup_id = crr.crgroup_id
			INNER JOIN t_distributionrules drl
				ON crr.distributionrule_id = drl.id_distributionrule
				AND crr.repository_id = drl.repository_id
		WHERE 
			crt.author = in_auth 
			AND crt.tstamp = in_tstamp 
			AND drl.filetype_id = (
				SELECT id_filetype FROM t_filetypes WHERE filetype = inv.filetype)) LOOP
	
		PERFORM p_distribute_item_on_demand(
			_row.filename,
			_row.fileversion,
			_row.filetype_id,
			_row.user_id,
			_row.fileclass_id,
			_row.format_tt_id,
			NULL);	
	END LOOP ;
	
	INSERT INTO t_crhistory SELECT * FROM t_crtemporaries crt
		WHERE crt.author = in_auth AND crt.tstamp = in_tstamp ;
		
	DELETE FROM t_crtemporaries ct 
		WHERE author = in_auth AND tstamp = in_tstamp ;
	
	RETURN 0 ;

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
		'cr-1.0.2',
		'2015-05-25',
		5,
		24,
		'CR_1-0-2',
		'Fixed bug on p_cr_distribution.'
) ;


