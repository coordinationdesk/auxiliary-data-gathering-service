/*
	Copyright 1995-2021, Exprivia SpA - DADF-AS
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
#ifndef _LTAJsonProductAttributes_H_
#define _LTAJsonProductAttributes_H_

#include <exException.h>
#include <iostream>
#include <ios>
#include <acs_c++config.hpp>
#include <json.hpp>
#include <rsResourceSet.h>
#include <netcdf>
#include <LTAUtilities.h>
#include <StringUtils.h>
#include <File.h>

using json = nlohmann::json;

_ACS_BEGIN_NAMESPACE(acs)

/*
	Base Class mapping the content of a single json attribute 
*/
class JSONAttribute
{
public:
	JSONAttribute();
	JSONAttribute(const std::string & name, const std::string & type);
	virtual ~JSONAttribute() ACS_NOEXCEPT = default;

	/**
	 * \brief  Class copy constructor
	 */
	JSONAttribute (JSONAttribute const & a);
	/**
	 * \brief  Operator=
	 */
	JSONAttribute &operator=(JSONAttribute const &);

        std::string name() const { return _name; };
        std::string valueType() const { return _valueType;};

	void name(const std::string & n) { _name = n;};
	void valueType(const std::string & c) { _valueType = c;};
private:
        std::string 	_name;
        std::string 	_valueType;

};
/*
	Specialized Class mapping the content of a single json attribute having string value
*/
class JSONStringAttribute : public JSONAttribute
{
public:
	JSONStringAttribute();
	JSONStringAttribute(const std::string & name, const std::string & type, const std::vector<std::string> & value);
	virtual ~JSONStringAttribute() ACS_NOEXCEPT = default;

	/**
	 * \brief  Class copy constructor
	 */
	JSONStringAttribute (JSONStringAttribute const & a) = default;
	/**
	 * \brief  Operator=
	 */
	JSONStringAttribute &operator=(JSONStringAttribute const &)= default;

        std::vector<std::string> value() const {return _value;} ;
	void value(const std::vector<std::string> & v) { _value = v;};
private:
        std::vector<std::string>	_value;

};
/*
	Specialized Class mapping the content of a single json attribute having integer value
*/
class JSONIntegerAttribute : public JSONAttribute
{
public:
	JSONIntegerAttribute();
	JSONIntegerAttribute(const std::string & name, const std::string & type, const std::vector<int64_t> & value);
	virtual ~JSONIntegerAttribute() ACS_NOEXCEPT = default;

	/**
	 * \brief  Class copy constructor
	 */
	JSONIntegerAttribute (JSONIntegerAttribute const & a) = default;
	/**
	 * \brief  Operator=
	 */
	JSONIntegerAttribute &operator=(JSONIntegerAttribute const &)= default;

        std::vector<int64_t> value() const {return _value;} ;
	void value(const std::vector<int64_t> & v) { _value = v;};
private:
        std::vector<int64_t>	_value;

};
/*
	Specialized Class mapping the content of a single json attribute having double value
*/
class JSONDoubleAttribute : public JSONAttribute
{
public:
	JSONDoubleAttribute();
	JSONDoubleAttribute(const std::string & name, const std::string & type, const std::vector<double> & value);
	virtual ~JSONDoubleAttribute() ACS_NOEXCEPT = default;

	/**
	 * \brief  Class copy constructor
	 */
	JSONDoubleAttribute (JSONDoubleAttribute const & a) = default;
	/**
	 * \brief  Operator=
	 */
	JSONDoubleAttribute &operator=(JSONDoubleAttribute const &)= default;

        std::vector<double> value() const {return _value;} ;
	void value(const std::vector<double> & v) { _value = v;};
private:
        std::vector<double>	_value;

};
/*
	Specialized Class mapping the content of a single json attribute having DateTimeOffset value
*/
class JSONDateTimeOffsetAttribute : public JSONAttribute
{
public:
	JSONDateTimeOffsetAttribute();
	JSONDateTimeOffsetAttribute(const std::string & name, const std::string & type, const std::vector<std::string> & value);
	virtual ~JSONDateTimeOffsetAttribute() ACS_NOEXCEPT = default;

	/**
	 * \brief  Class copy constructor
	 */
	JSONDateTimeOffsetAttribute (JSONDateTimeOffsetAttribute const & a) = default;
	/**
	 * \brief  Operator=
	 */
	JSONDateTimeOffsetAttribute &operator=(JSONDateTimeOffsetAttribute const &)= default;

        std::vector<std::string> value() const {return _value;} ;
	void value(const std::vector<std::string> & v) { _value = v;};
private:
        std::vector<std::string>	_value;
};

/*
	Specialized Class mapping the content of a single json attribute having bool value
*/
class JSONBooleanAttribute : public JSONAttribute
{
public:
	JSONBooleanAttribute();
	JSONBooleanAttribute(const std::string & name, const std::string & type, const std::vector<bool> & value);
	virtual ~JSONBooleanAttribute() ACS_NOEXCEPT = default;

	/**
	 * \brief  Class copy constructor
	 */
	JSONBooleanAttribute (JSONBooleanAttribute const & a) = default;
	/**
	 * \brief  Operator=
	 */
	JSONBooleanAttribute &operator=(JSONBooleanAttribute const &)= default;

        std::vector<bool> value() const {return _value;} ;
	void value(const std::vector<bool> & v) { _value = v;};
private:
        std::vector<bool>	_value;

};


void to_json(json& j, const JSONStringAttribute& p) {
        std::string 		name = p.name();
        std::string 		valueType = p.valueType();
 
	size_t t = p.value().size();
	if (t == 1)
	{
	        std::string 	value = p.value()[0];
		j = json{{"Name", name}, {"ValueType", valueType}, {"Value", value}};
	}
	else
	{
	        std::vector<std::string> 	value = p.value();
		j = json{{"Name", name}, {"ValueType", valueType}, {"Value", value}};
	}
}
void to_json(json& j, const JSONIntegerAttribute& p) {
        std::string 		name = p.name();
        std::string 		valueType = p.valueType();

	size_t t = p.value().size();
	if (t == 1)
	{
	        int64_t 	value = p.value()[0];
		j = json{{"Name", name}, {"ValueType", valueType}, {"Value", value}};
	}
	else
	{
	        std::vector<int64_t> 	value = p.value();
		j = json{{"Name", name}, {"ValueType", valueType}, {"Value", value}};
	}
}
void to_json(json& j, const JSONDoubleAttribute& p) {
        std::string 		name = p.name();
        std::string 		valueType = p.valueType();

	size_t t = p.value().size();
	if (t == 1)
	{
	        double 	value = p.value()[0];
		j = json{{"Name", name}, {"ValueType", valueType}, {"Value", value}};
	}
	else
	{
	        std::vector<double> 	value = p.value();
		j = json{{"Name", name}, {"ValueType", valueType}, {"Value", value}};
	}
}
void to_json(json& j, const JSONDateTimeOffsetAttribute& p) {
        std::string 		name = p.name();
        std::string 		valueType = p.valueType();
	size_t t = p.value().size();
	if (t == 1)
	{
	        std::string 	value = p.value()[0];
		j = json{{"Name", name}, {"ValueType", valueType}, {"Value", value}};
	}
	else
	{
	        std::vector<std::string> 	value = p.value();
		j = json{{"Name", name}, {"ValueType", valueType}, {"Value", value}};
	}
}

void to_json(json& j, const JSONBooleanAttribute& p) {
        std::string 		name = p.name();
        std::string 		valueType = p.valueType();

	size_t t = p.value().size();
	if (t == 1)
	{
	        bool 	value = p.value()[0];
		j = json{{"Name", name}, {"ValueType", valueType}, {"Value", value}};
	}
	else
	{
	        std::vector<bool> 	value = p.value();
		j = json{{"Name", name}, {"ValueType", valueType}, {"Value", value}};
	}
}

/**
 * \brief Base class 
 */
class LTAJsonProductAttributes 
{

public:

	exDECLARE_EXCEPTION(LTAJsonProductAttributesException, exException) ; 
	
	/**
	 * \brief Default Class constructor
	 */
	LTAJsonProductAttributes(std::string const & workDir, std::string const & filename);
	/**
	 * \brief  Class destructor
	 */
	virtual ~LTAJsonProductAttributes() ACS_NOEXCEPT = default;
	
	void setConfAttributes(const std::vector <Attribute> & attr);
	void setConfAttributesFromFilename(const std::vector <AttributeFromFilename> & attr);
	void setAllowOptional(bool optional);
	void setIsNetcdf(bool isNc);

	std::vector<json> & readAttributes();
	
	std::vector<json> getJsonAttributes() const {return _jsonAttributes;};

protected:	
	/**
	 * \brief  Class copy constructor
	 */
	LTAJsonProductAttributes(const LTAJsonProductAttributes & );

	/**
	 * \brief  Operator=
	 */
	LTAJsonProductAttributes &operator=(const LTAJsonProductAttributes &);

private:

	void init();
	void getMetadataFile(const std::string & metadataFileRExp, std::string & metadataFile);
	void addAttribute(Attribute const &  attribute);
	void addAttributeFromManifest(std::string const & metadataFile, Attribute const &  attribute);
	void addAttributeFromNetCdf(const netCDF::NcFile & datafile, Attribute const &  attribute);

	template <typename T> void getFromCondition(const std::string & condition, const T & value, std::string& newValue);
	void getS2Coordinates(const std::vector<std::vector<std::string> > & value, std::vector<std::string> & newValue);
	void executeAlgorithm(const std::string & algo, const std::vector < std::vector<std::string> > & tmpValue, std::vector <std::string> & value, const std::string & paramFromName="", const std::string & filename="");
	void executeAlgorithm(const std::string & algo, const std::vector < std::vector<int64_t> > & tmpValue, std::vector <int64_t> & value);
	
	bool 		_isNetcdf;
	std::string 	_tmpWorkDir;
	std::string 	_productName;
	std::vector <json> 	_jsonAttributes;
	std::vector <Attribute> _attributes; 	// attribute read in conf space
	bool _attrFromFileSet;			// true if AttributeFromFilename has been set from conf space
	std::vector <AttributeFromFilename> _attrFromFilename; // attribute to be extracted from filename read in conf space
	bool _allowOptional; // set to optional all the attributes (and related metadata files)

	ACS_CLASS_DECLARE_DEBUG_LEVEL(LTAJsonProductAttributes) ;

};
template <typename T>
inline void LTAJsonProductAttributes::getFromCondition(const std::string & condition, const T & value, std::string & newValue)
{
	std::ostringstream tos;
	tos << value;
	std::string command = StringUtils::transform(condition, "%%%", tos.str());
	std::string tmpFile = StringUtils::pathJoin(_tmpWorkDir, File::tempname("tmpFile"));
	FileGuard guard(tmpFile);
	std::ostringstream os;
	os << "echo $(" << command <<")>" << tmpFile;
	//std::cout << "bash COMMAND:" << os.str() << std::endl;
	
	int status = system(os.str().c_str());
	if (status != 0) {
		ACS_THROW(LTAJsonProductAttributesException("Error executing command " + command)); // PRQA S 3081
	}
	std::ifstream outFile;
	outFile.open(tmpFile);

	outFile >> newValue;
	outFile.close();
	//std::cout << "Value read from TMP file:" << newValue << std::endl;

}
_ACS_END_NAMESPACE

#endif //_LTAJsonProductAttributes_H_
