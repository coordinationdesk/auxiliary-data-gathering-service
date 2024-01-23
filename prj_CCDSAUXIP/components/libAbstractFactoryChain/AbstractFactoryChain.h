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

	$Prod: A.C.S. Abstract Factory Library $

	$Id$

	$Author$

	$Log$
	Revision 3.6  2018/01/09 16:40:49  marpas
	qa warnings fixed
	
	Revision 3.5  2017/12/14 09:41:43  marfav
	S2PDGS-1900
	ECICMF-75
	Debug messaging improved
	
	Revision 3.4  2017/12/13 14:29:57  marfav
	ECICMF-75
	Added handles for default builder management and registration at ctor time
	
	Revision 3.3  2017/11/27 18:03:55  marpas
	qa warnings fixing in progress
	
	Revision 3.2  2017/08/31 09:55:40  marfav
	Debug messages cleaned
	
	Revision 3.1  2017/08/31 09:49:08  marfav
	Added debug messages support
	
	Revision 3.0  2016/11/21 16:02:25  marfav
	Major release
	
	Revision 2.17  2016/11/21 16:02:12  marfav
	S3MPM-847
	Adding interfaces to dump factory chain builders registrations
	
	Revision 2.16  2016/11/17 15:58:36  marfav
	Added support to named builders
	
	Revision 2.15  2016/05/11 09:44:35  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 2.14  2013/12/16 19:26:29  marpas
	coding best practice applied
	
	Revision 2.13  2013/07/07 17:34:12  marpas
	coding best practices applied
	qa warnings removed
	some compilation warning removed
	
	Revision 2.12  2013/04/18 17:20:44  marpas
	qacpp can now analize friends - ifdef PRQA_ANALYZER
	
	Revision 2.11  2013/01/25 11:25:19  marpas
	using pattern::Singleton from libException
	
	Revision 2.10  2012/06/11 16:14:59  marpas
	prqa intrumentation
	
	Revision 2.9  2012/06/11 14:28:31  marpas
	prqa instrumentation
	
	Revision 2.8  2012/06/06 15:01:38  marfav
	Removed inner class derived from std::map
	
	Revision 2.7  2012/05/18 15:16:15  marpas
	virtual non-throwing dtor added
	
	Revision 2.6  2012/02/21 13:21:00  marpas
	removing compiler warnings
	
	Revision 2.5  2011/04/21 14:22:10  giucas
	Added checks to avoid remotion of elements from empty deques and maps in unregister operation
	
	Revision 2.4  2011/03/01 16:18:52  marpas
	GCC 4.4.x support
	
	Revision 2.3  2011/01/24 13:02:21  marpas
	exDECLARE_EXCEPTION libException(3.8) is now inlined and can be used inside templates
	
	Revision 2.2  2010/09/13 16:28:02  marpas
	libException 3.4 interface adopted
	
	Revision 2.1  2009/03/11 18:45:06  marpas
	new exceptions copy ctor added
	
	Revision 2.0  2006/02/28 10:08:02  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2004/12/03 17:42:50  marfav
	Header fixed
	
	Revision 1.1.1.1  2004/04/07 13:28:31  marfav
	Import libAbstractFactoryChain
	

*/

#ifndef _AbstractFactoryChain_H_
#define _AbstractFactoryChain_H_

#include <acs_c++config.hpp>
#include <exException.h>
#include <Singleton.hpp>
#include <Filterables.h>

#include <map>
#include <deque>
#include <string>
#include <functional>
#include <algorithm>



_ACS_BEGIN_NAMESPACE(acs)

using pattern::Singleton ;

class AbstractFactoryChainDumpableInterface
{
public:
	virtual ~AbstractFactoryChainDumpableInterface() throw() {} // PRQA S 2131

	virtual void dumpCapabilities() const = 0;

protected:
	AbstractFactoryChainDumpableInterface() {}
	ACS_CLASS_DECLARE_DEBUG_LEVEL(AbstractFactoryChainDumpableInterface) // PRQA S 2101

private:
	// Not implemented
	AbstractFactoryChainDumpableInterface (const AbstractFactoryChainDumpableInterface&);
	AbstractFactoryChainDumpableInterface& operator= (const AbstractFactoryChainDumpableInterface&);
		
};

class FactoryChainSingletonHelper : public Singleton <FactoryChainSingletonHelper> // PRQA S 2109, 2153
{
public:
	virtual ~FactoryChainSingletonHelper() throw () {} // PRQA S 2131

	void addFactoryChainPtr (AbstractFactoryChainDumpableInterface * theptr);
	std::vector<AbstractFactoryChainDumpableInterface *> getFactoryList() const;

	void dumpCapabilities() const;

protected:
	FactoryChainSingletonHelper();

private:
	// Declared but not implemented
	FactoryChainSingletonHelper (const FactoryChainSingletonHelper&);
	FactoryChainSingletonHelper& operator= (const FactoryChainSingletonHelper&);

	friend FactoryChainSingletonHelper* Singleton< FactoryChainSingletonHelper >::instance(); // PRQA S 2107

private:
	std::vector<AbstractFactoryChainDumpableInterface *> _singletonsList;
	ThreadSafe _myMutex;
};


template <typename K>
class DefaultAbstractFactoryChainPredicate // PRQA S 2109
{
public:
	explicit DefaultAbstractFactoryChainPredicate (K key) : _key(key) {} // PRQA S 2010
	bool operator () (K key)
	{
		return (key == _key);
	}

	DefaultAbstractFactoryChainPredicate(const DefaultAbstractFactoryChainPredicate& a) :
        _key (a._key)
	{
		*this = a;
	}

	DefaultAbstractFactoryChainPredicate&
	operator= (const DefaultAbstractFactoryChainPredicate& a)
	{
		if (this != &a)
		{
			_key = a._key;
		}
		return *this;
	}
	virtual ~DefaultAbstractFactoryChainPredicate() throw () {} // PRQA S 2131
private:
    DefaultAbstractFactoryChainPredicate() ; // not implemented

private:
	K _key;
};


//class AbstractFactoryChain;
template <typename O, typename K = std::string, typename P = DefaultAbstractFactoryChainPredicate<K> ,  typename B = void>
// ObjectsType (returned by builders), Keytype, Predicate (extended equals between K objects),  Builder parameters

class AbstractFactoryChain : public AbstractFactoryChainDumpableInterface // PRQA S 2108, 2109
{
public:
	/*! class KeyNotFoundException declaration */
	exDECLARE_EXCEPTION(KeyNotFoundException,exException) ; // PRQA S 2502

	/*! class IllegalOperationException declaration */
	exDECLARE_EXCEPTION(IllegalOperationException,exException) ; // PRQA S 2502

	typedef P predicateType;
	typedef K keyType;
	typedef B builderParameter;
	typedef O objectType;
	typedef objectType* (*BuilderPointer)(const builderParameter&);
	typedef objectType* (*BuilderPointerParameterless)();

public:
    virtual ~AbstractFactoryChain() throw() {} // PRQA S 2131
	void registerFactory(const keyType &, BuilderPointer, const std::string& builderId = "");
	void unregisterFactory(const keyType &, BuilderPointer);
	objectType *newObject(const keyType &, const builderParameter&);
	BuilderPointer findKey (const keyType &);

	// Show the registered keys with the associated builder Ids
	virtual void dumpCapabilities() const;

	// Associate a default builder to be used in case the key is not found
	void setDefaultBuilder (BuilderPointer, const std::string& builderId = "");

private:
	BuilderPointer getDefaultBuilder () const;


protected:
	// the init function will be overriden by the specialized classes
	// by default does nothing
	void init() {}

	template <typename X> class predicateWrapper // PRQA S 2109, 2171, 2173, 2175
	{
	public:
		explicit predicateWrapper (const keyType& key) : _pred(key) {}
        predicateWrapper (const predicateWrapper& r) : _pred(r._pred) {}
        predicateWrapper &operator= (const predicateWrapper& r) {
            if (this != &r) {
                _pred = r._pred ;
            }
            return *this ;
        }
        ~predicateWrapper() throw() {} // PRQA S 2131

		bool operator () (const std::pair<keyType, X> &element)
		{
			return _pred(element.first);
		}
    private:
        predicateWrapper() ; // not implemented
	private:
		predicateType _pred;
	};

	AbstractFactoryChain() : AbstractFactoryChainDumpableInterface(), _builderIds(), _map(), _oldMap(), _defaultBuilder(0), _mutex()
	{
		FactoryChainSingletonHelper::instance()-> addFactoryChainPtr (this);
		init();
	}

    typename std::map <keyType, BuilderPointer>::iterator extendedFind (const keyType& key)
    {
		ThreadSafe::Lock lock(_mutex);
		// The predicate is in charge to detect and manage the "wildcard" used in the map
		return std::find_if (_map.begin(), _map.end(), predicateWrapper<BuilderPointer> (key)); // PRQA S 3081
    }

private:
    AbstractFactoryChain(const AbstractFactoryChain& ) ; // not implemented
    AbstractFactoryChain& operator=(const AbstractFactoryChain& ) ; // not implemented

private:
	std::map < BuilderPointer, std::string > _builderIds; // PRQA S 2101 3
    std::map < keyType, BuilderPointer > _map;
    std::map < keyType, std::deque<BuilderPointer> > _oldMap;

	BuilderPointer _defaultBuilder;

	ThreadSafe _mutex;

	friend AbstractFactoryChain<O,K,P,B>* Singleton< AbstractFactoryChain<O,K,P,B> >::instance(); // PRQA S 2107
};

template <typename O, typename K, typename P, typename B >
void AbstractFactoryChain<O,K,P,B>::dumpCapabilities() const
{
	// Thread safe implementation
	ThreadSafe::Lock lock(_mutex);

	typename std::map < keyType, BuilderPointer >::const_iterator mapIt;

	// Loop on the map elements and dump the associations	
	mapIt = _map.begin();
	while (mapIt != _map.end())
	{
		std::string theBuilderId = _builderIds.find(mapIt->second)->second;
		ACS_LOG_INFO ("  Key [" << mapIt->first << "] - BuilderId [" << theBuilderId << "]");
		++mapIt;
	}

	// Search also for the default builder (if any)
	BuilderPointer defPtr = getDefaultBuilder();
	if (0 == defPtr)
	{
		ACS_LOG_INFO ("  No default builder set");
	}
	else
	{	
		if (_builderIds.find(defPtr) != _builderIds.end())
		{
			ACS_LOG_INFO ("  Default BuilderId [" << _builderIds.find(defPtr)->second << "]");
		}
		else
		{
			ACS_LOG_INFO ("  Default BuilderId [Anonymous]");
		}
	}

}

template <typename O, typename K, typename P, typename B >
void AbstractFactoryChain<O,K,P,B>::registerFactory (const keyType &key, BuilderPointer builder, const std::string& builderId)
{
	// Thread safe implementation
	ThreadSafe::Lock lock(_mutex);


	ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Registering now Key [" << key << "] - BuilderId [" << builderId << "] - Address [" << reinterpret_cast<void*>(builder) << "]"); // PRQA S 3081

    typename std::map < keyType, BuilderPointer >::iterator it1;
    
	it1= _map.find(key); //see if the key is already registered in _map
	if(it1 != _map.end() )
	{
		//check if it is registered in _OldMap as well
		typename std::map < keyType, std::deque <BuilderPointer> >::iterator it2;
		it2 = _oldMap.find(key);
		if(it2 != _oldMap.end() )  //the list in _oldMap is already done: just insert the
		{                          //current builder (that is it1->second)
			it2->second.push_front(it1->second);
		}
		else  //build a new deque, fill it  and  insert it in _oldMap
		{
			std::deque<BuilderPointer> tmpDeq;
			tmpDeq.push_front(it1->second);
			_oldMap[key] = tmpDeq;
		}
		
		//At last upade the builder in _map
		_map[key]=builder;
	}
	else //just insert a new item in the map.
	{
		_map[key]=builder;
	}

	// At the end keep track of the builderId associated to the builder pointer to function
	if (builderId != "")
	{
		_builderIds[builder] = builderId;
	}
	else
	{
		_builderIds[builder] = exStackTrace::demangle( typeid(BuilderPointer).name() );
	}

	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)			
		    dumpCapabilities() ;
	ACS_CLASS_END_DEBUG

}

template <typename O, typename K, typename P, typename B >
void AbstractFactoryChain<O,K,P,B>::unregisterFactory (const keyType &key , BuilderPointer builder) // PRQA S 4020
{
	// Thread safe implementation
	ThreadSafe::Lock lock(_mutex);

    typename std::map < keyType, BuilderPointer >::iterator it1;

	it1= _map.find(key); //see if the key is already registered in _map
	if(it1 == _map.end() )
	{
		return ; //the builder is not present.
	}

	//Build a temporary deque
	std::deque<BuilderPointer> tmpDeq;



	//check if there's the key in _oldMap as well
	typename std::map < keyType, std::deque <BuilderPointer> >::iterator it2;

	it2 = _oldMap.find(key);
	if(it2 != _oldMap.end() )  //There's a replacements deque
	{
		tmpDeq=it2->second;
	}	
	
	//Push the current builder in front
	tmpDeq.push_front(it1->second);
	
	//now I'm going to work only on tmpDeq
	typename std::deque<BuilderPointer>::iterator it3 = tmpDeq.begin();
	while( (it3 !=  tmpDeq.end() ) && (*it3 != builder ) ) {
		it3++;
    }
	if(it3 ==  tmpDeq.end() )
	{
		return; //the builder was not present.
	}
	tmpDeq.erase(it3);


	if(tmpDeq.size() != 0)
	{	
		_map[key]=*(tmpDeq.begin()); //put into _map the first function in tmpDeq

		// Remove the first element if and only if there is sometingh to remove!
		tmpDeq.pop_front();
	}
	else
	{
		_map.erase(it1); // There are no more builder for that key
	}
		
	if(tmpDeq.size() != 0)
	{	
		_oldMap[key]=tmpDeq;
	}
	else
	{
		if (it2 != _oldMap.end())
		{
			_oldMap.erase(it2); // There are no more builder for that key
		}
	}

	// Should I remove the builderId from the list?
	// TODO

}

template <typename O, typename K, typename P, typename B >
typename AbstractFactoryChain<O,K,P,B>::BuilderPointer AbstractFactoryChain<O,K,P,B>::findKey (const keyType &key)
{
	// Thread safe implementation
	ThreadSafe::Lock lock(_mutex);

	ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Searching for Key [" << key << "]");


	typename AbstractFactoryChain<O,K,P,B>::BuilderPointer theBuilder = 0;
    typename std::map < keyType, BuilderPointer >::iterator it = extendedFind (key);
    
	if(it == _map.end() )
	{
		ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Key not found. Sarching for a default builder");
		theBuilder = getDefaultBuilder();
		if (0 == theBuilder)
		{
			// This is a failure/ Throw the exception
			std::ostringstream error;
			error << "key \"" << key << "\" is unknown.";
			KeyNotFoundException e(error.str());
			ACS_THROW(e);
		}

	}
	else
	{
		// Found in the map
		theBuilder = (it->second);
	}

	ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Found pointer at address [" << reinterpret_cast<void*>(theBuilder)  << "] - ID [" << _builderIds[theBuilder] << "]"); // PRQA S 3081

	return theBuilder;
}

template <typename O, typename K, typename P, typename B >
typename AbstractFactoryChain<O,K,P,B>::objectType *AbstractFactoryChain<O,K,P,B>::newObject(const keyType& key, const builderParameter& param)
{
	// Thread safe implementation
	ThreadSafe::Lock lock(_mutex);

	return ( *(findKey(key) ) )(param);
}



// Default builder management
template <typename O, typename K, typename P, typename B >
void AbstractFactoryChain<O,K,P,B>::setDefaultBuilder(BuilderPointer b, const std::string& builderId)
{

	// Thread safe implementation
	ThreadSafe::Lock lock(_mutex);
	
	ACS_CLASS_WRITE_DEBUG (ACS_LOW_VERB, "Setting default builder [" << reinterpret_cast<void*>(b)  << "] - ID [" << builderId << "]"); // PRQA S 3081
	_defaultBuilder = b;

	// At the end keep track of the builderId associated to the builder pointer to function
	if (builderId != "")
	{
		_builderIds[b] = builderId;
	}
	else
	{
		_builderIds[b] = exStackTrace::demangle( typeid(BuilderPointer).name() );
	}

	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)			
		    dumpCapabilities() ;
	ACS_CLASS_END_DEBUG
}

template <typename O, typename K, typename P, typename B >
typename AbstractFactoryChain<O,K,P,B>::BuilderPointer AbstractFactoryChain<O,K,P,B>::getDefaultBuilder() const
{
	// Thread safe implementation
	ThreadSafe::Lock lock(_mutex);
	
    // returns a copy of the member 
	return _defaultBuilder; // PRQA S 4628
}


/***************************************************

Partial specialization of AbstractFactoryChain
to manage parameterless builders....

***************************************************/

template <typename O, typename K, typename P>
class AbstractFactoryChain <O,K,P,void> : public AbstractFactoryChain <O,K,P,void*> // PRQA S 2109, 2193
{

public:
	typedef P predicateType;
	typedef K keyType;
	typedef O objectType;
	typedef objectType* (*BuilderPointerParameterless)();
	typedef BuilderPointerParameterless BuilderPointer;

public:
	virtual ~AbstractFactoryChain() throw() {} // PRQA S 2131

	void registerFactory(const  keyType &, BuilderPointerParameterless, const std::string& builderId = ""); // PRQA S 2121 3
	void unregisterFactory(const keyType &, BuilderPointerParameterless);
	objectType *newObject(const keyType &);

	// Show the registered keys with the associated builder Ids
	void dumpCapabilities() const;

	// Associate a default builder to be used in case the key is not found
	void setDefaultBuilder (BuilderPointerParameterless, const std::string& builderId = "");

protected:
	AbstractFactoryChain() : AbstractFactoryChain<O,K,P,void*> ()
	{
		init();
	}
#ifndef PRQA_ANALYZER
	friend AbstractFactoryChain<O,K,P,void>* Singleton< AbstractFactoryChain<O,K,P,void> >::instance();
#endif

protected:
	// the init function will be overriden by the specialized classes
	// by default does nothing
	void init() {}

private:
    AbstractFactoryChain(const AbstractFactoryChain &) ; // not implemented
    AbstractFactoryChain& operator= (const AbstractFactoryChain &) ; // not implemented

};

#ifndef PRQA_ANALYZER

template <typename O, typename K, typename P>
void AbstractFactoryChain<O,K,P,void >::dumpCapabilities () const
{
	// Base class has the mutex
	AbstractFactoryChain <O,K,P,void*>::dumpCapabilities();
}


template <typename O, typename K, typename P>
void AbstractFactoryChain<O,K,P,void >::registerFactory (const keyType &key, BuilderPointerParameterless builder, const std::string& builderId)
{
	if (builderId.empty() )
	{
		std::string readType;
		try
		{
#if __cplusplus > 199711L
			std::unique_ptr<objectType> p ( (*builder)() );
#else
			std::auto_ptr<objectType> p ( (*builder)() );
#endif
			readType = exStackTrace::demangle(typeid ( *p ).name());
		}
		catch (exException&)
		{ 
			readType="";
		}

		// Base class has the mutex
		AbstractFactoryChain <O,K,P,void*>::registerFactory(key, typename AbstractFactoryChain <O,K,P,void*>::BuilderPointer( builder), readType );
	}
	else
	{
		// Base class has the mutex
		AbstractFactoryChain <O,K,P,void*>::registerFactory(key, typename AbstractFactoryChain <O,K,P,void*>::BuilderPointer( builder), builderId );
	}

}

template <typename O, typename K, typename P>
void AbstractFactoryChain<O,K,P,void>::unregisterFactory (const keyType &key, BuilderPointerParameterless builder)
{
		// Base class has the mutex
		AbstractFactoryChain <O,K,P,void*>::unregisterFactory(key, typename AbstractFactoryChain <O,K,P,void*>::BuilderPointer( builder) );
}

template <typename O, typename K, typename P>
typename AbstractFactoryChain<O,K,P,void>::objectType *AbstractFactoryChain<O,K,P,void>::newObject(const keyType& key)
{
	// Base class has the mutex
	typename AbstractFactoryChain <O,K,P,void*>::BuilderPointer p = AbstractFactoryChain <O,K,P,void*>::findKey (key);
	BuilderPointerParameterless pp = BuilderPointerParameterless(p) ;
	return (*pp)();
}

// Default builder management
template <typename O, typename K, typename P>
void AbstractFactoryChain<O,K,P,void>::setDefaultBuilder(BuilderPointerParameterless b, const std::string& builderId)
{
	// Base class has the mutex
	// Convert the builder pointer type and set it in the base class
	typename AbstractFactoryChain <O,K,P,void*>::BuilderPointer p = typename AbstractFactoryChain <O,K,P,void*>::BuilderPointer(b);
	AbstractFactoryChain <O,K,P,void*>::setDefaultBuilder (p, builderId);
}


#endif //PRQA

_ACS_END_NAMESPACE


#endif // _AbstractFactoryChain_H_
