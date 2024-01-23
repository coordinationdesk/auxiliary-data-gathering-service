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

	$Prod: A.C.S. libAuxFileParsing $

	$Id$

	$Author$

	$Log$
	Revision 1.4  2014/03/19 19:14:55  marpas
	coding best practices application in progress (not ended)
	warning fixing (qa & g++)
	
	Revision 1.3  2011/03/01 17:16:02  marpas
	GCC 4.4.x support
	
	Revision 1.2  2009/06/17 12:09:27  marant
	Added print() method.
	
	Revision 1.1  2006/12/12 09:42:55  marant
	ErsTCFileParser class added
	
       
*/ 


#ifndef _ErsTCFileParser_H_
#define _ErsTCFileParser_H_ 

#include <ErsTimeCorrParams.h>
#include <OBTFileParser.h>
#include <set>
#include <algorithm>

_ACS_BEGIN_NAMESPACE(acs)

/**
 * This class specifics the parsing strategy for Ers Time Correlation File in the following text format:
 * 
 *  UTC=%04d-%02d-%02dT%02d:%02d:%02d.%06d
 *  %10d %10d
 * 
 */

class ErsTCFileParser : public OBTFileParser
{
public:

	/**
	 * \brief  Class constructor 
	 */	
	ErsTCFileParser();

	/**
	 * \brief  Class constructor 
	 * \param  The auxiliary TimeCorrelation file path
	 */	
	explicit ErsTCFileParser(const std::string&);

	/**
	 * \brief  Class copy constructor 
	 */ 
	ErsTCFileParser(const ErsTCFileParser & ); 

	/**
	 * \brief  Operator= 
	 */
	ErsTCFileParser &operator=(const ErsTCFileParser &);

	/**
	 * \brief  Class destructor 
	 */
	virtual ~ErsTCFileParser() throw() ;

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
	 * \brief  Parse the Time Correlation File using the ErsTimeCorrParams vector in input to store
	 *		   the results.
	 * \param  A vector of ErsTimeCorrParams to store params values during file parsing
	 */
	virtual void parse(std::vector<Parameters*>&);

    /**
	 * \brief  Prints the time correlations on the input stream formatted as expected in the Ers Time Correlation file
	 * \param  os: ostream on which the paramters are written 		   
	 * \param  jd50: Reference time in Jd50; 
     * \param  satBinTime: the satellite binary time; 
     * \param  clockStepLength: the clock step length in <nanosecs>
	 */
    static void print(std::ostream*, double referenceJd50, unsigned long satBinTime, unsigned long clockStepLength);


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

	ACS_CLASS_DECLARE_DEBUG_LEVEL(ErsTCFileParser)
};
	

_ACS_END_NAMESPACE

#endif //_ErsTCFileParser_H_
