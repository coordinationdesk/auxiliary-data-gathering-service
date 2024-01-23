/*
 
    Copyright 2019-2021, Advanced Computer Systems , Inc.
    Via Della Bufalotta, 378 - 00139 Roma - Italy
    http://www.acsys.it

    All Rights Reserved.

    This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
    the contents of this file may not be disclosed to third parties, copied or
    duplicated in any form, in whole or in part, without the prior written
    permission of Advanced Computer Systems, Inc.

    $Prod: A.C.S. ACSJsonObject header file $

    $Log$

*/

/**
	ENRCAR, November 2019.
	This is a minimalistic json module which is able to compile with older C++ compilers (pre C++11).
	It is required since library nlohmann needs at least C++11
	The module is NOT able to handle any format different by TEXT (I will add MIME64, probably).
	Anyway it's able to manage all the types expected in JSONs (STRING, NUMBER, BOOL, NULL)
	and the complex types (ARRAY and JSON itself), also recursively.
	Example: A JSON containing an array of JSONs ... and so on.
*/

#ifndef _ACSJsonObject_H_
#define _ACSJsonObject_H_

#include <acs_c++config.hpp>
#include <exException.h>


_ACS_BEGIN_NAMESPACE(acs)

exDECLARE_EXCEPTION(ACSJsonObjectException, exException);

class ACSJsonObject {

	enum Enum_JsonType { JT_STRING=0, JT_NUMBER=1, JT_ARRAY=2, JT_BOOL=3, JT_NULL=4, JT_JSON=5, JT_UNKNOWN=6 } ; 

	public:
		ACSJsonObject(const Enum_JsonType& t=JT_JSON) ;
		ACSJsonObject(const std::string& item) ;
		virtual ~ACSJsonObject() ACS_NOEXCEPT ;
	
		void reset(const Enum_JsonType& t=JT_JSON) { items_V_.clear() ; item_="" ; item_type_=t ; }

		void set_object(const std::string& item) ;
		inline void set_object(const std::vector<std::string> &item_V) {
			std::ostringstream o ; o << "[" ;	
			for (size_t i=0;i<item_V.size();i++) { o << ((i!=0)?",":"") ; o << "\"" << item_V.at(i) << "\"" ; }
			o << "]" ; set_object(o.str()) ;
		}
		template<typename T>
			void set_object(const std::vector<T> &item_V) {
				std::ostringstream o ; o << "[" ;	
				for (size_t i=0;i<item_V.size();i++) { o << ((i!=0)?",":"") ; o << item_V.at(i) ; }
				o << "]" ; set_object(o.str()) ;
			}
	
		std::string dump(const std::string& t="") const { return dump (*this, t) ; }
		std::string toStr() const { const bool rec (false) ; return toStr (*this, rec) ; }
		inline size_t size() const { return items_V_.size() ; } 

		void add_empty_value (const std::string& key) { items_V_[key] = ACSJsonObject(JT_NULL) ; }
		void add_pair (const std::string& key, const ACSJsonObject& obj) { items_V_[key] = obj ; }
		void add_pair (const std::string& key, const char* value_ptr) { add_pair (key, std::string(value_ptr)) ; }
		void add_pair (const std::string& key, const std::string& value) { items_V_[key] = ACSJsonObject("\""+value+"\"") ; }
		void add_pair (const std::string& key, const bool& value) { items_V_[key] = ACSJsonObject(value?"true":"false") ; }
		template<typename T>
		void add_pair (const std::string& key, const T& value) { 
			std::ostringstream v ; v << value ; ACSJsonObject obj(v.str()) ; 
			items_V_[key] = obj ;
		}
		template<typename T>
		void add_pair(const std::string& key, const std::vector<T> &item_V) 
		{
			ACSJsonObject obj ; obj.set_object(item_V) ;
			items_V_[key] = obj ;
		}

		inline bool has_key(const std::string& key) const		{ return (items_V_.find(key) != items_V_.end()) ; }
		inline bool has_key(const size_t& n) const				{ const std::string key (pad8(n)) ; return has_key (key) ; }	/* Used for vectors */
		const ACSJsonObject& at(const std::string& key) const	{ return items_V_.at(key) ; }
		const ACSJsonObject& at(const size_t& n) const			{ return items_V_.at(pad8(n)) ; }								/* Used for vectors */

		ACSJsonObject& operator[] (const std::string& key)		{ return items_V_[key] ; }
		ACSJsonObject& operator[] (const size_t& n)				{ return items_V_[pad8(n)] ; }

		bool is_null() const	{ return (JT_NULL == item_type_) ; }
		bool is_boolean() const { return (JT_BOOL == item_type_) ; }
		bool is_number() const	{ return (JT_NUMBER == item_type_) ; }
		bool is_array() const	{ return (JT_ARRAY == item_type_) ; }
		bool is_string() const	{ return (JT_STRING == item_type_) ; }
		bool is_object() const	{ return (JT_JSON == item_type_) ; }

		/* Cast to type -- type must be specified */
		std::string& get_value(std::string& value) const { value = toStr() ; return value ; }
		ACSJsonObject& get_value(ACSJsonObject& value) const { value = (*this) ; return value ; }
		bool& get_value(bool& value) const { value = ((true == is_boolean()) && ("true" == toStr()))? true : false ; return value ; }
		template<typename T>
			T& get_value (T& value) const { 
				std::istringstream ss(toStr()); ss >> value ; 
				return value ; 
			}

		std::vector <ACSJsonObject>& get_value (std::vector <ACSJsonObject>& values_V) const { 
			values_V.clear() ; for (std::map <std::string, ACSJsonObject>::const_iterator it=items_V_.begin(); it!=items_V_.end(); it++)
			{ values_V.push_back(it->second) ; } ; return values_V ; 
		}

		template<typename T>
			std::vector <T>& get_value (std::vector <T>& values_V) const { 
				values_V.clear() ; for (std::map <std::string, ACSJsonObject>::const_iterator it=items_V_.begin(); it!=items_V_.end(); it++)
				{ T tmp ; std::istringstream ss(it->second.toStr()); ss >> tmp ; values_V.push_back(tmp) ; }
				return values_V ; 
			}

		/* Implicit cast operator */
		operator std::string() const { return toStr() ; }
		operator bool() const { return ( ((true == is_boolean()) && ("true" == toStr()))? true : false ) ; }

	private:
		inline std::map <std::string, ACSJsonObject> get_items_V() const { return items_V_ ; } 
		inline std::string get_item() const { return item_ ; } 
		inline Enum_JsonType get_item_type() const { return item_type_ ; } 

		static std::string dump(const ACSJsonObject& value, const std::string& t="") ;
		static std::string toStr(const ACSJsonObject& value, const bool recursive) ;
		static std::vector <std::string> tokenize(const std::string& item, ssize_t in_bracket, ssize_t in_array, const char sep) ;
		static std::pair<std::string, std::string> tokenize_keyval(const std::string& item) ;
		static std::vector <std::pair <std::string,std::string> > tokenize_json(const std::string& item) ;
		static std::vector <std::string> tokenize_array(const std::string& item) ;

		static Enum_JsonType type_from_value(const std::string& item) ;
		static std::string refine_simple_value(const std::string& item, const Enum_JsonType& item_type) ;
		static std::string toStr(const Enum_JsonType& e) ;
		static std::string pad8(const size_t& n) ;
		static size_t unpad(const std::string& s) ;

	private:
		std::map <std::string, ACSJsonObject> items_V_ ; 	/* Aggregate values: JSON and ARRAYS. Key is used to force alphab. sorting. For array key is: "00..0", "00..1" and so on */
		std::string item_ ;									/* Single, not-composite values */
		Enum_JsonType item_type_ ;

		ACS_CLASS_DECLARE_DEBUG_LEVEL(ACSJsonObject);

} ;

inline std::ostream & operator << (std::ostream &s, const ACSJsonObject &f)	{ s << f.toStr() ; return s ; }
inline exostream & operator << (exostream &s, const ACSJsonObject &f)		{ s << f.toStr() ; return s ; }


_ACS_END_NAMESPACE

#endif /* _ACSJsonObject_H_ */
