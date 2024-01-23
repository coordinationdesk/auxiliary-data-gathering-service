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
	Revision 1.4  2015/06/23 14:54:31  achnat
	Implemented temporarydistribution insert into p_cr_distribution.
	
	Revision 1.3  2015/05/25 10:45:40  achnat
	Fixed bug on p_cr_distribution.
	
	Revision 1.2  2013/11/07 13:26:36  achnat
	Fixed bug on p_cr_distribution.
	
	Revision 1.1.1.1  2013/03/19 10:40:09  achnat
	Import psqlSchemaCR
	
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
		'cr-1.0.4',
		'2016-08-03',
		5,
		24,
		'CR_1-0-4',
		'Coordinated Release plug-in installation.'
) ;


