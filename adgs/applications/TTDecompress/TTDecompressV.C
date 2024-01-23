/*
	Copyright 2012, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: $

	$Id$

	$Author$

	$Log$
	Revision 1.12  2017/05/18 12:56:31  tergem
	Creating new tag.
	
	Revision 1.11  2017/05/04 10:38:07  tergem
	Creating new tag.
	
	Revision 1.10  2017/03/20 08:03:10  clanas
	Creating new tag.
	
	Revision 1.9  2016/10/24 10:12:18  francesco.avanzi
	Creating new tag.
	
	Revision 1.8  2015/06/25 14:24:00  damdec
	Creating new tag.
	
	Revision 1.7  2013/04/29 18:09:15  marpas
	Creating new tag.
	
	Revision 1.6  2013/04/29 14:45:34  chicas
	Creating new tag.
	
	Revision 1.5  2013/04/29 08:10:36  chicas
	Creating new tag.
	
	Revision 1.4  2013/02/04 15:29:02  chicas
	Creating new tag.
	
	Revision 1.3  2013/02/04 13:06:49  chicas
	Creating new tag.
	
	Revision 1.2  2013/01/28 15:59:46  chicas
	Creating new tag.
	
	Revision 1.1.1.1  2013/01/23 16:33:10  chicas
	Imported sources
	
	Revision 1.1.1.1  2012/11/30 12:44:54  chicas
	Imported sources
	
	
        
*/

#include <auto_version_file>
#include <TTDecompressV.h>

_ACS_BEGIN_NAMESPACE(acs)

const char* TTDecompressV::_name = auto_version_file::name;
const char* TTDecompressV::_tag = auto_version_file::tag;

TTDecompressV::TTDecompressV() throw() {}
TTDecompressV::~TTDecompressV() throw() {}

const char* TTDecompressV::tag() throw() { return _tag ; }
const char* TTDecompressV::name() throw() { return _name ; }

_ACS_END_NAMESPACE

