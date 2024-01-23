// PRQA S 1050 EOF
/* Copyrights {{{

    Copyright 1995-2013, Advanced Computer Systems ,Inc.
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.acsys.it
    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libPostgres8Dialect $

    $Id$

    $Author$
 
    $Log$
    Revision 1.3  2013/02/27 12:38:47  marpas
    coding best practices applied
    qa rules
    interface rationalization

    Revision 1.2  2012/02/13 15:27:48  marpas
    refactoring

    Revision 1.1.1.1  2007/12/04 10:44:23  crivig
    Imported source

}}} */

#ifndef _Psg8DV_H_
#define _Psg8DV_H_

namespace acs {

// support class for versioning

class Psg8DV { // PRQA S 2109
public:
    Psg8DV() throw() ;
    ~Psg8DV() throw() ;
    static const char* tag() throw() ;
    static const char* name() throw() ;
private:
    Psg8DV(const Psg8DV &) ; // not implemented
    Psg8DV& operator= (const Psg8DV &) ; // not implemented
private:
    static const char* 	_tag ;
    static const char* 	_name ;
} ;

} // Close namespace

#endif // _exV_H_

