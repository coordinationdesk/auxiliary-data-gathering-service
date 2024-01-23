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
	Revision 1.3  2017/10/23 13:40:42  nicvac
	Interface changed
	
	Revision 1.2  2017/10/19 15:46:51  marfav
	Developing Slot based server
	
	Revision 1.1  2017/10/17 15:27:21  nicvac
	ECICMF-93: Base and default TL Service Resources
	
        
*/ 

#include <TLServiceSlotSrv.h>
#include <rsResourceSet.h>
#include <ConfigurationSingleton.h>

#include <Filterables.h>

using namespace std;

namespace { // Unnamed namespace
	static const string MaxOrderKey = "TlDaemon.TLD_Max_Parallel_Orders";
};

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(TLServiceSlotSrv)



TLServiceSlotSrv::TLServiceSlotSrv() :
	maxSlots_ (1),
	mutex_ (),
	clientSet_()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	init();
}

TLServiceSlotSrv::~TLServiceSlotSrv() throw() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

bool TLServiceSlotSrv::isService (std::string& serviceUrl) const {
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_LOW_VERB);
	return false;
}

void TLServiceSlotSrv::setMaxSlots (unsigned long s)
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_LOW_VERB);
	ThreadSafe::Lock lock (mutex_);
	maxSlots_ = s;
	ACS_LOG_INFO ("Setting max number of available slots to " << s);
}

unsigned long TLServiceSlotSrv::getMaxSlots() const
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_LOW_VERB);
	ThreadSafe::Lock lock (mutex_);
	return maxSlots_;
}

void TLServiceSlotSrv::init()
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_LOW_VERB);
	// All the parameters can be set in the reload configuration
	reloadConfiguration();
}

void TLServiceSlotSrv::startService() {
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_LOW_VERB);
}

void TLServiceSlotSrv::stopService() {
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_LOW_VERB);
}

void TLServiceSlotSrv::reloadConfiguration() 
{
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_LOW_VERB);
	rsResourceSet& rs = ConfigurationSingleton::instance()->get();
	unsigned long slots = getMaxSlots();
	rs.getValue (MaxOrderKey, slots, false);
	if ( slots != getMaxSlots() )
	{
		setMaxSlots (slots);
	}
}

size_t TLServiceSlotSrv::getFreeResources() const {
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_LOW_VERB);
	ThreadSafe::Lock lock (mutex_);

	int freeResources = getMaxSlots() - clientSet_.size();
	
	// If negative (change of configuration) just return zero
	return ( freeResources > 0 ? freeResources : 0);
}

size_t TLServiceSlotSrv::getUnusedResources( const std::string& clientId) const {
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_LOW_VERB);
	return 0;
}

bool TLServiceSlotSrv::reserveResources (const std::string& clientId, size_t numOfResources) {
	ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Trying to reserve a resource for client " << clientId);	
	
	// Each client uses exactly one resource
	ThreadSafe::Lock lock (mutex_);
	bool retval = false;
	if (getFreeResources() > 0)
	{
		clientSet_.insert (clientId);
		retval = true;
	}
	ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Returning " << boolalpha << retval);	
	return retval;
}

bool TLServiceSlotSrv::useResources(const std::string& clientId, size_t numOfResources) {
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_LOW_VERB);
	return true;
}

bool TLServiceSlotSrv::releaseResources (const std::string& clientId, size_t numOfResources) {
	ACS_CLASS_ANNOUNCE_DEBUG (ACS_LOW_VERB);
	return true;
}

bool TLServiceSlotSrv::resetClient (const std::string& clientId) {
	ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Resetting resource usage for client " << clientId);	
	// Each client uses exactly one resource
	ThreadSafe::Lock lock (mutex_);
	if (clientSet_.erase (clientId) == 0)
	{
#warning exception if client never reserved??
	}
	return true;
}


_ACS_END_NAMESPACE

