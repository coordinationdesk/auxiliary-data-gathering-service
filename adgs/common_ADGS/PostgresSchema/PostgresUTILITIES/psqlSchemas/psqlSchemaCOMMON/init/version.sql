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
	'common-02.00.00',
	'2021-06-16',
	5,
	40, 
	'COMMON_2-0-0', 
	'COMMON schema installation.'
) ;


