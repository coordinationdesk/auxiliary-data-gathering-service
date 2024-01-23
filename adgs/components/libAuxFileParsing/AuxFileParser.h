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
	Revision 2.2  2006/10/18 18:08:14  marpas
	hopefully almost complete refactoring
	
	Revision 2.1  2006/10/18 16:38:08  marpas
	new interface released to get rid of involuntary dependencies
	
	Revision 2.0  2006/02/28 09:32:51  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2005/02/21 14:43:53  giucas
	Imported libAuxFileParsing
	
	Revision 1.1  2004/12/06 13:42:49  giucas
	Imported source
	

*/

#ifndef _AuxFileParser_H_
#define _AuxFileParser_H_ 

#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

/**
 * This class specifics the abstract interface for Auxiliary Files parsing strategies.
 * The specific parse methods will be defined in sub-classes.
 */

class AuxFileParser
{
public:


	/** class SatParameters 
	   *
	   *  an empty class to define Interfaces
	   */

	class Parameters {
	public:
		virtual ~Parameters() throw() {} // PRQA S 2131
		Parameters (const Parameters &r) {}
		Parameters& operator=(const Parameters &) { return *this ;} // PRQA S 4072
	protected: 
		Parameters() {} 

	} ;

	/**
	 * \brief  Class constructor 
	 */	
	AuxFileParser() {} 
	AuxFileParser(const AuxFileParser &) {}

	/**
	 * \brief  Class destructor 
	 */
	virtual ~AuxFileParser() throw() {} // PRQA S 2131

	/**
	 * \brief  Add an auxiliary file name path to parse 
	 */
	virtual void addFileName(const std::string&)=0;

	/**
	 * \brief  Remove an auxiliary file name path
	 */
	virtual void removeFileName(const std::string&)=0;

	/**
	 * \brief  Return the auxiliary file name paths 
	 */
	virtual std::vector<std::string> getFileNames()=0;

	/**
	 * \brief  Return the auxiliary file type
	 */
	virtual std::string getFileType()=0;

	/**
	 * \brief  Parse the File using the template type to store the results
	 *
	 * \param  A vector of SatParameters to store params values during file parsing
	 */
	virtual void parse(std::vector<Parameters *>&) = 0 ;
protected:
	AuxFileParser& operator= (const AuxFileParser &) { return *this ; }// PRQA S 4072

};

_ACS_END_NAMESPACE

#endif //_AuxFileParser_H_
