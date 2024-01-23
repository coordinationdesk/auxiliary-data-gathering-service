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

#ifndef _InfluxDbStatisticPoint_H_
#define _InfluxDbStatisticPoint_H_

#include <acs_c++config.hpp>
#include <Filterables.h>
#include <DateTime.h>

_ACS_BEGIN_NAMESPACE(acs)

/**
 * This class is an influxDb point used to represent statistical informations.
 */
class InfluxDbStatisticPoint {
public:
	exDECLARE_EXCEPTION(exInfluxDbStatisticPointException, exException);
	
	explicit InfluxDbStatisticPoint(const std::string& measurment="statistics");
	virtual ~InfluxDbStatisticPoint() ACS_NOEXCEPT;
	InfluxDbStatisticPoint(const InfluxDbStatisticPoint&);
	InfluxDbStatisticPoint& operator=(const InfluxDbStatisticPoint&);
	
	/** Resets the object */
	void reset();

	/** Getter/Setters for measurement and timestamp */
	inline const std::string& getMeasurement() const { return _measurement; }
	inline const DateTime& getTimestamp() const { return _tstamp; }
	inline void setTimestamp(const DateTime& tstamp) { _tstamp = tstamp; }
	
	/** Getter/Setters for tags */
	void addTagSet(const std::string& tagkey, const std::string& tagvalue);
	std::string getTagValue(const std::string& tagkey) const;

	/** Getter/Setters for fields */
	template <typename T>
	void addFieldSet(const std::string& fieldkey, const T& fieldvalue);
	/** This function always returns the string representation of the field value */
	std::string getFieldValue(const std::string& fieldkey) const;
	
	/** Returns the point formatted according the influxDb line protocol */
	std::string asLineProtocol() const;

private:
	std::string _measurement;
	DateTime _tstamp;
	std::map<std::string, std::string> _tagSets;
	std::map<std::string, std::string> _fieldSets;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(InfluxDbStatisticPoint) ;
};

_ACS_END_NAMESPACE

#endif /* _InfluxDbLineFormatter_H_ */
