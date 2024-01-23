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

	$Id$

	$Author$

	$Log$
	Revision 1.1  2013/08/09 12:24:52  lucio.pulvirenti
	First issue.
	
	

*/


#ifndef _SatelliteMissionAssociatorFactory_H_
#define _SatelliteMissionAssociatorFactory_H_ 

#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)

class SatelliteMissionAssociator;

class SatelliteMissionAssociatorFactory // PRQA S 2109
{
public:

	SatelliteMissionAssociatorFactory();

	~SatelliteMissionAssociatorFactory() throw() ;

	static SatelliteMissionAssociator* buildStdSatelliteMissionAssociator();		
	static SatelliteMissionAssociator* buildPlainSatelliteMissionAssociator();		

private:
    SatelliteMissionAssociatorFactory(const SatelliteMissionAssociatorFactory &) ; // not implemented
    SatelliteMissionAssociatorFactory& operator= (const SatelliteMissionAssociatorFactory &) ; // not implemented
private:

	ACS_CLASS_DECLARE_DEBUG_LEVEL(SatelliteMissionAssociatorFactory)

};

_ACS_END_NAMESPACE

#endif // _SatelliteMissionAssociatorFactory_H_

