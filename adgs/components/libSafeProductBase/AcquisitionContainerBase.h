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
 	Revision 1.3  2011/09/12 16:01:36  aleber
 	minor modifications of member comments
 	
 	Revision 1.2  2011/09/12 10:44:57  aleber
 	keyword inline placed outside class scope
 	
 	Revision 1.1  2011/09/12 08:56:52  aleber
 	first implementation
 	
*/

#ifndef _AcquisitionContainerBase_H_
#define _AcquisitionContainerBase_H_ 

#include <exStream.h>

_ACS_BEGIN_NAMESPACE(acs)

class AcquisitionContainerBase
{
public:

	/**
	 * \brief  Class constructor
	 *    
	 */
	explicit AcquisitionContainerBase(const std::string& startTime = "",
				 const std::string& stopTime = "",
				 double startTimeAnx = -1.,
				 double stopTimeAnx = -1.);	

	/**
	 * \brief  Class destructor 
	 *    
	 */
    virtual ~AcquisitionContainerBase() throw() ;

	/**
	 * \brief  Class copy constructor 
	 *    
	 */
	AcquisitionContainerBase(const AcquisitionContainerBase & ); 

	/**
	 * \brief  Operator= 
	 *    
	 */
	AcquisitionContainerBase &operator=(const AcquisitionContainerBase &) ;

	virtual const std::string &startTime() const throw() { return _startTime; } // PRQA S 2131 4
	virtual const std::string &stopTime() const throw() { return _stopTime; } 
	virtual double startTimeAnx() const throw() { return _startTimeAnx; } 
	virtual double stopTimeAnx() const throw() { return _stopTimeAnx; }

	virtual std::string toStr() const ;


private:

	std::string 	_startTime;
	std::string   	_stopTime;
	double 			_startTimeAnx;
	double 			_stopTimeAnx;


	ACS_CLASS_DECLARE_DEBUG_LEVEL(AcquisitionContainerBase) ;

};


std::ostream &operator << (std::ostream &, const AcquisitionContainerBase &) ;
acs::exostream &operator << (acs::exostream &, const AcquisitionContainerBase &) ; // PRQA S 2072


_ACS_END_NAMESPACE


#endif //_AcquisitionContainerBase_H_
