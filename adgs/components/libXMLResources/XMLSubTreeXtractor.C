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
	Revision 5.2  2014/02/13 12:42:30  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.1  2013/07/19 09:01:20  marpas
	doc fixed
	qa warnings fixed
	compilation warnings fixed
	
	Revision 5.0  2013/07/18 17:54:35  marpas
	adopting libException 5.x standards
	performances improved
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.4  2012/02/13 16:47:41  marpas
	refactoring
	
	Revision 2.3  2009/11/25 13:08:25  marpas
	RegExp used to enforce given path
	
	Revision 2.2  2009/11/23 18:34:57  marpas
	extraction now uses the '/tag<n>' notation
	
	Revision 2.1  2009/11/13 11:32:15  marpas
	class XMLSubTreeXtractor added with unitary test
	
	
	
	
*/

#include <XMLSubTreeXtractor.h>
#include <Filterables.h>
#include <RegExp.h>
#include <xercesc/sax/AttributeList.hpp> // PRQA S 1013


_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(XMLSubTreeXtractor)

// -----------------------------------------------------------------
//
//
// constructor and destructor
//
// -----------------------------------------------------------------
XMLSubTreeXtractor::XMLSubTreeXtractor(std::ostream &out, const std::string &node) :
	XMLDataConsumer(),
    _theOutputStream(out), // PRQA S 2528
    _theNode(),
	_whichInstance(0),
	_currInstance(0),
	_extracting(false),
	_indent(0),
	_indentString("\t"),
	_nextLevel(0) 

{	
	// node reconditioning 
	string nodepath = node ;
	if (nodepath.c_str()[0] != '/') { // add a leading / in case was missing.
		nodepath = "/" + nodepath ;
    }
	while (nodepath.find("//") != string::npos) { // replace '//' with '/'
		nodepath.replace(nodepath.find("//"), 2, "/") ; // PRQA S 4400
	}

	RegExp re("^(/([][A-Za-z_0-9]+(<[0-9]+>)?))+") ;
	re.verify(nodepath) ; // throws an exception ... 
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "*** reconditioned: \"" << nodepath << "\" (\"" << node << "\")") ;
	
	// trying to find xxx<nn> with the closed angle bracket
	size_t endnode = nodepath.find(">") ; // PRQA S 4412
	if (endnode != string::npos) {
		string mynode = nodepath.substr(0, endnode+1) ; // PRQA S 3084
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "node: \"" << mynode << "\"") ;
		// now find '<'
		size_t angbr = mynode.find("<") ; // PRQA S 4412
		ACS_COND_THROW(angbr == string::npos, exIllegalValueException("Wrong xml path:" + mynode)) ; // PRQA S 3081
		
        _whichInstance = atoi(mynode.substr(angbr+1).c_str()) ; // PRQA S 3000, 3084
		
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "_whichInstance: \"" << mynode.substr(angbr) << "\": " << _whichInstance) ;

		_theNode = mynode.substr(0,angbr) ;

		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "_theNode: \"" << _theNode << "\" rest: \"" << nodepath.substr(endnode+1) << "\"") ; // PRQA S 3084

		if (!nodepath.substr(endnode+1).empty()) { // PRQA S 3084 2
			_nextLevel = new XMLSubTreeXtractor(out,_theNode+"/"+nodepath.substr(endnode+1)) ;
        }
	}
	else {
		_theNode = nodepath ;
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "_theNode: \"" << _theNode << "\"") ;
	}
}

XMLSubTreeXtractor::~XMLSubTreeXtractor() throw() 
{
	delete _nextLevel ;
}

void XMLSubTreeXtractor::setIndentString(const std::string &s) // PRQA S 4121
{
	_indentString = s ;
}
		
		

void XMLSubTreeXtractor::indent(bool end)
{		
	if ((0 !=_indent) || end) { // not the first 
	    _theOutputStream << "\n" ;
    }
	for (unsigned int i=0; i < _indent; i++) {
		_theOutputStream << _indentString ;
    }
}

bool XMLSubTreeXtractor::nodeMatch(XMLExtractorHandlerBase &h,
                                   const std::string &father, 
                                   const std::string &node, 
                                   const XERCES_CPP_NAMESPACE::AttributeList &l, 
                                   const XMLCh* )
{
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "_theNode: \"" << _theNode << "\" -- " << father+"/"+node) ;

	// start of extracting logic
	// the performance can be better ... but complexity increase
	// to the next programmer: _currInstance can be used to avoid comparation when instance has already been found (after the end)
	// then one can compute xml indentation level starting from the beginning of extraction.. when it reaches 0 again this means the extraction is finished
	if (!_extracting && _theNode == father+"/"+node) { // nb: father+node is always absolute (the == is costly)
		if (_whichInstance==_currInstance) {
			_extracting = true ;
			if (_nextLevel) { // next level present: push it
				pushConsumer(h, _nextLevel) ; // push next level in order to activate it during parsing
            }
		}
		_currInstance++ ;
	}
	// end of extracting logic
		
	if (_extracting && !_nextLevel) { // I was extrating and I'm the 'leaf'
		// all the if "body" is to write into the output stream - no extracting logic inside
		indent(false) ;
		_indent++ ;
		_theOutputStream << "<" << node ;
			
		for (unsigned int i=0; i < l.getLength(); i++)
    	{
			_theOutputStream << " " ;
			string n ;
            string v ;
            string t ;
        	_theOutputStream << XMLHandler::hpTrimmedString(l.getName(i),n) << "="
                	 << "\"" << XMLHandler::hpTrimmedString(l.getValue(i),v) << "\"" ;

    	}
		_theOutputStream << ">" ;
	}
	
	return true ;
}


bool XMLSubTreeXtractor::nodeValue(XMLExtractorHandlerBase &h, 
                                   const std::string &father, 
                                   const std::string &node, 
                                   const std::string &value, 
                                   const XMLCh* )
{
	if (_extracting) { // extracting ?
	
		// start of extracting logic
		// to the next programmer ... 
	    // then one can compute xml indentation level starting from the beginning of extraction.. 
		// ..when it reaches 0 again this means the extraction is finished.
		// this should avoid the string comparation below
		if (_theNode == father+"/"+node) { // this comparation is costly - performaces can be better ...
			_extracting = false ;
			if (_nextLevel) { // next level present: pop it 
				popConsumer(h, _nextLevel) ;
            }
		}
		// end of extracting logic
		
		if (!_nextLevel) { // i am the 'leaf'

			// all the if "body" is to write into the output stream - no extracting logic inside
			_indent-- ;
			if (value.empty()) {
				indent(true) ;	
			}
			else {
				_theOutputStream << value ;
			}
			_theOutputStream << "</" << node << ">";
		}
	}
	return true ;
}

_ACS_END_NAMESPACE
