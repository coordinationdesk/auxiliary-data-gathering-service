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
	Revision 1.2  2017/10/17 17:14:07  nicvac
	ECICMF-93: fixed
	
	Revision 1.1  2017/10/17 15:27:21  nicvac
	ECICMF-93: Base and default TL Service Resources
	
        
*/ 

#include <TLServiceSlotClient.h>

#include <Filterables.h>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(TLServiceSlotClient)



TLServiceSlotClient::TLServiceSlotClient() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

TLServiceSlotClient::~TLServiceSlotClient() throw() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

void TLServiceSlotClient::setServerUrl(const std::string& url) {

}

void TLServiceSlotClient::setResponseTimeout(size_t timeOutSec) {

}

bool TLServiceSlotClient::useResources(const std::string& clientId, size_t numOfResources) {
	return true;
}

bool TLServiceSlotClient::releaseResources (const std::string& clientId, size_t numOfResources) {
	return true;
}

bool TLServiceSlotClient::resetClient (const std::string& clientId) {
	return true;
}



_ACS_END_NAMESPACE

