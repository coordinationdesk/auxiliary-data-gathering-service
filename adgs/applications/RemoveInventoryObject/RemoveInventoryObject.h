/*

	Copyright 2010-2013, Advanced Computer Systems , Inc.
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
	Revision 1.5  2013/03/14 16:00:06  marpas
	connection pool concept is left to class inheriting from Application
	
	Revision 1.4  2013/02/21 08:46:30  enrcar
	EC:: QAreports improved
	
	Revision 1.3  2013/02/21 08:23:39  enrcar
	EC:: exFMessage used for filterable / info messages
	
	Revision 1.2  2013/02/20 10:55:37  enrcar
	EC:: Application refactoring
	
	Revision 1.1  2011/09/23 14:26:20  danalt
	added first version
	
	
*/

#ifndef _RemoveInventoryObject_H_
#define _RemoveInventoryObject_H_



#if defined (HAS_MISSION_INCLUDES)
    #include <ProjectAppIncludes>
#elif GENERIC_STATION_APP
	#include <StationApp.h>					
#elif COSMO
	#include <CosmoApp.h>					
#elif CAL_VAL_APP
	#include <CalValApp.h>					
#else
    #include <CryosatApp.h>
#endif



_ACS_BEGIN_NAMESPACE(acs)


class RemoveInventoryObject: 
#if defined (HAS_MISSION_INCLUDES)
	public PROJECT_APP
#elif GENERIC_STATION_APP
	public StationApp
#elif COSMO
	public CosmoApp
#elif CAL_VAL_APP
	public CalValApp 
#else
	public CryosatApp 
#endif
{

public:
	RemoveInventoryObject(const std::string &appName, const std::string &subSystem);
	virtual ~RemoveInventoryObject() throw() ;
	virtual int main(int argc, char const * const * argv, char const * const * env);

private:
	RemoveInventoryObject() ;
	RemoveInventoryObject(const RemoveInventoryObject&);
	RemoveInventoryObject & operator=(const RemoveInventoryObject&);
	virtual int usage(const std::string &) const ;
	int doWork(const std::vector<std::string>&) ;

private:
	bool _removeExt_firstdot ;
	bool _removeExt_lastdot ; 

	ACS_CLASS_DECLARE_DEBUG_LEVEL(RemoveInventoryObject)
};


_ACS_END_NAMESPACE


#endif /* _RemoveInventoryObject_H_ */
