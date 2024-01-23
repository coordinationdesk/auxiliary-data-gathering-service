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
	Revision 2.2  2014/03/19 19:14:54  marpas
	coding best practices application in progress (not ended)
	warning fixing (qa & g++)
	
	Revision 2.1  2012/04/18 08:15:40  giucas
	compilation warnings fixed
	
	Revision 2.0  2006/02/28 09:32:51  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2005/02/21 14:43:53  giucas
	Imported libAuxFileParsing
	
	Revision 1.3  2004/12/09 16:15:23  giucas
	Updated after PredictedOrbitData class changes
	
	Revision 1.2  2004/12/07 11:35:23  giucas
	Implemented conversion from string to JD for times in PredictedOrbitData
	
	Revision 1.1  2004/12/06 17:34:07  giucas
	Imported sources
	

*/

#include <CryosatPOFileParser.h>
#include <File.h>
#include <XMLIstream.h>
#include <rsResourceSet.h>
#include <DateTime.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(CryosatPOFileParser)

//Class constructor
CryosatPOFileParser::CryosatPOFileParser() :
    PredictedOrbitFileParser(),
	_fileNames(),
	_fileNamesVec(),
	_fileType(),
	_validParams()
{
	static char Id[] = "@(#) A.C.S.: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Id: " << Id) ;
}

//Class constructor
CryosatPOFileParser::CryosatPOFileParser(const string& fileName) :
    PredictedOrbitFileParser(),
	_fileNames(),
	_fileNamesVec(),
	_fileType(),
	_validParams()
{
	static char Id[] = "@(#) A.C.S.: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Id: " << Id) ;
	
	_fileNames.insert(fileName);
	_fileNamesVec.push_back(fileName);
	buildParams();

} 

//Class destructor
CryosatPOFileParser::~CryosatPOFileParser() throw() 
{
}

//copy constructor
CryosatPOFileParser::CryosatPOFileParser(const CryosatPOFileParser & e) :
    PredictedOrbitFileParser(e),
	_fileNames(e._fileNames),
	_fileNamesVec(e._fileNamesVec),
	_fileType(e._fileType),
	_validParams(e._validParams)
{
}

//operator = 
CryosatPOFileParser&  CryosatPOFileParser::operator=(const CryosatPOFileParser & e)
{
	if (this != &e) 
	{
        PredictedOrbitFileParser::operator=(e) ;
		_fileNames = e._fileNames;
		_fileNamesVec = e._fileNamesVec;
		_fileType = e._fileType;
		_validParams = e._validParams;
	}
	return *this ;
}


void CryosatPOFileParser::buildParams()
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method called.") ;

	//clean current Valid Params
	_validParams.clear();
	
	//iterate through the set and build the valid parameter sequence
	set<string>::iterator it = _fileNames.begin();
	while ( it != _fileNames.end() )
	{
		parseFile(*it);
		it++;
	}
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method executed.") ;
}

void CryosatPOFileParser::parseFile(const string& fileName)
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
	rsFile.getValue("Data_Block.List_of_OSVs.count.value",num);
	for(unsigned int i=0; i<num; i++)
	{
		ostringstream count;
		count << i;
		string key = "Data_Block.List_of_OSVs.OSV[" + count.str() + "].";
		PredictedOrbitData poData;
		
		string taiTime="";
		rsFile.getValue(key+"TAI",taiTime);
		poData.taiTime = asciiCcsdsaRefMicrosecToJD(taiTime);
		
		string utcTime="";
		rsFile.getValue(key+"UTC",utcTime);
		poData.utcTime = asciiCcsdsaRefMicrosecToJD(utcTime);
		
		string ut1Time="";
		rsFile.getValue(key+"UT1",ut1Time);
		poData.ut1Time = asciiCcsdsaRefMicrosecToJD(ut1Time);
		
		rsFile.getValue(key+"Absolute_Orbit",poData.absOrbit);
		rsFile.getValue(key+"X",poData.xPosition);
		rsFile.getValue(key+"Y",poData.yPosition);
		rsFile.getValue(key+"Z",poData.zPosition);
		rsFile.getValue(key+"VX",poData.xVelocity);
		rsFile.getValue(key+"VY",poData.yVelocity);
		rsFile.getValue(key+"VZ",poData.zVelocity);
		
		_validParams.push_back(poData);
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Added element :" << poData.show()) ;
	}

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method executed.") ;
}

long double CryosatPOFileParser::asciiCcsdsaRefMicrosecToJD(const string& inName) const
{
 
  unsigned short year  = atoi((inName.substr(4,4)).c_str()); // PRQA S 3000, 3010 6
  unsigned short month = atoi((inName.substr(9,2)).c_str());
  unsigned short day = atoi((inName.substr(12,2)).c_str());
  unsigned short hour = atoi((inName.substr(15,2)).c_str());
  unsigned short min = atoi((inName.substr(18,2)).c_str());
  unsigned short sec = atoi((inName.substr(21,2)).c_str());
  unsigned long usec = atol((inName.substr(24,6)).c_str());
 
  DateTime::UTCD utcd(year,month,day,hour,min,sec,0); // PRQA S 3000
  DateTime dt (utcd) ;    
  DateTime::JD jd(dt); // PRQA S 3050
  return jd.jd() + usec/86400000000.0L; // PRQA S 3011, 3084, 4400
}



void CryosatPOFileParser::addFileName(const string& name)
{
	
	set<string>::iterator it = _fileNames.find(name);
	if( it == _fileNames.end() )//add filename only if NOT PRESENT yet
	{
///////////////////////////////////////////////////////////////////////
//   MULTIPLE FILE handling not implemented!!!!
///////////////////////////////////////////////////////////////////////
		_fileNames.erase(_fileNames.begin(),_fileNames.end());
		_fileNamesVec.erase(_fileNamesVec.begin(),_fileNamesVec.end());
		_fileNames.insert(name);
		_fileNamesVec.push_back(name);
		buildParams();
	}
}

void CryosatPOFileParser::removeFileName(const string& name)
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

void CryosatPOFileParser::parse(std::vector<Parameters *>& params)
{
	params.clear() ;
    size_t p_size = _validParams.size() ;
	for (size_t i=0; i < p_size; ++i) {
		params.push_back(&_validParams[i]) ;
    }
}


_ACS_END_NAMESPACE
