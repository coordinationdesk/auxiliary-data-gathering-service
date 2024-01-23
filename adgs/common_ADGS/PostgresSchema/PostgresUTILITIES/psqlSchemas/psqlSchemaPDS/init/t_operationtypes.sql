/*

Copyright 2010-2020, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S.  T_OperationTypes table population script $

	$Id$

	$Author$

*/

/* 
	//////////////////////////////////////////////
	//	T_OperationTypes 
	//	Types of Operation
	//////////////////////////////////////////////
*/
INSERT INTO t_operationtypes (id_operationtype, type) VALUES (1, 'UNKNOWN') ;
INSERT INTO t_operationtypes (id_operationtype, type) VALUES (2, 'IMPORT') ;
INSERT INTO t_operationtypes (id_operationtype, type) VALUES (3, 'EXPORT') ;
INSERT INTO t_operationtypes (id_operationtype, type) VALUES (4, 'UPLOAD') ;
INSERT INTO t_operationtypes (id_operationtype, type) VALUES (5, 'DOWNLOAD') ;
INSERT INTO t_operationtypes (id_operationtype, type) VALUES (6, 'EREASE') ;
INSERT INTO t_operationtypes (id_operationtype, type) VALUES (7, 'POSTPROC') ;


