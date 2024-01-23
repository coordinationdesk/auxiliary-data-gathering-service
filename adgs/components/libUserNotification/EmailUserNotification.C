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
	Revision 5.3  2016/05/10 14:34:46  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 5.2  2013/06/17 18:53:08  marpas
	qa rules
	
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

#include <EmailUserNotification.h>

#include <ConfigurationSingleton.h>
#include <SendMail.h>
#include <File.h>
#include <Timer.h>

#include <Filterables.h>

#include <algorithm>
#include <sstream>
#include <iterator>

#include <boost/tokenizer.hpp> // PRQA S 1013 2 
#include <boost/algorithm/string/trim.hpp>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(EmailUserNotification)



EmailUserNotification::EmailUserNotification(string const& rootNotificationConf, string const& dbKey): 
	UserNotification(rootNotificationConf, dbKey),
	_subject(),
	_body(),
	_destAddresses(),
	_attachments(),
	_attachBody(false),
	_contentType(),
	_isMailSent(false),
	_rootMailNotifConf()
{
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Id: "<< Id) ;

	//Get the key for configuring mail server, port, ...
	ConfigurationSingleton::instance()->get().getValue( rootNotificationConf+".MailRootKeyConf", _rootMailNotifConf );
}

EmailUserNotification::~EmailUserNotification() throw() {
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ; // PRQA S 4631

}


bool EmailUserNotification::send() {

	//Based on the original dcSendMail::sendMail

	ACS_LOG_INFO("Sending mail for notification ["<<getDescription()<<"]") ;

	rsResourceSet& conf = ConfigurationSingleton::instance()->get();

	// repeat in case of communication errors
	unsigned int repeatCounter(0);
	try {
		// Read configuration - From mail settings section
		string conf_server                      (""); conf.getValue(_rootMailNotifConf+".server"                      , conf_server                     );
		int    conf_port                         (0); conf.getValue(_rootMailNotifConf+".port"                        , conf_port                       );
		string conf_login                       (""); conf.getValue(_rootMailNotifConf+".login"                       , conf_login                      );
		string conf_password                    (""); conf.getValue(_rootMailNotifConf+".password"                    , conf_password                   );
		string conf_mailSender                  (""); conf.getValue(_rootMailNotifConf+".mailSender"                  , conf_mailSender                 );
		string conf_forceMailDestinationAddress (""); conf.getValue(_rootMailNotifConf+".ForceMailDestinationAddress" , conf_forceMailDestinationAddress);
		bool   conf_setForceMailAddress      (false); conf.getValue(_rootMailNotifConf+".SetForceMailAddress"         , conf_setForceMailAddress        );
		u_int  conf_elapsedTimeToRetry           (0); conf.getValue(_rootMailNotifConf+".elapsedTimeToRetry"          , conf_elapsedTimeToRetry         );
		u_int  conf_maxNumberOfRetries           (0); conf.getValue(_rootMailNotifConf+".maxNumberOfRetries"          , conf_maxNumberOfRetries         );
    
		//Override destinations if requested by config
		if ( conf_setForceMailAddress ) {
			_destAddresses = conf_forceMailDestinationAddress;
		}

		//Attach the body itself if required by configuration
		FileGuard bodyFilename( File::tempname("/tmp/MailBody-Attachment-") ); // file self-deletion
		if ( _attachBody ) {
			string bodyFilenameStr = bodyFilename; // PRQA S 3050
			ofstream bodyFile(bodyFilenameStr.c_str());
			bodyFile << _body;
			bodyFile.close(); 
			_attachments.push_back(bodyFilenameStr); 
		}
		

		//Send the mail
		_isMailSent = false;
		while ( !_isMailSent &&
				(repeatCounter < conf_maxNumberOfRetries) && 
				(this->getHaltLevel() == StopController::NONE) ) {

			{   ostringstream msg; msg << 
				  " - attempt #: "<<	repeatCounter<<" / "<<conf_maxNumberOfRetries<<"\n"<<
				  " - mailto: "<<		_destAddresses<<"\n"<<
				  " - subject: " <<  	_subject<<"\n"<<
				  " - attachment: ";	std::copy(_attachments.begin(), _attachments.end(), std::ostream_iterator<string>(msg, " "));
				ACS_LOG_INFO(msg.str()) ;	
            }

			//autodelete pointer - auto_ptr will be deprecated soon...
			boost::shared_ptr <SendMail> sendMailSptr(
				( conf_login.empty() ? // PRQA S 3380, 3385
					new SendMail( conf_server, conf_mailSender, conf_port ) : // PRQA S 3010 2
					new SendMail( conf_server, conf_login, conf_password, conf_mailSender, conf_port )
				)
			);

			//Set "To"
			//Tokenize on "," and trim
			boost::tokenizer< boost::escaped_list_separator<char> > tok(_destAddresses);
			for(boost::tokenizer< boost::escaped_list_separator<char> >::iterator it=tok.begin(); it!=tok.end(); ++it){
				sendMailSptr->addToEntry( boost::algorithm::trim_copy( *it ) );
			}

			//Set Subject and Body
			sendMailSptr->setSubjectAndBody( _subject, _body );

			//Attachments
            
			for ( vector<string>::const_iterator it=_attachments.begin(); it!=_attachments.end(); ++it ) { // PRQA S 4238
				sendMailSptr->addAttachment( *it );
			}

			//Set the content type in the mail header
			if ( !_contentType.empty() ) {
				sendMailSptr->setTextContentType( _contentType );
			}

			//Send
			_isMailSent = sendMailSptr->send();

			ACS_LOG_INFO("Sending mail for notification ["<<getDescription() <<"]: "<<((_isMailSent)?"Ok":"Not ok")) ; // PRQA 3380 // PRQA S 3380

			if ( !_isMailSent ) {
				//Try again
				Timer::delay( conf_elapsedTimeToRetry * 1000 ); // PRQA S 3084, 4400
			}

			++repeatCounter;
		}

		if ( !_isMailSent ) {

			ostringstream msg; msg<<
				"Cannot send Email Notification to: "<<_destAddresses<<" for notification ["<<getDescription()<<"]\n"<<
				"Email Server Settings: ["<<conf_server<<", "<<conf_login<<", "<<conf_password<<", "<<conf_mailSender<<
				", "<<conf_port<<"]" ;

			if ( this->getHaltLevel() != StopController::NONE ) {
				msg<<" - Received CANCEL request.";
			}
			ACS_LOG_WARNING(msg.str()) ;
		}


	} catch( exception& ex ) {
		ACS_THROW( exEmailUserNotificationCriticalException(ex, "Error sending mail for notification [" + getDescription() + "]") );
	}

	return _isMailSent;

}



_ACS_END_NAMESPACE
