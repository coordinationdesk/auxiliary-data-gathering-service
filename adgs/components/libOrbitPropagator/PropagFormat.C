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
	Revision 2.1  2014/03/20 11:50:54  marpas
	stream inserter operators added
	
	
	

*/

#include <PropagFormat.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

std::ostream &operator<< (std::ostream &os, PropagFormat::SatelliteID sid) 
{
    return os << static_cast<int>(sid) ; // PRQA S 3081
}

exostream &operator<< (exostream &os, PropagFormat::SatelliteID sid) // PRQA S 4222
{
    os << static_cast<int>(sid) ; // PRQA S 3081
    return os ; // PRQA S 4028
}

std::ostream &operator<< (std::ostream &os, PropagFormat::TimeModel tm) 
{
    return os << static_cast<int>(tm) ; // PRQA S 3081
}

exostream &operator<< (exostream &os, PropagFormat::TimeModel tm) // PRQA S 4222
{
    os << static_cast<int>(tm) ; // PRQA S 3081
    return os ; // PRQA S 4028
}


std::ostream &operator<< (std::ostream &os, PropagFormat::TimeReference r) 
{
    return os << static_cast<int>(r) ; // PRQA S 3081
}

exostream &operator<< (exostream &os, PropagFormat::TimeReference r) // PRQA S 4222
{
    os << static_cast<int>(r) ; // PRQA S 3081
    return os ; // PRQA S 4028
}


std::ostream &operator<< (std::ostream &os, PropagFormat::TimeInitMode r) 
{
    return os << static_cast<int>(r) ; // PRQA S 3081
}

exostream &operator<< (exostream &os, PropagFormat::TimeInitMode r) // PRQA S 4222
{
    os << static_cast<int>(r) ; // PRQA S 3081
    return os ; // PRQA S 4028
}


std::ostream &operator<< (std::ostream &os, PropagFormat::RefCoordinateFrames r) 
{
    return os << static_cast<int>(r) ; // PRQA S 3081
}

exostream &operator<< (exostream &os, PropagFormat::RefCoordinateFrames r) // PRQA S 4222
{
    os << static_cast<int>(r) ; // PRQA S 3081
    return os ; // PRQA S 4028
}

_ACS_END_NAMESPACE
