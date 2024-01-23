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
	Revision 5.7  2016/05/10 21:33:32  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 5.6  2016/04/11 13:36:22  marpas
	adopting new db interfaces
	
	Revision 5.5  2016/03/26 21:38:45  marpas
	using new db I/F
	
	Revision 5.4  2014/02/06 09:13:22  francesco.avanzi
	passed from dbConv() to db::toSQL()
	
	Revision 5.3  2014/02/03 13:45:52  francesco.avanzi
	coding best practices applied & new db::Transaction used
	
	Revision 5.2  2013/07/22 16:44:04  marpas
	compilation warnings removed
	
	Revision 5.1  2013/07/22 13:42:06  marpas
	ADC-193 implemented
	coding best practices enforced
	compilation and qa warnings fixed
	
	Revision 5.0  2013/06/19 18:07:26  marpas
	adopting libException 5.x standards
	types fixed
	using OrderCreator::getNUllSatId and OrderCreator::getNullSatName()
	
	Revision 2.19  2013/06/19 14:54:14  marfav
	Restored null satellite id management (use zero to identify null value)
	
	Revision 2.18  2013/06/18 15:24:01  marfav
	Null satellite management fixed
	
	Revision 2.17  2013/05/14 18:50:16  marpas
	no longer master/slave support through dbConnection
	
	Revision 2.16  2013/04/12 12:18:00  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 2.15  2013/04/08 09:39:06  marpas
	fixing bug on waiting time computation
	
	Revision 2.14  2013/02/26 18:20:55  marpas
	dbSet new I/F adopted
	
	Revision 2.13  2013/02/12 13:46:53  marpas
	dbConnectionPool concept introduced
	libDBOrderCreator new interfaces
	coding best practices applied
	debug improved
	qa rules
	FileGuard used when needed instead of File::unlink mess around
	
	Revision 2.12  2012/03/07 13:50:54  marpas
	preparing order parameters file
	
	Revision 2.11  2010/07/09 16:12:05  marpas
	useless messages removed
	
	Revision 2.10  2010/03/26 13:24:19  marpas
	messaging slightly changed: no msg in max waiting time default
	
	Revision 2.9  2010/03/25 14:07:55  marpas
	Allowing to search for TemporaryOrdersMaxWaitingTime in namespace:
	<OrderGeneration><"ordertype"><"Sat"><"NoMerge|Level1|Level2"><"Processor"> if not found:
	<OrderGeneration><"ordertype"><"Sat"><"NoMerge|Level1|Level2"> will be used as before (default)
	
	Revision 2.8  2010/03/08 13:43:35  enrcar
	EC:: Oracle DB handled
	
	Revision 2.7  2009/07/01 16:26:01  marpas
	order mission is set for MERGE and NOMERGE
	
	Revision 2.6  2009/05/07 09:49:06  marpas
	TemporaryOrder::isExpired method is now deprecated. Call isInventoryExpired instead. this is due to libOrderCreator Rev_2_20 to solve the VEN-1198
	
	Revision 2.5  2008/11/27 14:02:16  marpas
	venus implementation ongoing
	other changes for convenience method(s)
	
	Revision 2.4  2008/11/24 16:42:52  marpas
	triggering files are now inserted into t_ordtriggeringfiles
	
	Revision 2.3  2008/10/07 12:43:47  crivig
	porting to postgres
	
	Revision 2.2  2006/06/22 14:37:13  marpas
	temporary version, aligned with new libDateTime I/F
	
	Revision 2.1  2006/03/03 10:06:33  marpas
	exception catching fixed
	
	Revision 2.0  2006/02/28 10:24:17  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2005/10/06 15:28:44  marfav
	Import libDefaultOrderCreator
	
	
*/


#include <MergeOrderCreator.h>
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


#include <sstream>


_ACS_BEGIN_NAMESPACE(acs)
	
using namespace std ;



ACS_CLASS_DEFINE_DEBUG_LEVEL(MergeOrderCreator) 




MergeOrderCreator::MergeOrderCreator(const string &type, const string &algo, unsigned int satId, dbConnection &) :
	OrderCreator(type,algo,satId)
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
        ACS_LOG_DEBUG(SimpleDebugSignature << "Ctor called") ;
	ACS_CLASS_END_DEBUG
}

MergeOrderCreator::~MergeOrderCreator() throw() 
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
        ACS_LOG_DEBUG(SimpleDebugSignature << "Dtor called") ;
	ACS_CLASS_END_DEBUG
}


void MergeOrderCreator::fillOrder(dbOrder &order, const vector<TemporaryOrder> &tl, size_t first , size_t last) const // PRQA S 4214
{
	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB) 
        ACS_LOG_DEBUG(SimpleDebugSignature << "Method called") ;
	ACS_CLASS_END_DEBUG

    // t_orderdQueue description
    //
    //   orderid                 SERIAL not null,
    //   creationdate            DATETIME YEAR TO FRACTION not null,
    //   priority                SMALLINT not null,
    //   status                  NCHAR(32) not null,
    //   processorname           NCHAR(32) not null,
    //   processorversion        NCHAR(8) not null,
    //   processorconfiguration  NCHAR(8),
    //   troubleshooting         boolean not null,
    //   starttime               DATETIME YEAR TO FRACTION not null,
    //   stoptime                DATETIME YEAR TO FRACTION not null,
    //   jobresponsible          NCHAR(16),
    //   productionstarttime     DATETIME YEAR TO FRACTION,
    //   productionstoptime      DATETIME YEAR TO FRACTION,
    //   processingtimewindow    SMALLINT not null,
    //   processorid             NCHAR(16),
    //   ordertype               NCHAR(8) not null,
    //   externalorderid         NCHAR(32)

	DateTime dt ; if (getCreationTime().isFilled()) { dt = getCreationTime() ; }
	order.setCreationDate(dt) ;
	order.setPriority(tl[first].priority()) ;
	order.setStatus("ELIGIBLE") ;
	order.setProcessorName(tl[first].processorName()) ; 
	order.setProcessorVersion(tl[first].processorVersion()) ;
	order.setProcessorConfiguration(tl[first].processorConfiguration()) ;
	order.setTroubleshooting(tl[first].troubleShooting()) ;
	order.setStartTime(tl[first].start()) ;
	order.setStopTime(tl[last].stop()) ;
	order.setProcessingTimeWindow(tl[first].processingTimeWindow()) ;
	order.setOrderType(tl[first].orderType()) ;
	// set the satellite id inside the order if not null
	if (tl[first].satelliteId() != 0) {
		order.setSatelliteId(tl[first].satelliteId()) ;
    }
	int mission = 0;
	bool missionNull = false ;
	mission = tl[first].mission(&missionNull) ;
	if (!missionNull) {
		order.setMission(mission) ;
    }
	
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
	    ostringstream dbg ;
		dbg << "temporary order index " << first << " " << last << "\n\n" ;
		dbg << "MergeOrderCreator::createOrder dump \n" ;
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
			bool set = false  ;
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


string MergeOrderCreator::createParamFile(const vector<TemporaryOrder> &tl, 
										const size_t& first, 
										const size_t& last, 
										dbConnection &theConnection) const 
{
	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB) 
        ACS_LOG_DEBUG(SimpleDebugSignature << "Method called.") ;
	ACS_CLASS_END_DEBUG

	return "" ; // Empty filename
}


void MergeOrderCreator::createOrder(const vector<TemporaryOrder> &tl, size_t first , size_t last, dbConnection &theConnection) const // PRQA S 4020, 4214
{
	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB) 
        ACS_LOG_DEBUG(SimpleDebugSignature << "Method called") ;
	ACS_CLASS_END_DEBUG

	dbOrder *order= dynamic_cast<dbOrder*>(dbPersistentFactoryChain::instance()->newObject("dbOrder", theConnection.getKey())); // PRQA S 3081, 4412
	if(!order) {
		ACS_THROW(TemporaryOrder::OrderException("Cannot allocate a dbOrder")) ; // PRQA S 3081
	}
	

	dbPersistentScopeGuard theOrderScopeGuard(order) ;
    fillOrder(*order, tl, first, last) ;
 
 	// FIXME: ENRCAR:: THE INPUT INTERFACE IS STILL IN PROGRESS -- IT MIGHT CHANGE !
	const string path ( createParamFile(tl, first, last, theConnection) ) ;
 
 	FileGuard fg(path) ;
	if (path.empty()) { fg.detach() ; }	/* Disable the automatic guard */
 
	db::Transaction transaction(theConnection) ;
	transaction.begin_work("") ; 

	
	ACS_LOG_INFO("Emitting order " << order->getOrderType() << " for processor " << order->getProcessorName() << " " << order->getProcessorVersion() << " conf " << order->getProcessorConfiguration()) ;
	ostringstream ostart ;
    ostringstream ostop ;
	order->getStartTime().utcs(ostart) ;
	order->getStopTime().utcs(ostop) ;

	if (false == path.empty()) { order->setParametersUploadFileName(path) ; }
	
	ACS_LOG_INFO("               " << "Processing window: " << ostart.str() << " " << ostop.str()) ;

	try {
	
		if (!simulation()) {
			// save the order (remember is inside a transaction) 
			order->save(theConnection) ;
			for(size_t i =first; i <=last; i++) {
				insertTriggeringFile(tl[i], order->getOrderId(),theConnection) ;
			}
		}
		else {
			ACS_LOG_WARNING("SIMUL MODE no order emitted") ;
		}
		
		if (!simulation()) {
			// remove the temporay orders
            const string &dbKey = theConnection.getKey() ;

			// Prepare the list of Ids
			vector <dbPersistent::IdType> idList;
			for (size_t i= first; i <= last; ++i) 
			{
				idList.push_back (tl[i].id());
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
		else {
			ACS_LOG_WARNING("SIMUL MODE no temporary order removed") ;
		}
		
		transaction.commit() ; 
		ACS_LOG_INFO("The order id: " << order->getOrderId() << " has just been succesfully emitted.") ;
		appendSummary(OrderIdSuccesfullyCreated,order->getOrderId()) ;
	}
	catch (...) {
		transaction.roll_back() ;
		throw ;
	}
	
}


void MergeOrderCreator::emitOrders(const vector<TemporaryOrder> &orders, dbConnection &theConnection ) const // PRQA S 4214
{
	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB) 
        ACS_LOG_DEBUG(SimpleDebugSignature << "Method called") ;
	ACS_CLASS_END_DEBUG

	if(not orders.empty()) {

        size_t osize = orders.size() ;
        // NB current in incremented at the very loop end
		for (size_t current=0; current < osize ;) { // PRQA S 4235

			// get the configuration values
			double mergeDistance = getMaxMergeDistance("") ; // PRQA S 4412 2
			double maxTime = getMaxWaitingTime("") ;
			ostringstream tmsg ;
			try {
				maxTime = getMaxWaitingTime("",orders[current].processorName()) ; // PRQA S 4400
				tmsg << algo() << ": Using waiting time for processor \"" << orders[current].processorName() << "\" " << maxTime << " [sec]" ;
			}
			catch (exception &x) {
				ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
					ACS_LOG_NOTIFY_EX(x) ;
					ACS_LOG_DEBUG(SimpleDebugSignature << x 
					    << "This exception has been ignored: using default") ;
				ACS_CLASS_END_DEBUG
				tmsg << algo() << ": Using default waiting time for processor \"" << orders[current].processorName() << "\" " << maxTime << " [sec]" ;
			}

			// convert times into days
			mergeDistance /= 86400 ; // PRQA S 4400 2
			maxTime /= 86400 ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, orders[current].fileType() << " mergeDistance is "<< mergeDistance << "[day]" 
				 << " maxTime is "<< maxTime << "[day]") ;

			// try to find a chain
			size_t index = orders[current].orderChain(orders,current,mergeDistance) ;
			size_t firstChain = current ;
			// orderChain method returns max_size if no chain
			size_t lastChain = index ; 

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "firstChain is "<< firstChain 
				    << " lastChain is "<< lastChain 
                    << " orders[firstChain] was inventoried on: " << orders[firstChain].inventoryDate().YYYYMMDDThhmmss()) ;
                
			// if the beginning of a chain is too old (more than maxTime since its inventory)
			// the whole chain can be emitted.
			if (orders[firstChain].isInventoryExpired(maxTime)) {
				ACS_LOG_INFO(tmsg.str()) ;
				// emit the order for the chain (any length)
				ostringstream msg ;
				if (firstChain < lastChain) {
					ostringstream os ;
					for (size_t oid=firstChain; oid <= lastChain; oid++) {
						os <<  orders[oid].id() << ", " ;
                    }
					msg << "The temporary order id(s) " << os.str() << " will be merged to create an order." ;
				}
				else {
					msg << "The temporary order id " << orders[firstChain].id()<< " will create an order." ;
                }
				ACS_LOG_INFO(msg.str()) ;

				createOrder(orders,firstChain,lastChain, theConnection) ;

			}
			else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "orders[firstChain] has not yet expired: no order emitted for the chain") ;
			}

			// update current to the element after the just evaluated chain
			current = lastChain + 1 ; // PRQA S 3084
		}
	}
}


void MergeOrderCreator::createOrders(dbConnectionPool &pool,ConfigurationSingleton::StopPredicate &stop_pred) const  // PRQA S 4020, 4214
{
	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB) 
        ACS_LOG_DEBUG(SimpleDebugSignature << "Method called") ;
	ACS_CLASS_END_DEBUG

    dbConnectionWrapper pw(pool) ;
    dbConnection &theConnection = pw ; // PRQA S 3050
    const string &dbKey = theConnection.getKey() ;

	// find unique file types temporary orders
	string UniqueFileTypesCommand = TemporaryOrder::getUniqueEntriesSqlCommand(dbKey, type(), algo(), satelliteId()) ; // OKKIO
	SQLString sqlString(dbKey, SQLString::COMMAND) ;
	sqlString.forceSQLString(UniqueFileTypesCommand) ;
	dbQuery fTypeQuery(theConnection,sqlString, "SelUniqTmpOrders") ;
	dbSet fTypeSet;
	fTypeQuery.execForSet(fTypeSet) ;
	vector<TemporaryOrder::UniqueEntry> uniqueEntries ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "unique file types in temporary orders are " << fTypeSet.size()) ;
    size_t tcount = fTypeSet.size() ;
	for (size_t i=0; i < tcount; ++i) {
		dbRow row(dbKey) ;
		fTypeSet.getNextRow(row) ;
		TemporaryOrder::UniqueEntry entry ;
		entry.loadRow(row) ;
		if (find(uniqueEntries.begin(),uniqueEntries.end(), entry) == uniqueEntries.end()) {

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "\t[" << i << "] " << entry.filetype << " "  << entry.processorName  << " "  << entry.processorVersion << " "  << entry.processorConfiguration) ;
			uniqueEntries.push_back(entry) ;
		}
	}

	// group by different filetype because 
	// orders can be joined if they belongs to the same filetype
    size_t uesize = uniqueEntries.size() ;
	for (size_t tmpOrder = 0; tmpOrder < uesize; ++ tmpOrder) {

        if (stop_pred()) { return ; }
		string sqlCommand = TemporaryOrder::getSqlCommandForFType(dbKey,type(), algo(), uniqueEntries[tmpOrder]) ;
		SQLString sqlStringUe(dbKey, SQLString::COMMAND) ;
		sqlStringUe.forceSQLString(sqlCommand) ;
		dbQuery getQuery(theConnection,sqlStringUe, "SelFtypeTmpOrders") ;
		dbSet set;
		getQuery.execForSet(set) ;

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "createOrders found "<< set.size() << " object for " << type() << " " << algo() << " " << uniqueEntries[tmpOrder].processorName) ;

		vector<TemporaryOrder> orders ;
        
        size_t rcount1 = set.size() ;
		for(size_t r=0; r<rcount1; ++r)
		{
			TemporaryOrder order ;
			dbRow row(dbKey) ;
			set.getNextRow(row);
			order.loadRow(row) ;

			if(order.validityFlag()) {

				orders.push_back(order) ;
			} else {	

				ACS_LOG_INFO("The temporary order id " << order.id() << " cannot be processed because it refers to file \"" << order.fileName() << "\" currently invalid.") ;
				appendSummary(TemporaryOrderIdHasProblem,order.id()) ;
			}
		}	
		emitOrders(orders, theConnection) ;
	} // for ftype 
}

_ACS_END_NAMESPACE

