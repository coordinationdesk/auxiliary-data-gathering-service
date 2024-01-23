// PRQA S 1050 EOF
/*

	Copyright 1995-2014, Advanced Computer Systems , Inc.
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
	Revision 5.2  2014/05/26 16:37:40  marfav
	Thread safety improved
	Destructors performs try-catch and notify
	hpTrimmedString robustness improved
	
	Revision 5.1  2014/02/13 12:42:29  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.0  2013/07/18 17:54:34  marpas
	adopting libException 5.x standards
	performances improved
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.4  2012/02/13 16:47:41  marpas
	refactoring
	
	Revision 2.3  2009/09/16 11:14:07  marpas
	XMLValidator used in XMLIstream
	xerces namespace management simplified
	
	Revision 2.2  2008/01/18 18:55:56  clanas
	fixed namespace management for new xerces version
	
	Revision 2.1  2006/07/14 09:23:20  marpas
	documentation fixed and improved.
	global variables are now encaspulated into classes better than before
	interruption of saxExtract method allowed when the last data consumer leave.
	new test to show this feature added
	
	Revision 2.0  2006/02/28 08:37:31  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1  2004/09/16 12:15:35  marpas
	architecture redesign
	
	
*/

#include <XMLHandler.h>
#include <Filterables.h>


#include <iostream>
#include <sstream>


_ACS_BEGIN_NAMESPACE(acs)

namespace {
void releaseXMLString (char* item) ACS_NOEXCEPT
{
	if (item != 0)
	{
#ifdef XERCES_HAS_NAME_SPACE
		XERCES_CPP_NAMESPACE::XMLString::release(&item);
#else
		delete[] item;
#endif
	}
}
}

using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(XMLHandler)

// -----------------------------------------------------------------
//
//
// constructor and destructor
//
// -----------------------------------------------------------------
XMLHandler::XMLHandler() : 
    XERCES_CPP_NAMESPACE::HandlerBase(),
	_ignoreWarning(_defaultIgnoreWarning),
	_ignoreError(_defaultIgnoreError)
{
}

XMLHandler::~XMLHandler() throw() {}

//
// receive notification of a recoverable parser error
//
void XMLHandler::error(const XERCES_CPP_NAMESPACE::SAXParseException& ex) // PRQA S 4020
{
	if (_ignoreError) {
		return ;
    }
		
	ostringstream os ;
	os << " at line: " << ex.getLineNumber() << ", column: " << ex.getColumnNumber() ; 
	string t ;
	ACS_THROW(XMLParserError(hpTrimmedString(ex.getMessage(),t) + os.str() ));	 // PRQA S 3081	
}

//
// report a fatal XML parsing error
//
void XMLHandler::fatalError(const XERCES_CPP_NAMESPACE::SAXParseException& ex)
{
	ostringstream os ;
	os << " at line: " << ex.getLineNumber() << ", column: " << ex.getColumnNumber() ; 
	string t ;
	XMLParserFatalError e(hpTrimmedString(ex.getMessage(),t) + os.str() ) ;
	ACS_THROW(e);
}

//
// receive notification of a parser warning
//
void XMLHandler::warning(const XERCES_CPP_NAMESPACE::SAXParseException& ex) // PRQA S 4020
{
	if (_ignoreWarning) {
		return ;
    }
		
	ostringstream os ;
	os << " at line: " << ex.getLineNumber() << ", column: " << ex.getColumnNumber() ; 
	string t ;
	XMLParserWarning e(hpTrimmedString(ex.getMessage(),t) + os.str() ) ;
	ACS_THROW(e);	
}



//
// changes warnings default knowledge
//
void XMLHandler::ignoreWarning(bool v) // PRQA S 4121
{
	_ignoreWarning = v ;
}

//
// return current warnings knowledge
//
bool XMLHandler::ignoreWarning() const throw() // PRQA S 4120
{
	return _ignoreWarning ;
}

//
// changes errors default knowledge
//
void XMLHandler::ignoreError(bool v)  // PRQA S 4121
{
	_ignoreError = v ;
}

//
// return current errors knowledge
//
bool XMLHandler::ignoreError() const throw() // PRQA S 4120
{
	return _ignoreError ;
}


const string& XMLHandler::hpTrimmedString(const XMLCh* xmlData , std::string &res) throw()
{
    char * tmp = 0;
    try {
	    tmp = XERCES_CPP_NAMESPACE::XMLString::transcode(xmlData);
	    XERCES_CPP_NAMESPACE::XMLString::trim(tmp);

	    res = tmp ;
    }
    catch (const XERCES_CPP_NAMESPACE::XMLException& x)
    {
		char* msg = 0;
		char* type = 0;
		try
		{
			msg = XERCES_CPP_NAMESPACE::XMLString::transcode(x.getMessage());
			type = XERCES_CPP_NAMESPACE::XMLString::transcode(x.getType());
			ACS_LOG_ERROR("XMLException caugth: \"" << exDebugSignatureThread << "\": " << type << ", " << msg ) ;
		}
		catch (const XERCES_CPP_NAMESPACE::XMLException& y)
		{
			ACS_LOG_ERROR("XMLException caugth: \"" << exDebugSignatureThread << "\": unable to decode XERCES exception message") ;
		}
		releaseXMLString (msg);
		releaseXMLString (type);
    }    
    catch(std::exception & x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }
	releaseXMLString (tmp);
	return res ; // PRQA S 4028
}



_ACS_END_NAMESPACE
