// PRQA S 1050 EOF
/*

	Copyright 2006-2013, Advanced Computer Systems , Inc.
	Via della Bufalotta, 378 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

 	$Prod: A.C.S. SafeWrapper Base Library $
 
 	$Id$
 
 	$Author$
 
 	$Log$
 	Revision 1.5  2011/09/12 16:01:36  aleber
 	minor modifications of member comments
 	
 	Revision 1.4  2011/09/12 10:44:57  aleber
 	keyword inline placed outside class scope
 	
 	Revision 1.3  2011/08/01 10:21:35  aleber
 	destructor method made virtual
 	
 	Revision 1.2  2011/07/16 14:20:04  aleber
 	leap second info handled as string type
 	
 	Revision 1.1.1.1  2011/07/06 15:56:51  aleber
 	First implementation
 	
*/

#ifndef _PlatformContainerBase_H_
#define _PlatformContainerBase_H_ 

#include <exStream.h>

_ACS_BEGIN_NAMESPACE(acs)


class PlatformContainerBase
{
public:

	/**
	 * \brief  Class constructor
	 *    
	 */
	explicit PlatformContainerBase(double referenceUtcTime = 0.,
				  unsigned long satelliteReferenceBinaryTime = 0,
				  unsigned long clockStepLength = 0,
				  unsigned int swathNumber = 0,
				  const std::string& leapSecondUtcTimeOfOccurrence = "",
				  const std::string& leapSecondPositiveSign = "");	

	/**
	 * \brief  Class destructor 
	 *    
	 */
    virtual ~PlatformContainerBase() throw() ;

	/**
	 * \brief  Class copy constructor 
	 *    
	 */
	PlatformContainerBase(const PlatformContainerBase & ); 

	/**
	 * \brief  Operator= 
	 *    
	 */
	PlatformContainerBase &operator=(const PlatformContainerBase &) ;

	double referenceUtcTime() const throw() { return _referenceUtcTime; } 
	unsigned long satelliteReferenceBinaryTime() const throw() { return _satelliteReferenceBinaryTime; }
	unsigned long clockStepLength() const throw() { return _clockStepLength; } 
	unsigned int swathNumber() const throw() { return _swathNumber; } 
	const std::string &leapSecondTimeOfOccurrence() const throw() { return _leapSecondUtcTimeOfOccurrence; } 
	const std::string &leapSecondPositiveSign() const throw() { return _leapSecondPositiveSign; }

	virtual std::string toStr() const ;


private:

	double 			_referenceUtcTime;
	unsigned long 	_satelliteReferenceBinaryTime;
	unsigned long 	_clockStepLength;
	unsigned int 	_swathNumber;
	std::string 	_leapSecondUtcTimeOfOccurrence;
	std::string   	_leapSecondPositiveSign;


	ACS_CLASS_DECLARE_DEBUG_LEVEL(PlatformContainerBase) ;

};

std::ostream &operator << (std::ostream &, const PlatformContainerBase &) ;
acs::exostream &operator << (acs::exostream &, const PlatformContainerBase &) ; // PRQA S 2072


_ACS_END_NAMESPACE

#endif //_PlatformContainerBase_H_
