/*

Copyright 2010-2020, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S.  T_DistributionAlgo table population script $

	$Id$

	$Author$

*/

/*
	//////////////////////////////////////////////
	//	T_DistributionAlgo 
	//	Id Name
	//////////////////////////////////////////////
*/
INSERT INTO t_distributionalgo (id, name, on_demand) VALUES (1, 'ALWAYS', 't') ;
INSERT INTO t_distributionalgo (id, name, on_demand) VALUES (2, 'QUALIFIED', 't') ;
INSERT INTO t_distributionalgo (id, name, on_demand) VALUES (3, 'REDISTRIBUTION', 't') ;
INSERT INTO t_distributionalgo (id, name, on_demand) VALUES (4, 'QUALIFYREPORT', 't') ;
INSERT INTO t_distributionalgo (id, name, on_demand) VALUES (5, 'PARTIALCVRG', 'f') ;
INSERT INTO t_distributionalgo (id, name, on_demand) VALUES (6, 'TOTALCVRG', 'f') ;
INSERT INTO t_distributionalgo (id, name, on_demand) VALUES (7, 'DATASTRIPCONDITIONED', 'f') ;
INSERT INTO t_distributionalgo (id, name, on_demand) VALUES (8, 'QCC_DISTR_CONDITIONED', 't') ;

