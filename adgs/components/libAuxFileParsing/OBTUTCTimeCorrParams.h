// PRQA S 1050 EOF
/*

	Copyright 1995-2014, Advanced Computer Systems , Inc.
	Via della Bufalotta, 378 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$
	
	$Log$
	Revision 2.2  2014/03/19 19:14:55  marpas
	coding best practices application in progress (not ended)
	warning fixing (qa & g++)
	
	Revision 2.1  2006/10/18 18:08:14  marpas
	hopefully almost complete refactoring
	
	Revision 2.0  2006/02/28 09:32:51  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.4  2005/11/02 13:25:33  giucas
	Added new long double param and operators implemented
	
	Revision 1.3  2005/10/28 18:05:15  danalt
	added parameters for compatibility with test data generation
	
	Revision 1.2  2005/10/25 17:16:15  giucas
	Modified to use for COSMO OBT-UTC conversion
	
	Revision 1.1.1.1  2005/02/21 14:43:53  giucas
	Imported libAuxFileParsing
	
	Revision 1.1  2005/02/02 09:18:59  danalt
	first version
	


*/

#ifndef _OBTUTCTimeCorrParams_H_
#define _OBTUTCTimeCorrParams_H_ 

#include <AuxFileParser.h>

_ACS_BEGIN_NAMESPACE(acs)


/**
 * This class holds reference OBTUTC parameters used for OBT-UTC Time conversions
 */	
class OBTUTCTimeCorrParams : public AuxFileParser::Parameters // PRQA S 2153
{
public:

	/**
	 * \brief  Class constructor 
	 */	
	OBTUTCTimeCorrParams();

	/**
	 * \brief  Class destructor 
	 */
	virtual ~OBTUTCTimeCorrParams() throw() ;

	/**
	 * \brief  Class copy constructor 
	 */ 
	OBTUTCTimeCorrParams(const OBTUTCTimeCorrParams & ); 

	/**
	 * \brief  Operator= 
	 */
	OBTUTCTimeCorrParams &operator=(const OBTUTCTimeCorrParams &);


	/**
	* \brief  Reference Mission
	*/
	int     refMission; // PRQA S 2100 L1

	/**
	 * \brief  Reference counter
	 */
	unsigned long long int	OBRTUnits;

	/**
	* \brief  Reference OBT time
	*/
	std::string     refOBTTime;

	/**
	 * \brief  Reference UTC time
	 */
	std::string	refUTCTime;

	/**
	 * \brief  Reference UTC time in long double format
	 */
	long double	dRefUTCTime;
    // PRQA L:L1 

	/**
	 * \brief  Return a dump of object state
	 */
	std::string dump() const ;

};//end OBTUTCTimeCorrParams class

inline bool operator<=(const OBTUTCTimeCorrParams &a, const OBTUTCTimeCorrParams &b) // PRQA S 2134
{ 
	return a.dRefUTCTime <= b.dRefUTCTime ; 
}

inline bool operator<(const OBTUTCTimeCorrParams &a, const OBTUTCTimeCorrParams &b) // PRQA S 2134
{ 
	return a.dRefUTCTime < b.dRefUTCTime ; 
}


_ACS_END_NAMESPACE


#endif //_OBTUTCTimeCorrParams_H_
