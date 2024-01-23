// PRQA S 1050 EOF
/*

	Copyright 1995-2018, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	A.C.S. MQTT Statistics Library
*/

#include <InfluxDbStatisticPoint.h>

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(InfluxDbStatisticPoint);

InfluxDbStatisticPoint::InfluxDbStatisticPoint(const std::string& measurment) :
	_measurement(measurment),
	_tstamp(),
	_tagSets(),
	_fieldSets()
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_LOW_VERB);
}

InfluxDbStatisticPoint::~InfluxDbStatisticPoint() ACS_NOEXCEPT { }

InfluxDbStatisticPoint::InfluxDbStatisticPoint(const InfluxDbStatisticPoint& point) :
	_measurement(point._measurement),
	_tstamp(point._tstamp),
	_tagSets(point._tagSets),
	_fieldSets(point._fieldSets)
{
	
}

InfluxDbStatisticPoint& InfluxDbStatisticPoint::operator=(const InfluxDbStatisticPoint& point) {
	if (this != &point) {
		_measurement = point._measurement;
		_tstamp = point._tstamp;
		_tagSets = point._tagSets;
		_fieldSets = point._fieldSets;
	}
	
	return *this;
}

std::string InfluxDbStatisticPoint::getTagValue(const std::string& tagkey) const {
	std::string val;
	std::map<std::string, std::string>::const_iterator it = _tagSets.find(tagkey);
	if (it != _tagSets.end()) {
		val = it->second;
	} else {
		ACS_THROW(exInfluxDbStatisticPointException("Tag key \"" + tagkey + "\" does not exists.")); // PRQA S 3081
	}
	
	return val;
}

std::string InfluxDbStatisticPoint::getFieldValue(const std::string& fieldkey) const {
	std::string val;
	std::map<std::string, std::string>::const_iterator it = _fieldSets.find(fieldkey);
	if (it != _fieldSets.end()) {
		val = it->second;
	} else {
		ACS_THROW(exInfluxDbStatisticPointException("Field key \"" + fieldkey + "\" does not exists.")); // PRQA S 3081
	}
	
	return val;
}

void InfluxDbStatisticPoint::addTagSet(const std::string& tagkey, const std::string& tagvalue) {
	_tagSets[tagkey] = tagvalue;
}

// Template specialization for 'int' type
template <>
void InfluxDbStatisticPoint::addFieldSet(const std::string& fieldkey, const int& fieldvalue) {
	std::ostringstream ss; 
	ss << fieldvalue; //<< "i";
	_fieldSets[fieldkey] = ss.str();
}

// Template specialization for 'long' type
template <>
void InfluxDbStatisticPoint::addFieldSet(const std::string& fieldkey, const long& fieldvalue) {
	std::ostringstream ss; 
	ss << fieldvalue; //<< "i";
	_fieldSets[fieldkey] = ss.str();
}

// Template specialization for 'long long' type
template <>
void InfluxDbStatisticPoint::addFieldSet(const std::string& fieldkey, const long long& fieldvalue) {
	std::ostringstream ss; 
	ss << fieldvalue; // << "i";
	_fieldSets[fieldkey] = ss.str();
}

// Template specialization for 'size_t' type
template <>
void InfluxDbStatisticPoint::addFieldSet(const std::string& fieldkey, const size_t& fieldvalue) {
	std::ostringstream ss; 
	ss << fieldvalue; //<< "i";
	_fieldSets[fieldkey] = ss.str();
}

// Template specialization for 'float' type
template <>
void InfluxDbStatisticPoint::addFieldSet(const std::string& fieldkey, const float& fieldvalue) {
	std::ostringstream ss; 
	ss << fieldvalue;
	_fieldSets[fieldkey] = ss.str();
}

// Template specialization for 'double' type
template <>
void InfluxDbStatisticPoint::addFieldSet(const std::string& fieldkey, const double& fieldvalue) {
	std::ostringstream ss; 
	ss << fieldvalue;
	_fieldSets[fieldkey] = ss.str();
}

// Template specialization for 'long double' type
template <>
void InfluxDbStatisticPoint::addFieldSet(const std::string& fieldkey, const long double& fieldvalue) {
	std::ostringstream ss; 
	ss << fieldvalue;
	_fieldSets[fieldkey] = ss.str();
}

// Template specialization for 'string' type
template <>
void InfluxDbStatisticPoint::addFieldSet(const std::string& fieldkey, const std::string& fieldvalue) {
	std::ostringstream ss; 
	ss << "\"" << fieldvalue << "\"";
	_fieldSets[fieldkey] = ss.str();
}

void InfluxDbStatisticPoint::reset() {
	_tstamp = DateTime();
	_tagSets.clear();
	_fieldSets.clear();
}

std::string InfluxDbStatisticPoint::asLineProtocol() const {
	std::ostringstream ss;
	ss << _measurement;
	
	// Serialize tags
	std::map<std::string, std::string>::const_iterator tagit = _tagSets.begin();
	while (tagit != _tagSets.end()) {
		ss << "," << tagit->first << "=\"" << tagit->second << "\"";
		std::advance(tagit, 1);
	}
	
	ss << " ";
	
	// Serialize fields
	std::map<std::string, std::string>::const_iterator fieldit = _fieldSets.begin();
	while (fieldit != _fieldSets.end()) {
		// The field value represents always a numerical value in statistic points, so there is no need for double quotes
		ss << fieldit->first << "=" << fieldit->second; 
		if (std::distance(fieldit, _fieldSets.end()) > 1) {
			ss << ",";
		}
		std::advance(fieldit, 1);
	}
	
	struct timespec ts = _tstamp.Datetime2timespec();
	ss << " " << ((ts.tv_sec * 1000000000) + ts.tv_nsec); // PRQA S 4400
	
	return ss.str();
}

_ACS_END_NAMESPACE
