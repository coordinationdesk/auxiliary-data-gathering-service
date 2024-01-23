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
	
	Revision 2.6  2012/12/02 20:03:49  marpas
	qa rules
	
	Revision 2.5  2012/02/13 16:47:41  marpas
	refactoring
	
	Revision 2.4  2009/09/16 11:14:07  marpas
	XMLValidator used in XMLIstream
	xerces namespace management simplified
	
	Revision 2.3  2009/09/10 14:55:24  marpas
	Xercers 3.0.x i/f compatibilty
	
	Revision 2.2  2008/01/18 18:55:56  clanas
	fixed namespace management for new xerces version
	
	Revision 2.1  2006/07/14 09:23:20  marpas
	documentation fixed and improved.
	global variables are now encaspulated into classes better than before
	interruption of saxExtract method allowed when the last data consumer leave.
	new test to show this feature added
	
	Revision 2.0  2006/02/28 08:37:31  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1  2005/06/09 00:41:47  marpas
	XML filter functionality added
	
	
	
*/

#ifndef _XMLExtractorHandlerBase_H_
#define _XMLExtractorHandlerBase_H_

#include <acs_c++config.hpp>
#include <XMLHandler.h>

_ACS_BEGIN_NAMESPACE(acs)

class XMLDataConsumer ;

class rsResourceSet ;
	
class XMLExtractorHandlerBase:  // PRQA S 2108, 2109, 2153, 2195
	public XMLHandler {


protected:
	/**
	 * \brief  Class constructor 
	 * 
	 * \param dc is a vector of data consumers see XMLDataConsumer class.   
	 * \param intNoH will interrupt XMLIstream::saxExtract method when no more consumers left.   
	 */	
	 XMLExtractorHandlerBase(std::vector<XMLDataConsumer *> &dc, bool intNoH);
		
public:
	/**
	 * \brief  Class destructor 
	 */			
	virtual ~XMLExtractorHandlerBase() throw() ;


	/// Receive notification of the end of an XML document.  		
	virtual void endDocument() = 0 ;

	/**
	 * \brief  Receive notification of the end of an XML document element. 
	 * 
	 * This method overrides \a HandlerBase \b endElement method wrapping 
	 * current element properties in the target \a rsResourceSet.
	 * \param el Pointer to \a XMLCh object which defines element name ended.   
	 */		
 #if ( XERCES_VERSION_MAJOR > 2 )
	virtual void endElement( const XMLCh* const el) = 0 ;
 #else
	virtual void endElement( const XMLCh* el) = 0 ;
 #endif

	/**
	 * \brief  Receive notification of the current element character data. 
	 * 
	 * This method overrides \a HandlerBase \b characters method wrapping 
	 * current element value in the target \a rsResourceSet.
	 * \param el Pointer to \a XMLCh object which defines current element value.
	 * \param len Integer value which defines the number of characters to read from the array.   
	 */			
 #if ( XERCES_VERSION_MAJOR > 2 )
	virtual void characters( const XMLCh* const el, const XMLSize_t len) = 0 ;
 #else
	virtual void characters( const XMLCh* el, const unsigned int len) = 0 ;
 #endif

	/**
	 * \brief  Receive notification of the beginning of an element. 
	 * 
	 * This method overrides \a HandlerBase \b startElement method wrapping 
	 * current element name and its attributes list in the target \a rsResourceSet.
	 * \param el Pointer to \a XMLCh object which defines current element name.
	 * \param att Reference to \a AttributeList object which contains list of element attributes.   
	 */			
	
 #if ( XERCES_VERSION_MAJOR > 2 )
	virtual void startElement( const XMLCh* const el, XERCES_CPP_NAMESPACE::AttributeList& att) = 0 ;
 #else
	virtual void startElement( const XMLCh* el, XERCES_CPP_NAMESPACE::AttributeList& att) = 0 ;
 #endif


	/// Receive notification of the beginning of an XML document.
	virtual void startDocument() = 0 ;

	void pushConsumer(XMLDataConsumer *) ;
	void pushConsumer(std::vector<XMLDataConsumer *> &) ;
	void popConsumer(XMLDataConsumer *) ;
	void popConsumer(std::vector<XMLDataConsumer *> &) ;
	
	virtual void writeStart( const XMLCh* name, const XERCES_CPP_NAMESPACE::AttributeList& attributes) ;

	virtual void writeEnd( const XMLCh* name) ;
 #if ( XERCES_VERSION_MAJOR > 2 )
	virtual void formatBuf(const XMLCh* chars, const XMLSize_t lenght) ;
 #else
	virtual void formatBuf(const XMLCh* chars, const unsigned int lenght) ;
 #endif
	virtual void formatBuf(const std::string & chars) ;
	void endOfExtraction() ;
private:
	// the default cotr copy ctor and assignement operator are declared 
    // private to prevent their use.
	// They are, hence, not implemented
	XMLExtractorHandlerBase(); 
	XMLExtractorHandlerBase(const XMLExtractorHandlerBase &); 
	XMLExtractorHandlerBase &operator=(const XMLExtractorHandlerBase &); 
		
protected:
    // PRQA S 2101 8
	std::vector<XMLDataConsumer *> &_dataConsumer ;
	std::vector<XMLDataConsumer *> _interestedConsumers ;
	std::string _currentValue ;
	std::string _currentNode ;
	std::string _parentNode ;
	bool _startNewEl ;
	bool _endOfExtraction ;
private:
	bool interruptWithNoConsumer ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(XMLExtractorHandlerBase) ;
		
};

_ACS_END_NAMESPACE




#endif //_XMLExtractorHandlerBase_H_

