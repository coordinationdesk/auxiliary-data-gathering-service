/*

Copyright 2010-2015, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S. t_dbparameters table population script $

	$Id$

	$Author$

	$Log$
	Revision 1.4  2014/01/13 11:31:39  achnat
	Upgraded to schema version 1.8.0
	
	Revision 1.3  2011/09/21 15:38:25  achnat
	Changed name.
	
	Revision 1.1  2011/09/21 15:37:15  achnat
	Changed name.
	
	Revision 1.1  2011/06/10 13:33:41  achnat
	Added new tables.
	
*/





/* ------------------------------
	t_dbparameters
*/ ------------------------------
INSERT INTO t_dbparameters (parameter, parametertype_id, value_int) VALUES ('circ_def_max_ret', 1, 5) ;
--INSERT INTO t_dbparameters (parameter, parametertype_id, value_str) VALUES ('circ_start_stat', 2, 'STANDBY') ;


