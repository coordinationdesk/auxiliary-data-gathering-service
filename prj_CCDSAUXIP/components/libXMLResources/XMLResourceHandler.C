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
	Revision 5.4  2014/02/13 12:42:30  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.3  2013/12/05 17:11:07  lucio.pulvirenti
	MP: coding best practices applied
	
	Revision 5.2  2013/11/29 13:16:23  lucio.pulvirenti
	Static method removeNSpaces added to remove all namespaces from node name.
	
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
	
	Revision 2.8  2013/05/16 15:34:40  marpas
	tag namespaces are NEVER put in the resourceSet hence
	<sentinel-safe:nssdcIdentifier>XXXX</sentinel-safe:nssdcIdentifier>
	will result in
	nssdcIdentifier: XXXX
	
	Revision 2.7  2012/02/13 16:47:41  marpas
	refactoring
	
	Revision 2.6  2009/12/21 17:30:08  marpas
	root extraction is conditioned to XMLIstream::extractRoot(...) method
	default false
	
	Revision 2.5  2009/11/20 16:48:24  marpas
	resource "XmlExtractionRootTagName_" added at extraction end.
	
	Revision 2.4  2009/09/16 11:14:07  marpas
	XMLValidator used in XMLIstream
	xerces namespace management simplified
	
	Revision 2.3  2009/09/14 12:51:52  marpas
	XMLValidator class added
	unitary test added
	other test improved
	
	Revision 2.2  2009/09/10 14:55:24  marpas
	Xercers 3.0.x i/f compatibilty
	
	Revision 2.1  2008/01/18 18:55:56  clanas
	fixed namespace management for new xerces version
	
	Revision 2.0  2006/02/28 08:37:31  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.9  2004/09/16 12:15:35  marpas
	architecture redesign
	
	Revision 1.8  2004/09/15 16:05:00  marpas
	improved
	
	Revision 1.7  2003/10/03 15:48:47  marpas
	comments inside multiple lined tag values are no loger attaching lines
	
	Revision 1.6  2003/07/11 13:00:56  marpas
	exException::clone implemented
	
	Revision 1.5  2003/04/15 18:08:02  marpas
	GCC3.2
	
	Revision 1.4  2002/12/04 10:17:05  giucas
	Added trim in toString method for deleting extra characters
	
	Revision 1.3  2002/12/03 16:56:20  giucas
	Special characters handling
	
	Revision 1.2  2002/11/11 18:35:55  giucas
	Implemented count attribute specific handling
	
	Revision 1.1.1.1  2002/10/17 14:26:51  giucas
	Import libXMLResources
	
*/

#include <XMLResourceHandler.h>
#include <rsResourceSet.h>
#include <Filterables.h>
#include <StringUtils.h>

#include <xercesc/sax/AttributeList.hpp> // PRQA S 1013


#include <iostream>
#include <sstream>


_ACS_BEGIN_NAMESPACE(acs)



using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(XMLResourceHandler)

// -----------------------------------------------------------------
//
//
// constructor and destructor
//
// -----------------------------------------------------------------
XMLResourceHandler::XMLResourceHandler(rsResourceSet &rs) : 
    XMLHandler(),
    _root(),
    _elements(),
	_resource(rs), // PRQA S 2528
	_gotChars(false),
	_writeRoot(false) 
{
}

XMLResourceHandler::~XMLResourceHandler() throw() {}

//
// notify start of an XML file element
//
void XMLResourceHandler::startElement(const XMLCh* name, XERCES_CPP_NAMESPACE::AttributeList& attributes)
{
	ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
		string t ;
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Found element: " << hpTrimmedString(name,t)) ;
	ACS_CLASS_END_DEBUG	
	
	ResourceElement elem;
	string en ;
    elem.setName(removeNSpace(hpTrimmedString(name,en)));
    size_t alen = attributes.getLength() ;
    for (size_t i=0; i < alen; ++i)
    {
		string n ;
        string v ;
        string t ;
        elem.setAttribute(hpTrimmedString(attributes.getName(i),n),
                          hpTrimmedString(attributes.getValue(i),v),
                          hpTrimmedString(attributes.getType(i),t));
    }
    _elements.push(elem);
}

//
// notify end of an XML file element
//
void XMLResourceHandler::endElement(const XMLCh* name)
{
    ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)	
		string t ;
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "end element: " << hpTrimmedString(name,t)) ;
	ACS_CLASS_END_DEBUG

	ResourceElement currentElement = _elements.top();
    _elements.pop();
    if (_elements.size() > 0) {
        _elements.top().setChild(currentElement);
    }
    else {
        _root = currentElement;
    }
	_gotChars = false ;

}

//
// notify characters data into an XML file element
//
 #if ( XERCES_VERSION_MAJOR > 2 ) 
void XMLResourceHandler::characters(const XMLCh* chars, const XMLSize_t)
 #else
void XMLResourceHandler::characters(const XMLCh* chars, const unsigned int)
 #endif
{
    ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)	
		string t ;
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "'" << toString(chars,t,_gotChars) << "'") ;
	ACS_CLASS_END_DEBUG
	string c ;
	_elements.top().setValue(toString(chars,c,_gotChars));
	_gotChars = true ;
}


//
// receive notification of the beginning of the document
//
void XMLResourceHandler::startDocument()
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "XML document begins") ;
}

//
// receive notification of the end of the document
//
void XMLResourceHandler::endDocument()
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "XML document ends") ;

	analize("", _root);
    setResource( _root.getChildren());
	setResourceRootName(_root) ;
}

// recursive function
void XMLResourceHandler::setResource(std::vector<ResourceElement>& list)
{
    size_t lsize = list.size() ;
    for (size_t i=0; i < lsize ; ++i)
    {
        string elemName ;
		string elemValue ;
		list[i].getName(elemName);
		list[i].getValue(elemValue);		
		
        elemName = removeNSpace(elemName) ;
        
		int counter = list[i].getArrayCounter();
		if(counter != -1) {
			string arrayName=elemName;
			if(counter > 0) {
				arrayName = elemName.substr(0,elemName.rfind('['));
            }
			_resource.setValue(arrayName + "[]", counter);
		}
		
		_resource.setValue(elemName, elemValue);
		
        //attributes
        stack<ResourceElementAttribute> attr = list[i].getAttributes();
		size_t numAttr = attr.size();
        if(numAttr > 0) {
			string names[attr.size()];
			for(long k = numAttr-1; k>=0; k--) { // PRQA S 3000, 3010, 3084
				string attrName ;
            	attr.top().getAttrName(attrName);
                string attrValue;
            	attr.top().getAttrValue(attrValue);
                string attrType;
            	attr.top().getAttrType(attrType);
				
				names[k] = attrName;

				// setting attribute value
				_resource.setValue(elemName + "." + names[k] + ".value", attrValue );				   

				// setting attribute type
				_resource.setValue(elemName + "." + names[k] + ".type", attrType );

        	    attr.pop();
			}
			_resource.setArray(elemName + ".attributes", names, numAttr);
		}
        //visit recursively element children
        vector<ResourceElement> children = list[i].getChildren();
        if ( children.size() > 0 ) {
            setResource(children);
        }
    }
}

void XMLResourceHandler::setResourceRootName(ResourceElement& root)
{
	if (_writeRoot) {
		string rootName ;
		root.getName(rootName) ;
		_resource.setValue("XmlExtractionRootTagName_", removeNSpace(rootName)) ;
		
		stack<ResourceElementAttribute> attr = root.getAttributes();
		size_t numAttr = attr.size();
        if(numAttr > 0) {
			string names[attr.size()];
			for(long k = numAttr-1; k>=0; k--) { // PRQA S 3000, 3010, 3084
				string attrName ;
            	attr.top().getAttrName(attrName);
                string attrValue;
            	attr.top().getAttrValue(attrValue);
                string attrType;
            	attr.top().getAttrType(attrType);
				
				names[k] = attrName;

				// setting attribute value
				_resource.setValue("XmlExtractionRootTagName_." + names[k] + ".value", attrValue );				   

				// setting attribute type
				_resource.setValue("XmlExtractionRootTagName_." + names[k] + ".type", attrType );

        	    attr.pop();
			}
			_resource.setArray("XmlExtractionRootTagName_.attributes", names, numAttr);
		}
	}
}

/*
 * private methods
 */
void XMLResourceHandler::analize(const std::string& name, ResourceElement& elem)
{
    bool count=false;
	int countVal=0;
	stack<ResourceElementAttribute> attr;
	elem.getElemAttributes(attr);
	while( (attr.size()>0) && !count){
		string attrName="";
		attr.top().getAttrName(attrName);
		if(attrName == "count"){
			string valCount;
			attr.top().getAttrValue(valCount);
			countVal = atoi(valCount.c_str());
			count=true;
		}
		attr.pop();
	}
	
	if( count && (countVal == 0)) {
		elem.setArrayElement(true);
		elem.setArrayCounter(countVal);
	}
	
	if (elem.getChildren().size() > 0)
    {   
		setNames(elem.getChildren(),count);
		setArraySize(elem.getChildren());
		setPathNames(name, elem.getChildren());
    }
    size_t childrens = elem.getChildren().size() ;
    for (size_t i=0; i < childrens ; ++i)
    {
        string elemName;
        elem.getChildren()[i].getName(elemName);
        analize(elemName, elem.getChildren()[i]);
    }

}

void XMLResourceHandler::setNames(std::vector<ResourceElement>& list, const bool& count)
{
    if(count){
	
		string listName;
		list[0].getName(listName);
		size_t lsize = list.size() ;
		for (size_t i=0; i < lsize ; ++i)
    	{
			string name;
        	list[i].getName(name);
			
			if(name == listName)
			{
				ostringstream cnt;
            	cnt << i ;
            	string value = name + "[" + cnt.str() + "]";
            	list[i].setArrayElement(true);
				list[i].setTagName(name);
				list[i].setName(value);
			}
			else
			{
				ACS_THROW(XMLHandlerException("Invalid tag name " + name + " in array.")) ; // PRQA S 3081
			}
		}
	}
	else {
		size_t lsize = list.size() ;
		for (size_t i=0; i < lsize ; ++i)
    	{
        	string name;
        	list[i].getName(name);
			int counter = -1;
        	bool nameSet = false;
		    size_t lsize2 = list.size() ;
			for (size_t j=i+1; j < lsize2 ; ++j) // PRQA S 3084
        	{
            	string elemName;
            	list[j].getName(elemName);
				if ( elemName == name )
            	{ 
					if (!nameSet)
                	{
                    	counter++;
                    	ostringstream cnt;
                    	cnt << counter ;
                    	string value = name + "[" + cnt.str() + "]";
                    	list[i].setArrayElement(true);
						list[i].setTagName(elemName);
						list[i].setName(value);
						nameSet = true;
                	}
                	counter++;
                	ostringstream cnt;
                	cnt << counter;
                	string value = name + "[" + cnt.str() + "]";
					list[j].setArrayElement(true);
					list[j].setTagName(elemName);
                	list[j].setName(value);
	           	}
        	}
    	}
	}
}


void XMLResourceHandler::setArraySize(std::vector<ResourceElement>& list)
{
	size_t lsize = list.size() ;
	for (size_t i=0; i < lsize ; ++i)
    {        
		if(list[i].isArrayElement()) { //this element belongs to an array
			
			string tagName = list[i].getTagName();
			int counter=1;
        	string n;
			list[i].getName(n);
		    size_t lsize2 = list.size() ;
			for (size_t j=i+1; j < lsize2 ; ++j) // PRQA S 3084
        	{
            	
				if(list[j].isArrayElement()) {
				
					if ( list[j].getTagName() == tagName ) {
            			counter++;
						list[j].setArrayElement(false);
					}
				}
        	}
			list[i].setArrayCounter(counter);
		}
    }
}

void XMLResourceHandler::setPathNames(const std::string& root, std::vector<ResourceElement>& list)
{
    size_t lsize = list.size()  ;
    for (size_t i=0; i < lsize ; ++i)
    {
        string name;
        list[i].getName(name);
        if (root.length() > 0) {
            name = root + "." + name;
        }
        list[i].setName(name);
    }

}



void XMLResourceHandler::writeRoot(bool v) // PRQA S 4121
{
	_writeRoot = v;
}


string XMLResourceHandler::removeNSpaces(const std::string &s)
{
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "original node name \"" << s << "\"") ;
	
	string result;
	if( s.find(':') != string::npos ) { 
		vector<string> tokens;
		StringUtils::Tokenize(s, tokens, "/");
        size_t t_size = tokens.size(); 
		for(size_t i = 0; i < t_size; ++i ) { 
			string node = removeNSpace(tokens[i]);
			if( !StringUtils::startsWith(node, "/") ) {
				node = "/" + node;
			}
			result = result + node;
		}
	}
	else {
		result = s;
	}
	
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "final node name \"" << result << "\"") ;
	return result;
}

_ACS_END_NAMESPACE
