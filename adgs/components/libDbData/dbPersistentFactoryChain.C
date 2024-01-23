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

	$Prod: A.C.S. Db Data Library $

	$Id$

	$Author$

	$Log$
	Revision 5.2  2014/02/07 18:39:00  marpas
	ACS_THROW and ACS_COND_THROW macros adopted
	
	Revision 5.1  2013/07/22 17:46:10  marpas
	enforcing use of dbkey string - deprecting older methods
	
	Revision 5.0  2013/06/13 14:53:05  marpas
	adopting libException 5.x standards
	removing deprecated use of dbConnectionMgr
	general quality improvements
	best practices applied
	
	Revision 2.12  2013/02/28 14:06:46  marpas
	coding best practices applied
	interfaces rationalization
	libMessage dependencies removed
	qa warnings
	
	Revision 2.11  2013/02/26 18:04:37  marpas
	coding best practices applied
	interface rationalization
	
	Revision 2.10  2013/01/25 11:39:33  marpas
	using pattern::Singleton from libException
	
	Revision 2.9  2013/01/22 17:37:07  marpas
	coding best practices
	interfaces rationalization
	singletons use SingletonTemplate
	qa rules
	efficiency slightly improved
	
	Revision 2.8  2012/11/16 13:40:45  marpas
	HEAD REALIGNEMENT
	
	Revision 2.6  2012/02/21 10:50:29  marpas
	refactoring
	
	Revision 2.5  2012/02/17 14:09:26  marpas
	refactoring
	multiple db design
	less warning from compiler
	
	Revision 2.4  2012/02/13 16:51:16  marpas
	refactoring in progress
	
	Revision 2.3  2009/07/14 12:02:00  matteo.airoldi
	added new define to overrun unsupported cast on RHEL3 (pig action to many RHEL 3 define)
	
	Revision 2.2  2009/05/14 11:49:16  marpas
	some rules fixing
	comments makeup
	
	Revision 2.1  2006/05/11 20:41:33  marpas
	constant strings changed from std::string to const char * to avoid std::string repository problem with shared objects
	
	Revision 2.0  2006/02/28 08:47:38  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.14  2005/11/03 09:54:19  marpas
	dbOperation01Factory class added and used
	
	Revision 1.13  2005/10/20 18:03:57  marpas
	embedded declaration of a static instance of dbDataFactory removed.
	It is now reerenced explicitelly into libCryosatApp.CryosatApp.C
	
	Revision 1.12  2005/01/03 21:26:12  marpas
	headers fixed
	
	Revision 1.11  2004/09/15 14:03:36  marpas
	thread safety, debug messages improved
	
	Revision 1.10  2004/09/02 18:21:30  marpas
	no exceptions is notified when unknown objtype is queried for
	
	Revision 1.9  2004/09/01 17:23:39  marpas
	objtype concept introduced
	
	Revision 1.8  2004/09/01 13:14:55  marpas
	*** empty log message ***
	
	Revision 1.7  2004/04/07 17:00:28  marpas
	dbQueryWrapperMT & dbGeneralQueryIF classes added
	
	Revision 1.6  2004/03/08 23:00:55  marpas
	singleton relese at exit conditioned by the compilation flag RELEASE_SINGLETON_AT_EXIT
	
	Revision 1.5  2003/07/11 13:31:01  marpas
	exException::clone implemented
	
	Revision 1.4  2003/04/30 14:06:05  paoscu
	using namespace std removed.
	
	Revision 1.3  2003/03/14 18:04:31  paoscu
	Moved dbDataFactory instance
	
	Revision 1.2  2003/01/27 17:53:26  paoscu
	DEstructors called bt atexit
	
	Revision 1.1.1.1  2002/11/29 17:19:30  marpas
	Import libDbData
		

*/

#include <dbPersistentFactoryChain.h>
#include <dbOperation01Factory.h>
#include <exStream.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)


using namespace std;


ACS_CLASS_DEFINE_DEBUG_LEVEL(dbPersistentFactoryChain);
const char * dbPersistentFactoryChain::_unknown = "Unknown" ;

dbOperation01Factory *dbPersistentFactoryChain::_theDbOperationFactory = 0 ;

void dbPersistentFactoryChain::registerFactory(string const & key, dbPersistent* (*builder)(const std::string &) ) // PRQA S 4020
{
	Lock lock(_mutex) ;

#ifndef EL3	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "key is \""<<key<<"\". builder address is \""<<builder<<"\".") ;
#endif
	map<string, dbPersistent* (*)(const string &)>::iterator it1;
	it1= _map.find(key); //see if the key is already registered in _map
	if(it1 != _map.end() )
	{
		//check if it is registered in _OldMap as well
		map<string, deque<dbPersistent* (*)(const string &)> >::iterator it2;
		it2 = _oldMap.find(key);
		if(it2 != _oldMap.end() )  //the list in _oldMap is already done: just insert the
		{                          //current builder (that is it1->second)
			// check if the b builder already exists:
			if (find(it2->second.begin(), it2->second.end(), builder) != it2->second.end()) {
#ifdef EL3
				ACS_LOG_WARNING("dbPersistentFactoryChain attempt to register multiple builder(" << hex << (void *)builder << ") for " << key) ;
#else
				ACS_LOG_WARNING("dbPersistentFactoryChain attempt to register multiple builder(" << hex << builder << ") for " << key) ;
#endif
				return ;
			}
			
			it2->second.push_front(it1->second);
		}
		else  //build a new deque, fill it  and  insert it in _oldMap
		{
			deque<dbPersistent* (*)(const string &)> tmpDeq;
			tmpDeq.push_front(it1->second);    
			_oldMap.insert( pair<string, deque<dbPersistent* (*)(const string &)> >(key,tmpDeq) );
		}
		
		//At last upade the builder in _map
		_map[key]=builder;
	}
	else //just insert a new item in the map.
	{
		_map.insert( pair<string, dbPersistent* (*)(const string &) >(key,builder) );
	}
}


void dbPersistentFactoryChain::unregisterFactory(string const & key, dbPersistent* (*builder)(const string &) ) // PRQA S 4020
{
	Lock lock(_mutex) ;

#ifndef EL3	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "key is \""<<key<<"\". builder address is \""<<builder<<"\".") ;
#endif
	map<string, dbPersistent* (*)(const string &)>::iterator it1; // ********  _map iterator ******************* 
	it1= _map.find(key); //see if the key is already registered in _map
	if(it1 == _map.end() )
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "key is not present.") ;		
		return ; //the builder is not present.
	}

	//Build a temporary deque
	deque<dbPersistent* (*)(const string &)> tmpDeq;

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "temp deque built.") ;


	//check if there's the key in _oldMap as well
	map<string, deque<dbPersistent* (*)(const string &)> >::iterator it2; // ********  _oldMap iterator ******************* 
	it2 = _oldMap.find(key);
	if(it2 != _oldMap.end() )  //There's a replacements deque
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "There are old versions.") ;		
		tmpDeq=it2->second;
	}	
	
	
	//Push the current builder in front
	tmpDeq.push_front(it1->second);    
	
	//now I'm going to work only on tmpDeq
	deque<dbPersistent* (*)(const string &)>::iterator it3 = tmpDeq.begin();
	while( (it3 !=  tmpDeq.end() ) && (*it3 != builder ) ) {
		++it3 ;
    }
	if(it3 ==  tmpDeq.end() )
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "The builder is not present.") ;		
		return; //the builder was not present.
	}
	tmpDeq.erase(it3); 

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "The builder was erased from temp deque.") ;

	if(!tmpDeq.empty()) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "The temp deque is not empty.") ;		
		_map[key]=*(tmpDeq.begin()); //put into _map the first function in tmpDeq
		tmpDeq.pop_front();
	}
	else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "The temp deque is empty.") ;		
		_map.erase(it1); // There are no more builder for that key
	}
	

	if(!tmpDeq.empty()) {	
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "The temp deque is not empty.") ;		
		_oldMap[key]=tmpDeq;
	}
	else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "The temp deque is empty.") ;		
		if (it2 != _oldMap.end()) {
			_oldMap.erase(it2); // There are no more builder for that key
        }
	}
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "END.") ;

}



void dbPersistentFactoryChain::registerObjType(string const & key, const string &type)
{
	Lock lock(_mutex) ;
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "key is \""<<key<<"\". type is \""<<type<<"\".") ;

	map<string, string>::iterator it1;
	it1= _objTypes.find(key); //see if the key is already registered in _map
	if(it1 != _objTypes.end() )
	{
		//check if it is registered in _OldMap as well
		map<string, deque<string> >::iterator it2;
		it2 = _oldObjTypes.find(key);
		if(it2 != _oldObjTypes.end() )  //the list in _oldMap is already done: just insert the
		{                         	   //current builder (that is it1->second)
			it2->second.push_front(it1->second);
		}
		else  //build a new deque, fill it  and  insert it in _oldMap
		{
			deque<string> tmpDeq;
			tmpDeq.push_front(it1->second);    
			_oldObjTypes.insert( pair<string, deque<string> >(key,tmpDeq) );
		}
		
		//At last upade the builder in _map
		_objTypes[key]=type;
	}
	else //just insert a new item in the map.
	{
		_objTypes.insert( pair<string, string >(key,type) );
	}
}

void dbPersistentFactoryChain::unregisterObjType(string const & key, const string &type ) // PRQA S 4020
{
	Lock lock(_mutex) ;
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"key is \""<<key<<"\". type is \""<< type <<"\".") ;
	map<string, string>::iterator it1; // ********  _map iterator ******************* 
	it1= _objTypes.find(key); //see if the key is already registered in _map
	if(it1 == _objTypes.end() )
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "key is not present.") ;		
		return ; //the builder is not present.
	}

	//Build a temporary deque
	deque<string> tmpDeq;

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"temp deque built.") ;


	//check if there's the key in _oldMap as well
	map<string, deque<string> >::iterator it2; // ********  _oldMap iterator ******************* 
	it2 = _oldObjTypes.find(key);
	if(it2 != _oldObjTypes.end() )  //There's a replacements deque
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "There are old versions.") ;		
		tmpDeq=it2->second;
	}	
	
	//Push the current type in front
	tmpDeq.push_front(it1->second);    
	
	//now I'm going to work only on tmpDeq
	deque<string>::iterator it3 = tmpDeq.begin();
	while( (it3 !=  tmpDeq.end() ) && (*it3 != type ) ) {
		++it3;
    }
	if(it3 ==  tmpDeq.end() )
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "The type is not present.") ;		
		return; //the type was not present.
	}
	tmpDeq.erase(it3); 

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"The type was erased from temp deque.") ;

	if(!tmpDeq.empty())	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "The temp deque is not empty.") ;		
		_objTypes[key]=*(tmpDeq.begin()); //put into _map the first function in tmpDeq
		tmpDeq.pop_front();
	}
	else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "The temp deque is empty.") ;		
		_objTypes.erase(it1); // There are no more type for that key
	}
		
	if(!tmpDeq.empty()) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "The temp deque is not empty.") ;		
		_oldObjTypes[key]=tmpDeq;
	}
	else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "The temp deque is empty.") ;
		if (it2 != _oldObjTypes.end()) {
			_oldObjTypes.erase(it2); // There are no more type for that key
        }
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "END.") ;

}




dbPersistent *dbPersistentFactoryChain::newObject(string const & key, const string &dbKey)
{
	Lock lock(_mutex) ;
	
	map<string, dbPersistent* (*)(const string &)>::iterator it= _map.find(key);
	if(it == _map.end() )
	{
		ACS_THROW(KeyNotFound(" no registered type for \""+key+"\".")) ; // PRQA S 3081
	}
#ifndef EL3
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"key is \""<<key<<"\". builder address is \""<<it->second<<"\".");
#endif
	return ( *(it->second ) )(dbKey);
}




std::string dbPersistentFactoryChain::getObjType(string const & key) // PRQA S 4020
{
	Lock lock(_mutex) ;
	
	map<string, string>::iterator it= _objTypes.find(key) ;
	try {
		if(it == _objTypes.end() )
		{
			ACS_THROW(KeyNotFound("key \""+key+"\" is unknown.")) ; // PRQA S 3081
		}
	}
	catch(KeyNotFound &e) {
		return unknownObjType() ;
	}
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"key is \""<<key<<"\". type is \""<<it->second<<"\".") ;
	return it->second ;
}


void dbPersistentFactoryChain::getAllObjType(std::vector<std::string> &l) 
{
	Lock lock(_mutex) ;
	
	l.clear() ;
	for (map<string, string>::iterator it= _objTypes.begin(); it != _objTypes.end(); ++it) { // PRQA S 4238
		if (find(l.begin(), l.end(), it->second) == l.end()) {
			l.push_back(it->second) ;
		}
	}
}


dbPersistentFactoryChain::dbPersistentFactoryChain() :
    Singleton<dbPersistentFactoryChain >(),
	_map(),
	_objTypes (),
	_oldMap(),
	_oldObjTypes(),
    _mutex()
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"exists now. " << this) ;
}


dbPersistentFactoryChain::~dbPersistentFactoryChain() throw() 
{
    // PRQA S 4631 L1
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "this: " <<  this) ; 
    // PRQA L:L1		
}

string dbPersistentFactoryChain::unknownObjType() const 
{
	return _unknown ;	
}


void registerDbPersistentFactoryMethod(string const & key, dbPersistent* (*builder)(const string &) )
{
	dbPersistentFactoryChain::instance()->registerFactory(key,builder);
}


void unregisterDbPersistentFactoryMethod(string const & key, dbPersistent* (*builder)(const string &) )
{
	dbPersistentFactoryChain::instance()->unregisterFactory(key,builder);
}

void registerDbPersistentObjType(string const & key, const string &type )
{
	dbPersistentFactoryChain::instance()->registerObjType(key,type);
}


void unregisterDbPersistentObjType(string const & key, const string &type )
{
	dbPersistentFactoryChain::instance()->unregisterObjType(key,type);
}


_ACS_END_NAMESPACE
