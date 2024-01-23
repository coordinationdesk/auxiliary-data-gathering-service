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
	Revision 1.1  2017/10/17 15:27:21  nicvac
	ECICMF-93: Base and default TL Service Resources
	
        
*/

#ifndef _TLServiceResourceSrvBase_H_
#define _TLServiceResourceSrvBase_H_

#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)



/**
* \brief
* Base class for TLServiceResourceSrv
* Define the interface for all the Services handling resources
**/

class TLServiceResourceSrvBase  {
public:

	/** TLServiceResourceSrvBase Exceptions */
	exDECLARE_EXCEPTION(exTLServiceResourceSrvBaseException, exException); // TLServiceResourceSrvBase Exception.
	exDECLARE_EXCEPTION(exTLServiceResourceSrvBaseCriticalException, exCriticalException); // TLServiceResourceSrvBase Critical Exception.

	/** Default Class constructor */
	TLServiceResourceSrvBase();
	/** Destructor */
	virtual ~TLServiceResourceSrvBase() throw();

private:  // copy constructor and operator= defined but not implemented
	/** Copy Constructor */
	TLServiceResourceSrvBase(const TLServiceResourceSrvBase & );
	/** Operator = */
	TLServiceResourceSrvBase &operator=(const TLServiceResourceSrvBase &);
	

public:

	/** Return true if it is an external service. Return the url of the external service */
	virtual bool isService (std::string& serviceUrl) const = 0;

	/** Start the service */
	virtual void startService() = 0;

	/** Stop the service */
	virtual void stopService() = 0;

	/** Reload configuration */
	virtual void reloadConfiguration() = 0;

	/** Return number of free resources (e.g. cores, slots). Reserved resources are not counted. */
	virtual size_t getFreeResources() const = 0;

	/** Return number of reserved resources (but still not used) by the client */
	virtual size_t getUnusedResources( const std::string& clientId) const = 0;

	/** Reserve, if possible, a number of resources for the client. Return false if not possible */
	virtual bool reserveResources (const std::string& clientId, size_t numOfResources) = 0;

	/** Client tries to use a number of resources (using extra resource if available).
	 *  slotNumber is a progressive counter related to the number of resource used */
	virtual bool useResources(const std::string& clientId, size_t numOfResources) = 0;

	/** Free specified resources. Extra resources must be released first */
	virtual bool releaseResources (const std::string& clientId, size_t numOfResources) = 0;

	/** Free all resources used by the client */
	virtual bool resetClient (const std::string& clientId) = 0;

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(TLServiceResourceSrvBase)

};

_ACS_END_NAMESPACE

#endif //_TLServiceResourceSrvBase_H_

