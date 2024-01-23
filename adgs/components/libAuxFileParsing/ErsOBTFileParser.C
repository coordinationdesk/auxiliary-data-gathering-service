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
	Revision 2.5  2018/09/27 13:53:07  enrcar
	EC:: missing include fixed
	
	Revision 2.4  2014/03/19 19:14:55  marpas
	coding best practices application in progress (not ended)
	warning fixing (qa & g++)
	
	Revision 2.3  2012/04/18 08:15:40  giucas
	compilation warnings fixed
	
	Revision 2.2  2011/03/01 17:16:02  marpas
	GCC 4.4.x support
	
	Revision 2.1  2006/04/05 15:15:24  seralb
	O_RDONLY instead of O_RDWR
	
	Revision 2.0  2006/02/28 09:32:51  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1  2005/07/01 14:10:36  seralb
	Added ERS satellite
	
	Revision 1.1.1.1  2005/02/21 14:43:53  giucas


*/

#include <ErsOBTFileParser.h>
#include <ErsTimeCorrParams.h>
#include <File.h>
#include <ConfigurationSingleton.h>
#include <TimeFormat.h>
#include <rsResourceSet.h>


#include <vector>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <sys/types.h> // PRQA S 1013 2 
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <algorithm>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(ErsOBTFileParser)

//Class constructor
ErsOBTFileParser::ErsOBTFileParser() :
    OBTFileParser(), 
	_fileNames(),
	_fileType(),
	_fileNamesVec(),
	_validParams()
{
	static char Id[] = "@(#) A.C.S.: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Id: " << Id) ;
}

//Class constructor
ErsOBTFileParser::ErsOBTFileParser(const string& fileName) :
    OBTFileParser(), 
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
ErsOBTFileParser::~ErsOBTFileParser() throw() 
{
	cleanupParams(); // PRQA S 4631
}

//copy constructor
ErsOBTFileParser::ErsOBTFileParser(const ErsOBTFileParser & e):
    OBTFileParser(e), 
	_fileNames(e._fileNames),
	_fileType(e._fileType),
	_fileNamesVec(e._fileNamesVec),
	_validParams(e._validParams)
{
	*this = e ;
}

//operator = 
ErsOBTFileParser&  ErsOBTFileParser::operator=(const ErsOBTFileParser & e)
{
	if (this != &e) 
	{
        OBTFileParser::operator=(e) ;
		_fileNames = e._fileNames;
		_fileType = e._fileType;
		_fileNamesVec = e._fileNamesVec;
		_validParams = e._validParams;
	}
	return *this ;
}


void ErsOBTFileParser::cleanupParams()
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method called.") ;

	vector<ErsTimeCorrParams*>::iterator parIt = _validParams.begin();
	while( parIt != _validParams.end() )
	{
		delete *parIt;
		*parIt=0;
		parIt = _validParams.erase(parIt);
	}

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method executed.") ;
}

//parse each file in _fileNames set and add the valid parameters to _validParams vector.
//Since _fileNames is ordered by validity start/stop times, the _validParams are themselves ordered by time
void ErsOBTFileParser::buildParams()
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method called.") ;

	//clean current Valid Params
	cleanupParams();
	
	//iterate through the set and build the valid parameter sequence
	std::set<std::string>::iterator it = _fileNames.begin();
	while ( it != _fileNames.end() )
	{
		parseFile(*it);
		it++;
	}
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method executed.") ;
}


void ErsOBTFileParser::parseFile(const string& fileName)
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method called.") ;
	
	int fd = open(fileName.c_str(),O_RDONLY);
	if(fd==-1)
	{
		ACS_THROW(exIOException("Error opening " + fileName, errno)); // PRQA S 3081
	}
	
	ssize_t sizeOfFile = File::size(fileName);
	unsigned char* fileStream = new unsigned char[sizeOfFile]; // PRQA S 3000 2
	ssize_t ret = read(fd,fileStream,sizeOfFile);
    int eno = errno ;
	if(ret==-1)
	{
        close(fd) ;
    	delete [] fileStream;

		ACS_THROW(exIOException("Error reading " + fileName, eno)); // PRQA S 3081
	}
	close(fd);

    try {
	    // Positioning on input stream	
	    int filePos = 30;
	    if(fileStream[filePos] == '\n') {
		    filePos++;
        }

 	    ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		    ostringstream msg;
		    msg << "ErsOBTFileParser::parse : file   = ";
		    for(int i=filePos; i<filePos+8; i++) // PRQA S 4400
		    {
			    msg <<"'" << fileStream[i] << "' - ";
		    }
		    for(int i=filePos+8; i<filePos+24; i++) // PRQA S 4400
		    {
			    msg <<"'" << static_cast<int>(fileStream[i]) << "' - "; // PRQA S 3081
		    }
		    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, msg.str()) ;
	    ACS_CLASS_END_DEBUG

	    // atoi value from ascii portion
        int orbit=0;
	    char tmpOrbit[5]; // PRQA S 4403
	    memcpy(tmpOrbit, &fileStream[filePos], 5); // PRQA S 4400
	    orbit=atoi(tmpOrbit);
	    filePos += 8; // skip also 3 blanks // PRQA S 4400 L2

        // Binary section 
	    // NB: file is LITTLE ENDIAN
	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "File Pos at day is :" << filePos) ;
	    unsigned int day = fileStream[filePos+3]; 
	    day  = ( day << 8 ) | fileStream[filePos+2];
	    day  = ( day << 8 ) | fileStream[filePos+1];
	    day  = ( day << 8 ) | fileStream[filePos];
	    filePos += 4; 

	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "File Pos at msec is :" << filePos) ;
	    unsigned int ms = fileStream[filePos+3]; 
	    ms  = ( ms << 8 ) | fileStream[filePos+2];
	    ms  = ( ms << 8 ) | fileStream[filePos+1];
	    ms  = ( ms << 8 ) | fileStream[filePos];
	    filePos += 4; 

	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "File Pos at binTime is :" << filePos) ;
   	    unsigned int binTime = fileStream[filePos+3]; 
	    binTime  = ( binTime << 8 ) | fileStream[filePos+2];
	    binTime  = ( binTime << 8 )  | fileStream[filePos+1];
	    binTime  = ( binTime << 8 )  | fileStream[filePos];
	    filePos += 4; 

	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "File Pos at clockPeriod is :" << filePos) ;
	    unsigned int clockPeriod = fileStream[filePos+3]; 
	    clockPeriod  = ( clockPeriod << 8 ) | fileStream[filePos+2];
	    clockPeriod  = ( clockPeriod << 8 ) | fileStream[filePos+1];
	    clockPeriod  = ( clockPeriod << 8 ) | fileStream[filePos];

	    ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
		    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "PARSED VALUES ARE :" ) ;
		    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "      orbit = '" << orbit << "'") ;
		    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "        day = '" << day << "'") ;
		    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "         ms = '" << ms << "'") ;
		    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "    binTime = '" << binTime << "'") ;
		    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "clockPeriod = '" << clockPeriod << "'") ;
	    ACS_CLASS_END_DEBUG
        
        // PRQA L:L2

	    ErsTimeCorrParams* validParams = new ErsTimeCorrParams;

 	    validParams->orbit    	 = orbit; // PRQA S 3000
 	    validParams->day	  	 = day; 
 	    validParams->ms 	  	 = ms; 
 	    validParams->BinTime  	 = binTime; 
 	    validParams->ClockPeriod = clockPeriod; 

	    _validParams.push_back(validParams);
	    delete [] fileStream;
	
    	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "returning param :" << validParams->dump()) ;
    	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method executed.") ;
    }
    catch(exception &) {
	    delete [] fileStream;
        throw ;
    }
}


void ErsOBTFileParser::addFileName(const std::string& name)
{
	std::set<std::string>::iterator it = _fileNames.find(name);
	if( it == _fileNames.end() )//add filename only if NOT PRESENT yet
	{
		_fileNames.insert(name);
		_fileNamesVec.push_back(name);
		buildParams();
	}
}
	
void ErsOBTFileParser::removeFileName(const std::string& name)
{
	std::set<std::string>::iterator it = _fileNames.find(name);
	if( it != _fileNames.end() )//remove filename only if previously added
	{
		_fileNames.erase(name);
		std::vector<std::string>::iterator itV = find(_fileNamesVec.begin(),_fileNamesVec.end(),name);
		_fileNamesVec.erase(itV);
		buildParams();
	}
}

void ErsOBTFileParser::parse(std::vector<Parameters *>& params)
{
	params.clear() ;
    size_t p_size = _validParams.size() ;
	for (size_t i=0; i < p_size; ++i) {
		params.push_back(_validParams[i]) ;
    }
}

_ACS_END_NAMESPACE
