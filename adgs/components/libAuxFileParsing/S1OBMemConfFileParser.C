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
	Revision 2.4  2014/03/19 19:14:56  marpas
	coding best practices application in progress (not ended)
	warning fixing (qa & g++)
	
	Revision 2.3  2012/04/04 06:35:22  giucas
	The VCID is no more made 1-relative
	
	Revision 2.2  2011/09/22 08:52:57  giucas
	Using string - for empty PacketStoreId and setting dataVcid to 63 for Filler
	
	Revision 2.1  2011/09/22 08:22:06  giucas
	Added S1OBMemConfFileParser
	

*/

#include <S1OBMemConfFileParser.h>
#include <File.h>
#include <XMLIstream.h>
#include <StringUtils.h>
#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(S1OBMemConfFileParser)

//Class constructor
S1OBMemConfFileParser::S1OBMemConfFileParser() :
    AuxFileParser(),
	_fileNames(),
	_fileNamesVec(),
	_fileType(),
	_validParams()
{
	static char Id[] = "@(#) A.C.S.: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Id: " << Id) ;
}

//Class constructor
S1OBMemConfFileParser::S1OBMemConfFileParser(const string& fileName) :
    AuxFileParser(),
	_fileNames(),
	_fileNamesVec(),
	_fileType(),
	_validParams()
{
	static char Id[] = "@(#) A.C.S.: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Id: " << Id) ;
	
	_fileNames.insert(fileName);
	_fileNamesVec.push_back(fileName);
	buildParams();
} 

//Class constructor
S1OBMemConfFileParser::S1OBMemConfFileParser(const rsResourceSet& fileRes) :
    AuxFileParser(),
	_fileNames(),
	_fileNamesVec(),
	_fileType(),
	_validParams()
{
	static char Id[] = "@(#) A.C.S.: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Id: " << Id) ;
	
	parseFile(fileRes) ;
} 

//Class destructor
S1OBMemConfFileParser::~S1OBMemConfFileParser() throw() 
{
}

//copy constructor
S1OBMemConfFileParser::S1OBMemConfFileParser(const S1OBMemConfFileParser & e) :
    AuxFileParser(e),
	_fileNames(e._fileNames),
	_fileNamesVec(e._fileNamesVec),
	_fileType(e._fileType),
	_validParams(e._validParams)
{
}

//operator = 
S1OBMemConfFileParser&  S1OBMemConfFileParser::operator=(const S1OBMemConfFileParser & e)
{
	if (this != &e) 
	{
        AuxFileParser::operator=(e) ;
		_fileNames				= e._fileNames;
		_fileNamesVec			= e._fileNamesVec;
		_fileType				= e._fileType;
		_validParams			= e._validParams;
	}
	return *this ;
}

void S1OBMemConfFileParser::buildParams()
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"method called") ;

	//clean current Valid Params
	_validParams.clear();
	
	//iterate through the set and build the valid parameter sequence
	std::set<std::string>::iterator it = _fileNames.begin();
	while ( it != _fileNames.end() )
	{
		if(!File::exists(*it))
		{
			ACS_THROW(exFileOpenException("The input OBMEM File [" + *it + "] does not exist")) ; // PRQA S 3081
		}
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"processing file "+(*it)) ;

		rsResourceSet rsFile;
		XMLIstream istr(*it);
		istr >> rsFile;
		parseFile(rsFile);
		it++;
	}
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"method executed.") ;
}

void S1OBMemConfFileParser::parseFile(const rsResourceSet& rs)
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"method called") ;
	
	unsigned int count=0 ;
	bool onlyOne = false ;
	try {
#warning MP this shoud use getArray insted
		rs.getValue("List_of_PacketStoreAssociations.PacketStoreAssociation[]",count) ;
	}
	catch(exception& e)
	{
		onlyOne = true ;
		count = 1 ;
	}

	S1OBMemConfParams param ;

	for(unsigned int j=0; j<count; j++)
	{
		ostringstream root ;
		if( onlyOne ) {
			root << "List_of_PacketStoreAssociations.PacketStoreAssociation." ;
        }
		else {
			root << "List_of_PacketStoreAssociations.PacketStoreAssociation[" << j << "]." ;
        }
		
        string currPsId ;
		rs.getValue(root.str()+"PacketStoreId",currPsId) ;
        string vcidStrConf ;
		rs.getValue(root.str()+"VirtualChannelId",vcidStrConf) ;
		string currPsType ;
		rs.getValue(root.str()+"PacketStoreType",currPsType) ;
		
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"READ [ " << currPsId << "," << vcidStrConf << "," << currPsType << "]") ;
		
		if(currPsId.empty()) { currPsId = "-" ; }
		
		pair<string,string> elem ;
		elem.first = vcidStrConf ;
		elem.second = currPsType ;
		param._psInfo[currPsId] = elem ;
		
		string vcidStr = vcidStrConf.substr(vcidStrConf.find('-') +1, vcidStrConf.size() -1 - vcidStrConf.find('-')); // PRQA S 3084
		
		//the VCID field on data is 1-relative with respect to the PS-VCID mapping table <-- giucas removed 04/04/2012
		unsigned int dataVcid = atoi(vcidStr.c_str()) ; // PRQA S 3000
		if(dataVcid == 64) { dataVcid = 63 ; } // PRQA S 4400
		param._vcidDataInfo[dataVcid] = currPsType ;
			
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"LOADED PARAMETER " << param.dump()) ;
	}			

	_validParams.push_back(param) ;

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"method executed.") ;
}

void S1OBMemConfFileParser::addFileName(const std::string& name)
{	
	std::set<std::string>::iterator it = _fileNames.find(name);
	if( it == _fileNames.end() )//add filename only if NOT PRESENT yet
	{
		_fileNames.insert(name);
		_fileNamesVec.push_back(name);
		buildParams();
	}
}

void S1OBMemConfFileParser::removeFileName(const string& name)
{
	set<string>::iterator it = _fileNames.find(name);
	if( it != _fileNames.end() )//remove filename only if previously added
	{
		_fileNames.erase(name);
		std::vector<std::string>::iterator itV = find(_fileNamesVec.begin(),_fileNamesVec.end(),name);
		_fileNamesVec.erase(itV);
		buildParams();
	}
}

void S1OBMemConfFileParser::parse(vector<Parameters *>& params)
{
	params.clear() ;
	size_t vp_size = _validParams.size();
    for(size_t i=0; i < vp_size; ++i) {
		params.push_back(&_validParams[i]) ;
    }
}


_ACS_END_NAMESPACE

