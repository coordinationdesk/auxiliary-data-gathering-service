/*

Copyright 2010-2015, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S. t_datastoragetypes table population script $

	$Id$

	$Author$

	$Log$
	Revision 1.2  2014/12/15 15:59:33  achnat
	Added new datastoragetype
	
	Revision 1.1  2011/07/15 13:01:24  achnat
	Added t_datastoragequeue and new procedures.
	
*/





/* ------------------------------
	t_datastoragetypes
*/ ------------------------------
INSERT INTO t_datastoragetypes (id, datastoragetype) VALUES (1, 'ARCHIVE') ;
INSERT INTO t_datastoragetypes (id, datastoragetype) VALUES (2, 'DELETE') ;
INSERT INTO t_datastoragetypes (id, datastoragetype) VALUES (3, 'RETRIEVE') ;
INSERT INTO t_datastoragetypes (id, datastoragetype) VALUES (4, 'LTA_ARCHIVE') ;
INSERT INTO t_datastoragetypes (id, datastoragetype) VALUES (5, 'REPUBLISHING') ;
INSERT INTO t_datastoragetypes (id, datastoragetype) VALUES (6, 'RETRIEVEONLY') ;


