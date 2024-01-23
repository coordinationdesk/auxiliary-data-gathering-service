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
	Revision 5.1  2013/07/07 19:16:10  marpas
	k table can be downloaded in a temporary filename
	adoption of libException 5.x standards
	qa and compilation warnings removed
	
	Revision 5.0  2013/06/19 08:41:07  chicas
	*** empty log message ***
	
	Revision 1.2  2013/06/17 16:43:27  nicvac
	New email interface implemented.
	
	Revision 1.1  2013/06/12 10:35:33  nicvac
	S2PDGS-298: Notifications on success.
	
        
*/ 

#include <TTProcessEmailNotification.h>

#include <ConfigurationSingleton.h>

#include <XMLOstream.h>
#include <XMLTransformer.h>

#include <DateTime.h>

#include <Filterables.h>

#include <boost/algorithm/string.hpp> // PRQA S 1013

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(TTProcessEmailNotification)



TTProcessEmailNotification::TTProcessEmailNotification():
	EmailUserNotification("TTProcessEmailNotification"),
	_filename(""),
	_destAddrs(""),
	_success(true)
{
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Id: "<< Id) ;
}

TTProcessEmailNotification::~TTProcessEmailNotification() throw() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ; // PRQA S 4631
}


TTProcessEmailNotification& TTProcessEmailNotification::setInfo( std::string const& filename, 
																 std::string const& destAddrs, 
																 bool success) {
	_filename = filename;
	_destAddrs = destAddrs;
	_success = success;

	return *this;
}

bool TTProcessEmailNotification::setUp() {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

	ACS_COND_THROW( // PRQA S 3081
		_filename.empty() || _destAddrs.empty(),
		exTTProcessEmailNotificationCriticalException("No input configured for Notification") );

	return true;
}

bool TTProcessEmailNotification::prepare() {

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Preparing mail for Notification ["<<this->getDescription()<<"]") ; 

	//Ref to config
	rsResourceSet& conf = ConfigurationSingleton::instance()->get();

	//Format the subject from template
	string subjectTemplate("");
	if ( _success ) {
		conf.getValue( "TTProcessEmailNotification.SubjectTemplate", subjectTemplate );
	} else {
		conf.getValue( "TTProcessEmailNotification.SubjectErrorTemplate", subjectTemplate );
	}

	boost::replace_all( subjectTemplate, "${filename}", _filename );

	this->setSubject( subjectTemplate );
	
	//Format the body
	{
		ostringstream os;
		{
			XMLOstream xos(os, "MailNotification");
			{
				string utcs; DateTime currentTime; currentTime.utcs(utcs);
				XMLOstream::Tag tag(xos, "UtcsTime",   utcs,       "", true);
			}
			{	XMLOstream::Tag tag(xos, "Filename",   _filename,  "", true);	 }
			{	XMLOstream::Tag tag(xos, "Success",    (_success)?"true":"false",   "", true);	 } // PRQA S 3380
			{	XMLOstream::Tag tag(xos, "EmailDests", _destAddrs, "", true);	 }
		}

		string mailFormat ;
		conf.getValue("TTProcessEmailNotification.MailFormat", mailFormat);

		string mailFormatXslFile ;
		conf.getValue("TTProcessEmailNotification.MailFormatXSL."+mailFormat, mailFormatXslFile);

		//If xsl not defined, the text is kept as the original xml
		if ( mailFormatXslFile != "" ) {
			XMLTransformer xmlTransformer;
			string formattedBody ;
			xmlTransformer.string2string( string(base::defaultConfPath()) +"/"+ mailFormatXslFile, os.str(), formattedBody); // PRQA S 3081

			os.str(formattedBody);
		}

		//Set the actual body
		this->setBody( os.str() );

	}

	//*/ Set remaining fields on super class
	this->setDestAddresses( _destAddrs );

	string conf_mailFormat ; 
	bool conf_attachBody = false;
	string conf_contentType ;
	conf.getValue("TTProcessEmailNotification.MailFormat.attachBody.value", conf_attachBody); this->setAttachBody(conf_attachBody);
	conf.getValue("TTProcessEmailNotification.MailFormat", conf_mailFormat);
	conf.getValue("TTProcessEmailNotification.MailFormatXSL."+conf_mailFormat+".contentType.value", conf_contentType); this->setContentType(conf_contentType);
	//*/

	return true;
}

void TTProcessEmailNotification::clean() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;

	return;
}

_ACS_END_NAMESPACE

