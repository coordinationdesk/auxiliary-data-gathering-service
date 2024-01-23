//  PRQA S 1050 EOF
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
	Revision 2.5  2014/03/19 19:14:55  marpas
	coding best practices application in progress (not ended)
	warning fixing (qa & g++)
	
	Revision 2.4  2012/04/18 08:15:40  giucas
	compilation warnings fixed
	
	Revision 2.3  2011/03/01 17:16:02  marpas
	GCC 4.4.x support
	
	Revision 2.2  2008/02/28 13:47:44  giucas
	The utcTime in PredictedOrbitData objects are expressed in JD50
	
	Revision 2.1  2007/12/03 10:16:49  giucas
	L5DefinitiveFileParser added
	

*/

#ifndef _L5DefinitiveFileParser_H_
#define _L5DefinitiveFileParser_H_ 

#include <AuxFileParser.h>
#include <PredictedOrbitData.h>
#include <PredictedOrbitFileParser.h>
#include <set>
#include <vector>
#include <algorithm>
#include <fstream>

_ACS_BEGIN_NAMESPACE(acs)

/**
 * This class implements the parsing strategy for ACS-defined State Vector File
 */

class L5DefinitiveFileParser : public PredictedOrbitFileParser
{
public:

	/**
	 * \brief  Class constructor 
	 */	
	L5DefinitiveFileParser();

	/**
	 * \brief  Class constructor 
	 * \param  The auxiliary State Vector file path
	 */	
	explicit L5DefinitiveFileParser(const std::string&);

	/**
	 * \brief  Class destructor 
	 */
	virtual ~L5DefinitiveFileParser() throw() ;

	/**
	 * \brief  Class copy constructor 
	 */ 
	L5DefinitiveFileParser(const L5DefinitiveFileParser & ); 

	/**
	 * \brief  Operator= 
	 */
	L5DefinitiveFileParser &operator=(const L5DefinitiveFileParser &);

	/**
	 * \brief  Add a State Vector file name path to use for retrieving parameters 
	 */
	virtual void addFileName(const std::string&);

	/**
	 * \brief  Remove a State Vector file name path to use for retrieving parameters 
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
	 * \brief  Parse the  State Vector File and stores the results in the vector of PredictedOrbitData objects.
	 *		   <b>The time in PredictedOrbitData objects is expressed as JD50</b>
	 * \param  data A vector of PredictedOrbitData to store values during file parsing
	 */
	virtual void parse(std::vector<Parameters *>&);

private:

	void buildParams();
	void parseFile(const std::string&);
	long double getJDFromDefinitiveTime(const std::string&) const ;
	bool addElement(std::ifstream* is) ;

	std::set<std::string>				_fileNames;
	std::string 						_fileType;
	std::vector<std::string>			_fileNamesVec;

	std::vector<PredictedOrbitData>  	_validParams;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(L5DefinitiveFileParser) ;
};

_ACS_END_NAMESPACE

#endif //_L5DefinitiveFileParser_H_
