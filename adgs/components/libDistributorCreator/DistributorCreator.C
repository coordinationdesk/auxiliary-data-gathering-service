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

	$Prod: $

	$Id$

	$Author$

	$Log$
	Revision 2.20  2016/11/30 09:31:31  francesco.avanzi
	now QualOk Algos can correclty load threshold
	
	Revision 2.19  2016/07/13 09:24:54  marfav
	CSGACQ-86 using SQLString instead of free SQL code
	
	Revision 2.18  2016/05/10 21:37:13  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 2.17  2016/04/11 13:37:19  marpas
	adopting new db interfaces
	
	Revision 2.16  2016/03/26 21:37:02  marpas
	using new db I/F
	
	Revision 2.15  2016/03/14 11:32:55  marpas
	nominal message moved from warning to privinfo
	
	Revision 2.14  2015/08/19 12:15:08  nicvac
	APF-306: Algo QUALIFIEDALWAYS
	
	Revision 2.13  2015/05/15 16:31:35  nicvac
	APF-264
	
	Revision 2.12  2014/02/05 15:51:50  francesco.avanzi
	ACS_CLASS_DEFINE_DEBUG_LEVEL reintroduced after being removed by mistake
	
	Revision 2.11  2013/04/30 13:34:39  marpas
	dbPersistent interface changed
	
	Revision 2.10  2013/04/17 17:43:35  marpas
	dbPersistent interface changed
	
	Revision 2.9  2013/04/12 12:19:50  marpas
	SQLDialectFactory::dialectName() needs a db connection key
	
	Revision 2.8  2013/04/08 12:47:33  marpas
	logical flow simplified in case of simulation
	
	Revision 2.7  2013/02/26 18:26:27  marpas
	dbSet new I/F adopted
	
	Revision 2.6  2013/02/07 17:51:33  marpas
	DistributorExecutor class added
	Factories, Creators and related classes interface changed to allow dbConnectionPoll easy handling and stop predicates
	
	Revision 2.5  2013/02/07 12:02:04  marpas
	introducing CompletedOrderFinalizer class
	qa rules enforced
	
	Revision 2.4  2013/02/05 10:23:18  marfav
	PackagingAlgoId management updated
	
	Revision 2.3  2013/02/04 15:46:24  marpas
	new schema fixed
	
	Revision 2.2  2013/02/04 15:40:17  marpas
	disseminationpackingalgo_id used for t_temporarydistribution
	
	Revision 2.1  2013/01/24 10:34:27  marpas
	qa warnings
	
	Revision 2.0  2013/01/21 17:52:32  marpas
	introducing dbConnectionPool
	coding best practices
	error management improved
	qa rules
	
	Revision 1.20  2012/12/21 12:41:05  marpas
	coding best practices
	introducing if compiled without PDS_OLD_SCHEMA the distributiorule_id field saving distribution items
	qa rules
	
	Revision 1.19  2012/03/08 11:18:27  marpas
	refactoring
	
	Revision 1.18  2012/02/29 15:17:45  chicas
	Added getAlgoKeyString and extended getconf to bool and vector string
	
	Revision 1.17  2010/03/08 13:47:51  enrcar
	EC:: DB Oracle handled
	
	Revision 1.16  2009/06/12 08:13:07  crivig
	log added
	
	Revision 1.15  2009/06/05 09:02:49  crivig
	added cvrg algos
	
	Revision 1.14  2009/03/24 09:41:40  crivig
	modified to handle new records in t_distributionitems
	
	Revision 1.13  2008/09/16 16:00:36  crivig
	bug fixed in porting
	
	Revision 1.12  2008/09/15 13:32:35  crivig
	porting to postgres8
	
	Revision 1.11  2008/05/20 13:30:01  crivig
	message modified
	
	Revision 1.10  2008/04/22 12:57:46  crivig
	shorten transaction by doing separatly insert and delete
	
	Revision 1.9  2008/02/01 15:21:40  crivig
	timestamp added when creatin distribution items
	
	Revision 1.8  2008/02/01 14:27:15  crivig
	qualifyreport algo added
	
	Revision 1.7  2007/04/11 14:04:08  crivig
	transaction removed
	
	Revision 1.6  2007/04/02 14:53:28  crivig
	Redistribution algo added
	
	Revision 1.5  2007/03/29 09:42:27  crivig
	*** empty log message ***
	
	Revision 1.4  2007/03/22 16:04:18  crivig
	*** empty log message ***
	
	Revision 1.3  2007/03/21 16:51:50  crivig
	testing
	
	Revision 1.2  2007/03/15 15:04:15  crivig
	coding and compiling...
	
	Revision 1.1.1.1  2007/03/14 13:11:12  crivig
	Imported source
	
*/

#include <DistributorCreator.h>
#include <DGDBSimulationSingleton.h>

#include <DateTime.h>
#include <SatelliteName.h>
#include <ConfigurationSingleton.h>
#include <StringUtils.h>
#include <Application.h>
#include <Filterables.h>
#include <sys/types.h> // PRQA S 1013
#include <dbRow.h>
#include <dbQuery.h>
#include <dbSet.h>
#include <dbPersistentFactoryChain.h>
#include <dbPersistentScopeGuard.h>
#include <dbQueryFactoryChain.h>
#include <dbDistributionItem.h>
#include <dbConv.h>
#include <Transaction.h>
#include <dbConnectionPool.h>
#include <unistd.h>

#include <MCFilterables.h>


#ifdef __linux__
  #if (__GNUC__ < 3)
	#include <algorithm>
  #endif
#endif

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;
using namespace mc ;
ACS_CLASS_DEFINE_DEBUG_LEVEL(DistributorCreator);

map<DistributorCreator::SummaryType, std::vector<dbPersistent::IdType> > DistributorCreator::_summary ;

DistributorCreator::DistributorCreator(const string &algo) :
	_algo(algo)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
}

DistributorCreator::~DistributorCreator() throw() 
{
}

bool DistributorCreator::simulation()  const 
{
	return DGDBSimulationSingleton::instance()->simulation() ;
}


const string &DistributorCreator::algo() const throw() // PRQA S 4120
{
	return _algo ;
}

string DistributorCreator::getAlgoKeyString() const
{
	return "" ;
}


string DistributorCreator::buildKey(const string &keyEnd, const string &subparam) const // PRQA S 5520 
{

	// build key for namespace
	string algoKeyString = getAlgoKeyString() ;
	
    if (algoKeyString == "") {
        if (StringUtils::equalsNoCase(_algo,"always")) {
            algoKeyString = "Always" ; 
        }
        else if (StringUtils::equalsNoCase(_algo,"qualified")) { 
            algoKeyString = "Qualified" ; 
        }
        else if (StringUtils::equalsNoCase(_algo,"preselection")) { 
            algoKeyString = "Preselection" ; 	
        }
        else if (StringUtils::equalsNoCase(_algo,"redistribution")) { 
            algoKeyString = "Redistribution" ; 	
        }
        else if (StringUtils::equalsNoCase(_algo,"qualifyreport")) { 
            algoKeyString = "QualifyReport" ; 	
        }
        else if (StringUtils::equalsNoCase(_algo,"totalcvrg")) { 
            algoKeyString = "TotalCvrg" ; 	
        }
        else if (StringUtils::equalsNoCase(_algo,"partialcvrg")) { 
            algoKeyString = "PartialCvrg" ; 	
        }
        else if (StringUtils::equalsNoCase(_algo,"partialcvrg")) {
            algoKeyString = "PartialCvrg" ;
        }
        else if (StringUtils::equalsNoCase(_algo,"qualifiedvalstopgt")) {
            algoKeyString = "QualifiedValstopGt" ;
        }
        else if (StringUtils::equalsNoCase(_algo,"qualifiedvalstoplt")) {
            algoKeyString = "QualifiedValstopLt" ;
        }
        else if (StringUtils::equalsNoCase(_algo,"qualifiedalways")) {
            algoKeyString = "QualifiedAlways" ;
        }
        else if (StringUtils::equalsNoCase(_algo,"qualok")) {
            algoKeyString = "QualOk" ;
        }
        else if (StringUtils::equalsNoCase(_algo,"qualokvalstopgt")) {
            algoKeyString = "QualOkValstopGt" ;
        }
        else if (StringUtils::equalsNoCase(_algo,"qualokvalstoplt")) {
            algoKeyString = "QualOkValstopLt" ;
        }
        else if (StringUtils::equalsNoCase(_algo,"qualifiedfileclassoriginator")) {
        	algoKeyString = "QualifiedFileclassOriginator" ;
        }
        else {
            // nothing 
        }
    }

	string key = "DistributionGeneration." ;
	key += algoKeyString + "." ;
	if (not subparam.empty()) {
		key += subparam + "." ;
    }
	key += keyEnd ;
	
	return key ;
}

void DistributorCreator::getConf(const string &keyEnd, string &key, off_t &value, const string &subparam) const  // PRQA S 4214
{
	key = buildKey(keyEnd, subparam) ;
	ConfigurationSingleton::instance()->get().getValue(key, value) ;
}


void DistributorCreator::getConf(const string &keyEnd, string &key, double &value, const string &subparam) const  // PRQA S 4214
{
	key = buildKey(keyEnd, subparam) ;	
	ConfigurationSingleton::instance()->get().getValue(key, value) ;
}

void DistributorCreator::getConf(const string &keyEnd, string &key, string &value, const string &subparam) const   // PRQA S 4214
{
	key = buildKey(keyEnd, subparam) ;	
	ConfigurationSingleton::instance()->get().getValue(key, value) ;
}

void DistributorCreator::getConf(const string &keyEnd, string &key, bool &value, const string &subparam) const   // PRQA S 4214
{
	key = buildKey(keyEnd, subparam) ;	
	ConfigurationSingleton::instance()->get().getValue(key, value) ;
}

void DistributorCreator::getConf(const string &keyEnd, string &key, vector<string> &value, const string &subparam) const   // PRQA S 4214
{
	key = buildKey(keyEnd, subparam) ;	
	ConfigurationSingleton::instance()->get().getArray(key, value) ;
}


void DistributorCreator::appendSummary(SummaryType t, dbPersistent::IdType id) 
{
	_summary[t].push_back(id) ;
}
void DistributorCreator::deleteFromTempDist(const vector<dbPersistent::IdType> &id, dbConnection &theConnection) const // PRQA S 4020, 4214
{

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "start" );
	const string &theKey = theConnection.getKey();
	// deletes all record from t_temporarydistribution
	if (not id.empty())
	{
		SQLString sqlString(theKey, SQLString::DELETE) ;
		sqlString.addTableList("t_temporaryDistribution");
		{
			dbQueryParameters pp(theKey) ;
			pp.addParameter(dbParam(theKey,"t_temporaryDistribution","id" , dbParam::IN , id )) ;
			sqlString.addANDWhereParameter(pp) ;
		}

		// Logs
		for (unsigned int i= 0; i < id.size(); ++i) 
		{
			ACS_LOG_PRIVINFO ("Removing id: " << id[i] << " from t_temporarydistribution." );
		}

		// Do the actual Delete only if not in simulation mode
		if (!simulation()) 
		{
			db::Transaction transaction(theConnection) ;

			// remove the temporay distributions
			dbQuery deleteQuery(theConnection,sqlString, "delete t_temporaryDistribution") ;
			deleteQuery.execForStatus() ;
		}
		else 
		{
			ACS_LOG_INFO( "SIMUL MODE no temporary distribution removed");
		}
	}
}


dbPersistent::IdType DistributorCreator::send1InDistribution(const TemporaryInfo &tmpInfo, dbConnection &theConnection) const // PRQA S 4214
{
	// create one record for each tmpInfo element

	dbDistributionItem *distItem= dynamic_cast<dbDistributionItem*>(dbPersistentFactoryChain::instance()->newObject("dbDistributionItem", theConnection.getKey())); // PRQA S 3081, 4412
	if(!distItem) {
	
		ACS_THROW(DistributorCreator::DistributorException("Cannot allocate a dbDistributionItem")) ;
	}

	dbPersistentScopeGuard theDistItemScopeGuard(distItem) ;
	DateTime	dt ;
	distItem->setCreationDate(dt) ;	
	distItem->setDueDate(tmpInfo.duedate) ;	
	distItem->setFileName(tmpInfo.filename) ;
	distItem->setFileVersion(tmpInfo.fileversion) ;
	distItem->setMediaType(tmpInfo.mediatype) ;
	distItem->setUserId(tmpInfo.userid) ;
	distItem->setFormat(tmpInfo.format) ;
	if (tmpInfo.cartid != -1) { distItem->setCartId(tmpInfo.cartid) ; } // PRQA S 3010
	if (tmpInfo.subreq!= -1) { distItem->setSubReq(tmpInfo.subreq) ; }
#ifndef PDS_OLD_SCHEMA
    distItem->setDistributionRuleId(tmpInfo.policy) ;
    distItem->setDisseminationPackingAlgoId(tmpInfo.dissempkalgoid) ;
#endif
	
		exFDebug dbg ;
        dbg << "New distItem \n" ;
		ostringstream os ;
		distItem->getCreationDate().utcs(os) ;
		dbg << "CreationDate " << os.str() << "\n" ;
		ostringstream os1 ;
		distItem->getDueDate().utcs(os1) ;
		dbg << "DueDate " <<  os1.str() << "\n" ;
		dbg << "Filename " << distItem->getFileName() << "\n" ;
		dbg << "FileVersion " << distItem->getFileVersion() << "\n" ;
		dbg << "MediaType " << distItem->getMediaType() << "\n" ;
		dbg << "UserId " << distItem->getUserId() << "\n" ;
		dbg << "Format " << distItem->getFormat() << "\n" ;		
		dbg << "CartId " << tmpInfo.cartid << "\n" ;		
		dbg << "Subreq " << tmpInfo.subreq << "\n" ;		
#ifndef PDS_OLD_SCHEMA
		dbg << "DistRuleId " << tmpInfo.policy << "\n" ;		
		dbg << "DissemPkAlgoId " << tmpInfo.dissempkalgoid << "\n" ;		
#endif
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, dbg) ;


   	DateTime curDateTime ; string tmpCurDate ;
	ACS_LOG_INFO( "Emitting distribution item for filename " <<  distItem->getFileName() << 
				  " with fileversion " << distItem->getFileVersion() << 
				  " on media " << distItem->getMediaType() << 
				  " to user " <<  distItem->getUserId()
#ifndef PDS_OLD_SCHEMA
        	   << " for policy " <<  distItem->getDistributionRuleId()
#endif
    			);

	if (!simulation()) {
		// save the distribution item (remember is inside a transaction) 
		distItem->save(theConnection) ;
	}
	else {
		ACS_LOG_INFO("SIMUL MODE no distribution item emitted");
	}
    
    return distItem->getId() ;
}

void DistributorCreator::sendInDistribution(const vector<TemporaryInfo> &tmpInfo, dbConnection &theConnection ) const // PRQA S 4020, 4214
{


	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	// creates  records in the t_distributionitems
    
	db::Transaction transaction(theConnection) ;

	vector<dbPersistent::IdType> ids;
	
	try 
	{
		// open a transaction
		transaction.begin_work("") ; 
		// loop on tmpInfo size
		for (unsigned int i = 0; i<tmpInfo.size(); ++i)
		{ 
			dbPersistent::IdType savedId = send1InDistribution(tmpInfo[i], theConnection) ;

			ids.push_back(tmpInfo[i].id); // PRQA S 3000, 3010
			appendSummary(DistributionIdSuccesfullyCreated,savedId) ; // PRQA S 3000, 3010
			ACS_LOG_INFO( "The distribution item id: " << savedId << " has just been successfully emitted." );
			// CRYO2PDS-96: message for splunk added.
			ostringstream os;
			os << "The distribution item id: " << savedId << " has just been successfully emitted.";
			ACS_LOG_MC_INFO( mc_param("jobid", savedId ) <<
						 mc_param("filename",  tmpInfo[i].filename) <<
						 mc_param("message",  os.str() ) <<
						 mc_metric(Metric_LOG) <<
						 MCCode ("DD") <<
						 MCFunction ("Data Dissemination")) ;
		}
	    deleteFromTempDist(ids, theConnection);
		transaction.commit() ; 
	}
	catch (...) {
		transaction.roll_back() ;
		for (unsigned int i = 0; i<tmpInfo.size(); ++i) {
			appendSummary(TemporaryDistributionIdHasProblem,tmpInfo[i].id) ; // PRQA S 3000, 3010
			// CRYO2PDS-96: message for splunk added.
			ostringstream os;
			os << "The distribution item for filename " << tmpInfo[i].filename << " has not been emitted.";
			ACS_LOG_MC_INFO( mc_param("filename",  tmpInfo[i].filename) <<
						 mc_param("message",  os.str() ) <<
						 mc_metric(Metric_FAIL) <<
						 MCCode ("DD") <<
						 MCFunction ("Data Dissemination")) ;
        }
		throw ;
	}
}



void DistributorCreator::queryFromTempDist(const string &sqlCommand,  // PRQA S 4214
                                           const string &whyQuery, 
                                           vector<TemporaryInfo> & TmpInfo,
                                           dbConnection &theConnection) const
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"executing \"" << sqlCommand << '\"' );

	// execute the query sent as argument
	SQLString sqlString(theConnection.getKey(),SQLString::COMMAND) ;	
	sqlString.forceSQLString(sqlCommand) ;

    dbQuery query(theConnection, sqlString, whyQuery) ;
	dbSet set ;
	query.execForSet(set) ;
	size_t num_col = 0;
	
	// for each record loads a TemporaryInfo structure
    size_t ssize = set.size() ;
	for (size_t i=0; i < ssize ; ++i) 
	{
			
		dbRow row(theConnection.getKey()) ;
		set.getNextRow(row) ;
		if (0 == i) { num_col = row.columns(); } // PRQA S 3010
		TemporaryInfo rowInfo ;
        int current_column = 0 ;
		row.column(current_column++,rowInfo.id) ;
		row.column(current_column++,rowInfo.userid) ;
		row.column(current_column++,rowInfo.mediatype) ;
		row.column(current_column++,rowInfo.filename) ;
		row.column(current_column++,rowInfo.fileversion) ;
		row.column(current_column++,rowInfo.format) ;
		row.column(current_column++,rowInfo.duedate) ;
		row.column(current_column++,rowInfo.timestamp) ;
		row.column(current_column++,rowInfo.cartid) ;
		row.column(current_column++,rowInfo.subreq) ;
		if (num_col > 10) { // PRQA S 4400
            row.column(current_column++,rowInfo.qualifierFile) ; 
        } // PRQA S 4400 2
		if (num_col >= 12) { 
            row.column(current_column++,rowInfo.policy) ; 
	    row.column(current_column++,rowInfo.dissempkalgoid) ;
        }
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB," Id : " << rowInfo.id << " from t_temporarydistribution " );
		TmpInfo.push_back(rowInfo);		
	}
}

void DistributorCreator::emitSummary() 
{
	// needs to generate a summary
	if (!_summary[TemporaryDistributionIdHasProblem].empty()) {
		exFWarning war ;
		war << "Some temporary distribution has problem, see " << Application::instance()->applicationName() << " log file(s) for detail. Temporary distribution id(s) follow: " ;
		for (size_t i=0; i<_summary[TemporaryDistributionIdHasProblem].size(); ++i) { // PRQA S 4238, 4244
			if (i > 0) {
				war << "," ;
            }
			war << _summary[TemporaryDistributionIdHasProblem][i] ;
		}
		
		ACS_LOG_WARNING( war );

	}
	
	if (!_summary[DistributionIdSuccesfullyCreated].empty()) {
		exFMessage msg ;
		msg << "The following distribution item has been succesfully created (see " << Application::instance()->applicationName() << " log file(s) for detail): " ;
		for (size_t i=0; i<_summary[DistributionIdSuccesfullyCreated].size(); ++i) { // PRQA S 4238, 4244
			if (i > 0) {
				msg << "," ;
            }
			msg << _summary[DistributionIdSuccesfullyCreated][i] ;
		}
		
		ACS_LOG_INFO ( msg );
	}

	
	_summary.clear() ;
}			


_ACS_END_NAMESPACE
