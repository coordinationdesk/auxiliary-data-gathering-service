// PRQA S 1050 EOF
/*

	Copyright 2019, Exprivia - DFDA-AS
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.exprivia.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia SpA;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Exprivia SpA

	$Prod: LTA Rest Interface $


*/

#ifndef _EstimationTimeServiceResponsePayload_h_
#define _EstimationTimeServiceResponsePayload_h_

#include <acs_c++config.hpp>
#include <ThreadSafe.h>
#include <string>
#include <json.hpp>
#include <dbPersistent.h>

_ACS_BEGIN_NAMESPACE(acs)

/** This object implements the RESTful response payload sent by LTAEstimationTimeService */
class EstimationTimeServiceResponsePayload {
public:
	exDECLARE_EXCEPTION(EstimationTimeServiceResponsePayloadException, exException);
	
	class EstimationResponseEntry {

	public:
		static const std::string RESPONSE_PAYLOAD_KEY_INVENTORY_ID;
		static const std::string RESPONSE_PAYLOAD_KEY_ERROR_MESSAGE;
		static const std::string RESPONSE_PAYLOAD_KEY_ESTIMATED_DURATION;
	
		EstimationResponseEntry();
		EstimationResponseEntry(const std::string& jsstr);
		EstimationResponseEntry(const nlohmann::json& json);
		EstimationResponseEntry(const EstimationResponseEntry& e);
		EstimationResponseEntry& operator=(const EstimationResponseEntry& e);
		
		void fromJSON(const std::string& jsstr);
		void fromJSONObject(const nlohmann::json& json);
		std::string toJSON() const;
		nlohmann::json toJSONObject() const;
		
		inline dbPersistent::IdType getInventoryId() { 
			return inventoryId_;
		}
		
		inline double getEstimatedDuration(bool* isset = 0) { 
			if (0 != isset) {
				*isset = estimDurationSet_;
			}
			return estimDuration_;
		}
		
		inline const std::string& getErrorMessage(bool* isset = 0) { 
			if (0 != isset) {
				*isset = errmsgSet_;
			}
			return errmsg_;
		}
		
		inline void setInventoryId(dbPersistent::IdType id) { 
			inventoryId_ = id;
		}
		
		inline void setEstimatedDuration(double estim) { 
			estimDuration_ = estim;
			estimDurationSet_ = true;
		}
		
		inline void setErrorMessage(const std::string& errmsg) { 
			errmsg_ = errmsg;
			errmsgSet_ = true;
		}

	private:
		dbPersistent::IdType inventoryId_;
		double estimDuration_;
		std::string errmsg_;
		bool estimDurationSet_;
		bool errmsgSet_;
	};

public:
	/** Ctors/Dtors */
	EstimationTimeServiceResponsePayload();
	explicit EstimationTimeServiceResponsePayload(const std::string& jsstr);
	explicit EstimationTimeServiceResponsePayload(const nlohmann::json& json);
	EstimationTimeServiceResponsePayload(const EstimationTimeServiceResponsePayload&);
	virtual ~EstimationTimeServiceResponsePayload() ACS_NOEXCEPT;
	
	/** Operators overload */
	EstimationTimeServiceResponsePayload& operator=(const EstimationTimeServiceResponsePayload&);
	
	/** Setters/Getters */
	void setJobResponsible(const std::string& jr);
	void setEstimationResponseEntries(const std::vector<EstimationResponseEntry>&);
	void addEstimationResponseEntry(const EstimationResponseEntry&);
	
	const std::string& getJobResponsible();
	void getEstimationResponseEntries(std::vector<EstimationResponseEntry>&);
	
	/** Returns the serialized json representation of this object */
	std::string toJSON() const;
	nlohmann::json toJSONObject() const;
	
	/** Initialize this object from the json string in input */
	void fromJSONObject(const nlohmann::json& json);
	void fromJSON(const std::string& jsstr);

private:
	static const std::string RESPONSE_PAYLOAD_KEY_JOBRESPONSIBLE;
	static const std::string RESPONSE_PAYLOAD_KEY_ESTIMATIONS;

private:
	Mutex mutex_;
	std::string jobResponsible_;
	std::vector<EstimationResponseEntry> estimations_;

	ACS_CLASS_DECLARE_DEBUG_LEVEL(EstimationTimeServiceResponsePayload);
};

class EstimationTimeServiceResponsePayloadList {
public:
	exDECLARE_EXCEPTION(EstimationTimeServiceResponsePayloadListException, exException);
public:
	/** Ctors / Dtors */
	EstimationTimeServiceResponsePayloadList();
	explicit EstimationTimeServiceResponsePayloadList(const std::string& jsstr);
	EstimationTimeServiceResponsePayloadList(const EstimationTimeServiceResponsePayloadList&);
	virtual ~EstimationTimeServiceResponsePayloadList() ACS_NOEXCEPT;

	/** Operators overload */
	EstimationTimeServiceResponsePayload& operator[](size_t idx);
	const EstimationTimeServiceResponsePayload& operator[](size_t idx) const;

	/** Insert the payload in the list at the position specified by payload.requestId */
	void add(const EstimationTimeServiceResponsePayload& payload);
	void add(const EstimationTimeServiceResponsePayloadList& payload);
	void sort();
	void sort(std::vector<EstimationTimeServiceResponsePayload>&) const;
	void clear();
	size_t size() const;
	
	const std::vector<EstimationTimeServiceResponsePayload>& getResponses() const;
	
	/** Returns the serialized json representation of this object */
	std::string toJSON() const;
	nlohmann::json toJSONObject() const;
	
	/** Initialize this object from the json string in input */
	void fromJSON(const std::string& json);
	void fromJSONObject(const nlohmann::json& json);

private:
	Mutex _mutex;
	std::vector<EstimationTimeServiceResponsePayload> _responses;
	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(EstimationTimeServiceResponsePayloadList);

};

_ACS_END_NAMESPACE

#endif /* _EstimationTimeServiceResponsePayload_h_ */
