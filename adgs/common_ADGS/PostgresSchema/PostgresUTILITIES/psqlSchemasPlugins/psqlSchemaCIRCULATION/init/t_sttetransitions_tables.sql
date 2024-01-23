/*

Copyright 2010-2020, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S.  T_StateTransitions_Tables table population script $

	$Id$

	$Author$

*/

/* 
	//////////////////////////////////////////////
	//	T_StateTransitions_Tables 
	//	List of tables to check transitions
	//////////////////////////////////////////////
*/
INSERT INTO t_statetransitions_tables (table_name) VALUES ('t_dsqueue_x_cirqueue') ;
INSERT INTO t_statetransitions_tables (table_name) VALUES ('t_circulationsqueue') ;
INSERT INTO t_statetransitions_tables (table_name, flow_id) VALUES ('t_datastoragequeue', 2) ;
INSERT INTO t_statetransitions_tables (table_name, flow_id) VALUES ('t_datastoragequeue', 3) ;
INSERT INTO t_statetransitions_tables (table_name, flow_id) VALUES ('t_datastoragequeue', 4) ;
INSERT INTO t_statetransitions_tables (table_name, flow_id) VALUES ('t_datastoragequeue', 5) ;
INSERT INTO t_statetransitions_tables (table_name, flow_id) VALUES ('t_datastoragequeue', 6) ;
INSERT INTO t_statetransitions_tables (table_name, flow_id) VALUES ('t_datastoragequeue', 7) ;


