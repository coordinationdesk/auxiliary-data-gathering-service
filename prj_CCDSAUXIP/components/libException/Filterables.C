// PRQA S 1050 EOF
/*

	Copyright 1995-2022, Exprivia SpA - DFDA
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. Exception Library $

	$Id$

	$Author$

	$Log$
	Revision 5.10  2015/03/12 12:43:15  marpas
	introducing exFWStatus and exFEStatus filterables along their macros ACS_LOG_WSTATUS and ACS_LOG_ESTATUS respectively.
	The sysinfo log handler uses LOG_WARNING and LOG_ERR prio for them.
	
	Revision 5.9  2014/05/15 13:27:35  marfav
	exFProgress filterable added
	
	Revision 5.8  2014/02/07 18:03:07  marpas
	introducing ACS_THROW and ACS_COND_THROW macros to substitute throwException and throwConditionedException
	documentation improved and tests aligned
	
	Revision 5.7  2013/11/18 17:03:06  marpas
	exFStatus filterable added along its macro
	
	Revision 5.6  2013/11/15 13:19:30  marpas
	introducing the isFilterable concept into an exostream.
	It is used when inserting an exception into a Filterable: in this case the formatter will be invoked with the forFilterable bool true, indicating that probably the text should not have an header that will be presumably applied to the filterabel where the exception has been inserted
	
	Revision 5.5  2013/11/13 18:03:56  marpas
	implementing abstract method clear to clean filterable content
	
	Revision 5.4  2013/09/27 14:05:17  marpas
	streams have names
	
	Revision 5.3  2013/09/26 18:17:43  marpas
	housekeeping, types renaming and cleanup
	
	Revision 5.2  2013/09/26 18:09:34  marpas
	robustness strongly improved:
	singleton functions (with static variables inside) used
	some private members are now accessed only through the above technique
	exRTDebuggable classes do not longer unregster themselves from exRTDebug.
	it was a logic error: sooner or later the container class (exRTDebug) will throw away memory in any case.
	The above changes will lead in a more efficient process closure.
	moreover the exException class now uses the reentrant method strerror_t to obtain the errno explaination.
	
	Revision 5.1  2013/06/05 15:52:29  marpas
	Filterables.C and Filterables.h introduced to collect all standard filterables.
	all header files are kept and they simply include the Filterables.h one, while their respective .C files have been removed
	
	
*/

#include <Filterables.h>
#include <exMsgFmtFactory.h>

_ACS_BEGIN_NAMESPACE(acs)


//
//
// exFMessage
//
//
exFMessage::exFMessage(const std::string &n) :
    exostream(n),
    exFilterable()
{
    setIsFilterable(true) ;
}

std::ostream &exFMessage::stream() 
{
	Lock lock(globalMutex()) ;
	if (_osLocked) { // was locked ? means that the info have been extracted
		_os.reset(nullptr) ;
		_osLocked = false ;
		_details = "" ;
	}
	if (!_os) { 
		_os = std::make_unique<std::ostringstream>() ; // PRQA S 2707 # see clear method and dtor
		_os->exceptions(std::ios_base::badbit | std::ios_base::failbit) ;
	}
	// at this point, the stream will be recreated if null
	return *_os ;
}

void exFMessage::clear() 
{
	Lock const lock(globalMutex()) ;

	_os.reset(nullptr) ;
	_osLocked = false ;
	_details = "" ;
}


std::string exFMessage::text() // PRQA S 4020
{
	Lock lock(globalMutex()) ;
	if (!_os) {
		return "" ;
	}
	
	try {
		// lock the internal stream 
		insertionLock() ;
		return exMsgFmtFactory::instance()->text(*this) ;
	}
	catch (const std::exception &) { 
		_osLocked = false ;
		throw ; // rethrow
	}
}

std::string exFMessage::message() const // PRQA S 4020
{
	Lock lock(globalMutex()) ;
	if (_os) {
		return _os->str() ;
	}
	return "" ;
}

std::string exFMessage::detail() const 
{
	return _details ;
}


void exFMessage::detail(const std::string &s)  
{
	stream() ; // to unlock if the object is to be reused
	_details = s ;
}

bool exFMessage::notifyException(const std::exception &x) noexcept
{
	exostream::notifyException(x) ; // notify to the base class
	auto e = dynamic_cast<const exException *>(&x) ; // PRQA S 3081
	if (e && ! e->stack().empty()) { // add details if any
		detail(e->stack()) ;
    }
	return true ;
}


void exFMessage::insertionLock() 
{
	_osLocked= true ;
}

bool exFMessage::defaultNotify() const
{
	return true ;
}



//
//
// exFPrivateInfo
//
//

exFPrivateInfo::exFPrivateInfo(const std::string &n) :
    exFMessage(n)
{
}




//
//
// exFStatus
//
//

exFStatus::exFStatus(const std::string &n) :
    exFMessage(n)
{
}



//
//
// exFDebug
//
//

exFDebug::exFDebug(const std::string &n) :
    exFMessage(n) 
{
}


//
//
// exFWarning
//
//

exFWarning::exFWarning(const std::string &n) :
    exFMessage(n) 
{
}


//
//
// exFWStatus
//
//

exFWStatus::exFWStatus(const std::string &n) :
    exFWarning(n)
{
}



//
//
// exFError
//
//
exFError::exFError(const std::string &n) :
    exFMessage(n)
{
}


//
//
// exFEStatus
//
//

exFEStatus::exFEStatus(const std::string &n) :
    exFError(n)
{
}

 

exFEvent::exFEvent(const std::string &key, const std::string &value, const std::string &n) :
    exFMessage(n),
	_key(key),
	_value(value)
{
	*this << _key << ": " << _value ;
	if (!_value.empty()) {
		*this << " " ;
    }
}

exFEvent::exFEvent(const std::string &key, int value, const std::string &n) :
    exFMessage(n),
	_key(key)
{
	std::ostringstream os ;
	os << value ;
	_value = os.str() ;
	*this << _key << ": " << _value << " " ;
}

const std::string &exFEvent::key() const // PRQA S 4120
{
	return _key ;
}


const std::string &exFEvent::value() const // PRQA S 4120
{
	return _value ;
}




//
//
// exFOpAlert
//
//
exFOpAlert::exFOpAlert(const std::string &n) : 
    exFMessage(n) 
{
}


//
//
// exFOpEventAlert
//
//
exFOpEventAlert::exFOpEventAlert(const std::string &key, const std::string &value, const std::string &n) :
	exFEvent(key, value, n)
{
}

exFOpEventAlert::exFOpEventAlert(const std::string &key, int value, const std::string &n) :
	exFEvent(key, value, n)
{
}


//
//
// exFProgress
//
//

exFProgress::exFProgress(const std::string &n) :
    exFMessage(n) 
{
}

_ACS_END_NAMESPACE
