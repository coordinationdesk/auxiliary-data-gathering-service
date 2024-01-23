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

	$Prod: A.C.S. Exception Library $

	$Id$

	$Author$

	$Log$
	Revision 5.17  2018/09/12 10:05:39  marpas
	exception's stack trace management revived into streams.
	tested
	
	Revision 5.16  2018/09/04 16:09:59  marpas
	ACS_CLASS_DEFINE_LINKDEBUG_LEVEL macro added
	
	Revision 5.15  2015/03/12 12:43:15  marpas
	introducing exFWStatus and exFEStatus filterables along their macros ACS_LOG_WSTATUS and ACS_LOG_ESTATUS respectively.
	The sysinfo log handler uses LOG_WARNING and LOG_ERR prio for them.
	
	Revision 5.14  2014/05/15 13:27:35  marfav
	exFProgress filterable added
	
	Revision 5.13  2014/02/07 18:03:07  marpas
	introducing ACS_THROW and ACS_COND_THROW macros to substitute throwException and throwConditionedException
	documentation improved and tests aligned
	
	Revision 5.12  2013/11/19 17:52:20  marpas
	using  exostream::getDefaultErrLogger()
	
	Revision 5.11  2013/11/18 17:03:07  marpas
	exFStatus filterable added along its macro
	
	Revision 5.10  2013/10/03 10:57:39  marpas
	doc fixed
	
	Revision 5.9  2013/06/17 09:04:00  marpas
	 define SILENT_MACROS_DEPRECATED_WARNING to silent deprecated macros
	
	Revision 5.8  2013/06/11 09:30:21  marpas
	func macros added
	
	Revision 5.7  2013/06/11 08:44:25  marpas
	macros deprecation improved
	
	Revision 5.6  2013/06/10 16:13:59  marpas
	using and enforcing new standards
	
	Revision 5.5  2013/06/07 13:24:42  marpas
	minor changes
	
	Revision 5.4  2013/06/06 12:54:03  marpas
	ACS_VERYLOW_VERB changed into ACS_VLO_VERB
	
	Revision 5.3  2013/06/06 08:30:28  marpas
	template support to real time debugging added
	
	Revision 5.2  2013/06/05 15:54:48  marpas
	new ACS_CLASS_XXX and ACS_LOG_XXX macros introduced
	all new signature macros introduced and some rewritten in terms of new ones
	
	Revision 5.1  2013/06/04 17:23:46  marpas
	introducing ne RTDebuggable template
	
	Revision 5.0  2013/06/04 15:35:02  marpas
	introducing new log macros, debug macros
	work in progress
	
	Revision 4.11  2013/04/29 15:39:01  marpas
	exFPrivateInfo filterable concept introduced and tested
	coding best practices applied
	
	Revision 4.10  2013/04/15 08:45:28  enrcar
	EC:: Added macro exDebugShortSignatureFile
	
	Revision 4.9  2013/04/09 07:48:31  marpas
	doc fix
	
	Revision 4.8  2013/04/04 09:41:34  enrcar
	ENRCAR:: added a compact debug, with also threadid
	
	Revision 4.7  2013/04/04 09:23:51  enrcar
	ENRCAR:: added a shorter debug (only the function name not the full signature is shown)
	
	Revision 4.6  2012/06/25 17:42:06  marpas
	qacpp instrumentation
	
	Revision 4.5  2012/06/22 13:41:47  marpas
	pr qa instrumentation
	
	Revision 4.4  2012/06/14 17:16:41  marpas
	qacpp instrumentation
	
	Revision 4.3  2012/06/12 17:37:01  marpas
	qacpp instrumentation
	
	Revision 4.2  2012/03/19 15:49:25  marpas
	improving debug and getting rid of std::e_n_d_l()
	
	Revision 4.1  2012/01/23 10:31:49  micmaz
	removed macro LOG(...)
	
	Revision 4.0  2011/10/18 11:22:00  marpas
	new statistics
	
	Revision 3.12  2011/09/29 18:03:42  marpas
	LOG macro neutralized when codechek runs
	
	Revision 3.11  2011/09/29 16:42:55  marpas
	minor change
	
	Revision 3.10  2011/01/24 12:57:43  marpas
	exDECLARE_EXCEPTION macros modified (inlined) in order to provide convenience into templates too.
	As a consequence, the exDEFINE_EXCEPTION macro is a no-op and its use is deprecated (will be removed in the PDS_2 stream)
	The above change has impact on (at least) the following libraries:
	libAbstractFactoryChain
	libBroadcast
	libIpcBase
	libStatusMgr
	libTclFtp
	
	Revision 3.9  2011/01/20 19:21:27  marpas
	getErrno function implemented
	
	Revision 3.8  2010/11/24 20:39:23  marpas
	errno initialized to 0
	
	Revision 3.7  2010/11/19 16:11:19  micmaz
	added the LOG macro
	
	Revision 3.6  2010/11/11 16:53:45  marpas
	new macro: debug signature with thread id added
	
	Revision 3.5  2010/10/01 09:31:06  micmaz
	added ACS_EXCEPTION macro
	
	Revision 3.4  2010/09/15 10:20:52  marpas
	exostream::notifyStackFromEx  changed into exostream::allowStackTrace
	
	Revision 3.3  2010/09/10 13:54:51  marpas
	exDebugMsg changed into exDebugSignature
	
	Revision 3.2  2010/09/10 13:51:56  marpas
	exDebugMsg macro fixed
	
	Revision 3.1  2010/09/10 13:48:27  marpas
	exDebugMsg macros added:
	CV: ----------------------------------------------------------------------
	
	Revision 3.0  2009/10/08 10:48:10  marpas
	Statistics class added
	rules application improved
	
	Revision 2.1  2006/02/28 08:24:36  marpas
	new release, exException inherits from std::exception
	
	Revision 1.5  2005/10/28 16:50:27  marpas
	exCLASS_EQ_DEBUG macro added
	
	Revision 1.4  2005/10/11 12:39:41  marpas
	runtime debug extended to old macros
	
	Revision 1.3  2005/10/06 10:30:21  marpas
	Runtime debug almost completed
	
	Revision 1.2  2004/06/17 14:52:45  marpas
	long long related macros
	
	Revision 1.1  2004/04/05 09:06:39  marpas
	forgot
	

*/

#ifndef _exMacros_H_
#define _exMacros_H_ 

#include <acs_c++config.hpp>
#include <RTTr.hpp>
#include <string>
#include <ctime>
#include <cstdlib> // atoi
#include <features.h>

// PRQA S 1020, 1023, 1030, 1031 EOF

/* this to define __GLIBC_HAVE_LONG_LONG on Linux different from RH7.3 (e.g. SuSE Linux Standard Server 8.0)
   NOTE the dependency from __USE_ISOC99 ... see /usr/include/features.h file 
   The __GLIBC_HAVE_LONG_LONG is used, hence, to conditionally declare "long long" related methods/functions etc.
*/
#if (!defined __GLIBC_HAVE_LONG_LONG)
  #if (defined __USE_ISOC99)
    # define __GLIBC_HAVE_LONG_LONG	1
  #endif
#endif


_ACS_BEGIN_NESTED_NAMESPACE(acs, debug)

  /*! 
   * \brief Verbosity level standards
   *
   * The following values are the standard to define verbosity levels when debugging. 
   * \see ACS_LOW_VERB, ACS_MID_VERB, ACS_HIG_VERB, ACS_INS_VERB
   */
  struct VerbosityLevel {
    static const int VeryLow = 1 ; ///< default very low verbosity
    static const int Low = 10 ; ///< default low verbosity
    static const int Medium = 100 ; ///< default medium verbosity
    static const int High = 1000 ; ///< default high verbosity
    static const int Insane = 10000 ; ///< default insane verbosity
  } ;

_ACS_END_NESTED_NAMESPACE

/*! \addtogroup macro_logging 
 *  @{
 */

/*!
 * \brief debug convenience value (low verbosity)
 *
 * This is to standardize the debug verbosity
 * \code
 *   ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "This message will be printed from verbosity very-Low on") ;
 * \endcode
 * \see acs::debug::VerbosityLevel for current verbosity values 
 */
#define ACS_VLO_VERB acs::debug::VerbosityLevel::VeryLow

/*!
 * \brief debug convenience value (low verbosity)
 *
 * This is to standardize the debug verbosity
 * \code
 *   ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "This message will be printed from verbosity Low on") ;
 * \endcode
 * \see acs::debug::VerbosityLevel for current verbosity values 
 */
#define ACS_LOW_VERB acs::debug::VerbosityLevel::Low

/*!
 * \brief debug convenience value (medium verbosity)
 *
 * This is to standardize the debug verbosity
 * \code
 *   ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "This message will be printed from verbosity Medium on") ;
 * \endcode
 * \see acs::debug::VerbosityLevel for current verbosity values 
 */
#define ACS_MID_VERB acs::debug::VerbosityLevel::Medium

/*!
 * \brief debug convenience value (medium verbosity)
 *
 * This is to standardize the debug verbosity
 * \code
 *   ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "This message will be printed from verbosity High on") ;
 * \endcode
 * \see acs::debug::VerbosityLevel for current verbosity values 
 */
#define ACS_HIG_VERB acs::debug::VerbosityLevel::High

/*!
 * \brief debug convenience value (medium verbosity)
 *
 * This is to standardize the debug verbosity
 * \code
 *   ACS_CLASS_WRITE_DEBUG(ACS_INS_VERB, "This message will be printed from verbosity Insane on") ;
 * \endcode
 * \see acs::debug::VerbosityLevel for current verbosity values 
 */
#define ACS_INS_VERB acs::debug::VerbosityLevel::Insane


/*!	
 * \def ACS_LOG_PRIVINFO
 * \brief message logging macro 
 * 
 * 
 * The macro can be used to log an info message (acs::exFMessage) into a specified logger. 
 * Because it is a filterable (acs::exFilterable), 
 * it will be formatted according with the current acs::exMsgFmt assigned to the logger.
 * The message is sent to the stream currently set with acs::exostream::setDefaultOutLogger
 * 
 * example:
 * 
 * \code
 * 
 *    ACS_LOG_PRIVINFO("This is an info message " << 123) ;
 *  
 * \endcode
 * \see acs::exostream::setDefaultOutLogger, acs::exMsgFmt and acs::exFMessage
 */
#define ACS_LOG_PRIVINFO(message) { acs::exFPrivateInfo _f_ ; _f_ << message ; acs::exostream::getDefaultOutLogger() << _f_ ; } 

/*!	
 * \def ACS_LOG_INFO
 * \brief message logging macro 
 * 
 * 
 * The macro can be used to log an info message (acs::exFMessage) into a specified logger. 
 * Because it is a filterable (acs::exFilterable), 
 * it will be formatted according with the current acs::exMsgFmt assigned to the logger.
 * The message is sent to the stream currently set with acs::exostream::setDefaultOutLogger
 * 
 * example:
 * 
 * \code
 * 
 *    ACS_LOG_INFO("This is an info message " << 123) ;
 *  
 * \endcode
 * \see acs::exostream::setDefaultOutLogger, acs::exMsgFmt and acs::exFMessage
 */
#define ACS_LOG_INFO(message) { acs::exFMessage _f_ ; (void)(_f_ << message) ; (void)(acs::exostream::getDefaultOutLogger() << _f_) ; } 

/*!	
 * \def ACS_LOG_STATUS
 * \brief status logging macro 
 * 
 * 
 * The macro can be used to log a status (acs::exFStatus) into a specified logger. 
 * Because it is a filterable (acs::exFilterable), 
 * it will be formatted according with the current acs::exMsgFmt assigned to the logger.
 * The message is sent to the stream currently set with acs::exostream::setDefaultOutLogger
 * 
 * example:
 * 
 * \code
 * 
 *    ACS_LOG_STATUS("UP AND RUNNING") ;
 *  
 * \endcode
 * \see acs::exostream::setDefaultOutLogger, acs::exMsgFmt and acs::exFMessage
 */
#define ACS_LOG_STATUS(message) { acs::exFStatus _f_ ; _f_ << message ; acs::exostream::getDefaultOutLogger() << _f_ ; } 

/*!	
 * \def ACS_LOG_WSTATUS
 * \brief warning status logging macro 
 * 
 * 
 * The macro can be used to log a warning status (acs::exFWStatus) into a specified logger. 
 * Because it is a filterable (acs::exFilterable), 
 * it will be formatted according with the current acs::exMsgFmt assigned to the logger.
 * The message is sent to the stream currently set with acs::exostream::setDefaultOutLogger
 * 
 * example:
 * 
 * \code
 * 
 *    ACS_LOG_WSTATUS("DEGRADED") ;
 *  
 * \endcode
 * \see acs::exostream::setDefaultOutLogger, acs::exMsgFmt and acs::exFWarning
 */
#define ACS_LOG_WSTATUS(message) { acs::exFWStatus _f_ ; _f_ << message ; acs::exostream::getDefaultOutLogger() << _f_ ; } 


/*!	
 * \def ACS_LOG_ESTATUS
 * \brief error status logging macro 
 * 
 * 
 * The macro can be used to log an error status (acs::exFEStatus) into a specified logger. 
 * Because it is a filterable (acs::exFilterable), 
 * it will be formatted according with the current acs::exMsgFmt assigned to the logger.
 * The message is sent to the stream currently set with acs::exostream::setDefaultOutLogger
 * 
 * example:
 * 
 * \code
 * 
 *    ACS_LOG_ESTATUS("NO CONNECTION") ;
 *  
 * \endcode
 * \see acs::exostream::setDefaultOutLogger, acs::exMsgFmt and acs::exFError
 */
#define ACS_LOG_ESTATUS(message) { acs::exFEStatus _f_ ; _f_ << message ; acs::exostream::getDefaultOutLogger() << _f_ ; } 


/*!	
 * \def ACS_LOG_EVENT
 * \brief debug message logging macro 
 * 
 * 
 * The macro can be used to log a acs::exFEvent message (filterable) into a specified logger. 
 * Because it is a filterable (acs::exFilterable), 
 * it will be formatted according with the current acs::exMsgFmt assigned to the logger.
 * The Debug are sent to the stream currently set with acs::exostream::setDefaultOutLogger
 * 
 * example:
 * 
 * \code
 * 
 *    ACS_LOG_EVENT("key", "value") ;
 *  
 * \endcode
 * \see acs::exostream::setDefaultOutLogger, acs::exMsgFmt and acs::exFEvent
 */
#define ACS_LOG_EVENT(key,value) { acs::exFEvent _f_(key,value) ; acs::exostream::getDefaultOutLogger() << _f_ ; } 

/*!	
 * \def ACS_LOG_EVENTMSG
 * \brief debug message logging macro 
 * 
 * 
 * The macro can be used to log a acs::exFEvent message (filterable) into a specified logger. 
 * Because it is a filterable (acs::exFilterable), 
 * it will be formatted according with the current acs::exMsgFmt assigned to the logger.
 * The Debug are sent to the stream currently set with acs::exostream::setDefaultOutLogger
 * 
 * example:
 * 
 * \code
 * 
 *    ACS_LOG_EVENTMSG("key", "value", "additional " << "message") ;
 *  
 * \endcode
 * \see acs::exostream::setDefaultOutLogger, acs::exMsgFmt and acs::exFEvent
 */
#define ACS_LOG_EVENTMSG(key,value, message) { acs::exFEvent _f_(key,value); _f_ << message ; acs::exostream::getDefaultOutLogger() << _f_ ; } 

/*!	
 * \def ACS_LOG_OPALERT
 * \brief debug message logging macro 
 * 
 * 
 * The macro can be used to log a acs::exFOpAlert message (filterable) into a specified logger. 
 * Because it is a filterable (acs::exFilterable), 
 * it will be formatted according with the current acs::exMsgFmt assigned to the logger.
 * The Debug are sent to the stream currently set with acs::exostream::setDefaultOutLogger
 * 
 * example:
 * 
 * \code
 * 
 *    ACS_LOG_OPALERT("This is a debug message " << 234) ;
 *  
 * \endcode
 * \see acs::exostream::setDefaultOutLogger, acs::exMsgFmt and acs::exFOpAlert
 */
#define ACS_LOG_OPALERT(message) { acs::exFOpAlert _f_ ; _f_ << message ; acs::exostream::getDefaultOutLogger() << _f_ ; } 

/*!	
 * \def ACS_LOG_OPEVENTALERT
 * \brief debug message logging macro 
 * 
 * 
 * The macro can be used to log a acs::exFOpEventAlert message (filterable) into a specified logger. 
 * Because it is a filterable (acs::exFilterable), 
 * it will be formatted according with the current acs::exMsgFmt assigned to the logger.
 * The Debug are sent to the stream currently set with acs::exostream::setDefaultOutLogger
 * 
 * example:
 * 
 * \code
 * 
 *    ACS_LOG_OPEVENTALERT("key", "value") ;
 *  
 * \endcode
 * \see acs::exostream::setDefaultOutLogger, acs::exMsgFmt and acs::exFOpEventAlert
 */
#define ACS_LOG_OPEVENTALERT(key,value) { acs::exFOpEventAlert _f_(key,value); acs::exostream::getDefaultOutLogger() << _f_ ; } 

/*!	
 * \def ACS_LOG_OPEVENTALERTMSG
 * \brief debug message logging macro 
 * 
 * 
 * The macro can be used to log a acs::exFOpEventAlert message (filterable) into a specified logger. 
 * Because it is a filterable (acs::exFilterable), 
 * it will be formatted according with the current acs::exMsgFmt assigned to the logger.
 * The Debug are sent to the stream currently set with acs::exostream::setDefaultOutLogger
 * 
 * example:
 * 
 * \code
 * 
 *    ACS_LOG_OPEVENTALERTMSG("key", "value", "additional value: " << 3) ;
 *  
 * \endcode
 * \see acs::exostream::setDefaultOutLogger, acs::exMsgFmt and acs::exFOpEventAlert
 */
#define ACS_LOG_OPEVENTALERTMSG(key,value,message) { acs::exFOpEventAlert _f_(key,value); _f_ << message ; acs::exostream::getDefaultOutLogger() << _f_ ; } 
/*!	
 * \def ACS_LOG_DEBUG
 * \brief debug message logging macro 
 * 
 * 
 * The macro can be used to log a acs::exFDebug message (filterable) into a specified logger. 
 * Because it is a filterable (acs::exFilterable), 
 * it will be formatted according with the current acs::exMsgFmt assigned to the logger.
 * The Debug are sent to the stream currently set with acs::exostream::setDefaultOutLogger
 * 
 * example:
 * 
 * \code
 * 
 *    ACS_LOG_DEBUG("This is a debug message " << 234) ;
 *  
 * \endcode
 * \see acs::exostream::setDefaultOutLogger, acs::exMsgFmt and acs::exFDebug
 */
#define ACS_LOG_DEBUG(message) { acs::exFDebug _f_ ; (void)(_f_ << message) ; (void)(acs::exostream::getDefaultOutLogger() << _f_) ; } 

/*!	
 * \def ACS_LOG_WARNING
 * \brief warning message logging macro 
 * 
 * 
 * The macro can be used to log a acs::exFWarning message (filterable) into a specified logger. 
 * Because it is a filterable (acs::exFilterable), 
 * it will be formatted according with the current acs::exMsgFmt assigned to the logger.
 * The Warning are sent to the stream currently set with acs::exostream::setDefaultOutLogger
 * 
 * example:
 * 
 * \code
 * 
 *    ACS_LOG_WARNING("This is a warning message " << 345) ;
 *  
 * \endcode
 * \see acs::exostream::setDefaultOutLogger, acs::exMsgFmt and acs::exFWarning
 */
#define ACS_LOG_WARNING(message) { acs::exFWarning _f_ ; _f_ << message ; acs::exostream::getDefaultOutLogger() << _f_ ; } 

/*!	
 * \def ACS_LOG_ERROR
 * \brief error message logging macro 
 * 
 * 
 * The macro can be used to log a acs::exFError message (filterable) into a specified logger. 
 * Because it is a filterable (acs::exFilterable), 
 * it will be formatted according with the current acs::exMsgFmt assigned to the logger.
 * The Error are sent to the stream currently set with acs::exostream::setDefaultErrLogger
 * 
 * example:
 * 
 * \code
 * 
 *    ACS_LOG_ERROR("This is an error message " << 456) ;
 *  
 * \endcode
 * \see acs::exostream::setDefaultErrLogger, acs::exMsgFmt and acs::exFError
 */
#define ACS_LOG_ERROR(message) { acs::exFError _f_ ; _f_ << message ; acs::exostream::getDefaultOutLogger() << _f_ ; } 

/*!	
 * \def ACS_LOG_PROGRESS
 * \brief progress message logging macro 
 * 
 * 
 * The macro can be used to log a acs::exFProgress message (filterable) into a specified logger. 
 * Because it is a filterable (acs::exFilterable), 
 * it will be formatted according with the current acs::exMsgFmt assigned to the logger.
 * The Progress are sent to the stream currently set with acs::exostream::setDefaultOutLogger
 * 
 * example:
 * 
 * \code
 * 
 *    ACS_LOG_PROGRESS("This is a progress message " << 345) ;
 *  
 * \endcode
 * \see acs::exostream::setDefaultOutLogger, acs::exMsgFmt and acs::exFProgress
 */
#define ACS_LOG_PROGRESS(message) { acs::exFProgress _f_ ; _f_ << message ; acs::exostream::getDefaultOutLogger() << _f_ ; }

/*!	
 * \def ACS_LOG_NOTIFY_EX
 * \brief exception logging macro 
 * 
 * 
 * The macro can be used to log an std::exception into a specified logger. 
 * The exception are sent to the stream currently set with acs::exostream::setDefaultErrLogger
 * 
 * example:
 * 
 * \code
 *    try {
 *       callDangerousfunction() ; // will throw an exception
 *    }
 *    catch (std::exception &x) {
 *      ACS_LOG_NOTIFY_EX(x) ;
 *    }
 *  
 * \endcode
 * \see acs::exostream::setDefaultErrLogger, acs::ex::Exception
 */
#define ACS_LOG_NOTIFY_EX(x) { acs::exostream::getDefaultOutLogger() << x ; }

/*!	
 * \def ACS_LOG_MC_INFO
 * \brief monitor & control info message logging macro 
 * 
 * 
 * The macro can be used to log a acs::mc::exFMCInfo message (filterable) into a specified logger. 
 * Because it is a filterable (acs::exFilterable), 
 * it will be formatted according with the current acs::exMsgFmt assigned to the logger.
 * The Info are sent to the stream currently set with acs::exostream::setDefaultErrLogger
 * 
 * example:
 * 
 * \code
 * 
 *    ACS_LOG_MC_INFO("This is an info message " << 456) ;
 *  
 * \endcode
 * \see acs::exostream::setDefaultErrLogger, acs::exMsgFmt and acs::mc::exFMCInfo
 */
#define ACS_LOG_MC_INFO(message) { acs::mc::exFMCInfo _f_ ; _f_ << message ; acs::exostream::getDefaultOutLogger() << _f_ ; } 

/*!	
 * \def ACS_LOG_MC_WARNING
 * \brief monitor & control warning message logging macro 
 * 
 * 
 * The macro can be used to log a acs::mc::exFMCWarning message (filterable) into a specified logger. 
 * Because it is a filterable (acs::exFilterable), 
 * it will be formatted according with the current acs::exMsgFmt assigned to the logger.
 * The warning are sent to the stream currently set with acs::exostream::setDefaultErrLogger
 * 
 * example:
 * 
 * \code
 * 
 *    ACS_LOG_MC_WARNING("This is a warning message " << 456) ;
 *  
 * \endcode
 * \see acs::exostream::setDefaultErrLogger, acs::exMsgFmt and acs::mc::exFMCWarning
 */
#define ACS_LOG_MC_WARNING(message) { acs::mc::exFMCWarning _f_ ; _f_ << message ; acs::exostream::getDefaultOutLogger() << _f_ ; } 

/*!	
 * \def ACS_LOG_MC_ERROR
 * \brief monitor & control error message logging macro 
 * 
 * 
 * The macro can be used to log a acs::mc::exFMCError message (filterable) into a specified logger. 
 * Because it is a filterable (acs::exFilterable), 
 * it will be formatted according with the current acs::exMsgFmt assigned to the logger.
 * The error are sent to the stream currently set with acs::exostream::setDefaultErrLogger
 * 
 * example:
 * 
 * \code
 * 
 *    ACS_LOG_MC_ERROR("This is an error message " << 456) ;
 *  
 * \endcode
 * \see acs::exostream::setDefaultErrLogger, acs::exMsgFmt and acs::mc::exFMCError
 */
#define ACS_LOG_MC_ERROR(message) { acs::mc::exFMCError _f_ ; _f_ << message ; acs::exostream::getDefaultOutLogger() << _f_ ; } 

/*!	
 * \def ACS_LOG_MC_CRITICAL
 * \brief monitor & control error message logging macro 
 * 
 * 
 * The macro can be used to log a acs::mc::exFMCCritical message (filterable) into a specified logger. 
 * Because it is a filterable (acs::exFilterable), 
 * it will be formatted according with the current acs::exMsgFmt assigned to the logger.
 * The error are sent to the stream currently set with acs::exostream::setDefaultErrLogger
 * 
 * example:
 * 
 * \code
 * 
 *    ACS_LOG_MC_CRITICAL("This is an error message " << 456) ;
 *  
 * \endcode
 * \see acs::exostream::setDefaultErrLogger, acs::exMsgFmt and acs::mc::exFMCCritical
 */
#define ACS_LOG_MC_CRITICAL(message) { acs::mc::exFMCCritical _f_ ; _f_ << message ; acs::exostream::getDefaultOutLogger() << _f_ ; } 

/*!	
 * \def ACS_LOG_TRYCATCHNOTIFY_EX
 * \brief exception throwing and notifying macro 
 * 
 * 
 * The macro can be used to throw and log an std::exception into a specified logger. 
 * The exception are sent to the stream currently set with acs::exostream::setDefaultErrLogger
 * 
 * example:
 * 
 * \code
 *  // ...
 *
 *  ACS_LOG_TRYCATCHNOTIFY_EX(FileOpenException(string("Cannot open file: ") + filename)) ;
 *
 *  // ...
 *  
 * \endcode
 * \see acs::exostream::setDefaultErrLogger, tryCatchAndNotify
 */
#define ACS_LOG_TRYCATCHNOTIFY_EX(x) { tryCatchAndNotify(x, acs::exostream::getDefaultOutLogger()) ; }

/*! @}*/


_ACS_BEGIN_NESTED_NAMESPACE(acs,utility)


/*!
 * \brief returns a string with the following info: "<class name if any>::<function name>(...)"
 * \note inlined for perfo
 */

inline std::string FuncName_( std::string const &s__) // PRQA S 2134
{
    // PRQA S 3084, 3143, 3278, 4406, 4414 L1
 std::string::size_type const ol__ = s__.find('(') ;                // find first open ellipse
 std::string::size_type sp__ = s__.find_last_of ( ' ', ol__ ) ;    // find last space BEFORE open ell. (for removal)
 sp__ = (( std::string::npos == sp__ ) ? 0 : (sp__ + 1)) ;   // space is 0 if not found            // PRQA S 3380
 return s__.substr(sp__, ol__-sp__+1) +                     // build: from space to '('           // PRQA S 3383 3   
		            ( (s__.at(ol__+1) == ')') ? "" : "...") + // ... if "()" contains something
					')' ;                                   // final ')'
    // PRQA L:L1
}

_ACS_END_NESTED_NAMESPACE


_ACS_BEGIN_NAMESPACE(acs)

// deprecated macro trick - harmless function used at compile time to emit deprecated warning
#ifndef SILENT_MACROS_DEPRECATED_WARNING
#define ATTRIBUTE_DEPRECATED __attribute__((deprecated)) 
#else
#define ATTRIBUTE_DEPRECATED 
#endif
ATTRIBUTE_DEPRECATED static inline void exCLASS_END_DEBUG_D() {} // PRQA S 2134,2314,2645,4649 6
ATTRIBUTE_DEPRECATED static inline const int & exCLASS_GET_DEBUG_I(const int &v__) { return v__ ; } // PRQA S 4624 5
ATTRIBUTE_DEPRECATED static inline const int & exCLASS_BGN_DEBUG_I(const int &v__) { return v__ ; } 
ATTRIBUTE_DEPRECATED static inline const int & exCLASS_INIT_DEBUG_I(const int &v__) { return v__ ; } 
ATTRIBUTE_DEPRECATED static inline const int & exCLASS_EQ_DEBUG_I(const int &v__) { return v__ ; } 
ATTRIBUTE_DEPRECATED static inline const int & exCLASS_SET_DEBUG_I(const int &v__) { return v__ ; } 
/*! \addtogroup macro_debug
 *  @{
 */

/*! \addtogroup deprecated
 *  @{
 */
/*!	\brief Deprecated use ACS_CLASS_DECLARE_DEBUG_LEVEL(classname)

	\def exCLASS_DECLARE_RTDEBUG_LEVEL

    \deprecated use ACS_CLASS_DECLARE_DEBUG_LEVEL(classname) instead.
    
	The macro can be used to declare a static member that will be used to debug
	the class.
	
	example:
	
	\code
	
	class A {
		// ...
	
	private:
		exCLASS_DECLARE_RTDEBUG_LEVEL // keep debug info private
	} ;
	
	\endcode
	
	\see exCLASS_DEFINE_RTDEBUG_LEVEL, exCLASS_BGN_DEBUG, exCLASS_END_DEBUG, exCLASS_SET_DEBUG, exCLASS_GET_DEBUG
	
	\note If the class is a base class and you want to debug the whole class hierarchy
	based on it, just put the exCLASS_DECLARE_DEBUG_LEVEL in the protected member area, 
	only on the base class and not on its derivates.
	
*/
// PRQA S 1020,1023 2
#define exCLASS_DECLARE_RTDEBUG_LEVEL		static acs::exRTDebuggable exCDebugLevel ;

/*!  @} */

/*!	
 * \def ACS_CLASS_DECLARE_DEBUG_LEVEL (classname)
 * \brief class level debug convenience macro
 * 
 * 
 * The macro can be used to declare a static member that will be used to debug
 * the class.
 * 
 * example:
 * 
 * \code
 * 
 *    class A {
 *       ...
 * 
 *    private:
 *       ACS_CLASS_DECLARE_DEBUG_LEVEL (A) ; // keep debug info private
 *    } ;
 * 
 * \endcode
 * 
 * \note If the class is a base class and you want to debug the whole class hierarchy
 * based on it, just put the ACS_CLASS_DECLARE_DEBUG_LEVEL in the protected member area, 
 * only on the base class and not on its derivates.
 * \note ACS_CLASS_DECLARE_DEBUG_LEVEL will try to get an env var to get the debug level \sa acs::strings::RTTr for transformation rules
 * 
 */

#define ACS_CLASS_DECLARE_DEBUG_LEVEL(Class_)	 \
static acs::utility::RTDebuggable< Class_ > \
	exCDebugLevel;



/*!	\brief class level debug convenience macro
 * 
 * \def ACS_CLASS_DEFINE_DEBUG_LEVEL(classname)
 * 
 * The macro can be used to define the class debug static member RTDebuggable<class> DebugLevel. The costructor of RTDebuggable<class> 
 * will initialize the static member.
 * Because this initialization uses getenv(), its performances have been optimized to ensure that for each class (not for each istances), 
 * only one getenv will be called for each session.
 * After the call, if the environment variable was set, the static member will	be initialized to that value (atoi() call), else the value will be set to -1 
 * and no further getenv will be attempted on that static member.
 * The best place to put this macro is in every traslation unit where there are the other definitions of the class. 
 * 
 * example:
 * 
 * \code
 * 
 *    // class A implementation
 *    
 *    ACS_CLASS_DEFINE_DEBUG_LEVEL(A) // define the debug static member on class A
 *    
 *    void A::A() // A ctor
 *    {
 *       // ...
 *    } 
 *    
 * \endcode
 * 
 */
#define ACS_CLASS_DEFINE_DEBUG_LEVEL(Class_)	\
acs::utility::RTDebuggable<Class_> \
	Class_::exCDebugLevel ; 

/*!	\brief class level debug convenience macro
 * 
 * \def ACS_CLASS_DEFINE_LINKDEBUG_LEVEL(classname)
 * 
 * The macro can be used to define the class debug static member RTDebuggable<class> DebugLevel. 
 * The costructor of RTDebuggable<class> will initialize the static member. 
 * The debug is linked to the class given as second argument.
 * Setting a debug level for the REF class will also set the same debug for this one.
 * Notice also that all the classes linked directly or indirectly to classname 
 * will also be affected by the activation of debug of REF class.
 * \note: to use this macro the class REF definition shall be available. Consider also that
 * if the link is defined between independent classes (classes defined in different
 * shared objects with no direct dependency) the link debug facility might not work as 
 * expected because of the unpredictability on global variable initialization.
 *
 * All the notes from related macros are valid \see ACS_CLASS_DEFINE_DEBUG_LEVEL
 * 
 * example:
 * 
 * \code
 * 
 *    // class A implementation
 *    
 *    // define the debug static member on class A linked to class REF debug 
 *    ACS_CLASS_DEFINE_LINKDEBUG_LEVEL(A, REF) 
 *    
 *    void A::A() // A ctor
 *    {
 *       // ...
 *    }
 * 
 *    // class B implementation

 *    // define the debug static member on class B linked to class A debug 
 *    ACS_CLASS_DEFINE_LINKDEBUG_LEVEL(B, A) 
 *    
 *    void B::B() // A ctor
 *    {
 *       // ...
 *    }
 *
 *    // As result:
 * 	  //    - if debug for REF is enabled, then also A and B enters in debug mode.
 * 	  //    - if debug for A is enabled, then B enters in debug mode.
 *  
 * \endcode
 * 
 * 
 */
 #define ACS_CLASS_DEFINE_LINKDEBUG_LEVEL(Class_, Ref_)	\
acs::utility::RTDebuggable<Class_> \
	Class_::exCDebugLevel(std::string(strings::RTTr(exStackTrace::demangle(typeid(Ref_).name())))) ; 




/*!	
 * \def ACS_TEMPLATE_DECLARE_DEBUG_LEVEL (C,T1)
 * \brief class level debug convenience macro
 * 
 * 
 * The macro can be used to declare a static member that will be used to debug
 * the template (with two types).
 * 
 * example:
 * 
 * \code
 *    // class A definition
 *    template<typename T1> class A
 *    {
 *       ...
 *    private:
 *       ACS_TEMPLATE_DECLARE_DEBUG_LEVEL (A,T1) ; // keep debug info private
 *    }
 * 
 * \endcode
 * 
 */

#define ACS_TEMPLATE_DECLARE_DEBUG_LEVEL(Class_,T)	\
static acs::utility::RTDebuggable< Class_<T> >	\
	exCDebugLevel;

/*!	\brief class level debug convenience macro
 * 
 * \def ACS_TEMPLATE_DECLARE_DEBUG_LEVEL2 (C,T1,T2)
 * 
 * The macro can be used to declare the template debug static member
 * 
 * example:
 * 
 * \code
 *    // class A definition
 *    template<typename T1, T2> class A
 *    {
 *       ...
 *    private:
 *       ACS_TEMPLATE_DECLARE_DEBUG_LEVEL2 (A,T1,T2) ; // keep debug info private
 *    }
 *    
 * \endcode
 * 
 */

#define ACS_TEMPLATE_DECLARE_DEBUG_LEVEL2(Class_,T1,T2)	\
static acs::utility::RTDebuggable< Class_<T1, T2> >	\
	exCDebugLevel;

/*!	\brief class level debug convenience macro
 * 
 * \def ACS_TEMPLATE_DECLARE_DEBUG_LEVEL3 (C,T1,T2, T3)
 * 
 * The macro can be used to declare the template debug static member
 * 
 * example:
 * 
 * \code
 *    // class A definition
 *    template<typename T1, T2, T3> class A
 *    {
 *       ...
 *    private:
 *       ACS_TEMPLATE_DECLARE_DEBUG_LEVEL3 (A,T1,T2,T3) ; // keep debug info private
 *    }
 *    
 * \endcode
 * 
 */



#define ACS_TEMPLATE_DECLARE_DEBUG_LEVEL3(Class_,T1,T2,T3)	\
static acs::utility::RTDebuggable< Class_<T1, T2, T3> >	\
	exCDebugLevel;




/*!	\brief class level debug convenience macro
 * 
 * \def ACS_TEMPLATE_DEFINE_DEBUG_LEVEL (C,T1)
 * 
 * The macro can be used to define the template debug static member
 * 
 * example:
 * 
 * \code
 *    // class A definition
 *    template<typename T1> class A
 *    {
 *       ...
 *    private:
 *       ACS_TEMPLATE_DECLARE_DEBUG_LEVEL (A,T1)) ; // keep debug info private
 *    }
 *    
 *    // class A implementation
 *    
 *    ACS_TEMPLATE_DEFINE_DEBUG_LEVEL (A,T1) ; // define the debug static member on class A<T1>
 *    
 *    void A::A() // A ctor
 *    {
 *       // ...
 *    } 
 *    
 * \endcode
 * 
 */

#define ACS_TEMPLATE_DEFINE_DEBUG_LEVEL(Class_, t1_)	\
template<typename t1_> acs::utility::RTDebuggable<Class_<t1_> > \
	Class_<t1_>::exCDebugLevel ; 
    
/*!	\brief class level debug convenience macro
 * 
 * \def ACS_TEMPLATE_DEFINE_DEBUG_LEVEL2 (C,T1,T2)
 * 
 * The macro can be used to define the template debug static member
 * 
 * example:
 * 
 * \code
 *    // class A definition
 *    template<typename T1, T2> class A
 *    {
 *       ...
 *    private:
 *       ACS_TEMPLATE_DECLARE_DEBUG_LEVEL2 (A,T1,T2) ; // keep debug info private
 *    }
 *    
 *    // class A implementation
 *    
 *    ACS_TEMPLATE_DEFINE_DEBUG_LEVEL2 (A, T1, T2) ; // define the debug static member on class A<T1,T2>
 *    
 *    void A::A() // A ctor
 *    {
 *       // ...
 *    } 
 *    
 * \endcode
 * 
 */
#define ACS_TEMPLATE_DEFINE_DEBUG_LEVEL2(Class_,t1_,t2_)	\
template<typename t1_, typename t2_> acs::utility::RTDebuggable<Class_<t1_,t2_> > \
	Class_<t1_,t2_>::exCDebugLevel ;


/*!	\brief class level debug convenience macro
 * 
 * \def ACS_TEMPLATE_DEFINE_DEBUG_LEVEL3 (C,T1,T2,T3)
 * 
 * The macro can be used to define the template debug static member
 * 
 * example:
 * 
 * \code
 *    // class A definition
 *    template<typename T1, T2, T3> class A
 *    {
 *       ...
 *    private:
 *       ACS_CLASS_DECLARE_DEBUG_LEVEL3 (A,T1,T2,T3)) ; // keep debug info private
 *    }
 *    
 *    // class A implementation
 *    
 *    ACS_TEMPLATE_DEFINE_DEBUG_LEVEL3 (A, T1, T2, T3) ; // define the debug static member on class A<T1,T2,T3>
 *    
 *    void A::A() // A ctor
 *    {
 *       // ...
 *    } 
 *    
 * \endcode
 * 
 */
#define ACS_TEMPLATE_DEFINE_DEBUG_LEVEL3(Class_,t1_,t2_,t3_)	\
template<typename t1_, typename t2_, typename t3_> acs::utility::RTDebuggable<Class_<t1_,t2_,t3_> > \
	Class_<t1_,t2_,t3_>::exCDebugLevel ;


/*! \addtogroup deprecated
 *  @{
 */

    
/*!	\brief Deprecated use ACS_CLASS_DEFINE_DEBUG_LEVEL(classname)

	\def exCLASS_DEFINE_RTDEBUG_LEVEL(classname) 

    \deprecated use ACS_CLASS_DEFINE_DEBUG_LEVEL(classname) instead
    
	The macro can be used to define the class debug static member
	
	example:
	
	\code
	
	// class A implementation
	
	exCLASS_DEFINE_DEBUG_LEVEL(A) // define the debug static member on class A
	
	void A::A() // A ctor
	{
		// ...
	} 
	
	\endcode
	
	\see exCLASS_DECLARE_RTDEBUG_LEVEL, exCLASS_BGN_RTDEBUG, exCLASS_SET_RTDEBUG, exCLASS_GET_RTDEBUG
*/
// PRQA S 1020,1023 2
#define exCLASS_DEFINE_RTDEBUG_LEVEL(classname)	acs::exRTDebuggable classname::exCDebugLevel(# classname"_DEBUG");




/*!	\brief  Deprecated use ACS_CLASS_DECLARE_DEBUG_LEVEL(classname)

	\def exCLASS_DECLARE_DEBUG_LEVEL

    \deprecated use ACS_CLASS_DECLARE_DEBUG_LEVEL(classname) instead 
    
	The macro can be used to declare a static member that will be used to debug
	the class.
	
	example:
	
	\code
	
	class A {
	  ...
	
	private:
		exCLASS_DECLARE_DEBUG_LEVEL // keep debug info private
	} ;
	
	\endcode
	
	\see exCLASS_DEFINE_DEBUG_LEVEL, exCLASS_INIT_DEBUG, exCLASS_BGN_DEBUG, exCLASS_END_DEBUG, exCLASS_SET_DEBUG, exCLASS_GET_DEBUG
	
	\note If the class is a base class and you want to debug the whole class hierarchy
	based on it, just put the exCLASS_DECLARE_DEBUG_LEVEL in the protected member area, 
	only on the base class and not on its derivates.
	
*/
// PRQA S 1020,1023 2
#define exCLASS_DECLARE_DEBUG_LEVEL		static acs::exRTDebuggable exCDebugLevel ;

/*!  @} */

/*!	\brief class level debug convenience macro
 * 
 * \def ACS_CLASS_INIT_DEBUG(env)
 * 
 * example:
 * 
 * \code
 * 
 *    // class A implementation
 * 
 *    ACS_CLASS_DEFINE_DEBUG_LEVEL(A) // define the debug static member on class A
 *    
 *    void A::A() // A ctor
 *    {
 *       // get the debug level for class A from 
 *       // ADifferent_DEBUG environment variable
 *       ACS_CLASS_INIT_DEBUG(ADifferent_DEBUG) ; 
 *    
 *       // ...
 *    } 
 *    
 * \endcode
 * 
 * \note 
 * If the class DebugLevel static member need to be initialized manually for some reason, this macro can be used.
 * Reasons to do that are:
 * - if the class has not an RTDebuggable<class> DebugLevel member (e.g the acs::utility::RTDebug class itself
 * - if the environment variable is not the default one but you want to use a different string
 * 
 * \note
 * In case the class has static members, and you want to debug them under any of the two circumstances above, 
 * because the static members can be invoked even if no object of the class has been created, you should ensure 
 * proper initialization of the debug static member. 
 * To do so, the best pratical way is to put an initialization 
 * line, as in the example above as the first statement in every public static member function.
 * If you have some performance issue and you know, by design that a public
 * static member function should be called on top of the other, is enough to put the initialization line 
 * as the first statement of that member.
 * 
 */
#define ACS_CLASS_INIT_DEBUG(env)	\
if (exCDebugLevel == 0) {  acs::strings::RTTr::envget_(#env, exCDebugLevel) ; } 


/*!	\brief class level debug convenience macro
 * 
 * The macro can be used to set the debug level to a new value
 * 
 * example:
 * 
 * \code
 * 
 *    void A::f()
 *    {
 *       // ...
 *    
 *       ACS_CLASS_SET_DEBUG(100) ; // from now on the debug level is 100
 *    
 *       // ...
 *    } 
 *    
 * \endcode
 * 
 * \note It is provided in case you would like to change your debug level runtime.
 */
#define ACS_CLASS_SET_DEBUG(newlevel) setInt(exCDebugLevel,newlevel) 


/*!	\brief class level debug convenience macro
 * 
 * The macro can be used to set the debug level to a new value
 * 
 * example:
 * 
 * \code
 * 
 *    void f()
 *    {
 *       // ...
 *    
 *       ACS_FUNC_SET_DEBUG(A,100) ; // from now on the debug level of class A is 100
 *    
 *       // ...
 *    } 
 *    
 * \endcode
 * 
 * \note It is provided in case you would like to change your debug level runtime.
 */
#define ACS_FUNC_SET_DEBUG(c, newlevel) setInt(c::exCDebugLevel,newlevel) 



/*! \addtogroup legacy
 *  @{
 */


/*!	\brief global/special debug convenience macro for legacy code (C)

	\def exBGN_DEBUG(v,level)

	The macro can be used to start a special/global conditioned block.
	
	example:
	
	\code
	
	void A::f()
	{
		// ...
	
		exBGN_DEBUG(1,specialDebug) // debug level is 1 or greater
			cerr << "A::f:" << __LINE__
				 << " count: " << count << '\n' ;
		exEND_DEBUG
		
		// ...
		
	} 
	
	\endcode
	
	\see exEND_DEBUG
*/
// PRQA S 1020,1023 2
#define exBGN_DEBUG(v,level)	if (asInt(v) >= (level)) {

/*!	\brief global/special debug convenience macro for legacy code (C)

	\def exEND_DEBUG

	The macro can be used to finish conditioned global or special debug statements.
	
	example:
	
	\code
	
	void A::f()
	{
		// ...
	
		exBGN_DEBUG(1,specialDebug) // debug level is 1 or greater
			cerr << "A::f:" << __LINE__
				 << " count: " << count  << '\n' ;
		exEND_DEBUG
		
		// ...
		
	} 
	
	\endcode
	
	\see exBGN_DEBUG
	
*/
#ifdef __cplusplus
#define exEND_DEBUG ; exCLASS_END_DEBUG_D() ; }
#else
#define exEND_DEBUG	; }
#endif
/*! @}*/


/*! \addtogroup deprecated 
 *  @{
 */

/*!	\brief Deprecated - use ACS_CLASS_BGN_DEBUG(level)

	\def exCLASS_BGN_DEBUG(level)

    \deprecated use ACS_CLASS_BGN_DEBUG(level) instead 
    
	The macro can be used to put under debug some statements.
	
	example:
	
	\code
	
	void f()
	{
		// ...
	
		exCLASS_BGN_DEBUG(10) // debug level is 10 or greater
			cerr << "f:" << __LINE__
				 << " count: " << count << '\n' ;
		exCLASS_END_DEBUG
		
		// ...
	} 
	
	\endcode
	
	\see exCLASS_DECLARE_DEBUG_LEVEL, exCLASS_DEFINE_DEBUG_LEVEL, 
	exCLASS_INIT_DEBUG, exCLASS_END_DEBUG, 
	exCLASS_SET_DEBUG, exCLASS_GET_DEBUG
*/
// PRQA S 1020,1023 2
#define exCLASS_BGN_DEBUG(level)		if (asInt(exCDebugLevel) >= exCLASS_BGN_DEBUG_I(level)) { 




/*!	\brief Deprecated use ACS_CLASS_EQ_DEBUG(level)

	\def exCLASS_EQ_DEBUG(level) 

    \deprecated use ACS_CLASS_EQ_DEBUG(level) instead 
    
	The macro can be used to put under debug some statements.
	
	example:
	
	\code
	
	void f()
	{
		// ...
	
		exCLASS_EQ_DEBUG(10) // debug level is 10 
			cerr << "f:" << __LINE__
				 << " count: " << count << '\n' ;
		exCLASS_END_DEBUG
		
		// ...
	} 
	
	\endcode
	
	\see exCLASS_DECLARE_DEBUG_LEVEL, exCLASS_DEFINE_DEBUG_LEVEL, exCLASS_INIT_DEBUG, exCLASS_END_DEBUG, exCLASS_SET_DEBUG, exCLASS_GET_DEBUG exCLASS_BGN_DEBUG
*/
// PRQA S 1020,1023 2
#define exCLASS_EQ_DEBUG(level)		if (asInt(exCDebugLevel) == exCLASS_EQ_DEBUG_I(level)) {

/*! @}*/



/*!	\brief class level debug convenience macro

	\def ACS_CLASS_EQ_DEBUG(level)

	The macro can be used to put under debug some statements.
	
	example:
	
	\code
	
	void f()
	{
		// ...
	
		ACS_CLASS_EQ_DEBUG(10) // debug level is 10 
			cerr << "f:" << __LINE__
				 << " count: " << count << '\n' ;
		ACS_CLASS_END_DEBUG
		
		// ...
	} 
	
	\endcode
	
	\see exCLASS_DECLARE_DEBUG_LEVEL, exCLASS_DEFINE_DEBUG_LEVEL, exCLASS_INIT_DEBUG, exCLASS_END_DEBUG, exCLASS_SET_DEBUG, exCLASS_GET_DEBUG exCLASS_BGN_DEBUG
*/
// PRQA S 1020,1023 2
#define ACS_CLASS_EQ_DEBUG(level)		if (exCDebugLevel == (level)) {




/*! \addtogroup deprecated 
 *  @{
 */

/*!	\brief Deprecated use ACS_CLASS_SET_DEBUG(newlevel)

	\def exCLASS_SET_DEBUG(newlevel)

	\deprecated use ACS_CLASS_SET_DEBUG(newlevel) instead
    
    The macro can be used to set the debug level to a new value
	
	example:
	
	\code
	
	void A::f()
	{
		// ...
	
		exCLASS_SET_DEBUG(100) ; // from now on the debug level is 100
		
		// ...
	} 
	
	\endcode

	\see exCLASS_DECLARE_DEBUG_LEVEL, exCLASS_DEFINE_DEBUG_LEVEL, exCLASS_INIT_DEBUG, exCLASS_BGN_DEBUG, exCLASS_END_DEBUG, exCLASS_GET_DEBUG
	
	\note It is provided in case you would like to change your debug level runtime.
*/

#define exCLASS_SET_DEBUG(newlevel)		setInt(exCDebugLevel,exCLASS_SET_DEBUG_I(newlevel))





/*!	\brief Deprecated use ACS_CLASS_GET_DEBUG()

	\def exCLASS_GET_DEBUG()

    \deprecated use ACS_CLASS_GET_DEBUG() instead

	The macro can be used to get the debug level
	
	example:
	
	\code
	
	void A::f()
	{
		// ...
		
		cout << "Debug level is: " << exCLASS_GET_DEBUG()
	
		// ...
	} 
	
	\endcode
	
	\see exCLASS_DECLARE_DEBUG_LEVEL, exCLASS_DEFINE_DEBUG_LEVEL, exCLASS_INIT_DEBUG, exCLASS_BGN_DEBUG, exCLASS_END_DEBUG, exCLASS_SET_DEBUG

	\note It is provided in case you would like to change your debug level runtime.
*/
// PRQA S 1020,1023 2
#define exCLASS_GET_DEBUG()				exCLASS_GET_DEBUG_I(asInt(exCDebugLevel))



/*!	\brief  Deprecated use ACS_CLASS_END_DEBUG()

	\def exCLASS_END_DEBUG 

    \deprecated use ACS_CLASS_END_DEBUG() instead 
    
	The macro can be used to finish conditioned debug statements.
	
	example:
	
	\code
	
	void A::f()
	{
		// ...
	
		exCLASS_BGN_DEBUG(1) // debug level is 1 or greater
			cerr << "A::f:" << __LINE__
				 << " count: " << count << '\n' ;
		exCLASS_END_DEBUG
		
		// ...
		
	} 
	
	\endcode
	
	\see exCLASS_DECLARE_DEBUG_LEVEL, exCLASS_DEFINE_DEBUG_LEVEL, exCLASS_INIT_DEBUG, exCLASS_BGN_DEBUG, exCLASS_SET_DEBUG, exCLASS_GET_DEBUG
*/
// PRQA S 1020,1023 2
#define exCLASS_END_DEBUG ; exCLASS_END_DEBUG_D() ; }



/*!	\brief Deprecated use ACS_CLASS_DEFINE_DEBUG_LEVEL(classname)

	\def exCLASS_DEFINE_DEBUG_LEVEL(classname)

    \deprecated use ACS_CLASS_DEFINE_DEBUG_LEVEL(classname) instead
    
	The macro can be used to define the class debug static member
	
	example:
	
	\code
	
	// class A implementation
	
	exCLASS_DEFINE_DEBUG_LEVEL(A) // define the debug static member on class A
	
	void A::A() // A ctor
	{
		// ...
	} 
	
	\endcode
	
	\see exCLASS_DECLARE_DEBUG_LEVEL, exCLASS_INIT_DEBUG, exCLASS_BGN_DEBUG, exCLASS_SET_DEBUG, exCLASS_GET_DEBUG
*/
// PRQA S 1020,1023 2
#define exCLASS_DEFINE_DEBUG_LEVEL(classname)	acs::exRTDebuggable classname::exCDebugLevel(# classname"_DEBUG");



/*!	\brief Deprecated use ACS_CLASS_INIT_DEBUG(env)

	\def exCLASS_INIT_DEBUG(env)

    \deprecated use ACS_CLASS_INIT_DEBUG(env) instead 
    
	The macro can be used to initialize the class debug static member.
	Because this macro uses getenv(), its performances have been optimized to
	ensure that for each class, only one getenv will be called for each session.
	After the call, if the environment variable was set, the static member will
	be initialized to that value (atoi() call), else the value will be set to -1 
	and no further getenv will be attempt on that static member.
	The best place to put the macro is in every costructor. You can leave out the copy 
	constructor, because it is called with an argument of the same type and, the latter 
	shall be constructed in some way.
	
	example:
	
	\code
	
	// class A implementation
	
	exCLASS_DEFINE_DEBUG_LEVEL(A) // define the debug static member on class A
	
	void A::A() // A ctor
	{
		// get the debug level for class A from 
		// A_DEBUG environment variable
		exCLASS_INIT_DEBUG(A_DEBUG) ; 
		
		// ...
	} 
	
	\endcode

	\see exCLASS_DECLARE_DEBUG_LEVEL, exCLASS_DEFINE_DEBUG_LEVEL, exCLASS_BGN_DEBUG, exCLASS_SET_DEBUG, exCLASS_GET_DEBUG

	\note If the class has public static members, and you want to debug them, because they can be invoked
	even if no object of the class has been created, you should ensure proper initialization
	of the debug static member. To do so, the best pratical way is to put an initialization 
	line, as in the example above as the first statement in every public static member function.
	If you have some performance issue and you know, by design that a public
	static member function should be called on top of the other, is enough to put the initialization line 
	as the first statement of that member.

*/
// PRQA S 1020,1023,3380 2 
#define exCLASS_INIT_DEBUG(env)	{ if (asInt(exCDebugLevel) == 0) { const char *exDbgGE = getenv(# env); setInt(exCDebugLevel,exCLASS_INIT_DEBUG_I(acs::getDebugValueFromEnv(exDbgGE))) ; } }

/*! @} */

/*! @} */



/*! \addtogroup utility
 *  @{
 */


/** 
 * \brief returns the debug value from an env var of the form 'env'=\<debug level as int\>
 */
inline int getDebugValueFromEnv(const char *env) ACS_NOEXCEPT // PRQA S 2134
{
    return ( 0 != env) ? std::atoi(env) : -1  ; // PRQA S 3232,3382,3385 
}


/*! \addtogroup Deprecated 
 *  @{
 */
 
/**
 * \brief returns a string with the following info: compact function name :(line number)  (NOTE, only the function name not the full signature is shown)
 */
#define exDebugShortSignature __FUNCTION__ << ":(" << __LINE__ << ") " 

/**
 * \brief returns a string with the following info: compact function name :(line number)  (NOTE, only the function name not the full signature is shown)
 */
#define exDebugShortSignatureFile "[" << __FILE__ << ": " << __FUNCTION__ << ":(" << __LINE__ << ")] " 


/**
 * \brief returns a string with the following info: [thread]compact function name :(line number) 
 */
#define exDebugShortSignatureThread "[" << pthread_self() << "]" << __FUNCTION__ << ":(" << __LINE__ << ") "


/**
 * \brief returns a string with the following info: [file (line number) at: function name]
 */

#define exDebugFullSignature "[" << __FILE__<<" ("<<__LINE__ <<") at: "<<__PRETTY_FUNCTION__<<"] "

/*! @} */

/*!	\mainpage
	This library is intended to support exception handling, diagnostic and 
	and ordinary output and, global and class level debugging.
	It provides facilities for logfiles and for unordinary stream handling.
*/

/**
 * \brief returns a string with the following info: "<class name if any>::<function name>(...)[line number]"
 * 
 * \code
 * @ functionName(...):[68] 
 * 
 * \endcode
 */
#define SimpleDebugSignature "@ " << acs::utility::FuncName_(__PRETTY_FUNCTION__) << ":[" << __LINE__ << "] " 


/*! \addtogroup Deprecated 
 *  @{
 */
 
/**
 * \brief Deprecated use SimpleDebugSignature
 * \deprecated use SimpleDebugSignature instead
 */
#define exDebugSignature SimpleDebugSignature
/*! @} */
/**
 * \brief returns a string with the following info: "{thread}<class name if any>::<function name>(...)[line number]"
 * 
 * \code
 * {139792106669856} @ A<T_>::functionVoidName():[40] 
 * 
 * \endcode
 */
#define SimpleDebugSignatureThread "{" << pthread_self() << "} @ " << acs::utility::FuncName_(__PRETTY_FUNCTION__) << ":[" << __LINE__ << "] " 


/**
 * \brief alias for SimpleDebugSignatureThread
 */
#define exDebugSignatureThread SimpleDebugSignatureThread 


/**
 * \brief returns a string with the following info: "<extended function signature>:[line number]"
 * \code
 * @ std::vector<std::pair<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> > >, std::allocator<std::pair<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> > > > > A<T_>::functionVoidName() [with T_ = int]:[44] 
 * 
 * \endcode
 */
#define DebugSignature "@ " << __PRETTY_FUNCTION__ << ":[" << __LINE__ << "] " 

/**
 * \brief returns a string with the following info: "{thread}<extended function signature>:[line number]"
 * \code
 * {139692843018016} @ std::ostream& functionName(T_, std::ostream&) [with T_ = int]:[77]
 * 
 * \endcode
 */
#define DebugSignatureThread "{" << pthread_self() << "} @ " << __PRETTY_FUNCTION__ << ":[" << __LINE__ << "] " 

/**
 * \brief returns a string with the following info: [file (line number) at: function name]
 * \code
 * @ macros.C:[83] in: std::ostream& functionName(T_, std::ostream&) [with T_ = int]
 * 
 * \endcode
 */
#define DebugFullSignature "@ " << __FILE__<<":["<<__LINE__ <<"] in: "<<__PRETTY_FUNCTION__

/**
 * \brief returns a string with the following info: [file (line number) at: function name]
 * \code
 * {139692843018016} @ macros.C:[83] in: std::ostream& functionName(T_, std::ostream&) [with T_ = int]
 * 
 * \endcode
 */
#define DebugFullSignatureThread "{" << pthread_self() << "} @ " << __FILE__<<":["<<__LINE__ <<"] in: "<<__PRETTY_FUNCTION__

/*! @}*/


/*!	\brief class level debug convenience macro
 * 
 * \def ACS_CLASS_BGN_DEBUG(level)
 * 
 * The macro can be used to put under debug some statements.
 * 
 * example:
 * 
 * \code
 * 
 *    void f()
 *    {
 *       // ...
 *     
 *       ACS_CLASS_BGN_DEBUG(10) // debug level is 10 or greater
 *          cerr << SimpleDebugSignatureThread
 *               << " count: " << count 
 *               << '\n' ;
 *       ACS_CLASS_END_DEBUG
 *     
 *       // ...
 *    } 
 * 
 * \endcode
 * 
 */
#define ACS_CLASS_BGN_DEBUG(level) if (asInt(exCDebugLevel) >= (level)) {


/*!	\brief class level debug convenience macro
 * 
 * \def ACS_FUNC_BGN_DEBUG(c,level)
 * 
 * The macro can be used to put under debug some statements.
 * 
 * example:
 * 
 * \code
 * 
 *    void f()
 *    {
 *       // ...
 *     
 *       ACS_FUNC_BGN_DEBUG(A,10) // debug level is 10 or greater
 *          ACS_LOG_DEBUG(" count: " << count ) ;
 *       ACS_FUNC_END_DEBUG
 *     
 *       // ...
 *    } 
 * 
 * \endcode
 * 
 */
#define ACS_FUNC_BGN_DEBUG(c,level) if (asInt(c::exCDebugLevel) >= (level)) {

/*!	\brief class level debug convenience macro
 * 
 * \def ACS_CLASS_END_DEBUG
 * 
 * The macro can be used to finish conditioned debug statements.
 * 
 * example:
 * 
 * \code
 *    
 *    void A::f()
 *    {
 *       // ...
 *    
 *       ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB) // debug level is 1 or greater
 *          ACS_LOG_DEBUG(" count: " << count) ;
 *       ACS_CLASS_END_DEBUG
 *    
 *       // ...
 *    
 *    } 
 *    
 * \endcode
 * 
 */
#define ACS_CLASS_END_DEBUG }

/*!	\brief class level debug convenience macro
 * 
 * \def ACS_FUNC_END_DEBUG
 * 
 * The macro can be used to finish conditioned debug statements.
 * 
 * example:
 * 
 * \code
 *    
 *    void f()
 *    {
 *       // ...
 *    
 *       ACS_FUNC_BGN_DEBUG(ACS_LOW_VERB) // debug level is 1 or greater
 *          ACS_LOG_DEBUG("This is a log" ) ;
 *       ACS_FUNC_END_DEBUG
 *    
 *       // ...
 *    
 *    } 
 *    
 * \endcode
 * 
 */
#define ACS_FUNC_END_DEBUG }




/*!	\brief class level debug convenience macro
 * 
 * \def ACS_CLASS_GET_DEBUG()
 * 
 * The macro can be used to get the debug level
 * 
 * example:
 * 
 * \code
 * 
 *    void A::f()
 *    {
 *       // ...
 *    
 *       cout << "Debug level is: " << ACS_CLASS_GET_DEBUG()
 *    
 *       // ...
 *    } 
 *    
 * \endcode
 * 
 * \note It is provided in case you would like to know your debug level runtime.
 */
#define ACS_CLASS_GET_DEBUG() asInt(exCDebugLevel)



/*!	\brief class level debug convenience macro
 * 
 * \def ACS_FUNC_GET_DEBUG(c)
 * 
 * The macro can be used to get the debug level
 * 
 * example:
 * 
 * \code
 * 
 *    void ()
 *    {
 *       // ...
 *    
 *       cout << "Debug level of class A is: " << ACS_CLASS_GET_DEBUG(A)
 *    
 *       // ...
 *    } 
 *    
 * \endcode
 * 
 * \note It is provided in case you would like to know your debug level runtime.
 */
#define ACS_FUNC_GET_DEBUG(c) asInt(c::exCDebugLevel)




/*!	\brief class level debug convenience macro
 * 
 * \def ACS_CLASS_WRITE_DEBUG(v, m)
 * 
 * The macro can be used to announce the execution when the devug level is v or greater with a parameter that will printed along the announce.
 * 
 * example:
 * 
 * \code
 * 
 *    // class A implementation
 *    
 *    
 *    void A::A() // A ctor
 *    {
 *       // ...
 *    
 *       ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Valore: " << a << " -.") 
 *       // ...
 *    } 
 *    
 *    // will write:
 *    @ A::A():[43] Valore: 0 -.
 + \endcode
 */
#define ACS_CLASS_WRITE_DEBUG(v,m)	\
ACS_CLASS_BGN_DEBUG(v) \
	ACS_LOG_DEBUG(SimpleDebugSignature << m ) ;\
ACS_CLASS_END_DEBUG


/*!	\brief class level debug convenience macro
 * 
 * \def ACS_FUNC_WRITE_DEBUG(c, v, m)
 * 
 * The macro can be used to announce the execution when the debug level is v or greater with a parameter that will printed along the announce.
 * 
 * example:
 * 
 * \code
 * 
 *    // class A implementation
 *    
 *    
 *    void A::A() // A ctor
 *    {
 *       // ...
 *    
 *       // ...
 *    } 
 *    void f () {
 *      ACS_FUNC_WRITE_DEBUG(A,ACS_MID_VERB, "Valore: " << 0 << " -.") ;
 *    }
 *    // will write:
 *    @ f():[51] Valore: 0 -.
 + \endcode
 */
#define ACS_FUNC_WRITE_DEBUG(c,v,m)	\
ACS_FUNC_BGN_DEBUG(c,v) \
	ACS_LOG_DEBUG(SimpleDebugSignature << m ) ;\
ACS_FUNC_END_DEBUG

/*!	\brief class level debug convenience macro
 * 
 * \def ACS_CLASS_ANNOUNCE_DEBUG(v)
 * 
 * The macro can be used to announce the execution when the devug level is v or greater.
 * 
 * example:
 * 
 * \code
 * 
 *    // class A implementation
 *    
 *    
 *    void A::A() // A ctor
 *    {
 *       // ...
 *    
 *       ACS_CLASS_ANNOUNCE_DEBUG(ACS_MID_VERB)
 *       // ...
 *    } 
 *    
 *    // will write:
 *    @ A::A():[42] 
 *    
 * \endcode
 * 
 */
#define ACS_CLASS_ANNOUNCE_DEBUG(v)	\
ACS_CLASS_BGN_DEBUG(v) \
	ACS_LOG_DEBUG(SimpleDebugSignature) ; \
ACS_CLASS_END_DEBUG


/*!	\brief class level debug convenience macro
 * 
 * \def ACS_FUNC_ANNOUNCE_DEBUG(c,v)
 * 
 * The macro can be used to announce the execution when the devug level is v or greater.
 * 
 * example:
 * 
 * \code
 * 
 *    // class A implementation
 *    
 *    
 *    void A::A() // A ctor
 *    {
 *       // ...
 *    
 *       // ...
 *    } 
 *    
 *    void f() {
 *       ACS_FUNC_ANNOUNCE_DEBUG(A,ACS_MID_VERB) ;
 *    }
 *    // will write:
 *    @ f():[123] 
 *    
 * \endcode
 * 
 */
#define ACS_FUNC_ANNOUNCE_DEBUG(c,v)	\
ACS_FUNC_BGN_DEBUG(c,v) \
	ACS_LOG_DEBUG(SimpleDebugSignature) ; \
ACS_FUNC_END_DEBUG

/*!	\brief class level debug convenience macro
 * 
 * \def ACS_CLASS_ANNOUNCE_DEBUG_TH(v)
 * 
 * The macro can be used to announce the execution when the devug level is v or greater - the thread id information is added.
 * 
 * example:
 * 
 * \code
 * 
 *    // class A implementation
 *    
 *    
 *    void A::A() // A ctor
 *    {
 *       // ...
 *    
 *       ACS_CLASS_ANNOUNCE_DEBUG_TH(1)
 *       // ...
 *    } 
 *    
 *    will write: 
 *    {139741887911712} @ A::A():[45] 
 * \endcode
 * 
 */
#define ACS_CLASS_ANNOUNCE_DEBUG_TH(v)	\
ACS_CLASS_BGN_DEBUG(v) \
	ACS_LOG_DEBUG(SimpleDebugSignatureThread) ; \
ACS_CLASS_END_DEBUG



/*!	\brief class level debug convenience macro
 * 
 * \def ACS_FUNC_ANNOUNCE_DEBUG_TH(c,v)
 * 
 * The macro can be used to announce the execution when the devug level is v or greater - the thread id information is added.
 * 
 * example:
 * 
 * \code
 * 
 *    // class A implementation
 *    
 *    
 *    void A::A() // A ctor
 *    {
 *       // ...
 *    
 *       // ...
 *    } 
 *    
 *    void f() {
 *       ACS_FUNC_ANNOUNCE_DEBUG(A,ACS_MID_VERB) ;
 *    }
 *    
 *    will write: 
 *    {139741887911712} @ f():[124] 
 * \endcode
 * 
 */
#define ACS_FUNC_ANNOUNCE_DEBUG_TH(c,v)	\
ACS_FUNC_BGN_DEBUG(c,v) \
	ACS_LOG_DEBUG(SimpleDebugSignatureThread) ; \
ACS_FUNC_END_DEBUG



/*!	\brief class level debug convenience macro
 * 
 * \def ACS_CLASS_WRITE_DEBUG_TH(v, m)
 * 
 * The macro can be used to announce the execution when the devug level is v or greater with two parameters that will printed along the announce.
 * 
 * example:
 * 
 * \code
 * 
 *    // class A implementation
 *    
 *    
 *    void A::A() // A ctor
 *    {
 *       // ...
 *    
 *       ACS_CLASS_WRITE_DEBUG_TH(ACS_VLO_VERB, "Valore: " << a << " -.")  
 *       // ...
 *    } 
 *    
 *    // will write:
 *    {139741887911712} @ A::A():[43] Valore: 0 -.
 * \endcode
 */
#define ACS_CLASS_WRITE_DEBUG_TH(v,m)	\
ACS_CLASS_BGN_DEBUG(v) \
	ACS_LOG_DEBUG(SimpleDebugSignatureThread << m ) ; \
ACS_CLASS_END_DEBUG 


/*!	\brief class level debug convenience macro
 * 
 * \def ACS_FUNC_WRITE_DEBUG_TH(c, v, m)
 * 
 * The macro can be used to announce the execution when the devug level is v or greater with two parameters that will printed along the announce.
 * 
 * example:
 * 
 * \code
 * 
 *    void A::A() // A ctor
 *    {
 *       // ...
 *    
 *       // ...
 *    } 
 *    
 *    void f() {
 *       ACS_CLASS_WRITE_DEBUG_TH(A, ACS_VLO_VERB, "Valore: " << a << " -.")  
 *       // ...
 *    } 
 *    
 *    // will write:
 *    {139741887911712} @ f():[43] Valore: 0 -.
 * \endcode
 */
#define ACS_FUNC_WRITE_DEBUG_TH(c,v,m)	\
ACS_FUNC_BGN_DEBUG(c,v) \
	ACS_LOG_DEBUG(SimpleDebugSignatureThread << m ) ; \
ACS_FUNC_END_DEBUG 

/*! \brief the class is used to create a manipulator-like inserter function \see explain */ 
  class _ExplainEx { // PRQA S 2173, 2175
  public:
    _ExplainEx()  ACS_NOEXCEPT  : _M_s(){} 
    _ExplainEx(const _ExplainEx &r_)  ACS_NOEXCEPT  : _M_s(r_._M_s) {} 
    ~_ExplainEx() ACS_NOEXCEPT {} 
    _ExplainEx &operator=(const _ExplainEx &r_) ACS_NOEXCEPT { if (&r_!= this) { _M_s = r_._M_s ; } return *this ; } // PRQA S 4075
  	  std::string _M_s ; // PRQA S 2100
  } ;
  
  

/*! \addtogroup exceptions 
 *  @{
 */
   
/*! 
 * \fn explain(const std::exception &) 
 * \brief std::exception inserter (manipulator)
 * the inserter can be used to insert std::exception classes and their derived into the standard streams
 * this allow also the stack trace if the class support it.
 * Example:
 * \code
 *    try {
 *       ACS_THROW(IOException("Cannot open file: " + filename, errno)) ;
 *    }
 *    catch(std::exception &x) 
 *    {
 *       std::cerr << explain(x) ;
 *    }
 * \endcode
 * resulting in:
 * \code
 *    acs::ex::IOException @ handlers.C:[42] in myClass::methodCanThrow() thread "" [140667347773216]
 *    Cannot open file: unexistentFile
 *    stack trace
 *    /raid0/pds2/components/libAcsBase/test/ex/../../libAcsBase.so.3.13 ( acs::ex::Exception::Exception(std::string const&, int) )
 *    ./handlers ( myClass::methodCanThrow() )
 *    ./handlers ( f(int, char**) )
 * \endcode
 */
  _ExplainEx explain(const std::exception &e_) ACS_NOEXCEPT ;

  _ExplainEx explain(const std::exception &e_, bool stack_) ACS_NOEXCEPT ;

/*! @} */

/*! 
 * \brief std::exception and derived classes manipulator inserter 
 *
 * \see explain
 */
std::ostream &operator<<(std::ostream &s_, const _ExplainEx &e_) ACS_NOEXCEPT ;

_ACS_END_NAMESPACE

#include <exRTDebuggable.h>

#endif // _exStream_H_
