/*

Copyright 2010-2020, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S.  T_UserModes table population script $

	$Id$

	$Author$

*/

/* 
	//////////////////////////////////////////////
	//	T_UserModes 
	//	Types of User Mode
	//////////////////////////////////////////////
*/
INSERT INTO t_usermodes (id, mode) VALUES (1, 'NOMINAL') ;
INSERT INTO t_usermodes (id, mode) VALUES (2, 'TEST') ;
INSERT INTO t_usermodes (id, mode) VALUES (3, 'DEBUG') ;
