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
	
	Revision 2.5  2012/02/13 16:47:41  marpas
	refactoring
	
	Revision 2.4  2011/03/01 12:09:34  marpas
	GCC 4.4.x support
	
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
	
	Revision 1.1  2005/06/09 00:41:47  marpas
	XML filter functionality added
	
	
	
*/

#include <XMLExtractorHandlerBase.h>
#include <XMLDataConsumer.h>
#include <Filterables.h>


#include <xercesc/sax/AttributeList.hpp> // PRQA S 1013 

#include <iostream>
#include <sstream>
#include <algorithm>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(XMLExtractorHandlerBase)

// -----------------------------------------------------------------
//
//
// constructor and destructor
//
// -----------------------------------------------------------------
XMLExtractorHandlerBase::XMLExtractorHandlerBase(std::vector<XMLDataConsumer *> &dc, bool intNoH) : 
	XMLHandler(),
    _dataConsumer(dc), // PRQA S 2528
    _interestedConsumers(),
    _currentValue(),
    _currentNode(),
    _parentNode(),
	_startNewEl(false),
	_endOfExtraction(false),
	interruptWithNoConsumer(intNoH)
{
}

XMLExtractorHandlerBase::~XMLExtractorHandlerBase() throw() {}


void XMLExtractorHandlerBase::pushConsumer(XMLDataConsumer *dc) 
{
	vector<XMLDataConsumer *> vdc ;
	vdc.push_back(dc) ;
	pushConsumer(vdc) ;
}

void XMLExtractorHandlerBase::pushConsumer(std::vector<XMLDataConsumer *> &vdc) 
{
    size_t vdcsize = vdc.size();
	for (size_t i=0; i < vdcsize; ++i) {
		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
			string t ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "adding:" << vdc[i])  ;
		ACS_CLASS_END_DEBUG
		if (find(_dataConsumer.begin(), _dataConsumer.end(), vdc[i]) == _dataConsumer.end()) {
			_dataConsumer.push_back(vdc[i]) ;
        }
	}
}

void XMLExtractorHandlerBase::popConsumer(XMLDataConsumer *dc)
{
	vector<XMLDataConsumer *> vdc ;
	vdc.push_back(dc) ;
	popConsumer(vdc) ;
}

void XMLExtractorHandlerBase::popConsumer(std::vector<XMLDataConsumer *> &vdc)
{
    size_t vdcsize = vdc.size();
	for (size_t i=0; i < vdcsize; ++i) {
		vector<XMLDataConsumer *>::iterator tbr ;
		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
			string t ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "removing:" << vdc[i])  ;
		ACS_CLASS_END_DEBUG
		if ((tbr = find(_dataConsumer.begin(), _dataConsumer.end(), vdc[i])) != _dataConsumer.end()) {
			_dataConsumer.erase(tbr) ;
        }
		if ((tbr = find(_interestedConsumers.begin(), _interestedConsumers.end(), vdc[i])) != _interestedConsumers.end()) {
			_interestedConsumers.erase(tbr) ;
        }
	}
	
	ACS_COND_THROW(_dataConsumer.empty() && interruptWithNoConsumer, XMLHandlerInterrupt("No more data consumer")) ; // PRQA S 3081
}


 #if ( XERCES_VERSION_MAJOR > 2 )
void XMLExtractorHandlerBase::formatBuf(const XMLCh* , const XMLSize_t ) 
 #else
void XMLExtractorHandlerBase::formatBuf(const XMLCh* , const unsigned int ) 
 #endif
{
}


void XMLExtractorHandlerBase::formatBuf(const std::string &) 
{
}

void XMLExtractorHandlerBase::writeStart(const XMLCh* , const XERCES_CPP_NAMESPACE::AttributeList& )
{
}


void XMLExtractorHandlerBase::writeEnd(const XMLCh* )
{
}

void XMLExtractorHandlerBase::endOfExtraction() 
{
	_endOfExtraction = true ;
}


_ACS_END_NAMESPACE
