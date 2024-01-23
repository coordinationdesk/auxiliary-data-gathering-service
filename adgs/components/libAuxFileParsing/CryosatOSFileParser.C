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
	Revision 2.3  2014/03/19 19:14:54  marpas
	coding best practices application in progress (not ended)
	warning fixing (qa & g++)
	
	Revision 2.2  2012/04/18 08:15:39  giucas
	compilation warnings fixed
	
	Revision 2.1  2006/08/02 13:24:12  crivig
	bug fixed
	
	Revision 2.0  2006/02/28 09:32:51  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2005/02/21 14:43:53  giucas
	Imported libAuxFileParsing
	
	Revision 1.2  2004/12/07 14:32:52  giucas
	Added set of MLST
	
	Revision 1.1  2004/12/07 13:45:25  giucas
	Imported source
	

*/

#include <CryosatOSFileParser.h>
#include <File.h>
#include <XMLIstream.h>
#include <rsResourceSet.h>
#include <DateTime.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(CryosatOSFileParser)

//Class constructor
CryosatOSFileParser::CryosatOSFileParser() :
    OrbitScenarioFileParser(),
	_fileNames(),
	_fileType(),
	_fileNamesVec(),
	_validParams()
{
	static char Id[] = "@(#) A.C.S.: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Id: " << Id) ;
}

//Class constructor
CryosatOSFileParser::CryosatOSFileParser(const string& fileName) :
    OrbitScenarioFileParser(),
	_fileNames(),
	_fileType(),
	_fileNamesVec(),
	_validParams()
{
	static char Id[] = "@(#) A.C.S.: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Id: " << Id) ;
	
	_fileNames.insert(fileName);
	_fileNamesVec.push_back(fileName);
	buildParams();

} 

//Class destructor
CryosatOSFileParser::~CryosatOSFileParser() throw()  
{
}

//copy constructor
CryosatOSFileParser::CryosatOSFileParser(const CryosatOSFileParser & e) :
    OrbitScenarioFileParser(e),
	_fileNames(e._fileNames),
	_fileType(e._fileType),
	_fileNamesVec(e._fileNamesVec),
	_validParams(e._validParams)
{
}

//operator = 
CryosatOSFileParser&  CryosatOSFileParser::operator=(const CryosatOSFileParser & e)
{
	if (this != &e) 
	{
        OrbitScenarioFileParser::operator=(e),
		_fileNames				= e._fileNames;
		_fileNamesVec			= e._fileNamesVec;
		_fileType				= e._fileType;
		_validParams			= e._validParams;
	}
	return *this ;
}

void CryosatOSFileParser::cleanupParams()
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method called.") ;
	_validParams.erase(_validParams.begin(),_validParams.end());	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method executed.") ;
}

void CryosatOSFileParser::buildParams()
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

void CryosatOSFileParser::parseFile(const string& fileName)
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method called to process file " << fileName) ;
	
	if(!File::exists(fileName))
	{
		ACS_THROW(exFileOpenException("The input Auxiliary File [" + fileName + "] does not exist!")) ; // PRQA S 3081
	}
	
	rsResourceSet rsFile;
	XMLIstream istr(fileName);
	istr >> rsFile;
	
	rsFile.getValue("Earth_Explorer_Header.Fixed_Header.File_Type",_fileType);
	
	unsigned int num=0;
#warning MP: dangerous use getArray instead
	rsFile.getValue("Data_Block.List_of_Orbit_Changes.count.value",num);
	for(unsigned int i=0; i<num; i++)
	{
		ostringstream count;
		count << i;
		string key = "Data_Block.List_of_Orbit_Changes.Orbit_Change[" + count.str() + "].";
		OrbitScenario orbScen;
#warning MP: this constants is dangerous
		orbScen.satID(16);//CRYOSAT
		
        long int relOrbit=0 ;
		rsFile.getValue(key+"Orbit.Relative_Orbit",relOrbit);
		orbScen.relOrbit(relOrbit);
		
        long int absOrbit=0 ;
		rsFile.getValue(key+"Orbit.Absolute_Orbit",absOrbit);
		orbScen.absOrbit(absOrbit);
		
        long int cycle=0 ;
		rsFile.getValue(key+"Orbit.Cycle_Number",cycle);
		orbScen.cycle(cycle);
		
        long int phase=0 ;
		rsFile.getValue(key+"Orbit.Phase_Number",phase);
		orbScen.phase(phase);

        long int repeatCycle=0 ;
		rsFile.getValue(key+"Cycle.Repeat_Cycle",repeatCycle);
		orbScen.repeatCycle(repeatCycle);
		
        long int cycleLength=0 ;
		rsFile.getValue(key+"Cycle.Cycle_Length",cycleLength);
		orbScen.cycleLength(cycleLength);

		double drift=0.0 ;
		rsFile.getValue(key+"Cycle.MLST_Drift",drift);
		orbScen.drift(drift);

		string ascMlst ;
		rsFile.getValue(key+"Cycle.MLST",ascMlst);
		if (ascMlst != "") { orbScen.ascMlst( mlstToSec(ascMlst) ); }

		string ANXTime ;
		rsFile.getValue(key+"Time_of_ANX.UTC",ANXTime);
		orbScen.ANXTime(asciiCcsdsaRefMicrosecToJD(ANXTime));

        double ANXLon=0.0;
		rsFile.getValue(key+"Cycle.ANX_Longitude",ANXLon);
		orbScen.ANXLon(ANXLon);
		
		_validParams.push_back(orbScen);
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Added element : " << orbScen.show()) ;
	}

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method executed.") ;
}

long double CryosatOSFileParser::asciiCcsdsaRefMicrosecToJD(const string& inName) const 
{
 
  unsigned short year  = atoi((inName.substr(4,4)).c_str()); // PRQA S 3000, 3010 6
  unsigned short month = atoi((inName.substr(9,2)).c_str());
  unsigned short day = atoi((inName.substr(12,2)).c_str());
  unsigned short hour = atoi((inName.substr(15,2)).c_str());
  unsigned short min = atoi((inName.substr(18,2)).c_str());
  unsigned short sec = atoi((inName.substr(21,2)).c_str());
  unsigned long usec = atol((inName.substr(24,6)).c_str());
 
  return DateTime::JD(DateTime(DateTime::UTCD(year,month,day,hour,min,sec,usec,6))).jd(); // PRQA S 3050, 3081, 4400 
}

double CryosatOSFileParser::mlstToSec(const string& inName) const
{
  //inName is of the form hh:mm:ss.uuuuuu
  
  long int hour = atoi((inName.substr(0,2)).c_str());
  long int min = atoi((inName.substr(3,2)).c_str());
  long int sec = atoi((inName.substr(6,2)).c_str());
 
  return (24 - hour) * (60 - min) * (60 - sec); // PRQA S 3011, 4400
 
}


void CryosatOSFileParser::addFileName(const string& name)
{
	set<string>::iterator it = _fileNames.find(name);
	if( it == _fileNames.end() )//add filename only if NOT PRESENT yet
	{
		_fileNames.insert(name);
		_fileNamesVec.push_back(name);
		buildParams();
	}
}

void CryosatOSFileParser::removeFileName(const string& name)
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


void CryosatOSFileParser::parse(std::vector<Parameters *>& params)
{
	params.clear() ;
    size_t p_size = _validParams.size() ;
	for (size_t i=0; i < p_size; ++i) {
		params.push_back(&_validParams[i]) ;
    }
}


_ACS_END_NAMESPACE
