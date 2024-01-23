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
	Revision 2.3  2009/03/04 18:24:57  marpas
	useless include removed
	
	Revision 2.2  2006/10/18 18:08:14  marpas
	hopefully almost complete refactoring
	
	Revision 2.1  2006/10/18 16:38:08  marpas
	new interface released to get rid of involuntary dependencies
	
	Revision 2.0  2006/02/28 09:32:51  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.5  2005/11/03 16:15:29  giucas
	Removed useless parse method for set OBTUTCTimeCorrParams
	
	Revision 1.4  2005/11/02 13:24:53  giucas
	Added parse method for set of OBTUTCTimeCorrParams
	
	Revision 1.3  2005/09/12 14:36:42  seralb
	Added Spot satellite
	
	Revision 1.2  2005/07/01 14:10:36  seralb
	Added ERS satellite
	
	Revision 1.1.1.1  2005/02/21 14:43:53  giucas
	Imported libAuxFileParsing
	
	Revision 1.2  2005/02/01 17:33:40  danalt
	added OBTUTCTimeCorrParams for Cosmo
	
	Revision 1.1  2004/12/07 10:09:24  giucas
	Moved from libTimeConverter
	
	Revision 1.5  2004/12/03 09:33:26  giucas
	Header fixed
	
	Revision 1.4  2004/11/15 13:31:56  giucas
	Changed FileParser interface to handle multi-file initialization
	
	Revision 1.3  2004/10/12 07:41:49  giucas
	Added getFileName and getFileType methods
	
	Revision 1.2  2004/10/06 07:17:52  giucas
	Added parse method overload for USCTimeCorrParams parameters.
	
	Revision 1.1  2004/09/16 16:57:32  giucas
	First release
	


*/

#ifndef _OBTFileParser_H_
#define _OBTFileParser_H_ 

#include <AuxFileParser.h>


_ACS_BEGIN_NAMESPACE(acs)

/**
 * This class specifics the parsing strategy for Time Correlation File
 */

class OBTFileParser : public AuxFileParser
{
public:


	/*! class OBTFileParserException declaration */
	exDECLARE_EXCEPTION(OBTFileParserException,exException) ; // PRQA S 2131, 2502


	/**
	 * \brief  Class constructor 
	 */	
	OBTFileParser() : AuxFileParser() {}

	/**
	 * \brief  Class copy constructor 
	 */ 
	OBTFileParser(const OBTFileParser &r ) : AuxFileParser(r) {} 

	/**
	 * \brief  Class destructor 
	 */
	virtual ~OBTFileParser() throw() {} // PRQA S 2131

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
	OBTFileParser &operator=(const OBTFileParser &r) { if (this != &r) {AuxFileParser::operator=(r) ; } return *this ; }
};

_ACS_END_NAMESPACE

#endif //_OBTFileParser_H_
