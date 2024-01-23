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

#ifndef _Transaction_H_
#define _Transaction_H_

#include <acs_c++config.hpp>
#include <exMacros.h>
#include <Timer.h>
#include <string>

_ACS_BEGIN_NAMESPACE(acs)

class dbConnection ;

_ACS_BEGIN_NAMESPACE(math)
class StatNode ;
_ACS_END_NAMESPACE 

_ACS_BEGIN_NAMESPACE(db)


/*! \addtogroup db 
 *  @{
 */

/*! 
 * \class Transaction 
 * \brief Provides an auto rollback transaction mechanism
 */
class Transaction // PRQA S 2109
{
public:
	/**
	 * just create the class 
     *\param conn the dbConnection for the transaction
     *\param doStat allow statistics emission
 	 */
	explicit Transaction(dbConnection &conn, bool doStat=true) ; 	
    					
	/**
	 * create the class and start the transaction
     *\param conn the dbConnection for the transaction
     *\param name the name of the transaction useful for statistics and census
     *\param doStat allow statistics emission
     *\note the transaction is also started 
 	 */
	explicit Transaction(dbConnection &conn, const std::string &name, bool doStat=true) ; 						

	/**
	 * Does the following:
	 * \li calls rollback if not commited()
	 */
	~Transaction() ACS_NOEXCEPT; 										

	/**
	 * Begin a transaction you must give the transaction name
     *\param name the name of the transaction useful for statistics and census
 	 */
	bool begin_work(const std::string &name) ;

	/**
	 * Commit the transaction.
 	 */
	void commit() ;

	/**
	 * Rollback the transaction.
 	 */
	void roll_back() ;
    bool inTransaction() const ACS_NOEXCEPT { return in_transaction_ ; }
    
private:
    static math::StatNode &statN(const std::string &key) ;
// declared but not implemented
	Transaction() ;
	Transaction(const Transaction &) ;
	Transaction &operator=(const Transaction &) ;
private:
	dbConnection &conn_ ;
	bool in_transaction_ ;
    Timer timer_ ;
    std::string name_ ;
    bool started_ ;
    bool stat_ ;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(Transaction) ;
} ;

/*! @}*/


_ACS_END_NESTED_NAMESPACE

#endif
