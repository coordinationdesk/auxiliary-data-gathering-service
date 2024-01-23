/*

Copyright 2010-2020, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S.  T_Schema table population script $

	$Id$

	$Author$

*/

/* 
	//////////////////////////////////////////////
	//	T_QualifiedFlag 
	//	Types of Qualification Flags
	//////////////////////////////////////////////
*/
INSERT INTO t_qualifiedflag (name) VALUES ('NOT_AVAILABLE') ;
INSERT INTO t_qualifiedflag (name) VALUES ('VALID_PRODUCT') ;
INSERT INTO t_qualifiedflag (name) VALUES ('PRODUCT_HAS_ERRORS') ;
INSERT INTO t_qualifiedflag (name) VALUES ('PRODUCT_HAS_WARNINGS') ;
INSERT INTO t_qualifiedflag (name) VALUES ('UNSUPPORTED_PRODUCT_TYPE') ;
INSERT INTO t_qualifiedflag (name) VALUES ('UNSUPPORTED_PRODUCT_VERSION') ;
INSERT INTO t_qualifiedflag (name) VALUES ('QUALIFIED_PRODUCT') ;
INSERT INTO t_qualifiedflag (name) VALUES ('UNQUALIFIED_PRODUCT') ;
INSERT INTO t_qualifiedflag (name) VALUES ('VALIDATION_FAILURE') ;
INSERT INTO t_qualifiedflag (name) VALUES ('NO_SCHEMA_AVAILABLE') ;
INSERT INTO t_qualifiedflag (name) VALUES ('PASSED') ;
INSERT INTO t_qualifiedflag (name) VALUES ('DEGRADED') ;
INSERT INTO t_qualifiedflag (name) VALUES ('FAILED') ;
INSERT INTO t_qualifiedflag (name) VALUES ('QUALIFIED_DUP') ;


