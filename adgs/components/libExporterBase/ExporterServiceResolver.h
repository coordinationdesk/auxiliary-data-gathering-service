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

	$Prod: A.C.S. libExporterBase$

	$Id$

	$Author$

	$Log$
	Revision 5.1  2013/06/17 13:57:07  nicvac
	New macro used.
	
	Revision 5.0  2013/06/07 15:51:57  marpas
	adopting libException 5.x standards
	
	Revision 1.3  2013/01/25 13:53:38  marpas
	using pattern::Singleton from libException
	
	Revision 1.2  2012/12/06 18:34:35  nicvac
	Removed dependency from libDataConsumer.
	
	Revision 1.1  2012/11/23 17:31:06  nicvac
	ExporterServiceResolver added.
	
        
*/

#ifndef _ExporterServiceResolver_H_
#define _ExporterServiceResolver_H_


#include <acs_c++config.hpp>
#include <Cache.h>

#include <exException.h>
#include <Singleton.hpp>
#include <boost/shared_ptr.hpp> // PRQA S 1013


_ACS_BEGIN_NAMESPACE(acs)

using pattern::Singleton ;

/** Shared pointer Types definitions */
typedef boost::shared_ptr<Cache> CacheSptr;


/**
* \brief
* Register of Services accessible by all the Exporter Classes
**/

class ExporterServiceResolver : public Singleton <ExporterServiceResolver> { // PRQA S 2109, 2153
	friend ExporterServiceResolver* Singleton<ExporterServiceResolver>::instance(); //SINGLETON // PRQA S 2107
public:

	/** ExporterServiceResolver Exceptions */
	exDECLARE_EXCEPTION(exExporterServiceResolverException, exException) ; // Base ExporterServiceResolver Exception. // PRQA S 2131, 2502 2 
	exDECLARE_EXCEPTION(exExporterServiceResolverCriticalException, exExporterServiceResolverException) ; // Critical Exception. // PRQA S 2153


	/** Set the download Cache */
	void setDownloadCacheSptr( CacheSptr const& );

	/** Get the download Cache */
	CacheSptr getDownloadCacheSptr() const;

	/** Unregister the objects */
	void unregister();
    virtual ~ExporterServiceResolver() throw() {} // PRQA S 2131

protected:  // Constructor cannot be called explicitly
	/** Default Class constructor */
	ExporterServiceResolver();
	

private:  // copy constructor and operator= defined but not implemented
	/** Copy Constructor */
	ExporterServiceResolver(const ExporterServiceResolver & );
	/** Operator = */
	ExporterServiceResolver &operator=(const ExporterServiceResolver &);

private:

	/** Shared pointer to the Cache */
	CacheSptr _downloadCacheSptr;

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(ExporterServiceResolver)

};

_ACS_END_NAMESPACE

#endif //_ExporterServiceResolver_H_

