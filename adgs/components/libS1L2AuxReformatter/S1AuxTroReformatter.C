/*
	Copyright 1995-2023, Exprivia SpA - DADF
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: libS1L2AuxReformatter$

*/ 

#include <S1AuxTroReformatter.h>

#include <Filterables.h>
#include <RegExp.h>
#include <StringUtils.h>
#include <File.h>
#include <DateTime.h>
#include <ConfigurationSingleton.h>

using namespace std;

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(S1AuxTroReformatter)


S1AuxTroReformatter::S1AuxTroReformatter() : S1L2AuxReformatter() 
{
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, " Id: "<< Id);
}

S1AuxTroReformatter::~S1AuxTroReformatter() throw() 
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

void S1AuxTroReformatter::handleDataFiles(SafeManifest & manifest, const string & dataDir)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	ostringstream filesize; filesize << getInFilename().size();
	ACS_COND_THROW(getInFilename().size() > 5, exS1AuxTroReformatterException("Unexpected number of input files: <"+ filesize.str() + ">"));
	
	for (uint i = 0; i < getInFilename().size(); ++i)
	{
		File::copy(getInFilename()[i], dataDir); 
		File::rename(dataDir+"/"+File::getFileName(getInFilename()[i]), dataDir+"/"+File::getFileName(getDataFilename()[i]));
		RegExp::Matches matches;
		// set filename reg exp
		RegExp re;
		re.setReg("[A-Z0-9_]{5}_[A-Z0-9_]{4}_ML00_[0-9]{2}H_([0-9]{3})_GP_[A-Z0-9_]{4}_[[:digit:]]{8}T[[:digit:]]{6}");
		if (re.match(getInFilename()[i], matches)) 
		{
			// set the key of the map describing the content of the DataObjectId
			ostringstream os;
			os << "auxData"<< matches[1].getMatched();
			string dOId = os.str();
			manifest.setDataObject(dOId, "./data/"+File::getFileName(getDataFilename()[i]), 1);
			// for the filetype AUX_TRO dataObjectID has to be set with the filename: configured value has to be overwritten
			// manifest.resetDataObject(dOId, getDataFilename()[i]);
		}
	}
}
void S1AuxTroReformatter::setValidity(const string & v)
{
	validity(v);
}
void S1AuxTroReformatter::setSoftware(ProcessLog & pLog, Software & swLog)
{
	pLog.addSoftware(swLog);
}
string S1AuxTroReformatter::buildOutputFilename()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	ostringstream outFilename;
	rsResourceSet& rs=ConfigurationSingleton::instance()->get();
	string missionId;
	rs.getValue("MissionId", missionId);	
	
	DateTime val("UTC="+getValidity());
	DateTime gen("UTC="+getGeneration());
	
	//outFilename << missionId << "_" << getOutProductType() << "_V" << val.YYYYMMDDThhmmss() << "_" << val.YYYYMMDD() <<"T235959" << "_G" << gen.YYYYMMDDThhmmss() << ".SAFE";
	outFilename << missionId << "_" << getOutProductType() << "_V" << val.YYYYMMDDThhmmss() << "_G" << gen.YYYYMMDDThhmmss() << ".SAFE";
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "output file name <" << outFilename.str() << ">");

	return outFilename.str();
}


_ACS_END_NAMESPACE
