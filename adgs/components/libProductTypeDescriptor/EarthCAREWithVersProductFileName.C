// PRQA S 1050 EOF
/*

	Copyright 2017, Advanced Computer Systems , Inc.
	Via della Bufalotta, 378 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$
	
	$Log$
	
	

*/

#include <EarthCAREWithVersProductFileName.h>
#include <File.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

namespace {

	const size_t versionLength = 4;
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(EarthCAREWithVersProductFileName) ;

//Class constructor
EarthCAREWithVersProductFileName::EarthCAREWithVersProductFileName() :
    ProductFileName()
{
}


//Class destructor
EarthCAREWithVersProductFileName::~EarthCAREWithVersProductFileName() throw()
{
}

/////////////////////////////////////////////////////////////////////////////////////////
// Public methods 
/////////////////////////////////////////////////////////////////////////////////////////
void EarthCAREWithVersProductFileName::getNamesById(const string& prodId, 
										   vector<string>& names)
{
	ACS_THROW( ProductFileNameException("Method not implemented") );
}

void EarthCAREWithVersProductFileName::getNamesById(const string& prodId, 
								      const string& startTime,
								      const string& stopTime,
								  	  vector<string>& names)
{
	ACS_THROW( ProductFileNameException("Method not implemented") );
}



void EarthCAREWithVersProductFileName::getNamesByMission(const string& missionId, const string& prodId, vector<string>& names)
{
	ACS_THROW( ProductFileNameException("Method not implemented") );
}

void EarthCAREWithVersProductFileName::getNamesByMission(const string& missionId,
										   const string& prodId, 
									       const string& startTime,
								  	   	   const string& stopTime,
									   	   vector<string>& names)
{
	ACS_THROW( ProductFileNameException("Method not implemented") );
}

void EarthCAREWithVersProductFileName::getNamesByExtension(const string& ext,const string& prodId,vector<string>& names)
{
	ACS_THROW( ProductFileNameException("Method not implemented") );
}

void EarthCAREWithVersProductFileName::getNamesByExtension(const string& ext,
											 const string& prodId, 
										 	 const string& startTime,
								  	     	 const string& stopTime,
										 	 vector<string>& names)
{
	ACS_THROW( ProductFileNameException("Method not implemented") );
}		


bool EarthCAREWithVersProductFileName::isValidExtension(const string& ext) 
{
	ACS_THROW( ProductFileNameException("Method not implemented") );
}


bool EarthCAREWithVersProductFileName::isValidName(const string& fileName)
{
	size_t d;
	return checkNameValidity(fileName,d);
}

bool EarthCAREWithVersProductFileName::isValidRoot(const string& fileName)
{
	ACS_THROW( ProductFileNameException("Method not implemented") );
}

bool EarthCAREWithVersProductFileName::isValidL0Name(const string& fileName)
{
	ACS_THROW( ProductFileNameException("Method not implemented") );
}

bool EarthCAREWithVersProductFileName::isValidL0Root(const string& fileName)
{
	ACS_THROW( ProductFileNameException("Method not implemented") );
}

bool EarthCAREWithVersProductFileName::isValidL1Name(const string& fileName)
{
	ACS_THROW( ProductFileNameException("Method not implemented") );
}

bool EarthCAREWithVersProductFileName::isValidL1Root(const string& fileName)
{
	ACS_THROW( ProductFileNameException("Method not implemented") );
}


//build a filename from root, extension, start and stop dates
//It is made NO control about dates correctness!!!!!!!!!!!!!!
void EarthCAREWithVersProductFileName::getName(const string& root, 
			 					 const string& startDate, 
			 					 const string& stopDate, 
			 					 const string& ext, 
			 					 string& result)
{
	ACS_THROW( ProductFileNameException("Method not implemented") );
}

void EarthCAREWithVersProductFileName::getName(const string& root, 
								 double start, 
								 double stop, 
								 const string& ext, 
								 string& result)
{
	ACS_THROW( ProductFileNameException("Method not implemented") );
}

void EarthCAREWithVersProductFileName::getName(const string& root, 
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
	ACS_THROW( ProductFileNameException("Method not implemented") );
}

void EarthCAREWithVersProductFileName::getName(const string& root, 
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
	ACS_THROW( ProductFileNameException("Method not implemented") );
}



string EarthCAREWithVersProductFileName::getProductIDFromRoot(const string& fileName)
{
	ACS_THROW( ProductFileNameException("Method not implemented") );
}

string EarthCAREWithVersProductFileName::getProductIDFromName(const string& fileName)
{
	ACS_THROW( ProductFileNameException("Method not implemented") );
}

///////////////////////////////////////////////////////////
// useful methods to extract info from complete file names
///////////////////////////////////////////////////////////

string EarthCAREWithVersProductFileName::getVersion(const string& fileName)
{
	size_t versionStart = 0;
	checkNameValidity(fileName, versionStart);

	return fileName.substr(versionStart, versionLength);
}

string EarthCAREWithVersProductFileName::getFileClass(const string& fileName)
{
	ACS_THROW( ProductFileNameException("Method not implemented") );
}

string EarthCAREWithVersProductFileName::getFileClassFromRoot(const string& fileName)
{
	ACS_THROW( ProductFileNameException("Method not implemented") );
}


long double EarthCAREWithVersProductFileName::getStartTime(const string& fileName)
{
	ACS_THROW( ProductFileNameException("Method not implemented") );
}

long double EarthCAREWithVersProductFileName::getStopTime(const string& fileName)
{
	ACS_THROW( ProductFileNameException("Method not implemented") );
}

string EarthCAREWithVersProductFileName::getDefaultFileNameVersion()
{
	ACS_THROW( ProductFileNameException("Method not implemented") );
}

string EarthCAREWithVersProductFileName::getMissionID()
{
	ACS_THROW( ProductFileNameException("Method not implemented") );
}

string EarthCAREWithVersProductFileName::getOriginatorFromRoot(const std::string&)
{
	ACS_THROW( ProductFileNameException("Method not implemented") );
}


string EarthCAREWithVersProductFileName::nameWVersion(const string& fileName,const string& version)
{
	
	size_t versionStart = 0;
	checkNameValidity(fileName, versionStart);
	
	if( version.length() != versionLength ) {
		ostringstream exMsg;
		exMsg << '\"' << version << "\" version lenght is " << version.length() << " chars long (" << versionLength << " expected)";
		ACS_THROW( ProductFileNameException(exMsg.str()) );
	}
	
	string newFileName = fileName;
	newFileName.replace(versionStart, versionLength, version);
	return newFileName;
}


/////////////////////////////////////////////////////////////////////////////////////////
// Private methods 
/////////////////////////////////////////////////////////////////////////////////////////


bool EarthCAREWithVersProductFileName::checkNameValidity(const string& fileName, size_t & versionFieldStartPos)
{
	
	string filenameRoot = File::removeExt(fileName)	;
	// compute new name
	size_t pos = filenameRoot.find_last_of('_');
	ACS_COND_THROW( pos == string::npos, ProductFileNameException("Cannot find '_' character in \""+fileName+"\" filename") );
	ACS_COND_THROW( (filenameRoot.length() - pos) == 1, ProductFileNameException("Wrong name format of \""+fileName+"\" filename") ); // it ends with '_'
	
	versionFieldStartPos = pos+1;
	size_t filenameVersionLen = filenameRoot.length() - versionFieldStartPos;
	if( filenameVersionLen != versionLength ) {
		ostringstream exMsg;
		exMsg << "Version in \"" << fileName << "\" filename is " << filenameVersionLen << " chars long (" << versionLength << " expected)";
		ACS_THROW( ProductFileNameException(exMsg.str()) );
	}
	
	return true;
}




_ACS_END_NAMESPACE

