// PRQA S 1050 EOF
/*

	Copyright 1995-2014, Advanced Computer Systems , Inc.
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
	
	Revision 2.11  2013/05/30 14:11:47  marpas
	coding best practices applied
	
	Revision 2.10  2012/12/02 20:03:49  marpas
	qa rules
	
	Revision 2.9  2012/02/13 16:47:41  marpas
	refactoring
	
	Revision 2.8  2009/12/21 17:30:08  marpas
	root extraction is conditioned to XMLIstream::extractRoot(...) method
	default false
	
	Revision 2.7  2009/11/06 17:47:13  marpas
	GLOBAL Thread safety on XMLIstream Class added due to the global xerces XERCES_CPP_NAMESPACE::XMLPlatformUtils::Initialize() method
	
	Revision 2.6  2009/09/16 11:25:29  marpas
	old xerces w/o namespace fixing
	
	Revision 2.5  2009/09/16 11:14:07  marpas
	XMLValidator used in XMLIstream
	xerces namespace management simplified
	
	Revision 2.4  2008/01/18 18:55:56  clanas
	fixed namespace management for new xerces version
	
	Revision 2.3  2007/07/31 14:19:56  marpas
	encoding management fixed
	
	Revision 2.2  2007/07/13 13:41:16  marpas
	XMLOstream & XMLIstream classes now support encoding - see ctors
	
	Revision 2.1  2006/07/14 09:23:20  marpas
	documentation fixed and improved.
	global variables are now encaspulated into classes better than before
	interruption of saxExtract method allowed when the last data consumer leave.
	new test to show this feature added
	
	Revision 2.0  2006/02/28 08:37:31  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.8  2005/06/09 00:41:47  marpas
	XML filter functionality added
	
	Revision 1.7  2004/09/17 20:07:43  marpas
	Data extraction almost complete, beta release
	
	Revision 1.6  2004/09/16 16:58:18  marpas
	Data consumer concept implemented and almost tested
	work still in progress
	
	Revision 1.5  2003/11/25 16:34:30  marfav
	Added doInputCheck and getInputSource virtual methods
	for reuse this class as super-class when implementing XML-Istream
	that read from input different by XML based files
	
	Revision 1.4  2003/07/11 13:00:56  marpas
	exException::clone implemented
	
	Revision 1.3  2003/04/30 16:38:00  marpas
	using namespace std no longer into acs includes
	
	Revision 1.2  2003/01/21 17:54:20  giucas
	Solved bug for input file name as absolute path give segmentation fault
	
	Revision 1.1.1.1  2002/10/17 14:26:51  giucas
	Import libXMLResources
	
*/

#ifndef _XMLIstream_H_
#define _XMLIstream_H_

#include <acs_c++config.hpp>
#include <exException.h>
#include <xercesc/parsers/SAXParser.hpp> // PRQA S 1013
#include <File.h>
#include <ThreadSafe.h>
#ifndef XERCES_HAS_NAME_SPACE
	#define XERCES_CPP_NAMESPACE
#endif


_ACS_BEGIN_NAMESPACE(acs)

class XMLDataConsumer ;

class rsResourceSet ;

	//////////////////////////////////
	//								//
	//      classe XMLIstream		//
	//								//
	//////////////////////////////////
	
	/**
	 * This class activates SAX parse procedure of the input XML document 
	 * and its wrapping in a \a rsResourceSet target.
	 */	

class XMLIstream { // PRQA S 2109

public:


	/*! class acs::XMLIstreamException declaration */
	exDECLARE_EXCEPTION(XMLIstreamException,exException) ;// PRQA S 2131, 2502

	/**
	 * \brief  Class constructor 
	 * 
	 * \param  name Reference to std::string object which defines XML document name.   
	 * \param  encoding type of encoding in the XML.   
	 */	
	explicit XMLIstream(const std::string& name, const std::string &encoding="UTF-8");

	/**
	 * \brief  Class destructor 
	 */			
	virtual ~XMLIstream() throw() ;

	/**
	 * \brief  Activates SAX parse procedure of the input XML document and its wrapping in a \a rsResourceSet target. 
	 *
	 * In this method \a SAXParser class is istanced. The \a DocumentHandler 
	 * and \a ErrorHandler instances required by SAX API are provided using 
	 * an instance of \a XMLResourceHandler class (derived from \a XMLHandler class).
	 * \param outResource Reference to \a rsResourceSet object which defines \a rsResourceSet target which XML document is wrapped in. 
	 * \return Reference to the \a XMLIstream object itself. 
	 */			
	XMLIstream& saxParser(rsResourceSet & outResource);

	/**
	 * \brief  Activates SAX parse procedure of the input XML document and its wrapping in a \a rsResourceSet target. 
	 *
	 * In this method \a SAXParser class is istanced. The \a DocumentHandler 
	 * and \a ErrorHandler instances required by SAX API are provided using 
	 * an instance of \a XMLResourceHandler class (derived from \a XMLHandler class).
	 * \param path Reference to \a path file that will contain the output. 
	 * \return Reference to the \a XMLIstream object itself. 
	 */			
	XMLIstream& saxCopy(const std::string & path );

	void addConsumer(XMLDataConsumer *) ;
	void addConsumer(std::vector<XMLDataConsumer *> &) ;

	/**
	 * \brief  Activates SAX parse procedure of the input XML document. 
	 *
	 * In this method \a SAXParser class is istanced. The \a DocumentHandler 
	 * and \a ErrorHandler instances required by SAX API are provided using 
	 * an instance of \a XMLExtractorHandlerBase class (derived from \a XMLHandler class).
	 */			
	void saxExtract() ;

	void seekg(int) throw() ;

	void setDoNamespaces(bool) throw() ;
	void setDoSchema(bool) throw() ;
	void setValidationSchemaFullChecking(bool) throw() ;
	void setValidationSchema(const XERCES_CPP_NAMESPACE::SAXParser::ValSchemes);
	virtual void setDoExternalSchemaValidation(bool) ;
	
	void extractRoot(bool) throw() ;

protected:
	virtual XERCES_CPP_NAMESPACE::InputSource* getInputSource();

	virtual void doInputCheck();

private:
	XMLIstream(); 

	// copy contructor
	XMLIstream(const XMLIstream &); 

	// operator =
	XMLIstream &operator=(const XMLIstream &); 

private:

	std::string  _xmlFileName;

	int _seekPos;


	//  doNamespaces
	//      Indicates whether namespace processing should be enabled or not.
	//      Defaults to disabled.
	bool _doNamespaces;

	//
	//  doSchema
	//      Indicates whether schema processing should be enabled or not.
	//      Defaults to disabled.
	bool _doSchema;

	//
	//  schemaFullChecking
	//      Indicates whether full schema constraint checking should be enabled or not.
	//      Defaults to disabled.
	bool _schemaFullChecking;

	//  valScheme
	//      Indicates what validation scheme to use. It defaults to 'auto', but
	//      can be set via the -v= command.
	XERCES_CPP_NAMESPACE::SAXParser::ValSchemes _valScheme; 
	

	std::vector<XMLDataConsumer *> _dataConsumer ;
	const std::string _encoding ;
	bool _extenalSchemaValidation ;
	bool _extractRoot ;
};

/**
 *  This "global" function extracts resources from an XMLIstream document.
 *
 * \param  xmlIstream Reference to the \a XMIstream object which is wrapped in the \a rsResourceSet target.
 * \param  outResource Reference to the \a rsResourceSet target that will be filled.
 * \return Reference to the \a XMLIstream object itself. 
 */	
XMLIstream& operator>>(XMLIstream &xmlIstream, rsResourceSet & outResource); // PRQA S 2072

/**
 *  This "global" function parse an XMLIstream document into the given file.
 *
 * \param  xmlIstream to the \a XMIstream object which is wrapped in the \a rsResourceSet target.
 * \param  path Reference to the \a file path.
 * \return Reference to the \a XMLIstream object itself. 
 */	
XMLIstream& operator>>(XMLIstream &xmlIstream, const std::string &path); // PRQA S 2072

_ACS_END_NAMESPACE

#endif //_XMLIstream_H_






