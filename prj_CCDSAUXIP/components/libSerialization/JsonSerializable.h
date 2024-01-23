/*
 
    Copyright 2018-2021, Advanced Computer Systems , Inc.
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.acsys.it

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

    $Prod: A.C.S. libSerialization $

    $Log$
    Release 1.0.1  2019/10/28 16:31:35  enrico.carta@acsys.it
    First release
    

*/

#ifndef _JsonSerializable_H_
#define _JsonSerializable_H_

/**
 *     json nlohmann library requires at least C++11 (of course, C++14 will work).
 *     A specific macro is set, __cplusplus, which returns a LONG INT:
 *     0 -> not C++ compiler (C compiler ?)
 *     1 -> C++ version pre C++11
 *     201103L -> At least C++11
 *     201402L -> At least C++14
 */

#if __cplusplus < 201103L
    #define ACS_USE_SIMPLIFIED_JSONOBJECT 1
#endif

// Decomment below to force the use of ACSJsonObject :
// #define ACS_USE_SIMPLIFIED_JSONOBJECT 1

#ifdef ACS_USE_SIMPLIFIED_JSONOBJECT
#include <ACSJsonObject.h>
#else
#include <nlohmann/json.hpp>
#endif

#include <acs_c++config.hpp>
#include <exStream.h>
#include <exException.h>

#include <Serializable.h>
#include <stdint.h>


_ACS_BEGIN_NAMESPACE(acs)

exDECLARE_EXCEPTION(JsonSerializableException, exException);

/**
 * Base class for serialization.
 * Service library for data structure serialization exchange.
 * The base class implements to virtual pure methods, serialize and unserialize.
 * /see https://github.com/nlohmann/json
 */

class JsonSerializable: Serializable {

public:
	enum Enum_JsonSerializableFormat { 
		SF_TEXT = 0x00,		/** Default JSON format, as human-readable Ascii text: { "Key": "value" ... } */ 
		SF_BINARY = 0x01,	/** Binary format (BSON). The most compact and fastest, can be stored on binary files. For example: "\x40\xd2\xff ..." */ 
		SF_HEXDUMP = 0x02,	/** Binary format (BSON), as printable hex-dump. Useful to create string-objects without any special character 
								(e.g. single o double quotes) Fox example: "40d2ff ..." */
		SF_UNKNOWN = 0xff } ;/** Uninitialized -- will raise an error if not overwritten */

	JsonSerializable(const Enum_JsonSerializableFormat& jformat=SF_TEXT) ;
#ifdef ACS_USE_SIMPLIFIED_JSONOBJECT
	JsonSerializable(const ACSJsonObject& object) ;
#else
	JsonSerializable(const nlohmann::json& object) ;
#endif
	virtual ~JsonSerializable() ACS_NOEXCEPT;

	/**--------------------------------------------------------------------------------------------------
       set and get format
	---------------------------------------------------------------------------------------------------*/
	inline void set_format(const Enum_JsonSerializableFormat& jformat)	{ jformat_ = jformat ; }
	inline Enum_JsonSerializableFormat get_format() const				{ return jformat_ ; }

	/**--------------------------------------------------------------------------------------------------
       dump the JSON object
	---------------------------------------------------------------------------------------------------*/
	virtual std::string dump() const ;							

	/**--------------------------------------------------------------------------------------------------
       import (deserialize) string-blob to a JSON object
	---------------------------------------------------------------------------------------------------*/
	void unserialize(const std::string& blob, const Enum_JsonSerializableFormat&) ;	
	virtual void unserialize(const std::string& blob) ;
	virtual void unserialize_from_nested_string(const std::string& blob) ;	/* Turn the /" in " to allow de-encapsulating into string */
	
	
	/**--------------------------------------------------------------------------------------------------
       export (serialize) the JSON object to a string-blob
	---------------------------------------------------------------------------------------------------*/
	std::string serialize(const Enum_JsonSerializableFormat&) const ;
	virtual std::string serialize()	const ;
	virtual std::string serialize_to_nested_string() const ;	/* Turn the " in /" to allow encapsulating into string */
	
	/**--------------------------------------------------------------------------------------------------
       methods to export (serialize) the JSON object to a file and viceversa
	---------------------------------------------------------------------------------------------------*/
	/** export JSON to file, also providing the format */
	void serializeToFile(const std::string& filename, const Enum_JsonSerializableFormat&) const ;
	/** export JSON to file, build-in format */
	virtual void serializeToFile(const std::string& filename) const	;
	/** import JSON from a serialized file, overriding the build-in format */
	void unserializeFromFile(const std::string& filename, const Enum_JsonSerializableFormat&) ;	
	/** import JSON from a serialized file, overriding the build-in format */
	virtual void unserializeFromFile(const std::string& filename) ;



	/**--------------------------------------------------------------------------------------------------
       methods to get/set values in the JSON container
	---------------------------------------------------------------------------------------------------*/

	/** reset the whole container : */
	void reset() ;
	
	/** return true if a key is existing, false if not existing */
	bool has_key (const std::string& key) const ;
	bool has_key (const std::string& key1, const std::string& key2) const ;
	bool has_key (const std::string& key1, const std::string& key2, const std::string& key3) const ; 
		
	/** return a specific exception if key does not exist: SerializableKeyNotFoundException */
	inline void assert_key (const std::string& key) const { ACS_COND_THROW ( (false==has_key(key)), SerializableKeyNotFoundException("No such key: \"" + key + "\"")); }
	inline void assert_key (const std::string& key, const std::string& key2) const { ACS_COND_THROW ( (false==has_key(key, key2)), \
		SerializableKeyNotFoundException("No such key: \"" + key + "/" + key2 + "\"")); }
	inline void assert_key (const std::string& key, const std::string& key2, const std::string& key3) const { ACS_COND_THROW ( (false==has_key(key, key2, key3)), \
		SerializableKeyNotFoundException("No such key: \"" + key + "/" + key2 + "/" + key3 + "\"")); }

#ifdef ACS_USE_SIMPLIFIED_JSONOBJECT
	/** store a key with no value (empty object) */
	inline void add_empty_value (const std::string& key) { json_object_.add_empty_value(key) ; }
	inline void add_empty_value (const std::string& key1, const std::string& key2) { assert_key(key1) ; json_object_[key1].add_empty_value(key2) ; }

	/** store a key/value pair (common, supported values) : */
	void add_pair (const std::string& key, const JsonSerializable& obj) { json_object_.add_pair(key, obj.get_object()) ; }
	void add_pair (const std::string& key, const std::vector<JsonSerializable>& v) { 
		std::vector <ACSJsonObject> tmp ; for (size_t i=0; i<v.size(); i++) { tmp.push_back(v.at(i).get_object()) ; } json_object_.add_pair(key, tmp) ; 
	}

	void add_pair (const std::string& key1, const std::string& key2, const JsonSerializable& obj) { assert_key(key1) ; json_object_[key1].add_pair(key2, obj.get_object()) ; }
	template<typename T>
	void add_pair (const std::string& key, const T& value) { json_object_.add_pair(key, value) ; }
	template<typename T>
	void add_pair (const std::string& key1, const std::string& key2, const T& value) { json_object_[key1].add_pair(key2, value) ; }
#else
	/** store a key with no value (empty object) */
	inline void add_empty_value (const std::string& key) { json_object_[key] = nullptr ; }
	inline void add_empty_value (const std::string& key1, const std::string& key2) {  assert_key(key1) ; json_object_[key1][key2] = nullptr ; }

	/** store a key/value pair (common, supported values) : */
	void add_pair (const std::string& key, const JsonSerializable& obj) { json_object_[key] = obj.get_object() ; }
	void add_pair (const std::string& key, const std::vector<JsonSerializable>& v) { 
		std::vector <nlohmann::json> tmp ; for (size_t i=0; i<v.size(); i++) { tmp.push_back(v.at(i).get_object()) ; } json_object_[key] = tmp ; 
	}
	void add_pair (const std::string& key1, const std::string& key2, const JsonSerializable& obj) { assert_key(key1) ; json_object_[key1][key2] = obj.get_object() ; }
	template<typename T>
	void add_pair (const std::string& key, const T& value) { json_object_[key] = value ; }
	template<typename T>
	void add_pair (const std::string& key1, const std::string& key2, const T& value) { assert_key(key1) ; json_object_[key1][key2] = value ; }
#endif

	template<typename T>
	void add_pair (const std::pair<std::string, T>& p) { add_pair(p.first, p.second) ; }


	/** return TRUE if the value is NULL (anyway, key must exists) */
	inline bool is_null(const std::string& key) const { assert_key(key); return (json_object_.at(key).is_null()) ; }
	inline bool is_null(const std::string& key, const std::string& key2) const { assert_key(key, key2); return (json_object_.at(key).at(key2).is_null()) ; }
	inline bool is_null(const std::string& key, const std::string& key2, const std::string& key3) const { assert_key(key, key2, key3); return (json_object_.at(key).at(key2).at(key3).is_null()) ; }
	/** return TRUE if the value is a bool (key must exists) */
	inline bool is_boolean(const std::string& key) const { assert_key(key); return (json_object_.at(key).is_boolean()) ; }
	inline bool is_boolean(const std::string& key, const std::string& key2) const { assert_key(key, key2); return (json_object_.at(key).at(key2).is_boolean()) ; }
	inline bool is_boolean(const std::string& key, const std::string& key2, const std::string& key3) const { assert_key(key, key2, key3); return (json_object_.at(key).at(key2).at(key3).is_boolean()) ; }
	/** return TRUE if the value is a number (key must exists) */
	inline bool is_number(const std::string& key) const { assert_key(key); return (json_object_.at(key).is_number()) ; }
	inline bool is_number(const std::string& key, const std::string& key2) const { assert_key(key, key2); return (json_object_.at(key).at(key2).is_number()) ; }
	inline bool is_number(const std::string& key, const std::string& key2, const std::string& key3) const { assert_key(key, key2, key3); return (json_object_.at(key).at(key2).at(key3).is_number()) ; }
	/** return TRUE if the value is an array (key must exists) */
	inline bool is_array(const std::string& key) const { assert_key(key); return (json_object_.at(key).is_array()) ; }
	inline bool is_array(const std::string& key, const std::string& key2) const { assert_key(key, key2); return (json_object_.at(key).at(key2).is_array()) ; }
	inline bool is_array(const std::string& key, const std::string& key2, const std::string& key3) const { assert_key(key, key2, key3); return (json_object_.at(key).at(key2).at(key3).is_array()) ; }
	/** return TRUE if the value is a string (key must exists) */
	inline bool is_string(const std::string& key) const { assert_key(key); return (json_object_.at(key).is_string()) ; }
	inline bool is_string(const std::string& key, const std::string& key2) const { assert_key(key, key2); return (json_object_.at(key).at(key2).is_string()) ; }
	inline bool is_string(const std::string& key, const std::string& key2, const std::string& key3) const { assert_key(key, key2, key3); return (json_object_.at(key).at(key2).at(key3).is_string()) ; }
	/** return TRUE if the value is a [recursive] JSON (key must exists) */
	inline bool is_json(const std::string& key) const { assert_key(key); return (json_object_.at(key).is_object()) ; }
	inline bool is_json(const std::string& key, const std::string& key2) const { assert_key(key, key2); return (json_object_.at(key).at(key2).is_object()) ; }
	inline bool is_json(const std::string& key, const std::string& key2, const std::string& key3) const { assert_key(key, key2, key3); return (json_object_.at(key).at(key2).at(key3).is_object()) ; }


	/** retrieve a value: */
#ifdef ACS_USE_SIMPLIFIED_JSONOBJECT
	inline ACSJsonObject get_value(const std::string& key) const { assert_key(key); return json_object_.at(key) ; }

	std::vector <JsonSerializable>&
		get_value (const std::string& key, std::vector<JsonSerializable>& value_out) const { 
			value_out.clear() ; assert_key(key); 
			std::vector <ACSJsonObject> tmp ; json_object_.at(key).get_value(tmp) ;
			for (size_t i=0; i<tmp.size(); i++) { value_out.push_back(JsonSerializable(tmp.at(i))) ; } ; 
		return value_out ;
	} ;

	template<typename T>
	T& get_value (const std::string& key, T& value_out) const { 
		assert_key(key); return json_object_.at(key).get_value(value_out) ; 
	} ;
	template<typename T>
	T& get_value (const std::string& key1, const std::string& key2, T& value_out) const { 
		assert_key(key1, key2); return json_object_.at(key1).at(key2).get_value(value_out) ; 
	} 
	template<typename T>
	T& get_value (const std::string& key1, const std::string& key2, const std::string& key3, T& value_out) const { 
		assert_key(key1, key2, key3); return json_object_.at(key1).at(key2).at(key3).get_value(value_out) ; 
	}
	 
	JsonSerializable& get_value(const std::string& key, JsonSerializable& value_out) const
	{ value_out.set_object( json_object_.at(key) ) ; return value_out ; }
	JsonSerializable& get_value(const std::string& key1, const std::string& key2, JsonSerializable& value_out) const
	{ value_out.set_object( json_object_.at(key1).at(key2) ) ; return value_out ; }
	JsonSerializable& get_value(const std::string& key1, const std::string& key2, const std::string& key3, JsonSerializable& value_out) const
	{ value_out.set_object( json_object_.at(key1).at(key2).at(key3) ) ; return value_out ; }
#else
	/* This cannot be implemened in the simpler class: */
	inline auto get_value(const std::string& key) const { assert_key(key); return json_object_.at(key); }

	/* Provided for I/F compatibility to the simpler class: */
	std::vector <JsonSerializable>&
		get_value (const std::string& key, std::vector<JsonSerializable>& value_out) const { 
			value_out.clear() ; assert_key(key); 
			std::vector <nlohmann::json> tmp = json_object_.at(key) ; 
			for (size_t i=0; i<tmp.size(); i++) { value_out.push_back(JsonSerializable(tmp.at(i))) ; } ; return value_out ;
	} ;
	template<typename T>
		T& get_value (const std::string& key, T& value_out) const { 
			assert_key(key); T tmp = json_object_.at(key) ; return (value_out = tmp) ; 
	} ;	

	template<typename T>
		T& get_value (const std::string& key1, const std::string& key2, T& value_out) const { 
			assert_key(key1, key2); T tmp = json_object_.at(key1).at(key2) ; return (value_out = tmp) ; 
	} ;
	template<typename T>
		T& get_value (const std::string& key1, const std::string& key2, const std::string& key3, T& value_out) const { 
			assert_key(key1, key2, key3); T tmp = json_object_.at(key1).at(key2).at(key3) ; return (value_out = tmp) ; 
	} ;
#endif


public:

	static std::string toHexDigits(const std::vector<uint8_t>&) ; 
	static std::vector<uint8_t>  fromHexDigits(const std::string& s) ;
	static std::string toBinary(const std::vector<uint8_t>&) ; 
	static std::vector<uint8_t>  fromBinary(const std::string& s) ;

	static std::string toStr(const Enum_JsonSerializableFormat& e) ;
	static Enum_JsonSerializableFormat toJsonSerializableFormat(const std::string& e) ;
	static std::string hex_dump(const unsigned char* ptr, const off64_t& sz) ; 
	static std::string hex_dump(const char* ptr, const off64_t& sz) { return hex_dump(reinterpret_cast<const unsigned char*>(ptr), sz) ; }

	/**
		Get/replace the core object (useful to handle JSON entries in which the "value" is a JSON itself
	*/
#ifdef ACS_USE_SIMPLIFIED_JSONOBJECT
	ACSJsonObject get_object() const { return json_object_ ; }
	void set_object(const ACSJsonObject& object) { json_object_ = object ; }
#else
	nlohmann::json get_object() const { return json_object_ ; }
	void set_object(const nlohmann::json& object) { json_object_ = object ; }
#endif
	
	/**
		Run the automatic self-diagnostic test (to find out regressions)
	*/
	static void self_test() ; 
	/**
		Load a specific (previously exported) file, create the json object and parse it against a model
	*/
	static void self_test_from_file(const std::string& filename, const Enum_JsonSerializableFormat&) ;

private:
	/** Parse a predefined filled object and check against a model */
	void self_test_parser() ; 

private:
	Enum_JsonSerializableFormat	jformat_ ; 
#ifdef ACS_USE_SIMPLIFIED_JSONOBJECT
	ACSJsonObject	json_object_ ; 
#else
	nlohmann::json	json_object_ ; 
#endif


	ACS_CLASS_DECLARE_DEBUG_LEVEL(JsonSerializable);
};

inline std::ostream & operator << (std::ostream &s, const JsonSerializable::Enum_JsonSerializableFormat &f)	{ s << JsonSerializable::toStr(f) ; return s ; }
inline exostream & operator << (exostream &s, const JsonSerializable::Enum_JsonSerializableFormat &f)		{ s << JsonSerializable::toStr(f) ; return s ; }

_ACS_END_NAMESPACE

#endif /* _JsonSerializable_H_ */
