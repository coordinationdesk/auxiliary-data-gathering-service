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
	Revision 2.1  2012/04/18 08:15:39  giucas
	compilation warnings fixed
	
	Revision 2.0  2006/02/28 09:32:51  marpas
	Exception 2.1 I/F adopted
	
	Revision 1.1.1.1  2005/02/21 14:43:53  giucas
	Imported libAuxFileParsing
	
	Revision 1.1  2004/12/07 10:09:24  giucas
	Moved from libTimeConverter
	
	Revision 1.3  2004/12/03 09:33:26  giucas
	Header fixed
	
	Revision 1.2  2004/11/16 08:53:03  giucas
	Added filename attribute
	
	Revision 1.1  2004/09/16 16:57:17  giucas
	First release
	


*/

#include <CFGOTimeCorrParams.h>

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;


///////////////////////////////////////////////////////////////////////////
// GoceOBTParams class 
///////////////////////////////////////////////////////////////////////////

CFGOTimeCorrParams::CFGOTimeCorrParams() :
    AuxFileParser::Parameters(),
	fileName(),
	utcTime(),
	utc0Coarse(0),
	utc0Fine(0),
	obt0Coarse(0),
	obt0Fine(0),
	gradient(0.0),
	offset(0.0)
{
}

CFGOTimeCorrParams::~CFGOTimeCorrParams() throw()
{
}

CFGOTimeCorrParams::CFGOTimeCorrParams(const CFGOTimeCorrParams& e):
    AuxFileParser::Parameters(e),
	fileName(e.fileName),
	utcTime(e.utcTime),
	utc0Coarse(e.utc0Coarse),
	utc0Fine(e.utc0Fine),
	obt0Coarse(e.obt0Coarse),
	obt0Fine(e.obt0Fine),
	gradient(e.gradient),
	offset(e.offset)
{
}

CFGOTimeCorrParams& 
CFGOTimeCorrParams::operator=(const CFGOTimeCorrParams& e)
{
	if (this != &e) 
	{
        AuxFileParser::Parameters::operator=(e),
		fileName		= e.fileName;
		utcTime			= e.utcTime;
		utc0Coarse		= e.utc0Coarse;
		utc0Fine		= e.utc0Fine;
		obt0Coarse		= e.obt0Coarse;
		obt0Fine		= e.obt0Fine;
		gradient		= e.gradient;
		offset			= e.offset;		
	}
	return *this ;
}

string CFGOTimeCorrParams::dump() const 
{
	ostringstream msg;
	msg << "---CFGOTimeCorrParams::dump() : fileName   = [" << this->fileName << "]" << '\n'
		<< "---CFGOTimeCorrParams::dump() : utcTime    = [" << this->utcTime << "]" << '\n'
		<< "---CFGOTimeCorrParams::dump() : utc0Coarse = [" << this->utc0Coarse << "]" << '\n'
		<< "---CFGOTimeCorrParams::dump() : utc0Fine   = [" << this->utc0Fine << "]" << '\n'
		<< "---CFGOTimeCorrParams::dump() : obt0Coarse = [" << this->obt0Coarse << "]" << '\n'
		<< "---CFGOTimeCorrParams::dump() : obt0Fine   = [" << this->obt0Fine << "]" << '\n'
		<< "---CFGOTimeCorrParams::dump() : gradient   = [" << setprecision(17) << this->gradient << "]" << '\n' // PRQA S 4400 4
		<< "---CFGOTimeCorrParams::dump() : offset     = [" << setprecision(17) << this->offset << "]" << '\n'
		<< "---CFGOTimeCorrParams::dump() : UTC0       = [" << setprecision(17) << this->getUTC0() << "]" << '\n'
		<< "---CFGOTimeCorrParams::dump() : OBT0       = [" << setprecision(17) << this->getOBT0() << "]" << '\n';
	return msg.str();
}

_ACS_END_NAMESPACE
