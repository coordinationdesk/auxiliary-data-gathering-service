/*

Copyright 2010-2020, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S. t_schema table population script $

	$Id$

	$Author$

	$Log$
	Revision 1.3  2013/06/04 13:22:35  achnat
	Upgraded schema version
	
	Revision 1.2  2013/03/27 09:54:20  achnat
	Moved t_filetypes_stylesheets and t_stylesheetoutputtypes in REPORT schema
	
*/





/* 
	//////////////////////////////////////////////
	//	T_Schema 
	//	Schema version installed
	//////////////////////////////////////////////
*/
INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'reports-2.1.1',
		'2014-05-05',
		5,
		22,
		'REPORTS_2-1-1',
		'REPORTS plug-in schema installation.'
) ;


