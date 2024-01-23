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

	$Prod: A.C.S. libTaskTableProcess$

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/06/19 08:41:07  chicas
	*** empty log message ***
	
	Revision 1.1  2013/06/12 10:35:33  nicvac
	S2PDGS-298: Notifications on success.
	
        
*/

#ifndef _TTProcessEmailNotification_H_
#define _TTProcessEmailNotification_H_

#include <acs_c++config.hpp>
#include <exException.h>

#include <EmailUserNotification.h>

#include <boost/shared_ptr.hpp> // PRQA S 1013


_ACS_BEGIN_NAMESPACE(acs)


/**
* \brief
* Email notification for TT Processing.
* Notify the outcome of a processing requested to libTaskTableProcess.
* Used for example by Import.
**/

class TTProcessEmailNotification: public EmailUserNotification { // PRQA S 2109
public:

	/** TTProcessEmailNotification Exceptions */
	exDECLARE_EXCEPTION(exTTProcessEmailNotificationException, exEmailUserNotificationException) ; // Base TTProcessEmailNotification Exception. // PRQA S 2131, 2153, 2502 2
	exDECLARE_EXCEPTION(exTTProcessEmailNotificationCriticalException, exTTProcessEmailNotificationException) ; // Critical Exception.


	/** Default Class constructor */
	TTProcessEmailNotification();
	/** Destructor */
	virtual ~TTProcessEmailNotification() throw();

	/** Set the inputs for notification */
	TTProcessEmailNotification& setInfo( std::string const& filename, std::string const& destAddrs,bool success);
	

protected:
	
	/** Interface implementation */
	virtual bool setUp();
	/** Interface implementation */
	virtual bool prepare();
	/** Interface implementation */
	virtual void clean();
										 

private:  // copy constructor and operator= defined but not implemented
	/** Copy Constructor */
	TTProcessEmailNotification(const TTProcessEmailNotification & );
	/** Operator = */
	TTProcessEmailNotification &operator=(const TTProcessEmailNotification &);

private:

	/** The processed filename */
	std::string _filename;
	/** The destination addresses */
	std::string _destAddrs;
	/** Success / Failure notification */
	bool _success;

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(TTProcessEmailNotification) ;

};

/** Shared pointer Types definitions */
typedef boost::shared_ptr<TTProcessEmailNotification> TTProcessEmailNotificationSptr;
typedef boost::shared_ptr<const TTProcessEmailNotification> TTProcessEmailNotificationConstSptr;

_ACS_END_NAMESPACE

#endif //_TTProcessEmailNotification_H_

