/*
	Copyright 1995-2012, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libPackageProcessing$

	$Id$

	$Author$

	$Log$
	Revision 1.12  2016/12/16 16:56:01  nicvac
	S3MPM-860: PackageExporter resources improvement.
	
	Revision 1.11  2016/05/10 14:44:56  marfav
	APF-376 removing references to the ExpWaitingAckDataPackage structure
	ExpProcInfo is systematically wrote in the database
	
	Revision 1.10  2013/06/17 13:56:44  nicvac
	New macro used.
	
	Revision 1.9  2013/03/21 14:44:27  nicvac
	S2PDGS-186: handling parameters from t_distributionrules.parameters.
	
	Revision 1.8  2013/03/20 17:13:59  nicvac
	minor. load dump signature.
	
	Revision 1.7  2013/03/15 10:36:27  nicvac
	S2PDGS-186
	ADC-83
	WaitingAcknowledge Step implemented
	
	Revision 1.6  2013/03/11 17:32:16  nicvac
	More than one Circulation id supported
	
	Revision 1.5  2013/02/22 17:38:22  nicvac
	S2PDGS-186: mail notification implemented.
	
	Revision 1.4  2013/02/07 16:03:35  nicvac
	load and dump for serializable objects.
	
	Revision 1.3  2012/12/12 16:17:19  nicvac
	Ref to distribution and datapackage file.
	
	Revision 1.2  2012/11/23 17:35:45  nicvac
	Implementation.
	
	Revision 1.1  2012/11/22 18:06:09  nicvac
	Implementation.
	
        
*/

#ifndef _ExpProcInfo_H_
#define _ExpProcInfo_H_

#include <exException.h>

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/utility.hpp>

#include <DataPackage.h>
#include <DistributionSerializable.h>

namespace acs {

/**
* \brief
* Processing parameter file: an input/output file used by processing tasks. It is used by tasks to comunicate each other the parameters. 
* Contains:
* - The datapackage absolute path
* - The working dir
* - The cache absolute path name
* - The list of additional cache's items to be released after the processing ends
* - ...
**/

typedef enum { 
	ExpProcInfoDistributionNONE=-1, 	//NOT DEFINED
	ExpProcInfoDistributionCompleted, 	//The Package has been distributed
	ExpProcInfoDistributionAccepted		//The Package has been accepted and will be distributed (asynch distribution)
} ExpProcInfoDistributionStatus;

class ExpProcInfo  { 

	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) {
		ar &
			BOOST_SERIALIZATION_NVP(_datapackageAbsPath) &     
			BOOST_SERIALIZATION_NVP(_workingDir) &
			BOOST_SERIALIZATION_NVP(_datapackageFilename) &
			BOOST_SERIALIZATION_NVP(_datapackageContent) &
			BOOST_SERIALIZATION_NVP(_distributionFilename) &
			BOOST_SERIALIZATION_NVP(_distributionContent) &
			BOOST_SERIALIZATION_NVP(_parametersFilename) &
			BOOST_SERIALIZATION_NVP(_cacheDownloadRepository) &
			BOOST_SERIALIZATION_NVP(_cacheAvailableDownloadSpaceSize) &
			BOOST_SERIALIZATION_NVP(_cacheItemsToRelease) &
			BOOST_SERIALIZATION_NVP(_circulationIds) &
			BOOST_SERIALIZATION_NVP(_distributionStatus) & 
			BOOST_SERIALIZATION_NVP(_notificationListOfFiles) & 
			BOOST_SERIALIZATION_NVP(_notificationDeliverySlipFilename) & 
			BOOST_SERIALIZATION_NVP(_notificationContentFilename) &
			BOOST_SERIALIZATION_NVP(_packageNames);
	}


public:

	/** ExpProcInfo Exceptions */
	exDECLARE_EXCEPTION(exExpProcInfoException, exException) ; // Base ExpProcInfo Exception.
	exDECLARE_EXCEPTION(exExpProcInfoCriticalException, exExpProcInfoException) ; // Critical Exception.


	/** The Task in charge of the distribution declares the Distribution status */


	/** Default Class constructor */
	ExpProcInfo();
	/** Destructor */
	virtual ~ExpProcInfo();

	/** Dump the Object */
	void dump(std::string const& filename) const;
	void dump(std::ostream& oss) const;

	/** Load the Object */
	void load(std::string const& filename);
	void load(std::istream& iss);
	

	/** Getter */
	std::string const& getDatapackageAbsPath() const;
	std::string const& getWorkingDir() const;
	std::string const& getDatapackageFilename() const;
	dataprovider::DataPackage const& getDatapackageContent() const;
	std::string const& getDistributionFilename() const;
	DistributionSerializable const& getDistributionContent() const;
	std::string const& getParametersFilename() const;
	std::string const& getCacheDownloadRepository() const;
	off_t const& getCacheAvailableDownloadSpaceSize() const;
	std::vector<std::string> const& getCacheItemsToRelease() const;
	std::vector<std::string> const& getCirculationIds() const;
	ExpProcInfoDistributionStatus const& getDistributionStatus() const;
	std::vector<std::string> const& getNotificationListOfFiles         () const;
	std::string const&         getNotificationDeliverySlipFilename() const;
	std::string const&         getNotificationContentFilename     () const;
	std::vector<std::string> const& getPackageNames () const;
	



	/** Setter */
	void setDatapackageAbsPath(std::string const&);
	void setWorkingDir(std::string const&);
    void setDatapackageFilename(std::string const&);
	void setDatapackageContent(dataprovider::DataPackage const& datapackageContent);
    void setDistributionFilename(std::string const&);
	void setDistributionContent(DistributionSerializable const& distributionContent);
	void setParametersFilename(std::string const&);
	void setCacheDownloadRepository(std::string const&);
	void setCacheAvailableDownloadSpaceSize(off_t const&);
	void setCirculationIds(std::vector<std::string> const&);
	void addCirculationId(std::string const&);
	void setDistributionStatus(ExpProcInfoDistributionStatus const&);
	void setNotificationListOfFiles         (std::vector<std::string> const& notificationListOfFiles    );
	void setNotificationDeliverySlipFilename(std::string const&         notificationDeliverySlipFilename);
	void setNotificationContentFilename     (std::string const&         notificationContentFilename     );
	void setPackageNames (std::vector<std::string> const&);
	void addPackageName (std::string const&);


	/** Notify that these items has been added to the cache and they need to be released */
	void appendCacheItemsToRelease(std::vector<std::string> const& );

	/** Clean the Cache Item To release (consumed) */
	void resetCacheItemsToRelease();

private:  // copy constructor and operator= defined but not implemented
	/** Copy Constructor */
	ExpProcInfo(const ExpProcInfo & );
	/** Operator = */
	ExpProcInfo &operator=(const ExpProcInfo &);
	

protected:
	/** Task Input/Output: The datapackage absolute path */
	std::string _datapackageAbsPath;

	/** Task Input: The working dir */
	std::string _workingDir;

    /** Task Input: The Datapackage input I/F file filename */
    std::string _datapackageFilename;
    /** Task Input: The Datapackage information */
    dataprovider::DataPackage _datapackageContent;

    /** Task Input: The Distribution input I/F file filename */
    std::string _distributionFilename;
    /** Task Input: The Distribution requested */
    DistributionSerializable _distributionContent;

	/** Task Input: The Parameters input I/F file filename (from distribution rules) */
	std::string _parametersFilename;

	/** Task Input: The cache Info - needed to instantiate the Cache from Task */
	std::string _cacheDownloadRepository;
	off_t _cacheAvailableDownloadSpaceSize;

	/** Task Output: The list of additional cache's items to be released after the processing ends */
	std::vector<std::string> _cacheItemsToRelease;

	/** Task Output: The Circulation ids related to this Export processing */
	/** - Not mandatory. 
	 *  - Set if used Data Circulation for transferring. 
	 *  - Set by the TaskTable's Task in charge of circulation if it uses the Data Circulator 
	 *  - For a package exporting, more than one circulation can be prepared. E.g.: One datapackage split in piecies with each piece with its own circulation priority. */
	std::vector<std::string> _circulationIds;

	/** Task Output: The list of transferred package names */
	std::vector<std::string> _packageNames;

	/** Task Output: The Status of Distribution */
	ExpProcInfoDistributionStatus _distributionStatus;

	//- Notification Section -//

	/** Task Output: The list of files composing the Formatted datapackage */
	std::vector<std::string> 	_notificationListOfFiles;
	/** Task Output: The delivery slip filename */
	std::string 				_notificationDeliverySlipFilename;
	/** Task Output: The Subject and Body of the Notification. It is the dump of a PackageEmailContent instance */
	std::string 				_notificationContentFilename;


private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(ExpProcInfo)

};

}

#endif //_ExpProcInfo_H_

