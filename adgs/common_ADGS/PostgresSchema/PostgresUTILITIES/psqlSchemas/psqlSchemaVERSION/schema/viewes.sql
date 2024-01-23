/*

Copyright 2010-2015, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S. Databases schema views script $

	$Id$

	$Author$

	$Log$
	Revision 1.3  2015/07/24 13:58:01  achnat
	Changed view
	
	Revision 1.2  2013/07/17 12:47:56  achnat
	Added new schema and changed view.
	
	Revision 1.1.1.1  2010/12/13 16:24:08  achnat
	Import psqlSchemaVERSION
	

*/





--------------------------------------------------------------------------------
--
--    	  	  	  	  	  	  	  VIEWS
--
--------------------------------------------------------------------------------

/* ------------------------------
	v_schema_version
*/ ------------------------------
CREATE OR REPLACE VIEW v_schema_version (
	version, 
	schema_type,
	modification_type,
	modification_date,
	applied_on,  
	cvs_tag,
	description)
AS SELECT 
	sc.version, 
	st.schematype, 
	sm.modificationtype,
	sc.modificationdate,
	sc.appliedon, 
	sc.cvstag,
	sc.description
FROM
	t_schema AS sc 
		INNER JOIN (SELECT schematype, MAX(f_get_version_number(cvstag)) AS maxtag FROM t_schema GROUP BY schematype) gsc
			ON sc.schematype = gsc.schematype AND f_get_version_number(sc.cvstag) = gsc.maxtag
		INNER JOIN t_schematypes AS st
			ON sc.schematype = st.id
		INNER JOIN t_schemamodificationtypes AS sm
			ON sc.modificationtype = sm.id
ORDER BY 
	sc.version
;


