//  PRQA S 1050 EOF
/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
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
	
	Revision 2.5  2012/12/02 20:03:49  marpas
	qa rules
	
	Revision 2.4  2012/02/13 16:47:41  marpas
	refactoring
	
	Revision 2.3  2009/09/16 11:14:07  marpas
	XMLValidator used in XMLIstream
	xerces namespace management simplified
	
	Revision 2.2  2009/09/10 14:55:24  marpas
	Xercers 3.0.x i/f compatibilty
	
	Revision 2.1  2008/01/18 18:55:56  clanas
	fixed namespace management for new xerces version
	
	Revision 2.0  2006/02/28 08:37:31  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1  2005/06/09 00:41:47  marpas
	XML filter functionality added
	
	
	
*/

#ifndef _XMLFilterHandler_H_
#define _XMLFilterHandler_H_

#include <acs_c++config.hpp>
#include <XMLExtractorHandlerBase.h>
#include <xercesc/framework/XMLFormatter.hpp> // PRQA S 1013
#include <fstream>
#include <sstream>


_ACS_BEGIN_NAMESPACE(acs)


class XMLDataConsumer ;

class rsResourceSet ;


class XMLFilterHandler: // PRQA S 2109, 2193
	public XMLExtractorHandlerBase, 
    private XERCES_CPP_NAMESPACE::XMLFormatTarget {

	

public:


	/**
	 * \brief  Class constructor 
	 * 
	 */	
	 XMLFilterHandler(std::vector<XMLDataConsumer *> &, 
	 				  const std::string &path, 
					  const std::string &encodingName,
					  XERCES_CPP_NAMESPACE::XMLFormatter::UnRepFlags unRepFlags) ;

	
	/**
	 * \brief  Class destructor 
	 */			
	virtual ~XMLFilterHandler() throw() ;


	/// Receive notification of the end of an XML document.  		
	virtual void endDocument();

	/**
	 * \brief  Receive notification of the end of an XML document element. 
	 * 
	 * This method overrides \a HandlerBase \b endElement method wrapping 
	 * current element properties in the target \a rsResourceSet.
	 * \param name Pointer to \a XMLCh object which defines element name ended.   
	 */		
 #if ( XERCES_VERSION_MAJOR > 2 )
	virtual void endElement( const XMLCh* const name);
 #else
	virtual void endElement( const XMLCh* name);
 #endif

	/**
	 * \brief  Receive notification of the current element character data. 
	 * 
	 * This method overrides \a HandlerBase \b characters method wrapping 
	 * current element value in the target \a rsResourceSet.
	 * \param chars Pointer to \a XMLCh object which defines current element value.
	 * \param lenght Integer value which defines the number of characters to read from the array.   
	 */			
 #if ( XERCES_VERSION_MAJOR > 2 )
	virtual void characters( const XMLCh* const chars, const XMLSize_t lenght) ;
 #else
	virtual void characters( const XMLCh* chars, const unsigned int lenght) ;
 #endif

	/**
	 * \brief  Receive notification of the beginning of an element. 
	 * 
	 * This method overrides \a HandlerBase \b startElement method wrapping 
	 * current element name and its attributes list in the target \a rsResourceSet.
	 * \param name Pointer to \a XMLCh object which defines current element name.
	 * \param attributes Reference to \a AttributeList object which contains list of element attributes.   
	 */			
	
 #if ( XERCES_VERSION_MAJOR > 2 )
	virtual void startElement( const XMLCh * const name, XERCES_CPP_NAMESPACE::AttributeList& attributes);
 #else
	virtual void startElement( const XMLCh* name, XERCES_CPP_NAMESPACE::AttributeList& attributes);
 #endif


	/// Receive notification of the beginning of an XML document.
	virtual void startDocument();


	void writeChars (const XMLByte* const toWrite);

 #if ( XERCES_VERSION_MAJOR > 2 )
	virtual void writeChars (const XMLByte* toWrite, XMLSize_t count, XERCES_CPP_NAMESPACE::XMLFormatter* formatter);
 #else
	virtual void writeChars (const XMLByte* const toWrite, const unsigned int count, XERCES_CPP_NAMESPACE::XMLFormatter* const formatter);
 #endif
	virtual void ignorableWhitespace(const XMLCh* const chars, XMLSize_t) ;
	virtual void processingInstruction(const XMLCh* const target, const XMLCh* const data) ;
	
	virtual void writeStart( const XMLCh* name, const XERCES_CPP_NAMESPACE::AttributeList& attributes);

	virtual void writeEnd( const XMLCh* name) ;
 #if ( XERCES_VERSION_MAJOR > 2 )
	virtual void formatBuf(const XMLCh* chars, const XMLSize_t lenght) ;
 #else
	virtual void formatBuf(const XMLCh* chars, const unsigned int lenght) ;
 #endif
	virtual void formatBuf(const std::string &) ;

private:
	void writeTmpOut() ;

	// the default ctor, copy ctor and assignement operator are declared 
    // private to prevent their use.
	// They are, hence, not implemented
	XMLFilterHandler(); 
	XMLFilterHandler(const XMLFilterHandler &); 
	XMLFilterHandler &operator=(const XMLFilterHandler &); 
		
private:
	XERCES_CPP_NAMESPACE::XMLFormatter    fFormatter;
	std::ofstream	_out ;
	std::ostringstream	*_tmpout ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(XMLFilterHandler) ;
		
};

_ACS_END_NAMESPACE


#endif //_XMLFilterHandler_H_

