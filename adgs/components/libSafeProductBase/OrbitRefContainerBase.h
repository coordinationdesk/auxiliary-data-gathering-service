// PRQA S 1050 EOF
/*

	Copyright 2011-2013, Advanced Computer Systems , Inc.
	Via della Bufalotta, 378 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

 	$Prod: A.C.S. Safe Wrapper Base Library $
 
 	$Id$
 
 	$Author$
 
 	$Log$
 	Revision 1.5  2013/05/28 16:55:50  aleber
 	virtual throw() added to class destructor
 	
 	Revision 1.4  2011/09/12 16:01:36  aleber
 	minor modifications of member comments
 	
 	Revision 1.3  2011/09/12 10:44:57  aleber
 	keyword inline placed outside class scope
 	
 	Revision 1.2  2011/07/16 14:19:21  aleber
 	added data member _ascendingNodeTime
 	
 	Revision 1.1.1.1  2011/07/06 15:56:51  aleber
 	First implementation
 	

*/

#ifndef _OrbitRefContainerBase_H_
#define _OrbitRefContainerBase_H_ 

#include <exStream.h>
#include <string>

_ACS_BEGIN_NAMESPACE(acs)

class OrbitRefContainerBase
{
public:

    static const std::string DEFAULT_GROUND_TRACK_DIRECTION ; //  = "unknown"; // PRQA S 2100
	/**
	 * \brief  Class constructor
	 *    
	 */
    static const unsigned int uninitialized ; //  std::numeric_limits<unsigned int>::max() ; // PRQA S 2100
    
	explicit OrbitRefContainerBase(	
			unsigned int startOrbitNumber = uninitialized,
			unsigned int stopOrbitNumber = uninitialized,
			unsigned int refOrbitNumber = uninitialized,
		    int cycleNumber = 0,
			const std::string & phaseId = "",
			int startTrackNumber = -1,
			int stopTrackNumber = -1,
			unsigned int relativeStartOrbitNumber = uninitialized,
			unsigned int relativeStopOrbitNumber = uninitialized,
			unsigned int relativeRefOrbitNumber = uninitialized,
			const std::string &additionalData = "",
			const std::string &startGroundTrackDirection = DEFAULT_GROUND_TRACK_DIRECTION,
			const std::string &stopGroundTrackDirection = DEFAULT_GROUND_TRACK_DIRECTION,
			const std::string &refGroundTrackDirection = DEFAULT_GROUND_TRACK_DIRECTION,
			const std::string &ascendingNodeTime = "");

	/**
	 * \brief  Class destructor 
	 *    
	 */
    virtual ~OrbitRefContainerBase() throw();

	/**
	 * \brief  Class copy constructor 
	 *    
	 */
	OrbitRefContainerBase(const OrbitRefContainerBase & ); 

	/**
	 * \brief  Operator= 
	 *    
	 */
	OrbitRefContainerBase &operator=(const OrbitRefContainerBase &) ;

	unsigned int startOrbitNumber() const throw() { return _startOrbitNumber; }
	void setStartOrbitNumber(unsigned int v) throw() { _startOrbitNumber = v ; } 

	unsigned int stopOrbitNumber() const throw() { return _stopOrbitNumber ; }
	void setStopOrbitNumber(unsigned int v) throw() { _stopOrbitNumber = v ; } 

	unsigned int refOrbitNumber() const throw() { return _refOrbitNumber; }
	void setRefOrbitNumber(unsigned int v) throw() { _refOrbitNumber = v ; } 

	int cycleNumber() const throw() { return _cycleNumber ; }
	void setCycleNumber(int v) throw() { _cycleNumber = v ; } 

	const std::string& phaseId() const throw() { return _phaseId ; }
	void setPhaseId(const std::string& v) { _phaseId = v ; }

	int startTrackNumber() const throw() { return _startTrackNumber ; }
	void setStartTrackNumber(int v) throw() { _startTrackNumber = v ; } 

	int stopTrackNumber() const throw() { return _stopTrackNumber ; }
	void setStopTrackNumber(int v) throw() { _stopTrackNumber = v ; } 

	unsigned int relativeStartOrbitNumber() const throw() { return _relativeStartOrbitNumber ; }
	void setRelativeStartOrbitNumber(unsigned int v) throw() { _relativeStartOrbitNumber = v ; } 

	unsigned int relativeStopOrbitNumber() const throw() { return _relativeStopOrbitNumber ; }
	void setRelativeStopOrbitNumber(unsigned int v) throw() { _relativeStopOrbitNumber = v ; } 

	unsigned int relativeRefOrbitNumber() const throw() { return _relativeRefOrbitNumber ; }
	void setRelativeRefOrbitNumber(unsigned int v) throw() { _relativeRefOrbitNumber = v ; } 

	const std::string& additionalData() const throw() { return _additionalData ; }
	void setAdditionalData(const std::string& v) { _additionalData = v ; } 

	const std::string& startGroundTrackDirection() const throw() { return _startGroundTrackDirection ; }
	void setStartGroundTrackDirection(const std::string& v) { _startGroundTrackDirection = v ; } 

	const std::string& stopGroundTrackDirection() const throw() { return _stopGroundTrackDirection ; }
	void setStopGroundTrackDirection(const std::string& v) { _stopGroundTrackDirection = v ; } 

	const std::string& refGroundTrackDirection() const throw() { return _refGroundTrackDirection ; }
	void setRefGroundTrackDirection(const std::string&  v) { _refGroundTrackDirection = v ; } 

	const std::string& ascendingNodeTime() const throw() { return _ascendingNodeTime ; }
	void setAscendingNodeTime(const std::string& v) { _ascendingNodeTime = v ; } 

	virtual std::string toStr() const ;

private:

	unsigned int _startOrbitNumber;
	unsigned int _stopOrbitNumber;
	unsigned int _refOrbitNumber;
	int _cycleNumber;
	std::string _phaseId;
	int _startTrackNumber;
	int _stopTrackNumber;
	unsigned int _relativeStartOrbitNumber;
	unsigned int _relativeStopOrbitNumber;
	unsigned int _relativeRefOrbitNumber;
	std::string _additionalData;
	std::string _startGroundTrackDirection;
	std::string _stopGroundTrackDirection;
	std::string _refGroundTrackDirection;
	std::string _ascendingNodeTime;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(OrbitRefContainerBase) ;

};

std::ostream &operator << (std::ostream &, const OrbitRefContainerBase &) ;
acs::exostream &operator << (acs::exostream &, const OrbitRefContainerBase &) ; // PRQA S 2072

_ACS_END_NAMESPACE

#endif //_OrbitRefContainerBase_H_
