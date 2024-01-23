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
	
	Revision 1.2  2005/11/17 08:08:50  giucas
	Fixed case of only 1 record in Orbit Scenario File
	
	Revision 1.1  2005/11/16 18:22:57  giucas
	First release
	

*/

#ifndef _EnvisatOSFileParser_H_
#define _EnvisatOSFileParser_H_ 

#include <OrbitScenarioFileParser.h>
#include <set>
#include <vector>

_ACS_BEGIN_NAMESPACE(acs)

/**
 * This class implements the parsing strategy for CRYOSAT FOS Predicted Orbit File
 */

class EnvisatOSFileParser : public OrbitScenarioFileParser // PRQA S 2109
{
public:

	/**
	 * \brief  Class constructor 
	 */	
	EnvisatOSFileParser();

	/**
	 * \brief  Class constructor 
	 * \param  The auxiliary Predicted Orbit file path
	 */	
	explicit EnvisatOSFileParser(const std::string&);

	/**
	 * \brief  Class destructor 
	 */
	virtual ~EnvisatOSFileParser() throw() ;

	/**
	 * \brief  Class copy constructor 
	 */ 
	EnvisatOSFileParser(const EnvisatOSFileParser & ); 

	/**
	 * \brief  Operator= 
	 */
	EnvisatOSFileParser &operator=(const EnvisatOSFileParser &);

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
	 * \brief  Parse the  Orbit Reference Scenario File and stores the results in the vector of OrbitScenario objects.
	 *		   <b>The time in OrbitScenario objects is expressed as JD</b>
	 * \param  data A vector of OrbitScenario to store values during file parsing
	 */
	virtual void parse(std::vector<Parameters *>& ) ;

private:

	void buildParams();
	void cleanupParams();
	void parseFile(const std::string&);
	long double asciiCcsdsaRefMicrosecToJD(const std::string&) const ;
	double mlstToSec(const std::string&) const ;
	int parseStringMonth(const std::string&) const ;
	std::string trim_right ( const std::string& source , const std::string & t = " " );
	std::string trim_left ( const std::string& source ,const std::string & t = " " );
	std::string trim ( const std::string& source ,const std::string & t = " " );

private:

	std::set<std::string> _fileNames;
	std::string _fileType;
	std::vector<std::string> _fileNamesVec;

	std::vector<OrbitScenario> _validParams;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(EnvisatOSFileParser)
};


_ACS_END_NAMESPACE

#endif //_EnvisatOSFileParser_H_
