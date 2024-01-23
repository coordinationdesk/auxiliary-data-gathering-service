// PRQA S 1050 EOF
/*

	Copyright 1995-2014, Advanced Computer Systems , Inc.
	Via della Bufalotta, 347 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$
	
	$Log$
	Revision 2.2  2011/03/01 17:16:02  marpas
	GCC 4.4.x support
	
	Revision 2.1  2006/10/18 18:08:14  marpas
	hopefully almost complete refactoring
	
	Revision 2.0  2006/02/28 09:32:51  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1  2005/07/01 14:10:36  seralb
	Added ERS satellite
	
	Revision 1.1.1.1  2005/02/21 14:43:53  giucas


*/

#ifndef _ErsOBTFileParser_H_
#define _ErsOBTFileParser_H_ 

#include <OBTFileParser.h>
#include <set>

_ACS_BEGIN_NAMESPACE(acs)

class ErsTimeCorrParams ;

/**
 * This class specifics the parsing strategy for Ers Time Correlation File
 */

class ErsOBTFileParser : public OBTFileParser
{
public:

	/**
	 * \brief  Class constructor 
	 */	
	ErsOBTFileParser();

	/**
	 * \brief  Class constructor 
	 * \param  The auxiliary TimeCorrelation file path
	 */	
	explicit ErsOBTFileParser(const std::string&);

	/**
	 * \brief  Class copy constructor 
	 */ 
	ErsOBTFileParser(const ErsOBTFileParser & ); 

	/**
	 * \brief  Operator= 
	 */
	ErsOBTFileParser &operator=(const ErsOBTFileParser &);

	/**
	 * \brief  Class destructor 
	 */
	virtual ~ErsOBTFileParser() throw() ;

	/**
	 * \brief  Set the TimeCorrelation file name path to use for retrieving parameters 
	 */
	virtual void addFileName(const std::string& name);

	/**
	 * \brief  Remove the TimeCorrelation file name path to use for retrieving parameters 
	 */
	virtual void removeFileName(const std::string& name);

	/**
	 * \brief  Return the TimeCorrelation file name path
	 */
	virtual std::vector<std::string> getFileNames() { return _fileNamesVec; } // PRQA S 2131

	/**
	 * \brief  Return the TimeCorrelation file type loaded from Configuration File 
	 */
	virtual std::string getFileType() { return _fileType ; } // PRQA S 2131

	/**
	 * \brief  Parse the Time Correlation File using the ErsTimeCorrParams vector in input to store
	 *		   the results.
	 * \param  A vector of ErsTimeCorrParams to store params values during file parsing
	 */
	virtual void parse(std::vector<Parameters*>&);


private:

	void buildParams();
	void parseFile(const std::string&);

	void cleanupParams();

	std::set<std::string>		_fileNames;
	std::string 				_fileType;
	std::vector<std::string>	_fileNamesVec;

/////////////////////////////////////////////////
//Variables used to extract parameters from file		
/////////////////////////////////////////////////
	std::vector<ErsTimeCorrParams*>	_validParams;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(ErsOBTFileParser) 
};
	




_ACS_END_NAMESPACE


#endif //_ErsOBTFileParser_H_
