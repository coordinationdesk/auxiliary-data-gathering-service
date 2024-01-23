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
	Revision 2.3  2012/04/13 12:35:32  giucas
	Old style cast removed
	
	Revision 2.2  2006/10/18 18:08:14  marpas
	hopefully almost complete refactoring
	
	Revision 2.1  2006/10/18 16:38:08  marpas
	new interface released to get rid of involuntary dependencies
	
	Revision 2.0  2006/02/28 09:32:51  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/08/02 13:56:50  clanas
	inserted math.h  to be compliant with gcc 3.4.3
	
	Revision 1.1.1.1  2005/02/21 14:43:53  giucas
	Imported libAuxFileParsing
	
	Revision 1.1  2004/12/07 10:09:24  giucas
	Moved from libTimeConverter
	
	Revision 1.3  2004/12/03 09:33:26  giucas
	Header fixed
	
	Revision 1.2  2004/11/16 08:53:03  giucas
	Added filename attribute
	
	Revision 1.1  2004/09/16 16:57:17  giucas
	First release
	


*/

#ifndef _CFGOTimeCorrParams_H_
#define _CFGOTimeCorrParams_H_ 

#include <AuxFileParser.h>
#include <cmath>

_ACS_BEGIN_NAMESPACE(acs)

/**
 * This class holds Coarse-Fine-Gradient-Offset parameters used for UTC-SBT Time conversions
 */	
class CFGOTimeCorrParams: public AuxFileParser::Parameters // PRQA S 2153
{
public:

	/**
	 * \brief  Class constructor 
	 */	
	CFGOTimeCorrParams();

	/**
	 * \brief  Class destructor 
	 */
	virtual ~CFGOTimeCorrParams() throw() ;

	/**
	 * \brief  Class copy constructor 
	 */ 
	CFGOTimeCorrParams(const CFGOTimeCorrParams & ); 

	/**
	 * \brief  Operator= 
	 */
	CFGOTimeCorrParams &operator=(const CFGOTimeCorrParams &);

	/**
	 * \brief  The name of the TimeCorrelation file where the params have been extracted from.
	 */
	std::string fileName; // PRQA S 2100 L1

	/**
	 * \brief  UTC Time in ascii format 
	 */
	std::string utcTime;

	/**
	 * \brief  UTC0 COARSE Time
	 */
	unsigned long int	utc0Coarse;

	/**
	 * \brief  UTC0 FINE Time
	 */
	unsigned int utc0Fine;

	/**
	 * \brief  OBT0 COARSE
	 */
	unsigned long int	obt0Coarse;

	/**
	 * \brief  OBT0 FINE
	 */
	unsigned int obt0Fine;

	/**
	 * \brief  Gradient
	 */
	double gradient;

	/**
	 * \brief  Offset
	 */
	double offset; // PRQA L:L1

	/**
	 * \brief  Return a dump of object state
	 */
	std::string dump() const ;

	/**
	 * \brief  Return the value of UTC0 from the COARSE and FINE values
	 */
	double getUTC0() const throw() { return utc0Coarse + utc0Fine/pow(2.0,16.0); } // PRQA S 3011, 3084, 4400

	/**
	 * \brief  Return the value of OBT0 from the COARSE and FINE values
	 */
	double getOBT0() const throw() { return obt0Coarse + obt0Fine/pow(2.0,16.0); }// PRQA S 3011, 3084, 4400


private:

	exCLASS_DECLARE_DEBUG_LEVEL

};//end CFGOTimeCorrParams class

_ACS_END_NAMESPACE

#endif //_CFGOTimeCorrParams_H_
