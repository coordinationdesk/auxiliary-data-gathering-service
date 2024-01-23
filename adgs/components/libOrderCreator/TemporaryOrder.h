// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Exprivia SpA.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA.

	$Prod: A.C.S. libOrderCreator $

	$Id$

	$Author$

	$Log$
	Revision 5.3  2017/10/19 17:41:52  marpas
	dtor shall not throw
	
	Revision 5.2  2013/07/22 13:40:18  marpas
	ADC-193 implemented
	coding besta practices enforced
	compilation and qa warnings fixed
	
	Revision 5.1  2013/06/19 18:05:58  marpas
	fixing types
	
	Revision 5.0  2013/06/19 13:54:02  marpas
	satelliteid is now unsigned int - to be coherent with its NameIdAssociator
	OrderCreator has 2 static methods to deal with null satellite.
	libException 5.x standards adopted
	qa rules
	
	Revision 2.13  2013/04/12 12:17:44  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 2.12  2013/03/07 19:42:40  marpas
	DUP inventory management as in jira ADC-122
	
	Revision 2.11  2013/02/12 12:43:50  marpas
	dbConnectionPool enforced
	interface rationalization
	coding best practices applied
	qa rules
	
	Revision 2.10  2013/02/11 12:50:20  marpas
	coding best practices
	dbConnectionPool concept enforced
	qa rules
	new interfaces
	
	Revision 2.9  2012/09/17 17:07:49  marpas
	new method to get conf values,
	new class TriggeringFile with triggering filename, fileversion and orderid - 0 if was null
	updated XMLOrderParameters class, writes also the generating orderid for the triggering file(s)
	update TemporaryOrder with new queries and a method to get the generating orderid for the triggering file
	
	Revision 2.8  2012/09/17 10:17:23  marpas
	triggering file orderid read from temporary order
	
	Revision 2.7  2012/03/07 13:09:24  marpas
	refactoring
	introducing XMLOrderParameters class and its unitary test
	
	Revision 2.6  2009/07/01 16:18:31  marpas
	mission management on triggering file added
	
	Revision 2.5  2009/05/12 10:10:20  marpas
	sites() const method added
	
	Revision 2.4  2009/05/07 09:46:43  marpas
	new method added: isInventoryExpired, isTmpOrderExpired
	older method isExpired deprecated.
	This is in preparation to solve VEN-1198
	
	Revision 2.3  2009/04/10 13:22:02  marpas
	OrderCreator emits an exception if no orbit in GOCE
	some rules fixed
	
	Revision 2.2  2009/01/28 12:53:07  marpas
	test mode enabled
	
	Revision 2.1  2008/11/25 15:01:15  marpas
	wrong const attribute to integral value removed
	
	Revision 2.0  2006/02/28 10:23:56  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/10/06 15:18:37  marfav
	Header tags fixed
	

*/



#ifndef _TemporaryOrder_H_
#define _TemporaryOrder_H_ 

#include <acs_c++config.hpp>
#include <dbPersistent.h>
#include <dbException.h>
#include <DateTime.h>

_ACS_BEGIN_NAMESPACE(acs)


class dbRow ;
class dbConnection ;



class TemporaryOrder { // PRQA S 2109
public:
	/*! class OrderException declaration */
	exDECLARE_EXCEPTION(OrderException,dbException) ; // PRQA S 2131, 2153, 2502

	
public :
	struct UniqueEntry { // PRQA S 2173, 2175
		UniqueEntry() ;
		UniqueEntry(const UniqueEntry &) ;
	    ~UniqueEntry() throw() ;
		UniqueEntry &operator=(const UniqueEntry &) ;
		void loadRow(const dbRow &) ;
		std::string filetype ; // PRQA S 2100 5
		std::string processorName ;
		std::string processorVersion ;
		std::string processorConfiguration ;
		unsigned int satelliteId ;
	} ;
    
    
public :

	TemporaryOrder() ;
	virtual ~TemporaryOrder() throw() ;
	TemporaryOrder(const TemporaryOrder&);
	TemporaryOrder & operator=(const TemporaryOrder&);

	bool isJoinable(const TemporaryOrder&, double dtime) const;
	TemporaryOrder join(const TemporaryOrder&) ;
	bool isInventoryExpired(double) const ;
	bool isTmpOrderExpired(double) const ;
	bool isDup() const ;
	size_t orderChain(const std::vector<TemporaryOrder> &, size_t , double ) const ;

	std::string dump(const std::string& t="") const ;
	uint16_t loadRow(const dbRow &) ;
	static std::string getSqlCommandOrderBy(const std::string &db_key, const std::string &orderType, const std::string &algo, unsigned int satId, const std::string &orderby) ;
	static std::string getSqlCommand(const std::string &db_key, const std::string &orderType, const std::string &algo, unsigned int satId) ;
	static std::string getSqlCommandForFType(const std::string &db_key, const std::string &orderType, const std::string &algo, const UniqueEntry &/*const std::string &ftype*/) ;
	static std::string getUniqueEntriesSqlCommand(const std::string &db_key, const std::string &orderType, const std::string &algo, unsigned int satId) ;

	static bool findById(const std::vector <TemporaryOrder> &, dbPersistent::IdType id, size_t &pos) ;


	void removeFromDb(dbConnection &) const ;
	bool sameProcessor(const TemporaryOrder &) const ;


	dbPersistent::IdType id() const throw() ;
	const std::string &fileName() const throw() ;
	const std::string &fileVersion() const throw() ;
	const std::string &algorithm() const throw() ;
	const std::string &fileType() const throw() ;
	const std::string &orderType() const throw() ;
	const std::string &processorName() const throw() ;
	const std::string &processorVersion() const throw() ;
	const std::string &processorConfiguration() const throw() ;
	bool validityFlag() const throw() ;
	const DateTime &start() const throw() ;
	const DateTime &stop() const throw() ;
	const DateTime &inventoryDate() const throw() ;
	const DateTime &equatorxtime() const throw() ;
	int priority() const throw() ;
	bool troubleShooting() const throw() ;
	int processingTimeWindow() const throw() ;
	unsigned int ordersat(bool *isNull = 0) const ; // returns 0 for null
	// synonymous for the above ordersat
	unsigned int satelliteId(bool *isNull = 0) const ; // returns 0 for null
	int mission(bool *isNull = 0) const ; // returns 0 for null
    int trigOrderId(bool *isNull = 0) const ; // returns 0 for null
	const DateTime &timestamp() const throw() ;
	const std::vector<int> & sites(const std::string &dbKey) const ;
    const std::string &qualityInfo() const throw() ;
	int qualified() const throw() ;
	const DateTime &timeDrivenStart(bool *isNull = 0) const ;
	const DateTime &timeDrivenStop(bool *isNull = 0) const ;
#ifdef TEST_ORDER_CREATOR
public:
#else
protected:
#endif
	bool isExpired(double) const ;
	
	/**
	 * \brief Returns true if the db schema has fields 'time_driven_start' and 'time_driven_stop'
	 */
	static bool hasTimeDrivenFields();	
	
	/**
	 * \brief Checks if db schema has fields 'time_driven_start' and 'time_driven_stop'
	 */
	static void initHasTimeDrivenFields(const std::string& dbKey);

	dbPersistent::IdType _id ;
	std::string _fileName ;
	std::string _fileVersion ;
	std::string _algorithm ;
	std::string _fileType ;
	std::string _orderType ;
	std::string _processorName ;
	std::string _processorVersion ;
	std::string _processorConfiguration ;
	bool _validityFlag ;
	DateTime _start ;
	DateTime _stop ;
	DateTime _inventoryDate ;
	DateTime _equatorxtime ;
	int _priority ;
	bool _troubleShooting ;
	int _processingTimeWindow ;
	unsigned int _ordersat ;
	bool _ordersatNull ;
	int _mission ;
	bool _missionNull ;
	DateTime _tstamp ;
	mutable std::vector<int> _sites ;
	int _orderId ;
	bool _orderIdNull ;
    std::string _qualityInfo ;
    int _qualified ;

	DateTime _timeDrivenStart ;
	bool _timeDrivenStartNull ;
	DateTime _timeDrivenStop ;
	bool _timeDrivenStopNull ;

	static Mutex _tdmutex;	
	static bool _hasTimeDrivenFields;
	static bool _isTimeDrivenInit;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(TemporaryOrder) ;
} ;

bool operator==(const TemporaryOrder::UniqueEntry &, const TemporaryOrder::UniqueEntry &) ;

struct TriggeringFile { // PRQA S 2173, 2175
    explicit TriggeringFile(const TemporaryOrder &) ;
    explicit TriggeringFile(const std::string &fn, const std::string &fv, const std::string &ft, dbPersistent::IdType toid) : 
        filename(fn), fileversion(fv), filetype(ft), orderid(toid) {} // PRQA S 3000
    TriggeringFile() : filename(), fileversion(), filetype(), orderid() {} 
    TriggeringFile(const TriggeringFile &r) :
        filename(r.filename), fileversion(r.fileversion), filetype(r.filetype), orderid(r.orderid) {}
    ~TriggeringFile() throw() {}
    TriggeringFile &operator = (const TriggeringFile &r) {
        if (this != &r) {
            filename = r.filename ;
            fileversion = r.fileversion ;
            filetype = r.filetype ;
            orderid = r.orderid ;
        }
        return *this ;
    }
    std::string filename ;  // PRQA S 2100 3
    std::string fileversion ;
    std::string filetype ;
    dbPersistent::IdType orderid ;
} ;

_ACS_END_NAMESPACE


#endif /* _TemporaryOrder_H_ */
