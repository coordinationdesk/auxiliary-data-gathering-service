// PRQA S 1050 EOF
/*

	Copyright 2015-2021, Exprivia - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: Rest Base Interface $

*/
#ifndef _RestServerConfigurer_H_
#define _RestServerConfigurer_H_ 

#include <acs_c++config.hpp>
#include <Thread.h>

_ACS_BEGIN_NAMESPACE(acs)
/**
	Class mapping the Rest Server configuration
*/
class RestServerConfigurer  {
public:
	RestServerConfigurer(const std::string & defaultServiceName, const std::string & rootTag="");
	virtual ~RestServerConfigurer() ACS_NOEXCEPT;
	RestServerConfigurer(const RestServerConfigurer & ) = default ; 
	RestServerConfigurer& operator= (const RestServerConfigurer & ) = default; 

	RestServerConfigurer(RestServerConfigurer&& other) = default; //C++11 move constructor
	RestServerConfigurer& operator=(RestServerConfigurer&& other) = default;//C++11 move assignment operator
	
	void load() ;

	unsigned short port();
	std::string host();
	std::string url();
	std::string protocol();
	uint maxRequestSize();
	unsigned short maxThreadNum();
	
private:
    
	static const unsigned short _defaultPort = -1 ;    
	static const unsigned short _defaultThreadNum = 2 ;    
	static const unsigned short _defaultMaxRequestSize = 15000 ;
	static const std::string _defaultProtocol; 
private:
	unsigned short 	_port ;
	unsigned short 	_threadNum ;
	std::string 	_host;
	std::string 	_url;
	std::string 	_protocol;
	std::string 	_rootTag;	// root tag in the conf file (typically referring to the application/service that runs the Rest Service).
	uint	_maxRequestSize;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(RestServerConfigurer) ;
};

inline	unsigned short RestServerConfigurer::port()
{
	return _port;
};
inline	std::string RestServerConfigurer::host()
{
	return _host;
};
inline	std::string RestServerConfigurer::url()
{
	return _url;
};
inline	std::string RestServerConfigurer::protocol()
{
	return _protocol;
};
inline	uint RestServerConfigurer::maxRequestSize()
{
	return _maxRequestSize;
};
inline	unsigned short RestServerConfigurer::maxThreadNum()
{
	return _threadNum;
};

_ACS_END_NAMESPACE
#endif /* _RestServerConfigurer_H_ */
