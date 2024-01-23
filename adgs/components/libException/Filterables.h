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
	Revision 5.13  2015/06/23 15:51:44  marpas
	minor changes
	
	Revision 5.12  2015/06/16 14:45:30  marpas
	removing useless qa suppressions
	
	Revision 5.11  2015/03/12 12:43:15  marpas
	introducing exFWStatus and exFEStatus filterables along their macros ACS_LOG_WSTATUS and ACS_LOG_ESTATUS respectively.
	The sysinfo log handler uses LOG_WARNING and LOG_ERR prio for them.
	
	Revision 5.10  2014/05/15 13:27:35  marfav
	exFProgress filterable added
	
	Revision 5.9  2013/11/28 10:07:54  marpas
	qa warnings removed
	
	Revision 5.8  2013/11/19 17:51:24  marpas
	using  exostream::getDefaultErrLogger()
	
	Revision 5.7  2013/11/18 17:03:06  marpas
	exFStatus filterable added along its macro
	
	Revision 5.6  2013/11/13 18:03:56  marpas
	implementing abstract method clear to clean filterable content
	
	Revision 5.5  2013/10/03 10:57:16  marpas
	minor changes
	
	Revision 5.4  2013/09/27 14:05:17  marpas
	streams have names
	
	Revision 5.3  2013/06/10 16:13:59  marpas
	using and enforcing new standards
	
	Revision 5.2  2013/06/07 13:24:42  marpas
	minor changes
	
	Revision 5.1  2013/06/05 15:52:29  marpas
	Filterables.C and Filterables.h introduced to collect all standard filterables.
	all header files are kept and they simply include the Filterables.h one, while their respective .C files have been removed
	

*/

#ifndef _Filterables_H_
#define _Filterables_H_ 

#include <acs_c++config.hpp>
#include <exFilterable.h>
#include <exStream.h>
#include <FEConvenienceValues.h>

_ACS_BEGIN_NAMESPACE(acs)


/*! \class exFMessage
	
	\brief A filterable class whose category is Message
	
	A filterable class to log messages.
*/
class exFMessage : // PRQA S 2109,2153, 2156
	public exostream,
	public exFilterable
{
public:
	explicit exFMessage(const std::string &n="exFMessage") ; // PRQA S 4634
	virtual ~exFMessage() ACS_NOEXCEPT ;
	
	// inherited from exFilterable
	// It uses the exMsgFmtFactory available on top of the stack to format the msg
	virtual std::string text() ;
	
    /*- clears any previous content*/
    virtual void clear() ;

	// returns the unformatted msg
	virtual std::string message() const ;
	
	// returns the details
	virtual std::string detail() const ;

	// sets the details
	virtual void detail(const std::string &) ;

	// inherited from ostream 
	virtual std::ostream &stream() ;
	
	// it put the exception stack if any into the details
	virtual bool __attribute__((deprecated)) notifyException(const std::exception &) ACS_NOEXCEPT ;
	
	
	/*! Inherited from exFilterable, prevents appending of more messages after the class has been inserted into a stream.
	 */
	virtual void insertionLock() ;
	
	virtual bool defaultNotify() const ;
private:
	// declared but not implemented to prevent their use
	exFMessage(const exFMessage &) ;
	exFMessage &operator=(const exFMessage &) ;
private:	
	std::ostringstream *_os ;
	bool _osLocked ;	
	std::string _details ;
} ;


/*! \class exFStatus
	\brief Status category filterable message
	
	A filterable class to log messages to be used to communicate Private category messages.
*/
class exFStatus : public exFMessage // PRQA S 2153
{
public:
	explicit exFStatus(const std::string &n="exFStatus") ;  // PRQA S 4634 
	virtual ~exFStatus() ACS_NOEXCEPT ;
	
private:
	// declared but not implemented to prevent their use
	exFStatus(const exFStatus &) ;
	exFStatus &operator=(const exFStatus &) ;
} ;

/*! \class exFPrivateInfo
	\brief Debug category filterable message
	
	A filterable class to log messages to be used to communicate Private category messages.
*/
class exFPrivateInfo : public exFMessage // PRQA S 2153
{
public:
	explicit exFPrivateInfo(const std::string &n="exFPrivateInfo") ;  // PRQA S 4634
	virtual ~exFPrivateInfo() ACS_NOEXCEPT ;
	
private:
	// declared but not implemented to prevent their use
	exFPrivateInfo(const exFPrivateInfo &) ;
	exFPrivateInfo &operator=(const exFPrivateInfo &) ;
} ;

/*! \class exFDebug
	\brief Debug category filterable message
	
	A filterable class to log messages to be used to communicate Debug category messages.
*/
class exFDebug : public exFMessage // PRQA S 2153
{
public:
	explicit exFDebug(const std::string &n="exFDebug") ;  // PRQA S 4634
	virtual ~exFDebug() ACS_NOEXCEPT ;
	
private:
	// declared but not implemented to prevent their use
	exFDebug(const exFDebug &) ;
	exFDebug &operator=(const exFDebug &) ;
} ;



/*! \class exFWarning
	
	\brief filterable object whose category is Warning.
	
	A filterable class to log messages with an initial and ending tag 
	to be used to communicate Warning messages.
*/
class exFWarning : public exFMessage // PRQA S 2153
{
public:
	explicit exFWarning(const std::string &n="exFWarning") ;  // PRQA S 4634
	virtual ~exFWarning() ACS_NOEXCEPT ;
		
private:
	// declared but not implemented to prevent their use
	exFWarning(const exFWarning &) ;
	exFWarning &operator=(const exFWarning &) ;
} ;

/*! \class exFWStatus
	\brief Warning Status category filterable message
	
	A filterable class to log messages to be used to communicate Private category messages.
*/
class exFWStatus : public exFWarning // PRQA S 2153
{
public:
	explicit exFWStatus(const std::string &n="exFWStatus") ;  // PRQA S 4634
	virtual ~exFWStatus() ACS_NOEXCEPT ;
	
private:
	// declared but not implemented to prevent their use
	exFWStatus(const exFWStatus &) ;
	exFWStatus &operator=(const exFWStatus &) ;
} ;



/*! \class exFError
	
	\brief Error category filterable message
	
	A filterable class to log messages to be used to communicate Error messages.
*/
class exFError : public exFMessage // PRQA S 2153
{
public:
	explicit exFError(const std::string &n="exFError") ;  // PRQA S 4634
	virtual ~exFError() ACS_NOEXCEPT ;
	
private:
	// declared but not implemented to prevent their use
	exFError(const exFError &) ;
	exFError &operator=(const exFError &) ;
} ;

/*! \class exFEStatus
	\brief Error Status category filterable message
	
	A filterable class to log messages to be used to communicate Private category messages.
*/
class exFEStatus : public exFError // PRQA S 2153
{
public:
	explicit exFEStatus(const std::string &n="exFEStatus") ;  // PRQA S 4634
	virtual ~exFEStatus() ACS_NOEXCEPT ;
	
private:
	// declared but not implemented to prevent their use
	exFEStatus(const exFEStatus &) ;
	exFEStatus &operator=(const exFEStatus &) ;
} ;



/*! \class exFOpAlert
	
	\brief a filterable object type whose category is Operator Alert.
	
	A filterable class to log messages in a special category allowing handlers to filter them in
	order to show high priority messages to an operator.
*/
class exFOpAlert : public exFMessage // PRQA S 2153
{
public:
	explicit exFOpAlert(const std::string &n="exFOpAlert") ;  // PRQA S 4634
	virtual ~exFOpAlert() ACS_NOEXCEPT ;
	
private:
	// declared but not implemented to prevent their use
	exFOpAlert(const exFOpAlert &) ;
	exFOpAlert &operator=(const exFOpAlert &) ;
} ;

/*! \class exFEvent
	
	\brief A filterable class whose category is Event

	A filterable class to log messages having "key: value" capability.
*/
class exFEvent : // PRQA S 2109,2153
	public exFMessage
{
public:
	exFEvent(const std::string &key, const std::string &value, const std::string &n="exFEvent") ;  // PRQA S 4634 2
	exFEvent(const std::string &key, int value, const std::string &n="exFEvent") ; 
	virtual ~exFEvent() ACS_NOEXCEPT ;
	const std::string &key() const ;
	const std::string &value() const ;

private:
	// declared but not implemented to prevent their use
	exFEvent() ;
	exFEvent(const exFEvent &) ;
	exFEvent &operator=(const exFEvent &) ;
private:
	std::string _key ;
	std::string _value ;
} ;

/*! \class exFOpEventAlert
	
	\brief filterable object having "name: value" capability.

	A filterable class to log messages of the type key: value. Often this kind of message
	is used to send messages with an event attached or just to send events with an empty message.
*/
class exFOpEventAlert : // PRQA S 2153
	public exFEvent
{
public:
	explicit exFOpEventAlert(const std::string &key, const std::string &value = "", const std::string &n="exFOpEventAlert") ;  // PRQA S 4634 2
	exFOpEventAlert(const std::string &key, int value,  const std::string &n="exFOpEventAlert") ; 
	virtual ~exFOpEventAlert() ACS_NOEXCEPT ;

private:
	// declared but not implemented to prevent their use
	exFOpEventAlert() ;
	exFOpEventAlert(const exFOpEventAlert &) ;
	exFOpEventAlert &operator=(const exFOpEventAlert &) ;
} ;



/*! \class exFProgress
	
	\brief filterable object whose category is Progress
	
	A filterable class to log messages with an initial and ending tag 
	to be used to communicate Progress messages.
*/
class exFProgress : public exFMessage // PRQA S 2153
{
public:
	explicit exFProgress(const std::string &n="exFProgress") ;   // PRQA S 4634
	virtual ~exFProgress() ACS_NOEXCEPT ;
	
private:
	// declared but not implemented to prevent their use
	exFProgress (const exFProgress &) ;
	exFProgress &operator=(const exFProgress &) ;
} ;

_ACS_END_NAMESPACE

#endif // _Filterables_H_
