/*
	Copyright 1995-2020, Exprivia SPA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SPA - DADF;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SPA - DADF.

	$Prod: A.C.S. libRollingAlgo$
        
*/ 

#include <InvPolicy.h>

#include <dbSmartPersistent.h>
#include <ConfigurationSingleton.h>
#include <File.h>
#include <Filterables.h>
#include <ftmpstream.h>

#include <RollingPoliciesActions.h>


using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(InvPolicy)

const __typeof__( *(InvPolicy::_fld_fetchrowslimit) ) InvPolicy::Unlimited_fetchrowslimit = 0;
const __typeof__( *(InvPolicy::_fld_idlenumitems  ) ) InvPolicy::Unlimited_idlenumitems   = std::numeric_limits< __typeof__(*_fld_idlenumitems) >::max();

const string ConfKey_FetchRowsLimit           = "RollingArchive.FetchRowsLimit";
const string ConfKey_PolicyIdle_NumItems      = "RollingArchive.PolicyIdle.NumItems";
const string ConfKey_PolicyIdle_IdleTimeHours = "RollingArchive.PolicyIdle.IdleTime";

template<typename T>
void loadConf(const string& confKey, shared_ptr< T > & field) {
	if ( not field ) {
		rsResourceSet& conf = ConfigurationSingleton::instance()->get();
		field.reset( new T() );
		conf.getValue( confKey, *field, false );
	}
};


InvPolicy::InvPolicy(const dbPersistent::IdType&  id, dbConnection &conn):
	_dbKey( conn.getKey() ),

	_fld_id            (),
	_fld_filetype      (),
	_fld_latencyhours  (),
	_fld_action        (),
	_fld_algorithm     (),
	_fld_priority      (),
	_fld_fileclass     (),
	_fld_storageid     (),
	_fld_fetchrowslimit(),
	_fld_idlenumitems  (),
	_fld_idlehours     (),
	_fld_triggertime   (),
	_fld_parameters    (),
	_fld_status        ()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	//Load the entry from db
	dbSmartPersistent dbentry("t_invrollingpolicies", conn);
	dbentry.load ( conn, id );

	//Load fields

#define loadField( _name_, _type_ ) {                                        \
		bool valueIsNull(true);                                              \
		_type_ value_ ## _type_ ;                                            \
		dbentry.get( #_name_ , value_ ## _type_, valueIsNull, conn );        \
	    if ( not valueIsNull ) {                                             \
	    	_fld_ ## _name_ .reset( new _type_ ( value_ ## _type_ ) );       \
	    }                                                                    \
}                                                                            \

	auto loadClob = [ & ] (const string& col_name, shared_ptr<string>& fieldClob) {

		if ( dbentry.isSet( col_name, conn) ) {
			// download and read clob. At the end delete temporary files
			string dwnName = File::tempname("/tmp/InvPolicy", ".clob"); //Temp filename
			FileGuard dwnNameGuard (dwnName);
			dbentry.downloadFile( col_name, dwnName, conn) ;
			{
				std::ifstream ifs( dwnName.c_str() );
				std::stringstream buffer;
				buffer << ifs.rdbuf();
				fieldClob .reset ( new string(buffer.str()) ) ;
			}
		}
	};


	loadField( id               , NumId      );
	loadField( filetype         , string     );
	loadField( latencyhours     , NumInteger );
	loadField( action           , NumId      );
	loadField( algorithm        , NumId      );
	loadField( priority         , NumInteger );
	loadField( fileclass        , string     );
	loadField( storageid        , NumId      );
	loadField( fetchrowslimit   , NumInteger );
	loadField( idlenumitems     , NumInteger );
	loadField( idlehours        , NumReal    );
	loadField( triggertime      , DateTime   );

	loadClob( "parameters", _fld_parameters );
	loadClob( "status"    , _fld_status     );

	//If set to null in DB, take values from configuration
	loadConf( ConfKey_FetchRowsLimit          , _fld_fetchrowslimit );
	loadConf( ConfKey_PolicyIdle_NumItems     , _fld_idlenumitems   );
	loadConf( ConfKey_PolicyIdle_IdleTimeHours, _fld_idlehours      );

	//Set defaults and fix misconfigurations:
	if ( 0 == *_fld_idlehours ) {
		*_fld_idlehours = 6.0;
	}

	// If not query limit and policy idle not set, set the idle policy to the upper limit.
	//  policy is completed when returned items are <= policy idle number.
	if ( 0 == *_fld_fetchrowslimit ) {
		*_fld_fetchrowslimit = Unlimited_fetchrowslimit;
	}

	if ( 0 == *_fld_idlenumitems ) {

		if ( *_fld_fetchrowslimit > 0 ) {
			*_fld_idlenumitems = *_fld_fetchrowslimit;
		} else {
			//All unlimited
			*_fld_idlenumitems = Unlimited_idlenumitems;
		}

	} else {

		// The policy idle number must be <= query limit (if set)
		if ( *_fld_fetchrowslimit > 0 && *_fld_idlenumitems > *_fld_fetchrowslimit ) {
			*_fld_idlenumitems = *_fld_fetchrowslimit ;
		}

	}


}

InvPolicy::~InvPolicy() ACS_NOEXCEPT {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

bool InvPolicy::check(string& report) const {

	report = "";

	//Check General constraints
	// - A remove action cannot be configured on a specific storage id
	RollingPoliciesActions*  actions = RollingPoliciesActions::instance( _dbKey );

	bool isStorageIdSet = _fld_storageid && *_fld_storageid != 0;
	bool isRemoveAction =
			StringUtils::equalsNoCase(actions->name( *_fld_action ),"REMOVE") ||
			StringUtils::equalsNoCase(actions->name( *_fld_action ),"REMOVEPARENT-REMOVECHILDREN");

	bool isKeepAction = StringUtils::equalsNoCase(actions->name( *_fld_action ),"KEEP");

	if ( isStorageIdSet && isRemoveAction ) {
		ostringstream msg; msg<<"Policy id ["<< *_fld_id <<"]: Misconfiguration detected. "
			"A Remove action cannot be performed on specific storage id. Remove action affects all storage manager. "
			"Set storage id to NULL or 0 in the rolling policy.";

		report = msg.str();
	}

	if ( isKeepAction ) {
		ostringstream msg; msg<<"Policy id ["<< *_fld_id <<"]: Misconfiguration detected. "
			"A Keep action cannot be used in the rolling policies. Inventory items can be preserved this ways: "
			"1. adding a manual rolling on a set of specific inventory items; Or"
			"2. configuring the [CustomPreservedItems] Configuration Section.";

		report = msg.str();
	}

	return report.empty();

}

void InvPolicy::add_fetchrowslimit_idlenumitems( size_t limitDelta ) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	this->add_fetchrowslimit( limitDelta );

	if ( _fld_idlenumitems && (*_fld_idlenumitems != Unlimited_idlenumitems ) ) {
		 *_fld_idlenumitems += limitDelta;
	}
}

void InvPolicy::add_fetchrowslimit( size_t limitDelta ) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	if ( _fld_fetchrowslimit && ( *_fld_fetchrowslimit != Unlimited_fetchrowslimit ) ) {
		*_fld_fetchrowslimit += limitDelta;
	}
}

bool  InvPolicy::updateStatusAndTriggerTime( const dbPersistent::IdType&  id,
		const bool & updateTriggerTime,
		const bool & updatePolicyStatus, const std::string & policyStatusToUpdate,
		DateTime & nextTimeTrigger,
		dbConnection &conn ) ACS_NOEXCEPT {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	bool done(false);
	try {

		{ // Transaction scope

			db::Transaction transaction(conn);
			transaction.begin_work("InvPolicy_updateStatusAndTriggerTime");

			dbSmartPersistent dbentry("t_invrollingpolicies", conn);
			dbentry.load ( conn, id );

			oftmpstream clobstream ("/tmp/InvPolicy",".clob") ;
			//Update the status clob
			if ( updatePolicyStatus ) {
				clobstream() << policyStatusToUpdate;
				dbentry.setFilePath("status", clobstream.path(), conn) ;
			}
			clobstream.close(); // flush the buffer

			//Update the triggertime
			if ( updateTriggerTime ) {
				DateTime now;
				DateTime::JD50 triggerTimeJd50( now.jd50() + *_fld_idlehours/24 );
				DateTime triggerTimeDt( triggerTimeJd50 );

				nextTimeTrigger = triggerTimeDt;

				dbentry.set( "triggertime", triggerTimeDt, conn );
			}

			dbentry.update( conn );

			transaction.commit();
		}
		done = true;

	} catch (exception& ex) {

		ostringstream msg;
		msg << "Error updating Rolling policy ["<<id<<"]. This will cause the re-execution of the policy, without considering the policy idle time";
		ACS_LOG_WARNING( msg.str() );
        ACS_LOG_NOTIFY_EX(ex) ;
	}

	return done;

}


_ACS_END_NAMESPACE

