/*
	Copyright 1995-2012, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libS1L2AuxReformatter$

	$Id$

	$Author$

	$Log$
	Revision 1.6  2013/12/11 15:47:56  nicvac
	Coding rules implementation (by marpas?)
	
	Revision 1.5  2013/10/29 16:00:23  nicvac
	S1PDGS-1899: implementation.
	
	Revision 1.4  2013/10/17 15:56:11  nicvac
	compilation warnings processed.
	
	Revision 1.3  2012/12/12 13:47:14  damdec
	Compilation warning fixed.
	
	Revision 1.2  2012/06/28 08:45:27  damdec
	Code aligned to S1.RS.MDA.52.7441_02_04 and related schemas [CFI-02.1] GMES-S1GS-EOPG-TN-12-0013-Sentinel-1-IPF-L1-L2-Auxiliary_files_Schemas_v2.4
	
	Revision 1.1  2012/04/23 08:02:38  damdec
	Class added.
	
        
*/ 

#include <S1AuxIceReformatter.h>

#include <Filterables.h>
#include <RegExp.h>
#include <StringUtils.h>
#include <File.h>

using namespace std;

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(S1AuxIceReformatter)


S1AuxIceReformatter::S1AuxIceReformatter() : S1L2AuxReformatter() 
{
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, " Id: "<< Id);
}

S1AuxIceReformatter::~S1AuxIceReformatter() throw() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

void S1AuxIceReformatter::handleDataFiles(SafeManifest & manifest, const string & dataDir)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	ostringstream filesize; filesize << getInFilename().size();
	ACS_COND_THROW(getInFilename().size() > 2, exS1AuxIceReformatterException("Unexpected number of input files: <"+ filesize.str() + ">"));
	
	for (uint i = 0; i < getInFilename().size(); ++i)
	{
		File::copy(getInFilename()[i], dataDir); 
		File::rename(dataDir+"/"+getInFilename()[i], dataDir+"/"+getDataFilename()[i]);

		string dataObjectId = ( getDataFilename()[i].find("nh") != string::npos )? "auxData_NH" : "auxData_SH" ;
		manifest.setDataObject(dataObjectId, "./data/"+getDataFilename()[i], 1);

	}
}


void S1AuxIceReformatter::setValidity(const string & v)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	RegExp::Matches matches;
	RegExp re;
	re.setReg("([[:digit:]]{8})([[:digit:]]{4})");
	if (re.match(v, matches)) 
	{
		ostringstream val;
		val << matches[1].getMatched() << "T" << matches[2].getMatched() << "00";
		validity(val.str());
	}
	else
		ACS_THROW(exS1L2AuxReformatterException("Unexpected validity time format found."));
}

_ACS_END_NAMESPACE
