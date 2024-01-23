// PRQA S 1050 EOF
/*
 *
 *  Copyright 2013-2017, Advanced Computer Systems , Inc.
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
 *  Revision 7.1  2017/10/17 15:56:09  marpas
 *  getting rid of EntityBeam and rewriting all based on smart persistent
 *
 *  Revision 7.0  2016/10/12 10:08:27  lucio.pulvirenti
 *  Pre S1PDGS-31034 schema no more supported: t_circulationsqueue nextretrydate columns must exist
 *
 *  Revision 6.0  2016/08/05 10:21:05  marfav
 *  Creating a major release due to db schema change involved
 *
 *  Revision 1.2  2015/02/19 14:56:52  francesco.avanzi
 *  getFromNToCiphers() method added ./ConsumableCirculation.C ./ConsumableCirculation.h
 *
 *  Revision 1.1  2013/08/07 14:17:58  lucio.pulvirenti
 *  First issue.
 *
 *
 *
 */


#include <Repository.h>

_ACS_BEGIN_NESTED_NAMESPACE(acs, circulation)


// constructor
Repository::Repository(dbConnection &conn) : dbSmartPersistent(table(), conn)
{
}


// copy constructor
Repository::Repository(const Repository &o):
	dbSmartPersistent(o)
{
}


Repository::~Repository() throw()
{
}


// operator =
Repository &Repository::operator=(const Repository &o)
{
	Lock lt(*this) ; // PRQA S 3050 2
	Lock lo(o) ;

	if(this != &o)
	{
		dbSmartPersistent::operator=(o) ;
	}
	return *this ;
}

using std::string ;

string Repository::str(dbConnection &conn, bool withId) const // PRQA S 4214
{
    std::ostringstream os;
    os << "{Repository";
    if (withId) {
        os << "@";
        os << getId();
    }

    bool isNull = false ;
    string s = getSymbolicname(isNull, conn) ;
    os << ", symbolicname: " << ( not isNull ? s : string("<null>") ) ;// PRQA S 3081, 3380, 3385
    s = getUsername(isNull, conn) ;
    os << ", username: " << ( not isNull ? s : string("<null>") ) ;// PRQA S 3081, 3380, 3385
    int i = -1 ;
    i = getHost(isNull, conn) ;
    os << ", host_id: " << i ;
    short sh = -1 ;
    sh = getTransferProtocol(isNull, conn) ;
    os << ", transferprotocol_id: " << sh ;
    s = getRemotePath(isNull, conn) ;
    os << ", remotepath: " << ( not isNull ? s : string("<null>") ) ;  // PRQA S 3081, 3380, 3385 
    s = getTempNameValue(isNull, conn) ;
    os << ", tempname_value: " << ( not isNull ? s : string("<null>") ) ;  // PRQA S 3081, 3380, 3385
    i = -1 ;
    i = getTempNameMode(isNull, conn) ; 
    os << ", tempname_id: " << i ;
    {
        s = "" ;
        s = getAllowedCiphers(isNull, conn) ; 
        if( not s.empty() )
        {
            os << ", allowedciphers: " << s ;
        }
    }
    os << "}";
    return os.str();
}

_ACS_END_NESTED_NAMESPACE

