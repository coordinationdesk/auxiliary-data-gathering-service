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

	$Prod: $

	$Id$

	$Author$

	$Log$
	Revision 2.14  2014/12/12 16:45:20  enrcar
	EC:: Added method: ACSTimeConvWrap::getNumberOfLeapSecondsAtDate
	
	Revision 2.13  2014/04/14 14:01:19  marpas
	using new libACSTimeConverter 2.14
	interface and code rationalization
	
	Revision 2.12  2014/04/10 18:34:34  marpas
	qa warning fixed
	
	Revision 2.11  2014/04/08 12:29:12  marpas
	connection pool is now got from application.
	In case app does not exists or still has no pool, a default pool is asked - this could compromise multidatabase application.
	
	Revision 2.10  2014/02/03 15:31:04  giucas
	New macros adopted
	
	Revision 2.9  2013/03/12 17:54:18  marpas
	dbConnectionPool is mandatory in file actions
	
	Revision 2.8  2011/10/07 08:41:21  giucas
	excerr -> exF*
	
	Revision 2.7  2011/03/15 11:23:57  giucas
	Try/catch added during the downloader creation to avoid exiting in error when only methods without downloading have to be called
	
	Revision 2.6  2009/01/22 11:20:28  giucas
	Modified to handle satellite id value 0
	
	Revision 2.5  2007/11/30 14:13:08  giucas
	Checking the mission when retrieving the configuration
	
	Revision 2.4  2007/09/12 10:08:08  giucas
	Execute setParser for each method call and not only the first time the converter is instantiated
	
	Revision 2.3  2007/01/10 11:22:57  giucas
	Updated for IF changes (CFI 3.5 porting)
	
	Revision 2.2  2006/10/20 08:24:50  giucas
	Modified to align parsers IF changes
	
	Revision 2.1  2006/05/11 12:18:24  seralb
	Now multimission supported
	
	Revision 2.0  2006/03/01 16:39:23  serfol
	Exception 2.0 I/F adapted
	
	Revision 1.23  2006/01/31 14:03:34  seralb
	Added AuxDownloaderSingleton
	
	Revision 1.22  2005/11/21 11:12:53  giucas
	setSatellite methods changed.Implemented initOBTTimeConversion and getFileType methods
	
	Revision 1.21  2005/11/07 14:39:58  seralb
	InitOBCTimeConversion method now instance a OBTFileParser object
	
	Revision 1.20  2005/11/03 13:44:31  seralb
	Modify setSatelliteId and initOBTConverter
	
	Revision 1.19  2005/11/02 18:38:50  giucas
	Changed use of OBTConverter. Now using Singleton
	
	Revision 1.18  2005/10/26 14:21:13  seralb
	Added AuxFileParser singleton
	
	Revision 1.17  2005/10/12 11:11:08  seralb
	Added ascii cosmo compact
	
	Revision 1.16  2005/09/20 15:15:21  seralb
	TimeConverterException now is TimeConverterInitException in init methods
	
	Revision 1.15  2005/09/16 11:44:47  seralb
	Now parser on orbit data instead of OBT class
	
	Revision 1.14  2005/09/12 14:49:24  seralb
	Added debug
	
	Revision 1.13  2005/07/08 11:39:56  seralb
	Bug solved
	
	Revision 1.12  2005/07/08 11:37:41  seralb
	initOBCTimeConversion method now multisatellite
	
	Revision 1.11  2005/07/07 10:45:43  seralb
	Nonsense
	
	Revision 1.10  2005/07/06 15:06:49  seralb
	Added methods
	
	Revision 1.9  2005/07/04 09:14:17  seralb
	Removed some methods
	
	Revision 1.8  2005/05/31 14:58:09  marpas
	file action interface changed
	
	Revision 1.7  2005/04/22 14:50:59  giucas
	Moved includes from .h to .C
	

 */


#include <ACSTimeConvWrap.h>
#include <ConfigurationSingleton.h>
#include <OBTConverterFactory.h>
#include <OnBoardTime.h>
#include <TimeConverterSingleton.h>
#include <TimeConverter.h>
#include <AuxFileParserSingleton.h>
#include <AuxDownloader.h>
#include <AuxDownloaderSingleton.h>
#include <OBTConverterSingleton.h>
#include <OBTFileParser.h>
#include <USCTimeCorrParams.h>
#include <Filterables.h>
#include <Application.h>
#include <dbConnectionPool.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

namespace {

    inline 
    long double convert_JD50_JD2000(long double inTime)	// PRQA S 2134 
    {
	    return inTime - DateTime::MJD2000::_jd502mjd2000_OFFSET;
    }
    
    inline
    long double convert_JD2000_JD50(long double inTime)	// PRQA S 2134
    {
	    return inTime + DateTime::MJD2000::_jd502mjd2000_OFFSET;
    }
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(ACSTimeConvWrap)

//constructor
ACSTimeConvWrap::ACSTimeConvWrap()  :
    TimeConverter(),
 	_fileName(""),
	_satelliteName(""),
 	_configuredTimeModel(TimeFormat::TC_TIMEMOD_NONE),
	_obtConverter(0),
	_parser(0),
	_token(),
	_acsTimeConv(),
	_downloader(0),
	_onBoardTimeFileType(""),
	_confFileTypes(),
	_satelliteID(TimeFormat::TC_SAT_DEFAULT)
{
	static char Id[] = "ACS: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Id: " << Id  )
}

//destructor
ACSTimeConvWrap::~ACSTimeConvWrap() throw() 
{
	delete _obtConverter;
	delete	_parser;
	delete	_downloader;
}


// Public method NOT IMPLEMENTED!!!
void ACSTimeConvWrap::getValidationInterval(long double& start, long double& stop)
{
	ACS_THROW(exNotImplemented("function not implemented")); // PRQA S 3081
}

void ACSTimeConvWrap::setSatelliteID(TimeFormat::SatelliteID id, int mission) 
{ 	
	acs::ThreadSafe::Lock guard(_token);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Called to set TimeFormat::SatelliteID "  << id << " -> Current is " << _satelliteID )

	_satelliteID = id;
	loadConfiguration(TimeFormat::SatelliteID(_satelliteID),mission); // PRQA S 3013, 3081

	delete _downloader;
	_downloader=0;
	try {
	  _downloader = dynamic_cast<AuxDownloader*>(AuxDownloaderSingleton::instance()->newAuxDownloader(_satelliteName, getPool())); // PRQA S 3081
	  _downloader->setSatelliteName(_satelliteName);
	  _downloader->setSatelliteMission(mission);
	}
	catch(exception& e)
	{
		ACS_LOG_WARNING("Cannot retrieve downloader for " << _satelliteName) ;
		if(_downloader!=0)
		{
			delete _downloader;
			_downloader=0;
		}
	}

}

void ACSTimeConvWrap::setSatelliteID(const string& satName, int mission) // PRQA S 4020
{ 
	acs::ThreadSafe::Lock guard(_token);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Called to set Satellite " << satName << " -> Current is " << _satelliteID )
	
    const rsResourceSet &rsConf = ConfigurationSingleton::instance()->get() ;

	//try load the list of satellite name configurated
	unsigned int n=0;
	try
	{
		rsConf.getValue("ExplorerWrapper.List_of_Satellites.count.value",n);
	}
	catch( exception& e)
	{
		ACS_LOG_WARNING("ExplorerWrapper configuration not found, using default values");
		if(!_satelliteName.empty() && satName!=_satelliteName)
		{
			ACS_THROW(TimeConverterException(e,"Error: satellite name already set!")) ; // PRQA S 3081
		}

		_satelliteName = satName;
		try {
			if(!_downloader) {
				_downloader = dynamic_cast<AuxDownloader*>(AuxDownloaderSingleton::instance()->newAuxDownloader(_satelliteName, getPool())); // PRQA S 3081
			}
			_downloader->setSatelliteName(_satelliteName);
			_downloader->setSatelliteMission(mission);
		}
		catch(exception&)
		{
			ACS_LOG_WARNING("Cannot retrieve downloader for " << _satelliteName) ;
			if(_downloader!=0)
			{
				delete _downloader;
				_downloader=0;
			}
		}
		return ;
	}

	if(n == 0)
	{
		ACS_THROW(TimeConverterException(satName + " no satellites in conf space")) // PRQA S 3081
	}
	int id=-1;
	for(unsigned int i=0; i<n; i++)
	{
		ostringstream num;
		num << i;
		string name="";
		int miss=0;
		rsConf.getValue("ExplorerWrapper.List_of_Satellites.Satellite["+num.str()+"].Name",name);
		rsConf.getValue("ExplorerWrapper.List_of_Satellites.Satellite["+num.str()+"].Mission",miss);
		if((name == satName) && (miss==mission))
		{
			//retrieve the Id associated to the requested satellite name
			rsConf.getValue("ExplorerWrapper.List_of_Satellites.Satellite["+num.str()+"].Id",id);
			break;
		}
	}
	
	if(id==-1)
	{
		ACS_THROW(TimeConverterException("Unknown satellite name: \"" + satName + "\"")) ; // PRQA S 3081
	}
	
	_satelliteID = TimeFormat::SatelliteID(id); // PRQA S 3013, 3081

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"TimeFormat::SatelliteID to set is" << _satelliteID)

	loadConfiguration(TimeFormat::SatelliteID(_satelliteID), mission);// PRQA S 3013, 3081

	if(_downloader!=0)
	{
		delete _downloader;
		_downloader=0;
	}

	try {
		if(!_downloader) {
		  _downloader = dynamic_cast<AuxDownloader*>(AuxDownloaderSingleton::instance()->newAuxDownloader(_satelliteName, getPool())); // PRQA S 3081
		 }
		_downloader->setSatelliteName(_satelliteName);
		_downloader->setSatelliteMission(mission);
	}
	catch(exception&)
	{
		ACS_LOG_WARNING("Cannot retrieve downloader for " << _satelliteName) ;
		if(_downloader!=0)
		{
			delete _downloader;
			_downloader=0;
		}
	}
}


string ACSTimeConvWrap::getFileType(TimeFormat::TimeModel model) const
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for model " << model)

	string fileType;
	map<TimeFormat::TimeModel,string>::const_iterator it = _confFileTypes.find(model);
	if(it != _confFileTypes.end())
	{
		//filetype defined in TimeFormat::TimeModel but does not have a file associated 
		//throw an exception
		if( (it->second) == "")
		{
			ostringstream msg;
			msg << "Error: Time Model " << model << " configured to NULL file type";
			ACS_THROW(TimeConverterException(msg.str())); // PRQA S 3081
		}
		else {
			fileType = it->second;
		}
	}
	else
	{
		ostringstream msg;
		msg << "ACSTimeConvWrap Error: Time Model " << model << " not configured to any file type";
			ACS_THROW(TimeConverterException(msg.str())); // PRQA S 3081
	}
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"returning file type " << fileType)

	return fileType;
}


namespace {

    template <typename T>
    inline void validate_value(const T& t) { // PRQA S 2134
	    ACS_COND_THROW(t.getValue().empty(), // PRQA S 3081
            ACSTimeConvWrap::TimeConverterException("Input time value not set")) ;
    }
    
    template <>
    inline void validate_value<ProcessingTime>(const ProcessingTime& t) { // PRQA S 2134
	    ACS_COND_THROW((t.getValue() >= 0) && (t.getValue() <= 0),  // means its 0 // PRQA S 3081
            ACSTimeConvWrap::TimeConverterException("Input time value not set")) ;
    }
        
    template <typename T>
    inline void validate(const T& t, const string &s)  // PRQA S 2134
    {
	    ACS_COND_THROW(t.getRef() == TimeFormat::TC_TIME_UNDEF,  // PRQA S 3081
            ACSTimeConvWrap::TimeConverterException(s+ "time ref not set")) ;
	    ACS_COND_THROW(t.getFormat() == -1,  // PRQA S 3081
            ACSTimeConvWrap::TimeConverterException(s+ " time format not set")) ;
        
    }

    template <typename T>
    inline void validate_in(const T& t)  // PRQA S 2134
    {
        validate_value(t) ;
        validate(t, "Input") ;
    }
    
    template <typename T>
    inline void validate_out(const T& t)  // PRQA S 2134
    {
        validate(t, "Output") ;
    }
    
    template <typename I, typename O>
    inline void validateIO(const I &i, const O &o)  // PRQA S 2134
    {
        validate_in(i) ;
        validate_out(o) ;
    }
    
    template<>
    inline void validateIO<ProcessingTime, OnBoardTime>(const ProcessingTime& i, const OnBoardTime& o) // PRQA S 2134
    {
        validate_in(i) ;
    }
    
    template<>
    inline void validateIO<OnBoardTime, ProcessingTime>(const OnBoardTime& i, const ProcessingTime& o) // PRQA S 2134
    {
        validate_value(i) ;
        validate_out(o) ;
    }

}




// init method NOT IMPLEMENTED!!!
void ACSTimeConvWrap::initFromFile(const string& fileName, TimeFormat::TimeModel model,long double time0, long double time1)
{	
	ACS_THROW(exNotImplemented("function not implemented")); // PRQA S 3081
}

// init method NOT IMPLEMENTED!!!
void ACSTimeConvWrap::initFromFile(TimeFormat::TimeModel model,long double time0, long double time1)
{	
	ACS_THROW(exNotImplemented("function not implemented")); // PRQA S 3081
}

// init method NOT IMPLEMENTED!!!
void ACSTimeConvWrap::initConditioned(TimeFormat::TimeModel model,long double time0, long double time1)
{	
	ACS_THROW(exNotImplemented("function not implemented")); // PRQA S 3081
}


void ACSTimeConvWrap::initOBCTimeConversion(long double time0, long double time1)
{	
	acs::ThreadSafe::Lock guard(_token);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for interval [" << setprecision(17) << time0 << " , " << time1 << " ]")
	
	if(time0 >= time1)
	{
		ACS_THROW(TimeConverterInitException("Error: Invalid Time Interval for Library initialization. Start time is bigger than Stop time")) ; // PRQA S 3081
	}

	try
	{
		if(!_parser)
		{
			_parser = dynamic_cast<OBTFileParser*>(AuxFileParserSingleton::instance()->newObtFileParser(_satelliteName)); // PRQA S 3081
			if(!_parser)
			{
				ACS_THROW(exIllegalValueException("Not instanced a OBTFileParser obj pointer from factory!")) ; // PRQA S 3081
			}

		}
		
		if(!_downloader) {
			_downloader = dynamic_cast<AuxDownloader*>(AuxDownloaderSingleton::instance()->newAuxDownloader(_satelliteName, getPool())); // PRQA S 3081
		}
		string fileName = _downloader->downloadFromValCover( _onBoardTimeFileType, time0, time1 );
		
		_parser->addFileName(fileName);
		if(!_obtConverter)
		{
			_obtConverter = dynamic_cast<OBTConverter*>(OBTConverterSingleton::instance()->newObtConverter(_satelliteName)); // PRQA S 3081
			if(!_obtConverter)
			{
				ACS_THROW(exIllegalValueException("Not instanced a OBTConverter obj pointer from factory!")) ; // PRQA S 3081
			}
			_obtConverter->setParser(_parser);
		}
	}
	catch( exception& ex)
	{
		ostringstream msg;
		msg << "Error in initOBCTimeConversion " << " [" << time0 << "," << time1 << "]";
		ACS_THROW(TimeConverterInitException(ex, msg.str())) ; // PRQA S 3081
	}
	
}

void ACSTimeConvWrap::initOBCTimeConversion(const string& fileName)
{	
	acs::ThreadSafe::Lock guard(_token);
	if(!_parser)
	{
		_parser = dynamic_cast<OBTFileParser*>(AuxFileParserSingleton::instance()->newObtFileParser(_satelliteName)); // PRQA S 3081
		if(!_parser)
		{
			ACS_THROW(exIllegalValueException("Not instanced a OBTFileParser obj pointer from factory")) ; // PRQA S 3081
		}

	}
	_parser->addFileName(fileName);
	if(!_obtConverter)
	{
		_obtConverter = dynamic_cast<OBTConverter*>(OBTConverterSingleton::instance()->newObtConverter(_satelliteName)); // PRQA S 3081
		if(!_obtConverter)
		{
			ACS_THROW(exIllegalValueException("Not instanced a OBTConverter obj pointer from factory")) ; // PRQA S 3081
		}
	}
	_obtConverter->setParser(_parser);
}


///////////////////////////////////////
//ENVISAT specific implementation!!!!
///////////////////////////////////////
void ACSTimeConvWrap::initOBCTimeConversion(const string& dirPath,long double &refTime,vector<unsigned long int>& refSBT,unsigned long int& cStep)
{	
	acs::ThreadSafe::Lock guard(_token);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called to retrieve Time Conversion File in dir " << dirPath )
	
	try
	{
		if(!_downloader) {
		    _downloader = dynamic_cast<AuxDownloader*>(AuxDownloaderSingleton::instance()->newAuxDownloader(_satelliteName,getPool())); // PRQA S 3081
        }
        
		string fileName = _downloader->checkFileType(_onBoardTimeFileType,dirPath);
		if(fileName.empty())
		{
			ACS_THROW(exIllegalValueException("filename returned is empty")) ; // PRQA S 3081
		
		}	
		if(!_parser)
		{
			_parser = dynamic_cast<OBTFileParser*>(AuxFileParserSingleton::instance()->newObtFileParser(_satelliteName)); // PRQA S 3081
			if(!_parser)
			{
				ACS_THROW(exIllegalValueException("Not instanced a OBTFileParser obj pointer from factory")) ; // PRQA S 3081
			}

		}
		_parser->addFileName(fileName);
		
		if(!_obtConverter)
		{
			_obtConverter = dynamic_cast<OBTConverter*>(OBTConverterSingleton::instance()->newObtConverter(_satelliteName)); // PRQA S 3081
			if(!_obtConverter)
			{
				ACS_THROW(exIllegalValueException("Not instanced a OBTConverter obj pointer from factory")) ; // PRQA S 3081
			}
			_obtConverter->setParser(_parser);
		}
		
		vector<AuxFileParser::Parameters*> validParams;
		_parser->parse(validParams);

		if(validParams.empty())
		{
			ACS_THROW(TimeConverterInitException("NO VALID PARAMETERS in TimeCorrelationFile parsing retrieved")) ; // PRQA S 3081
		}
		USCTimeCorrParams *pP = dynamic_cast<USCTimeCorrParams *>(validParams[0]) ; // PRQA S 3081
		ACS_COND_THROW(!pP,exIllegalValueException("null pointer to USCTimeCorrParams")) ;	// PRQA S 3081


		//set return values
		refTime = pP->refTime;
		refSBT	= pP->refSBT;
		cStep	= pP->clockStep;

	}
	catch( exception& ex)
	{
		ostringstream msg;
		msg << "Error in initOBCTimeConversion : dirPath= " << dirPath << " - refTime=" << refTime 
			<< " - refSBT size =" << refSBT.size()  << " - cStep=" << cStep ;
		ACS_THROW(TimeConverterInitException(ex, msg.str())); // PRQA S 3081
	}
}

// init method NOT IMPLEMENTED!!!
void ACSTimeConvWrap::getLeapSecondInfo(AsciiTime& asciiTimeBefore, AsciiTime& asciiTimeAfter, long int& leapFlag)
{	
	ACS_THROW(exNotImplemented("function not implemented")); // PRQA S 3081
}


ssize_t ACSTimeConvWrap::getNumberOfLeapSecondsAtDate(const AsciiTime& timeIn) 
{	
	ssize_t leapSeconds (0) ; 
	const ssize_t differenceTaiUTC (10) ; // Before the introduction of the leap second
	ProcessingTime pTimeIn(0, TimeFormat::TC_TIME_UTC, TimeFormat::TC_PROC) ; 
	convert(timeIn, pTimeIn) ;

	map <long double, ssize_t> leapSecondTimeDifferencesMap ; 

	excerr << pTimeIn.getValue() << endl ;
	vector < pair< string, ssize_t > > v ( _acsTimeConv.getLeapSecondTimeDifferences() ) ;

	for (size_t i=0; i<v.size(); i++)
	{
		AsciiTime asciiTime( v[i].first, TimeFormat::TC_TIME_UTC, TimeFormat::TC_ASCII_COMPACT ) ;

		ProcessingTime pTimeTmp(0, TimeFormat::TC_TIME_UTC, TimeFormat::TC_PROC) ; 
		convert(asciiTime, pTimeTmp);	// From ASCII_COMPACT/UTC to ProcessingTime/UTC (i.e. a long double)

		leapSecondTimeDifferencesMap[ pTimeTmp.getValue() ] = v[i].second ;
	}

	if ( leapSecondTimeDifferencesMap.begin() != leapSecondTimeDifferencesMap.end() )
	{ 
		// MAP IS FULL:
		// Look for a value ABOVE (not lower, not equal) than the input time.
		// The value of current leap second will be the one BEFORE this one
		map <long double, ssize_t>::const_iterator k ( leapSecondTimeDifferencesMap.upper_bound(pTimeIn.getValue()) ) ; 

		if ( k == leapSecondTimeDifferencesMap.begin() )
		{ 
			// The input time is BEFORE THE INTRODUCTION OF THE LEAP SECOND.
			leapSeconds = differenceTaiUTC ; // Difference TAI-UTC also before the introduction of the leap second
		}
		else if ( k == leapSecondTimeDifferencesMap.end() )
		{ 
			// The input time is AFTER THE LAST LEAP SECOND.
			leapSeconds = (leapSecondTimeDifferencesMap.rbegin())->second ;
		} 
		else 
		{ 
			// The input time is NEITHER BEFORE THE FIRST, NOR AFTER THE LAST
			k-- ; leapSeconds = k->second  ; 
		} 
	} else {
		// MAP IS EMPTY:
		leapSeconds = differenceTaiUTC ; // Difference TAI-UTC also before the introduction of the leap second
	
	}
	
	return leapSeconds ; 
}


////////////////////////////////////
// convert methods 
////////////////////////////////////

TransportTime& ACSTimeConvWrap::convert(const TransportTime& i, TransportTime& o)
{	
	try 
	{
        validateIO(i, o) ;

		ACSTimeConv::TimeConv in;
		ACSTimeConv::TimeConv out;
		in.ref  = convertReferenceEnum(i.getRef());
		out.ref = convertReferenceEnum(o.getRef());
		in.format  = convertTransportEnum(i.getFormat());
		out.format = convertTransportEnum(o.getFormat());
		const vector<long> &values = i.getValue();
		in.timeI[0] = convert_JD2000_JD50(values[0]); // PRQA S 3011
		in.timeI[1] = values[1];
		in.timeI[2] = values[2]; // PRQA S 4400

		_acsTimeConv.convertTime(in,out);

		vector<long> tmp(3);
		tmp[0]=convert_JD50_JD2000(out.timeI[0]); // PRQA S 3011
		tmp[1]= out.timeI[1] ;
		tmp[2]= out.timeI[2] ; // PRQA S 4400
		o.setValue(tmp);

	    return o; // PRQA S 4028
	}
	catch(exception& ex)
	{
		ACS_THROW(TimeConverterException(ex, "Conversion error.")) ; // PRQA S 3081
	}
}

ProcessingTime& ACSTimeConvWrap::convert(const TransportTime& i, ProcessingTime& o)
{
	try 
	{
        validateIO(i, o) ;
		ACSTimeConv::TimeConv in;
		ACSTimeConv::TimeConv out;
		in.ref  = convertReferenceEnum(i.getRef());
		out.ref = convertReferenceEnum(o.getRef());
		in.format  = convertTransportEnum(i.getFormat());
		out.format = convertProcessingEnum(o.getFormat());
		const vector<long> &values = i.getValue();
		in.timeI[0] = convert_JD2000_JD50(values[0]); // PRQA S 3011
		in.timeI[1] = values[1];
		in.timeI[2] = values[2]; // PRQA S 4400

		_acsTimeConv.convertTime(in,out);
		o.setValue(convert_JD50_JD2000(out.timeD));
			
	    return o;		// PRQA S 4028
	}
	catch( exception& ex)
	{
		ACS_THROW(TimeConverterException(ex, "Conversion error.")) ; // PRQA S 3081
	}
}

AsciiTime& ACSTimeConvWrap::convert(const TransportTime& i, AsciiTime& o)
{
	try 
	{
        validateIO(i, o) ;
		ACSTimeConv::TimeConv in;
		ACSTimeConv::TimeConv out;
		in.ref  = convertReferenceEnum(i.getRef());
		out.ref = convertReferenceEnum(o.getRef());
		in.format  = convertTransportEnum(i.getFormat());
		out.format = convertAsciiEnum(o.getFormat());
		const vector<long> &values = i.getValue();
		in.timeI[0] = convert_JD2000_JD50(values[0]); // PRQA S 3011
		in.timeI[1] = values[1];
		in.timeI[2] = values[2]; // PRQA S 4400

		_acsTimeConv.convertTime(in,out);
		o.setValue(out.timeS);
	    return o; // PRQA S 4028
	}
	catch( exception& ex)
	{
		ACS_THROW(TimeConverterException(ex, "Conversion error.")) ; // PRQA S 3081
	}
}


TransportTime& ACSTimeConvWrap::convert(const ProcessingTime& i, TransportTime& o)
{	
	try 
	{
        validateIO(i, o) ;
		ACSTimeConv::TimeConv in;
		ACSTimeConv::TimeConv out;
		in.ref  = convertReferenceEnum(i.getRef());
		out.ref = convertReferenceEnum(o.getRef());
		in.format  = convertProcessingEnum(i.getFormat());
		out.format = convertTransportEnum(o.getFormat());
		in.timeD = convert_JD2000_JD50(i.getValue());

		_acsTimeConv.convertTime(in,out);

		vector<long> tmp(3);
		tmp[0]=(convert_JD50_JD2000(out.timeI[0])); // PRQA S 3011
		tmp[1]=out.timeI[1];
		tmp[2]=out.timeI[2]; // PRQA S 4400
		o.setValue(tmp);
	    return o; // PRQA S 4028
	}
	catch( exception& ex)
	{
		ACS_THROW(TimeConverterException(ex, "Conversion error.")) ; // PRQA S 3081
	}	
}

ProcessingTime& ACSTimeConvWrap::convert(const ProcessingTime& i, ProcessingTime& o)
{	
	try 
	{
        validateIO(i, o) ;
		ACSTimeConv::TimeConv in;
		ACSTimeConv::TimeConv out;
		in.ref  = convertReferenceEnum(i.getRef());
		out.ref = convertReferenceEnum(o.getRef());
		in.format  = convertProcessingEnum(i.getFormat());
		out.format = convertProcessingEnum(o.getFormat());
		in.timeD = convert_JD2000_JD50(i.getValue());

		_acsTimeConv.convertTime(in,out);
		o.setValue(convert_JD50_JD2000(out.timeD));
	    return o; // PRQA S 4028
	}
	catch( exception& ex)
	{
		ACS_THROW(TimeConverterException(ex, "Conversion error.")) ; // PRQA S 3081
	}
}

AsciiTime& ACSTimeConvWrap::convert(const ProcessingTime& i, AsciiTime& o)
{	
	try 
	{
        validateIO(i, o) ;
		ACSTimeConv::TimeConv in;
		ACSTimeConv::TimeConv out;
		in.ref  = convertReferenceEnum(i.getRef());
		out.ref = convertReferenceEnum(o.getRef());
		in.format  = convertProcessingEnum(i.getFormat());
		out.format = convertAsciiEnum(o.getFormat());
		in.timeD = convert_JD2000_JD50(i.getValue());

		_acsTimeConv.convertTime(in,out);
		o.setValue(out.timeS);

	    return o; // PRQA S 4028
	}
	catch( exception& ex)
	{
		ACS_THROW(TimeConverterException(ex, "Conversion error.")) ; // PRQA S 3081
	}
}


TransportTime& ACSTimeConvWrap::convert(const AsciiTime& i, TransportTime& o)
{	
	try 
	{
        validateIO(i, o) ;
		ACSTimeConv::TimeConv in;
		ACSTimeConv::TimeConv out;
		in.ref  = convertReferenceEnum(i.getRef());
		out.ref = convertReferenceEnum(o.getRef());
		in.format  = convertAsciiEnum(i.getFormat());
		out.format = convertTransportEnum(o.getFormat());
		in.timeS = i.getValue();

		_acsTimeConv.convertTime(in,out);

		vector<long> tmp(3);
		tmp[0]=(convert_JD50_JD2000(out.timeI[0])); // PRQA S 3011
		tmp[1]=out.timeI[1];
		tmp[2]=out.timeI[2]; // PRQA S 4400
		o.setValue(tmp);

	    return o; // PRQA S 4028
	}
	catch( exception& ex)
	{
		ACS_THROW(TimeConverterException(ex, "Conversion error.")) ; // PRQA S 3081
	}
}

ProcessingTime& ACSTimeConvWrap::convert(const AsciiTime& i, ProcessingTime& o)
{	
	try 
	{
        validateIO(i, o) ;
		ACSTimeConv::TimeConv in ;
		ACSTimeConv::TimeConv out ;
		in.ref  = convertReferenceEnum(i.getRef());
		out.ref = convertReferenceEnum(o.getRef());
		in.format  = convertAsciiEnum(i.getFormat());
		out.format = convertProcessingEnum(o.getFormat());
		in.timeS = i.getValue();

		_acsTimeConv.convertTime(in,out);
		o.setValue(convert_JD50_JD2000(out.timeD));

	    return o; // PRQA S 4028
	}
	catch( exception& ex)
	{
		ACS_THROW(TimeConverterException(ex, "Conversion error.")) ; // PRQA S 3081
	}
}

AsciiTime& ACSTimeConvWrap::convert(const AsciiTime& i, AsciiTime& o)
{	
	try 
	{
        validateIO(i, o) ;
		ACSTimeConv::TimeConv in;
		ACSTimeConv::TimeConv out;
		in.ref  = convertReferenceEnum(i.getRef());
		out.ref = convertReferenceEnum(o.getRef());
		in.format  = convertAsciiEnum(i.getFormat());
		out.format = convertAsciiEnum(o.getFormat());
		in.timeS = i.getValue();

		_acsTimeConv.convertTime(in,out);
		o.setValue(out.timeS);

	    return o; // PRQA S 4028
	}
	catch( exception& ex)
	{
		ACS_THROW(TimeConverterException(ex, "Conversion error.")) ; // PRQA S 3081
	}
}

ProcessingTime& ACSTimeConvWrap::convert(const OnBoardTime& i, ProcessingTime& o)
{	
	try 
	{
		if(!_parser && !_obtConverter)
		{
			ACS_THROW(TimeConverterException("impossible to perform the OnBoardTime --> ProcessingTime conversion without initOBCTimeConversion")); // PRQA S 3081
		}
		
        validateIO(i, o) ;
		o.setValue(_obtConverter->obtToTime(i));

	    return o; // PRQA S 4028
	}
	catch( exception& ex)
	{
		ACS_THROW(TimeConverterException(ex, "Conversion error.")) ; // PRQA S 3081
	}

}


OnBoardTime& ACSTimeConvWrap::convert(const ProcessingTime& i, OnBoardTime& o)
{	
	acs::ThreadSafe::Lock guard(_token);
	try 
	{
		if(!_parser && !_obtConverter)
		{
			ACS_THROW(TimeConverterException("cannot perform the ProcessingTime --> OnBoardTime conversion without initOBCTimeConversion")) ; // PRQA S 3081
		}

        validateIO(i, o) ;
		OnBoardTime obt;
		obt = _obtConverter->timeToObt(i.getValue(), obt);
		o.setValue(obt.getValue());
			
	    return o; // PRQA S 4028
	}
	catch( exception& ex)
	{
		ACS_THROW(TimeConverterException(ex, "Conversion error.")) ; // PRQA S 3081
	}
}


///////////////////////////////////////////
// Private methods
///////////////////////////////////////////




ACSTimeConv::TimeFormatDefines ACSTimeConvWrap::convertTransportEnum(TimeFormat::TransportTimeFormat enumIn) // PRQA S 4020
{
    switch(enumIn) {
	case TimeFormat::TC_TRANS_STD:
    case TimeFormat::TC_TRANS_ENVI_GS:
    case TimeFormat::TC_TRANS_CRYO_GS:
        {
		    return ACSTimeConv::TRANSPORT_DAY_SEC_USEC;
	    }
	case TimeFormat::TC_TRANS_CRYO_TM:
    case TimeFormat::TC_TRANS_CRYO_TM_SIRAL:
        {
		    return ACSTimeConv::TRANSPORT_DAY_MSEC_USEC;
	    }
	default:
	    {
		    ostringstream msg;
		    msg << "unknown TimeFormat::TransportTimeFormat(" << int(enumIn) << ")" ; // PRQA S 3081
		    ACS_THROW(TimeConverterException(msg.str())); // PRQA S 3081
	    }
    }
}

ACSTimeConv::TimeFormatDefines ACSTimeConvWrap::convertProcessingEnum(TimeFormat::ProcessingTimeFormat enumIn) // PRQA S 4020
{
	if(enumIn==TimeFormat::TC_PROC) {
		return ACSTimeConv::PROCESSING ;
	}
	else
	{
		ostringstream msg;
		msg << "unsupported TimeFormat::ProcessingTimeFormat(" << int(enumIn) << ")" ; // PRQA S 3081
        ACS_THROW(TimeConverterException(msg.str())); // PRQA S 3081
	}
}

ACSTimeConv::TimeFormatDefines ACSTimeConvWrap::convertAsciiEnum(TimeFormat::AsciiTimeFormat enumIn) // PRQA S 4020
{	 
    switch(enumIn) {
    case TimeFormat::TC_ASCII_STD: { return ACSTimeConv::ASCII_STD; }
    case TimeFormat::TC_ASCII_STD_REF: { return ACSTimeConv::ASCII_STD_REF; }
    case TimeFormat::TC_ASCII_STD_MICROSEC: { return ACSTimeConv::ASCII_STD_MICROSEC; }
    case TimeFormat::TC_ASCII_STD_REF_MICROSEC: { return ACSTimeConv::ASCII_STD_REF_MICROSEC; }
    case TimeFormat::TC_ASCII_COMPACT: { return ACSTimeConv::ASCII_COMPACT; }
    case TimeFormat::TC_ASCII_COMPACT_REF: { return ACSTimeConv::ASCII_COMPACT_REF; }
    case TimeFormat::TC_ASCII_COMPACT_MICROSEC: { return ACSTimeConv::ASCII_COMPACT_MICROSEC; }
    case TimeFormat::TC_ASCII_COMPACT_REF_MICROSEC: { return ACSTimeConv::ASCII_COMPACT_REF_MICROSEC; }
    case TimeFormat::TC_ASCII_ENVI: { return ACSTimeConv::ASCII_ENVI; }
    case TimeFormat::TC_ASCII_ENVI_REF: { return ACSTimeConv::ASCII_ENVI_REF; }
    case TimeFormat::TC_ASCII_ENVI_MICROSEC: { return ACSTimeConv::ASCII_ENVI_MICROSEC; }
    case TimeFormat::TC_ASCII_ENVI_REF_MICROSEC: { return ACSTimeConv::ASCII_ENVI_REF_MICROSEC; }
    case TimeFormat::TC_ASCII_CCSDSA: { return ACSTimeConv::ASCII_CCSDSA; }
    case TimeFormat::TC_ASCII_CCSDSA_REF: { return ACSTimeConv::ASCII_CCSDSA_REF; }
    case TimeFormat::TC_ASCII_CCSDSA_MICROSEC: { return ACSTimeConv::ASCII_CCSDSA_MICROSEC; }
    case TimeFormat::TC_ASCII_CCSDSA_REF_MICROSEC: { return ACSTimeConv::ASCII_CCSDSA_REF_MICROSEC; }
    case TimeFormat::TC_ASCII_CCSDSA_COMPACT: { return ACSTimeConv::ASCII_CCSDSA_COMPACT; }
    case TimeFormat::TC_ASCII_CCSDSA_COMPACT_REF: { return ACSTimeConv::ASCII_CCSDSA_COMPACT_REF; }
    case TimeFormat::TC_ASCII_CCSDSA_COMPACT_MICROSEC: { return ACSTimeConv::ASCII_CCSDSA_COMPACT_MICROSEC; }
    case TimeFormat::TC_ASCII_CCSDSA_COMPACT_REF_MICROSEC: { return ACSTimeConv::ASCII_CCSDSA_COMPACT_REF_MICROSEC; }
    case TimeFormat::TC_ASCII_COSMO_NANOSEC: { return ACSTimeConv::ASCII_COSMO_NANOSEC; }
    case TimeFormat::TC_ASCII_COSMO_MICROSEC: { return ACSTimeConv::ASCII_COSMO_MICROSEC; }
    case TimeFormat::TC_ASCII_COSMO_MILLISEC: { return ACSTimeConv::ASCII_COSMO_MILLISEC; }
    case TimeFormat::TC_ASCII_COSMO_SEC: { return ACSTimeConv::ASCII_COSMO_SEC; }
    case TimeFormat::TC_ASCII_COSMO_COMPACT: { return ACSTimeConv::ASCII_COSMO_COMPACT; }
    case TimeFormat::TC_ASCII_ERS_MILLISEC: { return ACSTimeConv::ASCII_ERS_MILLISEC; }
    default:
	    {
		    ostringstream msg;
		    msg << "unknown TimeFormat::AsciiTimeFormat(" << int(enumIn) << ")" ; // PRQA S 3081
		    ACS_THROW(TimeConverterException(msg.str())); // PRQA S 3081
	    }
    }
}

ACSTimeConv::TimeReferenceDefines ACSTimeConvWrap::convertReferenceEnum(TimeFormat::TimeReference enumIn) // PRQA S 4020
{	
    switch(enumIn) {
	case TimeFormat::TC_TIME_TAI: { return ACSTimeConv::TAI; }
	case TimeFormat::TC_TIME_UTC: { return ACSTimeConv::UTC; }
	case TimeFormat::TC_TIME_UT1: { return ACSTimeConv::UT1; }
	case TimeFormat::TC_TIME_GPS: { return ACSTimeConv::GPS; }
	default:
	    {
		    ostringstream msg;
		    msg << "unknown TimeFormat::TimeReference(" << int(enumIn) << ")" ; // PRQA S 3081
		    ACS_THROW(TimeConverterException(msg.str())); // PRQA S 3081
	    }
    }
}


void ACSTimeConvWrap::loadConfiguration(TimeFormat::SatelliteID id, int mission)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"called for satelliteID " << id << " and mission " << mission )
	unsigned int n=0;
    const rsResourceSet &rsConf = ConfigurationSingleton::instance()->get() ;
	rsConf.getValue("ExplorerWrapper.List_of_Satellites.count.value",n);
	if(n == 0)
	{
		ostringstream msg;
		msg << "Error : Cannot configure SatelliteID from input value " 
			<< id << " : No satellites configured in Configuration Space";
		TimeConverterException e(msg.str());
		ACS_THROW(e);	
	}
	for(unsigned int i=0; i<n; i++)
	{
		ostringstream num;
		num << i;
		int rsId=0 ;
        int miss=1;;
		rsConf.getValue("ExplorerWrapper.List_of_Satellites.Satellite["+num.str()+"].Id",rsId);
		rsConf.getValue("ExplorerWrapper.List_of_Satellites.Satellite["+num.str()+"].Mission",miss);

    	if( (rsId == id) && (miss == mission) )//got the rigth satellite : load data
		{
			//load the satellite name
			rsConf.getValue("ExplorerWrapper.List_of_Satellites.Satellite["+num.str()+"].Name",_satelliteName);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Got Satellite Name " << _satelliteName )
			//load the TimeModel<->FileType associations
			unsigned int modNum=0;
			rsConf.getValue("ExplorerWrapper.List_of_Satellites.Satellite["+num.str()+"].List_of_Models.count.value",modNum);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Number of TimeModel<->FileType associations configurated is " << modNum )
			for(unsigned int j=0; j < modNum; j++)
			{
				int model=0;
				string fileType="";
				ostringstream k;
				k << j;
				rsConf.getValue("ExplorerWrapper.List_of_Satellites.Satellite["+num.str()+"].List_of_Models.TimeModel["+k.str()+"].Model",model);
				rsConf.getValue("ExplorerWrapper.List_of_Satellites.Satellite["+num.str()+"].List_of_Models.TimeModel["+k.str()+"].FileType",fileType);
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Found association TimeModel [" << model << "] with FileType[" << fileType << "]")
				_confFileTypes[TimeFormat::TimeModel(model)] = fileType; // PRQA S 3013, 3081
			}
			//load the UTC/SBT TimeConversion File data
			rsConf.getValue("ExplorerWrapper.List_of_Satellites.Satellite["+num.str()+"].TimeConversionFile.FileType",
							 _onBoardTimeFileType);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Found Time Conversion File Type [" << _onBoardTimeFileType << "]")
			break;
		}
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"configuration loaded for satellite " << _satelliteName )
}

dbConnectionPool &ACSTimeConvWrap::getPool() // PRQA S 4020
{
    try {
        dbConnectionPool *p = Application::instance()->getDefaultConnectionPool() ;
        ACS_COND_THROW(not p, exIllegalValueException("No default pool got from the application")) ; // PRQA S 3081
        return *p ;
    }
    catch(exception &x) {
        ACS_LOG_NOTIFY_EX(x) ;
        ACS_LOG_WARNING("Using default connection pool - dangerous for multi-DB apps") ;
        return db::ConnPools::instance()->getPool("") ;
    } 
}


_ACS_END_NAMESPACE

