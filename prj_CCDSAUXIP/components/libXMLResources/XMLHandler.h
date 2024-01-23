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
	
	Revision 5.1  2013/07/19 09:01:20  marpas
	doc fixed
	qa warnings fixed
	compilation warnings fixed
	
	Revision 5.0  2013/07/18 17:54:34  marpas
	adopting libException 5.x standards
	performances improved
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.7  2012/12/02 20:03:49  marpas
	qa rules
	
	Revision 2.6  2012/02/15 12:38:21  marpas
	potential memory corruption fixed
	
	Revision 2.5  2012/02/13 16:47:41  marpas
	refactoring
	
	Revision 2.4  2009/09/16 11:14:07  marpas
	XMLValidator used in XMLIstream
	xerces namespace management simplified
	
	Revision 2.3  2009/09/14 12:51:52  marpas
	XMLValidator class added
	unitary test added
	other test improved
	
	Revision 2.2  2008/01/18 18:55:56  clanas
	fixed namespace management for new xerces version
	
	Revision 2.1  2006/07/14 09:23:20  marpas
	documentation fixed and improved.
	global variables are now encaspulated into classes better than before
	interruption of saxExtract method allowed when the last data consumer leave.
	new test to show this feature added
	
	Revision 2.0  2006/02/28 08:37:31  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2004/09/17 20:07:43  marpas
	Data extraction almost complete, beta release
	
	Revision 1.1  2004/09/16 12:15:35  marpas
	architecture redesign
	
	
*/

#ifndef _XMLHandler_H_
#define _XMLHandler_H_

#include <acs_c++config.hpp>
#include <exException.h>
#include <xercesc/sax/HandlerBase.hpp> // PRQA S 1013
#include <string>

#ifndef XERCES_HAS_NAME_SPACE
	#define XERCES_CPP_NAMESPACE
#endif

_ACS_BEGIN_NAMESPACE(acs)


//////////////////////////////////
//								//
//   classe XMLHandler			//
//								//
//////////////////////////////////


class XMLHandler: // PRQA S 2109, 2153
public XERCES_CPP_NAMESPACE::HandlerBase {		

public:

	/*! class XMLHandlerInterrupt declaration */
	exDECLARE_EXCEPTION(XMLHandlerInterrupt,exException) ; // PRQA S 2131, 2502

	/*! class XMLHandlerException declaration */
	exDECLARE_EXCEPTION(XMLHandlerException,exException) ; // PRQA S 2131, 2502


	/*! class XMLParserWarning declaration */
	exDECLARE_EXCEPTION(XMLParserWarning,XMLHandlerException) ; // PRQA S 2131, 2153, 2502


	/*! class XMLParserError declaration */
	exDECLARE_EXCEPTION(XMLParserError,XMLParserWarning) ; // PRQA S 2131, 2153, 2502

	/*! class XMLParserFatalError declaration */
	exDECLARE_EXCEPTION(XMLParserFatalError,XMLParserError) ; // PRQA S 2131, 2153, 2502


	/**
	 * \brief  Class constructor 
	 * 
	 */	
	 XMLHandler();

	/**
	 * \brief  Class destructor 
	 */			
	virtual ~XMLHandler() throw() ;

	/**
	 * \brief  Receive notification of a recoverable parser error. 
	 * 
	 * This method overrides \a ErrorHandler \b error method wrapping 
	 * \a SAXParserException error in \a XMLParserError exception.
	 * \param ex Reference to \a SAXParseException object.
  	 */	
	virtual void error(const XERCES_CPP_NAMESPACE::SAXParseException& ex);

	/**
	 * \brief  Receive notification of a parser fatal error. 
	 * 
	 * This method overrides \a ErrorHandler \b fatalError method wrapping 
	 * \a SAXParserException fatalError in \a XMLParserFatalError exception.
	 * \param ex Reference to \a SAXParseException object.
  	 */			
	virtual void fatalError(const XERCES_CPP_NAMESPACE::SAXParseException& ex);

	/**
	 * \brief  Receive notification of a parser warning. 
	 * 
	 * This method overrides \a ErrorHandler \b warning method wrapping 
	 * \a SAXParserException warning in \a XMLParserWarning exception.
	 * \param ex Reference to \a SAXParseException object.
  	 */			
	virtual void warning(const XERCES_CPP_NAMESPACE::SAXParseException& ex);

	/// This method changes warnings default knowledge.
	void ignoreWarning(bool) ;

	/** 
	 * \brief  This method changes warnings knowledge status.
	 * \return  Warnings knowledge status.
	 */
	bool ignoreWarning() const throw() ;

	/// This method changes errors default knowledge.		
	void ignoreError(bool) ;

	/** 
	 * \brief  This method changes errors knowledge status.
	 * \return  Errors knowledge status.
	 */
	bool ignoreError() const throw() ;

	/**
	 * \brief  This function converts \a XMLCh pointer into \a STL \a std::string. 
	 * 
	 * This is a convenience method .
	 * It manages the memory too.
	 * \param  xmlData pointer to \a XMLCh object.
	 * \param  res the string to be filled, also returned
 	 * \param  dontTrim if true do not trims the resulting string
 	 */		
	static const std::string &toString(const XMLCh* xmlData, std::string &res, bool dontTrim = false) // PRQA S 2502
    {
	    char * tmp = XERCES_CPP_NAMESPACE::XMLString::transcode(xmlData);
	    if (not dontTrim) {
		    XERCES_CPP_NAMESPACE::XMLString::trim(tmp);
        }
	    res = tmp;
#ifdef XERCES_HAS_NAME_SPACE
            XERCES_CPP_NAMESPACE::XMLString::release(&tmp);
#else
            delete[] tmp;
#endif

	    return res;                                            // PRQA S 4028
    }

	/**
	 * \brief  This function converts \a XMLCh pointer into an \a STL \a std::string. 
     * It trims the string too.
	 * 
	 * This is a convenience method.
	 * It manages the memory too.
	 * \param  xmlData pointer to \a XMLCh object.
	 * \param  res the string to be filled, returned
  	 */		
	static const std::string& hpTrimmedString(const XMLCh* xmlData , std::string &res) throw() ;

private:

	// the copy ctor and assignement operator are declared private to prevent their use.
	// They are, hence, not implemented
	XMLHandler(const XMLHandler &); 
	XMLHandler &operator=(const XMLHandler &); 
private:

	bool _ignoreWarning ;
	bool _ignoreError ;

	static const bool _defaultIgnoreWarning = false ;
	static const bool _defaultIgnoreError = false ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(XMLHandler) ;

};

_ACS_END_NAMESPACE


#endif //_XMLHandler_H_

