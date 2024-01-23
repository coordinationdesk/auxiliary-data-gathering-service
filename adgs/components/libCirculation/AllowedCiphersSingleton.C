// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2021, Advanced Computer Systems , Inc.
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
 */
#include <AllowedCiphersSingleton.h>
#include <ImplicitCirculationConfigurer.h>

#include <dbConnectionPool.h>
#include <dbQuery.h>
#include <dbSet.h>


_ACS_BEGIN_NESTED_NAMESPACE(acs, circulation) 
using namespace std;

bool CirculationAllowedCiphers::getCiphers(dbPersistent::IdType key, pair<std::string, std::string> &p)
{
    Lock lock(m_ );
    updateAllowedCiphers(key);
    
    bool keyIsFound =  everyAllowedCiphers_.find(key) != everyAllowedCiphers_.end()   ;
   
    if ( keyIsFound )
    {
        p = everyAllowedCiphers_[key] ;
    }
    
    return keyIsFound ; 
}

bool CirculationAllowedCiphers::makeUpdate()
{
    Lock lock(m_) ;       
    dbConnectionPool & pool = db::ConnPools::instance()->getPool(dbKey_) ;
    dbConnectionWrapper cw(pool) ;
    dbConnection &conn = cw; // PRQA S 3050

    SQLString theString(dbKey_, SQLString::SELECT );

/*    
*    select CP.id , TR1.allowedciphers , TR2.allowedciphers from t_circulationpolicies AS CP 
*    join t_repositories as TR1 on (CP.source_id = TR1.id_repository)
*    join t_repositories as TR2 on (CP.destination_id = TR2.id_repository)
*/    


    // Add Select List
    theString.addSelectList("CP.id");
    theString.addSelectList("TR1.allowedciphers");
    theString.addSelectList("TR2.allowedciphers");


    // Table
    theString.addTableList ("t_circulationpolicies", "CP");

    // Joins
    {
   		dbQueryParameters pp( dbKey_);
 		pp.addParameter(dbParam( dbKey_, "TR1","id_repository" , dbParam::EQUAL_TO, "CP.source_id")) ;
		theString.addJoinList(SQLDialect::INNER,"t_repositories TR1",pp) ;
    }
    {
   		dbQueryParameters pp( dbKey_);
 		pp.addParameter(dbParam( dbKey_, "TR2","id_repository" , dbParam::EQUAL_TO, "CP.destination_id")) ;
		theString.addJoinList(SQLDialect::INNER,"t_repositories TR2",pp) ;
    }
    dbQuery q(conn, theString, "SEL repositories ciphers " );
    dbSet set;
    bool isUpdated = false ;
    try
    {
        q.execForSet(set);

        everyAllowedCiphers_.clear() ;
        size_t upTo = set.size() ;
        for(size_t i = 0 ; i < upTo; ++i)
        {
            dbRow row(dbKey_) ;
            set.getNextRow(row);
            dbPersistent::IdType key = 0 ; 
            row.column(0, key);
            string fromCiph;
            row.column(1, fromCiph);
            string toCiph;
            row.column(2, toCiph); // PRQA S 4400
            everyAllowedCiphers_[key] = make_pair(fromCiph, toCiph);

        }
        isUpdated = true ;
    }
    catch(...)
    {
        // keeping old everyAllowedCiphers_
    }
    return isUpdated ;

}

void CirculationAllowedCiphers::updateAllowedCiphers(dbPersistent::IdType key)
{
    Lock lock(m_) ;
    
    time_t tempTimeZero = time(0) ;
    
    bool failure = ( -1 == tempTimeZero);
    
    // Use the default connection pool 
    if( (failure) || ( tempTimeZero - updateDelta_ > lastUpdate_ ) || everyAllowedCiphers_.find(key) != everyAllowedCiphers_.end())
    {
        makeUpdate();
        
        if( not failure )
        {
            lastUpdate_ = tempTimeZero ;
        }
        else
        {
            ACS_LOG_WARNING ( "time(0) call failed!" )
        }
    }    
}


bool CirculationAllowedCiphers::forceUpdateAllowedCiphers()
{
    Lock lock(m_) ;
    return makeUpdate() ;
}


CirculationAllowedCiphers::CirculationAllowedCiphers(const string &dbKey):
    dbKey_(dbKey),
    m_(),
    everyAllowedCiphers_(),
    lastUpdate_(),
    updateDelta_( acs::conf::CirculationConfigurerSingleton::instance()->getUpdateDelta() )
{   
}

_ACS_END_NESTED_NAMESPACE;
