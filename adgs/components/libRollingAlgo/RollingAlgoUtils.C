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

	$Prod: A.C.S. libRollingAlgo$

	$Id$

	$Author$

	$Log$
	Revision 1.9  2018/01/17 15:57:34  marpas
	distributionqueue and migration stati are now enumerated and not longer obtained with a NOT IN
	
	Revision 1.8  2018/01/17 15:55:29  marpas
	distributionqueue and migration stati are now enumerated and not longer obtained with a NOT IN
	
	Revision 1.7  2018/01/17 14:14:01  marpas
	implementing getSqlRealFiletypes in terms of dbFileTypeRecognition::getRealFTypesList - no longer a sub query but a list, even only one, of quoted string
	qa warning fixed
	
	Revision 1.6  2018/01/17 11:39:56  marpas
	using dbFileTypeRecognition to check if filetype is real or virtual (needs libDbCryosat Rev_7_8 or newer)
	
	Revision 1.5  2018/01/16 15:43:38  marpas
	filetype selection depends otpimization based on (real/virtual) ftype
	
	Revision 1.4  2017/12/05 16:32:16  nicvac
	S2PDGS-1856: Query optimization.
	
	Revision 1.3  2017/12/04 18:08:16  nicvac
	S2PDGS-1856: Policy select optimization query for performances.
	
	Revision 1.2  2015/12/09 17:07:29  nicvac
	S3MPM-650: Virtual filetype handling.
	
	Revision 1.1  2015/02/26 16:44:07  nicvac
	RollingAlgoUtils.h
	
        
*/ 

#include <RollingAlgoUtils.h>
#include <dbFileTypeRecognition.h> 

#include <Filterables.h>

#include <dbConnectionPool.h>
#include <dbQuery.h>
#include <dbRow.h>
#include <dbSet.h>

#include <ConfigurationSingleton.h>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(RollingAlgoUtils)



RollingAlgoUtils::RollingAlgoUtils():
    Singleton <RollingAlgoUtils>(),
    _inventoryPreserveQueries(),
    _invIdAttributeName("id_inv")
{
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Id: " << Id);
}


void RollingAlgoUtils::init(std::string const& connKey) {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	/* Inventory Items to be preserved: Cf. http://jira.acsys.it/browse/S2PDGS-897 */

	_inventoryPreserveQueries.clear();

	{
        const char *disQstati[] = {"DOWNLOADING", "PRECONDITIONCHECK", "ELIGIBLE", "TRANSIENT", "WAITINGACK", "ABORTED", "DISTRIBUTION", "RECOVERABLEERROR", "SUSPENDED" } ;

		//Preserve Items to be distribute
		ostringstream ossSql;
		ossSql <<
			"select	inv.id_inv as "<< _invIdAttributeName <<"                                                              \n"
			"from (                                                                                                        \n"
			"      t_distributionitems  disIt   inner join                                                                 \n"
			"      t_inventory          inv     on (inv.filename = disIt.filename AND inv.fileversion = disIt.fileversion) \n"
			")                                                                                                             \n"
			"left join t_distributionqueue disQ on disIt.packageid = disQ.packageid                                        \n"
			"where                                                                                                         \n"
			"   disIt.packageid is Null                                                                                    \n"
			"   OR                                                                                                         \n"
			"   disQ.status IN (" ;
        for (size_t i = 0; i < sizeof(disQstati)/sizeof(disQstati[0]); ++i) {
            if (i>0) { 
                ossSql << ',' ;
            }
            ossSql << db::toSQL(connKey, string(disQstati[i])) ;
        }
        ossSql << ')';
        // disQ.status IN (select status from t_stati where status NOT IN ("<<db::toSQL(connKey, "COMPLETED")<<", "<<db::toSQL(connKey, "ERROR")<<") )";

		_inventoryPreserveQueries.push_back( make_pair( "invToPreserveDistribution", ossSql.str() ) );
	}

	{
        const char *smqstati[] = { "STANDBY", "ELIGIBLE", "SUBMITTED", "WAITINGACK", "ACKNOWLEDGED", "UPLOADING", "PREPARING"} ;
    
		//Preserve Items to be Migrated (storage)
		ostringstream ossSql;
		ossSql <<
			"select smAct.inv_id as "<< _invIdAttributeName <<"                               \n"
			"from                                                                             \n"
			"	t_smactions smAct inner join t_smqueue smQ on smQ.packageid = smAct.packageid \n"
			"where                                                                            \n"
			"	smQ.status IN (" ;
        for (size_t i = 0; i < sizeof(smqstati)/sizeof(smqstati[0]); ++i) {
            if (i>0) { 
                ossSql << ',' ;
            }
            ossSql << db::toSQL(connKey, string(smqstati[i])) ;
        }
        ossSql << ')' ;

        // smQ.status IN (select status from t_stati where status NOT IN ("<<db::toSQL(connKey, "COMPLETED")<<", "<<db::toSQL(connKey, "ERROR")<<") )";

		_inventoryPreserveQueries.push_back( make_pair( "invToPreserveStorage", ossSql.str() ) );
	}

	//Load custom preservation rules from configuration
	rsResourceSet & conf =  ConfigurationSingleton::instance()->get();
	if ( conf.hasKey("RollingArchive.ExpertSection.CustomPreservedItems.Rule[]") ) {
		vector<string> customQueries;
		conf.getArray("RollingArchive.ExpertSection.CustomPreservedItems.Rule", customQueries);
		int i(0);
		for ( auto customQuery : customQueries ) {
			ostringstream name; name<<"invToPreserve_config_custom"<<++i;
			_inventoryPreserveQueries.push_back( make_pair(name.str(), customQuery) );
		}
	}

	for ( auto rule : _inventoryPreserveQueries ) {
		ACS_LOG_PRIVINFO( "Rolling rule preserving Items: ["<< rule.first <<"]" );
	}

}

const vector<RollingAlgoUtils::Pair_QueryName_SqlQuery>& RollingAlgoUtils::getInventoryItemsToBePreservedQueries( std::string& invIdAttributeName ) const {

	invIdAttributeName = _invIdAttributeName;

	return _inventoryPreserveQueries;
}


string RollingAlgoUtils::getSqlInventoryItemsToBePreserved(string const& connKey) const {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	// Union of all selects defined in  _inventoryPreserveQueries

	ostringstream ossSql;

	for ( vector< Pair_QueryName_SqlQuery >::const_iterator it=_inventoryPreserveQueries.begin(); it!=_inventoryPreserveQueries.end(); ++it ) {

		//const string& queryName = it->first;
		const string& querySqlQuery = it->second;

		string unionStr = ( it == _inventoryPreserveQueries.begin() )? "" : "UNION"; // PRQA S 3384

		ossSql <<"\n"<< unionStr <<"\n"<< querySqlQuery;

	}

	return ossSql.str();

}

void RollingAlgoUtils::getInventoryItemsToBePreserved( dbConnection &conn, set< dbPersistent::IdType >& invIdsToPreserve ) const {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	ostringstream ossSql( this->getSqlInventoryItemsToBePreserved(conn.getKey()) );

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, ": Get inventory items preservation list ["<<ossSql.str()<<"]");

	try {
		SQLString sqlString(conn.getKey(), ossSql.str() );
		dbQuery query(conn, sqlString, "getSqlInventoryItemsToBePreserved");

		dbSet set; dbRow row(conn.getKey());
		query.execForSet( set );
		while ( set.getNextRow(row) ) {
			dbPersistent::IdType invId = 0 ;
			row.column(0, invId);
			invIdsToPreserve.insert( invId );
			ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Preserving Inventory id ["<<invId<<"]");
		}
	} catch( exception& ex ) {
		ACS_LOG_WARNING("Problem occurred getting inventory items to be preserved. Sql was ["<<ossSql.str()<<"]");
		throw;
	}

}

std::string RollingAlgoUtils::getSqlRealFiletypes(dbConnection &conn, string const& filetype) const {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	//Virtual filetype <==> is in t_filetypescomps as parent
    dbFileTypeRecognition ftr;

    vector<string> rftypes = ftr.getRealFTypesList(filetype, conn) ;
	ostringstream ossSql;

    size_t rft_size = rftypes.size(); 
    for (size_t i=0; i < rft_size; ++i) {
        if (i > 0) {
            ossSql << ',' ;
        }
        ossSql << db::toSQL(conn.getKey(), rftypes[i]) ;
    }
	return ossSql.str();

/*
	ossSql <<
		" select ft_c.filetype \
		from \
		t_filetypescomps ft_h \
		join \
		t_filetypes ft_p \
		on ft_h.pfiletype = ft_p.id_filetype \
		join \
		t_filetypes ft_c \
		on ft_h.cfiletype = ft_c.id_filetype \
		where \
		ft_p.filetype = "<<db::toSQL(conn.getKey(), filetype)<<" \
		\
		UNION \
		\
		select ft.filetype \
		from  \
		t_filetypes ft \
		where ft.filetype = "<<db::toSQL(conn.getKey(), filetype)<<" \
		and ft.id_filetype not in (select pfiletype from t_filetypescomps) ";
*/

}


_ACS_END_NAMESPACE

