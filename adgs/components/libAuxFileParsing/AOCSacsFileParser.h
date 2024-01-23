// PRQA S 1050 EOF
/*

	Copyright 2017-2022, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Processors $

	$Id$

	$Author$

	$Log$
	Revision 1.1  2017/12/21 08:18:55  ricfer
	Added this script to add ACS header to all source/header/makefile files.
	

*/

#ifndef _AOCSacsFileParser_H_
#define _AOCSacsFileParser_H_ 

#include <exException.h>
#include <AuxFileParser.h>
#include <PredictedOrbitData.h>
#include <PredictedOrbitFileParser.h>
#include <set>
#include <vector>
#include <algorithm>

_ACS_BEGIN_NAMESPACE(acs)


/**
 * This class implements the parsing strategy for CRYOSAT FOS Predicted Orbit File
 */

class AOCSacsFileParser : public PredictedOrbitFileParser
{
public:

	/**
	 * \brief  Class constructor 
	 */	
	AOCSacsFileParser();

	/**
	 * \brief  Class constructor 
	 * \param  The auxiliary Predicted Orbit file path
	 */	
	explicit AOCSacsFileParser(const std::string&);

	/**
	 * \brief  Class destructor 
	 */
	virtual ~AOCSacsFileParser() throw() ;

	/**
	 * \brief  Class copy constructor 
	 */ 
	AOCSacsFileParser(const AOCSacsFileParser & ); 

	/**
	 * \brief  Operator= 
	 */
	AOCSacsFileParser &operator=(const AOCSacsFileParser &);

	/**
	 * \brief  Add a Predicted Orbit file name path to use for retrieving parameters 
	 */
	virtual void addFileName(const std::string&);

	/**
	 * \brief  Remove a Predicted Orbit file name path to use for retrieving parameters 
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
	 * \brief  Parse the  Predicted Orbit File and stores the results in the vector of PredictedOrbitData objects.
	 *		   <b>The time in PredictedOrbitData objects is expressed as JD</b>
	 * \param  A vector of PredictedOrbitData to store values during file parsing
	 */
	virtual void parse(std::vector<Parameters *>&);

private:

	void buildParams();
	void parseFile(const std::string&);
	long double asciiCcsdsaRefMicrosecToJD(const std::string&) const ;

	std::set<std::string>				_fileNames;
	std::vector<std::string>			_fileNamesVec;
	std::string 						_fileType;

	std::vector<PredictedOrbitData>  	_validParams;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(AOCSacsFileParser)
};
	
_ACS_END_NAMESPACE

#endif //_AOCSacsFileParser_H_
