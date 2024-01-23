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
	Revision 2.1  2013/03/28 14:43:35  lucio.pulvirenti
	Copy CTOR fixed. Parameter initialization in CTOR.
	
	Revision 2.0  2006/02/28 09:32:51  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2005/02/21 14:43:53  giucas
	Imported libAuxFileParsing
	
	Revision 1.1  2004/12/07 10:09:24  giucas
	Moved from libTimeConverter
	
	Revision 1.3  2004/12/03 09:33:26  giucas
	Header fixed
	
	Revision 1.2  2004/10/08 09:03:36  giucas
	Fixed dump method
	
	Revision 1.1  2004/09/17 14:29:49  giucas
	First Release
	


*/

#include <USCTimeCorrParams.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;


///////////////////////////////////////////////////////////////////////////
// GoceOBTParams class 
///////////////////////////////////////////////////////////////////////////

USCTimeCorrParams::USCTimeCorrParams() : 
    AuxFileParser::Parameters(),
	refTime(0.0),
	refSBT(),
	clockStep(0)
{
}

USCTimeCorrParams::~USCTimeCorrParams() throw()
{
}

USCTimeCorrParams::USCTimeCorrParams(const USCTimeCorrParams& e):
    AuxFileParser::Parameters(e),
	refTime(e.refTime),
	refSBT(e.refSBT),
	clockStep(e.clockStep)
{
}

USCTimeCorrParams& 
USCTimeCorrParams::operator=(const USCTimeCorrParams& e)
{
	if (this != &e) 
	{
        AuxFileParser::Parameters::operator=(e),
		refTime =e.refTime;
		refSBT =e.refSBT;
		clockStep =e.clockStep;
	}
	return *this ;
}

string USCTimeCorrParams::dump() const
{
	ostringstream msg;
	msg << "---USCTimeCorrParams::dump() : refTime     = [ " << setprecision(17) << this->refTime << " ]" << '\n' // PRQA S 4400
		<< "---USCTimeCorrParams::dump() : clockStep   = [ " << this->clockStep << " ]" << '\n';
	for(unsigned int i=0; i<refSBT.size(); i++)
	{
		msg << "---USCTimeCorrParams::dump() : refSBT[" << i << "]   = [ " << refSBT[i] << " ]" << '\n';
	}
	return msg.str();
}

_ACS_END_NAMESPACE
