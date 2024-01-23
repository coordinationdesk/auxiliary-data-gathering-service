/*
 
    Copyright 2018-2019, Advanced Computer Systems , Inc.
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.acsys.it

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

    $Prod: A.C.S. libSerialization $

    $Log$

*/

#include <Serializable.h>
#include <SerializableV.h>

_ACS_BEGIN_NAMESPACE(acs)

namespace {
    SerializableV version;
}


/***********************************************************************
 * 
 * 						Serializable definition
 * 
 ***********************************************************************/
ACS_CLASS_DEFINE_DEBUG_LEVEL(Serializable);

Serializable::Serializable() {
}

Serializable::~Serializable() ACS_NOEXCEPT {
}

_ACS_END_NAMESPACE
