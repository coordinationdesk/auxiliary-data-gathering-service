
#ifdef __TRIAL__

#include <time.h>
#include <ctype.h>

class CLicenseManager
{
public:
	static void checkLicense ();
private:
	CLicenseManager ();
	static CLicenseManager _instance;
	time_t _finishTime;
};

#else

class CLicenseManager
{
public:
	static void checkLicense () {}
};

#endif
