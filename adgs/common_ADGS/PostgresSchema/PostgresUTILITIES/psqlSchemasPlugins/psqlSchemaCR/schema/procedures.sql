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
	Revision 1.5  2015/05/25 10:45:40  achnat
	Fixed bug on p_cr_distribution.
	
	Revision 1.4  2013/11/07 13:26:36  achnat
	Fixed bug on p_cr_distribution.
	
	Revision 1.3  2013/09/03 14:40:55  achnat
	Added crtag field to t_crhistory and t_crtemporaries tables and changed p_cr_distribution
	
	Revision 1.2  2013/04/19 10:33:43  achnat
	Changed procedure p_cr_distribution and added new indexes
	
	Revision 1.1.1.1  2013/03/19 10:40:09  achnat
	Import psqlSchemaCR
	
*/





--------------------------------------------------------------------------------
--
--    	  	  	  	  	  	  	  PROCEDURES
--
--------------------------------------------------------------------------------


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


