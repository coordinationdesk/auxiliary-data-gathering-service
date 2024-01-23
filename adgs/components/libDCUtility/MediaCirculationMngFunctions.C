// PRQA S 1050 EOF
/*
	Copyright 2013-2015, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libDataStorage Library $

	$Id$

	$Author$
	
    $Log$
    Revision 5.13  2016/03/18 12:53:54  marpas
    debug variable definition moved into source file

    Revision 5.12  2015/10/23 07:56:27  marpas
    some info filterables changed into private info

    Revision 5.11  2015/10/05 15:59:06  lucio.pulvirenti
    S2PDGS-1249: getAcknowledgeReportName method implemented. getReportPath method removed.

    Revision 5.10  2015/10/05 09:56:07  lucio.pulvirenti
    S2PDGS-1247: getSendingReportPath substituted with getSendingReportName method.

    Revision 5.9  2015/10/02 16:06:07  lucio.pulvirenti
    S2PDGS-1247/1249: getSendingReportPath method added to IF. ACS string removed from mediumIdPrefix variable.

    Revision 5.8  2015/10/02 11:05:11  lucio.pulvirenti
    S2PDGS-1247/1249: standard debug macros used. DigestTree_md5 new IF used not to check files permissions when computing md5sum.

    Revision 5.7  2015/10/02 09:10:17  lucio.pulvirenti
    S2PDGS-1247/1249: md5sum on directory now performed by a dedicated class. md5sum on files algo put into libFile File class.

    Revision 5.6  2015/09/17 10:00:20  lucio.pulvirenti
    S2PDGS-1247: md5sum fake directory code changed.

    Revision 5.5  2015/09/16 16:00:35  lucio.pulvirenti
    S2PDGS-1247 & S2PDGS-1249: method added to compute md5sum. directory are skipped: fake signature returned.

    Revision 5.4  2015/09/01 12:39:33  lucio.pulvirenti
    S2PDGS-1247 & S2PDGS-1249: Report filetypes moved into the proper nodes for both Sender and Receiver.

    Revision 5.3  2015/08/14 15:46:13  lucio.pulvirenti
    S2PDGS-1247, S2PDGS-1249: buildMediumId method added to IF. medium id prefix now const

    Revision 5.2  2015/08/14 14:36:29  lucio.pulvirenti
    S2PDGS-1249: getTempPath(), getMcStatusTag(), getMcAlertTag() method added.

    Revision 5.1  2015/08/13 15:30:47  lucio.pulvirenti
    S2PDGS-1249: MediaCirculationMngFunctions class added.



*/
#include <MediaCirculationMngFunctions.h>

#include <ConfigurationSingleton.h>
#include <DigestTree_md5.h> 
#include <StringUtils.h>
#include <rsResourceSet.h>
#include <RegExp.h>
#include <Dir.h>
#include <DateTime.h>


using namespace std;

_ACS_BEGIN_NAMESPACE(acs)


namespace { 
    
class MediaCirculationMngConf {
public:
    MediaCirculationMngConf() throw()
    {    
        try {	   
            ConfigurationSingleton::instance() -> addSpace (ConfigurationSingleton::ConfSpace ("MediaCirculationMng", "", false), ConfigurationSingleton::NoLoad);
            ConfigurationSingleton::instance() -> addCallback (MediaCirculationMngConfCallback, this);
        }
        catch(exception &x) {
            ACS_LOG_NOTIFY_EX( x) ;
        }
    }

	static void MediaCirculationMngConfCallback (void *)
	{
    	//default message
    	if ( not _messageSent ) {
        	_messageSent = true;

        	rsResourceSet &conf = ConfigurationSingleton::instance()->get() ;

            conf.getValue ("MediaCirculationMng.Sender.ReportFiletype", _senderReportFiletype);
            ACS_LOG_PRIVINFO( "SenderReportFiletype set to \"" << _senderReportFiletype <<'\"');

            conf.getValue ("MediaCirculationMng.Receiver.ReportFiletype", _receiverReportFiletype);
            ACS_LOG_PRIVINFO( "ReceiverReportFiletype set to \"" << _receiverReportFiletype <<'\"');

			conf.getValue ("MediaCirculationMng.SatelliteId", _satelliteId);
            ACS_LOG_PRIVINFO( "SatelliteId set to \"" << _satelliteId <<'\"');

			if (not conf.hasKey ("MediaCirculationMng.TempPath")) {
                ACS_LOG_PRIVINFO( "Configuration key MediaCirculationMng.TempPath not found. Using default: \"" << _tempPath << '\"');
            }
            else {
                conf.getValue ("MediaCirculationMng.TempPath", _tempPath);
				ACS_LOG_PRIVINFO( "TempPath set to \"" << _tempPath <<'\"');
            }

	   }
	}

	string getSenderReportFiletype () 
	{ 
		ACS_COND_THROW(_senderReportFiletype.empty(), exIllegalValueException("SenderReportFiletype not valorized") );
		return _senderReportFiletype; 
	}

	string getReceiverReportFiletype () 
	{ 
		ACS_COND_THROW(_receiverReportFiletype.empty(), exIllegalValueException("ReceiverReportFiletype not valorized") );
		return _receiverReportFiletype; 
	}

	string getSatelliteId () 
	{ 
		ACS_COND_THROW(_satelliteId.empty(), exIllegalValueException("SatelliteId not valorized") );
		return _satelliteId; 
	}

	string getTempPath () { return _tempPath; }


private:
	static bool _messageSent;
	static string _senderReportFiletype;
	static string _receiverReportFiletype;
	static string _satelliteId;
	static string _tempPath;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(MediaCirculationMngConf);

};
	
	ACS_CLASS_DEFINE_DEBUG_LEVEL(MediaCirculationMngConf) ;


	bool MediaCirculationMngConf::_messageSent = false;
	string MediaCirculationMngConf::_senderReportFiletype;
	string MediaCirculationMngConf::_receiverReportFiletype;
	string MediaCirculationMngConf::_satelliteId;
	string MediaCirculationMngConf::_tempPath = "/tmp";


	MediaCirculationMngConf embeddedMediaCirculationMngConf; 
	
	const string mcStatusTag = "MEDIA_CIRC_CTRL";
	const string mcAlertTag = "MEDIA_CIRC_ALERT";
	const string mediumIdPrefix = "MEDIA_CIRC_MNG";


} //namespace


ACS_CLASS_DEFINE_DEBUG_LEVEL(MediaCirculationMngFunctions)


void MediaCirculationMngFunctions::getMediumIdFromMountPoint(const string & mountPoint, bool unique, vector<string> & mediumIdV )
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	
	mediumIdV.clear();
	
	RegExp re;
	string rexpStr(mediumIdPrefix + "_[0-9]{4}[0-1][0-9][0-3][0-9][0-2][0-9][0-5][0-9][0-5][0-9]");  // to find dates e.g. 20150712082266
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Regexp to find=\"" << rexpStr << '\"');
	re.setReg(rexpStr);

	Dir mpDir(mountPoint);
	size_t dirSize = mpDir.size();
	for( size_t i = 0; i < dirSize; i++ ) {
		string item = mountPoint + '/' + mpDir[i];
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Going to check \""  << item << "\" item");
		if( File::isDir(item) && re.match(mpDir[i]) ) {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, '\"'  << mpDir[i] << "\" is a medium Id");
			ACS_COND_THROW( not mediumIdV.empty() && unique, MediaCirculationMngFunctionsException("Multiple media id found in \"" + mountPoint + "\" mount point") );
			mediumIdV.push_back(item) ;
		}
		
	}
	
}

string MediaCirculationMngFunctions::getSanitizedMountPoint( const string & mountPoint )
{
	return ('/' + StringUtils::trim(mountPoint, '/') );
}

string MediaCirculationMngFunctions::getFilenameFromMountPoint( const string & mountPoint )
{
	string ret = getSanitizedMountPoint(mountPoint);
	return StringUtils::transform( ret.substr(1), "/", "_");
}



string MediaCirculationMngFunctions::getSendingReportName(const string & mediumId)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	
	string reportName = embeddedMediaCirculationMngConf.getSatelliteId() + '_' + embeddedMediaCirculationMngConf.getSenderReportFiletype() + '_' + mediumId + ".xml";
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Report name=\"" << reportName << '\"');
	return reportName;
}

string MediaCirculationMngFunctions::getAcknowledgeReportName(const string & mediumId, const string & centreId)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	
	size_t pos = mediumId.find(mediumIdPrefix);
	ACS_COND_THROW( string::npos == pos, MediaCirculationMngFunctionsException("Cannot find \""+mediumIdPrefix+"\" string in \""+mediumId+"\" medium id") );
	
	size_t startPos = pos + mediumIdPrefix.length() + 1;
	string validityStartFromMediumId = mediumId.substr(startPos, (DateTime().YYYYMMDDhhmmss()).length());
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Validity start time from medium id=\"" << validityStartFromMediumId << '\"');
	string validityStart = validityStartFromMediumId.substr(0, 8) + 'T' + validityStartFromMediumId.substr(8, 6);

// S2__OPER_REP_DCACK__SSSS_YYYYMMDDTHHMMSS_SYYYYMMDDTHHMMSS
	string reportName = embeddedMediaCirculationMngConf.getSatelliteId() + "_OPER_" + embeddedMediaCirculationMngConf.getReceiverReportFiletype() + '_' + centreId + '_' + DateTime().YYYYMMDDThhmmss() + "_S" + validityStart;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Report name=\"" << reportName << '\"');
	return reportName;
}

string MediaCirculationMngFunctions::getTempPath()
{
	return embeddedMediaCirculationMngConf.getTempPath();
}

string MediaCirculationMngFunctions::getMcStatusTag()
{
	return mcStatusTag;
}

string MediaCirculationMngFunctions::getMcAlertTag()
{
	return mcAlertTag;
}

string MediaCirculationMngFunctions::buildMediumId(const string & mountPoint, const string & senderCentreId, const string & destCentreId, const string & destinationHost)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

	ACS_COND_THROW(mountPoint.empty(), MediaCirculationMngFunctionsException("mount point not valorized") );
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "mount point: \"" << mountPoint << '\"' );
	
// to discriminate 2 instances of the same having the same parameters (e.g. hostname, etc.), stick filename from mount point: the mount point cannot be the same
	string mediumId = mediumIdPrefix + '_' + DateTime().YYYYMMDDhhmmss() + '_' + senderCentreId + '_' + ( destCentreId.empty() ? "" : destCentreId + '_' ) + destinationHost + '_' + getFilenameFromMountPoint( mountPoint) ;
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "mediumId: \"" << mediumId << '\"' );
	return mediumId;

}

string MediaCirculationMngFunctions::computeMd5Checksum(const string& filename)  // PRQA S 4020
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	string signature;
	if( File::isDir(filename) ) {
	
		DigestTree_md5 digest;
		digest.setObject(filename, false) ;
		signature = digest.getObjectID();
	}
	else {
		signature = File::computeMd5Sum(filename);
	}
			
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Md5 CheckSum of \"" << filename << '\"' << ( File::isDir(filename) ? " directory=" : " file=" ) << signature );

	return  signature;

}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////******** SenderReportItem struct ctor, copy ctor and assignment operator **********/////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MediaCirculationMngFunctions::SenderReportItem::SenderReportItem():
										itemId(),
										itemPath(),
										md5Signature()
{
}								

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MediaCirculationMngFunctions::SenderReportItem::SenderReportItem(MediaCirculationMngFunctions::SenderReportItem const &o):
								itemId(o.itemId),
								itemPath(o.itemPath),
								md5Signature(o.md5Signature)
{
}								

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MediaCirculationMngFunctions::SenderReportItem & MediaCirculationMngFunctions::SenderReportItem::operator = (MediaCirculationMngFunctions::SenderReportItem const &o)
{
	if( this != &o ) {
		itemId=o.itemId;
		itemPath=o.itemPath;
		md5Signature=o.md5Signature;
	}
	return (*this);
}								


ostream &operator<<(ostream &s, const MediaCirculationMngFunctions::SenderReportItem & v)
{
	s	<< "****************** SenderReportItem ******************\n"
		<< "\tItemId=\"" 		<< v.itemId 				<< "\"\n"
	  	<< "\tItemPath=\"" 		<< v.itemPath 				<< "\"\n"
		<< "\tMD5Signature=\""	<< v.md5Signature 			<< "\"\n"
		<< "******************************************************\n";
	return s ;
}

exostream &operator<<(exostream &s, const MediaCirculationMngFunctions::SenderReportItem & v)
{
	s	<< "****************** SenderReportItem ******************\n"
		<< "\tItemId=\"" 		<< v.itemId 				<< "\"\n"
	  	<< "\tItemPath=\"" 		<< v.itemPath 				<< "\"\n"
		<< "\tMD5Signature=\""	<< v.md5Signature 			<< "\"\n"
		<< "******************************************************\n";
	return s ;
}


_ACS_END_NAMESPACE

