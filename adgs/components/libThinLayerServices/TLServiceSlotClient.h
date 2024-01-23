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

#ifndef _TLServiceSlotClient_H_
#define _TLServiceSlotClient_H_

#include <exException.h>

#include <TLServiceResourceClientBase.h>

_ACS_BEGIN_NAMESPACE(acs)

/**
* \brief
* Slot Specialization for TLServiceResourceClientBase
**/

class TLServiceSlotClient: public TLServiceResourceClientBase {
public:

	/** Default Class constructor */
	TLServiceSlotClient();
	/** Destructor */
	virtual ~TLServiceSlotClient() throw();

private:  // copy constructor and operator= defined but not implemented
	TLServiceSlotClient(const TLServiceSlotClient & );
	TLServiceSlotClient &operator=(const TLServiceSlotClient &);

public:
	/** Interface implementation */
	virtual void setServerUrl(const std::string& url);
	virtual void setResponseTimeout(size_t timeOutSec);
	virtual bool useResources(const std::string& clientId, size_t numOfResources);
	virtual bool releaseResources (const std::string& clientId, size_t numOfResources);
	virtual bool resetClient (const std::string& clientId);

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(TLServiceSlotClient)

};

_ACS_END_NAMESPACE

#endif //_TLServiceSlotClient_H_

