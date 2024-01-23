// PRQA S 1050 EOF
/*

	Copyright 2014, Advanced Computer Systems , Inc.
	Via della Bufalotta, 378 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$
	
	$Log$
	Revision 2.1  2014/03/18 12:32:04  marpas
	insertion operators added
	
	

*/

#include <TimeFormat.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

std::ostream &operator<< (std::ostream &os, TimeFormat::SatelliteID sid) 
{
    return os << static_cast<int>(sid) ; // PRQA S 3081
}

exostream &operator<< (exostream &os, TimeFormat::SatelliteID sid) // PRQA S 4222
{
    os << static_cast<int>(sid) ; // PRQA S 3081
    return os ; // PRQA S 4028
}

std::ostream &operator<< (std::ostream &os, TimeFormat::TimeModel tm) 
{
    return os << static_cast<int>(tm) ; // PRQA S 3081
}

exostream &operator<< (exostream &os, TimeFormat::TimeModel tm) // PRQA S 4222
{
    os << static_cast<int>(tm) ; // PRQA S 3081
    return os ; // PRQA S 4028
}

std::ostream &operator<< (std::ostream &os, TimeFormat::TimeReference tr) 
{
    return os << static_cast<int>(tr) ; // PRQA S 3081
}

exostream &operator<< (exostream &os, TimeFormat::TimeReference tr) // PRQA S 4222
{
    os << static_cast<int>(tr) ; // PRQA S 3081
    return os ; // PRQA S 4028
}

_ACS_END_NAMESPACE
