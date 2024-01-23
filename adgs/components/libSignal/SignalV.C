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

    libSignal  version file

*/ 

#include <auto_version_file>
#include <SignalV.h>


_ACS_BEGIN_NAMESPACE(acs)

const char* SignalV::_name = auto_version_file::name;
const char* SignalV::_tag = auto_version_file::tag;


SignalV::SignalV() ACS_NOEXCEPT {} // PRQA S 2628
SignalV::~SignalV() ACS_NOEXCEPT {}

const char* SignalV::tag() ACS_NOEXCEPT { return _tag ; }
const char* SignalV::name() ACS_NOEXCEPT { return _name ; }


_ACS_END_NAMESPACE

