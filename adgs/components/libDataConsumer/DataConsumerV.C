// PRQA S 1050 EOF
/*

    Copyright 1995-2018, Advanced Computer Systems 
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.acsys.it

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

    libDataConsumer  version file

*/ 

#include <auto_version_file>
#include "DataConsumerV.h"

using namespace acs ;
using namespace acs::dc ;

const char* DataConsumerV::_name = auto_version_file::name;
const char* DataConsumerV::_tag = auto_version_file::tag;


DataConsumerV::DataConsumerV() {}
DataConsumerV::~DataConsumerV() throw() {}


const char*  DataConsumerV::tag() { return _tag ; }
const char*  DataConsumerV::name() { return _name ; }

