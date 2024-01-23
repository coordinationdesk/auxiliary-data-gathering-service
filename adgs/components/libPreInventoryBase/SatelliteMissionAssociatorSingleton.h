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

	$Prod: $

	$Id$

	$Author$

	$Log$
	Revision 1.1  2013/08/09 12:23:13  lucio.pulvirenti
	First issue.
	
	
	
*/ 


#ifndef _SatelliteMissionAssociatorSingleton_H_
#define _SatelliteMissionAssociatorSingleton_H_

#include <SatelliteMissionAssociator.h>
#include <Singleton.hpp>
#include <exException.h>
#include <map>

_ACS_BEGIN_NAMESPACE(acs)

using pattern::Singleton ;


class SatelliteMissionAssociatorSingleton : public Singleton<SatelliteMissionAssociatorSingleton> // PRQA S 2109, 2153
{
    friend SatelliteMissionAssociatorSingleton* Singleton<SatelliteMissionAssociatorSingleton>::instance(); // PRQA S 2107
public:
	/*! class KeyNotFound declaration */
	exDECLARE_EXCEPTION(KeyNotFound,exException) ; // PRQA S 2131, 2502 
	 /** SatelliteMissionAssociatorSingleton Exception */
    exDECLARE_EXCEPTION(exSatelliteMissionAssociatorSingletonException, exException) ; // PRQA S 2131, 2153, 2502 

	void registerFactory(std::string const &, SatelliteMissionAssociator* (*)() );

	void unregisterFactory(std::string const &, SatelliteMissionAssociator* (*)() );

	/**
	 * \brief  Return an object derived from SatelliteMissionAssociator class
	 * \param The map key
	 */    	
	SatelliteMissionAssociator *newSatelliteMissionAssociator(std::string const &) const;
    virtual ~SatelliteMissionAssociatorSingleton() throw() ;
protected:

	SatelliteMissionAssociatorSingleton();

private:

	SatelliteMissionAssociatorSingleton(const SatelliteMissionAssociatorSingleton &); // not implemented

	SatelliteMissionAssociatorSingleton& operator=(const SatelliteMissionAssociatorSingleton &); // not implemented

// data
private:
	std::map<std::string, SatelliteMissionAssociator* (*)()> _map;


	ACS_CLASS_DECLARE_DEBUG_LEVEL(SatelliteMissionAssociatorSingleton)

};

_ACS_END_NAMESPACE

#endif //_SatelliteMissionAssociatorSingleton_H_
