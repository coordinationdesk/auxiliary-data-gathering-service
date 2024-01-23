/*
	Copyright 1995-2021, Exprivia SpA - DADF- AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: A.C.S. libLTAJsonProductAttributes$

	$Id$

	$Author$
*/
#include <ConfigurationSingleton.h>
#include <LTAJsonProductAttributesV.h>
#include <LTAJsonProductAttributes.h>
#include <rsResourceSet.h>
#include <RegExp.h>
#include <Filterables.h>
#include <vector>
#include <XmlDoc.h>
#include <File.h>
#include <Dir.h>
#include <iostream>
#include <ostream>

#include <netcdf>
#include <ncFile.h>

#include <StringUtils.h>
#include <DateTime.h>

_ACS_BEGIN_NAMESPACE(acs)
//using namespace std;
using std::string;
using std::ostringstream;
using std::vector;

JSONAttribute::JSONAttribute() : 
	_name(),
	_valueType()
{
}
JSONAttribute::JSONAttribute(const std::string & name, const std::string & type) :
	_name(name),
	_valueType(type)
{
}
JSONAttribute::JSONAttribute (JSONAttribute const & a):
	_name(a._name),
	_valueType(a._valueType)
{
}
JSONAttribute &JSONAttribute::operator=(JSONAttribute const & a)
{
	if( this != &a ) {
		_name = a._name;
		_valueType = a._valueType;
	}
	return *this ;
}

JSONStringAttribute::JSONStringAttribute() : JSONAttribute(), _value()
{
}

JSONStringAttribute::JSONStringAttribute(const std::string & name, const std::string & type, const std::vector<std::string> & value):
					JSONAttribute(name, type),
					_value(value)
{
}
JSONDateTimeOffsetAttribute::JSONDateTimeOffsetAttribute() : JSONAttribute(), _value()
{
}

JSONDateTimeOffsetAttribute::JSONDateTimeOffsetAttribute(const std::string & name, const std::string & type, const std::vector<std::string> & value):
					JSONAttribute(name, type),
					_value(value)
{
}
JSONIntegerAttribute::JSONIntegerAttribute() : JSONAttribute(),
						_value()
{
}

JSONIntegerAttribute::JSONIntegerAttribute(const std::string & name, const std::string & type, const std::vector<int64_t> & value):
					JSONAttribute(name, type),
					_value(value)
{
}
JSONDoubleAttribute::JSONDoubleAttribute() : JSONAttribute(),
						_value()
{
}

JSONDoubleAttribute::JSONDoubleAttribute(const std::string & name, const std::string & type, const std::vector<double> & value):
					JSONAttribute(name, type),
					_value(value)
{
}
JSONBooleanAttribute::JSONBooleanAttribute() : JSONAttribute(),
						_value()
{
}

JSONBooleanAttribute::JSONBooleanAttribute(const std::string & name, const std::string & type, const std::vector<bool> & value):
					JSONAttribute(name, type),
					_value(value)
{
}


ACS_CLASS_DEFINE_DEBUG_LEVEL(LTAJsonProductAttributes)

LTAJsonProductAttributes::LTAJsonProductAttributes(string const & workDir, string const & filename) :
			_isNetcdf(false),
			_tmpWorkDir(workDir),
			_productName(filename),
			_jsonAttributes(),
			_attributes(),
			_attrFromFileSet(false),
			_attrFromFilename(),
			_allowOptional(false)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);	//PRQA S 3803
	init();
}

LTAJsonProductAttributes::LTAJsonProductAttributes(LTAJsonProductAttributes const& e) :
			_isNetcdf(e._isNetcdf),
			_tmpWorkDir(e._tmpWorkDir),
			_productName(e._productName),
			_jsonAttributes(e._jsonAttributes),
			_attributes(e._attributes),
			_attrFromFileSet(e._attrFromFileSet),
			_attrFromFilename(e._attrFromFilename),
			_allowOptional(e._allowOptional)
{
	//ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);	//PRQA S 3803
}

LTAJsonProductAttributes& LTAJsonProductAttributes::operator=(LTAJsonProductAttributes const& e)
{
	if( this != &e ) {
		_isNetcdf = e._isNetcdf;
		_productName = e._productName;
		_tmpWorkDir = e._tmpWorkDir;
		_jsonAttributes = e._jsonAttributes;
		_attributes = e._attributes;
		_attrFromFileSet = e._attrFromFileSet;
		_attrFromFilename = e._attrFromFilename;
		_allowOptional = e._allowOptional;
	}
	return *this ;
}
void LTAJsonProductAttributes::init()
{
// 	string extension;
// 	string rule;
// 
// 	if (!extension.compare("nc"))
// 	{
// 		_isNetcdf = true;
// 	}
	
}
void LTAJsonProductAttributes::setAllowOptional(bool optional)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	_allowOptional = optional;
}
void LTAJsonProductAttributes::setIsNetcdf(bool isNc)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	_isNetcdf = isNc;
}
void LTAJsonProductAttributes::setConfAttributes(const std::vector <Attribute> & attr)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	_attributes = attr;
}
void LTAJsonProductAttributes::setConfAttributesFromFilename(const std::vector <AttributeFromFilename> & attr)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	_attrFromFilename = attr;
	_attrFromFileSet = true;
}
std::vector<json> & LTAJsonProductAttributes::readAttributes()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);

	string metadataFile, metadataFileRegExp;

	if (_isNetcdf)
	{
		ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Product filename "<< StringUtils::pathJoin(_tmpWorkDir, _productName));

		if (File::isRegularFile(StringUtils::pathJoin(_tmpWorkDir, _productName)))
		// S5P case...product is a single netcdf file
		{
			metadataFile = StringUtils::pathJoin(_tmpWorkDir, _productName);
		}
		else
		{
			// it should not occur
			ACS_THROW(LTAJsonProductAttributesException("Input product is expected as regular file!"));
		}

		try
		{
			netCDF::NcFile dataFile(metadataFile, netCDF::NcFile::read);
			for (size_t j = 0; j < _attributes.size(); ++j)
			{
				if (!_attributes[j].fixedValue().empty())
				{
					addAttribute(_attributes[j]);
				}
				else
				{
					addAttributeFromNetCdf(dataFile, _attributes[j]);
				}
			}
		}
		catch (const netCDF::exceptions::NcException & e)
		{
			ACS_THROW(LTAJsonProductAttributesException(e, "NetCdf exception"));
		}
		catch (const exException & e)
		{
			ACS_THROW(LTAJsonProductAttributesException(e, "Error processing NetCdf file"));
		}
	}
	else
	{
		for (size_t j = 0; j < _attributes.size(); ++j)
		{
			// initialize metadataFile with the regular expression set in the configuration file
			metadataFile = _attributes[j].metadataFile();
			metadataFileRegExp = _attributes[j].metadataFileRegExp();
			//std::cout << "metadataFile " << metadataFile << " - metadataFileRegExp - " << metadataFileRegExp<< "-name: " << _attributes[j].name()<< std::endl;

			try
			{
				// scan the product directory searcing for the exact name of the metadata file matching the configured regular expression
				if (!metadataFileRegExp.empty())
				{
					getMetadataFile(metadataFileRegExp, metadataFile);
					//std::cout << "metadataFile build by regExp:" << metadataFileRegExp << std::endl;
				}
				try
				{
					if (!_attributes[j].fixedValue().empty())
					{
						addAttribute(_attributes[j]);
					}
					else
					{
						//std::cout << "metadataFile passed:" << metadataFile << std::endl;
						if (!File::exists(StringUtils::pathJoin(_tmpWorkDir, metadataFile)) && _allowOptional)
						{
							// configuration flag indicates that attributes are optional
							ACS_LOG_WARNING("No mandatory metadata file " << metadataFile << " for retrieving attribute " << _attributes[j].name() << " not available.");
						}
						else
						{
							addAttributeFromManifest(metadataFile, _attributes[j]);
						}
					}
				}
				catch (const exException & e)
				{
					if (!_allowOptional) // configuration flag indicates that the attributes are not optional
					{
						ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Error processing " << metadataFile << " for retrieving attribute " << _attributes[j].name());
						ACS_THROW(LTAJsonProductAttributesException(e.what()));
					}
				}
			}
			catch (const exException & e)
			{
				if (!_allowOptional) // configuration flag indicates that the attributes are not optional
				{
					ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Error in retrieving attribute " << _attributes[j].name());
					ACS_THROW(LTAJsonProductAttributesException(e.what()));
				}
			}
		}
	}

	ACS_COND_THROW(false == _attrFromFileSet, LTAJsonProductAttributesException("Retrieval of attributes from filename not performed!")); // it should not occur

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Attribute from filename configured " << _attrFromFilename.size());
	for (size_t j = 0; j < _attrFromFilename.size(); ++j)
	{
		vector<string> value;
		RegExp re;
		re.setReg(_attrFromFilename[j].regExp());
		if(re.match(File::getFileName(_productName)))
		{
			RegExp::Matches matches;
   			if (re.match(File::getFileName(_productName), matches)) 
   			{
				string val = matches.at(1).getMatched();
				if (!_attrFromFilename[j].condition().empty())
				{
					string newValue;
					getFromCondition<string>(_attrFromFilename[j].condition(), val, newValue);
					if (!newValue.empty())
					{
						// overwrite the value
						val = newValue;
					}
				}
				value.push_back(val);
			}
		}
		if (value.size()>0)
		{
			// set attribute retrieved by filename
			JSONStringAttribute attr(_attrFromFilename[j].name(), _attrFromFilename[j].odatatype(), value);

			json j_att = attr;
			_jsonAttributes.push_back(j_att);
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Attribute from filename " << _attrFromFilename[j].name() << " not set!" );
		}
	}

	return _jsonAttributes;
}
void LTAJsonProductAttributes::getMetadataFile(const string & metadataFileRExp, string & metadataFile )
{
//	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	// scan tmp working folder
	Dir prod(_tmpWorkDir);
	size_t t = 0;
	bool found(false);
	while( t < prod.size() && !found)
	{
		RegExp re;
		re.setReg(metadataFileRExp);

		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Dir content " << prod[t] << " metadata " << metadataFile );
		if(re.match(prod[t]))
		{
			found = true;
			metadataFile = File::getFileName(prod[t]);
		}
		++t;
	}
	if (!found)
	{
		ACS_THROW(LTAJsonProductAttributesException("No metadata file found matching the regular expression."));
	}
}
void LTAJsonProductAttributes::addAttribute(Attribute const &  attribute)
{
	//ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	
	if (StringUtils::equalsNoCase(attribute.odatatype(), "Int64") )
	{
		vector<int64_t> value;
		value.push_back(std::stoll(attribute.fixedValue()));
		JSONIntegerAttribute attr(attribute.name(), attribute.odatatype(), value);
		json j_att = attr;
		_jsonAttributes.push_back(j_att);
	}
	else if (StringUtils::equalsNoCase(attribute.odatatype(), "Double"))
	{
		vector<double> value;
		value.push_back(std::stod(attribute.fixedValue()));
		JSONDoubleAttribute attr(attribute.name(), attribute.odatatype(), value);
		json j_att = attr;
		_jsonAttributes.push_back(j_att);
	}
	else if (StringUtils::equalsNoCase(attribute.odatatype(), "DateTimeOffset"))
	{
		vector<string> value;
		value.push_back(attribute.fixedValue());
		JSONDateTimeOffsetAttribute attr(attribute.name(), attribute.odatatype(), value);
		json j_att = attr;
		_jsonAttributes.push_back(j_att);
	}
	else if (StringUtils::equalsNoCase(attribute.odatatype(), "Boolean"))
	{
		vector<bool> value;
		value.push_back(std::stod(attribute.fixedValue()));
		JSONBooleanAttribute attr(attribute.name(), attribute.odatatype(), value);
		json j_att = attr;
		_jsonAttributes.push_back(j_att);
	}
	else
	// string case or unknown
	{
		vector <string> value;
		value.push_back(attribute.fixedValue());
		JSONStringAttribute attr(attribute.name(), attribute.odatatype(), value);
		json j_att = attr;
		_jsonAttributes.push_back(j_att);
	}
}
void LTAJsonProductAttributes::addAttributeFromManifest(string const & metadataFile, Attribute const &  attribute)
{
	//ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	XmlDocSptr xmlDocSptr( new XmlDoc );
	xmlDocSptr->setParameters(XmlDoc::parDefault).initWithFile(StringUtils::pathJoin(_tmpWorkDir, metadataFile));
	
	if (StringUtils::equalsNoCase(attribute.odatatype(), "Int64") )
	{
		vector<int64_t> value;
		vector < vector<int64_t> > tmpValue;
		bool found(false);
		bool isset(0);
		for (size_t t = 0; t < attribute.xpath().size(); ++t)
		{
			value.clear();
			found = xmlDocSptr->getValue(attribute.xpath(t), value, attribute.minNum());
			string count =  "count(" + StringUtils::substringBeforeLast(attribute.xpath(t), "/") + " ) > 0";
			xmlDocSptr->getValue( count , isset );
			if (found || isset)
			// condition handling
			{
				if (!attribute.condition().empty())
				{
					for (size_t i = 0; i < value.size(); ++i)
					{
						string newValue;
						getFromCondition<int64_t>(attribute.condition(), value[i], newValue);
						if (!newValue.empty())
						{
							// overwrite the value
							value[i] = std::stoll(newValue);
						}
						else
						{
							// no attribute will be added 
							found = false;
							isset = false;
						}
					}
				}
				tmpValue.push_back(value);
			}
		}
		if (found || isset)
		{
			try{
				if (!attribute.algo().empty())
				{
					value.clear();
					executeAlgorithm(attribute.algo(), tmpValue, value);
				}
				if (!value.empty())
				{
					JSONIntegerAttribute attr(attribute.name(), attribute.odatatype(), value);
					json j_att = attr;
					_jsonAttributes.push_back(j_att);
				}
			}
			catch (exException & e)
			{
				// it should not occur
				ACS_LOG_WARNING("Attribute " << attribute.name() << " not set due to exception catched.");
				ACS_LOG_NOTIFY_EX(e.what());
			}
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "SAFE Attribute " << attribute.name() << " not found!" );
		}
	}
	else if (StringUtils::equalsNoCase(attribute.odatatype(), "Double"))
	{
		vector<double> value;
		vector < vector<double> > tmpValue;
		bool found(false);
		bool isset(0);
		for (size_t t = 0; t < attribute.xpath().size(); ++t)
		{
			found = xmlDocSptr->getValue(attribute.xpath(t), value, attribute.minNum());
			string count =  "count(" + StringUtils::substringBeforeLast(attribute.xpath(t), "/") + " ) > 0";
			xmlDocSptr->getValue( count , isset );
			if (found || isset)
			{
				if (!attribute.condition().empty())
				{
					for (size_t i = 0; i < value.size(); ++i)
					{
						string newValue;
						getFromCondition<double>(attribute.condition(), value[i], newValue);
						if (!newValue.empty())
						{
							// overwrite the value
							value[i] = std::stod(newValue);
						}
						else
						{
							// no attribute will be added 
							found = false;
							isset = false;
						}
					}
				}
				tmpValue.push_back(value);
			}
		}
		if (found || isset)
		{
			JSONDoubleAttribute attr(attribute.name(), attribute.odatatype(), value);
			json j_att = attr;
			_jsonAttributes.push_back(j_att);
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "SAFE Attribute " << attribute.name() << " not found!" );
		}
	}
	else if (StringUtils::equalsNoCase(attribute.odatatype(), "DateTimeOffset"))
	{
		vector<string> value;
		vector < vector<string> > tmpValue;
		bool found(false);
		bool isset(0);
		for (size_t t = 0; t < attribute.xpath().size(); ++t)
		{
			found = xmlDocSptr->getValue(attribute.xpath(t), value, attribute.minNum());
			string count =  "count(" + StringUtils::substringBeforeLast(attribute.xpath(t), "/") + " ) > 0";
			xmlDocSptr->getValue( count , isset );
			if (found || isset)
			{
				if (!attribute.condition().empty())
				{
					for (size_t i = 0; i < value.size(); ++i)
					{
						string newValue;
						getFromCondition<string>(attribute.condition(), value[i], newValue);
						if (!newValue.empty())
						{
							// overwrite the value
							value[i] = newValue;
						}
						else
						{
							// no attribute will be added 
							found = false;
							isset = false;
						}
					}
				}
				tmpValue.push_back(value);
			}
		}
		if (found || isset)
		{
			// pass to Zulu format
			for (size_t i = 0; i <  value.size(); ++i)
			{
				//check if date starts with UTC=
				if (StringUtils::startsWith(value[i], "UTC="))
				{
					value[i] = StringUtils::removeBegin(value[i], 4);
				}
				// check if ends with Z, if not add
				if (!StringUtils::endsWith(value[i], "Z"))
				{
					ostringstream os;
					os << value[i] << "Z";
					value[i] = os.str();
				}
				RegExp reg;
				reg.setReg("[0-9]{4}-[01][0-9]-[0-3][0-9]T[0-2][0-9]:[0-5][0-9]:[0-5][0-9].[0-9]{3}([0-9]*)Z");
				RegExp::Matches matches;
				if (!reg.match(value[i], matches))
				{
					ostringstream os;
					//if date is: 9999-99-99T99:99:99
					if (StringUtils::equals(value[i], "9999-99-99T99:99:99Z"))
					{
						os << "2100-01-01T00:00:00.000Z";
					}
					else
					{
						// reformat in the expected format
						DateTime date(DateTime::zuluUTCString2DateTime(value[i]));
						date.asISO8601(os,3) ;
						os << "Z";
					}
					value[i] = os.str();
				}
			}

			JSONDateTimeOffsetAttribute attr(attribute.name(), attribute.odatatype(), value);
			json j_att = attr;
			_jsonAttributes.push_back(j_att);
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "SAFE Attribute " << attribute.name() << " not found!" );
		}
	}
	else if (StringUtils::equalsNoCase(attribute.odatatype(), "Boolean"))
	{
		vector<bool> value;
		vector < vector<bool> > tmpValue;
		bool found(false);
		bool isset(0);
		vector <string> stValue;
		for (size_t t = 0; t < attribute.xpath().size(); ++t)
		{
			// bool are read as string for convenience (upper cases can be present) and converted in bool
			found = xmlDocSptr->getValue(attribute.xpath(t), stValue, attribute.minNum());
			string count =  "count(" + StringUtils::substringBeforeLast(attribute.xpath(t), "/") + " ) > 0";
			xmlDocSptr->getValue( count , isset );
			if (found || isset)
			{
				if (!attribute.condition().empty())
				{
					for (size_t i = 0; i < stValue.size(); ++i)
					{
						string newValue;
						getFromCondition<string>(attribute.condition(), stValue[i], newValue);
						if (!newValue.empty())
						{
							// overwrite the value
							stValue[i] = newValue;
						}
						else
						{
							// no attribute will be added 
							found = false;
							isset = false;
						}
					}
				}
				// string to bool conversion
				value  = to_bool(stValue);
				tmpValue.push_back(value);
			}
		}
		if (found || isset)
		{
			JSONBooleanAttribute attr(attribute.name(), attribute.odatatype(), value);
			json j_att = attr;
			_jsonAttributes.push_back(j_att);
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "SAFE Attribute " << attribute.name() << " not found!" );
		}
	}
	else
	// string case or unknown
	{
		vector <string> value;
		vector < vector<string> > tmpValue;
		bool found(false);
		bool isset(0);
		if (!attribute.fixedValue().empty())
		{
			value.push_back(attribute.fixedValue());
			found = true;
		}
		else
		{
			for (size_t t = 0; t < attribute.xpath().size(); ++t)
			{
				value.clear();
				found = xmlDocSptr->getValue(attribute.xpath(t), value, attribute.minNum());
				string count =  "count(" + StringUtils::substringBeforeLast(attribute.xpath(t), "/") + " ) > 0";
				xmlDocSptr->getValue( count , isset );
				if (found || isset)
				{
					if (!attribute.condition().empty())
					{
						for (size_t i = 0; i < value.size(); ++i)
						{
							string newValue;
							getFromCondition<string>(attribute.condition(), value[i], newValue);
							if (!newValue.empty())
							{
								// overwrite the value
								value[i] = newValue;
							}
							else
							{
								// no attribute will be added 
								found = false;
								isset = false;
							}
						}
					}
					tmpValue.push_back(value);
				}
			}
		}
		if (found || isset)
		{
			try
			{
				if (!attribute.algo().empty())
				{
					executeAlgorithm(attribute.algo(), tmpValue, value, attribute.algoParamFromName(), metadataFile);
				}
				ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Adding Attribute " << attribute.name() );
			
				JSONStringAttribute attr(attribute.name(), attribute.odatatype(), value);
				json j_att = attr;
				_jsonAttributes.push_back(j_att);
			}
			catch (exException & e)
			{
				// it should not occur
				ACS_LOG_WARNING("Attribute " << attribute.name() << " not set due to exception catched.");
				ACS_LOG_NOTIFY_EX(e.what());
			}
		}
		else
		{
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "SAFE Attribute " << attribute.name() << " not found!" );
		}
	}
}
void LTAJsonProductAttributes::executeAlgorithm(const std::string & algo, const vector < vector<string> > & tmpValue, vector <string> & value, const std::string & paramFromName, const std::string & filename)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	try
	{
		if (StringUtils::equalsNoCase(algo, "getS2Coordinates"))
		{
			vector <string> gValue;
			getS2Coordinates(tmpValue, gValue);
			//overwrite value with the new computed one
			value  = gValue;
		}
		else if (StringUtils::equalsNoCase(algo, "concatenate"))
		{
			string sep(",");
			ACS_COND_THROW(tmpValue.size()!=1, LTAJsonProductAttributesException("Unexpected dimension of attribute vector loaded."));

			vector <string> tmp;
			for (size_t i = 0; i < tmpValue.size(); ++i)
			{
				// replace output value with the new concatenated
				tmp.push_back(StringUtils::join(tmpValue[i], ","));
			}
			value = tmp;
		}
		else if (StringUtils::equalsNoCase(algo, "joinBand"))
		{
			ACS_COND_THROW(tmpValue.size()!=1, LTAJsonProductAttributesException("Unexpected dimension of attribute vector loaded."));
			ACS_COND_THROW(tmpValue[0].size()!=1, LTAJsonProductAttributesException("Unexpected dimension of attribute vector loaded."));
			if (!paramFromName.empty())
			{
				RegExp re;
				re.setReg(paramFromName);
				string band;
				
				RegExp::Matches matches;
				if (re.match(filename, matches)) {
					band = matches.at(1).getMatched();
				}
				vector <string> tmp;
				// replace output value with the new concatenated
				ostringstream os;
				os << tmpValue[0][0] << "_" << band;
				tmp.push_back(os.str());
				value = tmp;
			}
		}
		else
		{
			ACS_THROW(exNotImplemented("Algorithm not recognized!\n"));
		}
	}
	catch (exException & e)
	{
		throw;
	}
}
void LTAJsonProductAttributes::executeAlgorithm(const std::string & algo, const vector < vector<int64_t> > & tmpValue, vector <int64_t> & value)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	try
	{
		if (StringUtils::equalsNoCase(algo, "isLastDifferent"))
		{
			// handling of orbit on stop
			// check matrix dimension
			ACS_COND_THROW(tmpValue.size() !=2, LTAJsonProductAttributesException("Unexpected number of orbits found!"));
			for (size_t k = 0; k < tmpValue.size(); ++k)
			{
				ACS_COND_THROW(tmpValue[k].size() != 1, LTAJsonProductAttributesException("Unexpected number of orbits found!"));
			}
			int64_t start = tmpValue[0][0];
			int64_t stop = tmpValue[1][0];
			if (stop != start)
			{
				value.push_back(stop);
			}
		}
		else
		{
			ACS_THROW(exNotImplemented("Algorithm not recognized!\n"));
		}
	}
	catch (exException & e)
	{
		throw;
	}
}

void LTAJsonProductAttributes::addAttributeFromNetCdf(const netCDF::NcFile & dataFile, Attribute const &  attribute)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	ACS_COND_THROW(1!=attribute.xpath().size(), exNotImplemented("Unexpected configuration found! xpath shoulb be a single tag."))
	string path = attribute.xpath(0);
	vector<string> tokens;
	StringUtils::Tokenize(path, tokens, "/");
	if (tokens.size() == 2)
	{
		// attribute are in group definitions
		netCDF::NcGroup group = dataFile.getGroup(tokens[0], netCDF::NcGroup::GroupLocation::AllChildrenGrps );

		if (!group.isNull())
		{
			netCDF::NcGroupAtt groupAtt = group.getAtt(tokens[1]);

			if (StringUtils::equalsNoCase(attribute.odatatype(), "Int64") )
			{
				size_t t = groupAtt.getAttLength();
				long long * intv = new long long[t];
				groupAtt.getValues(intv);
				//ostringstream os; os << *intv;
				//val = os.str();
				vector<int64_t> values;
				values.push_back(*intv);

				JSONIntegerAttribute attr(attribute.name(), attribute.odatatype(), values);
				json j_att = attr;
				_jsonAttributes.push_back(j_att);
			}
			else if (StringUtils::equalsNoCase(attribute.odatatype(), "String"))
			{
				string val = "";
				groupAtt.getValues(val);
				vector<string> values;
				values.push_back(val);
				JSONStringAttribute attr(attribute.name(), attribute.odatatype(), values);
				json j_att = attr;
				_jsonAttributes.push_back(j_att);
			}
			else if (StringUtils::equalsNoCase(attribute.odatatype(), "Double"))
			{
				size_t t = groupAtt.getAttLength();
				double * dval = new double[t];
				groupAtt.getValues(dval);
				vector<double> values;
				values.push_back(*dval);
				JSONDoubleAttribute attr(attribute.name(), attribute.odatatype(), values);
				json j_att = attr;
				_jsonAttributes.push_back(j_att);
			}
			else if (StringUtils::equalsNoCase(attribute.odatatype(), "DateTimeOffset"))
			{
				string val = "";
				groupAtt.getValues(val);
				
				RegExp reg;
				reg.setReg("[0-9]{4}-[01][0-9]-[0-3][0-9]T[0-2][0-9]:[0-5][0-9]:[0-5][0-9].[0-9]{3}([0-9]*)");
				RegExp::Matches matches;
				if (!reg.match(val, matches))
				{
					ostringstream os;
					//if date is: 9999-99-99T99:99:99
					if (StringUtils::equals(val, "99999999T999999"))
					{
						os << "2100-01-01T00:00:00.000";
					}
					else if (StringUtils::equals(val, "00000000T000000"))
					{
						os << "2000-01-01T00:00:00.000";
					}
					else
					{
						DateTime date;
						if (StringUtils::endsWith(val, "Z"))
						{
							//DateTime date(DateTime::zuluUTCString2DateTime(val));
							date = DateTime::zuluUTCString2DateTime(val);
							//date.asISO8601(os,3) ;
						}
						else
						{
							date = val;
						}
						// reformat in the expected format
						//DateTime date(val);
						date.asISO8601(os,3) ;
					}
					val = os.str();
				}
				
				vector<string> values;
				values.push_back(val);
				JSONDateTimeOffsetAttribute attr(attribute.name(), attribute.odatatype(), values);
				json j_att = attr;
				_jsonAttributes.push_back(j_att);
			}
		}
	}
	else if (tokens.size() == 1)
	{
		// retrieve global attributes
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Getting Netcdf Attribute " << tokens[0] );

		auto global_attr = dataFile.getAtt(tokens[0]);
		if (StringUtils::equalsNoCase(attribute.odatatype(), "String"))
		{
			string val = "";
			global_attr.getValues(val);
			if (!attribute.condition().empty())
			{
				string newValue;
				getFromCondition<string>(attribute.condition(), val, newValue);
				if (!newValue.empty())
				{
					// overwrite the value
					val = newValue;
				}
			}
			
			vector<string> values;
			values.push_back(val);
			JSONStringAttribute attr(attribute.name(), attribute.odatatype(), values);
			json j_att = attr;
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Got Attribute " << val );
			_jsonAttributes.push_back(j_att);
		}
		else if (StringUtils::equalsNoCase(attribute.odatatype(), "DateTimeOffset"))
		{
			string val = "";
			global_attr.getValues(val);

			RegExp reg;
			reg.setReg("[0-9]{4}-[01][0-9]-[0-3][0-9]T[0-2][0-9]:[0-5][0-9]:[0-5][0-9].[0-9]{3}([0-9]*)");
			RegExp::Matches matches;
			if (!reg.match(val, matches))
			{
				ostringstream os;
				//if date is: 9999-99-99T99:99:99
				if (StringUtils::equals(val, "99999999T999999"))
				{
					os << "2100-01-01T00:00:00.000";
				}
				else if (StringUtils::equals(val, "00000000T000000"))
				{
					os << "2000-01-01T00:00:00.000";
				}
				else
				{
					DateTime date;
					if (StringUtils::endsWith(val, "Z"))
					{
						//DateTime date(DateTime::zuluUTCString2DateTime(val));
						date = DateTime::zuluUTCString2DateTime(val);
						//date.asISO8601(os,3) ;
					}
					else
					{
						date = val;
					}
					// reformat in the expected format
					//DateTime date(val);
					date.asISO8601(os,3) ;
				}
				val = os.str();
			}

			vector<string> values;
			values.push_back(val);
			JSONDateTimeOffsetAttribute attr(attribute.name(), attribute.odatatype(), values);
			json j_att = attr;
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Got Attribute " << val );
			_jsonAttributes.push_back(j_att);
		}
		else if (StringUtils::equalsNoCase(attribute.odatatype(), "Double"))
		{
			double val = 0;
			global_attr.getValues(&val);
			
			vector<double> values;
			values.push_back(val);
			JSONDoubleAttribute attr(attribute.name(), attribute.odatatype(), values);
			json j_att = attr;
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Got Attribute " << val );
			_jsonAttributes.push_back(j_att);
		}
		else if (StringUtils::equalsNoCase(attribute.odatatype(), "Int64"))
		{
			int64_t val = 0;
			global_attr.getValues(&val);
			
			vector<int64_t> values;
			values.push_back(val);
			JSONIntegerAttribute attr(attribute.name(), attribute.odatatype(), values);
			json j_att = attr;
			ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Got Attribute " << val );
			_jsonAttributes.push_back(j_att);
		}
	}
	else
	{
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Attribute " << attribute.name() << " not found!" );
	}

}

void LTAJsonProductAttributes::getS2Coordinates(const std::vector<std::vector<std::string> > & value, std::vector<std::string> & newValue)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
	// check vector dimension
	size_t m = value[0].size();
	ACS_COND_THROW(value.size()!=2, LTAJsonProductAttributesException("Unexpected coordinates vector dimension!"));
	for (size_t i = 0; i < value.size(); ++i)
	{
		if (m != value[i].size())
		{
			ACS_THROW(LTAJsonProductAttributesException("Unexpected coordinates vector dimension!"));
		}
	}
	// LAT, LON
	std::ostringstream tmpVal;
	for (size_t j = 0; j < m; ++j)
	{
		// LTA-113: alignment to attribute mapping 1.4 
		std::ostringstream pair;
		pair << value[0][j] << ", " << value[1][j];
		tmpVal << pair.str() << " ";
	}
	newValue.push_back(tmpVal.str());
}

_ACS_END_NAMESPACE
