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
	Revision 2.5  2014/03/19 19:14:55  marpas
	coding best practices application in progress (not ended)
	warning fixing (qa & g++)
	
	Revision 2.4  2012/04/18 08:15:40  giucas
	compilation warnings fixed
	
	Revision 2.3  2008/03/06 09:12:54  giucas
	Fixed bug in getJdFromDefinitiveTime
	
	Revision 2.2  2008/02/28 13:47:44  giucas
	The utcTime in PredictedOrbitData objects are expressed in JD50
	
	Revision 2.1  2007/12/03 10:16:49  giucas
	L5DefinitiveFileParser added
	

*/

#include <L5DefinitiveFileParser.h>
#include <File.h>
#include <DateTime.h>
#include <StringUtils.h>
#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(L5DefinitiveFileParser)

//Class constructor
L5DefinitiveFileParser::L5DefinitiveFileParser() :
    PredictedOrbitFileParser(),
	_fileNames(),
	_fileType(),
	_fileNamesVec(),
	_validParams()
{
	static char Id[] = "@(#) A.C.S.: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Id: " << Id) ;
}

//Class constructor
L5DefinitiveFileParser::L5DefinitiveFileParser(const string& fileName)  :
    PredictedOrbitFileParser(),
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
L5DefinitiveFileParser::~L5DefinitiveFileParser() throw() 
{
}

//copy constructor
L5DefinitiveFileParser::L5DefinitiveFileParser(const L5DefinitiveFileParser & e) :
    PredictedOrbitFileParser(e),
	_fileNames(e._fileNames),
	_fileType(e._fileType),
	_fileNamesVec(e._fileNamesVec),
	_validParams(e._validParams)
{
}

//operator = 
L5DefinitiveFileParser&  L5DefinitiveFileParser::operator=(const L5DefinitiveFileParser & e)
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

void L5DefinitiveFileParser::buildParams()
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


void L5DefinitiveFileParser::parseFile(const string& fileName)
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"method called to process file " << fileName) ;
	
	if(!File::exists(fileName))
	{
		ACS_THROW(exFileOpenException("Auxiliary File [" + fileName + "] does not exist")) ; // PRQA S 3081
	}

	ifstream is(fileName.c_str());
	//read the first 3 lines
	string tmpLine("");
	unsigned int i=1;
	while( i < 4 ) // PRQA S 4400
	{
        ostringstream os ;
        os << "Unexpected eof at line " << i ;
		ACS_COND_THROW( is.eof(), exIllegalValueException(os.str()) ) ; // PRQA S 3081
		getline(is,tmpLine) ;
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Skipping line [" << tmpLine << "]") ;
		i++ ;
	}

	while( addElement(&is) )
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"parsing element n." << i) ;
		i++ ;
	}

	is.close() ;

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"method executed.") ;
}

bool L5DefinitiveFileParser::addElement(ifstream* is) // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"method called.") ;
	
	//read data
	string tmpLine="";
	getline(*is,tmpLine) ;
	if( is->eof() ) {
		return false ;
    }
	
	PredictedOrbitData poData;
	
	vector<string> tokens;
	StringUtils::Tokenize(tmpLine,tokens,",") ;
	if( tokens.size() != 7 ) // PRQA S 4400
	{
		ACS_LOG_WARNING("Problems converting line ["+tmpLine+"] -> Skipping it...") ;
		return true ;
	}

	//fill data
	poData.utcTime = getJDFromDefinitiveTime(tokens[0]);
	poData.xPosition = atof( (tokens[1]).c_str() ) * 1000.0; // PRQA S 4400 7
	poData.yPosition = atof( (tokens[2]).c_str() ) * 1000.0;
	poData.zPosition = atof( (tokens[3]).c_str() ) * 1000.0;
	poData.xVelocity = atof( (tokens[4]).c_str() ) * 1000.0;
	poData.yVelocity = atof( (tokens[5]).c_str() ) * 1000.0;
	poData.zVelocity = atof( (tokens[6]).c_str() ) * 1000.0;

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Parsed element: ") ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,setprecision(17) <<">> UTC Time  is : '" << setprecision(17) <<  poData.utcTime << "'") ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,setprecision(17) <<">> ABS Orbit is : '" <<  poData.absOrbit << "'") ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,setprecision(17) <<">> X_POS is : '" 	<<  poData.xPosition << "'") ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,setprecision(17) <<">> Y_POS is : '" 	<<  poData.yPosition << "'") ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,setprecision(17) <<">> Z_POS is : '" 	<<  poData.zPosition << "'") ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,setprecision(17) <<">> X_VEL is : '" 	<<  poData.xVelocity << "'") ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,setprecision(17) <<">> Y_VEL is : '"     <<  poData.yVelocity << "'") ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,setprecision(17) <<">> Z_VEL is : '" 	<<  poData.zVelocity << "'") ;;
	ACS_CLASS_END_DEBUG
	
	_validParams.push_back(poData);
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"method executed.") ;

	return true ;
}


long double L5DefinitiveFileParser::getJDFromDefinitiveTime(const string& refTime) const // PRQA S 4020
{
	//refTime is of the form : 153/2006 00:00:00.000
	vector<string> tokens;
	StringUtils::Tokenize(refTime,tokens," ") ;
	if( tokens.size() != 2 ) // PRQA S 4400
	{
		ACS_LOG_WARNING("Problems converting time ["+refTime+"] -> Returning 0") ;
		return 0.0 ;
	}
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Parsed element " << refTime) ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"[ tokens[0]  is : '" <<  tokens[0] << "']") ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"[ tokens[1]  is : '" <<  tokens[1] << "']") ; 
	ACS_CLASS_END_DEBUG
	//extract DayOfYear/Year
	vector<string> tokens2;
	StringUtils::Tokenize(tokens[0],tokens2,"/") ;
	if( tokens2.size() != 2 ) // PRQA S 4400
	{
		ACS_LOG_WARNING("Problems converting DayOfYear/Year ["+tokens[0]+"] -> Returning 0") ;
		return 0.0 ;
	}
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Parsed element " << tokens[0]) ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"[ tokens2[0]  is : '" <<  tokens2[0] << "']") ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"[ tokens2[1]  is : '" <<  tokens2[1] << "']") ; 
	ACS_CLASS_END_DEBUG
	
	unsigned short year = atoi(tokens2[1].c_str()) ; // PRQA S 3000, 3010 5
	unsigned short hour = atoi(tokens[1].substr(0,2).c_str()) ;
	unsigned short min = atoi(tokens[1].substr(3,2).c_str()) ;
	unsigned short sec = atoi(tokens[1].substr(6,2).c_str()) ;
	unsigned short msec = atoi(tokens[1].substr(9,3).c_str()) ;

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"Time is ") ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"[ Year  is : '" <<  year << "']") ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"[ Hour  is : '" << hour  << "']") ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"[ Min  is : '" << min  << "']") ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"[ Sec  is : '" <<  sec << "']") ;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB,"[ Millisec  is : '" << msec  << "']") ; 
	ACS_CLASS_END_DEBUG

	return DateTime(DateTime::UTCD(year,1,1,hour,min,sec,msec)).jd50() + atof(tokens2[0].c_str()) - 1.0; // PRQA S 3081
	
}

	
void L5DefinitiveFileParser::addFileName(const string& name)
{
	
	set<string>::iterator it = _fileNames.find(name);
	if( it == _fileNames.end() )//add filename only if NOT PRESENT yet
	{
		_fileNames.insert(name);
		_fileNamesVec.push_back(name);
		buildParams();
	}
}

void L5DefinitiveFileParser::removeFileName(const string& name)
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


void L5DefinitiveFileParser::parse(vector<Parameters *>& params)
{
	params.clear() ;
	size_t vp_size = _validParams.size(); 
    for (size_t i=0; i < vp_size; ++i) {
		params.push_back(&_validParams[i]) ;
    }
}

_ACS_END_NAMESPACE

