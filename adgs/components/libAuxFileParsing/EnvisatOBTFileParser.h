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
	Revision 2.3  2014/03/19 19:14:55  marpas
	coding best practices application in progress (not ended)
	warning fixing (qa & g++)
	
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
	
	Revision 1.6  2004/12/03 09:33:26  giucas
	Header fixed
	
	Revision 1.5  2004/11/16 08:59:24  giucas
	Corrected include of std::set
	
	Revision 1.4  2004/11/15 13:31:56  giucas
	Changed FileParser interface to handle multi-file initialization
	
	Revision 1.3  2004/10/12 07:41:49  giucas
	Added getFileName and getFileType methods
	
	Revision 1.2  2004/10/08 09:02:56  giucas
	Implemented parse method
	
	Revision 1.1  2004/10/06 07:16:25  giucas
	First release.Implementation still to be completed.
	


*/

#ifndef _EnvisatOBTFileParser_H_
#define _EnvisatOBTFileParser_H_ 

#include <exException.h>
#include <USCTimeCorrParams.h>
#include <OBTFileParser.h>
#include <set>
#include <algorithm>

_ACS_BEGIN_NAMESPACE(acs)

/**
 * This class specifics the parsing strategy for ENVISAT Time Correlation File
 */

class EnvisatOBTFileParser : public OBTFileParser
{
public:

	/**
	 * \brief  Class constructor 
	 */	
	EnvisatOBTFileParser();

	/**
	 * \brief  Class constructor 
	 * \param  The auxiliary TimeCorrelation file path
	 */	
	explicit EnvisatOBTFileParser(const std::string&);

	/**
	 * \brief  Class copy constructor 
	 */ 
	EnvisatOBTFileParser(const EnvisatOBTFileParser & ); 

	/**
	 * \brief  Operator= 
	 */
	EnvisatOBTFileParser &operator=(const EnvisatOBTFileParser &);

	/**
	 * \brief  Class destructor 
	 */
	virtual ~EnvisatOBTFileParser() throw() ;

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
	virtual std::string getFileType() { return _fileType; } // PRQA S 2131

	/**
	 * \brief  Parse the Time Correlation File using the USCTimeCorrParams vector in input to store
	 *		   the results.
	 * \param  A vector of USCTimeCorrParams to store params values during file parsing
	 */
	virtual void parse(std::vector<Parameters *>&);


private:

	void loadConfiguration();
	int parseStringMonth(const std::string&) const ;
	long double getMJD2000FromAsciiEnviMicrosec(const std::string&) const ;

	void buildParams();
	void parseFile(const std::string&);

	void cleanupParams();

	std::set<std::string>		_fileNames;
	std::string 				_fileType;
	std::vector<std::string>	_fileNamesVec;

/////////////////////////////////////////////////
//Variables used to extract parameters from file		
/////////////////////////////////////////////////
	unsigned int	_onBoardTimeGadsSize;		     		   
	unsigned int	_onBoardTimeGadsTimeSize;	     		   
	unsigned int	_onBoardTimeGadsSBTSize;	     		   
	unsigned int	_onBoardTimeGadsClockStepSize; 

	std::vector<USCTimeCorrParams*>	_validParams;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(EnvisatOBTFileParser)
};
	


_ACS_END_NAMESPACE

#endif //_EnvisatOBTFileParser_H_
