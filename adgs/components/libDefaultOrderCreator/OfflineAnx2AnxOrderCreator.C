// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Default Order Creation Library $

	$Id$

	$Author$

	$Log$
	Revision 5.8  2016/07/22 14:23:28  marfav
	CSGACQ-86
	SQL code review for Oracle 11
	
	Revision 5.7  2016/05/10 21:33:32  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 5.6  2016/04/11 13:36:22  marpas
	adopting new db interfaces
	
	Revision 5.5  2016/03/26 21:38:45  marpas
	using new db I/F
	
	Revision 5.4  2014/02/06 09:08:16  francesco.avanzi
	passed from dbConv() to db::toSQL()
	
	Revision 5.3  2014/02/03 13:45:52  francesco.avanzi
	coding best practices applied & new db::Transaction used
	
	Revision 5.2  2013/07/22 16:44:04  marpas
	compilation warnings removed
	
	Revision 5.1  2013/07/22 13:42:06  marpas
	ADC-193 implemented
	coding best practices enforced
	compilation and qa warnings fixed
	
	Revision 5.0  2013/06/19 18:07:27  marpas
	adopting libException 5.x standards
	types fixed
	using OrderCreator::getNUllSatId and OrderCreator::getNullSatName()
	
	Revision 2.16  2013/06/19 14:54:15  marfav
	Restored null satellite id management (use zero to identify null value)
	
	Revision 2.15  2013/06/18 15:24:01  marfav
	Null satellite management fixed
	
	Revision 2.14  2013/05/14 18:50:17  marpas
	no longer master/slave support through dbConnection
	
	Revision 2.13  2013/04/12 12:18:00  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 2.12  2013/02/26 18:20:56  marpas
	dbSet new I/F adopted
	
	Revision 2.11  2013/02/12 13:46:53  marpas
	dbConnectionPool concept introduced
	libDBOrderCreator new interfaces
	coding best practices applied
	debug improved
	qa rules
	FileGuard used when needed instead of File::unlink mess around
	
	Revision 2.10  2012/03/07 13:50:54  marpas
	preparing order parameters file
	
	Revision 2.9  2010/03/08 13:43:36  enrcar
	EC:: Oracle DB handled
	
	Revision 2.8  2009/05/07 09:49:06  marpas
	TemporaryOrder::isExpired method is now deprecated. Call isInventoryExpired instead. this is due to libOrderCreator Rev_2_20 to solve the VEN-1198
	
	Revision 2.7  2008/11/27 14:02:17  marpas
	venus implementation ongoing
	other changes for convenience method(s)
	
	Revision 2.6  2008/11/24 15:58:36  marpas
	triggering files work in progress
	
	Revision 2.5  2008/10/07 12:43:47  crivig
	porting to postgres
	
	Revision 2.4  2008/09/26 14:29:11  marpas
	GOCE patch applied to HEAD
	
	Revision 2.3.2.1  2008/01/28 18:17:17  giucas
	bug fixed: the newest must be expired in order to ensure that potentially,
	a temporary order overlapping more than a single orbit could be deleted
	before every order it overlaps can be emitted.
	
	Revision 2.3  2007/05/18 10:27:47  marpas
	robustness of list creation improved, a temporary order giving any error does not stop other anx2anx creation
	
	Revision 2.2  2006/06/23 10:01:00  ivafam
	StringUtils::compareNoCase replaced with StringUtils::equalsNoCase
	
	Revision 2.1  2006/06/20 16:46:37  ivafam
	Method compareNoCAse replaced with StringUtils::equalsNoCase
	
	Revision 2.0  2006/02/28 10:24:18  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2005/10/06 15:28:45  marfav
	Import libDefaultOrderCreator
	
	
*/


#include <OfflineAnx2AnxOrderCreator.h>
#include <dbRow.h>
#include <dbQuery.h>
#include <dbSet.h>
#include <DateTime.h>
#include <TemporaryOrder.h>
#include <dbInventoryObject.h>
#include <dbPersistentFactoryChain.h>
#include <dbPersistentScopeGuard.h>
#include <dbQueryFactoryChain.h>
#include <dbOrder.h>
#include <dbConv.h>
#include <ConfigurationSingleton.h>
#include <Transaction.h>
#include <dbConnectionPool.h>
#include <Filterables.h>
#include <StringUtils.h>

#include <sstream>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;


void OfflineAnx2AnxOrderCreator::A2AOrder::getOrbits(const TemporaryOrder &t, OrbitMode omode, OrderCreator &oc, vector<long int> & orbits)
{
#warning for the moment orbits are consecutive integers for all satellites
	pair<long, long> range = oc.absOrbitFromTime(t.start(), t.stop()) ;
	orbits.clear() ;
	
	switch (omode) {
	case First: { orbits.push_back(range.first) ; break ; }
	case Last:  { orbits.push_back(range.second) ; break ; }
	case Multiple:
        {
		    for (long i=range.first; i <= range.second; ++i) {
			    orbits.push_back(i) ;
		    }
            break ;
        }
	case MultipleButLast:
        {
		    for (long i=range.first; i <= range.second; ++i) {
			    orbits.push_back(i) ;
		    }
		    if (orbits.size() > 1) { // remove last orbit if there are more than just one
			    orbits.erase(orbits.begin()+orbits.size()-1) ; // PRQA S 3000
		    }
            break ;
        }
	}
	
}


OfflineAnx2AnxOrderCreator::A2AOrder::A2AOrder(const TemporaryOrder &t, long orbit, OrderCreator &oc) :
	_absOrbit(orbit),
	_inventoryDate(t.inventoryDate()),
	_startAnx(oc.getAnxFromOrbit(_absOrbit)),
	_stopAnx(oc.getAnxFromOrbit(_absOrbit+1)),
	_orders()
{
	_orders.push_back(t) ;
}

OfflineAnx2AnxOrderCreator::A2AOrder::A2AOrder(const A2AOrder &o) :
	_absOrbit(o._absOrbit),
	_inventoryDate(o._inventoryDate),
	_startAnx(o._startAnx),
	_stopAnx(o._stopAnx),
	_orders(o._orders)
{
    // empty 
}

OfflineAnx2AnxOrderCreator::A2AOrder & OfflineAnx2AnxOrderCreator::A2AOrder::operator=(const A2AOrder &o)
{
	if (&o != this) {
		_absOrbit = o._absOrbit ;
		_inventoryDate = o._inventoryDate ;
		_startAnx = o._startAnx ;
		_stopAnx = o._stopAnx ;
		_orders = o._orders ;
	}
	
	return *this ;
}

OfflineAnx2AnxOrderCreator::A2AOrder::~A2AOrder() throw() {}


void OfflineAnx2AnxOrderCreator::A2AOrder::add(const A2AOrder &o) 
{
	if (o._absOrbit != _absOrbit) {
		ostringstream os ;
		os << "Absolute orbit mismatch (" << _absOrbit << " != " << o._absOrbit << ") " ;
		ACS_THROW(TemporaryOrder::OrderException(os.str())) ; // PRQA S 3081
	}
	
    size_t osize = o._orders.size() ;
	for (size_t i=0; i < osize ; ++i) {
		size_t pos = 0 ;
		if (!TemporaryOrder::findById(_orders,o._orders[i].id(), pos)) {
			_orders.push_back(o._orders[i]) ;
        }
	}	
}


bool OfflineAnx2AnxOrderCreator::A2AOrder::isExpired(double maxTime) const // PRQA S 4020
{	
	// the newest must be expired in order to ensure that potentially, a temporary order overlapping more than a single orbit
	// could be deleted before every order it overlaps can be emitted.
	for (size_t i=0; i < _orders.size(); i++) {
		if (!_orders[i].isInventoryExpired(maxTime)) {
			return false ;
        }
	}
	
	return true ;
}

long OfflineAnx2AnxOrderCreator::A2AOrder::absOrbit() const // PRQA S 4120
{
	return _absOrbit ;
}

const vector<TemporaryOrder> OfflineAnx2AnxOrderCreator::A2AOrder::orders() const  // PRQA S 4120
{
	return _orders ;
}

const DateTime &OfflineAnx2AnxOrderCreator::A2AOrder::startAnx() const  // PRQA S 4120
{
	return _startAnx ;
}

const DateTime &OfflineAnx2AnxOrderCreator::A2AOrder::stopAnx() const  // PRQA S 4120
{
	return _stopAnx ;
}
			

const vector<TemporaryOrder>& OfflineAnx2AnxOrderCreator::A2AOrder::getTmpOrders() const  // PRQA S 4120
{
	return _orders ;
}


////////////////////////////////////////////
//
//
// class OfflineAnx2AnxOrderCreator
//
//
////////////////////////////////////////////









ACS_CLASS_DEFINE_DEBUG_LEVEL(OfflineAnx2AnxOrderCreator) 




OfflineAnx2AnxOrderCreator::OfflineAnx2AnxOrderCreator(const string &type, const string &algo, unsigned int satId, dbConnection &) :
	OrderCreator(type,algo, satId)
{
}

OfflineAnx2AnxOrderCreator::~OfflineAnx2AnxOrderCreator() throw() {}


bool OfflineAnx2AnxOrderCreator::find(const A2AOrder &o, const std::vector<A2AOrder> &vo, size_t &pos)  // PRQA S 4020
{
	for (size_t i=0; i < vo.size(); i++) {
		if (( vo[i].absOrbit() == o.absOrbit() ) && 
            ( vo[i].orders()[0].sameProcessor(o.orders()[0])) ) {
			pos = i ;
			return true ;
		}
	}
	
	return false ;
}


void OfflineAnx2AnxOrderCreator::fillOrder(dbOrder &order, const A2AOrder &L2O) const //PRQA S 4214
{
	
	// t_orderdQueue description
	//
// // 	orderid 				SERIAL not null,
// // 	creationdate			DATETIME YEAR TO FRACTION not null,
// // 	priority 				SMALLINT not null,
// // 	status 					NCHAR(32) not null,
// // 	processorname 			NCHAR(32) not null,
// // 	processorversion		NCHAR(8) not null,
// // 	processorconfiguration	NCHAR(8),
// // 	troubleshooting			boolean not null,
// // 	starttime 				DATETIME YEAR TO FRACTION not null,
// // 	stoptime 				DATETIME YEAR TO FRACTION not null,
// // 	jobresponsible 			NCHAR(16),
// // 	productionstarttime 	DATETIME YEAR TO FRACTION,
// // 	productionstoptime 		DATETIME YEAR TO FRACTION,
// // 	processingtimewindow 	SMALLINT not null,
// // 	processorid 			NCHAR(16),
// // 	ordertype 				NCHAR(8) not null,
// // 	externalorderid 		NCHAR(32)
// //   satelliteid 			SMALLINT 

	DateTime	dt ;
	order.setCreationDate(dt) ;
	order.setPriority(L2O.orders()[0].priority()) ;
	order.setStatus("ELIGIBLE") ;
	order.setProcessorName(L2O.orders()[0].processorName()) ; 
	order.setProcessorVersion(L2O.orders()[0].processorVersion()) ;
	order.setProcessorConfiguration(L2O.orders()[0].processorConfiguration()) ;
	order.setTroubleshooting(L2O.orders()[0].troubleShooting()) ;
	order.setStartTime(L2O.startAnx()) ;
	order.setStopTime(L2O.stopAnx()) ;
	order.setProcessingTimeWindow(L2O.orders()[0].processingTimeWindow()) ;
	order.setOrderType(L2O.orders()[0].orderType()) ;
	// set the satellite id inside the order if not null
	if (satelliteId() != 0 ) {
		order.setSatelliteId(satelliteId()) ;
    }
	
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
	    ostringstream dbg ;
		dbg << "dump \n" ;
		ostringstream os ;
		order.getCreationDate().utcs(os) ;
		dbg << "CreationDate " << os.str() << "\n" ;
		dbg << "Priority " << order.getPriority() << "\n" ;
		dbg << "Status " << order.getStatus() << "\n" ;
		dbg << "Troubleshooting " << order.getTroubleshooting() << "\n" ;
		ostringstream os1 ;
		order.getStartTime().utcs(os1) ;
		dbg << "StartTime " <<  os1.str() << "\n" ;
		ostringstream os2 ;
		order.getStopTime().utcs(os2) ;
		dbg << "StopTime " <<  os2.str() << "\n" ;
		dbg << "ProcessingTimeWindow " << order.getProcessingTimeWindow() << "\n" ;
		dbg << "ProcessorName " << order.getProcessorName() << "\n" ; 
		dbg << "ProcessorVersion " << order.getProcessorVersion() << "\n" ;
		dbg << "ProcessorConfiguration " << order.getProcessorConfiguration() << "\n" ;
		dbg << "OrderType " << order.getOrderType() << "\n" ;
		ostringstream os3 ;
		{
			bool set = false ;
			order.getSatelliteId(&set) ;
			if (set) {
				os3 << order.getSatelliteId() ;
			}
            else {	
				os3 << "<null>" ;
            }
		}
		dbg << "SatelliteId " << os3.str() ;
        ACS_LOG_DEBUG(SimpleDebugSignature << dbg.str()) ;
	ACS_CLASS_END_DEBUG
}



void OfflineAnx2AnxOrderCreator::createOrder(const A2AOrder &L2O, dbConnection &theConnection) const // PRQA S 4020, 4214
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	
	dbOrder *order= dynamic_cast<dbOrder*>(dbPersistentFactoryChain::instance()->newObject("dbOrder", theConnection.getKey()));// PRQA S 3081, 4412
	if(!order) {
		ACS_THROW(TemporaryOrder::OrderException("Cannot allocate a dbOrder")) ; // PRQA S 3081
	}
	dbPersistentScopeGuard theOrderScopeGuard(order) ;
	fillOrder(*order, L2O) ;
	
	db::Transaction transaction(theConnection) ;
	transaction.begin_work("") ; 
	
	ACS_LOG_INFO("Emitting order " << order->getOrderType() << " for processor " << order->getProcessorName() << " " << order->getProcessorVersion() << " conf " << order->getProcessorConfiguration()) ;
	ostringstream ostart ; 
    ostringstream ostop ;
	order->getStartTime().utcs(ostart) ;
	order->getStopTime().utcs(ostop) ;
	ACS_LOG_INFO("               " << "Processing window: " << ostart.str() << " " << ostop.str()) ;

	try {
	
		if (!simulation()) {
			// save the order (remember is inside a transaction) 
			order->save(theConnection) ;
			insertTriggeringFiles(L2O.getTmpOrders(), order->getOrderId(),theConnection) ;
		}
		
		if (!simulation()) {

			// remove the temporay orders
            const string &dbKey = theConnection.getKey() ;

			// Prepare the list of Ids
			vector <dbPersistent::IdType> idList;
			for (size_t i= 0; i < L2O.orders().size(); ++i) 
			{
				idList.push_back (L2O.orders()[i].id());
			}

			SQLString sqlString(dbKey, SQLString::DELETE) ;
			string table = "t_temporaryOrders" ;
            sqlString.addTableList( table ) ;
			{
                dbQueryParameters pp(dbKey) ; 
				pp.addParameter(dbParam(dbKey,table,"id", dbParam::IN, idList)) ; // PRQA S 3050, 3081
                sqlString.addANDWhereParameter(pp) ;
    		}

			dbQuery deleteQuery(theConnection,sqlString, "DeleteTmpOrder") ;
			deleteQuery.execForStatus() ;
		}
		
		transaction.commit() ; 
		ACS_LOG_INFO("The order id: " << order->getOrderId() << " has just been succesfully emitted.") ;
		appendSummary(OrderIdSuccesfullyCreated,order->getOrderId()) ;
	}
	catch (exception &) {
		transaction.roll_back() ;
		for (size_t i=0; i < L2O.orders().size(); i++) {
			appendSummary(TemporaryOrderIdHasProblem,L2O.orders()[i].id()) ;
		}
		throw ;
	}
}

void OfflineAnx2AnxOrderCreator::fillL2O(const vector<TemporaryOrder> &tmporders, vector <A2AOrder> &L2Orders) const // PRQA S 4214
{
	// now we have a whole set of valid temporary orders, we need to check their orbit.
    size_t tosize = tmporders.size() ;
	for (size_t order=0; order < tosize; ++order) {
		try {
			vector<long> orbits ;
			A2AOrder::getOrbits(tmporders[order], orbitMode(tmporders[order]), *(const_cast<OfflineAnx2AnxOrderCreator *>(this)), orbits) ; // PRQA S 3081
			size_t orbsize = orbits.size() ;
            for (size_t currOrb = 0; currOrb < orbsize; ++currOrb) {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"createOrders orbit "<< orbits[currOrb] 
                        << " object for " << type() << " " << algo() << " " << tmporders[order].processorName()) ;

				A2AOrder corder(tmporders[order], orbits[currOrb], *(const_cast<OfflineAnx2AnxOrderCreator *>(this))) ; // PRQA S 3081
				size_t pos = 0 ;
				if (find(corder, L2Orders, pos)) {
					L2Orders[pos].add(corder) ;
				}
				else {
					L2Orders.push_back(corder) ;
                }
			}
		}
		catch(exception &e) {
			ACS_LOG_WARNING("Exception got: " << e.what() << " on temporary order id: " << tmporders[order].id()
			    << " proceeding with Anx2Anx order processing ... ") ;
		}
	}
}	

void OfflineAnx2AnxOrderCreator::createOrders(dbConnectionPool &pool,ConfigurationSingleton::StopPredicate &stop_pred) const  // PRQA S 4020
{
    dbConnectionWrapper pw(pool) ;
    dbConnection &theConnection = pw ; // PRQA S 3050
    const string &dbKey = theConnection.getKey() ;
	// find unique file types temporary orders
	string UniqueFileTypesCommand = TemporaryOrder::getUniqueEntriesSqlCommand(dbKey, type(), algo(), satelliteId()) ;
	SQLString sqlStringUe(dbKey, SQLString::COMMAND) ;
	sqlStringUe.forceSQLString(UniqueFileTypesCommand) ;
	dbQuery fTypeQuery(theConnection,sqlStringUe, "SelUniqTmpOrders") ;
	dbSet fTypeSet;
	fTypeQuery.execForSet(fTypeSet) ;
	vector<TemporaryOrder::UniqueEntry> uniqueEntries ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"unique file types in temporary order are " << fTypeSet.size()) ;
    size_t rcount = fTypeSet.size() ;
	for (size_t i=0; i < rcount; ++i) {
		dbRow row(dbKey) ;
		fTypeSet.getNextRow(row) ;
		TemporaryOrder::UniqueEntry entry ;
		entry.loadRow(row) ;
		if (std::find(uniqueEntries.begin(),uniqueEntries.end(), entry) == uniqueEntries.end()) {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"\t[" << i << "] " << entry.filetype << " "  << entry.processorName  << " "  << entry.processorVersion << " "  << entry.processorConfiguration) ;
			uniqueEntries.push_back(entry) ;
		}
	}

	vector<TemporaryOrder> tmporders ;

	// get every needed temporary order
    size_t uesize = uniqueEntries.size() ;
	for (size_t tmpOrder = 0; tmpOrder < uesize; ++tmpOrder) {

        if (stop_pred()) { return ; }

		string sqlCommand = TemporaryOrder::getSqlCommand(theConnection.getKey(), type(), algo(), uniqueEntries[tmpOrder].satelliteId) ;
		SQLString sqlString(dbKey, SQLString::COMMAND) ;
		sqlString.forceSQLString(sqlCommand) ;
		dbQuery getQuery(theConnection,sqlString, "SelectTmpOrders") ;
		dbSet set;
		getQuery.execForSet(set) ;

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"createOrders found "<< set.size() << " object for " << type() << " " << algo() << " " << uniqueEntries[tmpOrder].processorName) ;

        size_t rcount2 = set.size() ;
		for(size_t r=0; r<rcount2 ; ++r)
		{
			TemporaryOrder tmporder ;
			dbRow row(dbKey) ;
			set.getNextRow(row);
			tmporder.loadRow(row) ;

			if(tmporder.validityFlag()) {

				tmporders.push_back(tmporder) ;
			} else {	

				ACS_LOG_WARNING("The temporary order id " << tmporder.id() << " cannot be processed because it refers to file \"" << tmporder.fileName() << "\" currently invalid.") ;
			}
		}	
	    // for this unique entry (Processor {3} + Sat + Order Type)
	    // now we have a whole set of valid temporary orders, we need to check their orbit.
	    vector <A2AOrder> L2Orders ;
        fillL2O(tmporders, L2Orders) ;	

        size_t l2size = L2Orders.size() ;
	    for (size_t l2o=0 ; l2o < l2size; ++l2o) {
            if (stop_pred()) { return ; }
		    // get the configuration values
		    double maxTime = getMaxWaitingTime("", L2Orders[l2o].orders()[0].fileType()) ; // PRQA S 4412
		    maxTime /= 86400 ;  // PRQA S 4400
		    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,L2Orders[l2o].orders()[0].fileType() << " maxTime is "<< maxTime << "[day]") ;
		    if (L2Orders[l2o].isExpired(maxTime)) {
			    createOrder(L2Orders[l2o], theConnection) ;
            }
	    }
	
	}
}


OfflineAnx2AnxOrderCreator::OrbitMode OfflineAnx2AnxOrderCreator::orbitMode(const TemporaryOrder &t) const // PRQA S 4214
{
	string A2AOrbMode = getOrbitMode("", t.fileType()) ;

	OrbitMode omode = First ;
	if (StringUtils::equalsNoCase(A2AOrbMode,"First")) {
		omode = First ;
	}
	else if (StringUtils::equalsNoCase(A2AOrbMode,"Last")) {
		omode = Last ;
	}
	else if (StringUtils::equalsNoCase(A2AOrbMode,"Multiple")) {
		omode = Multiple ;
	}
	else if (StringUtils::equalsNoCase(A2AOrbMode,"MultipleButLast")) {
		omode = MultipleButLast ;
	}
	else {
		ACS_LOG_WARNING("Unrecognized orbit mode " << "\"" << A2AOrbMode << "\"" 
			<< " Default has been set to " << "First") ;

		omode = First ;
	}
	
	return omode ;	
}


_ACS_END_NAMESPACE
