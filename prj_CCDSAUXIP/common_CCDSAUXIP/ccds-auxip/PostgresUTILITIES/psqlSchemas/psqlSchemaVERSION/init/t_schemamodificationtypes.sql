/*

Copyright 2010-2015, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S. T_SchemaModificationTypes population script $

	$Id$

	$Author$

	$Log$

*/





/* 
	//////////////////////////////////////////////
	//	T_SchemaModificationTypes 
	//	Types of schema modifications
	//////////////////////////////////////////////
*/
INSERT INTO t_schemamodificationtypes VALUES (1, 'UNKNOWN') ;
INSERT INTO t_schemamodificationtypes VALUES (2, 'NEW INSTALLATION') ;
INSERT INTO t_schemamodificationtypes VALUES (3, 'UPGRADE SCHEMA') ;
INSERT INTO t_schemamodificationtypes VALUES (4, 'PATCH ON SCHEMA') ;
INSERT INTO t_schemamodificationtypes VALUES (5, 'PLUGIN INSTALLATION');
INSERT INTO t_schemamodificationtypes VALUES (6, 'PLUGIN UPGRADE');
