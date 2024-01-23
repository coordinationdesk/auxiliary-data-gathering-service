// PRQA S 1050 EOF
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
	Revision 1.15  2013/12/11 15:47:56  nicvac
	Coding rules implementation (by marpas?)
	
	Revision 1.14  2013/10/30 15:01:54  nicvac
	S1PDGS-1899: implementation.
	
	Revision 1.13  2013/10/29 16:00:23  nicvac
	S1PDGS-1899: implementation.
	
	Revision 1.12  2013/10/24 14:05:27  nicvac
	S1PDGS-1899: file naming conversion.
	
	Revision 1.11  2013/10/17 15:56:12  nicvac
	compilation warnings processed.
	
	Revision 1.10  2012/12/12 13:48:04  damdec
	Compilation warning fixed. Logging improved.
	
	Revision 1.9  2012/10/11 15:10:27  damdec
	Output filename composition updated.
	
	Revision 1.8  2012/06/28 08:45:27  damdec
	Code aligned to S1.RS.MDA.52.7441_02_04 and related schemas [CFI-02.1] GMES-S1GS-EOPG-TN-12-0013-Sentinel-1-IPF-L1-L2-Auxiliary_files_Schemas_v2.4
	
	Revision 1.7  2012/04/23 08:06:36  damdec
	Design reviewed.
	
	Revision 1.6  2012/04/19 14:34:46  damdec
	Handling of multiple input files added for AUX_ICE filetype (Northern and Southern hemisphere). Facility info added to manifest.
	
	Revision 1.5  2012/04/18 08:28:27  damdec
	Regression test performed.
	
	Revision 1.4  2012/04/17 16:03:50  damdec
	Class updated according to libACSSafeFormatter interface changes.
	
	Revision 1.3  2012/04/05 09:00:21  damdec
	Implementation finalized
	
	Revision 1.2  2012/03/29 13:59:59  damdec
	Work in progress
	
	Revision 1.1.1.1  2012/03/27 11:50:11  damdec
	Import libS1L2AuxReformatter
	
        
*/ 

#include <S1L2AuxReformatter.h>

#include "S1L2AuxReformatterV.h"
#include <Filterables.h>
#include <ProductNaming.h>
#include <RegExp.h>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <ConfigurationSingleton.h>
#include <XMLIstream.h>
#include <UtilXml.h>
#include <DateTime.h>
#include <StringUtils.h>

#include <boost/algorithm/string.hpp>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(S1L2AuxReformatter)

static S1L2AuxReformatterV version;

S1L2AuxReformatter::S1L2AuxReformatter() : _infilename(), _dataFilename(), _outProductType(""), _safeVersion(""), _validity(""), _generation(""), _outputPath(""), 
					_startTime(""), _stopTime(""), _rsConf (ConfigurationSingleton::instance() -> get()), 
					_platform(NULL), _processing(NULL), _generalInfo(NULL)
{
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, " Id: "<< Id);

	DateTime start;
 	ostringstream os ;
 	start.asISO8601(os, 6) ;
	
	_startTime = os.str();
	
}

void S1L2AuxReformatter::applyNamingRules( vector< XmlDocSptr > const& actions, string& filename ) {

	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Converting name: \t"<<filename);

	for (vector< XmlDocSptr >::const_iterator ait = actions.begin(); ait!=actions.end(); ++ait ) {

		string actionName = (*ait)->xPath("//@name").at(0)->str();

		if  ( actionName == "tolower" ) {

			boost::to_lower( filename );

		} else if ( actionName == "toupper" ) {

			boost::to_upper( filename );

		} else if ( actionName == "append" ) {

			filename += (*ait)->xPath("//Param/text()").at(0)->str();

		} else if ( actionName == "replace-all" ) {
			vector< XmlDocSptr > params = (*ait)->xPath("//Param/text()");
			string from = params.at(0)->str();
			string to = params.at(1)->str();
			boost::replace_all(filename, from, to);
		}
	}
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Converted name: \t"<<filename);
}

void S1L2AuxReformatter::init(const vector<string> & infilename, const std::string& ioNamingRuleFilename, const string & outProdType, const string & safeVer, const string & outputPath)
{
	
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	_infilename = infilename;
	_dataFilename = infilename;
	_outProductType = outProdType;
	_safeVersion = safeVer;
	_outputPath = outputPath;

	//*/ _dataFilename init as _infilename. Apply, if any, the translation rules found on ioNamingRuleFilename,
	XmlDocSptr xmlDocSptr( new XmlDoc );
	xmlDocSptr->initWithFile(ioNamingRuleFilename);
	vector< XmlDocSptr > rules = xmlDocSptr->xPath("//InputOutputNamingRules/Rule");

	for ( vector<string>::iterator dit=_dataFilename.begin(); dit!=_dataFilename.end(); ++dit) {
		//For each file check if it matches some naming rule

		bool found(false);
		for (vector< XmlDocSptr >::iterator rit = rules.begin(); rit!=rules.end() && !found; ++rit ) {
			RegExp regExp( (*rit)->xPath("//@match").at(0)->str() );
			found = regExp.match( *dit );
			if ( found ) {
				//A rule match the filename
				//Apply the actions
				vector< XmlDocSptr > actions = (*rit)->xPath("//Action");
				this->applyNamingRules( actions, *dit );
			}
		}
	}
	//*/

}


S1L2AuxReformatter::~S1L2AuxReformatter() throw() {
	
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	delete _platform;
	delete _processing;	
	delete _generalInfo;
}

void S1L2AuxReformatter::setPlatform(const string & safeNamespace, const std::string& safePrefix)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "FILLING PLATFORM INFO METADATA");
	
	string nssdcID;
	_rsConf.getValue("Platform.nssdcID", nssdcID);		
	string familyName;
	_rsConf.getValue("Platform.familyName", familyName);		
	string number;
	_rsConf.getValue("Platform.number", number);		
	string instFamName;
	_rsConf.getValue("Platform.instrumentFamName", instFamName);		
	string abbr;
	_rsConf.getValue("Platform.instrumentAbbr", abbr);		

	Instrument instrument(instFamName, abbr);
	
	vector<TimeRef> timeRef;
	_platform = new Platform(safeNamespace, familyName, nssdcID, number, instrument, timeRef, safePrefix);
	
}

void S1L2AuxReformatter::setProcessing(const string & processorName, const string& procVersion, const string & safeNamespace, const string& safePrefix)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "FILLING PROCESSING INFO METADATA");

	_stopTime = _generation;

	ProcessLog procLog(processorName, _startTime, _stopTime);
	// Software
	Software swLog(processorName, procVersion);

	/*/ S1PDGS-30085: Not required for S1L2Aux.
	procLog.addSoftware(swLog);
	//*/
	setSoftware(procLog, swLog);
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
 		swLog.dump();
	ACS_CLASS_END_DEBUG
	
	//Resource
 	string name;
 	_rsConf.getValue("Resource.name", name);		
 	string role;
 	_rsConf.getValue("Resource.role", role);		
 	Resource res(name, role);
 	procLog.addResource(res);
 
 	for (uint i=0; i < _infilename.size(); i++)
 	{
  		Resource input(_infilename.at(i), "Input file", File::getAbsolutePath(_infilename.at(i))); 
 		
		ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
 			input.dump();
		ACS_CLASS_END_DEBUG

 		procLog.addResource(input);
 	}
	
	//Facility
 	string fac_name;
 	_rsConf.getValue("ProcessingFacility.Name", fac_name);
 	if ( fac_name == "${ProcessorName}" ) {
 		fac_name = processorName;
 	}

 	string org;
 	_rsConf.getValue("ProcessingFacility.Organization", org);		
 	string site;
 	_rsConf.getValue("ProcessingFacility.Site", site);		
 	string country;
 	_rsConf.getValue("ProcessingFacility.Country", country);		
 	Facility fac(fac_name, org, site, country);
 	procLog.addFacility(fac);
	
	_processing = new Processing(safeNamespace, procLog, safePrefix);
}

void S1L2AuxReformatter::setGeneralInfo(const string & safeNamespace)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "FILLING GENERAL PRODUCT INFO METADATA");
	
	ProductNaming filename(_infilename.at(0));
	string validity = filename.getField(ProductNaming::StartTimeKey);
	for (uint i=1; i< _infilename.size(); i++)
	{
		ProductNaming filename(_infilename.at(i));
		string validity_next = filename.getField(ProductNaming::StartTimeKey);
		
		ACS_COND_THROW(validity!=validity_next, exS1L2AuxReformatterException("Unexpected validity time found for file "+ _infilename.at(i)));
	}
	
	setValidity(validity);
	
	unsigned int instrConfId;
	_rsConf.getValue("InstrumentConfigId", instrConfId);
	
	string prodType("");
	_rsConf.getValue("GeneralProductInformation.AuxType."+_outProductType, prodType);
	
	DateTime val("UTC="+_validity);
 	ostringstream os ;
 	val.asISO8601(os, 6) ;

	DateTime gen;
 	ostringstream os2 ;
 	gen.asISO8601(os2, 6) ;
	
	_generation = os2.str();
	_generalInfo = new S1AuxGeneralInformation(safeNamespace, prodType, os.str(), _generation, instrConfId);
	
}

string S1L2AuxReformatter::buildOutputFilename()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	ostringstream outFilename;
	rsResourceSet& rs=ConfigurationSingleton::instance()->get();
	string missionId;
	rs.getValue("MissionId", missionId);	
	
	DateTime val("UTC="+_validity);
	DateTime gen("UTC="+_generation);
	
	outFilename << missionId << "_" << _outProductType << "_V" << val.YYYYMMDDThhmmss() << "_G" << gen.YYYYMMDDThhmmss() << ".SAFE";
	
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "output file name <" << outFilename.str() << ">");

	return outFilename.str();
}

string S1L2AuxReformatter::createSafePackage(const std::string & processorName, const std::string& procVersion, const string & confFilename)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	string tmpOutputFile = _outputPath + "/" + "temp";
	string outFilename("");
	try 
	{
		File::mkdir(tmpOutputFile); //create safe package

		string dataDir(tmpOutputFile + "/data/");
		File::mkdirtree(dataDir); //create data sub-folder
		
		string supportDir(tmpOutputFile + "/support/");
		File::mkdirtree(supportDir); //create data sub-folder

		SafeManifest manifest(_outProductType, _safeVersion, tmpOutputFile, confFilename);

		string confSafe(confFilename);
        	XMLIstream xstr(confSafe);
        	rsResourceSet rs;
		xstr >> rs;
		
		string safePrefix("");
		rs.getValue("SafeProduct.safePrefix", safePrefix);

		if (!StringUtils::endsWith(safePrefix, ":"))
		{
			safePrefix+=":";
		}
		string safeNamespace("");
		rs.getValue("SafeProduct.safeNamespace", safeNamespace);
		MetadataWrapDescriptor genInfo = manifest.getMetadataWrap("generalProductInformation");
		
		setGeneralInfo(genInfo.safeNamespace);
		
		handleDataFiles(manifest, dataDir);

		setPlatform(safeNamespace, safePrefix);
		if (_platform!=NULL)
			manifest.setMetadataWrap("platform", *_platform);

 		string prodFilename = _outputPath + "/" + buildOutputFilename();
		File::rename(tmpOutputFile, prodFilename);

		manifest.resetPath(prodFilename);
		if (_generalInfo!=NULL)
			manifest.setMetadataWrap("generalProductInformation", *_generalInfo);

		setProcessing(processorName, procVersion, safeNamespace, safePrefix);
		if (_processing!=NULL)
			manifest.setMetadataWrap("processing", *_processing);

		manifest.writeManifest();
		ACS_LOG_INFO("Generated SAFE package <" << prodFilename<< ">");

		outFilename = prodFilename;

	}
	catch (exception & e)
	{
		ACS_LOG_NOTIFY_EX(e);
		if (File::exists(tmpOutputFile))
			File::remove(tmpOutputFile);
	}

	return outFilename;

}
void S1L2AuxReformatter::handleDataFiles(SafeManifest & manifest, const string & dataDir)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	ostringstream filesize; filesize << _infilename.size();
	ACS_COND_THROW(_infilename.size() > 1, exS1L2AuxReformatterException("Unexpected number of input files: <"+ filesize.str() + ">"));
	File::copy(_infilename.at(0), dataDir); 
	File::rename(dataDir+"/"+File::getFileName(_infilename.at(0)), dataDir+"/"+File::getFileName(_dataFilename.at(0)));
	
	manifest.setDataObject("auxData", "./data/"+File::getFileName(_dataFilename.at(0)), 1);

}

_ACS_END_NAMESPACE
