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
	Revision 5.7  2016/04/11 13:36:22  marpas
	adopting new db interfaces
	
	Revision 5.6  2016/03/26 21:38:45  marpas
	using new db I/F
	
	Revision 5.5  2016/03/02 10:19:44  marfav
	Forced a cast to string in dbcolum set call
	
	Revision 5.4  2016/03/01 17:36:44  nicvac
	OPMAN-155: dissemination id fixed
	
	Revision 5.3  2016/03/01 14:55:37  nicvac
	OPMAN-155: On demand distribution for the new db schema.
	
	Revision 5.2  2014/02/03 13:45:52  francesco.avanzi
	coding best practices applied & new db::Transaction used
	
	Revision 5.1  2013/07/22 16:44:04  marpas
	compilation warnings removed
	
	Revision 5.0  2013/06/19 18:07:27  marpas
	adopting libException 5.x standards
	types fixed
	using OrderCreator::getNUllSatId and OrderCreator::getNullSatName()
	
	Revision 2.13  2013/06/19 14:54:15  marfav
	Restored null satellite id management (use zero to identify null value)
	
	Revision 2.12  2013/06/18 15:24:01  marfav
	Null satellite management fixed
	
	Revision 2.11  2013/05/14 18:50:17  marpas
	no longer master/slave support through dbConnection
	
	Revision 2.10  2013/04/12 12:18:00  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 2.9  2013/03/12 17:54:38  marpas
	dbConnectionPool is mandatory in file actions
	
	Revision 2.8  2013/02/26 18:20:55  marpas
	dbSet new I/F adopted
	
	Revision 2.7  2013/02/12 13:46:53  marpas
	dbConnectionPool concept introduced
	libDBOrderCreator new interfaces
	coding best practices applied
	debug improved
	qa rules
	FileGuard used when needed instead of File::unlink mess around
	
	Revision 2.6  2012/03/07 13:50:54  marpas
	preparing order parameters file
	
	Revision 2.5  2010/07/23 10:53:23  marpas
	Formatt management added.
	In case no "DistributionFormat" is found on the configuration, the "on demand distribution" records for each destination (if any) will have the null format.
	
	Revision 2.4  2008/11/27 14:02:17  marpas
	venus implementation ongoing
	other changes for convenience method(s)
	
	Revision 2.3  2008/11/24 16:42:52  marpas
	triggering files are now inserted into t_ordtriggeringfiles
	
	Revision 2.2  2008/10/07 12:43:47  crivig
	porting to postgres
	
	Revision 2.1  2006/03/03 10:06:33  marpas
	exception catching fixed
	
	Revision 2.0  2006/02/28 10:24:18  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/11/28 20:18:02  marpas
	NRTExternalOrderCreator class added - supports Envisat for EXTERNAL algorithm
	
	Revision 1.1.1.1  2005/10/06 15:28:44  marfav
	Import libDefaultOrderCreator
	
	
*/

#include <NRTLevel1OrderCreator.h>
#include <TemporaryOrder.h>
#include <dbQuery.h>
#include <dbSet.h>
#include <dbInventoryObject.h>
#include <dbPersistentFactoryChain.h>
#include <dbSmartPersistent.h>
#include <ConfigurationSingleton.h>
#include <dbDownloadFile.h>
#include <dbOrder.h>
#include <dbOnDemandDistribution.h>
#include <dbConnectionPool.h>
#include <Transaction.h>
#include <dbPersistentScopeGuard.h>
#include <Filterables.h>

#include <dbConv.h>
#include <FileGuard.h>
#include <XMLIstream.h>
#include <dbFileActionFactoryChain.h>
#include <dbFileActionScopeGuard.h>
#include <unistd.h>




_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(NRTLevel1OrderCreator) 

NRTLevel1OrderCreator::NRTLevel1OrderCreator(const string &type, const string &algo, unsigned int satId, dbConnection &) :
	OrderCreator(type,algo, satId) ,
    _goodOrders()
{
}

NRTLevel1OrderCreator::~NRTLevel1OrderCreator() throw() {}

void NRTLevel1OrderCreator::createOrders(dbConnectionPool &pool,ConfigurationSingleton::StopPredicate &stop_pred) const  // PRQA S 4020
{
    dbConnectionWrapper pw(pool) ;
    dbConnection &theConnection = pw ; // PRQA S 3050
    const string &dbKey = theConnection.getKey() ;

	string sqlCommand = TemporaryOrder::getSqlCommand(dbKey, type(), algo(), satelliteId()) ;
	SQLString sqlString(dbKey, SQLString::COMMAND) ;
	sqlString.forceSQLString(sqlCommand) ;

	dbQuery getQuery(theConnection,sqlString, "SelTmpOrders") ;
	dbSet set;
	getQuery.execForSet(set) ;


	vector<TemporaryOrder> orders ;

    size_t rcount = set.size() ;
	for(size_t r=0; r<rcount; ++r)
	{
		TemporaryOrder order ;
		dbRow row(dbKey) ;
		set.getNextRow(row);
		order.loadRow(row) ;
		if (order.validityFlag()) {
			orders.push_back(order) ;
        }
	}
	
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "NRT orders are" << orders.size()) ;
    size_t osize = orders.size() ;
	for (size_t i=0; i < osize; ++i) {
		
        if (stop_pred()) { return ; }
		
		try {
			ACS_LOG_INFO("Parsing temporary order id: " << orders[i].id()) ;
			dbInventoryObject *theObj = 0 ;
			try {
				theObj = dynamic_cast<dbInventoryObject*>(dbPersistentFactoryChain::instance()->newObject("dbInventoryObject", theConnection.getKey())); // PRQA S 3081				
                if (!theObj) {
					ACS_THROW(TemporaryOrder::OrderException("Cannot allocate a dbInventoryObject")) ; // PRQA S 3081
				}
				dbPersistentScopeGuard theObjScopeGuard(theObj) ;
				theObj->setFileName(orders[i].fileName()) ;
				theObj->setFileVersion(orders[i].fileVersion()) ;
 				ACS_LOG_INFO("Downloading NRT file: \"" << orders[i].fileName() << " " << orders[i].fileVersion()) ;
				downloadAndEmitOrder(orders[i],theObj, theConnection, stop_pred, pool) ;
			}
			catch(exception &e) {
				ACS_THROW(TemporaryOrder::OrderException(e,"Cannot download NRT file " + orders[i].fileName() + " version " + orders[i].fileVersion())) ; // PRQA S 3081
			}			
		}
		catch(exception &e) { // notify now and continue with next if any.
			ACS_LOG_NOTIFY_EX(e) ; 
		}
	}
}

void NRTLevel1OrderCreator::downloadAndEmitOrder(  // PRQA S 4020, 4214
        const TemporaryOrder& tmpOrder, 
        dbInventoryObject *obj, 
        dbConnection &theConnection, 
        ConfigurationSingleton::StopPredicate &stop_pred,
        dbConnectionPool &pool) const
{	
    try {
	    obj->load(theConnection) ;

	    // now download the NRT (XML) file in a temporary path
	    string tmpPath = getTempDownloadPath("") ;

	    string tmpName = File::tempname(tmpPath+"/Nrt", "xml") ; // PRQA S 3050
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Downloading:" << obj->getFileName() << " as:" << tmpName ) ;
	    dbDownloadFile* dloadObj = dynamic_cast<dbDownloadFile*>(dbFileActionFactoryChain::instance()->newDownloadAction(pool, *obj,tmpName)); // PRQA S 3081

	    if(!dloadObj)
	    {
		    ACS_THROW(exIllegalValueException("dbDownloadFile dynamic_cast failure.")); // PRQA S 3081
	    }
	    dbFileActionScopeGuard guard(dloadObj);
	    FileGuard tmpNameGuard(	tmpName); // remove the file

	    dloadObj->start() ; // start the downloading thread
	    do {
            if (stop_pred()) {
                ACS_THROW(exThreadException("Interrupt due a external signal")) ; // PRQA S 3081
            }

		    Timer::delay(100) ; // PRQA S 4400
	    } while (dloadObj->downloading()) ;


	    exception *te = 0;
	    dloadObj->runException(te) ;

	    if (te) {
		    TemporaryOrder::OrderException ex(*te,"Cannot download " + obj->getFileName() + " version " + obj->getFileVersion() + " as " +tmpName) ;
		    delete te ;
		    ACS_THROW(ex) ;
	    }
	
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Parsing:" << tmpName) ;
	    rsResourceSet nrtOrder ;

		XMLIstream xml(tmpName) ;

		xml >> nrtOrder ;

		emitOrder(nrtOrder, tmpOrder, theConnection) ;
		goodOrdersSummary() ;
	}
	catch(...) {
		appendSummary(TemporaryOrderIdHasProblem,tmpOrder.id()) ;
		throw ;
	}

}


void NRTLevel1OrderCreator::fillOrder( // PRQA S 4214
    const rsResourceSet &nrtOrder, 
    const string &basekey,
    const TemporaryOrder &tmpOrder, 
    dbOrder &theOrder) const 
{
	DateTime dt ;
	theOrder.setCreationDate(dt) ;
	theOrder.setPriority(tmpOrder.priority()) ;
	theOrder.setStatus("ELIGIBLE") ;
	string processorName ;
	try {
		nrtOrder.getValue(basekey+".SIRAL_Processing.Processor", processorName) ;
	}
	catch(exception &e) {
		bool rethrow = false ;
		try {
			nrtOrder.getValue(basekey+".SIRAL_Processing.Mode", processorName) ;
			ACS_LOG_WARNING("processor name found under the <Mode> tag instead of <Processor> tag ") ;
		}
		catch(exception &x) {
			rethrow = true ; // re-throw main exception to avoid confusion
		}
		if (rethrow) {
			throw ;
        }
	}
	theOrder.setProcessorName(processorName) ; 

	string processorVersion ;
	nrtOrder.getValue(basekey+".SIRAL_Processing.Version", processorVersion) ;
	theOrder.setProcessorVersion(processorVersion) ;

	string processorConfiguration ;
	nrtOrder.getValue(basekey+".SIRAL_Processing.Config", processorConfiguration) ;
	theOrder.setProcessorConfiguration(processorConfiguration) ;

	theOrder.setTroubleshooting(tmpOrder.troubleShooting()) ;

	string Start ;
	nrtOrder.getValue(basekey+".SIRAL_Processing.Start", Start) ;
#ifdef NRT_TAI_UTC_CONVERSION
	theOrder.setStartTime(tai2utc(Start)) ;
#else
	if (Start.length() < 4) { // PRQA S 4400
		ACS_THROW(TemporaryOrder::OrderException("Start time string len is shorter than 4")) ; // PRQA S 3081
	}
	theOrder.setStartTime(Start.substr(4)) ; // PRQA S 4400
#endif
	string Stop ;
	nrtOrder.getValue(basekey+".SIRAL_Processing.Stop", Stop) ;
#ifdef NRT_TAI_UTC_CONVERSION
	theOrder.setStopTime(tai2utc(Stop)) ;
#else
	if (Stop.length() < 4) { // PRQA S 4400
		ACS_THROW(TemporaryOrder::OrderException("Stop time string lenis shorter than 4")) ; // PRQA S 3081
	}
	theOrder.setStopTime(Stop.substr(4)) ; // PRQA S 4400
#endif

	theOrder.setProcessingTimeWindow(tmpOrder.processingTimeWindow()) ;
	theOrder.setOrderType(tmpOrder.orderType()) ;

	string externalId ;
	nrtOrder.getValue(basekey+".ID", externalId) ;
	theOrder.setExternalOrderId(externalId) ;

}

void NRTLevel1OrderCreator::setOnDemandDist( // PRQA S 4214
    const rsResourceSet &nrtOrder, 
    const string &basekey,
    const dbOrder &theOrder, 
    dbConnection &theConnection
) const 
{
	string format ;
	ostringstream fmtMsgPortion ;
	try {
		format = getDistributionFormat("") ;	
		fmtMsgPortion << "with format " << format ;
	}
	catch(rsResourceSet::NotFoundException &) {
		// nothing to do 
		fmtMsgPortion << "with no explicit format" ;
	}		

	// get the number of destination
	vector<string> theUsers ;
	string destinationsKey= basekey + ".Distribution.List_of_Destinations.Destination" ;
	nrtOrder.getArray(destinationsKey, theUsers) ;
    ostringstream msg ;
	msg << "Result of the order id:" << theOrder.getOrderId() << " will be sent "
	    << fmtMsgPortion.str() << " to the following " << theUsers.size() << " users:\n" ;
	size_t usize = theUsers.size() ;
	for (size_t j=0; j < usize; ++j) {
		msg << "     [" << j << "]: " << theUsers[j] << "\n" ;
    }
    ACS_LOG_INFO(msg.str()) ;
	for (size_t j=0; j < usize; ++j) {
		// read each order

		dbSmartPersistent onDemandDistribution( "t_ondemanddistribution", theConnection );
		onDemandDistribution.set( "orderid", theOrder.getOrderId(), theConnection );
		onDemandDistribution.set( "userid", theUsers[j], theConnection );
		onDemandDistribution.set( "mediatype", string("FTP"), theConnection ); //If FTP is missing, the update will fail because referential integrity.
		// Format:
		// T_ondemanddistribution.format set to NULL => when the order's output will be put in the
		//   t_temporarydistribution, it will be applied the format (distribution task table's id)
		//   of the distribution rule that will be used (cf. stored procedure: p_completed_order_distribution, p_distribute_item_on_demand).
		int disseminationpackingalgo_id;
		{
			string key = theConnection.getKey();
			SQLString sql( key, SQLString::SELECT );
			sql.addSelectList("id_disseminationpackingalgo");
			sql.addTableList("t_disseminationpackingalgos");
			dbQueryParameters p(key) ;
			p.addParameter(dbParam(key, "t_disseminationpackingalgos", "factorykey" , dbParam::EQUAL_TO, db::toSQL(key,"OneItem")) ) ;
			sql.addANDWhereParameter(p) ;
			dbQuery query( theConnection, sql, "disseminationpackingalgo_id_retrieval" );
			try{
				dbRow row(key); 
                dbSet set ;
		        query.execForSet(set) ;
                ACS_COND_THROW(set.empty(), dbQuery::QueryNoDataFound("empty set")) ; // PRQA S 3081 2
                ACS_COND_THROW(set.size() > 1, exIllegalValueException("Set size > 1")) ;
                set.getNextRow(row) ; // first and only 

				row.column( 0, disseminationpackingalgo_id );
			} catch ( exception& ex ) {
				ACS_THROW( TemporaryOrder::OrderException(ex, string("Error getting dissemination algo id. Sql was: ")+sql.getSQL()) );
			}
		}
		onDemandDistribution.set( "disseminationpackingalgo_id", disseminationpackingalgo_id, theConnection );

		if (!simulation()) {
			onDemandDistribution.save( theConnection );
		} else {
			ACS_LOG_WARNING("SIMUL MODE no distribution set") ;
		}

	}
}

void NRTLevel1OrderCreator::emitOrder(const rsResourceSet &nrtOrder, const TemporaryOrder &tmpOrder, dbConnection &theConnection) const  // PRQA S 4020, 4214
{	
	// parse and emit orders
	// how many NRT orders are in the request ?
	string requestsKey = "Data_Block.List_of_NRT_Requests.NRT_Request" ;
	size_t numOrders = nrtOrder.getArraySize(requestsKey) ;
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "temporary NRT file "
            << "contains " << numOrders << " request(s).") ;
#ifdef NRT_TAI_UTC_CONVERSION
	// time converter library initialization
	string valPeriodKey = "Earth_Explorer_Header.Fixed_Header.Validity_Period" ;
	string valPeriodStart ;
	nrtOrder.getValue(valPeriodKey + ".Validity_Start" , valPeriodStart) ;
	string valPeriodStop ;
	nrtOrder.getValue(valPeriodKey + ".Validity_Stop", valPeriodStop) ;
	
	initTimeConverter(valPeriodStart,valPeriodStop) ;
#endif	

	// Start a transaction
	db::Transaction transaction(theConnection) ;
	transaction.begin_work("") ; 
		
	try {
		for (size_t i=0; i < numOrders; i++) {
			ostringstream basekey ;
			basekey << requestsKey << "[" << i << "]" ;
			// read each order
			dbOrder *theOrder= dynamic_cast<dbOrder*>(dbPersistentFactoryChain::instance()->newObject("dbOrder", theConnection.getKey())); // PRQA S 3081, 4412
			if (!theOrder) {
				ACS_THROW(TemporaryOrder::OrderException("Cannot allocate a dbOrder")) ; // PRQA S 3081
			}
			dbPersistentScopeGuard theOrderScopeGuard(theOrder) ;

            fillOrder(nrtOrder, basekey.str(), tmpOrder, *theOrder) ;

			ACS_LOG_INFO("Emitting order " << theOrder->getOrderType() << " for processor " << theOrder->getProcessorName() << " " << theOrder->getProcessorVersion() << " conf " << theOrder->getProcessorConfiguration()) ;
			ostringstream ostart ;
            ostringstream ostop ;
			theOrder->getStartTime().utcs(ostart) ;
			theOrder->getStopTime().utcs(ostop) ;
			ACS_LOG_INFO("               " << "Processing window: " << ostart.str() << " " << ostop.str()) ;

			// set the satellite id inside the order if not null
			if (satelliteId() != 0) {
				theOrder->setSatelliteId(satelliteId()) ;
            }

			if (!simulation()) {
				// save the order (remember is inside a transaction) 
				theOrder->save(theConnection) ;
				insertTriggeringFile(tmpOrder, theOrder->getOrderId(), theConnection) ;
				appendGoodOrder(theOrder->getOrderId()) ;
			}
			else {
				ACS_LOG_WARNING("SIMUL MODE no order emitted") ;
			}

            setOnDemandDist(nrtOrder, basekey.str(), *theOrder, theConnection) ;
		}

		if (!simulation()) {
			// remove the temporay order
			tmpOrder.removeFromDb(theConnection) ;
		}
		else {
			ACS_LOG_WARNING("SIMUL MODE The temporary order is not removed") ;
		}
		transaction.commit() ; 
	}
	catch (...) {
		transaction.roll_back() ;
		throw ;
	}

}

void NRTLevel1OrderCreator::appendGoodOrder(dbPersistent::IdType id) const
{
	_goodOrders.push_back(id) ;
}


void NRTLevel1OrderCreator::goodOrdersSummary() const
{
    size_t osize = _goodOrders.size() ;
	for (size_t i=0; i < osize; ++i) {
		appendSummary(OrderIdSuccesfullyCreated,_goodOrders[i]) ;
    }
		
	_goodOrders.clear() ;
}

_ACS_END_NAMESPACE

