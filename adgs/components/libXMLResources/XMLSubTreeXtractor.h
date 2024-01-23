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
	Revision 5.0  2013/07/18 17:54:35  marpas
	adopting libException 5.x standards
	performances improved
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.6  2012/12/02 20:03:49  marpas
	qa rules
	
	Revision 2.5  2012/06/11 16:20:05  marpas
	some fixes to interfaces and tests
	
	Revision 2.4  2012/06/05 16:41:28  marfav
	code improved to remove compiler warnings
	
	Revision 2.3  2012/02/13 16:47:41  marpas
	refactoring
	
	Revision 2.2  2009/11/23 18:34:57  marpas
	extraction now uses the '/tag<n>' notation
	
	Revision 2.1  2009/11/13 11:32:15  marpas
	class XMLSubTreeXtractor added with unitary test
	
	
*/

#ifndef _XMLSubTreeXtractor_H_
#define _XMLSubTreeXtractor_H_

#include <acs_c++config.hpp>
#include <XMLDataConsumer.h>
#include <exException.h>
#include <iostream>

_ACS_BEGIN_NAMESPACE(acs)


//////////////////////////////////
//								//
//   classe XMLSubTreeXtractor	//
//								//
//////////////////////////////////

/**
 * This class is the base for a user defined data consumer.
 * Conventionally, each xml node name is attached to its children with a '/' character
 * e.g. \<parent\>\<child\> will result for parent/child path for the node child.
 */	

class XMLSubTreeXtractor : public XMLDataConsumer // PRQA S 2109, 2153
{
public:

	/**
	 * \brief  Class constructor 
	 * This is the ctor of the class. The passed stream will be the one were the extraction will be performed.
	 *
	 */	
	XMLSubTreeXtractor(std::ostream &, const std::string &node) ; 

	/**
	 * \brief  Class destructor 
	 */			
	virtual ~XMLSubTreeXtractor() throw() ;

	void setIndentString(const std::string &) ;

protected:
	// import not redefined signatures of nodeMatch from XMLDataConsumer
	using XMLDataConsumer::nodeMatch;
	using XMLDataConsumer::nodeValue;

	virtual bool nodeMatch(XMLExtractorHandlerBase &, const std::string &, const std::string &node, const XERCES_CPP_NAMESPACE::AttributeList &, const XMLCh* name = 0) ;	
	virtual bool nodeValue(XMLExtractorHandlerBase &, const std::string &, const std::string &node, const std::string &value, const XMLCh* name = 0) ;


private:
	// the copy ctor and assignement operator are declared private to prevent their use.
	// They are, hence, not implemented
	XMLSubTreeXtractor(); 
	XMLSubTreeXtractor(const XMLSubTreeXtractor &); 
	XMLSubTreeXtractor &operator=(const XMLSubTreeXtractor &); 

	void indent(bool end) ;

private:
	std::ostream &_theOutputStream ;
	std::string _theNode ;
	unsigned int _whichInstance ;
	unsigned int _currInstance ;
	bool _extracting ;
	unsigned int _indent ;
	std::string _indentString ;
	XMLSubTreeXtractor *_nextLevel ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(XMLSubTreeXtractor) ;

} ;

_ACS_END_NAMESPACE


#endif //_XMLSubTreeXtractor_H_

