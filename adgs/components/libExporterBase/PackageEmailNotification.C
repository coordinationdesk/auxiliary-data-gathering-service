// PRQA S 1050 EOF
/*
	Copyright 1995-2021, Advanced Computer Systems , Inc.
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
	Revision 5.9  2016/12/12 13:53:51  marfav
	Adding dbKey in ctor
	Using the base class function to perform queries
	
	Revision 5.8  2016/05/11 08:58:42  danalt
	use of ACS_THROW and ACS_COND_THROW
	
	Revision 5.7  2016/03/18 17:34:14  marpas
	adapted to c2evo dev
	
	Revision 5.6  2015/08/21 12:37:02  nicvac
	ALSAT1B-44: Mail setting from cart
	
	Revision 5.5  2013/10/03 14:48:38  nicvac
	acs sdk warnings processed.
	
	Revision 5.4  2013/07/16 15:59:53  nicvac
	S2PDGS-326: t_repositories updated.
	
	Revision 5.3  2013/06/20 12:32:48  marpas
	qa rules
	coding best practices applied
	instrumented to avoid qac++ analisys crash
	
	Revision 5.2  2013/06/17 16:34:02  nicvac
	New interfaces implemented.
	
	Revision 5.1  2013/06/17 13:57:07  nicvac
	New macro used.
	
	Revision 5.0  2013/06/07 15:51:57  marpas
	adopting libException 5.x standards
	
	Revision 1.2  2013/05/24 18:29:54  nicvac
	S2PDGS-308: Refactoring because Inventory notifications implemented.
	
	Revision 1.1  2013/05/23 13:46:38  nicvac
	S2PDGS-308: Notification Refactoring.
	
        
*/ 

#include <boost/archive/xml_oarchive.hpp> // PRQA S 1013 2
#include <boost/archive/xml_iarchive.hpp>

#include <PackageEmailNotification.h>

#include <ExporterServiceResolver.h>

#include <ConfigurationSingleton.h>

#include <Application.h>
#include <dbQuery.h>
#include <dbRow.h>
#include <dbSet.h>
#include <dbPersistent.h>
#include <XMLOstream.h>
#include <XMLTransformer.h>

#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;
using namespace dataprovider;

ACS_CLASS_DEFINE_DEBUG_LEVEL(PackageEmailNotification)
ACS_CLASS_DEFINE_DEBUG_LEVEL(PackageEmailContent)


//EMail Serializable
PackageEmailContent::PackageEmailContent():
	_subject(""),
	_body("")
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

PackageEmailContent::~PackageEmailContent() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB); // PRQA S 4631
}

PackageEmailContent::PackageEmailContent(const PackageEmailContent & rhl):
_subject(),
_body()
{
	*this = rhl;
}

PackageEmailContent & PackageEmailContent::operator=(const PackageEmailContent & rhl) {
	if (this != &rhl) {
		_subject = rhl._subject;
		_body = rhl._body;
	}
	return *this;
}


void PackageEmailContent::dump(const string& filename) const {
	std::ofstream ofs(filename.c_str());

	ACS_COND_THROW( ! ofs.good(), exPackageEmailContentCriticalException(string("Cannot dump PackageEmailContent on file ")+filename) ); // PRQA S 3081

	//Dump the datapackage
	boost::archive::xml_oarchive oa(ofs);

	const PackageEmailContent& dataPackageEmail = *this;

	// dump into the archive
	oa << BOOST_SERIALIZATION_NVP( dataPackageEmail );
}

void PackageEmailContent::load(const string& filename) {
	std::ifstream ifs( filename.c_str() );

	ACS_COND_THROW( ! ifs.good(), exPackageEmailContentCriticalException(string("Cannot load PackageEmailContent from file ")+filename) ); // PRQA S 3081

	//Load the datapackage
	boost::archive::xml_iarchive ia(ifs);

	PackageEmailContent& dataPackageEmail = *this;

	// restore from the archive
	ia >> BOOST_SERIALIZATION_NVP( dataPackageEmail );
}

string const& PackageEmailContent::getSubject() const throw() { return _subject; } // PRQA S 4120 2
string const& PackageEmailContent::getBody() const throw() { return _body; }

void PackageEmailContent::setSubject(string const& subject) { _subject=subject; } // PRQA S 4121 2
void PackageEmailContent::setBody(string const& body) { _body = body; }

//Notification instance

PackageEmailNotification::PackageEmailNotification(string const& dbKey):
	EmailUserNotification("PackageEmailNotification", dbKey),
	_dataPackage(),
	_status(),
	_success(false),
	_listOfFiles(),
	_deliverySlipFilename(),
	_deliverySlipFilenameOnCache(false),
	_packageEmailContent(),
	_distRuleId(),
	_distUserid(),
	_distUsrEmailaddress(),
	_distRepoProtocol(),
	_distRepoUsername(),
	_distRepoHostname(),
	_distRepoRemotepath(),
	_distSuccessAddresses(),
	_distFailureAddresses()
{
	static char Id[] = "@(#) ACS: $Id$";
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Id: " << Id);
}

PackageEmailNotification::~PackageEmailNotification() throw() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB); // PRQA S 4631
}

PackageEmailNotification& PackageEmailNotification::setDataPackage(dataprovider::DataPackage const& dataPackage) {
	_dataPackage = dataPackage;

	if ( this->getDescription().empty() ) {
		ostringstream oss; oss<<"e-mail notification for data package id "<<_dataPackage.getPackageId();
		this->setDescription( oss.str() );
	}

	return *this;
}

PackageEmailNotification& PackageEmailNotification::setStatus(std::string const& status) {
	_status = status;
	return *this;
}

PackageEmailNotification& PackageEmailNotification::setSuccess(bool const& success) {
	_success = success;
	return *this;
}

PackageEmailNotification& PackageEmailNotification::setListOfFiles(vector<string> const& listOfFiles) {
	_listOfFiles = listOfFiles;
	return *this;
}

PackageEmailNotification& PackageEmailNotification::setDeliverySlipFilename( std::string const& deliverySlipFilename, bool const& isOnCache ) {
	_deliverySlipFilename = deliverySlipFilename;
	_deliverySlipFilenameOnCache = isOnCache;
	return *this;
}

PackageEmailNotification& PackageEmailNotification::setPackageEmailContentFromFile( std::string const& mailSerializedFilename ) {

	_packageEmailContent.load( mailSerializedFilename );

	return *this;
}

PackageEmailNotification& PackageEmailNotification::setPackageEmailContent( PackageEmailContent const& packageEmailContent ) {
	_packageEmailContent = packageEmailContent;
	return *this;
}

bool PackageEmailNotification::setUp() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	bool mailToBeSent(true);

	dbPersistent::IdType dpid = _dataPackage.getPackageId();

	//Query the DB retrieving configured e-mails

	dbSet set;
	ostringstream sqlSStream;
    string dbKey = this->getDbKey();
	try {


		SQLString sqlString( dbKey, SQLString::SELECT );
		sqlString.setLimit(1);
		sqlString.addSelectList("disRul.id_distributionrule");		sqlString.addSelectList("disRul.successaddresses");
		sqlString.addSelectList("disRul.failureaddresses"   );
		sqlString.addSelectList("usr.userid"                );		sqlString.addSelectList("usr.emailaddress"       );
		sqlString.addSelectList("prot.protocol"             );		sqlString.addSelectList("repo.username"          );
		sqlString.addSelectList("hosts.host_name"           );		sqlString.addSelectList("repo.remotepath"        );
		sqlString.addTableList("t_distributionitems", "disIte");
		sqlString.addTableList("t_distributionrules", "disRul");
		sqlString.addTableList("t_users",             "usr"   );
		sqlString.addTableList("t_repositories",      "repo"  );
		sqlString.addTableList("t_transferprotocols", "prot"  );
		sqlString.addTableList("t_knownhosts",        "hosts" );
		dbQueryParameters pp( dbKey );
		pp.addParameter( dbParam(dbKey, "disIte","packageid"          , dbParam::EQUAL_TO, db::toSQL(dbKey, dpid)       )); // PRQA S 3050, 3081
		pp.addParameter( dbParam(dbKey, "disIte","distributionrule_id", dbParam::EQUAL_TO, "disRul.id_distributionrule"));
		pp.addParameter( dbParam(dbKey, "disRul","user_id"            , dbParam::EQUAL_TO, "usr.id_user"               ));
		pp.addParameter( dbParam(dbKey, "disRul","repository_id"      , dbParam::EQUAL_TO, "repo.id_repository"        ));
		pp.addParameter( dbParam(dbKey, "repo","transferprotocol_id"  , dbParam::EQUAL_TO, "prot.id"                   ));
		pp.addParameter( dbParam(dbKey, "repo","host_id"              , dbParam::EQUAL_TO, "hosts.id"                  ));
        sqlString.addANDWhereParameter( pp );

        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Query: "<<sqlString.getSQL());

		this->executeSQLStatement (sqlString, set);		

	} catch (exception& ex) {
		std::stringstream ss;
		ss << "Database Error retrieving Mail configuration for datappackage id [" << dpid << "]";
		exPackageEmailNotificationCriticalException nex(ex, ss.str());
		throw( nex );
	}

	std::stringstream ss;
	ss << "Cannot retrieve Mail configuration for datapackage id [" << dpid  << "]";
	ACS_COND_THROW( // PRQA S 3081
		set.size() == 0,
		exPackageEmailNotificationCriticalException(ss.str()));

	//Get row from the query result - Just the first
	dbRow row(dbKey); 
    set.getRow(1, row);
	uint16_t i = 0 ; 

	bool isNull(false);
	//If Null on DB, the attribute is left with its default value
	row.column(i++, _distRuleId          , &isNull);	row.column(i++, _distSuccessAddresses, &isNull);
	row.column(i++, _distFailureAddresses, &isNull);	row.column(i++, _distUserid          , &isNull);
	row.column(i++, _distUsrEmailaddress , &isNull);	row.column(i++, _distRepoProtocol    , &isNull);
	row.column(i++, _distRepoUsername    , &isNull);	row.column(i++, _distRepoHostname    , &isNull);
	row.column(i++, _distRepoRemotepath  , &isNull);


	//*/@@@ Temporary implementation! Cart architecture to be revised! (Cf. http://jira.acsys.it/browse/PDSEV-9)
	//@@@ In Current design the above query is performed fine because, even in case of cart scenario, the
	//@@@ distribution is linked to ad-hoc distribution rule, ad-hoc repository, ad-hoc user.
	//@@@ These ad-hoc entities are applicable to all the cart requests.
	if ( DataPackage::NullCartId !=  _dataPackage.getCartId()  ) {

		//Overwrite values from Cart

		rsResourceSet cartInfo;
		_dataPackage.getCartInfo( cartInfo );

		cartInfo.getValue( "mailWarning", mailToBeSent );

		string mediaType; cartInfo.getValue( "mediaType", mediaType );
		ACS_COND_THROW( mediaType != "FTP",
				exPackageEmailNotificationCriticalException( "Mediatype ["+mediaType+"] from Cart is not supported." ) );
		bool secureFtp; cartInfo.getValue( "secureFtp", secureFtp );
		_distRepoProtocol = secureFtp ? "SFTP" : "FTP" ;

		cartInfo.getValue( "email", _distSuccessAddresses );
		cartInfo.getValue( "email", _distFailureAddresses );
		cartInfo.getValue( "email", _distUsrEmailaddress );

		cartInfo.getValue( "userId", _distUserid );

		cartInfo.getValue( "ftpUserId", _distRepoUsername );
		{
			string hostAndPath; cartInfo.getValue( "remoteUrl", hostAndPath );
			size_t pos = hostAndPath.find("/");
			ACS_COND_THROW( pos == string::npos,
					exPackageEmailNotificationCriticalException( "Cannot found '/' in Cart url ["+hostAndPath+"] to get remote host and path." ) );

			_distRepoHostname = hostAndPath.substr( 0, pos );
			_distRepoRemotepath = hostAndPath.substr( pos+1 );
		}

	}

	ACS_COND_THROW( // PRQA S 3081
		(_success && _distSuccessAddresses == "") || (!_success && _distFailureAddresses == ""),
		exPackageEmailNotificationCriticalException(string("No e mail addresses configured for distribution rule id [")+_distRuleId+"]"));


	//List of files to be included in the mail:
	// - already set by client (Formatted datapackage)
	// - not set by client ==> use the cache files in Datapackage (the best I can have)
	if ( _listOfFiles.empty() ) {
		vector<string> cacheFiles; _dataPackage.getCacheFiles(cacheFiles);
		for ( vector<string>::const_iterator it=cacheFiles.begin(); it!=cacheFiles.end(); ++it ) { // PRQA S 4238

			string filename ;
            string fileversion ;
            time_t t = 0 ;
			File::splitCacheFileName( *it, filename, fileversion, t );

			_listOfFiles.push_back( filename );
		}
	}

	return mailToBeSent;
}

bool PackageEmailNotification::prepare() {

	//Based on the original dcSendMail::prepareMail

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Preparing mail for datapackage id ["<<_dataPackage.getPackageId()<<"]");

	//If Subject is empty create the default one
    if ( _packageEmailContent.getSubject() == "" ) {
		DateTime currentTime;
		string utcs;
		utcs = currentTime.utcs(utcs);

        ostringstream oss; oss<<"["<<_status<<"] pkg_"<<_dataPackage.getPackageId()<<"_"<<utcs;
		_packageEmailContent.setSubject( oss.str() );
	}

	//If Body is empty create the default one
	if ( _packageEmailContent.getBody() == "" ) {
		ostringstream os;

		if ( _deliverySlipFilename != "" ) { 
			//Default body in case of delivery slip (Cf. dcSendMail::prepareMail)
			os<<"Cf. attached file ["<<File::getFileName( _deliverySlipFilename )<<"]\n";

		} else {
			//Default body
			{
				XMLOstream xos(os, "MailNotification");
				{
					DateTime currentTime;
					string utcs;
					currentTime.utcs(utcs);
					XMLOstream::Tag tag(xos, "UtcsTime", utcs, "", true);
				}
				{
					XMLOstream::Tag tag(xos, "UserId", _distUserid, "", true);
				}
				{
					// Here the REAL Address here, even if sent to a mail repository
					XMLOstream::Tag tag(xos, "Email", _distUsrEmailaddress, "", true);
				}
				{
					XMLOstream::Tag tag(xos, "Repository", _distRepoHostname+"/"+_distRepoRemotepath, "", true);
				}
				{
					XMLOstream::Tag tag(xos, "Login", _distRepoUsername, "", true);
				}
				{
					XMLOstream::Tag tag(xos, "Protocol", _distRepoProtocol, "", true);
				}
				{
					XMLOstream::Tag tag(xos, "PackageId", _dataPackage.getPackageId(), "", true);
				}
				/*/ From old schema. If requested OrderType can be retrieved going to the order via t_inventory
				{
					XMLOstream::Tag tag(xos, "OrderType", distributionInfo.getOrderType());
				}
				{
					XMLOstream::Tag tag(xos, "Format", distributionInfo.getFormat());
				}
				//*/
				{
					ostringstream count;
					count << "count=\"" << _listOfFiles.size() << "\"";
					XMLOstream::Tag tag(xos, "List_of_Files", "", count.str());
					for ( vector<string>::iterator it=_listOfFiles.begin(); it!=_listOfFiles.end(); ++it ) { // PRQA S 4238
						XMLOstream::Tag tag2(xos, "File", *it, "", true);
					}
				}
			}
		}

		//Format body
		rsResourceSet& conf = ConfigurationSingleton::instance()->get();

		//@@@ Valutare se prendere il mail format dalla t_users (modifica da fare sullo schema)
		string mailFormat("");
		conf.getValue("PackageEmailNotification.MailFormat", mailFormat);

		string mailFormatXslFile("");
		conf.getValue("PackageEmailNotification.MailFormatXSL."+mailFormat, mailFormatXslFile);

		//If xsl not defined, the text is kept as the original xml
		if ( mailFormatXslFile != "" ) {
			XMLTransformer xmlTransformer;
			string formattedBody("");
			xmlTransformer.string2string( string(base::defaultConfPath()) +"/"+ mailFormatXslFile, os.str(), formattedBody); // PRQA S 3081

			os.str(formattedBody);
		}

		//Set the actual body
		_packageEmailContent.setBody( os.str() );

	}

	//*/ Set fields on super class
    this->setBody( _packageEmailContent.getBody() );
	this->setSubject( _packageEmailContent.getSubject() );
	this->setDestAddresses(  (_success?_distSuccessAddresses:_distFailureAddresses) ); // PRQA S 3380

	rsResourceSet& conf = ConfigurationSingleton::instance()->get();
	string conf_mailFormat ; 
	bool conf_attachBody = false ;
	string conf_contentType ;
	conf.getValue("PackageEmailNotification.MailFormat.attachBody.value", conf_attachBody); this->setAttachBody( conf_attachBody );
	conf.getValue("PackageEmailNotification.MailFormat", conf_mailFormat);
	conf.getValue("PackageEmailNotification.MailFormatXSL."+conf_mailFormat+".contentType.value", conf_contentType); this->setContentType(conf_contentType);

	//Attachments. If available, attach the delivery slip
	if ( !_deliverySlipFilename.empty() ) {

		string deliverySlipFilename("");
		if ( _deliverySlipFilenameOnCache ) {
			CacheSptr cacheSptr = ExporterServiceResolver::instance()->getDownloadCacheSptr();
			deliverySlipFilename = cacheSptr->getPath()+"/"+_deliverySlipFilename;
		} else {
			deliverySlipFilename = _deliverySlipFilename;
		}

		this->addAttachment( deliverySlipFilename );
	}
	//*/

	return true;
}

void PackageEmailNotification::clean() {

	try {
		//Release the delivery slip if it was on cache
		if ( _deliverySlipFilenameOnCache ) {

			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "releasing delivery slip ["<<_deliverySlipFilename<<"] from cache.");

			CacheSptr cacheSptr = ExporterServiceResolver::instance()->getDownloadCacheSptr();

			cacheSptr->release(_deliverySlipFilename);

			if ( this->isSent() ) {

				cacheSptr->eraseEntry( _deliverySlipFilename );

				//If deliverySlip has been passed with a cache relative path, the client
				// created it temporarely. 
				//If cache relative path used, remove it.
				string path = File::getDirPath( _deliverySlipFilename );
				if ( path != "." || path != "" ) {
					File::rmdir( cacheSptr->getPath()+"/"+path ); //Removed only if empty
				}
			}
			//If not sent Keep on disk for a while to allow investigation or manual sending

		}
	} catch (exception& ex) {
		ACS_LOG_WARNING("PackageEmailNotification::clean: Exception during cleaning: "<<ex.what());
	}

}

_ACS_END_NAMESPACE
