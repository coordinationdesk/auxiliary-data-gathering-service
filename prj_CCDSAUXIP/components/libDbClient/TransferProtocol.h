// PRQA S 1050 EOF
/*
 *
	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

 *  $Prod: A.C.S. libDbClient Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *
 */

#ifndef _TransferProtocol_H_
#define _TransferProtocol_H_

#include <acs_c++config.hpp>
#include <NameIdAssociator.h>
#include <Singleton.hpp>

_ACS_BEGIN_NAMESPACE(acs) 


using pattern::ParamConstructibleSingletonMap ;

class TransferProtocol: // PRQA S 2153
    public NameIdAssociator<short>,
    public ParamConstructibleSingletonMap<TransferProtocol, std::string> {
    
    friend TransferProtocol* ParamConstructibleSingletonMap<TransferProtocol, std::string>::instance(const std::string &); // PRQA S 2107
public:
	TransferProtocol &operator=(TransferProtocol &&) = delete ;
	TransferProtocol (TransferProtocol &&) = delete ;
	TransferProtocol &operator=(const TransferProtocol &) = delete ;
	TransferProtocol (const TransferProtocol &) = delete ;
	TransferProtocol () = delete ;
	virtual ~TransferProtocol() = default ;

protected:
	explicit TransferProtocol(const std::string &key) ;
};

_ACS_END_NAMESPACE;


#endif /* _TransferProtocol_H_ */
