
/*
    Copyright 1995-2013, Advanced Computer Systems , Inc.
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.acsys.it

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Exception Library $

    $Id$

    $Author$

    $Log$
    Revision 5.2  2013/06/11 08:44:25  marpas
    macros deprecation improved

    Revision 5.1  2013/06/10 16:13:59  marpas
    using and enforcing new standards

    Revision 5.0  2013/06/04 15:34:59  marpas
    introducing new log macros, debug macros
    work in progress

    Revision 4.2  2012/03/29 15:37:28  marpas
    coverage integration

    Revision 4.1  2012/01/31 14:13:04  marpas
    getting rid of boost and compilation warning reduced

    Revision 4.0  2011/10/18 11:21:58  marpas
    new statistics

    Revision 3.1  2011/03/03 10:54:27  marpas
    porting to GXX 4.4.x

    Revision 3.0  2009/10/08 10:48:07  marpas
    Statistics class added
    rules application improved

    Revision 2.9  2009/07/14 12:40:35  matteo.airoldi
    Added include unistd to compile with RHEL3

    Revision 2.8  2008/01/11 14:24:02  marpas
    title implmented for elConfirm

    Revision 2.7  2008/01/09 09:50:45  enrcar
    EC:: elConfirm was unable to run in Linux. Fixed.

    Revision 2.6  2007/11/15 11:22:13  lucia.galli
    ENRCAR:: elConfirm method added

    Revision 2.5  2006/11/27 10:07:59  stefano.vignoli
    The anchor of Error.h is forced to prevent this file to be loaded

    Revision 2.4  2006/11/16 09:44:24  enrcar
    Anchor was missing. fixed.

    Revision 2.3  2006/10/11 16:12:18  enrcar
    EC:: missing include added

    Revision 2.2  2006/10/11 13:11:28  marpas
    more support added to ErrorLegacy, test added

    Revision 2.1  2006/10/11 10:02:42  marpas
    ErrorLegacy.h include file provided


*/


/*
	The anchor of Error.h is FORCED, TO PREVENT THE FILE TO BE LOADED
*/
#define __LIBERROR_H__



#ifndef __LIBERRORLEGACY_H__
#define __LIBERRORLEGACY_H__



#ifndef sgi
#include <Filterables.h>
#include <exMacros.h>
#include <stdarg.h>
#include <cstring>
#include <unistd.h>
#endif

#ifdef sgi
elDEBUG_LEVEL
#else

#define elBGN_DEBUG(v) if (asInt(ErrorLegacy::exCDebugLevel) >= (v)) { 
#define elEND_DEBUG  ; }
#define elINIT_DEBUG(v) { if (asInt(ErrorLegacy::exCDebugLevel) == 0) { const char *exDbgGE = getenv(# v); setInt(exCDebugLevel,acs::getDebugValueFromEnv(exDbgGE)) ; } }

#define elDEBUG_LEVEL
#define elMAX_MESSAGE_LEN 4096
#define elDebugLevel ErrorLegacy::exCDebugLevel

#define elSetTime() { elSetTimeMessage(); elSetTimeError() ;}

namespace ErrorLegacy {
	using namespace acs ;
	using namespace std ;

/*! \namespace ErrorLegacy
	Contains all needed macros and methods to substitute libError
	
	Not all the libError functions are yet supported. Please notify M.Pastori/E.Carta any special needs.(prepare money.)
*/

	
	static int exCDebugLevel = 0 ;
	
	const int elSH_OFFSET = 3 ;	/* used to get which button in notifier */
	const int elBUTTWIDTH = 85 ;	/* notifier width of buttons */
	const int elCHARWIDTH = 7 ;	/* notifier character font width (approx) */
	const int elMAXBTNS = 7 ;	/* notifier character font width (approx) */
	const int elMAX_ERROR_LEN = 1024 ;	/* notifier character font width (approx) */

	static char elAppName[elMAX_ERROR_LEN] = "" ;
	static int elPid = 0 ;


	typedef enum {
		elError      	 	    = 1 << 0,
		elWarning    	 	    = 1 << 1,
		elInfo	     	 	    = 1 << 2,
		elErrorBkg   	 	    = 1 << 3,
		elWarningBkg 	 	    = 1 << 4,
		elInfoBkg    	 	    = 1 << 5,
		elRecoveredError 		= 1 << 6,
		elRecoveredErrorBkg 	= 1 << 7,
		elRecoveredWarning 		= 1 << 8,
		elRecoveredWarningBkg	= 1 << 9
	} elNotifyMode ;

	inline static void elNotifyMessage(const char *fmt,	...	) {
		char message[elMAX_MESSAGE_LEN] ;
		va_list args ;

		va_start(args,fmt) ;

		if (args) {
			(void)vsprintf(message,fmt,args) ;
			ACS_LOG_INFO(message) ;
		}
		else
			fprintf(stderr,"\alibError: integration failure at %s(%d)\n",__FILE__,__LINE__) ;
		va_end(args) ;

		return ;
	}


	#define elNotifyError(n) (ErrorLegacy::notifyErrorMsg((n), __FILE__,__LINE__,#n, ErrorLegacy::elError) )
	#define elNotifyWarning(n) (ErrorLegacy::notifyErrorMsg((n), __FILE__,__LINE__,#n, ErrorLegacy::elWarning) )
	
	inline static int notifyErrorMsg(int error, const std::string file, int line, const std::string mnemo, elNotifyMode mode)
	{
		exFError fe ;
		exFError fw ;
		exFMessage *fmp = &fe;
		switch(mode) {
		case elWarning:
		case elWarningBkg:
		case elRecoveredWarning:
			fmp = &fw ;
		default:
			break ;
		}
		exFMessage &fm = *fmp ;
		fm << "'" << file << "':" << line << " *** " ;
		switch(mode) {
		case elWarning:
		case elWarningBkg:
		case elRecoveredWarning:
			fm << "Warning" ;
			break ;
		
		case elError:
		case elErrorBkg:
		case elRecoveredErrorBkg:
			fm << "Error" ;
			break ;
		default:
			fm << "Unknown Mode" ;
			break ;
		}
		
		fm << " [" << error << "]" ;
		ostringstream ascerr ;
		ascerr << error ;
		if (mnemo != ascerr.str())
			fm << " (" << mnemo << ")" ;
		
		exostream::getDefaultOutLogger() << fm ;
		return error ;
		   
	}






/*
 * Description:
 * 	Displayes a message using the xconfirm command and wait for the response
 * 	returns 0 for the first button (default button), 1 for the second and so on.
 * 	The lists of button is terminated when a NULL is encounterd
 */
inline int elConfirm(
    const char	*message, 
    const char	*defbutt, 
    ...
)
{
    return -1 ;
}




	inline static const char *elGetLogfile(void ) { return "" ; }

	inline static int elSetLogfile(const char *logname, int ) 
	{
		return 0 ;
	}


	inline void elSetAppName(const char *appname) { (void)strcpy(elAppName,appname ? appname : "") ; return ; }


	/*
	 * Description:
	 * 	Gets the pid flag value for output.
	 */
	inline int elGetPid(void) { return  elPid ; }


	/*
	 * Description:
	 * 	Sets the pid output in the messages. It will be reported at each notification.
	 */
	inline void elSetPid(void) { elPid++ ; }
	
	inline static FILE *elGetLogfilePtr(void )  { return 0 ; }
	inline static FILE *elGetOutputStream(void ) { return 0 ; }
	inline static void elSetErrorCallback(void (*)(const char *)) {}
	inline static void elSetErrorCallback(void (*)(char *)) {}
	inline static int elSetOutputStream(FILE * )   { return 0 ; }
	inline static void elSetInfoCallback(void (*)(const char *)) {}
	inline static void elSetInfoCallback(void (*)(char *)) {}
	inline static void elUnsetInfoCallback(void ) {}
	inline static void elSetTimeError(void ) {}
	inline static void elSetTimeMessage(void ) {}
	inline static void elUnsetTimeError(void ) {}
	inline static void elLogMessage(const char *, ...) {}

} // namespace 

using namespace ErrorLegacy ;

#endif


#endif /* !__LIBERRORLEGACY_H__ */
