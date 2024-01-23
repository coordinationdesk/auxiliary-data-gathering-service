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
	Revision 2.4  2014/03/19 19:14:55  marpas
	coding best practices application in progress (not ended)
	warning fixing (qa & g++)
	
	Revision 2.3  2012/04/18 08:15:40  giucas
	compilation warnings fixed
	
	Revision 2.2  2011/03/01 17:16:02  marpas
	GCC 4.4.x support
	
	Revision 2.1  2006/10/18 18:08:14  marpas
	hopefully almost complete refactoring
	
	Revision 2.0  2006/02/28 09:32:51  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.3  2005/12/02 11:44:54  giucas
	Fixed bug: the time in PredictedOrbitData was calculated as MJD2000 and not as JD
	
	Revision 1.2  2005/11/16 13:20:38  giucas
	Added static attributed used for parsing
	
	Revision 1.1  2005/11/16 10:32:35  giucas
	First issue
	

*/

#ifndef _EnvisatPOFileParser_H_
#define _EnvisatPOFileParser_H_ 

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

class EnvisatPOFileParser : public PredictedOrbitFileParser
{
public:

	/**
	 * \brief  Class constructor 
	 */	
	EnvisatPOFileParser();

	/**
	 * \brief  Class constructor 
	 * \param  The auxiliary Predicted Orbit file path
	 */	
	explicit EnvisatPOFileParser(const std::string&);

	/**
	 * \brief  Class destructor 
	 */
	virtual ~EnvisatPOFileParser() throw() ;

	/**
	 * \brief  Class copy constructor 
	 */ 
	EnvisatPOFileParser(const EnvisatPOFileParser & ); 

	/**
	 * \brief  Operator= 
	 */
	EnvisatPOFileParser &operator=(const EnvisatPOFileParser &);

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

	void buildParams();
	void parseFile(const std::string&);
	long double getJDFromAsciiEnviMicrosec(const std::string&) const ;
	int parseStringMonth(const std::string&) const ;

    static const unsigned int _firstValidLineIndex = 52 ;
    static const unsigned int _validLineSize = 128 ;

    static const unsigned int _utcTimeOffset = 0;
    static const unsigned int _utcTimeLength = 27;

    static const unsigned int _absOrbitOffset = 37;
    static const unsigned int _absOrbitLength = 6;

    static const unsigned int _xPosOffset = 44;
    static const unsigned int _xPosLength = 12;

    static const unsigned int _yPosOffset = 57;
    static const unsigned int _yPosLength = 12;

    static const unsigned int _zPosOffset = 70;
    static const unsigned int _zPosLength = 12;

    static const unsigned int _xVelOffset = 83;
    static const unsigned int _xVelLength = 12;

    static const unsigned int _yVelOffset = 96;
    static const unsigned int _yVelLength = 12;

    static const unsigned int _zVelOffset = 109;
    static const unsigned int _zVelLength = 12;

	std::set<std::string>				_fileNames;
	std::vector<std::string>			_fileNamesVec;
	std::string 						_fileType;

	std::vector<PredictedOrbitData>  	_validParams;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(EnvisatPOFileParser)
};


_ACS_END_NAMESPACE

#endif //_EnvisatPOFileParser_H_
