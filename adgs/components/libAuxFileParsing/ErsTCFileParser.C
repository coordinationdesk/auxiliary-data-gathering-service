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

	$Prod: A.C.S. libAuxFileParsing $

	$Id$

	$Author$

	$Log$
	Revision 1.6  2014/03/27 12:44:39  marpas
	coding best practices applied
	interface rationalization
	warnings fixed (qa & g++)
	
	Revision 1.5  2014/03/19 19:14:55  marpas
	coding best practices application in progress (not ended)
	warning fixing (qa & g++)
	
	Revision 1.4  2012/04/18 08:15:40  giucas
	compilation warnings fixed
	
	Revision 1.3  2009/06/22 12:49:50  marpas
	printf format fixed
	
	Revision 1.2  2009/06/17 12:09:26  marant
	Added print() method.
	
	Revision 1.1  2006/12/12 09:42:55  marant
	ErsTCFileParser class added
	
       
*/ 

#include <ErsTCFileParser.h>
#include <File.h>
#include <ConfigurationSingleton.h>
#include <TimeFormat.h>
#include <rsResourceSet.h>

#include <DateTime.h>


#include <cstdio>
#include <cstdlib>
#include <sys/types.h> // PRQA S 1013 2 
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <sstream>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(ErsTCFileParser)

//Class constructor
ErsTCFileParser::ErsTCFileParser() :
    OBTFileParser(),
	_fileNames(),
	_fileType(),
	_fileNamesVec(),
	_validParams()
{
	static char Id[] = "@(#) A.C.S.: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Id: " << Id );
}

//Class constructor
ErsTCFileParser::ErsTCFileParser(const string& fileName):
    OBTFileParser(),
	_fileNames(),
	_fileType(),
	_fileNamesVec(),
	_validParams()
{
	static char Id[] = "@(#) A.C.S.: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Id: " << Id );
	_fileNames.insert(fileName);
	_fileNamesVec.push_back(fileName);
	buildParams();
} 

//Class destructor
ErsTCFileParser::~ErsTCFileParser() throw() 
{
	cleanupParams(); // PRQA S 4631
}

//copy constructor
ErsTCFileParser::ErsTCFileParser(const ErsTCFileParser & e):
    OBTFileParser(e),
	_fileNames(e._fileNames),
	_fileType(e._fileType),
	_fileNamesVec(e._fileNamesVec),
	_validParams(e._validParams)
{
}

//operator = 
ErsTCFileParser&  ErsTCFileParser::operator=(const ErsTCFileParser & e)
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


void ErsTCFileParser::cleanupParams()
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"method called.") ;

	vector<ErsTimeCorrParams*>::iterator parIt = _validParams.begin();
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
void ErsTCFileParser::buildParams()
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


void ErsTCFileParser::parseFile(const string& fileName)
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"method called.") ;
	

    ifstream fd(fileName.c_str(), ios::in); 


    string tmpString("");

    fd >> tmpString ;

    if (tmpString.substr(0,3) != "UTC") { // PRQA S 4400
		fd.close();
		ACS_THROW(exIOException("Error reading file " + fileName + " - Bad format for UTC Reference time")) ; // PRQA S 3081
	}

    DateTime dt(tmpString);

    long double referenceUtc = dt.jd50();

    /* satellite reference binary time */
	unsigned long satBinTime(0);
    fd >> satBinTime;

    /* step length of clock in nanoseconds */
    unsigned long clockStepLength(0);
    fd >> clockStepLength;



    ErsTimeCorrParams* validParams = new ErsTimeCorrParams;

    ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Ers PARSED VALUES ARE :") ;
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"referenceUtc	= '" << tmpString << "'") ;
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"referenceUtc	= '" << setprecision(30) << referenceUtc	<< "'") ;
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"satBinTime		= '" << satBinTime      << "'") ;
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"clockStepLength	= '" << clockStepLength << "'") ;
    ACS_CLASS_END_DEBUG

    // set parameters
    validParams->ReferenceUTC     = referenceUtc; 
    validParams->SatBinTime       = satBinTime;   
    validParams->ClockStepLength  = clockStepLength;
    
    _validParams.push_back(validParams);

    fd.close();
    
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"method executed.") ;
}



void ErsTCFileParser::print(ostream* fd, 
                            double referenceJd50, 
                            unsigned long satBinTime, 
                            unsigned long clockStepLength)
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"method called.") ;

    DateTime::UTCD utcd;
    DateTime::JD50 jd50(referenceJd50); 
	DateTime dt(jd50);
	dt.toUTCD(utcd); // convert in UTC

    //char buffer[30]; // PRQA S 4403
    //sprintf(buffer,"UTC=%04d-%02d-%02dT%02d:%02d:%02d.%06ld", utcd.year, utcd.month, utcd.day, utcd.hour, utcd.min, utcd.sec, utcd.microsec());
    //*fd << buffer << '\n' ;
    
    *fd << "UTC=" << dt.asISO8601(6) << endl;
    *fd << satBinTime << " " << clockStepLength  << '\n' ;

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"method executed.") ;
}

void ErsTCFileParser::addFileName(const string& name)
{
	set<string>::iterator it = _fileNames.find(name);
	if( it == _fileNames.end() )//add filename only if NOT PRESENT yet
	{
		_fileNames.insert(name);
		_fileNamesVec.push_back(name);
		buildParams();
	}
}
	
void ErsTCFileParser::removeFileName(const string& name)
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

void ErsTCFileParser::parse(vector<Parameters*>& params)
{
	params.clear() ;
	size_t vp_size = _validParams.size() ;
    for (size_t i=0; i < vp_size; ++i) {
		params.push_back(_validParams[i]) ;
    }
}


_ACS_END_NAMESPACE
