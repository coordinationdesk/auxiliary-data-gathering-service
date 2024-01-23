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

*/

#ifndef _OrbitDataFileParser_H_
#define _OrbitDataFileParser_H_ 

#include <OBTFileParser.h>

_ACS_BEGIN_NAMESPACE(acs)

/**
 * This class specifics the parsing strategy for Time Correlation File
 */

class OrbitDataFileParser : public OBTFileParser
{
public:
	/**
	 * \brief  Class constructor 
	 */	
	OrbitDataFileParser() : OBTFileParser() {}

	/**
	 * \brief  Class copy constructor 
	 */ 
	OrbitDataFileParser(const OrbitDataFileParser &r ) : OBTFileParser(r) {} 

	/**
	 * \brief  Class destructor 
	 */
	virtual ~OrbitDataFileParser() throw() {} // PRQA S 2131

	/**
	 * \brief  Add a TimeCorrelation file name path to use for retrieving parameters 
	 */
	virtual void addFileName(const std::string&)=0;

	/**
	 * \brief  Remove a TimeCorrelation file name path to use for retrieving parameters 
	 */
	virtual void removeFileName(const std::string&)=0;

	/**
	 * \brief  Return the TimeCorrelation file name paths 
	 */
	virtual std::vector<std::string> getFileNames()=0;

	/**
	 * \brief  Return the TimeCorrelation file type loaded from Configuration File 
	 */
	virtual std::string getFileType()=0;
protected:
	/**
	 * \brief  Operator= 
	 */
	OrbitDataFileParser &operator=(const OrbitDataFileParser &r) { if (this != &r) {OBTFileParser::operator=(r) ; } return *this ; }


};

_ACS_END_NAMESPACE

#endif //_OrbitDataFileParser_H_
