/*
	Copyright 1995-2017, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. TlDaemon$

	$Id$

	$Author$

	$Log$
        
*/

#ifndef _OrderReservedResources_H_
#define _OrderReservedResources_H_

#include <exException.h>

#include <boost/shared_ptr.hpp>

_ACS_BEGIN_NAMESPACE(acs)



/**
* \brief
* Reserved Resources for an Order.
* The last owner that destroy me, will reset the resources related to clientId.
**/

class OrderExecutionMgr;

class OrderReservedResources  { 
public:

	/** Class constructor */
	OrderReservedResources( OrderExecutionMgr& orderExecutionMgr,
			const bool& isService,
			const std::string& serviceResManagerKey,
			const std::string& clientId,
			const std::string& resManagerUrl
			);
	/** Destructor */
	virtual ~OrderReservedResources();

	/** Getters */
	const std::string& getClientId() const             { return _clientId; }
	const bool isService() const                       { return _isService; }
	const std::string& getResManagerUrl() const        { return _resManagerUrl;}
	const std::string& getServiceResManagerKey() const { return _serviceResManagerKey; }

private:  // copy constructor and operator= defined but not implemented
	/** Copy Constructor */
	OrderReservedResources(const OrderReservedResources & );
	/** Operator = */
	OrderReservedResources &operator=(const OrderReservedResources &);
	
private:
	//Keep the reference to release the resources at destruction time
	OrderExecutionMgr& _orderExecutionMgr;

	const bool _isService;
	const std::string _serviceResManagerKey;
	const std::string _clientId;
	const std::string _resManagerUrl;


private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(OrderReservedResources)

};

/** Shared pointer Types definitions */
typedef boost::shared_ptr<OrderReservedResources> OrderReservedResourcesSptr;

_ACS_END_NAMESPACE

#endif //_OrderReservedResources_H_

