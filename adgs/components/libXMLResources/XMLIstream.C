// PRQA S 1050 EOF
/*

	Copyright 1995-2018, Advanced Computer Systems , Inc.
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
	Revision 5.6  2018/09/03 11:09:13  marfav
	Using setLoadExternalDTD with the xml parser to avoid access
	to the declared DTD in the case the DTD checking has been disabled
	
	Revision 5.5  2017/01/13 10:37:38  marfav
	Xerces initialization moved in XMLInitializer singleton class
	
	Revision 5.4  2015/10/07 14:14:36  damdec
	XERCES_HAS_NAME_SPACE flag introduced for making the code compatible with old xerces versions.
	
	Revision 5.3  2014/05/26 16:37:40  marfav
	Thread safety improved
	Destructors performs try-catch and notify
	hpTrimmedString robustness improved
	
	Revision 5.2  2014/02/13 12:42:29  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
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
	
	Revision 2.16  2013/05/30 14:12:10  marpas
	way to get rid of xerces allocated resources
	
	Revision 2.15  2012/12/02 20:03:49  marpas
	qa rules
	
	Revision 2.14  2012/02/15 12:38:21  marpas
	potential memory corruption fixed
	
	Revision 2.13  2012/02/13 16:47:41  marpas
	refactoring
	
	Revision 2.12  2011/09/29 14:03:46  marpas
	instrumented to avoid codecheck crash
	
	Revision 2.11  2011/03/01 12:09:34  marpas
	GCC 4.4.x support
	
	Revision 2.10  2009/12/21 17:30:08  marpas
	root extraction is conditioned to XMLIstream::extractRoot(...) method
	default false
	
	Revision 2.9  2009/11/06 17:47:13  marpas
	GLOBAL Thread safety on XMLIstream Class added due to the global xerces XERCES_CPP_NAMESPACE::XMLPlatformUtils::Initialize() method
	
	Revision 2.8  2009/09/16 11:14:07  marpas
	XMLValidator used in XMLIstream
	xerces namespace management simplified
	
	Revision 2.7  2008/01/22 13:16:51  clanas
	fixed memory management for xerces > 2.3
	
	Revision 2.6  2008/01/18 18:55:56  clanas
	fixed namespace management for new xerces version
	
	Revision 2.5  2007/07/31 14:19:56  marpas
	encoding management fixed
	
	Revision 2.4  2007/07/13 13:41:16  marpas
	XMLOstream & XMLIstream classes now support encoding - see ctors
	
	Revision 2.3  2007/06/19 09:48:48  marpas
	default encoding is UTF-8
	applied even when geerating a new file
	
	Revision 2.2  2006/07/14 09:23:20  marpas
	documentation fixed and improved.
	global variables are now encaspulated into classes better than before
	interruption of saxExtract method allowed when the last data consumer leave.
	new test to show this feature added
	
	Revision 2.1  2006/03/17 16:14:21  danalt
	added RedirectHandler
	
	Revision 2.0  2006/02/28 08:37:31  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.11  2005/08/08 12:48:31  marpas
	diagnostics and realted tests improved
	
	Revision 1.10  2005/06/09 00:41:47  marpas
	XML filter functionality added
	
	Revision 1.9  2004/09/16 16:58:18  marpas
	Data consumer concept implemented and almost tested
	work still in progress
	
	Revision 1.8  2004/09/15 16:05:00  marpas
	improved
	
	Revision 1.7  2003/11/25 16:34:30  marfav
	Added doInputCheck and getInputSource virtual methods
	for reuse this class as super-class when implementing XML-Istream
	that read from input different by XML based files
	
	Revision 1.6  2003/07/11 13:00:56  marpas
	exException::clone implemented
	
	Revision 1.5  2003/07/01 08:06:17  marpas
	memory leak fixed
	
	Revision 1.4  2003/04/30 16:38:00  marpas
	using namespace std no longer into acs includes
	
	Revision 1.3  2003/01/21 17:54:15  giucas
	Solved bug for input file name as absolute path give segmentation fault
	
	Revision 1.2  2002/10/23 10:18:49  giucas
	Added missing import for xmlResV
	
	Revision 1.1.1.1  2002/10/17 14:26:51  giucas
	Import libXMLResources
	
*/

#include <XMLIstream.h>
#include <XMLResourceHandler.h>
#include <XMLExtractorHandler.h>
#include <XMLFilterHandler.h>
#include <XMLValidator.h>
#include <LocalSeekFileInputSource.h>

#include <rsResourceSet.h>
#include <RedirectHandler.h>

#include <XMLInitializer.h>

#include <xercesc/util/PlatformUtils.hpp> // PRQA S 1013
#include <memory>
#include <xmlResV.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

namespace {
    xmlResV version;
}


// -----------------------------------------------------------------
//
//
// constructor
//
// -----------------------------------------------------------------
XMLIstream::XMLIstream(const std::string& name , const std::string &encoding):
	_xmlFileName(name ),
	_seekPos(0),
	_doNamespaces(true),
	_doSchema(false),
	_schemaFullChecking(true),
	_valScheme(XERCES_CPP_NAMESPACE::SAXParser::Val_Auto), 
	_dataConsumer(),
	_encoding(encoding),
	_extenalSchemaValidation(false),
	_extractRoot(false)
{
    // No operations.. Just create the instance if needed
    XMLInitializer::instance();
}

XMLIstream::~XMLIstream() throw() {}



//
//  Executes a check on the input parameters
//  Throws XMLException if the chek fails
//

void
XMLIstream::doInputCheck()
{
	ACS_COND_THROW(!File::exists(_xmlFileName),  // PRQA S 3081
							  XMLIstreamException("XMLException: Input file \"" + 
							  					  _xmlFileName + 
												  "\" does not exist")
							  ) ;	
	
	ACS_COND_THROW(!File::isReadable(_xmlFileName),  // PRQA S 3081
							  XMLIstreamException("XMLException: Input file \"" + 
							  					  _xmlFileName + 
							  					  "\" is not readable")
							 ) ;	

	if (_extenalSchemaValidation) {
		XMLValidator validator(_xmlFileName) ;
		if (!validator.validate()) {
			ACS_THROW(XMLIstreamException("XMLException: Input file \"" +  // PRQA S 3081
							  					  _xmlFileName + 
							  				   "\" is not valid")) ;
		}
	}
}

//
// Returns the inputSource to be used as input to the SAX parser
// it is overriden in the derivate class for support other kinds of inputs
//
XERCES_CPP_NAMESPACE::InputSource* XMLIstream::getInputSource()
{
	XMLCh * xmlstr = XERCES_CPP_NAMESPACE::XMLString::transcode(_xmlFileName.c_str()) ;
	LocalSeekFileInputSource* inputSource = new LocalSeekFileInputSource(xmlstr,_seekPos);
#ifdef XERCES_HAS_NAME_SPACE
	XERCES_CPP_NAMESPACE::XMLString::release(&xmlstr);
#else
	delete[] xmlstr;
#endif
	return inputSource;
}

//
// activates SAX parse procedure of the input XML file
//
XMLIstream& XMLIstream::saxParser(rsResourceSet& outResource)
{

	doInputCheck();
		
	
	// check XML file SAX parse
	
	try{

		//
		// Attention!! Resource passed to XMLResourceHandler must be empty.
		//
		rsResourceSet tmpRes ;
		{	
			//
			// Attention!! Resource passed to XMLResourceHandler must be empty.
			//
			// instance XMLResourceHandler class
			XMLResourceHandler  resourceHandler(tmpRes);
			resourceHandler.writeRoot(_extractRoot) ;
			//
			// Attention!! Resource passed to XMLResourceHandler must be empty.
			//

			// instance SAXParser class

			XERCES_CPP_NAMESPACE::SAXParser parser ;

			parser.setValidationScheme(_valScheme);
			// This method allows users to enable or disable the loading of external DTD. 
			// When set to false, the parser will ignore any external DTD completely if the validationScheme is set to Val_Never.
			// In case of Val_Always or Val_Auto (default case for this library) it will be ignored and DTD loaded anyway
			parser.setLoadExternalDTD(false);

			parser.setDoNamespaces(_doNamespaces);
			parser.setDoSchema(_doSchema);
			parser.setValidationSchemaFullChecking(_schemaFullChecking);


			parser.setDocumentHandler(&resourceHandler);
			parser.setErrorHandler(&resourceHandler); 

			//
			// Handle Redirection of base DTD path
			// if not enabled by config space does not make any effect
			//
			RedirectHandler redirectHandler;
			parser.setEntityResolver(&redirectHandler); 


			// Use RAII for declare-and-forget the new input resource

#if __cplusplus > 199711L
			unique_ptr<XERCES_CPP_NAMESPACE::InputSource> inputSource(getInputSource());
#else
			auto_ptr<XERCES_CPP_NAMESPACE::InputSource> inputSource(getInputSource());
#endif

			parser.parse(*inputSource);


			// merging between input resource and temporary resource
			outResource.merge(tmpRes);

			// call the parser termination method
		}
	}
	catch (const XERCES_CPP_NAMESPACE::XMLException& toCatch) {
	
		 string t ;
		 XMLIstreamException e("XMLException: " + XMLResourceHandler::hpTrimmedString(toCatch.getMessage(),t)) ;		
		 ACS_THROW(e);
	}
	catch (const XERCES_CPP_NAMESPACE::SAXParseException& toCatch) {

         string t ;
         XMLIstreamException e("SAXParseException: " + XMLResourceHandler::hpTrimmedString(toCatch.getMessage(),t)) ;		
         ACS_THROW(e);
	}
	catch (exception &) {
        throw;
	}
	
	return *this ;	
}


void XMLIstream::addConsumer(XMLDataConsumer *dc) 
{
	if (dc) {
		vector<XMLDataConsumer *> vdc ;
		vdc.push_back(dc) ;
		addConsumer(vdc) ;
	}
}

void XMLIstream::addConsumer(std::vector<XMLDataConsumer *> &vdc) 
{
    size_t vdc_size = vdc.size() ;
	for (size_t i=0; i < vdc_size; ++i) {
		if (vdc[i]) {
			_dataConsumer.push_back(vdc[i]) ;
		}
	}
}

		


//
// activates SAX parse procedure of the input XML file
//
void XMLIstream::saxExtract()
{

	doInputCheck();
		
	
	// check XML file SAX parse
	
	try{
		// instance XMLResourceHandler class
		XMLExtractorHandler  xhandler(_dataConsumer) ;

		// instance SAXParser class

		XERCES_CPP_NAMESPACE::SAXParser parser ;

		parser.setValidationScheme(_valScheme);
		// This method allows users to enable or disable the loading of external DTD. 
		// When set to false, the parser will ignore any external DTD completely if the validationScheme is set to Val_Never.
		// In case of Val_Always or Val_Auto (default case for this library) it will be ignored and DTD loaded anyway
		parser.setLoadExternalDTD(false);

		parser.setDoNamespaces(_doNamespaces);
		parser.setDoSchema(_doSchema);
		parser.setValidationSchemaFullChecking(_schemaFullChecking);

		parser.setDocumentHandler(&xhandler);
		parser.setErrorHandler(&xhandler); 

		//
		// Handle Redirection of base DTD path
		// if not enabled by config space does not make any effect
		//
		RedirectHandler redirectHandler;
		parser.setEntityResolver(&redirectHandler); 

		// Use RAII for declare-and-forget the new input resource
#if __cplusplus > 199711L
		unique_ptr<XERCES_CPP_NAMESPACE::InputSource> inputSource(getInputSource());
#else
		auto_ptr<XERCES_CPP_NAMESPACE::InputSource> inputSource(getInputSource());
#endif

		parser.parse(*inputSource);
	
	}
	catch (const XMLHandler::XMLHandlerInterrupt& toCatch) {
		 // nothing to do: was terminated
	}
	catch (const XERCES_CPP_NAMESPACE::XMLException& toCatch) {

		 string t ;
		 XMLIstreamException e("XMLException: " + XMLResourceHandler::hpTrimmedString(toCatch.getMessage(),t)) ;		
		 ACS_THROW(e);
	}
	catch (const XERCES_CPP_NAMESPACE::SAXParseException& toCatch) {

		 string t ;
		 XMLIstreamException e("SAXParseException: " + XMLResourceHandler::hpTrimmedString(toCatch.getMessage(),t)) ;		
		 ACS_THROW(e);
	}
	catch (exception &) {
		 throw;
	}
}


//
// activates SAX parse procedure of the input XML file
//
XMLIstream& XMLIstream::saxCopy(const string &path)
{

	doInputCheck();
		
	
	// check XML file SAX parse
	
	try{
		// instance XMLResourceHandler class

		XMLFilterHandler  xhandler(_dataConsumer, path, _encoding, XERCES_CPP_NAMESPACE::XMLFormatter::UnRep_CharRef) ;
			
		// instance SAXParser class
		XERCES_CPP_NAMESPACE::SAXParser parser ;

		parser.setValidationScheme(_valScheme);
		// This method allows users to enable or disable the loading of external DTD. 
		// When set to false, the parser will ignore any external DTD completely if the validationScheme is set to Val_Never.
		// In case of Val_Always or Val_Auto (default case for this library) it will be ignored and DTD loaded anyway
		parser.setLoadExternalDTD(false);

		parser.setDoNamespaces(_doNamespaces);
		parser.setDoSchema(_doSchema);
		parser.setValidationSchemaFullChecking(_schemaFullChecking);

		parser.setDocumentHandler(&xhandler);
		parser.setErrorHandler(&xhandler); 

		//
		// Handle Redirection of base DTD path
		// if not enabled by config space does not make any effect
		//
		RedirectHandler redirectHandler;
		parser.setEntityResolver(&redirectHandler); 


		// Use RAII for declare-and-forget the new input resource
#if __cplusplus > 199711L
		unique_ptr<XERCES_CPP_NAMESPACE::InputSource> inputSource(getInputSource());
#else
		auto_ptr<XERCES_CPP_NAMESPACE::InputSource> inputSource(getInputSource());
#endif
		parser.parse(*inputSource);
	}
	catch (const XERCES_CPP_NAMESPACE::XMLException& toCatch) {

		 string t ;
		 ACS_THROW(XMLIstreamException("XMLException: " + XMLResourceHandler::hpTrimmedString(toCatch.getMessage(),t))); // PRQA S 3081
	}
	catch (const XERCES_CPP_NAMESPACE::SAXParseException& toCatch) {
		 string t ;
		 ACS_THROW(XMLIstreamException("SAXParseException: " + XMLResourceHandler::hpTrimmedString(toCatch.getMessage(),t))); // PRQA S 3081
	}
	catch (exception &) {
		 throw;
	}
	
	return *this ;
}


//
//
//
// this "global" function read resource from an XMLIstream and fills a resourceSet
//
//
XMLIstream& operator>>(XMLIstream& xmlIstream, rsResourceSet& outResource) // PRQA S 4222
{
	return xmlIstream.saxParser(outResource);
}

//
//
//
// this "global" function read resource from an XMLIstream and fills a resourceSet
//
//
XMLIstream& operator>>(XMLIstream& xmlIstream, const string &path) // PRQA S 4222
{
	return xmlIstream.saxCopy(path);
}



void XMLIstream::seekg(int pos) throw() // PRQA S 4121
{
	_seekPos = pos;
}


//setting methods for schema validation flags

void XMLIstream::setDoNamespaces(bool doNamespace) throw() // PRQA S 4121
{
	_doNamespaces = doNamespace;
}

void XMLIstream::setDoSchema(bool doSchema) throw() // PRQA S 4121
{
	_doSchema = doSchema;
}

void XMLIstream::setValidationSchemaFullChecking(bool schemaFullCheck) throw() // PRQA S 4121
{
	_schemaFullChecking = schemaFullCheck;
}

void XMLIstream::setValidationSchema(const XERCES_CPP_NAMESPACE::SAXParser::ValSchemes valSchema) // PRQA S 4121
{
	_valScheme = valSchema;
}

void XMLIstream::setDoExternalSchemaValidation(bool v) // PRQA S 4121
{
	_extenalSchemaValidation=v ;
}

void XMLIstream::extractRoot(bool v) throw() // PRQA S 4121
{
	_extractRoot = v;
}



_ACS_END_NAMESPACE
