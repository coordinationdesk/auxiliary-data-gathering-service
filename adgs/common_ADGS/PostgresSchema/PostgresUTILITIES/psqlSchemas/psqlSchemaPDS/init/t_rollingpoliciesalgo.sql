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
	//	T_RollingPoliciesAlgo
	//////////////////////////////////////////////
*/
INSERT INTO t_rollingpoliciesalgo (name) VALUES ('TimeBased') ;
INSERT INTO t_rollingpoliciesalgo (name) VALUES ('OnDistribution') ;
INSERT INTO t_rollingpoliciesalgo (name) VALUES ('Used') ;
INSERT INTO t_rollingpoliciesalgo (name) VALUES ('ParametersBased') ;
