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

	$Prod:  $

	$Id$

	$Author$

	$Log$
	Revision 4.7  2013/04/09 07:53:07  marpas
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
	
	Revision 1.2  2009/05/22 13:29:18  marpas
	some rules fixed
	
	Revision 1.1  2008/08/05 10:10:57  manuel.maccaroni
	template to perform queries on t_qualifiedflag
	

	



*/

#ifndef _QualifiedFlag_H_
#define _QualifiedFlag_H_

#include <NameIdAssociator.h>
#include <Singleton.hpp>

_ACS_BEGIN_NAMESPACE(acs)


using pattern::ParamConstructibleSingletonMap ;

//
//
// class QualifiedFlag Name
//
//

class QualifiedFlag:   // PRQA S 2153
    public NameIdAssociator<unsigned int>, 
    public ParamConstructibleSingletonMap<QualifiedFlag, std::string>
{
    friend QualifiedFlag* ParamConstructibleSingletonMap<QualifiedFlag, std::string>::instance(const std::string &); // PRQA S 2107
public:
	virtual ~QualifiedFlag() throw() {}  // PRQA S 2131
	
protected:
	explicit QualifiedFlag(const std::string &key) :
        NameIdAssociator<unsigned int>("t_qualifiedflag", "id","name", key),
        ParamConstructibleSingletonMap<QualifiedFlag, std::string>()
        {
            NameIdAssociator<unsigned int>::load() ;
        }

private:
	QualifiedFlag &operator=(const QualifiedFlag &) ; 	// declared but not implemented
	QualifiedFlag (const QualifiedFlag &) ; 			// declared but not implemented
	QualifiedFlag () ; 									// declared but not implemented
} ; 


_ACS_END_NAMESPACE

#endif // _QualifiedFlag_H_
