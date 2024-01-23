/*

	Copyright 1995-2006, Advanced Computer Systems , Inc.
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
	Revision 1.4  2006/10/18 18:08:14  marpas
	hopefully almost complete refactoring
	
	Revision 1.3  2006/10/18 16:38:08  marpas
	new interface released to get rid of involuntary dependencies
	
	Revision 1.2  2006/05/11 15:07:50  giucas
	Added setFileType method
	
	Revision 1.1  2006/05/10 07:26:41  giucas
	Class added
	

*/

#ifndef _STDMFileParser_H_
#define _STDMFileParser_H_ 

#include <AuxFileParser.h>

_ACS_BEGIN_NAMESPACE(acs)


/**
 * This abstract class specifics the parsing strategy interface for STDM File
 */

class STDMFileParser : public AuxFileParser
{
public:

	/**
	 * \brief  Class constructor 
	 */	
	STDMFileParser() : AuxFileParser() {}


	/**
	 * \brief  Class copy constructor 
	 */ 
	STDMFileParser(const STDMFileParser &r ) : AuxFileParser(r) {} 

	/**
	 * \brief  Class destructor 
	 */
	virtual ~STDMFileParser() throw() {} // PRQA S 2131

	/**
	 * \brief  Add a STDM file name path to use for retrieving parameters 
	 */
	virtual void addFileName(const std::string&)=0;

	/**
	 * \brief  Remove a STDM file name path to use for retrieving parameters 
	 */
	virtual void removeFileName(const std::string&)=0;

	/**
	 * \brief  Return the STDM file name paths 
	 */
	virtual std::vector<std::string> getFileNames()=0;

	/**
	 * \brief  Return the STDM file type
	 */
	virtual std::string getFileType()=0;

	/**
	 * \brief  Set the STDM file type
	 */
	virtual void setFileType(const std::string&)=0;
protected:
	/**
	 * \brief  Operator= 
	 */
	STDMFileParser &operator=(const STDMFileParser &r)
    { if (this != &r) { AuxFileParser::operator=(r) ; } return *this ; }
};

_ACS_END_NAMESPACE

#endif //_STDMFileParser_H_
