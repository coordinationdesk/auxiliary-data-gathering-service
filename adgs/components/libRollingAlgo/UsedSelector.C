// PRQA S 1050 EOF
/*
	Copyright 1995-2018, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Rolling Algo Library $
 */

#include <UsedSelector.h>

#include <RollingAlgoUtils.h>
#include <RollingPoliciesActions.h>

#include <dbOrdersInput.h>

#include <DateTime.h>
#include <File.h>
#include <rsPushNSpace.h>
#include <Filterables.h>



_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(UsedSelector)

// Costructor
UsedSelector::UsedSelector(const InvPolicy & policy) :
InvPolicyCommonSelector(policy),
_latency                (),
_usedTimes              ()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	// Set attributes from policy specification
	policy.get_latencyhours ( _latency   );

	// Get from parameter how many times the object has to be used at least
	{
		istringstream iss( _polParameters );
		rsResourceSet rset;
		iss >> rset ;
		rsPushNSpace nsp(rset,"UsedParameters") ;
		rset.getValue("_usedTimes", _usedTimes) ;
	}

}

UsedSelector::~UsedSelector() ACS_NOEXCEPT {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

void UsedSelector::selectInit( dbConnection &conn ) {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	RollingPoliciesActions*  actions = RollingPoliciesActions::instance(conn.getKey());
	DateTime dt;
	dt-= DateTime::JD50(_latency/24.0); // PRQA S 3050, 3011, 3081, 3084, 4400

	ACS_LOG_INFO("Policy id ["<<_polId<<"] Filetype ["<<_polFileType<<"]: Selecting Inventory items used "<<_usedTimes<<" times "
			"(or more) for processing. Action set to [" << actions->name(_polAction)<<"]" );

	ostringstream sqlCommand; sqlCommand<<"SELECT ";
	{

		sqlCommand<< "v.id_inv as "<< _invIdAttributeName <<" ";
		sqlCommand<< " FROM t_inventory v " ;

		sqlCommand<< "  WHERE v.filetype in (" << RollingAlgoUtils::instance()->getSqlRealFiletypes(conn, _polFileType) << ") ";

		if( !_polFileClass.empty() ) {
			sqlCommand<< " AND v.fileclass = " << db::toSQL(conn, _polFileClass);
		}

		if(StringUtils::equalsNoCase(actions->name(_polAction),"INVALIDATE"))	{	/////// commented to keep the invalidate object ---- 09/10/08 comment removed
			sqlCommand<< " AND validityflag = " <<db::toSQL(conn, true)  ;
		}
		sqlCommand
			<< " AND ( select count(*) FROM " << dbOrdersInput::getTable()
			<< " WHERE " << dbOrdersInput::getTable() <<".filename" << " = v.filename AND "
			<< dbOrdersInput::getTable() << ".fileversion"<< " = v.fileversion ) >= " <<_usedTimes;

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,
				"SQLSTRING  " << sqlCommand.str() << endl
				<<" _usedTimes  = " << _usedTimes
		);

	}

	_sqlSelections.clear();
	_sqlSelections.push_back( sqlCommand.str() );

}


_ACS_END_NAMESPACE
