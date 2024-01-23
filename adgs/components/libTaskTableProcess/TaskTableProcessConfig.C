/*
	Copyright 1995-2020, Exprivia SPA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SPA - DADF;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SPA - DADF.

	$Prod: A.C.S. libTaskTableProcess$
        
*/ 

#include <TaskTableProcessConfig.h>

#include <Filterables.h>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(TaskTableProcessConfig)




TaskTableProcessConfig::TaskTableProcessConfig( const std::string & schedulerPath,
                        bool                copyInputData,
                        bool                cleanWorkingDir ) :
    _schedulerPath  ( schedulerPath  ),
    _copyInputData  ( copyInputData  ),
    _cleanWorkingDir( cleanWorkingDir)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}



TaskTableProcessConfig::~TaskTableProcessConfig() ACS_NOEXCEPT {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}


bool TaskTableProcessConfig::isCleanWorkingDir() const {
	return _cleanWorkingDir;
}

bool TaskTableProcessConfig::isCopyInputData() const {
	return _copyInputData;
}

const std::string& TaskTableProcessConfig::getSchedulerPath() const {
	return _schedulerPath;
}


_ACS_END_NAMESPACE



