
-- Version 1.0.3
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

BEGIN

	INSERT INTO t_temporarydistribution (
		duedate, 
		filename, 
		fileversion, 
		mediatype, 
		userid, 
		algo, 
		policy,
		format,
		disseminationpackingalgo_id )
	SELECT DISTINCT
		(NOW()::DATE + drl.maxdaytodeliver) + NOW()::TIME(6),
		inv.filename,
		inv.fileversion,
		mdt.mediatype,
		usr.userid,
		drl.distributionalgo_id,
		drl.id_distributionrule,
		drl.format_tt_id,
		drl.disseminationpackingalgo_id
	FROM
		t_crtemporaries crt
		INNER JOIN t_inventory inv
			ON crt.inventory_id = inv.id_inv
		INNER JOIN t_crrelations crr
			ON crt.crgroup_id = crr.crgroup_id
		INNER JOIN t_distributionrules drl
			ON crr.distributionrule_id = drl.id_distributionrule
			AND crr.repository_id = drl.repository_id
		INNER JOIN t_mediatypes mdt
			ON drl.mediatype_id = mdt.id_mediatype
		INNER JOIN t_users usr
			ON drl.user_id = usr.id_user
	WHERE 
		crt.author = in_auth 
		AND crt.tstamp = in_tstamp 
		AND drl.filetype_id = (
			SELECT id_filetype FROM t_filetypes WHERE filetype = inv.filetype)
	;
	
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
		'cr-1.0.3',
		'2015-06-23',
		5,
		24,
		'CR_1-0-3',
		'Implemented temporarydistribution insert into p_cr_distribution.'
) ;


