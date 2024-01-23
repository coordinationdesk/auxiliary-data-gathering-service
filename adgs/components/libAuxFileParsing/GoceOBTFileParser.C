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
	Revision 2.3  2018/09/27 13:53:07  enrcar
	EC:: missing include fixed
	
	Revision 2.2  2014/03/19 19:14:55  marpas
	coding best practices application in progress (not ended)
	warning fixing (qa & g++)
	
	Revision 2.1  2012/04/18 08:15:40  giucas
	compilation warnings fixed
	
	Revision 2.0  2006/02/28 09:32:51  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2005/02/21 14:43:53  giucas
	Imported libAuxFileParsing
	
	Revision 1.1  2004/12/07 10:09:24  giucas
	Moved from libTimeConverter
	
	Revision 1.8  2004/12/03 09:33:26  giucas
	Header fixed
	
	Revision 1.7  2004/12/03 08:57:21  giucas
	Added debug printouts
	
	Revision 1.6  2004/11/16 08:53:34  giucas
	Added set of file name
	
	Revision 1.5  2004/11/15 15:15:40  giucas
	Added debug print
	
	Revision 1.4  2004/11/15 13:31:56  giucas
	Changed FileParser interface to handle multi-file initialization
	
	Revision 1.3  2004/10/19 12:24:15  giucas
	Changed algo to extract parameters from file
	
	Revision 1.2  2004/10/12 07:41:49  giucas
	Added getFileName and getFileType methods
	
	Revision 1.1  2004/09/16 16:57:17  giucas
	First release
	


*/

#include <GoceOBTFileParser.h>
#include <File.h>
#include <ConfigurationSingleton.h>
#include <TimeFormat.h>
#include <CFGOTimeCorrParams.h>
#include <vector>
#include <algorithm>

#include <algorithm>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(GoceOBTFileParser)

//Class constructor
GoceOBTFileParser::GoceOBTFileParser() :
    OBTFileParser(),
	_rsConf(ConfigurationSingleton::instance()->get()),
	_fileNames(),
	_fileNamesVec(),
	_fileType(),
	_firstValidLineIndex(0),
	_validityFlagValue(),
	_validLineElements(0),
	_validParams()
{
	static char Id[] = "@(#) A.C.S.: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Id: " << Id) ;
	loadConfiguration();
}

//Class constructor
GoceOBTFileParser::GoceOBTFileParser(const string& fileName) :
    OBTFileParser(),
	_rsConf(ConfigurationSingleton::instance()->get()),
	_fileNames(),
	_fileNamesVec(),
	_fileType(),
	_firstValidLineIndex(0),
	_validityFlagValue(),
	_validLineElements(0),
	_validParams()
{
	static char Id[] = "@(#) A.C.S.: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Id: " << Id) ;
	loadConfiguration();
	_fileNames.insert(fileName);
	_fileNamesVec.push_back(fileName);
	buildParams();
} 

//Class destructor
GoceOBTFileParser::~GoceOBTFileParser() throw()  
{
	cleanupParams(); // PRQA S 4631
}

//copy constructor
GoceOBTFileParser::GoceOBTFileParser(const GoceOBTFileParser & e):
    OBTFileParser(e),
	_rsConf(ConfigurationSingleton::instance()->get()),
	_fileNames(e._fileNames),
	_fileNamesVec(e._fileNamesVec),
	_fileType(e._fileType),
	_firstValidLineIndex(e._firstValidLineIndex),
	_validityFlagValue(e._validityFlagValue),
	_validLineElements(e._validLineElements),
	_validParams(e._validLineElements)
{
}

//operator = 
GoceOBTFileParser&  GoceOBTFileParser::operator=(const GoceOBTFileParser & e)
{
	if (this != &e) 
	{
        OBTFileParser::operator=(e) ;
		_rsConf					= e._rsConf;
		_fileNames				= e._fileNames;
		_fileNamesVec			= e._fileNamesVec;
		_fileType				= e._fileType;
		_firstValidLineIndex	= e._firstValidLineIndex;
		_validityFlagValue		= e._validityFlagValue;
		_validLineElements		= e._validLineElements;
		_validParams			= e._validParams;
	}
	return *this ;
}


//parse each file in _fileNames set and add the valid parameters to _validParams vector.
//Since _fileNames is ordered by validity start/stop times, the _validParams are themselves ordered by time
void GoceOBTFileParser::buildParams()
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

void GoceOBTFileParser::parseFile(const string& fileName)
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method called to process file " << fileName) ;
	
	if(!File::exists(fileName))
	{
		ACS_THROW(exFileOpenException("The input Auxiliary File [" + fileName + "] does not exist!")) ; // PRQA S 3081
	}

	ifstream is(fileName.c_str());
	string tmpLine ;
	unsigned int lineIndex=0;
	vector<string> tmpValidLines;
	bool saveLine=false;
	while( getline(is,tmpLine) ) // PRQA S 3050
	{
		lineIndex++;
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "got line number " << lineIndex << " : [" << tmpLine << "]" ) ;
		if(lineIndex == _firstValidLineIndex)
		{
			saveLine = true;
		}
		
		if( saveLine && (tmpLine != "") ) {
			tmpValidLines.push_back(tmpLine);
        }
		
		//reset tmpLine
		tmpLine="";
	}

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Dump tmpValidLines [" << tmpValidLines.size() << " elements] : ") ;
        size_t tv_size = tmpValidLines.size() ;
		for(unsigned int i=0; i<tv_size; ++i)
		{
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Dump tmpValidLines line n." << i+1 << " : '" << tmpValidLines[i] << "'") ; // PRQA S 3084
		}
	ACS_CLASS_END_DEBUG
	
	//extract the VALIDITY flag to build the valid lines sequence
    size_t tv_size = tmpValidLines.size() ;
	for(unsigned int i=0; i<tv_size; ++i)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Parsing tmpValidLines line n." << i+1 << " : '"  // PRQA S 3084
				<< tmpValidLines[i] << "'") ;
		
		CFGOTimeCorrParams* param = new CFGOTimeCorrParams;
		if( extractParameters(tmpValidLines[i], *param) )
		{
			param->fileName = File::getFileName(fileName);
			_validParams.push_back( param );
			
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Stored object : " <<  param->dump()) ;
		}
		else
		{
			delete param;
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "The line '" << tmpValidLines[i] << "' is skipped because INVALID") ;
		}		
	}
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method executed.") ;
}

//etract the parameters from the valid line in input and store them in CFGOTimeCorrParams object
bool GoceOBTFileParser::extractParameters(const string& validLine, CFGOTimeCorrParams& param) const // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method called to extract parameters from line '" 
			   << validLine << "'") ;
	
	vector<string> result;
	size_t currPos=0 ;
	ssize_t prevPos=-1;	
	while( (currPos = validLine.find('\t', prevPos+1) ) != string::npos ) // PRQA S 3000
	{
		result.push_back(validLine.substr(prevPos+1,currPos-prevPos-1)); // PRQA S 3000, 3084
		prevPos = currPos; // PRQA S 3000
	}
	result.push_back(validLine.substr(prevPos+1,validLine.size())); // PRQA S 3000
	
	if( result.size() != _validLineElements)
	{
		ostringstream msg;
		msg << "Error : Found " << result.size() << " elements for line '" << validLine << "' -> Expected " << _validLineElements;
		ACS_THROW(OBTFileParserException(msg.str())) ; // PRQA S 3081
	}
	
	if(result[7] != _validityFlagValue) { // PRQA S 4400
		return false;
    }
	
	istringstream utcTime(result[0]);
	utcTime >> param.utcTime;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Adding value : " <<  result[0]) ;

	istringstream utc0Coarse(result[1]);
	utc0Coarse >> param.utc0Coarse;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Adding value : " <<  result[1]) ;
	
	istringstream utc0Fine(result[2]);
	utc0Fine >> param.utc0Fine;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Adding value : " <<  result[2]) ;
	
	istringstream obt0Coarse(result[3]);
	obt0Coarse >> param.obt0Coarse;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Adding value : " <<  result[3]) ;
	
	istringstream obt0Fine(result[4]);
	obt0Fine >> param.obt0Fine;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Adding value : " <<  result[4]) ;
	
	istringstream gradient(result[5]);
	gradient >> param.gradient;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Adding value : " <<  result[5]) ;
	
	istringstream offset(result[6]);
	offset >> param.offset;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Adding value : " <<  result[7]) ;

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "CFGOTimeCorrParams object build : " <<  param.dump()) ;
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method executed.") ;

	return true;
}

void GoceOBTFileParser::cleanupParams()
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method called.") ;
	vector<CFGOTimeCorrParams*>::iterator parIt = _validParams.begin();
	while( parIt != _validParams.end() )
	{
		delete *parIt;
		*parIt=0;
		parIt = _validParams.erase(parIt);
	}
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method executed.") ;
}

void GoceOBTFileParser::loadConfiguration()
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method called.") ;
	unsigned int n=0;
#warning MP: use getArray instead	
	_rsConf.getValue("ExplorerWrapper.List_of_Satellites.count.value",n);
	
	ACS_COND_THROW(n == 0,OBTFileParserException("no satellites in configuration")) ; // PRQA S 3081

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, n << " satellites configured") ;

	bool infoSet=false;
	for(unsigned int i=0; i<n; i++)
	{
		ostringstream num;
		num << i;
		int rsId=0;
		_rsConf.getValue("ExplorerWrapper.List_of_Satellites.Satellite["+num.str()+"].Id",rsId);
		if(rsId == static_cast<int>(TimeFormat::TC_SAT_GOCE))//got the rigth satellite : load data // PRQA S 3081
		{
			//load the UTC/SBT TimeConversion File data
			_rsConf.getValue("ExplorerWrapper.List_of_Satellites.Satellite["+num.str()+"].TimeConversionFile.FileType",_fileType);

			_rsConf.getValue("ExplorerWrapper.List_of_Satellites.Satellite["+num.str()+"].TimeConversionFile.FirstValidLineIndex",
						     _firstValidLineIndex);
			
			_rsConf.getValue("ExplorerWrapper.List_of_Satellites.Satellite["+num.str()+"].TimeConversionFile.ValidityField",
						     _validityFlagValue);

			_rsConf.getValue("ExplorerWrapper.List_of_Satellites.Satellite["+num.str()+"].TimeConversionFile.ValidLineElements",
						     _validLineElements);

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "FirstValidLineIndex [" << _firstValidLineIndex << "]") ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ValidityField [" << _validityFlagValue << "]") ;
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "ValidLineElements [" << _validLineElements << "]") ;
			infoSet=true;
			break;
		}
	}
	
	if(!infoSet)
	{
		ACS_THROW(OBTFileParserException("no configuration found for satellite Goce")) ; // PRQA S 3081
	}
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method executed.") ;
}


void GoceOBTFileParser::addFileName(const string& name)
{
	set<string>::iterator it = _fileNames.find(name);
	if( it == _fileNames.end() )//add filename only if NOT PRESENT yet
	{
		_fileNames.insert(name);
		_fileNamesVec.push_back(name);
		buildParams();
	}
}

void GoceOBTFileParser::removeFileName(const string& name)
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


void GoceOBTFileParser::parse(std::vector<Parameters *>& params)
{
	params.clear() ;
    size_t p_size = _validParams.size() ;
	for (size_t i=0; i < p_size; ++i) {
		params.push_back(_validParams[i]) ;
    }
}


_ACS_END_NAMESPACE

