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

	$Prod: A.C.S. libProc$

	$Id$

	$Author$

	$Log$
	Revision 2.3  2018/03/12 11:18:53  nicvac
	Error checking
	
	Revision 2.2  2018/03/07 12:11:04  nicvac
	QA check
	
	Revision 2.1  2018/03/07 10:41:37  nicvac
	Spawn Proc with logs. Executor of pool of Spawn Proc.
	
        
*/ 

#include <SpawnProcWithLogs.h>

#include <Filterables.h>

#include <File.h>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)


ACS_CLASS_DEFINE_DEBUG_LEVEL(SpawnProcWithLogs)


SpawnProcWithLogs::SpawnProcWithLogs(
		const string &pr, const std::string& tagName,
		const string &logFilenameStdOut, const string &logFilenameStdErr,
		bool resolvSymLink):

SpawnProc(pr, resolvSymLink),
_logFilenameStdOut(logFilenameStdOut),
_logFilenameStdErr(logFilenameStdErr),
_filePtrs()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	this->setTagName( (tagName.empty() ? File::getFileName( pr ) : tagName) ); // PRQA S 3380, 3385

	string defErrLog;
	string defOutLog;
	generateDefaultLogFilename( defOutLog, defErrLog );

	if ( _logFilenameStdOut.empty() ) {
		_logFilenameStdOut = defOutLog;
	}

	if ( _logFilenameStdErr.empty() ) {
		_logFilenameStdErr = defErrLog;
	}

}

SpawnProcWithLogs::~SpawnProcWithLogs() throw() {
// PRQA S 4631 L1
	try {	
		ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

		for ( vector<FILE*>::iterator it = _filePtrs.begin(); it!=_filePtrs.end(); ++it ) { // PRQA S 4238
			if (*it) {
				if ( 0 != fclose( *it ) ) {
					int eno = errno;
					ostringstream fds; fds<<*it;
					ACS_LOG_TRYCATCHNOTIFY_EX( exIOException( "Error closing file descriptor ["+fds.str()+"] for ["+this->getTagName()+"]", eno ) );
				}
			}
		}
	} catch (exception& ex) {
		ACS_LOG_NOTIFY_EX(ex);
	}
// PRQA L:L1
}

void SpawnProcWithLogs::generateDefaultLogFilename(string& outLog, string& errLog) const {

	ostringstream logStr;
	logStr<< File::getCurrentPath() <<"/Log_"<< this->getTagName();
	string log = File::tempname( logStr.str() ); // PRQA S 3050

	errLog = log + "_stdErr.log";
	outLog = log + "_stdOut.log";
}


void SpawnProcWithLogs::spawn(bool detached, unsigned int delay, bool autoKill) {

	string errLog = this->getLogFilenameStdErr();
	string outLog = this->getLogFilenameStdOut();
	string tagName = this->getTagName();

	int eno(0);

	FILE* logErr = fopen ( errLog.c_str(), "w"); // PRQA S 4412
	eno = errno ;
    ACS_COND_THROW(0 == logErr, exIOException("Cannot open file: [" + errLog + "]", eno)); // PRQA S 3081

	FILE* logOut = fopen ( outLog.c_str(), "w"); // PRQA S 4412
	eno = errno ;
	ACS_COND_THROW(0 == logOut, exIOException("Cannot open file: [" + outLog + "]", eno)); // PRQA S 3081

	int filenoErr = fileno( logErr );
	eno = errno ;
	ACS_COND_THROW( -1 == filenoErr, exIOException("Cannot get file descriptor for [" + errLog + "]", eno)); // PRQA S 3081

	int filenoOut = fileno( logOut );
	eno = errno ;
	ACS_COND_THROW( -1 == filenoOut, exIOException("Cannot get file descriptor for [" + outLog + "]", eno)); // PRQA S 3081

	this->setErrDescriptor( filenoErr );
	this->setOutDescriptor( filenoOut );

	_filePtrs.push_back( logErr );
	_filePtrs.push_back( logOut );

	ACS_LOG_PRIVINFO( "Process ["<<tagName<<"] log files: ["<<outLog<<", "<<errLog<<"]");

	SpawnProc::spawn( detached, delay, autoKill );

}





_ACS_END_NAMESPACE

