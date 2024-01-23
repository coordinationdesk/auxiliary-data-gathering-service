/*

	Copyright 1995-2012, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Sentinel3 Auxiliary Reformatter application $

	$Id$

	$Author$
	
	$Log$
	Revision 1.34  2016/05/20 08:40:05  matteo.airoldi
	Creating new tag.
	
	Revision 1.33  2016/04/07 12:28:04  matteo.airoldi
	Creating new tag.
	
	Revision 1.32  2016/01/27 13:23:53  matteo.airoldi
	Creating new tag.
	
	Revision 1.31  2015/11/24 08:20:58  matteo.airoldi
	Creating new tag.
	
	Revision 1.30  2015/10/22 14:41:31  matteo.airoldi
	Creating new tag.
	
	Revision 1.29  2015/09/11 10:51:08  nicvac
	Creating new tag.
	
	Revision 1.28  2015/08/11 09:13:57  matteo.airoldi
	Creating new tag.
	
	Revision 1.27  2015/05/12 13:30:29  matteo.airoldi
	Creating new tag.
	
	Revision 1.26  2015/01/29 09:15:36  matteo.airoldi
	Creating new tag.
	
	Revision 1.25  2015/01/26 12:46:30  matteo.airoldi
	Creating new tag.
	
	Revision 1.24  2015/01/13 15:22:48  matteo.airoldi
	Creating new tag.
	
	Revision 1.23  2014/12/17 11:11:01  matteo.airoldi
	Creating new tag.
	
	Revision 1.22  2014/06/25 09:49:04  matteo.airoldi
	Creating new tag.
	
	Revision 1.21  2014/06/11 14:43:15  matteo.airoldi
	Creating new tag.
	
	Revision 1.20  2014/04/07 14:41:07  matteo.airoldi
	Creating new tag.
	
	Revision 1.19  2014/03/25 11:24:17  matteo.airoldi
	Creating new tag.
	
	Revision 1.18  2014/03/25 08:32:16  matteo.airoldi
	Creating new tag.
	
	Revision 1.17  2014/03/19 10:46:24  matteo.airoldi
	Creating new tag.
	
	Revision 1.16  2014/02/13 14:41:05  matteo.airoldi
	Creating new tag.
	
	Revision 1.15  2014/02/06 13:07:46  matteo.airoldi
	Creating new tag.
	
	Revision 1.14  2013/12/09 10:33:31  matteo.airoldi
	Creating new tag.
	
	Revision 1.13  2013/10/04 14:18:59  matteo.airoldi
	Creating new tag.
	
	Revision 1.12  2013/10/04 09:49:15  matteo.airoldi
	Creating new tag.
	
	Revision 1.11  2013/10/03 13:46:17  matteo.airoldi
	Creating new tag.
	
	Revision 1.10  2013/09/19 12:46:14  matteo.airoldi
	Creating new tag.
	
	Revision 1.9  2013/09/13 14:16:41  marfav
	Creating new tag.
	
	Revision 1.8  2013/07/12 13:02:06  marfav
	Creating new tag.
	
	Revision 1.7  2013/07/12 13:00:26  marfav
	Creating new tag.
	
	Revision 1.6  2013/07/08 15:26:40  marfav
	Creating new tag.
	
	Revision 1.5  2013/06/18 14:40:36  marfav
	Creating new tag.
	
	Revision 1.4  2013/06/14 15:08:31  matteo.airoldi
	Creating new tag.
	
	Revision 1.3  2013/06/14 11:00:14  marfav
	Creating new tag.
	
	Revision 1.2  2013/06/14 10:33:35  marfav
	Creating new tag.
	
	Revision 1.1.1.1  2013/06/11 13:33:29  marfav
	Importing S3AuxReformatterCore
	

*/

#include <auto_version_file>
#include <S3AuxRefCoreV.h>

_ACS_BEGIN_NAMESPACE(acs)

const char* S3AuxRefCoreV::_name = auto_version_file::name;
const char* S3AuxRefCoreV::_tag = auto_version_file::tag;

S3AuxRefCoreV::S3AuxRefCoreV() throw() {}
S3AuxRefCoreV::~S3AuxRefCoreV() throw() {}

const char* S3AuxRefCoreV::tag() throw() { return _tag ; }
const char* S3AuxRefCoreV::name() throw() { return _name ; }

_ACS_END_NAMESPACE

