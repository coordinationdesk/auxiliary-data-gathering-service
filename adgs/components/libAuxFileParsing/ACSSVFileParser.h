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

	$Author$

	@(#) A.C.S.: $Id$
	
	$Log$
	Revision 1.6  2014/03/19 19:14:54  marpas
	coding best practices application in progress (not ended)
	warning fixing (qa & g++)
	
	Revision 1.5  2012/04/18 08:15:39  giucas
	compilation warnings fixed
	
	Revision 1.4  2011/03/01 17:16:02  marpas
	GCC 4.4.x support
	
	Revision 1.3  2006/10/18 18:08:14  marpas
	hopefully almost complete refactoring
	
	Revision 1.2  2006/07/10 13:56:04  giucas
	ParseFile method implemented
	
	Revision 1.1  2006/07/10 12:15:35  giucas
	First issue. Still to be implemented
	


*/

#ifndef _ACSSVFileParser_H_
#define _ACSSVFileParser_H_ 

#include <PredictedOrbitData.h>
#include <PredictedOrbitFileParser.h>
#include <set>
#include <vector>
#include <fstream>

_ACS_BEGIN_NAMESPACE(acs)

/**
 * This class implements the parsing strategy for ACS-defined State Vector File
 */

class ACSSVFileParser : public PredictedOrbitFileParser
{
public:

	/**
	 * \brief  Class constructor 
	 */	
	ACSSVFileParser();

	/**
	 * \brief  Class constructor 
	 * \param  The auxiliary State Vector file path
	 */	
	explicit ACSSVFileParser(const std::string&);

	/**
	 * \brief  Class destructor 
	 */
	virtual ~ACSSVFileParser() throw() ;

	/**
	 * \brief  Class copy constructor 
	 */ 
	ACSSVFileParser(const ACSSVFileParser & ); 

	/**
	 * \brief  Operator= 
	 */
	ACSSVFileParser &operator=(const ACSSVFileParser &);

	/**
	 * \brief  Add a State Vector file name path to use for retrieving parameters 
	 */
	virtual void addFileName(const std::string&);

	/**
	 * \brief  Remove a State Vector file name path to use for retrieving parameters 
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
	 * \brief  Parse the  State Vector File and stores the results in the vector of PredictedOrbitData objects.
	 *		   <b>The time in PredictedOrbitData objects is expressed as JD</b>
	 * \param  data A vector of PredictedOrbitData to store values during file parsing
	 */
	virtual void parse(std::vector<Parameters *>& );

private:

	void buildParams();
	void parseFile(const std::string&);
	long double getJDFromCCSDSAMicrosec(const std::string&) const ;
	bool addElement(std::ifstream* is) ;

	std::set<std::string>				_fileNames;
	std::string 						_fileType;
	std::vector<std::string>			_fileNamesVec;

	std::vector<PredictedOrbitData>  	_validParams;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(ACSSVFileParser)
};


_ACS_END_NAMESPACE

#endif //_ACSSVFileParser_H_
