// PRQA S 1050 EOF
/*

    Copyright 2021, Exprivia SPA - DFDA-AS
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.exprivia.it


    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Exprivia SpA

    SRV Notification Dispatcher

*/ 
#include <SRVRestInterface.h>
#include <NotificationServiceResponsePayload.h>
#include <exException.h>

#include <SQLString.h>
#include <dbQuery.h>
#include <dbConnectionPool.h>

#include <Application.h>
#include <RemoteSiteUtility.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(SRVRestInterface)

SRVRestInterface::SRVRestInterface(const string & service) : 
		RestService(service)
{ 
        ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	setPostEnabled(true);

}
// void SRVRestInterface::init(size_t thr)
// {
// 	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"threads: " << thr) ;
// 
// 	if (! _maxRequestSizeSet)
// 	{
// 		ACS_LOG_WARNING ("Max payload size not set. Using default provided by pistache.");
// 	}
// 
// 	auto opts = Pistache::Http::Endpoint::options()
// 	.threads(thr)
// 	.flags(Pistache::Tcp::Options::Linger | Pistache::Tcp::Options::FastOpen | Pistache::Tcp::Options::ReuseAddr);
// 
// 	if (_maxRequestSizeSet)
// 	{
// 		opts.maxRequestSize(_maxRequestSize);
// 	}
// 
// 	_httpEndpoint->init(opts);
// 	setupRoutes();
// }
// 
// void SRVRestInterface::start() {
// 	_httpEndpoint->setHandler(_router.handler());
// 	_httpEndpoint->serve();
// }
// 
// void SRVRestInterface::shutdown() {
// 	_httpEndpoint->shutdown();
// }
// void SRVRestInterface::setupRoutes()
// {
//         ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"binding routes") ;
//         using namespace Pistache::Rest;
// 	Routes::Post(_router, _serviceName, Routes::bind(&SRVRestInterface::doPost, this));
// 
//         ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"routes bound: service name: "<< _serviceName) ;
// }

void SRVRestInterface::doPost(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response)
{
        ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
        ACS_LOG_INFO("REST/POST requested received.");
	RTTimer theTimer;
        theTimer.start();
        try
        {
		{
			ostringstream os;
			os << "POST Request [" << request.resource() << "]";
			size_t bodySize = request.body().size();
			if (bodySize > 0)
			{
    				const size_t bodySizeLimit = 5000;
				string bodySubstring = request.body().substr(0, bodySizeLimit);
			    	// remove \n and \r
				bodySubstring.erase(std::remove(bodySubstring.begin(), bodySubstring.end(), '\n'), bodySubstring.end());
				bodySubstring.erase(std::remove(bodySubstring.begin(), bodySubstring.end(), '\r'), bodySubstring.end());
				os << " - Body [" << bodySubstring;
				if (bodySize > bodySizeLimit)
				{
					os << " ... ";
				}
				os << "]";
			}
			os << "\n";
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, os.str());
		}

		std::istringstream body(request.body());
		
		std::string b=body.str();
		// Always JSON answers
       		response.headers().add<Pistache::Http::Header::ContentType> (MIME (Application, Json));

		NotificationServiceResponsePayloadList responseList;
		std::vector<NotificationServiceRequestPayload> requests;
		try
		{
			NotificationServiceRequestPayloadList pl(body.str());
			
			pl.sort();
 			requests = pl.getRequests();
		}
		catch (exception & e)
		{
			ACS_LOG_NOTIFY_EX(e);
			ACS_THROW(SRVRestInterfaceException("Error getting request payload."));
		}
	
		for (size_t k = 0; k < requests.size(); ++k)
		{
			try
			{
 				ACS_LOG_INFO("Received notification request for product '" << requests[k].getProductName() << "' (subscription id: '"<< requests[k].getSubscriptionId() << "', event: '" << NotificationServiceRequestPayload::subscriptionEvent2String(requests[k].getSubscriptionEvent()) << "')");
				// fill t_srv_pending_notifications
				setPendingTable(requests[k]);
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Request with id " << requests[k].getRequestId() << " inserted.");
			}
			catch (exception & e)
			{
				ostringstream os;
				os << "Error setting t_srv_pending_notifications";
				NotificationServiceResponsePayload plResp(requests[k].getRequestId());
				plResp.setErrorMessage(os.str());
				responseList.add(plResp);

				ACS_LOG_NOTIFY_EX(e);
			}
		}
        
		theTimer.stop();
		double elapsedTimeMs = theTimer.elapsed() * 1000.0;
		ostringstream elapsedMsec;
		elapsedMsec << setprecision(6) << elapsedTimeMs;

		ostringstream os;
		os << "REST/POST response created (" << static_cast<int>(response.getResponseCode()) << ") [" << elapsedMsec.str() << " ms]" << endl;
		ACS_LOG_INFO(os.str());
		response.send(Pistache::Http::Code::Created, responseList.toJSON());
        }
        catch (SRVRestInterfaceException& ex)
        {
		theTimer.stop();
		double elapsedTimeMs = theTimer.elapsed() * 1000.0;
		ostringstream elapsedMsec;
		elapsedMsec << setprecision(6) << elapsedTimeMs;
		ostringstream os;
		os << "REST/POST Request FAILED due to exception [" << elapsedMsec.str() << " ms]" << ": " << ex.what() << endl;
		ACS_LOG_WARNING (os.str());

		response.send(Pistache::Http::Code::Bad_Request, os.str());
        }
        catch (exception& ex)
        {
		theTimer.stop();
		double elapsedTimeMs = theTimer.elapsed() * 1000.0;
		ostringstream elapsedMsec;
		elapsedMsec << setprecision(6) << elapsedTimeMs;
		ostringstream os;
		os << "REST/POST Request FAILED due to exception [" << elapsedMsec.str() << " ms]" << ": " << ex.what() << endl;
		ACS_LOG_WARNING (os.str());

		response.send(Pistache::Http::Code::Internal_Server_Error, os.str());
        }
}

void SRVRestInterface::setPendingTable(const NotificationServiceRequestPayload & payload)
{
        ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	dbConnectionPool& cPool = *(Application::instance()->getDefaultConnectionPool());
	dbConnectionWrapper cw(cPool) ;
	dbConnection &conn = cw ; // PRQA S 3050
	const string &dbKey = conn.getKey() ;
	
	
	// Insert a single element
	SQLString queryString( dbKey, SQLString::INSERT ) ;
	queryString.addTableList ( "t_srv_pending_notifications" );


	queryString.addValuesList("product_uuid", db::toSQL(dbKey, payload.getProductId()) ) ;
	queryString.addValuesList("product_name", db::toSQL(dbKey, payload.getProductName()) ) ;
	bool isSet = false;
	string subId = payload.getSubscriptionId(&isSet);
	if (isSet!=false)
	{
		queryString.addValuesList("subscription_uuid", db::toSQL(dbKey, subId) ) ;
	}
	bool oSet(false);
	string ordId = payload.getOrderId(&oSet);

	if (oSet!=false)
	{
		queryString.addValuesList("order_uuid", db::toSQL(dbKey, ordId) ) ;
	}
	bool bSet(false);
	string batchOrd = payload.getBatchOrderId(&bSet);
	if (bSet!=false)
	{
		queryString.addValuesList("batchorder_uuid", db::toSQL(dbKey, batchOrd) ) ;
	}
	string subEvent = StringUtils::uppercase(NotificationServiceRequestPayload::subscriptionEvent2String(payload.getSubscriptionEvent(&isSet)));
	if (isSet!=false)
	{
		queryString.addValuesList("subscriptionevent", db::toSQL(dbKey, subEvent) ) ;
	}
	queryString.addValuesList("endpoint", db::toSQL(dbKey, payload.getEndpoint()) ) ;
	string endPUser = payload.getEndpointUser(&isSet);
	if (isSet!=false)
	{
		queryString.addValuesList("endpointuser", db::toSQL(dbKey, endPUser) ) ;
	}
	string endPPass = payload.getEndpointPassword(&isSet);
	if (isSet!=false)
	{
		queryString.addValuesList("endpointpassword", db::toSQL(dbKey, curl::RemoteSiteUtility::encrypt(endPPass)) ) ;
	}

	dbQuery query(conn, queryString, "INS t_srv_pending_notifications") ;	
	try
	{
		query.execForStatus ();
		ostringstream os;
		os << "t_srv_pending_notifications row added for subscription uuid " << subId << " (";
		if (oSet)
		{
			os << " order id: '" << ordId << "', ";
		}
		if (bSet)
		{
			os << " batch order id: '" << batchOrd << "', ";
		}
		os << " endpoint: '"<< payload.getEndpoint() << "', user:'"<< endPUser << "', password: '*****' )" ;
		ACS_LOG_INFO(os.str());
	}
	catch (exception & e)
	{
		ACS_THROW( // PRQA S 3081
                        dbQuery::QueryExecuteException("Query Execution error: Command : " + queryString.getSQL() + " Error : " + query.errorString())) ;
	}
}

_ACS_END_NAMESPACE
