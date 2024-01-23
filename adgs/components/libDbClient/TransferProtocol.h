// PRQA S 1050 EOF
/*
 *
 *  Copyright 1995-2013, Advanced Computer Systems , Inc.
 *  Via Della Bufalotta, 378 - 00139 Roma - Italy
 *  http://www.acsys.it
 *
 *  All Rights Reserved.
 *
 *  This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
 *  the contents of this file may not be disclosed to third parties, copied or
 *  duplicated in any form, in whole or in part, without the prior written
 *  permission of Advanced Computer Systems, Inc.
 *
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
	virtual ~TransferProtocol() throw() ;

protected:
	explicit TransferProtocol(const std::string &key) ;
private:
	TransferProtocol &operator=(const TransferProtocol &) ; 	// declared but not implemented
	TransferProtocol (const TransferProtocol &) ; 			// declared but not implemented
	TransferProtocol () ; 									// declared but not implemented
};

_ACS_END_NAMESPACE;


#endif /* _TransferProtocol_H_ */
