// PRQA S 1050 EOF
/*
	Copyright 1995-2014, Advanced Computer Systems , Inc.
	Via della Bufalotta, 347 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Id$

	$Author$

	$Log$
	Revision 2.1  2013/03/28 14:35:49  lucio.pulvirenti
	Copy CTOR fixed.
	
	Revision 2.0  2006/02/28 09:32:51  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/11/16 10:33:45  giucas
	Added operators < and <=
	
	Revision 1.1.1.1  2005/02/21 14:43:53  giucas
	Imported libAuxFileParsing
	
	Revision 1.2  2004/12/09 16:15:46  giucas
	Changed attribute names
	
	Revision 1.1  2004/12/06 17:34:07  giucas
	Imported sources
	
*/

#include <PredictedOrbitData.h>



_ACS_BEGIN_NAMESPACE(acs)

using namespace std;


//Class constructor
PredictedOrbitData::PredictedOrbitData() :
    AuxFileParser::Parameters(),
	taiTime(0.0),
	utcTime(0.0),
	ut1Time(0.0),
	xPosition(0.0),
	yPosition(0.0),
	zPosition(0.0),
	xVelocity(0.0),
	yVelocity(0.0),
	zVelocity(0.0),
	absOrbit(0)
{
}


//Class destructor
PredictedOrbitData::~PredictedOrbitData() throw() 
{
}


//copy constructor
PredictedOrbitData::PredictedOrbitData(const PredictedOrbitData & e):
    AuxFileParser::Parameters(e),
	taiTime(e.taiTime),
	utcTime(e.utcTime),
	ut1Time(e.ut1Time),
	xPosition(e.xPosition),
	yPosition(e.yPosition),
	zPosition(e.zPosition),
	xVelocity(e.xVelocity),
	yVelocity(e.yVelocity),
	zVelocity(e.zVelocity),
	absOrbit(e.absOrbit)
{
}


//operator =
PredictedOrbitData&  PredictedOrbitData::operator=(const PredictedOrbitData & e)
{
	if (this != &e) 
	{
        AuxFileParser::Parameters::operator=(e); 
		taiTime		= e.taiTime;
		utcTime		= e.utcTime;
		ut1Time		= e.ut1Time;
		xPosition	= e.xPosition;
		yPosition	= e.yPosition;
		zPosition	= e.zPosition;
		xVelocity	= e.xVelocity;
		yVelocity	= e.yVelocity;
		zVelocity	= e.zVelocity;
		absOrbit	= e.absOrbit;
	}
	return *this ;
}

string PredictedOrbitData::show() const
{
	ostringstream msg;
	msg << "PredictedOrbitData::show" << '\n'
		<< "---TAI Time = " << setprecision(12) << taiTime << '\n' // PRQA S 4400 9
		<< "---UTC Time = " << setprecision(12) << utcTime << '\n'
		<< "---UT1 Time = " << setprecision(12) << ut1Time << '\n'
		<< "---X Position = " << setprecision(12) << xPosition << " m " << '\n'
		<< "---Y Position = " << setprecision(12) << yPosition << " m " << '\n'
		<< "---Z Position = " << setprecision(12) << zPosition << " m " << '\n'
		<< "---X Velocity = " << setprecision(12) << xVelocity << " m/s " << '\n'
		<< "---Y Velocity = " << setprecision(12) << yVelocity << " m/s " << '\n'
		<< "---Z Velocity = " << setprecision(12) << zVelocity << " m/s " << '\n'
		<< "---ABS ORBIT  = " << absOrbit << '\n';
	return msg.str();
}

_ACS_END_NAMESPACE
