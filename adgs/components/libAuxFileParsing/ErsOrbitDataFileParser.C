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
	Revision 2.4  2014/03/19 19:14:55  marpas
	coding best practices application in progress (not ended)
	warning fixing (qa & g++)
	
	Revision 2.3  2012/04/18 08:15:40  giucas
	compilation warnings fixed
	
	Revision 2.2  2007/01/10 11:34:07  giucas
	Updated TimeConverter lib calls for IF changes (CFI 3.5 porting)
	
	Revision 2.1  2006/04/05 15:15:24  seralb
	O_RDONLY instead of O_RDWR
	
	Revision 2.0  2006/02/28 09:32:51  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/09/30 13:19:03  seralb
	Added setprecision
	
	Revision 1.1  2005/09/16 11:47:30  seralb
	Added class
	
	Revision 1.1  2005/09/12 14:35:25  seralb
	Added class
	

*/

#include <ErsOrbitDataFileParser.h>
#include <ErsTimeCorrParams.h>
#include <File.h>
#include <ConfigurationSingleton.h>
#include <TimeFormat.h>
#include <rsResourceSet.h>
#include <AsciiTime.h>
#include <ProcessingTime.h>
#include <TimeConverter.h>
#include <TimeConverterSingleton.h>
#include <DateTime.h>

#include <cstdio>
#include <cstdlib>
#include <sys/types.h> // PRQA S 1013 2 
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(ErsOrbitDataFileParser)

//Class constructor
ErsOrbitDataFileParser::ErsOrbitDataFileParser() :
    OrbitDataFileParser(),
	_fileNames(),
	_fileType(),
	_fileNamesVec(),
	_validParams()
{
	static char Id[] = "@(#) A.C.S.: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Id: " << Id) ;
}

//Class constructor
ErsOrbitDataFileParser::ErsOrbitDataFileParser(const string& fileName):
    OrbitDataFileParser(),
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
ErsOrbitDataFileParser::~ErsOrbitDataFileParser() throw()
{
	cleanupParams(); // PRQA S 4631
}

//copy constructor
ErsOrbitDataFileParser::ErsOrbitDataFileParser(const ErsOrbitDataFileParser & e):
    OrbitDataFileParser(e),
	_fileNames(e._fileNames),
	_fileType(e._fileType),
	_fileNamesVec(e._fileNamesVec),
	_validParams(e._validParams)
{
}

//operator = 
ErsOrbitDataFileParser&  ErsOrbitDataFileParser::operator=(const ErsOrbitDataFileParser & e)
{
	if (this != &e) 
	{
        OrbitDataFileParser::operator=(e),
		_fileNames	= e._fileNames;
		_fileType	= e._fileType;
		_fileNamesVec	= e._fileNamesVec;
		_validParams	= e._validParams;
	}
	return *this ;
}


void ErsOrbitDataFileParser::cleanupParams()
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
void ErsOrbitDataFileParser::buildParams()
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


void ErsOrbitDataFileParser::parseFile(const string& fileName)
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method called.") ;
	
	// open orbit file
	int fd = open(fileName.c_str(),O_RDONLY);
	if(fd==-1)
	{
		ACS_THROW(exIOException("Error opening " + fileName, errno));
	}
	
	ssize_t sizeOfFile = File::size(fileName);
	unsigned char* fileStream = new unsigned char[sizeOfFile]; // PRQA S 3000 2
	ssize_t ret = read(fd,fileStream,sizeOfFile);
    int eno = errno ;
	if(ret != sizeOfFile )
	{
        delete [] fileStream ;
        close(fd) ;
		ACS_THROW(exIOException("Error reading " + fileName, eno)) ;
	}
	close(fd);
    
    // PRQA S 4400 L2
	try {
	    ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
	    // read and print as int all the file
		    int pos = 0;
            ssize_t sizeq = sizeOfFile/4 ;
		    for(ssize_t i=0; i<=sizeq; ++i)
		    {	
			    unsigned int tmp = fileStream[pos];	
			    tmp  = (tmp << 8) | fileStream[pos+1];
			    tmp  = (tmp << 8) | fileStream[pos+2];
			    tmp  = (tmp << 8) | fileStream[pos+3];
			    ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "tmp = " << tmp << " at byte " << i*4) ;
			    pos += 4; 
		    }
	    ACS_CLASS_END_DEBUG

	    ErsTimeCorrParams* validParams = new ErsTimeCorrParams;

	    TimeConverter* converter = TimeConverterSingleton::Instance()->getConverter();

	    // Positioning Ers input stream	
	    int filePos = (48) ;

	    /* utc epoch of sat position */
	    char tmpBuff[25]; // PRQA S 4403
	    memcpy(tmpBuff,&fileStream[filePos],25); 
	    string ascNodeUtc(tmpBuff);
	    AsciiTime aT(ascNodeUtc);
	    aT.setRef(TimeFormat::TC_TIME_UTC);
	    aT.setFormat(TimeFormat::TC_ASCII_ERS_MILLISEC);
	    ProcessingTime pT;
	    pT.setRef(TimeFormat::TC_TIME_UTC);
	    pT.setFormat(TimeFormat::TC_PROC);
	    converter->convert(aT,pT);	
	    long double aNu = pT.getValue()+ DateTime::MJD2000::_jd502mjd2000_OFFSET;	
	    filePos += 25; 

	    /* utc reference time */        
	    char tmpBuff2[25]; // PRQA S 4403
	    memcpy(tmpBuff2,&fileStream[filePos],25); 
	    string referenceUtc(tmpBuff2);
	    AsciiTime aT2(referenceUtc);
	    aT2.setRef(TimeFormat::TC_TIME_UTC);
	    aT2.setFormat(TimeFormat::TC_ASCII_ERS_MILLISEC);
	    ProcessingTime pT2;
	    pT2.setRef(TimeFormat::TC_TIME_UTC);
	    pT2.setFormat(TimeFormat::TC_PROC);
	    converter->convert(aT2,pT2);
	    long double rU = pT2.getValue()+ DateTime::MJD2000::_jd502mjd2000_OFFSET;	
	    filePos += 27; 

	    /* satellite reference binary time */
	    unsigned long satBinTime = fileStream[filePos];	
	    satBinTime  = ( satBinTime << 8 ) | fileStream[filePos+1];
	    satBinTime  = ( satBinTime << 8 ) | fileStream[filePos+2];
	    satBinTime  = ( satBinTime << 8 ) | fileStream[filePos+3];
	    filePos += 4; 

	    /* step length of clock in nanoseconds */
	    unsigned long clockStepLength = fileStream[filePos];	
	    clockStepLength  = ( clockStepLength << 8 ) | fileStream[filePos+1];
	    clockStepLength  = ( clockStepLength << 8 ) | fileStream[filePos+2];
	    clockStepLength  = ( clockStepLength << 8 ) | fileStream[filePos+3];

	    ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
		    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Ers PARSED VALUES ARE :") ;
		    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "ascNodeUtc		= '" << ascNodeUtc << "'") ;
 		    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "ascNodeUtc		= '" << setprecision(30) << aNu	<< "'") ;
		    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "referenceUtc	= '" << referenceUtc << "'") ;
		    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "referenceUtc	= '" << setprecision(30) << rU << "'") ;
		    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "satBinTime		= '" << satBinTime << "'") ;
		    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "clockStepLength= '" << clockStepLength << "'") ;
	    ACS_CLASS_END_DEBUG

 	    // set parameters
	    validParams->AscNodeUTC       = aNu;   
 	    validParams->ReferenceUTC     = rU; 
 	    validParams->SatBinTime       = satBinTime;   
 	    validParams->ClockStepLength  = clockStepLength;

	    _validParams.push_back(validParams);
	    delete [] fileStream;

	    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method executed.") ;
    } // PRQA L:L2
    catch(exception &) {
	    delete [] fileStream;
        throw ;
    }
}


void ErsOrbitDataFileParser::addFileName(const string& name)
{
	set<string>::iterator it = _fileNames.find(name);
	if( it == _fileNames.end() )//add filename only if NOT PRESENT yet
	{
		_fileNames.insert(name);
		_fileNamesVec.push_back(name);
		buildParams();
	}
}
	
void ErsOrbitDataFileParser::removeFileName(const string& name)
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


void ErsOrbitDataFileParser::parse(std::vector<Parameters *>& params)
{
	params.clear() ;
    size_t p_size = _validParams.size() ;
	for (size_t i=0; i < p_size; ++i) {
		params.push_back(_validParams[i]) ;
    }
}

_ACS_END_NAMESPACE
