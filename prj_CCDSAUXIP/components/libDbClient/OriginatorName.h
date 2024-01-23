// PRQA S 1050 EOF
/*

	Copyright 1995-2022, Exprivia SpA - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.com

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. DataBase Client Library $

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
	
	Revision 4.0  2011/10/18 12:31:11  marpas
	new statistics adopted
	
	Revision 3.0  2009/10/05 14:47:47  marpas
	getting rid of db_values table and all parameters name with one string (was deprecated)
	now dbParam can be constructed with tab/col pair (or just 2 strings)
	efficiency improved and some little bugs removed
	
	Revision 1.2  2009/05/22 15:13:50  marpas
	some rules fixed - need more work
	
	Revision 1.1  2008/07/28 09:01:09  manuel.maccaroni
	new library OriginatorName to query tOriginators table
	



*/

#ifndef _OriginatorName_H_
#define _OriginatorName_H_

#include <NameIdAssociator.h>
#include <Singleton.hpp>

_ACS_BEGIN_NAMESPACE(acs)

using pattern::ParamConstructibleSingletonMap ;

//
//
// class OriginatorName Name
//
//

class OriginatorName:   // PRQA S 2153
    public NameIdAssociator<unsigned int>, 
    public ParamConstructibleSingletonMap<OriginatorName, std::string>
{
    friend OriginatorName* ParamConstructibleSingletonMap<OriginatorName, std::string>::instance(const std::string &); // PRQA S 2107
public:
	OriginatorName &operator=(OriginatorName &&) = delete ;
	OriginatorName (OriginatorName &&) = delete ;
	OriginatorName &operator=(const OriginatorName &) = delete ;
	OriginatorName (const OriginatorName &) = delete ;
	OriginatorName () = delete ;
	virtual ~OriginatorName() = default ;
	
protected:
	explicit OriginatorName(const std::string &key) :
        NameIdAssociator<unsigned int>("T_Originators", "id","name", key),
        ParamConstructibleSingletonMap<OriginatorName, std::string>()
        {
            NameIdAssociator<unsigned int>::load() ;
        }
} ; 



_ACS_END_NAMESPACE


#endif // _OriginatorName_H_
