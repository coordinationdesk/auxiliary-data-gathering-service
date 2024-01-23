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
	Revision 5.8  2016/05/10 21:33:32  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 5.7  2016/04/11 13:36:22  marpas
	adopting new db interfaces
	
	Revision 5.6  2016/03/26 21:38:45  marpas
	using new db I/F
	
	Revision 5.5  2014/02/06 09:05:42  francesco.avanzi
	passed from dbConv() to db::toSQL()
	
	Revision 5.4  2014/02/03 13:45:52  francesco.avanzi
	coding best practices applied & new db::Transaction used
	
	Revision 5.3  2013/07/22 16:44:04  marpas
	compilation warnings removed
	
	Revision 5.2  2013/07/22 13:42:06  marpas
	ADC-193 implemented
	coding best practices enforced
	compilation and qa warnings fixed
	
	Revision 5.1  2013/07/09 11:54:43  marpas
	new dbRow inteface caused ambiguity
	
	Revision 5.0  2013/06/19 18:07:28  marpas
	adopting libException 5.x standards
	types fixed
	using OrderCreator::getNUllSatId and OrderCreator::getNullSatName()
	
	Revision 2.19  2013/06/19 14:54:15  marfav
	Restored null satellite id management (use zero to identify null value)
	
	Revision 2.18  2013/06/18 15:24:01  marfav
	Null satellite management fixed
	
	Revision 2.17  2013/05/14 18:50:17  marpas
	no longer master/slave support through dbConnection
	
	Revision 2.16  2013/04/12 12:18:00  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 2.15  2013/04/09 08:39:55  marpas
	NameIdAssociator refactoring
	
	Revision 2.14  2013/03/12 17:54:38  marpas
	dbConnectionPool is mandatory in file actions
	
	Revision 2.13  2013/02/26 18:20:56  marpas
	dbSet new I/F adopted
	
	Revision 2.12  2013/02/12 13:46:53  marpas
	dbConnectionPool concept introduced
	libDBOrderCreator new interfaces
	coding best practices applied
	debug improved
	qa rules
	FileGuard used when needed instead of File::unlink mess around
	
	Revision 2.11  2012/03/07 13:50:54  marpas
	preparing order parameters file
	
	Revision 2.10  2010/03/08 13:43:36  enrcar
	EC:: Oracle DB handled
	
	Revision 2.9  2009/09/29 14:10:24  marpas
	dbResourceSet no longer used
	
	Revision 2.8  2008/10/07 12:43:47  crivig
	porting to postgres
	
	Revision 2.7  2008/09/03 17:19:25  marpas
	fixed processor based RPRO (not CRYOSAT)
	
	Revision 2.6  2008/09/03 17:15:07  ivafam
	goodOrdersSummary and appendGoodOrder method implemented
	
	Revision 2.5  2008/09/03 16:30:04  marpas
	CRYOSAT: RPRO External is like NRT - was lost during Aeolus coding
	
	Revision 2.4  2007/04/18 13:59:27  marpas
	RPRO External for AEOLUS implemented
	
	Revision 2.3  2007/04/16 21:10:12  marpas
	work in progress
	
	Revision 2.2  2007/03/29 12:05:24  marpas
	work in progress
	
	Revision 2.1  2006/03/03 10:06:33  marpas
	exception catching fixed
	
	Revision 2.0  2006/02/28 10:24:18  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2005/10/06 15:28:45  marfav
	Import libDefaultOrderCreator
	
	
*/

#include <RPROExtOrderCreator.h>
#include <TemporaryOrder.h>
#include <dbQuery.h>
#include <dbSet.h>
#include <dbInventoryObject.h>
#include <dbOrderGeneration.h>
#include <dbPersistentFactoryChain.h>
#include <ConfigurationSingleton.h>
#include <dbDownloadFile.h>
#include <dbOrder.h>
#include <dbOnDemandDistribution.h>
#include <Transaction.h>
#include <dbConnectionPool.h>
#include <dbPersistentScopeGuard.h>
#include <Filterables.h>
#include <SatelliteName.h>
#include <FileGuard.h>
#include <XMLIstream.h>
#include <dbFileActionFactoryChain.h>
#include <dbFileActionScopeGuard.h>
#include <dbConv.h>
#include <unistd.h>



_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(RPROExtOrderCreator) 

RPROExtOrderCreator::RPROExtOrderCreator(const string &type, const string &algo, unsigned int satId, dbConnection &conn) :
	OrderCreator(type,algo, satId) ,
    _goodOrders()
{
}

RPROExtOrderCreator::~RPROExtOrderCreator() throw() {}

void RPROExtOrderCreator::createOrders(dbConnectionPool &pool,ConfigurationSingleton::StopPredicate &stop_pred) const  // PRQA S 4020
{
    dbConnectionWrapper pw(pool) ;
    dbConnection &theConnection = pw ; // PRQA S 3050
    const string &dbKey = theConnection.getKey() ;
	string sqlCommand = TemporaryOrder::getSqlCommand(dbKey, type(), algo(), satelliteId()) ;
	SQLString sqlString(dbKey, SQLString::COMMAND) ;
	sqlString.forceSQLString(sqlCommand) ;
	dbQuery getQuery(theConnection,sqlString, "SelectTmpOrders") ;
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
	
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "ExtRPRO orders are" << orders.size()) ;
    size_t osize = orders.size() ;
	for (size_t i=0; i < osize; i++) {
		
        if (stop_pred()) { return ; }
		
		try {
			ACS_LOG_INFO("Parsing temporary order id: " << orders[i].id()) ;
			dbInventoryObject *theObj = 0 ;
			try {
				theObj = dynamic_cast<dbInventoryObject*>(dbPersistentFactoryChain::instance()->newObject("dbInventoryObject", theConnection.getKey()));// PRQA S 3081
				if (!theObj) {
					ACS_THROW(TemporaryOrder::OrderException("Cannot allocate a dbInventoryObject")) ; // PRQA S 3081
				}
				dbPersistentScopeGuard theObjScopeGuard(theObj) ;
				theObj->setFileName(orders[i].fileName()) ;
				theObj->setFileVersion(orders[i].fileVersion()) ;
 				ACS_LOG_INFO("Downloading ExtRPRO file: \"" << orders[i].fileName() << " " << orders[i].fileVersion()) ;
				downloadAndEmitOrder(orders[i],theObj, theConnection, stop_pred, pool) ;
			}
			catch(exception &e) {
				ACS_THROW(TemporaryOrder::OrderException(e,"Cannot download ExtRPRO file " + orders[i].fileName() + " version " + orders[i].fileVersion())) ; // PRQA S 3081
			}
		}
		catch(exception &e) { // notify now and continue with next if any.
			ACS_LOG_NOTIFY_EX(e) ; 
			appendSummary(TemporaryOrderIdHasProblem,orders[i].id()) ; // PRQA S 3000, 3010
		}
	}
}

void RPROExtOrderCreator::downloadAndEmitOrder(  // PRQA S 4020, 4214
    const TemporaryOrder& tmpOrder, 
    dbInventoryObject *obj, 
    dbConnection &theConnection, 
    ConfigurationSingleton::StopPredicate &stop_pred,
    dbConnectionPool &pool) const
{	
	try {
	    obj->load(theConnection) ;

	    // now download the ExtRPRO (XML) file in a temporary path
	    string tmpPath = getTempDownloadPath("") ;

	    string tmpName = File::tempname(tmpPath+"/ERP", "xml") ; // PRQA S 3050 
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
		emitOrder(tmpOrder, tmpName, theConnection) ;
	}
	catch(...) {
		appendSummary(TemporaryOrderIdHasProblem,tmpOrder.id()) ;
		throw ;
	}
	
}

void RPROExtOrderCreator::emitOrder(const TemporaryOrder& tmpOrder, const string &tmpName, dbConnection &theConnection) const  // PRQA S 4214
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Parsing:" << tmpName) ;
	rsResourceSet erpOrder ;
	XMLIstream xml(tmpName) ;

	xml >> erpOrder ;
	
	SatelliteName* sat = SatelliteName::instance(getConnKeyFromApp());

        string satName = ( OrderCreator::getNullSatId() == satelliteId() ) ? getNullSatName() : sat->name(satelliteId()) ; // PRQA S 3000, 3384, 3385

	if (StringUtils::equalsNoCase(satName,"CRYOSAT")) {
		emitOrderList(erpOrder, tmpOrder, theConnection) ;
		goodOrdersSummary() ;
	}
	else {
		emitOrder(erpOrder, tmpOrder, theConnection) ;
    }
}




string RPROExtOrderCreator::getLastProcessorVersion(dbConnection &conn, const string &processorName) const // PRQA S 4214
{

    const string& dbKey = conn.getKey() ;
    SQLString queryString(dbKey, SQLString::SELECT) ;

    SQLString count(dbKey, SQLString::FUNCTION ) ;
    count.setFunctionName("MAX"); 
    count.addFunctionParameter("processorversion");
    queryString.addSelectList(count);

    queryString.addTableList( "t_processors" ) ;

    dbQueryParameters pars(dbKey) ;
    pars.addParameter(dbParam(dbKey, "t_processors","processorname",dbParam::EQUAL_TO,db::toSQL(dbKey, processorName))) ;
    queryString.addANDWhereParameter(pars) ;

	dbQuery query1(conn, queryString, "SelMaxProcVersion") ;
	dbSet set;
	dbRow row(dbKey);
	query1.execForSet(set) ;
	set.getNextRow(row) ;
	string res ;
	return row.column(0,res, 0) ;
}



void RPROExtOrderCreator::emitOrder(const rsResourceSet &erpOrder, const TemporaryOrder &tmpOrder, dbConnection &theConnection) const  // PRQA S 4020, 4214
{	
	// parse and emit orders
	// how many orderrequests are in the request ?
	string procsKey = "Data_Block.List_of_Processors.Processor" ;
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "temporary ExtRPRO file "
            << "contains request for " << erpOrder.getArraySize(procsKey) << " processors.") ;

	vector<string> theProcessors ;
	erpOrder.getArray(procsKey, theProcessors) ;
	// string processorNames ;
	const string &dbKey = theConnection.getKey() ;

	vector<string> processorNamesList;

    size_t psize = theProcessors.size() ;
	for (size_t i=0; i < psize ; ++i) {
		//processorNames += ( i > 0 ) ? "," : "" ; // PRQA S 3380
		//processorNames += db::toSQL(dbKey, theProcessors[i]) ; // PRQA S 3050, 3081
		processorNamesList.push_back (db::toSQL(dbKey, theProcessors[i]) );
	}
	
	
	string valStart ;
	erpOrder.getValue("Data_Block.Time_Interval.Start", valStart) ;
	DateTime validityStart = valStart ;

	string valStop ;
	erpOrder.getValue("Data_Block.Time_Interval.Stop", valStop) ;
	DateTime validityStop = valStop ;


    SQLString sqlString(dbKey, SQLString::SELECT) ;

	// DISTINCT
	sqlString.setDistinct (true);
    sqlString.addSelectList("filetype");

    sqlString.addTableList( dbOrderGeneration::getTable() ) ;

	{
		dbQueryParameters pars(dbKey) ;
		pars.addParameter(dbParam(dbKey, dbOrderGeneration::getTable(), "ordertype", dbParam::EQUAL_TO,db::toSQL(dbKey, "RPRO"))) ;
		sqlString.addANDWhereParameter(pars) ;
	}

	{
		dbQueryParameters pars(dbKey) ;
		pars.addParameter(dbParam(dbKey, dbOrderGeneration::getTable(), "processorname", dbParam::IN, processorNamesList)) ;
		sqlString.addANDWhereParameter(pars) ;
	}

	db::Transaction transaction(theConnection) ;

	dbQuery query(theConnection, sqlString, "OrdGenSelDistinctFType") ;
	dbSet genset ;
	query.execForSet(genset) ;
	size_t ftypesNum = genset.size();
	if(0 == ftypesNum)
	{
		ACS_LOG_WARNING("No filetypes to reprocess with the given parameters.") ;
		return;
	}

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "there are "<< ftypesNum <<" ftypes selected for reprocessing") ;

	vector<string> filetypesList;

	string inftypes ;

	dbRow row(dbKey) ;
	for (int ftype=0; genset.getNextRow(row); ++ftype)  // PRQA S 4238
	{
		if (ftype >= 0) {
			inftypes += ", " ;
        }
		string filetype;
		row.column(0, filetype);
		inftypes += filetype;
		filetypesList.push_back ( db::toSQL (dbKey, filetype) );
	}
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "list of them: " << inftypes) ;

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "selecting inventory files into interval " << inftypes) ;


    SQLString sqlString2 (dbKey, SQLString::SELECT) ;

    sqlString2.addSelectList("filename");
    sqlString2.addSelectList("fileversion");
    sqlString2.addSelectList("filetype");
    sqlString2.addSelectList("fileclass");
    sqlString2.addSelectList("satelliteid");

    sqlString2.addTableList( dbInventoryObject::getTable() ) ;

	{
		dbQueryParameters par1 (dbKey) ;
		par1.setParamOperator( dbQueryParameters::AND ) ;
		par1.addParameter (dbParam(dbKey, "t_inventory", "validitystart", dbParam::MAJOR_OR_EQUAL_OF, db::toSQL(dbKey, validityStart, dbConv::YearToFraction))) ;
		par1.addParameter (dbParam(dbKey, "t_inventory", "validitystart", dbParam::MINOR_OR_EQUAL_OF, db::toSQL(dbKey, validityStop, dbConv::YearToFraction))) ;

		dbQueryParameters par2 (dbKey) ;
		par2.setParamOperator( dbQueryParameters::AND ) ;
		par2.addParameter (dbParam(dbKey, "t_inventory", "validitystop", dbParam::MAJOR_OR_EQUAL_OF, db::toSQL(dbKey, validityStart, dbConv::YearToFraction))) ;
		par2.addParameter (dbParam(dbKey, "t_inventory", "validitystop", dbParam::MINOR_OR_EQUAL_OF, db::toSQL(dbKey, validityStop, dbConv::YearToFraction))) ;

		dbQueryParameters par3 (dbKey) ;
		par3.setParamOperator( dbQueryParameters::AND ) ;
		par3.addParameter (dbParam(dbKey, "t_inventory", "validitystart", dbParam::MINOR_OR_EQUAL_OF, db::toSQL(dbKey, validityStart, dbConv::YearToFraction))) ;
		par3.addParameter (dbParam(dbKey, "t_inventory", "validitystop" , dbParam::MAJOR_OR_EQUAL_OF, db::toSQL(dbKey, validityStop, dbConv::YearToFraction))) ;

		dbQueryParameters group1 (dbKey) ;
		group1.setParamOperator( dbQueryParameters::OR ) ;
		group1.addQueryParameter (par1);		
		group1.addQueryParameter (par2);		
		group1.addQueryParameter (par3);		

		sqlString2.addANDWhereParameter (group1); 
	}

	{
		dbQueryParameters par1 (dbKey) ;
		par1.addParameter (dbParam(dbKey, "t_inventory", "validityflag", dbParam::EQUAL_TO, db::toSQL(dbKey, true))) ;
		sqlString2.addANDWhereParameter (par1); 
	}

	{
		dbQueryParameters par1 (dbKey) ;
		par1.addParameter (dbParam(dbKey, "t_inventory", "filetype", dbParam::IN, filetypesList)) ;
		sqlString2.addANDWhereParameter (par1); 
	}

	query.sqlString(sqlString2, "SEL " + dbInventoryObject::getTable()) ;

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "query string is: " << sqlString.getSQL()) ;

	dbSet set ;
	query.execForSet(set) ;
	size_t invNum = set.size();

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "there are "<<invNum <<" inventory file(s) selected to trigger reprocessing") ;
	
	if (0 == invNum) {
		ACS_LOG_WARNING("No inventory files to Reprocess with the given parameters.") ;
		return ;
	}

	try {

		transaction.begin_work("") ; 

		// Temporary table 
		// will be delete when exiting the scope
		dbTemporaryTable ttable(theConnection) ;	/* Create temporary table (will be removed leaving the scope) */
		const string tmpColName ("pname");
        ttable.setColumnName (tmpColName);
        string ttable_name = ttable.createTable( theProcessors ) ; // create and populate temp.table

		for (size_t inventory=0; set.getNextRow(row); inventory++) // PRQA S 4238
		{
			uint16_t rowindex = 0 ;
			string filename ; row.column(rowindex++,filename) ;
			string fileversion ; row.column(rowindex++,fileversion) ;
			string filetype ; row.column(rowindex++,filetype) ;
			string fileclass ; bool fileclassNull = false ; row.column(rowindex++,fileclass,&fileclassNull) ;
			int satid = 0 ;  bool satidNull = false ; row.column(rowindex++,satid, &satidNull) ;



	        SQLString sqlString3 (dbKey, SQLString::CUSTPROCEDURE ) ;
	        sqlString3.setProcedureName("p_generateTmpOrderPName2");

	        sqlString3.addProcedureParameter (db::toSQL(dbKey, "f") );
	        sqlString3.addProcedureParameter (db::toSQL(dbKey, filename) );
	        sqlString3.addProcedureParameter (db::toSQL(dbKey, fileversion) );
	        sqlString3.addProcedureParameter (db::toSQL(dbKey, filetype) );

	        sqlString3.addProcedureParameter ((fileclassNull ? SQLDialectFactory::sqlDialect(dbKey).nullValue() : db::toSQL(dbKey, fileclass)) );
	        sqlString3.addProcedureParameter ((satidNull     ? SQLDialectFactory::sqlDialect(dbKey).nullValue() : db::toSQL(dbKey, satid))     );

	        sqlString3.addProcedureParameter (db::toSQL(dbKey, ttable_name));
	        sqlString3.addProcedureParameter (db::toSQL(dbKey, tmpColName));

			try {
				ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "SQL: " << sqlString3.getSQL() ) ;
				query.sqlString(sqlString3, "p_generateTmpOrderPName call") ;
				query.execForStatus() ;
			}
			catch(exception &x) {
				ACS_LOG_NOTIFY_EX(x) ;
				// go on 
			}
		}//for

		ostringstream finalReport;
		finalReport<<"Finished: "<<invNum<<" inventory injected into the OrderGenerator queue" ;
		ACS_LOG_INFO(finalReport.str()) ;


		ACS_CLASS_BGN_DEBUG(1) 
		    try {

			    string checkTmpQry ;

				SQLString sqlString4(dbKey, SQLString::SELECT) ;
				sqlString4.addSelectList("*");
				sqlString4.addTableList( "t_temporaryorders" ) ;

			    query.sqlString(sqlString4, "DEBUG_selTmpOrders") ;
			    dbSet set2 ;
			    query.execForSet(set2) ;
                ostringstream dbg ;
                dbg << exDebugSignature ;
			    for (size_t r=0; set2.getNextRow(row); r++) // PRQA S 4238
			    {
				    for (uint16_t c=0; c<row.columns(); c++) {
					    string v ; 
					    dbg << (row.column(c).isNull() ? string("<null>") : row.column(c).column(v)) << " " ; // PRQA S 3081, 3380, 3385
				    }
				    ACS_LOG_DEBUG(SimpleDebugSignature << dbg.str()) ;
			    }
            }
            catch(exception &xx) {
                ACS_LOG_NOTIFY_EX(xx) ;
            }
		ACS_CLASS_END_DEBUG

		if (simulation()) {
			ACS_LOG_WARNING("SIMUL MODE The temporary order is not removed") ;
			transaction.roll_back() ;
		}
		else {
			// remove the temporay order
			tmpOrder.removeFromDb(theConnection) ;
			transaction.commit() ; 
		}
		
	}
	catch(exception &x) {
		transaction.roll_back() ;
		ACS_THROW(TemporaryOrder::OrderException(x,"Rolling back reprocessing orders because the following exception")) ;  // PRQA S 3081
	}

	return ;
}




void RPROExtOrderCreator::fillOrder( // PRQA S 4214
    dbConnection &conn, 
    const rsResourceSet &erpOrder, 
    const string &basekey,
    const TemporaryOrder &tmpOrder, 
    dbOrder &theOrder,
    size_t req_num) const 
{
	DateTime dt ;
	theOrder.setCreationDate(dt) ;
	theOrder.setPriority(tmpOrder.priority()) ;
	theOrder.setStatus("ELIGIBLE") ;

	string processorName ;
	erpOrder.getValue(basekey+".SIRAL_Processing.Processor", processorName) ;

	theOrder.setProcessorName(processorName) ; 


	string processorVersion ;
	try {
		erpOrder.getValue(basekey+".SIRAL_Processing.Version", processorVersion) ;
	}
	catch(exception &e) {
		ACS_LOG_WARNING("cannot address processor Version for request #" << req_num) ;
	}


	if (processorVersion.empty()) {
		ACS_LOG_WARNING("Trying to localize the last processor version") ;
		processorVersion = getLastProcessorVersion(conn, processorName) ;
#warning Last Processor version to be retrieved			
	}

	theOrder.setProcessorVersion(processorVersion) ;

	string processorConfiguration ;
	try {
		erpOrder.getValue(basekey+".SIRAL_Processing.Config", processorConfiguration) ;
	}
	catch(exception &e) {
		ACS_LOG_WARNING("cannot address processor Configuration for request #" << req_num << " proceeding with no specific conf") ;
	}

	theOrder.setProcessorConfiguration(processorConfiguration) ;

	theOrder.setTroubleshooting(tmpOrder.troubleShooting()) ;

	string Start ;
	erpOrder.getValue(basekey+".SIRAL_Processing.Start", Start) ;
	theOrder.setStartTime(Start) ;

	string Stop ;
	erpOrder.getValue(basekey+".SIRAL_Processing.Stop", Stop) ;
	theOrder.setStopTime(Stop) ;


	theOrder.setProcessingTimeWindow(tmpOrder.processingTimeWindow()) ;

	string ordertype ;
	erpOrder.getValue(basekey+".Order_Type", ordertype) ;
	theOrder.setOrderType(tmpOrder.orderType()) ;

	string externalId ;
	erpOrder.getValue(basekey+".LTA-ID", externalId) ;
	theOrder.setExternalOrderId(externalId) ;
}


void RPROExtOrderCreator::emitOrderList(const rsResourceSet &erpOrder, const TemporaryOrder &tmpOrder, dbConnection &theConnection) const  // PRQA S 4020, 4214
{	
	// parse and emit orders
	// how many orderrequests are in the request ?
	string requestsKey = "Data_Block.List_of_Order_Requests.OrderRequest" ;
	size_t numOrders = erpOrder.getArraySize(requestsKey) ;
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "temporary ExtRPRO file "
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

            fillOrder(theConnection, erpOrder, basekey.str(), tmpOrder, *theOrder, i) ;

			ACS_LOG_INFO("Emitting order " << theOrder->getOrderType() << " for processor " << theOrder->getProcessorName() << " " << theOrder->getProcessorVersion() << " conf " << theOrder->getProcessorConfiguration()) ;
			ostringstream ostart ;
            ostringstream ostop ;
			theOrder->getStartTime().utcs(ostart) ;
			theOrder->getStopTime().utcs(ostop) ;
			ACS_LOG_INFO("               " << "Processing window: " << ostart.str() << " " << ostop.str()) ;

			// set the satellite id inside the order if not null
			if (satelliteId() != 0 ) {
				theOrder->setSatelliteId(satelliteId()) ;
            }

			if (!simulation()) {
				// save the order (remember is inside a transaction) 
				theOrder->save(theConnection) ;
				appendGoodOrder(theOrder->getOrderId()) ;
			}
			else {
				ACS_LOG_WARNING("SIMUL MODE no order emitted") ;
			}
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

void RPROExtOrderCreator::appendGoodOrder(dbPersistent::IdType id) const
{
	_goodOrders.push_back(id) ;
}


void RPROExtOrderCreator::goodOrdersSummary() const
{
    size_t osize = _goodOrders.size() ;
	for (size_t i=0; i < osize; ++i) {
		appendSummary(OrderIdSuccesfullyCreated,_goodOrders[i]) ;
    }
		
	_goodOrders.clear() ;
}


_ACS_END_NAMESPACE
