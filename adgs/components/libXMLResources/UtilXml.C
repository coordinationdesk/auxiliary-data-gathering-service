// PRQA S 1050 EOF
/*
	Copyright 1995-2015, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libPackageTask$

	$Id$

	$Author$

	$Log$
	Revision 5.17  2015/09/03 10:09:35  nicvac
	Refactoring. Update implemented.
	
	Revision 5.16  2015/06/18 08:40:37  damdec
	Useless code in silentError removed.
	
	Revision 5.15  2015/04/15 14:45:23  nicvac
	Properties inheritance in case of xpath
	
	Revision 5.14  2015/03/03 09:58:39  marpas
	fixing qa warnings
	coding best practices applied
	
	Revision 5.13  2014/12/11 09:39:41  damdec
	Methods to redirect or silent libxml2 error messages added.
	
	Revision 5.12  2014/10/16 16:14:26  nicvac
	Remember xml filename.
	
	Revision 5.11  2014/02/28 17:24:32  nicvac
	Default Namespace reviewed.
	
	Revision 5.10  2014/02/13 15:50:12  marpas
	qa warnings
	
	Revision 5.9  2014/02/13 12:42:29  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.8  2013/12/18 17:06:31  nicvac
	Default namespace xmlns handling.
	
	Revision 5.7  2013/12/05 17:06:36  marpas
	coding best practices applied
	
	Revision 5.6  2013/11/28 15:21:52  marpas
	coding best practices applied and warnings removed
	
	Revision 5.5  2013/11/20 16:02:17  nicvac
	Fixed formatting issue.
	
	Revision 5.4  2013/11/05 16:42:17  nicvac
	skipXmlHeader option improved.
	
	Revision 5.3  2013/10/01 14:41:26  nicvac
	Parameters handling.
	
	Revision 5.2  2013/09/11 15:31:49  nicvac
	Added support for attributes.
	
	Revision 5.1  2013/08/02 16:12:20  nicvac
	UtilXml moved from libPackageTask to libXMLResources.
	
	Revision 1.16  2013/08/02 09:41:11  nicvac
	Improved str() method parameters.
	
	Revision 1.15  2013/07/31 16:33:38  nicvac
	xPathWrap improved.
	
	Revision 1.14  2013/07/31 16:11:52  nicvac
	Improved xPathWrap
	
	Revision 1.13  2013/07/31 15:20:27  nicvac
	Refactoring.
	
	Revision 1.12  2013/07/31 13:08:14  nicvac
	XmlUtil refactoring.
	
	Revision 1.11  2013/07/31 09:44:10  nicvac
	UtilXml refactoring.
	
	Revision 1.10  2013/07/24 18:14:19  nicvac
	xml header strip fixed.
	
	Revision 1.9  2013/07/24 17:20:46  nicvac
	Supported setrootns. Namespace registration to support xpath with namespace.
	
	Revision 1.8  2013/07/24 16:08:50  nicvac
	Empty resut handling.
	
	Revision 1.7  2013/07/24 15:38:57  nicvac
	xPath can return a list of results.
	
	Revision 1.6  2013/05/14 15:47:41  marfav
	Added xpath support
	
	Revision 1.5  2013/05/14 10:45:55  clanas
	Added support to XML subtree extraction
	
	Revision 1.4  2013/04/11 17:23:35  nicvac
	Fixed regExp for cdata
	
	Revision 1.3  2013/04/09 17:24:05  nicvac
	Strip cdata added
	
	Revision 1.2  2013/04/08 17:16:03  nicvac
	debug definition
	
	Revision 1.1.1.1  2013/04/08 17:08:03  nicvac
	Import libPackageTask
	
        
*/ 

#include <UtilXml.h>

#include <Filterables.h>

#include <libxml/xmlreader.h> // PRQA S 1013 2
#include <libxml/xinclude.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(UtilXml)

namespace {
	inline const xmlChar* toXmlChar(const string &c) { return reinterpret_cast<const xmlChar*>(c.c_str())  ; } // PRQA S 2134, 3030, 3081 2
	inline const char * fromXmlChar(const xmlChar* xc) { return reinterpret_cast<const char*>(xc)  ; }
}


/// --- UtilXml: higher level interface --- ///

UtilXml::UtilXml() {
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Id: " << Id);
}

UtilXml::~UtilXml() {
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Destructor called."); // PRQA S 4631
}


string UtilXml::xPath (string const& xmlFile, string const& xPath, string const& rootTag) {

	XmlDoc xml; xml.setParameters(XmlDoc::parDefault ).initWithFile(xmlFile);

	return (xml.xPathWrap( xPath, rootTag ))->str();
}


void UtilXml::xPath (string const& xmlFile, string const& xPath, std::vector<std::string>& result) {

	XmlDoc xml; xml.setParameters(XmlDoc::parDefault ).initWithFile(xmlFile);
	std::vector< XmlDocSptr > xmls = xml.xPath(xPath);

	for ( vector< XmlDocSptr >::iterator it = xmls.begin(); it!=xmls.end(); ++it ) { // PRQA S 4238

		result.push_back( (*it)->str() );
	}
}


namespace {
static xmlDocPtr
extractFile(const char *filename, const xmlChar *pattern) { // PRQA S 4020
	xmlDocPtr doc = 0 ;
	//build an xmlReader for that file
	xmlTextReaderPtr reader = xmlReaderForFile(filename, 0, 0);
	if (reader != 0) {
		//add the pattern to preserve
		if (xmlTextReaderPreservePattern(reader, pattern, 0) < 0) {
			fprintf(stderr, "%s : failed add preserve pattern %s\n",
				filename, fromXmlChar (pattern));
		}
		//Parse and traverse the tree, collecting the nodes in the process
		int ret = xmlTextReaderRead(reader);
		while (ret == 1) {
			ret = xmlTextReaderRead(reader);
		}
		if (ret != 0) {
			fprintf(stderr, "%s : failed to parse\n", filename);
			xmlFreeTextReader(reader);
			return 0 ;
		}
		//get the resulting nodes
		doc = xmlTextReaderCurrentDoc(reader);
		//Free up the reader
		xmlFreeTextReader(reader);
	} else {
		fprintf(stderr, "Unable to open %s\n", filename);
		return 0 ;
	}
	return doc ;
}
};

string UtilXml::getSubTree(string const& xmlFile, string const& xPath)
{
	// Init the library
	xmlInitParser();
	string toReturn ;
	
	// Extract the subdocument
	xmlDocPtr doc = extractFile(xmlFile.c_str(), toXmlChar (xPath.c_str()));
	
	// Check if doc exists
	if (doc != 0) {
		// Prepare the results
		xmlChar* buffer = 0 ;
		int size = 0;
		xmlDocDumpMemory(doc, &buffer, &size);
		toReturn = fromXmlChar (buffer);

		// Cleanup
		xmlFree (buffer);
		xmlFreeDoc(doc);
		xmlCleanupParser();

	} else {
		ostringstream os;
		os << "Unable to extract subtree from [" << xmlFile << "] using pattern [" << xPath << "]";
		xmlCleanupParser();
		ACS_THROW( exUtilXmlCriticalException( os.str()) ); // PRQA S 3081
	}

	// return the subtree (if any)
	return toReturn;
}


_ACS_END_NAMESPACE

