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

#ifndef _TLServiceResourceClientBase_H_
#define _TLServiceResourceClientBase_H_

#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)



/**
* \brief
* Base class for TLServiceResourceClient
* Define the interface for all the Clients requesting resources
**/

class TLServiceResourceClientBase  { 
public:

	/** TLServiceResourceClientBase Exceptions */
	exDECLARE_EXCEPTION(exTLServiceResourceClientBaseException, exException); // TLServiceResourceClientBase Exception.
	exDECLARE_EXCEPTION(exTLServiceResourceClientBaseCriticalException, exCriticalException); // TLServiceResourceClientBase Critical Exception.

	/** Default Class constructor */
	TLServiceResourceClientBase();
	/** Destructor */
	virtual ~TLServiceResourceClientBase() throw();

private:  // copy constructor and operator= defined but not implemented
	/** Copy Constructor */
	TLServiceResourceClientBase(const TLServiceResourceClientBase & );
	/** Operator = */
	TLServiceResourceClientBase &operator=(const TLServiceResourceClientBase &);

public:

	/** Url is empty if this service does not have a server */
	virtual void setServerUrl(const std::string& url) = 0;

	/** Responce timeout [s] */
	virtual void setResponseTimeout(size_t timeOutSec) = 0;

	/** Client tries to use a number of resources (using extra resource if available).
	 *  slotNumber is a progressive counter related to the number of resource used */
	virtual bool useResources(const std::string& clientId, size_t numOfResources) = 0;

	/** Free specified resources. Extra resources must be released first */
	virtual bool releaseResources (const std::string& clientId, size_t numOfResources) = 0;

	/** Free all resources used by the client */
	virtual bool resetClient (const std::string& clientId) = 0;


private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(TLServiceResourceClientBase)

};

_ACS_END_NAMESPACE

#endif //_TLServiceResourceClientBase_H_

