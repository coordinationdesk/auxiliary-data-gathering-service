// PRQA S 1050 EOF
/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Db Catalog Library $

	$Id$

	$Author$

	$Log$
	Revision 4.7  2013/04/09 07:53:06  marpas
	./NameIdAssociator refactoring
	new statistics
	
	Revision 4.6  2013/04/05 09:36:03  marpas
	NameIdAssociator partial refactoring
	
	Revision 4.5  2012/12/11 17:30:10  marpas
	qa rules
	
	Revision 4.4  2012/11/19 13:30:19  marpas
	HEAD REALIGNEMENT
	
	Revision 4.2  2012/02/17 13:55:12  marpas
	refactoring
	multiple db design supported
	less compilation warning
	
	Revision 4.0  2011/10/18 12:31:10  marpas
	new statistics adopted
	
	Revision 3.0  2009/10/05 14:47:47  marpas
	getting rid of db_values table and all parameters name with one string (was deprecated)
	now dbParam can be constructed with tab/col pair (or just 2 strings)
	efficiency improved and some little bugs removed
	
	Revision 2.1  2009/05/22 15:13:50  marpas
	some rules fixed - need more work
	
	Revision 2.0  2006/02/28 08:44:43  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2005/03/07 14:00:35  marpas
	Importing dbExpressWrapper
	
	Revision 1.2  2004/04/14 15:39:00  marpas
	NameIdAssociator is a template
	
	Revision 1.1  2004/04/13 17:18:08  ivafam
	Header file contain only one class
	

*/

#ifndef _DataModeName_H_
#define _DataModeName_H_

#include <NameIdAssociator.h>
#include <Singleton.hpp>

_ACS_BEGIN_NAMESPACE(acs)


using pattern::ParamConstructibleSingletonMap ;


//
//
// class DataMode Name
//
//

class DataModeName:   // PRQA S 2153
    public NameIdAssociator<unsigned int>, 
    public ParamConstructibleSingletonMap<DataModeName, std::string>
{
    friend DataModeName* ParamConstructibleSingletonMap<DataModeName, std::string>::instance(const std::string &); // PRQA S 2107
public:
	virtual ~DataModeName() throw() {}  // PRQA S 2131
	
protected:
	explicit DataModeName(const std::string &key) :
        NameIdAssociator<unsigned int>("T_Data_Mode", "id", "name", key),
        ParamConstructibleSingletonMap<DataModeName, std::string>()
        {
            NameIdAssociator<unsigned int>::load() ;
        }

private:
	DataModeName &operator=(const DataModeName &) ; 	// declared but not implemented
	DataModeName (const DataModeName &) ; 			// declared but not implemented
	DataModeName () ; 								// declared but not implemented
} ; 


_ACS_END_NAMESPACE

#endif // _DataModeName_H_
