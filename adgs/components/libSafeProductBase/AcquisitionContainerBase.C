// PRQA S 1050 EOF
/*

	Copyright 2011-2013, Advanced Computer Systems , Inc.
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
 	Revision 1.1  2011/09/12 08:32:15  aleber
 	first implementation
 	

*/

#include <AcquisitionContainerBase.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(AcquisitionContainerBase)


AcquisitionContainerBase::AcquisitionContainerBase(
	const string& startTime,
	const string& stopTime,
	double startTimeAnx,
	double stopTimeAnx) :

    _startTime(startTime),
    _stopTime(stopTime),
    _startTimeAnx(startTimeAnx),
    _stopTimeAnx(stopTimeAnx)

{
	static char Id[] = "@(#) ACS: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Id: " << Id) ;
}

AcquisitionContainerBase::~AcquisitionContainerBase() throw() 
{
}

// copy constructor
AcquisitionContainerBase::AcquisitionContainerBase(const AcquisitionContainerBase & e) :
    _startTime(e._startTime),
    _stopTime(e._stopTime),
    _startTimeAnx(e._startTimeAnx),
    _stopTimeAnx(e._stopTimeAnx)


{
}

// operator =
AcquisitionContainerBase&  AcquisitionContainerBase::operator=(const AcquisitionContainerBase & e)
{
	if (this != &e) {
	    _startTime = e._startTime;
	    _stopTime = e._stopTime;
	    _startTimeAnx = e._startTimeAnx;
	    _stopTimeAnx = e._stopTimeAnx;
	}
	return *this ;
}


string AcquisitionContainerBase::toStr() const
{                                                                                                           

	std::ostringstream os; 
	os << "\tAcquisitionContainerBase content: " << '\n'
	    << "\t\t startTime    =" <<  _startTime  << '\n'
	    << "\t\t stopTime     =" <<  _stopTime  << '\n'
	    << "\t\t startTimeAnx =" <<  _startTimeAnx << '\n'
	    << "\t\t stopTimeAnx  =" <<  _stopTimeAnx ;
	return os.str() ;
	
}                                                                                                           

std::ostream &operator << (std::ostream &os, const AcquisitionContainerBase &v) 
{
    return os << v.toStr() ;
}

acs::exostream &operator << (acs::exostream &os, const AcquisitionContainerBase &v) // PRQA S 4222
{
    return os << v.toStr() ;
}
  
  

_ACS_END_NAMESPACE



