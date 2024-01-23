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

	$Author$ $

	$Log$
	Revision 5.4  2016/04/11 13:36:22  marpas
	adopting new db interfaces
	
	Revision 5.3  2016/03/26 21:38:45  marpas
	using new db I/F
	
	Revision 5.2  2014/02/03 13:45:52  francesco.avanzi
	coding best practices applied & new db::Transaction used
	
	Revision 5.1  2013/07/22 16:44:04  marpas
	compilation warnings removed
	
	Revision 5.0  2013/06/19 18:07:27  marpas
	adopting libException 5.x standards
	types fixed
	using OrderCreator::getNUllSatId and OrderCreator::getNullSatName()
	
	Revision 2.14  2013/06/19 14:54:14  marfav
	Restored null satellite id management (use zero to identify null value)
	
	Revision 2.13  2013/05/14 18:50:17  marpas
	no longer master/slave support through dbConnection
	
	Revision 2.12  2013/04/12 12:18:00  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 2.11  2013/04/09 08:39:55  marpas
	NameIdAssociator refactoring
	
	Revision 2.10  2013/03/12 17:54:38  marpas
	dbConnectionPool is mandatory in file actions
	
	Revision 2.9  2013/02/26 18:20:55  marpas
	dbSet new I/F adopted
	
	Revision 2.8  2013/02/12 13:46:53  marpas
	dbConnectionPool concept introduced
	libDBOrderCreator new interfaces
	coding best practices applied
	debug improved
	qa rules
	FileGuard used when needed instead of File::unlink mess around
	
	Revision 2.7  2012/03/07 13:50:54  marpas
	preparing order parameters file
	
	Revision 2.6  2010/07/23 10:53:23  marpas
	Formatt management added.
	In case no "DistributionFormat" is found on the configuration, the "on demand distribution" records for each destination (if any) will have the null format.
	
	Revision 2.5  2009/06/22 16:54:03  marpas
	priority got from NRT file, in case is missing from temporaryorder
	
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
	
	Revision 1.1  2005/11/28 20:18:02  marpas
	NRTExternalOrderCreator class added - supports Envisat for EXTERNAL algorithm
	
	
	
*/

#include <NRTExternalOrderCreator.h>
#include <TemporaryOrder.h>
#include <dbQuery.h>
#include <dbSet.h>
#include <dbInventoryObject.h>
#include <dbPersistentFactoryChain.h>
#include <ConfigurationSingleton.h>
#include <dbDownloadFile.h>
#include <dbOrder.h>
#include <dbOnDemandDistribution.h>
#include <Transaction.h>
#include <dbConnectionPool.h>
#include <dbPersistentScopeGuard.h>
#include <SatelliteName.h>
#include <Filterables.h>

#include <dbConv.h>
#include <FileGuard.h>
#include <XMLIstream.h>
#include <dbFileActionFactoryChain.h>
#include <dbFileActionScopeGuard.h>
#include <unistd.h>



_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(NRTExternalOrderCreator) 

NRTExternalOrderCreator::NRTExternalOrderCreator(const string &type, const string &algo, unsigned int satId, dbConnection &) :
	OrderCreator(type,algo, satId) ,
    _goodOrders()
{
}

NRTExternalOrderCreator::~NRTExternalOrderCreator() throw() {}

void NRTExternalOrderCreator::createOrders(dbConnectionPool &pool,ConfigurationSingleton::StopPredicate &stop_pred) const  // PRQA S 4020 
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
	for(size_t r=0; r< rcount; ++r)
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
			dbInventoryObject *theObj = 0;
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

void NRTExternalOrderCreator::downloadAndEmitOrder(  // PRQA S 4020, 4214
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
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Downloading:" << obj->getFileName() << " as:" << tmpName) ;
	    dbDownloadFile* dloadObj = dynamic_cast<dbDownloadFile*>(dbFileActionFactoryChain::instance()->newDownloadAction(pool,*obj,tmpName)); // PRQA S 3081

	    if(!dloadObj)
	    {
            ACS_THROW(exIllegalValueException("dbDownloadFile dynamic_cast failure.")) ; // PRQA S 3081
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


	    exception *te = 0 ;
	    dloadObj->runException(te) ;

	    if (te) {
		    TemporaryOrder::OrderException ex(*te,"Cannot download " + obj->getFileName() + " version " + obj->getFileVersion() + " as " + tmpName) ;
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


	

void NRTExternalOrderCreator::emitOrder(const rsResourceSet &nrtOrder, const TemporaryOrder &tmpOrder, dbConnection &theConnection) const  // PRQA S 4020, 4214
{	
	// parse and emit orders
	// how many NRT orders are in the request ?
	string requestsKey = "Data_Block.List_of_Requests.Request" ;
	size_t numOrders = nrtOrder.getArraySize(requestsKey) ;
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "temporary NRT file "
            << "contains " << numOrders << " request(s).") ;

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

			DateTime dt ;
			theOrder->setCreationDate(dt) ;
			int priority = tmpOrder.priority() ;
			try {
				nrtOrder.getValue(basekey.str() + ".Priority", priority) ;
			}
			catch(rsResourceSet::NotFoundException &) {
				ACS_LOG_WARNING("Request # " << i << " of " << numOrders << " has no Priority: setting it to: " << priority) ;
			}
			
			theOrder->setPriority(priority) ;
			theOrder->setStatus("ELIGIBLE") ;

			string orderType ;
			nrtOrder.getValue(basekey.str() + ".OrderType", orderType) ;
			theOrder->setOrderType(orderType) ;

			string processorName ;
			nrtOrder.getValue(basekey.str()+".Processing.Processor", processorName) ;
			theOrder->setProcessorName(processorName) ; 

			string processorVersion ;
			nrtOrder.getValue(basekey.str()+".Processing.Version", processorVersion) ;
			theOrder->setProcessorVersion(processorVersion) ;

			string processorConfiguration ;
			nrtOrder.getValue(basekey.str()+".Processing.Config", processorConfiguration) ;
			theOrder->setProcessorConfiguration(processorConfiguration) ;

			theOrder->setTroubleshooting(tmpOrder.troubleShooting()) ;

			string Start ;
			nrtOrder.getValue(basekey.str()+".Processing.Start", Start) ;
			if (Start.length() < 4) {   // PRQA S 4400
				ACS_THROW(TemporaryOrder::OrderException("Start time string len is shorter than 4")) ; // PRQA S 3081
			}
			theOrder->setStartTime(Start.substr(4)) ;   // PRQA S 4400
			string Stop ;
			nrtOrder.getValue(basekey.str()+".Processing.Stop", Stop) ;
			if (Stop.length() < 4) {   // PRQA S 4400
				ACS_THROW(TemporaryOrder::OrderException("Stop time string len is shorter than 4"))  ; // PRQA S 3081
			}
			theOrder->setStopTime(Stop.substr(4)) ;   // PRQA S 4400

			theOrder->setProcessingTimeWindow(tmpOrder.processingTimeWindow()) ;

			string externalId ;
			nrtOrder.getValue(basekey.str()+".ID", externalId) ;
			theOrder->setExternalOrderId(externalId) ;

			ACS_LOG_INFO("Emitting order " << theOrder->getOrderType() << " for processor " << theOrder->getProcessorName() << " " << theOrder->getProcessorVersion() << " conf " << theOrder->getProcessorConfiguration()) ;
			ostringstream ostart ;
            ostringstream ostop ;
			theOrder->getStartTime().utcs(ostart) ;
			theOrder->getStopTime().utcs(ostop) ;
			ACS_LOG_INFO("               " << "Processing window: " << ostart.str() << " " << ostop.str()) ;

			string satellite ;
			nrtOrder.getValue(basekey.str()+".SatelliteId", satellite) ;

			try {
				SatelliteName* sat = SatelliteName::instance(getConnKeyFromApp());
				theOrder->setSatelliteId(sat->id(satellite,true)) ; // PRQA S 3000
			}
			catch(exception &x) {
				
				ACS_LOG_WARNING("No satellite: \"" << satellite << "\" it will be left NULL because: " << x) ;
			}


			int mission = 0 ;
			nrtOrder.getValue(basekey.str()+".MissionNum", mission) ;
			theOrder->setMission(mission) ;
#warning Request.sensor is left unused for the moment

			if (!simulation()) {
				// save the order (remember is inside a transaction) 
				theOrder->save(theConnection) ;
				insertTriggeringFile(tmpOrder, theOrder->getOrderId(), theConnection) ;
				appendGoodOrder(theOrder->getOrderId()) ;
			}
			else {
				ACS_LOG_WARNING("SIMUL MODE no order emitted") ;
			}
			
			bool formatSet = false ;
			string format ;
			ostringstream fmtMsgPortion ;
			try {
				format = getDistributionFormat("") ;	
				formatSet = true ;
				fmtMsgPortion << "with format " << format ;
			}
			catch(rsResourceSet::NotFoundException &) {
				// nothing to do 
				fmtMsgPortion << "with no explicit format" ;
			}		

            ostringstream msg ;
			msg << "Result of the order id:" << theOrder->getOrderId() << " will be sent " ;
			msg << fmtMsgPortion.str() ;
			vector<string> theUsers ;
			try {
				// get the number of destination
				string destinationsKey= basekey.str() + ".Distribution.List_of_Destinations.Destination" ;
				nrtOrder.getArray(destinationsKey, theUsers) ;
				msg << " to the following " << theUsers.size() << " users: \n" ;
			}
			catch(exception &) {
				msg << " using default distribution because no explicit destination is set.\n" ;
			}

            size_t usize = theUsers.size() ;
			for (unsigned int j=0; j < usize; ++j) {
				msg << "     [" << j << "]: " << theUsers[j] << "\n" ;
				// read each order
				dbOnDemandDistribution *theDistribution= dynamic_cast<dbOnDemandDistribution*>(dbPersistentFactoryChain::instance()->newObject("dbOnDemandDistribution", theConnection.getKey())); // PRQA S 3081, 4412
				if (!theDistribution) {
					ACS_THROW(TemporaryOrder::OrderException("Cannot allocate a dbOnDemandDistribution"))  ; // PRQA S 3081
				}
				dbPersistentScopeGuard theDistributionScopeGuard(theDistribution) ;
				theDistribution->setOrderId(theOrder->getOrderId()) ; // PRQA S 3000, 3010 
				theDistribution->setUserId(theUsers[j]) ; 
				if (formatSet) {
					theDistribution->setFormat(format) ;
                }
				else {
					theDistribution->unsetFormat() ;
                }
				if (!simulation()) {
					theDistribution->save(theConnection) ;
				}
				else {
					ACS_LOG_WARNING("SIMUL MODE no distribution set") ;
				}
			}
			ACS_LOG_INFO( msg.str() ) ; // users msg

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

void NRTExternalOrderCreator::appendGoodOrder(dbPersistent::IdType id) const
{
	_goodOrders.push_back(id) ;
}


void NRTExternalOrderCreator::goodOrdersSummary() const
{
    size_t osize = _goodOrders.size() ;
	for (size_t i=0; i < osize; ++i) {
		appendSummary(OrderIdSuccesfullyCreated,_goodOrders[i]) ;
    }
		
	_goodOrders.clear() ;
}

_ACS_END_NAMESPACE
