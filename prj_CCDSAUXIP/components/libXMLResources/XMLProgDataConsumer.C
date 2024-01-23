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
	Revision 5.5  2013/12/05 17:37:08  lucio.pulvirenti
	ifdef added in characters method as in base function XMLDataConsumer.
	
	Revision 5.4  2013/11/29 13:19:05  lucio.pulvirenti
	Name of flag to remove namesapces now neglectNSpaces_ since the same namespace may have multiple occurrence on the same composed node name.
	If namespaces must be neglected, XMLResourceHandler::removeNSpaces static method used now.
	
	Revision 5.3  2013/11/29 09:14:03  lucio.pulvirenti
	Bug fixed: if namespace to neglect, add leading "/" just if not occurring.
	
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

#include <XMLProgDataConsumer.h>
#include <XMLExtractorHandlerBase.h>
#include <XMLResourceHandler.h>
#include <StringUtils.h>
#include <Filterables.h>
#include <algorithm>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(XMLProgDataConsumer)

namespace {
    struct node_builder { // PRQA S 2171, 2173, 2175
        explicit node_builder(map<string, bool> &n) : n_(n), last_() {} // PRQA S 2528
        node_builder(const node_builder &rhs) : n_(rhs.n_), last_(rhs.last_) {} // PRQA S 2528
        ~node_builder() throw() {}
        void operator()(const std::string &n) { 
             last_ += "/" + n ;
             n_.insert(make_pair(last_, false)) ; 
        }

        map<string, bool> &n_ ; // PRQA S 2100 2
        string last_ ; 
    private:
        node_builder() ; // not implemented
        node_builder& operator= (const node_builder &) ; // not implemented
    } ;

    struct node_inserter { // PRQA S 2171, 2173, 2175
        explicit node_inserter(map<string, bool> &n) : n_(n) {} // PRQA S 2528
        node_inserter(const node_inserter &r) : n_(r.n_) {} // PRQA S 2528
        ~node_inserter() throw() {}
        void operator()(const std::string &s) { 
            vector<string> tokens ;

            StringUtils::Tokenize(s, tokens, "/") ;
            for_each(tokens.begin(), tokens.end(), node_builder(n_) ) ; // PRQA S 3081
        }
        map<string, bool> &n_ ; // PRQA S 2100
    private:
        node_inserter() ; // not implemented
        node_inserter& operator= (const node_inserter &) ; // not implemented
    } ;
}

XMLProgDataConsumer::XMLProgDataConsumer(const std::vector<std::string> &nodepaths, bool neglectNSpaces) :
    XMLDataConsumer(),
    _nodes(),
    found_(false),
    in_node_(false), 
	neglectNSpaces_(neglectNSpaces)
{
    for_each(nodepaths.begin(), nodepaths.end(), node_inserter(_nodes)) ; // PRQA S 3081
	ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
        for ( map<string, bool>::iterator it = _nodes.begin(); // PRQA S 4238
             it != _nodes.end();
             ++it) {
            ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, it-> first) ;
        }
	ACS_CLASS_END_DEBUG
}

XMLProgDataConsumer::~XMLProgDataConsumer() throw() {}


bool XMLProgDataConsumer::nodeMatch(XMLExtractorHandlerBase &h, 
                                    const std::string &father, 
                                    const std::string &node, 
                                    const XERCES_CPP_NAMESPACE::AttributeList &attr, 
                                    const XMLCh* name)
{
	string nodeFullName = father+"/"+node;
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "node: " << nodeFullName) ;
	if( neglectNSpaces_ ) {
		nodeFullName = XMLResourceHandler::removeNSpaces(nodeFullName);
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Neglecting namespace node: " << nodeFullName) ;
	}

    map<string, bool>::iterator it = _nodes.find(nodeFullName) ;
    if ( it != _nodes.end() ) {
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "node found: " << nodeFullName << " - " << it->second) ;
        it->second = true ;
        h.writeStart(name, attr) ;
        found_ = true ;
    }
    in_node_ = true ;
    return true ;
}


bool XMLProgDataConsumer::nodeValue(XMLExtractorHandlerBase &h, 
                                    const std::string &father, 
                                    const std::string &node, 
                                    const std::string &value, 
                                    const XMLCh* name)
{
	string nodeFullName = father+"/"+node;
	if( neglectNSpaces_ ) {
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Removing : " << nodeFullName ) ;
        nodeFullName = XMLResourceHandler::removeNSpaces(nodeFullName);
	}
    map<string, bool>::iterator it = _nodes.find(nodeFullName) ;
    if ( it != _nodes.end() ) {
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "node: " << nodeFullName << " - " << boolalpha << it->second << " [" << value << "]") ;
        h.writeEnd(name) ;
        found_ = false ;
        in_node_ = false ;
    }
    return true ;
}


#if ( XERCES_VERSION_MAJOR > 2 )
bool XMLProgDataConsumer::characters(XMLExtractorHandlerBase &h, const XMLCh* c, const XMLSize_t l) 
#else
bool XMLProgDataConsumer::characters(XMLExtractorHandlerBase &h, const XMLCh* c, const unsigned int l) 
#endif
{
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "found_: " << boolalpha << found_ << " " << "in_node_: " << in_node_) ;
    if (found_) {
        ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB) 
        string s ; 
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "formatting \"" << XMLHandler::toString(c,s) << "\"") ;
        ACS_CLASS_END_DEBUG
        h.formatBuf(c,l) ;
    }
    else if (not in_node_) {
        ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB) 
        string s ; 
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "formatting \"" << XMLHandler::toString(c,s) << "\"") ;
        ACS_CLASS_END_DEBUG
        h.formatBuf(c,l) ;
        in_node_ = true ;
    }
    else { /* nothing */ }
    
	return true ;
}


		

_ACS_END_NAMESPACE


