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
	Revision 2.1  2012/04/18 08:15:40  giucas
	compilation warnings fixed
	
	Revision 2.0  2006/02/28 09:32:51  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2005/02/21 14:43:53  giucas
	Imported libAuxFileParsing
	
	Revision 1.2  2004/12/07 14:34:00  giucas
	Added show method
	
	Revision 1.1  2004/12/07 11:31:24  giucas
	Moved from libACSOrbit
	

*/

#include <OrbitScenario.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;


//Class constructors
OrbitScenario::OrbitScenario() :
    AuxFileParser::Parameters(),
	_relOrbit(0),
	_absOrbit(0),
	_cycle(0),
	_phase(0),
	_satID(0),
	_repeatCycle(0),
	_cycleLength(0),
	_drift(0.0),
	_ascMlst(0.0),
	_ANXTime(0.0),
	_ANXLon(0.0)
{
} 



//Class destructor
OrbitScenario::~OrbitScenario() throw() 
{
}



//copy constructor
OrbitScenario::OrbitScenario(const OrbitScenario & e)  :
    AuxFileParser::Parameters(e),
	_relOrbit(e._relOrbit),
	_absOrbit(e._absOrbit),
	_cycle(e._cycle),
	_phase(e._phase),
	_satID(e._satID),
	_repeatCycle(e._repeatCycle),
	_cycleLength(e._cycleLength),
	_drift(e._drift),
	_ascMlst(e._ascMlst),
	_ANXTime(e._ANXTime),
	_ANXLon(e._ANXLon)
{
}

//operator = 
OrbitScenario&  OrbitScenario::operator=(const OrbitScenario & e)
{
	if (this != &e) 
	{
        AuxFileParser::Parameters::operator=(e) ;
		_relOrbit		= e._relOrbit;
		_absOrbit		= e._absOrbit;
		_cycle			= e._cycle;
		_phase			= e._phase;
		_satID			= e._satID;
		_repeatCycle	= e._repeatCycle;
		_cycleLength	= e._cycleLength;
		_drift			= e._drift;
		_ascMlst		= e._ascMlst;
		_ANXTime		= e._ANXTime;
		_ANXLon			= e._ANXLon;
	}
	return *this ;
}

string OrbitScenario::show() const
{
	ostringstream msg;
	msg << "OrbitScenario::show():" << '\n'
		<< "--- satID       = " << satID() << '\n'
		<< "--- relOrbit    = " << relOrbit() << '\n'
		<< "--- absOrbit    = " << absOrbit() << '\n'
		<< "--- cycle       = " << cycle() << '\n'
		<< "--- phase       = " << phase() << '\n'
		<< "--- repeatCycle = " << repeatCycle() << '\n'
		<< "--- cycleLength = " << cycleLength() << '\n'
		<< "--- drift       = " << setprecision(12) << drift() << '\n'	// PRQA S 4400 4
		<< "--- ascMlst     = " << setprecision(12)<< ascMlst() << '\n'
		<< "--- ANXTime     = " << setprecision(12)<< ANXTime() << '\n'
		<< "--- ANXLon      = " << setprecision(12)<< ANXLon() << '\n';
	return msg.str();
}
_ACS_END_NAMESPACE
