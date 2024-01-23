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
	Revision 2.3  2014/03/19 19:14:55  marpas
	coding best practices application in progress (not ended)
	warning fixing (qa & g++)
	
	Revision 2.2  2006/10/18 18:08:14  marpas
	hopefully almost complete refactoring
	
	Revision 2.1  2006/10/18 16:38:08  marpas
	new interface released to get rid of involuntary dependencies
	
	Revision 2.0  2006/02/28 09:32:51  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/09/16 11:54:47  seralb
	Added some fields for the ErsOrbitDataFileParser
	
	Revision 1.1  2005/07/01 14:10:36  seralb
	Added ERS satellite
	
	Revision 1.1.1.1  2005/02/21 14:43:53  giucas
	


*/

#ifndef _ErsTimeCorrParams_H_
#define _ErsTimeCorrParams_H_ 

#include <AuxFileParser.h>

_ACS_BEGIN_NAMESPACE(acs)

/**
 * This class holds reference UTC-SBT-ClockStep parameters used for UTC-SBT Time conversions
 */	
class ErsTimeCorrParams : public AuxFileParser::Parameters // PRQA S 2153
{
public:

	/**
	 * \brief  Class constructor 
	 */	
	ErsTimeCorrParams();

	/**
	 * \brief  Class destructor 
	 */
	virtual ~ErsTimeCorrParams() throw() ;

	/**
	 * \brief  Class copy constructor 
	 */ 
	ErsTimeCorrParams(const ErsTimeCorrParams & ); 

	/**
	 * \brief  Operator= 
	 */
	ErsTimeCorrParams &operator=(const ErsTimeCorrParams &);

	unsigned int day; // PRQA S 2100 9 
	unsigned int ms;
	unsigned int orbit;
	unsigned int BinTime;
	unsigned int ClockPeriod;

	long double AscNodeUTC;
	long double ReferenceUTC;
	unsigned long SatBinTime;
	unsigned long ClockStepLength; 

	/**
	 * \brief  Return a dump of object state
	 */
	std::string dump() const ;

};//end ErsTimeCorrParams class

_ACS_END_NAMESPACE

#endif //_ErsTimeCorrParams_H_
