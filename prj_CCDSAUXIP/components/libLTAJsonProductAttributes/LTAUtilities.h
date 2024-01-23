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
#ifndef _LTAUtilities_H_
#define _LTAUtilities_H_

#include <exException.h>
#include <iostream>
#include <acs_c++config.hpp>
#include <rsResourceSet.h>
#include <StringUtils.h>

_ACS_BEGIN_NAMESPACE(acs)

bool str_to_bool(std::string str) ;
std::vector<bool> to_bool(const std::vector<std::string> & str) ;

enum ExtractionStrategy {STANDARD=0, NONE, UNCOMPRESS, JP2, UNTAR };

ExtractionStrategy string2ExtractionStrategy(const std::string & value);

struct FileTypeConf
{
	FileTypeConf(const std::string & re, const std::string & s, const std::vector<std::string> & r, const ExtractionStrategy st) : regExp(re), section(s), rule(r), strategy(st) {};
	FileTypeConf(): regExp(), section(), rule(), strategy() {};
	std::string regExp;
	std::string section;
	std::vector<std::string> rule;
	ExtractionStrategy strategy;
};

template<> inline
void rsResourceSet::getValue<FileTypeConf>(const std::string &res,  FileTypeConf&v) const // template specialization // PRQA S 2133, 2134
{
	Lock lock(_mutex) ;
	std::string regExp;
	std::string section;
	std::vector<std::string> rule;
	std::string tmpRes(res);
    	if (tmpRes != "")
    	{
		tmpRes = tmpRes + ".";
	}
       	getValue(tmpRes+"RegExp", regExp) ;
       	getValue(tmpRes+"SectionName", section) ;
	getArray(tmpRes+"List_of_ExtractionRules.ExtractionRule", rule, false) ;
       	std::string s;
	getValue(tmpRes+"ExtractionStrategy", s, false) ;
	
	ExtractionStrategy strategy = string2ExtractionStrategy(s);

	v = FileTypeConf(regExp,section, rule, strategy);
}

/*

* Class mapping the content of a single configurated attribute
*/
class Attribute
{
public: 
	Attribute(): _name(), _metadataFile(), _metadataFileRegExp(), _fixedValue(), _xpath(), _condition(), _algo(), _algoParamFromName(), _odatatype(), _minNum(), _maxNum(){};
	Attribute(const std::string & name, const std::string & metadataFile, const std::string & metadataFileRegExp, const std::vector<std::string> & xpath, const std::string & odatatype, const short & min, const short & max): 
		_name(name), _metadataFile(metadataFile), _metadataFileRegExp(metadataFileRegExp), _fixedValue(), _xpath(xpath), _condition(), _algo(), _algoParamFromName(), _odatatype(odatatype), _minNum(min), _maxNum(max){};
	virtual ~Attribute() ACS_NOEXCEPT = default;
	/**
	 * \brief  Class copy constructor
	 */
	Attribute (Attribute const & a);
	/**
	 * \brief  Operator=
	 */
	Attribute &operator=(Attribute const &);
	std::string name() const {return _name;};
	std::string metadataFile() const{return _metadataFile;};
	std::string metadataFileRegExp() const{return _metadataFileRegExp;};
	std::string fixedValue() const{return _fixedValue;};
	std::vector<std::string> xpath() const{return _xpath;};
	std::string xpath(const size_t & i) const{return _xpath[i];};
	std::string condition() const{return _condition;};
	std::string algo() const{return _algo;};
	std::string algoParamFromName() const{return _algoParamFromName;};
	std::string odatatype() const{return _odatatype;}; 
	short 	minNum() const { return _minNum;};
	short 	maxNum() const {return _maxNum;};
	void setCondition(const std::string  & cond) { _condition = cond; };
	void setFixedValue(const std::string  & value) { _fixedValue = value; };
	void setAlgo(const std::string  & algo) { _algo = algo; };
	void setAlgoParameterFromFilename(const std::string  & param) { _algoParamFromName = param; };
private:
	std::string _name;
	std::string _metadataFile;
	std::string _metadataFileRegExp;	// if metadatafile is not available, the reg exp to extract it is indicated
	std::string _fixedValue;
	std::vector<std::string> _xpath;
	std::string _condition;			// condition to be applied to the attribute read
	std::string _algo;			// flag indicating if a conversion algorithm as to be applied (for coordinates attribute)
	std::string _algoParamFromName; 	// parameter for filename to be used in the algoritm
	std::string _odatatype;	
	short 	_minNum;
	short 	_maxNum;
};
template<> inline
void rsResourceSet::getValue<Attribute>(const std::string &res,  Attribute&v) const // template spacialization // PRQA S 2133, 2134
{
	Lock lock(_mutex) ;
	std::string name;
	std::string metadataFile;
	std::string metadataFileRegExp;
	std::string fixedValue;
	std::vector<std::string> xpath;
	std::string condition;
	std::string algo;
	std::string param;
	std::string odatatype; 
	short 	minNum;
	short 	maxNum;
	
	std::string tmpRes(res);
    	if (tmpRes != "")
    	{
		tmpRes = tmpRes + ".";
	}
        getValue(tmpRes+"name")->get(name) ;
	getValue(tmpRes+"metadataFile", metadataFile, false) ;
	getValue(tmpRes+"metadataFileRegExp", metadataFileRegExp, false) ;
	getValue(tmpRes+"fixedValue", fixedValue, false) ;
        getArray(tmpRes+"xpath", xpath) ;
	getValue(tmpRes+"condition", condition, false) ;
	getValue(tmpRes+"algo", algo, false) ;
	getValue(tmpRes+"algoParamFromName", param, false) ;
        getValue(tmpRes+"odatatype")->get(odatatype) ;        	
        getValue(tmpRes+"cardinality.min")->get(minNum) ;    	
        getValue(tmpRes+"cardinality.max")->get(maxNum) ;
        
   	v = Attribute(name, metadataFile, metadataFileRegExp, xpath, odatatype, minNum, maxNum);
	if (!condition.empty())
	{
		v.setCondition(condition);
	}
	if (!fixedValue.empty())
	{
		v.setFixedValue(fixedValue);
	}
	if (!algo.empty())
	{
		v.setAlgo(algo);
	}
	if (!param.empty())
	{
		v.setAlgoParameterFromFilename(param);
	}
}

class AttributeFromFilename
{
public: 
	AttributeFromFilename(): _name(), _regExp(), _odatatype(), _condition() {};
	AttributeFromFilename(const std::string & name, const std::string & regExp, const std::string & odatatype, const std::string & condition = ""): _name(name), _regExp(regExp), _odatatype(odatatype), _condition(condition) {};
	virtual ~AttributeFromFilename() ACS_NOEXCEPT = default;
	/**
	 * \brief  Class copy constructor
	 */
	AttributeFromFilename (AttributeFromFilename const & a);
	/**
	 * \brief  Operator=
	 */
	AttributeFromFilename &operator=(AttributeFromFilename const &);
	std::string name() const {return _name;};
	std::string regExp() const {return _regExp;};
	std::string odatatype() const {return _odatatype;}; 
	std::string condition() const {return _condition;}; 
private:
	std::string _name;
	std::string _regExp;
	std::string _odatatype;
	std::string _condition;

};
template<> inline
void rsResourceSet::getValue<AttributeFromFilename>(const std::string &res,  AttributeFromFilename&v) const // template spacialization // PRQA S 2133, 2134
{
	Lock lock(_mutex) ;
	std::string id;
	std::string name;
	std::string regExp;
	std::string odatatype; 
	std::string condition;
	std::string tmpRes(res);
    if (tmpRes != "")
    {
		tmpRes = tmpRes + ".";
	}

       	getValue(tmpRes+"name")->get(name) ;
       	getValue(tmpRes+"regExp")->get(regExp) ;
       	getValue(tmpRes+"odatatype")->get(odatatype) ;        	
       	getValue(tmpRes+"condition", condition, false) ;   	
        
   	v = AttributeFromFilename(name, regExp, odatatype, condition);     
};

class LTAExtractedConf
{
public:
	LTAExtractedConf();
	virtual ~LTAExtractedConf() ACS_NOEXCEPT = default;
	
	// getter methods
	std::string 	satellite() { return _satellite;};
	std::string 	extension() { return _extension;};
	std::string 	sectionName() {return _sectionName;};
	std::vector<std::string> extractionRule() {return _extractionRule;};
	ExtractionStrategy extractionStrategy() {return _extractionStrategy;};
	// setter methods
	void setSatellite(const std::string & s ) { _satellite = s;};
	void setExtension(const std::string & e) { _extension = e;};
	void setSectionName(const std::string & s) {_sectionName = s;};
	void setExtractionRule(const std::vector<std::string> & r) {_extractionRule = r;};
	void setExtractionStrategy(const ExtractionStrategy & s) {_extractionStrategy = s;};
	// get methods
	std::string getExtension() const { return _extension;} ;
	
private:
	/**
	 * \brief  Class copy constructor
	 */
	LTAExtractedConf (LTAExtractedConf const & a) = delete;
	/**
	 * \brief  Operator=
	 */
	LTAExtractedConf &operator=(LTAExtractedConf const &) = delete;

	std::string 	_satellite;
	std::string 	_extension;
	std::string 	_sectionName;
	std::vector<std::string> _extractionRule;
	ExtractionStrategy 	_extractionStrategy;
};
/**
 * \brief Base class 
 */
class LTAUtilities 
{

public:

	exDECLARE_EXCEPTION(LTAUtilitiesException, exException) ; 

	static void loadConf(const std::string & productName, LTAExtractedConf & conf);
	static void getConfAttributes(const std::string & satellite, const std::string & sectionName, std::vector <Attribute> & attributes);
	static void getConfAttributes(const std::string & satellite, const std::string & sectionName, std::vector <AttributeFromFilename> & attrFromFilename);

	ACS_CLASS_DECLARE_DEBUG_LEVEL(LTAUtilities) ;

};
_ACS_END_NAMESPACE

#endif //_LTAUtilities_H_
