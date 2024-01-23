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
	Revision 2.2  2014/03/19 19:14:56  marpas
	coding best practices application in progress (not ended)
	warning fixing (qa & g++)
	
	Revision 2.1  2011/09/22 08:22:06  giucas
	Added S1OBMemConfFileParser
	

*/

#ifndef _S1OBMemConfFileParser_H_
#define _S1OBMemConfFileParser_H_ 

#include <exException.h>
#include <AuxFileParser.h>
#include <rsResourceSet.h>
#include <S1OBMemConfParams.h>
#include <set>
#include <vector>
#include <algorithm>

_ACS_BEGIN_NAMESPACE(acs)

/**
 * This class implements the parsing strategy for S1 On-Board Memory Configuration Update File
 */

class S1OBMemConfFileParser : public AuxFileParser // PRQA S 2153 
{
public:

	/**
	 * \brief  Class constructor 
	 */	
	S1OBMemConfFileParser();

	/**
	 * \brief  Class constructor 
	 * \param  The On-Board Memory Configuration Update file path
	 */	
	explicit S1OBMemConfFileParser(const std::string&);

	/**
	 * \brief  Class constructor 
	 * \param  The On-Board Memory Configuration Update file stored in resource set
	 */	
	explicit S1OBMemConfFileParser(const rsResourceSet&);

	/**
	 * \brief  Class destructor 
	 */
	virtual ~S1OBMemConfFileParser() throw() ;

	/**
	 * \brief  Class copy constructor 
	 */ 
	S1OBMemConfFileParser(const S1OBMemConfFileParser & ); 

	/**
	 * \brief  Operator= 
	 */
	S1OBMemConfFileParser &operator=(const S1OBMemConfFileParser &);

	/**
	 * \brief  Add a Predicted Orbit file name path to use for retrieving parameters 
	 */
	virtual void addFileName(const std::string&);

	/**
	 * \brief  Remove a Predicted Orbit file name path to use for retrieving parameters 
	 */
	virtual void removeFileName(const std::string&);

	/**
	 * \brief  Return the TimeCorrelation file name path
	 */
	virtual std::vector<std::string> getFileNames() { return _fileNamesVec; } // PRQA S 2131


	/**
	 * \brief  Return the TimeCorrelation file type loaded from Configuration File 
	 */
	virtual std::string getFileType() { return _fileType; } // PRQA S 2131

	/**
	 * \brief  Parse the files and stores the results in the vector of S1OBMemConfParams objects.
	 * \param  A vector of S1OBMemConfParams to store values during files parsing
	 */
	virtual void parse(std::vector<Parameters *>&);

private:

	void buildParams();
	void parseFile(const rsResourceSet&);
	double asciiCcsdsaRefMicrosecToJD(const std::string&);

	std::set<std::string>				_fileNames;
	std::vector<std::string>			_fileNamesVec;
	std::string 						_fileType;

	std::vector<S1OBMemConfParams>  	_validParams;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(S1OBMemConfFileParser) ;
};


_ACS_END_NAMESPACE

#endif //_S1OBMemConfFileParser_H_
