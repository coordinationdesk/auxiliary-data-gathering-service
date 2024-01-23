/*

Copyright 2010-2020, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S.  T_TmpNameStrategy table population script $

	$Id$

	$Author$

*/

/* 
	//////////////////////////////////////////////
	//	T_RollingPolicyActions
	//////////////////////////////////////////////
*/
INSERT INTO t_rollingpolicyactions VALUES (1, 'KEEP') ;
INSERT INTO t_rollingpolicyactions VALUES (2, 'CLEANSTORAGE') ;
INSERT INTO t_rollingpolicyactions VALUES (3, 'INVALIDATE') ;
INSERT INTO t_rollingpolicyactions VALUES (4, 'REMOVE') ;
INSERT INTO t_rollingpolicyactions VALUES (5, 'INVALIDATEPARENT-REMOVECHILDREN') ;
INSERT INTO t_rollingpolicyactions VALUES (6, 'REMOVEPARENT-REMOVECHILDREN') ;
INSERT INTO t_rollingpolicyactions VALUES (7, 'AUTOPARENT-REMOVECHILDREN') ;
