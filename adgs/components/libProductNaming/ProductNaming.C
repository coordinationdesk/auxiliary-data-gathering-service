// PRQA S 1050 EOF
/*
	Copyright 1995-2013, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libProductNaming$

	$Id$

	$Author$

	$Log$
	Revision 1.18  2014/07/18 08:03:36  damdec
	ValidityKey, ImageTypeKey, ResolutionKey added.
	
	Revision 1.17  2014/05/28 12:25:01  damdec
	TileDescriptorKey added.
	
	Revision 1.16  2013/11/12 09:40:29  marpas
	coding best practices applied
	qa warnings and compilation warnings fixed
	
	Revision 1.15  2013/11/11 10:44:33  ricfer
	conf file handling changed.
	
	Revision 1.14  2013/07/19 15:53:41  enrcar
	EC:: Added tag CreationTimeKey
	
	Revision 1.13  2013/03/26 10:42:52  marfav
	Exception verbosity improved when filenames do not match any regexp
	
	Revision 1.12  2012/12/11 09:32:23  damdec
	Baseline and FrameCoord keys added.
	
	Revision 1.11  2012/10/19 10:22:19  damdec
	Compilation warning fixed.
	
	Revision 1.10  2012/10/19 09:10:11  damdec
	Timeliness field added
	
	Revision 1.9  2012/02/23 16:39:21  marant
	compilation warning solved
	
	Revision 1.8  2010/12/15 11:46:23  damdec
	Version and Instance ID keys added.
	
	Revision 1.7  2010/09/27 10:07:47  marant
	TimeFieldUtility modified to handle two kinds of special dates:
	1) 00000000T000000-like --> the returned jd50 is the min long double;
	2) 99999999T999999-like --> the returned jd50 is the max long double.
	
	Revision 1.6  2010/09/23 14:58:47  damdec
	Debug message modified.
	
	Revision 1.5  2010/09/06 09:01:23  damdec
	Duration, Cycle and RelativeOrbit keys added.
	
	Revision 1.4  2010/06/25 10:34:01  damdec
	Specific exMatchNotFoundException exception introduced.
	
	Revision 1.3  2010/04/16 08:33:18  marant
	TimeFieldUtility::getJd50() modified: jd50 returned as long double. Moreover,
	in case of special input values like 99999999T99999999 the max long double value
	is returned.
	
	Revision 1.2  2010/04/12 15:42:19  marant
	TimeFieldUtility class added
	
	Revision 1.1.1.1  2010/04/12 10:02:06  marant
	Import libProductNaming
	
        
*/ 

#include <ProductNaming.h>
#include <ProductNamingV.h>
#include <NamingRules.h>
#include <Filterables.h>
#include <DateTime.h>
#include <limits>
#include <XMLIstream.h>
#include <ConfigurationSingleton.h>
#include <rsPushNSpace.h>


_ACS_BEGIN_NAMESPACE(acs)

using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(ProductNaming)

namespace {
 ProductNamingV version;
}

const ProductNaming::FieldKey ProductNaming::PlatformNameKey("PlatformName");
const ProductNaming::FieldKey ProductNaming::MissionNumberKey("MissionNumber");
const ProductNaming::FieldKey ProductNaming::FileTypeKey("FileType");
const ProductNaming::FieldKey ProductNaming::FileClassKey("FileClass");
const ProductNaming::FieldKey ProductNaming::StartTimeKey("StartTime");
const ProductNaming::FieldKey ProductNaming::StopTimeKey("StopTime");
const ProductNaming::FieldKey ProductNaming::CreationTimeKey("CreationTime");
const ProductNaming::FieldKey ProductNaming::OriginFacilityKey("OriginFacility");
const ProductNaming::FieldKey ProductNaming::AbsoluteOrbitKey("AbsoluteOrbit");
const ProductNaming::FieldKey ProductNaming::CrcKey("Crc");
const ProductNaming::FieldKey ProductNaming::ExtensionKey("Extension");
const ProductNaming::FieldKey ProductNaming::DurationKey("Duration");
const ProductNaming::FieldKey ProductNaming::CycleKey("Cycle");
const ProductNaming::FieldKey ProductNaming::RelativeOrbitKey("RelativeOrbit");
const ProductNaming::FieldKey ProductNaming::InstanceIdKey("InstanceId");
const ProductNaming::FieldKey ProductNaming::VersionKey("Version");
const ProductNaming::FieldKey ProductNaming::TimelinessKey("Timeliness");
const ProductNaming::FieldKey ProductNaming::BaselineKey("Baseline");
const ProductNaming::FieldKey ProductNaming::FrameCoordKey("FrameCoord");
const ProductNaming::FieldKey ProductNaming::TileDescriptorKey("TileDescriptor");
const ProductNaming::FieldKey ProductNaming::ValidityKey("Validity");
const ProductNaming::FieldKey ProductNaming::ImageTypeKey("ImageType");
const ProductNaming::FieldKey ProductNaming::ResolutionKey("Resolution");


ProductNaming::ProductNaming(const string& filename, const string& confFileName):
	_namingRules(0),
	_fieldsMap()
{
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Id: "<< Id) ;
	
	if(!confFileName.empty()) {
		rsResourceSet rset;
		XMLIstream xstr(confFileName);
		xstr >> rset;
		
		init(rset, "");
    }
	else {
		ConfigurationSingleton* config = ConfigurationSingleton::instance();
		// Load a non-mandatory configuration space
		config->addSpace(ConfigurationSingleton::ConfSpace("ProductNaming", "", false), ConfigurationSingleton::LoadImmediate);
		init(config->get(), "");
    }
	 
    parse(basename(filename.c_str()));
}

ProductNaming::ProductNaming(const std::string& filename, rsResourceSet& rset, const std::string& rootTag) : 
	_namingRules(0),
	_fieldsMap()
{
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Id: "<< Id) ;
	
	init(rset, rootTag);
    parse(basename(filename.c_str()));
}

ProductNaming::~ProductNaming() throw()  
{
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "called.") ; // PRQA S 4631

    delete _namingRules;
}

void ProductNaming::init(rsResourceSet& rset, const std::string& rootTag){

    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "called.") ;
    
    rsPushNSpace pushName(rset, rootTag);
        
    _namingRules = new NamingRules;
    _namingRules->load(rset);
}

void ProductNaming::parse(const string& filename)  {

    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "called on file " << filename) ;

    RegExp re;
    bool matchFound(false);

    size_t nrsize = _namingRules->size() ;
    for (size_t i=0; !matchFound && (i< nrsize); ++i) {
        string regexp = _namingRules->getFilenameRegExp(i);
        vector<FilenameField> fields = _namingRules->getFilenameFields(i);
        re.setReg( regexp );
        RegExp::Matches matches;
        
        ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "trying matching with regexp " << regexp) ;
        if (re.match(filename, matches)) {
            matchFound=true;
            retrieveFields(matches, fields);
            
        }
    }

    ACS_COND_THROW(!matchFound, exMatchNotFoundException("No match found with configured regular expression for filename: " + filename)); // PRQA S 3081
    

}

void ProductNaming::retrieveFields(const RegExp::Matches& matches, const vector<FilenameField>& fields){

    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "called.") ;

    for (unsigned int i=0; i< fields.size(); i++) {
        string name = fields.at(i).name;
        unsigned short index = fields.at(i).index ;

        ACS_COND_THROW(index>=matches.size(), exProductNamingException("Field index exceeds the number of regexp matches")); // PRQA S 3081
        _fieldsMap.insert(make_pair(name, matches.at(index).getMatched()));
    }

}

std::string ProductNaming::getField(const FieldKey& fieldKey)
{

    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "called.") ;

    string key = fieldKey ;

    map<string, string>::iterator it;

    it = _fieldsMap.find(key);

    return it!=_fieldsMap.end()? it->second: ""; // PRQA S 3382, 3385
}


namespace {

    vector<string> fillMaxSpecialDates()
    {
        vector<string> tmp;
        tmp.push_back("99999999T999999");
        return tmp;
    }

    vector<string> fillMinSpecialDates()
    {
        vector<string> tmp;
        tmp.push_back("00000000T000000");
        return tmp;
    }
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(TimeFieldUtility)

vector<string> TimeFieldUtility::_minSpecialDates = fillMinSpecialDates();
vector<string> TimeFieldUtility::_maxSpecialDates = fillMaxSpecialDates();


TimeFieldUtility::TimeFieldUtility(){
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "Id: "<< Id) ;
}

bool TimeFieldUtility::getJd50(const string& time, long double& jd50)
{
   string timeStr = time; 
   bool valid = true ;

   if (find(_maxSpecialDates.begin(), _maxSpecialDates.end(), timeStr)!= _maxSpecialDates.end()) {
       jd50 = numeric_limits<long double>::max();
       valid = false;
   }

   if(find(_minSpecialDates.begin(), _minSpecialDates.end(), timeStr)!= _minSpecialDates.end()) {
       jd50 = numeric_limits<long double>::min();
       valid = false;
   }

   if(valid)
   {

       //patch format YYYYMMDDTHHMMSS since DateTime recognizes the string "UTC=YYYYMMDDTHHMMSS"
       RegExp re;
       re.setReg("[[:digit:]]{8}T[[:digit:]]{6}");
       if (re.match(timeStr)) {
           timeStr="UTC=" + timeStr;
       }
       
       jd50 = DateTime::JD50(DateTime(timeStr)).jd(); // PRQA S 3050, 3081
   }

   ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "called with time string <" << time << ">. Computed jd50: " << jd50) ;
   return valid;

}



_ACS_END_NAMESPACE
