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
	Revision 2.2  2014/03/19 19:14:56  marpas
	coding best practices application in progress (not ended)
	warning fixing (qa & g++)
	
	Revision 2.1  2006/10/18 18:08:14  marpas
	hopefully almost complete refactoring
	
	Revision 2.0  2006/02/28 09:32:51  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2005/02/21 14:43:53  giucas
	Imported libAuxFileParsing
	
	Revision 1.1  2004/12/07 13:45:25  giucas
	Imported source
	

*/

#ifndef _OrbitScenarioFileParser_H_
#define _OrbitScenarioFileParser_H_ 

#include <AuxFileParser.h>
#include <OrbitScenario.h>

_ACS_BEGIN_NAMESPACE(acs)

/**
 * This abstract class specifics the parsing strategy interface for Predicted Orbit File
 */

class OrbitScenarioFileParser : public AuxFileParser
{
public:

	/*! class OrbitScenarioFileParserException declaration */
	exDECLARE_EXCEPTION(OrbitScenarioFileParserException,exException) ; // PRQA S 2131, 2502

	/**
	 * \brief  Class constructor 
	 */	
	OrbitScenarioFileParser() : AuxFileParser() {} 
	OrbitScenarioFileParser(const OrbitScenarioFileParser &r) : AuxFileParser(r) {} 

	/**
	 * \brief  Class destructor 
	 */
	virtual ~OrbitScenarioFileParser() throw() {} // PRQA S 2131

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
	OrbitScenarioFileParser& operator= (const OrbitScenarioFileParser &r)
    {
        if (this != &r) {
            AuxFileParser::operator=(r) ;
        }
        return *this ;
    }



};

_ACS_END_NAMESPACE

#endif //_OrbitScenarioFileParser_H_
