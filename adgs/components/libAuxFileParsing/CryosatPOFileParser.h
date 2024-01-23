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
	Revision 2.3  2014/03/19 19:14:54  marpas
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
	
	Revision 1.3  2004/12/09 16:15:23  giucas
	Updated after PredictedOrbitData class changes
	
	Revision 1.2  2004/12/07 11:35:23  giucas
	Implemented conversion from string to JD for times in PredictedOrbitData
	
	Revision 1.1  2004/12/06 17:34:07  giucas
	Imported sources
	

*/

#ifndef _CryosatPOFileParser_H_
#define _CryosatPOFileParser_H_ 

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

class CryosatPOFileParser : public PredictedOrbitFileParser
{
public:

	/**
	 * \brief  Class constructor 
	 */	
	CryosatPOFileParser();

	/**
	 * \brief  Class constructor 
	 * \param  The auxiliary Predicted Orbit file path
	 */	
	explicit CryosatPOFileParser(const std::string&);

	/**
	 * \brief  Class destructor 
	 */
	virtual ~CryosatPOFileParser() throw() ;

	/**
	 * \brief  Class copy constructor 
	 */ 
	CryosatPOFileParser(const CryosatPOFileParser & ); 

	/**
	 * \brief  Operator= 
	 */
	CryosatPOFileParser &operator=(const CryosatPOFileParser &);

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

	ACS_CLASS_DECLARE_DEBUG_LEVEL(CryosatPOFileParser)
};
	
_ACS_END_NAMESPACE

#endif //_CryosatPOFileParser_H_
