// PRQA S 1050 EOF
/*

    Copyright 1995-2020, Exprivia SPA - DFDA-AS
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.exprivia.it


    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Exprivia SpA

    SRV Notification Dispatcher

*/ 
#include <SRVNotifier.h>
#include <exException.h>
#include <Filterables.h>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <ConfigurationSingleton.h>
#include <DateTime.h>
#include <SRVRestInterface.h>
#include <dbConnectionPool.h>
#include <Application.h>
#include <SQLString.h>
#include <dbSet.h>
#include <dbQuery.h>
#include <RemoteSite.h>
#include <RemoteSiteFactory.h>
#include <RemoteSiteUtility.h>
#include <FtpCurl.h>
#include <Curl.h>
#include <ACS_SSL.h>
#include <ODataNotificationServicePayload.h>

#include <json.hpp>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;
using json = nlohmann::json;

ACS_CLASS_DEFINE_DEBUG_LEVEL(SRVNotifier)

SRVNotifier::SRVNotifier() : 
    Thread("SRVNotifier"),
    _maxRetry(_defaultMaxRetry),
    _queryLimit(_defaultLimit),
    _retryTime(_defaultRetryTime),
    _retentionTime(_defaultRetentionTime),
    _authentication(true), // authentication option 2 (certificate requested)
    _disableNotificationInfo(false)
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
    loadConf() ;
}

SRVNotifier::~SRVNotifier() throw() 
{
	// PRQA S 4631 L1
	try {
		// Stop the underlying thread to before destroying
		// the object to avoid segfaults
		Thread::stop(); // PRQA S 4631
	} catch (std::exception& e) {
		ACS_LOG_NOTIFY_EX(e);
	}
	// PRQA L:L1
}

void SRVNotifier::loadConf()
{
    	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
    	rsResourceSet &conf = ConfigurationSingleton::instance()->get() ;
    	rsPushNSpace srv_ns (conf, "SRVNotificationDispatcher.SRVNotifier") ;
    
	if (conf.hasKey("MaxRetry")) {
		conf.getValue("MaxRetry", _maxRetry) ;
		ACS_LOG_INFO("SRVNotifier.MaxRetry: " << _maxRetry) ;
	}
	else {
		ACS_LOG_WARNING("Using default value for MaxRetry: " << _maxRetry) ;
	}
	if (conf.hasKey("QueryLimit")) {
		conf.getValue("QueryLimit", _queryLimit) ;
		ACS_LOG_INFO("SRVNotifier.QueryLimit: " << _queryLimit) ;
	}
	else {
		ACS_LOG_WARNING("Using default value for QueryLimit: " << _queryLimit) ;
	}
	if (conf.hasKey("RetryTime")) {
		conf.getValue("RetryTime", _retryTime) ;
	ACS_LOG_INFO("SRVNotifier.RetryTime: " << _retryTime) ;
	}
	else {
		ACS_LOG_WARNING("Using default value for RetryTime: " << _retryTime) ;
	}
	if (conf.hasKey("RetentionTime")) {
		conf.getValue("RetentionTime", _retentionTime) ;
	ACS_LOG_INFO("SRVNotifier.RetentionTime: " << _retentionTime) ;
	}
	else {
		ACS_LOG_WARNING("Using default value for RetentionTime: " << _retentionTime) ;
	}
	if (conf.hasKey("StrictAuthentication")) {
		conf.getValue("StrictAuthentication", _authentication) ;
		ACS_LOG_INFO("SRVNotifier.StrictAuthentication: " << _authentication) ;
	}
	else {
		ACS_LOG_WARNING("Using default value for strictAuthentication: " << _authentication) ;
	}
	if (conf.getValue("DisableNotificationInfo", _disableNotificationInfo, false))
	{
		ACS_LOG_INFO("SRVNotifier.DisableNotificationInfo: " << _disableNotificationInfo) ;
	}
	else {
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Using default value for DisableNotificationInfo: " << _disableNotificationInfo) ;
	}
	
	
}

void SRVNotifier::run()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
	
	while (!requestedCancel())
	{
		cleanExpiredNotifications();
		getPendingNotification();
		//Timer::delay (500);
		timeoutOrCancel(500);
	}
}
void SRVNotifier::cleanExpiredNotifications()
{
	//ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

	dbConnectionPool& cPool = *(Application::instance()->getDefaultConnectionPool());
	dbConnectionWrapper cw(cPool) ;
	dbConnection &conn = cw ; // PRQA S 3050
	const string &dbKey = conn.getKey() ;
	
	// Select items to remove
	SQLString sqlString( dbKey, SQLString::DELETE ) ;
	sqlString.addTableList("t_srv_pending_notifications", "tpn") ;
	DateTime now;
	{
    		dbQueryParameters pp(dbKey);
    		pp.addParameter( dbParam(dbKey, "tpn", "expirationdate", dbParam::MINOR_OR_EQUAL_OF, db::toSQL( dbKey, now, dbConv::YearToFraction ))); // PRQA S 3050, 3081
    		sqlString.addANDWhereParameter(pp);
	}  
	dbQuery query(conn, sqlString, "DEL old t_srv_pending_notifications") ;
	try
	{
	    	query.execForStatus();
	}
	catch (exception & e)
	{
		ACS_LOG_ERROR("An error occurred deleting expired notifications");
		ACS_LOG_NOTIFY_EX(e);
	}

}

void SRVNotifier::getPendingNotification()
{
	//ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;

	dbConnectionPool& cPool = *(Application::instance()->getDefaultConnectionPool());
	dbConnectionWrapper cw(cPool) ;
	dbConnection &conn = cw ; // PRQA S 3050
	const string &dbKey = conn.getKey() ;
	
	// Select items to notify
	SQLString sqlString( dbKey, SQLString::SELECT ) ;
	sqlString.addTableList("t_srv_pending_notifications", "tpn") ;
	sqlString.addSelectList("id");
	sqlString.addSelectList("product_uuid");
	sqlString.addSelectList("product_name");
	sqlString.addSelectList("subscription_uuid");
	sqlString.addSelectList("order_uuid");
	sqlString.addSelectList("batchorder_uuid");
	sqlString.addSelectList("subscriptionevent");
	sqlString.addSelectList("notificationdate");
	sqlString.addSelectList("retrycount");
	sqlString.addSelectList("retrytime");
	sqlString.addSelectList("endpoint");
	sqlString.addSelectList("endpointuser");
	sqlString.addSelectList("endpointpassword");

	{
    		dbQueryParameters pp(dbKey) ;
		pp.setParamOperator( dbQueryParameters::OR ) ;
		DateTime now;
    		pp.addParameter( dbParam(dbKey, "tpn", "retrytime", dbParam::MINOR_OR_EQUAL_OF, db::toSQL( dbKey, now, dbConv::YearToFraction ))); // PRQA S 3050, 3081
  		pp.addParameter( dbParam(dbKey, "tpn", "retrytime", dbParam::ISNULL) ); // PRQA S 3050, 3081
		sqlString.addANDWhereParameter(pp) ;
	}
	{
    		dbQueryParameters pp(dbKey);
     		pp.addParameter( dbParam(dbKey, "tpn", "expirationdate", dbParam::ISNULL)); // PRQA S 3050, 3081
    		pp.addParameter( dbParam(dbKey, "tpn", "completiondate", dbParam::ISNULL)); // PRQA S 3050, 3081
    		sqlString.addANDWhereParameter(pp);
	}
	sqlString.setOrderByString("tpn.notificationdate ASC"); 
	sqlString.setLimit (_queryLimit); 

	try
	{
		dbQuery q(conn, sqlString, "SEL t_srv_pending_notifications") ;
		dbSet set ;
		q.execForSet(set) ;
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, " Found " << set.size() << " pending notifications. ");
		dbRow row(dbKey);
        	while (set.getNextRow(row) && ( Application::instance()->getCurrentStopMode() != Application::ImmediateStop || Application::instance()->getCurrentStopMode() != Application::CleanStop))
        	{
			uint maxRetry = _maxRetry;
			dbPersistent::IdType id;
			string 	productId;
			string  productName;
			string  subId;
			string  ordId;
			string  batchOrdId;
			string  subEvent;
			string notificationDate;
			size_t retryC;
			DateTime retryT;
			string  endpoint;
			string  endpointUser;
			string  endpointPassword;		  
			uint16_t i = 0;
			
			json payload;

		    	row.column(i++, id ) ;
		    	row.column(i++, productId ) ;
		    	row.column(i++, productName ) ;


			bool isNull(false);
		    	row.column(i++, subId, &isNull ) ;
			bool isNullOId(false);
		    	row.column(i++, ordId, &isNullOId ) ;
			bool isNullBO(false);
		    	row.column(i++, batchOrdId, &isNullBO ) ;
			bool isNullSe(false);
		    	row.column(i++, subEvent, &isNullSe ) ;
		    	row.column(i++, notificationDate) ;
	
			ODataNotificationServicePayload pload(0);
			pload.setProductId(productId);
			pload.setProductName(productName);
			DateTime date(DateTime::zuluUTCString2DateTime(notificationDate));
			ostringstream os ;
			date.asISO8601(os,3) ;
			os << "Z";
			notificationDate = os.str();
			pload.setNotificationDate(notificationDate);
			if (!isNullBO && !_disableNotificationInfo) // batch order id not null and has to be sent in the notification
			{
				pload.setBatchOrderId(batchOrdId);
			}
			if (!isNullOId && !_disableNotificationInfo) // order id not null and has to be sent in the notification
			{
				pload.setOrderId(ordId);
			}
			if (!isNullSe && !_disableNotificationInfo) // SubscriptionEvent not null and has to be sent in the notification
			{
				pload.setSubscriptionEvent(ODataNotificationServicePayload::string2SubscriptionEvent(subEvent));
			}
			if (!isNull) // subscriptionId not null 
			{
				pload.setSubscriptionId(subId);

				if (!isNullSe)
				{
					if (ODataNotificationServicePayload::string2SubscriptionEvent(subEvent) == ODataNotificationServicePayload::created)
					{
						maxRetry = 1;
					}
					else if (ODataNotificationServicePayload::string2SubscriptionEvent(subEvent) == ODataNotificationServicePayload::deleted && isNullOId)
					{
						maxRetry = 1;
					}
				}
				
			}
			
			string plString = pload.toJSON();
			
		    	row.column(i++, retryC) ;
		    	row.column(i++, retryT) ;
			
		    	row.column(i++, endpoint) ;
			bool isEUserNull(false);
			row.column(i++, endpointUser, &isEUserNull) ;
			bool isEPPassNull(false);
			row.column(i++, endpointPassword, &isEPPassNull) ;
			
			// send curl request
			// set endpoint
			curl::Curl::CurlHttpOpt opt;
			opt.setMessage(plString);
			opt.setContentType("application/json");
			opt.setUser(endpointUser);
			if (_authentication)
			{
				opt.setSecureConnection(_authentication);
				opt.setStrictAuthentication(_authentication);
			}
			try
			{
				string d_password;
				if (!isEPPassNull)
				{
					d_password = curl::RemoteSiteUtility::decrypt(endpointPassword);
					opt.setPassword(d_password);
				}

				opt.setUri(endpoint);

				ACS_LOG_INFO("Sending product availability notification request for record id '"<< id << "' - productName '"<< productName<< "' to '" + endpoint + "' ( user = '"<< endpointUser << "', password  = '"<< endpointPassword << "')");
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Payload sent" + plString);
				curl::BandPriority noPrio;
				FtpCurl curl(noPrio);
				
				std::string responseJson = curl.httpPost(opt);
				ACS_LOG_INFO("Http server response: " << responseJson);
				// update t_srv_pending_notifications
				DateTime now;
				SQLString sqlString( dbKey, SQLString::UPDATE ) ;
				sqlString.addTableList("t_srv_pending_notifications", "tpn") ;
				sqlString.addSetList( "completiondate", db::toSQL( dbKey, db::toSQL(dbKey, now, dbConv::YearToFraction) ) );
				DateTime exp=now+_retentionTime/86400;
				sqlString.addSetList( "expirationdate", db::toSQL( dbKey, db::toSQL(dbKey, exp, dbConv::YearToFraction) ) );
    				{
					dbQueryParameters qp(dbKey) ;
					qp.addParameter(dbParam(dbKey, "tpn","id" , dbParam::EQUAL_TO, db::toSQL(dbKey, id) ) ) ; // PRQA S 3050, 3081
				    	sqlString.addANDWhereParameter(qp) ;
		   		}

			        dbQuery q(conn, sqlString, "Update completiondate ") ;
	       		 	if (!q.execForStatus())  {
                			ACS_THROW( // PRQA S 3081
                    			dbQuery::QueryExecuteException("Query Execution error: Command : " + sqlString.getSQL() + " Error : " + q.errorString())) ;
            			}
				ACS_LOG_INFO("t_srv_pending_notifications table updated.");
				// update t_srv_subscriptions
				if (!isNull) // subscription uuid not null
				{
					update_subscription(subId, now);
				}
			}
			catch (curl::RemoteSiteUtility::CryptException & e)
			{
				deleteRow(id);
				ACS_LOG_NOTIFY_EX(e);
			}
			catch (curl::URLMalformedException & e)
			{
				deleteRow(id);
				ACS_LOG_NOTIFY_EX(e);
			}
			catch(dbQuery::QueryExecuteException & e) // query error
			{
				ACS_LOG_NOTIFY_EX(e);
			}		
			catch(exception & e) // http error
			{
				ACS_LOG_ERROR("An error occurred sending http notification");
				ACS_LOG_NOTIFY_EX(e);
				
				size_t retryCount = retryC+1;
				SQLString sqlString( dbKey, SQLString::UPDATE ) ;        	
				sqlString.addTableList("t_srv_pending_notifications", "tpn") ;
				sqlString.addSetList( "retrycount", db::toSQL( dbKey, retryCount ) );
				DateTime retTime;
				retTime+=_retryTime/86400;
				sqlString.addSetList( "retrytime", db::toSQL( dbKey, db::toSQL(dbKey, retTime, dbConv::YearToFraction)  ) );

				if (retryCount >=maxRetry)
				{
					DateTime exp;
					exp+=_retentionTime/86400;
					sqlString.addSetList( "expirationdate", db::toSQL( dbKey, db::toSQL(dbKey, exp, dbConv::YearToFraction) ) );
				}
    				{
        				dbQueryParameters qp(dbKey) ;
	    				qp.addParameter(dbParam(dbKey, "tpn","id" , dbParam::EQUAL_TO, db::toSQL(dbKey, id) ) ) ; // PRQA S 3050, 3081
				    	sqlString.addANDWhereParameter(qp) ;
		   		}
			        dbQuery q(conn, sqlString, "Update retry info ") ;
	       		 	try
				{
					if (!q.execForStatus())  {
						ACS_LOG_ERROR("t_srv_pending_notifications updating with retry info failure.");
            				}
					ACS_LOG_INFO("t_srv_pending_notifications table updated with retry info.");
				}
				catch(exception & e2)
				{
					ACS_LOG_NOTIFY_EX(e2);
				}
				
			}
	        }
	}
	catch (exception & e)
	{
		ACS_LOG_ERROR("An error occurred searching for pending notifications");
		ACS_LOG_NOTIFY_EX(e);
	}
}

void SRVNotifier::update_subscription(const string & uuid, const DateTime & date)
{
	dbConnectionPool& cPool = *(Application::instance()->getDefaultConnectionPool());
	dbConnectionWrapper cw(cPool) ;
	dbConnection &conn = cw ; // PRQA S 3050
	const string &dbKey = conn.getKey() ;

	SQLString sqlString( dbKey, SQLString::UPDATE ) ;
	sqlString.addTableList("t_srv_subscriptions") ;
	sqlString.addSetList( "lastquerydate", db::toSQL( dbKey, db::toSQL(dbKey, date, dbConv::YearToFraction) ) );
	{
		dbQueryParameters qp(dbKey) ;
		qp.addParameter(dbParam(dbKey, "t_srv_subscriptions","uuid" , dbParam::EQUAL_TO, db::toSQL(dbKey, uuid) ) ) ; // PRQA S 3050, 3081
		sqlString.addANDWhereParameter(qp) ;
	}

	dbQuery q(conn, sqlString, "Update lastquerydate") ;
	try
	{
		q.execForStatus();
		if (q.getAffectedRows()==0)
		{
			ACS_THROW( RecordNotUpdatedException("No rows updated on table t_srv_subscriptions for uuid " + uuid)) ;
		}

		ACS_LOG_INFO("t_srv_subscriptions updated for subscription uuid " << uuid );
        }
	catch(exException & e)
	{
		ACS_LOG_WARNING("Error executing command: \'" << sqlString.getSQL(true) << "\'");
		ACS_LOG_NOTIFY_EX(e);
	}

}
void SRVNotifier::deleteRow(const dbPersistent::IdType & id)
{
	dbConnectionPool& cPool = *(Application::instance()->getDefaultConnectionPool());
	dbConnectionWrapper cw(cPool) ;
	dbConnection &conn = cw ; // PRQA S 3050
	const string &dbKey = conn.getKey() ;

	SQLString sqlStr( dbKey, SQLString::DELETE ) ;
	sqlStr.addTableList("t_srv_pending_notifications", "tpn") ;
	{
		dbQueryParameters qp(dbKey) ;
		qp.addParameter(dbParam(dbKey, "tpn","id" , dbParam::EQUAL_TO, db::toSQL(dbKey, id) ) ) ; // PRQA S 3050, 3081
		sqlStr.addANDWhereParameter(qp) ;
	}
	dbQuery queryD(conn, sqlStr, "DEL t_srv_pending_notifications") ;
	try
	{
		queryD.execForStatus();
		ACS_LOG_WARNING ("Deleted t_srv_pending_notifications row with id " << id);
	}
	catch (exception & ex)
	{
		ACS_LOG_ERROR("An error occurred deleting notification");
		ACS_LOG_NOTIFY_EX(ex);
		throw;
	}
}
_ACS_END_NAMESPACE
