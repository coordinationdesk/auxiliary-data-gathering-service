/*
	Copyright 1995-2017, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libThinLayerServices$

	$Id$

	$Author$

	$Log$
        
*/

#ifndef _TLServiceResourceDefaultFactory_H_
#define _TLServiceResourceDefaultFactory_H_

#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)


/**
* \brief
* Default Factory for TLServiceRource
**/

class TLServiceResourceClientBase;
class TLServiceResourceSrvBase;

class TLServiceResourceDefaultFactory  { 
public:
	TLServiceResourceDefaultFactory();
	virtual ~TLServiceResourceDefaultFactory() throw();

	static TLServiceResourceSrvBase*    buildDefaultTLServiceResourceSrv();
	static TLServiceResourceClientBase* buildDefaultTLServiceResourceClient();

	void registerFactory();
	void unregisterFactory();

private:  // copy constructor and operator= defined but not implemented
	TLServiceResourceDefaultFactory(const TLServiceResourceDefaultFactory & );
	TLServiceResourceDefaultFactory &operator=(const TLServiceResourceDefaultFactory &);

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(TLServiceResourceDefaultFactory)
};

_ACS_END_NAMESPACE

#endif //_TLServiceResourceDefaultFactory_H_

