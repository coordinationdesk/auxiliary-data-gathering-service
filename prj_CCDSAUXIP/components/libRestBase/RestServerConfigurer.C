// PRQA S 1050 EOF
/*

    Copyright 1995-2021, Exprivia SPA - DFDA-AS
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.exprivia.it


    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Exprivia SPA

    $Prod: rest base interface $

*/ 
#include <RestServerConfigurer.h>

#include <ConfigurationSingleton.h>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(RestServerConfigurer)

const string RestServerConfigurer::_defaultProtocol = "http";


RestServerConfigurer::RestServerConfigurer(const string & defaultServiceName, const string & rootTag) : 
    _port(_defaultPort),
    _threadNum(_defaultThreadNum),
    _host("0.0.0.0"),
    _url(defaultServiceName),
    _protocol(_defaultProtocol),
    _rootTag(rootTag),
    _maxRequestSize(_defaultMaxRequestSize)
{
    ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB) ;
}

RestServerConfigurer::~RestServerConfigurer() ACS_NOEXCEPT 
{
}

void RestServerConfigurer::load()
{
	rsResourceSet &conf = ConfigurationSingleton::instance()->get() ;
	string tag;
	if (!_rootTag.empty())
	{
		tag = _rootTag+".RestServer";
	}
	else
	{
		tag = "RestServer";
	}
	rsPushNSpace srv_ns (conf, tag) ;

	if (conf.hasKey("ListeningHost")) 
	{
		conf.getValue("ListeningHost", _host) ;
		ACS_LOG_INFO("RestServer.ListeningHost: " << _host) ;
	}
	else 
	{
		ACS_LOG_WARNING("Using default value for host: " << _host) ;
	}
	if (conf.hasKey("Port")) 
	{
		conf.getValue("Port", _port) ;
		ACS_LOG_INFO("RestServer.Port: " << _port) ;
	}
	else 
	{
		ACS_THROW(exIOException("Mandatory configuration value RestServer.Port not set!"));
	}
	if (conf.hasKey("Url")) 
	{
		conf.getValue("Url", _url) ;
		ACS_LOG_INFO("RestServer.Url: " << _url) ;
	}
	else 
	{
		ACS_LOG_WARNING("Using default value for RestServer.Url: " << _url) ;
	}
	if (_url.empty())
	{
		ACS_THROW(exIOException("Empty string for RestServer.Url found!")) ;
	}

	if (conf.hasKey("Protocol")) 
	{
		conf.getValue("Protocol", _protocol) ;
		ACS_LOG_INFO("RestServer.Protocol: " << _protocol) ;
	}
	else 
	{
		ACS_LOG_WARNING("Using default value for protocol: " << _protocol) ;
	}
	if (conf.hasKey("MaxRequestSize"))
	{
		conf.getValue("MaxRequestSize", _maxRequestSize) ;
		ACS_LOG_INFO("RestServer.MaxRequestSize: " << _maxRequestSize) ;
	}
	else
	{
		ACS_LOG_WARNING("Using default value for MaxRequestSize: " << _maxRequestSize) ;
	}
	if (conf.hasKey("MaxNumThreads"))
	{
		conf.getValue("MaxNumThreads", _threadNum) ;
		ACS_LOG_INFO("RestServer.MaxNumThreads: " << _threadNum) ;
	}
	else
	{
		ACS_LOG_WARNING("Using default value for MaxNumThreads: " << _threadNum) ;
	}
}

_ACS_END_NAMESPACE
