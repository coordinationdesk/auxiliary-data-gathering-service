/*

Copyright 2010-2020, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S.  T_StateTransitions_Flows table population script $

	$Id$

	$Author$

*/

/* 
	//////////////////////////////////////////////
	//	T_StateTransitions_Flows 
	//	Types of flows for state transitions
	//////////////////////////////////////////////
*/
INSERT INTO t_statetransitions_flows (id, flow_type) VALUES (1, 'none') ;
INSERT INTO t_statetransitions_flows (id, flow_type) VALUES (2, 'ARCHIVE') ;
INSERT INTO t_statetransitions_flows (id, flow_type) VALUES (3, 'DELETE') ;
INSERT INTO t_statetransitions_flows (id, flow_type) VALUES (4, 'LTA_ARCHIVE') ;
INSERT INTO t_statetransitions_flows (id, flow_type) VALUES (5, 'RETRIEVE') ;
INSERT INTO t_statetransitions_flows (id, flow_type) VALUES (6, 'REPUBLISHING') ;
INSERT INTO t_statetransitions_flows (id, flow_type) VALUES (7, 'RETRIEVEONLY') ;


