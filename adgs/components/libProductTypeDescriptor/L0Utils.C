// PRQA S 1050 EOF
/*

	Copyright 2002-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Product Type Descriptor Library $

	$Id$

	$Author$

	$Log$
	Revision 2.4  2013/12/09 14:01:12  giucas
	Library completely revisited.
	
	Revision 2.3  2012/03/12 10:19:19  giucas
	Some compile warnings fixed
	
	Revision 2.2  2010/09/03 09:59:42  marpas
	removed useless or redundant methods
	In the header file there are suggestions how to replace them
	
	Revision 2.1  2008/07/25 12:53:51  giucas
	-f option added to mv and cp commands
	
	Revision 2.0  2006/02/28 09:31:26  marpas
	Exception 2.1 I/F adopted
	

*/

#include <L0Utils.h>
#include <sstream>
#include <ConfigurationSingleton.h>
#include <fcntl.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(L0Utils) ;
namespace {
    const string _sshKey = "L0Utils.UseSSH";
}

bool L0Utils::useSSH()
{
	// Check if the UseSSH key is enabled
	// if no key is found the default is to NOT USE ssh without complaining
	// so the return value will be "false"
	bool retval = false;
	rsResourceSet& rs=ConfigurationSingleton::instance()->get();
	if (rs.hasKey (_sshKey)) {
		rs.getValue (_sshKey, retval);
	}

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "SSH usage is " << (retval ? "REQUIRED" : "NOT REQUIRED")  ); // PRQA S 3380
	
	return retval;	
}


/*
 * rcp or scp file
 * 
 */
int L0Utils::copyFromRemoteDIS(const string& remoteFileName, const string& remAddress, const string& dest, const int retry)
{	
	string command;
	if (L0Utils::useSSH()) {
		command = "scp -p " + remAddress + ":" + remoteFileName + " " + dest; 
	}
	else {
		command = "rcp -p " + remAddress + ":" + remoteFileName + " " + dest; 
	}
		
		
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "cmd: " << command );

	int returnVal = system(command.c_str());
	
	int tries=0;
	while((0 != returnVal) && (tries < retry)) 
	{ 
		tries++;
		ACS_LOG_INFO("copyFromRemoteDIS: remote copy NOT OK : waiting 1 sec and retrying [" << tries << "] ...." );
		sleep(1);
		returnVal = system(command.c_str());
	}
	
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method executed with result value " << returnVal) ;
	return returnVal;
}

/*
 * ping remote DIS
 * return value : 1 if remote DIS responds
 *				 -1 if not
 */
int L0Utils::pingRemoteDIS(const string& remAddress)
{
	int result = -1;
	
	string command = "ping -c 5 -w 5 " + remAddress; 

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "cmd: " << command );

	int returnVal = system(command.c_str());

	if (returnVal == 0) {
		result = 1;
	}
		
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method executed with result value " << returnVal) ;
	
	return result;
}


/*
 * convert Product Header Time format in XML Product Header time format
 */
string L0Utils::convertTime(const string& timeToConvert)
{
	string convertedTime = "";
	
	string day = timeToConvert.substr(0,2);
	string year = timeToConvert.substr(7,4);
	ostringstream os ;
	os << setw(2) << setfill('0') << DateTime::month2Num(timeToConvert.substr(3,3)) ; // PRQA S 4400 3
	string digMont = os.str() ;
	
	convertedTime = year + "-" + digMont + "-" + day + "T" + timeToConvert.substr(12,timeToConvert.size()-12); // PRQA S 3084
	return convertedTime;
}

bool L0Utils::createDISLockingFile(int& syncFd, const string& syncFileName)
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method called for FD " << syncFd << " to create file " << syncFileName );
	bool result=true;
	///////////////////////////////
	//open the Syncronization file
	///////////////////////////////
	if((syncFd = open(syncFileName.c_str(), O_CREAT|O_WRONLY, 0666)) >= 0) // PRQA S 4400
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Synchronization file " << syncFileName << " successfully opened..." );
		//lock the Syncronization file
		if( lockf(syncFd, F_TLOCK, 50) >=0 ) // PRQA S 4400
		{
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Synchronization file " << syncFileName << " successfully locked..." );
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "ERROR cannot lock synchronization file "
					<< syncFileName << " : Unable to check correctly DIS Synchronization" );
			result=false;			
		}						
		/////////////////////////////////////////////////
		//write Creation time in MJD2000 before locking
		/////////////////////////////////////////////////
		DateTime::MJD2000 mjd2000 = DateTime() ; // PRQA S 3050
		long double currentMJD2000Time = mjd2000.jd();
		ostringstream t;
		t << setprecision(17) << currentMJD2000Time; // PRQA S 4400
		if( write(syncFd , t.str().c_str(), t.str().size() ) != ssize_t(t.str().size())) // PRQA S 3000, 3081
		{
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Unable to write current time [" << t.str() << "] to file " << syncFileName ) ;
			result=false;
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Current creation time [" << t.str() << "] for file " << syncFileName << " written" );
		}
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "ERROR cannot open synchronization file "
								<< syncFileName << " : Unable to check correctly DIS Synchronization" );
		result=false;
	}
	return result;
}

long double L0Utils::checkDISLockingFileDate(const std::string& path, const std::string& name)
{
	string fileName = path + "/" + name;

	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "method called to check date of file " << fileName);

	char tmp[128]; // PRQA S 4403
	ifstream fileStr(fileName.c_str());
	if(not fileStr.good())
	{
		ACS_THROW(exFileOpenException("Error in opening file "+fileName)) ; // PRQA S 3081
	}
	fileStr.getline(tmp,sizeof(tmp)); 
	fileStr.close();
	double remDate = atof(tmp);
	DateTime::MJD2000 mjd2000 = DateTime() ; // PRQA S 3050
	ACS_LOG_INFO("current time is " << setprecision(17) << mjd2000.jd() // PRQA S 4400 5
			<< " and Remote File Time is " << setprecision(17) << remDate << " -> Diff in minutes is " 
			<< ( (mjd2000.jd() - remDate) * 1440.0 ) );
	
	return ( (mjd2000.jd() - remDate) * 1440.0 );
}
_ACS_END_NAMESPACE

