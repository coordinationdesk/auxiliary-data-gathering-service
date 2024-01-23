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

#ifndef _InvPolicy_H_
#define _InvPolicy_H_

#include <exException.h>

#include <dbPersistent.h>
#include <DateTime.h>

_ACS_BEGIN_NAMESPACE(acs)

/**
* \brief
* Rolling Policy. An instance is related to an entry in t_invrollingpolicies.
* Fields set to null are valued from configuration space.
**/

class InvPolicy {
public:

	/** Class constructor */
	InvPolicy(const dbPersistent::IdType&  id, dbConnection &);
	/** Class destructor */
	virtual ~InvPolicy() ACS_NOEXCEPT ;

	ACS_DECLARE_NO_COPY_CLASS(InvPolicy);
	
public:

	/** Type definition */
	typedef dbPersistent::IdType NumId;
	typedef int64_t              NumInteger;
	typedef double               NumReal;

	/** Policy self check - detect misconfigurations */
	bool check(std::string& report) const;

	/** Extend the fetch rows limit coming from db */
	void add_fetchrowslimit_idlenumitems( size_t limitDelta );
	void add_fetchrowslimit( size_t limitDelta );

	/** Getter methods */
	const std::string& getDbKey() const { return _dbKey; }

	/**  Get policy fields.
	 * return the "is null" state of the field */
	bool  get_id             ( NumId       & v ) const { return getField( _fld_id            , v); }
	bool  get_filetype       ( std::string & v ) const { return getField( _fld_filetype      , v); }
	bool  get_latencyhours   ( NumInteger  & v ) const { return getField( _fld_latencyhours  , v); }
	bool  get_action         ( NumId       & v ) const { return getField( _fld_action        , v); }
	bool  get_algorithm      ( NumId       & v ) const { return getField( _fld_algorithm     , v); }
	bool  get_priority       ( NumInteger  & v ) const { return getField( _fld_priority      , v); }
	bool  get_fileclass      ( std::string & v ) const { return getField( _fld_fileclass     , v); }
	bool  get_storageid      ( NumId       & v ) const { return getField( _fld_storageid     , v); }
	bool  get_fetchrowslimit ( NumInteger  & v ) const { return getField( _fld_fetchrowslimit, v); }
	bool  get_idlenumitems   ( NumInteger  & v ) const { return getField( _fld_idlenumitems  , v); }
	bool  get_idlehours      ( NumReal     & v ) const { return getField( _fld_idlehours     , v); }
	bool  get_triggertime    ( DateTime    & v ) const { return getField( _fld_triggertime   , v); }
	bool  get_parameters     ( std::string & v ) const { return getField( _fld_parameters    , v); }
	bool  get_status         ( std::string & v ) const { return getField( _fld_status        , v); }

	/** Update (optionally) Status and the Trigger time.
	 * Return the next trigger time */
	bool  updateStatusAndTriggerTime( const dbPersistent::IdType&  id,
			const bool & updateTriggerTime,
			const bool & updatePolicyStatus, const std::string & policyStatusToUpdate,
			DateTime & nextTimeTrigger,
			dbConnection &conn ) ACS_NOEXCEPT ;


private:

	/** DB key */
	std::string _dbKey;

	/** Fields. can be null */
	std::shared_ptr< NumId       >  _fld_id             ;
	std::shared_ptr< std::string >  _fld_filetype       ;
	std::shared_ptr< NumInteger  >  _fld_latencyhours   ;
	std::shared_ptr< NumId       >  _fld_action         ;
	std::shared_ptr< NumId       >  _fld_algorithm      ;
	std::shared_ptr< NumInteger  >  _fld_priority       ;
	std::shared_ptr< std::string >  _fld_fileclass      ;
	std::shared_ptr< NumId       >  _fld_storageid      ;
	std::shared_ptr< NumInteger  >  _fld_fetchrowslimit ;
	std::shared_ptr< NumInteger  >  _fld_idlenumitems   ;
	std::shared_ptr< NumReal     >  _fld_idlehours      ;
	std::shared_ptr< DateTime    >  _fld_triggertime    ;
	std::shared_ptr< std::string >  _fld_parameters     ;
	std::shared_ptr< std::string >  _fld_status         ;

public:
	/** Const definition */
	static const __typeof__( *_fld_fetchrowslimit ) Unlimited_fetchrowslimit;
	static const __typeof__( *_fld_idlenumitems   ) Unlimited_idlenumitems;

private:

	/** get fields */
	template < typename T >
	bool getField( const std::shared_ptr< T > & field, T& value ) const;

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(InvPolicy)

};


template < typename T >
bool InvPolicy::getField( const std::shared_ptr< T > & field, T& value ) const {
	bool isNull(true);
	if ( field ) {
		isNull = false;
		value = *field;
	}
	return isNull;
}

_ACS_END_NAMESPACE

#endif //_InvPolicy_H_

