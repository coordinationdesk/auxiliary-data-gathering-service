/*

	Copyright 1995-2001, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. DIWorkOrder Library $

	$Id$

	$Author$
	
	$Log$
	Revision 1.9  2014/02/13 10:07:55  giucas
	New log macros adopted
	
	Revision 1.8  2012/04/20 07:04:01  giucas
	Compilation warnings removed
	
	Revision 1.7  2005/06/23 13:27:58  giucas
	Added getFileTypes method
	
	Revision 1.6  2004/12/03 10:36:56  giucas
	Added remove and get file methods
	
	Revision 1.5  2004/12/03 09:42:37  giucas
	Header fixed
	
	Revision 1.4  2004/11/17 15:39:35  giucas
	Changed handling and interfaces for Auxiliary Files
	
	Revision 1.3  2004/11/15 17:34:14  giucas
	Changed getAuxInputData sign and solved bug
	
	Revision 1.2  2004/11/15 15:54:46  giucas
	Added getAuxInputData method filtering for a specific FileType
	
	Revision 1.1  2004/10/11 14:10:43  giucas
	First release
	

*/
#include <sstream>
#include <GoceDisWorkOrder.h>

#include <ConfigurationSingleton.h>
#include <rsPushNSpace.h>
#include <DateTime.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(GoceDisWorkOrder)


// constructor
GoceDisWorkOrder::GoceDisWorkOrder(bool UseConfSet) :
	disWorkOrder(UseConfSet), //load default DIWorkOrder information
	_confSet(ConfigurationSingleton::instance()->get()),
	_auxInfoData(),
	_auxInfoDataRange()
{
    if(UseConfSet)
    {
        //load Goce specific informations
		unsigned int n=0;
		_confSet.getValue("GoceConf.List_of_InputFiles.count.value",n);
		for(unsigned int i=0; i<n; i++)
		{
			ostringstream num;
			num << i;
			string fileType="", fileName="";
			string startStr="", stopStr="";
			_confSet.getValue("GoceConf.List_of_InputFiles.InputFile["+num.str()+"].FileType",fileType);
			if(fileType.empty())
			{
				ostringstream msg;
				msg << "GoceDisWorkOrder ERROR : Empty Input File Type for input file number "  << i+1;
				exIllegalValueException e(msg.str());
				ACS_THROW(e);			
			}
			_confSet.getValue("GoceConf.List_of_InputFiles.InputFile["+num.str()+"].FileNamePath",fileName);
			_confSet.getValue("GoceConf.List_of_InputFiles.InputFile["+num.str()+"].TimeInterval.Start",startStr);
			_confSet.getValue("GoceConf.List_of_InputFiles.InputFile["+num.str()+"].TimeInterval.Stop",stopStr);
			
			GoceDisWorkOrder::addFile(fileType,fileName,startStr,stopStr);
		}
    }
}

GoceDisWorkOrder::~GoceDisWorkOrder() throw()
{
}



void
GoceDisWorkOrder::show()
{
	disWorkOrder::show();
    for(map<string, TimeOrderedAuxList>::iterator it = _auxInfoData.begin(); it != _auxInfoData.end(); it++)
	{
    	TimeOrderedAuxList currList = it->second;
		for(TimeOrderedAuxList::iterator lIt = currList.begin(); lIt != currList.end(); lIt++)
		{
			lIt->show();
		}
	}
}



GoceDisWorkOrder& 
GoceDisWorkOrder::operator>>(ostream & os) 
{
    XMLOstream xst(os,"DIWorkOrderFile");
        XMLOstream::Tag DIOrder(xst , "DIOrder");
            XMLOstream::Tag Satellite(xst , "Satellite", disWorkOrder::getSatellite());
            Satellite.close();
            {
                ostringstream tmp;
                tmp << disWorkOrder::getMission();
                XMLOstream::Tag Mission(xst , "Mission",tmp.str());
                Mission.close();
            }
            {
                ostringstream tmp;
                tmp << disWorkOrder::getDataRate();
                XMLOstream::Tag DataRate(xst , "DataRate",tmp.str());
                DataRate.close();
            }
            XMLOstream::Tag Sensor(xst , "Sensor",disWorkOrder::getSensor());
            Sensor.close();
            XMLOstream::Tag AcqStation(xst , "AcqStation",disWorkOrder::getAcqStation());
            AcqStation.close();
            XMLOstream::Tag ProcessingCentreID(xst , "ProcessingCentreID",disWorkOrder::getProcessingCentreID());
            ProcessingCentreID.close();
            XMLOstream::Tag MessageQueuePath(xst , "MessageQueuePath",disWorkOrder::getMessageQueuePath());
            MessageQueuePath.close();
            XMLOstream::Tag ProcessingStage(xst , "ProcessingStage",disWorkOrder::getProcessingStage());
            ProcessingStage.close();
        DIOrder.close();
        XMLOstream::Tag TelemetryInput(xst , "TelemetryInput");
            XMLOstream::Tag RFPath(xst , "RFPath");
                {
                    ostringstream tmp;
                    tmp << disWorkOrder::getRFPathID();
                    XMLOstream::Tag ID(xst , "ID",tmp.str());
                    ID.close();
               }
                XMLOstream::Tag Name(xst ,"Name", disWorkOrder::getRFPathName());
                Name.close();
            RFPath.close();
            {
                std::vector<disWorkOrder::device> inputDevices = disWorkOrder::getInputDevices();
				ostringstream tmp;
                tmp << "count=\"" << inputDevices.size() << "\"";
                XMLOstream::Tag ListOfInput(xst , "ListOfInput","",tmp.str());
                for(unsigned int i=0; i< inputDevices.size(); i++)
                {
                    XMLOstream::Tag Input(xst , "Input");
                        ostringstream tmp1;
                        tmp1 << inputDevices[i]._Channel;
                        XMLOstream::Tag Channel(xst , "Channel",tmp1.str());
                        Channel.close();
                        XMLOstream::Tag Device(xst , "Device");
                            XMLOstream::Tag PathName(xst , "PathName",inputDevices[i]._PathName);
                            PathName.close();
                            XMLOstream::Tag Type(xst , "Type",inputDevices[i]._Type);
                            Type.close();
                        Device.close();
                        XMLOstream::Tag FileName(xst , "FileName",inputDevices[i]._FileName);
                        FileName.close();
                    Input.close();
                }
                ListOfInput.close();
            }
        TelemetryInput.close();
        XMLOstream::Tag TelemetryOutput(xst , "TelemetryOutput");
            XMLOstream::Tag ProcessId(xst , "ProcessId",disWorkOrder::getProcessId());
            ProcessId.close();
			std::vector<disWorkOrder::device> outputDevices = disWorkOrder::getOutputDevices();
            ostringstream tmp;
            tmp << "count=\"" << outputDevices.size() << "\"";
            XMLOstream::Tag ListOfOutput(xst , "ListOfOutput","",tmp.str());
            for(unsigned int i=0; i< outputDevices.size(); i++)
            {
                XMLOstream::Tag Output(xst , "Output");
                    ostringstream tmp1;
                    tmp1 << outputDevices[i]._Channel;
                    XMLOstream::Tag Channel(xst , "Channel",tmp1.str());
                    Channel.close();
                    XMLOstream::Tag Device(xst , "Device");
                        XMLOstream::Tag PathName(xst , "PathName",outputDevices[i]._PathName);
                        PathName.close();
                        XMLOstream::Tag Type(xst , "Type",outputDevices[i]._Type);
                        Type.close();
                    Device.close();
                    XMLOstream::Tag FileName(xst , "FileName",outputDevices[i]._FileName);
                    FileName.close();
                Output.close();
            }
            ListOfOutput.close();
        TelemetryOutput.close();
        XMLOstream::Tag Downlink(xst , "Downlink");
            XMLOstream::Tag StartTime(xst , "StartTime",disWorkOrder::getStartTime());
            StartTime.close();
            XMLOstream::Tag StopTime(xst , "StopTime",disWorkOrder::getStopTime());
            StopTime.close();
            ostringstream tempStr;
			tempStr << disWorkOrder::getOrbit();
			XMLOstream::Tag Orbit(xst , "Orbit",tempStr.str());
            Orbit.close();
            ostringstream tempStr2;
            tempStr2 << disWorkOrder::getDuration();
			XMLOstream::Tag Duration(xst , "Duration",tempStr2.str(), "unit=\"s\"");
            Duration.close();
        Downlink.close();
		XMLOstream::Tag goceConf(xst , "GoceConf");
		{
			ostringstream tmp;
			tmp << "count=\"" << GoceDisWorkOrder::numberOfFiles() << "\"";
			XMLOstream::Tag listOfInputFiles(xst , "List_of_InputFiles","",tmp.str());
    		for(map<string, TimeOrderedAuxList>::iterator it = _auxInfoData.begin(); it != _auxInfoData.end(); it++)
			{
    			TimeOrderedAuxList currList = it->second;
				for(TimeOrderedAuxList::iterator lIt = currList.begin(); lIt != currList.end(); lIt++)
				{
					lIt->exportXML(xst);
				}
			}			
            listOfInputFiles.close();
		}
		goceConf.close();
    return *this;
}



void GoceDisWorkOrder::addFile(const string& type,const string& name, const string& start, const string& stop)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Adding file " << name << " with Validity Interval [" << setprecision(12) << start 
							<< " , " << stop << "] and type " << type )
	AuxFileInfo info(type,name,start,stop);
	_auxInfoData[type].insert(info);
	
	//update minValStart and maxValStop values for this specific file type
	set<AuxFileInfo,ltAuxInfo>::iterator itFirst = _auxInfoData[type].begin();
	set<AuxFileInfo,ltAuxInfo>::iterator itLast  = _auxInfoData[type].end();
	itLast--;
	double minValStart = (*itFirst).valStart;
	double maxValStop  = (*itLast).valStop;
	_auxInfoDataRange[type].first  = minValStart;
	_auxInfoDataRange[type].second = maxValStop;
}


string  GoceDisWorkOrder::getMinCoverFile(const string& type,const double& start, const double& stop)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Retrieving Min cover file for input interval [ " 
			<< setprecision(12) << start << " , " << stop << " ] and file type " << type )

	//check if input file type is recognized
	ACS_COND_THROW( _auxInfoDataRange.find(type) == _auxInfoDataRange.end() , 
		exIllegalValueException("GoceDisWorkOrder::getMinCoverFile ERROR : Unknown Input File Type ["+type+"] . You must add a file of that file type before.") ) ;
	
	//retrieve minValStart and maxValStop for that file type
	double minValStart = _auxInfoDataRange[type].first;
	double maxValStop  = _auxInfoDataRange[type].second;
	
	//check the [MIN,MAX] ranges respect to input interval
	if(start < minValStart)
	{
		ostringstream msg;
		msg << "GoceDisWorkOrder::getMinCoverFile ERROR : Input Start Time [" << setprecision(12) << start
			<< "] is less than MIN Start Time [" << minValStart << "] for file type " << type;
		exIllegalValueException e(msg.str());
		ACS_THROW(e);
	}

	if(stop > maxValStop)
	{
		ostringstream msg;
		msg << "GoceDisWorkOrder::getMinCoverFile ERROR : Input Stop Time [" << setprecision(12) << stop
			<< "] is greater than MAX Stop Time [" << maxValStop << "] for file type " << type;
		exIllegalValueException e(msg.str());
		ACS_THROW(e);
	}
	
	//retrieve the list of AuxFiles associated to the input file type and try to find the 'good' file
	TimeOrderedAuxList auxInfoList = _auxInfoData[type]; 
	//point to the first element and iterate...
	TimeOrderedAuxList::iterator it = auxInfoList.begin();
	string result="";
	double minDiff = 0.0;
	while( it != auxInfoList.end() )
	{
		double currentStart = (*it).valStart;
		double currentStop  = (*it).valStop;
		bool covered = ( (currentStart <= start) && (stop <= currentStop) );

		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Processing element : " << (*it).show() )

		if( covered )
		{
			double currentDiff = start - currentStart;
			if( (minDiff == 0.0) || (currentDiff < minDiff) )
			{
				minDiff = currentDiff;
				result = (*it).fileName;
			}
		}
		it++;
	}
	
	if( result.empty())
	{
		ostringstream msg;
		msg << "GoceDisWorkOrder::getMinCoverFile ERROR : No file covering input Interval Time [" << setprecision(12) 
			<< start << " , " << stop << "]";
		exIllegalValueException e(msg.str());
		ACS_THROW(e);
	}
	return result;
}


vector<string> GoceDisWorkOrder::getFiles()
{
    vector<string> result;
	for(map<string, TimeOrderedAuxList>::iterator it = _auxInfoData.begin(); it != _auxInfoData.end(); it++)
	{
    	TimeOrderedAuxList currList = it->second;
		for(TimeOrderedAuxList::iterator lIt = currList.begin(); lIt != currList.end(); lIt++)
		{
			result.push_back(lIt->fileName);
		}
	}
	return result;
}

vector<string> GoceDisWorkOrder::getFiles(const string& type)
{
    map<string, TimeOrderedAuxList>::iterator it;
	ACS_COND_THROW(  (it = _auxInfoData.find(type)) == _auxInfoData.end() , exIllegalValueException("GoceDisWorkOrder::getFiles : Unknown File Type "+type) ) ;
	vector<string> result;
    TimeOrderedAuxList currList = it->second;
	for(TimeOrderedAuxList::iterator lIt = currList.begin(); lIt != currList.end(); lIt++)
	{
		result.push_back(lIt->fileName);
	}
	return result;
}

vector<string> GoceDisWorkOrder::getFileTypes()
{
    vector<string> result;
	for(map<string, TimeOrderedAuxList>::iterator it = _auxInfoData.begin(); it != _auxInfoData.end(); it++)
	{
    	string currentFileType = it->first;
		if( find( result.begin(), result.end(), currentFileType) == result.end() )
			result.push_back(currentFileType);
	}
	return result;
}

void GoceDisWorkOrder::removeFile(const string& type, const string& name)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Removing File [ " << name << " ] and file type " << type )

    map<string, TimeOrderedAuxList>::iterator it;
	ACS_COND_THROW(  (it = _auxInfoData.find(type)) == _auxInfoData.end() , exIllegalValueException("GoceDisWorkOrder::removeFile : Unknown File Type "+type) ) ;
	bool deleted=false;
	//iterate through the list to search for the 
	for(TimeOrderedAuxList::iterator lIt = (it->second).begin(); lIt != (it->second).end(); lIt++)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Processing Element " << endl << lIt->show())
		if( lIt->fileName == name)
		{
			(it->second).erase(lIt);
			deleted = true;
			break;
		}
	}
	
	ACS_COND_THROW(!deleted , exIllegalValueException("GoceDisWorkOrder::removeFile : Unknown File Name "+name) )
	if( _auxInfoData[type].size() > 0)
	{
		//update minValStart and maxValStop values for this specific file type
		set<AuxFileInfo,ltAuxInfo>::iterator itFirst = _auxInfoData[type].begin();
		set<AuxFileInfo,ltAuxInfo>::iterator itLast  = _auxInfoData[type].end();
		itLast--;
		double minValStart = (*itFirst).valStart;
		double maxValStop  = (*itLast).valStop;
		_auxInfoDataRange[type].first  = minValStart;
		_auxInfoDataRange[type].second = maxValStop;
	}
}


void GoceDisWorkOrder::removeFile(const string& type)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Removing Files for FileType "<< type )

    map<string, TimeOrderedAuxList>::iterator it;
	ACS_COND_THROW(  (it = _auxInfoData.find(type)) == _auxInfoData.end() , exIllegalValueException("GoceDisWorkOrder::removeFile : Unknown File Type "+type) ) ;
	
	_auxInfoData.erase(type);
	_auxInfoDataRange.erase(type);
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"All Files deleted... " )
}


//////////////////////////////////////////////////////////////////////
//   Private methods
//////////////////////////////////////////////////////////////////////

int GoceDisWorkOrder::numberOfFiles()
{
	int result=0;
    for(map<string, TimeOrderedAuxList>::iterator it = _auxInfoData.begin(); it != _auxInfoData.end(); it++)
	{
    	result += (it->second).size();
	}
	return (result);
}

//////////////////////////////////////////////////////////////////////
//   AuxFileInfo implementation
//////////////////////////////////////////////////////////////////////
GoceDisWorkOrder::AuxFileInfo::AuxFileInfo(const string& type,const string& name, const string& start, const string& stop) :
	fileType(type),
	fileName(name),
	valStart(0.0),
	valStop(0.0),
	valStartString(start),
	valStopString(stop)
{
	valStart = convertTime(valStartString, valStart);
	valStop = convertTime(valStopString, valStop);
}

GoceDisWorkOrder::AuxFileInfo::AuxFileInfo() :
	fileType(""),
	fileName(""),
	valStart(0.0),
	valStop(0.0),
	valStartString(""),
	valStopString("")
{
}

GoceDisWorkOrder::AuxFileInfo::~AuxFileInfo()
{
}

GoceDisWorkOrder::AuxFileInfo::AuxFileInfo(const AuxFileInfo& e) :
	fileType(""),
	fileName(""),
	valStart(0.0),
	valStop(0.0),
	valStartString(""),
	valStopString("")
{
	*this = e;
}

GoceDisWorkOrder::AuxFileInfo&
GoceDisWorkOrder::AuxFileInfo::operator=(const AuxFileInfo& e)
{
	if (this != &e) 
	{
		fileType 		= e.fileType ;
		fileName 		= e.fileName ;
		valStart 		= e.valStart ;
		valStop 		= e.valStop ;
		valStartString	= e.valStartString;
		valStopString	= e.valStopString;
	}
	return *this ;
}

bool 
GoceDisWorkOrder::AuxFileInfo::operator<=(const AuxFileInfo &x) const 
{ 
	return valStart <= x.valStart ; 
}

bool 
GoceDisWorkOrder::AuxFileInfo::operator<(const AuxFileInfo &x) const 
{ 
	return valStart < (x.valStart) ; 
}

bool 
GoceDisWorkOrder::AuxFileInfo::operator==(const AuxFileInfo &x) const 
{ 
	return (fileName == x.fileName) && (valStart == x.valStart) &&  (valStop == x.valStop); 
}

string
GoceDisWorkOrder::AuxFileInfo::show() const
{ 
	ostringstream result;
	result << "AuxFileInfo::fileType       = [" << this->fileType << "]" << endl
		   << "AuxFileInfo::fileName       = [" << this->fileName << "]" << endl
		   << "AuxFileInfo::valStart       = [" << setprecision(12) << this->valStart << "]"<< endl
		   << "AuxFileInfo::valStop        = [" << setprecision(12) << this->valStop  << "]"<< endl
		   << "AuxFileInfo::valStartString = [" << setprecision(12) << this->valStartString << "]"<< endl
		   << "AuxFileInfo::valStopString  = [" << setprecision(12) << this->valStopString  << "]"<< endl;
	return result.str(); 
}

void
GoceDisWorkOrder::AuxFileInfo::exportXML(XMLOstream & xst) const
{ 
    XMLOstream::Tag inputFile(xst , "InputFile");
        XMLOstream::Tag fileType(xst , "FileType",this->fileType,"",true);
        XMLOstream::Tag fileNamePath(xst , "FileNamePath",this->fileName,"",true);
		XMLOstream::Tag interval(xst , "TimeInterval");
			XMLOstream::Tag start(xst , "Start",this->valStartString,"",true);
			XMLOstream::Tag stop(xst , "Stop",this->valStopString,"",true);
		interval.close();
    inputFile.close();
}

double& GoceDisWorkOrder::AuxFileInfo::convertTime(const string& inTimeStr, double& result)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Converting input time [" << inTimeStr<< "]" )
	
	//parse string and extract info
	
	//day
	istringstream day(inTimeStr.substr(8,2));
	int days;
	day >> days;
	
	//month
	istringstream month(inTimeStr.substr(5,2));
	int months;
	month >> months;
	
	//year
	istringstream year(inTimeStr.substr(0,4));
	int years;
	year >> years;
	
	//hour
	istringstream hour(inTimeStr.substr(11,2));
	int hours;
	hour >> hours;
	
	//min
	istringstream min(inTimeStr.substr(14,2));
	int mins;
	min >> mins;
	
	//sec
	istringstream sec(inTimeStr.substr(17,2));
	int secs;
	sec >> secs;
	
	//msec
	istringstream msec(inTimeStr.substr(20,3));
	int msecs;
	msec >> msecs;
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Parsed value : YEARS  [" << years<< "][" << inTimeStr.substr(0,4) << "]" )
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Parsed value : MONTHS [" << months<< "][" << inTimeStr.substr(5,2) << "]")
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Parsed value : DAYS   [" << days<< "][" << inTimeStr.substr(8,2) << "]")
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Parsed value : HOURS  [" << hours<< "][" << inTimeStr.substr(11,2) << "]")
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Parsed value : MIN    [" << mins<< "][" << inTimeStr.substr(14,2) << "]")
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Parsed value : SEC    [" << secs<< "][" << inTimeStr.substr(17,2) << "]")
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Parsed value : MSEC   [" << msecs<< "][" << inTimeStr.substr(20,3) << "]" );

	
	DateTime::UTCD utcd(years,months,days,hours,mins,secs,msecs);
	DateTime dt(utcd);
	DateTime::MJD2000 mjd2000(dt);
	
	result = mjd2000.jd();
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Converted input time [" << setprecision(12) << result<< "]" )

	return (result);
}
_ACS_END_NAMESPACE

