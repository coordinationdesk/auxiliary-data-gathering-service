/*
	Copyright 1995-2018, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libProc$

	$Id$

	$Author$

	$Log$
	Revision 2.1  2018/03/07 10:41:37  nicvac
	Spawn Proc with logs. Executor of pool of Spawn Proc.
	
        
*/

#ifndef _SpawnProcWithLogs_H_
#define _SpawnProcWithLogs_H_

#include <exException.h>
#include <Singleton.hpp>

#include <SpawnProc.h>

_ACS_BEGIN_NAMESPACE(acs)

using pattern::Singleton;

/**
* \brief
* Spawn Proc Utils - Higher level utilities
**/

class SpawnProcWithLogs: public SpawnProc {
public:
	/** Define proc executable with log filenames (optional) */
	explicit SpawnProcWithLogs(
			const std::string& pr, const std::string& tagName="",
			const std::string& logFilenameStdOut="", const std::string& logFilenameStdErr="",
			bool resolvSymLink = true) ;

	virtual ~SpawnProcWithLogs() throw() ;

	/** Get std err log filename */
	const std::string& getLogFilenameStdErr() const {
		return _logFilenameStdErr;
	}

	/** Get std out log filename */
	const std::string& getLogFilenameStdOut() const {
		return _logFilenameStdOut;
	}

	/** spawn Specialization */
	virtual void spawn(bool detached = false, unsigned int delay = 0, bool autoKill = true) ;

private:
	/** Filename for standard out */
	std::string _logFilenameStdOut;
	/** Filename for standard err */
	std::string _logFilenameStdErr;

	/** log files */
	std::vector<FILE*> _filePtrs;

private:  // do not copy
	SpawnProcWithLogs(const SpawnProcWithLogs & );
	SpawnProcWithLogs &operator=(const SpawnProcWithLogs &);

private:
	/** Generate default log filename for std out and std err */
	void generateDefaultLogFilename(std::string& outLog, std::string& errLog) const;

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(SpawnProcWithLogs)

};

_ACS_END_NAMESPACE

#endif //_SpawnProcWithLogs_H_

