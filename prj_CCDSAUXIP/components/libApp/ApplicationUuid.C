// PRQA S 1050 EOF
/*
	Copyright 2020, Exprivia SPA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it


	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	A.C.S. libApp
*/

#include <ApplicationUuid.h>
#include <exStream.h>
#include <Filterables.h>
#include <Uuid.h>

#include <sstream>
#include <sys/types.h> // PRQA S 1013
#include <unistd.h>



_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(ApplicationUuid)
#define MAXINSTANCES 1000000

ApplicationUuid::ApplicationUuid(const string& lockName) ACS_NOEXCEPT :
	_baseLockName(),
	_actualLockName(),
	_lock(""),
	_uuid()
{
	ostringstream os ;
	os << acs::base::defaultPersistencePath() << "/" << lockName;
	_baseLockName = os.str() ;
}

ApplicationUuid::~ApplicationUuid() ACS_NOEXCEPT
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	if (!_actualLockName.empty()) {
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, _actualLockName << " not empty...trying to unlock!") ;
		try {
			_lock.unlock();
		}
		catch(exception &x) {
	        	ACS_LOG_NOTIFY_EX(x) ;
        	}
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "actualLockName empty!") ;
	}
}
std::string ApplicationUuid::getUuid()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
	if (_uuid.empty())
	{
		lockAndCreate();
	}
	return _uuid;
}

const string &ApplicationUuid::lockAndCreate(bool exception) // PRQA S 4020
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;

	ACS_COND_THROW(_baseLockName.empty(), exIllegalValueException("ApplicationUuid cannot have an empty string as file")) ;           // PRQA S 3081
	// infinite loop
	for (size_t actualInstance =0; actualInstance < MAXINSTANCES; ++actualInstance)
	{
		ostringstream os;
		os << _baseLockName << "_"<< actualInstance;
		_actualLockName = os.str();
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "trying to lock the file " << _actualLockName) ;

		_lock.setLock(_actualLockName, true) ;

		if (_lock.lock(50)) { // 50 [msec] to lock the file else next       // PRQA S 4400
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "the lock was succesful") ;
			_lock.autoRemoveLockFile(true);
			if (!File::exists(_actualLockName))
			{
				// generate and write uuid
				generateNewUuid();
				writeUuid();
				try {
					// set file permission
					File::chmod(_actualLockName, base::defaultUuidPermissions() ) ;                     // PRQA S 4400
				}
				catch (std::exception &e)
				{
					ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "exception caught chmod...ding: " << e) ;
				}
			}
			else
			{
				// just read the uuid from file
				_uuid = readUuid();
			}
			return _actualLockName;
		}
	}
	ACS_THROW(exIOException("Error getting UUID."));
}

void ApplicationUuid::writeUuid()    // PRQA S 4211
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
	ofstream uuid(_actualLockName.c_str()) ;
	uuid <<  _uuid ;
	uuid.close();
}
string ApplicationUuid::readUuid()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
	ifstream uuidF(_actualLockName);
	std::stringstream strStream;
	strStream << uuidF.rdbuf(); //read the file
	uuidF.close();
	return strStream.str();
}
void ApplicationUuid::generateNewUuid()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
	_uuid = Uuid::generateUuid();
};

_ACS_END_NAMESPACE

