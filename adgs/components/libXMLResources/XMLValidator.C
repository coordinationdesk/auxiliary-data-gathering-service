// PRQA S 1050 EOF
/*

	Copyright 2009-2017, Advanced Computer Systems , Inc.
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
	Revision 5.15  2017/10/11 10:42:40  lucio.pulvirenti
	PRS-56: validateStructure pubilc method added to check xml structure with xmllint. spawnCommonValidator private method added to be called
	with schema path by spawnValidator method. spawnStructureValidator private method added to call spawnCommonValidator with no schema path.
	
	Revision 5.14  2017/01/13 10:37:38  marfav
	Xerces initialization moved in XMLInitializer singleton class
	
	Revision 5.13  2016/01/28 14:46:59  davide.tiriticco
	removing unused parameters name in functions
	
	Revision 5.12  2015/03/03 09:58:39  marpas
	fixing qa warnings
	coding best practices applied
	
	Revision 5.11  2014/12/17 11:45:31  marfav
	Fixed management of FD closure
	
	Revision 5.10  2014/08/01 13:17:28  lucio.pulvirenti
	ADC-270 spawnValidator: the pipe where stderr and stdout is redirected is read during the process execution and after the process end
	to get possible bytes left. An output truncation mechanism has been implemented by introducing a configurable maximum ouput length
	 (default 100 KB). Truncation can be performed only if endl char is reached.
	
	Revision 5.9  2014/08/01 10:46:34  lucio.pulvirenti
	ADC-270 work in progress.
	
	Revision 5.8  2014/05/14 14:11:55  marpas
	XMLValidator::validate method now can reports if a schema was used to validate the file.
	Note that if a schema was found does not affect the validation function that can report the file does not validate.
	
	Revision 5.7  2014/02/13 12:42:30  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.6  2013/12/05 17:22:50  lucio.pulvirenti
	xsi:noNamespaceSchemaLocation attribute now managed.
	xsi:schemaLocation values managed according to XML standard: if relative path sticked to redirectPath, if URL not sticked. Backward compatibility saved.
	
	Revision 5.5  2013/12/04 17:26:03  lucio.pulvirenti
	Work in progress.
	
	Revision 5.4  2013/12/03 17:04:54  lucio.pulvirenti
	Work in progress.
	
	Revision 5.3  2013/12/02 18:17:02  lucio.pulvirenti
	Work in progress.
	
	Revision 5.2  2013/11/28 15:25:32  lucio.pulvirenti
	SchemaExtractor: debug macro added and dubug messages added.
	
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
	
	Revision 2.8  2013/06/03 13:13:34  lucio.pulvirenti
	setSchemaName module added.
	
	Revision 2.7  2012/02/13 16:47:41  marpas
	refactoring
	
	Revision 2.6  2011/03/02 12:17:59  marpas
	xerces 2 call removed
	
	Revision 2.5  2011/03/01 12:09:34  marpas
	GCC 4.4.x support
	
	Revision 2.4  2009/09/17 15:31:36  marfav
	Replacing dos-backslashes with unix-slashes while looking for schema filename
	
	Revision 2.3  2009/09/16 11:25:29  marpas
	old xerces w/o namespace fixing
	
	Revision 2.2  2009/09/16 11:14:07  marpas
	XMLValidator used in XMLIstream
	xerces namespace management simplified
	
	Revision 2.1  2009/09/14 12:51:52  marpas
	XMLValidator class added
	unitary test added
	other test improved
	
	
*/

#include <XMLValidator.h>
#include <RedirectHandler.h>
#include <StringUtils.h>
#include <ConfigurationSingleton.h>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <File.h>
#include <Timer.h>
#include <SpawnProc.h>
#include <Filterables.h>
#include <xercesc/framework/XMLPScanToken.hpp> // PRQA S 1013 2
#include <xercesc/util/PlatformUtils.hpp>
#include <algorithm>

#include <XMLInitializer.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(XMLValidator::SchemaExtractor)

XMLValidator::SchemaExtractor::SchemaExtractor() : XMLHandler(), _schemaName(), _noNsSchemaLocation(false), _multipleSchemas(false) {} ;

XMLValidator::SchemaExtractor::~SchemaExtractor() throw() {} ;

void XMLValidator::SchemaExtractor::endDocument() {} ;
void XMLValidator::SchemaExtractor::endElement( const XMLCh*) {} ;

#if ( XERCES_VERSION_MAJOR > 2 )
void XMLValidator::SchemaExtractor::characters( const XMLCh* , const XMLSize_t ) {} ;
#else 
void XMLValidator::SchemaExtractor::characters( const XMLCh* , const unsigned int ) {} ;
#endif

void XMLValidator::SchemaExtractor::startDocument() {} 

bool XMLValidator::SchemaExtractor::schemaNameFound() const { return _schemaName.length() != 0 ; }
		
const std::string &XMLValidator::SchemaExtractor::getSchemaName() const throw() { return _schemaName ;} // PRQA S 4120

const bool &XMLValidator::SchemaExtractor::getNoNsSchemaLocation() const throw() { return _noNsSchemaLocation ;} // PRQA S 4120

const bool &XMLValidator::SchemaExtractor::multipleSchemas() const throw() { return _multipleSchemas ;} // PRQA S 4120

void XMLValidator::SchemaExtractor::startElement(const XMLCh* , XERCES_CPP_NAMESPACE::AttributeList& attributes)
{
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Attribute list length: " << attributes.getLength() );
	bool schemaLocationFound = false;
	string attributeValue;
	
// for compatibility with the past, find first schemaLocation attr if any
    for (unsigned int i=0; i < attributes.getLength(); i++)
    {
		string n ;
		if (hpTrimmedString(attributes.getName(i),n) == "xsi:schemaLocation") {
			hpTrimmedString(attributes.getValue(i),attributeValue) ;
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "xsi:schemaLocation: \"" << attributeValue << "\"" );
			schemaLocationFound = true;
			break ;
    	}
	}
	
	for (unsigned int i=0; ( i < attributes.getLength() ) && !schemaLocationFound; i++) {
		string n ;
		if (hpTrimmedString(attributes.getName(i),n) == "xsi:noNamespaceSchemaLocation") {
			hpTrimmedString(attributes.getValue(i),attributeValue) ;
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "xsi:noNamespaceSchemaLocation: \"" << attributeValue << "\"" );
			_noNsSchemaLocation = true;
			break ;
    	}
	}

// As described in http://www.w3.org/TR/xmlschema-1/#schema-repr, xsi:schemaLocation may have multiple values. They are pairs composed by namespace - location URI /path
// In principle	even noNamespaceSchemaLocation attribute may have multiple values, even if I did not find it explicitely mentioned
	map<char, char> replacements;
	replacements.insert ( pair<char,char>('\n',' ') );
	replacements.insert ( pair<char,char>('\\','/') );
	attributeValue = StringUtils::transform(attributeValue, replacements);
	vector<string> values;
	{
        istringstream is(attributeValue) ;
        do {
            string a ; 
            is >> a ;
			if( a.empty() ) {
				continue;
			}
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "value extracted: \"" << a << "\"" );
			values.push_back(a);
        } while (is) ; // PRQA S 3050
	}


// *** xmllint tool used by the current class does not support xml files validation against multiple schemas ***
		 
// For that reason, if noNamespaceSchemaLocation attribute is used, exception is thrown since just introduced.	
	if( _noNsSchemaLocation && ( values.size() > 1 ) )  {
		ostringstream exMsg;
		exMsg << "Multiple schemas not managed: the following values occur in xsi:noNamespaceSchemaLocation attribute:\n";
		size_t v_size =  values.size() ;
        for( size_t i = 0; i < v_size; ++i ) {
			exMsg << "#" << i << " " << values[i] << "\n";
		}
		ACS_THROW( exIllegalValueException( exMsg.str() ) ); // PRQA S 3081
	}

	if( not values.empty() ) {
		_schemaName = values[values.size() - 1]; // PRQA S 3084
	}
	
// If schemaLocation attribute is used, in case of multiple schemas occur, just a WARNING is emitted and a flag is set, then try to find schema by sticking redirectPath value to
// File::getFileName of last attribute value as before.
	if( values.size() > 2 ) { // PRQA S 4400
		ACS_LOG_WARNING("Multiple schemas not managed: <" << values.size() << "> values occur in xsi:schemaLocation attribute, the last one only will be considered: \"" << _schemaName << "\"");
		_multipleSchemas = true;
	}
	else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "schema: \"" << _schemaName << "\"" );
	}

}

namespace {
	vector<string> createLocalProtocolsVect()
    {
      vector<string> v;
	  
	  v.push_back("fs");
	  v.push_back("file");
	  v.push_back("localhost");

      return v;
    }
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(XMLValidator)

const string XMLValidator::_protocolSeparator = "://";
const vector<string> XMLValidator::_localProtocolsVect = createLocalProtocolsVect();
const string XMLValidator::_localProtocolConfKey = "List_Of_LocalProtocols.LocalProtocol";



XMLValidator::XMLValidator(const std::string& path) :
    _xmlFileName(path), 
    _xmlSchemaName(),
	_noNamespaceSchemaLocation(false),
	_multipleSchemasOccurring(false)
{
}

XMLValidator::~XMLValidator() throw() {}

string XMLValidator::getSchemaName(bool fullPath) 
{
	if (_xmlSchemaName.empty()) {
        // Init XML infrastrucutre
        XMLInitializer::instance();

        SchemaExtractor  myHandler ;
 		
		XERCES_CPP_NAMESPACE::SAXParser parser ;
        parser.setDocumentHandler(&myHandler);
        parser.setErrorHandler(&myHandler); 
		
		XERCES_CPP_NAMESPACE::XMLPScanToken xmltoken ;
		bool ret = parser.parseFirst(_xmlFileName.c_str(),xmltoken) ;

		ACS_COND_THROW(!ret,XMLHandler::XMLHandlerException("Cannot extract schema name from: " + _xmlFileName)) ; // PRQA S 3081

		for (;ret;) {
			ret = parser.parseNext(xmltoken) ;
			if (myHandler.schemaNameFound()) {
				parser.parseReset(xmltoken) ;
				parser.resetDocument() ;
				break ;
			}
		}
		_xmlSchemaName = myHandler.getSchemaName() ;
		_noNamespaceSchemaLocation = myHandler.getNoNsSchemaLocation();
		_multipleSchemasOccurring = myHandler.multipleSchemas();
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "_xmlSchemaName: \"" << _xmlSchemaName << "\" - _noNamespaceSchemaLocation: " << boolalpha << _noNamespaceSchemaLocation 
											<< " - _multipleSchemasOccurring: " << boolalpha << _multipleSchemasOccurring);
		if (_xmlSchemaName.empty()) {
			ACS_LOG_WARNING("WARNING (XMLValidator::getSchemaName) Cannot find schema name in \"" << _xmlFileName << "\"") ;
		}
	}
	
	string schemaName = _xmlSchemaName;
	if( !fullPath ) {
		schemaName = File::getFileName(_xmlSchemaName);
	}
	return schemaName ;
}

void XMLValidator::setSchemaName(const std::string& s) 
{
	_xmlSchemaName = s;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "_xmlSchemaName: " << _xmlSchemaName ) ;
	
// reset flag
	_noNamespaceSchemaLocation = false;
	_multipleSchemasOccurring = false;
}

bool XMLValidator::validate(bool raiseException, bool *schemaExists) // PRQA S 4020 
{
	string redirectPath ;
	bool enabled = false ;
	RedirectHandler::getValues(redirectPath, enabled);

	string schemaName = getSchemaName(true) ;
    if (schemaExists) {
        *schemaExists = not schemaName.empty() ;
    }
	if (schemaName.empty()) {
		return true ;
    }

	string schemaPath = getSchemaPath(schemaName, redirectPath);
		
	string ret ;
	int retcode = spawnValidator(schemaPath, ret) ; // PRQA S 4412
	if( ( retcode != 0 ) && raiseException ) {
		ostringstream exMsg;
		exMsg << ret;
		if( _multipleSchemasOccurring ) {
			exMsg << " - Multiple schemas occurring in xsi:schemaLocation attribute";
		}
		ACS_THROW(XMLHandler::XMLParserError(exMsg.str())) ; // PRQA S 3081
	}
	
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "spawnValidator output:\n" << retcode) ; 

	
	return retcode == 0 ;
}

bool XMLValidator::validateStructure(bool raiseException) // PRQA S 4211 
{
	
	string ret ;
	int retcode = spawnStructureValidator(ret) ; // PRQA S 4412
	if( ( retcode != 0 ) && raiseException ) {
		ostringstream exMsg;
		exMsg << ret;
		ACS_THROW(XMLHandler::XMLParserError(exMsg.str())) ; // PRQA S 3081
	}
	
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "spawnStructureValidator output: " << retcode) ; 

	
	return retcode == 0 ;
}


int XMLValidator::spawnValidator(const std::string &schemaPath, std::string &outstring) const // PRQA S 4214
{
	return spawnCommonValidator(outstring, schemaPath  );
	
}

int XMLValidator::spawnStructureValidator(string & outstring) const // PRQA S 4214
{
	return spawnCommonValidator(outstring );
}

int XMLValidator::spawnCommonValidator(string & outstring, const string & schemaPath  ) const  // PRQA S 4214
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB);

	const string validatorTool("xmllint");
	SpawnProc proc(validatorTool) ;
	int argcount = 0 ;
	proc.args()[argcount++] = WaitPid::ProcArg("--stream") ; // PRQA S 3000, 3081 4
	proc.args()[argcount++] = WaitPid::ProcArg("--noout") ; // PRQA S 3000, 3081 4
	if( not schemaPath.empty() ) {
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "schemaPath: "<< schemaPath) ;
		proc.args()[argcount++] = WaitPid::ProcArg("--schema") ;
		proc.args()[argcount++] = WaitPid::ProcArg(schemaPath) ;
	}
	else {
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "No schema passed") ;
	}
	proc.args()[argcount++] = WaitPid::ProcArg(_xmlFileName) ; // PRQA S 3000, 3081

	int outfiledes[2] ; // PRQA S 4403
	
	ACS_COND_THROW(0 != pipe(outfiledes),exIOException("XMLValidator: Cannot open pipe")) ; // PRQA S 3081
	
	{
		proc.setOutDescriptor(outfiledes[1]) ;
		proc.setErrDescriptor(outfiledes[1]) ;

		// this will close outfiledes[0] on dtor
		File::ifdstream input(outfiledes[0]) ;

		proc.spawn() ; //  detached = false, autoKill = true
		close(outfiledes[1]) ;
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "process running: "<< boolalpha << proc.isRunning() << " pid is : " << proc.pid()) ; 
		
		size_t outMaxLength = _validatorToolMaxOutputLength;
		try {
			rsResourceSet& rs = ConfigurationSingleton::instance()->get();
			rsPushNSpace pushedSpace (rs, RedirectHandler::getNameSpace());
			ssize_t tmp = 0 ;
			rs.getValue("ValidatorToolOutMaxLength", tmp);
			if( tmp <= 0 ) {
				ostringstream exMsg;
				exMsg << "Illegal value for ValidatorToolOutMaxLength in configuration: " << tmp << " ( >0 expected )";
				ACS_THROW( exIllegalValueException( exMsg.str() ) ); // PRQA S 3081
			}
			outMaxLength = size_t(tmp); // PRQA S 3081
			ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "ValidatorToolOutMaxLength from configuration: "<< outMaxLength << " bytes" ) ;
		}
		catch( exIllegalValueException &e )
		{
			ACS_LOG_WARNING(e.what() << ". Settig the default value: " << outMaxLength << " bytes" );
		}
		catch( exception &e )
		{
			ACS_CLASS_WRITE_DEBUG( ACS_LOW_VERB, "ValidatorToolOutMaxLength not occurring in configuration. Setting the default value " << outMaxLength << " bytes" ) ;
		}

		ostringstream out ;
		size_t outLength = 0;
		string lastLine; 
		string currentline;
		bool writeOutputStream = true;
		do {
// no try - catch block needed since SpawnProc DTOR kills child process: see spawn method parameters
			char c = 0;
			while( input.get(c) ) { // PRQA S 4244, 3050
				currentline += c;
				if( '\n' == c ) {
					
					if( (outLength > outMaxLength) && writeOutputStream ) {
						ACS_LOG_WARNING(validatorTool << " so far output length (" << outLength << " bytes) exceeds the maximum (" << outMaxLength << " bytes)! Output will be truncated." );
						writeOutputStream = false;
					}
					
					if( writeOutputStream ) {
						outLength += currentline.size(); // PRQA S 3084
						out << currentline;
						ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, currentline);
					}
					
					if( not currentline.empty() ) {
						lastLine = currentline; 
					}
					currentline.clear();
				}
			}

			Timer::delay(100) ; // PRQA S 4400 
		} while (proc.isRunning()) ;
		
		char c = 0;
		while( input.get(c) ) { // PRQA S 4244, 3050
			currentline += c;
			if( '\n' == c ) {
				
				if( (outLength > outMaxLength) && writeOutputStream ) {
					ACS_LOG_WARNING(validatorTool << " so far output length (" << outLength << " bytes) exceeds the maximum (" << outMaxLength << " bytes)! Output will be truncated." );
					writeOutputStream = false;
				}

				if( writeOutputStream ) {
					outLength += currentline.size(); // PRQA S 3084
					out << currentline;
					ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, currentline);
				}
				
				if( not currentline.empty() ) {
					lastLine = currentline;
				}
				currentline.clear();
			}
		}

		if( !writeOutputStream ) {
// the last line of tool output could summarize the result.
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Last line: " << lastLine );
			out << lastLine;
		}

		int ret = 0;
		bool ex = proc.exitCode(ret) ;
		int sig = 0 ;
		bool sg = proc.exitSignal(sig)  ;
		
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Process " << (ex ? "exited" : "running") << " exit code: " << ret // PRQA S 3380
				   << " Process " << (sg ? "signaled" : "running") << " signal: " << sig) ;
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "input good: " << boolalpha << input.good() 
                   << " eof: " << boolalpha << input.eof() 
				   << " bad: " << boolalpha << input.bad()) ;

			
		outstring = out.str() ;

		return ret ;	
	}
}


string XMLValidator::getSchemaPath(const string& schemaName, const string& redirectPath) const // PRQA S 4020, 4214
{
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "schemaName: \""<< schemaName << "\" redirectPath: \"" << redirectPath << "\"") ;

	string schemaPath;
// First of all, stick filename with no path to redirect path name for backward compatibility, if not _noNamespaceSchemaLocation
	if( not _noNamespaceSchemaLocation ) { 
		schemaPath = redirectPath+"/"+File::getFileName(schemaName) ;
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Trying \""<< schemaPath << "\" schema path") ;
		if( File::exists(schemaPath) ) {
			return schemaPath;
		}
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "\""<< schemaPath << "\" schema path does not exist") ;
	}

// Find if schema path is a URL
	size_t protocolSeparatorPos = schemaName.find(_protocolSeparator);
	if( string::npos == protocolSeparatorPos ) {
// Schema path is not an URL!
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "\""<< schemaName << "\" is not an URL") ;
		if( _noNamespaceSchemaLocation ) {
// schemaName is relative path in the same path of the xml file as stated in http://www.xmlblueprint.com/help/html/topic_134.htm
			schemaPath = File::getDirPath(_xmlFileName) + "/" + schemaName;
		}
		else {
			string dirPath = File::getDirPath(schemaName, true);
			if( !dirPath.empty() ) {
// Stick filename full path to redirect path name
				string tmp = redirectPath+"/"+schemaName;
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Trying \""<< tmp << "\" schema path") ;
				if( File::exists(tmp) ) {
					schemaPath = tmp;
				}
				else {
					ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "\""<< tmp << "\" schema path does not exist") ;
				}
			}
		}
	}
	else {
// Schema path is a URL!
		string protocol = schemaName.substr(0, protocolSeparatorPos);
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Schema name protocol: \""<< protocol << "\"") ;
// Get the protocol: if it is a local protocol (e.g. fs, file, etc.) extract url body, else return the whole url
		if( isLocalProtocol(protocol) ) {
			schemaPath = schemaName.substr(protocolSeparatorPos + _protocolSeparator.length(), schemaName.length()); // PRQA S 3084
		}
		else { // e.g. http, https, etc
			schemaPath = schemaName;
		}
	}
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Schema path: \""<< schemaPath << "\"") ;

	return schemaPath;
}

bool XMLValidator::isLocalProtocol(const string & protocol) const // PRQA S 4214
{
	bool ret = false;
	for( size_t i = 0; i < _localProtocolsVect.size(); i++ ) {
		if( protocol == _localProtocolsVect[i] ) {
			ret = true;
			break;
		}
	}
	
// check if input protocol is configured as local
	if( !ret ) {
		
		string xmlDTDConfigurationNs = RedirectHandler::getNameSpace();
		
		try {
			rsResourceSet& rs = ConfigurationSingleton::instance()->get();
			rsPushNSpace pushedSpace (rs, xmlDTDConfigurationNs);

			vector<string> localProtocolsVectConf ;
        	size_t pSize = rs.getArray(_localProtocolConfKey, localProtocolsVectConf) ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "<" << pSize << "> " <<  _localProtocolConfKey << " elements") ;
			size_t lpv_size = localProtocolsVectConf.size() ;
            for( size_t i = 0; i < lpv_size; ++i ) { 
				if( protocol == localProtocolsVectConf[i] ) {
					ret = true;
					break;
				}
			}
		}
		catch (exception& ex)
		{
			ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
				ACS_LOG_NOTIFY_EX(ex) ;
			ACS_CLASS_END_DEBUG	
		}
		
	}
	
	return ret;
}

_ACS_END_NAMESPACE
