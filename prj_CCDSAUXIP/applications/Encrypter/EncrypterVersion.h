/*
 *
 *  Copyright 1995-2011, Advanced Computer Systems , Inc.
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
 *  $Prod: A.C.S. Application $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *
 *
 */

#ifndef _EncrypterVersion_h_
#define _EncrypterVersion_h_

namespace acs {

/**
 * Application version class.
 */
class EncrypterVersion {
public:
    EncrypterVersion();
    virtual ~EncrypterVersion();
private:
    EncrypterVersion(EncrypterVersion const& other);
    EncrypterVersion& operator=(EncrypterVersion const& other);
public:
    static char const* tag();
    static char const* name();
private:
    static char const* _tag;
    static char const* _name;
};

}

#endif // _EncrypterVersion_h_
