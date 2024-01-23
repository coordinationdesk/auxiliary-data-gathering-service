/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Sentinel-3 ADF reformatter library $

	$Id$

	$Author$

	$Log$
	Revision 1.13  2015/03/31 08:37:44  matteo.airoldi
	Creating new tag.
	
	Revision 1.12  2014/06/25 10:30:02  matteo.airoldi
	Creating new tag.
	
	Revision 1.11  2014/06/11 14:50:57  matteo.airoldi
	Creating new tag.
	
	Revision 1.10  2014/03/27 11:17:15  matteo.airoldi
	Creating new tag.
	
	Revision 1.9  2014/02/25 15:19:58  matteo.airoldi
	Creating new tag.
	
	Revision 1.8  2014/02/13 13:11:42  matteo.airoldi
	Creating new tag.
	
	Revision 1.7  2013/10/04 09:52:02  matteo.airoldi
	Creating new tag.
	
	Revision 1.6  2013/07/12 12:58:22  marfav
	Creating new tag.
	
	Revision 1.5  2013/07/08 15:28:03  marfav
	Creating new tag.
	
	Revision 1.4  2013/06/14 10:11:37  marfav
	Creating new tag.
	
	Revision 1.3  2013/06/11 14:56:13  marfav
	Creating new tag.
	
	Revision 1.2  2013/06/11 08:49:31  marfav
	Creating new tag.
	
	Revision 1.1.1.1  2013/06/10 12:44:59  marfav
	importing libS3AuxFormatter
	

*/


#include <auto_version_file>
#include <s3afV.h>

using namespace acs ;

const char* s3afV::_name = auto_version_file::name;
const char* s3afV::_tag = auto_version_file::tag;


s3afV::s3afV() ACS_NOEXCEPT {}
s3afV::~s3afV() ACS_NOEXCEPT {}

const char* s3afV::tag() ACS_NOEXCEPT { return _tag ; }
const char* s3afV::name() ACS_NOEXCEPT { return _name ; }

