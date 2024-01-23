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
	Revision 5.2  2013/12/05 17:10:23  lucio.pulvirenti
	MP: coding best practices applied.
	
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
	
	Revision 2.6  2012/12/02 20:03:49  marpas
	qa rules
	
	Revision 2.5  2012/02/13 16:47:41  marpas
	refactoring
	
	Revision 2.4  2009/11/13 10:13:20  marpas
	useless forward declaration fixed
	
	Revision 2.3  2009/09/16 11:14:07  marpas
	XMLValidator used in XMLIstream
	xerces namespace management simplified
	
	Revision 2.2  2008/01/18 18:55:55  clanas
	fixed namespace management for new xerces version
	
	Revision 2.1  2006/07/14 09:23:20  marpas
	documentation fixed and improved.
	global variables are now encaspulated into classes better than before
	interruption of saxExtract method allowed when the last data consumer leave.
	new test to show this feature added
	
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

#ifndef _XMLDataConsumer_H_
#define _XMLDataConsumer_H_

#include <acs_c++config.hpp>
#include <XMLHandler.h>
#include <exException.h>
#include <string>

_ACS_BEGIN_NAMESPACE(acs)

class XMLExtractorHandlerBase ;

//////////////////////////////////
//								//
//   classe XMLDataConsumer		//
//								//
//////////////////////////////////

/**
 * This class is the base for a user defined data consumer.
 * Conventionally, each xml node name is attached to its children with a '/' character
 * e.g. \<parent\>\<child\> will result for parent/child path for the node child.
 */	

class XMLDataConsumer { // PRQA S 2109
public:

	/**
	 * \brief  Class constructor 
	 * 
	 */	
	 XMLDataConsumer();

	/**
	 * \brief  Class destructor 
	 */			
	virtual ~XMLDataConsumer() throw() ;

	/** 
	 * The method is called during the parse action of an XML file
	 * If the method returns true, the class instance will be called when there will be values \see nodeValue()
	 * \param h the calling extractor handler.
	 * \param father the parent xml node path 
	 * \param node the current node
	 * \param l the node attributes list.
	 * \param name the name.
	 *
	 * The base implementation calls and returns nodematch(h,father+"/"+node,l) 
	 *
	 * \b Example
	 * \code
	 * bool MySatDataConsumer::nodeMatch(XMLExtractorHandlerBase &, const string &father, const string &node, const AttributeList &) 
	 * {
	 *     // return true if the current node is exactly GPolygon, PassStartEquatorXDeltaTime or the father node contains GPolygon
	 *     // the reverse find on the latter case is to speedup the search knowing the XML data struture 
	 *     return node == "GPolygon" || node == "PassStartEquatorXDeltaTime" || father.rfind("GPolygon") != string::npos ;
	 * }
	 * 
	 * \endcode
	 */
	virtual bool nodeMatch(XMLExtractorHandlerBase &h, const std::string &father, const std::string &node, const XERCES_CPP_NAMESPACE::AttributeList &l, const XMLCh* name = 0) ;	



	/** 
	 * \see nodeMatch(XMLExtractorHandlerBase &h, const std::string &father, const std::string &node, const AttributeList &l, const XMLCh* name = 0)
	 *
	 * If the method returns true, the class instance will be called when there will be values \see nodeValue()
	 * \param h the calling extractor handler.
	 * \param nodepath the parent xml node path (complete)
	 * \param l the node attributes list.
	 * \param name the name
	 *
	 * the base implementation returns \a false 
	 *
	 * \b Example
	 * \code
	 * bool MyDataConsumer::nodeMatch(XMLExtractorHandlerBase &h, const string &nodepath, const AttributeList &) 
	 * {
	 *    if (!nodepath.find(_node)) { // a specific node or its children
	 *  	  if (_node==nodepath) { // exactly that one 
	 *  		  if (_counter >= _min && (!_max || _counter <=_max)) { // exactly the instance we are interested in
	 *  			  _counter++ ; 
	 *  			  pushConsumer(h,&_MySatDataConsumer) ; // push a new consumer
	 *   			  _added = true ; // remember it
	 *   			  _counter2++ ;
	 *   			  return true ; // notify we are still interested
	 *   		  }
	 *   		  _counter ++ ;
	 *   	  }
	 *   	  return true ; // notify we are still interested
	 *     }
	 *     return false ; // notify we are not interested
	 * }   
	 * \endcode
	 *
	 * \note AttributeList is an \a XERCES type the following example explain how to deal with
	 * \see XMLHandler::hpTrimmedString XMLHandler::toString
	 *
	 * \b Example
	 * \code
	 *
	 * #include <xercesc/sax/AttributeList.hpp>
	 *
	 * void XMLResourceHandler::startElement(const XMLCh* name, AttributeList& attributes)
	 * {
	 *     ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
	 *  	   string t ;
	 *  	   ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Found element: " << hpTrimmedString(name,t) )  ;
	 *     ACS_CLASS_END_DEBUG   
	 *      
	 *     ResourceElement elem;
	 *     string en ;
	 *     elem.setName(hpTrimmedString(name,en));
	 *     for (unsigned int i=0; i < attributes.getLength(); i++)
	 *     {
	 *  	   string n,v,t ;
	 *         elem.setAttribute(XMLHandler::hpTrimmedString(attributes.getName(i),n),
	 *                           XMLHandler::hpTrimmedString(attributes.getValue(i),v),
	 *                           XMLHandler::hpTrimmedString(attributes.getType(i),t));
	 *     }
	 *     _elements.push(elem);
	 *
	 * }
	 *
	 * 
	 * \endcode
	 */
	virtual bool nodeMatch(XMLExtractorHandlerBase &h, const std::string &nodepath, const XERCES_CPP_NAMESPACE::AttributeList &l, const XMLCh* name = 0);	



	/** 
	 * \see nodeMatch(XMLExtractorHandlerBase &eh, const std::string &parent, const std::string &node, const AttributeList &)
	 *
	 * The method is called when an XML element ends. 
	 * \param h the calling extractor handler.
	 * \param father the parent xml node path 
	 * \param node the current node
	 * \param value the node value.
	 * \param name the name.
	 *
	 * The base implementation calls nodeValue(eh,parent+"/"+node,value) 
	 *
	 * \b Example
	 * \code
	 * void MySatDataConsumer::nodeValue(XMLExtractorHandlerBase &, const string &parent, const string &node, const string &value) 
	 * {
	 *     if (node == "GPolygon") // count the GPolygon elements ... note the node shall be exactly GPolygon not its children
	 *  	   countGPolygon++ ;
	 *     else if (node == "PassStartEquatorXDeltaTime") // count the PassStartEquatorXDeltaTime elements
	 *  	   countPassStartEquatorXDeltaTime++ ;
	 * }
	 *
	 * 
	 * \endcode
	 */
	virtual bool nodeValue(XMLExtractorHandlerBase &h, const std::string &father, const std::string &node, const std::string &value, const XMLCh* name = 0) ;



	/** 
	 * \see nodeMatch(XMLExtractorHandlerBase &h, const std::string &father, const std::string &node, const AttributeList &)
	 *
	 * The method is called when an XML element ends. 
	 * \param h the calling extractor handler.
	 * \param nodepath the parent xml node path (complete)
	 * \param value the node value.
	 * \param name the name.
	 *
	 * \b Example
	 * \code
	 * void MyDataConsumer::nodeValue(XMLExtractorHandlerBase &xh, const string &node, const string &value) 
	 * {
	 *     if (_node==node) { // that node ended
	 *  	   _values.push_back(atoi(value.c_str())) ; // add its value as integer into a vector
	 *  	   if(_added) { // a previous consumer was added
	 *  		   popConsumer(xh,&_satDataCons) ; // it is time to remove it
	 *  		   _added = false ; // forget it
	 *  	   }
	 *     }
	 * }   
	 * 
	 * \endcode
	 */
	virtual bool nodeValue(XMLExtractorHandlerBase &h, const std::string &nodepath, const std::string &value, const XMLCh* name = 0) ;


 #if ( XERCES_VERSION_MAJOR > 2 )
	virtual bool characters(XMLExtractorHandlerBase &eh, const XMLCh* chars, const XMLSize_t lenght) ;
 #else
	virtual bool characters(XMLExtractorHandlerBase &eh, const XMLCh* chars, const unsigned int lenght) ;
 #endif


protected:

	/** 
	  * This method can be used to push a temporary consumer into an \a XMLExtractorHandlerBase.
	  *
	  * \note Only the consumer that pushed another one can pop the latter to avoid errors during XML parse action.
	  * \param xh the extractor handler on which the consumer has to be installed.
	  * \param c the consumer to install. \note the consumer cannot be deleted until removed \see popConsumer()
	  */
	void pushConsumer(XMLExtractorHandlerBase &xh, XMLDataConsumer *c) ;

	/** 
	  * This method can be used to push a bunch of temporary consumers into an \a XMLExtractorHandlerBase.
	  *
	  * \note Only the consumer that pushed another one can pop the latter to avoid errors during XML parse action.
	  * \param xh the extractor handler on which the consumers have to be installed.
	  * \param c the bunch of consumers to install. \note the consumers cannot be deleted until removed \see popConsumer()
	  */
	void pushConsumer(XMLExtractorHandlerBase &xh, std::vector<XMLDataConsumer *> &c) ;

	/** 
	  * This method can be used to remove a temporary consumer from an \a XMLExtractorHandlerBase.
	  *
	  * \note Only the consumer that pushed another one can pop the latter to avoid errors during XML parse action. \see pushConsumer()
	  * \param xh the extractor handler from which the consumer has to be removed.
	  * \param c the consumer to remove. \note after the method returned the consumer can be deleted.
	  */
	void popConsumer(XMLExtractorHandlerBase &xh, XMLDataConsumer *c) ;

	/** 
	  * This method can be used to remove a bunch of temporary consumer from an \a XMLExtractorHandlerBase.
	  *
	  * \note Only the consumer that pushed another one can pop the latter to avoid errors during XML parse action. \see pushConsumer()
	  * \param xh the extractor handler from which the consumers have to be removed.
	  * \param c the consumers to remove. \note after the method returned the consumers can be deleted.
	  */
	void popConsumer(XMLExtractorHandlerBase &xh, std::vector<XMLDataConsumer *> &c) ;

private:
	// the copy ctor and assignement operator are declared private to prevent their use.
	// They are, hence, not implemented
	XMLDataConsumer(const XMLDataConsumer &); 
	XMLDataConsumer &operator=(const XMLDataConsumer &); 
private:	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(XMLDataConsumer) ;

} ;


_ACS_END_NAMESPACE



#endif //_XMLDataConsumer_H_

