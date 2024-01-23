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
	//	T_TmpNameStrategy 
	//	Types of temporary names
	//////////////////////////////////////////////
*/
INSERT INTO t_tmpnamestrategy (id_tmpnamestrategy, tempname) VALUES (1, 'PREFIX') ;
INSERT INTO t_tmpnamestrategy (id_tmpnamestrategy, tempname) VALUES (2, 'SUFFIX') ;
INSERT INTO t_tmpnamestrategy (id_tmpnamestrategy, tempname) VALUES (3, 'TMPDIR') ;
INSERT INTO t_tmpnamestrategy (id_tmpnamestrategy, tempname) VALUES (4, 'TSTAMP') ;
