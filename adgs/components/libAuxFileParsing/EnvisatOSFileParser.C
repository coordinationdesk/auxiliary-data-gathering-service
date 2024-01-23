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
	
	Revision 1.2  2005/11/17 08:08:50  giucas
	Fixed case of only 1 record in Orbit Scenario File
	
	Revision 1.1  2005/11/16 18:22:57  giucas
	First release
	

*/

#include <EnvisatOSFileParser.h>
#include <File.h>
#include <DateTime.h>
#include <boost/tokenizer.hpp> // PRQA S 1013

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;
using namespace boost;

ACS_CLASS_DEFINE_DEBUG_LEVEL(EnvisatOSFileParser)

namespace {
    const string _firstValidLineValue 	= "LIST num_osf_rec";
    const string _recordStart 			= "RECORD osf_rec";
    const string _recordStop  			= "ENDRECORD osf_rec";
}
//Class constructor
EnvisatOSFileParser::EnvisatOSFileParser() :
    OrbitScenarioFileParser(),
	_fileNames(),
	_fileType("MPL_ORB_SC"),
	_fileNamesVec(),
	_validParams()
{
	static char Id[] = "@(#) A.C.S.: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Id: " << Id) ;
}

//Class constructor
EnvisatOSFileParser::EnvisatOSFileParser(const string& fileName) :
    OrbitScenarioFileParser(),
	_fileNames(),
	_fileType("MPL_ORB_SC"),
	_fileNamesVec(),
	_validParams()
{
	static char Id[] = "@(#) A.C.S.: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Id: " << Id) ;
	
	_fileNames.insert(fileName);
	_fileNamesVec.push_back(fileName);
	buildParams() ;
} 

//Class destructor
EnvisatOSFileParser::~EnvisatOSFileParser() throw() 
{
}

//copy constructor
EnvisatOSFileParser::EnvisatOSFileParser(const EnvisatOSFileParser & e):
    OrbitScenarioFileParser(e), 
	_fileNames(e._fileNames),
	_fileType(e._fileType),
	_fileNamesVec(e._fileNamesVec),
	_validParams(e._validParams)
{
}

//operator = 
EnvisatOSFileParser&  EnvisatOSFileParser::operator=(const EnvisatOSFileParser & e)
{
	if (this != &e) 
	{
        OrbitScenarioFileParser::operator=(e); 
		_fileNames				= e._fileNames;
		_fileNamesVec			= e._fileNamesVec;
		_fileType				= e._fileType;
		_validParams			= e._validParams;
	}
	return *this ;
}

void EnvisatOSFileParser::cleanupParams()
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method called.") ;
	_validParams.erase(_validParams.begin(),_validParams.end());	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method executed.") ;
}

void EnvisatOSFileParser::buildParams()
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

void EnvisatOSFileParser::parseFile(const string& fileName)
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method called to process file " << fileName) ;
	
	if(!File::exists(fileName))
	{
		ACS_THROW(exFileOpenException("The input Auxiliary File [" + fileName + "] does not exist!")) ; // PRQA S 3081
	}
	
	ifstream is(fileName.c_str());
	string tmpLine="";
	unsigned int lineIndex=0 ;
	int numberOfRecords=-1;
	
	//retrieve the number of records to read
	while( getline(is,tmpLine) ) // PRQA S 3050
	{
		lineIndex++;
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "got line number " << lineIndex << " : [" << tmpLine << "]") ;
		if( tmpLine.find(_firstValidLineValue) != string::npos )
		{
			//extract the number of elements to store
			string num_osf_rec = tmpLine.substr(tmpLine.find('=') +1, tmpLine.size() -1 - tmpLine.find('=')); // PRQA S 3084
			numberOfRecords = atoi(num_osf_rec.c_str());
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "numberOfRecords is " << numberOfRecords) ;
			tmpLine="";
			break;
		}		
		//reset tmpLine
		tmpLine="";
	}

	if( numberOfRecords == -1 )
	{
		ACS_THROW( OrbitScenarioFileParserException("Bad input file "+fileName+" format") ); // PRQA S 3081
	}

	if( numberOfRecords == 0 )
	{
		ACS_THROW( exIllegalValueException("No records defined in input file "+fileName) ); // PRQA S 3081
	}
	
	int count=1;
	typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
	boost::char_separator<char> sep(" ");
	OrbitScenario orbScen;
#warning MP: this constants is dangerous
	orbScen.satID(17);//ENVISAT 
	//start retrieving the LAST record values in LIST
	while( getline(is,tmpLine) ) // PRQA S 3050
	{
		string trimmedLine = trim(tmpLine);
		
		if( trimmedLine == "" ) {
			continue;
        }

		if( (trimmedLine.find(_recordStart) != string::npos ) && ( count == numberOfRecords ) )
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "osf_rec found...") ;
			for(int k=1; k<=2; k++) // PRQA S 4400
			{	
				//line 1 is -> "RECORD orbit:"
				//line 2 is -> "RECORD cycle:"
				getline(is,tmpLine);
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Parsing record " << k << " '" <<tmpLine << "'") ;

				string tmp = tmpLine.erase(0,tmpLine.find("RECORD"));
				tokenizer tokens(tmp, sep);
				for (tokenizer::iterator tok_iter = tokens.begin();
				   tok_iter != tokens.end(); tok_iter++)
				{

					if( (*tok_iter).find("ABS") != string::npos )
					{
						//extract the absolute orbit
						string abs = (*tok_iter).substr((*tok_iter).find('=') +1, (*tok_iter).size() -1 - (*tok_iter).find('=')); // PRQA S 3084
						long int absOrbit = atoi(abs.c_str());
						orbScen.absOrbit(absOrbit);
						ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ABS is '" <<abs << "' -> " << absOrbit ) ;
						continue;
					}
					if( (*tok_iter).find("REL") != string::npos )
					{
						//extract the relative orbit
						string relOrb = (*tok_iter).substr((*tok_iter).find('=') +1, (*tok_iter).size() -1 - (*tok_iter).find('=')); // PRQA S 3084
						long int relOrbit = atoi(relOrb.c_str());
						orbScen.relOrbit(relOrbit);
						ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "RELATIVE ORBIT is '" <<relOrb << "' -> " <<  relOrbit) ;
						continue;
					}
					if( (*tok_iter).find("CYCLE") != string::npos )
					{
						//extract the cycle
						string cycle = (*tok_iter).substr((*tok_iter).find('=') +1, (*tok_iter).size() -1 - (*tok_iter).find('=')); // PRQA S 3084
						long int icycle = atoi(cycle.c_str());
						orbScen.cycle(icycle);
						ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "CYCLE is '" <<cycle << "' -> " << icycle) ;
						continue;
					}
					if( (*tok_iter).find("PHASE") != string::npos )
					{
						//extract the PHASE
						string phase = (*tok_iter).substr((*tok_iter).find('=') +1, (*tok_iter).size() -1 - (*tok_iter).find('=')); // PRQA S 3084
						long int iphase = atoi(phase.c_str());
						orbScen.phase(iphase);
						ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "PHASE is '" <<phase << "' -> " << iphase) ;
						continue;
					}
					if( (*tok_iter).find("DAYS") != string::npos )
					{
						//extract the repeat cycle
						string days = (*tok_iter).substr((*tok_iter).find('=') +1, (*tok_iter).size() -1 - (*tok_iter).find('=')); // PRQA S 3084
						long int repeatCycle = atoi(days.c_str());
						orbScen.repeatCycle(repeatCycle);
						ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "REPEAT CYCLE is '" <<days << "' -> " << repeatCycle ) ;		
						continue;
					}
					if( (*tok_iter).find("ORBITS") != string::npos )
					{
						//extract the CYCLE LENGTH
						string orbits = (*tok_iter).substr((*tok_iter).find('=') +1, (*tok_iter).size() -1 - (*tok_iter).find('=')); // PRQA S 3084
						long int cycleLength = atoi(orbits.c_str());
						orbScen.cycleLength(cycleLength);
						ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "CYCLE LENGTH is '" <<orbits << "' -> " << cycleLength) ;
						continue;
					}
					if( (*tok_iter).find("ANX_LONG") != string::npos )
					{
						//extract the ANX LONGITUDE
						string anx_long = (*tok_iter).substr((*tok_iter).find('=') +1, (*tok_iter).size() -1 - (*tok_iter).find('=')); // PRQA S 3084
						string longWithoutDeg = anx_long.substr(0,anx_long.find("<"));
						double ANXLon = atof(longWithoutDeg.c_str());
						orbScen.ANXLon(ANXLon);
						ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ANX LONGITUDE is '" <<longWithoutDeg << "' -> "<< setprecision(17)<< ANXLon) ;;
						continue;
					}
					if( (*tok_iter).find("MLST") != string::npos )
					{
						//extract the MLST
						string mlst = (*tok_iter).substr((*tok_iter).find('=') +1, (*tok_iter).size() -1 - (*tok_iter).find('=')); // PRQA S 3084
						string cleanmlst = mlst.substr(1,mlst.size()-2); // PRQA S 3084
						orbScen.ascMlst( mlstToSec(cleanmlst) );
						ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "MLST is '" <<cleanmlst << "' -> " << mlstToSec(cleanmlst)) ;
						continue;
					}
				}
				tmpLine="";
			}
			//retrieve ANX_UTC time
			getline(is,tmpLine);
			string anxTime = tmpLine.substr(tmpLine.find("\"")+1, tmpLine.rfind("\"")-1-tmpLine.find("\"")); // PRQA S 3084
			orbScen.ANXTime(asciiCcsdsaRefMicrosecToJD(anxTime));
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ANX UTC TIME is '" <<anxTime << "' -> " << asciiCcsdsaRefMicrosecToJD(anxTime)) ;						
			break;
		}//end if _recordStart found
		
		if( trimmedLine.find(_recordStop) != string::npos )
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Stop record found...") ;
			count++;
		}
	}
	
	_validParams.push_back(orbScen);
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method executed.") ;
}

long double EnvisatOSFileParser::asciiCcsdsaRefMicrosecToJD(const string& inName) const
{
  	//refTime is of the form : DD-MMM-YYYY HH:NN:SS.UUUUUU (TimeFormat::TC_ASCII_ENVI_MICROSEC)
	unsigned short year  = atoi((inName.substr(7,4)).c_str()); // PRQA S 3000, 3010 8
	string stringMonth = inName.substr(3,3);
	unsigned short month = parseStringMonth(stringMonth);
	unsigned short day = atoi((inName.substr(0,2)).c_str());
	unsigned short hour = atoi((inName.substr(12,2)).c_str());
	unsigned short min = atoi((inName.substr(15,2)).c_str());
	unsigned short sec = atoi((inName.substr(18,2)).c_str());
	unsigned long usec = atol((inName.substr(21,6)).c_str());
 
  	return DateTime(DateTime::UTCD(year,month,day,hour,min,sec,usec, 6)).jd50(); // PRQA S 3081, 4400
}

int EnvisatOSFileParser::parseStringMonth(const string& stringMonth) const // PRQA S 4020
{
	if ( (stringMonth == "JAN") || (stringMonth == "Jan") ) { return 1; }
	else if ( (stringMonth == "FEB") || (stringMonth == "Feb") ) {  return 2; } // PRQA S 4400 11
	else if ( (stringMonth == "MAR") || (stringMonth == "Mar") ) {  return 3; }
	else if ( (stringMonth == "APR") || (stringMonth == "Apr") ) {  return 4; }
	else if ( (stringMonth == "MAY") || (stringMonth == "May") ) {  return 5; }
	else if ( (stringMonth == "JUN") || (stringMonth == "Jun") ) {  return 6; }
	else if ( (stringMonth == "JUL") || (stringMonth == "Jul") ) {  return 7; }
	else if ( (stringMonth == "AUG") || (stringMonth == "Aug") ) {  return 8; }
	else if ( (stringMonth == "SEP") || (stringMonth == "Sep") ) {  return 9; }
	else if ( (stringMonth == "OCT") || (stringMonth == "Oct") ) {  return 10; }
	else if ( (stringMonth == "NOV") || (stringMonth == "Nov") ) {  return 11; }
	else if ( (stringMonth == "DEC") || (stringMonth == "Dec") ) {  return 12; }
	else
	{
		ACS_THROW(exIllegalValueException("Cannot handle month \""+stringMonth+ "\"")) ; // PRQA S 3081
	}
}


double EnvisatOSFileParser::mlstToSec(const string& inName) const
{
  //inName is of the form hh:mm:ss
  
  long int hour = atoi((inName.substr(0,2)).c_str());
  long int min = atoi((inName.substr(3,2)).c_str());
  long int sec = atoi((inName.substr(6,2)).c_str());
 
  return (24 - hour) * (60 - min) * (60 - sec); // PRQA S 3011, 4400
  
}

string EnvisatOSFileParser::trim_right ( const string& source , const string & t)
{
	string str = source;
	return str.erase ( str.find_last_not_of ( t ) + 1 ) ; // PRQA S 3084
}

string EnvisatOSFileParser::trim_left ( const string& source , const string & t)
{
	string str = source;
	return str.erase ( 0 , source.find_first_not_of ( t ) ) ;
}

string EnvisatOSFileParser::trim ( const string& source , const string & t)
{
	string str = source;
	return trim_left ( trim_right ( str , t ) , t ) ;
}


void EnvisatOSFileParser::addFileName(const string& name)
{
	set<string>::iterator it = _fileNames.find(name);
	if( it == _fileNames.end() )//add filename only if NOT PRESENT yet
	{
		_fileNames.insert(name);
		_fileNamesVec.push_back(name);
		buildParams();
	}
}

void EnvisatOSFileParser::removeFileName(const string& name)
{
	set<string>::iterator it = _fileNames.find(name);
	if( it != _fileNames.end() )//remove filename only if previously added
	{
		_fileNames.erase(name);
		_fileNamesVec.erase(find(_fileNamesVec.begin(),_fileNamesVec.end(),name)) ;
		buildParams();
	}
}

void EnvisatOSFileParser::parse(std::vector<Parameters *>& params)
{
	params.clear() ;
    size_t p_size = _validParams.size() ;
	for (size_t i=0; i < p_size; ++i) {
		params.push_back(&_validParams[i]) ;
    }
}

_ACS_END_NAMESPACE
