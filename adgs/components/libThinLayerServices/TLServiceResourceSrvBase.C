/*
	Copyright 1995-2017, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libThinLayerServices$

	$Id$

	$Author$

	$Log$
        
*/ 

#include <TLServiceResourceSrvBase.h>

#include <Filterables.h>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(TLServiceResourceSrvBase)



TLServiceResourceSrvBase::TLServiceResourceSrvBase() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

TLServiceResourceSrvBase::~TLServiceResourceSrvBase() throw() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}



_ACS_END_NAMESPACE

