// PRQA S 1050 EOF
/*

	Copyright 1995-2014, Advanced Computer Systems , Inc.
	Via della Bufalotta, 347 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$
	
	$Log$
	Revision 2.1  2013/03/28 14:42:46  lucio.pulvirenti
	Copy CTOR fixed.
	
	Revision 2.0  2006/02/28 09:32:51  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.2  2005/09/16 11:54:47  seralb
	Added some fields for the ErsOrbitDataFileParser
	
	Revision 1.1  2005/07/01 14:10:36  seralb
	Added ERS satellite
	


*/

#include <ErsTimeCorrParams.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;


///////////////////////////////////////////////////////////////////////////
// ErsTimeCorrParams class 
///////////////////////////////////////////////////////////////////////////

ErsTimeCorrParams::ErsTimeCorrParams() :
    AuxFileParser::Parameters(), 
	day(0),
	ms(0),
	orbit(0),
	BinTime(0),
	ClockPeriod(0),
	AscNodeUTC(0.0),
	ReferenceUTC(0.0),
	SatBinTime(0),
	ClockStepLength(0)
{
}

ErsTimeCorrParams::~ErsTimeCorrParams() throw()
{
}

ErsTimeCorrParams::ErsTimeCorrParams(const ErsTimeCorrParams& e):
    AuxFileParser::Parameters(e), 
	day(e.day),
	ms(e.ms),
	orbit(e.orbit),
	BinTime(e.BinTime),
	ClockPeriod(e.ClockPeriod),
	AscNodeUTC(e.AscNodeUTC),
	ReferenceUTC(e.ReferenceUTC),
	SatBinTime(e.SatBinTime),
	ClockStepLength(e.ClockStepLength)
{
}

ErsTimeCorrParams& 
ErsTimeCorrParams::operator=(const ErsTimeCorrParams& e)
{
	if (this != &e) 
	{
        AuxFileParser::Parameters::operator=(e), 
		day		=e.day;
		ms		=e.ms;
		orbit		=e.orbit;
		BinTime	 	=e.BinTime;
		ClockPeriod	=e.ClockPeriod;
		AscNodeUTC	=e.AscNodeUTC;
		ReferenceUTC	=e.ReferenceUTC;
		SatBinTime	=e.SatBinTime;
		ClockStepLength	=e.ClockStepLength;
	}
	return *this ;
}

string ErsTimeCorrParams::dump() const
{
	ostringstream msg;
	msg 	<< "---ErsTimeCorrParams::dump() : day		= [ " << this->day  	   	<< " ]" << '\n'
		<< "---ErsTimeCorrParams::dump() : ms		= [ " << this->ms		<< " ]" << '\n'
		<< "---ErsTimeCorrParams::dump() : orbit	= [ " << this->orbit	   	<< " ]" << '\n'
		<< "---ErsTimeCorrParams::dump() : BinTime   	= [ " << this->BinTime     	<< " ]" << '\n'
		<< "---ErsTimeCorrParams::dump() : ClockPeriod	= [ " << this->ClockPeriod 	<< " ]" << '\n'
		<< "---ErsTimeCorrParams::dump() : ClockPeriod	= [ " << this->AscNodeUTC 	<< " ]" << '\n'
		<< "---ErsTimeCorrParams::dump() : ClockPeriod	= [ " << this->ReferenceUTC 	<< " ]" << '\n'
		<< "---ErsTimeCorrParams::dump() : ClockPeriod	= [ " << this->SatBinTime 	<< " ]" << '\n'
		<< "---ErsTimeCorrParams::dump() : ClockPeriod	= [ " << this->ClockStepLength 	<< " ]" << '\n';
	return msg.str();
}

_ACS_END_NAMESPACE
