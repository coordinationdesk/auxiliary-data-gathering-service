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
	//	DB Parameters values
	//////////////////////////////////////////////
*/

INSERT INTO t_dbparameters 
	(parameter, parametertype_id, value_int, param_description) 
VALUES 
	('app_even_expir_days', 1, 30, 'Represents the number of days after which an application event must be removed.') ;


INSERT INTO t_dbparameters 
	(parameter, parametertype_id, value_bool, param_description) 
VALUES 
	('order_transition_table_enabled', 4, 't', 'TRUE if the new system to chek the status transition is installed') ;
