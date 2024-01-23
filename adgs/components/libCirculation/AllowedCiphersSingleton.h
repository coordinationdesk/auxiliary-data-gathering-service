// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2015, Advanced Computer Systems , Inc.
 *  Via Della Bufalotta, 378 - 00139 Roma - Italy
 *  http://www.acsys.it
 *
 *  All Rights Reserved.
 *
 *  This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
 *  the contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of Advanced Computer Systems, Inc.
 *
 *  $Prod: A.C.S. libCirculation Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 7.0  2016/10/12 10:08:26  lucio.pulvirenti
 *  Pre S1PDGS-31034 schema no more supported: t_circulationsqueue nextretrydate columns must exist
 *
 *  Revision 6.0  2016/08/05 10:21:05  marfav
 *  Creating a major release due to db schema change involved
 *
 *  Revision 1.2  2015/02/19 14:55:25  francesco.avanzi
 *  Refactoring took place:
 *  AllowedCiphersSingleton is now a ParamConstructibleSingletonMap.
 *  It aggregate CirculationAllowedCiphers.
 *
 *  Revision 1.1  2015/02/05 13:37:42  francesco.avanzi
 *  work in progress
 *
 *
 *
 *
 */
#ifndef _AllowedCiphersSingleton_H_
#define _AllowedCiphersSingleton_H_

#include <acs_c++config.hpp>
#include <Singleton.hpp>
#include <ThreadSafe.h>
#include <dbPersistent.h>
#include <map>

_ACS_BEGIN_NESTED_NAMESPACE(acs, circulation) 
using pattern::ParamConstructibleSingletonMap ;


class CirculationAllowedCiphers // PRQA S 2109
{
public:
    
    explicit CirculationAllowedCiphers(const std::string &dbKey);

    bool getCiphers(dbPersistent::IdType, std::pair<std::string, std::string> &);
    
    bool forceUpdateAllowedCiphers();
	~CirculationAllowedCiphers() throw() {}
 

private:
    CirculationAllowedCiphers(); // not Implemented
    CirculationAllowedCiphers(const CirculationAllowedCiphers &); // not Implemented
    CirculationAllowedCiphers& operator=(const CirculationAllowedCiphers &); // not Implemented
    void updateAllowedCiphers(dbPersistent::IdType);
    bool makeUpdate();
private:
    std::string dbKey_ ;
    Mutex m_;
    std::map < dbPersistent::IdType, std::pair<std::string, std::string> > everyAllowedCiphers_ ; 
    time_t lastUpdate_ ; // UdM [s]
    time_t updateDelta_ ; // UdM [s]
};

class AllowedCiphersSingleton: // PRQA S 2109, 2153
    public ParamConstructibleSingletonMap<CirculationAllowedCiphers, std::string> {
    
    friend CirculationAllowedCiphers* ParamConstructibleSingletonMap<CirculationAllowedCiphers, std::string>::instance(const std::string &); // PRQA S 2107
public:
	virtual ~AllowedCiphersSingleton() throw() ;
	 
protected:
	explicit AllowedCiphersSingleton(const std::string &key) ;
private:
	AllowedCiphersSingleton &operator=(const AllowedCiphersSingleton &) ; // declared but not implemented
	AllowedCiphersSingleton (const AllowedCiphersSingleton &) ;          // declared but not implemented
	AllowedCiphersSingleton () ;                       // declared but not implemented

};



_ACS_END_NESTED_NAMESPACE;

#endif // _AllowedCiphersSingleton_H_
