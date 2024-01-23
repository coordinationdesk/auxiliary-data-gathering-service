// PRQA S 1050 EOF
/*

	Copyright 1995-2017, Advanced Computer Systems , Inc.
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
	Revision 5.9  2017/10/11 10:39:42  lucio.pulvirenti
	PRS-56: validateStructure pubilc method added to check xml structure with xmllint. spawnStructureValidator and spawnCommonValidator
	private methods added.
	
	Revision 5.8  2014/08/01 13:05:40  lucio.pulvirenti
	ADC-270: static const variable added for default of maximum length of XMLValidator tool output.
	
	Revision 5.7  2014/08/01 10:46:34  lucio.pulvirenti
	ADC-270 work in progress.
	
	Revision 5.6  2014/05/14 14:11:55  marpas
	XMLValidator::validate method now can reports if a schema was used to validate the file.
	Note that if a schema was found does not affect the validation function that can report the file does not validate.
	
	Revision 5.5  2013/12/05 17:22:50  lucio.pulvirenti
	xsi:noNamespaceSchemaLocation attribute now managed.
	xsi:schemaLocation values managed according to XML standard: if relative path sticked to redirectPath, if URL not sticked. Backward compatibility saved.
	
	Revision 5.4  2013/12/04 17:26:03  lucio.pulvirenti
	Work in progress.
	
	Revision 5.3  2013/12/03 17:04:54  lucio.pulvirenti
	Work in progress.
	
	Revision 5.2  2013/12/02 18:17:02  lucio.pulvirenti
	Work in progress.
	
	Revision 5.1  2013/11/28 15:24:34  lucio.pulvirenti
	SchemaExtractor: debug macro added.
	
	Revision 5.0  2013/07/18 17:54:35  marpas
	adopting libException 5.x standards
	performances improved
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.6  2013/06/03 13:13:34  lucio.pulvirenti
	setSchemaName module added.
	
	Revision 2.5  2012/12/02 20:03:49  marpas
	qa rules
	
	Revision 2.4  2012/02/13 16:47:41  marpas
	refactoring
	
	Revision 2.3  2009/09/16 11:25:30  marpas
	old xerces w/o namespace fixing
	
	Revision 2.2  2009/09/16 11:14:07  marpas
	XMLValidator used in XMLIstream
	xerces namespace management simplified
	
	Revision 2.1  2009/09/14 12:51:52  marpas
	XMLValidator class added
	unitary test added
	other test improved
	
*/

#ifndef _XMLValidator_H_
#define _XMLValidator_H_

#include <acs_c++config.hpp>
#include <XMLHandler.h>
#include <xercesc/parsers/SAXParser.hpp> // PRQA S 1013

#ifndef XERCES_HAS_NAME_SPACE
	#define XERCES_CPP_NAMESPACE
#endif

_ACS_BEGIN_NAMESPACE(acs)

class XMLValidator { // PRQA S 2109
public:
	class SchemaExtractor: // PRQA S 2109, 2153
		public XMLHandler {
	public:

		SchemaExtractor() ;

		virtual ~SchemaExtractor() throw() ;

		virtual void endDocument() ;
		virtual void endElement( const XMLCh* el) ;

#if ( XERCES_VERSION_MAJOR > 2 )
		virtual void characters( const XMLCh* el, const XMLSize_t len) ;
#else 
		virtual void characters( const XMLCh* el, const unsigned int len) ;
#endif

		virtual void startElement( const XMLCh* name, XERCES_CPP_NAMESPACE::AttributeList& attributes);

		/// Receive notification of the beginning of an XML document.
		virtual void startDocument();

		bool schemaNameFound() const ;
		const std::string &getSchemaName() const throw() ;
		const bool &getNoNsSchemaLocation() const throw() ;
		const bool &multipleSchemas() const throw() ;
	private:
		// declared but not implemented 
		SchemaExtractor(const SchemaExtractor &); 
		SchemaExtractor &operator=(const SchemaExtractor &); 

	private:
		std::string _schemaName ;
		bool _noNsSchemaLocation;
		bool _multipleSchemas;
		ACS_CLASS_DECLARE_DEBUG_LEVEL(SchemaExtractor) ;
	};

public:


	explicit XMLValidator(const std::string& path);
	virtual ~XMLValidator() throw() ;

	std::string getSchemaName(bool fullPath=false) ;
	void setSchemaName(const std::string& ) ;
    
    /*- validate the file against a schema.
        The latter can be extracted from file or set from external.
        If neither (internal or set from external) conditions are met, the validate method returns true without run the validator, but the file xml is formally correct.
        \params raiseException if set to true (default) when the validator function does not validate, raise an exception, 
                else returns simly true or false according to the validate function result.
        \params schemaExists, a pointer to a boolean that will be filled with true or false to signal 
                that the validation function has been called, meaning that a schema was found (internally or from external)
     */
	bool validate(bool raiseException=true, bool *schemaExists = 0 ) ;

    /*- validate the xml file structure: no schema passed
        \params raiseException if set to true (default) when the validator function does not validate, raise an exception, 
                else returns simly true or false according to the validate function result.
     */
	bool validateStructure(bool raiseException=true) ;

private:

	// declared but not implemented 
	XMLValidator(); 
	XMLValidator(const XMLValidator &); 
	XMLValidator &operator=(const XMLValidator &); 

	int spawnValidator(const std::string &, std::string &) const ;
	int spawnStructureValidator(std::string & outstring) const ;
	int spawnCommonValidator(std::string & outstring, const std::string & schemaPath = std::string() ) const ;
	std::string getSchemaPath(const std::string& schemaName, const std::string& redirectPath) const;
	

	bool isLocalProtocol(const std::string & protocol) const;

private:

	std::string  _xmlFileName;
	std::string  _xmlSchemaName;
	bool _noNamespaceSchemaLocation;
	bool _multipleSchemasOccurring;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(XMLValidator) ;
	
	static const std::string _protocolSeparator; // "://"
	static const std::vector<std::string> _localProtocolsVect ;
	static const std::string _localProtocolConfKey ; //  = "List_Of_LocalProtocols.LocalProtocol"
	static const size_t _validatorToolMaxOutputLength = 102400; // 100 Kbytes, the maximum length of XML Validator tool (xmllint) output

};

_ACS_END_NAMESPACE


#endif //_XMLValidator_H_






