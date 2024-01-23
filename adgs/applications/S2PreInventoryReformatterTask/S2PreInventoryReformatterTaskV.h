// PRQA S 1050 EOF
/*
 *
 *  Copyright 2020, Advanced Computer Systems , Inc.
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
 */

#ifndef S2PreInventoryReformatterTaskV_H_
#define S2PreInventoryReformatterTaskV_H_

namespace acs {

class S2PreInventoryReformatterTaskV { // PRQA S 2109
public:
    S2PreInventoryReformatterTaskV();
    virtual ~S2PreInventoryReformatterTaskV() throw() ;
    static const char* tag() throw() ;
    static const char* name() throw() ;
private:
    // declared but not defined
    S2PreInventoryReformatterTaskV(S2PreInventoryReformatterTaskV const& rhl);
    S2PreInventoryReformatterTaskV & operator=(S2PreInventoryReformatterTaskV const& rhl);
private:
    static const char* _tag;
    static const char* _name;

};

}

#endif /* S2PreInventoryReformatterTaskV_H_ */
