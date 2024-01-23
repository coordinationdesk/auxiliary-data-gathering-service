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
	Revision 5.1  2013/12/05 17:10:23  lucio.pulvirenti
	MP: coding best practices applied.
	
	Revision 5.0  2013/07/18 17:54:34  marpas
	adopting libException 5.x standards
	performances improved
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.5  2013/01/28 13:27:09  marpas
	XMLProgDataConsumer class added
	unitary test added
	
	Revision 2.4  2012/02/13 16:47:41  marpas
	refactoring
	
	Revision 2.3  2009/11/13 10:13:54  marpas
	debug message fixed
	
	Revision 2.2  2009/09/16 11:14:07  marpas
	XMLValidator used in XMLIstream
	xerces namespace management simplified
	
	Revision 2.1  2008/01/18 18:55:55  clanas
	fixed namespace management for new xerces version
	
	Revision 2.0  2006/02/28 08:37:31  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.4  2005/08/08 12:00:36  marpas
	charaters interface changed
	ATTENTION can cause regressions to the GOCE versions translations
	see new test
	
	Revision 1.3  2005/06/09 00:41:47  marpas
	XML filter functionality added
	
	Revision 1.2  2004/09/17 20:07:43  marpas
	Data extraction almost complete, beta release
	
	Revision 1.1  2004/09/16 16:58:18  marpas
	Data consumer concept implemented and almost tested
	work still in progress
	
	
	
*/

#include <XMLDataConsumer.h>
#include <XMLExtractorHandlerBase.h>
#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(XMLDataConsumer)

// -----------------------------------------------------------------
//
//
// constructor and destructor
//
// -----------------------------------------------------------------
XMLDataConsumer::XMLDataConsumer() 
{
}

XMLDataConsumer::~XMLDataConsumer() throw() {}


bool XMLDataConsumer::nodeMatch(XMLExtractorHandlerBase &h, const string &father, const string &node, const XERCES_CPP_NAMESPACE::AttributeList &l, const XMLCh* name)
{
	return nodeMatch(h,father+"/"+node,l, name) ;
}

bool XMLDataConsumer::nodeMatch(XMLExtractorHandlerBase &, const string &nodepath, const XERCES_CPP_NAMESPACE::AttributeList &, const XMLCh* )
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "node: " << nodepath) ;
	return true ;
}

bool XMLDataConsumer::nodeValue(XMLExtractorHandlerBase &h, const string &father, const string &node, const string &value, const XMLCh* name)
{
	return nodeValue(h,father+"/"+node,value,name) ;
}

bool XMLDataConsumer::nodeValue(XMLExtractorHandlerBase &, const string &nodepath, const string &value, const XMLCh* )
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "node: " << nodepath  << " -> \"" << value << "\"") ;
	return false ;	
}
		
void XMLDataConsumer::pushConsumer(XMLExtractorHandlerBase &xh, XMLDataConsumer *c) 
{
	xh.pushConsumer(c) ;
}

void XMLDataConsumer::pushConsumer(XMLExtractorHandlerBase &xh, std::vector<XMLDataConsumer *> &c) 
{
	xh.pushConsumer(c) ;
}

void XMLDataConsumer::popConsumer(XMLExtractorHandlerBase &xh, XMLDataConsumer *c)
{
	xh.popConsumer(c) ;
}


void XMLDataConsumer::popConsumer(XMLExtractorHandlerBase &xh, std::vector<XMLDataConsumer *> &c)
{
	xh.popConsumer(c) ;
}


#if ( XERCES_VERSION_MAJOR > 2 )
bool XMLDataConsumer::characters(XMLExtractorHandlerBase &, const XMLCh*, const XMLSize_t) 
#else
bool XMLDataConsumer::characters(XMLExtractorHandlerBase &, const XMLCh*, const unsigned int) 
#endif
{
	return false ;
}


_ACS_END_NAMESPACE
