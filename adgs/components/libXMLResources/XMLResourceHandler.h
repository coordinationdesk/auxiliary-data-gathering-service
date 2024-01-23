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
	Revision 5.2  2013/11/29 13:16:23  lucio.pulvirenti
	Static method removeNSpaces added to remove all namespaces from node name.
	
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
	
	Revision 2.8  2013/05/16 15:34:40  marpas
	tag namespaces are NEVER put in the resourceSet hence
	<sentinel-safe:nssdcIdentifier>XXXX</sentinel-safe:nssdcIdentifier>
	will result in
	nssdcIdentifier: XXXX
	
	Revision 2.7  2012/12/02 20:03:49  marpas
	qa rules
	
	Revision 2.6  2012/02/13 16:47:41  marpas
	refactoring
	
	Revision 2.5  2009/12/21 17:30:09  marpas
	root extraction is conditioned to XMLIstream::extractRoot(...) method
	default false
	
	Revision 2.4  2009/11/20 16:48:24  marpas
	resource "XmlExtractionRootTagName_" added at extraction end.
	
	Revision 2.3  2009/09/16 11:14:07  marpas
	XMLValidator used in XMLIstream
	xerces namespace management simplified
	
	Revision 2.2  2009/09/10 14:55:24  marpas
	Xercers 3.0.x i/f compatibilty
	
	Revision 2.1  2008/01/18 18:55:56  clanas
	fixed namespace management for new xerces version
	
	Revision 2.0  2006/02/28 08:37:31  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.8  2004/09/17 20:07:43  marpas
	Data extraction almost complete, beta release
	
	Revision 1.7  2004/09/16 12:15:35  marpas
	architecture redesign
	
	Revision 1.6  2004/09/15 16:05:00  marpas
	improved
	
	Revision 1.5  2003/10/03 15:48:47  marpas
	comments inside multiple lined tag values are no loger attaching lines
	
	Revision 1.4  2003/07/11 13:00:56  marpas
	exException::clone implemented
	
	Revision 1.3  2003/04/30 10:30:59  marpas
	using namespace std was removed from includes
	
	Revision 1.2  2002/11/11 18:35:59  giucas
	Implemented count attribute specific handling
	
	Revision 1.1.1.1  2002/10/17 14:26:51  giucas
	Import libXMLResources

*/

#ifndef _XMLResourceHandler_H_
#define _XMLResourceHandler_H_

#include <acs_c++config.hpp>
#include <XMLHandler.h>
#include <ResourceElement.h>
#include <string>
#include <stack>

_ACS_BEGIN_NAMESPACE(acs)


class rsResourceSet ;

//////////////////////////////////
//								//
//   classe XMLResourceHandler	//
//								//
//////////////////////////////////

/**
 * \brief  This class wraps an XML file in a \a rsResourceSet object.
 * 
 * Receive notification of general document events.
 * This class is derived from \a HandlerBase class which implements 
 * the SAX parser default functionality.
 *
 *
 *\b  wrapping  \b rules \n
 *
 *
 * Given an XML file: \n
 *
 *	\<ROOT\> \n
 *	    \<NODE_A  attribute="bFirst"\> \n
 *			<C  attribute1="c1"   attribute2="c2">cFirstValue</C> \n
 *			\<D\>dValue\</D\> \n 
 *			\<E\>\</E\> \n
 *		\</NODE_A\> \n
 *		\<NODE_A  attribute="bSecond"\> \n
 *			\<C  attribute1="c1"   attribute2="c2"\>cSecondValue\</C\> \n
 *			\<D\>dValue\</D\> \n 
 *			\<E\>\</E\> \n
 *		\</NODE_A\> \n
 *		\<NODE_B  attribute1="f1"   attribute2="f2"   attribute3="f3"\> \n
 *		\</NODE_B\> \n
 *	\</ROOT\> \n
 *
 *
 * it is wrapped in a \a rsResourceSet object containing the following data\n
 *
 *
 *  NODE_A[]=2
 *	NODE_A[0].attributes[]:1 \n
 *	NODE_A[0].attributes[0]:attribute \n
 *	NODE_A[0].attribute.value:bNODE_Birst \n
 *	NODE_A[0].attribute.type:CDATA \n
 *	NODE_A[0]: \n
 *
 *	NODE_A[0].C.attributes[]:2 \n
 *	NODE_A[0].C.attributes[0]:attribute1 \n
 *	NODE_A[0].C.attributes[1]:attribute2 \n
 *	NODE_A[0].C.attribute1.value:c1 \n
 *	NODE_A[0].C.attribute1.type:CDATA \n
 *	NODE_A[0].C.attribute2.value:c2 \n
 *	NODE_A[0].C.attribute2.type:CDATA \n
 *	NODE_A[0].C:cNODE_BirstValue \n
 *	NODE_A[0].D:dValue \n
 *	NODE_A[0].E: \n
 *	NODE_A[1].C:cSecondValue \n
 *	NODE_A[1].C.attribute2.value:c2 \n
 *	NODE_A[1].C.attribute2.type:CDATA \n
 *	NODE_A[1].C.attribute1.value:c1 \n
 *	NODE_A[1].C.attribute1.type:CDATA \n
 *	NODE_A[1].C.attributes[]:2 \n
 *	NODE_A[1].C.attributes[0]:attribute1 \n
 *	NODE_A[1].C.attributes[1]:attribute2 \n
 *	NODE_A[1].D:dValue \n
 *	NODE_A[1].E: \n
 *	NODE_A[1]: \n
 *
 *	NODE_A[1].attribute.value:bSecond \n
 *	NODE_A[1].attribute.type:CDATA \n
 *	NODE_A[1].attributes[]:1 \n
 *	NODE_A[1].attributes[0]:attribute \n
 *	NODE_B: \n
 *
 *	NODE_B.attribute3.value:f3 \n
 *	NODE_B.attribute3.type:CDATA \n
 *	NODE_B.attribute2.value:f2 \n
 *	NODE_B.attribute2.type:CDATA \n
 *	NODE_B.attribute1.value:f1 \n
 *	NODE_B.attribute1.type:CDATA \n
 *	NODE_B.attributes[]:3 \n
 *	NODE_B.attributes[0]:attribute1 \n
 *	NODE_B.attributes[1]:attribute2 \n
 *	NODE_B.attributes[2]:attribute3 \n
 *	 
 * Like shown before, each tree path, defined by XML input file, is listed separating each tree 
 * node in the path by a dot. \n 
 * For each node its attributes name if present is listed and so for  each attribute name is defined its value and type. \n
 * Finally if a node has a list of child nodes with the same name, they are listed. 
 *
 *
 */	

class XMLResourceHandler:  // PRQA S 2109, 2153
	public XMLHandler {

public:

	/**
	 * \brief  Class constructor 
	 * 
	 * \param  rs Reference to \a rsResourceSet object which XML file is wrapped in.   
	 */	
	explicit XMLResourceHandler(rsResourceSet &rs);

	/**
	 * \brief  Class destructor 
	 */			
	virtual ~XMLResourceHandler() throw() ;

	/// Receive notification of the end of an XML document.  		
	virtual void endDocument();

	/**
	 * \brief  Receive notification of the end of an XML document element. 
	 * 
	 * This method overrides \a HandlerBase \b endElement method wrapping 
	 * current element properties in the target \a rsResourceSet.
	 * \param name Pointer to \a XMLCh object which defines element name ended.   
	 */		
	virtual void endElement( const XMLCh* name);

	/**
	 * \brief  Receive notification of the current element character data. 
	 * 
	 * This method overrides \a HandlerBase \b characters method wrapping 
	 * current element value in the target \a rsResourceSet.
	 * \param chars Pointer to \a XMLCh object which defines current element value.
	 * \param lenght Integer value which defines the number of characters to read from the array.   
	 */			
#if ( XERCES_VERSION_MAJOR > 2 )
	virtual void characters( const XMLCh* chars, const XMLSize_t lenght);
#else 
	virtual void characters( const XMLCh* chars, const unsigned int lenght);
#endif

	/**
	 * \brief  Receive notification of the beginning of an element. 
	 * 
	 * This method overrides \a HandlerBase \b startElement method wrapping 
	 * current element name and its attributes list in the target \a rsResourceSet.
	 * \param name Pointer to \a XMLCh object which defines current element name.
	 * \param attributes Reference to \a AttributeList object which contains list of element attributes.   
	 */			
	virtual void startElement( const XMLCh* name, XERCES_CPP_NAMESPACE::AttributeList& attributes);



	/// Receive notification of the beginning of an XML document.
	virtual void startDocument();

	void writeRoot(bool) ;

    static std::string removeNSpace(const std::string &s) { // PRQA S 4020 
        size_t colon = s.find_last_of(':') ;
        if (colon != std::string::npos) {
            return s.substr(colon+1) ; // PRQA S 3084
        }
        return s ;
    }
	
	/**
	 * \brief  Remove all namespaces from node, e.g. /h:table/t:ts ->  /table/ts
	 * 
	 * \param node name.
	 * \return node name with no namspaces  
	 */			
	static std::string removeNSpaces(const std::string &s);
private:


	// the default ctor copy ctor and assignement operator are declared 
    // private to prevent their use.
	// They are, hence, not implemented
	XMLResourceHandler(); 
	XMLResourceHandler(const XMLResourceHandler &); 
	XMLResourceHandler &operator=(const XMLResourceHandler &); 

	//methods
    void analize(const std::string&, ResourceElement&);
    void setNames(std::vector<ResourceElement>&, const bool&);
    void print(std::vector<ResourceElement>&);
    void setPathNames(const std::string&, std::vector<ResourceElement>&);
	void setResource(std::vector<ResourceElement>&);
	void setArraySize(std::vector<ResourceElement>&);
	void setResourceRootName(ResourceElement &);

private:
    ResourceElement _root;
    std::stack<ResourceElement> _elements;
    rsResourceSet& _resource;		
	bool _gotChars ;
	bool _writeRoot ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(XMLResourceHandler) ;

};


_ACS_END_NAMESPACE


#endif //_XMLResourceHandler_H_

