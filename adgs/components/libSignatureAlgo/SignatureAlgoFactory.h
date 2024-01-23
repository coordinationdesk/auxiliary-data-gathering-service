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
	Revision 2.4  2017/05/23 16:10:16  lucio.pulvirenti
	CSGACQ-121: objs registration/unregistration performed into CTOR/DTOR. getAlgo now uses SignatureAlgoInterfaceSingleton class.
	
	Revision 2.3  2013/11/27 16:21:24  marpas
	coding best practices applied
	warnings removed
	
	Revision 2.2  2006/06/20 15:47:27  ivafam
	Method compareNoCase replaced with StringUtils::compareNoCase
	
	Revision 2.1  2006/03/08 14:37:24  ivafam
	Added in PDS Rose Model
	
	Revision 2.0  2006/02/28 09:12:12  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2006/01/03 10:01:12  ivafam
	First Import
	

*/


#ifndef _SignatureAlgoFactory_H_
#define _SignatureAlgoFactory_H_ 

#include <AbstractFactoryChainSingleton.h>
#include <string>
#include <exException.h>

_ACS_BEGIN_NAMESPACE(acs)

class SignatureAlgo ;



class SignatureAlgoFactory : public Singleton<SignatureAlgoFactory > // PRQA S 2109, 2153
{
	friend SignatureAlgoFactory* Singleton< SignatureAlgoFactory >::instance(); // PRQA S 2107

public:
	/*! class NotFoundException declaration */
    
	exDECLARE_EXCEPTION(NotFoundException,exException) ; // PRQA S 2131, 2502

public:
    
	virtual ~SignatureAlgoFactory() throw() ;
	
    
	SignatureAlgo *getAlgo(const std::string &algo,const std::string & filename);
	
	static SignatureAlgo* buildMd5SumAlgoInterface(const std::string &);	
	static SignatureAlgo* buildMd5SumThAlgoInterface(const std::string &);

    
	std::string capabilities() const ;

protected:
    
	SignatureAlgoFactory() ;
    
 	
private:
	SignatureAlgoFactory(const SignatureAlgoFactory &) ; // not implemented
	SignatureAlgoFactory &operator=(const SignatureAlgoFactory &) ;	 // not implemented

private:
    
	ACS_CLASS_DECLARE_DEBUG_LEVEL(SignatureAlgoFactory)
} ;


typedef AbstractFactoryChainSingleton <SignatureAlgo, std::string, DefaultAbstractFactoryChainPredicate<std::string>, std::string> SignatureAlgoFactoryChain; 


void registerSignatureAlgoFactoryChainMethod(const std::string &, SignatureAlgo* (*)(const std::string &) );
void unregisterSignatureAlgoFactoryChainMethod(const std::string &, SignatureAlgo* (*)(const std::string &) );

_ACS_END_NAMESPACE

#endif /* _SignatureAlgoFactory_H_ */

