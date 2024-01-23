// PRQA S 1050 EOF
/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod:  A.C.S. WS Config Merger $

	$Id$

	$Author$

	$Log$
	Revision 2.6  2015/11/19 16:43:58  marpas
	coding best practices application in progress
	
	Revision 2.5  2013/06/20 10:53:18  marfav
	Aligned to libException v5
	
	Revision 2.4  2012/02/13 11:39:53  marpas
	refactoring
	
	Revision 2.3  2010/06/15 11:35:27  marpas
	HAS_MISSION_INCLUDES management added
	
	Revision 2.2  2007/04/19 08:53:55  giucas
	Removed dependencies from StationApp for ENVISAT
	
	Revision 2.1  2006/04/21 13:36:50  clanas
	implemented StandAlone application model
	
	Revision 2.0  2006/02/28 17:57:53  paoscu
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/09/02 13:24:06  danalt
	added Cosmo project
	
	Revision 1.1.1.1  2004/03/12 09:16:50  marfav
	Import WS Config Merger
	
	


*/


#ifndef _WS_CONFIG_MERGER_H
#define _WS_CONFIG_MERGER_H


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

#include <rsResourceSet.h>
#include <exException.h>

#include <string>


_ACS_BEGIN_NAMESPACE(acs)

class WSConfig;

class WSConfigMerger : public acs::PROJECT_APP
{
  public:
	/*! class WSConfigMergerException declaration */
	exDECLARE_EXCEPTION(WSConfigMergerException,acs::exException) ;

 
    enum MergeMode {MM_APPEND, MM_DELETE};

    WSConfigMerger (const std::string &appName, const std::string &subSystem);
    virtual ~WSConfigMerger() throw() ;

    virtual int main(int argc, char const * const * argv, char const * const * env);
    void run();

    virtual int usage(const std::string &) const;


  private: // methods
    void loadTLConfiguration();
    void loadWSConfiguration();
    void loadDeltaWSConfiguration();
    void mergeConfigurations(WSConfig &_wsConfiguration, const WSConfig &_wsDeltaConfiguration);
    void subtractConfiguration(WSConfig &_wsConfiguration, const WSConfig &_wsDeltaConfiguration);

  WSConfigMerger& operator = (const WSConfigMerger& d);
  WSConfigMerger (const WSConfigMerger& d);


  private: // attributes
    std::string _WSConfigPath;
    std::string _deltaWSConfigPath;
    WSConfig *_wsConfiguration ;
    WSConfig *_wsDeltaConfiguration;
    MergeMode _mergeMode;

  private: // static stuff...
    static std::string getNodeName();

    ACS_CLASS_DECLARE_DEBUG_LEVEL (WSConfigMerger)
};


_ACS_END_NAMESPACE

#endif // _WS_CONFIG_MERGER_H
