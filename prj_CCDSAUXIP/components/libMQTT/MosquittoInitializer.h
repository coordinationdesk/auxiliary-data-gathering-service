// PRQA S 1050 EOF
/*
   Copyright 2019-2022, Exprivia - DFDA-AS
   Via Della Bufalotta, 378 - 00139 Roma - Italy
   http://www.exprivia.it
   All Rights Reserved.

   This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
   the contents of this file may not be disclosed to third parties, copied or
   duplicated in any form, in whole or in part, without the prior written
   permission of Exprivia SpA. 

   libMQTT External library initialization singleton
*/ 

#ifndef _MosquittoInitializer_H_
#define _MosquittoInitializer_H_

#include <acs_c++pers.hpp>
#include <Singleton.hpp>

_ACS_BEGIN_NAMESPACE(acs) 

using acs::pattern::Singleton ;

class MosquittoInitializer : public Singleton<MosquittoInitializer> // NOSONAR PRQA S 2109, 2153
{
	friend MosquittoInitializer* Singleton<MosquittoInitializer>::instance(); // PRQA S 2107

public:
	MosquittoInitializer(const MosquittoInitializer &) = delete ;
	MosquittoInitializer & operator= (const MosquittoInitializer &) = delete ;
	MosquittoInitializer(MosquittoInitializer &&) = delete ;
	MosquittoInitializer & operator= (MosquittoInitializer &&) = delete ;
	virtual ~MosquittoInitializer() noexcept ;

protected:
	MosquittoInitializer() noexcept;
	

	ACS_CLASS_DECLARE_DEBUG_LEVEL(MosquittoInitializer) ;
};

_ACS_END_NAMESPACE

#endif //_MosquittoInitializer_H_
