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

	$Prod: A.C.S. libAuxFileParsing$

	$Id$

	$Author$

	$Log$
	Revision 2.6  2014/03/19 19:14:55  marpas
	coding best practices application in progress (not ended)
	warning fixing (qa & g++)
	
	Revision 2.5  2012/04/18 08:15:40  giucas
	compilation warnings fixed
	
	Revision 2.4  2009/07/23 13:40:41  marant
	sscanf replaced with DateTime ctor(string)
	
	Revision 2.3  2009/07/13 15:49:04  marant
	Root added in xml aux_tc file; tag l0 utc time corrected
	
	Revision 2.2  2009/07/13 13:33:54  marant
	Class MosTCFileParser re-implemented: an xml file is used to store the MOS
	time correlation info plus some other time info coming from the L0 processing
	
	Revision 2.1  2007/07/25 17:46:42  nicvac
	stable and tested.
	
        
*/ 

#include <MosTCFileParser.h>
#include <XMLIstream.h>
#include <XMLOstream.h>
#include <Filterables.h>
#include <DateTime.h>
#include <rsResourceSet.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

namespace {

    const string TimeCorrelation_Key("TimeCorrelation");
    const string TC_utcTime_Key("UtcTime");
    const string TC_clockStep_Key("ClockStep");
    const string TC_AcrossTrackBias_Key("AcrossTrackBias");
    const string TC_timeCode_Key("TimeCode");

    const string L0TimeInfo_Key("L0TimeInfo");
    const string L0_lineNumber_Key("LineNumber");
    const string L0_utcTime_Key("UtcTime");
    const string L0_timeCode_Key("TimeCode");

    const string Root_Key("MosAuxTimeCorrelation");
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(MosTCFileParser)



//Class constructor
MosTCFileParser::MosTCFileParser() :
    OBTFileParser(),
	_fileNames(),
	_fileType(),
	_fileNamesVec(),
	_validParams()
{
	static char Id[] = "@(#) A.C.S.: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Id: " << Id) ;
}

//Class constructor
MosTCFileParser::MosTCFileParser(const string& fileName) :
    OBTFileParser(),
	_fileNames(),
	_fileType(),
	_fileNamesVec(),
	_validParams()
{
	static char Id[] = "@(#) A.C.S.: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Id: " << Id) ;
	_fileNames.insert(fileName);
	_fileNamesVec.push_back(fileName);
	buildParams();
} 

//Class destructor
MosTCFileParser::~MosTCFileParser() throw()
{
	cleanupParams(); // PRQA S 4631
}

//copy constructor
MosTCFileParser::MosTCFileParser(const MosTCFileParser & e):
    OBTFileParser(e),
	_fileNames(e._fileNames),
	_fileType(e._fileType),
	_fileNamesVec(e._fileNamesVec),
	_validParams(e._validParams)
{
}

//operator = 
MosTCFileParser&  MosTCFileParser::operator=(const MosTCFileParser & e)
{
	if (this != &e) 
	{
        OBTFileParser::operator=(e) ;
		_fileNames						= e._fileNames;
		_fileType						= e._fileType;
		_fileNamesVec					= e._fileNamesVec;
		_validParams					= e._validParams;
	}
	return *this ;
}


void MosTCFileParser::cleanupParams()
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"method called.") ;

	vector<MosTimeCorrParams*>::iterator parIt = _validParams.begin();
	while( parIt != _validParams.end() )
	{
		delete *parIt;
		*parIt=0;
		parIt = _validParams.erase(parIt);
	}

    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"method executed.") ;
}

//parse each file in _fileNames set and add the valid parameters to _validParams vector.
//Since _fileNames is ordered by validity start/stop times, the _validParams are themselves ordered by time
void MosTCFileParser::buildParams()
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"method called.") ;

	//clean current Valid Params
	cleanupParams();
	
	//iterate through the set and build the valid parameter sequence
	set<string>::iterator it = _fileNames.begin();
	while ( it != _fileNames.end() )
	{
		parseFile(*it);
		it++;
	}

    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"method executed.") ;
}


void MosTCFileParser::parseFile(const string& fileName)
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"method called.") ;

    rsResourceSet rs;
    XMLIstream xs(fileName);
    xs >> rs ;

    if (!_validParams.empty()){
        rs.getValue(L0TimeInfo_Key+"."+L0_lineNumber_Key, _validParams[0]->l0_line_number);
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"L0 line number		= '" << _validParams[0]->l0_line_number	<< "'") ; 
        return;
    }

    MosTimeCorrParams* validParams = new MosTimeCorrParams;

    rs.getValue(TimeCorrelation_Key+"."+TC_timeCode_Key, validParams->time_code);
    rs.getValue(TimeCorrelation_Key+"."+TC_clockStep_Key, validParams->clock_step_length);
    try {
        rs.getValue(TimeCorrelation_Key+"."+TC_AcrossTrackBias_Key, validParams->AcrossTrackBias);
    }catch(exception &){
        validParams->AcrossTrackBias = 0;
    }

    string utcString("");
    rs.getValue(TimeCorrelation_Key+"."+TC_utcTime_Key, utcString);

    if (utcString.substr(0,3) != "UTC") { // PRQA S 4400
    	ACS_THROW(exIOException("Error reading file " + fileName + " - Bad format for UTC Reference time")); // PRQA S 3081
    }


    DateTime dt(utcString);
    validParams->reference_j50 = dt.jd50();

    //L0 info 
    rs.getValue(L0TimeInfo_Key+"."+L0_lineNumber_Key, validParams->l0_line_number);
    rs.getValue(L0TimeInfo_Key+"."+L0_timeCode_Key, validParams->l0_time_code);

    string l0UtcString("");
    rs.getValue(L0TimeInfo_Key+"."+L0_utcTime_Key, l0UtcString);

    if (l0UtcString.substr(0,3) != "UTC") { // PRQA S 4400
    	ACS_THROW(exIOException("Error reading file " + fileName + " - Bad format for UTC Reference time")) ; // PRQA S 3081
    }


    DateTime dt2(l0UtcString);
    validParams->l0_reference_j50 = dt2.jd50();


    ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Mos PARSED VALUES ARE :") ;
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"referenceUtc	= '" << utcString << "'") ;
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"referenceJ50	= '" << setprecision(30) << validParams->reference_j50 << "'") ;
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"satBinTime		= '" << validParams->time_code	<< "'") ;
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"clockStepLength	= '" << validParams->clock_step_length << "'") ;
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"AcrossTrackBias	= '" << validParams->AcrossTrackBias << "'") ;
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"L0 referenceUtc		= '" << l0UtcString	<< "'") ;
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"L0 line number		= '" << validParams->l0_line_number	<< "'") ; 
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"L0 referenceJ50		= '" << validParams->l0_reference_j50 << "'") ;
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"L0 time code		= '" << validParams->l0_time_code	<< "'") ;
    ACS_CLASS_END_DEBUG

    // set parameters
    _validParams.push_back(validParams);

    
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"method executed.") ;
}


void MosTCFileParser::print(ostream* os, MosTimeCorrParams* params)
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"method called.") ;
 
    string utcTime;
    {
        DateTime::UTCD utcd;
        DateTime::JD50 jd50(params->reference_j50); 
    	DateTime dt(jd50);
    	dt.toUTCD(utcd); // convert in UTC
        utcTime = ("UTC=" + dt.asISO8601(6));
        //sprintf(buffer,"UTC=%04d-%02d-%02dT%02d:%02d:%02d.%06ld", utcd.year, utcd.month, utcd.day, utcd.hour, utcd.min, utcd.sec, utcd.microsec());
    }

    XMLOstream xst(*os, Root_Key);

    //Time Correlation Info
    {
        XMLOstream::Tag TAG(xst , TimeCorrelation_Key);
    
        XMLOstream::Tag tag1(xst , TC_utcTime_Key, utcTime, "", true);
        XMLOstream::Tag tag2(xst , TC_timeCode_Key, params->time_code, "", true);
        XMLOstream::Tag tag3(xst , TC_clockStep_Key, params->clock_step_length, "", true);
        XMLOstream::Tag tag4(xst , TC_AcrossTrackBias_Key, params->AcrossTrackBias, "", true);
        
    }

    //L0 Time Info
    {
        DateTime::UTCD utcd;
        DateTime::JD50 jd50(params->l0_reference_j50); 
        DateTime dt(jd50);
        dt.toUTCD(utcd); // convert in UTC
        utcTime = ("UTC=" + dt.asISO8601(6));
        //sprintf(buffer,"UTC=%04d-%02d-%02dT%02d:%02d:%02d.%06ld", utcd.year, utcd.month, utcd.day, utcd.hour, utcd.min, utcd.sec, utcd.microsec());
    }
   
    {
        XMLOstream::Tag TAG(xst , L0TimeInfo_Key);

        XMLOstream::Tag tag1(xst , L0_lineNumber_Key, params->l0_line_number, "", true);
        XMLOstream::Tag tag2(xst , L0_timeCode_Key, params->l0_time_code, "", true);
        XMLOstream::Tag tag3(xst , L0_utcTime_Key, utcTime, "", true);
    }

    
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"method executed.") ;
}

	
void MosTCFileParser::addFileName(const string& name)
{
	set<string>::iterator it = _fileNames.find(name);
	if( it == _fileNames.end() )//add filename only if NOT PRESENT yet
	{
		_fileNames.insert(name);
		_fileNamesVec.push_back(name);
		buildParams();
	}
}
	
void MosTCFileParser::removeFileName(const string& name)
{
	set<string>::iterator it = _fileNames.find(name);
	if( it != _fileNames.end() )//remove filename only if previously added
	{
		_fileNames.erase(name);
		vector<string>::iterator itV = find(_fileNamesVec.begin(),_fileNamesVec.end(),name);
		_fileNamesVec.erase(itV);
		buildParams();
	}
}

void MosTCFileParser::parse(vector<Parameters*>& params)
{
	params.clear() ;
	size_t vp_size = _validParams.size() ;
    for (size_t i=0; i < vp_size; ++i) {
		params.push_back(_validParams[i]) ;
    }
}




_ACS_END_NAMESPACE
