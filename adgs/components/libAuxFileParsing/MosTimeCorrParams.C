// PRQA S 1050 EOF
/*

	Copyright 1995-2021, Advanced Computer Systems , Inc.
	Via della Bufalotta, 347 Roma

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Author$

	@(#) A.C.S.: $Id$
	
	$Log$
	Revision 1.3  2014/03/19 19:14:55  marpas
	coding best practices application in progress (not ended)
	warning fixing (qa & g++)
	
	Revision 1.2  2012/04/18 08:15:40  giucas
	compilation warnings fixed
	
	Revision 1.1  2009/07/13 13:27:35  marant
	Class MosTimeCorrParams added
	


*/

#include <MosTimeCorrParams.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(MosTimeCorrParams)


///////////////////////////////////////////////////////////////////////////
// GoceOBTParams class 
///////////////////////////////////////////////////////////////////////////

MosTimeCorrParams::MosTimeCorrParams() : 
    AuxFileParser::Parameters(),
	time_code(),
    reference_j50(),
    clock_step_length(),
    l0_line_number(),
    l0_time_code(),
    l0_reference_j50(),
    AcrossTrackBias()
{
	static char Id[] = "@(#) A.C.S.: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Id: " << Id );
}

MosTimeCorrParams::~MosTimeCorrParams() throw() 
{
}

MosTimeCorrParams::MosTimeCorrParams(const MosTimeCorrParams& e) : 
    AuxFileParser::Parameters(e), 
	time_code(e.time_code),
    reference_j50(e.reference_j50),
    clock_step_length(e.clock_step_length),
    l0_line_number(e.l0_line_number),
    l0_time_code(e.l0_time_code),
    l0_reference_j50(e.l0_reference_j50),
    AcrossTrackBias(e.AcrossTrackBias)
{
}

MosTimeCorrParams& 
MosTimeCorrParams::operator=(const MosTimeCorrParams& e)
{
	if (this != &e) 
	{
        AuxFileParser::Parameters::operator=(e) ; 
		time_code = e.time_code;
        reference_j50 = e.reference_j50;
        clock_step_length = e.clock_step_length;
        l0_line_number = e.l0_line_number;
        l0_time_code = e.l0_time_code;
        l0_reference_j50 = e.l0_reference_j50;
        AcrossTrackBias = e.AcrossTrackBias;
	}
	return *this ;
}

string MosTimeCorrParams::dump() const
{
	ostringstream msg;
	msg 	<< "---MosTimeCorrParams::dump() : time_code		= [ " << time_code  	   	<< " ]" << '\n'
		<< "---MosTimeCorrParams::dump() : reference_j50		= [ " << reference_j50		<< " ]" << '\n'
		<< "---MosTimeCorrParams::dump() : clock_step_length	= [ " << clock_step_length	   	<< " ]" << '\n'
		<< "---MosTimeCorrParams::dump() : l0_line_number   	= [ " << l0_line_number     	<< " ]" << '\n'
		<< "---MosTimeCorrParams::dump() : l0_time_code	        = [ " << l0_time_code 	<< " ]" << '\n'
		<< "---MosTimeCorrParams::dump() : l0_reference_j50   	= [ " << l0_reference_j50 	<< " ]" << '\n';
		
	return msg.str();
}

_ACS_END_NAMESPACE
