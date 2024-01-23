// PRQA S 1050 EOF
/*

	Copyright 2019, Exprivia - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: Rest Base Interface $

*/

#ifndef _RestService_H_
#define _RestService_H_

#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>
#include <exException.h>
#include <exMacros.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)

class RestServerBase {
public:
	/**
	 * This class is used to represent a rest service and its associated operations
	 */
	class RestService {
	public:
		exDECLARE_EXCEPTION(RestServiceException, exException);
		exDECLARE_EXCEPTION(QueryExtractionException, exException);
		exDECLARE_EXCEPTION(QueryMissingParameterException, QueryExtractionException);
		exDECLARE_EXCEPTION(QueryParameterParsingException, QueryExtractionException);

	
		friend class RestServerBase;
	
		/** Deleted special functions */
		RestService() = delete;

		/** Special functions */
		explicit RestService(const std::string& service);
		RestService(const RestService&);
		RestService& operator=(const RestService&);
		RestService(RestService&&);
		RestService& operator=(RestService&&);
		virtual ~RestService() noexcept;

		/** Returns true if the two services have the same name */
		bool operator==(const RestService&);
		
		/** 
		 * This functions allows to specify wether or not the request handlers are enabled or not for this service 
		 * By default all handlers are disabled.
		 **/
		inline void setGetEnabled(bool val) { Lock l(mutex_); getEnabled_ = val; }
		inline void setPostEnabled(bool val) { Lock l(mutex_); postEnabled_ = val; }
		inline void setPutEnabled(bool val) { Lock l(mutex_); putEnabled_ = val; }
		inline void setDeleteEnabled(bool val) { Lock l(mutex_); deleteEnabled_ = val; }
		
		/** Returns true if the corresponding operation is enabled */
		inline bool isGetEnabled() const { Lock l(mutex_); return getEnabled_; }
		inline bool isPostEnabled() const { Lock l(mutex_); return postEnabled_; }
		inline bool isPutEnabled() const { Lock l(mutex_); return putEnabled_; }
		inline bool isDeleteEnabled() const { Lock l(mutex_); return deleteEnabled_; }
		
		/** Returns the service name */
		const std::string& getService() const { Lock l(mutex_); return service_; }
	protected:
		/** GET handler function. Must be reimplemented by derived class if needed */
		virtual void doGet(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
		/** POST handler function. Must be reimplemented by derived class if needed */
		virtual void doPost(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
		/** PUT handler function. Must be reimplemented by derived class if needed */
		virtual void doPut(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);
		/** DELETE handler function. Must be reimplemented by derived class if needed */
		virtual void doDelete(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response);

		/** Dump a incoming request with INFO log level **/
		virtual void dumpRequest(const Pistache::Rest::Request& request, const long long bodySizeLimit = 150) const;

		/** Extract a query parameter from URL (string version) **/
		static void getQueryParam(const Pistache::Rest::Request& request, const std::string& name, std::string& val);

		/** Extract a query parameter from URL (bool version) **/
		static void getQueryParam(const Pistache::Rest::Request& request, const std::string& name, bool& val);

		/** Extract a query parameter from URL (integer version) **/
		template<class Output, std::enable_if_t<std::is_arithmetic<Output>::value && !std::is_const<Output>::value>* = nullptr>
		static void getQueryParam(const Pistache::Rest::Request& request, const std::string& name, Output& val);

	private:
		Mutex mutex_;
		std::string service_;
		bool getEnabled_;
		bool postEnabled_;
		bool putEnabled_;
		bool deleteEnabled_;
		
		ACS_CLASS_DECLARE_DEBUG_LEVEL(RestServerBase::RestService);
	};

public:
	
	exDECLARE_EXCEPTION(RestServerBaseException, exException);
	exDECLARE_EXCEPTION(RestServerBaseAlreadyStartedException, RestServerBaseException);

	/** Deleted special functions */
	RestServerBase()                                   = delete;
	RestServerBase(const RestServerBase&)               = delete;
	RestServerBase& operator=(const RestServerBase&)    = delete;
	RestServerBase(RestServerBase&&)                    = delete;
	RestServerBase& operator=(RestServerBase&&)         = delete;
	
	RestServerBase(const std::string& hostname, unsigned short port);
	virtual ~RestServerBase() noexcept;
	
	const std::string& getHostname() const;
	unsigned short getPort() const;
	size_t getMaxThreads() const;
	size_t getMaxRequestSize(bool* set = 0) const;
	size_t getMaxResponseSize(bool* set = 0) const;
	bool isServerOnline() const;
	
	/** Setters */
	void setMaxThreads(size_t maxThreads);
	void setMaxRequestSize(size_t maxRequestSize);
	void setMaxResponseSize(size_t maxRequestSize);
	
	void registerService(const std::shared_ptr<RestService>& service);
	void unregisterService(const std::string& service);
	bool isServiceRegistered(const std::string& service);
	
	/** Starts the service. If "threaded" is set to true, a dedicated thread is created
	 * to serve requests, otherwise the current thread will be used.
	 * Recall that if threaded parameter is set to false the calling thread will remain blocked
	 * until a to stopService is performed.
	 */
	virtual void startServer(bool threaded = true);
	virtual void stopServer();
	virtual void restartServer();
	
	virtual void dump();
private:

	/** Endpoint initialization. Called by startService */
	void initServerEndpoint();
	
	/** Routes binding function. Called by startService */
	virtual void setupRoutes();
	
private:
	static const size_t defaultMaxThreadsNumber;
private:
	std::map<std::string, std::shared_ptr<RestService> > services_;
	std::unique_ptr<Pistache::Http::Endpoint> httpEndpoint_;
	Pistache::Rest::Router router_;
	std::string hostname_;
	unsigned short port_;
	size_t maxThreads_;
	size_t maxRequestSize_;
	size_t maxResponseSize_;
	bool maxRequestSizeSet_;
	bool maxResponseSizeSet_;
	bool serverOnline_;
	bool serverInitialized_;
	ACS_CLASS_DECLARE_DEBUG_LEVEL(RestServerBase);
};

template<class Output, std::enable_if_t<std::is_arithmetic<Output>::value && !std::is_const<Output>::value>*>
void RestServerBase::RestService::getQueryParam(const Pistache::Rest::Request& request, const std::string& name, Output& val)
{
	std::string valStr;
	getQueryParam(request, name, valStr);

	std::istringstream is(valStr);
	is >> val;

	ACS_COND_THROW(is.rdstate() != std::ios::eofbit, // Is at end of string and no errors occurs
				   QueryParameterParsingException() << "Error parsing query parameter '" << name << "', extracted val '"
													<< valStr << "' cannot be converted to "
													<< std::string(typeid(Output).name()));
}

_ACS_END_NAMESPACE

#endif /* _RestService_H_ */
