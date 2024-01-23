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
	
	Revision 1.1.1.1  2005/02/21 14:43:53  giucas
	Imported libAuxFileParsing
	
	Revision 1.1  2004/12/07 10:09:24  giucas
	Moved from libTimeConverter
	
	Revision 1.5  2004/12/03 09:33:26  giucas
	Header fixed
	
	Revision 1.4  2004/11/15 13:31:56  giucas
	Changed FileParser interface to handle multi-file initialization
	
	Revision 1.3  2004/10/19 12:24:15  giucas
	Changed algo to extract parameters from file
	
	Revision 1.2  2004/10/12 07:41:49  giucas
	Added getFileName and getFileType methods
	
	Revision 1.1  2004/09/16 16:57:17  giucas
	First release
	


*/

#ifndef _GoceOBTFileParser_H_
#define _GoceOBTFileParser_H_ 

#include <rsResourceSet.h>
#include <OBTFileParser.h>
#include <set>

_ACS_BEGIN_NAMESPACE(acs)

class CFGOTimeCorrParams ; 

/**
 * This class specifics the parsing strategy for Time Correlation File
 */

class GoceOBTFileParser : public OBTFileParser
{
public:

	/**
	 * \brief  Class constructor 
	 */	
	GoceOBTFileParser();

	/**
	 * \brief  Class constructor 
	 * \param  The auxiliary TimeCorrelation file path
	 */	
	explicit GoceOBTFileParser(const std::string&);

	/**
	 * \brief  Class copy constructor 
	 */ 
	GoceOBTFileParser(const GoceOBTFileParser & ); 

	/**
	 * \brief  Operator= 
	 */
	GoceOBTFileParser &operator=(const GoceOBTFileParser &);

	/**
	 * \brief  Class destructor 
	 */
	virtual ~GoceOBTFileParser() throw() ;

	/**
	 * \brief  Add a TimeCorrelation file name path to use for retrieving parameters 
	 */
	virtual void addFileName(const std::string&); 

	/**
	 * \brief  Remove a TimeCorrelation file name path to use for retrieving parameters 
	 */
	virtual void removeFileName(const std::string&); 

	/**
	 * \brief  Return the Predicted Orbit file name paths 
	 */
	virtual std::vector<std::string> getFileNames() { return _fileNamesVec; } // PRQA S 2131

	/**
	 * \brief  Return the Predicted Orbit file type loaded from Configuration File 
	 */
	virtual std::string getFileType() { return _fileType; } // PRQA S 2131

	/**
	 * \brief  Parse the Time Correlation File using the vector of CFGOTimeCorrParams to store
	 *		   the results.
	 * \param  A vector of CFGOTimeCorrParams to store params values during file parsing
	 */
	virtual void parse(std::vector<Parameters *>&); 


private:

	void loadConfiguration();
	bool extractParameters(const std::string&, CFGOTimeCorrParams&) const ;

	void buildParams();
	void parseFile(const std::string&);
	void cleanupParams();

	rsResourceSet& 				_rsConf;
	std::set<std::string>		_fileNames;
	std::vector<std::string>	_fileNamesVec;
	std::string 				_fileType;

/////////////////////////////////////////////////
//Variables used to extract parameters from file		
/////////////////////////////////////////////////
	unsigned int 	_firstValidLineIndex;

	std::string		_validityFlagValue;
	unsigned int	_validLineElements;

	std::vector<CFGOTimeCorrParams*> _validParams;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(GoceOBTFileParser) 
};

_ACS_END_NAMESPACE

#endif //_GoceOBTFileParser_H_
