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

	$Prod: A.C.S. XMLResources Library $

	$Id$

	$Author$

	$Log$
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
	
	Revision 1.4  2005/06/09 00:41:47  marpas
	XML filter functionality added
	
	Revision 1.3  2004/09/17 20:07:43  marpas
	Data extraction almost complete, beta release
	
	Revision 1.2  2004/09/16 16:58:18  marpas
	Data consumer concept implemented and almost tested
	work still in progress
	
	Revision 1.1  2004/09/16 12:27:21  marpas
	new class added
	
	
*/

#ifndef _XMLExtractorHandler_H_
#define _XMLExtractorHandler_H_

#include <acs_c++config.hpp>
#include <XMLExtractorHandlerBase.h>

_ACS_BEGIN_NAMESPACE(acs)


class XMLDataConsumer ;

class rsResourceSet ;



class XMLExtractorHandler: // PRQA S 2109
	public XMLExtractorHandlerBase {

public:


	/**
	 * \brief  Class constructor 
	 * 
	 */	
	explicit XMLExtractorHandler(std::vector<XMLDataConsumer *> &);

	/**
	 * \brief  Class destructor 
	 */			
	virtual ~XMLExtractorHandler() throw() ;


	/// Receive notification of the end of an XML document.  		
	virtual void	endDocument();

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
	 * \param len Integer value which defines the number of characters to read from the array.   
	 */			
 #if ( XERCES_VERSION_MAJOR > 2 )
	virtual void characters( const XMLCh* const chars, const XMLSize_t len) ;
 #else
	virtual void characters( const XMLCh* chars, const unsigned int len) ;
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

private:


	// the default ctor, copy ctor and assignement operator are declared 
    // private to prevent their use.
	// They are, hence, not implemented
	XMLExtractorHandler(); 
	XMLExtractorHandler(const XMLExtractorHandler &); 
	XMLExtractorHandler &operator=(const XMLExtractorHandler &); 
		
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(XMLExtractorHandler) ;
};


_ACS_END_NAMESPACE


#endif //_XMLExtractorHandler_H_

