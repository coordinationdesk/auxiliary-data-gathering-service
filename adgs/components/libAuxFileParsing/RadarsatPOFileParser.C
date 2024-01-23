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
	Revision 1.4  2014/03/19 19:14:56  marpas
	coding best practices application in progress (not ended)
	warning fixing (qa & g++)
	
	Revision 1.3  2012/04/18 08:15:40  giucas
	compilation warnings fixed
	
	Revision 1.2  2009/03/04 15:56:02  giucas
	Velocities fixed
	
	Revision 1.1  2009/02/25 14:27:43  giucas
	First issue
	

*/

#include <RadarsatPOFileParser.h>
#include <File.h>
#include <DateTime.h>
#include <RegExp.h>
#include <StringUtils.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(RadarsatPOFileParser)

const string RadarsatPOFileParser::FIRST_LINE_REG_EXPR = "^[0-9]{4}-[0-9]{3}-[0-9]{2}:[0-9]{2}:[0-9]{2}.[0-9]{3}$" ;
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
RadarsatPOFileParser::RadarsatPOFileParser()  :
    PredictedOrbitFileParser(),
	_fileNames(),
	_fileType(),
	_fileNamesVec(),
	_validParams()
{
	static char Id[] = "@(#) A.C.S.: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Id: " << Id) ;
}

//Class constructor
RadarsatPOFileParser::RadarsatPOFileParser(const string& fileName) :
    PredictedOrbitFileParser(),
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
RadarsatPOFileParser::~RadarsatPOFileParser() throw()
{
}

//copy constructor
RadarsatPOFileParser::RadarsatPOFileParser(const RadarsatPOFileParser & e) :
    PredictedOrbitFileParser(e),
	_fileNames(e._fileNames),
	_fileType(e._fileType),
	_fileNamesVec(e._fileNamesVec),
	_validParams(e._validParams)
{
}

//operator = 
RadarsatPOFileParser&  RadarsatPOFileParser::operator=(const RadarsatPOFileParser & e)
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

void RadarsatPOFileParser::buildParams()
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

void RadarsatPOFileParser::parseFile(const string& fileName)
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method called to process file " << fileName) ;
	
	if(!File::exists(fileName))
	{
		ACS_THROW(exFileOpenException("The input Auxiliary File [" + fileName + "] does not exist!")) ; // PRQA S 3081
	}
	
	ifstream is(fileName.c_str());
	unsigned int lineIndex=0 ;
	int linesToExtract=0;
	vector<string> tmpValidLines;
	bool startLineFound=false;
	
	RegExp re(FIRST_LINE_REG_EXPR) ;
	long int absOrbit = 0 ;
	string tmpLine ;
    string tempOutputLine ;
	while( getline(is,tmpLine) ) // PRQA S 3050
	{
		lineIndex++;
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "got line number " << lineIndex << " : [" << tmpLine << "]") ;
		if( ! tmpLine.empty() )
		{
			if( tmpLine.find("ORBIT_NUMBER") != string::npos )
			{
				vector<string> tokens;
				StringUtils::Tokenize(tmpLine,tokens," ") ;
				if( tokens.size() != 3 ) // PRQA S 4400
				{
					ACS_LOG_WARNING("Problems Parsing ORBIT_NUMBER line ["+tmpLine+"] -> Skipping it ") ;
					ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
						ostringstream msg ;
						msg << "RadarsatPOFileParser::parseFile : tmpLine [" << tmpLine << "] tokens are [" ;
                        size_t t_s = tokens.size() ;
						for(unsigned int j=0; j<t_s; ++j)
						{
							msg	<< "'" << tokens[j] << "' ";
						}
						msg << "]" ;
						ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, msg.str()) ;
					ACS_CLASS_END_DEBUG
				}
				absOrbit = atol(tokens[2].c_str()) ; // PRQA S 4400
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Extraccted ORBIT_NUMBER is " << absOrbit) ;
			}
			else if( linesToExtract > 0 )
			{
				tempOutputLine += " ";
				tempOutputLine += tmpLine ;
				linesToExtract-- ;
			}
			else if( re.match(tmpLine) )
			{
				startLineFound = true ;
				linesToExtract = 2 ; // PRQA S 4400
				tempOutputLine += tmpLine ;
			}
			else if( startLineFound && (linesToExtract == 0))
			{
				tmpValidLines.push_back(tempOutputLine);
				startLineFound = false ;
				tempOutputLine = "" ;
			}
            else { /* nothing */ }

			//reset tmpLine
			tmpLine="";
		}
	}

	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Dump tmpValidLines [" << tmpValidLines.size() << " elements] : ") ;
        size_t t_s = tmpValidLines.size() ;
		for(unsigned int i=0; i<t_s; ++i)
		{
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Dump tmpValidLines line n." << i+1 << " : '" << tmpValidLines[i] << "'") ; // PRQA S 3084
		}
	ACS_CLASS_END_DEBUG

	set<PredictedOrbitData,ltTimeParams> validSetParams;
	
    size_t vl_size = tmpValidLines.size() ;
	for(unsigned int i=0; i<vl_size; ++i)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Parsing tmpValidLines line n." << i+1 << " : '"  // PRQA S 3084
				<< tmpValidLines[i] << "' of size " << tmpValidLines[i].size()) ;
		
		vector<string> tokens;
		StringUtils::Tokenize(tmpValidLines[i],tokens," ") ;
		if( tokens.size() != 7 ) // PRQA S 4400
		{
			ACS_LOG_WARNING("Problems Parsing valid line ["+tmpValidLines[i]+"] -> Skipping it ") ;
			ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
				ostringstream msg ;
				msg << "tmpValidLine [" << tmpValidLines[i] << "] tokens are [" ;
                size_t t_s = tokens.size() ;
				for(unsigned int j=0; j<t_s; ++j)
				{
					msg	<< "'" << tokens[j] << "' ";
				}
				msg << "]" ;
				ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, msg.str()) ;
			ACS_CLASS_END_DEBUG
			continue;
		}
		
		PredictedOrbitData poData;
		
		poData.utcTime = getJDFromOrbitDataTime(tokens[0]);		
		poData.absOrbit = absOrbit;
		
		poData.xPosition = strtod(tokens[1].c_str(),0) ; // PRQA S 4400 7
		poData.yPosition = strtod(tokens[2].c_str(),0) ;
		poData.zPosition = strtod(tokens[3].c_str(),0) ;

		poData.xVelocity = strtod(tokens[4].c_str(),0)/1000.0 ;
		poData.yVelocity = strtod(tokens[5].c_str(),0)/1000.0 ;
		poData.zVelocity = strtod(tokens[6].c_str(),0)/1000.0 ;
		
		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Parsing tmpValidLines line n." << i+1) ; // PRQA S 3084
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, ">> UTC Time  is : " << setprecision(17) << poData.utcTime) ; // PRQA S 4400 8
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, ">> ABS Orbit is : " << setprecision(17) << poData.absOrbit) ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, ">> X_POS is : " 	<< setprecision(17) << poData.xPosition) ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, ">> Y_POS is : " 	<< setprecision(17) << poData.yPosition) ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, ">> Z_POS is : " 	<< setprecision(17) << poData.zPosition) ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, ">> X_VEL is : " 	<< setprecision(17) << poData.xVelocity) ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, ">> Y_VEL is : " 	<< setprecision(17) << poData.yVelocity) ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, ">> Z_VEL is : " 	<< setprecision(17) << poData.zVelocity) ;
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

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method executed.") ;
}


long double RadarsatPOFileParser::getJDFromOrbitDataTime(const string& refTime) const // PRQA S 4020
{
	//refTime is of the form : 1996-225-22:42:01.933
	// i.e. YYYY-DOY-HH:MM::SS.NNN
	if( refTime.size() != 21 ) // PRQA S 4400
	{
		ACS_LOG_WARNING("Unable to compute time from input ["+refTime+"] -> Setting to 0.0") ;
		return 0.0 ;
	}
	
	unsigned short year = atoi(refTime.substr(0,4).c_str()); // PRQA S 3000, 3010, 4400 7
	unsigned short doy  = atoi(refTime.substr(5,3).c_str());
	unsigned short hour = atoi(refTime.substr(9,2).c_str());
	unsigned short min = atoi(refTime.substr(12,2).c_str());
	unsigned short sec = atoi(refTime.substr(15,2).c_str());
	unsigned short msec = atol(refTime.substr(18,3).c_str());

	unsigned short month = 0 ;
    unsigned short day = 0 ;		
	DateTime::UTCD::dayAndMonthFromDOY(doy,year,day,month) ;
	
	long double result = DateTime::JD(DateTime(DateTime::UTCD(year,month,day,hour,min,sec,msec))).jd() ; // PRQA S 3050, 3081
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "year is "<< year ) ;
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "month is "<< month ) ;
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "day is "<< day ) ;
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "hour is "<< hour ) ;
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "min is "<< min ) ;
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "sec is "<< sec ) ;
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "msec is "<< msec ) ;
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "time "<< refTime << " converted in JD is " << setprecision(17) << result ) ;
	ACS_CLASS_END_DEBUG
	return result ;
}

void RadarsatPOFileParser::addFileName(const string& name)
{
	
	set<string>::iterator it = _fileNames.find(name);
	if( it == _fileNames.end() )//add filename only if NOT PRESENT yet
	{
		_fileNames.insert(name);
		_fileNamesVec.push_back(name);
		buildParams();
	}
}

void RadarsatPOFileParser::removeFileName(const string& name)
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

void RadarsatPOFileParser::parse(std::vector<Parameters *>& params)
{
	params.clear() ;
    size_t p_size = _validParams.size() ;
	for (size_t i=0; i < p_size; ++i) {
		params.push_back(&_validParams[i]) ;
    }
}


_ACS_END_NAMESPACE

