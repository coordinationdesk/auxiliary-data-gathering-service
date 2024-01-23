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
	
	Revision 2.14  2013/06/19 14:52:59  marfav
	Restored null satellite id management (use zero to identify null value)
	Aligned to libException V5
	
	Revision 2.13  2013/06/18 15:24:01  marfav
	Null satellite management fixed
	
	Revision 2.12  2013/05/14 18:50:16  marpas
	no longer master/slave support through dbConnection
	
	Revision 2.11  2013/04/12 12:18:00  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 2.10  2013/02/26 18:20:55  marpas
	dbSet new I/F adopted
	
	Revision 2.9  2013/02/12 13:46:52  marpas
	dbConnectionPool concept introduced
	libDBOrderCreator new interfaces
	coding best practices applied
	debug improved
	qa rules
	FileGuard used when needed instead of File::unlink mess around
	
	Revision 2.8  2012/03/07 13:50:54  marpas
	preparing order parameters file
	
	Revision 2.7  2010/03/08 13:43:35  enrcar
	EC:: Oracle DB handled
	
	Revision 2.6  2009/05/07 09:49:06  marpas
	TemporaryOrder::isExpired method is now deprecated. Call isInventoryExpired instead. this is due to libOrderCreator Rev_2_20 to solve the VEN-1198
	
	Revision 2.5  2008/11/27 14:02:16  marpas
	venus implementation ongoing
	other changes for convenience method(s)
	
	Revision 2.4  2008/11/24 16:42:52  marpas
	triggering files are now inserted into t_ordtriggeringfiles
	
	Revision 2.3  2008/10/07 12:43:47  crivig
	porting to postgres
	
	Revision 2.2  2006/04/05 15:08:43  giucas
	bug fix: time was passed as seconds whilst the interface was day
	debug improved
	
	Revision 2.1  2006/03/03 10:06:33  marpas
	exception catching fixed
	
	Revision 2.0  2006/02/28 10:24:17  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2005/10/06 15:28:44  marfav
	Import libDefaultOrderCreator
	
	
*/


#include <GroupedCollectionOrderCreator.h>
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
#include <dbConnectionPool.h>
#include <Transaction.h>
#include <Filterables.h>

#include <sstream>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(GroupedCollectionOrderCreator::Group) 


GroupedCollectionOrderCreator::Group::Group(const std::vector<std::string> &f, double wait, double timewindow, bool anyFTypeMatch) :
	_group(),
    _filetypes(f),
	_maxWaitingTime(wait),
	_timeWindow(timewindow),
	_anyFTypeMatch(anyFTypeMatch)
{
}

GroupedCollectionOrderCreator::Group::~Group() throw() {}

GroupedCollectionOrderCreator::Group::Group(const Group &rhs) :
    _group(rhs._group),
	_filetypes(rhs._filetypes),
	_maxWaitingTime(rhs._maxWaitingTime),
	_timeWindow(rhs._timeWindow),
	_anyFTypeMatch(rhs._anyFTypeMatch)
{
	// empty
}


GroupedCollectionOrderCreator::Group &GroupedCollectionOrderCreator::Group::operator=(const Group &rhs) 
{
	if (this != &rhs) {
        _group = rhs._group ;
		_filetypes = rhs._filetypes ;
		_maxWaitingTime = rhs._maxWaitingTime ;
		_timeWindow = rhs._timeWindow ;
		_anyFTypeMatch = rhs._anyFTypeMatch ;
	}

	return *this ;
}


bool GroupedCollectionOrderCreator::Group::matchFileType(const std::string &ftype) const 
{
	return _anyFTypeMatch || find(_filetypes.begin(), _filetypes.end(), ftype) != _filetypes.end() ;
}

double GroupedCollectionOrderCreator::Group::maxWaitingTime() const // PRQA S 4120
{
	return _maxWaitingTime ;
}


double GroupedCollectionOrderCreator::Group::timeWindow() const // PRQA S 4120
{
	return _timeWindow ;
}

		
void GroupedCollectionOrderCreator::Group::addToGroup(const TemporaryOrder &o) const 
{
	_group.push_back(o) ;
}


bool GroupedCollectionOrderCreator::Group::getChain(vector <TemporaryOrder> &chain) const // PRQA S 4020, 4214
{
	chain.clear() ;
	
    ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
        ostringstream dbg ;
		dbg << exDebugSignature << "size: " << _group.size() ;
        ACS_LOG_DEBUG(dbg.str()); 
		if (not _group.empty()) {
            dbg.str("");
			dbg << exDebugSignature 
                << "_group[0].isInventoryExpired since " << _maxWaitingTime << " [sec]: " << boolalpha << _group[0].isInventoryExpired(_maxWaitingTime) ;
			ACS_LOG_DEBUG(dbg.str()) ;
		}
    ACS_CLASS_END_DEBUG


	if (_group.empty() || !_group[0].isInventoryExpired(_maxWaitingTime/86400)) { // PRQA S 4400
		return false ;
    }
	
	while (addFirstToChain(chain)) { /* nothing to do */ } ;
	
	return chain.size() > 0 ;
}

bool GroupedCollectionOrderCreator::Group::addFirstToChain(vector <TemporaryOrder> &chain) const  // PRQA S 4020
{
	if (_group.empty()) { return false ; }
	
	if (chain.empty()) { // the first is always added, check about its expiration is done into the caller
		chain.push_back(_group[0]) ;
		_group.erase(_group.begin()) ;
		return true ;
	}
	
	DateTime::JD50  jd_stop  = _group[0].stop() ; // PRQA S 3050 2
	DateTime::JD50  jd_start  = chain[0].start() ;
	
	// the relation should be ensured from the query filling the group
	if ((double(jd_stop) - double(jd_start)) <= _timeWindow/86400) { // PRQA S 3050, 3081, 4400
		chain.push_back(_group[0]) ;
		_group.erase(_group.begin()) ;
		return true ;
	}
	
	return false ;
}






ACS_CLASS_DEFINE_DEBUG_LEVEL(GroupedCollectionOrderCreator) 




GroupedCollectionOrderCreator::GroupedCollectionOrderCreator(const string &type, const string &algo, unsigned int satId, dbConnection &) :
	OrderCreator(type,algo,satId),
    _groups()
{
	loadGroups() ;
}

GroupedCollectionOrderCreator::~GroupedCollectionOrderCreator() throw() {}


void GroupedCollectionOrderCreator::fillOrder(dbOrder &order, const vector<TemporaryOrder> &tl) const // PRQA S 4214
{
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
	DateTime	dt ;
	order.setCreationDate(dt) ;
	order.setPriority(tl[0].priority()) ;
	order.setStatus("ELIGIBLE") ;
	order.setProcessorName(tl[0].processorName()) ; 
	order.setProcessorVersion(tl[0].processorVersion()) ;
	order.setProcessorConfiguration(tl[0].processorConfiguration()) ;
	order.setTroubleshooting(tl[0].troubleShooting()) ;
	order.setStartTime(tl[0].start()) ;
	order.setStopTime(tl[tl.size()-1].stop()) ; // PRQA S 3084
	order.setProcessingTimeWindow(tl[0].processingTimeWindow()) ;
	order.setOrderType(tl[0].orderType()) ;
	// set the satellite id inside the order if not null
	if (tl[0].satelliteId() != 0)  {
		order.setSatelliteId(tl[0].satelliteId()) ;
    }

    ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
        ostringstream dbg ;
		dbg << exDebugSignature ;
		dbg << "GroupedCollectionOrderCreator::createOrder dump \n" ;
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
		ACS_LOG_DEBUG(dbg.str()) ;
    ACS_CLASS_END_DEBUG

}

void GroupedCollectionOrderCreator::createOrder(vector<TemporaryOrder> &tl, dbConnection &theConnection) const // PRQA S 4020, 4214
{
	dbOrder *order= dynamic_cast<dbOrder*>(dbPersistentFactoryChain::instance()->newObject("dbOrder", theConnection.getKey())); // PRQA S 3081, 4412
	if(!order) {
		TemporaryOrder::OrderException ex("Cannot allocate a dbOrder") ;
		ACS_THROW(ex) ;
	}
	
	
	dbPersistentScopeGuard theOrderScopeGuard(order) ;
    fillOrder(*order, tl) ;
	
	db::Transaction transaction(theConnection) ;
	transaction.begin_work("") ; 
	
    ACS_LOG_INFO ("Emitting order " << order->getOrderType() << " for processor " << order->getProcessorName() << " " << order->getProcessorVersion() << " conf " << order->getProcessorConfiguration());
	ostringstream ostart ;
    ostringstream ostop ;
	order->getStartTime().utcs(ostart) ;
	order->getStopTime().utcs(ostop) ;
    ACS_LOG_INFO ("               " << "Processing window: " << ostart.str() << " " << ostop.str());

	try {
	
		if (!simulation()) {
			// save the order (remember is inside a transaction) 
			order->save(theConnection) ;
			insertTriggeringFiles(tl,order->getOrderId(),theConnection) ;			
		}
		else {
            ACS_LOG_INFO ("SIMUL MODE no order emitted")
		}
		
		if (!simulation()) {
			// remove the temporay orders
            const string &dbKey = theConnection.getKey() ;

			// Prepare the list of Ids
			vector <dbPersistent::IdType> idList;
			for (size_t i= 0; i < tl.size(); ++i) 
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
			ACS_LOG_INFO ("SIMUL MODE no temporary order removed") ;
		}
		
		transaction.commit() ; 
		ACS_LOG_INFO ( "The order id: " << order->getOrderId() << " has just been succesfully emitted." );
		appendSummary(OrderIdSuccesfullyCreated,order->getOrderId()) ;
	}
	catch (...) {
		transaction.roll_back() ;
		throw ;
	}
	
}


void GroupedCollectionOrderCreator::createOrders(dbConnectionPool &pool,ConfigurationSingleton::StopPredicate &stop_pred) const  // PRQA S 4020
{
    dbConnectionWrapper pw(pool) ;
    dbConnection &theConnection = pw ; // PRQA S 3050
    const string &dbKey = theConnection.getKey() ;

	// find unique file types temporary orders
	string allTemporaryAlgoSql = TemporaryOrder::getSqlCommand(dbKey, type(), algo(), satelliteId()) ; 
	SQLString sqlString(dbKey, SQLString::COMMAND) ;
	sqlString.forceSQLString(allTemporaryAlgoSql) ;

	dbQuery tempAlgoQuery(theConnection,sqlString, "SelTmpOrders") ;
	dbSet tempAlgoSet;
	tempAlgoQuery.execForSet(tempAlgoSet) ;

    ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "All matching algo temporary orders are " << tempAlgoSet.size());

	// now try to group orders into groups
    size_t rcount = tempAlgoSet.size() ;
	for (size_t i=0; i < rcount; ++i) {
		dbRow row(dbKey) ;
		tempAlgoSet.getNextRow(row) ;
		TemporaryOrder tord ;
		tord.loadRow(row) ;
		assignToGroup(tord) ;
	}
	
	// here we have all groups assigned
	// because the query was ordered by validitystart, we check if the first temporary order 
	// in each group has expired.
	// if it has expired, we need to chain orders until their validity stop fits into the time window.
	// A problem arises, if an order with a validity start in the past has just been inventoried, we need to wait
	// until it expires to emit chains.
	// If we do not wait, potentially there could be overlapping chains or there will be ambiguity to where a 
	// chain should start because the starting point is computed on inventorydate and the length is computed on validitystart
	// and potentially there is no relation between the two date.
	
	// so for each group
	for (size_t i=0; i < _groups.size(); i++) {
        ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "chains for group[" << i << "]");

		vector<TemporaryOrder> chain ;
		while (_groups[i].getChain(chain)) {
            ACS_CLASS_WRITE_DEBUG (ACS_MID_VERB, "_groups["<< i << "]: chain length:" << chain.size());
			createOrder(chain, theConnection) ;
            if (stop_pred()) { return ; }
		}
	}
}


void GroupedCollectionOrderCreator::loadGroups() 
{
	double otherMaxWait = getMaxWaitingTime("") ; // PRQA S 4412 2 
	double otherTimeWindow = getTimeWindow("") ;

    ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
        ACS_LOG_DEBUG(SimpleDebugSignature << "MaxWaitingTime: " << otherMaxWait << " [s]" );
        ACS_LOG_DEBUG(SimpleDebugSignature << "TimeWindow: " << otherTimeWindow << " [s]" );
    ACS_CLASS_END_DEBUG

	size_t groups = ConfigurationSingleton::instance()->get().getArraySize(buildKey("group","groups")) ; // PRQA S 4412

    ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "number of groups" << groups);

	for (size_t i=0; i<groups; i++) {
		ostringstream osgroup ;
		osgroup << "group[" << i << "]" ;
		string ftypesuffix = ".filetypes.filetype" ;
        size_t filetypes = ConfigurationSingleton::instance()->get().getArraySize(buildKey(osgroup.str()+ftypesuffix,"groups")) ;// PRQA S 4412
        ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "number of filetypes in group[" << i << "]: " << filetypes);
		vector<string> ftypes ;
		ConfigurationSingleton::instance()->get().getArray(buildKey(osgroup.str()+ftypesuffix,"groups"), ftypes) ;

        ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
            ostringstream os;
            size_t fsize = ftypes.size() ;
			for (size_t f = 0; f < fsize; ++f) { 
				os << "group[" << i << "]: " << ftypes[f] << "\n" ;
			}
            ACS_LOG_DEBUG(SimpleDebugSignature << os.str());
        ACS_CLASS_END_DEBUG
	
		double maxWait = getMaxWaitingTime("","groups." + osgroup.str()) ;	// PRQA S 4412 2 	
		double timeWin = getTimeWindow("","groups." + osgroup.str()) ;		

        ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "group[" << i << "] maxWait:" << maxWait << " timeWin:" << timeWin);

		_groups.push_back(Group(ftypes,maxWait,timeWin)) ;
	}
	
	// now fill the other group
	_groups.push_back(Group(vector<string>(),otherMaxWait,otherTimeWindow)) ;
	
	
}



size_t GroupedCollectionOrderCreator::assignToGroup(const TemporaryOrder &o) const // PRQA S 4020, 4214
{
	for (size_t i=0; i < _groups.size() ; i++) {
        ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "check filetype " << o.fileType() << " for group #" << i );

		if (_groups[i].matchFileType(o.fileType())) {
            ACS_CLASS_WRITE_DEBUG (ACS_VLO_VERB, "assigned" );
			_groups[i].addToGroup(o) ;
			return i ;
		}
	}
	
	return size_t(-1) ; // PRQA S 2895, 3081
} 

_ACS_END_NAMESPACE
