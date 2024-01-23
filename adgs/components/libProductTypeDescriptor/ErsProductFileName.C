// PRQA S 1050 EOF
/*

	Copyright 2002-2014, Advanced Computer Systems , Inc.
	Via della Bufalotta, 347 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$
	
	$Log$
	Revision 1.8  2013/12/09 14:01:11  giucas
	Library completely revisited.
	
	Revision 1.7  2012/03/12 10:19:18  giucas
	Some compile warnings fixed
	
	Revision 1.6  2011/03/01 17:04:59  marpas
	GCC 4.4.x support
	
	Revision 1.5  2008/10/16 13:51:34  serfol
	getting the resource values, now the tag name doesn't contain the satellite name
	
	Revision 1.4  2008/08/22 11:43:26  giucas
	WARNING removed
	
	Revision 1.3  2008/08/22 11:39:56  giucas
	Methods for L1 implemented. Note that for L0 the Naming Convention is ESA NAMING CONVENTION while for L1 is ENVISAT NAMING CONVENTION
	
	Revision 1.2  2008/07/25 16:38:20  giucas
	Fixed erroneous call
	
	Revision 1.1  2008/07/22 13:26:25  giucas
	First issue
	

*/


#include <ErsProductFileName.h>
#include <File.h>
#include <ACSStationsWrapper.h>
#include <ConfigurationSingleton.h>
#include <TimeConverter.h>
#include <TimeConverterSingleton.h>
#include <sys/types.h> // PRQA S 1013
#include <unistd.h>
#include <cmath>
#include <algorithm>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;
namespace {
    const double DAYSECONDS = 86400.;

/* FILENAME CONVENTION FOR ASAR MISSION:
 *
 * PRODUCT="ASA_ IMP_1P  X           PDE      20020818_212501_ 00000016  2      008_    00387_   02445_   0007     .N1"
 * pos      0            10          11       14               30        38     39      43       49       55              
 * len          10             1         3         15               8        1      3       5        5          4        2
 *          <FILETYPE>   <FILECLASS> <ORIGID> <STARTIME_HOUR>_ <DURATION><PHASE><CYCLE>_<RELORB>_<ABSORB>_<VERSION>.<MISSID>
 */
    const unsigned int PN_FILETYPE_L = 10;
    const unsigned int PN_FILECLASS_L = 1;
    const unsigned int PN_STARTIME_L = 15;
    const unsigned int PN_DURATION_L = 8;
    const unsigned int PN_VERSION_L = 4;
    const unsigned int PN_FILETYPE_P = 0;
    const unsigned int PN_FILECLASS_P = 10;
    const unsigned int PN_STARTIME_P = 14;
    const unsigned int PN_DURATION_P = 30;
    const unsigned int PN_VERSION_P = 55;
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(ErsProductFileName) ;

//Class constructor
ErsProductFileName::ErsProductFileName() :
    ProductFileName(),
	_rsConf(ConfigurationSingleton::instance()->get()),
	_missionId(),
	_fileNameVersion(),
	_rootSize(0),
	_fileSize(0),
	_permittedOriginators(),
	_permittedExtensions(),
	_permittedClasses(),
	_validL0FileNames(),
	_validL1FileNames(),
	_validBWSFileNames(),
	_validFileNames()
{
	init() ;
}


//Class destructor
ErsProductFileName::~ErsProductFileName() throw()
{
}


bool ErsProductFileName::isValidExtension(const string& ext)
{
	return find(_permittedExtensions.begin(),_permittedExtensions.end(),ext) != _permittedExtensions.end() ;
}

bool ErsProductFileName::isValidName(const string& fileName)
{
	return checkNameValidity(fileName,_validFileNames);
}

bool ErsProductFileName::isValidRoot(const string& fileName)
{
	return checkRootValidity(fileName,_validFileNames);
}

bool ErsProductFileName::isValidL1Name(const string& fileName)
{
	return checkNameValidity(fileName,_validL1FileNames);
}

bool ErsProductFileName::isValidBWSName(const string& fileName)
{
	return checkNameValidity(fileName,_validBWSFileNames);
}

bool ErsProductFileName::isValidL1Root(const string& fileName)
{
	return checkRootValidity(fileName,_validL1FileNames);
}

bool ErsProductFileName::isValidBWSRoot(const string& fileName)
{
	return checkRootValidity(fileName,_validBWSFileNames);
}


void ErsProductFileName::getName(const std::string&,
							 double,
							 double,
							 long int, 
							 long int,
							 long int,
							 long int,
							 int,
							 const std::string&,
							 std::string&
							)
{
}

void ErsProductFileName::getName(const std::string&,
							 const std::string&,
							 const std::string&,
							 long int, 
							 long int,
							 long int,
							 long int,
							 int,
							 const std::string&,
							 std::string&
							)
{
}


bool ErsProductFileName::isValidL0Root(const string& fileName) // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "isValidL0Root : Checking fileName '" << fileName << "'" ) ;
    
    size_t vn_size = _validL0FileNames.size() ;
    if (fileName.size() > 15) { // PRQA S 4400
	    for(unsigned int i=0; i<vn_size; i++)
	    {
		    if( fileName.substr(5,10) == _validL0FileNames[i] ) // PRQA S 4400
		    {
			    return true; 
		    }
	    }
    }
	return false ;
}



bool ErsProductFileName::isValidL0Name(const string& fileName) // PRQA S 4020
{
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "isValidL0Name : Checking fileName '" << fileName << "'" ) ;
    size_t vn_size = _validL0FileNames.size() ;
    if (fileName.size() > 15) { // PRQA S 4400
	    for(unsigned int i=0; i<vn_size; i++)
	    {
		    if( fileName.substr(5,10) == _validL0FileNames[i] ) // PRQA S 4400
		    {
			    return 0 != ACS_ESAProductFileName_CheckFileNameIsValid(fileName.c_str()) ;
		    }
	    }
    }
	return false ;
}


string ErsProductFileName::getDuration(const string& start, const string& stop)
{
	//convert Start/Stop times from ASCII TC_ASCII_COMPACT in processing
	AsciiTime startIn(start, TimeFormat::TC_TIME_UTC, TimeFormat::TC_ASCII_COMPACT);
	AsciiTime stopIn(stop, TimeFormat::TC_TIME_UTC, TimeFormat::TC_ASCII_COMPACT);
	ProcessingTime startOut ;
    ProcessingTime stopOut;
	startOut.setRef(TimeFormat::TC_TIME_UTC);
	stopOut.setRef(TimeFormat::TC_TIME_UTC);
	startOut.setFormat(TimeFormat::TC_PROC);
	stopOut.setFormat(TimeFormat::TC_PROC);
	TimeConverterSingleton::Instance()->getConverter()->convert(startIn,startOut);
	TimeConverterSingleton::Instance()->getConverter()->convert(stopIn,stopOut);
	//calculate duration
	unsigned int dur = ceil(( stopOut.getValue() - startOut.getValue() ) * DAYSECONDS ) ; // PRQA S 3011
	ostringstream duration;
	duration.fill('0');
	duration << setw(8) << dur; // PRQA S 4400
	return duration.str();
}


bool ErsProductFileName::checkNameValidity(const string& fileName, vector<string>& validRoots)
{
	bool result=false;
	//check if it's extension or missionId
	string ext = File::getExt(fileName);
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "checkNameValidity : Check extension '" << ext << "'" );
	if( ext == _missionId)
	{
		//the input filename has not extension
		if(fileName.size() == _fileSize)
		{
			string root = fileName.substr(0, _rootSize);
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "checkNameValidity : FILE ROOT is      " << root ) ;
			vector<string>::iterator it = find(validRoots.begin(),validRoots.end(),root);
			if( it != validRoots.end()) {
				result = true;
			}
		}
	}
	else
	{
		//the input file name has extension
		string fileWithoutExt = File::removeExt(fileName);
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "checkNameValidity : fileWithoutExt is '" << fileWithoutExt 
		       						<< "' of size " << fileWithoutExt.size() << "[" << _fileSize << "]" );
	
		if ( fileWithoutExt.size() == _fileSize)
		{
			string root          = fileName.substr(0, _rootSize);
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "checkNameValidity : FILE ROOT is      " << root ) ;

			vector<string>::iterator it = find(validRoots.begin(),validRoots.end(),root);

			if( it != validRoots.end())
			{
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "checkNameValidity : " << root << " is a Valid file name" );
				if(ext != "")
				{
					it = find(_permittedExtensions.begin(),_permittedExtensions.end(),ext);
					if(it != _permittedExtensions.end())
					{
						ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "checkNameValidity : " << ext << " is a Valid file extension" );
						result=true;
					}
					else
					{
						ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "checkNameValidity : " << ext << " is not a Valid file extension");				
					}
				}
				else {
					result=true;
				}
			}
		}
	}
	return result;
}




bool ErsProductFileName::checkRootValidity(const string& fileName, vector<string>& validRoots)
{
	bool result=false;
	
	string fileWithoutExt = File::removeExt(fileName);

	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "checkRootValidity : File ROOT is '" << fileWithoutExt << "' of size " 
							<< fileWithoutExt.size() << " [configured root size is " << _rootSize <<"]" );
	
	if(fileWithoutExt.size() == _rootSize)
	{
		string fileExtension = File::getExt(fileName);
		
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "checkRootValidity : File EXTENSION is '" << fileExtension << "'" );
		
		vector<string>::iterator it = find(validRoots.begin(),validRoots.end(),fileWithoutExt);
		
		if( it != validRoots.end() )
		{
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "checkRootValidity : File ROOT " << fileWithoutExt << " is valid" );

			if(fileExtension != "")
			{
				it = find(_permittedExtensions.begin(),_permittedExtensions.end(),fileExtension);
				if(it != _permittedExtensions.end())
				{
					result=true;
					ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "checkRootValidity : File EXTENSION " << fileExtension << " is valid" );
				}
				else
				{
					ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "checkRootValidity : File EXTENSION " << fileExtension << " is NOT valid" );				
				}
			}
			else {
				result=true;
			}
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "checkRootValidity : File ROOT " << fileWithoutExt << " is NOT valid" );		
		}
	}
	return result;
}


void ErsProductFileName::init()
{
	//storing MissionId
	_rsConf.getValue("ProductFileName.MissionID", _missionId);

	//storing OriginatorId
	ACSStationsWrapper statWrap ;
	vector<ACSStationsWrapper::StationInfo> info ;
	statWrap.getInfo(info) ;
    size_t i_size = info.size() ;
	for (unsigned int i=0; i < i_size; ++i)
	{
		_permittedOriginators.push_back(info[i].centreId);
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "added Originator " << info[i].centreId );
	}
	
	//storing extensions
	unsigned int numberOfExtensions=0;
	_rsConf.getValue("ProductFileName.List_of_File_Ext.count.value", numberOfExtensions);
	for (unsigned int i=0; i < numberOfExtensions; i++)
	{
		string ext = "";
		ostringstream num;
		num << i;
		string temp(num.str());
		
		string key = "ProductFileName.List_of_File_Ext.File_Ext["+temp+"]";
		_rsConf.getValue(key,ext);
		_permittedExtensions.push_back(ext);
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "added Valid Extension " << ext );
	}

	//storing fileClasses
	unsigned int numberOfClasses=0;
	_rsConf.getValue("ProductFileName.List_of_File_Classes.count.value", numberOfClasses);
	for (unsigned int i=0; i < numberOfClasses; i++)
	{
		string fClass = "";
		ostringstream num;
		num << i;
		string temp(num.str());
		
		string key = "ProductFileName.List_of_File_Classes.File_Class["+temp+"]";
		_rsConf.getValue(key,fClass);
		_permittedClasses.push_back(fClass);
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "added Valid File Class " << fClass );
	}
	
	//storing L0 ProductIds and ProductInfo objects
	unsigned int numberOfFiles = 0 ;
	_rsConf.getValue("ProductFileName.List_of_Level0_Product_Id.count.value", numberOfFiles);
	string validName="";
	for (unsigned int i=0; i < numberOfFiles; i++)
	{
		string productID="";
		ostringstream num;
		num << i;
		string temp(num.str());
		string key = "ProductFileName.List_of_Level0_Product_Id.Id["+temp+"]";
		_rsConf.getValue(key,productID);
		//build valid roots	for this productId
		validName=productID ;
		_validL0FileNames.push_back(validName);
		_validFileNames.push_back(validName);
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "added L0 Valid FileName Root " << validName );
	}	

	//storing L1 ProductIds and ProductInfo objects
	_rsConf.getValue("ProductFileName.List_of_Level1_Product_Id.count.value", numberOfFiles);
	validName="";
	for (unsigned int i=0; i < numberOfFiles; i++)
	{
		string productID="";
		ostringstream num;
		num << i;
		string temp(num.str());
		string key = "ProductFileName.List_of_Level1_Product_Id.Id["+temp+"]";
		_rsConf.getValue(key,productID);
		//build valid roots	for this productId
        size_t pc_size = _permittedClasses.size() ;
        size_t po_size = _permittedOriginators.size() ;
		for(unsigned int j=0; j<pc_size; ++j)
		{				
            for(unsigned int k=0; k<po_size; ++k)
			{				
				validName=productID + _permittedClasses[j] + _permittedOriginators[k];
				_validL1FileNames.push_back(validName);
				_validFileNames.push_back(validName);
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "added L1 Valid FileName Root " << validName );
			}
		}
	}	

	//storing BWS ProductIds and ProductInfo objects
	_rsConf.getValue("ProductFileName.List_of_Browse_Product_Id.count.value", numberOfFiles);
	validName="";
	for (unsigned int i=0; i < numberOfFiles; i++)
	{
		string productID="";
		ostringstream num;
		num << i;
		string temp(num.str());
		string key = "ProductFileName.List_of_Browse_Product_Id.Id["+temp+"]";
		_rsConf.getValue(key,productID);
		//build valid roots	for this productId
        size_t pc_size = _permittedClasses.size() ;
        size_t po_size = _permittedOriginators.size() ;
		for(unsigned int j=0; j<pc_size; ++j)
		{				
            for(unsigned int k=0; k<po_size; ++k)
			{				
				validName=productID + _permittedClasses[j] + _permittedOriginators[k];
				_validBWSFileNames.push_back(validName);
				_validFileNames.push_back(validName);
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "added L2 Valid FileName Root " << validName );
			}
		}
	}	
	//storing other general informations
	_rsConf.getValue("ProductFileName.FileNameVersion", _fileNameVersion);
	_rsConf.getValue("ProductFileName.FileRootSize", _rootSize);
	_rsConf.getValue("ProductFileName.FileNameSize", _fileSize);
} 
_ACS_END_NAMESPACE
