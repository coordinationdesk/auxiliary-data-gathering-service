//PRQA S 1050 EOF
/*

	Copyright 1995-2017, Advanced Computer Systems , SpA.
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
	Revision 1.3  2016/07/08 12:42:20  marfav
	Application definition management fixed
	
	Revision 1.2  2013/09/25 16:59:29  nicvac
	S2PDGS-466: record client downloading.
	
	Revision 1.1  2013/03/26 10:41:08  enrcar
	EC:: Interface modified
	
	
*/

#ifndef _MultiDownloadInventoryObject_H_
#define _MultiDownloadInventoryObject_H_

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


class MultiDownloadInventoryObject: 
	public PROJECT_APP
{
private:
	enum Cmdparam { NOARGS=0x00, FTYPE, XFTYPE, SOURCE, XSOURCE, FNAME, XFNAME, DATE, WHERE, ALL, ALL_NOW, ENSTATS, DESTPATH, EXT_DBL, EXT_HDR, EXT_UNSPLIT, DEMO, CLIENTNAME } ;

public:
	MultiDownloadInventoryObject(const std::string &appName, const std::string &subSystem);
	virtual ~MultiDownloadInventoryObject() throw() ;
	virtual int main(int argc, char const * const * argv, char const * const * env);

private:
	MultiDownloadInventoryObject() ;
	MultiDownloadInventoryObject(const MultiDownloadInventoryObject&);
	MultiDownloadInventoryObject & operator=(const MultiDownloadInventoryObject&);
	virtual int usage(const std::string &) const ;
	void dumpParameters() ;
	int doWork() ;

private:
	bool _verbose ;

	std::map < Cmdparam, std::vector<std::string> > _paramsMap ; 
	std::vector <Cmdparam> _usedParams ; 
	std::string _def_ext_hdr, _def_ext_dbl, _def_ext_unsplit ;

	ACS_CLASS_DECLARE_DEBUG_LEVEL( MultiDownloadInventoryObject) ;
};


_ACS_END_NAMESPACE


#endif /* _MultiDownloadInventoryObject_H_ */
