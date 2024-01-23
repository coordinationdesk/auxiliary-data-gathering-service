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

#ifndef _TaskTableProcessConfig_H_
#define _TaskTableProcessConfig_H_

#include <exException.h>

#include <memory>

_ACS_BEGIN_NAMESPACE(acs)


/**
* \brief
* The configuration for a Task Table Processor Instance.
**/

class TaskTableProcessConfig  { 
public:

	/** Default Class constructor */
	TaskTableProcessConfig( const std::string & schedulerPath,
	                        bool                copyInputData,
	                        bool                cleanWorkingDir );
	/** Destructor */
	virtual ~TaskTableProcessConfig() ACS_NOEXCEPT ;

	/** Shared Pointer */
	typedef std::shared_ptr< TaskTableProcessConfig > Sptr;

	ACS_DECLARE_NO_COPY_CLASS(TaskTableProcessConfig);

public:
	/** Getters */
	bool isCleanWorkingDir() const;
	bool isCopyInputData() const;
	const std::string& getSchedulerPath() const;

private:
    /** Scheduler executable path */
    std::string _schedulerPath;
    /** false: input data are moved inside working dir; set true for debug */
    bool _copyInputData;
    /** true: delete working dir after processing. false: used for debug */
    bool _cleanWorkingDir;



private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(TaskTableProcessConfig)

};

_ACS_END_NAMESPACE

#endif //_TaskTableProcessConfig_H_

