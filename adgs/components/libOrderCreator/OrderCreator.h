// PRQA S 1050 EOF
/*

	Copyright 1995-2014, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Order Creator library $

	$Id$

	$Author$

	$Log$
	Revision 5.6  2017/10/19 11:23:31  marpas
	vector assignement out of the loop
	
	Revision 5.5  2017/10/18 09:04:46  davide.tiriticco
	getMaxPendingTime, isQualifiedAllowed, isQualifiedDiscarded and loadDiscardedQualified implemented
	
	Revision 5.4  2014/05/15 18:09:27  marpas
	data encapsulation enforced
	
	Revision 5.3  2014/03/27 12:50:09  marpas
	reflect changes in orbit propagation libraries family
	warnings fixed
	
	Revision 5.2  2014/02/12 14:32:49  francesco.avanzi
	better code practices applied
	
	Revision 5.1  2013/07/22 13:40:18  marpas
	ADC-193 implemented
	coding besta practices enforced
	compilation and qa warnings fixed
	
	Revision 5.0  2013/06/19 13:54:02  marpas
	satelliteid is now unsigned int - to be coherent with its NameIdAssociator
	OrderCreator has 2 static methods to deal with null satellite.
	libException 5.x standards adopted
	qa rules
	
	Revision 2.19  2013/04/09 08:26:00  marpas
	NameIdAssociator refactoring
	
	Revision 2.18  2013/02/12 12:43:50  marpas
	dbConnectionPool enforced
	interface rationalization
	coding best practices applied
	qa rules
	
	Revision 2.17  2013/02/11 13:05:51  marpas
	fixing types
	
	Revision 2.16  2013/02/11 12:50:19  marpas
	coding best practices
	dbConnectionPool concept enforced
	qa rules
	new interfaces
	
	Revision 2.15  2012/10/25 13:28:22  marpas
	getConf overloaded for bool too
	
	Revision 2.14  2012/09/17 17:07:49  marpas
	new method to get conf values,
	new class TriggeringFile with triggering filename, fileversion and orderid - 0 if was null
	updated XMLOrderParameters class, writes also the generating orderid for the triggering file(s)
	update TemporaryOrder with new queries and a method to get the generating orderid for the triggering file
	
	Revision 2.13  2012/03/07 13:32:18  marpas
	parameters file management improved and i/f changed
	
	Revision 2.12  2011/07/12 10:17:56  marpas
	Capitalization for sat names are done until the first non-alpha char, then it is stopped.
	The "first-non-alpha" char is the one for which !isalpha() is true.
	So: crYoSaT becomes Cryosat and sentineL-2A becomes Sentinel-2A.
	this applies in converting sat names only.
	
	Revision 2.11  2010/07/09 09:15:18  marpas
	tests added
	
	Revision 2.10  2010/06/23 09:45:08  marpas
	Null satellite management for keys
	
	Revision 2.9  2009/04/10 13:22:02  marpas
	OrderCreator emits an exception if no orbit in GOCE
	some rules fixed
	
	Revision 2.8  2009/01/26 14:24:36  marpas
	getConf extended for integers
	
	Revision 2.7  2008/12/01 13:33:43  marpas
	getAlgoKeyString method added
	
	Revision 2.6  2008/11/27 14:01:29  marpas
	venus implementation ongoing
	
	Revision 2.5  2008/11/25 17:19:13  marpas
	work in progress
	
	Revision 2.4  2008/11/24 17:18:29  marpas
	getProcessorName added
	
	Revision 2.3  2008/10/29 17:21:34  danalt
	getOrderLength added
	
	Revision 2.2  2007/07/19 12:42:52  marpas
	GOCE work in progress
	
	Revision 2.1  2006/06/20 15:37:22  ivafam
	Method compareNoCase replaced with StringUtils::compareNoCase
	
	Revision 2.0  2006/02/28 10:23:56  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2005/10/06 15:09:06  marfav
	Import libOrderCreator
	
		

*/



#ifndef _OrderCreator_H_
#define _OrderCreator_H_ 

#include <acs_c++config.hpp>
#include <TemporaryOrder.h>
#include <ConfigurationSingleton.h>
#include <dbPersistent.h>
#ifdef NRT_TAI_UTC_CONVERSION
	#include <DateTime.h>
#endif			
#ifdef GOCE
	#include <AnxInfo.h>
#endif
#include <exMacros.h>
#include <string>
#include <map>
#include <vector>
#include <set>


_ACS_BEGIN_NAMESPACE(acs)


class DateTime ;
class dbConnection ;
class dbConnectionPool ;

class OrderCreator // PRQA S 2109
{		
public:


	enum SummaryType {
						OrderIdSuccesfullyCreated,
						TemporaryOrderIdHasProblem
					 } ;
public:
	OrderCreator(const std::string &type, const std::string &algo, unsigned int satId);
	virtual ~OrderCreator() throw() ;
	const std::string &type() const ;
	const std::string &algo() const ;
	unsigned int satelliteId(bool *isNull = 0) const ;
	std::string satelliteName() const ;

	virtual void createOrders(dbConnectionPool &,ConfigurationSingleton::StopPredicate &) const = 0 ;

	std::pair<long int, long int> absOrbitFromTime(const DateTime &dstart, const DateTime &dstop) ;
	DateTime getAnxFromOrbit(long int orbit) ;

	virtual bool simulation() const ;
	void overrideCreationTime(const DateTime& dt) { _creationTime = dt ; }
	DateTime getCreationTime() const { return _creationTime ; }

	static void emitSummary() ;
    static std::string getNullSatName() { return "NullSatellite" ; }
    static unsigned int getNullSatId() throw() { return 0 ; }
#ifdef TEST_ORDER_CREATOR
public:
#else
protected:
#endif
	void initOrbitPropagator(const DateTime &dstart, const DateTime &dstop) ;
#ifdef NRT_TAI_UTC_CONVERSION
	void initTimeConverter(const std::string &start, const std::string &stop) const ;
	DateTime tai2utc(const std::string &tai) const ;
#endif
    void loadAllowedQualified(const std::string &dbkey) ;
	void loadDiscardedQualified(const std::string &dbKey) ;
	std::string getGeneralConf(const std::string &subparam) const ;
	std::string getOrbitMode(const std::string &keyalt, const std::string &subparam="") const ; // PRQA S 4412
	std::string getDistributionFormat(const std::string &keyalt) const ;
	std::string getTempDownloadPath(const std::string &keyalt) const ;
	double getMaxMergeDistance(const std::string &keyalt, const std::string &subparam="") const ; // PRQA S 4412
	double getMaxWaitingTime(const std::string &keyalt, const std::string &subparam="") const ; // PRQA S 4412
	double getMaxPendingTime(const std::string &keyalt, const std::string &subparam="") const ; // PRQA S 4412
	double getTimeWindow(const std::string &keyalt, const std::string &subparam="") const ; // PRQA S 4412
    double getOrderLength(const std::string &keyalt, const std::string &subparam="") const ; // PRQA S 4412

	void getConf(const std::string &keyEnd, std::string &, bool &,const std::string &subparam) const ;
	void getConf(const std::string &keyEnd, std::string &, int &,const std::string &subparam) const ;
	void getConf(const std::string &keyEnd, std::string &, double &,const std::string &subparam) const ;
	void getConf(const std::string &keyEnd, std::string &, std::string &, const std::string &subparam) const ;
	virtual std::string buildKey(const std::string &keyEnd, const std::string &subparam="") const ;

    std::vector<TriggeringFile >
        getTriggeringFiles(const TemporaryOrder &) const ;
    std::vector<TriggeringFile >
        getTriggeringFiles(const std::vector<TemporaryOrder> &) const ;

	void insertTriggeringFile(const std::string &, const std::string &, dbPersistent::IdType orderid, dbConnection &) const ;
	void insertTriggeringFile(const TemporaryOrder &, dbPersistent::IdType orderid, dbConnection &) const ;
	void insertTriggeringFiles(const std::vector<TemporaryOrder> &, dbPersistent::IdType orderid, dbConnection &) const ;
    bool dupOrNotQualified(const TemporaryOrder &) const ;
    bool isQualifiedAllowed(const TemporaryOrder&) const;
    bool isQualifiedDiscarded(const TemporaryOrder&) const;

	static void appendSummary(SummaryType, dbPersistent::IdType) ;
	virtual std::string getAlgoKeyString() const ;
	virtual std::string getTypeName() const ;
	static const std::string &capitalize_to_firstnonalpha(std::string &s) ;
    std::string getConnKeyFromApp() const ;         
private:
	// declared but not implemented 
	OrderCreator();
	OrderCreator(const OrderCreator&);
	OrderCreator & operator=(const OrderCreator&);	

private:
	std::string _type ; 
	std::string _algo ;
	unsigned int _satelliteId ; // == 0  means null see getNullSatId & getNullSatName static methods
	DateTime _creationTime ; 
#ifdef GOCE
	AnxInfo _anxInfo ;
#else
	long double _actualOrbitStartValidity ;
	long double _actualOrbitStopValidity ;
#endif
	static std::map<SummaryType, std::vector<dbPersistent::IdType> > _summary ;
	static std::set<dbPersistent::IdType>	_tmpOrderIdWithProblems;

    std::vector<int> _allowedQualified ;
	std::vector<int> _discardedQualified;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(OrderCreator) ;
};

_ACS_END_NAMESPACE

#endif /* _OrderCreator_H_ */
