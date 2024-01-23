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
	Revision 2.4  2014/03/19 19:14:56  marpas
	coding best practices application in progress (not ended)
	warning fixing (qa & g++)
	
	Revision 2.3  2013/03/28 14:54:13  lucio.pulvirenti
	CTOR and Copy CTOR fixed.
	
	Revision 2.2  2011/09/22 08:51:47  giucas
	dump improved
	
	Revision 2.1  2011/09/22 07:33:43  giucas
	Added S1OBMemConfParams
	


*/

#include <S1OBMemConfParams.h>
#include <Filterables.h>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(S1OBMemConfParams)


///////////////////////////////////////////////////////////////////////////
// GoceOBTParams class 
///////////////////////////////////////////////////////////////////////////

S1OBMemConfParams::S1OBMemConfParams():
    AuxFileParser::Parameters(),
	_psInfo(),
	_vcidDataInfo()
{
	static char Id[] = "@(#) A.C.S.: $Id$" ;
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB,"Id: " << Id );
}

S1OBMemConfParams::~S1OBMemConfParams() throw() 
{
}

S1OBMemConfParams::S1OBMemConfParams(const S1OBMemConfParams& e):
    AuxFileParser::Parameters(e),
	_psInfo(e._psInfo),
	_vcidDataInfo(e._vcidDataInfo)
{
}

S1OBMemConfParams& 
S1OBMemConfParams::operator=(const S1OBMemConfParams& e)
{
	if (this != &e) 
	{
        AuxFileParser::Parameters::operator=(e) ;
		_psInfo			=e._psInfo;
		_vcidDataInfo	=e._vcidDataInfo;
	}
	return *this ;
}

string S1OBMemConfParams::dump() const
{
	ostringstream msg;
	std::map<std::string,std::pair<std::string,std::string> >::const_iterator it = _psInfo.begin() ;
	while( it != _psInfo.end() )
	{
		msg << "S1OBMemConfParams : PacketStore '" << it->first << "' [ " << (it->second).first << " , " << (it->second).second << " ]" << '\n' ;
		it++ ;
	}
	std::map<unsigned int,std::string>::const_iterator it2 = _vcidDataInfo.begin() ;
	while( it2 != _vcidDataInfo.end() )
	{
		msg << "S1OBMemConfParams : Data VCID " << it2->first << " PacketStore Type is '" << (it2->second) << "'" << '\n' ;
		it2++ ;
	}
	return msg.str();
}

_ACS_END_NAMESPACE
