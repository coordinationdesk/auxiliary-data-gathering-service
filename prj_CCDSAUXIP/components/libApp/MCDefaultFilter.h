// PRQA S 1050 EOF
/*

	Copyright 1995-2018, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. MCDefaultFilter Library $

	$Id$

	$Author$

	$Log$
	Revision 1.1  2015/06/12 08:01:32  tergem
	Monitoring anc Control Filter class added
	

*/

#ifndef _MCDefaultFilter_H_
#define _MCDefaultFilter_H_

#include <exFilter.h>

_ACS_BEGIN_NAMESPACE(acs)

class MCDefaultFilter : public exFilter // PRQA S 2153
{
public: 
	exDECLARE_EXCEPTION(Exception,exException) ;                        // PRQA S 2131, 2502
	
	explicit MCDefaultFilter(exostream *) ;
	virtual ~MCDefaultFilter() ACS_NOEXCEPT ;
	
	virtual bool filterInsertion(exFilterable &) ;

private:
	//Not implemented to prevent their use
	MCDefaultFilter();
	MCDefaultFilter(MCDefaultFilter const &);
	MCDefaultFilter & operator=(MCDefaultFilter const &);

	ACS_CLASS_DECLARE_DEBUG_LEVEL(MCDefaultFilter)

} ;

_ACS_END_NAMESPACE

#endif //_MCDefaultFilter_H_
