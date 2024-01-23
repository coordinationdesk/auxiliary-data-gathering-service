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
	//	T_Schema 
	//	Schema version installed
	//////////////////////////////////////////////
*/
INSERT INTO t_schema (
	version, 
	modificationdate,
	modificationtype, 
	schematype, 
	cvstag, 
	description)
VALUES (
	'srv-01.09.01',
	'2021-11-25',
	5,
	35, 
	'SRV_1-9-1', 
	'SRV plug-in installation.'
) ;


