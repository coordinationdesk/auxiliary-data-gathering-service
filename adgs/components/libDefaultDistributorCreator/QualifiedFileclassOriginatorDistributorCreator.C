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

	$Prod: A.C.S. libDefaultDistributorCreator$

	$Id$

	$Author$

	$Log$
	Revision 2.5  2018/02/15 09:51:17  nicvac
	ECICMF-97: Handling of virtual filetypes.
	
	Revision 2.4  2017/11/27 17:52:35  marpas
	qa warnings fixed
	
	Revision 2.3  2017/11/24 16:07:28  nicvac
	ECICMF-97: Component Test and Fix loop.
	
	Revision 2.2  2017/11/23 20:14:33  nicvac
	ECICMF-97: Implementation completed
	
	Revision 2.1  2017/11/22 13:52:30  nicvac
	ECICMF-97: QualifiedFileclassOriginatorDistributorCreator added.


 */

#include <QualifiedFileclassOriginatorDistributorCreator.h>
#include <DistributionAlgo.h>
#include <dbConnectionPool.h>
#include <dbRow.h>
#include <dbConnection.h>
#include <dbQuery.h>
#include <dbSet.h>
#include <dbColumn.h>
#include <dbQueryFactoryChain.h>
#include <dbConv.h>
#include <ConfigurationSingleton.h>

#include <StringUtils.h>
#include <RegExp.h>

#include <Filterables.h>
#include <dbFileTypeRecognition.h>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(QualifiedFileclassOriginatorDistributorCreator)


QualifiedFileclassOriginatorDistributorCreator::QualifiedFileclassOriginatorDistributorCreator(const string &algo):
DistributorCreator(algo),
_rules()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	ACS_LOG_PRIVINFO("Distribution Algo is [" << algo << "]");

	//Init from configuration
	string key ;
	try	{
		off_t numRules = 0 ;

		//DistributionGeneration.QualifiedFileclassOriginator.List_of_Rules.Rule[]:2
		getConf("Rule[]", key, numRules, "List_of_Rules") ;

		for ( off_t i = 0 ; i<numRules; ++i ) {
			//DistributionGeneration.QualifiedFileclassOriginator.List_of_Rules.Rule[0].FileType:EC_FTYPE
			//DistributionGeneration.QualifiedFileclassOriginator.List_of_Rules.Rule[0].Fileclass:FCLASS_B0.+
			//DistributionGeneration.QualifiedFileclassOriginator.List_of_Rules.Rule[0].Distribute:true

			ostringstream currKeyI; currKeyI<<"List_of_Rules.Rule["<<i<<"]";

			KeyRule_FT_FC keyRule;
			bool actionIsDistribute(true);

			getConf("FileType"   , key, keyRule.first  , currKeyI.str() ) ;
			getConf("Fileclass"  , key, keyRule.second , currKeyI.str() ) ;
			getConf("Distribute" , key, actionIsDistribute, currKeyI.str() ) ;

			RuleContent& ruleContent = _rules[ keyRule ];
			ruleContent.actionIsDistribute = actionIsDistribute;

			{
				//DistributionGeneration.QualifiedFileclassOriginator.List_of_Rules.Rule[0].List_of_Originators.Originator[]:2
				ostringstream currKeyOrig; currKeyOrig<< currKeyI.str() << ".List_of_Originators";
				off_t numOriginators = 0 ;
				try{
					getConf("Originator[]", key, numOriginators, currKeyOrig.str() ) ;
				} catch(rsResourceSet::NotFoundException& ex) {
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Originator List is empty for key: " << keyRule.first<<","<<keyRule.second );
				}

				ruleContent.procs.resize( numOriginators ); // PRQA S 3000

				for ( off_t j = 0 ; j<numOriginators; ++j ) {

					//DistributionGeneration.QualifiedFileclassOriginator.List_of_Rules.Rule[0].List_of_Originators.Originator[0].Name:L1P
					//DistributionGeneration.QualifiedFileclassOriginator.List_of_Rules.Rule[0].List_of_Originators.Originator[0].Version:001

					ostringstream currKeyJ; currKeyJ<< currKeyOrig.str() << ".Originator["<<j<<"]";

					getConf("Name"   , key, ruleContent.procs.at(j).first , currKeyJ.str() ) ; // PRQA S 3000 2
					getConf("Version", key, ruleContent.procs.at(j).second, currKeyJ.str() ) ;
				}
			}
		}

	} catch (rsResourceSet::NotFoundException &e) {
		ostringstream msg; msg<<"Distribution algo ["<<algo<<"] .Cannot find configuration value for key ["<< key<<"]";
		DistributorException de(e, msg.str());
		ACS_THROW( de );
	}
}

QualifiedFileclassOriginatorDistributorCreator::~QualifiedFileclassOriginatorDistributorCreator() throw() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB); // PRQA S 4631
}

void QualifiedFileclassOriginatorDistributorCreator::createDistribution(dbConnectionPool &pool, ConfigurationSingleton::StopPredicate &stop_pred) const // PRQA S 4020
{

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	dbConnectionWrapper pw(pool) ;
	dbConnection &theConnection = pw ; // PRQA S 3050
	const string &k = theConnection.getKey();

	// get the algoId from t_distributionalgo table
	int algoid = DistributionAlgo::instance(k)->id(_algo); // PRQA S 3000

	/*
	 SELECT t.id, t.userid, t.mediatype, t.filename, t.fileversion, t.format, t.duedate, t.tstamp,
	 	(CASE WHEN t.cartid IS NULL THEN -1 ELSE t.cartid END),
	 	(CASE WHEN t.subreq IS NULL THEN -1 ELSE t.subreq END),
	 	(CAST ' ' as varchar(5) ),
	 	t.policy, t.disseminationpackingalgo_id,
	 	-- Above part is required by framework
	 	-- Below part is required for algo filtering
	    i.filetype, i.fileclass, i.source, i.sourceswversion,
	    ftr.filetype, fcr.fileclass,    -- In this algo fcr.fileclass can be different from i.fileclass (can include wildcards)
	    q.name
	 FROM
	 t_temporarydistribution  t
	 join t_users             u   ON t.userid = u.userid
	 join t_inventory         i   ON (t.filename = i.filename AND t.fileversion=i.fileversion)
	 join t_qualifiedflag     q   ON i.qualified = q.id
	 join t_distributionrules dr  ON t.policy = dr.id_distributionrule
	 join t_filetypes         ftr ON dr.filetype_id = ftr.id_filetype
	 left join t_fileclasses  fcr ON dr.fileclass_id = fcr.id_fileclass
	 WHERE
	 t.preselection = true
	 AND (t.orderid is NULL OR u.allqualified = false)
	 AND t.algo = algoid
	*/
	SQLString selectString( k , SQLString::SELECT ) ;
	selectString.addSelectList("t.id") ;
	selectString.addSelectList("t.userid") ;
	selectString.addSelectList("t.mediatype") ;
	selectString.addSelectList("t.filename") ;
	selectString.addSelectList("t.fileversion") ;
	selectString.addSelectList("t.format") ;
	selectString.addSelectList("t.duedate") ;
	selectString.addSelectList("t.tstamp") ;
	{
		SQLString::SQLCase sql_case ;
		{
			dbQueryParameters cond1(k) ;
			cond1.addParameter(dbParam(k, "t", "cartid" , dbParam::ISNULL ) ) ;
			sql_case.addWhen(cond1, db::toSQL(k,-1) ) ;
		}
		sql_case.setElse( "t.cartid" ) ;
		selectString.addSelectList( sql_case  ) ;
	}
	{
		SQLString::SQLCase sql_case ;
		{
			dbQueryParameters cond1(k) ;
			cond1.addParameter(dbParam(k, "t","subreq" , dbParam::ISNULL ) ) ;
			sql_case.addWhen(cond1, db::toSQL(k,-1) ) ;
		}
		sql_case.setElse( "t.subreq" ) ;
		selectString.addSelectList( sql_case  ) ;
	}
	selectString.addSelectList("t.policy") ;
	selectString.addSelectList("t.disseminationpackingalgo_id") ;
	selectString.addSelectList("i.qualifierfile") ;

	selectString.addSelectList("i.filetype") ;
	selectString.addSelectList("i.fileclass") ;
	selectString.addSelectList("i.source") ;
	selectString.addSelectList("i.sourceswversion") ;
	selectString.addSelectList("ftr.filetype") ;
	selectString.addSelectList("fcr.fileclass") ;
	selectString.addSelectList("q.name") ;

	selectString.addTableList("t_temporarydistribution", "t");

	// Add Join
	{
		dbQueryParameters pp(k) ;
		pp.addParameter(dbParam(k, "u","userid" , dbParam::EQUAL_TO , "t.userid" )) ;
		selectString.addJoinList(SQLDialect::LEFT,"t_users","u", pp) ;
	}
	{
		dbQueryParameters pp(k) ;
		pp.addParameter(dbParam(k, "i","filename" , dbParam::EQUAL_TO , "t.filename" )) ;
		pp.addParameter(dbParam(k, "i","fileversion" , dbParam::EQUAL_TO , "t.fileversion" )) ;
		selectString.addJoinList(SQLDialect::INNER,"t_inventory","i", pp) ;
	}
	{
		dbQueryParameters pp(k) ;
		pp.addParameter(dbParam(k, "q","id" , dbParam::EQUAL_TO , "i.qualified" )) ;
		selectString.addJoinList(SQLDialect::INNER,"t_qualifiedflag","q", pp) ;
	}
	{
		dbQueryParameters pp(k) ;
		pp.addParameter(dbParam(k, "dr","id_distributionrule" , dbParam::EQUAL_TO , "t.policy" )) ;
		selectString.addJoinList(SQLDialect::INNER,"t_distributionrules","dr", pp) ;
	}
	{
		dbQueryParameters pp(k) ;
		pp.addParameter(dbParam(k, "dr","filetype_id" , dbParam::EQUAL_TO , "ftr.id_filetype" )) ;
		selectString.addJoinList(SQLDialect::INNER,"t_filetypes","ftr", pp) ;
	}
	{
		dbQueryParameters pp(k) ;
		pp.addParameter(dbParam(k, "dr","fileclass_id" , dbParam::EQUAL_TO , "fcr.id_fileclass" )) ;
		selectString.addJoinList(SQLDialect::LEFT,"t_fileclasses","fcr", pp) ;
	}
	//add Where
	{
		dbQueryParameters pp(k) ;
		pp.addParameter( dbParam(k, "t" , "preselection" , dbParam::EQUAL_TO , db::toSQL( k , true ) ) ) ;
		pp.addParameter( dbParam(k, "t" , "orderid" , dbParam::ISNULL  ) ) ;
		pp.addParameter( dbParam(k, "t" , "algo" , dbParam::EQUAL_TO , db::toSQL( k , algoid ) ) ) ;
		selectString.addANDWhereParameter(pp) ;
	}
	//add Or
	{
		dbQueryParameters pp(k) ;
		pp.addParameter( dbParam(k, "t" , "preselection" , dbParam::EQUAL_TO , db::toSQL( k , true ) ) ) ;
		pp.addParameter( dbParam(k, "u" , "allqualified" , dbParam::EQUAL_TO , db::toSQL( k , false )  ) ) ;
		pp.addParameter( dbParam(k, "t" , "algo" , dbParam::EQUAL_TO , db::toSQL( k , algoid ) ) ) ;
		selectString.addORWhereParameter(pp) ;
	}
	dbQuery selectQuery(theConnection, selectString, "SelQualFClassOriginator") ;
	dbSet s ;
	selectQuery.execForSet(s) ;

	vector <TemporaryInfo> toBePromoted ;
	vector <dbPersistent::IdType> toBeDeleted ;
	dbRow row(k) ;

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "query returned num items: " << s.size() );

	while ( s.getNextRow(row) ) {
		if (stop_pred()) { return ; }
		try {


			TemporaryInfo ti ;
			bool isNull_qualifierFile = false ;

			unsigned short i = 0 ;
			row.column( i++, ti.id );
			row.column( i++, ti.userid );
			row.column( i++, ti.mediatype );
			row.column( i++, ti.filename );
			row.column( i++, ti.fileversion );
			row.column( i++, ti.format );
			row.column( i++, ti.duedate );
			row.column( i++, ti.timestamp );
			row.column( i++, ti.cartid );
			row.column( i++, ti.subreq );
			row.column( i++, ti.policy );
			row.column( i++, ti.dissempkalgoid );
			row.column( i++, ti.qualifierFile, &isNull_qualifierFile);

			string invFiletype;
			string invFileclass;
			string invProcName;
			string invProcVers;
			string distRuleFiletype;
			string distRuleFileclass; 
            bool isNull_distRuleFileclass = false ;
			string qualifiedName;

			row.column( i++, invFiletype  );
			row.column( i++, invFileclass );
			row.column( i++, invProcName  );
			row.column( i++, invProcVers  );
			row.column( i++, distRuleFiletype );
			row.column( i++, distRuleFileclass, &isNull_distRuleFileclass);
			row.column( i++, qualifiedName);

			bool qualifiedProduct = ("QUALIFIED_PRODUCT" == qualifiedName); // PRQA S 4412

			//Get the rule
			if ( isNull_distRuleFileclass ) {
				distRuleFileclass = "NULL";
			}

			KeyRule_FT_FC keyRule = make_pair( distRuleFiletype, distRuleFileclass );
			MapRules::const_iterator ruleIt = _rules.find( keyRule );
			bool foundRule = ( ruleIt != _rules.end() );

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Distribution Check for file [" << ti.filename << "] of policy " << ti.policy
					<< "; distRuleFiletype: "<<distRuleFiletype<<"; distRuleFileclass: "<<distRuleFileclass
					<< "; invFiletype: "<<invFiletype<<"; invFileclass: "<<invFileclass ) ;

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Product qualification: [" << qualifiedName<< "] ==> qualified: "<<boolalpha<<qualifiedProduct );
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Key Rule for configuration space: [" << keyRule.first <<","<<keyRule.second<<"] ==> found: "<<boolalpha<<foundRule );

			if ( not qualifiedProduct ) {
				//Not qualified ==> drop

				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Distribution dropped for file [" << ti.filename << "] of policy " << ti.policy
						<< " because Not qualified ["<< qualifiedName <<"]" );

				toBeDeleted.push_back( ti.id ) ;
				deleteFromTempDist(toBeDeleted, theConnection) ;
				toBeDeleted.clear() ;

			} else {

				//Is qualified
				if ( foundRule ) {

					//Match configured rule filetype, fileclass Vs inventory's filetype, fileclass
					const RuleContent& ruleContent = ruleIt->second;

					//bool filetypeMatch = (invFiletype == distRuleFiletype);
					bool filetypeMatch(false);
					try {
						dbFileTypeRecognition ftr;
						filetypeMatch = ftr.relationship( invFiletype, distRuleFiletype, theConnection );
					} catch( exception& ex ) {
						ACS_LOG_WARNING( "Error checking inventory filetype ["<<invFiletype<<"] Vs rule filetype ["<<distRuleFiletype<<"]" );
					}

					bool fileclassMatch(false);

					// _ in fileclass has to be interpret as "any char"
					if( StringUtils::findNoCase(distRuleFileclass, "_") != string::npos) {
						RegExp re( StringUtils::transform(distRuleFileclass, "_", ".") );
						fileclassMatch = re.match( invFileclass );
					} else {
						fileclassMatch = ( invFileclass == distRuleFileclass );
					}

					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Filetype match ["<<invFiletype<<"]-["<<distRuleFiletype<<"]: "<< boolalpha<<filetypeMatch );
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Fileclass match ["<<distRuleFileclass<<"]-["<<invFileclass<<"]: "<< boolalpha<<fileclassMatch );

					//Check Proc name and version are in the configured list
					if ( filetypeMatch && fileclassMatch ) {
						const vector< Proc_Name_Vers > & procs = ruleContent.procs;
						bool procFound = std::find( procs.begin(), procs.end(), make_pair( invProcName, invProcVers ) ) != procs.end() ;

						ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Processor ["<<invProcName<<","<<invProcVers<<"] found in list: "<< boolalpha<<procFound );

						//According to configured action, promote or remove the distribution
						if ( procFound ) {

							if ( ruleContent.actionIsDistribute ) {

								ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Distribution promotion for file [" << ti.filename
										<< "] of policy " << ti.policy
										<< ". Matched item in promotion list for " << invProcName <<","<< invProcVers );

								toBePromoted.push_back(ti) ;
								sendInDistribution(toBePromoted,theConnection) ;
								toBePromoted.clear() ;

							} else {

								ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Distribution dropped for file [" << ti.filename << "] of policy " << ti.policy
										<< ". Matched item in drop list for " << invProcName <<","<< invProcVers );

								toBeDeleted.push_back( ti.id ) ;
								deleteFromTempDist(toBeDeleted, theConnection) ;
								toBeDeleted.clear() ;

							}

						} else {

							if ( ruleContent.actionIsDistribute ) {

								ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Distribution dropped for file [" << ti.filename << "] of policy " << ti.policy
										<< ". Matched item not found in promotion list for " << invProcName <<","<< invProcVers );

								toBeDeleted.push_back( ti.id ) ;
								deleteFromTempDist(toBeDeleted, theConnection) ;
								toBeDeleted.clear() ;

							} else {

								ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Distribution promotion for file [" << ti.filename
										<< "] of policy " << ti.policy
										<< ". Matched item not found in the drop list for " << invProcName <<","<< invProcVers );

								toBePromoted.push_back(ti) ;
								sendInDistribution(toBePromoted,theConnection) ;
								toBePromoted.clear() ;
							}

						}
					}
				}

			}


		} catch(exception &ex) {
			ACS_LOG_NOTIFY_EX( ex );
			appendSummary(TemporaryDistributionIdHasProblem, 1 ) ;
		}
	}

}

_ACS_END_NAMESPACE

