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

#include <memory>
#include <StringUtils.h>
#include <RestServerBase.h>

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(RestServerBase);
ACS_CLASS_DEFINE_DEBUG_LEVEL(RestServerBase::RestService);

const size_t RestServerBase::defaultMaxThreadsNumber = 8;

/*=============================================================
 * 
 *                RestService Implementation
 * 
 *============================================================*/

RestServerBase::RestService::RestService(const std::string& service) :
	mutex_(),
	service_(service),
	getEnabled_(false),
	postEnabled_(false),
	putEnabled_(false),
	deleteEnabled_(false)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	ACS_COND_THROW(service.empty(), RestServiceException("Empty service given as input."));
	
	if (!StringUtils::startsWith(service_, "/")) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Adding missing / to input service " << service_);
		service_ = "/" + service_;
	}
}

RestServerBase::RestService::RestService(const RestServerBase::RestService& s) :
	mutex_(),
	service_(s.service_),
	getEnabled_(s.getEnabled_),
	postEnabled_(s.postEnabled_),
	putEnabled_(s.putEnabled_),
	deleteEnabled_(s.deleteEnabled_)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

RestServerBase::RestService& RestServerBase::RestService::operator=(const RestServerBase::RestService& s) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	if (this != &s) {
		bool toSet = true;
		while (toSet) {
			Lock l1(mutex_);
			TryLock l2(s.mutex_);
			if (l2.locked()) {
				service_ = s.service_;
				getEnabled_ = s.getEnabled_;
				postEnabled_ = s.postEnabled_;
				putEnabled_ = s.putEnabled_;
				deleteEnabled_ = s.deleteEnabled_;
				toSet = false;
			}
			else {
				usleep(100);
			}
		}
	}
	return *this;
}

RestServerBase::RestService::RestService(RestServerBase::RestService&& s) :
	mutex_(),
	service_(std::move(s.service_)),
	getEnabled_(s.getEnabled_),
	postEnabled_(s.postEnabled_),
	putEnabled_(s.putEnabled_),
	deleteEnabled_(s.deleteEnabled_)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	s.getEnabled_ = false;
	s.postEnabled_ = false;
	s.putEnabled_ = false;
	s.deleteEnabled_ = false;
}

RestServerBase::RestService& RestServerBase::RestService::operator=(RestServerBase::RestService&& s) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	Lock l(mutex_);
	
	// No need to lock s, it is a temporary object here by definition
	service_ = std::move(s.service_);
	getEnabled_ = s.getEnabled_;
	postEnabled_ = s.postEnabled_;
	putEnabled_ = s.putEnabled_;
	deleteEnabled_ = s.deleteEnabled_;

	s.getEnabled_ = false;
	s.postEnabled_ = false;
	s.putEnabled_ = false;
	s.deleteEnabled_ = false;
	
	return *this;
}

RestServerBase::RestService::~RestService() noexcept {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

/* Returns true if the two services has the same name */
bool RestServerBase::RestService::operator==(const RestServerBase::RestService& s) { 
	bool equals = false;
	bool toCheck = true;
	while (toCheck) {
		Lock l1(mutex_);
		TryLock l2(s.mutex_);
		if (l2.locked()) {
			equals = (service_ == s.service_);
			toCheck = false;
		}
		else {
			usleep(100);
		}
	}
	return equals;
}

/* GET handler function. Must be reimplemented by derived class if needed */
void RestServerBase::RestService::doGet(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) { 
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	// No-op function
	response.send(Pistache::Http::Code::Not_Implemented);

}

/* POST handler function. Must be reimplemented by derived class if needed */
void RestServerBase::RestService::doPost(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) { 
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	// No-op function
	response.send(Pistache::Http::Code::Not_Implemented);
}

/* PUT handler function. Must be reimplemented by derived class if needed */
void RestServerBase::RestService::doPut(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) { 
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	// No-op function
	response.send(Pistache::Http::Code::Not_Implemented);
}

/* DELETE handler function. Must be reimplemented by derived class if needed */
void RestServerBase::RestService::doDelete(const Pistache::Rest::Request& request, Pistache::Http::ResponseWriter response) { 
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	// No-op function
	response.send(Pistache::Http::Code::Not_Implemented);
}

void RestServerBase::RestService::dumpRequest(const Pistache::Rest::Request& request, const long long bodySizeLimit) const
{
  std::ostringstream os;
  os << request.method() << " Request [" << request.resource() << "]";

  const Pistache::Http::Uri::Query& query = request.query();
  if (query.parameters_begin() != query.parameters_end())
  {
    auto it = query.parameters_begin();
    os << " - Query params [" << (*it).first << ": " << (*it).second;
    std::for_each(++it, query.parameters_end(),
                  [&os](const auto& pair) {
                    os << ", " << pair.first << ": " << pair.second;
                  });
    os << "]";
  }

  size_t bodySize = request.body().size();
  if (bodySize > 0UL)
  {
    std::string body = request.body();

    // remove \n and \r
    body.erase(std::remove(body.begin(), body.end(), '\n'), body.end());
    body.erase(std::remove(body.begin(), body.end(), '\r'), body.end());

    if (bodySizeLimit >= 0LL)
    {
      body = body.substr(0UL, bodySizeLimit);
    }

    os << " - Body [" << body;
    if (bodySize > static_cast<size_t>(bodySizeLimit) && bodySizeLimit >= 0LL)
    {
      os << " ... ";
    }
    os << "]";
  }
  ACS_LOG_INFO(os.str());
}

void RestServerBase::RestService::getQueryParam(const Pistache::Rest::Request& request, const std::string& name, std::string& val)
{
	const Pistache::Http::Uri::Query& query = request.query();

	std::optional<std::string> param = query.get(name);

	ACS_COND_THROW(! param.has_value(), QueryMissingParameterException() << "Query parameter '" << name << "' not found");

	val = param.value();
}

void RestServerBase::RestService::getQueryParam(const Pistache::Rest::Request& request, const std::string& name, bool& val)
{
	std::string valStr;
	getQueryParam(request, name, valStr);

	size_t strSize = valStr.size();
	val = strSize == 0 ||
		  StringUtils::equalsNoCase(valStr, "true") ||
		  (strSize == 1 && (StringUtils::equals(valStr, "1") || StringUtils::equalsNoCase(valStr, "t")));

	ACS_COND_THROW(!val &&
				   !StringUtils::equalsNoCase(valStr, "false") &&
				   (strSize != 1 || (!StringUtils::equals(valStr, "0") && !StringUtils::equalsNoCase(valStr, "f"))),
				   QueryParameterParsingException() << "Error parsing query parameter '" << name << "', extracted val '"
													<< valStr << "' cannot be converted to bool");
}

/*=============================================================
 * 
 *                RestServerBase Implementation
 * 
 *============================================================*/

/** Ctor/Dtor */
RestServerBase::RestServerBase(const std::string& hostname, unsigned short port) :
	services_(),
	httpEndpoint_(),
	router_(),
	hostname_(hostname),
	port_(port),
	maxThreads_(defaultMaxThreadsNumber),
	maxRequestSize_(0),
	maxResponseSize_(0),
	maxRequestSizeSet_(false),
	maxResponseSizeSet_(false),
	serverOnline_(false),
	serverInitialized_(false)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

RestServerBase::~RestServerBase() noexcept {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	stopServer();
}

void RestServerBase::registerService(const std::shared_ptr<RestServerBase::RestService>& service) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	ACS_COND_THROW(nullptr == service.get(), RestServerBaseException("Invalid null pointer to service"));
	
	std::string serviceName = service->getService();
	auto it = services_.find(serviceName);
	if (it == services_.end()) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Service \"" << serviceName << "\" registered"); 
	} else {
		ACS_LOG_WARNING("Service \"" << serviceName << "\" is already registered. It will be replaced with the new one.");
	}
	services_.insert(std::map<std::string, std::shared_ptr<RestService> >::value_type(serviceName, service));
}

void RestServerBase::unregisterService(const std::string& serviceName) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	auto it = services_.find(serviceName);
	if (it != services_.end()) {
		services_.erase(it);
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Service \"" << serviceName << "\" unregistered"); 
	} else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Service \"" << serviceName << "\" has not been found");
	}
}

bool RestServerBase::isServiceRegistered(const std::string& serviceName) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	auto it = services_.find(serviceName);
	return (it != services_.end());
}


void RestServerBase::initServerEndpoint() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	Pistache::IP any = Pistache::Ipv4::any();
	Pistache::Port portNumber(port_);
	Pistache::Address address(any, portNumber);
	try {
		Pistache::Address newAddress(hostname_, portNumber);
		address = newAddress;
	} catch (std::exception& ex) {
		ACS_LOG_WARNING ("Address " + hostname_ + " not allowed. Using " << any.toString());
		hostname_ = any.toString();
	}
	
	httpEndpoint_ = std::make_unique<Pistache::Http::Endpoint>(address);
	
	Pistache::Http::Endpoint::Options opt = Pistache::Http::Endpoint::options();
	opt.flags(Pistache::Tcp::Options::Linger | Pistache::Tcp::Options::ReuseAddr | Pistache::Tcp::Options::CloseOnExec);

	size_t threads = getMaxThreads();
	opt.threads(threads);
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Max number of threads set to " << threads);
	
	bool set = false;
	size_t maxRqSize = getMaxRequestSize(&set);
	if (set) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting max request size to " << maxRqSize << " bytes");
		opt.maxRequestSize(maxRqSize);
	}
	size_t maxRespSize = getMaxResponseSize(&set);
	if (set) {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Setting max response size to " << maxRespSize << " bytes");
		opt.maxResponseSize(maxRespSize);
	}
	
	httpEndpoint_->init(opt);
	setupRoutes();
	httpEndpoint_->setHandler(router_.handler());
	
	serverInitialized_ = true;
}



void RestServerBase::setupRoutes() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	auto it = services_.begin();
	while (it != services_.end()) {
		const std::shared_ptr<RestServerBase::RestService>& service = it->second;
		std::string serviceName = it->first;
		if (service->isGetEnabled()) {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Binding GET to service \"" << serviceName << "\"");
			Pistache::Rest::Routes::Get(router_, serviceName, Pistache::Rest::Routes::bind(&RestServerBase::RestService::doGet, service));
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "GET is not supported for service \"" << serviceName << "\"");
		}
		
		if (service->isPostEnabled()) {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Binding POST to service \"" << serviceName << "\"");
			Pistache::Rest::Routes::Post(router_, serviceName, Pistache::Rest::Routes::bind(&RestServerBase::RestService::doPost, service));
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "POST is not supported for service \"" << serviceName << "\"");
		}
		
		if (service->isPutEnabled()) {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Binding PUT to service \"" << serviceName << "\"");
			Pistache::Rest::Routes::Put(router_, serviceName, Pistache::Rest::Routes::bind(&RestServerBase::RestService::doPut, service));
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "PUT is not supported for service \"" << serviceName << "\"");
		}
		
		if (service->isDeleteEnabled()) {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Binding DELETE to service \"" << serviceName << "\"");
			Pistache::Rest::Routes::Delete(router_, serviceName, Pistache::Rest::Routes::bind(&RestServerBase::RestService::doDelete, service));
		} else {
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "DELETE is not supported for service \"" << serviceName << "\"");
		}
		
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Routes for service \"" << serviceName << "\" successfully binded");
		
		std::advance(it, 1);
	}
}


void RestServerBase::startServer(bool threaded) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	if (!isServerOnline()) {
		// Dump server parameters
		try {
			// Initialize endpoint and bind routes
			if (!serverInitialized_) {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Initializing server.");
				initServerEndpoint();
			} else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Server already initialized.");
			}

			std::ostringstream ss;
			ss << "Server endpoint is \"" << getHostname() << ":" << getPort() << "\"";
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, ss.str());
			
			serverOnline_ = true;
			if (threaded) {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Using dedicated thread for serve operation");
				httpEndpoint_->serveThreaded();
				ACS_LOG_INFO("Server started");
			}
			else {
				ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Using current thread to serve.");
				ACS_LOG_INFO("Server started");
				httpEndpoint_->serve();
			}
		} catch (std::exception& e) {
			ACS_LOG_NOTIFY_EX(e);
			serverOnline_ = false;
		}
	} else {
		ACS_THROW(RestServerBaseAlreadyStartedException("Rest server is already online"));
	}
}

void RestServerBase::stopServer() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	if (isServerOnline()) {
		httpEndpoint_->shutdown();
		// Destroy the HttpEndpoint forcing the join with the pistache listener thread
		httpEndpoint_.reset();
		serverOnline_ = false;
		serverInitialized_ = false;
		ACS_LOG_INFO("Server stopped");
	} else {
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Service was already stopped.");
	}
}

void RestServerBase::restartServer() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	stopServer();
	startServer();
}

const std::string& RestServerBase::getHostname() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	return hostname_;
}

unsigned short RestServerBase::getPort() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	return port_;
}

size_t RestServerBase::getMaxThreads() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	return maxThreads_;
}

size_t RestServerBase::getMaxRequestSize(bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	if (nullptr != set) {
		*set = maxRequestSizeSet_;
	}
	return maxRequestSize_;
}

size_t RestServerBase::getMaxResponseSize(bool* set) const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	if (nullptr != set) {
		*set = maxResponseSizeSet_;
	}
	return maxResponseSize_;
}

bool RestServerBase::isServerOnline() const {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	return serverOnline_;
}

void RestServerBase::setMaxThreads(size_t maxThreads) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	size_t th = maxThreads;
	if (maxThreads < 1) {
		ACS_LOG_WARNING("Invalid threads given as input. Using default value 1");
		th = 1;
	}
	maxThreads_ = th;
}

void RestServerBase::setMaxRequestSize(size_t maxRequestSize) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	if (maxRequestSize > 0) {
		maxRequestSize_ = maxRequestSize;
		maxRequestSizeSet_ = true;
	} else {
		ACS_LOG_WARNING("Invalid max request size given as input. Using pistache default value.");
	}
}

void RestServerBase::setMaxResponseSize(size_t maxResponseSize) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	if (maxResponseSize > 0) {
		maxResponseSize_ = maxResponseSize;
		maxResponseSizeSet_ = true;
	} else {
		ACS_LOG_WARNING("Invalid max request size given as input. Using pistache default value.");
	}
}

void RestServerBase::dump() {
	bool set = false;
	size_t maxsize = 0;
	
	std::ostringstream ss;
	ss << "\nREST server configuration:\n";
	ss << "    - Endpoint......................: " << getHostname() << "\n";
	ss << "    - Port..........................: " << getPort() << "\n";
	ss << "    - Maximum threads allowed.......: " << getMaxThreads() << "\n";

	maxsize = getMaxRequestSize(&set);
	if (set) {
		ss << "    - Maximum request size..........: " << maxsize << "\n";
		set = false;
	}
	
	maxsize = getMaxResponseSize(&set);
	if (set) {
		ss << "    - Maximum response size.........: " << maxsize << "\n";
		set = false;
	}
	
	if (!services_.empty()) {
		std::ostringstream ssserv;
		ss << "Registered services:\n";
		auto it = services_.begin();
		while (it != services_.end()) {
			ss << "    - \"" << it->first << "\"";
			ss << " [";
			if (it->second->isGetEnabled()) {
				ss << "GET/";
			}
			
			if (it->second->isPostEnabled()) {
				ss << "POST/";
			}
			
			if (it->second->isPutEnabled()) {
				ss << "PUT/";
			}
			
			if (it->second->isDeleteEnabled()) {
				ss << "DELETE/";
			}
			ss.seekp(-1, std::ios_base::end); // remove last slash
			ss << "]\n";
			
			std::advance(it, 1);
		}
	}
	ACS_LOG_INFO(ss.str());
}


_ACS_END_NAMESPACE
