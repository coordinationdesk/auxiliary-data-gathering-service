// PRQA S 1050 EOF
/*

	Copyright 1995-2014, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$
	
	$Log$
	Revision 2.2  2014/03/19 19:14:55  marpas
	coding best practices application in progress (not ended)
	warning fixing (qa & g++)
	
	Revision 2.1  2006/10/18 18:08:14  marpas
	hopefully almost complete refactoring
	
	Revision 2.0  2006/02/28 09:32:51  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2005/02/21 14:43:53  giucas
	Imported libAuxFileParsing
	
	Revision 1.2  2004/12/07 14:34:00  giucas
	Added show method
	
	Revision 1.1  2004/12/07 11:31:24  giucas
	Moved from libACSOrbit
	

*/

#ifndef _OrbitScenario_H_
#define _OrbitScenario_H_ 


#include <exException.h>
#include <AuxFileParser.h>


_ACS_BEGIN_NAMESPACE(acs)

	
class OrbitScenario : public AuxFileParser::Parameters // PRQA S 2153
{
public:


	/**
	 * \brief  Default Class constructor 
	 *    
	 */	
	OrbitScenario();


	/**
	 * \brief  Class destructor 
	 *    
	 */
	virtual ~OrbitScenario() throw() ;

	/**
	 * \brief  Class copy constructor 
	 *    
	 */ 
	OrbitScenario(const OrbitScenario & ); 

	/**
	 * \brief  Operator= 
	 *    
	 */
	OrbitScenario &operator=(const OrbitScenario &);

	/**
	 * \brief  Set the SatID field value
	 *    
	 */
	void satID(long int value) throw() { _satID = value; };

	/**
	 * \brief  Set the Relative Orbit field value
	 *    
	 */
	void relOrbit(long int value) throw() { _relOrbit = value; };

	/**
	 * \brief  Set the Absolute Orbit field value
	 *    
	 */
	void absOrbit(long int value) throw() { _absOrbit = value; };

	/**
	 * \brief  Set the Cycle field value
	 *    
	 */
	void cycle(long int c) throw() { _cycle = c; };

	/**
	 * \brief  Set the Phase field value
	 *    
	 */
	void phase(long int ph) throw()  { _phase = ph; };

	/**
	 * \brief  Set the Repeat Cycle (day) field value
	 *    
	 */
	void repeatCycle(long int value) throw() { _repeatCycle = value; };

	/**
	 * \brief  Set the Cycle Length (orbit) field value
	 *    
	 */
	void cycleLength(long int value) throw() { _cycleLength = value; };

	/**
	 * \brief  Set the Drift (sec/day) field value
	 *    
	 */
	void drift(double value) throw() { _drift = value; };

	/**
	 * \brief  Set the Ascending MLST (sec) field value
	 *    
	 */
	void ascMlst(double value) throw() { _ascMlst = value; };

	/**
	 * \brief  Set the ANX time (jd) field value
	 *    
	 */
	void ANXTime(long double value) throw() { _ANXTime = value; };

	/**
	 * \brief  Set the ANX Lon (deg) field value
	 *    
	 */
	void ANXLon(double value) throw() { _ANXLon = value; };



	/**
	 * \brief  Get the SatID field value
	 *    
	 */
	long int satID() const throw() { return _satID; };

	/**
	 * \brief  Get the Relative Orbit field value
	 *    
	 */
	long int relOrbit() const throw() { return _relOrbit; };

	/**
	 * \brief  Get the Absolute Orbit field value
	 *    
	 */
	long int absOrbit() const throw() { return _absOrbit; };

	/**
	 * \brief  Get the Cycle field value
	 *    
	 */
	long int cycle() const throw() { return _cycle; };

	/**
	 * \brief  Get the Phase field value
	 *    
	 */
	long int phase() const throw() { return _phase; };

	/**
	 * \brief  Get the Repeat Cycle (day) field value
	 *    
	 */
	long int repeatCycle() const throw() { return _repeatCycle; };

	/**
	 * \brief  Get the Cycle Length (orbit) field value
	 *    
	 */
	long int cycleLength() const throw() { return _cycleLength; };

	/**
	 * \brief  Get the Drift (sec/day) field value
	 *    
	 */
	double drift() const throw() { return _drift; };

	/**
	 * \brief  Get the Ascending MLST (sec) field value
	 *    
	 */
	double ascMlst() const throw() { return _ascMlst; };

	/**
	 * \brief  Get the ANX time (jd) field value
	 *    
	 */
	long double ANXTime() const throw() { return _ANXTime; };

	/**
	 * \brief  Get the ANX Lon (deg) field value
	 *    
	 */
	double ANXLon() const throw() { return _ANXLon; };

	/**
	 * \brief Show the object state
	 */
	std::string show() const;		

private:

	long int 			_relOrbit;
	long int 			_absOrbit;
	long int 			_cycle;
	long int 			_phase;
	long int 			_satID;
	long int 			_repeatCycle;
	long int 			_cycleLength;
	double				_drift;
	double				_ascMlst;
	long double         _ANXTime;
	double				_ANXLon;

};

_ACS_END_NAMESPACE


#endif //_OrbitScenario_H_
