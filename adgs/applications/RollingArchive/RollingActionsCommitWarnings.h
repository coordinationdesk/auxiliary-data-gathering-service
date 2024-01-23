/*
	Copyright 1995-2020, Exprivia SPA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SPA - DADF;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SPA - DADF.

	$Prod: A.C.S. RollingArchive$
        
*/

#ifndef _RollingActionsCommitWarnings_H_
#define _RollingActionsCommitWarnings_H_

#include <RollingPolicyOutput.h>

#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

/**
* \brief
* Emit warnings for items going to be rolled
* - that failed a migration to asynch storage
* - that failed an export
* Note: items involved by on-going operations (migration and exporting) are filtered out at rolling selection time.
*  here, at commit time, emit warnings for operations (migration and exporting) that
*  went wrong (not filtered out at rolling selection time).
**/

class RollingActionsCommitWarnings  { 
public:

	/** Default Class constructor */
	explicit RollingActionsCommitWarnings();
	/** Destructor */
	virtual ~RollingActionsCommitWarnings() ACS_NOEXCEPT ;

	ACS_DECLARE_NO_COPY_CLASS(RollingActionsCommitWarnings);

public:
	/** Log configured warnings on items going to be rolled */
	void logWarnings( std::deque< RollingPolicyOutput::Sptr >& actionsQueue );

private:

	/** Performed sql query and log warnings */
	void queryAndWarning( dbConnection& conn, const std::ostringstream& ossSql, const std::string& message );

	/** Log warning configuration status */
	struct ConfWarning {
		ConfWarning(): enabled(false), migration(false), exporting(false) {}
		ConfWarning(bool en, bool mi, bool ex): enabled(en), migration(mi), exporting(ex) {}
		bool operator==(const ConfWarning & p) const {
			return (enabled == p.enabled && migration==p.migration && exporting==p.exporting);
		}

		bool enabled;
		bool migration;
		bool exporting;
	};

	/** Static to detectd configurarion change */
	static ConfWarning _confWarning;

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(RollingActionsCommitWarnings)

};

_ACS_END_NAMESPACE

#endif //_RollingActionsCommitWarnings_H_

