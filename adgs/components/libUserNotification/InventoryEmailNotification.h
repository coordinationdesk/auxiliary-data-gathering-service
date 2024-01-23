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

	$Prod: A.C.S. libUserNotification$

	$Id$

	$Author$

	$Log$
	Revision 5.1  2013/06/17 13:22:48  marpas
	finalized adoption of libException 5.x standards
	coding best practices applied
	qa rules applied
	
	Revision 5.0  2013/06/13 18:24:46  marpas
	adoption of libException 5.x standards in progress
	
	Revision 1.1  2013/05/24 18:31:27  nicvac
	S2PDGS-308: Inventory notifications implemented.
	
        
*/

#ifndef _InventoryEmailNotification_H_
#define _InventoryEmailNotification_H_

#include <acs_c++config.hpp>
#include <exException.h>

#include <dbPersistent.h>

#include <EmailUserNotification.h>

#include <boost/shared_ptr.hpp> // PRQA S 1013

_ACS_BEGIN_NAMESPACE(acs)


/**
* \brief
* Inventory email notification.
* Notify if the inventoried product (and its filetype) is part of a "file type group".
* Used for example in the coordinated delivery scenario to alert the operator something belonging to 
* defined groups as been ingested.
**/

class InventoryEmailNotification: public EmailUserNotification { // PRQA S 2109
public:

	/** InventoryEmailNotification Exceptions */
	exDECLARE_EXCEPTION(exInventoryEmailNotificationException, exException) ; // Base InventoryEmailNotification Exception. // PRQA S 2131, 2502 2 
	exDECLARE_EXCEPTION(exInventoryEmailNotificationCriticalException, exInventoryEmailNotificationException) ; // Critical Exception. // PRQA S 2153


	/** Default Class constructor */
	explicit InventoryEmailNotification( std::string const& dbKey = "");
	/** Destructor */
	virtual ~InventoryEmailNotification() throw();

	/** Set the inputs for notification */
	InventoryEmailNotification& setInfo( std::string const& filename, std::string const& filetype, std::string const& groupId);


protected:
	
	/** Interface implementation */
	virtual bool setUp();
	/** Interface implementation */
	virtual bool prepare();
	/** Interface implementation */
	virtual void clean();

private:  // copy constructor and operator= defined but not implemented
	/** Copy Constructor */
	InventoryEmailNotification(const InventoryEmailNotification & );
	/** Operator = */
	InventoryEmailNotification &operator=(const InventoryEmailNotification &);

private:

	/** The inventory filename */
	std::string _filename;
	/** The inventory filetype */
	std::string _filetype;
	/** The groupid in t_crgroups */
	std::string _groupId;

	/** Notification Infos taken from DB */
	std::string _groupName;
	std::string _groupDescription; 
	std::string _groupEmailDests;
	std::vector<std::string> _groupFiletypes;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(InventoryEmailNotification) ;

};

/** Shared pointer Types definitions */
typedef boost::shared_ptr<InventoryEmailNotification> InventoryEmailNotificationSptr;
typedef boost::shared_ptr<const InventoryEmailNotification> InventoryEmailNotificationConstSptr;

_ACS_END_NAMESPACE

#endif //_InventoryEmailNotification_H_

