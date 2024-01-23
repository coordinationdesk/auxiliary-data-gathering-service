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
	
	Revision 2.7  2012/02/13 16:47:41  marpas
	refactoring
	
	Revision 2.6  2011/03/01 12:09:33  marpas
	GCC 4.4.x support
	
	Revision 2.5  2009/11/13 11:31:45  marpas
	root node is "/" - fixed
	
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

#include <XMLExtractorHandler.h>
#include <XMLDataConsumer.h>
#include <Filterables.h>


#include <xercesc/sax/AttributeList.hpp> // PRQA S 1013

#include <iostream>
#include <sstream>
#include <algorithm>




_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(XMLExtractorHandler)

// -----------------------------------------------------------------
//
//
// constructor and destructor
//
// -----------------------------------------------------------------
XMLExtractorHandler::XMLExtractorHandler(std::vector<XMLDataConsumer *> &dc) : 
	XMLExtractorHandlerBase(dc, true)
{
}

XMLExtractorHandler::~XMLExtractorHandler() throw() {}

//
// notify start of an XML file element
//
 #if ( XERCES_VERSION_MAJOR > 2 )
	void XMLExtractorHandler::startElement(const XMLCh* const name, XERCES_CPP_NAMESPACE::AttributeList& attributes)
 #else
	void XMLExtractorHandler::startElement(const XMLCh* name, XERCES_CPP_NAMESPACE::AttributeList& attributes)
 #endif
{
	ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
		string t ;
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Found element: " << hpTrimmedString(name,t)) ;
	ACS_CLASS_END_DEBUG
	string en ;
	hpTrimmedString(name,en) ;
	_interestedConsumers.clear() ;
    size_t _dataConsumer_size = _dataConsumer.size();
	for (size_t i=0; i < _dataConsumer_size; ++i) {
		if (_dataConsumer[i]->nodeMatch(*this, _currentNode,en,attributes)) {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "intcons:" << _dataConsumer[i]) ;
			if (find(_interestedConsumers.begin(),_interestedConsumers.end(), _dataConsumer[i]) == _interestedConsumers.end()) {
				_interestedConsumers.push_back(_dataConsumer[i]) ;
            }
		}
	}
	_parentNode = _currentNode ;
	if (_currentNode != "/") {
		_currentNode += "/" ;
    }
	_currentNode += en ;
	_startNewEl = true ;
}

//
// notify end of an XML file element
//
 #if ( XERCES_VERSION_MAJOR > 2 )
	void XMLExtractorHandler::endElement(const XMLCh* const name)
 #else
	void XMLExtractorHandler::endElement(const XMLCh* name)
 #endif
{
	ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)	
		string t ;
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "element: " << hpTrimmedString(name,t)) ;
	ACS_CLASS_END_DEBUG

	string en ;
	hpTrimmedString(name,en) ;
	if (!_startNewEl) {
		_parentNode.erase(_parentNode.rfind(en)-1) ; // PRQA S 3084
	}
	if (!_interestedConsumers.empty()) {
		size_t _interestedConsumers_size = _interestedConsumers.size() ;
        for (size_t i=0; i < _interestedConsumers_size; ++i) {
			_interestedConsumers[i]->nodeValue(*this, _parentNode,en,_currentValue) ;
        }
		_currentValue = "" ;
	}
	_currentNode.erase(_currentNode.rfind(en)-1) ; // PRQA S 3084
	_startNewEl = false ;
}

//
// notify characters data into an XML file element
//
 #if ( XERCES_VERSION_MAJOR > 2 )
void XMLExtractorHandler::characters( const XMLCh* const chars, const XMLSize_t ) 
 #else
void XMLExtractorHandler::characters( const XMLCh* chars, const unsigned int ) 
 #endif
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)	
		string t ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "'" << toString(chars,t,!_currentValue.empty()) << "'") ;
	ACS_CLASS_END_DEBUG
	if (!_interestedConsumers.empty()) {
		string c ;
		_currentValue += toString(chars,c,!_currentValue.empty()) ;
	}
}


//
// receive notification of the beginning of the document
//
void XMLExtractorHandler::startDocument()
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "XML document begins") ;
	_interestedConsumers.clear() ;
	_currentValue = "" ;
	_currentNode = "/" ;
	_startNewEl = false ;
}

//
// receive notification of the end of the document
//
void XMLExtractorHandler::endDocument()
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "XML document ends") ;
	_interestedConsumers.clear() ;
	_currentValue = "" ;
	_startNewEl = false ;
}



_ACS_END_NAMESPACE
