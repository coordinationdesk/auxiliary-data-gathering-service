//PRQA S 1050 EOF
/*
	Copyright 1995-2018, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libPackageCirculatorTask$

	
        
*/ 

#include <PackageCirculatorTask.h>
#include "PackageCirculatorTaskV.h"

#include <Status.h>
#include <Circulation.h>
#include <CirculationMode.h>
#include <TmpNameStrategyTable.h>

#include <Application.h>
#include <dbConnectionPool.h>
#include <Transaction.h>


#pragma GCC diagnostic push // PRQA S 1040 4
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor" 
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wold-style-cast"

#include <CirculationPDS_USCOREOrderingBindingProxy.h>

#pragma GCC diagnostic pop

#include <ConfigurationSingleton.h>
#include <rsPushNSpace.h>
#include <RegExp.h>

#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;
using namespace acs::circulation;


ACS_CLASS_DEFINE_DEBUG_LEVEL(PackageCirculatorTask)

namespace {
    PackageCirculatorTaskV version;
}

PackageCirculatorTask::PackageCirculatorTask():
	_expProcInfo(),
	_distribution(),
	_datapackage(),
	_stop(false),
	_isTransferAsynch(false),
	_useSoap(false),
	_circulationDbName(),
	_confFromUrlPrefix(),
	_confFromHost(),
	_confSoapProtocol(),
	_confSoapHost(),
	_confSoapPort(),
	_confSoapService(),
	_confRetryTimes(5),
	_confRetryDelaySecs(30),
    _defaultDbKey()
{
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Id: " << Id);
}

PackageCirculatorTask::~PackageCirculatorTask() {
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Destructor called.");
}

string PackageCirculatorTask::getSoapServiceUrl() const {
	ostringstream endpointStream ;
	endpointStream<< _confSoapProtocol<<"://"<<_confSoapHost<<":"<<_confSoapPort<<"/"<<_confSoapService;

	return endpointStream.str();
}


string PackageCirculatorTask::getSourceUrl(string const& itemInDataPackagePath) const {

	ostringstream fromUrlss;

	if ( ! _confFromUrlPrefix.empty() ) {
		//Use the network to access to the datapackage

		//_confFromUrlPrefix like "ftp://opadc:jhdkjsahdkja"
		fromUrlss << _confFromUrlPrefix <<"@"<<_confFromHost<<"/"<<_expProcInfo.getDatapackageAbsPath();
	} else {
		//Can move the package from local filesystem
		fromUrlss << _expProcInfo.getDatapackageAbsPath();
	}

	fromUrlss <<"/"<< itemInDataPackagePath;

	return fromUrlss.str();
}

string PackageCirculatorTask::getDestinationUrl(string const& itemInDataPackagePath, bool skipUserPwd) const {
	ostringstream toUrlSs; 

	toUrlSs   	<<_distribution.getProtocol()<<"://";

	if ( !skipUserPwd  ) {
		toUrlSs	<< _distribution.getUsername();
	}

	if ( !skipUserPwd &&  !_distribution.getUsername().empty() &&  !_distribution.getPassword().empty() ) {
		toUrlSs<<":"<<_distribution.getPassword();
	}

	if ( !skipUserPwd && _distribution.getUsername()!="" ) {
		toUrlSs<<"@";
	}

	//Circulator wants '/' after host, even if the remote path begins with '/': eg.:  FTP://apf_to_adas:nU...Gs@adm-apfdgf//toADAS/AE1_OAPF_AUX_NOU_1A_20071031T015705_20071031T024341_000457_0021_0001.ZIP
	toUrlSs<<_distribution.getHostname()<<"/"<<_distribution.getRemotepath();

	toUrlSs <<"/"<<itemInDataPackagePath;

	return toUrlSs.str();
}




string PackageCirculatorTask::requestCirculationDb(string const& itemInDataPackagePath) const {

	string fromUrlss = this->getSourceUrl(itemInDataPackagePath);
	string toUrlSs = this->getDestinationUrl(itemInDataPackagePath, false); //include user and pwd

	ACS_LOG_PRIVINFO("PackageCirculatorTask: circulation from "<<fromUrlss);
	ACS_LOG_PRIVINFO("PackageCirculatorTask: circulation to "<<toUrlSs);

	//Create a new Circulation entry in the Circulator DB
	dbConnectionWrapper pw( db::ConnPools::instance()->getPool( _circulationDbName ) );
	dbConnection &conn = pw;

	int circulationId(0);

	{ // dbConnectionGuard commit at destructor time
		db::Transaction transaction(conn);
		transaction.begin_work("");

		try {
			int tempnamemodeId=0;
			tempnamemodeId = TmpNameStrategyTable::instance(_defaultDbKey)->id( _distribution.getTempStrategyName() ); 

			acs::circulation::Circulation circulation(conn);

			circulation.setFromURL( fromUrlss, conn );
			circulation.setToURL( toUrlSs, conn );
			circulation.setPriority( _datapackage.getPriority(), conn );
			circulation.setTempNameId( tempnamemodeId, conn );
			circulation.setTempNameValue( _distribution.getTempStrategyParam(), conn );

			circulation.setCirculationModeId( CirculationMode::instance(_defaultDbKey)->id(CirculationMode::implicitMode, false),
											  conn );
			circulation.setRemoveSource(false, conn);
			circulation.save(conn);
			circulation.setStatus( Status::instance(_defaultDbKey)->id( Status::eligible ),
								   conn );
			circulation.update(conn);

			//Retrieve the assigned circulation id
			circulationId = circulation.getId();
			transaction.commit();

			ACS_LOG_WARNING("Field 'Host id' not set in Circulation queue! Circulator DB interface is deprecated. Consider using Circulation Service soap interface.");

		} 
		catch (exception& ex) {

			ACS_LOG_ERROR("PackageCirculatorTask: Error recording a new Circulation entry for ["<<_expProcInfo.getDatapackageAbsPath()<<"]");
			transaction.roll_back();

			throw;
		}
	}

	ostringstream retId; retId<<circulationId;
	return retId.str();
}


string PackageCirculatorTask::requestCirculationSoap(string const& itemInDataPackagePath) const {

	string requestId ;

	string fromUrl = this->getSourceUrl(itemInDataPackagePath);
	string toUrl = this->getDestinationUrl(itemInDataPackagePath, true); //skip user and password

	ACS_LOG_PRIVINFO("PackageCirculatorTask: circulation from "<<fromUrl);
	ACS_LOG_PRIVINFO("PackageCirculatorTask: circulation to "<<toUrl);

	::Circulation::PDS_USCOREOrderingBindingProxy proxy;

	::Circulation::pds__CirculateDynamicRequestType newRequest;
	newRequest.RemoteUser         = _distribution.getUsername();
	newRequest.RemoteUserPassword = _distribution.getPassword();
	newRequest.RemoteUrl          = toUrl;
	newRequest.LocalPath          = fromUrl; // LocalPath is a little bit misleading... It is intended as "fromUrl": can be a remote url.
	newRequest.ClientID           = const_cast<string*> ( & _confFromHost );
	newRequest.TempnameMode       = const_cast<string*> ( & _distribution.getTempStrategyName() );
	newRequest.TempnameValue      = const_cast<string*> ( & _distribution.getTempStrategyParam() );

	ostringstream oss; oss<<_datapackage.getPriority(); string priority( oss.str() );
	newRequest.Priority = &priority;

	string soapServiceUrl = this->getSoapServiceUrl();
	proxy.soap_endpoint = soapServiceUrl.c_str();

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "try to contact soap service "<<soapServiceUrl);

	::Circulation::pds__CirculateResponseType newResponse;

	int soapRetCode = proxy.CirculateDynamic(&newRequest , &newResponse);

	if ( soapRetCode != SOAP_OK ) {
		ostringstream error; error<<"Error while contacting soap service "<<soapServiceUrl;
		proxy.soap_stream_fault(error) ;
		ACS_THROW( 
			exPackageCirculatorTaskCriticalException( error.str() ) );
	}

	if ( newResponse.Status != ::Circulation::pds__StatusEnum__ACCEPTED ) {
		ostringstream msg; msg<<"Dynamic Circulation soap request rejected. Error code is "<< static_cast<unsigned int>(newResponse.ErrorCode)<<endl
				<<"Requested parameters: "<<endl
				<<"RemoteUser        : "<<   newRequest.RemoteUser         <<endl
				<<"RemoteUserPassword: "<<   newRequest.RemoteUserPassword <<endl
				<<"RemoteUrl         : "<<   newRequest.RemoteUrl          <<endl
				<<"LocalPath         : "<<   newRequest.LocalPath          <<endl;
		ACS_THROW( exPackageCirculatorTaskCriticalException( msg.str() ) );
	}

	requestId = newResponse.RequestID;

	return requestId;
}

#define PackageCirculatorTask_RETRY(CODE) \
		size_t currentTry = 1;\
		bool isOk(false);\
		while ( !isOk && currentTry <= _confRetryTimes ) {\
			try {\
				CODE \
				\
				isOk=true;\
			} catch(exception& ex) {\
				ACS_LOG_WARNING( "Problem occurred. Current Attempt: "<<currentTry<<"/"<<_confRetryTimes<<". Problem is: "<<ex.what() );\
				ACS_LOG_PRIVINFO( "Problem detailed info: "<< ex );\
				ACS_COND_THROW( currentTry >= _confRetryTimes, exPackageCirculatorTaskCriticalException(ex, "") );\
				ACS_LOG_PRIVINFO("Retry after "<<_confRetryDelaySecs<<" [sec]");\
				Timer::delay( _confRetryDelaySecs * 1000 );\
				++currentTry;\
			}\
		}

string PackageCirculatorTask::requestCirculation(string const& itemInDataPackagePath) const {

	string toReturn;
	PackageCirculatorTask_RETRY(
			if ( _useSoap ) {
				toReturn = requestCirculationSoap(itemInDataPackagePath);
			} else {
				toReturn = requestCirculationDb(itemInDataPackagePath);
			}
	);
	return toReturn;

}

string PackageCirculatorTask::getCirculationStatusDb( string const& circulationId ) const {
	int status(0); 
	int circId(0); istringstream is(circulationId); is>>circId;

	dbConnectionWrapper pw( db::ConnPools::instance()->getPool( _circulationDbName ) );
	dbConnection &conn = pw;

	acs::circulation::Circulation circulation(conn);			
	circulation.setId( circId);

	circulation.load(conn);
    bool null ;
	status = circulation.getStatus(null, conn);

	return Status::instance(_defaultDbKey)->name(status);

	//Status::instance()->id( Status::instance()->completed )
}

string PackageCirculatorTask::getCirculationStatusSoap( string const& circulationId ) const {

	::Circulation::PDS_USCOREOrderingBindingProxy proxy;

	// fill the new Status request
	::Circulation::pds__StatusRequestType newRequest;
	newRequest.RequestID = circulationId;

	string soapServiceUrl = this->getSoapServiceUrl();
	proxy.soap_endpoint = soapServiceUrl.c_str();

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "try to contact soap service "<<soapServiceUrl);

	::Circulation::pds__StatusResponseType newResponse;

	int soapRetCode = proxy.GetStatus(&newRequest , &newResponse);

	if ( soapRetCode != SOAP_OK ) {
		ostringstream error; error<<"Error while contacting soap service "<<soapServiceUrl;
		proxy.soap_stream_fault( error) ;
		ACS_THROW( 
			exPackageCirculatorTaskCriticalException( error.str() ) );
	}

	return newResponse.Status;
}

string PackageCirculatorTask::getCirculationStatus( string const& circulationId ) const {

	string toReturn;
	PackageCirculatorTask_RETRY(
			if (_useSoap) {
				return getCirculationStatusSoap( circulationId );
			} else {
				return getCirculationStatusDb( circulationId );
			}
	);
	return toReturn;
}


void PackageCirculatorTask::monitorCirculations( vector<string> const& circulationIds,
												 bool joinFinalStatus,
                                                 bool& allAreCompleted, bool& someError, bool& stillWorking ) const {

	allAreCompleted =false;
	someError=false;
	stillWorking=false;

	//Circ status
	const short completedStatus = Status::instance(_defaultDbKey)->id( Status::completed );
	const short errorStatus =     Status::instance(_defaultDbKey)->id( Status::error );
	const short abortedStatus =   Status::instance(_defaultDbKey)->id( Status::aborted );
	short status(-1);

	bool firstLoop(true);
	vector<short> statusV(circulationIds.size(), 0xFF), statusVPrec(0); //Differ first time
	do {
		//Must be init true here (!)
		allAreCompleted=true;

		//First time don't wait
		if ( !firstLoop ) {
			firstLoop=false;
			Timer::delay(1000);
		}

		int i(0);
		for ( vector<string>::const_iterator it=circulationIds.begin(); it!=circulationIds.end(); ++it ) {

			status = Status::instance(_defaultDbKey)->id( 
				this->getCirculationStatus( *it ) );

			statusV[i++] = status;

			//Log messages only on status changing
			if ( statusVPrec != statusV ) {
				statusVPrec = statusV;
				ACS_LOG_PRIVINFO("PackageCirculatorTask: Circulation status for [datapackage, circulation id]=["<<_expProcInfo.getDatapackageAbsPath()<<", "<<*it<<"]: "
					<<Status::instance(_defaultDbKey)->name(status) ) ;
			}

			//Once became false, never will turn true again
			allAreCompleted = allAreCompleted && (status==completedStatus);
		}

		//All completed ==> OK
		/*/ This in C++11
		allAreCompleted = (
			statusV.end() ==
			find_if_not( statusV.begin(), statusV.end(), [](int i){return i==completedStatus;} )
		);
		//*/

		//Some error ==> ERROR
		someError = (
					statusV.end() !=
					find( statusV.begin(), statusV.end(), errorStatus )
		) || (
					statusV.end() !=
					find( statusV.begin(), statusV.end(), abortedStatus )
		);

		//Else ==> Still working
		stillWorking = !allAreCompleted && !someError;

	} while ( !_stop && joinFinalStatus && stillWorking);

}

void PackageCirculatorTask::abortCirculationsAndJoin( vector<string> const& circulationIds ) const {

	//Force all other circulation to go in error (cannot abort a circulation via IF)

	//Rename the datapackage
	string errDpName(
			File::getDirPath( _expProcInfo.getDatapackageAbsPath() ) + "/" +
			File::getFileName( _expProcInfo.getDatapackageAbsPath() ) +"_FORCING_ERROR");
	File::rename( _expProcInfo.getDatapackageAbsPath(), errDpName);

	//Loop until all the circulation fails (because forced datapackage renaming)
	ACS_LOG_INFO("PackageCirculatorTask: Waiting end of Circulations for datapackage ["<<_expProcInfo.getDatapackageAbsPath()<<"]");

	//Circ status
	const short completedStatus = Status::instance(_defaultDbKey)->id( Status::completed );
	const short errorStatus =     Status::instance(_defaultDbKey)->id( Status::error );
	const short abortedStatus =   Status::instance(_defaultDbKey)->id( Status::aborted );
	short status(-1);

	bool allTerminated(true);
	bool firstLoop(true);
	do {
		if (!firstLoop) { firstLoop=false; Timer::delay(1000); }

		allTerminated=true;
		for ( vector<string>::const_iterator it=circulationIds.begin(); it!=circulationIds.end(); ++it ) {

			status = Status::instance(_defaultDbKey)->id( 
				this->getCirculationStatus( *it ) );

			//Once became false, never will turn true again
			allTerminated = allTerminated && 
				(status==completedStatus || status==errorStatus || status==abortedStatus);
		}
	} while ( !_stop && !allTerminated );

	//Restore dp name
	File::rename( errDpName, _expProcInfo.getDatapackageAbsPath() );

	if (_stop && !allTerminated) {
        ostringstream oss; 
		std::copy(circulationIds.begin(), circulationIds.end(), std::ostream_iterator<string>(oss, " "));
		ACS_LOG_WARNING("PackageCirculatorTask::abortCirculationsAndJoin: Exiting because Stop requested. But Circulations ["<<oss.str()<<"] still running.");
	}

}


void PackageCirculatorTask::initFromConf() {
        	//Set Attributes from configuration
	rsResourceSet& conf = ConfigurationSingleton::instance()->get();

	conf.getValue("PackageTask.Circulation.DbMode.CirculationDbName", _circulationDbName);

	conf.getValue("PackageTask.Upload.fromUrlPrefix", _confFromUrlPrefix );
	RegExp re; re.setReg( "^<!\\[CDATA\\[(.*)\\]\\]>$" );
	RegExp::KeyMatches keyMatches;
	if ( re.match( _confFromUrlPrefix, keyMatches ) ) {
		_confFromUrlPrefix = keyMatches[1].getMatched();
	}

	conf.getValue( "PackageTask.Upload.fromHost", _confFromHost );
	if ( _confFromHost.empty() ) {
		char tmp[2048]; ::gethostname ( tmp, sizeof ( tmp ) );
		_confFromHost = tmp;
	}

	conf.getValue( "PackageTask.Circulation.SoapMode.protocol", _confSoapProtocol );
	conf.getValue( "PackageTask.Circulation.SoapMode.host"    , _confSoapHost     );
	conf.getValue( "PackageTask.Circulation.SoapMode.port"    , _confSoapPort     );
	conf.getValue( "PackageTask.Circulation.SoapMode.service" , _confSoapService  );

	if ( _confSoapHost.empty() ) {
		char tmp[2048]; ::gethostname ( tmp, sizeof ( tmp ) );
		_confSoapHost = tmp;
	}

	//Retry policy from configuration, if any
	conf.getValue( "PackageTask.Circulation.Retry.Times", _confRetryTimes, false );
	conf.getValue( "PackageTask.Circulation.Retry.DelaySecs", _confRetryDelaySecs, false );

	ACS_LOG_INFO( "Circulation client retry policy: retry "<<_confRetryTimes<<" times after delay "<<_confRetryDelaySecs<<" [sec]" );


}


_ACS_END_NAMESPACE
