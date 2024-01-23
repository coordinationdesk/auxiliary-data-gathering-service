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
	Revision 2.2  2012/02/13 16:47:41  marpas
	refactoring
	
	Revision 2.1  2006/07/14 09:23:20  marpas
	documentation fixed and improved.
	global variables are now encaspulated into classes better than before
	interruption of saxExtract method allowed when the last data consumer leave.
	new test to show this feature added
	
	Revision 2.0  2006/02/28 08:37:31  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.5  2004/12/06 14:09:22  marpas
	header fixed
	
	Revision 1.4  2003/04/30 10:30:59  marpas
	using namespace std was removed from includes
	
	Revision 1.3  2002/12/03 16:56:33  giucas
	Special characters handling
	
	Revision 1.2  2002/11/11 18:35:12  giucas
	Added public method getElemAttributes for count handling
	
	Revision 1.1.1.1  2002/10/17 14:26:51  giucas
	Import libXMLResources
	


*/

#include <ResourceElement.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

ResourceElement::ResourceElement() :
    _name(),
    _value(),
    _attributes(),
    _children(),
	_valueSet(false),
	_arrayElement(false),
	_arrayCounter(-1),
    _tagName()
{
}

ResourceElement::~ResourceElement() throw() {}

//copy constructor
ResourceElement::ResourceElement(const ResourceElement &e) :
    _name(e._name),
    _value(e._value),
    _attributes(e._attributes),
    _children(e._children),
    _valueSet(e._valueSet),
	_arrayElement(e._arrayElement),
	_arrayCounter(e._arrayCounter),
	_tagName(e._tagName)

{
    // empty 
}

//operator =
ResourceElement&
ResourceElement::operator=(const ResourceElement &e)
{
    if (this != &e)
    {
        _name = e._name;
        _value = e._value;
        _attributes = e._attributes;
        _children = e._children;
        _valueSet = e._valueSet;
		_arrayElement = e._arrayElement;
		_arrayCounter = e._arrayCounter;
		_tagName = e._tagName;
		
    }
    return *this;
}


/*
 * set methods
 */

void ResourceElement::setName(const string& name) // PRQA S 4121
{
    _name = name;
}

void ResourceElement::setValue(const string& value)
{
    _value += value;
	_valueSet = true;
}

void ResourceElement::setChild(const ResourceElement& child)
{
    _children.push_back(child);
}

void ResourceElement::setAttribute(const string& name,
                                    const string& value,
                                    const string& type)
{
   ResourceElementAttribute attribute;
   attribute.setAttrName(name);
   attribute.setAttrValue(value);
   attribute.setAttrType(type);
   _attributes.push(attribute);
}

void ResourceElement::setArrayElement(const bool& value) // PRQA S 4121
{
    _arrayElement = value;
}

void ResourceElement::setArrayCounter(const int& counter) // PRQA S 4121
{
	_arrayCounter = counter;
}

void ResourceElement::setTagName(const string& name) // PRQA S 4121
{
    _tagName = name;
}



/*
 * get methods
 */
void ResourceElement::getName(string& name) const throw()
{
    name = _name;
}

void ResourceElement::getValue(string& value) const throw() 
{
    value = _value;
}

vector<ResourceElement>& ResourceElement::getChildren() throw() // PRQA S 4120
{
    return _children; // PRQA S 4024
}

stack<ResourceElementAttribute>& ResourceElement::getAttributes() throw()  // PRQA S 4120
{
    return _attributes; // PRQA S 4024
}

string ResourceElement::getTagName() const // PRQA S 4020
{
    if (_tagName.size() > 0) {
		return _tagName;
    }
	else {
		return "";
    }
}

int ResourceElement::getArrayCounter() const throw()   // PRQA S 4120
{
    return _arrayCounter;
}

void ResourceElement::getElemAttributes(stack<ResourceElementAttribute>& attr) const
{
    attr = _attributes;
}

/*  
 * bool methods
 */
bool ResourceElement::hasValue() const throw()    // PRQA S 4120
{
    return _valueSet;
}

bool ResourceElement::isArrayElement() const throw()    // PRQA S 4120
{
	return _arrayElement;
}



_ACS_END_NAMESPACE

