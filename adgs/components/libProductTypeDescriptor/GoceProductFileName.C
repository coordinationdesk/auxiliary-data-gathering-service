// PRQA S 1050 EOF
/*

	Copyright 2002-2014, Advanced Computer Systems , Inc.
	Via della Bufalotta, 378 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$
	
	$Log$
	Revision 2.3  2013/12/09 14:01:11  giucas
	Library completely revisited.
	
	Revision 2.2  2012/03/12 10:19:19  giucas
	Some compile warnings fixed
	
	Revision 2.1  2011/03/01 17:04:59  marpas
	GCC 4.4.x support
	
	Revision 2.0  2006/02/28 09:31:26  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.4  2004/12/03 09:50:40  giucas
	Header fixed
	
	Revision 1.3  2004/09/01 10:30:42  giucas
	Added nameWVersionForQuery method
	
	Revision 1.2  2004/08/05 16:03:18  giucas
	Changed to handle a 0-sized list of configured File extensions
	
	Revision 1.1  2004/08/02 16:05:10  giucas
	Imported source.
	
	

*/

#include <GoceProductFileName.h>
#include <File.h>
#include <ConfigurationSingleton.h>
#include <TimeConverter.h>
#include <TimeConverterSingleton.h>
#include <algorithm>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(GoceProductFileName) ;

//Class constructor
GoceProductFileName::GoceProductFileName() :
    ProductFileName(),
	_rsConf(ConfigurationSingleton::instance()->get()),
	_missionId(),
	_fileNameVersion(),
	_rootSize(0),
	_fileSize(0),
	_permittedExtensions(),
	_permittedClasses(),
	_validL0FileNames(),
	_validL1FileNames(),
	_validL2FileNames(),
	_validFileNames()
{
	init();
}


//Class destructor
GoceProductFileName::~GoceProductFileName() throw()
{
}


/////////////////////////////////////////////////////////////////////////////////////////
// Public methods 
/////////////////////////////////////////////////////////////////////////////////////////
void GoceProductFileName::getNamesById(const string& prodId, vector<string>& names)
{
    size_t pc_size = _permittedClasses.size();
    size_t pe_size = _permittedExtensions.size();
	for(unsigned int j=0; j < pc_size; j++)
	{
		if(pe_size == 0)
		{
			string name = getRoot(_permittedClasses[j],prodId);
			names.push_back(name);
		}
		else {
			for(unsigned int i=0; i < pe_size; i++)
			{
				string name = getRoot(_permittedClasses[j],prodId) + "." + _permittedExtensions[i];
				names.push_back(name);
			}
        }
	}	
}

void GoceProductFileName::getNamesById(const string& prodId, 
								      const string& startTime,
								      const string& stopTime,
								  	  vector<string>& names)
{
    size_t pc_size = _permittedClasses.size();
    size_t pe_size = _permittedExtensions.size();
	for(unsigned int j=0; j < pc_size; j++)
	{
		if(pe_size == 0)
		{
			string name = getRoot(_permittedClasses[j],prodId) + "_" + startTime + "_" + stopTime + "_" +_fileNameVersion;
			names.push_back(name);		
		}
		else {
			for(unsigned int i=0; i < pe_size; i++)
			{
				string name = getRoot(_permittedClasses[j],prodId) + "_" + startTime + "_" + stopTime + "_" +_fileNameVersion + "." + _permittedExtensions[i];
				names.push_back(name);
			}
        }
	}
}



void GoceProductFileName::getNamesByMission(const string& missionId, const string& prodId, vector<string>& names)
{
	//check if missionId is a valid Mission Id
	if(!(missionId == _missionId))
	{
		ACS_THROW(ProductFileNameException("Incorrect MissionId " + missionId)) ; // PRQA S 3081
	}

    size_t pc_size = _permittedClasses.size();
    size_t pe_size = _permittedExtensions.size();
	for(unsigned int j=0; j < pc_size; j++)
	{
		if(pe_size == 0)
		{
			string name = getRoot(_permittedClasses[j],prodId);
			names.push_back(name);
		}
		else {
			for(unsigned int i=0; i < pe_size; i++)
			{
				string name = getRoot(_permittedClasses[j],prodId) + "." + _permittedExtensions[i];
				names.push_back(name);
			}
        }
	}
}

void GoceProductFileName::getNamesByMission(const string& missionId,
										   const string& prodId, 
									       const string& startTime,
								  	   	   const string& stopTime,
									   	   vector<string>& names)
{
	//check if missionId is a valid Mission Id
	if(!(missionId == _missionId))
	{
		ACS_THROW(ProductFileNameException("Incorrect MissionId " + missionId)) ; // PRQA S 3081
	}
	
    size_t pc_size = _permittedClasses.size();
    size_t pe_size = _permittedExtensions.size();
	for(unsigned int j=0; j < pc_size; j++)
	{
		if(pe_size == 0)
		{
			string name = getRoot(_permittedClasses[j],prodId) + "_" + startTime + "_" + stopTime + "_" +_fileNameVersion;
			names.push_back(name);		
		}
		else {
			for(unsigned int k=0; k < pe_size; k++)
			{
				string name = getRoot(_permittedClasses[j],prodId) + "_" + startTime + "_" + stopTime + "_" +_fileNameVersion + "." + _permittedExtensions[k];
				names.push_back(name);
			}
        }
	}
}

void GoceProductFileName::getNamesByExtension(const string& ext,const string& prodId,vector<string>& names)
{
	//check if ext is a valid Extension
	if(!isValidExtension(ext))
	{
		ACS_THROW(ProductFileNameException("Incorrect extension type " + ext)) ; // PRQA S 3081
	}
	
    size_t pc_size = _permittedClasses.size();
	for(unsigned int j=0; j < pc_size; j++)
	{
		string name = getRoot(_permittedClasses[j],prodId) + "." + ext;
		names.push_back(name);
	}
}

void GoceProductFileName::getNamesByExtension(const string& ext,
											 const string& prodId, 
										 	 const string& startTime,
								  	     	 const string& stopTime,
										 	 vector<string>& names)
{
	//check if ext is a valid Extension
	if(!isValidExtension(ext))
	{
		ACS_THROW(ProductFileNameException("Incorrect extension type " + ext)) ; // PRQA S 3081
	}
	
    size_t pc_size = _permittedClasses.size();
	for(unsigned int j=0; j < pc_size; j++)
	{
		string name = getRoot(_permittedClasses[j],prodId) + "_" + startTime + "_" + stopTime + "_" +_fileNameVersion + "." + ext;
		names.push_back(name);
	}
}		


bool GoceProductFileName::isValidExtension(const string& ext)
{
	return find(_permittedExtensions.begin(),_permittedExtensions.end(),ext) != _permittedExtensions.end() ;
}


bool GoceProductFileName::isValidName(const string& fileName)
{
	return checkNameValidity(fileName,_validFileNames);
}

bool GoceProductFileName::isValidRoot(const string& fileName)
{
	return checkRootValidity(fileName,_validFileNames);
}

bool GoceProductFileName::isValidL0Name(const string& fileName)
{
	return checkNameValidity(fileName,_validL0FileNames);
}

bool GoceProductFileName::isValidL0Root(const string& fileName)
{
	return checkRootValidity(fileName,_validL0FileNames);
}

bool GoceProductFileName::isValidL1Name(const string& fileName)
{
	return checkNameValidity(fileName,_validL1FileNames, true);
}

bool GoceProductFileName::isValidL1Root(const string& fileName)
{
	return checkRootValidity(fileName,_validL1FileNames);
}

bool GoceProductFileName::isValidL2Name(const string& fileName)
{
	return checkNameValidity(fileName,_validL2FileNames, true);
}

bool GoceProductFileName::isValidL2Root(const string& fileName)
{
	return checkRootValidity(fileName,_validL2FileNames);
}


//build a filename from root, extension, start and stop dates
//It is made NO control about dates correctness!!!!!!!!!!!!!!
void GoceProductFileName::getName(const string& root, 
			 					 const string& startDate, 
			 					 const string& stopDate, 
			 					 const string& ext, 
			 					 string& result)
{
	if(root.size() != _rootSize)
	{
		ACS_THROW(ProductFileNameException("Incorrect root " + root)); // PRQA S 3081
	}
	
	string fileExtension="";
	if(ext != "")
	{
		vector<string>::iterator it = find(_permittedExtensions.begin(),_permittedExtensions.end(),ext);
	
		if (it == _permittedExtensions.end())
		{
			ACS_THROW(ProductFileNameException("Incorrect extension " + ext)); // PRQA S 3081
		}
		fileExtension = "." + ext;
	}
	
	result = root + "_" + startDate + "_" + stopDate + "_" + _fileNameVersion + fileExtension;
}

void GoceProductFileName::getName(const string& root, 
								 double start, 
								 double stop, 
								 const string& ext, 
								 string& result)
{
	if(root.size() != _rootSize)
	{
		ACS_THROW(ProductFileNameException("Incorrect root " + root)); // PRQA S 3081
	}
	
	string fileExtension="";
	if(ext != "")
	{
		vector<string>::iterator it = find(_permittedExtensions.begin(),_permittedExtensions.end(),ext);

		if (it == _permittedExtensions.end())
		{
			ACS_THROW(ProductFileNameException("Incorrect extension " + ext)); // PRQA S 3081
		}
		fileExtension = "." + ext;
	}
	
	ProcessingTime startIn(start) ;
    ProcessingTime stopIn(stop);
	startIn.setRef(TimeFormat::TC_TIME_UTC);
	startIn.setFormat(TimeFormat::TC_PROC);
	stopIn.setRef(TimeFormat::TC_TIME_UTC);
	stopIn.setFormat(TimeFormat::TC_PROC);
	AsciiTime startOut ;
    AsciiTime stopOut;
	startOut.setRef(TimeFormat::TC_TIME_UTC);
	startOut.setFormat(TimeFormat::TC_ASCII_CCSDSA_COMPACT);
	stopOut.setRef(TimeFormat::TC_TIME_UTC);
	stopOut.setFormat(TimeFormat::TC_ASCII_CCSDSA_COMPACT);
	TimeConverterSingleton::Instance()->getConverter()->convert(startIn,startOut);
	TimeConverterSingleton::Instance()->getConverter()->convert(stopIn,stopOut);
	
	result = root + "_" + startOut.getValue() + "_" + stopOut.getValue() + "_" + _fileNameVersion + fileExtension;
}

void GoceProductFileName::getName(const string& root, 
			 					 const string& startDate, 
			 					 const string& stopDate, 
			 					 long int phase,
								 long int cycle,
								 long int relOrb,
								 long int absOrb,
								 int counter,
								 const string& ext,
			 					 string& result)
{
	GoceProductFileName::getName(root,startDate,stopDate,ext,result);
}

void GoceProductFileName::getName(const string& root, 
			 					 double startDate, 
			 					 double stopDate, 
			 					 long int phase,
								 long int cycle,
								 long int relOrb,
								 long int absOrb,
								 int counter,
								 const string& ext,
			 					 string& result)
{
	GoceProductFileName::getName(root,startDate,stopDate,ext,result);
}



string GoceProductFileName::getProductIDFromRoot(const string& fileName)
{
	if(!isValidRoot(fileName))
	{
		ACS_THROW(ProductFileNameException ("Error: invalid Product File Name Root" + fileName)) ; // PRQA S 3081
	}	
	return fileName.substr(8, 10); // PRQA S 4400
}

string GoceProductFileName::getProductIDFromName(const string& fileName)
{
	if(!isValidName(fileName))
	{
		ACS_THROW(ProductFileNameException ("Error: invalid Product File Name " + fileName)) ; // PRQA S 3081
	}	
	return fileName.substr(8, 10); // PRQA S 4400
}

///////////////////////////////////////////////////////////
// useful methods to extract info from complete file names
///////////////////////////////////////////////////////////

string GoceProductFileName::getVersion(const string& fileName)
{
	if(!isValidName(fileName))
	{
		ACS_THROW(ProductFileNameException ("Error: invalid Product File Name " + fileName)) ; // PRQA S 3081
	}	
	return fileName.substr(51, 4); // PRQA S 4400
}

string GoceProductFileName::getFileClass(const string& fileName)
{
	if(!isValidName(fileName))
	{
		ACS_THROW(ProductFileNameException ("Error: invalid Product File Name " + fileName)) ; // PRQA S 3081
	}	
	return fileName.substr(3, 4); // PRQA S 4400
}

string GoceProductFileName::getFileClassFromRoot(const string& fileName)
{
	if(!isValidRoot(fileName))
	{
		ACS_THROW(ProductFileNameException ("Error: invalid Product File Name Root" + fileName)) ; // PRQA S 3081
	}	
	return fileName.substr(3, 4); // PRQA S 4400
}


long double GoceProductFileName::getStartTime(const string& fileName)
{
	if(!isValidName(fileName))
	{
		ACS_THROW(ProductFileNameException ("Error: invalid Product File Name " + fileName)) ; // PRQA S 3081
	}	
	//convert ascii start time TC_ASCII_CCSDSA_COMPACT in ProcessingTime
	AsciiTime inTime(fileName.substr(19,15),TimeFormat::TC_TIME_UTC,TimeFormat::TC_ASCII_CCSDSA_COMPACT);
	ProcessingTime outTime;
	outTime.setRef(TimeFormat::TC_TIME_UTC);
	outTime.setFormat(TimeFormat::TC_PROC);
	TimeConverterSingleton::Instance()->getConverter()->convert(inTime,outTime);
	
	return outTime.getValue();
}

long double GoceProductFileName::getStopTime(const string& fileName)
{
	if(!isValidName(fileName))
	{
		ACS_THROW(ProductFileNameException ("Error: invalid Product File Name " + fileName)) ; // PRQA S 3081
	}	
	//convert ascii start time TC_ASCII_CCSDSA_COMPACT in ProcessingTime
	AsciiTime inTime(fileName.substr(35,15),TimeFormat::TC_TIME_UTC,TimeFormat::TC_ASCII_CCSDSA_COMPACT);
	ProcessingTime outTime;
	outTime.setRef(TimeFormat::TC_TIME_UTC);
	outTime.setFormat(TimeFormat::TC_PROC);
	TimeConverterSingleton::Instance()->getConverter()->convert(inTime,outTime);
	
	return outTime.getValue();
}

string GoceProductFileName::nameWVersion(const string& fileName,const string& version)
{
	if(!isValidName(fileName))
	{
		ACS_THROW(ProductFileNameException ("Error: invalid Product File Name " + fileName)) ; // PRQA S 3081
	}	
	if(version.size() > 4) // PRQA S 4400
	{
		ACS_THROW(ProductFileNameException ("Error: invalid version " + version+".Bad size")) ; // PRQA S 3081
	}
	//get extension if any
	string ext = File::getExt(fileName);
	string extension="";
	if(ext != "") {
		extension = "."+ext;
    }
	//format the version in input
	ostringstream vers;
	vers << setw(4) << setfill('0') << version; // PRQA S 4400 3
	string root = fileName.substr(0, 51);
	string oldVersion = fileName.substr(52, 4);
	return root+vers.str()+extension;
}


/////////////////////////////////////////////////////////////////////////////////////////
// Private methods 
/////////////////////////////////////////////////////////////////////////////////////////

/*
 * read data from rsResourceSet and set private attributes
 */ 
void GoceProductFileName::init()
{
	//storing MissionId
	_rsConf.getValue("GoceProductFileName.MissionID", _missionId);
	
	//storing extensions
	unsigned int numberOfExtensions=0;
	_rsConf.getValue("GoceProductFileName.List_of_File_Ext.count.value", numberOfExtensions);
	for (unsigned int i=0; i < numberOfExtensions; i++)
	{
		string ext = "";
		ostringstream num;
		num << i;
		string temp(num.str());
		
		string key = "GoceProductFileName.List_of_File_Ext.File_Ext["+temp+"]";
		_rsConf.getValue(key,ext);
		_permittedExtensions.push_back(ext);
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "added Valid Extension " << ext );
	}

	//storing fileClasses
	unsigned int numberOfClasses=0;
	_rsConf.getValue("GoceProductFileName.List_of_File_Classes.count.value", numberOfClasses);
	for (unsigned int i=0; i < numberOfClasses; i++)
	{
		string fClass = "";
		ostringstream num;
		num << i;
		string temp(num.str());
		
		string key = "GoceProductFileName.List_of_File_Classes.File_Class["+temp+"]";
		_rsConf.getValue(key,fClass);
		_permittedClasses.push_back(fClass);
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "added Valid File Class " << fClass ) ;
	}

	//storing L0 ProductIds and ProductInfo objects
	unsigned int numberOfFiles = 0;
	_rsConf.getValue("GoceProductFileName.List_of_Level0_Product_Id.count.value", numberOfFiles);
	string validName="";
	for (unsigned int i=0; i < numberOfFiles; i++)
	{
		string productID="";
		ostringstream num;
		num << i;
		string temp(num.str());
		string key = "GoceProductFileName.List_of_Level0_Product_Id.Id["+temp+"]";
		_rsConf.getValue(key,productID);
		//build valid roots	for this productId
        size_t pc_size = _permittedClasses.size() ;
		for(unsigned int j=0; j<pc_size; ++j)
		{				
			validName=_missionId + "_" + _permittedClasses[j] + "_" + productID;
			_validL0FileNames.push_back(validName);
			_validFileNames.push_back(validName);
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "added L0 Valid FileName Root " << validName ) ;
		}
	}	

	
	//storing L1 ProductIds and ProductInfo objects
	_rsConf.getValue("GoceProductFileName.List_of_Level1_Product_Id.count.value", numberOfFiles);
	validName="";
	for (unsigned int i=0; i < numberOfFiles; i++)
	{
		string productID="";
		ostringstream num;
		num << i;
		string temp(num.str());
		string key = "GoceProductFileName.List_of_Level1_Product_Id.Id["+temp+"]";
		_rsConf.getValue(key,productID);
		//build valid roots	for this productId
        size_t pc_size = _permittedClasses.size() ;
		for(unsigned int j=0; j<pc_size; ++j)
		{				
			validName=_missionId + "_" + _permittedClasses[j] + "_" + productID;
			_validL1FileNames.push_back(validName);
			_validFileNames.push_back(validName);
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "added L1 Valid FileName Root " << validName ) ;
		}
	}	

	//storing L2 ProductIds and ProductInfo objects
	_rsConf.getValue("GoceProductFileName.List_of_Level2_Product_Id.count.value", numberOfFiles);
	validName="";
	for (unsigned int i=0; i < numberOfFiles; i++)
	{
		string productID="";
		ostringstream num;
		num << i;
		string temp(num.str());
		string key = "GoceProductFileName.List_of_Level2_Product_Id.Id["+temp+"]";
		_rsConf.getValue(key,productID);
		//build valid roots	for this productId
        size_t pc_size = _permittedClasses.size() ;
		for(unsigned int j=0; j<pc_size; ++j)
		{				
			validName=_missionId + "_" + _permittedClasses[j] + "_" + productID;
			_validL2FileNames.push_back(validName);
			_validFileNames.push_back(validName);
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "added L2 Valid FileName Root " << validName ) ;
		}
	}	
	//storing other general informations
	_rsConf.getValue("GoceProductFileName.FileNameVersion", _fileNameVersion);
	_rsConf.getValue("GoceProductFileName.FileRootSize", _rootSize);
	_rsConf.getValue("GoceProductFileName.FileNameSize", _fileSize);
} 

// Modified by MarFav.
// The skipNameSizeCheck flag is added to ignore the filename size of the product
// and is used by the isValidL1Name method only. It has default value to FALSE and allows the managing of the product report
// (SIRPRODRPT filetype) that has the name size not compliant with the product naming convention.
bool GoceProductFileName::checkNameValidity(const string& fileName, vector<string>& validRoots, bool skipNameSizeCheck)
{
	bool result=false;
	string fileWithoutExt = File::removeExt(fileName);

	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "checkNameValidity : fileWithoutExt is '" << fileWithoutExt 
		       << "' of size " << fileWithoutExt.size() << "[" << _fileSize << "]" ) ;

#ifdef FORCE_NAME_SIZE_CHECK
	if (fileWithoutExt.size() == _fileSize)
#else  //FORCE_NAME_SIZE_CHECK
	if ((fileWithoutExt.size() == _fileSize) || skipNameSizeCheck)
#endif  //FORCE_NAME_SIZE_CHECK
	{
		string fileExtension = File::getExt(fileName);
		string root          = fileName.substr(0, _rootSize);
		
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "checkNameValidity : FILE EXTENSION is " << fileExtension );
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "checkNameValidity : FILE ROOT is      " << root ) ;
			
		vector<string>::iterator it = find(validRoots.begin(),validRoots.end(),root);
		
		if( it != validRoots.end())
		{
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "checkNameValidity : " << root << " is a Valid file name" );
			if(fileExtension != "")
			{
				it = find(_permittedExtensions.begin(),_permittedExtensions.end(),fileExtension);
				if(it != _permittedExtensions.end())
				{
					ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "checkNameValidity : " << fileExtension << " is a Valid file extension" );
					result=true;
				}
				else
				{
					ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "checkNameValidity : " << fileExtension << " is not a Valid file extension");
				}
			}
			else {
				result=true;
			}
		}
	}	
	return result;
}


bool GoceProductFileName::checkRootValidity(const string& fileName, vector<string>& validRoots)
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


_ACS_END_NAMESPACE


