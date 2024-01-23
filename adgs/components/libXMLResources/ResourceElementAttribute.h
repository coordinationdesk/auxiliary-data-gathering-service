// PRQA S 1050 EOF
/*

	Copyright 2002-2013, Advanced Computer Systems , Inc.
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
	Revision 2.3  2012/12/02 20:03:49  marpas
	qa rules
	
	Revision 2.2  2012/02/13 16:47:41  marpas
	refactoring
	
	Revision 2.1  2006/07/14 09:23:20  marpas
	documentation fixed and improved.
	global variables are now encaspulated into classes better than before
	interruption of saxExtract method allowed when the last data consumer leave.
	new test to show this feature added
	
	Revision 2.0  2006/02/28 08:37:31  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.3  2004/12/06 14:09:22  marpas
	header fixed
	
	Revision 1.2  2003/04/30 10:30:59  marpas
	using namespace std was removed from includes
	
	Revision 1.1.1.1  2002/10/17 14:26:51  giucas
	Import libXMLResources
	


*/

#ifndef _ResourceElementAttribute_H_
#define _ResourceElementAttribute_H_

#include <acs_c++config.hpp>
#include <exException.h>
#include <string>
#include <stack>
#include <vector>


_ACS_BEGIN_NAMESPACE(acs)

class ResourceElementAttribute {

public:

	/*! class acs::ResourceElementAttributeException declaration */
	exDECLARE_EXCEPTION(ResourceElementAttributeException,exException) ; // PRQA S 2131, 2502


	ResourceElementAttribute();
    ~ResourceElementAttribute() throw() ;
    //copy constructor
    ResourceElementAttribute(const ResourceElementAttribute&);
    //operator =
    ResourceElementAttribute& operator=(const ResourceElementAttribute&);

    //set methods
    void setAttrName(const std::string&);
    void setAttrValue(const std::string&);
    void setAttrType(const std::string&);

    //get methods
    void getAttrName(std::string&);
    void getAttrValue(std::string&);
    void getAttrType(std::string&);

private:
    std::string _name;
    std::string _value;
    std::string _type;

};

_ACS_END_NAMESPACE




#endif //_ResourceElementAttribute_H_






