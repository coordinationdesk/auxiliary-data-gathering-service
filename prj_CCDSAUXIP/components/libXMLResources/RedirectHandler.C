// PRQA S 1050 EOF
/*
    Copyright 2002-2013, Advanced Computer Systems , Inc.
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.acsys.it

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

    $Prod: A.C.S. XMLResources Library $

    $Id$

    $Author$

    $Log$
    Revision 5.1  2013/07/19 07:51:30  lucio.pulvirenti
    Fixed: Filterables.h include added.

    Revision 5.0  2013/07/18 17:54:33  marpas
    adopting libException 5.x standards
    performances improved
    coding best practices applied
    qa warnings removed
    compilation warnings removed

    Revision 2.9  2012/02/13 16:47:41  marpas
    refactoring

    Revision 2.8  2009/09/16 11:14:07  marpas
    XMLValidator used in XMLIstream
    xerces namespace management simplified

    Revision 2.7  2009/09/14 12:51:52  marpas
    XMLValidator class added
    unitary test added
    other test improved

    Revision 2.6  2009/07/29 15:35:23  marpas
    debug & robustness improved

    Revision 2.5  2009/05/28 12:08:12  marpas
    debug improved

    Revision 2.4  2009/03/02 10:28:55  marpas
    minor changes for CODECHEK

    Revision 2.3  2008/01/18 18:55:55  clanas
    fixed namespace management for new xerces version

    Revision 2.2  2006/07/14 09:23:20  marpas
    documentation fixed and improved.
    global variables are now encaspulated into classes better than before
    interruption of saxExtract method allowed when the last data consumer leave.
    new test to show this feature added

    Revision 2.1  2006/03/17 16:14:21  danalt
    added RedirectHandler


*/


#include <ConfigurationSingleton.h>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <File.h>
#include <RedirectHandler.h>
#include <exStream.h>
#include <Filterables.h>

#include <xercesc/framework/LocalFileInputSource.hpp> // PRQA S 1013 

#include <string>
#include <iostream>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std;
#ifdef XERCES_HAS_NAME_SPACE
using namespace XERCES_CPP_NAMESPACE;
#endif

const string RedirectHandler::_nameSpace = "XML_DTDConfiguration";
const string RedirectHandler::_enableKey = "EnableDTDRedirect";
const string RedirectHandler::_pathKey	= "RedirectPath";

ACS_CLASS_DEFINE_DEBUG_LEVEL(RedirectHandler)

// ---------------------------------------------------------------------------
//  RedirectHandler: Constructors and Destructor
// ---------------------------------------------------------------------------
RedirectHandler::RedirectHandler() :
    XERCES_CPP_NAMESPACE::HandlerBase()
{
}

RedirectHandler::~RedirectHandler() throw() {}

void RedirectHandler::getValues(std::string & path, bool &enabled) 
{
	try
	{
		rsResourceSet& rs = ConfigurationSingleton::instance()->get();
		rsPushNSpace pushedSpace (rs, _nameSpace);
		rs.getValue (_enableKey, enabled);	
		rs.getValue (_pathKey,   path);	
	}
	catch (exception& ex)
	{
		ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
			ACS_LOG_NOTIFY_EX(ex) ;
		ACS_CLASS_END_DEBUG	
		enabled = false;
	}
}


// -----------------------------------------------------------------------
//  Handlers for the SAX EntityResolver interface
// -----------------------------------------------------------------------
InputSource* RedirectHandler::resolveEntity(const XMLCh* const publicId, // PRQA S 4020
                                            const XMLCh* const systemId)
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,  // PRQA S 3380, 3385
                "publicId: \"" << (publicId ? XMLString::transcode(publicId): "<null>") << "\"" // PRQA S 3380, 3385
			 << " systemId: \"" << (systemId ? XMLString::transcode(systemId): "<null>") << "\"") ;
	bool enabled = false;
	string redirectPath ;

	try
	{
		getValues(redirectPath,enabled) ;
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB," " <<  _enableKey << ": " << boolalpha << enabled
				   << " " << _pathKey << ": " << redirectPath) ;
	}
	catch (exception& ex)
	{
		ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
			ACS_LOG_NOTIFY_EX(ex) ;
		ACS_CLASS_END_DEBUG	
		enabled = false;
	}

	if (enabled)
	{
		try
		{
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"checking existence of " <<  XMLString::transcode(systemId)) ;
			if (!File::exists(XMLString::transcode(systemId)))
			{
				string tmp;
				// Check redirectPath + systemId 
				tmp = redirectPath + string(XMLString::transcode(systemId)); // PRQA S 3081
				ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"trying with " << tmp) ;
				if (File::exists(tmp)) 
				{
					XMLCh* const dtdPath = XMLString::transcode(tmp.c_str());
					return new LocalFileInputSource(dtdPath);
				}

				// Check redirectPath + systemId FileName 
				tmp = redirectPath + File::getFileName(string(XMLString::transcode(systemId))); // PRQA S 3081
				ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"filename of " << XMLString::transcode(systemId) // PRQA S 3081
						   << " " << File::getFileName(string(XMLString::transcode(systemId)))) ; // PRQA S 3081
				ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"trying with " << tmp) ;
				if (File::exists(tmp))
				{
					XMLCh* const dtdPath = XMLString::transcode(tmp.c_str());
					return new LocalFileInputSource(dtdPath);
				}
			}
		}
		catch(exception& ex)
		{
		}
	}

	return 0;

}

const std::string & RedirectHandler::getNameSpace() throw() 
{
	return _nameSpace; 
}

_ACS_END_NAMESPACE
