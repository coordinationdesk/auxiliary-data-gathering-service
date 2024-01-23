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

#include <S1AuxTecReformatter.h>

#include <Filterables.h>
#include <RegExp.h>
#include <StringUtils.h>
#include <File.h>
#include <DateTime.h>
#include <ConfigurationSingleton.h>

using namespace std;

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(S1AuxTecReformatter)

void S1AuxTecReformatter::handleDataFiles(SafeManifest & manifest, const string & dataDir)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	ostringstream filesize; filesize << getInFilename().size();
	ACS_COND_THROW(getInFilename().size() > 1, exS1AuxTecReformatterException("Unexpected number of input files: <"+ filesize.str() + ">"));
	
	for (uint i = 0; i < getInFilename().size(); ++i)
	{
		File::copy(getInFilename()[i], dataDir); 
		File::rename(dataDir+"/"+File::getFileName(getInFilename()[i]), dataDir+"/"+File::getFileName(getDataFilename()[i]));

		manifest.setDataObject("auxData", "./data/"+File::getFileName(getDataFilename()[i]), 1);

	}
}
void S1AuxTecReformatter::setValidity(const string & v)
{
	RegExp::Matches matches;
	RegExp re;
	re.setReg("([[:digit:]]{3})[0-9]\\.([[:digit:]]{2})"); // codg2220.19i.Z 

	if (re.match(v, matches)) 
	{
		unsigned short day = 0;
		unsigned short month = 0;
		ostringstream os;
		os << "20" << matches[2].getMatched().c_str();
		string year_s = os.str();
		unsigned short year = atoi(year_s.c_str());
		unsigned short doy = atoi(matches[1].getMatched().c_str());
		DateTime::UTCD::dayAndMonthFromDOY( doy, year, day, month);
		DateTime val(DateTime::UTCD(year, month, day));
		validity(val.YYYYMMDDThhmmss());
	}
	else
	{
		re.setReg("([[:digit:]]{4})([[:digit:]]{3})"); //COD0OPSFIN_20223310000_01D_01H_GIM.INX.gz
		if (re.match(v, matches))
		{
			unsigned short day = 0;
			unsigned short month = 0;
			string year_s = matches[1].getMatched().c_str();
			unsigned short year = atoi(year_s.c_str());
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, " Year: "<< year);
			unsigned short doy = atoi(matches[2].getMatched().c_str());
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, " DOY: "<< doy);
			DateTime::UTCD::dayAndMonthFromDOY( doy, year, day, month);
			ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, " Year: "<< year << " Day: " << day << " month: " << month);
			DateTime val(DateTime::UTCD(year, month, day));
			validity(val.YYYYMMDDThhmmss());
			}
		else
		{
			ACS_LOG_WARNING("Validity " << v << " doesn't match any expected reg exp");
		}
}
	
}
void S1AuxTecReformatter::setSoftware(ProcessLog & pLog, Software & swLog)
{
	pLog.addSoftware(swLog);
}
string S1AuxTecReformatter::buildOutputFilename()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	ostringstream outFilename;
	rsResourceSet& rs=ConfigurationSingleton::instance()->get();
	string missionId;
	rs.getValue("MissionId", missionId);	
	
	DateTime val("UTC="+getValidity());
	DateTime gen("UTC="+getGeneration());
	
	// outFilename << missionId << "_" << getOutProductType() << "_V" << val.YYYYMMDDThhmmss() << "_" << val.YYYYMMDD() <<"T235959" << "_G" << gen.YYYYMMDDThhmmss() << ".SAFE";
	outFilename << missionId << "_" << getOutProductType() << "_V" << val.YYYYMMDDThhmmss() << "_G" << gen.YYYYMMDDThhmmss() << ".SAFE";
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "output file name <" << outFilename.str() << ">");

	return outFilename.str();
	
}
_ACS_END_NAMESPACE
