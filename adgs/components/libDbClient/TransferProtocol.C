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
 *
 *
 *
 */

#include <TransferProtocol.h>


_ACS_BEGIN_NAMESPACE(acs) ;


TransferProtocol::~TransferProtocol() throw() {
    //empty.
}


TransferProtocol::TransferProtocol(const std::string &key) :
    NameIdAssociator<short>("t_transferprotocols", "id", "protocol", key),
    ParamConstructibleSingletonMap<TransferProtocol, std::string>()
{
    NameIdAssociator<short>::load() ;
}


_ACS_END_NAMESPACE;

