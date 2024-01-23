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

	$Prod: $

	$Id$

	$Author$

	$Log$
	Revision 2.6  2013/03/14 15:17:18  marpas
	connection pool concept is left to class inheriting from Application
	
	Revision 2.5  2013/03/12 17:58:07  marpas
	dbConnectionPool is mandatory in file actions
	
	Revision 2.4  2012/02/14 15:17:53  marpas
	fixing dtor
	
	Revision 2.3  2011/09/22 12:05:40  marpas
	args management follows the new standard, logname naming changed
	
	Revision 2.2  2011/09/22 10:21:42  marpas
	usage method rewritten for standard
	
	Revision 2.1  2010/06/15 09:44:13  marpas
	HAS_MISSION_INCLUDES management added
	
	Revision 2.0  2006/02/28 10:38:17  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1  2005/11/17 13:28:47  serper
	Added interface file, first issue
	
*/

#if defined (HAS_MISSION_INCLUDES)
        #include <ProjectAppIncludes>
#elif GENERIC_STATION_APP
	#include <StationApp.h>	
	#define PROJECT_APP	StationApp			
#elif COSMO
	#include <CosmoApp.h>					
	#define PROJECT_APP	CosmoApp			
#elif CAL_VAL_APP
	#include <CalValApp.h>					
	#define PROJECT_APP	CalValApp			
#else
    #include <CryosatApp.h>
	#define PROJECT_APP	CryosatApp			
#endif


namespace acs
{
class DownloadInventoryFile: 
	public PROJECT_APP
{

public:
	DownloadInventoryFile(const std::string &appName, const std::string &subSystem);
	virtual ~DownloadInventoryFile() throw() ;
	virtual int main(int argc, char const * const * argv, char const * const * env);

	virtual std::string getDefaultLogName() const;

private:
	DownloadInventoryFile(const DownloadInventoryFile&);
	DownloadInventoryFile & operator=(const DownloadInventoryFile&);

	virtual int usage(const std::string &) const ;

private:
    vector<char *> vargs_ ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL (DownloadInventoryFile);

};

}//namespace
