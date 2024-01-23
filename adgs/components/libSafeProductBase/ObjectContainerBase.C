// PRQA S 1050 EOF
/*

	Copyright 2011-2013, Advanced Computer Systems , Inc.
	Via della Bufalotta, 347 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

 	$Prod: A.C.S. Safe Product Base Library $
 
 	$Id$
 
 	$Author$
 
 	$Log$
 	Revision 1.7  2013/10/02 13:05:54  giucas
 	rsResourceSet used as reference
 	
 	Revision 1.6  2012/03/28 12:22:12  aleber
 	comments on the generation of the Object map improved
 	
 	Revision 1.5  2011/07/11 17:51:57  aleber
 	handling of schema repository completed
 	
 	Revision 1.4  2011/07/08 17:54:48  aleber
 	Writing of DataObjectContainer completed (but for checksum)
 	
 	Revision 1.3  2011/07/07 16:04:08  aleber
 	Object configuration correctly extracted from SafeProduct.xml
 	
 	Revision 1.2  2011/07/06 17:52:50  aleber
 	method addNewItem implemented
 	
 	Revision 1.1.1.1  2011/07/06 15:56:51  aleber
 	First implementation
 	
 	

*/

#include <ObjectContainerBase.h>
#include <ConfigurationSingleton.h>
#include <Filterables.h>           

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(ObjectContainerBase)


ObjectContainerBase::ObjectContainerBase(const string& mission,
					 const string& satellite,
					 const string& sensor,
					 const string& sensorMode,
					 const string& level,
					 const string& productClass,
					 const string& polarisation): 
	_mission(mission),
	_satellite(satellite),
	_sensor(sensor),
	_sensorMode(sensorMode),
	_level(level),
	_productClass(productClass),
	_polarisation(polarisation),
    _productTypeId(),
    _idMap(),
    _numOfObject()
{

	static char Id[] = "@(#) ACS: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Id: " << Id) ;

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Object Safe created with following data members:" << endl
		    << " \t mission = " << mission) ;
}

ObjectContainerBase::~ObjectContainerBase() throw() 
{
}

// copy constructor
ObjectContainerBase::ObjectContainerBase(const ObjectContainerBase & e) : 
	_mission(e._mission),
	_satellite(e._satellite),
	_sensor(e._sensor),
	_sensorMode(e._sensorMode),
	_level(e._level),
	_productClass(e._productClass),
	_polarisation(e._polarisation),
    _productTypeId(e._productTypeId),
    _idMap(e._idMap),
    _numOfObject(e._numOfObject)
{
}

// operator =
ObjectContainerBase&  ObjectContainerBase::operator=(const ObjectContainerBase & e)
{
	if (this != &e) {
        _mission = e._mission ;
        _satellite = e._satellite ;
        _sensor = e._sensor ;
        _sensorMode = e._sensorMode ;
        _level = e._level ;
        _productClass = e._productClass ;
        _polarisation = e._polarisation ;
        _productTypeId = e._productTypeId ;
        _idMap = e._idMap ;
        _numOfObject = e._numOfObject ;
	}
	return *this ;
}

vector<string> ObjectContainerBase::getIds() const
{

	vector<string> idVector;
	
	for (map< string, map<string, string> >::const_iterator it=_idMap.begin(); it != _idMap.end(); ++it) {
		if ( not it->first.empty() ) {
			idVector.push_back(it->first); 
        }
    }
			
	return idVector;
			
}


void ObjectContainerBase::setFilename(const std::string& id, const std::string& filename)                                                                                    
{                                                                                                           

	map<string,string>	keyValueMap;

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "STARTED with id = " << id << " filename = " << filename ) ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, toStr()) ;
	
	// if there is an element (a map) in the map with the provided key "id" ....
	if (_idMap.find(id) != _idMap.end())
	{
		map< string, map<string, string> >::iterator it = _idMap.find(id);
		
		// if the map element contains a "filename" as key, set the map element value to the provided filename
		if ( (it->second).find("filename") != (it->second).end() ) {
			(it->second)["filename"] = filename;
		// if the map element does not contain a "filename" as key, create a map element with the key "element"
		} else {
			keyValueMap["filename"] = filename;		
			_idMap[id] = keyValueMap;		
		}
	
	} 
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "ENDED with id = " << id << " filename = " << filename) ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, toStr()) ;

// 	else {
// 		keyValueMap["filename"] = filename;		
// 		_idMap[id] = keyValueMap;
// 	} 	
			
}

string ObjectContainerBase::getAttribute(const std::string& id, const std::string& attribute) const // PRQA S 4020                                                                                    
{                                                                                                           

    map< string, map<string, string> >::const_iterator it = _idMap.find(id);
	if (it != _idMap.end())
	{
        map<string, string>::const_iterator it2 = it->second.find(attribute) ;
		if ( it2 != it->second.end() ) {
			return it2->second ;
		} 
        else {
			return "";
		}
	} 
    else {
		return "";
	}
}

string ObjectContainerBase::toStr() const
{                                                                                                           
    ostringstream os ;
	for (map< string, map<string, string> >::const_iterator it=_idMap.begin(); it != _idMap.end(); ++it)
	{
		for (map<string, string>::const_iterator jt = (it->second).begin(); jt != (it->second).end(); ++jt)
		{
            if (jt != it->second.begin()) { os << '\n' ; }
			os << "idMap [ " << it->first << " -> " << jt->first << " -> "  << jt->second <<" ]" ;
		}
	}
    return os.str() ;
	
}                                                                                                           

////////////////////////////////////////////////////////////////////
////////////////////// PROTECTED ///////////////////////////////////
////////////////////////////////////////////////////////////////////
void ObjectContainerBase::addNewItem(const string& id, const string& key)
{
	map< string, map<string, string> >::iterator it = _idMap.find(id);
    if (it != _idMap.end()) {
        map<string, string>::iterator it2 = it->second.find(key) ;
		if ( it2 == it->second.end() ) {
			it->second[key] = "";
		}
    }
}


string ObjectContainerBase::getRootFromList(const string& listName, int listElementId)                                                                                    
{                                                                                                           
	ostringstream os ;
	os << listName << '[' << listElementId << "]." ;
	return os.str();
}
				
string ObjectContainerBase::getTagFromList(const string& listName, int listElementId)                                                                                     
{                                                                                                           
	ostringstream os;
	os << listName << '[' << listElementId << "]." ;
	return os.str();
}

string ObjectContainerBase::getValueFromList(const string& listName, int id) // PRQA S 4020
{                                                                                                           
	ostringstream tagName ;
	tagName << listName << '[' << id << ']' ;
	try {	
	    string value ;
    	rsResourceSet &_rsConf(ConfigurationSingleton::instance()->get()) ;
		_rsConf.getValue(tagName.str(), value);
        return value ;
	} catch (rsResourceSet::NotFoundException & ){
		return "";		
	}
}

string ObjectContainerBase::getValueFromList(const string& listName, size_t id) // PRQA S 4020
{                                                                                                           
	ostringstream tagName ;
	tagName << listName << '[' << id << ']' ;
	try {	
	    string value ;
    	rsResourceSet &_rsConf(ConfigurationSingleton::instance()->get()) ;
		_rsConf.getValue(tagName.str(), value);
        return value ;
	} catch (rsResourceSet::NotFoundException & ){
		return "";		
	}
}

string ObjectContainerBase::getValueFromList(const string& listName, unsigned int id) // PRQA S 4020
{                                                                                                           
	ostringstream tagName ;
	tagName << listName << '[' << id << ']' ;
	try {	
	    string value ;
    	rsResourceSet &_rsConf(ConfigurationSingleton::instance()->get()) ;
		_rsConf.getValue(tagName.str(), value);
        return value ;
	} catch (rsResourceSet::NotFoundException & ){
		return "";		
	}
}

string ObjectContainerBase::getValueFromList(const string& listName, int id, const string& tag) // PRQA S 4020                                                                                   
{                                                                                                           

	ostringstream tagName;
	tagName << listName << '[' << id << "]." << tag ;

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "\t tagName = " << tagName.str()) ;
	try {	
	    string value ;
    	rsResourceSet &_rsConf(ConfigurationSingleton::instance()->get()) ;
		_rsConf.getValue(tagName.str(), value);
	    return value;
	} catch (rsResourceSet::NotFoundException &){
		return "";		
	}
}


string ObjectContainerBase::getValueFromList(const string& listName, size_t id, const string& tag) // PRQA S 4020                                                                                   
{                                                                                                           

	ostringstream tagName;
	tagName << listName << '[' << id << "]." << tag ;

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "\t tagName = " << tagName.str()) ;
	try {	
	    string value ;
    	rsResourceSet &_rsConf(ConfigurationSingleton::instance()->get()) ;
		_rsConf.getValue(tagName.str(), value);
	    return value;
	} catch (rsResourceSet::NotFoundException &){
		return "";		
	}
}


string ObjectContainerBase::getValueFromList(const string& listName, unsigned int id, const string& tag) // PRQA S 4020                                                                                   
{                                                                                                           

	ostringstream tagName;
	tagName << listName << '[' << id << "]." << tag ;

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "\t tagName = " << tagName.str()) ;
	try {	
	    string value ;
    	rsResourceSet &_rsConf(ConfigurationSingleton::instance()->get()) ;
		_rsConf.getValue(tagName.str(), value);
	    return value;
	} catch (rsResourceSet::NotFoundException &){
		return "";		
	}
}

string ObjectContainerBase::getValueFromList(const string& listName, const string& listElementName, const string& elementRefId, const string& tag) // PRQA S 4020                                                                                  
{                                                                                                           
	unsigned int numOfElements = 0 ;
//	_rsConf.getValue("ListOfMissions.count.value", numOfElements);
    rsResourceSet &_rsConf(ConfigurationSingleton::instance()->get()) ;
	_rsConf.getValue(listName + ".count.value", numOfElements);

	for(unsigned int elementNum = 0; elementNum < numOfElements; ++elementNum) {
		string elementId = getValueFromList(listName + "." + listElementName, elementNum, "Id"); // PRQA S 3000
		if (elementId == elementRefId) {
			return getValueFromList(listName + "." + listElementName, elementNum, tag); // PRQA S 3000
		}
	}
	return "";
		
}


std::ostream &operator << (std::ostream &os, const ObjectContainerBase &v) 
{
    return os << v.toStr() ;
}

acs::exostream &operator << (acs::exostream &os, const ObjectContainerBase &v) // PRQA S 4222
{
    return os << v.toStr() ;
}
  

_ACS_END_NAMESPACE

