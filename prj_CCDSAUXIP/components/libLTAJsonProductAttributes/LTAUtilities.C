/*
	Copyright 1995-2021, Exprivia SpA - DADF-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. libLTAUtilities$

	$Id$

	$Author$
*/
#include <ConfigurationSingleton.h>
#include <LTAUtilities.h>
#include <rsResourceSet.h>
#include <RegExp.h>
#include <Filterables.h>
#include <vector>
#include <File.h>
#include <Dir.h>
#include <iostream>
#include <ostream>
#include <StringUtils.h>
#include <SpawnProc.h>

_ACS_BEGIN_NAMESPACE(acs)
//using namespace std;
using std::string;
using std::ostringstream;
using std::vector;


bool str_to_bool(std::string str) 
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	std::istringstream is(str);
	bool b;
	is >> std::boolalpha >> b;
	return b;
}
std::vector<bool> to_bool(const std::vector<std::string> & str) 
{
	std::vector<bool> bv(str.size(), 0);
	std::transform(str.begin(), str.end(), bv.begin(), str_to_bool);
	return bv;
}

ACS_CLASS_DEFINE_DEBUG_LEVEL(LTAUtilities)
Attribute::Attribute(Attribute const & a) :
	_name(a._name),
	_metadataFile(a._metadataFile),
	_metadataFileRegExp(a._metadataFileRegExp),
	_fixedValue(a._fixedValue),
	_xpath(a._xpath),
	_condition(a._condition),
	_algo(a._algo),
	_algoParamFromName(a._algoParamFromName),
	_odatatype(a._odatatype),
	_minNum(a._minNum),
	_maxNum(a._maxNum)
{
}
Attribute &Attribute::operator=(Attribute const & a)
{
	if( this != &a ) {
		_name = a._name ;
		_metadataFile = a._metadataFile;
		_metadataFileRegExp = a._metadataFileRegExp;
		_fixedValue = a._fixedValue;
		_xpath = a._xpath;
		_condition = a._condition;
		_algo = a._algo;
		_algoParamFromName = a._algoParamFromName;
		_odatatype = a._odatatype;
		_minNum = a._minNum;
		_maxNum = a._maxNum;
	}
	return *this ;
}
AttributeFromFilename::AttributeFromFilename(AttributeFromFilename const & a) :
	_name(a._name),
	_regExp(a._regExp),
	_odatatype(a._odatatype),
	_condition(a._condition)
{
}
AttributeFromFilename &AttributeFromFilename::operator=(AttributeFromFilename const & a)
{
	if( this != &a ) {
		_name = a._name ;
		_regExp = a._regExp;
		_odatatype = a._odatatype;
		_condition = a._condition;
	}
	return *this ;
}

ExtractionStrategy string2ExtractionStrategy(const std::string & value)
{
	ExtractionStrategy strategy(STANDARD);
	if (StringUtils::equalsNoCase(value, "None"))
	{
		strategy=NONE;
	}
	else if (StringUtils::equalsNoCase(value, "Uncompress"))
	{
		strategy=UNCOMPRESS;
	}
	else if (StringUtils::equalsNoCase(value, "Jp2"))
	{
		strategy=JP2;
	}
	return strategy;
};

LTAExtractedConf::LTAExtractedConf() :_satellite() , _extension(), _sectionName(), _extractionRule(),_extractionStrategy() 
{}
void LTAUtilities::loadConf(const std::string & productName, LTAExtractedConf & conf)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	rsResourceSet rset = ConfigurationSingleton::instance()->get();

	vector<string> fileRegExp;
	rset.getArray("SRVJsonProductAttributes.List_of_FilenameRegExp.FilenameRegExp", fileRegExp);

	size_t i = 0 ;
	bool notFound = true;
	string filetype;
	// get the filetype and satellite from filename
	while (i < fileRegExp.size() && notFound)
	{
		RegExp re;
		re.setReg(fileRegExp[i]);

		//ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "RegExp: "<< fileRegExp[i]);

		if(re.match(File::getFileName(productName)))
		{
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "RegExp match : "<< fileRegExp[i]);
			notFound = false;
			RegExp::Matches matches;
   			if (re.match(File::getFileName(productName), matches)) {
				conf.setSatellite(matches.at(1).getMatched());
				filetype = matches.at(2).getMatched();
				if (matches.size()>3)
				{
					conf.setExtension(matches.at(3).getMatched());
				}
			}
		}
		++i;
	}
	if (notFound)
	{
		ostringstream os;
		os << "No RegEpx configured found matching the product name " << File::getFileName(productName);
		ACS_THROW(LTAUtilitiesException(os.str()));
	}
	// get the name of the section containing the proper configuration
	vector<FileTypeConf> fileTypes;
	rset.getArray(conf.satellite()+".List_of_Filetypes.Filetype", fileTypes);
	size_t k = 0;

	while ( k < fileTypes.size() && conf.sectionName().empty())
	{
		RegExp re;
		re.setReg(fileTypes[k].regExp);
		if(re.match(filetype))
		{
			conf.setSectionName(fileTypes[k].section);
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Matching filetype: "<< filetype<< " corresponding to section: "<< conf.sectionName() << " RegExp:" << fileTypes[k].regExp);
			if (!fileTypes[k].rule.empty())
			{
				conf.setExtractionRule(fileTypes[k].rule);
			}
			conf.setExtractionStrategy(fileTypes[k].strategy);
		}
		++k;
	}
	if (conf.sectionName().empty())
	{
		ostringstream os;
		os << "No configuration section associated to satellite " << conf.satellite() << " and filetype " << filetype << " found. ";
		ACS_THROW(LTAUtilitiesException(os.str()));
	}
}
void LTAUtilities::getConfAttributes(const string & satellite, const string & sectionName, vector <Attribute> & attributes)
{
	rsResourceSet rset = ConfigurationSingleton::instance()->get();
	rset.getArray(satellite+"."+sectionName+".List_of_Attributes.Attribute", attributes);

	ACS_COND_THROW(attributes.empty(), LTAUtilitiesException("No attribute loaded from configuration file associated to section "+ satellite+"."+sectionName));

	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Loaded "<< attributes.size()<< " attributes ");
}
void LTAUtilities::getConfAttributes(const string & satellite, const string & sectionName, vector <AttributeFromFilename> & attrFromFilename)
{
	rsResourceSet rset = ConfigurationSingleton::instance()->get();

	rset.getArray(satellite+"."+sectionName+".List_of_Attribute_From_Filename.Attribute_From_Filename", attrFromFilename);
	ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Loaded "<< attrFromFilename.size()<< " attributes to be extracted from filename");
}
_ACS_END_NAMESPACE
