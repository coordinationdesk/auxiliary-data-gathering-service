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
 *  $Prod: A.C.S.  Library $
 *
 *  $Id$
 *
 *  $Author$
 *
 *  $Log$
 *  Revision 6.0  2016/08/05 10:21:05  marfav
 *  Creating a major release due to db schema change involved
 *
 *  Revision 5.1  2013/07/15 17:21:11  marpas
 *  coding best practices applied
 *  qa warnings removedcompilation warnings removed
 *  robusteness and performances improved
 *
 *  Revision 5.0  2013/06/07 15:48:59  marpas
 *  adopting libException 5.x standards
 *
 *  Revision 1.1.1.1  2011/03/29 15:09:09  micmaz
 *  “Start”
 *
 *
 *
 *
 */

#ifndef CIRCULATIONVERSION_H_
#define CIRCULATIONVERSION_H_

namespace acs {

class CirculationVersion { // PRQA S 2109
public:
    CirculationVersion();
    virtual ~CirculationVersion() throw() ;
    static const char* tag() throw() ;
    static const char* name() throw() ;
private:
    // declared but not defined
    CirculationVersion(CirculationVersion const& rhl);
    CirculationVersion & operator=(CirculationVersion const& rhl);
private:
    static const char* _tag;
    static const char* _name;

};

}

#endif /* CIRCULATIONVERSION_H_ */
