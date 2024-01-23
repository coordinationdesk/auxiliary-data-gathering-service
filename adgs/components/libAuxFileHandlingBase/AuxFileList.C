// PRQA S 1050 EOF
#if 0

	Copyright 1995-2001, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. AuxFileList $

	$Id$

	$Author$
	
	$Log$
	Revision 2.1  2013/11/19 13:36:15  ricfer
	Compiler warnings removed. New logging macros adopted.
	
	Revision 2.0  2006/03/01 16:41:23  serfol
	Exception 2.0 I/F adapted
	
	Revision 1.1.1.1  2006/01/31 13:44:10  seralb
	Imported libAuxFileHandlingBase
	
	Revision 1.1.1.1  2004/12/06 11:21:40  giucas
	Imported libAuxFileHandling
	

#endif 

#include <sstream>
#include <AuxFileList.h>

#include <ConfigurationSingleton.h>
#include <rsPushNSpace.h>
#include <DateTime.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(AuxFileList)

// constructor
AuxFileList::AuxFileList() :
 _auxInfoData(),
 _auxInfoDataRange()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
}

AuxFileList::~AuxFileList() throw() 
{
}

AuxFileList::AuxFileList(const AuxFileList& e) :
 _auxInfoData(e._auxInfoData),
 _auxInfoDataRange(e._auxInfoDataRange)
{
}

AuxFileList& AuxFileList::operator=(const AuxFileList& e)
{
	if (this != &e) 
	{
		_auxInfoData		= e._auxInfoData;
		_auxInfoDataRange	= e._auxInfoDataRange;
	}
	return *this ;
}

void AuxFileList::addFile(const string& type,const string& name, const string& start, const string& stop)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "AuxFileList::addFile : Adding file " << name << " with Validity Interval [" << setprecision(12) << start << " , " << stop << "] and type " << type);
	
	AuxFileInfo info(type,name,start,stop);
	_auxInfoData[type].insert(info);
	
	//update minValStart and maxValStop values for this specific file type
	set<AuxFileInfo,ltAuxInfo>::iterator itFirst = _auxInfoData[type].begin();
	set<AuxFileInfo,ltAuxInfo>::iterator itLast  = _auxInfoData[type].end();
	itLast--;
	long double minValStart = (*itFirst).valStart;
	long double maxValStop  = (*itLast).valStop;
	_auxInfoDataRange[type].first  = minValStart;
	_auxInfoDataRange[type].second = maxValStop;
}

void AuxFileList::getMinFileInfo(const string& type, string& name, long double& start, long double& stop) const 
{
    map<string, TimeOrderedAuxList>::const_iterator it;
	if(  (it = _auxInfoData.find(type)) == _auxInfoData.end() )
	{
		ACS_THROW(exIllegalValueException("Unknown File Type "+type)) ; // PRQA S 3081
	}
	//point to the first element in list (the min)
	set<AuxFileInfo,ltAuxInfo>::const_iterator itFirst = it->second.begin();
	name  = itFirst->fileName;
	start = itFirst->valStart;
	stop  = itFirst->valStop;
}

long double AuxFileList::getMinValidityStart(const string& type) const 
{
    map<string, pair<long double,long double> >::const_iterator it;
	if(  (it = _auxInfoDataRange.find(type)) == _auxInfoDataRange.end() )
	{
		ACS_THROW(exIllegalValueException("Unknown File Type "+type)) ; // PRQA S 3081
	}
	return (it->second).first;
}

long double AuxFileList::getMaxValidityStop(const string& type) const 
{
    map<string, pair<long double,long double> >::const_iterator it;
	if(  (it = _auxInfoDataRange.find(type)) == _auxInfoDataRange.end() )
	{
		ACS_THROW(exIllegalValueException("Unknown File Type "+type)) ; // PRQA S 3081
	}
	return (it->second).second;
}

string  AuxFileList::getMinCoverFile(const string& type,long double start, long double stop) const 
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Retrieving Min cover file for input interval [ " << setprecision(12) << start << " , " << stop << " ] and file type " << type);

	//check if input file type is recognized
    map<string, pair<long double,long double> >::const_iterator it = _auxInfoDataRange.find(type) ;
	if( it == _auxInfoDataRange.end() )
	{
		ACS_THROW(exIllegalValueException("Unknown File Type "+type)) ; // PRQA S 3081
	}
	
	//retrieve minValStart and maxValStop for that file type
	long double minValStart = it->second.first;
	long double maxValStop  = it->second.second;
	
	//check the [MIN,MAX] ranges respect to input interval
	if(start < minValStart)
	{
		ostringstream msg;
		msg << "ERROR : Input Start Time [" << setprecision(12) << start // PRQA S 4400
			<< "] is less than MIN Start Time [" << minValStart << "] for file type " << type;
		ACS_THROW(exIllegalValueException(msg.str())); // PRQA S 3081
	}

	if(stop > maxValStop)
	{
		ostringstream msg;
		msg << "ERROR : Input Stop Time [" << setprecision(12) << stop // PRQA S 4400
			<< "] is greater than MAX Stop Time [" << maxValStop << "] for file type " << type;
		ACS_THROW(exIllegalValueException(msg.str())); // PRQA S 3081
	}
	
	//retrieve the list of AuxFiles associated to the input file type and try to find the 'good' file
	const TimeOrderedAuxList &auxInfoList = _auxInfoData.find(type)->second ; 
	//point to the first element and iterate...
	TimeOrderedAuxList::const_iterator itl = auxInfoList.begin();
	string result="";
	long double minDiff = 0.0;
	while( itl != auxInfoList.end() )
	{
		long double currentStart = itl->valStart;
		long double currentStop  = itl->valStop;
		bool covered = ( (currentStart <= start) && (stop <= currentStop) );
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "AuxFileList::getMinCoverFile : Processing element : " << itl->show());
		if( covered )
		{
			long double currentDiff = start - currentStart;
			if( (minDiff == 0.0) || (currentDiff < minDiff) )
			{
				minDiff = currentDiff;
				result = itl->fileName;
			}
		}
		itl++;
	}
	
	if( result == "")
	{
		ostringstream msg;
		msg << "ERROR : No file covering input Interval Time [" << setprecision(12)  // PRQA S 4400
			<< start << " , " << stop << "]";
		ACS_THROW(exIllegalValueException(msg.str())); // PRQA S 3081
	}
	return result;
}


vector<string> AuxFileList::getFiles() const 
{
    vector<string> result;
	for(map<string, TimeOrderedAuxList>::const_iterator it = _auxInfoData.begin(); it != _auxInfoData.end(); it++)
	{
    	const TimeOrderedAuxList &currList = it->second;
		for(TimeOrderedAuxList::const_iterator lIt = currList.begin(); lIt != currList.end(); lIt++)
		{
			result.push_back(lIt->fileName);
		}
	}
	return result;
}

vector<string> AuxFileList::getFiles(const string& type) const 
{
    map<string, TimeOrderedAuxList>::const_iterator it;
	if(  (it = _auxInfoData.find(type)) == _auxInfoData.end() )
	{
		ACS_THROW(exIllegalValueException("Unknown File Type "+type)) ; // PRQA S 3081
	}
	vector<string> result;
    const TimeOrderedAuxList &currList = it->second;
	for(TimeOrderedAuxList::const_iterator lIt = currList.begin(); lIt != currList.end(); lIt++)
	{
		result.push_back(lIt->fileName);
	}
	return result;
}

void AuxFileList::removeFile(const string& type, const string& name)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Removing File [ " << name << " ] and file type " << type);
    map<string, TimeOrderedAuxList>::iterator it;
	if(  (it = _auxInfoData.find(type)) == _auxInfoData.end() )
	{
		ACS_THROW(exIllegalValueException("Unknown File Type "+type)) ; // PRQA S 3081
	}
	bool deleted=false;
	//iterate through the list to search for the 
	for(TimeOrderedAuxList::iterator lIt = (it->second).begin(); lIt != (it->second).end(); lIt++)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Processing Element " << lIt->show());
		if( lIt->fileName == name)
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File Found... ");
			(it->second).erase(lIt);
			deleted = true;
			break;
		}
	}
	
	if(!deleted)
	{
		ACS_THROW(exIllegalValueException("Unknown File Name "+name)) ; // PRQA S 3081
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "File deleted... ");
	if( _auxInfoData[type].size() > 0)
	{
		//update minValStart and maxValStop values for this specific file type
		set<AuxFileInfo,ltAuxInfo>::iterator itFirst = _auxInfoData[type].begin();
		set<AuxFileInfo,ltAuxInfo>::iterator itLast  = _auxInfoData[type].end();
		itLast--;
		long double minValStart = (*itFirst).valStart;
		long double maxValStop  = (*itLast).valStop;
		_auxInfoDataRange[type].first  = minValStart;
		_auxInfoDataRange[type].second = maxValStop;
	}
}


void AuxFileList::removeFile(const string& type)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Removing Files for FileType "<< type);
    map<string, TimeOrderedAuxList>::iterator it;
	if(  (it = _auxInfoData.find(type)) == _auxInfoData.end() )
	{
		ACS_THROW(exIllegalValueException("Unknown File Type "+type)) ; // PRQA S 3081
	}
	
	_auxInfoData.erase(type);
	_auxInfoDataRange.erase(type);
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "All Files deleted... ");
}

size_t AuxFileList::numberOfFiles() const
{
	size_t result=0;
    for(map<string, TimeOrderedAuxList>::const_iterator it = _auxInfoData.begin(); it != _auxInfoData.end(); it++)
	{
    	result += (it->second).size(); // PRQA S 3084
	}
	return (result);
}


//////////////////////////////////////////////////////////////////////
//   Private methods
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
//   AuxFileInfo implementation
//////////////////////////////////////////////////////////////////////
AuxFileList::AuxFileInfo::AuxFileInfo(const string& type,const string& name, const string& start, const string& stop) :
	fileType(type),
	fileName(name),
	valStart(0),
	valStop(0),
	valStartString(start),
	valStopString(stop)
{
	valStart = convertTime(valStartString, valStart);
	valStop = convertTime(valStopString, valStop);
}

AuxFileList::AuxFileInfo::AuxFileInfo() :
	fileType(""),
	fileName(""),
	valStart(0.0),
	valStop(0.0),
	valStartString(""),
	valStopString("")
{
}

AuxFileList::AuxFileInfo::~AuxFileInfo() throw() 
{
}

AuxFileList::AuxFileInfo::AuxFileInfo(const AuxFileInfo& e) :
	fileType(e.fileType),
	fileName(e.fileName),
	valStart(e.valStart),
	valStop(e.valStop),
	valStartString(e.valStartString),
	valStopString(e.valStopString)
{
}

AuxFileList::AuxFileInfo&
AuxFileList::AuxFileInfo::operator=(const AuxFileInfo& e)
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


string
AuxFileList::AuxFileInfo::show() const
{ 
	ostringstream result;
	result << "AuxFileInfo::fileType       = [" << fileType << "]" << '\n'
		   << "AuxFileInfo::fileName       = [" << fileName << "]" << '\n'
		   << "AuxFileInfo::valStart       = [" << setprecision(12) << valStart << "]"<< '\n' // PRQA S 4400 4
		   << "AuxFileInfo::valStop        = [" << setprecision(12) << valStop  << "]"<< '\n'
		   << "AuxFileInfo::valStartString = [" << setprecision(12) << valStartString << "]"<< '\n'
		   << "AuxFileInfo::valStopString  = [" << setprecision(12) << valStopString  << "]"<< '\n';
	return result.str(); 
}

void
AuxFileList::AuxFileInfo::exportXML(XMLOstream & xst) const
{ 
    XMLOstream::Tag t1(xst , "InputFile");
      XMLOstream::Tag t2(xst , "FileType",fileType,"",true);
      XMLOstream::Tag t3(xst , "FileNamePath",fileName,"",true);
	  {
          XMLOstream::Tag t4(xst , "TimeInterval");
			XMLOstream::Tag t5(xst , "Start",valStartString,"",true);
			XMLOstream::Tag t6(xst , "Stop",valStopString,"",true);
      }
}

long double& AuxFileList::AuxFileInfo::convertTime(const string& inTimeStr, long double& result)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "AuxFileList::AuxFileInfo::convertTime : Converting input time [" << inTimeStr<< "]");
	//parse string and extract info (yyyymmdd_hhnnssuuuuuu)
	
	//day
	int days = atoi(inTimeStr.substr(6,2).c_str()) ;
	
	//month
	int months = atoi(inTimeStr.substr(4,2).c_str()) ;
	
	//year
	int years = atoi(inTimeStr.substr(0,4).c_str()) ;
	
	//hour
	int hours = atoi(inTimeStr.substr(9,2).c_str()) ;
	
	//min
	int mins = atoi(inTimeStr.substr(11,2).c_str()) ;
	
	//sec
	int secs = atoi(inTimeStr.substr(13,2).c_str()) ;
	
	//msec
	int msecs = atoi(inTimeStr.substr(15,3).c_str()) ;
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "AuxFileInfo::convertTime : Parsed value : YEARS  [" << years<< "][" << inTimeStr.substr(0,4) << "]"); 
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "AuxFileInfo::convertTime : Parsed value : MONTHS [" << months<< "][" << inTimeStr.substr(4,2) << "]");
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "AuxFileInfo::convertTime : Parsed value : DAYS   [" << days<< "][" << inTimeStr.substr(6,2) << "]");
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "AuxFileInfo::convertTime : Parsed value : HOURS  [" << hours<< "][" << inTimeStr.substr(9,2) << "]");
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "AuxFileInfo::convertTime : Parsed value : MIN    [" << mins<< "][" << inTimeStr.substr(11,2) << "]");
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "AuxFileInfo::convertTime : Parsed value : SEC    [" << secs<< "][" << inTimeStr.substr(13,2) << "]");
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "AuxFileInfo::convertTime : Parsed value : MSEC   [" << msecs<< "][" << inTimeStr.substr(15,3) << "]");

	DateTime::MJD2000 mjd2000(DateTime(DateTime::UTCD(years,months,days,hours,mins,secs,msecs))); // PRQA S 3000, 3010, 3050, 3081 
	
	result = mjd2000.jd();
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "AuxFileList::AuxFileInfo::convertTime : Converted input time [" << setprecision(12) << result<< "]");

	return result ; // PRQA S 4028
}

_ACS_END_NAMESPACE
