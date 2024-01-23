// PRQA S 1050 EOF
/*
	Copyright 2015, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libDataStorage Library $

	$Id$

	$Author $
	
    $Log$
    Revision 5.12  2018/10/02 15:24:28  marpas
    inline missing in template

    Revision 5.11  2016/12/22 16:00:02  lucio.pulvirenti
    S2PDGS-1643:  buildMediumId: destination centre id now optional. SenderReportItem class and related rsResourceSet get method added.

    Revision 5.10  2016/03/18 12:53:54  marpas
    debug variable definition moved into source file

    Revision 5.9  2015/10/05 15:59:06  lucio.pulvirenti
    S2PDGS-1249: getAcknowledgeReportName method implemented. getReportPath method removed.

    Revision 5.8  2015/10/05 09:56:07  lucio.pulvirenti
    S2PDGS-1247: getSendingReportPath substituted with getSendingReportName method.

    Revision 5.7  2015/10/02 16:06:07  lucio.pulvirenti
    S2PDGS-1247/1249: getSendingReportPath method added to IF. ACS string removed from mediumIdPrefix variable.

    Revision 5.6  2015/10/02 11:05:11  lucio.pulvirenti
    S2PDGS-1247/1249: standard debug macros used. DigestTree_md5 new IF used not to check files permissions when computing md5sum.

    Revision 5.5  2015/09/16 16:00:35  lucio.pulvirenti
    S2PDGS-1247 & S2PDGS-1249: method added to compute md5sum. directory are skipped: fake signature returned.

    Revision 5.4  2015/09/04 16:03:33  lucio.pulvirenti
    S2PDGS-1247 && S2PDGS-1249: temporary path now written in /tmp dir. If renamed fails, temporary path is saved.

    Revision 5.3  2015/08/14 15:46:13  lucio.pulvirenti
    S2PDGS-1247, S2PDGS-1249: buildMediumId method added to IF. medium id prefix now const

    Revision 5.2  2015/08/14 14:36:29  lucio.pulvirenti
    S2PDGS-1249: getTempPath(), getMcStatusTag(), getMcAlertTag() method added.

    Revision 5.1  2015/08/13 15:30:47  lucio.pulvirenti
    S2PDGS-1249: MediaCirculationMngFunctions class added.



*/


#ifndef _MediaCirculationMngFunctions_H_
#define _MediaCirculationMngFunctions_H_

#include <rsResourceSet.h>
#include <ftmpstream.h>
#include <File.h>
#include <Filterables.h>
#include <exException.h>

/**
 * \brief
 *  Collection of functions needed to Media Circulation Management Tasks 
 **/

_ACS_BEGIN_NAMESPACE(acs)


class MediaCirculationMngFunctions  { 

public:
		
    exDECLARE_EXCEPTION(MediaCirculationMngFunctionsException, exException) ; // Base Exception. // PRQA S 2131, 2502
	
    struct SenderReportItem {

	    SenderReportItem() ;
	    ~SenderReportItem() throw() {} 
	    SenderReportItem(const SenderReportItem& ) ;
	    SenderReportItem& operator=(const SenderReportItem& ) ;

	    std::string itemId;
	    std::string itemPath;
	    std::string md5Signature;
    } ;
				

	/*! \brief  

	  \param mountPoint  : mount point 
	  \param unique : if true, only one medium id dir must occur in mount point
	  \param mediumIdV : OUT medium Id vector.
	  \throw      - MediaCirculationMngFunctions::MediaCirculationMngFunctionsException if unique is true and more than one media id occur 
            	  - exception if some error occurs

	*/
	static void getMediumIdFromMountPoint(const std::string & mountPoint, bool unique, std::vector<std::string> & mediumIdV );
	
	static std::string getSanitizedMountPoint( const std::string & mountPoint );
	
	static std::string getFilenameFromMountPoint( const std::string & mountPoint );
	
	static std::string getAcknowledgeReportName( const std::string & mediumId, const std::string & centreId );
	static std::string getSendingReportName( const std::string & mediumId );
	
	static std::string getTempPath();
	
	static std::string getMcStatusTag();
	static std::string getMcAlertTag();
	
	static std::string buildMediumId(const std::string & mountPoint, const std::string & senderCentreId, const std::string & destCentreId, const std::string & destinationHost);

    
	template <typename T>
	static void writeResultOnFile(const std::string outfilePath, const T & value)
	{
		ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
		
		const std::string prefix = getTempPath() + "/MediaCirculationMngOutput_";
		oftmpstream of(prefix);
		std::string tmpFilePath = of.path();
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, '\"' << tmpFilePath << "\" temporary file generated to write <" << value << "> result") ;

		of() << value;

		try {
			File::rename(tmpFilePath, outfilePath);
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, '\"' << outfilePath << "\" output file successfully written") ;
		}
		catch(std::exception &e)
		{
			of.setAutoRemove(false);
			throw;
		}

	}
	
	static std::string computeMd5Checksum(const std::string & );
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(MediaCirculationMngFunctions) ;


		
};

template<>
inline
void rsResourceSet::getValue(const std::string &res, MediaCirculationMngFunctions::SenderReportItem &v) const 
{
    Lock lock(_mutex) ;
	getValue(res+".ItemId")->get(v.itemId) ;
	getValue(res+".ItemPath")->get(v.itemPath) ;
	getValue(res+".MD5Signature")->get(v.md5Signature) ;
	
} 

std::ostream &operator<<(std::ostream &, const MediaCirculationMngFunctions::SenderReportItem &) ;

acs::exostream &operator<<(acs::exostream &, const MediaCirculationMngFunctions::SenderReportItem &) ;


_ACS_END_NAMESPACE

#endif // _MediaCirculationMngFunctions_H_
