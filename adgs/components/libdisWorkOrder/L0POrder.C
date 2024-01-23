/*

	Copyright 2011, Advanced Computer Systems , Inc.
	Via della Bufalotta, 347 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$

	$Log$
	Revision 1.7  2014/02/13 10:07:55  giucas
	New log macros adopted
	
	Revision 1.6  2011/09/12 12:12:29  marpas
	write() signature changed - still compatible with previous version.
	Now it can overwrite an existing file
	
	Revision 1.5  2011/09/12 09:39:50  marpas
	size() method fixed
	
	Revision 1.4  2011/09/12 09:03:16  marpas
	size method added
	
	Revision 1.3  2011/09/07 15:57:31  marpas
	interface improved: constness and throw declaration where needed and perfo for methods returning structures
	
	Revision 1.2  2011/09/02 08:40:04  giucas
	Added dump method for L0PDataFile
	
	Revision 1.1  2011/09/02 07:49:43  giucas
	Imported
	

*/

#include <L0POrder.h>
#include <XMLOstream.h>
#include <File.h>
#include <DateTime.h>
#include <rsResourceSet.h>
#include <XMLIstream.h>
#include <StringUtils.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(L0POrder)

// -------------------
// default constructor
// -------------------
L0POrder::L0POrder() : _dataFiles()
{
	static char Id[] = "@(#) ACS: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"L0POrder::Id: " << Id )
}

L0POrder::~L0POrder() throw() 
{
}

//
// copy constructor
//
L0POrder::L0POrder(const L0POrder & r) : _dataFiles(r._dataFiles)
{
}

//
// operator =
//
L0POrder&  L0POrder::operator=(const L0POrder & e)
{
	if (this != &e) {
		_dataFiles = e._dataFiles ;
	}
	return *this ;
}


void L0POrder::write(const string& l0pOrderFileName, bool overwrite) const
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to write L0 Processing order file " << l0pOrderFileName )

	ACS_COND_THROW(File::exists(l0pOrderFileName) &&  not overwrite, exIOException("L0 Processing order file "+l0pOrderFileName+" already exists") ) ;
	
	ofstream  outFile(l0pOrderFileName.c_str());
    outFile.exceptions( ios::badbit | ios::failbit ) ;

	XMLOstream xos (outFile, "L0P_Order");

	ostringstream attr ;
	attr << "count=\"" << _dataFiles.size() << "\"" ;
	XMLOstream::Tag listOfFiles(xos, "List_of_Files", "", attr.str());
	for(size_t i=0; i<_dataFiles.size(); i++)
	{

		XMLOstream::Tag rawFile(xos, "File", "", "");
			XMLOstream::Tag rawFileName(xos, "filename", _dataFiles[i].getFileName(), "", true);
			//convert start/stop in string format YYYY-MM-DD hh:mm:ss.mmm
			DateTime start = _dataFiles[i].getValidityStart() ;
			DateTime stop = _dataFiles[i].getValidityStop() ;
			ostringstream startStr , stopStr ;
			DateTime::UTCD utcdStart, utcdStop ;
			start.toUTCD(utcdStart) ;
			stop.toUTCD(utcdStop) ;
			startStr << setw(4) << setfill('0') << utcdStart.year << "-"
					 << setw(2) << setfill('0') << utcdStart.month << "-"
					 << setw(2) << setfill('0') << utcdStart.day << " "
					 << setw(2) << setfill('0') << utcdStart.hour << ":"
					 << setw(2) << setfill('0') << utcdStart.min << ":"
					 << setw(2) << setfill('0') << utcdStart.sec << "."
					 << setw(3) << setfill('0') << utcdStart.millisec() ;

			stopStr  << setw(4) << setfill('0') << utcdStop.year << "-"
					 << setw(2) << setfill('0') << utcdStop.month << "-"
					 << setw(2) << setfill('0') << utcdStop.day << " "
					 << setw(2) << setfill('0') << utcdStop.hour << ":"
					 << setw(2) << setfill('0') << utcdStop.min << ":"
					 << setw(2) << setfill('0') << utcdStop.sec << "."
					 << setw(3) << setfill('0') << utcdStop.millisec() ;

			XMLOstream::Tag rawvalStart(xos, "validitystart", startStr.str() , "", true);
			XMLOstream::Tag rawvalStop(xos, "validitystop", stopStr.str() , "", true);
			XMLOstream::Tag rawdLOrbit(xos, "downlinkorbitnumber", _dataFiles[i].getOrbit(), "", true);
			XMLOstream::Tag rawFileType(xos, "filetype", _dataFiles[i].getFileType(), "", true);
			XMLOstream::Tag rawsensorId(xos, "sensorid", _dataFiles[i].getSensorId(), "", true);
			XMLOstream::Tag rawacqStationId(xos, "acquisitionstationid", _dataFiles[i].getAcqStationId(), "", true);
			XMLOstream::Tag rawprocStationId(xos, "processingstationid", _dataFiles[i].getProcStationId(), "", true);
			XMLOstream::Tag rawSatId(xos, "satelliteid", _dataFiles[i].getSatelliteId(), "", true);
			XMLOstream::Tag rawmission(xos, "mission", _dataFiles[i].getMission(), "", true);
		rawFile.close() ;
	}
	
	listOfFiles.close() ;
	xos.close() ;
	outFile.close() ;
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"L0 Processing order file " << l0pOrderFileName << " generated" )
}

unsigned int L0POrder::read(const std::string& fileName)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to read L0 Processing order file " << fileName )

	ACS_COND_THROW(!File::exists(fileName), exIOException("L0 Processing order file "+fileName+" does not exists") ) ;
	rsResourceSet rs ;
	XMLIstream istr(fileName) ;
	istr >> rs ;
	unsigned int count=0 ;
	rs.getValue("List_of_Files.count.value",count) ;
	for(unsigned int i=0; i<count; i++)
	{
		ostringstream root ;
		root << "List_of_Files.File[" << i << "]." ;
		L0POrder::L0PDataFile item ;
		//auxiliary variables to fill the item
		string 	fileName , fileType, startTimeStr, stopTimeStr;
		DateTime startTime,stopTime;		
		unsigned int orbit=0, sensorId=0, acqStationId=0, procStationId=0, satelliteId=0 , mission=0;
		//read the values and fill the item
		rs.getValue(root.str()+"filename", fileName) ;
		item.setFileName(fileName) ;
		
		rs.getValue(root.str()+"validitystart", startTimeStr) ;
		getTime(startTimeStr, startTime) ;
		item.setValidityStart(startTime) ;
	
		rs.getValue(root.str()+"validitystop", stopTimeStr) ;
		getTime(stopTimeStr, stopTime) ;
		item.setValidityStop(stopTime) ;
		
		rs.getValue(root.str()+"downlinkorbitnumber", orbit) ;
		item.setOrbit(orbit) ;
		
		rs.getValue(root.str()+"filetype", fileType) ;
		item.setFileType(fileType) ;
		
		rs.getValue(root.str()+"sensorid", sensorId) ;
		item.setSensorId(sensorId) ;
		
		rs.getValue(root.str()+"acquisitionstationid", acqStationId) ;
		item.setAcqStationId(acqStationId) ;
		
		rs.getValue(root.str()+"processingstationid", procStationId) ;
		item.setProcStationId(procStationId) ;
		
		rs.getValue(root.str()+"satelliteid", satelliteId) ;
		item.setSatelliteId(satelliteId) ;
		
		rs.getValue(root.str()+"mission", mission) ;
		item.setMission(mission) ;
		
		_dataFiles.push_back(item) ;
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"L0 Processing order file " << fileName << " successfully read" )
	return count ;
}


void L0POrder::getDataFiles(std::vector<L0POrder::L0PDataFile>& result) const 
{
	result = _dataFiles ;
}

void L0POrder::addDataFile(const L0POrder::L0PDataFile& dataFile)
{
	_dataFiles.push_back(dataFile) ;
}

void L0POrder::clear()
{
	_dataFiles.clear() ;
}


size_t L0POrder::size() const throw() 
{
    return _dataFiles.size() ;
}


void L0POrder::getTime(const std::string& valTime, DateTime& dt)
{
	//valTime must be of the format (23 chars) : YYYY-MM-DD hh:mm:ss.mmm
	ACS_COND_THROW( valTime.size() != 23 , exIllegalValueException("L0POrder::getTime : Bad Time string length ["+valTime+"]") ) ;
	unsigned short year  = atoi(valTime.substr(0,4).c_str()) ;
	unsigned short month = atoi(valTime.substr(5,2).c_str()) ;
	unsigned short day   = atoi(valTime.substr(8,2).c_str()) ;
	unsigned short hour  = atoi(valTime.substr(11,2).c_str()) ;
	unsigned short min   = atoi(valTime.substr(14,2).c_str()) ;
	unsigned short sec   = atoi(valTime.substr(17,2).c_str()) ;
	unsigned short msec  = atoi(valTime.substr(20,3).c_str()) ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Extracted parameters from input time " << valTime )
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"year=" << year << " - month=" << month << " - day=" << day << " - hour=" << hour << " - min=" << min 
							<< " - sec=" << sec << " - msec=" << msec )
	DateTime::UTCD utcd(year,month,day,hour,min,sec,msec) ;
	dt = utcd ;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// 							L0PDataFile inner class implementation
////////////////////////////////////////////////////////////////////////////////////////////////////
L0POrder::L0PDataFile::L0PDataFile() :
	_fileName(),
	_startTime(),
	_stopTime(),
	_orbit(),
	_fileType(),
	_sensorId(),
	_acqStationId(),
	_procStationId(),
	_satelliteId(),
	_mission()
{
}

L0POrder::L0PDataFile::~L0PDataFile() throw() 
{
}

//
// copy contructor
//
L0POrder::L0PDataFile::L0PDataFile(const L0PDataFile &r) :
	_fileName(r._fileName),
	_startTime(r._startTime),
	_stopTime(r._stopTime),
	_orbit(r._orbit),
	_fileType(r._fileType),
	_sensorId(r._sensorId),
	_acqStationId(r._acqStationId),
	_procStationId(r._procStationId),
	_satelliteId(r._satelliteId),
	_mission(r._mission)

{
}

//
// operator =
//
L0POrder::L0PDataFile& 
L0POrder::L0PDataFile::operator=(const L0PDataFile &e) 
{
	if (this != &e) {
		_fileName = e._fileName ;
		_startTime = e._startTime ;
		_stopTime = e._stopTime ;
		_orbit = e._orbit ;
		_fileType = e._fileType ;
		_sensorId = e._sensorId ;
		_acqStationId = e._acqStationId ;
		_procStationId = e._procStationId ;
		_satelliteId = e._satelliteId ;
		_mission = e._mission ;
	}
	return *this ;
}

string L0POrder::L0PDataFile::dump()
{
	ostringstream res , start, stop;
	_startTime.asISO8601(start,3) ;
	_stopTime.asISO8601(stop,3) ;
	res << "L0PDataFile : FileName=" << _fileName << " - ValStart=" << start.str() << " - ValStop=" << stop.str()
		<< " - Orbit=" << _orbit << " - fileType=" << _fileType << " - SensorId = " << _sensorId << " - AcqStatId=" << _acqStationId
		<< " - ProcStatId=" << _procStationId << " - SatId=" << _satelliteId << " - Mission=" << _mission ;
	return res.str() ;
}


const string &L0POrder::L0PDataFile::getFileName() const throw() { return _fileName ; }

void L0POrder::L0PDataFile::setFileName(const string& fileName) { _fileName = fileName ; }

const DateTime &L0POrder::L0PDataFile::getValidityStart() const throw() { return _startTime ; }

void L0POrder::L0PDataFile::setValidityStart(const DateTime& valStart) { _startTime = valStart ; }

const DateTime &L0POrder::L0PDataFile::getValidityStop() const throw() {	return _stopTime ; }

void L0POrder::L0PDataFile::setValidityStop(const DateTime& valStop) { _stopTime = valStop ; }

unsigned int L0POrder::L0PDataFile::getOrbit() const throw() { return _orbit ; }

void L0POrder::L0PDataFile::setOrbit(unsigned int orbit) throw() { _orbit = orbit ; }

const string &L0POrder::L0PDataFile::getFileType() const throw() { return _fileType ; }

void L0POrder::L0PDataFile::setFileType(const string& fileType) { _fileType = fileType ; }

unsigned int L0POrder::L0PDataFile::getSensorId() const throw() { return _sensorId ; }

void L0POrder::L0PDataFile::setSensorId(unsigned int sId) throw() {	_sensorId = sId ; }

unsigned int L0POrder::L0PDataFile::getAcqStationId() const throw() { return _acqStationId ; }

void L0POrder::L0PDataFile::setAcqStationId(unsigned int aId) throw() { _acqStationId = aId ; }

unsigned int L0POrder::L0PDataFile::getProcStationId() const throw() { return _procStationId ; }

void L0POrder::L0PDataFile::setProcStationId(unsigned int pId) throw() { _procStationId = pId ; }

unsigned int L0POrder::L0PDataFile::getSatelliteId() const throw() { return _satelliteId ; }

void L0POrder::L0PDataFile::setSatelliteId(unsigned int satId) throw() { _satelliteId = satId ; }

unsigned int L0POrder::L0PDataFile::getMission() const throw() { return _mission ; }

void L0POrder::L0PDataFile::setMission(unsigned int mission) throw() { _mission = mission ; }

_ACS_END_NAMESPACE








