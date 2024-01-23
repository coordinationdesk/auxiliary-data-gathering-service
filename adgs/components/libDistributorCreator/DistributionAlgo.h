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
	Revision 2.1  2013/04/05 09:37:40  marpas
	NameIdAssociator partial refactoring
	
	Revision 2.0  2013/01/21 17:52:31  marpas
	introducing dbConnectionPool
	coding best practices
	error management improved
	qa rules
	
	Revision 1.3  2012/12/21 12:41:05  marpas
	coding best practices
	introducing if compiled without PDS_OLD_SCHEMA the distributiorule_id field saving distribution items
	qa rules
	
	Revision 1.2  2012/03/08 09:14:39  marpas
	throw in dtor
	
	Revision 1.1  2012/03/01 15:44:23  chicas
	Added DistributionAlgo NameId associator
	
*/

#ifndef _DistributionAlgo_H_
#define _DistributionAlgo_H_

#include <NameIdAssociator.h>
#include <Singleton.hpp>

_ACS_BEGIN_NAMESPACE(acs)


using pattern::ParamConstructibleSingletonMap ;

//
//
// class DistributionAlgo Name
//
//

class DistributionAlgo: // PRQA S 2153
    public NameIdAssociator<unsigned int>, 
    public ParamConstructibleSingletonMap<DistributionAlgo, std::string>
{
    friend DistributionAlgo* ParamConstructibleSingletonMap<DistributionAlgo, std::string>::instance(const std::string &); // PRQA S 2107
public:
	virtual ~DistributionAlgo() throw() {}  // PRQA S 2131

protected:
	explicit DistributionAlgo(const std::string &key) :
        NameIdAssociator<unsigned int>("t_distributionalgo", "id","name", key),
        ParamConstructibleSingletonMap<DistributionAlgo, std::string>()
        {
            NameIdAssociator<unsigned int>::load() ;
        }

private:
	DistributionAlgo &operator=(const DistributionAlgo &) ; 	// declared but not implemented
	DistributionAlgo (const DistributionAlgo &) ; 			// declared but not implemented
	DistributionAlgo () ; 									// declared but not implemented
} ; 


_ACS_END_NAMESPACE


#endif // _DistributionAlgo_H_
