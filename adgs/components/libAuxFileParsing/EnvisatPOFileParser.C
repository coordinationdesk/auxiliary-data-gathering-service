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

	$Author$

	@(#) A.C.S.: $Id$
	
	$Log$
	Revision 2.2  2014/03/19 19:14:55  marpas
	coding best practices application in progress (not ended)
	warning fixing (qa & g++)
	
	Revision 2.1  2012/04/18 08:15:40  giucas
	compilation warnings fixed
	
	Revision 2.0  2006/02/28 09:32:51  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.3  2005/12/02 11:44:54  giucas
	Fixed bug: the time in PredictedOrbitData was calculated as MJD2000 and not as JD
	
	Revision 1.2  2005/11/16 13:20:06  giucas
	Finalized parseFile method implementation
	
	Revision 1.1  2005/11/16 10:32:35  giucas
	First issue
	

*/

#include <EnvisatPOFileParser.h>
#include <File.h>
#include <DateTime.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(EnvisatPOFileParser)
namespace {
	struct ltTimeParams
	{
		bool operator()(const PredictedOrbitData &x, const PredictedOrbitData &y) const
		{
			return ( x < y );
		}
	};

}

//Class constructor
EnvisatPOFileParser::EnvisatPOFileParser() :
    PredictedOrbitFileParser(), 
	_fileNames(),
	_fileNamesVec(),
	_fileType(),
	_validParams()
{
	static char Id[] = "@(#) A.C.S.: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Id: " << Id) ;
}

//Class constructor
EnvisatPOFileParser::EnvisatPOFileParser(const string& fileName):
    PredictedOrbitFileParser(), 
	_fileNames(),
	_fileNamesVec(),
	_fileType(),
	_validParams()
{
	static char Id[] = "@(#) A.C.S.: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Id: " << Id) ;
	
	_fileNames.insert(fileName);
	_fileNamesVec.push_back(fileName);
	buildParams();

} 

//Class destructor
EnvisatPOFileParser::~EnvisatPOFileParser() throw()
{
}

//copy constructor
EnvisatPOFileParser::EnvisatPOFileParser(const EnvisatPOFileParser & e):
    PredictedOrbitFileParser(e), 
	_fileNames(e._fileNames),
	_fileNamesVec(e._fileNamesVec),
	_fileType(e._fileType),
	_validParams(e._validParams)
{
}

//operator = 
EnvisatPOFileParser&  EnvisatPOFileParser::operator=(const EnvisatPOFileParser & e)
{
	if (this != &e) 
	{
        PredictedOrbitFileParser::operator=(e) ;
		_fileNames				= e._fileNames;
		_fileNamesVec			= e._fileNamesVec;
		_fileType				= e._fileType;
		_validParams			= e._validParams;
	}
	return *this ;
}

void EnvisatPOFileParser::buildParams()
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"method called.") ;

	//clean current Valid Params
	_validParams.clear();
	
	//iterate through the set and build the valid parameter sequence
	set<string>::iterator it = _fileNames.begin();
	while ( it != _fileNames.end() )
	{
		parseFile(*it);
		it++;
	}
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"method executed.") ;
}

void EnvisatPOFileParser::parseFile(const string& fileName)
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"method called.") ;
	
	if(!File::exists(fileName))
	{
		ACS_THROW(exFileOpenException("The input Auxiliary File [" + fileName + "] does not exist")) ; // PRQA S 3081
	}
	
	ifstream is(fileName.c_str());
	string tmpLine="";
	unsigned int lineIndex=0;
	vector<string> tmpValidLines;
	bool saveLine=false;
	while( getline(is,tmpLine) ) // PRQA S 3050
	{
		lineIndex++;
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"got line number " << lineIndex << " : [" << tmpLine << "]") ;
		if(lineIndex == _firstValidLineIndex)
		{
			saveLine = true;
		}
		
		if( saveLine && (tmpLine != "") ) {
			tmpValidLines.push_back(tmpLine);
        }
		
		//reset tmpLine
		tmpLine="";
	}

	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Dump tmpValidLines [" << tmpValidLines.size() << " elements] : ") ;
        size_t vl_size = tmpValidLines.size() ;
		for(unsigned int i=0; i<vl_size; i++)
		{
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Dump tmpValidLines line n." << i+1 << " : '" << tmpValidLines[i] << "'") ; // PRQA S 3084
		}
	ACS_CLASS_END_DEBUG

	set<PredictedOrbitData,ltTimeParams> validSetParams;
	
    size_t vl_size = tmpValidLines.size() ;
	for(unsigned int i=0; i<vl_size; i++)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"Parsing tmpValidLines line n." << i+1 << " : '" // PRQA S 3084
				<< tmpValidLines[i] << "' of size " << tmpValidLines[i].size()) ;
		
		if( tmpValidLines[i].size() != _validLineSize )
		{
			ACS_LOG_WARNING("Wrong line size " << tmpValidLines[i].size() << " for valid line '"
				<< tmpValidLines[i] << "' -> expected " << _validLineSize << " : Discarding it...") ;
			continue;
		}
		
		PredictedOrbitData poData;
		
		string utcTime = tmpValidLines[i].substr(_utcTimeOffset,_utcTimeLength);
		poData.utcTime = getJDFromAsciiEnviMicrosec(utcTime);
		
		string orbit   = tmpValidLines[i].substr(_absOrbitOffset,_absOrbitLength);
		poData.absOrbit = atoi(orbit.c_str());
		
		string xpos	= tmpValidLines[i].substr(_xPosOffset,_xPosLength);
		poData.xPosition = atof(xpos.c_str());
		string ypos	= tmpValidLines[i].substr(_yPosOffset,_yPosLength);
		poData.yPosition = atof(ypos.c_str());
		string zpos	= tmpValidLines[i].substr(_zPosOffset,_zPosLength);
		poData.zPosition = atof(zpos.c_str());

		string xvel	= tmpValidLines[i].substr(_xVelOffset,_xVelLength);
		poData.xVelocity = atof(xvel.c_str());
		string yvel	= tmpValidLines[i].substr(_yVelOffset,_yVelLength);
		poData.yVelocity = atof(yvel.c_str());
		string zvel	= tmpValidLines[i].substr(_zVelOffset,_zVelLength);
		poData.zVelocity = atof(zvel.c_str());
		
		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Parsing tmpValidLines line n." << i+1 ) ; // PRQA S 3084
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,setprecision(17) <<">> UTC Time  is : '" << utcTime 	<< "' -> "	<< poData.utcTime) ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,setprecision(17) <<">> ABS Orbit is : '" << orbit   	<< "' -> "	<< poData.absOrbit) ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,setprecision(17) <<">> X_POS is : '" 	<< xpos		<< "' -> "	<< poData.xPosition) ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,setprecision(17) <<">> Y_POS is : '" 	<< ypos		<< "' -> "	<< poData.yPosition) ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,setprecision(17) <<">> Z_POS is : '" 	<< zpos		<< "' -> "	<< poData.zPosition) ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,setprecision(17) <<">> X_VEL is : '" 	<< xvel		<< "' -> "	<< poData.xVelocity) ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,setprecision(17) <<">> Y_VEL is : '" 	<< yvel		<< "' -> "	<< poData.yVelocity) ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,setprecision(17) <<">> Z_VEL is : '" 	<< zvel		<< "' -> "	<< poData.zVelocity) ;
		ACS_CLASS_END_DEBUG
		
		validSetParams.insert(poData);
		
	}
	
	//fill the Valid parameters vector in UTC-TIME ORDERED way
	set<PredictedOrbitData,ltTimeParams>::iterator it = validSetParams.begin();
	while( it != validSetParams.end() )
	{
		_validParams.push_back(*it);
		it++;
	}

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"method executed.") ;
}


long double EnvisatPOFileParser::getJDFromAsciiEnviMicrosec(const string& refTime) const
{
	//refTime is of the form : DD-MMM-YYYY HH:NN:SS.UUUUUU (TimeFormat::TC_ASCII_ENVI_MICROSEC)
	unsigned short month = parseStringMonth(refTime.substr(3,3)); // PRQA S 3000, 3010, 4400 7
	unsigned short year  = atoi((refTime.substr(7,4)).c_str()); // PRQA S 3000, 3010
	unsigned short day = atoi((refTime.substr(0,2)).c_str());
	unsigned short hour = atoi((refTime.substr(12,2)).c_str());
	unsigned short min = atoi((refTime.substr(15,2)).c_str());
	unsigned short sec = atoi((refTime.substr(18,2)).c_str());
	unsigned long usec = atol((refTime.substr(21,6)).c_str());
	long double result = DateTime::JD(DateTime(DateTime::UTCD(year,month,day,hour,min,sec,usec,6))).jd(); // PRQA S 3050, 3081, 4400
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"year is "<< year) ;
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"month is "<< month) ;
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"day is "<< day) ;
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"hour is "<< hour) ;
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"min is "<< min) ;
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"sec is "<< sec) ;
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"usec is "<< usec) ;
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB,"time "<< refTime << " converted in JD is " << setprecision(17) << result) ;
	ACS_CLASS_END_DEBUG
	return result ;
}

int EnvisatPOFileParser::parseStringMonth(const string& stringMonth) const // PRQA S 4020
{
	if ( (stringMonth == "JAN") || (stringMonth == "Jan") ) { return  1; }
	else if ( (stringMonth == "FEB") || (stringMonth == "Feb") ) { return  2; } // PRQA S 4400 11
	else if ( (stringMonth == "MAR") || (stringMonth == "Mar") ) { return  3; }
	else if ( (stringMonth == "APR") || (stringMonth == "Apr") ) { return  4; }
	else if ( (stringMonth == "MAY") || (stringMonth == "May") ) { return  5; }
	else if ( (stringMonth == "JUN") || (stringMonth == "Jun") ) { return  6; }
	else if ( (stringMonth == "JUL") || (stringMonth == "Jul") ) { return  7; }
	else if ( (stringMonth == "AUG") || (stringMonth == "Aug") ) { return  8; }
	else if ( (stringMonth == "SEP") || (stringMonth == "Sep") ) { return  9; }
	else if ( (stringMonth == "OCT") || (stringMonth == "Oct") ) { return  10; }
	else if ( (stringMonth == "NOV") || (stringMonth == "Nov") ) { return  11; }
	else if ( (stringMonth == "DEC") || (stringMonth == "Dec") ) { return  12; }
	else
	{
		ACS_THROW(exIllegalValueException("ERROR -> Cannot handle month "+stringMonth)) ; // PRQA S 3081
	}
}


void EnvisatPOFileParser::addFileName(const string& name)
{
	
	set<string>::iterator it = _fileNames.find(name);
	if( it == _fileNames.end() )//add filename only if NOT PRESENT yet
	{
		_fileNames.insert(name);
		_fileNamesVec.push_back(name);
		buildParams();
	}
}

void EnvisatPOFileParser::removeFileName(const string& name)
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

void EnvisatPOFileParser::parse(std::vector<Parameters *>& params)
{
	params.clear() ;
    size_t p_size = _validParams.size() ;
	for (size_t i=0; i < p_size; ++i) {
		params.push_back(&_validParams[i]) ;
    }
}


_ACS_END_NAMESPACE
