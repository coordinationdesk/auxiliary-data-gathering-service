// PRQA S 1050 EOF
/*

	Copyright 1995-2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. GarbageCleaner $

	$Id$

	$Author$
	
	$Log$
	Revision 1.9  2016/01/29 11:47:05  marpas
	coding best practices application in progress
	
	Revision 1.8  2012/02/14 15:18:15  marpas
	fixing dtor
	
	Revision 1.7  2010/06/15 09:54:12  marpas
	HAS_MISSION_INCLUDES management added
	
	Revision 1.6  2006/10/03 16:36:08  davcas
	Added STANDALONE define and StandaloneApp
	
	Revision 1.5  2006/06/14 10:36:36  davcas
	Added project Harm
	
	Revision 1.4  2005/09/28 10:11:58  marpas
	header fixed for log
	

*/


#ifndef _GARBAGECLEANERAPP_H_
#define _GARBAGECLEANERAPP_H_

#if defined (HAS_MISSION_INCLUDES)
	#include <ProjectAppIncludes>
#elif GENERIC_STATION_APP
	#include <StationApp.h>			
	#define PROJECT_APP StationApp
#elif COSMO
	#include <CosmoApp.h>					
	#define PROJECT_APP	CosmoApp
#elif CAL_VAL_APP
	#include <CalValApp.h>	
	#define PROJECT_APP	CalValApp
#elif HARM
	# include <HarmApp.h>
	#define PROJECT_APP	HarmApp
#else
	#include <CryosatApp.h>					
	#define PROJECT_APP	CryosatApp	
#endif

#include <string>

namespace acs
{

    class GarbageCleanerApp
	: public PROJECT_APP
{
	public:
		GarbageCleanerApp (std::string const & appName, std::string const & subSystem) ;
		virtual ~GarbageCleanerApp() throw() ;
		virtual int main (int argc, char const* const* argv, char const* const* env);

	private:
		void init();
		const std::string getNodeName();
        GarbageCleanerApp(const GarbageCleanerApp &); // not implemented
        GarbageCleanerApp& operator= (const GarbageCleanerApp &); // not implemented

};

} // namespace

#endif 

