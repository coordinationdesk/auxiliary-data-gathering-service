// PRQA S 1050 EOF
/*

	Copyright 2011-2014, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. S3 Product File name helper class $
	
	$Id$

	$Author$
	
	$Log$
	Revision 2.8  2013/12/19 09:49:52  giucas
	S3PDGS-1721 : Naming convention aligned to [CFi-32.1] ls1r3C
	
	Revision 2.7  2013/12/09 14:01:12  giucas
	Library completely revisited.
	
	Revision 2.6  2012/08/01 10:41:40  giucas
	Modified to use TimeConverter for time operations
	
	Revision 2.5  2012/01/23 13:24:50  giucas
	setInstanceId fixed: added missing _ after rel orbit.
	
	Revision 2.4  2011/12/07 08:05:03  giucas
	setInstanceId method modify to manage zero values for input parameters. In this case underscores are set into the file name
	
	Revision 2.3  2011/11/17 08:08:27  giucas
	_instanceId default is 17 chars underscores
	
	Revision 2.2  2011/11/16 17:44:12  giucas
	Fixed wrong check on setValidity method.
	
	Revision 2.1  2011/11/16 14:27:45  giucas
	Added S3L0ProductFileName
	

*/

#include <S3L0ProductFileName.h>
#include <TimeConverterSingleton.h>
#include <DateTime.h>
#include <ProcessingTime.h>
#include <AsciiTime.h>
#include <rsResourceSet.h>
#include <ConfigurationSingleton.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(S3L0ProductFileName) ;

//Class constructors
S3L0ProductFileName::S3L0ProductFileName() :
	_mission(),
	_fileType(),
	_startTime(),
	_stopTime(),
	_currentTime(),
	_instanceId("_________________"),
	_ext(),
	_siteCentre(),
	_platform(),
	_timeliness(),
	_usage()
{
	rsResourceSet rsConf = ConfigurationSingleton::instance()->get() ;
	rsConf.getValue("ProductFileName.SiteCentre",_siteCentre) ;	
	rsConf.getValue("ProductFileName.Platform",_platform) ;	
	rsConf.getValue("ProductFileName.Timeliness",_timeliness) ;	
	rsConf.getValue("ProductFileName.ClassIdUsage",_usage) ;	
	rsConf.getValue("ProductFileName.Extension",_ext) ;	
}


//Class destructor
S3L0ProductFileName::~S3L0ProductFileName() throw()
{
}

void S3L0ProductFileName::setMission(const string& mission)
{
	ACS_COND_THROW( mission.size() != 3 , S3L0ProductFileNameException("Wrong mission "+mission+". Expected size is 3") ) ; // PRQA S 3081
	_mission = mission ;
}

void S3L0ProductFileName::setFileType(const string& fileType)
{
	ACS_COND_THROW( fileType.size() != 11 , S3L0ProductFileNameException("Wrong fileType "+fileType+". Expected size is 11") ) ; // PRQA S 3081
	_fileType = fileType ;
}

void S3L0ProductFileName::setValidity(double startTime, double stopTime)
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "setValidity input parameters are : startTime=" << setprecision(15) << startTime
			   << " - stopTime=" << setprecision(15) << stopTime );
	ACS_COND_THROW( stopTime < startTime , S3L0ProductFileNameException("Wrong validity: start time bigger than stop time") ) ; // PRQA S 3081

	{
		ProcessingTime inTime;
		inTime.setFormat(TimeFormat::TC_PROC);
		inTime.setRef(TimeFormat::TC_TIME_GPS);
		inTime.setValue(startTime);
		AsciiTime outTime ;
		outTime.setFormat(TimeFormat::TC_ASCII_CCSDSA_COMPACT) ;//yyyymmddThhnnss
		outTime.setRef(TimeFormat::TC_TIME_GPS);
		TimeConverterSingleton::Instance()->getConverter()->convert(inTime, outTime);
		_startTime = (outTime.getValue()) ;
	}
	{
		ProcessingTime inTime;
		inTime.setFormat(TimeFormat::TC_PROC);
		inTime.setRef(TimeFormat::TC_TIME_GPS);
		inTime.setValue(stopTime);
		AsciiTime outTime ;
		outTime.setFormat(TimeFormat::TC_ASCII_CCSDSA_COMPACT) ;//yyyymmddThhnnss
		outTime.setRef(TimeFormat::TC_TIME_GPS);
		TimeConverterSingleton::Instance()->getConverter()->convert(inTime, outTime);
		_stopTime = (outTime.getValue()) ;
	}
	{
		ProcessingTime inTime;
		inTime.setFormat(TimeFormat::TC_PROC);
		inTime.setRef(TimeFormat::TC_TIME_UTC);
		inTime.setValue(DateTime::MJD2000(DateTime()).jd()); // PRQA S 3050, 3081
		AsciiTime outTime ;
		outTime.setFormat(TimeFormat::TC_ASCII_CCSDSA_COMPACT) ;//yyyymmddThhnnss
		outTime.setRef(TimeFormat::TC_TIME_UTC);
		TimeConverterSingleton::Instance()->getConverter()->convert(inTime, outTime);
		_currentTime = (outTime.getValue()) ;
	}


	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "setValidity input results are : startTime=" << _startTime
			   				<< " - stopTime=" << _stopTime ) ;
}

void S3L0ProductFileName::setInstanceId(int duration, int cycle, int relOrbit, int frameAlongTrack)
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "setInstanceId input parameters are : duration=" << duration
			   				<< " - cycle=" << cycle << " - relOrbit=" << relOrbit << " - frameAlongTrack=" << frameAlongTrack ) ;	

	ostringstream ostr ;
	
	if( duration != 0 ) {
		ostr << setw(4) << setfill('0') << duration  ; // PRQA S 4400 L1
	}
	else {
		ostr << "____" ;
	}
	
	ostr << "_" ;
	
	if( cycle != 0 ) {
		ostr << setw(3) << setfill('0') << cycle ;
	}
	else {
		ostr << "___" ;
	}
		
	ostr << "_" ;
	
	if( relOrbit != 0 )	 {
		ostr  << setw(3) << setfill('0') << relOrbit << "_";
	}
	else {
		ostr << "___" ;
	}
		
	ostr << "_" ;

	if( frameAlongTrack != 0 ) {
		ostr << setw(4) << setfill('0') << frameAlongTrack ; // PRQA L:L1
	}
	else {
		ostr << "____" ;
	}
		
	_instanceId = ostr.str() ;
}

void S3L0ProductFileName::setExtension(const string& ext) // PRQA S 4121
{
	_ext = ext ;
}

string S3L0ProductFileName::getName() const
{
	ostringstream res ;
	res << _mission 	<< "_"
		<< _fileType	<< "_"
		<< _startTime	<< "_"
		<< _stopTime	<< "_"
		<< _currentTime	<< "_"
		<< _instanceId	<< "_"
		<< _siteCentre	<< "_"
		<< _platform << "_" << _timeliness << "_" << _usage ;
	if( ! _ext.empty() ) {
		res << "." << _ext ;
	}
	return res.str() ;
}


_ACS_END_NAMESPACE

