// PRQA S 1050 EOF
/*

	Copyright 1995-2014, Advanced Computer Systems , Inc.
	Via della Bufalotta, 347 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$
	
	$Log$
	Revision 2.2  2014/03/19 19:14:54  marpas
	coding best practices application in progress (not ended)
	warning fixing (qa & g++)
	
	Revision 2.1  2012/04/18 08:15:40  giucas
	compilation warnings fixed
	
	Revision 2.0  2006/02/28 09:32:51  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.3  2005/11/18 16:31:59  giucas
	Changed the extraction of refSBT parameter
	
	Revision 1.2  2005/11/17 15:41:14  giucas
	Improved refSBT parameter calculation
	
	Revision 1.1.1.1  2005/02/21 14:43:53  giucas
	Imported libAuxFileParsing
	
	Revision 1.1  2004/12/07 10:09:24  giucas
	Moved from libTimeConverter
	
	Revision 1.6  2004/12/03 09:33:26  giucas
	Header fixed
	
	Revision 1.5  2004/11/15 13:31:56  giucas
	Changed FileParser interface to handle multi-file initialization
	
	Revision 1.4  2004/10/12 07:41:49  giucas
	Added getFileName and getFileType methods
	
	Revision 1.3  2004/10/08 09:02:56  giucas
	Implemented parse method
	
	Revision 1.2  2004/10/07 10:02:25  giucas
	parse method still to be implemented
	
	Revision 1.1  2004/10/06 07:16:25  giucas
	First release.Implementation still to be completed.
	


*/

#include <EnvisatOBTFileParser.h>
#include <File.h>
#include <ConfigurationSingleton.h>
#include <TimeFormat.h>
#include <rsResourceSet.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(EnvisatOBTFileParser)

//Class constructor
EnvisatOBTFileParser::EnvisatOBTFileParser() :
    OBTFileParser(),
	_fileNames(),	
	_fileType(),
	_fileNamesVec(),
	_onBoardTimeGadsSize(0),
	_onBoardTimeGadsTimeSize(0),
	_onBoardTimeGadsSBTSize(0),
	_onBoardTimeGadsClockStepSize(0),
	_validParams()
{
	static char Id[] = "@(#) A.C.S.: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Id: " << Id) ;
	loadConfiguration();
}

//Class constructor
EnvisatOBTFileParser::EnvisatOBTFileParser(const string& fileName) :
    OBTFileParser(),
	_fileNames(),	
	_fileType(),
	_fileNamesVec(),
	_onBoardTimeGadsSize(0),
	_onBoardTimeGadsTimeSize(0),
	_onBoardTimeGadsSBTSize(0),
	_onBoardTimeGadsClockStepSize(0),
	_validParams()
{
	static char Id[] = "@(#) A.C.S.: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Id: " << Id) ;
	loadConfiguration();
	_fileNames.insert(fileName);
	_fileNamesVec.push_back(fileName);
	buildParams();
} 

//Class destructor
EnvisatOBTFileParser::~EnvisatOBTFileParser() throw() 
{
	cleanupParams(); // PRQA S 4631
}

//copy constructor
EnvisatOBTFileParser::EnvisatOBTFileParser(const EnvisatOBTFileParser & e):
    OBTFileParser(e),
    _fileNames(e._fileNames),	
	_fileType(e._fileType),
	_fileNamesVec(e._fileNamesVec),
	_onBoardTimeGadsSize(e._onBoardTimeGadsSize),
	_onBoardTimeGadsTimeSize(e._onBoardTimeGadsTimeSize),
	_onBoardTimeGadsSBTSize(e._onBoardTimeGadsSBTSize),
	_onBoardTimeGadsClockStepSize(e._onBoardTimeGadsClockStepSize),
	_validParams(e._validParams)
{
}

//operator = 
EnvisatOBTFileParser&  EnvisatOBTFileParser::operator=(const EnvisatOBTFileParser & e)
{
	if (this != &e) 
	{
		_fileNames						= e._fileNames;
		_fileType						= e._fileType;
		_fileNamesVec					= e._fileNamesVec;
		_onBoardTimeGadsSize			= e._onBoardTimeGadsSize;
		_onBoardTimeGadsTimeSize		= e._onBoardTimeGadsTimeSize;
		_onBoardTimeGadsSBTSize			= e._onBoardTimeGadsSBTSize;
		_onBoardTimeGadsClockStepSize	= e._onBoardTimeGadsClockStepSize;
		_validParams					= e._validParams;
	}
	return *this ;
}


void EnvisatOBTFileParser::cleanupParams()
{
	vector<USCTimeCorrParams*>::iterator parIt = _validParams.begin();
	while( parIt != _validParams.end() )
	{
		delete *parIt;
		*parIt=0;
		parIt = _validParams.erase(parIt);
	}
}

//parse each file in _fileNames set and add the valid parameters to _validParams vector.
//Since _fileNames is ordered by validity start/stop times, the _validParams are themselves ordered by time
void EnvisatOBTFileParser::buildParams()
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method called.") ;

	//clean current Valid Params
	cleanupParams();
	
	//iterate through the set and build the valid parameter sequence
	set<string>::iterator it = _fileNames.begin();
	while ( it != _fileNames.end() )
	{
		parseFile(*it);
		it++;
	}
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method executed.") ;
}


void EnvisatOBTFileParser::parseFile(const string& fileName)
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method called.") ;
	
	if(!File::exists(fileName))
	{
		ACS_THROW(exIOException("Cannot open auxiliary File \"" + fileName + "\""))  // PRQA S 3081
	}

	ifstream is(fileName.c_str());
	string line="";
	string tmpLine="";
	while( getline(is,tmpLine) ) // PRQA S 3050
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "got line '" << tmpLine << "'") ;
		line = tmpLine;
		tmpLine="";
	}

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Valid line is '" << line << "'" << " of size " << line.size()) ;
	
	//check if it is of expected size
	if(line.size() != _onBoardTimeGadsSize)
	{
		ostringstream msg;
		msg << "Error in getting OBT informations from file " << fileName
			<< " : Wrong line size [expected is " << _onBoardTimeGadsSize << " -> Found " << line.size() << "]";
		ACS_THROW(OBTFileParserException(msg.str()) ); // PRQA S 3081
	}	
	//now parse the line
	string refTime = line.substr(0,_onBoardTimeGadsTimeSize);
	string refSBT = line.substr(_onBoardTimeGadsTimeSize+1, _onBoardTimeGadsSBTSize); // PRQA S 3084 2
	string clockStep = line.substr(_onBoardTimeGadsTimeSize+_onBoardTimeGadsSBTSize+2,_onBoardTimeGadsClockStepSize);
	
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "PARSED VALUES ARE :") ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "refTime   = '" << refTime << "'") ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "refSBT    = '" << refSBT << "'") ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "clockStep = '" << clockStep << "'") ;
	ACS_CLASS_END_DEBUG
	
	//skip + sign
	refSBT = string(refSBT, 1 , refSBT.size());
	clockStep = string(clockStep, 1 , clockStep.size());

	USCTimeCorrParams* validParams = new USCTimeCorrParams;

 	//setting refTime
 	validParams->refTime = getMJD2000FromAsciiEnviMicrosec(refTime);	

	//setting reference SBT
	istringstream iSBTstr(refSBT);
	unsigned long int rSBT = 0;
	iSBTstr >> rSBT;
	validParams->refSBT.push_back(rSBT);

	//setting clock step
	istringstream iClstr(clockStep);
	unsigned long int clSt= 0 ;
	iClstr >> clSt;
	validParams->clockStep = clSt;

	_validParams.push_back(validParams);

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "returning param :" << validParams->dump()) ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method executed.") ;
}


void EnvisatOBTFileParser::loadConfiguration()
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method called.") ;
	
	rsResourceSet& rsConf = ConfigurationSingleton::instance()->get();
	
	unsigned int n=0;
#warning MP: use getArray instead	
	rsConf.getValue("ExplorerWrapper.List_of_Satellites.count.value",n);
	
	if(n == 0)
	{
		ACS_THROW(OBTFileParserException("no satellites configured in Configuration Space.")); // PRQA S 3081
	}

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, n << " satellites configured") ;

	bool infoSet=false;
	for(unsigned int i=0; i<n; i++)
	{
		ostringstream num;
		num << i;
		int rsId=0;
		rsConf.getValue("ExplorerWrapper.List_of_Satellites.Satellite["+num.str()+"].Id",rsId);
		if(rsId == static_cast<int>(TimeFormat::TC_SAT_ENVISAT))//got the rigth satellite : load data // PRQA S 3081
		{
			//load the UTC/SBT TimeConversion File data
			rsConf.getValue("ExplorerWrapper.List_of_Satellites.Satellite["+num.str()+"].TimeConversionFile.FileType",_fileType);
			rsConf.getValue("ExplorerWrapper.List_of_Satellites.Satellite["+num.str()+"].TimeConversionFile.OnBoardTimeGadsSize",
						     _onBoardTimeGadsSize);
			rsConf.getValue("ExplorerWrapper.List_of_Satellites.Satellite["+num.str()+"].TimeConversionFile.OnBoardTimeGadsTimeSize",
						     _onBoardTimeGadsTimeSize);
			rsConf.getValue("ExplorerWrapper.List_of_Satellites.Satellite["+num.str()+"].TimeConversionFile.OnBoardTimeGadsSBTSize",
						     _onBoardTimeGadsSBTSize);
			rsConf.getValue("ExplorerWrapper.List_of_Satellites.Satellite["+num.str()+"].TimeConversionFile.OnBoardTimeGadsClockStepSize",
						     _onBoardTimeGadsClockStepSize);
			ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
				ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Found Time Conversion File OnBoardTimeGadsSize [" << _onBoardTimeGadsSize << "]") ;
				ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Found Time Conversion File OnBoardTimeGadsTimeSize [" << _onBoardTimeGadsTimeSize << "]") ;
				ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Found Time Conversion File OnBoardTimeGadsSBTSize [" << _onBoardTimeGadsSBTSize << "]") ;
				ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Found Time Conversion File OnBoardTimeGadsClockStepSize [" << _onBoardTimeGadsClockStepSize << "]") ;
			ACS_CLASS_END_DEBUG
			infoSet=true;
			break;
		}
	}
	
	if(!infoSet)
	{
		ACS_THROW(OBTFileParserException("no configuration found for satellite Envisat")) ; // PRQA S 3081
	}
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method executed.") ;
}

int EnvisatOBTFileParser::parseStringMonth(const string& stringMonth) const // PRQA S 4020
{
	if ( (stringMonth == "JAN") || (stringMonth == "Jan") ) { return 1; }
	else if ( (stringMonth == "FEB") || (stringMonth == "Feb") ) { return 2; } // PRQA S 4400 11
	else if ( (stringMonth == "MAR") || (stringMonth == "Mar") ) { return 3; }
	else if ( (stringMonth == "APR") || (stringMonth == "Apr") ) { return 4; }
	else if ( (stringMonth == "MAY") || (stringMonth == "May") ) { return 5; }
	else if ( (stringMonth == "JUN") || (stringMonth == "Jun") ) { return 6; }
	else if ( (stringMonth == "JUL") || (stringMonth == "Jul") ) { return 7; }
	else if ( (stringMonth == "AUG") || (stringMonth == "Aug") ) { return 8; }
	else if ( (stringMonth == "SEP") || (stringMonth == "Sep") ) { return 9; }
	else if ( (stringMonth == "OCT") || (stringMonth == "Oct") ) { return 10; }
	else if ( (stringMonth == "NOV") || (stringMonth == "Nov") ) { return 11; }
	else if ( (stringMonth == "DEC") || (stringMonth == "Dec") ) { return 12; }
	else
	{
		ACS_THROW(OBTFileParserException("Cannot handle month \"" + stringMonth + "\"")) ; // PRQA S 3081
	}
}

long double EnvisatOBTFileParser::getMJD2000FromAsciiEnviMicrosec(const string& refTime) const
{
	//refTime is of the form : DD-MMM-YYYY HH:NN:SS.UUUUUU (TimeFormat::TC_ASCII_ENVI_MICROSEC)
	int year  = atoi((refTime.substr(7,4)).c_str());
	string stringMonth = refTime.substr(3,3);
	int month = parseStringMonth(stringMonth);
	int day = atoi((refTime.substr(0,2)).c_str());
	int hour = atoi((refTime.substr(12,2)).c_str());
	int min = atoi((refTime.substr(15,2)).c_str());
	int sec = atoi((refTime.substr(18,2)).c_str());
	int usec = atoi((refTime.substr(21,6)).c_str());
	int M1 = ( month - 14) / 12;
	int Y1 = year + 4800;
	int JD = 1461 * (Y1 + M1)/ 4 + 367 * (month - 2 - 12 * M1)/12 - (3 * ((Y1 + M1 + 100)/100))/4 + day - 32075;
	long double timeMJD2000 = JD - 2451545.0 ; // PRQA S 3011
	long double timeMJD2000Sec = timeMJD2000*86400 + hour*3600 + min*60 + sec + usec*0.000001; // PRQA S 3011
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "year is "<< year) ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "stringMonth is '"<< stringMonth << "'") ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "month is "<< month) ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "day is "<< day) ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "hour is "<< hour) ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "min is "<< min) ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "sec is "<< sec) ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "usec is "<< usec) ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "JD is "<< JD) ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "timeMJD2000 is "<< setprecision(17) << timeMJD2000) ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "time "<< refTime << " converted in MJD2000 (seconds) = " << setprecision(17) << timeMJD2000Sec) ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "time "<< refTime << " converted in MJD2000 (days) = " << setprecision(17) << timeMJD2000Sec/86400.0) ;
	ACS_CLASS_END_DEBUG
	return (timeMJD2000Sec/86400.0); // PRQA S 4400
}

void EnvisatOBTFileParser::addFileName(const string& name)
{
	set<string>::iterator it = _fileNames.find(name);
	if( it == _fileNames.end() )//add filename only if NOT PRESENT yet
	{
		_fileNames.insert(name);
		_fileNamesVec.push_back(name);
		buildParams();
	}
}
	
void EnvisatOBTFileParser::removeFileName(const string& name)
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

void EnvisatOBTFileParser::parse(std::vector<Parameters *>& params)
{
	params.clear() ;
    size_t p_size = _validParams.size() ;
	for (size_t i=0; i < p_size; ++i) {
		params.push_back(_validParams[i]) ;
    }
}


_ACS_END_NAMESPACE
