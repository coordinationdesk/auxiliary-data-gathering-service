// PRQA S 1050 EOF
/*

	Copyright 2012-2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Db Cryosat Library $

	$Id$

	$Author$

	$Log$
	Revision 6.0  2016/07/07 13:44:03  damdec
	CSGACQ-82: special queries modified to be compatible with Oracle
	
	Revision 5.3  2016/04/19 13:53:46  marpas
	coding best practice application in progress
	useless virtual filedName method removed
	
	Revision 5.2  2016/04/15 14:37:04  marpas
	coding best practices application in progress
	
	Revision 5.1  2015/08/07 09:46:45  tergem
	ALSATFixedHeaderFactory class added
	
	

*/

#ifndef _ALSATFixedHeaderFactory_H_
#define _ALSATFixedHeaderFactory_H_


#include <acs_c++config.hpp>
#include <dbFixedHeaderFactoryChain.h>


_ACS_BEGIN_NAMESPACE(acs)

class dbFixedHeaderWriter ;
class dbInventoryObject;


class ALSATFixedHeaderFactory : public dbFixedHeaderFactory { // PRQA S 2109
public:
    class ALSATRepUniqueNameFormatter : public UniqueNameFormatter { // PRQA S 2153
    public:
        ALSATRepUniqueNameFormatter() ;
	    virtual ~ALSATRepUniqueNameFormatter() throw() ;
        ALSATRepUniqueNameFormatter(const ALSATRepUniqueNameFormatter &) ;
        ALSATRepUniqueNameFormatter& operator= (const ALSATRepUniqueNameFormatter &) ;
        virtual std::string next(const std::string &previous) ;
        virtual void reset() ;
    private:
        int counter_ ; 
    } ;
    
	ALSATFixedHeaderFactory() ;
	virtual ~ALSATFixedHeaderFactory() throw() ;

	/**
	 * allocates a dbFixedHeaderWriter ... its destruction is left to the caller.
	 */
	virtual void getWriter(dbInventoryObject const &, std::string const &system, XMLOstream &, dbFixedHeaderWriter *&) const ;
	virtual std::string getFilename(dbInventoryObject const &) const ;
	virtual int getSatelliteId() const ;
    virtual UniqueNameFormatter &getReportUniqueNameFormatter() const ;
    static void checkConfiguration(void *) ;
    
private:
	ALSATFixedHeaderFactory(const ALSATFixedHeaderFactory & ) ; // not implemented 
	ALSATFixedHeaderFactory &operator=(const ALSATFixedHeaderFactory & ) ; // not implemented 
private:
    static std::string centreid_ ;

    mutable ALSATRepUniqueNameFormatter repunamefmt_ ;

} ;


_ACS_END_NAMESPACE


#endif // _ALSATFixedHeaderFactory_H_
