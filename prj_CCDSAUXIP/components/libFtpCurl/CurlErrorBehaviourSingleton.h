/*

 Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

 $Prod: A.C.S. libFtpCurl Library $
*/

#ifndef _CurlErrorBehaviourSingleton_H_
#define _CurlErrorBehaviourSingleton_H_

#include <acs_c++config.hpp>
#include <exException.h>
#include <Singleton.hpp>
#include <curl/curl.h> // PRQA S 1013

_ACS_BEGIN_NAMESPACE(acs)

using pattern::Singleton;

class CurlErrorBehaviourSingleton : public Singleton<CurlErrorBehaviourSingleton> {	
public:
	~CurlErrorBehaviourSingleton() override = default ;

	exDECLARE_EXCEPTION(CurlErrorBehaviourSingletonException, exException);
	
	enum CurlErrorBehaviour { // NOSONAR - old enum
		UNDEFINED = 0,
		RECOVERABLE = 1,
		UNRECOVERABLE = 2
	};
	
	struct CurlErrors {
		int error;
		char errname[64]; // NOSONAR 
		bool configurable;
		CurlErrorBehaviour behaviour;
	};

	struct CurlAliases {
		char errname[64];  // NOSONAR 
		char alias[64]; // NOSONAR 
	};
	
	static std::string curlErrorBehaviour2String(CurlErrorBehaviour beh);
	static CurlErrorBehaviour string2CurlErrorBehaviour(const std::string& behstr);
	
	/** Returns the behaviour associated to the input error */
	CurlErrorBehaviour getBehaviour(int error) const;
	bool setBehaviour(int error, CurlErrorBehaviour beh);
	bool setBehaviour(const std::string& errname, CurlErrorBehaviour beh);
	bool exists(int error) const;
	bool exists(const std::string& errname) const;
	bool isConfigurable(int error) const;
	bool isConfigurable(const std::string& errname) const;
	bool getErrorName(int error, std::string& errname) const;
	bool getErrorCode(const std::string& errname, int& errcode) const;
	void initErrorMaps();

protected:
	CurlErrorBehaviourSingleton() noexcept;

private:
	Mutex _mutex = {} ;
	std::map<std::string, int, std::less<> > _curlErrors = {} ;
	std::map<int, CurlErrors> _curlErrorsBehaviours = {} ;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(CurlErrorBehaviourSingleton);

	friend CurlErrorBehaviourSingleton* Singleton<CurlErrorBehaviourSingleton>::instance(); // PRQA S 2107
};

_ACS_END_NAMESPACE

#endif /* _CurlErrorBehaviourSingleton_H_ */
