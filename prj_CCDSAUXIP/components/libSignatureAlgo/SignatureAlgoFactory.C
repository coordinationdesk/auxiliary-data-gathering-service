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

	$Prod: A.C.S. Signature Algo Library $

	$Id$

	$Author$

	$Log$
	Revision 2.7  2017/05/24 08:28:07  lucio.pulvirenti
	CSGACQ-121: code moved from SignatureAlgoFactoryInterfaceSingleton class now removed.
	
	Revision 2.6  2017/05/23 16:10:16  lucio.pulvirenti
	CSGACQ-121: objs registration/unregistration performed into CTOR/DTOR. getAlgo now uses SignatureAlgoInterfaceSingleton class.
	
	Revision 2.5  2013/11/27 16:21:24  marpas
	coding best practices applied
	warnings removed
	
	Revision 2.4  2012/02/13 17:09:21  marpas
	removing compiler warnings
	
	Revision 2.3  2007/04/13 17:40:35  marpas
	threaded md5 signature added
	
	Revision 2.2  2006/06/23 08:41:25  ivafam
	StringUtils::compareNoCase replaced with StringUtils::euqalsNoCase
	
	Revision 2.1  2006/06/20 15:47:27  ivafam
	Method compareNoCase replaced with StringUtils::equalsNoCase
	
	Revision 2.0  2006/02/28 09:12:12  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2006/01/03 10:01:12  ivafam
	First Import
	

*/


#include <SignatureAlgoFactory.h>
#include <Md5Sum.h>
#include <Md5SumTh.h>
#include <StringUtils.h>

#include <Filterables.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(SignatureAlgoFactory)

SignatureAlgoFactory::SignatureAlgoFactory() :
    Singleton<SignatureAlgoFactory >()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
	registerSignatureAlgoFactoryChainMethod("md5sum", &buildMd5SumAlgoInterface );
	registerSignatureAlgoFactoryChainMethod("md5sumTh", &buildMd5SumThAlgoInterface );

}

SignatureAlgoFactory::~SignatureAlgoFactory() throw()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ; // PRQA S 4631
	unregisterSignatureAlgoFactoryChainMethod("md5sum", &buildMd5SumAlgoInterface );
	unregisterSignatureAlgoFactoryChainMethod("md5sumTh", &buildMd5SumThAlgoInterface );
}

SignatureAlgo * SignatureAlgoFactory::getAlgo(const std::string &algo,const std::string & filename) // PRQA S 4020
{
	SignatureAlgo *algoPtr = 0;
	try {
		algoPtr = SignatureAlgoFactoryChain::instance()->newObject(StringUtils::uppercase(algo), filename);
	}
	catch( SignatureAlgoFactoryChain::FactoryType::KeyNotFoundException & )
	{
		ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, '\"' << algo << "\" algo not found!");
	}
	
	return algoPtr ;
}

SignatureAlgo* SignatureAlgoFactory::buildMd5SumAlgoInterface(const string & filename)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ; // PRQA S 4631
	
	return new Md5Sum(filename) ;
	
}

SignatureAlgo* SignatureAlgoFactory::buildMd5SumThAlgoInterface(const string & filename)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ; // PRQA S 4631
	
	return new Md5SumTh(filename) ;
}	

string SignatureAlgoFactory::capabilities() const
{
	string ret ;
	ret += "Md5Sum\n" ;
	ret += "Md5SumTh\n" ;
	return ret ;
}


void registerSignatureAlgoFactoryChainMethod(const string & key, SignatureAlgo* (*builder)(const std::string &) )
{
	SignatureAlgoFactoryChain::instance()->registerFactory(StringUtils::uppercase(key), builder);
}


void unregisterSignatureAlgoFactoryChainMethod(const string & key, SignatureAlgo* (*builder)(const std::string &) )
{
	SignatureAlgoFactoryChain::instance()->unregisterFactory(StringUtils::uppercase(key), builder);
}



_ACS_END_NAMESPACE

