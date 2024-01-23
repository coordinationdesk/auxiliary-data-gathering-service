/*
	Copyright 1995-2014, Advanced Computer Systems , Inc.
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
	Revision 1.2  2014/06/23 10:55:21  lucio.pulvirenti
	Creating new tag.
	
	Revision 1.1.1.1  2014/06/20 15:22:31  lucio.pulvirenti
	Import FileVersioningTask
	
	
        
*/

#include <auto_version_file>
#include <FileVersioningTaskV.h>

_ACS_BEGIN_NAMESPACE(acs)

const char* FileVersioningTaskV::_name = auto_version_file::name;
const char* FileVersioningTaskV::_tag = auto_version_file::tag;

FileVersioningTaskV::FileVersioningTaskV() throw() {}
FileVersioningTaskV::~FileVersioningTaskV() throw() {}

const char* FileVersioningTaskV::tag() throw() { return _tag ; }
const char* FileVersioningTaskV::name() throw() { return _name ; }

_ACS_END_NAMESPACE

