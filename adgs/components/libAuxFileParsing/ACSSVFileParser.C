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
	Revision 1.5  2014/03/19 19:14:54  marpas
	coding best practices application in progress (not ended)
	warning fixing (qa & g++)
	
	Revision 1.4  2012/04/18 08:15:39  giucas
	compilation warnings fixed
	
	Revision 1.3  2006/09/19 13:35:14  crivig
	bug fixed in time conversion routine: DateTime used
	
	Revision 1.2  2006/07/10 13:56:04  giucas
	ParseFile method implemented
	
	Revision 1.1  2006/07/10 12:15:35  giucas
	First issue. Still to be implemented
	


*/

#include <ACSSVFileParser.h>
#include <File.h>
#include <DateTime.h>
#include <boost/tokenizer.hpp> // PRQA S 1013

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(ACSSVFileParser)

//Class constructor
ACSSVFileParser::ACSSVFileParser() :
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
ACSSVFileParser::ACSSVFileParser(const string& fileName) :
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
ACSSVFileParser::~ACSSVFileParser() throw()  
{
}

//copy constructor
ACSSVFileParser::ACSSVFileParser(const ACSSVFileParser & e):
    PredictedOrbitFileParser(e),
	_fileNames(e._fileNames),
	_fileType(e._fileType),
	_fileNamesVec(e._fileNamesVec),
	_validParams(e._validParams)
{
}

//operator = 
ACSSVFileParser&  ACSSVFileParser::operator=(const ACSSVFileParser & e)
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


void ACSSVFileParser::buildParams()
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


void ACSSVFileParser::parseFile(const string& fileName)
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method called to process file " << fileName) ;
	
	if(!File::exists(fileName))
	{
		ACS_THROW(exFileOpenException("File [" + fileName + "] does not exist")) ; // PRQA S 3081
	}

	ifstream is(fileName.c_str());
	unsigned int i=1;
	while( addElement(&is) )
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "parsing element n." << i) ;
        ++ i ;
	}

	is.close() ;

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method executed.") ;
}

bool ACSSVFileParser::addElement(ifstream* is) // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method called.") ;

	//read time
	string utcTime="" ;
	getline(*is,utcTime) ;
	if( is->eof() ) {
		return false ;
    }
	
	//read data
	string tmpLine="";
	getline(*is,tmpLine) ;
	if( is->eof() ) {
		return false ;
    }
	
	PredictedOrbitData poData;
	
	//fill data
	poData.utcTime = getJDFromCCSDSAMicrosec(utcTime);

	typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
	boost::char_separator<char> sep(" ");
	tokenizer tokens(tmpLine, sep);
	unsigned int tokenNum = 1 ;
	for (tokenizer::iterator tok_iter = tokens.begin();
	   tok_iter != tokens.end(); tok_iter++)
	{	
		switch( tokenNum )
		{
		case 1: 
		    {
			    poData.xPosition = atof((*tok_iter).c_str());
			    tokenNum++;
			    break ;
		    }
		case 2: // PRQA S 4402
		    {
			    poData.yPosition = atof((*tok_iter).c_str());
			    tokenNum++;
			    break ;
		    }
		case 3: // PRQA S 4402 
		    {
			    poData.zPosition = atof((*tok_iter).c_str());
			    tokenNum++;
			    break ;
		    }
		case 4:  // PRQA S 4402
		    {
			    poData.xVelocity = atof((*tok_iter).c_str());
			    tokenNum++;
			    break ;
		    }
		case 5: // PRQA S 4402 
		    {
			    poData.yVelocity = atof((*tok_iter).c_str());
			    tokenNum++;
			    break ;
		    }
		case 6: // PRQA S 4402 
		    {
			    poData.zVelocity = atof((*tok_iter).c_str());
			    tokenNum++;
			    break ;
		    }
		}
	}
	
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Parsed element ") ;
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, setprecision(17) << ">> UTC Time  is : '" <<  poData.utcTime) ;
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, setprecision(17) << ">> ABS Orbit is : '" <<  poData.absOrbit) ;
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, setprecision(17) << ">> X_POS is : '" 	<<  poData.xPosition) ;
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, setprecision(17) << ">> Y_POS is : '" 	<<  poData.yPosition) ;
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, setprecision(17) << ">> Z_POS is : '" 	<<  poData.zPosition) ;
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, setprecision(17) << ">> X_VEL is : '" 	<<  poData.xVelocity) ;
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, setprecision(17) << ">> Y_VEL is : '" 	<<  poData.yVelocity) ;
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, setprecision(17) << ">> Z_VEL is : '" 	<<  poData.zVelocity) ;
	ACS_CLASS_END_DEBUG
	
	_validParams.push_back(poData);
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method executed.");

	return true ;
}


long double ACSSVFileParser::getJDFromCCSDSAMicrosec(const string& refTime) const
{
	//refTime is of the form : UTC=YYYYMMDDTHHNNSSUUUUUU (TimeFormat::TC_ASCII_CCSDSA_MICROSEC)
	return DateTime::JD(DateTime(refTime)).jd(); // PRQA S 3050, 3081
}

	
void ACSSVFileParser::addFileName(const string& name)
{
	
	set<string>::iterator it = _fileNames.find(name);
	if( it == _fileNames.end() )//add filename only if NOT PRESENT yet
	{
		_fileNames.insert(name);
		_fileNamesVec.push_back(name);
		buildParams();
	}
}

void ACSSVFileParser::removeFileName(const string& name)
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


void ACSSVFileParser::parse(std::vector<Parameters *>& params)
{
	params.clear() ;
    size_t p_size = _validParams.size() ;
	for (size_t i=0; i < p_size; ++i) {
		params.push_back(&_validParams[i]) ;
    }
}


_ACS_END_NAMESPACE
