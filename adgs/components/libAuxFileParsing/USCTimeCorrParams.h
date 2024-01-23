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
	Revision 2.3  2014/03/19 19:14:56  marpas
	coding best practices application in progress (not ended)
	warning fixing (qa & g++)
	
	Revision 2.2  2006/10/18 18:08:14  marpas
	hopefully almost complete refactoring
	
	Revision 2.1  2006/10/18 16:38:08  marpas
	new interface released to get rid of involuntary dependencies
	
	Revision 2.0  2006/02/28 09:32:51  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2005/02/21 14:43:53  giucas
	Imported libAuxFileParsing
	
	Revision 1.1  2004/12/07 10:09:24  giucas
	Moved from libTimeConverter
	
	Revision 1.2  2004/12/03 09:33:26  giucas
	Header fixed
	
	Revision 1.1  2004/09/17 14:29:49  giucas
	First Release
	
	


*/

#ifndef _USCTimeCorrParams_H_
#define _USCTimeCorrParams_H_ 

#include <AuxFileParser.h>

_ACS_BEGIN_NAMESPACE(acs)

/**
 * This class holds reference UTC-SBT-ClockStep parameters used for UTC-SBT Time conversions
 */	
class USCTimeCorrParams : public AuxFileParser::Parameters // PRQA S 2153
{
public:

	/**
	 * \brief  Class constructor 
	 */	
	USCTimeCorrParams();

	/**
	 * \brief  Class destructor 
	 */
	virtual ~USCTimeCorrParams() throw() ;

	/**
	 * \brief  Class copy constructor 
	 */ 
	USCTimeCorrParams(const USCTimeCorrParams & ); 

	/**
	 * \brief  Operator= 
	 */
	USCTimeCorrParams &operator=(const USCTimeCorrParams &);

	/**
	 * \brief  Return a dump of object state
	 */
	std::string dump() const ;

	/**
	 * \brief  Reference UTC time
	 */
	long double refTime; // PRQA S 2100

	/**
	 * \brief  Reference SBT
	 */
	std::vector<unsigned long int> refSBT; // PRQA S 2100

	/**
	 * \brief SBT clock steps
	 */
	unsigned long int clockStep; // PRQA S 2100


};//end USCTimeCorrParams class

_ACS_END_NAMESPACE

#endif //_USCTimeCorrParams_H_
