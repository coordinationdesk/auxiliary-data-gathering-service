// PRQA S 1050 EOF
/*

	Copyright 2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod:  $

	$Author$

	Id: $
	
	$Log$
	Revision 1.3  2013/12/05 17:22:02  marpas
	moving static initialization methods in unnamed namespace into compilation unit
	
	Revision 1.2  2013/12/05 13:58:54  marpas
	coding best practices applied
	
	Revision 1.1  2013/08/09 12:28:43  lucio.pulvirenti
	first issue.
	
	
	
*/

#ifndef _StdSatelliteMissionAssociator_H_
#define _StdSatelliteMissionAssociator_H_ 

#include <SatelliteMissionAssociator.h>

_ACS_BEGIN_NAMESPACE(acs)

class StdSatelliteMissionAssociator :  public SatelliteMissionAssociator // PRQA S 2109
{

public:

	/**
	 * \brief  Class constructor 
	 *    
	 */
	StdSatelliteMissionAssociator();

	/**
	 * \brief  Class destructor 
	 */    	
	virtual ~StdSatelliteMissionAssociator() throw();

	virtual std::map< std::string, std::string > getSatelliteAcronymMap() ;

	virtual std::map< std::string, int > getMissionIdMap() ;

private:
	// copy constructor and operator= defined but not implemented
	StdSatelliteMissionAssociator(const StdSatelliteMissionAssociator &); 
	StdSatelliteMissionAssociator &operator=(const StdSatelliteMissionAssociator &);

// data
private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(StdSatelliteMissionAssociator)
};


_ACS_END_NAMESPACE

#endif //_StdSatelliteMissionAssociator_H_
