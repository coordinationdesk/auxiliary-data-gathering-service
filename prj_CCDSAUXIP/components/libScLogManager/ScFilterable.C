// PRQA S 1050 EOF
/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Scheduler Log Manager Library $

	$Id$

	$Author$

	$Log$
	Revision 5.3  2013/11/14 09:16:22  marpas
	implementing abstract clear method()
	
	Revision 5.2  2013/09/27 15:27:30  marpas
	streams have name
	
	Revision 5.1  2013/09/26 19:34:36  marpas
	adopting libException Rev_5_16 interfaces
	test improved
	still doubles messages
	
	Revision 5.0  2013/07/07 18:27:50  marpas
	adopting libException 5.x standards
	coding best practices applied
	
	Revision 1.4  2009/06/26 10:49:15  marpas
	some classes do not throw any exception from their dtors
	
	Revision 1.3  2004/12/03 17:55:32  marfav
	Header fixed
	
	Revision 1.2  2004/09/27 13:30:05  marfav
	insertionLock method added
	
	Revision 1.1  2004/09/23 15:17:19  marfav
	New object hierarchy structure
	

*/


#include <ScFilterable.h>
#include <sstream>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;

//
//
// ScFilterable
//
//
ScFilterable::ScFilterable(const string &name) :
    exFilterable(),
    exostream(name),
	_os(0),
	_osLocked(false),
	_originatorTaskName (""),
	_orderSignature ("")
{
}

ScFilterable::ScFilterable(const string &name, const string& originator) :
    exFilterable(),
    exostream(name),
	_os(0),
	_osLocked(false),
	_originatorTaskName (originator),
	_orderSignature("")
{
}


ScFilterable::~ScFilterable() throw()
{
	delete _os ; // PRQA S 4631
}

ostream &ScFilterable::stream()
{
	Lock lock(globalMutex()) ;
	if (_osLocked) {
		delete _os ;
		_os = 0 ;
		_osLocked = false ;
	}
	
	return *(_os ? _os : _os = new ostringstream) ; // PRQA S 3383, 3385
}


string ScFilterable::text() // PRQA S 4020
{
	Lock lock(globalMutex()) ;
	if (!_os) {
		return "" ;
    }

	// lock the internal stream
	_osLocked = true ;
	return message();
}

void ScFilterable::clear() 
{
	Lock lock(globalMutex()) ;

	// delete the stream and unlock
	delete _os ;
	_os = 0 ;
	_osLocked = false ;
}




string ScFilterable::message() const // PRQA S 4020
{
	Lock lock(globalMutex()) ;
	if (_os)
	{
		string msg = _os->str();
		// Remove the last '\n' if any
		if ( msg[msg.length()-1] == '\n') { // PRQA S 3084
			msg = msg.substr (0, msg.length()-1); // PRQA S 3084
        }
		return msg ;
	}
	return "" ;
}

void ScFilterable::insertionLock()
{
	if (_os) {
		_osLocked=true;
    }
}


void ScFilterable::setOriginatorTaskName (const string& source)
{
	Lock lock(globalMutex()) ;
	_originatorTaskName = source;
}

string ScFilterable::getOriginatorTaskName () const
{
	Lock lock(globalMutex()) ;
	return _originatorTaskName;
}

void ScFilterable::setOrderSignature (const string& s)
{
	Lock lock(globalMutex()) ;
	_orderSignature = s;
}

string ScFilterable::getOrderSignature () const
{
	Lock lock(globalMutex()) ;
	return _orderSignature;
}

_ACS_END_NAMESPACE
