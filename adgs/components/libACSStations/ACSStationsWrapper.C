// PRQA S 1050 EOF
/*

	Copyright 1995-2014, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. ACSStations library $

	$Id$

	$Author$

	$Log$
	Revision 1.10  2014/03/25 12:23:32  marpas
	coding best practices applied
	warnings fixed (g++ & qa)
	
	Revision 1.9  2014/02/13 15:04:54  giucas
	New log macros adopted.
	
	Revision 1.8  2009/07/22 10:17:38  marpas
	c++ rules applied
	
	Revision 1.7  2009/07/15 16:49:42  giucas
	comments added.
	
	Revision 1.6  2008/11/17 15:16:40  giucas
	Static constant added
	
	Revision 1.5  2008/05/23 07:23:02  giucas
	Id added to StationInfo inner class
	
	Revision 1.4  2008/05/23 07:13:56  giucas
	getInfo method added
	
	Revision 1.3  2008/05/22 16:39:59  giucas
	Version obj added
	
	Revision 1.2  2008/05/22 16:36:08  giucas
	New methods added
	
	Revision 1.1.1.1  2008/05/22 12:34:01  giucas
	Moved ACSStationsWrapper and python wrapper from libSgiIncludes
	
	Revision 1.3  2008/04/24 09:39:15  giucas
	StationInfo class extended and made public
	
	Revision 1.2  2007/02/05 11:41:01  giucas
	Copy ctor and operator= added
	
	Revision 1.1  2006/11/14 16:28:53  giucas
	Added CPP Wrapper class for acsStations.h include file
	
	

*/



#include <ACSStationsWrapper.h>
#include <Filterables.h>
#include <acsStatV.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;

namespace {
	acsStatV versObj ;
}

const string ACSStationsWrapper::SITES_CONFIG_FILE_NAME = "SitesConf.conf" ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(ACSStationsWrapper)

ACSStationsWrapper::ACSStationsWrapper(const string &confFile) :
	_stationsAvailables(0),
	_stationNames(),
	_stationNamesShort(),
	_stationInfo(),
	_sitesConfRs()
{ 
	static char Id[] = "@(#) ACS: $Id$" ;	

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"ACSStationsWrapper::ctor Id: " << Id << " - confFile = " << confFile)

	// read the conf file
	ifstream fs ;
	// the name ... 
	string confFileName = confFile + "/" + SITES_CONFIG_FILE_NAME ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"opening " << confFileName)
	// ... open ... 
	fs.open(confFileName.c_str()) ;
	if (not fs.good()) {
		ACS_THROW(exFileOpenException("Cannot open "+confFileName,errno)) ; // PRQA S 3081
	}

	// ... read ...
	fs >> _sitesConfRs ;

	// ... close !
	fs.close() ;

	_sitesConfRs.getValue("List_of_Stations.count.value",_stationsAvailables) ;

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"ACSStationsWrapper::ctor Start to read " << _stationsAvailables << " Stations information from file " << confFileName )

	for(int i=0; i<_stationsAvailables; i++) // each station
	{
		ostringstream root ;
		root << "List_of_Stations.Station[" << i << "]." ;
	
		// prepare info structure
		ACSStationsWrapper::StationInfo info ;
		_sitesConfRs.getValue(root.str()+"Id" , info.id) ;
		_sitesConfRs.getValue(root.str()+"Name" , info.name) ;
		_sitesConfRs.getValue(root.str()+"ShortName" , info.shortName) ;

		_sitesConfRs.getValue(root.str()+"Country" , info.country) ;
		_sitesConfRs.getValue(root.str()+"CentreID" , info.centreId) ;

		_sitesConfRs.getValue(root.str()+"Latitude" , info.latitude) ;
		_sitesConfRs.getValue(root.str()+"Longitude" , info.longitude) ;
		_sitesConfRs.getValue(root.str()+"Altitude" , info.altitude) ;
		_sitesConfRs.getValue(root.str()+"HorizonElevation" , info.orizonElevation) ;

		_sitesConfRs.getValue(root.str()+"AgencySiteIdentifier" , info.agencySiteId) ;
		_sitesConfRs.getValue(root.str()+"AgencyOrganizationId" , info.agencyOrgId) ;

		_stationNames[info.id] = info.name ;
		_stationNamesShort[info.id] = info.shortName ;
		
		// pair id - info
		pair<int,ACSStationsWrapper::StationInfo> elem(info.id,info) ;
		
		// insert it 
		_stationInfo.insert(elem) ;

		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
			info.dump() ;
		ACS_CLASS_END_DEBUG
	}
}

ACSStationsWrapper::~ACSStationsWrapper() throw()
{
} 

ACSStationsWrapper::ACSStationsWrapper(const ACSStationsWrapper& old) :
	_stationsAvailables(old._stationsAvailables),
	_stationNames(old._stationNames),
	_stationNamesShort(old._stationNamesShort),
	_stationInfo(old._stationInfo),
	_sitesConfRs(old._sitesConfRs)
{
}


ACSStationsWrapper& ACSStationsWrapper::operator=(const ACSStationsWrapper& old)
{
	if (this != &old) { // avoid auto-assingation 
	  _stationsAvailables = old._stationsAvailables;
	  _stationNames = old._stationNames;
	  _stationNamesShort = old._stationNamesShort;
	  _stationInfo = old._stationInfo;
	  _sitesConfRs = old._sitesConfRs;
	}
  	return *this;
}

const string &ACSStationsWrapper::getName(int id) const
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for Id " << id )
	// throw an exception if id out of bounds
	ACS_COND_THROW( (id < 1) || (id > _stationsAvailables), exIllegalValueException("ERROR -> Input Station Id out of range")) ; // PRQA S 3081
	map<int,string>::const_iterator it = _stationNames.find(id) ;
	return it->second;
}

const string &ACSStationsWrapper::getNameShort(int id) const
{
	// throw an exception if id out of bounds
	ACS_COND_THROW( (id < 1) || (id > _stationsAvailables), exIllegalValueException("ERROR -> Input Station Id out of range")) ; // PRQA S 3081
	map<int,string>::const_iterator it = _stationNamesShort.find(id) ;
	return it->second;
}

int ACSStationsWrapper::getIdFromName(const std::string& name) const 
{
	for (map<int,string>::const_iterator it = _stationNames.begin(); it != _stationNames.end(); it++ )
	{
		if( it->second == name ) // found
		{
			return it->first ; // return its id 
		}
	}

	ACS_THROW(exIllegalValueException("ERROR -> Unknown Input Station Name "+name)) ; // PRQA S 3081
}

int ACSStationsWrapper::getIdFromShortName(const std::string& name) const
{
	for(map<int,string>::const_iterator it = _stationNamesShort.begin() ;
	    it != _stationNamesShort.end(); it++ )
	{
		if( it->second == name ) // found
		{
			return it->first ; // return its id 
		}
	}
	ACS_THROW(exIllegalValueException("ERROR -> Unknown Input Station Name SHORT "+name)) ;// PRQA S 3081
}


void ACSStationsWrapper::getInfoFromName(const string& name, double& lat, double& lon, double& alt) const
{
	getInfoFromId(getIdFromName(name),lat,lon,alt) ;
}

void ACSStationsWrapper::getInfoFromName(const string& name, ACSStationsWrapper::StationInfo& info) const 
{
	getInfoFromId(getIdFromName(name),info) ;
}

void ACSStationsWrapper::getInfoFromShortName(const std::string& name, double& lat, double& lon, double& alt) const
{
	getInfoFromId(getIdFromShortName(name),lat,lon,alt) ;
}

void ACSStationsWrapper::getInfoFromId(int id, double& lat, double& lon, double& alt) const
{
	map<int, StationInfo>::const_iterator it = _stationInfo.find(id) ; // find it 
	
	// exception if not found
	ACS_COND_THROW( it == _stationInfo.end(), exIllegalValueException("ERROR -> Unknown Input Station Id")) ;	// PRQA S 3081
    lat = it->second.latitude ;
	lon = it->second.longitude ;
	alt = it->second.altitude ;
}

void ACSStationsWrapper::getInfoFromId(int id, ACSStationsWrapper::StationInfo& info) const
{
	map<int, StationInfo>::const_iterator it = _stationInfo.find(id) ; // find it 
	
	// exception if not found
	ACS_COND_THROW( it == _stationInfo.end(), exIllegalValueException("ERROR -> Unknown Input Station Id")) ;	// PRQA S 3081
	info = it->second ;
}


size_t ACSStationsWrapper::getStationsAvailable() const
{
	return _stationInfo.size() ;
}

void ACSStationsWrapper::getResources(rsResourceSet& rs) const
{
	// save the old merge mode on the given resourceset
	rsResourceSet::MergeMode oldmm = rs.mergeMode() ;
	try {
		// change the merge mode to "change"
		rs.mergeMode(rsResourceSet::ModeChange) ;

		// merge
		rs.merge(_sitesConfRs) ;

		// restore merge mode
		rs.mergeMode(oldmm) ;
	}
	catch(...) {
		// restore merge mode
		rs.mergeMode(oldmm) ;
		throw ;
	}
}

void ACSStationsWrapper::getInfo(std::vector<ACSStationsWrapper::StationInfo>& info) const
{
	for(map<int, StationInfo>::const_iterator it = _stationInfo.begin() ;
	    it != _stationInfo.end(); it++ )
	{
		info.push_back(it->second) ;
	}
}

/////////////////////////////////////////////////////////////////////////////
// StationInfo container class implementation
/////////////////////////////////////////////////////////////////////////////
ACSStationsWrapper::StationInfo::StationInfo() :
	id(-1),
	name(),
	country(),
	centreId(),		
	shortName(),
	latitude(0.0),
	longitude(0.0),
	altitude(0.0),
	orizonElevation(0.0),
	agencySiteId(),
	agencyOrgId()
{
}

ACSStationsWrapper::StationInfo::~StationInfo()
{
}

ACSStationsWrapper::StationInfo::StationInfo(const StationInfo& e):
	id(e.id),
	name(e.name),
	country(e.country),
	centreId(e.centreId),		
	shortName(e.shortName),
	latitude(e.latitude),
	longitude(e.longitude),
	altitude(e.altitude),
	orizonElevation(e.orizonElevation),
	agencySiteId(e.agencySiteId),
	agencyOrgId(e.agencyOrgId)
{
}

ACSStationsWrapper::StationInfo& 
ACSStationsWrapper::StationInfo::operator=(const StationInfo& e)
{
	if( this != &e ) // avoid auto-assignament
	{
		// copy field by field
		id 					= e.id ;
		name 				= e.name ;
		country 			= e.country ;
		centreId 			= e.centreId ;
		shortName 			= e.shortName ;
		latitude 			= e.latitude ;
		longitude			= e.longitude ;
		altitude			= e.altitude ;
		orizonElevation 	= e.orizonElevation ;
		agencySiteId		= e.agencySiteId ;
		agencyOrgId			= e.agencyOrgId ;
	}
	return *this ;
}

void ACSStationsWrapper::StationInfo::dump() const
{
	//all station information
	ostringstream msg ;
	// prepare string
	msg << "ACSStationsWrapper::StationInfo::dump : name = " << name << '\n'
		<< "ACSStationsWrapper::StationInfo::dump : id = " << id << '\n'
		<< "ACSStationsWrapper::StationInfo::dump : country = " << country << '\n'
		<< "ACSStationsWrapper::StationInfo::dump : centreId = " << centreId << '\n'
		<< "ACSStationsWrapper::StationInfo::dump : shortName = " << shortName << '\n'
		<< "ACSStationsWrapper::StationInfo::dump : latitude = " << setprecision(20) << latitude << '\n' // PRQA S 4400 4
		<< "ACSStationsWrapper::StationInfo::dump : longitude = " << setprecision(20)<< longitude << '\n'
		<< "ACSStationsWrapper::StationInfo::dump : altitude = " << setprecision(20)<< altitude << '\n'
		<< "ACSStationsWrapper::StationInfo::dump : orizonElevation = " << setprecision(20) << orizonElevation << '\n'
		<< "ACSStationsWrapper::StationInfo::dump : agencySiteId = " << agencySiteId << '\n'
		<< "ACSStationsWrapper::StationInfo::dump : agencyOrgId = " << agencyOrgId << '\n' ;
	// eventually dump it 
	ACS_LOG_DEBUG(msg.str())
}

_ACS_END_NAMESPACE
