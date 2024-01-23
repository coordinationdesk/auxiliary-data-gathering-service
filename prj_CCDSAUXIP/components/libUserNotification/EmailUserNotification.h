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
	Revision 5.2  2013/06/17 16:31:38  nicvac
	addAttachment added.
	
	Revision 5.1  2013/06/17 13:22:48  marpas
	finalized adoption of libException 5.x standards
	coding best practices applied
	qa rules applied
	
	Revision 5.0  2013/06/13 18:24:45  marpas
	adoption of libException 5.x standards in progress
	
	Revision 1.2  2013/05/24 18:31:27  nicvac
	S2PDGS-308: Inventory notifications implemented.
	
	Revision 1.1.1.1  2013/05/23 13:41:01  nicvac
	Import libUserNotification
	
        
*/

#ifndef _EmailUserNotification_H_
#define _EmailUserNotification_H_

#include <UserNotification.h>

#include <acs_c++config.hpp>
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)



/**
* \brief
* Notification via mail 
**/

class EmailUserNotification: public UserNotification { // PRQA S 2109
public:

	/** EmailUserNotification Exceptions */
	exDECLARE_EXCEPTION(exEmailUserNotificationException, exException) ; // Base EmailUserNotification Exception. // PRQA S 2131, 2502 2
	exDECLARE_EXCEPTION(exEmailUserNotificationCriticalException, exEmailUserNotificationException) ; // Critical Exception. // PRQA S 2153

	/** Class constructor. 
	 * rootNotificationConf: root of configuration space for specialized Notification settings
	 * rootMailNotifConf: root of configuration space for specialized Mail Notification settings */
	explicit EmailUserNotification(std::string const& rootNotificationConf, std::string const& dbkey = "");
	/** Destructor */
	virtual ~EmailUserNotification() throw();
protected:

	/** Interface implementation */
	virtual bool setUp()=0;
	/** Interface implementation */
	virtual bool prepare()=0;
	/** Interface implementation */
    virtual bool send();
	/** Interface implementation */
	virtual void clean()=0;

    const std::string &getBody() const throw() { return _body ; }
    void setBody(const std::string &s) { _body = s ;}
    
    const std::string &getSubject() const throw() { return _subject ; }
    void setSubject(const std::string &s) { _subject = s ;}
    
    const std::string &getDestAddresses() const throw() { return _destAddresses ; }
    void setDestAddresses(const std::string &s) { _destAddresses = s ;}
    
    bool getAttachBody() const throw() { return _attachBody ; }
    void setAttachBody(bool s) throw() { _attachBody = s ;}
    
    const std::string &getContentType() const throw() { return _contentType ; }
    void setContentType(const std::string &s) { _contentType = s ;}

	void addAttachment( const std::string& attachment ) { _attachments.push_back(attachment); }
    
private:  // default, copy constructor and operator= defined but not implemented
	/** Default Constructor */
	EmailUserNotification();
	/** Copy Constructor */
	EmailUserNotification(const EmailUserNotification & );
	/** Operator = */
	EmailUserNotification &operator=(const EmailUserNotification &);
	

private:

	/** Subject */
	std::string _subject;
	/** Body */
	std::string _body;
	/** Destination addresses */
	std::string _destAddresses;

	/** Attachments */
	std::vector<std::string> _attachments;

	/** True: attach the body itself in the mail */
	bool _attachBody;

	/** The content type of the mail. Empty uses default */
	std::string _contentType;

	/** true if mail sent */
	bool _isMailSent;

	/** The root in the conf space to access mail server, port, ... */
	std::string _rootMailNotifConf;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(EmailUserNotification) ;

};

/** Shared pointer Types definitions */
typedef boost::shared_ptr<EmailUserNotification> EmailUserNotificationSptr;
typedef boost::shared_ptr<const EmailUserNotification> EmailUserNotificationConstSptr;


_ACS_END_NAMESPACE

#endif //_EmailUserNotification_H_

