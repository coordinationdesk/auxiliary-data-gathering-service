// PRQA S 1050 EOF
/*
    Copyright 2022, Exprivia - DFDA-AS
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.exprivia.it
    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Exprivia SpA.

    libMQTT Helper struct to be used for topics
*/ 


#ifndef _MQTTSubIdHelper_H_
#define _MQTTSubIdHelper_H_

#include <string>

_ACS_BEGIN_NAMESPACE(acs) 


struct MQTTSubIdHelper
{
    MQTTSubIdHelper(const std::string &srv_, const std::string &db_) : server(srv_), db(db_) {}
    MQTTSubIdHelper() : server(), db() {}
    MQTTSubIdHelper(const MQTTSubIdHelper &) = default ; 
    MQTTSubIdHelper& operator=(const MQTTSubIdHelper &) = default ; 
    MQTTSubIdHelper(MQTTSubIdHelper &&) = default ; 
    MQTTSubIdHelper& operator=(MQTTSubIdHelper &&) = default ; 
    
    std::string server ;
    std::string db ;
} ;

_ACS_END_NAMESPACE

#endif //_MQTTSubIdHelper_H_
