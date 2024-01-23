// PRQA S 1050 EOF
/*
	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libExporterBase$

	$Id$

	$Author$

	$Log$
	Revision 5.2  2013/06/20 12:32:48  marpas
	qa rules
	coding best practices applied
	instrumented to avoid qac++ analisys crash
	
	Revision 5.1  2013/06/17 13:57:07  nicvac
	New macro used.
	
	Revision 5.0  2013/06/07 15:51:57  marpas
	adopting libException 5.x standards
	
	Revision 1.2  2012/12/06 18:34:35  nicvac
	Removed dependency from libDataConsumer.
	
	Revision 1.1  2012/11/23 17:31:05  nicvac
	ExporterServiceResolver added.
	
        
*/ 


#include <ExporterServiceResolver.h>
#include <Filterables.h>
#include "ExporterBaseV.h"


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

namespace { 
    ExporterBaseV version;
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(ExporterServiceResolver)


ExporterServiceResolver::ExporterServiceResolver():
    Singleton <ExporterServiceResolver>(),
	_downloadCacheSptr()
{
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Id: " << Id);
}


void ExporterServiceResolver::unregister() {
	_downloadCacheSptr.reset();
}


void ExporterServiceResolver::setDownloadCacheSptr( CacheSptr const& downloadCacheSptr) { // PRQA S 4121
	_downloadCacheSptr = downloadCacheSptr;
}

CacheSptr ExporterServiceResolver::getDownloadCacheSptr() const { // PRQA S 4120
	return _downloadCacheSptr;
}



_ACS_END_NAMESPACE
