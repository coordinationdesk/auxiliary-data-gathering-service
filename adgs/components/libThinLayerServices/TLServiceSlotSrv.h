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
	Revision 1.2  2017/10/19 15:46:51  marfav
	Developing Slot based server
	
	Revision 1.1  2017/10/17 15:27:21  nicvac
	ECICMF-93: Base and default TL Service Resources
	
        
*/

#ifndef _TLServiceSlotSrv_H_
#define _TLServiceSlotSrv_H_

#include <exException.h>
#include <ThreadSafe.h>
#include <set>

#include <TLServiceResourceSrvBase.h>

_ACS_BEGIN_NAMESPACE(acs)

/**
* \brief
* Slot Specialization for TLServiceResourceSrvBase
**/

class TLServiceSlotSrv: public TLServiceResourceSrvBase {
public:

	/** Default Class constructor */
	TLServiceSlotSrv();
	/** Destructor */
	virtual ~TLServiceSlotSrv() throw();

private:  // copy constructor and operator= defined but not implemented
	TLServiceSlotSrv(const TLServiceSlotSrv & );
	TLServiceSlotSrv &operator=(const TLServiceSlotSrv &);

public:
	/** Interface implementation */
	virtual bool isService (std::string& serviceUrl) const;
	virtual void startService();
	virtual void stopService();
	virtual void reloadConfiguration();
	virtual size_t getFreeResources() const;
	virtual size_t getUnusedResources( const std::string& clientId) const;
	virtual bool reserveResources (const std::string& clientId, size_t numOfResources);
	virtual bool useResources(const std::string& clientId, size_t numOfResources);
	virtual bool releaseResources (const std::string& clientId, size_t numOfResources);
	virtual bool resetClient (const std::string& clientId);

private:
	/** Private operations */
	void init();
	
	void setMaxSlots (unsigned long);
	unsigned long getMaxSlots() const;
	
private:
	/** Private attributes and debug definitions*/
	unsigned long maxSlots_;
	Mutex mutex_;
	std::set <std::string> clientSet_;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(TLServiceSlotSrv)
};

_ACS_END_NAMESPACE

#endif //_TLServiceSlotSrv_H_

