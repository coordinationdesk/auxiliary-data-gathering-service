/*

Copyright 2010-2020, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S.  T_Mediatypes table population script $

	$Id$

	$Author$

*/

/* 
	//////////////////////////////////////////////
	//	T_Mediatypes 
	//	Types of Media
	//////////////////////////////////////////////
*/
INSERT INTO t_mediatypes (id_mediatype, mediatype, uncompressedcapacity) VALUES (1, 'CD',  650000) ;
INSERT INTO t_mediatypes (id_mediatype, mediatype, uncompressedcapacity) VALUES (2, 'DVD', 4000000) ;
INSERT INTO t_mediatypes (id_mediatype, mediatype, uncompressedcapacity) VALUES (3, 'FTP', 214748367) ;
INSERT INTO t_mediatypes (id_mediatype, mediatype, uncompressedcapacity) VALUES (4, 'LTO', 120000000) ;


