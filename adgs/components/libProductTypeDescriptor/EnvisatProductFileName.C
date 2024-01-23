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
	Revision 2.9  2013/12/09 14:01:11  giucas
	Library completely revisited.
	
	Revision 2.8  2012/03/12 10:19:18  giucas
	Some compile warnings fixed
	
	Revision 2.7  2011/03/01 17:04:59  marpas
	GCC 4.4.x support
	
	Revision 2.6  2008/10/16 13:51:34  serfol
	getting the resource values, now the tag name doesn't contain the satellite name
	
	Revision 2.5  2008/08/22 11:44:47  giucas
	serfol&&enrcar patches commented
	
	Revision 2.4  2008/06/09 09:01:43  giucas
	DEBUG improved
	
	Revision 2.3  2008/06/05 13:30:09  davcas
	GC: Using ACSStationsWrapper CentreId info to fill the configured Originator Ids.
	
	Revision 2.2  2007/07/05 09:11:41  giucas
	Implemented List of Originators handling
	
	Revision 2.1  2007/01/10 11:32:57  giucas
	Updated TimeConverter lib calls for IF changes (CFI 3.5 porting)
	
	Revision 2.0  2006/02/28 09:31:26  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.12  2005/08/02 13:52:58  clanas
	inserted math.h  to be compliant with gcc 3.4.3
	
	Revision 1.11  2005/04/21 11:53:21  giucas
	Minor modifications
	
	Revision 1.10  2004/12/03 09:50:40  giucas
	Header fixed
	
	Revision 1.9  2004/09/01 10:30:42  giucas
	Added nameWVersionForQuery method
	
	Revision 1.8  2004/05/18 09:16:00  envisat
	Added debug printouts in getStartTime and getStopTime methods
	
	Revision 1.7  2004/05/06 17:06:45  envisat
	Added class BWSProductTypeDescriptor
	
	Revision 1.6  2004/01/21 17:27:34  envisat
	Changed ascii format in getName method
	
	Revision 1.5  2003/10/23 08:04:32  giucas
	Added isValidName and isValidRoot methods for L0 and L1 products
	
	Revision 1.4  2003/10/13 15:36:42  giucas
	Added method getFileClassFromRoot
	
	Revision 1.3  2003/10/03 09:28:58  giucas
	Introduced ProductFileName conf space
	
	Revision 1.2  2003/09/29 13:59:02  marfav
	included Envisat naming convention in define directives (micepi)
	
	Revision 1.1  2003/09/09 13:12:11  giucas
	Created from EnvisatL0FileNames class

	
*/
	/**modi by micepi: 
	 * included Envisat naming convention in define directives
	 */


#include <EnvisatProductFileName.h>

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
    const double DAYSECONDS = 86400.0 ;

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

ACS_CLASS_DEFINE_DEBUG_LEVEL(EnvisatProductFileName) ;

//Class constructor
EnvisatProductFileName::EnvisatProductFileName() :
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
	init();
}


//Class destructor
EnvisatProductFileName::~EnvisatProductFileName() throw()
{
}


/////////////////////////////////////////////////////////////////////////////////////////
// Public methods 
/////////////////////////////////////////////////////////////////////////////////////////

void EnvisatProductFileName::getNamesById(const string& prodId, vector<string>& names)
{
    size_t pc_size = _permittedClasses.size() ;
    size_t po_size = _permittedOriginators.size() ;
    size_t pe_size = _permittedExtensions.size() ;
	for(unsigned int j=0; j < pc_size; ++j)
	{
		for(unsigned int k=0; k < po_size; k++)
		{
			if( pe_size > 0 )
			{
				for(unsigned int i=0; i < pe_size; i++)
				{
					string name = getRoot(_permittedClasses[j],prodId,_permittedOriginators[k]) + "." + _permittedExtensions[i];
					names.push_back(name);
				}
			}
			else
			{
				string name = getRoot(_permittedClasses[j],prodId,_permittedOriginators[k]);
				names.push_back(name);		
			}
		}
	}		
}

void EnvisatProductFileName::getNamesById(const string& prodId, 
					  const string& startTime,
					  const string& stopTime,
					   vector<string>& names)
{
    size_t pc_size = _permittedClasses.size() ;
    size_t pe_size = _permittedExtensions.size() ;
	for(unsigned int j=0; j < pc_size; j++)
	{
		for(unsigned int k=0; k < pc_size; k++)
		{
			if( pe_size > 0 )
			{
				for(unsigned int i=0; i < pe_size; i++)
				{
					string name = getRoot(_permittedClasses[j],prodId,_permittedOriginators[k]) + startTime + "_" + getDuration(startTime,stopTime) + "." + _missionId + "." + _permittedExtensions[i];
					names.push_back(name);
				}
			}
			else
			{
				string name = getRoot(_permittedClasses[j],prodId,_permittedOriginators[k]) + startTime + "_" + getDuration(startTime,stopTime) + "." + _missionId;
				names.push_back(name);		
			}
		}
	}
}



void EnvisatProductFileName::getNamesByMission(const string& missionId, const string& prodId, vector<string>& names)
{
	//check if missionId is a valid Mission Id
	if(!(missionId == _missionId))
	{
		ACS_THROW(ProductFileNameException("Incorrect MissionId " + missionId)) ; // PRQA S 3081
	}
    size_t pc_size = _permittedClasses.size() ;
    size_t po_size = _permittedOriginators.size() ;
    size_t pe_size = _permittedExtensions.size() ;
	for(unsigned int j=0; j < pc_size; j++)
	{
		for(unsigned int k=0; k < po_size; k++)
		{
			if(pe_size > 0)
			{
				for(unsigned int i=0; i < pe_size; i++)
				{
					string name = getRoot(_permittedClasses[j],prodId,_permittedOriginators[k]) + "." + _missionId + "." + _permittedExtensions[i];
					names.push_back(name);
				}
			}
			else
			{
				string name = getRoot(_permittedClasses[j],prodId,_permittedOriginators[k]) + "." + _missionId;
				names.push_back(name);			
			}
		}
	}
}

void EnvisatProductFileName::getNamesByMission(const string& missionId,
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
	
    size_t pc_size = _permittedClasses.size() ;
    size_t po_size = _permittedOriginators.size() ;
    size_t pe_size = _permittedExtensions.size() ;
	for(unsigned int j=0; j < pc_size; j++)
	{
		for(unsigned int k=0; k < po_size; k++)
		{
			if(pe_size > 0)
			{
				for(unsigned int i=0; i < pe_size; i++)
				{
					string name = getRoot(_permittedClasses[j],prodId,_permittedOriginators[k]) + startTime + "_" + getDuration(startTime,stopTime) + "." + _missionId + "." + _permittedExtensions[i];
					names.push_back(name);
				}
			}
			else
			{
				string name = getRoot(_permittedClasses[j],prodId,_permittedOriginators[k]) + startTime + "_" + getDuration(startTime,stopTime) + "." + _missionId;
				names.push_back(name);			
			}
		}
	}
}

void EnvisatProductFileName::getNamesByExtension(const string& ext,const string& prodId,vector<string>& names)
{
	//check if ext is a valid Extension
	if(!isValidExtension(ext))
	{
		ACS_THROW(ProductFileNameException("Incorrect extension type " + ext)); // PRQA S 3081
	}
	
    size_t pc_size = _permittedClasses.size() ;
    size_t po_size = _permittedOriginators.size() ;
	for(unsigned int j=0; j < pc_size; j++)
	{
		for(unsigned int k=0; k <po_size; k++)
		{
			string name = getRoot(_permittedClasses[j],prodId,_permittedOriginators[k]) + "." + ext;
			names.push_back(name);
		}
	}
}

void EnvisatProductFileName::getNamesByExtension(const string& ext,
						 const string& prodId, 
						 const string& startTime,
						 const string& stopTime,
						 vector<string>& names)
{
	//check if ext is a valid Extension
	if(!isValidExtension(ext))
	{
		ACS_THROW(ProductFileNameException("Incorrect extension type " + ext)); // PRQA S 3081
	}

    size_t pc_size = _permittedClasses.size() ;
    size_t po_size = _permittedOriginators.size() ;
	for(unsigned int j=0; j < pc_size; j++)
	{
		for(unsigned int k=0; k < po_size; k++)
		{
			string name = getRoot(_permittedClasses[j],prodId,_permittedOriginators[k]) + startTime + "_" + getDuration(startTime,stopTime) + "." + _missionId + "." + ext;
			names.push_back(name);
		}
	}

}		


bool EnvisatProductFileName::isValidExtension(const string& ext)
{
	return (find(_permittedExtensions.begin(),_permittedExtensions.end(),ext) != _permittedExtensions.end()) ;
}

bool EnvisatProductFileName::isValidName(const string& fileName)
{
	return checkNameValidity(fileName,_validFileNames);
}

bool EnvisatProductFileName::isValidRoot(const string& fileName)
{
	return checkRootValidity(fileName,_validFileNames);
}

bool EnvisatProductFileName::isValidL0Name(const string& fileName)
{
	return checkNameValidity(fileName,_validL0FileNames);
}

bool EnvisatProductFileName::isValidL0Root(const string& fileName)
{
	return checkRootValidity(fileName,_validL0FileNames);
}

bool EnvisatProductFileName::isValidL1Name(const string& fileName)
{
	return checkNameValidity(fileName,_validL1FileNames);
}

bool EnvisatProductFileName::isValidBWSName(const string& fileName)
{
	return checkNameValidity(fileName,_validBWSFileNames);
}

bool EnvisatProductFileName::isValidL1Root(const string& fileName)
{
	return checkRootValidity(fileName,_validL1FileNames);
}

bool EnvisatProductFileName::isValidBWSRoot(const string& fileName)
{
	return checkRootValidity(fileName,_validBWSFileNames);
}




//build a filename from root, extension, start and stop dates
//It is made NO control about dates correctness!!!!!!!!!!!!!!
void EnvisatProductFileName::getName(const string& root, 
			 	                     const string& startDate, 
			 	                     const string& stopDate, 
			 	                     const string& ext, 
			 	                     string& result)
{
	if(root.size() != _rootSize)
	{
		ACS_THROW(ProductFileNameException("Incorrect root " + root)) ; // PRQA S 3081
	}
	
	vector<string>::iterator it = find(_permittedExtensions.begin(),_permittedExtensions.end(),ext);
	
	if (it == _permittedExtensions.end())
	{
		ACS_THROW(ProductFileNameException("Incorrect extension " + ext)) ; // PRQA S 3081
	}
	
	result = root + startDate + "_" + getDuration(startDate,stopDate) + "." + _missionId + "." + ext;
}


void EnvisatProductFileName::getName(const string& root, 
                                     double start, 
                                     double stop, 
                                     const string& ext, 
                                     string& result )
{
	if(root.size() != _rootSize)
	{
		ACS_THROW(ProductFileNameException("Incorrect root " + root)) ; // PRQA S 3081
	}
	
	vector<string>::iterator it = find(_permittedExtensions.begin(),_permittedExtensions.end(),ext);
	
	if (it == _permittedExtensions.end())
	{
		ACS_THROW(ProductFileNameException("Incorrect extension " + ext)) ; // PRQA S 3081
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
	
	result = root + startOut.getValue() + "_" + getDuration(startOut.getValue(),stopOut.getValue()) + "." + _missionId + "." + ext;
}

void EnvisatProductFileName::getName(const string& root, 
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
	if(root.size() != _rootSize)
	{
		ACS_THROW(ProductFileNameException("Incorrect root " + root)) ; // PRQA S 3081
	}
	
	string fileExtension="";
	if(ext != "") {
		fileExtension = "." + ext;
    }
	
	//PHASE is 1 chars length
	ostringstream filePhase;
	filePhase.fill('0');
	filePhase << setw(1) << phase;
	
	//CYCLE is 3 chars length
	ostringstream fileCycle;
	fileCycle.fill('0');
	fileCycle << setw(3) << cycle; // PRQA S 4400 L1
	
	//RELATIVE_ORBIT is 5 chars length
	ostringstream fileRelOrb;
	fileRelOrb.fill('0');
	fileRelOrb << setw(5) << relOrb;

	//ABSOLUTE_ORBIT is 5 chars length
	ostringstream fileAbsOrb;
	fileAbsOrb.fill('0');
	fileAbsOrb << setw(5) << absOrb;
	
	//COUNTER is 4 chars length
	ostringstream fileCount;
	fileCount.fill('0');
	fileCount << setw(4) << counter; // PRQA L:L1
	
	result = root + 							//PRODUCT_ID+PROC_STAGE_FLAG+ORIGINATOR_ID
			 startDate + 						//START_DAY+"_"+START_TIME
			 "_" + 
			 getDuration(startDate,stopDate) +  //DURATION
			 filePhase.str() + 					//PHASE
			 fileCycle.str() +					//CYCLE
			 "_" +
			 fileRelOrb.str() +					//RELATIVE_ORBIT
			 "_" +
			 fileAbsOrb.str() +					//ABSOLUTE_ORBIT
			 "_" +
			 fileCount.str() +					//COUNTER
			 "." +
			 _missionId+						//SATELLITE_ID
			 fileExtension;						//file extension
}

void EnvisatProductFileName::getName(const string& root, 
			 					     double start, 
			 					     double stop, 
			 					     long int phase,
								     long int cycle,
								     long int relOrb,
								     long int absOrb,
								     int counter,
								     const string& ext,
			 					     string& result)
{
	if(root.size() != _rootSize)
	{
		ACS_THROW(ProductFileNameException("Incorrect root " + root)) ; // PRQA S 3081
	}
	
	string fileExtension="";
	if(ext != "") {
		fileExtension = "." + ext;
    }
	
	//convert start and stop dates in TC_ASCII_CCSDSA_COMPACT string format
	ProcessingTime startIn(start, TimeFormat::TC_TIME_UTC,TimeFormat::TC_PROC) ;
    ProcessingTime stopIn(stop,TimeFormat::TC_TIME_UTC,TimeFormat::TC_PROC);
	AsciiTime startOut ;
    AsciiTime stopOut;
	startOut.setRef(TimeFormat::TC_TIME_UTC);
	startOut.setFormat(TimeFormat::TC_ASCII_COMPACT);
	stopOut.setRef(TimeFormat::TC_TIME_UTC);
	stopOut.setFormat(TimeFormat::TC_ASCII_COMPACT);
	TimeConverterSingleton::Instance()->getConverter()->convert(startIn,startOut);
	TimeConverterSingleton::Instance()->getConverter()->convert(stopIn,stopOut);
		
	//PHASE is 1 chars length
	ostringstream filePhase;
	filePhase.fill('0');
	filePhase << setw(1) << phase;
	
	//CYCLE is 3 chars length
	ostringstream fileCycle;
	fileCycle.fill('0');
	fileCycle << setw(3) << cycle; // PRQA S 4400 L2
	
	//RELATIVE_ORBIT is 5 chars length
	ostringstream fileRelOrb;
	fileRelOrb.fill('0');
	fileRelOrb << setw(5) << relOrb;

	//ABSOLUTE_ORBIT is 5 chars length
	ostringstream fileAbsOrb;
	fileAbsOrb.fill('0');
	fileAbsOrb << setw(5) << absOrb;
	
	//COUNTER is 4 chars length
	ostringstream fileCount;
	fileCount.fill('0');
	fileCount << setw(4) << counter; // PRQA L:L2
	
	result = root + 												//PRODUCT_ID+PROC_STAGE_FLAG+ORIGINATOR_ID
			 startOut.getValue() + 									//START_DAY+"_"+START_TIME
			 "_" + 
			 getDuration(startOut.getValue(),stopOut.getValue()) +  //DURATION
			 filePhase.str() + 										//PHASE
			 fileCycle.str() +										//CYCLE
			 "_" +
			 fileRelOrb.str() +										//RELATIVE_ORBIT
			 "_" +
			 fileAbsOrb.str() +										//ABSOLUTE_ORBIT
			 "_" +
			 fileCount.str() +										//COUNTER
			 "." +
			 _missionId+											//SATELLITE_ID
			 fileExtension;											//file extension
}



string EnvisatProductFileName::getProductIDFromRoot(const string& fileName)
{
	if(!isValidRoot(fileName))
	{
		ACS_THROW(ProductFileNameException("Error: invalid Level0 Product File Name Root" + fileName)) ; // PRQA S 3081
	}	
	// return fileName.substr(0, 10);
	return fileName.substr(PN_FILETYPE_P,PN_FILETYPE_L);
}

string EnvisatProductFileName::getProductIDFromName(const string& fileName)
{
	if(!isValidName(fileName))
	{
		ACS_THROW(ProductFileNameException("Error: invalid Level0 Product File Name " + fileName)) ; // PRQA S 3081
	}	
	// return fileName.substr(0, 10);
	return fileName.substr(PN_FILETYPE_P,PN_FILETYPE_L);

}

///////////////////////////////////////////////////////////
// useful methods to extract info from complete file names
///////////////////////////////////////////////////////////

string EnvisatProductFileName::getVersion(const string& fileName)
{
	if(!isValidName(fileName))
	{
		ACS_THROW(ProductFileNameException("Error: invalid Product File Name " + fileName)) ; // PRQA S 3081
	}	
	// return fileName.substr(55, 4);
	return fileName.substr(PN_VERSION_P,PN_VERSION_L);
}

string EnvisatProductFileName::getFileClass(const string& fileName)
{
	if(!isValidName(fileName))
	{
		ACS_THROW(ProductFileNameException("Error: invalid Product File Name " + fileName)) ; // PRQA S 3081
	}	
	// return fileName.substr(10, 1);
	return fileName.substr(PN_FILECLASS_P, PN_FILECLASS_L);	
}

string EnvisatProductFileName::getFileClassFromRoot(const string& fileName)
{
	if(!isValidRoot(fileName))
	{
		ACS_THROW(ProductFileNameException("Error: invalid Product File Name Root " + fileName)) ; // PRQA S 3081
	}	
	return fileName.substr(PN_FILECLASS_P, PN_FILECLASS_L);	
}

string EnvisatProductFileName::getOriginatorFromRoot(const string& fileName)
{
	if(!isValidRoot(fileName))
	{
		ACS_THROW(ProductFileNameException("Error: invalid Product File Name Root " + fileName)) ; // PRQA S 3081
	}	
	return fileName.substr(11, 3);	// PRQA S 4400
}


long double EnvisatProductFileName::getStartTime(const string& fileName)
{
	if(!isValidName(fileName))
	{
		ACS_THROW(ProductFileNameException("Error: invalid Product File Name " + fileName)) ; // PRQA S 3081
	}	
	//convert ascii start time TC_ASCII_COMPACT in ProcessingTime
	// AsciiTime inTime(fileName.substr(14,15),TimeFormat::TC_TIME_UTC,TimeFormat::TC_ASCII_COMPACT);
	AsciiTime inTime(fileName.substr(PN_STARTIME_P,PN_STARTIME_L),TimeFormat::TC_TIME_UTC,TimeFormat::TC_ASCII_COMPACT);
	ProcessingTime outTime;
	outTime.setRef(TimeFormat::TC_TIME_UTC);
	outTime.setFormat(TimeFormat::TC_PROC);
	TimeConverterSingleton::Instance()->getConverter()->convert(inTime,outTime);

	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "getStartTime : return " << setprecision(14) << outTime.getValue() );
	return outTime.getValue();
}

long double EnvisatProductFileName::getStopTime(const string& fileName)
{
	if(!isValidName(fileName))
	{
		ACS_THROW(ProductFileNameException("Error: invalid Product File Name " + fileName)) ; // PRQA S 3081
	}	
	//get the start time converted in seconds
	long double startTimeInSeconds = getStartTime(fileName) * DAYSECONDS ;
	//get the duration in double format
	long double duration = 0.0;
	// 31 has to be changed to 30
	// istringstream istr(fileName.substr(31,8));
	istringstream istr(fileName.substr(PN_DURATION_P,PN_DURATION_L));
	istr >> duration;

	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "getStopTime : return " << setprecision(14) << ( (startTimeInSeconds + duration) / DAYSECONDS ) );
	
	return ( (startTimeInSeconds + duration) / DAYSECONDS );
}

string EnvisatProductFileName::nameWVersion(const string& fileName,const string& version)
{
	if(!isValidName(fileName))
	{
		ACS_THROW(ProductFileNameException("Error: invalid Product File Name " + fileName)) ; // PRQA S 3081
	}	
	if(version.size() > 4) // PRQA S 4400
	{
		ACS_THROW(ProductFileNameException("Error: invalid version '" + version+"'.Bad size")) ; // PRQA S 3081
	}
	//format the version in input
	ostringstream vers;
	vers << setw(4) << setfill('0') << version; // PRQA S 4400
	//build the new file name
	string root = fileName.substr(0, 55); // PRQA S 4400 
	// 56 has to be changed to 55
	// string oldVersion = fileName.substr(56, 4);
	string oldVersion = fileName.substr(PN_VERSION_P,PN_VERSION_L);
	//string rest = fileName.substr(59, 3);
	int toVersL = PN_VERSION_P+PN_VERSION_L; // PRQA S 3084
	int restL = fileName.length() - toVersL;  // PRQA S 3000, 3010, 3084 4
	int restP = toVersL;
	string rest = fileName.substr(restP, restL);
	return root+vers.str()+rest;
}



/////////////////////////////////////////////////////////////////////////////////////////
// Private methods 
/////////////////////////////////////////////////////////////////////////////////////////

/*
 * read data from rsResourceSet and set private attributes
 */ 
void EnvisatProductFileName::init()
{
	//storing MissionId
	_rsConf.getValue("ProductFileName.MissionID", _missionId);

	//storing OriginatorId
	ACSStationsWrapper statWrap ;
	vector<ACSStationsWrapper::StationInfo> info ;
	statWrap.getInfo(info) ;
    size_t i_size = info.size() ;
	for (unsigned int i=0; i < i_size; i++)
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
        size_t pc_size = _permittedClasses.size(); 
		for(unsigned int j=0; j<pc_size; ++j)
		{
            size_t po_size = _permittedOriginators.size() ;				
			for(unsigned int k=0; k<po_size; ++k)
			{				
				validName=productID + _permittedClasses[j] + _permittedOriginators[k];
				_validL0FileNames.push_back(validName);
				_validFileNames.push_back(validName);
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "added Valid L0 FileName Root " << validName );
			}
		}
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
        size_t pc_size = _permittedClasses.size(); 
		for(unsigned int j=0; j<pc_size; ++j)
		{				
            size_t po_size = _permittedOriginators.size() ;				
			for(unsigned int k=0; k<po_size; ++k) {
				validName=productID + _permittedClasses[j] + _permittedOriginators[k];
				_validL1FileNames.push_back(validName);
				_validFileNames.push_back(validName);
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "added Valid L1 FileName Root " << validName ) ;
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
        size_t pc_size = _permittedClasses.size(); 
		for(unsigned int j=0; j<pc_size; ++j)
		{				
            size_t po_size = _permittedOriginators.size() ;				
			for(unsigned int k=0; k<po_size; ++k) {
				validName=productID + _permittedClasses[j] + _permittedOriginators[k];
				_validBWSFileNames.push_back(validName);
				_validFileNames.push_back(validName);
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "added Valid Browse FileName Root " << validName ) ;
			}
		}
	}	
	//storing other general informations
	_rsConf.getValue("ProductFileName.FileNameVersion", _fileNameVersion);
	_rsConf.getValue("ProductFileName.FileRootSize", _rootSize);
	_rsConf.getValue("ProductFileName.FileNameSize", _fileSize);
} 

string EnvisatProductFileName::getDuration(const string& start, const string& stop)
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
	unsigned int dur =  ceil(( stopOut.getValue() - startOut.getValue() ) * DAYSECONDS ) ; // PRQA S 3011
	ostringstream duration;
	duration.fill('0');
	duration << setw(8) << dur; // PRQA S 4400
	return duration.str();
}


bool EnvisatProductFileName::checkNameValidity(const string& fileName, vector<string>& validRoots)
{
	bool result=false;
	//check if it's extension or missionId
	string ext = File::getExt(fileName);
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "checkNameValidity : Check extension '" << ext << "'");
	if( ext == _missionId)
	{
		//the input filename has not extension
		if(fileName.size() == _fileSize)
		{
			string root = fileName.substr(0, _rootSize);
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "checkNameValidity : FILE ROOT is " << root ) ;
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
		    	   << "' of size " << fileWithoutExt.size() << "[ valid file size is " << _fileSize << " ]" ) ;
	
		if ( fileWithoutExt.size() == _fileSize)
		{
			string root = fileName.substr(0, _rootSize);
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




bool EnvisatProductFileName::checkRootValidity(const string& fileName, vector<string>& validRoots)
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



_ACS_END_NAMESPACE

