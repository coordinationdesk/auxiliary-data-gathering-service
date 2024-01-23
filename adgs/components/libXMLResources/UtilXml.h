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

	$Prod: A.C.S. libPackageTask$

	$Id$

	$Author$

	$Log$
	Revision 5.12  2015/09/03 10:09:35  nicvac
	Refactoring. Update implemented.
	
	Revision 5.11  2015/06/30 16:28:50  nicvac
	getValue fixed.
	
	Revision 5.10  2015/06/18 17:43:43  nicvac
	getValue fixed in string case
	
	Revision 5.9  2015/05/14 12:50:12  nicvac
	Added getValue to read a single item.
	
	Revision 5.8  2014/12/11 09:39:41  damdec
	Methods to redirect or silent libxml2 error messages added.
	
	Revision 5.7  2014/10/16 16:14:26  nicvac
	Remember xml filename.
	
	Revision 5.6  2013/12/18 17:06:31  nicvac
	Default namespace xmlns handling.
	
	Revision 5.5  2013/11/28 15:21:52  marpas
	coding best practices applied and warnings removed
	
	Revision 5.4  2013/11/05 16:42:17  nicvac
	skipXmlHeader option improved.
	
	Revision 5.3  2013/10/01 14:41:26  nicvac
	Parameters handling.
	
	Revision 5.2  2013/09/11 15:31:49  nicvac
	Added support for attributes.
	
	Revision 5.1  2013/08/02 16:12:21  nicvac
	UtilXml moved from libPackageTask to libXMLResources.
	
	Revision 1.10  2013/08/02 09:41:11  nicvac
	Improved str() method parameters.
	
	Revision 1.9  2013/07/31 16:33:38  nicvac
	xPathWrap improved.
	
	Revision 1.8  2013/07/31 13:08:14  nicvac
	XmlUtil refactoring.
	
	Revision 1.7  2013/07/31 09:44:10  nicvac
	UtilXml refactoring.
	
	Revision 1.6  2013/07/24 17:20:46  nicvac
	Supported setrootns. Namespace registration to support xpath with namespace.
	
	Revision 1.5  2013/07/24 15:38:57  nicvac
	xPath can return a list of results.
	
	Revision 1.4  2013/05/14 15:47:41  marfav
	Added xpath support
	
	Revision 1.3  2013/05/14 10:45:55  clanas
	Added support to XML subtree extraction
	
	Revision 1.2  2013/04/09 17:24:05  nicvac
	Strip cdata added
	
	Revision 1.1.1.1  2013/04/08 17:08:03  nicvac
	Import libPackageTask
	
        
*/

#ifndef _UtilXml_H_
#define _UtilXml_H_

#include <acs_c++config.hpp>
#include <exException.h>

#include <XmlDoc.h>

#include <boost/shared_ptr.hpp> // PRQA S 1013 


_ACS_BEGIN_NAMESPACE(acs)


/**
* \brief
* Common utilities for xml - based on libxml2
* DEPRECATED - USE XmlDoc.h
**/


/** Utilities
 * UtilXml: higher level interface: to be used by lazy coders. Use XmlDoc instead (http://wiki.acs/index.php/PDS_dev:_Xml)
 * */
class UtilXml  { // PRQA S 2109
public:

	/** UtilXml Exceptions */
	exDECLARE_EXCEPTION(exUtilXmlException, exException) ; // Base UtilXml Exception. // PRQA S 2131, 2502 2 
	exDECLARE_EXCEPTION(exUtilXmlCriticalException, exUtilXmlException) ; // Critical Exception. // PRQA S 2153

	/** Default Class constructor */
	__attribute__((deprecated)) UtilXml();
	/** Destructor */
	virtual ~UtilXml();

	/** Extract subtrees matching XPath and return them in the passed root tag */
	static std::string __attribute__((deprecated)) xPath  (std::string const& xmlFile, std::string const& xPath, std::string const& rootTag="Root" );    
    
	/** Extract subtrees matching XPath and return them */
	static void __attribute__((deprecated)) xPath (std::string const& xmlFile, std::string const& xPath, std::vector<std::string>& result);

	/** Get a subtree, preventing the remotion of all the parent tags, without loading the complete xml file in memory */
	static std::string __attribute__((deprecated)) getSubTree (std::string const& xmlFile, std::string const& xPath);


private:  // copy constructor and operator= defined but not implemented
	/** Copy Constructor */
	UtilXml(const UtilXml & );
	/** Operator = */
	UtilXml &operator=(const UtilXml &);

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(UtilXml)

};

_ACS_END_NAMESPACE

#endif //_UtilXml_H_

