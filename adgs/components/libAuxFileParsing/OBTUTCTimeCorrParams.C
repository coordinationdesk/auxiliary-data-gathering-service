// PRQA S 1050 EOF
/*

	Copyright 1995-2014, Advanced Computer Systems , Inc.
	Via della Bufalotta, 378 Roma

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
	
	Revision 2.1  2012/04/18 08:15:40  giucas
	compilation warnings fixed
	
	Revision 2.0  2006/02/28 09:32:51  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.4  2005/11/02 13:25:33  giucas
	Added new long double param and operators implemented
	
	Revision 1.3  2005/10/28 18:05:15  danalt
	added parameters for compatibility with test data generation
	
	Revision 1.2  2005/10/25 17:16:15  giucas
	Modified to use for COSMO OBT-UTC conversion
	
	Revision 1.1.1.1  2005/02/21 14:43:53  giucas
	Imported libAuxFileParsing
	
	Revision 1.1  2005/02/02 09:18:59  danalt
	first version
	
	


*/

#include <OBTUTCTimeCorrParams.h>


_ACS_BEGIN_NAMESPACE(acs)
using namespace std;



///////////////////////////////////////////////////////////////////////////
// GoceOBTParams class 
///////////////////////////////////////////////////////////////////////////

OBTUTCTimeCorrParams::OBTUTCTimeCorrParams() :
    AuxFileParser::Parameters(),
	refMission(0), 
	OBRTUnits(0),
	refOBTTime(),
	refUTCTime(),
	dRefUTCTime(0.0)
{
}

OBTUTCTimeCorrParams::~OBTUTCTimeCorrParams() throw() 
{
}

OBTUTCTimeCorrParams::OBTUTCTimeCorrParams(const OBTUTCTimeCorrParams& e):
    AuxFileParser::Parameters(e),
	refMission(e.refMission), 
	OBRTUnits(e.OBRTUnits),
	refOBTTime(e.refOBTTime),
	refUTCTime(e.refUTCTime),
	dRefUTCTime(e.dRefUTCTime)
{
	*this = e;
}

OBTUTCTimeCorrParams& 
OBTUTCTimeCorrParams::operator=(const OBTUTCTimeCorrParams& e)
{
	if (this != &e) 
	{
        AuxFileParser::Parameters::operator=(e) ;
		refMission      = e.refMission;
		OBRTUnits		= e.OBRTUnits;
		refUTCTime		= e.refUTCTime;
		refOBTTime      = e.refOBTTime;
		dRefUTCTime		= e.dRefUTCTime;
	}
	return *this ;
}



string OBTUTCTimeCorrParams::dump() const
{
	ostringstream msg;
	msg << "---OBTUTCTimeCorrParams::dump() : OBRTUnits   = [ " << setprecision(17) << OBRTUnits << " ]" << '\n' // PRQA S 4400 5
	    << "---OBTUTCTimeCorrParams::dump() : refOBTTime  = [ " << setprecision(17) << refOBTTime << " ]" << '\n'
	    << "---OBTUTCTimeCorrParams::dump() : refUTCTime  = [ " << setprecision(17) << refUTCTime << " ]" << '\n'
	    << "---OBTUTCTimeCorrParams::dump() : dRefUTCTime = [ " << setprecision(17) << dRefUTCTime << " ]" << '\n'
	    << "---OBTUTCTimeCorrParams::dump() : refMission  = [ " << refMission << " ]" << '\n';
	return msg.str();
}

_ACS_END_NAMESPACE

