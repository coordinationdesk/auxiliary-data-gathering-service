/*

Copyright 2010-2015, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S. t_circulationmodes table population script $

	$Id$

	$Author$

	$Log$
	Revision 1.1  2018/02/19 14:41:44  achnat
	Upgraded schema version to circulation-1.20.0
	
*/





/* ------------------------------
	t_flowtags
*/ ------------------------------
INSERT INTO t_flowtags (flowtagname) VALUES ('DEFAULT');
INSERT INTO t_flowtags (flowtagname) VALUES ('PROCESSING');
INSERT INTO t_flowtags (flowtagname) VALUES ('DAGS_REQUEST');
INSERT INTO t_flowtags (flowtagname) VALUES ('OTHER');
INSERT INTO t_flowtags (flowtagname) VALUES ('DPC_ARCHIVE');
INSERT INTO t_flowtags (flowtagname) VALUES ('AI2AI_ARCHIVE');
INSERT INTO t_flowtags (flowtagname) VALUES ('OTHER_ARCHIVE');


