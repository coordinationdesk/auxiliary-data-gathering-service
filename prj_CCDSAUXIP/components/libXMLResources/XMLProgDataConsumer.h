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
	Revision 5.3  2013/11/29 13:19:05  lucio.pulvirenti
	Name of flag to remove namesapces now neglectNSpaces_ since the same namespace may have multiple occurrence on the same composed node name.
	If namespaces must be neglected, XMLResourceHandler::removeNSpaces static method used now.
	
	Revision 5.2  2013/11/28 15:28:24  lucio.pulvirenti
	Nodes namespace can be now neglected by setting a flag passed as CTOR input.
	
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
	
	Revision 2.1  2013/01/28 13:27:09  marpas
	XMLProgDataConsumer class added
	unitary test added
	
	
*/

#ifndef _XMLProgDataConsumer_H_
#define _XMLProgDataConsumer_H_

#include <acs_c++config.hpp>
#include <XMLDataConsumer.h>
#include <map>
#include <vector>
#include <string>


_ACS_BEGIN_NAMESPACE(acs)


//////////////////////////////////
//								//
//   classe XMLDataConsumer		//
//								//
//////////////////////////////////

class XMLProgDataConsumer : public XMLDataConsumer // PRQA S 2109, 2153
{ 
public:

	/**
	 * \brief  Class constructor 
	 * 
	 */	
	explicit XMLProgDataConsumer(const std::vector<std::string> &, bool neglectNSpaces = false) ;

	/**
	 * \brief  Class destructor 
	 */			
	virtual ~XMLProgDataConsumer() throw() ;
    
    using XMLDataConsumer::nodeMatch ;
    using XMLDataConsumer::nodeValue ;

    // PRQA S 2120 3
	virtual bool nodeMatch(XMLExtractorHandlerBase &h, 
                         const std::string &father, 
                         const std::string &node, 
                         const XERCES_CPP_NAMESPACE::AttributeList &attr, 
                         const XMLCh* name = 0) ;	

    virtual bool nodeValue(XMLExtractorHandlerBase &h, 
                          const std::string &father, 
                          const std::string &node, 
                          const std::string &value, 
                          const XMLCh* name = 0) ;
                          
 #if ( XERCES_VERSION_MAJOR > 2 )
	virtual bool characters(XMLExtractorHandlerBase &h, const XMLCh* c, const XMLSize_t l) ;
 #else
	virtual bool characters(XMLExtractorHandlerBase &h, const XMLCh* c, const unsigned int l) ;
 #endif

private:
	// the copy ctor and assignement operator are declared private to prevent their use.
	// They are, hence, not implemented
	XMLProgDataConsumer();  // not implemented
	XMLProgDataConsumer(const XMLProgDataConsumer &);  // not implemented
	XMLProgDataConsumer &operator=(const XMLProgDataConsumer &);   // not implemented
private:	
    std::map <std::string, bool> _nodes ;
    bool found_ ;
    bool in_node_ ; 
	bool neglectNSpaces_;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(XMLProgDataConsumer) ;

} ;

_ACS_END_NAMESPACE



#endif //_XMLDataConsumer_H_

