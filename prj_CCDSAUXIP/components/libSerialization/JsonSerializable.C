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

#include <JsonSerializable.h>
#include <File.h>
#include <SerializableV.h>
#include <StringUtils.h>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

namespace {
    SerializableV version;
}

/***********************************************************************
 * 
 * 			JsonSerializable definition
 * 
 ***********************************************************************/
ACS_CLASS_DEFINE_DEBUG_LEVEL(JsonSerializable);

JsonSerializable::JsonSerializable(const Enum_JsonSerializableFormat& jformat) :
jformat_(jformat),
json_object_()
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method called. Format: " + toStr(jformat_) << endl ;
	ACS_CLASS_END_DEBUG
}

#ifdef ACS_USE_SIMPLIFIED_JSONOBJECT
JsonSerializable::JsonSerializable(const ACSJsonObject& object) :
#else
JsonSerializable::JsonSerializable(const nlohmann::json& object) :
#endif
jformat_(SF_TEXT),
json_object_(object)
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method called. Format: " + toStr(jformat_) << endl ;
	ACS_CLASS_END_DEBUG
}

JsonSerializable::~JsonSerializable() ACS_NOEXCEPT 
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method called." << endl ;
	ACS_CLASS_END_DEBUG
}


/****************************************************************************
**		dump
****************************************************************************/
string JsonSerializable::dump() const
{
	/** dump the object : */

#ifdef ACS_USE_SIMPLIFIED_JSONOBJECT
	return json_object_.toStr() ; 
#else
	return json_object_.dump() ; 
#endif
}


/****************************************************************************
**		reset
****************************************************************************/
void JsonSerializable::reset() 
{
	/** reset the whole container : */

#ifdef ACS_USE_SIMPLIFIED_JSONOBJECT
	ACSJsonObject empty_obj ; 
#else
	nlohmann::json empty_obj ;
#endif
	set_object(empty_obj) ; 
}


/****************************************************************************
**		has_key
****************************************************************************/
bool JsonSerializable::has_key (const std::string& key) const {
#ifdef ACS_USE_SIMPLIFIED_JSONOBJECT
	return (json_object_.has_key(key)) ;
#else
	return (json_object_.find(key) != json_object_.end()) ;
#endif
}

bool JsonSerializable::has_key (const std::string& key1, const std::string& key2) const {
	if (false == has_key(key1)) { return false ; } else 
#ifdef ACS_USE_SIMPLIFIED_JSONOBJECT
	{ return (json_object_.at(key1).has_key(key2)) ; }
#else
	{ return (json_object_.at(key1).find(key2) != json_object_.at(key1).end()) ; }
#endif
}

 bool JsonSerializable::has_key (const std::string& key1, const std::string& key2, const std::string& key3) const {
	if (false == has_key(key1, key2)) { return false ; } else 
#ifdef ACS_USE_SIMPLIFIED_JSONOBJECT
	{ return (json_object_.at(key1).at(key2).has_key(key3)) ; }
#else
	{ return (json_object_.at(key1).at(key2).find(key3) != json_object_.at(key1).at(key2).end()) ; }
#endif
}


/****************************************************************************
**		serialize
****************************************************************************/
string JsonSerializable::serialize() const
{
	return serialize(jformat_) ; 
}

/****************************************************************************
**		serialize_to_nested_string
****************************************************************************/
string JsonSerializable::serialize_to_nested_string() const
{
	const string from_s("\x22") ;	// That is: "
	const string to_s("\x5c\x22") ;	// That is: \"
	const string s ( serialize(jformat_) ) ; 
	return StringUtils::transform(s, from_s, to_s); 
}


/****************************************************************************
**		serialize
****************************************************************************/
string JsonSerializable::serialize(const Enum_JsonSerializableFormat& jformat) const
{
	/**--------------------------------------------------------------------------------------------------
       export (serialize) the JSON object to a string-blob
	---------------------------------------------------------------------------------------------------*/

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method called. Format: " + toStr(jformat) << endl ;
	ACS_CLASS_END_DEBUG

	ostringstream o ; 
	switch(jformat)
	{
		case SF_TEXT:
		{ o << json_object_ ; break ; }
#ifdef ACS_USE_SIMPLIFIED_JSONOBJECT
		case SF_HEXDUMP:
		case SF_BINARY:
#else
		case SF_HEXDUMP:
		{
			vector<std::uint8_t> v_bson = nlohmann::json::to_bson(json_object_);
			const string v_string(toHexDigits(v_bson)) ;
			o << v_string ; break ; 
			
		}
		case SF_BINARY:
		{
			vector<std::uint8_t> v_bson = nlohmann::json::to_bson(json_object_);
			const string v_string(toBinary(v_bson)) ;
			o << v_string ; break ; 
		}
#endif
		default:
			ACS_THROW(exIllegalValueException("Format unsupported: " + toStr(jformat))) ; 
	}

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method finished." << endl ;
	ACS_CLASS_END_DEBUG
	return o.str() ; 
}


/****************************************************************************
**		unserialize
****************************************************************************/
void JsonSerializable::unserialize(const string& blob)
{
	unserialize(blob, jformat_) ;	
}


/****************************************************************************
**		unserialize_from_nested_string
****************************************************************************/
void JsonSerializable::unserialize_from_nested_string(const string& blob)
{
	const string from_s ("\x5c\x22") ;	// That is: \"
	const string to_s ("\x22") ;		// That is: "
	const string s ( StringUtils::transform(blob, from_s, to_s) ) ; 

	unserialize(s, jformat_) ;	
}


/****************************************************************************
**		unserialize
****************************************************************************/
void JsonSerializable::unserialize(const string& blob, const Enum_JsonSerializableFormat& jformat)
{
	/**--------------------------------------------------------------------------------------------------
       import (deserialize) string-blob to a JSON object
	---------------------------------------------------------------------------------------------------*/

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method called. Format: " + toStr(jformat) << endl ;
	ACS_CLASS_END_DEBUG

	// Empty the current container (no "merge" should be possible...)
	reset() ; 
	
	switch(jformat)
	{
#ifdef ACS_USE_SIMPLIFIED_JSONOBJECT
		case SF_TEXT:
		{ 
			json_object_.set_object(blob) ; break ; 
		}
		case SF_HEXDUMP:
		case SF_BINARY:
#else
		case SF_TEXT:
		{ 
			istringstream is(blob) ; is >> json_object_ ; break ; 
		}
		case SF_HEXDUMP:
		{
			const vector<uint8_t> v_bson (fromHexDigits(blob)) ; 
			json_object_ = nlohmann::json::from_bson(v_bson);
			break ; 
		}
		case SF_BINARY:
		{
			const vector<uint8_t> v_bson (fromBinary(blob)) ; 
			json_object_ = nlohmann::json::from_bson(v_bson);
			break ; 
		}
#endif		
		default:
			ACS_THROW(exIllegalValueException("Format unsupported: " + toStr(jformat))) ; 
	}

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method finished." << endl ;
	ACS_CLASS_END_DEBUG
	return ; 
}	


/****************************************************************************
**		serializeToFile
****************************************************************************/
void JsonSerializable::serializeToFile(const string& filename) const
{
	serializeToFile(filename, jformat_) ; 
}


/****************************************************************************
**		serializeToFile
****************************************************************************/
void JsonSerializable::serializeToFile(const string& filename, const Enum_JsonSerializableFormat& jformat) const
{
	/**--------------------------------------------------------------------------------------------------
       export (serialize) the JSON object to a fiel
	---------------------------------------------------------------------------------------------------*/

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method called. Destination filename: \"" << filename << "\"  format: " << toStr(jformat) << endl ;
	ACS_CLASS_END_DEBUG

	std::ofstream ofs(filename.c_str(), ofstream::binary|ofstream::trunc|ofstream::ate);
	switch(jformat)
	{
		case SF_TEXT:
		{ ofs << json_object_ ; break ; }
#ifdef ACS_USE_SIMPLIFIED_JSONOBJECT
		case SF_HEXDUMP:
		case SF_BINARY:
#else
		case SF_HEXDUMP:
		{
			vector<std::uint8_t> v_bson = nlohmann::json::to_bson(json_object_);
			const string v_string(toHexDigits(v_bson)) ;
			ofs << v_string ; break ; 
		}
		case SF_BINARY:
		{
			vector<std::uint8_t> v_bson = nlohmann::json::to_bson(json_object_);
			const string v_string(toBinary(v_bson)) ;
			ofs << v_string ; break ; 
		}
#endif
		default:
			ACS_THROW(exIllegalValueException("Format unsupported: " + toStr(jformat))) ; 
	}
	
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method finished." << endl ;
	ACS_CLASS_END_DEBUG
}


/****************************************************************************
**		unserializeFromFile
****************************************************************************/
void JsonSerializable::unserializeFromFile(const string& filename)
{
	unserializeFromFile(filename, jformat_) ;
}


/****************************************************************************
**		unserializeFromFile
****************************************************************************/
void JsonSerializable::unserializeFromFile(const string& filename, const Enum_JsonSerializableFormat& jformat) 
{
	/**--------------------------------------------------------------------------------------------------
       import (deserialize) a file to a JSON object
	---------------------------------------------------------------------------------------------------*/

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method called. Source filename: \"" << filename << "\"  format: " << toStr(jformat) << endl ;
	ACS_CLASS_END_DEBUG

	ACS_COND_THROW((false == File::exists(filename)) , exIOException("File not found: \"" + filename + "\"")) ;

	// Empty the current container (no "merge" should be possible...)
	reset() ; 

	ifstream ifs(filename.c_str()) ; 
	switch(jformat)
	{
#ifdef ACS_USE_SIMPLIFIED_JSONOBJECT
		case SF_TEXT:
		{ 
			string blob, s ; while (std::getline(ifs, s)) { blob += s ; }
			json_object_.set_object(blob) ; break ; 
		}
		case SF_HEXDUMP:
		case SF_BINARY:
#else
		case SF_TEXT:
		{ 
			ifs >> json_object_ ;
			break ; 
		}
		case SF_HEXDUMP:
		{
			stringstream ss; ss << ifs.rdbuf() ;
			const vector<uint8_t> v_bson (fromHexDigits(ss.str())) ; 
			json_object_ = nlohmann::json::from_bson(v_bson);
			break ; 
		}
		case SF_BINARY:
		{
			stringstream ss; ss << ifs.rdbuf() ;
			const vector<uint8_t> v_bson (fromBinary(ss.str())) ; 
			json_object_ = nlohmann::json::from_bson(v_bson);
			break ; 
		}
#endif		
		default:
			ACS_THROW(exIllegalValueException("Format unsupported: " + toStr(jformat))) ; 
	}

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method finished." << endl ;
	ACS_CLASS_END_DEBUG
}	


/****************************************************************************
**		[STATIC] self_test
****************************************************************************/
void JsonSerializable::self_test()  
{
	/**
		Create a specific object and parse it
	*/
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method called." << endl ;
	ACS_CLASS_END_DEBUG

	{
		// Create a json object locally
		JsonSerializable json ;
		json.add_empty_value("Company name") ;
		json.add_pair("Name", "Jason") ;
		json.add_pair("Employee", false) ;
		{ vector <string> v ; v.push_back("555-1234") ; v.push_back("666-7777") ; json.add_pair("Home phones", v) ; }
		json.add_pair("ID", 4242) ;
		json.add_pair("weight", 123.456) ;
		{ vector <int> empty_v ; json.add_pair("Work phones", empty_v) ; }
		{	JsonSerializable bday ; bday.add_pair("day", 31) ; bday.add_pair("month", 12) ; bday.add_pair("year", 1678) ; 
			json.add_pair("birthday", bday) ; }
		{ 
			vector <JsonSerializable> v ;
			JsonSerializable j1 ; j1.add_pair("license", "AA-000-AA") ; j1.add_pair("year", 2000) ; v.push_back(j1) ; 
			JsonSerializable j2 ; j2.add_pair("license", "ZZ-999-ZZ") ; j2.add_pair("year", 1899) ; v.push_back(j2) ; 
			json.add_pair("Cars", v) ; 
		}

		json.add_pair("The_JSON_itself", json) ;
		json.self_test_parser() ; 
		JsonSerializable json_inner ;
		json.get_value("The_JSON_itself", json_inner) ; 
		json_inner.self_test_parser() ; 
	}

	{
		const Enum_JsonSerializableFormat format (SF_TEXT) ;
		FileGuard filename(File::tempname("./file", "json")) ;

		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
			excerr << SimpleDebugSignatureThread << "Creating temporary file: \"" << filename << "\"  format: " << toStr(format) << endl ;
		ACS_CLASS_END_DEBUG

		const string blob ("{ \"Cars\":[{\"license\":\"AA-000-AA\",\"year\":2000},{\"license\":\"ZZ-999-ZZ\",\"year\":1899}], \"Company name\":null, \"Employee\":false, \"Home phones\":[\"555-1234\",\"666-7777\"],\"ID\":4242,\"Name\":\"Jason\",\"Work phones\":[],\"birthday\":{\"day\":31,\"month\":12,\"year\":1678},\"weight\":123.456}") ;
		JsonSerializable json(format) ; 
		json.unserialize(blob) ; 
		json.serializeToFile(filename) ; 
		self_test_from_file(filename, format) ;
	}

#ifndef ACS_USE_SIMPLIFIED_JSONOBJECT
	{
		const Enum_JsonSerializableFormat format (SF_HEXDUMP) ;
		FileGuard filename(File::tempname("./file", "bson_hex")) ;

		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
			excerr << SimpleDebugSignatureThread << "Creating temporary file: \"" << filename << "\"  format: " << toStr(format) << endl ;
		ACS_CLASS_END_DEBUG

		const string blob (	"140100000443617273005700000003300026000000026c6963656e7365000a00000041412d3030302d414100107965617200d00700000003310026000000026c" \
							"6963656e7365000a0000005a5a2d3939392d5a5a001079656172006b07000000000a436f6d70616e79206e616d650008456d706c6f796565000004486f6d6520" \
							"70686f6e65730025000000023000090000003535352d3132333400023100090000003636362d3737373700001049440092100000024e616d6500060000004a61" \
							"736f6e0004576f726b2070686f6e6573000500000000036269727468646179002300000010646179001f000000106d6f6e7468000c0000001079656172008e06" \
							"000000017765696768740077be9f1a2fdd5e4000" ) ; 
		JsonSerializable json(format) ; 
		json.unserialize(blob) ; 
		json.serializeToFile(filename, format) ; 
		self_test_from_file(filename, format) ;
	}

	{
		const Enum_JsonSerializableFormat format (SF_BINARY) ;
		FileGuard filename(File::tempname("./file", "bson")) ;

		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
			excerr << SimpleDebugSignatureThread << "Creating temporary file: \"" << filename << "\"  format: " << toStr(format) << endl ;
		ACS_CLASS_END_DEBUG

		ostringstream o ; 
		o	<<'\024'<<'\001'<<'\000'<<'\000'<<'\004'<<'\103'<<'\141'<<'\162'<<'\163'<<'\000'<<'\127'<<'\000'<<'\000'<<'\000'<<'\003'<<'\060' \
			<<'\000'<<'\046'<<'\000'<<'\000'<<'\000'<<'\002'<<'\154'<<'\151'<<'\143'<<'\145'<<'\156'<<'\163'<<'\145'<<'\000'<<'\012'<<'\000' \
			<<'\000'<<'\000'<<'\101'<<'\101'<<'\055'<<'\060'<<'\060'<<'\060'<<'\055'<<'\101'<<'\101'<<'\000'<<'\020'<<'\171'<<'\145'<<'\141' \
			<<'\162'<<'\000'<<'\320'<<'\007'<<'\000'<<'\000'<<'\000'<<'\003'<<'\061'<<'\000'<<'\046'<<'\000'<<'\000'<<'\000'<<'\002'<<'\154' \
			<<'\151'<<'\143'<<'\145'<<'\156'<<'\163'<<'\145'<<'\000'<<'\012'<<'\000'<<'\000'<<'\000'<<'\132'<<'\132'<<'\055'<<'\071'<<'\071' \
			<<'\071'<<'\055'<<'\132'<<'\132'<<'\000'<<'\020'<<'\171'<<'\145'<<'\141'<<'\162'<<'\000'<<'\153'<<'\007'<<'\000'<<'\000'<<'\000' \
			<<'\000'<<'\012'<<'\103'<<'\157'<<'\155'<<'\160'<<'\141'<<'\156'<<'\171'<<'\040'<<'\156'<<'\141'<<'\155'<<'\145'<<'\000'<<'\010' \
			<<'\105'<<'\155'<<'\160'<<'\154'<<'\157'<<'\171'<<'\145'<<'\145'<<'\000'<<'\000'<<'\004'<<'\110'<<'\157'<<'\155'<<'\145'<<'\040' \
			<<'\160'<<'\150'<<'\157'<<'\156'<<'\145'<<'\163'<<'\000'<<'\045'<<'\000'<<'\000'<<'\000'<<'\002'<<'\060'<<'\000'<<'\011'<<'\000' \
			<<'\000'<<'\000'<<'\065'<<'\065'<<'\065'<<'\055'<<'\061'<<'\062'<<'\063'<<'\064'<<'\000'<<'\002'<<'\061'<<'\000'<<'\011'<<'\000' \
			<<'\000'<<'\000'<<'\066'<<'\066'<<'\066'<<'\055'<<'\067'<<'\067'<<'\067'<<'\067'<<'\000'<<'\000'<<'\020'<<'\111'<<'\104'<<'\000' \
			<<'\222'<<'\020'<<'\000'<<'\000'<<'\002'<<'\116'<<'\141'<<'\155'<<'\145'<<'\000'<<'\006'<<'\000'<<'\000'<<'\000'<<'\112'<<'\141' \
			<<'\163'<<'\157'<<'\156'<<'\000'<<'\004'<<'\127'<<'\157'<<'\162'<<'\153'<<'\040'<<'\160'<<'\150'<<'\157'<<'\156'<<'\145'<<'\163' \
			<<'\000'<<'\005'<<'\000'<<'\000'<<'\000'<<'\000'<<'\003'<<'\142'<<'\151'<<'\162'<<'\164'<<'\150'<<'\144'<<'\141'<<'\171'<<'\000' \
			<<'\043'<<'\000'<<'\000'<<'\000'<<'\020'<<'\144'<<'\141'<<'\171'<<'\000'<<'\037'<<'\000'<<'\000'<<'\000'<<'\020'<<'\155'<<'\157' \
			<<'\156'<<'\164'<<'\150'<<'\000'<<'\014'<<'\000'<<'\000'<<'\000'<<'\020'<<'\171'<<'\145'<<'\141'<<'\162'<<'\000'<<'\216'<<'\006' \
			<<'\000'<<'\000'<<'\000'<<'\001'<<'\167'<<'\145'<<'\151'<<'\147'<<'\150'<<'\164'<<'\000'<<'\167'<<'\276'<<'\237'<<'\032'<<'\057' \
			<<'\335'<<'\136'<<'\100'<<'\000' ;

		const string blob (o.str()) ;	// Also includes any special character, like e.g. '\0' 
	
		JsonSerializable json(format) ; 
		json.unserialize(blob) ; 
		json.serializeToFile(filename, format) ; 
		self_test_from_file(filename, format) ;
	}
#endif
	
	{
		// Test the NESTED JSON serialization/deserialization
		// (A nested JSON is a JSON which can be stored in the string-value of another JSON)
		// It differs from a standard JSON argument.
		// Nested JSON: { "A": "{ \"K\": \"V\"}"}
		// Not-nested JSON: { "A": { "K": "V" } }
		JsonSerializable nested_json ;
		string nested_string ;
		{
			JsonSerializable json ;
			json.add_empty_value("Company name") ;
			json.add_pair("Name", "Jason") ;
			json.add_pair("Employee", false) ;
			{ vector <string> v ; v.push_back("555-1234") ; v.push_back("666-7777") ; json.add_pair("Home phones", v) ; }
			json.add_pair("ID", 4242) ;
			json.add_pair("weight", 123.456) ;
			{ vector <int> empty_v ; json.add_pair("Work phones", empty_v) ; }
			{	JsonSerializable bday ; bday.add_pair("day", 31) ; bday.add_pair("month", 12) ; bday.add_pair("year", 1678) ; 
				json.add_pair("birthday", bday) ; }
			{ 
				vector <JsonSerializable> v ;
				JsonSerializable j1 ; j1.add_pair("license", "AA-000-AA") ; j1.add_pair("year", 2000) ; v.push_back(j1) ; 
				JsonSerializable j2 ; j2.add_pair("license", "ZZ-999-ZZ") ; j2.add_pair("year", 1899) ; v.push_back(j2) ; 
				json.add_pair("Cars", v) ; 
			}

			json.add_pair("The_JSON_itself", json) ;
			
			// Create a NESTED JSON, where the value is a STRING, rather than a JSON (which is expected to be).
			// This can be useful in case of hardcoded interfaces, in which the arguments of a JSON 
			// CANNOT be another JSON, but only a string.
			//
			// Content of Nested_JSON:
			// { "Nested_JSON": "{\"Company name\":null,\"Employee\":false,\"Home phones\":[\"555-1234\",\"666-7777\"],\"ID\":4242,\"Name\":\"Jason\",\"The_JSON_itself\":{\"Company name\":null,\"Employee\":false,\"Home phones\":[\"555-1234\",\"666-7777\"],\"ID\":4242,\"Name\":\"Jason\",\"Work phones\":[],\"birthday\":{\"day\":31,\"month\":12,\"year\":1678},\"weight\":123.456},\"Work phones\":[],\"birthday\":{\"day\":31,\"month\":12,\"year\":1678},\"weight\":123.456}" }
			nested_json.add_pair("Nested_JSON", json.serialize_to_nested_string()) ;
		}
		
		nested_json.get_value("Nested_JSON", nested_string) ; 
		JsonSerializable json ; json.unserialize_from_nested_string(nested_string) ; 
		json.self_test_parser() ; 
		JsonSerializable json_inner ;
		json.get_value("The_JSON_itself", json_inner) ; 
		json_inner.self_test_parser() ; 
	}
	
}


/****************************************************************************
**		[STATIC] self_test_from_file
****************************************************************************/
void JsonSerializable::self_test_from_file(const std::string& filename, const Enum_JsonSerializableFormat &format)  
{
	/**
		Load a specific (previously exported) file, create the json object and parse it
	*/
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method called. Source filename: \"" << filename << "\"  format: " << toStr(format) << endl ;
	ACS_CLASS_END_DEBUG

	JsonSerializable json ; 
	json.unserializeFromFile(filename, format) ;	
	json.self_test_parser() ; 

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method finished" << endl ;
	ACS_CLASS_END_DEBUG
}

/****************************************************************************
**		self_test_parser
****************************************************************************/
void JsonSerializable::self_test_parser()  
{
	/**
		Parse a predefined filled object and check against a model
	*/

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method called." << endl ;
	ACS_CLASS_END_DEBUG

	bool verbose (false) ;
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << dump() << endl ;
		verbose = true ; 
	ACS_CLASS_END_DEBUG

	string key, key2 ; 
	string s ; size_t z ; double d ; int i ;  
	
	
	// Check values:
	key="Name" ;	ACS_COND_THROW ( (false == has_key(key)), exIllegalValueException("Key not found: \"" + key + "\"") ) ; 		
					ACS_COND_THROW ( (true == is_null(key)), exIllegalValueException("Value should not be null for key: \"" + key + "\"") ) ; 
					ACS_COND_THROW ( (false == is_string(key)), exIllegalValueException("Unexpected value type for: \"" + key + "\"") ) ; 	
					ACS_COND_THROW ( (string(get_value(key)) != "Jason"), exIllegalValueException("Unexpected value comparing: \"" + key + "\"") ) ; 
	if (verbose) { ostringstream o ; o << json_object_.at(key) ; excerr << "Successfull matching for: \"" << key << "\" = \"" << o.str() << "\"" << endl ; }
	key="ID" ;		ACS_COND_THROW ( (false == is_number(key)), exIllegalValueException("Unexpected value type for: \"" + key + "\"") ) ; 	
					ACS_COND_THROW ( ((get_value(key, z)) != 4242), exIllegalValueException("Unexpected value comparing: \"" + key + "\"") ) ;					
	if (verbose) { ostringstream o ; o << json_object_.at(key) ; excerr << "Successfull matching for: \"" << key << "\" = \"" << o.str() << "\"" << endl ; }
	key="weight" ;	ACS_COND_THROW ( (false == is_number(key)), exIllegalValueException("Unexpected value type for: \"" + key + "\"") ) ;
					ACS_COND_THROW ( ((get_value(key, d)) != 123.456), exIllegalValueException("Unexpected value comparing: \"" + key + "\"") ) ;
	if (verbose) { ostringstream o ; o << json_object_.at(key) ; excerr << "Successfull matching for: \"" << key << "\" = " << o.str() << endl ; }
	key="Employee" ;ACS_COND_THROW ( (false == is_boolean(key)), exIllegalValueException("Unexpected value type for: \"" + key + "\"") ) ;
					ACS_COND_THROW ( (get_value(key) != false), exIllegalValueException("Unexpected value comparing: \"" + key + "\"") ) ; 	
	if (verbose) { excerr << "Successfull matching for: \"" << key << "\" = " << (bool(json_object_.at(key))?"T":"F") << endl ; }
	key="Company name" ;ACS_COND_THROW ( (false == is_null(key)), exIllegalValueException("Unexpected value type for: \"" + key + "\"") ) ; 	
	if (verbose) { excerr << "Successfull matching for: \"" << key << "\" = null" << endl ; }
	key="Home phones" ; 
		ACS_COND_THROW ( (false == is_array(key)), exIllegalValueException("Unexpected value type for: \"" + key + "\"") ) ;
		ACS_COND_THROW ( (get_value(key).size() != 2), exIllegalValueException("Unexpected size comparing: \"" + key + "\"") ) ;
		ACS_COND_THROW ( (string(get_value(key).at(0)) != "555-1234"), exIllegalValueException("Unexpected value comparing: \"" + key + "\" at(0)") ) ;
		if (verbose) { ostringstream o ; o << json_object_.at(key).at(0) ; excerr << "Successfull matching for: \"" << key << "\"[0] = \"" << o.str() << "\"" << endl ; }
		ACS_COND_THROW ( (string(get_value(key).at(1)) != "666-7777"), exIllegalValueException("Unexpected value comparing: \"" + key + "\" at(1)") ) ;
		if (verbose) { ostringstream o ; o << json_object_.at(key).at(1) ; excerr << "Successfull matching for: \"" << key << "\"[1] = \"" << o.str() << "\"" << endl ; }
	key="Cars" ; 
		ACS_COND_THROW ( (false == is_array(key)), exIllegalValueException("Unexpected value type for: \"" + key + "\"") ) ;
		ACS_COND_THROW ( (get_value(key).size() != 2), exIllegalValueException("Unexpected size comparing: \"" + key + "\"") ) ;
		vector <JsonSerializable>  cars_V ; get_value(key, cars_V) ;
		cars_V.at(0).get_value("license", s) ; ACS_COND_THROW ( (s != "AA-000-AA"), exIllegalValueException("Unexpected value comparing: \"" + key + "\" at(0).license") ) ;
		if (verbose) { ostringstream o ; o << cars_V.at(0).get_value("license") ; excerr << "Successfull matching for: \"" << key << "\"[0].license = \"" << o.str() << "\"" << endl ; }
		cars_V.at(0).get_value("year", i) ; ACS_COND_THROW ( (i != 2000), exIllegalValueException("Unexpected value comparing: \"" + key + "\" at(0).year") ) ;
		if (verbose) { ostringstream o ; o << cars_V.at(0).get_value("year") ; excerr << "Successfull matching for: \"" << key << "\"[0].year = \"" << o.str() << "\"" << endl ; }
		cars_V.at(1).get_value("license", s) ; ACS_COND_THROW ( (s != "ZZ-999-ZZ"), exIllegalValueException("Unexpected value comparing: \"" + key + "\" at(1).license") ) ;
		if (verbose) { ostringstream o ; o << cars_V.at(1).get_value("license") ; excerr << "Successfull matching for: \"" << key << "\"[1].license = \"" << o.str() << "\"" << endl ; }
		cars_V.at(1).get_value("year", i) ; ACS_COND_THROW ( (i != 1899), exIllegalValueException("Unexpected value comparing: \"" + key + "\" at(1).year") ) ;
		if (verbose) { ostringstream o ; o << cars_V.at(1).get_value("year") ; excerr << "Successfull matching for: \"" << key << "\"[1].year = \"" << o.str() << "\"" << endl ; }
	key="Work phones" ; 
		ACS_COND_THROW ( (false == is_array(key)), exIllegalValueException("Unexpected value type for: \"" + key + "\"") ) ;
 		ACS_COND_THROW ( (json_object_.at(key).size() != 0), exIllegalValueException("Unexpected size comparing: \"" + key + "\"") ) ;
		if (verbose) { excerr << "Successfull matching for: \"" << key << "\" (empty vector)" << endl ; }
	key="birthday" ; 
	key2 = "day" ;  ACS_COND_THROW ( (false == has_key(key, key2)), exIllegalValueException("Key not found: \"" + key +"."+key2 + "\"") ) ;	
					ACS_COND_THROW ( (false == is_number(key, key2)), exIllegalValueException("Unexpected value type for: \"" + key +"."+key2 + "\"") ) ;	
					ACS_COND_THROW ( ((get_value(key, key2, z)) != 31), exIllegalValueException("Unexpected value comparing: \"" + key +"."+key2 + "\"") ) ; 
	if (verbose) { ostringstream o ; o << get_value(key).at(key2) ; excerr << "Successfull matching for: \"" << key << "\"\"" << key2 << "\" = " << o.str() << endl ; }
	key2 = "month" ;ACS_COND_THROW ( (false == is_number(key, key2)), exIllegalValueException("Unexpected value type for: \"" + key +"."+key2 + "\"") ) ; 
					ACS_COND_THROW ( ((get_value(key, key2, z)) != 12), exIllegalValueException("Unexpected value comparing: \"" + key +"."+key2 + "\"") ) ; 
	if (verbose) { ostringstream o ; o << get_value(key).at(key2) ; excerr << "Successfull matching for: \"" << key << "\"\"" << key2 << "\" = " << o.str() << endl ; }
	key2 = "year" ;	ACS_COND_THROW ( (false == is_number(key, key2)), exIllegalValueException("Unexpected value type for: \"" + key +"."+key2 + "\"") ) ; 	
					ACS_COND_THROW ( ((get_value(key, key2, z)) != 1678), exIllegalValueException("Unexpected value comparing: \"" + key +"."+key2 + "\"") ) ; 
	if (verbose) { ostringstream o ; o << get_value(key).at(key2) ; excerr << "Successfull matching for: \"" << key << "\"\"" << key2 << "\" = " << o.str() << endl ; }

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method finished successfully." << endl ;
	ACS_CLASS_END_DEBUG
}


/****************************************************************************
**		[STATIC] toHexDigits
****************************************************************************/
string JsonSerializable::toHexDigits(const vector<uint8_t>& v) 
{
	/**
		From a sequence of uint8_t to a string of hex-digits. E.G. { 0x00, 0x12, 0xff } -> string("0001ff") [ from 3 bytes to 6 chars ]
	*/
	ostringstream o ; 
	for (size_t i=0; i<v.size(); i++) { o << hex << setw(2) << setfill('0') << static_cast<int> (v.at(i)) << dec; }
	return o.str() ; 
}


/****************************************************************************
**		[STATIC] fromHexDigits
****************************************************************************/
vector<uint8_t> JsonSerializable::fromHexDigits(const string& s)
{
	/**
		From a string of hex-digits to a sequence of uint8_t. E.G. string("fe4091") -> { 0xfe, 0x40, 0x91 } [ from 6 chars to 3 bytes ]
	*/
	const size_t s_len((s.length()/2)*2) ;	// Force length to be even
	vector<uint8_t> v ;
	stringstream ss; 
	size_t l, h ; 
	for (size_t i=0; i<s_len; i+=2) { 
		ss.clear() ; ss << hex << s.substr(i,1) ; ss >> h ; 
		ss.clear() ; ss << hex << s.substr((i+1),1) ; ss >> l ; // lowercase-L (not: one)
		v.push_back( (h << 4)|l ) ; // lowercase-L (not: one)
	}

	return v ; 
}


/****************************************************************************
**		[STATIC] toBinary
****************************************************************************/
string JsonSerializable::toBinary(const vector<uint8_t>& v) 
{
	/**
		From a sequence of uint8_t to a binary string (blob).  E.G. { 0x00, 0x12, 0xff } -> string("\x00\x12\xff") [ 3 bytes ]
	*/
	string s ; char c ; 
	for (size_t i=0; i<v.size(); i++) { c = ('\0' + v.at(i)) ; s += c ; }
	return s ; 
}


/****************************************************************************
**		[STATIC] fromBinary
****************************************************************************/
vector<uint8_t> JsonSerializable::fromBinary(const string& s)
{
	/**
		From binary string (blob) to a sequence of uint8_t. E.G. string("\xfe\x40\x91") -> { 0xfe, 0x40, 0x91 } [ 3 bytes ] 
	*/
	const size_t s_len(s.length()) ;	
	
	vector<uint8_t> v ;
	for (size_t i=0; i<s_len; i+=1) { 
		v.push_back( s[i] ) ; 
	}

	return v ; 
}


/****************************************************************************
**		[STATIC] toStr
****************************************************************************/
string JsonSerializable::toStr(const Enum_JsonSerializableFormat& e)
{
	string s ;
	if		(e == SF_TEXT)		{ s = "TEXT" ; }
	else if (e == SF_HEXDUMP)	{ s = "HEXDUMP" ; }
	else if (e == SF_BINARY)	{ s = "BINARY" ; }
	else						{ s = "UNKNOWN" ; }
	
	return s ;
}	


/****************************************************************************
**		[STATIC] toJsonSerializableFormat
****************************************************************************/
JsonSerializable::Enum_JsonSerializableFormat JsonSerializable::toJsonSerializableFormat(const string& s)
{	
	Enum_JsonSerializableFormat e ;
	if		("TEXT"		== StringUtils::uppercase(s))	{ e = SF_TEXT ; }
	else if ("HEXDUMP"	== StringUtils::uppercase(s))	{ e = SF_HEXDUMP ; }
	else if ("BINARY"	== StringUtils::uppercase(s))	{ e = SF_BINARY ; }
	else												{ e = SF_UNKNOWN ; } 

	return e ;
}	


/****************************************************************************
**		[STATIC] hex_dump
****************************************************************************/
string JsonSerializable::hex_dump(const unsigned char* ptr, const off64_t& sz) 
{
	ostringstream o ; 
	for (off64_t i=0; i<sz; i++) { o << hex << setw(2) << setfill('0') << static_cast<int>(ptr[i]) << dec << " " ; }
	return o.str() ; 
} 


_ACS_END_NAMESPACE
