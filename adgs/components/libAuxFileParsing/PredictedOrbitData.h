// PRQA S 1050 EOF
/*
	Copyright 1995-2014, Advanced Computer Systems , Inc.
	Via della Bufalotta, 347 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Id$

	$Author$

	$Log$
	Revision 2.2  2014/03/19 19:14:56  marpas
	coding best practices application in progress (not ended)
	warning fixing (qa & g++)
	
	Revision 2.1  2006/10/18 18:08:14  marpas
	hopefully almost complete refactoring
	
	Revision 2.0  2006/02/28 09:32:51  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/11/16 10:33:45  giucas
	Added operators < and <=
	
	Revision 1.1.1.1  2005/02/21 14:43:53  giucas
	Imported libAuxFileParsing
	
	Revision 1.2  2004/12/09 16:15:46  giucas
	Changed attribute names
	
	Revision 1.1  2004/12/06 17:34:07  giucas
	Imported sources
	

*/

#ifndef _PredictedOrbitData_H_
#define _PredictedOrbitData_H_


#include <exException.h>
#include <AuxFileParser.h>


_ACS_BEGIN_NAMESPACE(acs)

/*
 * Data structure for Predicted Orbit information
 */
class PredictedOrbitData : public AuxFileParser::Parameters // PRQA S 2153
{
public:

	/**
	* \brief Class constructor
	*/
	PredictedOrbitData();

	/**
	* \brief  Class destructor
	*/ 
	virtual ~PredictedOrbitData() throw() ;

	/**
	* \brief Class copy constructor
	*/
	PredictedOrbitData(const PredictedOrbitData&);

	/**
	* \brief Class operator=
	*/	
	PredictedOrbitData& operator=(const PredictedOrbitData&);

	/*
	 * State Vector Time expressed in TAI MJD
	 */
	long double taiTime; // PRQA S 2100 L1

	/*
	 * State Vector Time expressed in UTC MJD
	 */
	long double utcTime;

	/*
	 * State Vector Time expressed in UT1 MJD
	 */
	long double ut1Time;

	/*
	 * State Vector X position expressed in m
	 */
	double xPosition;

	/*
	 * State Vector Y position expressed in m
	 */
	double yPosition;

	/*
	 * State Vector Z position expressed in m
	 */
	double zPosition;

	/*
	 * State Vector X velocity expressed in m/s
	 */
	double xVelocity;

	/*
	 * State Vector Y velocity expressed in m/s
	 */
	double yVelocity;

	/*
	 * State Vector Z velocity expressed in m/s
	 */
	double zVelocity;

	/*
	 * State Vector absolute orbit
	 */			
	long int	absOrbit;
    // PRQA L:L1

	/*
	 * Show object state
	 */			
	std::string show() const ;
};

inline bool operator<=(const PredictedOrbitData &a, const PredictedOrbitData &b) // PRQA S 2134 
{ 
	return a.utcTime <= b.utcTime ; 
}

inline bool operator<(const PredictedOrbitData &a, const PredictedOrbitData &b) // PRQA S 2134 
{ 
	return a.utcTime < b.utcTime ; 
}



_ACS_END_NAMESPACE

#endif //_PredictedOrbitData_H_
