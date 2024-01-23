// PRQA S 1050 EOF
/*
 *
 *  Copyright 2013, Advanced Computer Systems , Inc.
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
 *
 *
 */

#ifndef _TmpNameStrategyTable_h_
#define _TmpNameStrategyTable_h_

#include<NameIdAssociator.h>
#include <Singleton.hpp>


_ACS_BEGIN_NAMESPACE(acs) ;


using pattern::ParamConstructibleSingletonMap ;

class TmpNameStrategyTable: // PRQA S 2153
    public NameIdAssociator<short>,
    public ParamConstructibleSingletonMap<TmpNameStrategyTable, std::string> {
    friend TmpNameStrategyTable* ParamConstructibleSingletonMap<TmpNameStrategyTable, std::string>::instance(const std::string &); // PRQA S 2107
public:
	virtual ~TmpNameStrategyTable() throw() ;
protected:
	explicit TmpNameStrategyTable(const std::string &key) ;
private:
	TmpNameStrategyTable &operator=(const TmpNameStrategyTable &) ; 	// declared but not implemented
	TmpNameStrategyTable (const TmpNameStrategyTable &) ; 			// declared but not implemented
	TmpNameStrategyTable () ; 									// declared but not implemented
};


_ACS_END_NAMESPACE;

#endif /* _TmpNameStrategyTable_h_ */
