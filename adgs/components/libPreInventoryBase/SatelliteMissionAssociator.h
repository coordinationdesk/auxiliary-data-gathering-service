// PRQA S 1050 EOF
/*

	Copyright 2013-2019, Advanced Computer Systems , Inc.
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
	
	Revision 1.1  2013/08/09 12:26:18  lucio.pulvirenti
	First issue.
	
	
	
*/

#ifndef _SatelliteMissionAssociator_H_
#define _SatelliteMissionAssociator_H_ 

#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

class SatelliteMissionAssociator // PRQA S 2109
{

public:

	exDECLARE_EXCEPTION(exKeyNotFoundException, exIllegalValueException) ; // Exception launched by getSatelliteAcronymMap and getMissionIdMap if map key not found. // PRQA S 2131, 2502

	/**
	 * \brief  Class constructor 
	 *    
	 */
	SatelliteMissionAssociator();

	/**
	 * \brief  Class destructor 
	 */    	
	virtual ~SatelliteMissionAssociator() throw();


	/**
	 * \brief Return the satellite acronym.
	 * \param The satellite code 
	 * \ret The satellite acronym 
	 */
	virtual std::string getSatelliteAcronym(std::string const &);

	/**
	 * \brief Return the mission.
	 * \param The satellite code 
	 * \ret The satellite mission 
	 */
	virtual int getMissionId(std::string const &) ;

	virtual std::map< std::string, std::string > getSatelliteAcronymMap() = 0;
	virtual std::map< std::string, int > getMissionIdMap() = 0;



private:
	// copy constructor and operator= defined but not implemented
	SatelliteMissionAssociator(const SatelliteMissionAssociator &); 
	SatelliteMissionAssociator &operator=(const SatelliteMissionAssociator &);


// data
private:

	ACS_CLASS_DECLARE_DEBUG_LEVEL(SatelliteMissionAssociator)
};


_ACS_END_NAMESPACE

#endif //_SatelliteMissionAssociator_H_
