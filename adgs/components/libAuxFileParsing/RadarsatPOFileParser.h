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
	Revision 1.4  2014/03/19 19:14:56  marpas
	coding best practices application in progress (not ended)
	warning fixing (qa & g++)
	
	Revision 1.3  2012/04/18 08:15:40  giucas
	compilation warnings fixed
	
	Revision 1.2  2011/03/01 17:16:02  marpas
	GCC 4.4.x support
	
	Revision 1.1  2009/02/25 14:27:43  giucas
	First issue
	

*/

#ifndef _RadarsatPOFileParser_H_
#define _RadarsatPOFileParser_H_ 

#include <PredictedOrbitData.h>
#include <PredictedOrbitFileParser.h>
#include <set>
#include <vector>

_ACS_BEGIN_NAMESPACE(acs)

/**
 * This class implements the parsing strategy for Radarsat 1 Orbit Data File
 */

class RadarsatPOFileParser : public PredictedOrbitFileParser
{
public:

	/**
	 * \brief  Class constructor 
	 */	
	RadarsatPOFileParser();

	/**
	 * \brief  Class constructor 
	 * \param  The auxiliary Predicted Orbit file path
	 */	
	explicit RadarsatPOFileParser(const std::string&);

	/**
	 * \brief  Class destructor 
	 */
	virtual ~RadarsatPOFileParser() throw() ;

	/**
	 * \brief  Class copy constructor 
	 */ 
	RadarsatPOFileParser(const RadarsatPOFileParser & ); 

	/**
	 * \brief  Operator= 
	 */
	RadarsatPOFileParser &operator=(const RadarsatPOFileParser &);

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
	 * \param  data A vector of PredictedOrbitData to store values during file parsing
	 */
	virtual void parse(std::vector<Parameters*>&);

private:

	static const std::string FIRST_LINE_REG_EXPR ;

	void buildParams();
	void parseFile(const std::string&);
	long double getJDFromOrbitDataTime(const std::string&) const ;

	std::set<std::string>				_fileNames;
	std::string 						_fileType;
	std::vector<std::string>			_fileNamesVec;
	std::vector<PredictedOrbitData>  	_validParams;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(RadarsatPOFileParser)
};
	
_ACS_END_NAMESPACE

#endif //_RadarsatPOFileParser_H_
