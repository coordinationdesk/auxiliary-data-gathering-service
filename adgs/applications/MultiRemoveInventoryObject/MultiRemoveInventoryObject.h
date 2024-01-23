/*

	Copyright 1995-2019, Exprivia SPA
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
	Revision 1.1  2013/02/26 15:12:25  enrcar
	EC:: Application updated
	

	
	
*/

#ifndef _MultiRemoveInventoryObject_H_
#define _MultiRemoveInventoryObject_H_



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


class MultiRemoveInventoryObject: 
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
private:
	enum Cmdparam { NOARGS=0x00, FTYPE, XFTYPE, SOURCE, XSOURCE, FNAME, XFNAME, DATE, WHERE, ALL, ALL_NOW, ENSTATS, DEMO } ; 

public:
	MultiRemoveInventoryObject(const std::string &appName, const std::string &subSystem);
	virtual ~MultiRemoveInventoryObject() throw() ;
	virtual int main(int argc, char const * const * argv, char const * const * env);

private:
	MultiRemoveInventoryObject() ;
	MultiRemoveInventoryObject(const MultiRemoveInventoryObject&);
	MultiRemoveInventoryObject & operator=(const MultiRemoveInventoryObject&);
	virtual int usage(const std::string &) const ;
	void dumpParameters() ;
	int doWork() ;
	
	size_t queryThreshold() const;

private:
	bool _verbose ;

	std::map < Cmdparam, std::vector<std::string> > _paramsMap ; 
	std::vector <Cmdparam> _usedParams ; 

	ACS_CLASS_DECLARE_DEBUG_LEVEL (MultiRemoveInventoryObject);
};


_ACS_END_NAMESPACE


#endif /* _MultiRemoveInventoryObject_H_ */
