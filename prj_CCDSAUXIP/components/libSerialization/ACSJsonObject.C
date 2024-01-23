/*
 
    Copyright 2019-, Advanced Computer Systems , Inc.
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.acsys.it

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

    $Prod: A.C.S. ACSJsonObject $

    $Log$

*/

#include <ACSJsonObject.h>
#include <File.h>
#include <StringUtils.h>

using namespace std;

_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(ACSJsonObject);

ACSJsonObject::ACSJsonObject(const Enum_JsonType& item_type) :
items_V_(),
item_(),
item_type_(item_type)
{
	// Create an empty object. Default type is JSON
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method called." << endl ;
	ACS_CLASS_END_DEBUG
}

ACSJsonObject::ACSJsonObject(const string& item) :
items_V_(),
item_(),
item_type_(ACSJsonObject::JT_UNKNOWN)
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method called." << endl ;
	ACS_CLASS_END_DEBUG

	set_object(item) ;
}

ACSJsonObject::~ACSJsonObject() ACS_NOEXCEPT
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method called." << endl ;
	ACS_CLASS_END_DEBUG
}


void ACSJsonObject::set_object(const string& item) 
{
	items_V_.clear() ;
	item_ = "" ;
	item_type_ = type_from_value (item) ; // Type is inferred from the item itself

	ACS_COND_THROW( (JT_UNKNOWN == item_type_), exIllegalValueException("Unknown type for item: \"" + item + "\"") ) ;

	if ( JT_JSON == item_type_ )
	{
		const vector < pair<string, string> > v ( tokenize_json(item) ) ;
		for (size_t i=0; i<v.size(); i++)
		{		
			const string key ( StringUtils::trim(StringUtils::trim (v.at(i).first), '\"') ) ;
			const string value ( StringUtils::trim (v.at(i).second) ) ;

			// Generate a ACSJsonObject from the value -- it can be either a simple or aggregate type !
			ACSJsonObject json_object(value) ;	
			
			// Add the object to the map
			items_V_[key] = json_object ;
		}
	}
	else if ( JT_ARRAY == item_type_ )
	{
		const vector <string> v ( tokenize_array(item) ) ;
		for (size_t i=0; i<v.size(); i++)
		{
			const string key ( pad8(i) ) ; // A fake sorted key is used. For example: "00000000", "00000001" and do on
 			const string value ( StringUtils::trim (v.at(i)) ) ;
			
			// Generate a ACSJsonObject from the array value -- it can be either a simple or aggregate type !
			ACSJsonObject json_object(value) ;	
			
			// Add the object to the map
			items_V_[key] = json_object ;
		}
	}
	else 
	{
		// simple (not-composite) type: just use the refined value:
	
		// Refine simple values (not composite ones, like arrays or json) by stripping spaces, upcase for bool, useless quotes and so on.
		item_ = refine_simple_value(item, item_type_) ;
	}
}


string ACSJsonObject::refine_simple_value(const string& value_in, const Enum_JsonType& item_type) 
{
	/** Refine simple values (not composite ones, like arrays or json) by stripping spaces, upcase for bool, useless quotes and so on. */

	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB) 
		excerr << SimpleDebugSignatureThread << "Method called. value_in: \"" << value_in << "\" item_type: " << toStr(item_type) << endl ;
	ACS_CLASS_END_DEBUG

	if ( (JT_ARRAY == item_type) || (JT_JSON == item_type) || (JT_UNKNOWN == item_type) )
	{ ACS_THROW( exIllegalValueException("Unsupported item type: " + toStr(item_type))) ; }	

	const string value (StringUtils::trim(value_in)) ;
	const string lw_value (StringUtils::lowercase(value)) ;
	string value_out ;

	if ( JT_NULL == item_type) { value_out = "null" ; }
	else if ( JT_BOOL == item_type) 
	{ 
		if (value.empty()) { value_out = "false" ; }
		else if ('t' == lw_value.at(0)) { value_out = "true" ; }
		else { value_out = "false" ; }
	}
	else if ( JT_STRING == item_type) 
	{ value_out = StringUtils::trim(value, '\"') ; }
	else 
	{ value_out = value ; }

	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB) 
		excerr << SimpleDebugSignatureThread << "Method finished. Returning: \"" << value_out << "\"" << endl ;
	ACS_CLASS_END_DEBUG

	return value_out ;
}


vector <string> ACSJsonObject::tokenize(const string& item, ssize_t in_bracket, ssize_t in_array, const char sep) 
{
	vector <string> tokenized_V ;
	
	bool in_string (false) ; 
	string s ; 
	for (size_t i=0; i<item.length(); i++)
	{
		const bool is_last_char ( (i+1) >= item.length() ) ; 
	
		const char c (item.at(i)) ; 
		if ('{' == c) { in_bracket ++ ; }
		else if ('}' == c) { in_bracket -- ; }
		else if ('[' == c) { in_array ++ ; }
		else if (']' == c) { in_array -- ; }
		else if ('\"' == c) { in_string = (not in_string) ; /* toggle */ }

		/* The spaces/tab are only useful when quoted (i.e. "inside a string"). Otherwise are useless and can be just ignored */
		if ((false == is_last_char) && (false == in_string) && ((' '==c)||('\t'==c)||('\n'==c)||('\r'==c))) { continue ; }

		/* Process the content, unless already inside an open blockof any kind, in order to tokenize */
		if ( ((0 == in_bracket) && (0 == in_array) && (false == in_string)) || (true == is_last_char) )
		{
			if ((c == sep) || (true == is_last_char)) 
			{ 
				if (c != sep) { if ( !(((in_bracket < 0) && ('}' == c)) || ((in_array < 0) && (']' == c))) ) { s += c ; } }
				s = StringUtils::trim(s) ; if (false == s.empty()) { tokenized_V.push_back( s ) ; } 
				s = "" ; 
			} 
			else { 
				if ( !(((in_bracket <= 0) && ('{' == c)) || ((in_array <= 0) && ('[' == c))) ) { s += c ; } 
			}
		}
		else
		{ s += c ; }
	}

//	for (size_t i=0; i<tokenized_V.size(); i++) { excerr << "tokenized_V [" << i << "]: !" << tokenized_V.at(i) << "!" << endl ; }

	return tokenized_V ;
}

vector < pair<string, string> > ACSJsonObject::tokenize_json(const string& item)
{
	const size_t in_bracket(-1) ; 
	const size_t in_array(0) ;
	const char sep(',') ;  

	// Disassemble the json a vector of entries (like: [ K1:V1, K2:V2 ...] and so on)
	const vector <string> v1 ( tokenize(item, in_bracket, in_array, sep) ) ;

	// Disassemble each entry, which is a key/value pair, regardless the complexity of the value (it is still kept as string)
	vector <pair <string, string> > v2 ;
	for (size_t i=0; i<v1.size(); i++) { v2.push_back ( tokenize_keyval(v1.at(i)) ) ; }	

	return v2 ; 
} 

vector <string> ACSJsonObject::tokenize_array(const string& item)
{
	const size_t in_bracket(0) ; 
	const size_t in_array(-1) ;
	const char sep(',') ;  

	return tokenize(item, in_bracket, in_array, sep) ; 

} 

pair <string, string> ACSJsonObject::tokenize_keyval(const string& item)
{
	const size_t in_bracket(0) ; 
	const size_t in_array(0) ;
	const char sep(':') ;  

	const vector <string> v ( tokenize(item, in_bracket, in_array, sep) ) ; 
	ACS_COND_THROW ( (2!=v.size()), exIllegalValueException("Internal error: not a key/value pair") ) ; 
	return pair<string, string> ( v.at(0), v.at(1) ) ; 
} 


ACSJsonObject::Enum_JsonType ACSJsonObject::type_from_value(const string& value_in)
{
	const string value ( StringUtils::trim(value_in) ) ;
	const string lw_value ( StringUtils::lowercase(value) ) ;
	Enum_JsonType type ( JT_UNKNOWN ) ;
	
	if ( (true == lw_value.empty()) || ("null" == lw_value) ) 
	{ type = JT_NULL ; } 
	else if ( ("true" == lw_value) || ("false" == lw_value) ) 
	{ type = JT_BOOL ; } 
	else if (lw_value.at(0) == '{') 
	{ type = JT_JSON ; } 
	else if (lw_value.at(0) == '[') 
	{ type = JT_ARRAY ; } 
	else if (lw_value.at(0) == '\"') 
	{ type = JT_STRING ; } 
	else
	{ type = JT_NUMBER ; }
	
	return type ; 
}

string ACSJsonObject::toStr(const Enum_JsonType& e)
{
	if		(e == JT_STRING)	return "STRING" ;
	else if (e == JT_NUMBER)	return "NUMBER" ;
	else if (e == JT_ARRAY)		return "ARRAY" ;
	else if (e == JT_BOOL)		return "BOOL" ;
	else if (e == JT_NULL)		return "NULL" ;  
	else if (e == JT_JSON)		return "JSON" ; 
	return "UNKNOWN" ;
}


string ACSJsonObject::pad8(const size_t& n)
{
	/** Create a string key given the index of a vector. The vector items will be saved in the items_V_ map */

    ostringstream o ; o << n ;
    const string s ( "0000000000000000" + o.str() ) ;
    return s.substr(s.length()-8) ;
}

size_t ACSJsonObject::unpad(const string& s)
{
    size_t n(0) ;
    if (false == s.empty()) { istringstream iss (s); iss >> n; }
    return n ;
}


string ACSJsonObject::dump(const ACSJsonObject& value, const string& t)  
{
	const Enum_JsonType item_type ( value.get_item_type() ) ;
	ostringstream value_out ;

	if ( JT_NULL == item_type) { value_out << "null" ; }
	else if ( JT_STRING == item_type) { value_out << ("\"" + value.get_item() + "\"") ; }
	else if ( JT_JSON == item_type )
	{
		const map <string, ACSJsonObject> items_V ( value.get_items_V() ) ;
		value_out << "<JSON of size: " << items_V.size() << ">" ;
		if (items_V.size() > 0) {
			value_out << "\n" << t << "{" ;
			for (map <string, ACSJsonObject>::const_iterator it=items_V.begin() ; it!=items_V.end() ; it ++)
			{ 
				if (it != items_V.begin()) { value_out << "," ; } value_out << "\n" ; 
				value_out << t << "\t'" << it->first << "': " << dump(it->second, (t+"\t")) ; 
			}
			value_out << "\n" << t << "}" ;
		}
	}
	else if ( JT_ARRAY == item_type )
	{		
		const map <string, ACSJsonObject> items_V ( value.get_items_V() ) ;
		value_out << "<ARRAY of size: " << items_V.size() << ">" ; 
		if (items_V.size() > 0) {
			value_out << "\n" << t << "[" ;
			for (map <string, ACSJsonObject>::const_iterator it=items_V.begin() ; it!=items_V.end() ; it ++)
			{ 
				if (it != items_V.begin()) { value_out << "," ; } value_out << "\n" ; 
				value_out << t << "\t" << dump(it->second, (t+"\t")) ; 
			}
			value_out << "\n" << t << "]" ;
		}
	}
	else
	{ value_out << value.get_item() ; }

	return value_out.str() ;
}

string ACSJsonObject::toStr(const ACSJsonObject& value, const bool recursive)  
{
	const Enum_JsonType item_type ( value.get_item_type() ) ;
	ostringstream value_out ;

	if ( JT_NULL == item_type) { value_out << "null" ; }
	else if ( JT_STRING == item_type) { 
		value_out << ((true == recursive)? ("\"" + value.get_item() + "\"") : value.get_item() ) ;
	} 
	else if ( JT_JSON == item_type )
	{
		const map <string, ACSJsonObject> items_V ( value.get_items_V() ) ;
		value_out << "{" ;
		for (map <string, ACSJsonObject>::const_iterator it=items_V.begin() ; it!=items_V.end() ; it ++)
		{ if (it != items_V.begin()) { value_out << "," ; } value_out << "\"" << it->first << "\":" << toStr(it->second, true) ; }
		value_out << "}" ;
	}
	else if ( JT_ARRAY == item_type )
	{		
		const map <string, ACSJsonObject> items_V ( value.get_items_V() ) ;
		value_out << "[" ;
		for (map <string, ACSJsonObject>::const_iterator it=items_V.begin() ; it!=items_V.end() ; it ++)
		{  if (it != items_V.begin()) { value_out << "," ; } value_out << toStr(it->second, true) ; }
		value_out << "]" ;
	}
	else
	{ value_out << value.get_item() ; }

	return value_out.str() ;
}


_ACS_END_NAMESPACE
