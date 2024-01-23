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
	Revision 1.2  2013/12/05 13:58:53  marpas
	coding best practices applied
	
	Revision 1.1  2013/08/09 12:29:36  lucio.pulvirenti
	First issue.
	
	
	
*/

#ifndef _PlainSatelliteMissionAssociator_H_
#define _PlainSatelliteMissionAssociator_H_ 

#include <SatelliteMissionAssociator.h>

_ACS_BEGIN_NAMESPACE(acs)

class PlainSatelliteMissionAssociator :  public SatelliteMissionAssociator // PRQA S 2109
{

public:


	/**
	 * \brief  Class constructor 
	 *    
	 */
	PlainSatelliteMissionAssociator();

	/**
	 * \brief  Class destructor 
	 */    	
	virtual ~PlainSatelliteMissionAssociator() throw();

	virtual std::map< std::string, std::string > getSatelliteAcronymMap() { return _satelliteAcronymMap; } // PRQA S 2131
	virtual std::map< std::string, int > getMissionIdMap() { return _missionIdMap; } // PRQA S 2131

private:
	// copy constructor and operator= defined but not implemented
	PlainSatelliteMissionAssociator(const PlainSatelliteMissionAssociator &); 
	PlainSatelliteMissionAssociator &operator=(const PlainSatelliteMissionAssociator &);
// data
private:

	static const std::map< std::string, std::string > _satelliteAcronymMap ;
	static const std::map< std::string, int > _missionIdMap ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(PlainSatelliteMissionAssociator)
};


_ACS_END_NAMESPACE

#endif //_PlainSatelliteMissionAssociator_H_
