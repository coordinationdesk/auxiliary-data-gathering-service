// PRQA S 1050 EOF
/*

	Copyright 2012-2013, Advanced Computer Systems , Inc.
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
	
	Revision 5.0  2015/02/17 11:25:24  marfav
	Field unique_id added to table t_smactions
	
	Revision 4.2  2013/12/03 19:06:16  marpas
	qa warnings
	
	Revision 4.1  2013/10/31 17:07:17  marpas
	UniqueNameFormatter concept added
	getReportUniqueNameFormatter method added
	this allow a report to be generated using a unique name.
	
	Revision 4.0  2013/09/30 12:30:52  marpas
	the following classes have been removed:
	dbDistributionPolicy
	dbDistributionPolicyQuery
	dbIORepository
	dbIORepositoryQuery
	dbReceptionPolicy
	dbReceptionPolicyQuery
	
	Revision 3.0  2013/09/19 14:57:19  marfav
	dbSMactions now include inventory id pointers
	
	Revision 2.2  2013/02/06 16:02:42  marpas
	coding best practices applied
	exception simplified
	qa rules
	
	Revision 2.1  2012/03/09 10:53:23  marpas
	S1PDGSFixedHeaderFactory created and tested
	all test compilation fixed
	

*/

#ifndef _S1PDGSFixedHeaderFactory_H_
#define _S1PDGSFixedHeaderFactory_H_


#include <acs_c++config.hpp>
#include <dbFixedHeaderFactoryChain.h>


_ACS_BEGIN_NAMESPACE(acs)

class dbFixedHeaderWriter ;
class dbInventoryObject;


class S1PDGSFixedHeaderFactory : public dbFixedHeaderFactory { // PRQA S 2109
public:
    class S1RepUniqueNameFormatter : public UniqueNameFormatter { // PRQA S 2153
    public:
        S1RepUniqueNameFormatter() ;
	    virtual ~S1RepUniqueNameFormatter() throw() ;
        S1RepUniqueNameFormatter(const S1RepUniqueNameFormatter &) ;
        S1RepUniqueNameFormatter& operator= (const S1RepUniqueNameFormatter &) ;
        virtual std::string next(const std::string &previous) ;
        virtual void reset() ;
    private:
        int counter_ ; 
    } ;
    
	S1PDGSFixedHeaderFactory() ;
	virtual ~S1PDGSFixedHeaderFactory() throw() ;

	/**
	 * allocates a dbFixedHeaderWriter ... its destruction is left to the caller.
	 */
	virtual void getWriter(dbInventoryObject const &, std::string const &system, XMLOstream &, dbFixedHeaderWriter *&) const ;
	virtual std::string getFilename(dbInventoryObject const &) const ;
	virtual int getSatelliteId() const ;
    virtual UniqueNameFormatter &getReportUniqueNameFormatter() const ;
    static void checkConfiguration(void *) ;
    
private:
    static std::string centreid_ ;

    mutable S1RepUniqueNameFormatter repunamefmt_ ;

	// declared but not implemented
	S1PDGSFixedHeaderFactory(const S1PDGSFixedHeaderFactory & ) ;
	S1PDGSFixedHeaderFactory &operator=(const S1PDGSFixedHeaderFactory & ) ;
} ;


_ACS_END_NAMESPACE


#endif // _S1PDGSFixedHeaderFactory_H_
