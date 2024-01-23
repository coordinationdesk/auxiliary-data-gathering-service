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
	//	T_Smtypes 
	//	Types of Storage Manager
	//////////////////////////////////////////////
*/
INSERT INTO t_smtype (id, type) VALUES (1,'ACSHSM') ;
INSERT INTO t_smtype (id, type) VALUES (2,'ACSISM') ;
INSERT INTO t_smtype (id, type) VALUES (3,'STAFSM') ;
INSERT INTO t_smtype (id, type) VALUES (4,'AMSSM') ;
INSERT INTO t_smtype (id, type) VALUES (5,'SatStore') ;
INSERT INTO t_smtype (id, type) VALUES (6,'DISCACHE');
INSERT INTO t_smtype (id, type) VALUES (7,'UMARF');
INSERT INTO t_smtype (id, type) VALUES (8,'ACSLTA');
