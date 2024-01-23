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
	
	Revision 4.6  2013/04/05 09:36:04  marpas
	NameIdAssociator partial refactoring
	
	Revision 4.5  2012/12/11 17:30:10  marpas
	qa rules
	
	Revision 4.4  2012/11/19 13:30:20  marpas
	HEAD REALIGNEMENT
	
	Revision 4.2  2012/02/17 13:55:13  marpas
	refactoring
	multiple db design supported
	less compilation warning
	
	Revision 4.0  2011/10/18 12:31:11  marpas
	new statistics adopted
	
	Revision 3.0  2009/10/05 14:47:48  marpas
	getting rid of db_values table and all parameters name with one string (was deprecated)
	now dbParam can be constructed with tab/col pair (or just 2 strings)
	efficiency improved and some little bugs removed
	
	Revision 2.3  2009/05/22 15:13:50  marpas
	some rules fixed - need more work
	
	Revision 2.2  2008/12/05 14:35:08  marpas
	SiteTypeName class added
	
	Revision 2.1  2008/10/30 11:32:00  manuel.maccaroni
	import SiteName class
	

	

	



*/

#ifndef _SiteName_H_
#define _SiteName_H_

#include <NameIdAssociator.h>
#include <Singleton.hpp>

_ACS_BEGIN_NAMESPACE(acs)


using pattern::ParamConstructibleSingletonMap ;


//
//
// class SiteName Name
//
//

class SiteName:   // PRQA S 2153
    public NameIdAssociator<unsigned int>, 
    public ParamConstructibleSingletonMap<SiteName, std::string>
{
    friend SiteName* ParamConstructibleSingletonMap<SiteName, std::string>::instance(const std::string &); // PRQA S 2107
public:
	virtual ~SiteName() throw() {}  // PRQA S 2131
	
protected:
	explicit SiteName(const std::string &key) :
        NameIdAssociator<unsigned int>("T_Sites", "id","name", key),
        ParamConstructibleSingletonMap<SiteName, std::string>()
        {
            NameIdAssociator<unsigned int>::load() ;
        }

private:
	SiteName &operator=(const SiteName &) ;	// declared but not implemented
	SiteName (const SiteName &) ; 			// declared but not implemented
	SiteName () ; 							// declared but not implemented
} ; 


//
//
// class SiteTypeName Name
//
//

class SiteTypeName:   // PRQA S 2153
    public NameIdAssociator<unsigned int>, 
    public ParamConstructibleSingletonMap<SiteTypeName, std::string>
{
    friend SiteTypeName* ParamConstructibleSingletonMap<SiteTypeName, std::string>::instance(const std::string &); // PRQA S 2107
public:
	virtual ~SiteTypeName() throw() {}  // PRQA S 2131
	
protected:
	explicit SiteTypeName(const std::string &key) :
        NameIdAssociator<unsigned int>("t_sitetype", "id","name", key),
        ParamConstructibleSingletonMap<SiteTypeName, std::string>()
        {
            NameIdAssociator<unsigned int>::load() ;
        }


private:
	SiteTypeName &operator=(const SiteTypeName &) ;	// declared but not implemented
	SiteTypeName (const SiteTypeName &) ; 			// declared but not implemented
	SiteTypeName () ; 								// declared but not implemented
} ; 


_ACS_END_NAMESPACE



#endif // _SiteName_H_
