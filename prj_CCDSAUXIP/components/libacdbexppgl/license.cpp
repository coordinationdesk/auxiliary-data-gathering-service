
#include "license.hpp"
#include "ifxerror.hpp"

#ifdef __TRIAL__

CLicenseManager::CLicenseManager ()
{
	time(&_finishTime);
	_finishTime += 2*60*60;
}

void CLicenseManager::checkLicense ()
{
	time_t nowTime;
	if (time(&nowTime) > _instance._finishTime)
	{
		throw CInformixDriverError(
			"Luxena dbExpress driver for Informix. "
			"Trial runtime limit has been reached, restart your application.");
	}
}

CLicenseManager CLicenseManager::_instance;

#endif
