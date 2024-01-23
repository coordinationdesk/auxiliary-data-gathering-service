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

	$Prod: A.C.S. libSafeProductBase Library $

	$Id$

	$Author$
	$Log$
	Revision 1.11  2014/02/18 13:05:31  enrcar
	EC:: modified in order to use an earlier DateTime I/F.
	
	Revision 1.10  2014/02/12 14:26:50  enrcar
	EnrCar:: Added member _datastrip in the ProcessingLog
	
	Revision 1.9  2014/02/11 14:24:55  enrcar
	EnrCar:: optional argument instance added
	
	Revision 1.8  2014/01/28 17:15:20  enrcar
	ENRCAR:: Added some non mandatory attributes to containers. This doesn't cause regressions.
	
	Revision 1.7  2013/12/03 17:51:11  marpas
	date time in variables with specific types
	
	Revision 1.6  2013/11/14 14:44:40  marpas
	coding best practices applied,
	interface robustness and performances improved
	
	Revision 1.5  2013/10/18 08:08:32  masdal
	RicFer:: code re-aligned
	
	Revision 1.4  2013/10/04 09:03:25  marpas
	some warnigns removed
	
	Revision 1.3  2013/01/21 16:32:07  aleber
	SPPDU version added as last argument of function addProcessing of ProcessingContainer class
	
	Revision 1.2  2011/09/08 07:36:27  aleber
	ProcessingLog and ResourceLog implemented
	
	Revision 1.1.1.1  2011/07/06 15:56:51  aleber
	First implementation
	

*/

#include <ProcessingContainerBase.h>
#include <DateTime.h>                                                                        

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;


ACS_CLASS_DEFINE_DEBUG_LEVEL(ResourceLog)


FacilityLog::FacilityLog(
	const string & name ,
	const string & organization ,
	const string & site ,
	const string & country 
) :
    _id(),
    _name(name),
    _organization(organization),
    _site(site),
    _country(country),
    _software(),
	_hardware(),
	_facility()

{
}

FacilityLog::FacilityLog(const FacilityLog& f):
    _id(f._id),
    _name(f._name),
    _organization(f._organization),
    _site(f._site),
    _country(f._country),
    _software(f._software),
	_hardware(f._hardware),
	_facility(f._facility)
{

}

FacilityLog::~FacilityLog() throw ()
{
}

FacilityLog &FacilityLog::operator=(const FacilityLog& f)
{
 	if (this != &f) {
        _id = f._id ;
        _name = f._name ;
        _organization = f._organization ;
        _site = f._site ;
        _country = f._country ;
        _software = f._software;
		_hardware = f._hardware;
		_facility = f._facility;
 	}
 	return *this ;
}


void FacilityLog::setId(const string& id) // PRQA S 4121
{
	_id = id ;
}
void FacilityLog::setName(const string& name) // PRQA S 4121
{
	_name = name;
}void FacilityLog::setOrganization(const string& organization)  // PRQA S 4121 
{ 
	_organization = organization;
}
void FacilityLog::setSite(const string& site)   // PRQA S 4121
{ 
	_site = site;
}
void FacilityLog::setCountry(const string& country)   // PRQA S 4121
{ 
	_country = country;
}

void FacilityLog::addSoftware(SoftwareLog* softwareLog)  
{ 
	if(softwareLog) { _software.push_back(softwareLog); }
}

void FacilityLog::addHardware(HardwareLog* hardwareLog)  
{ 
	if(hardwareLog) { _hardware.push_back(hardwareLog); }
}


string FacilityLog::toStr() const
{ 

	ostringstream msg; 
	msg << "FacilityLog content: " << '\n'
		<< "\tName: " << name() << '\n'
		<< "\tOrganization: " << organization() << '\n'
		<< "\tSite: " << site() << '\n'
		<< "\tCountry: " << country() ; 
        
	return msg.str();

}

std::ostream &operator << (std::ostream &os, const FacilityLog &v) 
{
    return os << v.toStr() ;
}

acs::exostream &operator << (acs::exostream &os, const FacilityLog &v) // PRQA S 4222
{
    return os << v.toStr() ;
}



SoftwareLog::SoftwareLog(
	const string & name,
	const string & version
) :
    _name(name),
    _version(version),
    _software()
{
}

SoftwareLog::SoftwareLog(const SoftwareLog& s):
    _name(s._name),
    _version(s._version),
    _software(s._software)
{
}

SoftwareLog::~SoftwareLog() throw()
{
}

SoftwareLog &SoftwareLog::operator=(const SoftwareLog& s)
{
 	if (this != &s) {
        _name = s._name ;
        _version = s._version ;
        _software = s._software;
	}
 	return *this ;
}


void SoftwareLog::setName(const string& name) // PRQA S 4121
{
	_name = name;
}
void SoftwareLog::setVersion(const string& version)  // PRQA S 4121 
{ 
	_version = version;
}


string SoftwareLog::toStr() const
{ 

	ostringstream msg; 
	msg << "SoftwareLog content: " << '\n'
		<< "\tName: " << name() << '\n'
		<< "\tVersion: " << version() << '\n'
		<< "\tSoftware size = " << _software.size() ;
	return msg.str();

}

std::ostream &operator << (std::ostream &os, const SoftwareLog &v) 
{
    return os << v.toStr() ;
}

acs::exostream &operator << (acs::exostream &os, const SoftwareLog &v) // PRQA S 4222
{
    return os << v.toStr() ;
}

HardwareLog::HardwareLog():
_name("","")
{
}

HardwareLog::HardwareLog(const HardwareLog& h):
	_name(h._name)
{
}

HardwareLog::~HardwareLog() throw()
{
}

HardwareLog &HardwareLog::operator=(const HardwareLog& h)
{
 	if (this != &h) {
		_name = h._name ;
	}
 	return *this ;
}


void HardwareLog::setName(const pair<string, string>& name)  // PRQA S 4121
{
	_name = name;
}


string HardwareLog::toStr() const 
{ 
	ostringstream msg; 
	msg << "HardwareLog content: " << '\n'
		<< "\tName: " << _name.first 
		<< "\tId: " << _name.second  ;
	return msg.str();
}

std::ostream &operator << (std::ostream &os, const HardwareLog &v) 
{
    return os << v.toStr() ;
}

acs::exostream &operator << (acs::exostream &os, const HardwareLog &v) // PRQA S 4222
{
    return os << v.toStr() ;
}

ResourceLog::ResourceLog(
   const string & name,
   const string & role,
   const string & href,
   const string & version
) :
    _id(""),
    _name(name),
   	_role(role),
    _href(href),
    _version(version),
    _processing()
{
    static char Id[] = "@(#) ACS: $Id$" ;
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Id: " << Id) ;
}

ResourceLog::ResourceLog(const ResourceLog& r):
    _id(r._id),
    _name(r._name),
    _role(r._role),
    _href(r._href),
    _version(r._version),
    _processing(r._processing)
{
}

ResourceLog::~ResourceLog() throw()
{
}

ResourceLog &ResourceLog::operator=(const ResourceLog& r)
{
 	if (this != &r) {
    	_id = r._id ;
        _name = r._name ;
 		_role = r._role ;
 		_href = r._href ;
		_version = r._version;
		_processing = r._processing;
 	}
 	return *this ;
}

void ResourceLog::setId(const string& id)   // PRQA S 4121
{
	_id = id;
}
void ResourceLog::setName(const string& name)   // PRQA S 4121
{
	_name = name;
}void ResourceLog::setRole(const string& role)    // PRQA S 4121
{ 
	_role = role;
}
void ResourceLog::setHref(const string& href)    // PRQA S 4121
{ 
	_href = href;
}
void ResourceLog::setVersion(const string& version)    // PRQA S 4121
{ 
	_version = version;
}
void ResourceLog::addProcessing(ProcessingLog* processing)  
{ 
	if(processing) { _processing.push_back( processing ); }
}


string ResourceLog::toStr() const
{ 
	ostringstream msg; 
	msg << "ResourceLog content: " << '\n'
		<< "\tName: " << name() << '\n'
		<< "\tVersion: " << version() << '\n'
		<< "\tRole: " << role() << '\n'
		<< "\tHref: " << href() ;
	return msg.str();

}

std::ostream &operator << (std::ostream &os, const ResourceLog &v) 
{
    return os << v.toStr() ;
}

acs::exostream &operator << (acs::exostream &os, const ResourceLog &v) // PRQA S 4222
{
    return os << v.toStr() ;
}



ProcessingLog::ProcessingLog(
	    const string & name ,
	    const DateTime::MJD2000 & startTimeStr, // start processing time provided as ASCII string of the type yyyy-mm-ddThh:nn:ss.uuuuuu 
	    const DateTime::MJD2000 & stopTimeStr,
	    const string & sppduVersion,
	    const string & outputLevel,
		const off_t & datastrip,
		const off_t & instance) :
    _id(),
    _name(name),
    _startTime(startTimeStr),
    _stopTime(stopTimeStr),
	_startTimeSet(true),
	_stopTimeSet(true),
    _sppduVersion(sppduVersion),
	_outputLevel(outputLevel),
	_xmlns(),
	_datastrip(datastrip),
	_instance(instance),
    _software(), 
	_hardware(),
    _resource(),  
    _facility(),  
    _processing(),
    _facilityMap()
{
}

ProcessingLog::ProcessingLog(const string & name) :
    _id(),
    _name(name),
    _startTime(),
    _stopTime(),
	_startTimeSet(false),
	_stopTimeSet(false),
    _sppduVersion(),
	_outputLevel(),
	_xmlns(),
	_datastrip(0),
	_instance(0),
    _software(), 
	_hardware(),
    _resource(),  
    _facility(),  
    _processing(),
    _facilityMap()
{
}

ProcessingLog::~ProcessingLog() throw()
{
}

// copy constructor
ProcessingLog::ProcessingLog(const ProcessingLog& p):
	_id(p._id),
    _name(p._name),
 	_startTime(p._startTime),
 	_stopTime(p._stopTime),
	_startTimeSet(p._startTimeSet),
 	_stopTimeSet(p._stopTimeSet),
 	_sppduVersion(p._sppduVersion),
	_outputLevel(p._outputLevel),
	_xmlns(p._xmlns),
	_datastrip(p._datastrip),
 	_instance(p._instance),
	_software(p._software),
	_hardware(p._hardware),
 	_resource(p._resource),
	_facility(p._facility),
	_processing(p._processing),
	_facilityMap(p._facilityMap)
{
}


// operator =    
ProcessingLog &ProcessingLog::operator=(const ProcessingLog& p)
{
 	if (this != &p) {
	    _id = p._id ;
		_name = p._name ;
 		_startTime = p._startTime ;
 		_stopTime = p._stopTime ;
 		_startTimeSet = p._startTimeSet ;
 		_stopTimeSet = p._stopTimeSet ;
 		_sppduVersion = p._sppduVersion ;
		_outputLevel = p._outputLevel ;
		_xmlns = p._xmlns ;
		_datastrip = p._datastrip ;
		_instance = p._instance ;
		_software = p._software;
		_hardware = p._hardware;
		_resource = p._resource;
		_facility = p._facility;
		_processing = p._processing;
		_facilityMap = p._facilityMap;
 	}
 	return *this ;
}

void ProcessingLog::setId(const string& id) // PRQA S 4121
{
	_id = id;
}
void ProcessingLog::setName(const string& name) // PRQA S 4121 
{
	_name = name;
}
void ProcessingLog::setStartTime(const DateTime::MJD2000& startTime)  // PRQA S 4121 
{ 
	_startTime = startTime; _startTimeSet = true ; 
}
void ProcessingLog::setStopTime(const DateTime::MJD2000& stopTime)   // PRQA S 4121
{ 
	_stopTime = stopTime; _stopTimeSet = true ; 
}
void ProcessingLog::setXmlns(const string& xmlns) 
{ 
	_xmlns = xmlns; 
}
void ProcessingLog::setSppduVersion(const string& sppduVersion)   // PRQA S 4121
{ 
	_sppduVersion = sppduVersion;
}
void ProcessingLog::setOutputLevel(const string& outputLevel)  // PRQA S 4121 
{ 
	_outputLevel = outputLevel;
}

void ProcessingLog::setDatastrip(const off_t& datastrip) 
{ 
	_datastrip = datastrip ;
}

void ProcessingLog::setInstance(const off_t& instance) 
{ 
	_instance = instance ;
}

void ProcessingLog::addSoftware(SoftwareLog* softwareLog)  
{ 
	if(softwareLog) { _software.push_back( softwareLog ); }
}

void ProcessingLog::addResource(ResourceLog* resourceLog)  
{ 
	if(resourceLog) { _resource.push_back(resourceLog); }
}

void ProcessingLog::addFacility(FacilityLog* facilityLog)  
{ 
	if(facilityLog) { _facility.push_back( facilityLog ); }
	if (_facilityMap.find(facilityLog->getId()) == _facilityMap.end()) {
		_facilityMap[facilityLog->getId()] = *facilityLog;	
    }
}

void ProcessingLog::addProcessing(ProcessingLog* processingLog)  
{ 
	if(processingLog) { _processing.push_back( processingLog ); }
}

const FacilityLog* ProcessingLog::facility(const string& id) const // PRQA S 4020
{ 
    map<string, FacilityLog>::const_iterator it = _facilityMap.find(id) ;
	if (it != _facilityMap.end()) {
		return &it->second ;
	} else {
		return 0;
	}
}

string ProcessingLog::toStr() const 
{ 
	ostringstream msg, o1, o2 ; 
	DateTime(_startTime).asISO8601(o1, 6) ;
	DateTime(_stopTime).asISO8601(o2, 6) ;
	msg <<"ProcessingLog content: " << '\n'
		<<"\tName: " << _name << '\n'
		<<"\tStart time: " << o1.str() << '\n'
		<<"\tStop time: " << o2.str() << '\n'   
		<<"\tSPPDU version: " << _sppduVersion ;
	return msg.str();

}

std::ostream &operator << (std::ostream &os, const ProcessingLog &v) 
{
    return os << v.toStr() ;
}

acs::exostream &operator << (acs::exostream &os, const ProcessingLog &v) // PRQA S 4222
{
    return os << v.toStr() ;
}



ProcessingContainerBase::ProcessingContainerBase() :
    _processing(),
    _processingMap()
{
}

ProcessingContainerBase::~ProcessingContainerBase() throw() 
{
}

// copy constructor
ProcessingContainerBase::ProcessingContainerBase(const ProcessingContainerBase& p):
	_processing(p._processing),
    _processingMap(p._processingMap)
{
}


// operator =    
ProcessingContainerBase &ProcessingContainerBase::operator=(const ProcessingContainerBase& p)
{
 	if (this != &p) {
		_processing = p._processing;
 	}
 	return *this ;
}



void ProcessingContainerBase::addProcessing(ProcessingLog* processing)  
{ 
	if(processing) { _processing.push_back(processing); }
	if (_processingMap.find(processing->getId()) == _processingMap.end()) {
		_processingMap[processing->getId()] = *processing;	
    }
}

void ProcessingContainerBase::addProcessing(const string& id, 
                                            const string& name, 
                                            const DateTime::MJD2000& startTime, 
                                            const DateTime::MJD2000& stopTime, 
                                            const string& sppduVersion)  
{ 

	ProcessingLog processingLog(name, startTime, stopTime, sppduVersion);
	if (_processingMap.find(id) == _processingMap.end()) {
		_processingMap[id] = processingLog;	
    }

}

const ProcessingLog* ProcessingContainerBase::processing(const string& id) const // PRQA S 4020
{ 
    map<string, ProcessingLog>::const_iterator it = _processingMap.find(id) ;
	if (it != _processingMap.end()) {
		return &it->second ;
	} else {
		return 0;
	}

}

string ProcessingContainerBase::toStr() const 
{ 

    ostringstream msg; 
    msg <<"ProcessingContainerBase content: " << '\n'
        <<"\tProcessing: " << (processing())[0]->name() ;
    return msg.str();

}
                                                                                                            
std::ostream &operator << (std::ostream &os, const ProcessingContainerBase &v) 
{
    return os << v.toStr() ;
}

acs::exostream &operator << (acs::exostream &os, const ProcessingContainerBase &v) // PRQA S 4222
{
    return os << v.toStr() ;
}

_ACS_END_NAMESPACE
