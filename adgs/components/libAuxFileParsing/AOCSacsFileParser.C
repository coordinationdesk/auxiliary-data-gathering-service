// PRQA S 1050 EOF
/*

	Copyright 2017-2022, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Processors $

	$Id$

	$Author$

	$Log$
	Revision 1.1  2017/12/21 08:18:55  ricfer
	Added this script to add ACS header to all source/header/makefile files.
	

*/

#include <AOCSacsFileParser.h>
#include <File.h>
#include <XMLIstream.h>
#include <rsResourceSet.h>
#include <DateTime.h>
#include <StringUtils.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(AOCSacsFileParser)

//Class constructor
AOCSacsFileParser::AOCSacsFileParser() :
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
AOCSacsFileParser::AOCSacsFileParser(const string& fileName) :
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
AOCSacsFileParser::~AOCSacsFileParser() throw() 
{
}

//copy constructor
AOCSacsFileParser::AOCSacsFileParser(const AOCSacsFileParser & e) :
    PredictedOrbitFileParser(e),
	_fileNames(e._fileNames),
	_fileNamesVec(e._fileNamesVec),
	_fileType(e._fileType),
	_validParams(e._validParams)
{
}

//operator = 
AOCSacsFileParser&  AOCSacsFileParser::operator=(const AOCSacsFileParser & e)
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


void AOCSacsFileParser::buildParams()
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

void AOCSacsFileParser::parseFile(const string& fileName)
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method called to process file " << fileName) ;
	
	if(!File::exists(fileName))
	{
		ACS_THROW(exFileOpenException("The input Auxiliary File [" + fileName + "] does not exist!")) ; // PRQA S 3081
	}

	rsResourceSet rsFile;
	XMLIstream istr(fileName);
	istr >> rsFile;

	// Read the time system */
	string timeSys ;
	rsFile.getValue("timeSys.SV", timeSys);
	ACS_COND_THROW((timeSys!="TAI" && timeSys!="UTC"), exFileOpenException(" Failed. Reference time system "+timeSys+" not allowed (it must be TAI or UTC).")) ;
	
	// Read UT1-UTC in seconds
	double UT1minusUTC = 0.0 ;
	rsFile.getValue("polarMotion.deltaUT1", UT1minusUTC) ;
	
	// Read TAI-UTC in seconds
	double TAIminusUTC = 0.0 ;
	rsFile.getValue("TAI-UTC", TAIminusUTC) ;
	
	// Read the SVs
	int num ;
	string tmpS ;
	vector<string> tokens ;
	rsFile.getValue("listOfSV.count.value", num) ;
	for(int i=0; i<num; ++i)
	{
		ostringstream rootTag ;
		rootTag << "listOfSV.SV[" << i << "]" ;
		rsFile.getValue(rootTag.str(), tmpS) ;
		tokens.clear() ;
		StringUtils::Tokenize(tmpS, tokens, " ") ;
		
		PredictedOrbitData poData;
		for(size_t s=0; s<tokens.size(); )
		{
			if(timeSys=="TAI")
				poData.utcTime = DateTime::JD(DateTime(tokens[s++].c_str())).jd() - TAIminusUTC/86400.0 ;
			else if(timeSys=="UTC")
				poData.utcTime = DateTime::JD(DateTime(tokens[s++].c_str())).jd() ;
			
			// Set TAI and UT1 times
			poData.taiTime = poData.utcTime + TAIminusUTC/86400.0 ;
			poData.ut1Time = poData.utcTime + UT1minusUTC/86400.0 ;
			
			vector<double> tmpD(3) ;
			for(int k=0; k<3; ++k, ++s)
				istringstream(tokens[s].c_str()) >> tmpD.at(k) ; 
			poData.xPosition = tmpD.at(0)*1000.0 ;
			poData.yPosition = tmpD.at(1)*1000.0 ;
			poData.zPosition = tmpD.at(2)*1000.0 ;
			
			for(int k=0; k<3; ++k, ++s)
				istringstream(tokens[s].c_str()) >> tmpD.at(k) ;
			poData.xVelocity = tmpD.at(0)*1000.0 ;
			poData.yVelocity = tmpD.at(1)*1000.0 ;
			poData.zVelocity = tmpD.at(2)*1000.0 ;
		}
		_validParams.push_back(poData);
		
		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Parsed element ") ;
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, setprecision(17) << ">> " << timeSys << " Time  is : '" <<  ((timeSys=="UTC")? poData.utcTime:poData.taiTime)) ;
			//ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, setprecision(17) << ">> ABS Orbit is : '" <<  poData.absOrbit) ;
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, setprecision(17) << ">> X_POS is     : '" 	<<  poData.xPosition) ;
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, setprecision(17) << ">> Y_POS is     : '" 	<<  poData.yPosition) ;
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, setprecision(17) << ">> Z_POS is     : '" 	<<  poData.zPosition) ;
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, setprecision(17) << ">> X_VEL is     : '" 	<<  poData.xVelocity) ;
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, setprecision(17) << ">> Y_VEL is     : '" 	<<  poData.yVelocity) ;
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, setprecision(17) << ">> Z_VEL is     : '" 	<<  poData.zVelocity) ;
		ACS_CLASS_END_DEBUG
	}

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method executed.") ;
}


void AOCSacsFileParser::addFileName(const string& name)
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

void AOCSacsFileParser::removeFileName(const string& name)
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

void AOCSacsFileParser::parse(std::vector<Parameters *>& params)
{
	params.clear() ;
    size_t p_size = _validParams.size() ;
	for (size_t i=0; i < p_size; ++i) {
		params.push_back(&_validParams[i]) ;
    }
}


_ACS_END_NAMESPACE
