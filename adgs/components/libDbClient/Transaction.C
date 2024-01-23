// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. DataBase Client Library $

	$Id$

	$Author$

	$Log$
	

*/

#include <Transaction.h>
#include <SQLDialectFactory.h>
#include <SQLDialect.h>
#include <Filterables.h>
#include <dbStat.h>


_ACS_BEGIN_NESTED_NAMESPACE(acs, db)

using acs::math::StatNode ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(Transaction)

StatNode &Transaction::statN(const std::string &key) { return db::stat(key)["TRANS"] ; }

Transaction::Transaction(dbConnection &conn, bool doStat) :
	conn_(conn), // PRQA S 2528
    in_transaction_(false),
    timer_(),
    name_(),
    started_(false),
    stat_(doStat) 
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB)
    if (stat_) { statN(conn_.getKey())["created"] += 1 ; }
}

Transaction::Transaction(dbConnection &conn, const std::string &name, bool doStat) :
	conn_(conn), // PRQA S 2528
    in_transaction_(false),
    timer_(),
    name_(name),
    started_(false),
    stat_(doStat) 
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB)
    if (stat_) { statN(conn_.getKey())["created #"] += 1 ; }
    begin_work(name_) ;
} 						


Transaction::~Transaction() ACS_NOEXCEPT
{
// PRQA S 4631 L1
    try {
        if (in_transaction_) {
            if (stat_) { 
                statN(conn_.getKey())["destroyed #"]["dtor rollback #"] += 1 ;
            }
            roll_back() ;
        }
    }
    catch(std::exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }
    try {
        if (stat_) {  
            statN(conn_.getKey())["destroyed #"] += 1 ;
            if (started_) {
                statN(conn_.getKey())["destroyed #"]["started #"] += 1 ;
            }
            else {
                statN(conn_.getKey())["destroyed #"]["nop #"] += 1 ;
            }
        }
    }
    catch(std::exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
    }
// PRQA L:L1
}

bool Transaction::begin_work(const std::string &name) // PRQA S 4020
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB)
	if(conn_.transactionState() != dbConnection::Begin) // the transaction has not begun yet
	{
        std::string n = name.empty() ? "<unnamed>" : name ; // PRQA S 3384, 3385
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "transaction \"" << n << "\" begin") ;
        conn_.beginWork(name) ; // begin it
		name_ = n ; // assign after - in case of problem the last name is retained
		in_transaction_ = true ; // remember 
        timer_.start() ; 
        if (stat_) {
            const std::string &dbKey = conn_.getKey() ;
            statN(dbKey)["tot begin #"] += 1 ;
            statN(dbKey)[name_]["begin #"] += 1 ;
        }
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "transaction \"" << n << "\" begun") ;
        started_ = true ;
        return true ;
	}
    return false ;
}

void Transaction::commit()
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB)
	if(in_transaction_) // transaction started by this object ?
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "transaction \"" <<  name_ << "\" commit") ;
        timer_.stop() ;
        Timer commitTimer ;
        commitTimer.start() ;
		conn_.commitWork() ; // yes: commit !
        commitTimer.stop() ;
		in_transaction_ = false ; // no longer in transaction
        if (stat_) { 
            const std::string cm = "commit #" ;
            const std::string &dbKey = conn_.getKey() ;
            statN(dbKey)["tot commit #"] += 1 ;
            statN(dbKey)["tot commit #"]["duration [ms]"] += double(timer_.elapsed()) ; // PRQA S 3081
            statN(dbKey)[name_] += 1 ;
            statN(dbKey)[name_][cm] += 1 ;
            statN(dbKey)[name_][cm]["duration [ms]"] += double(timer_.elapsed()) ; // PRQA S 3081
            statN(dbKey)[name_][cm]["tr commit time [ms]"] += double(commitTimer.elapsed()) ; // PRQA S 3081
        }
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "transaction \"" <<  name_ << "\" committed") ;
	}
}

void Transaction::roll_back()
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB)
	if(in_transaction_) // transaction started by this object ?
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "transaction \"" <<  name_ << "\" rolling back") ;
        timer_.stop() ;
        Timer rollbackTimer ;
        rollbackTimer.start() ;
		conn_.rollBack() ; // yes: rollback
        rollbackTimer.stop() ;
		in_transaction_ = false ; // no longer in transaction
        if (stat_) { 
            const std::string rb = "rollback #" ;
            const std::string &dbKey = conn_.getKey() ;
            statN(dbKey)["tot rollback #"] += 1 ;
            statN(dbKey)["tot rollback #"]["duration [ms]"] += double(timer_.elapsed()) ; // PRQA S 3081
            statN(dbKey)[name_][rb] += 1 ;
            statN(dbKey)[name_][rb]["duration [ms]"] += double(timer_.elapsed()) ; // PRQA S 3081
            statN(dbKey)[name_][rb]["tr rollback time [ms]"] += double(rollbackTimer.elapsed()) ; // PRQA S 3081
        }
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "transaction \"" <<  name_ << "\" rolled back") ;
	}
}


_ACS_END_NESTED_NAMESPACE
