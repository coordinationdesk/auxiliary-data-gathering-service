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
		'invbaseline-1.0.0',
		'2013-02-08',
		5,
		23,
		'INVBASELINE_1-0-0',
		'INVBASELINE plug-in installation.'
) ;


