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

	$Prod: A.C.S. libS1L2AuxReformatter$

	$Id$

	$Author$

	$Log$
	Revision 1.3  2013/10/17 15:56:12  nicvac
	compilation warnings processed.
	
	Revision 1.2  2013/01/25 13:59:28  marpas
	using pattern::Singleton from libException
	
	Revision 1.1  2012/04/23 08:02:39  damdec
	Class added.
	
        
*/

#ifndef _S1L2AuxReformatterSingleton_H_
#define _S1L2AuxReformatterSingleton_H_

#include <acs_c++config.hpp>
#include <Singleton.hpp>

#include <exException.h>
#include <map>


_ACS_BEGIN_NAMESPACE(acs)


class S1L2AuxReformatter;
using pattern::Singleton ;

/**
* \brief
* Sentinel-1 L2 Auxiliary Data File Formatter 
**/

class S1L2AuxReformatterSingleton : public Singleton <S1L2AuxReformatterSingleton> { 
public:

	/** S1L2AuxReformatterSingleton Exceptions */
	exDECLARE_EXCEPTION(exS1L2AuxReformatterSingletonException, exException) ; // Base S1L2AuxReformatterSingleton Exception.
	exDECLARE_EXCEPTION(exS1L2AuxReformatterSingletonCriticalException, exS1L2AuxReformatterSingletonException) ; // Critical Exception.
	
	void registerFactory(std::string const &, S1L2AuxReformatter * (*)());
	void unregisterFactory(std::string const &, S1L2AuxReformatter * (*)());
	
	/**
	  * \brief Return the S1L2AuxReformatter object pointer associated to the key in input
	  * \param The key must be the File Type
	  */										
	S1L2AuxReformatter * newReformatter(std::string const &);
	
	/** Destructor */
	virtual ~S1L2AuxReformatterSingleton() throw() ;

private:  // copy constructor and operator= defined but not implemented
	/** Copy Constructor */
	S1L2AuxReformatterSingleton(const S1L2AuxReformatterSingleton & );
	/** Operator = */
	S1L2AuxReformatterSingleton &operator=(const S1L2AuxReformatterSingleton &);
	friend S1L2AuxReformatterSingleton* Singleton<S1L2AuxReformatterSingleton>::instance(); //SINGLETON	

protected: // constructor cannot be called directly

	/** Default Class constructor */
	S1L2AuxReformatterSingleton();
private:
	
	std::map<std::string, S1L2AuxReformatter * (*)()> _mapReformatter;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(S1L2AuxReformatterSingleton)

};
void registerS1L2AuxReformatterFactoryMethod(std::string const & , S1L2AuxReformatter* (*)() );
void unregisterS1L2AuxReformatterFactoryMethod(std::string const & , S1L2AuxReformatter* (*)() );

_ACS_END_NAMESPACE


#endif //_S1L2AuxReformatterSingleton_H_

