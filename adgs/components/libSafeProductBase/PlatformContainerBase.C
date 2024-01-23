// PRQA S 1050 EOF
/*

	Copyright 2011-2021, Advanced Computer Systems , Inc.
	Via della Bufalotta, 347 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

 	$Prod: A.C.S. Safe Product Base Library $
 
 	$Id$
 
 	$Author$
 
 	$Log$
 	Revision 1.4  2011/09/27 16:49:48  danalt
 	added versioning class
 	
 	Revision 1.3  2011/07/16 14:20:04  aleber
 	leap second info handled as string type
 	
 	Revision 1.2  2011/07/08 17:54:48  aleber
 	Writing of DataObjectContainer completed (but for checksum)
 	
 	Revision 1.1.1.1  2011/07/06 15:56:51  aleber
 	First implementation
 	
 	

*/


#include <PlatformContainerBase.h>
#include <Filterables.h>
#include <l0SafeProdBaseV.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

namespace {
 l0SafeProdBaseV Version;
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(PlatformContainerBase)


PlatformContainerBase::PlatformContainerBase(
	    double referenceUtcTime,
	    unsigned long satelliteReferenceBinaryTime,
	    unsigned long clockStepLength,
	    unsigned int swathNumber,
	    const string& leapSecondUtcTimeOfOccurrence,
	    const string& leapSecondPositiveSign) :
    _referenceUtcTime(referenceUtcTime),
    _satelliteReferenceBinaryTime(satelliteReferenceBinaryTime),
    _clockStepLength(clockStepLength),
    _swathNumber(swathNumber),
    _leapSecondUtcTimeOfOccurrence(leapSecondUtcTimeOfOccurrence),
    _leapSecondPositiveSign(leapSecondPositiveSign)

{
	static char Id[] = "@(#) ACS: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Id: " << Id) ;

}

PlatformContainerBase::~PlatformContainerBase() throw()
{
}

// copy constructor
PlatformContainerBase::PlatformContainerBase(const PlatformContainerBase & e) : 
    _referenceUtcTime(e._referenceUtcTime),
    _satelliteReferenceBinaryTime(e._satelliteReferenceBinaryTime),
    _clockStepLength(e._clockStepLength),
    _swathNumber(e._swathNumber),
    _leapSecondUtcTimeOfOccurrence(e._leapSecondUtcTimeOfOccurrence),
    _leapSecondPositiveSign(e._leapSecondPositiveSign)
{
}

// operator =
PlatformContainerBase&  PlatformContainerBase::operator=(const PlatformContainerBase & e)
{
	if (this != &e) {
        _referenceUtcTime = e._referenceUtcTime;
        _satelliteReferenceBinaryTime = e._satelliteReferenceBinaryTime;
        _clockStepLength = e._clockStepLength;
        _swathNumber 			= e._swathNumber;
        _leapSecondUtcTimeOfOccurrence 	= e._leapSecondUtcTimeOfOccurrence;
        _leapSecondPositiveSign 	= e._leapSecondPositiveSign;
	}
	return *this ;
}


string PlatformContainerBase::toStr() const 
{                                                                                                           

	std::ostringstream msg; 
	msg << "\tPlatformContainerBase content: " << '\n'
	    << "\t\t referenceUtcTime\t\t=" <<  _referenceUtcTime              << '\n'
	    << "\t\t satelliteReferenceBinaryTime\t=" <<  _satelliteReferenceBinaryTime  << '\n'
	    << "\t\t clockStepLength\t\t=" <<  _clockStepLength               << '\n'
	    << "\t\t swathNumber\t\t\t=" <<  _swathNumber	              << '\n'	
	    << "\t\t leapSecondUtcTimeOfOccurrence\t=" <<  _leapSecondUtcTimeOfOccurrence << '\n'
	    << "\t\t leapSecondPositiveSign \t=" <<  _leapSecondPositiveSign ;
	return msg.str() ;
	
}                                                                                                           
                                                                                                            
std::ostream &operator << (std::ostream &os, const PlatformContainerBase &v) 
{
    return os << v.toStr() ;
}

acs::exostream &operator << (acs::exostream &os, const PlatformContainerBase &v) // PRQA S 4222
{
    return os << v.toStr() ;
}

_ACS_END_NAMESPACE


