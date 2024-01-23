// PRQA S 1050 EOF
/*

	Copyright 2002-2013, Advanced Computer Systems , Inc.
	Via della Bufalotta, 347 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. XMLResources Library $

	$Id$

	$Author$

	$Log$
	Revision 5.0  2013/07/18 17:54:35  marpas
	adopting libException 5.x standards
	performances improved
	coding best practices applied
	qa warnings removed
	compilation warnings removed
	
	Revision 2.5  2013/07/12 11:26:01  marpas
	refactoring to apply modern methods to deal wih tmp files
	
	Revision 2.4  2013/03/26 16:37:51  marpas
	file2file method added
	
	Revision 2.3  2012/12/02 20:03:49  marpas
	qa rules
	
	Revision 2.2  2012/02/13 16:47:41  marpas
	refactoring
	
	Revision 2.1  2009/09/10 14:55:24  marpas
	Xercers 3.0.x i/f compatibilty
	
	Revision 2.0  2006/02/28 08:37:31  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.7  2005/02/02 16:16:42  marpas
	Xalan timeout in a class constant and has been increased up to 10 min
	
	Revision 1.6  2004/04/15 19:35:14  paoscu
	Xalan is the default transformer. If it's not present xslproc is used
	
	Revision 1.5  2003/09/26 11:25:27  paoscu
	header style changed.
	
	Revision 1.4  2003/09/26 10:29:41  paoscu
	xsltproc is used instaed of xalan
	It's possible to specify a working path if string2string and string2string methods.
	
	Revision 1.3  2003/04/30 10:30:59  marpas
	using namespace std was removed from includes
	
	Revision 1.2  2003/02/04 18:50:18  paoscu
	Finished
	
	Revision 1.1  2003/01/31 16:29:56  paoscu
	Imported.
	
	

*/


#ifndef _XMLTransformer_H_
#define _XMLTransformer_H_

#include <acs_c++config.hpp>
#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)

/**
 * This class performs xml transformation.
 * To perform this task it uses Xalan application (if present in PATH)
 * or ,as a second choice, xsltproc
 */

class XMLTransformer // PRQA S 2109
{

public:
	XMLTransformer();
	~XMLTransformer() throw() ;

	/**
	 * Set XSL file path
	 */
	void xslFilePath(std::string const &);


	/**
	 * Set XML file path
	 */
	void xmlFilePath(std::string const &);


	/**
	 * Set output file path
	 */
	void outputFilePath(std::string const &);


	/**
	 * Write output file
	 * \throw exIOException XSL file path has not been specified or it is not valid
	 * \throw exIOException XML file path has not been specified or it is not valid
	 * \throw exIOException Output file path has not been specified 
	 * \throw exIOException Both Xalan and xsltproc are not present in path 
	 * \throw exIOException External transformation program exited with an error. 
	 */
	void writeOutputFile();

	/**
	 * Write an output file given an input string containing the xml stream to transform
	 * It uses writeOutputFile() method.
	 * \param XslFilePath The XSL file path 
	 * \param XmlString The XML string
	 * \param OutputFilePath The ouput file path
	 * \param workingPath Working dir to create temporary files.
	 */
	static void string2file(std::string const & XslFilePath, std::string const & XmlString, std::string const & OutputFilePath, std::string const & workingPath="/tmp");

	/**
	 * Write an output file given an input file containing the xml stream to transform
	 * It uses writeOutputFile() method.
	 * \param XslFilePath The XSL file path 
	 * \param XmlFilePath The XML file
	 * \param OutputFilePath The ouput file path
	 * \param removeInput if true removes input.
	 */
	static void file2file(std::string const & XslFilePath, 
                         std::string const & XmlFilePath, 
                         std::string const & OutputFilePath, 
                         bool removeInput=false);



	/**
	 * Transform an input string and write the result into anopther string.
	 * It uses writeOutputFile() method.
	 * \param XslFilePath The XSL file path 
	 * \param XmlString The XML string
	 * \param OutString The ouput string
	 * \param workingPath Working dir to create temporary files.
	 */
	static void string2string(std::string const & XslFilePath, std::string const & XmlString, std::string & OutString, std::string const & workingPath="/tmp")	;

private:
	void writeOutputFileXalan();
	void writeOutputFileXsltProc();

	//Not implemented
	XMLTransformer(const XMLTransformer &);
	XMLTransformer & operator=(const XMLTransformer &);

public:
	static const int XalanSecTimeout = 600 ; // 10 min

private:
	std::string _xslFilePath;
	std::string _xmlFilePath;
	std::string _outputFilePath;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(XMLTransformer) ;
};


_ACS_END_NAMESPACE


#endif //_XMLTransformer_H_
