// PRQA S 1050 EOF
/*

	Copyright 2002-2013, Advanced Computer Systems , Inc.
	Via della Bufalotta, 347 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$

	$Log$
	Revision 2.2  2013/12/09 14:01:12  giucas
	Library completely revisited.
	
*/

#include <L2ProductInfo.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

/*
 * Class constructors
 */
L2ProductInfo::L2ProductInfo(const string& key) :
		L1ProductInfo (key)
{
}

/*
 * Class destructor
 */
L2ProductInfo::~L2ProductInfo() throw()
{
}

/*
 * copy constructor
 */
L2ProductInfo::L2ProductInfo (const L2ProductInfo & e) :
	L1ProductInfo( e)
{
	*this = e ;
}

/*
 * operator =
 */
L2ProductInfo&  L2ProductInfo::operator=(const L2ProductInfo & r)
{
    if (this != &r) {}
	return *this ;
}

_ACS_END_NAMESPACE


