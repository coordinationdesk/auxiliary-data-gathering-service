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

	$Prod: A.C.S. libACSSafeFormatter$

	$Id$

	$Author$

	$Log$
	Revision 1.5  2013/12/11 15:46:16  nicvac
	Coding rules implemented. (by marpas?)
	
	Revision 1.4  2013/10/29 19:08:57  nicvac
	compilation warning processed.
	
	Revision 1.3  2013/10/29 15:59:08  nicvac
	S1PDGS-1899: implementation.
	
	Revision 1.2  2012/04/17 15:45:50  damdec
	_safeNamespace attribute added.
	
	Revision 1.1.1.1  2012/03/15 09:17:48  damdec
	Import libACSSafeFormatter
	
        
*/ 
#include <WrappedMetadata.h>

#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(WrappedMetadata)

WrappedMetadata::WrappedMetadata(const std::string &safeNameSpace, const std::string &safePrefix) :
    _safeNamespace(safeNameSpace), 
    _safePrefix(safePrefix), 
    _xmlStream() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

WrappedMetadata::~WrappedMetadata() throw() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB); // PRQA S 4631
}

WrappedMetadata::WrappedMetadata(const WrappedMetadata & r) :
    _safeNamespace(r._safeNamespace), 
    _safePrefix(r._safePrefix), 
    _xmlStream(r._xmlStream) 
{
}

WrappedMetadata& WrappedMetadata::operator=(const WrappedMetadata &r)
{
    if (this != &r ) {
        _safeNamespace = r._safeNamespace ; 
        _safePrefix = r._safePrefix ; 
        _xmlStream = r._xmlStream ;
    }
    return *this ;
}


_ACS_END_NAMESPACE
