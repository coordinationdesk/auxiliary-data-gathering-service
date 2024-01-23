/*

Copyright 2010-2020, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S.  T_DbParameters table population script $

	$Id$

	$Author$

*/

/* 
	//////////////////////////////////////////////
	//	T_DbParameters 
	//	Set value for cov_ondemand_del_threshold
	//////////////////////////////////////////////
*/
INSERT INTO t_dbparameters (parameter, parametertype_id, value_int) VALUES ('cov_ondemand_del_threshold', 1, 86400) ;


