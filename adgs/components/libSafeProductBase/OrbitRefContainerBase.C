// PRQA S 1050 EOF
/*

	Copyright 2006-2013, Advanced Computer Systems , Inc.
	Via della Bufalotta, 347 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

 	$Prod: A.C.S. SafeWrapper Base Library $
 
 	$Id$
 
 	$Author$
 
 	$Log$
 	Revision 1.4  2013/05/28 16:55:50  aleber
 	virtual throw() added to class destructor
 	
 	Revision 1.3  2011/09/08 13:51:35  aleber
 	dumping of RefGroundTrackDirection fixed
 	
 	Revision 1.2  2011/07/16 14:19:21  aleber
 	added data member _ascendingNodeTime
 	
 	Revision 1.1.1.1  2011/07/06 15:56:51  aleber
 	First implementation
 	

*/

#include <OrbitRefContainerBase.h>
#include <Filterables.h>
#include <limits>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;

ACS_CLASS_DEFINE_DEBUG_LEVEL(OrbitRefContainerBase)

const std::string OrbitRefContainerBase::DEFAULT_GROUND_TRACK_DIRECTION = "unknown";
const unsigned int OrbitRefContainerBase::uninitialized = std::numeric_limits<unsigned int>::max() ;


OrbitRefContainerBase::OrbitRefContainerBase(
    unsigned int startOrbitNumber,
	unsigned int stopOrbitNumber,
	unsigned int refOrbitNumber,
	int cycleNumber,
	const std::string & phaseId,
	int startTrackNumber,
	int stopTrackNumber,
	unsigned int relativeStartOrbitNumber,
	unsigned int relativeStopOrbitNumber,
	unsigned int relativeRefOrbitNumber,
	const std::string & additionalData,
	const std::string &  startGroundTrackDirection,
	const std::string & stopGroundTrackDirection,
	const std::string & refGroundTrackDirection,
	const std::string & ascendingNodeTime) :

    _startOrbitNumber(startOrbitNumber),
    _stopOrbitNumber(stopOrbitNumber),
    _refOrbitNumber(refOrbitNumber),
    _cycleNumber(cycleNumber),
    _phaseId(phaseId),
    _startTrackNumber(startTrackNumber),
    _stopTrackNumber(stopTrackNumber),
    _relativeStartOrbitNumber(relativeStartOrbitNumber),
    _relativeStopOrbitNumber(relativeStopOrbitNumber),
    _relativeRefOrbitNumber(relativeRefOrbitNumber),
    _additionalData(additionalData),
    _startGroundTrackDirection(startGroundTrackDirection),
    _stopGroundTrackDirection(stopGroundTrackDirection),
    _refGroundTrackDirection(refGroundTrackDirection),
    _ascendingNodeTime(ascendingNodeTime)


{
	static char Id[] = "@(#) ACS: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Id: " << Id) ;
}

OrbitRefContainerBase::~OrbitRefContainerBase() throw() 
{
}

// copy constructor
OrbitRefContainerBase::OrbitRefContainerBase(const OrbitRefContainerBase & r) :

    _startOrbitNumber(r._startOrbitNumber),
    _stopOrbitNumber(r._stopOrbitNumber),
    _refOrbitNumber(r._refOrbitNumber),
    _cycleNumber(r._cycleNumber),
    _phaseId(r._phaseId),
    _startTrackNumber(r._startTrackNumber),
    _stopTrackNumber(r._stopTrackNumber),
    _relativeStartOrbitNumber(r._relativeStartOrbitNumber),
    _relativeStopOrbitNumber(r._relativeStopOrbitNumber),
    _relativeRefOrbitNumber(r._relativeRefOrbitNumber),
    _additionalData(r._additionalData),
    _startGroundTrackDirection(r._startGroundTrackDirection),
    _stopGroundTrackDirection(r._stopGroundTrackDirection),
    _refGroundTrackDirection(r._refGroundTrackDirection),
    _ascendingNodeTime(r._ascendingNodeTime)
{
}

// operator =
OrbitRefContainerBase&  OrbitRefContainerBase::operator=(const OrbitRefContainerBase & e)
{
	if (this != &e) {
		_startOrbitNumber = e._startOrbitNumber;
		_stopOrbitNumber = e._stopOrbitNumber;
		_refOrbitNumber = e._refOrbitNumber;
		_cycleNumber = e._cycleNumber;
		_phaseId = e._phaseId;
		_startTrackNumber = e._startTrackNumber;
		_stopTrackNumber = e._stopTrackNumber;
		_relativeStartOrbitNumber = e._relativeStartOrbitNumber;
		_relativeStopOrbitNumber = e._relativeStopOrbitNumber;
		_relativeRefOrbitNumber = e._relativeRefOrbitNumber;
		_additionalData = e._additionalData;
		_startGroundTrackDirection = e._startGroundTrackDirection;
		_stopGroundTrackDirection = e._stopGroundTrackDirection;
		_refGroundTrackDirection = e._refGroundTrackDirection;
		_ascendingNodeTime = e._ascendingNodeTime;
	}
	return *this ;
}

string OrbitRefContainerBase::toStr() const 
{                                                                                                           

	ostringstream msg;

	msg << "OrbitRefContainerBase class members: " << '\n'
	    << "\tStartOrbitNumber = " << _startOrbitNumber << '\n'	  		   
	    << "\tStopOrbitNumber = " << _stopOrbitNumber << '\n'			   
	    << "\tRefOrbitNumber = " << _refOrbitNumber << '\n'      			   
	    << "\tCycleNumber = " << _cycleNumber << '\n'				   
	    << "\tPhaseId = " << _phaseId << '\n'					   
	    << "\tStartTrackNumber = " << _startTrackNumber << '\n'			   
	    << "\tStopTrackNumber = " << _stopTrackNumber << '\n'			   
	    << "\tRelativeStartOrbitNumber = " << _relativeStartOrbitNumber << '\n'	   
            << "\tRelativeStopOrbitNumber = " << _relativeStopOrbitNumber << '\n' 	   
            << "\tRelativeRefOrbitNumber = " << _relativeRefOrbitNumber << '\n' 	   
            << "\tAdditionalData = " << _additionalData << '\n' 			   
            << "\tStartGroundTrackDirection = " << _startGroundTrackDirection << '\n'	   
            << "\tStopGroundTrackDirection = " << _stopGroundTrackDirection << '\n' 	   
            << "\tRefGroundTrackDirection = " << _refGroundTrackDirection << '\n' 	   
            << "\tAscendingNodeTime = " << _ascendingNodeTime; 	   

	return msg.str() ; 

}                                                                                                           
                                                                                                            
std::ostream &operator << (std::ostream &os, const OrbitRefContainerBase &v) 
{
    return os << v.toStr() ;
}

acs::exostream &operator << (acs::exostream &os, const OrbitRefContainerBase &v) // PRQA S 4222
{
    return os << v.toStr() ;
}


_ACS_END_NAMESPACE
