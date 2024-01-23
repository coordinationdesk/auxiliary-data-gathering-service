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
	Revision 2.1  2006/10/18 18:08:14  marpas
	hopefully almost complete refactoring
	
	Revision 2.0  2006/02/28 09:32:51  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2005/02/21 14:43:53  giucas
	Imported libAuxFileParsing
	
	Revision 1.1  2004/12/06 17:34:07  giucas
	Imported sources
	

*/

#ifndef _PredictedOrbitFileParser_H_
#define _PredictedOrbitFileParser_H_ 

#include <AuxFileParser.h>

_ACS_BEGIN_NAMESPACE(acs)

/**
 * This abstract class specifics the parsing strategy interface for Predicted Orbit File
 */

class PredictedOrbitFileParser : public AuxFileParser
{
public:

	/**
	 * \brief  Class constructor 
	 */	
	PredictedOrbitFileParser() : AuxFileParser() {}

	/**
	 * \brief  Class constructor 
	 */	
	PredictedOrbitFileParser(const PredictedOrbitFileParser &r ) : AuxFileParser(r ) {}

	/**
	 * \brief  Class destructor 
	 */
	virtual ~PredictedOrbitFileParser() throw() {} // PRQA S 2131

	/**
	 * \brief  Add a Predicted Orbit file name path to use for retrieving parameters 
	 */
	virtual void addFileName(const std::string&)=0;

	/**
	 * \brief  Remove a Predicted Orbit file name path to use for retrieving parameters 
	 */
	virtual void removeFileName(const std::string&)=0;

	/**
	 * \brief  Return the Predicted Orbit file name paths 
	 */
	virtual std::vector<std::string> getFileNames()=0;

	/**
	 * \brief  Return the Predicted Orbit file type loaded from Configuration File 
	 */
	virtual std::string getFileType()=0;
protected: 

	PredictedOrbitFileParser& operator=(const PredictedOrbitFileParser &r ) 
    { if (this != &r) {AuxFileParser::operator=(r ) ; } return *this ; }

};

_ACS_END_NAMESPACE

#endif //_PredictedOrbitFileParser_H_
