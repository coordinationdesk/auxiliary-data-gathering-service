// PRQA S 1050 EOF
/*
	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libExporterBase$

	$Id$

	$Author$

	$Log$
	Revision 5.2  2013/06/20 12:32:48  marpas
	qa rules
	coding best practices applied
	instrumented to avoid qac++ analisys crash
	
	Revision 5.1  2013/06/17 13:57:07  nicvac
	New macro used.
	
	Revision 5.0  2013/06/07 15:51:57  marpas
	adopting libException 5.x standards
	
	Revision 1.2  2013/05/24 18:29:54  nicvac
	S2PDGS-308: Refactoring because Inventory notifications implemented.
	
	Revision 1.1  2013/05/23 13:46:38  nicvac
	S2PDGS-308: Notification Refactoring.
	
        
*/

#ifndef _PackageEmailNotification_H_
#define _PackageEmailNotification_H_

#include <acs_c++config.hpp>
#include <EmailUserNotification.h>

#include <DataPackage.h>

#include <exException.h>

#ifndef PRQA_ANALYZER
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/utility.hpp>
#endif // PRQA_ANALYZER
#include <boost/shared_ptr.hpp> // PRQA S 1013 

_ACS_BEGIN_NAMESPACE(acs)



/** A serializable Mail Object 
 * Any Client (tipically Tasks running in the export Task Table) can create and dump this Object 
 * to override the default mail content created in PackageEmailNotification.
 * Dumped mail has to be set in expProcInfo 
 * The framework reads expProcInfo and override the mail content using PackageEmailNotification::setMailSerializedFilename
 */
class PackageEmailContent {
#ifndef PRQA_ANALYZER
	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) {
		ar &
			BOOST_SERIALIZATION_NVP(_subject) &     
			BOOST_SERIALIZATION_NVP(_body);
	}
#endif // PRQA_ANALYZER

public:

	/** Exceptions */
	exDECLARE_EXCEPTION(exPackageEmailContent, exException) ; // Base  Exception. // PRQA S 2131, 2502 2 
	exDECLARE_EXCEPTION(exPackageEmailContentCriticalException, exPackageEmailContent) ; // Critical Exception. // PRQA S 2153

	/** Default Class constructor */
	PackageEmailContent();
	/** Destructor */
	virtual ~PackageEmailContent();

	/** Copy Constructor */
	PackageEmailContent(const PackageEmailContent & );
	/** Operator = */
	PackageEmailContent &operator=(const PackageEmailContent &);

	/** Dump the Object to file */
	void dump(const std::string& filename) const;
	/** Load the Object from file */
	void load(const std::string& filename);

	/** Getter */
	std::string const& getSubject() const throw() ;
	std::string const& getBody() const throw() ;

	/** Setter */
	void setSubject(std::string const&);
	void setBody(std::string const&);


private:
	/** The Mail Subject */
	std::string _subject;
	/** The Mail body */
	std::string _body;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(PackageEmailContent)

};


/**
* \brief
 * The email notification instance 
 * The Client creates the instance, sets it and calls UserNotifier::sendAsynch
 * 
 * Setting accordingly PackageEmailNotification, it's possible to:
 * - Notify a download error (as the old Exporter): send default mail warning without delivery slip
 * - Notify a suspended package error (as the old Exporter): send default mail warning without delivery slip
 * - Notify a delivery error. Error occurred during processing in TaskTable. Send mail warning. Attach the deliverySlip if available (expProcInfo). Default mail text can be overridden by Tasks in TaskTables (expProcInfo).
 * - Notify a delivery. Processing in TaskTable is ok. Send success mail. Attach the deliverySlip if available (expProcInfo). Default mail text can be overridden by Tasks in TaskTables (expProcInfo).
 * 
 * Note: Notification is sent by PackageExporter Framework. But tasks in TT can prepare the attachment (delivery slip) or the subject/text to override the default mail content.
 */

class PackageEmailNotification: public EmailUserNotification { 
public:

	/** PackageEmailNotification Exceptions */
	exDECLARE_EXCEPTION(exPackageEmailNotificationException, exException) ; // Base PackageEmailNotification Exception.
	exDECLARE_EXCEPTION(exPackageEmailNotificationCriticalException, exPackageEmailNotificationException) ; // Critical Exception.


	/** Default Class constructor */
	explicit PackageEmailNotification(std::string const& dbKey="");
	/** Destructor */
	virtual ~PackageEmailNotification() throw();

	/** Set the Datapackage - Mandatory */
	PackageEmailNotification& setDataPackage(dataprovider::DataPackage const& dataPackage);
	/** Set the Status - Mandatory */
	PackageEmailNotification& setStatus(std::string const& status);
	/** Set as Success or Failure Notification - Mandatory */
	PackageEmailNotification& setSuccess(bool const& success);

	/** Set the list of files composing the Formatted Datapackage 
	 * - This list is just reported in the default mail body 
	 * - If not set the default mail body lists the downloaded files indicated in datapackage */
	PackageEmailNotification& setListOfFiles( std::vector<std::string> const& listOfFiles );

	/** Set the Delivery Slip 
	 * - param releaseCache: the file is on cache. After sending, release it  */
	PackageEmailNotification& setDeliverySlipFilename( std::string const& deliverySlipFilename, bool const& isOnCache=false );

	/** Set the Mail Subject and Body (Override the default one)
	 * Cf. PackageEmailContent description */
	PackageEmailNotification& setPackageEmailContentFromFile( std::string const& mailSerializedFilename );

	/** Set the Mail Subject and Body (Override the default one) - Override using the Object instead of the dumped file (cf. previous method) */
	PackageEmailNotification& setPackageEmailContent( PackageEmailContent const& packageEmailContent );
protected:

	/** Interface implementation */
	virtual bool setUp();
	/** Interface implementation */
	virtual bool prepare();
	/** Interface implementation */
	virtual void clean();

private:  // copy constructor and operator= defined but not implemented
	/** Copy Constructor */
	PackageEmailNotification(const PackageEmailNotification & );
	/** Operator = */
	PackageEmailNotification &operator=(const PackageEmailNotification &);


private:
	/** Datapackage */
	dataprovider::DataPackage _dataPackage;
	/** Status */
	std::string _status;
	/** Success/Failure notification */
	bool _success;

	/** List of files composing the datapackage */
	std::vector<std::string> _listOfFiles;

	/** The delivery Slip filename */
	std::string _deliverySlipFilename;
	/** The delivery Slip is on cache */
	bool _deliverySlipFilenameOnCache;

	/** email subject and body */
	PackageEmailContent _packageEmailContent;

	/** Info taken from DB */
	std::string _distRuleId;
	std::string _distUserid;
	std::string _distUsrEmailaddress;
	std::string _distRepoProtocol;
	std::string _distRepoUsername;
	std::string _distRepoHostname;
	std::string _distRepoRemotepath;
	/** e mail addresses for successful export */
	std::string _distSuccessAddresses;
	/** e mail addresses for failed export */
	std::string _distFailureAddresses;


private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(PackageEmailNotification)

};

/** Shared pointer Types definitions */
typedef boost::shared_ptr<PackageEmailNotification> PackageEmailNotificationSptr;
typedef boost::shared_ptr<const PackageEmailNotification> PackageEmailNotificationConstSptr;


_ACS_END_NAMESPACE

#endif //_PackageEmailNotification_H_

