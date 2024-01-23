// 
// 	Copyright 1995-2014, Advanced Computer Systems , Inc.
// 	Via Della Bufalotta, 378 - 00139 Roma - Italy
// 	http://www.acsys.it
// 
// 	All Rights Reserved.
// 
// 	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
// 	the contents of this file may not be disclosed to third parties, copied or
// 	duplicated in any form, in whole or in part, without the prior written
// 	permission of Advanced Computer Systems, Inc.
// 
// 	$Prod: A.C.S. PackagerOptimiser Singleton Class $
// 
// 	$Id$
// 
// 	$Author$
// 	
//  $Log$
//  Revision 2.6  2014/05/13 16:03:59  lucio.pulvirenti
//  S2PDGS-760: New debug macros used.
//
//  Revision 2.5  2013/02/07 15:09:12  enrcar
//  EC:: Updated to handle the new interface of the PackagerOptimiser
//
//


#include <PackagerOptimiserSingleton.h>
#include <dbPackagerOptimiserFactory.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(PackagerOptimiserSingleton);

void PackagerOptimiserSingleton::registerFactory(string const & key, PackagerOptimiser* (*builder)(PACKAGEROPT_ARGS) )
{
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"key is \""<<key<<"\". builder address is \""<< reinterpret_cast <void*> (builder) <<"\"."); // PRQA S 3081

	map<string, PackagerOptimiser* (*)(PACKAGEROPT_ARGS)>::iterator it1;
	it1= _mapOfOptimisers.find(key); //see if the key is already registered in _mapOfOptimisers
	if(it1 != _mapOfOptimisers.end() )
	{
		//check if it is registered in _oldMapOfOptimisers as well
		map<string, deque<PackagerOptimiser* (*)(PACKAGEROPT_ARGS)> >::iterator it2;
		it2 = _oldMapOfOptimisers.find(key);
		if(it2 != _oldMapOfOptimisers.end() )  //the list in _oldMapOfOptimisers is already done: just insert the
		{                          //current builder (that is it1->second)
			it2->second.push_front(it1->second);
		}
		else  //build a new deque, fill it  and  insert it in _oldMapOfOptimisers
		{
			deque<PackagerOptimiser* (*)(PACKAGEROPT_ARGS)> tmpDeq;
			tmpDeq.push_front(it1->second);    
			_oldMapOfOptimisers.insert( pair<string, deque<PackagerOptimiser* (*)(PACKAGEROPT_ARGS)> >(key,tmpDeq) );
		}
		
		//At last upade the builder in _mapOfOptimisers
		_mapOfOptimisers[key]=builder;
	}
	else //just insert a new item in the map.
	{
		_mapOfOptimisers.insert( pair<string, PackagerOptimiser* (*)(PACKAGEROPT_ARGS) >(key,builder) );
	}
}


void PackagerOptimiserSingleton::unregisterFactory(string const & key, PackagerOptimiser* (*builder)(PACKAGEROPT_ARGS) ) // PRQA S 4020
{
        // EnrCar::
        // W8115_OFF
        // This method has multiple returns in order to keep its complexity
        // as low as possible

	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"key is \""<<key<<"\". builder address is \""<< reinterpret_cast<void*>(builder)<<"\"."); // PRQA S 3081
	map<string, PackagerOptimiser* (*)(PACKAGEROPT_ARGS)>::iterator it1; // ********  _mapOfOptimisers iterator ******************* 
	it1= _mapOfOptimisers.find(key); //see if the key is already registered in _mapOfOptimisers
	if(it1 == _mapOfOptimisers.end() )
	{
		ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB,"key is not present.");
		return ; //the builder is not present.
	}

	//Build a temporary deque
	deque<PackagerOptimiser* (*)(PACKAGEROPT_ARGS)> tmpDeq;

	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB,"temp deque built.");


	//check if there's the key in _oldMapOfOptimisers as well
	map<string, deque<PackagerOptimiser* (*)(PACKAGEROPT_ARGS)> >::iterator it2;
	it2 = _oldMapOfOptimisers.find(key);
	if(it2 != _oldMapOfOptimisers.end() )  //There's a replacements deque
	{
		ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB,"There are old versions.");
		tmpDeq=it2->second;
	}	
	
	//Push the current builder in front
	tmpDeq.push_front(it1->second);    
	
	//now I'm going to work only on tmpDeq
	deque<PackagerOptimiser* (*)(PACKAGEROPT_ARGS)>::iterator it3 = tmpDeq.begin();
	while( (it3 !=  tmpDeq.end() ) && (*it3 != builder ) ) {
		it3++;
    }
    
	if(it3 ==  tmpDeq.end() )
	{
		ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB,"The builder is not present.");
		return; //the builder was not present.
	}
	tmpDeq.erase(it3); 

	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB,"The builder was erased from temp deque.");

	if(tmpDeq.size() != 0)	
    {
		_mapOfOptimisers[key]=*(tmpDeq.begin()); //put into _mapOfOptimisers the first function in tmpDeq
        tmpDeq.pop_front();
    }
	else
    {
		_mapOfOptimisers.erase(it1); // There are no more builder for that key
    }
		

	if(tmpDeq.size() != 0)	
    {
		_oldMapOfOptimisers[key]=tmpDeq;
    }
	else
    {
        if (it2 != _oldMapOfOptimisers.end()) {
    		_oldMapOfOptimisers.erase(it2); // There are no more builder for that key
        }
    }

	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB,"END.");

}


PackagerOptimiser *PackagerOptimiserSingleton::newObject(
		string const& key, 
		const string& mediaType, const int& mediaFormatId, const string& mediaFormat, 
		const int& packingAlgoId, const unsigned long long int& mintr, 
    	const unsigned long long int& maxtr, const DateTime& delay, const DateTime& stabilityTime,
    	const PackagerOptimiser::TypeReportFormats& reportFormats)
{
	//the key input parameter indicates the Optimisation Method ("DateTime", "OneItem")
	//Build the Valid key to return the expected pointer:
    //
    // "DateTime" -> PackagerOptimiser_DateTime
    // "OneItem" -> PackagerOptimiser_OneItem
    
	
    string mapKey = "PackagerOptimiser_" + key ;
	map<string, PackagerOptimiser* (*)(PACKAGEROPT_ARGS)>::iterator it= _mapOfOptimisers.find(mapKey);
	if(it == _mapOfOptimisers.end() )
	{
		ACS_THROW(KeyNotFound("key \"" + mapKey+"\" is unknown.")); // PRQA S 3081
		
	}
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"key is \""<<mapKey<<"\". builder address is \""<< reinterpret_cast<void*>(it->second)<<"\"."); // PRQA S 3081
    

	return ( *(it->second ) )(mediaType, mediaFormatId, mediaFormat, packingAlgoId, mintr, maxtr, delay, stabilityTime, reportFormats);
}


PackagerOptimiserSingleton::PackagerOptimiserSingleton() :
    Singleton<PackagerOptimiserSingleton>(),
	_mapOfOptimisers(),
	_oldMapOfOptimisers(),
	_dbPackagerOptimiserFactory(new dbPackagerOptimiserFactory)
{

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"instance now."); 
}

PackagerOptimiserSingleton::~PackagerOptimiserSingleton() throw()
{
}



void registerPackagerOptimiserFactoryMethod(string const & key, PackagerOptimiser* (*builder)(PACKAGEROPT_ARGS) )
{
	PackagerOptimiserSingleton* c=PackagerOptimiserSingleton::instance();
	c->registerFactory(key,builder);
}


void unregisterPackagerOptimiserFactoryMethod(string const & key, PackagerOptimiser* (*builder)(PACKAGEROPT_ARGS) )
{
	PackagerOptimiserSingleton* c=PackagerOptimiserSingleton::instance();
	c->unregisterFactory(key,builder);
}


_ACS_END_NAMESPACE
