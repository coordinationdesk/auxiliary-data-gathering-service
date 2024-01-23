/*
	Copyright 1995-2023, Exprivia SpA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. libS1L2AuxReformatter$

	$Id$

	$Author$

	$Log$
	Revision 1.4  2013/12/11 15:47:56  nicvac
	Coding rules implementation (by marpas?)
	
	Revision 1.3  2013/10/24 14:05:27  nicvac
	S1PDGS-1899: file naming conversion.
	
	Revision 1.2  2013/10/17 15:56:11  nicvac
	compilation warnings processed.
	
	Revision 1.1  2012/04/23 08:02:38  damdec
	Class added.
	
        
*/ 

#include <S1AuxWavReformatter.h>

#include <Filterables.h>
#include <RegExp.h>
#include <StringUtils.h>

using namespace std;

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(S1AuxWavReformatter)


S1AuxWavReformatter::S1AuxWavReformatter() : S1L2AuxReformatter()
{
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, " Id: "<< Id);
}

S1AuxWavReformatter::~S1AuxWavReformatter() throw() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

void S1AuxWavReformatter::setValidity(const string & v)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Validity to match: "<<v);

	RegExp::Matches matches;
	RegExp re;
	re.setReg("([[:digit:]]{8})[tT]([[:digit:]]{2})");
	if (re.match(v, matches)) 
	{
		ostringstream val;
		val << matches.at(1).getMatched() << "T"<< matches.at(2).getMatched() << "0000" ;
		validity(val.str());
	}
	else
		ACS_THROW(exS1AuxWavReformatterException("Unexpected validity time format found."));

	/*/Conversion done in configuration (cf. S1_AUX_RFMT_CONF_01.xml / InputOutputNamingRules)
	ostringstream outFilename;
	outFilename << "SHOM_WW3-GLOB-30MIN_" << _validity << "_uss.nc";	
	_dataFilename.at(0) = outFilename.str();
	//*/
}

_ACS_END_NAMESPACE
