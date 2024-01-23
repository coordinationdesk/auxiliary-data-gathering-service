// PRQA S 1050 EOF
/*

	Copyright 2002-2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. XMLProcessor Module $

	$Id$

	$Author$

	$Log$
	Revision 5.9  2016/12/16 13:04:24  enrcar
	EC:: Empty files handled in case of INPUT tag
	
	Revision 5.8  2016/12/01 15:15:13  danalt
	ACS_THROW and ACS_COND_THROW macros adopted

	Revision 5.7  2016/11/09 15:21:27  enrcar
	EC:: ownership of FileGuard lost due to poor typization of make_pair. Replaced with pair.
	
	Revision 5.6  2016/11/07 13:35:18  enrcar
	EC::nodeMatch/TAG_CMD__CALL_FUNCTION  modified in order to handle an unspecified root
	
	Revision 5.5  2016/10/28 14:35:25  enrcar
	EC:: Method to handle a custom header added
	
	Revision 5.4  2016/10/25 12:59:31  enrcar
	EC:: In case of problems calling a function, the temporary files won't to be removed to allow investigations
	
	Revision 5.3  2016/10/19 15:45:59  ricfer
	EC:: typo fixed
	
	Revision 5.2  2016/10/14 14:29:55  enrcar
	EC:: CALL_FUNCTION modified in order to handle the clone_root_tag also on the file created by the function
	
	Revision 5.1  2016/09/27 12:27:50  enrcar
	EC:: Imported module XMLProcessor
	
	
*/

#include <XMLProcessor.h>
#include <XMLExtractorHandlerBase.h>
#include <StringUtils.h>
#include <RegExp.h>
#include <Filterables.h>
#include <SpawnProc.h>
#include <File.h>
#include <FileGuard.h>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <limits>

_ACS_BEGIN_NAMESPACE(acs)

using namespace std ;


////////////////////////////////////////////////////////////////////////////////////////
//		apply 		
////////////////////////////////////////////////////////////////////////////////////////
string LUT_struct::apply(const string& key, bool& lut_is_triggered) const 
{
/*
	The LUT-Table (i.e. a vector of one or more regexp_key -> value transformations) is applied to an input key
	The output is:
	- The original value (with the bool lut_is_triggered UNSET) if the key did not match any of regexp
	- The new value (with the bool lut_is_triggered SET) if the key did matched one of the regexp
	
	Example:
	LUT: { { "^A$" -> "B" } , { "0" -> "ZERO" } }
		Input #1 : "A"  ->  returned value is "B" , lut_is_triggered = true
		Input #2 : "Alpha"  ->  returned value is "Alpha" , lut_is_triggered = false
*/

	string value_output (key) ; 
	lut_is_triggered = false ;
	for (size_t i=0; i<_lut_V.size(); i++)
	{
		RegExp regexp ( _lut_V.at(i).first );
		if ( regexp.match( key ) )
		{
			value_output = _lut_V.at(i).second ;
			lut_is_triggered = true ;
			break ; 
		} 
	}

	return value_output ;
}


////////////////////////////////////////////////////////////////////////////////////////
//		dump		
////////////////////////////////////////////////////////////////////////////////////////
string LUT_struct::dump(const string& tab) const
{
	ostringstream o ;
	o << tab << "\tEntries: (" << _lut_V.size() << ")" << endl; 
	for (size_t i=0; i<_lut_V.size(); i++)
	{ o << tab << "\t" << (i+1) << ".\tRegexp: \"" << _lut_V.at(i).first << "\"\t->\tValue: \"" << _lut_V.at(i).second << "\"" << endl ; }

	return o.str() ;
}

		
////////////////////////////////////////////////////////////////////////////////////////
//		STATIC SUBST_instance::SUBST_Type_to_String	
////////////////////////////////////////////////////////////////////////////////////////
string SUBST_instance::SUBST_Type_to_String(enum SUBST_instance::SUBST_Type e) 
{
	string s ; 
	
	if (e == SUBST_LUT)		{ s = "SUBST_LUT" ; } 
	else if (e == SUBST_DROP)	{ s = "SUBST_DROP" ; } 
	else if (e == SUBST_ADD)	{ s = "SUBST_ADD" ; } 
	else if (e == SUBST_FUNCT)	{ s = "SUBST_FUNCT" ; } 
	else if (e == SUBST_CMD)	{ s = "SUBST_CMD" ; } 
	else if (e == SUBST_NONE)	{ s = "SUBST_NONE" ; } 
	else { s = "SUBST_UNDEFINED" ; } 
	
	return s ;
}


////////////////////////////////////////////////////////////////////////////////////////
//		STATIC String_to_SUBST_Type	
////////////////////////////////////////////////////////////////////////////////////////
SUBST_instance::SUBST_Type SUBST_instance::String_to_SUBST_Type(const string& s) 
{
	enum SUBST_instance::SUBST_Type e (SUBST_UNDEFINED) ; 

	if (s == "SUBST_LUT")			{ e = SUBST_LUT ; } 	  
	else if (s == "SUBST_DROP")		{ e = SUBST_DROP ; }	  
	else if (s == "SUBST_ADD")		{ e = SUBST_ADD ; } 
	else if (s == "SUBST_FUNCT")	{ e = SUBST_FUNCT ; }	  
	else if (s == "SUBST_CMD")		{ e = SUBST_CMD ; }	  
	else if (s == "SUBST_NONE")		{ e = SUBST_NONE ; }	  
	else { e = SUBST_UNDEFINED ; } 

	return e ;
}


//**************************************************************************************
//									
//   struct SUBST_instance		
//									
//**************************************************************************************
ACS_CLASS_DEFINE_DEBUG_LEVEL(SUBST_instance)


////////////////////////////////////////////////////////////////////////////////////////
//		STATIC toString		
////////////////////////////////////////////////////////////////////////////////////////
string SUBST_instance::Match_Element_Rule_to_String(enum Match_Element_Rule e) 
{
	string s ; 
	
	if (e & TAG_NAME)		{ s += (!s.empty()? "  " : "") + string("TAG_NAME") ; } 
	if (e & TAG_VALUE)		{ s += (!s.empty()? "  " : "") + string("TAG_VALUE") ; } 
	if (e & TAG_ATTR_NAME)	{ s += (!s.empty()? "  " : "") + string("TAG_ATTR_NAME") ; } 
	if (e & TAG_ATTR_VALUE)	{ s += (!s.empty()? "  " : "") + string("TAG_ATTR_VALUE") ; } 
	
	if (e == TAG_UNDEFINED) { s = "TAG_UNDEFINED" ; } 
	
	if (s.empty()) { s = "TAG_UNKNOWN" ; }

	return s ;
}


////////////////////////////////////////////////////////////////////////////////////////
//		STATIC String_to_Match_Element_Rule		
////////////////////////////////////////////////////////////////////////////////////////
SUBST_instance::Match_Element_Rule SUBST_instance::String_to_Match_Element_Rule(const string& s) 
{
	enum Match_Element_Rule e (TAG_UNDEFINED) ; 

	if (s == "TAG_NAME")	{ e = TAG_NAME ; } 	  
	else if (s == "TAG_VALUE")	{ e = TAG_VALUE ; }	  
	else if (s == "TAG_ATTR_NAME")	{ e = TAG_ATTR_NAME ; } 
	else if (s == "TAG_ATTR_VALUE")	{ e = TAG_ATTR_VALUE ; }
	else { e = TAG_UNDEFINED ; } 

	return e ;
}


////////////////////////////////////////////////////////////////////////////////////////
//		dump		
////////////////////////////////////////////////////////////////////////////////////////
string SUBST_instance::dump(const string& tab) const
{
	ostringstream o ;
	
	o << tab << "Type: " << SUBST_instance::SUBST_Type_to_String(_type) ;
	if (_type != SUBST_DROP) { o << "\tId: \"" << _id << "\"" ; }
	o << "\tOpts:" ; for (size_t i=0; i<_opts_V.size(); i++) { o << "\t\"" << _opts_V.at(i) << "\"" ; } 
	o << endl << tab << "\tMatch element rule of the instance: [" << Match_Element_Rule_to_String(_match_element_rule) << "]  " ;
	if (_match_element_rule != TAG_UNDEFINED) { o << "\"" << _match_element << "\"" ; } 

	return o.str() ;
}


//**************************************************************************************
//									
//   struct xml_tag_struct 		
//									
//**************************************************************************************
ACS_CLASS_DEFINE_DEBUG_LEVEL(xml_tag_struct)


////////////////////////////////////////////////////////////////////////////////////////
//		xml_tag_struct 		
////////////////////////////////////////////////////////////////////////////////////////
xml_tag_struct::xml_tag_struct() : _index(0), _name(), _value(), _attributes(), _containing(), _parent(0) {

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream msg; 
		msg << exDebugShortSignatureFile << "C'tor called. ptr: " << this ;
		ACS_LOG_DEBUG(msg.str());
	ACS_CLASS_END_DEBUG
}


////////////////////////////////////////////////////////////////////////////////////////
//		~xml_tag_struct 		
////////////////////////////////////////////////////////////////////////////////////////
xml_tag_struct::~xml_tag_struct() throw() 
{
    // PRQA S 4631 xml_tag_struct_dtor
    try {			
	    ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
		    ostringstream msg; 
		    msg << exDebugShortSignatureFile << "D'tor called. ptr: " << this << "  name: \"" << _name << "\"";
		    if (_parent) { msg << "  parent: " << _parent << "  parent_name: \"" << _parent->_name << "\"" ; }
		    msg << "  Removing all the components (" << _containing.size() << ")" ; 
		    ACS_LOG_DEBUG(msg.str());
	    ACS_CLASS_END_DEBUG

	    std::map <std::string, xml_tag_struct*>::const_iterator i ;
	    for (i=_containing.begin() ; i!=_containing.end() ; i++) { remove_tag(i->second, "") ; }

	    ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
		    ostringstream msg; 
		    msg << exDebugShortSignatureFile << "D'tor finished. ptr: " << this << "  name: \"" << _name << "\"";
		    if (_parent) { msg << "  parent: " << _parent << "  parent_name: \"" << _parent->_name << "\"" ; }
		    msg << "  REMOVED." ;
		    ACS_LOG_DEBUG(msg.str());
	    ACS_CLASS_END_DEBUG
    }
    catch(exception &x) {
	    ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
            ACS_LOG_NOTIFY_EX(x) ;
        ACS_CLASS_END_DEBUG
    }
    // PRQA L:xml_tag_struct_dtor
}

////////////////////////////////////////////////////////////////////////////////////////
//		STATIC: move_container		
////////////////////////////////////////////////////////////////////////////////////////
size_t xml_tag_struct::move_container (xml_tag_struct* src, xml_tag_struct* dst) 
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream msg; 
		msg << exDebugShortSignatureFile << "Method called. source " << src << "  destination: " << dst ;
		ACS_LOG_DEBUG(msg.str());
	ACS_CLASS_END_DEBUG

	ACS_COND_THROW ( !src, exIllegalValueException("src pointer is empty !")) ; // PRQA S 3081
	ACS_COND_THROW ( !dst, exIllegalValueException("dst pointer is empty !")) ; // PRQA S 3081
	
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream msg; 
		msg << exDebugShortSignatureFile << "Size of src container: " << (src->_containing).size() << "\tSize of dst container: " << (dst->_containing).size() ;
		ACS_LOG_DEBUG(msg.str());
	ACS_CLASS_END_DEBUG

	multimap <string, xml_tag_struct*>::const_iterator i ;
	ssize_t offset (0) ; 
	
	if (not (dst->_containing).empty())
	{
		// Search for the maximum index in destination -- newly added elements will use this as OFFSET (to have an index always greater than the previous max)
		for (i=(dst->_containing).begin() ; i!=(dst->_containing).end() ; i++) { offset = max (offset, (i->second)->_index) ; }
    	offset ++ ;	// offset (+1) to be added to the newly added elements
	} else {
		offset = 0 ; // The destination is EMPTY, therefore the index == 0 is free to be used
	}

	for (i=(src->_containing).begin() ; i!=(src->_containing).end() ; i++) { 
	
		(i->second)->_index += offset ;	// Update the index (before move the whole content, it's the same)
		(dst->_containing).insert ( make_pair ( (i->second)->_name, (i->second) ) ) ; // Move the content from src to dst
	}

	(src->_containing).clear() ; // Remove the destination -- OTHERWISE THE SAME POINTERS WOULD BELONG TO TWO DIFFERENT STRUCTURES !!!!

	const size_t src_size( (src->_containing).size() ) ;
	const size_t dst_size( (dst->_containing).size() ) ;

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream msg; 
		msg << exDebugShortSignatureFile << "Method finish. New size of src container : " << src_size << "  New size of dst container: " << dst_size ; 
		ACS_LOG_DEBUG(msg.str());
	ACS_CLASS_END_DEBUG

	return ( dst_size ) ; 
}


////////////////////////////////////////////////////////////////////////////////////////
//		remove_tag		
////////////////////////////////////////////////////////////////////////////////////////
void xml_tag_struct::remove_tag(xml_tag_struct* ptr, const string& parent_name) 
{
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
		ostringstream msg; 
		msg << exDebugShortSignatureFile << "Method called. About to recursively remove the content of ptr: " << ptr ;
		if (ptr) {
			msg << "  name: \"" << ptr->_name << "\"";
			if (ptr->_parent) { msg << "  parent: " << (ptr->_parent) << "  parent_name: \"" << (ptr->_parent)->_name << "\"" ; }
		}
		ACS_LOG_DEBUG(msg.str());
	ACS_CLASS_END_DEBUG
	
	if (ptr)
	{
		const string abs_path ( StringUtils::pathJoin(parent_name, ptr->_name) ) ; 
		map <string, xml_tag_struct*>::iterator i ;
		for (i=(ptr->_containing).begin() ; i!=(ptr->_containing).end() ; i++) { 
			remove_tag(i->second, abs_path) ; 
			i->second = 0 ; 
		}
		
		ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
			ostringstream msg; 
			msg << exDebugShortSignatureFile << "Removing ptr: " << ptr << "  name: \"" << ptr->_name << "\"";
			if (ptr->_parent) { msg << "  parent: " << (ptr->_parent) << "  parent_name: \"" << (ptr->_parent)->_name << "\"" ; }
			ACS_LOG_DEBUG(msg.str());
		ACS_CLASS_END_DEBUG

		delete ptr ; ptr = 0 ; 
	}
}


////////////////////////////////////////////////////////////////////////////////////////
//		STATIC convert_attributes
////////////////////////////////////////////////////////////////////////////////////////
multimap < string, string> xml_tag_struct::convert_attributes(const XERCES_CPP_NAMESPACE::AttributeList &attrs)
{
	multimap < string, string > attrs_M ;
	
	for (XMLSize_t i=0; i<attrs.getLength(); i++)
	{
		const pair<string, string> p (make_pair ( XERCES_CPP_NAMESPACE::XMLString::transcode(attrs.getName(i)), // PRQA S 3081
													string(XERCES_CPP_NAMESPACE::XMLString::transcode(attrs.getValue(i))) ) ) ;
		attrs_M.insert(p) ;
	}

	return attrs_M ;
}


////////////////////////////////////////////////////////////////////////////////////////
//		push_tag 		
////////////////////////////////////////////////////////////////////////////////////////
xml_tag_struct* xml_tag_struct::push_tag(const string& node_fp, const XERCES_CPP_NAMESPACE::AttributeList &attrs)
{
	const multimap < string, string > attrs_M (convert_attributes(attrs)) ;

	return push_tag(node_fp, attrs_M) ;
}	


xml_tag_struct* xml_tag_struct::push_tag(const string& node_fp, const multimap < string, string >& attrs_M) // PRQA S 4020
{
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
		ostringstream msg; 
		msg << "Method called. node_fp: \"" << node_fp << "  Ptr: " << this ;
		msg << "  name: \"" << _name << "\"";
		if (_parent) { msg << "  parent: " << (_parent) << "  parent_name: \"" << _parent->_name << "\"" ; }
		ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, msg.str());
	ACS_CLASS_END_DEBUG
	
	if (node_fp.empty()) { return this ; }

	vector <pair<string, string> > attrs_V ;
	for (multimap < string, string >::const_iterator i=attrs_M.begin() ; i!=attrs_M.end(); i++)
	{ attrs_V.push_back( make_pair(i->first, i->second) ) ;  }

	const bool removeLeadingSlash (true), removeLeadingDot(true) ;
	const vector<string> p_v = StringUtils::pathDisassemble(node_fp, removeLeadingSlash, removeLeadingDot) ;

	ACS_COND_THROW ( (p_v.size() < 1), exIllegalValueException("Unexpected empty node: \"" + node_fp + "\"")) ; // PRQA S 3081

	const string node ( p_v.at( p_v.size()-1 ) ) ;
	const ssize_t sz( (_containing).size() ) ;

	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
		ostringstream msg; 
		msg << exDebugShortSignatureFile << "Inserting node: \"" << node << "  Attrs: {" <<  dump_attributes(attrs_V) << " }" << endl ;
		ACS_LOG_DEBUG(msg.str());
	ACS_CLASS_END_DEBUG

	xml_tag_struct* ptr = new xml_tag_struct ;
	ptr->_name = node ; 
	ptr->_index = sz ; 
	ptr->_attributes = attrs_V ; 
	ptr->_parent = this ; 

	_containing.insert( make_pair( node, ptr )) ; 

	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
		ostringstream msg; 
		msg << exDebugShortSignatureFile << "Method finished. Returning Ptr: " << ptr ;
		ACS_LOG_DEBUG(msg.str());
	ACS_CLASS_END_DEBUG

	return ptr ; // PUSH
}


////////////////////////////////////////////////////////////////////////////////////////
//		pop_tag 		
////////////////////////////////////////////////////////////////////////////////////////
xml_tag_struct* xml_tag_struct::pop_tag()
{
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
		ostringstream msg; 
		msg << exDebugShortSignatureFile << "Method called" ;
		ACS_LOG_DEBUG(msg.str());
	ACS_CLASS_END_DEBUG

	if (_parent == 0) {
		ostringstream msg ; msg << "FATAL ERROR: Out of stack: " << static_cast<void*>(this) ; // PRQA S 3081
		ACS_THROW ( exIllegalValueException( msg.str() )) ;	// Too many POPs !!! // PRQA S 3081
	}

	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
		ostringstream msg; 
		msg << exDebugShortSignatureFile << "Method finished. Returning Ptr: " << _parent ;
		ACS_LOG_DEBUG(msg.str());
	ACS_CLASS_END_DEBUG
	
	return _parent ; // POP
}


////////////////////////////////////////////////////////////////////////////////////////
//		set_value 		
////////////////////////////////////////////////////////////////////////////////////////
void xml_tag_struct::set_value(const string& /* node_fp_IGNORED*/ , const string& value_in) 
{
	_value = value_in ; 
}


////////////////////////////////////////////////////////////////////////////////////////
//		dump_attributes 		
////////////////////////////////////////////////////////////////////////////////////////
string xml_tag_struct::dump_attributes(const vector <pair<string, string> >& attrs_V) 
{
	string attrs ; const char attributes_separator (0x20) ; /* Single space */
	for (size_t i=0; i<attrs_V.size(); i++) 
	{ if (i>0) { attrs += attributes_separator ; } attrs += ( attrs_V.at(i).first + "=" + "\"" + attrs_V.at(i).second + "\"") ; }
	return attrs ;
}


////////////////////////////////////////////////////////////////////////////////////////
//		dump		
////////////////////////////////////////////////////////////////////////////////////////
string xml_tag_struct::dump(const xml_tag_struct* ptr, const string& path_so_far, const size_t& level, const string& tab_so_far_in)
{
	const string root_tab	( "+---[" ) ;
	const string tab		( "|   " ) ;
	const size_t max_level (10) ; // NEVER TESTED
	const string max_level_tab	( "?---[" ) ;	// Width too large -- the root is graphically "flattened" (so node at different levels collapse at the same, maximum, level)
	string tab_so_far( tab_so_far_in ) ; 
	ostringstream msg ; 

	if (ptr)
	{
		const string abs_path ( StringUtils::pathJoin(path_so_far, ptr->_name) ) ; 
		if (level < max_level) { 		
			msg	<< tab_so_far << root_tab << ptr->_name << "]" ; // msg << "  (\"" << abs_path << "\")" ;
			msg << endl ; 
			tab_so_far += tab ;	
		} else {
			msg	<< tab_so_far << max_level_tab << "\"" << ptr->_name << "\"]  (\"" << abs_path << "\")" << endl ; 
		}		
		
		msg	<< tab_so_far << tab << (ptr) << "  (parent ptr: " << (ptr->_parent) << ")" << "  index: " << ptr->_index << endl ;
		msg	<< tab_so_far << tab << "Value: \"" << ptr->_value << "\"" << endl ;
		msg	<< tab_so_far << tab << "Attrs: { " << dump_attributes(ptr->_attributes) << " }" << endl ;	
		
		// Sort by index, otherwise the dump would be performed by alphabetic sorting of tags
		map <ssize_t, xml_tag_struct*> sorted_map ;
		multimap <string, xml_tag_struct*>::const_iterator i ;
		for (i=(ptr->_containing).begin() ; i!=(ptr->_containing).end() ; i++) { sorted_map.insert( make_pair( (i->second)->_index, (i->second)) ) ; }

		map <ssize_t, xml_tag_struct*>::const_iterator j ;
		for (j=sorted_map.begin() ; j!=sorted_map.end() ; j++) { msg << dump ( j->second, abs_path, (level+1), tab_so_far ) ; }
	}

	return ( msg.str() ) ; 	
}


//**************************************************************************************
//									
//   class XMLProcessor 		
//									
//**************************************************************************************
ACS_CLASS_DEFINE_DEBUG_LEVEL(XMLProcessor)
const string XMLProcessor::TAG_CMD__FROM_INPUT ("__FROM_INPUT") ;   
const string XMLProcessor::TAG_CMD__APPLY_SUBST ("__APPLY_SUBST") ;   
const string XMLProcessor::TAG_CMD__SORT_TAGS ("__SORT_TAGS") ;   
const string XMLProcessor::TAG_CMD__HEADER ("__HEADER") ;   
const string XMLProcessor::TAG_CMD__EXECUTE_COMMAND ("__EXECUTE_COMMAND") ;   
const string XMLProcessor::TAG_CMD__CALL_FUNCTION ("__CALL_FUNCTION") ;   


////////////////////////////////////////////////////////////////////////////////////////
//		XMLProcessor 		
////////////////////////////////////////////////////////////////////////////////////////
XMLProcessor::XMLProcessor(const string& base_path) 
	:	_xml_tag_ptr (new xml_tag_struct), _destroy_ptr_at_exit(true), _node_V(), _node_is_regexp(true), \
		_luts_M(), _subst_instances_V(), _sort_instances_V(), _filenames_in_M(), _executables_M(), _tags_to_add_M(), \
		_functs_M(), _env_M(), _entry_point(), _filename_out(), _base_path(base_path), _header_to_add(), _clone_root_tag(true)
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream msg; 
		msg << exDebugShortSignatureFile << "C'tor called.  new ptr: " << _xml_tag_ptr << "  base_path: \"" << _base_path << "\"" ;
		ACS_LOG_DEBUG(msg.str());
	ACS_CLASS_END_DEBUG
		
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream msg; msg << exDebugShortSignatureFile << "Method finished." ; ACS_LOG_DEBUG(msg.str());
	ACS_CLASS_END_DEBUG
}


////////////////////////////////////////////////////////////////////////////////////////
//		XMLProcessor 		
////////////////////////////////////////////////////////////////////////////////////////
XMLProcessor::XMLProcessor(xml_tag_struct* xml_tag_ptr, const string& base_path) 
	: 	_xml_tag_ptr (xml_tag_ptr), _destroy_ptr_at_exit(false), _node_V(), _node_is_regexp(true), \
		_luts_M(), _subst_instances_V(), _sort_instances_V(), _filenames_in_M(), _executables_M(), _tags_to_add_M(), \
		_functs_M(), _env_M(), _entry_point(), _filename_out(), _base_path(base_path), _header_to_add(), _clone_root_tag(true)
{ 
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream msg; 
		msg << exDebugShortSignatureFile << "C'tor called.  ptr_in: " << _xml_tag_ptr << "  base_path: \"" << _base_path << "\"" ;
		ACS_LOG_DEBUG(msg.str());
	ACS_CLASS_END_DEBUG

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream msg; msg << exDebugShortSignatureFile << "Method finished." ; ACS_LOG_DEBUG(msg.str());
	ACS_CLASS_END_DEBUG
}
	

////////////////////////////////////////////////////////////////////////////////////////
//		~XMLProcessor 		
////////////////////////////////////////////////////////////////////////////////////////
XMLProcessor::~XMLProcessor() throw() 
{
    // PRQA S 4631 XMLProcessor_dtor
    try { 
	    ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		    ostringstream msg; 
		    msg << exDebugShortSignatureFile << "D'tor called." ;
		    ACS_LOG_DEBUG(msg.str());
	    ACS_CLASS_END_DEBUG

	    ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
		    if (_xml_tag_ptr != 0) {
			    ostringstream msg; msg << _xml_tag_ptr->dump() ;
			    ACS_LOG_DEBUG(msg.str());
		    }
	    ACS_CLASS_END_DEBUG

	    if (_destroy_ptr_at_exit && _xml_tag_ptr) 
	    {
		    ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
			    ostringstream msg; 
			    msg << exDebugShortSignatureFile << "Destroying ptr: " << _xml_tag_ptr ;
			    ACS_LOG_DEBUG(msg.str());
		    ACS_CLASS_END_DEBUG

		    delete _xml_tag_ptr ; _xml_tag_ptr = 0 ; 
	    } 

	    ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		    ostringstream msg; msg << exDebugShortSignatureFile << "Method finished." ; ACS_LOG_DEBUG(msg.str());
	    ACS_CLASS_END_DEBUG
    }
    catch(exception &x) {
	    ACS_CLASS_BGN_DEBUG(ACS_VLO_VERB)
            ACS_LOG_NOTIFY_EX(x) ;
        ACS_CLASS_END_DEBUG
    }
    // PRQA L:XMLProcessor_dtor
}


////////////////////////////////////////////////////////////////////////////////////////
//		STATIC DEBUG_processing_function 		
////////////////////////////////////////////////////////////////////////////////////////
int XMLProcessor::DEBUG_processing_function (	const vector< pair<string, string> >& input_files_V,
												const vector <string>& funct_params_V,
												const string& processed_file, 
												const map<string, string> &env_M,
												const string& log_file) 
{
	// Used for DEBUG PURPOSES.
	// Replace the definition of a custom processing function with this function.

	// Create a fake XML file with all the input parameters
	ostringstream o ; 
	o << "<DEBUG_processing_function>\n" ;
	o << "<List_Of_Input_Files count=\"" << input_files_V.size() << "\">\n" ;
	for (size_t i=0; i<input_files_V.size(); i++) {
		o << "<Name Id=\"" << input_files_V.at(i).first << "\">" << input_files_V.at(i).second << "</Name>\n" ; 
	}
	o << "</List_Of_Input_Files>\n" ;

	o << "<List_Of_Params count=\"" << funct_params_V.size() << "\">\n" ;
	for (size_t i=0; i<funct_params_V.size(); i++) { o << "<Param>" << funct_params_V.at(i) << "</Param>\n" ; }
	o << "</List_Of_Params>\n" ;

	o << "<Output_File>" << processed_file << "</Output_File>\n" ;
	o << "<Log_File>" << log_file << "</Log_File>\n" ;
	o << "</DEBUG_processing_function>\n" ;

	// Copy the XML above to the output file
	{
		ofstream of_c(processed_file.c_str(), std::ios_base::binary | std::ofstream::trunc);
		ACS_COND_THROW ( (!of_c.good()), exIOException( "DEBUG_processing_function Could not open output file: \"" + processed_file + "\"" ) ) ; // PRQA S 3081
		of_c << o.str() ;
	}

	// Fill the logfile
	{
		ofstream of_c(log_file.c_str(), std::ios_base::binary | std::ofstream::trunc);
		ACS_COND_THROW ( (!of_c.good()), exIOException( "DEBUG_processing_function Could not open output file: \"" + processed_file + "\"" ) ) ; // PRQA S 3081
		of_c << "Begin of log file.\n" ; 
		of_c << o.str() ;
		of_c << "End of log file.\n" ; 
	}
			
	return 0 ; 
}


////////////////////////////////////////////////////////////////////////////////////////
//		dump 		
////////////////////////////////////////////////////////////////////////////////////////
string XMLProcessor::dump(const vector <string>& vec)
{
	ostringstream o ; for (size_t i=0; i<vec.size(); i++) { o << i << "\t\"" << vec.at(i) << "\"\n" ; }
	return o.str() ; 
}


////////////////////////////////////////////////////////////////////////////////////////
//		dump_as_path 		
////////////////////////////////////////////////////////////////////////////////////////
string XMLProcessor::dump_as_path(const vector <string>& vec)
{
	string s ; for (size_t i=0; i<vec.size(); i++) { s += "/" + vec.at(i) ; }
	return (s.empty()? "/" : s) ; 
}


////////////////////////////////////////////////////////////////////////////////////////
//		match_node_V 		
////////////////////////////////////////////////////////////////////////////////////////
bool XMLProcessor::match_node_V(const string& node_fp, const bool clone_root_tag, string& node_to_store)
{
	const string node ( StringUtils::pathNormalize(node_fp) ) ;
	vector <string> node_V ; 
	if (! node.empty())
	{
		const bool removeLeadingSlash (true), removeLeadingDot(true) ;
		node_V = StringUtils::pathDisassemble(node, removeLeadingSlash, removeLeadingDot) ;
	}

	return match_node_V(node_V, clone_root_tag, node_to_store) ; 
}


////////////////////////////////////////////////////////////////////////////////////////
//		match_node_V 		
////////////////////////////////////////////////////////////////////////////////////////
bool XMLProcessor::match_node_V(const vector <string>& node_V_needle, const bool clone_root_tag, string& node_to_store) // PRQA S 4020
{
// Note: node_V_needle is the single node provided by the parser. _node_V is the registered path (as regexp) 

	// IF clone_root_tag == false :
	// Note about operator less-equal ("<=") below:
	// It means that if the needle (node_V_needle provided as local input) has size <= of the haystack (the _node_V registered), the needle is ignored.
	// "==" happens when the needle has the same size, and being ignored allows to skip the root (this is the reason of <= and not just <).
	// Example: Haystack: /root/mytag
	// 			Needles:	/, /root, /root/other, /root/mytag: all ignored 
	// 						/root/mytag/entry: matches. So the first matching entry is "entry". This allows to remove the root "root/mytag"
	// Example: To match any node, including the root, use "/"
	// Example: to match any node BUT the root (whichever the root) use "/.*" and turn on regexp;
	//			In this case, due to "<=" /root won't match (size: 1==1) and /root/entry will match as "entry" (the root is skipped, as requested)


	// IF clone_root_tag == true :
	// Note about operator less-equal ("<") below:
	// It means that if the needle (node_V_needle provided as local input) has size < of the haystack (the _node_V registered), so it cannot be contained.
	// Example: Haystack: /root/mytag
	// 			Needles:	/, /root, /root/other, /root2, /root2/mytag: all ignored 
	// 						/root/mytag: matches. The output is "mytag" (the new root, actually)
	// 						/root/mytag/entry: matches. The output is mytag/entry (being mytag the new root)


	node_to_store = "" ; // Initialize
	if ( (clone_root_tag==false) && (node_V_needle.size() <= _node_V.size()) ) { return false ;	} // The root is skipped since DOES NOT exceed the haystack (and clone_root_tag is F) 
	if ( (clone_root_tag==true)  && (node_V_needle.size() <  _node_V.size()) ) { return false ;	} // The needle to match is SHORTER than the haystack (and clone_root_tag is T)
	
    size_t n_v_size = _node_V.size() ;
	for (size_t i=0; i<n_v_size; ++i) 
	{ 
		if (! _node_is_regexp)
		{
			if ( (node_V_needle.at(i) != _node_V.at(i)) ) { return false ; }
		} else {
		
			RegExp regexp ( _node_V.at(i) );
			if ( ! regexp.match( node_V_needle.at(i)) ) { return false ; }
		
		}
	}

	if ( (clone_root_tag==false) || (_node_V.size()<1) ) {
		node_to_store = path_assemble(node_V_needle, _node_V.size()) ; // Subpath, relative to the matching portion of the path
	} else {
		node_to_store = path_assemble(node_V_needle, _node_V.size()-1) ; // Subpath, relative to the matching portion of the path and including the root, too
	}

	return true ;
}


////////////////////////////////////////////////////////////////////////////////////////
//		STATIC match_exact_path_with_node 		
////////////////////////////////////////////////////////////////////////////////////////
bool XMLProcessor::match_exact_path_with_node(const string& path_haystack, const vector <string>& node_V_needle) // PRQA S 4020
{
// Note: path_haystack is the node under analysis (e.g. when applying a LUT to that path), while node_V_needle is the regexp contained in the LUT

	const bool removeLeadingSlash (true), removeLeadingDot(true) ;
	const vector <string> node_V_haystack = StringUtils::pathDisassemble(path_haystack, removeLeadingSlash, removeLeadingDot) ;

	size_t n_V_needle_size = node_V_needle.size() ;
	if (node_V_haystack.size() != n_V_needle_size) { return false ; }
	
	for (size_t i=0; i<n_V_needle_size; ++i)
	{
		RegExp regexp ( node_V_needle.at(i) );
		if ( ! regexp.match( node_V_haystack.at(i)) ) { return false ;  }
	}

	return true ;
}


////////////////////////////////////////////////////////////////////////////////////////
//		STATIC match_path_contained_in_node 		
////////////////////////////////////////////////////////////////////////////////////////
bool XMLProcessor::match_path_contained_in_node(const string& path_haystack, const vector <string>& node_V_needle) // PRQA S 4020
{
// Note: path_haystack is the node under analysis (e.g. when applying a LUT to that path), while node_V_needle is the regexp contained in the LUT
// Example:	haystack="/root/tag1" ; node=".*/tag2"  ->  FALSE
//			haystack="/root/tag1" ; node=".*/tag1"  ->  TRUE
//			haystack="/root/tag1/tag2" ; node=".*/tag1"  ->  TRUE (since the node is enterely contained in the path)


	const bool removeLeadingSlash (true), removeLeadingDot(true) ;
	const vector <string> path_V_haystack = StringUtils::pathDisassemble(path_haystack, removeLeadingSlash, removeLeadingDot) ;

	size_t n_V_needle_size = node_V_needle.size() ;
	if (path_V_haystack.size() < n_V_needle_size) { return false ;	} // The node is LARGER than the path (so is impossible it's contained)
	
	// If here: size(path) >= size(node)  --  The node might be contained in the path
	
	for (size_t i=0; i<n_V_needle_size; i++)
	{ 
		RegExp regexp ( node_V_needle.at(i) );
		if ( ! regexp.match( path_V_haystack.at(i)) ) { return false ;  }
	}

	return true ;
}


////////////////////////////////////////////////////////////////////////////////////////
//		STATIC path_assemble 		
////////////////////////////////////////////////////////////////////////////////////////
string XMLProcessor::path_assemble(const vector <string>& node_V, const size_t& offset) 
{
// offset is zero-relative
// Example: node_v = { "/raid0", "/path" } 
// offset = 0  ==>  "/raid0/path"
// offset = 1  ==>  "/path"
// offset = 2  ==>  ""

	string r ; 
	if ((offset+1) <= node_V.size()) { for (size_t i=offset; i<node_V.size(); i++) { r = StringUtils::pathJoin(r, node_V.at(i)) ; } } 
	return r ; 
}


////////////////////////////////////////////////////////////////////////////////////////
//		set_node_V 		
////////////////////////////////////////////////////////////////////////////////////////
void XMLProcessor::set_node_V(const std::string& node_fp, const bool is_regexp, const bool exact_regexp)
{
	_node_is_regexp = is_regexp ;
	const string node ( StringUtils::pathNormalize(node_fp) ) ;
	_node_V = vector <std::string>() ; 
	if (! node.empty())
	{
		const bool removeLeadingSlash (true), removeLeadingDot(true) ;
		_node_V = StringUtils::pathDisassemble(node, removeLeadingSlash, removeLeadingDot) ;
		
		if (is_regexp && exact_regexp) {
            size_t _n_V_size =  _node_V.size() ;
			for (size_t i=0; i<_n_V_size; ++i) { _node_V.at(i) = "^" + (_node_V.at(i)) + "$" ; } // Turn the path component in a exact regexp (^..$, so SAT cannot match MY_SAT_2)
		}
	}
}


////////////////////////////////////////////////////////////////////////////////////////
//		add_to_XMLOstream 		
////////////////////////////////////////////////////////////////////////////////////////
void XMLProcessor::add_to_XMLOstream(const xml_tag_struct* ptr, XMLOstream &xst)
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream msg; 
		msg << exDebugShortSignatureFile << "Method called. ptr: " << ptr ;
		ACS_LOG_DEBUG(msg.str());
	ACS_CLASS_END_DEBUG

	ACS_COND_THROW ( (ptr==0), exIllegalValueException( "NULL Pointer" )) ; // PRQA S 3081

	// Sort by index, otherwise the dump would be performed by alphabetic sorting of tags
	map <ssize_t, xml_tag_struct*> sorted_map ;
	map <string, xml_tag_struct*>::const_iterator i ;
	for (i=(ptr->_containing).begin() ; i!=(ptr->_containing).end() ; i++) { sorted_map.insert( make_pair( (i->second)->_index, (i->second)) ) ; }

	map <ssize_t, xml_tag_struct*>::const_iterator j ;
	for (j=sorted_map.begin() ; j!=sorted_map.end() ; j++) 
	{ 
		const string attrs ( xml_tag_struct::dump_attributes( (j->second)->_attributes ) ) ; 

		if ( (j->second)->_value == "" )
		{
			XMLOstream::Tag section_tag(xst ,(j->second)->_name , "", attrs);
				add_to_XMLOstream(j->second, xst) ; 
			section_tag.close() ; 
		}
		else
		{
			XMLOstream::Tag single_tag(xst, (j->second)->_name, (j->second)->_value, attrs, true);
		}
	}
			
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream msg; msg << exDebugShortSignatureFile << "Method finished" ; ACS_LOG_DEBUG(msg.str());
	ACS_CLASS_END_DEBUG
}


////////////////////////////////////////////////////////////////////////////////////////
//		process 		
////////////////////////////////////////////////////////////////////////////////////////
void XMLProcessor::process(const string& file_id, const string& node_path, bool is_regexp, const string& filename_out, bool add_header)
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream msg; 
		msg << exDebugShortSignatureFile << "Method called.  file_id: \"" << file_id << "\"  path: \"" << node_path 
			<< "\"  path_is_regexp: " << (is_regexp?"T":"F") ;
		if (! filename_out.empty()) { msg << "  filename_out: \"" << filename_out << "\"" ; }
		ACS_LOG_DEBUG(msg.str());
	ACS_CLASS_END_DEBUG

	_filename_out = filename_out ;	// Set filename_out 

	if (_filenames_in_M.find(file_id) == _filenames_in_M.end())
	{
		ACS_THROW ( exIllegalValueException( "Invalid Entry Point: file_id is undefined: \"" + file_id + "\"" )) ; // PRQA S 3081
	} else if (! File::exists( _filenames_in_M.at(file_id)) )  
	{
		ACS_THROW ( exIOException(	"Invalid Entry Point: file not found \"" + _filenames_in_M.at(file_id)  // PRQA S 3081
										+ "\"  (file_id: \"" + file_id + "\")" )) ;
	
	} else if (! File::isReadable( _filenames_in_M.at(file_id)) ) 
	{
		ACS_THROW ( exIOException(	"Invalid Entry Point: file not readable \"" + _filenames_in_M.at(file_id)  // PRQA S 3081
										+ "\"  (file_id: \"" + file_id + "\")" )) ;
	}
    else { /* nothing */ }

	// WHEN HERE: Entry point is a regular file

	_entry_point = file_id ;
	const string entry_point_file ( _filenames_in_M.at(_entry_point) ) ;
	
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream msg; 
		msg << exDebugShortSignatureFile << "Entry point file: \"" + entry_point_file + "\" seems to be a regular file" ; 
		ACS_LOG_DEBUG(msg.str());
	ACS_CLASS_END_DEBUG

	

	_node_is_regexp = is_regexp ; 

	const bool exact_regexp(is_regexp) ; // This value is ALWAYS TRUE, i.e. if it's a regexp is always EXACT (the pattern: "^..$" is always added). 
									// To prevent exact match, use .* in the input path
									// Example: /root/.*ANY.* will be turned to: { ^root$ ; ^.*ANY.*$ } witch still matches "root/hereANYname/
									// Without the add, from external, of .* it would not match.
	set_node_V(node_path, _node_is_regexp, exact_regexp) ; 

	XMLIstream xmlIstream(entry_point_file);
	xmlIstream.setDoSchema(false)  ;
	xmlIstream.addConsumer(this) ;
	xmlIstream.saxExtract() ;


	if (! _filename_out.empty())
	{
		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
			ostringstream msg; msg << exDebugShortSignatureFile << "Creating output file: \"" + _filename_out + "\"" ; ACS_LOG_DEBUG(msg.str());
		ACS_CLASS_END_DEBUG

		ofstream of ; of.open(_filename_out.c_str(), ofstream::out | std::ofstream::trunc) ;
		if (add_header) { 
			of << "<?xml version =\"1.0\" encoding=\"UTF-8\"?>\n" ;
			of << "<!-- A.C.S. XMLProcessor: $Id$ -->\n" ; 
			if (! _header_to_add.empty()) { of << _header_to_add ; } 
			of << "\n" ; // blank line
		}

		XMLOstream xst(of) ;  
		XMLProcessor::add_to_XMLOstream(_xml_tag_ptr, xst) ;
	

		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
			ostringstream msg; msg << exDebugShortSignatureFile << "File created successfully: \"" + _filename_out + "\""; ACS_LOG_DEBUG(msg.str());
		ACS_CLASS_END_DEBUG 
	}


	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream msg; msg << exDebugShortSignatureFile << "Method finished"; ACS_LOG_DEBUG(msg.str());
	ACS_CLASS_END_DEBUG 
}


////////////////////////////////////////////////////////////////////////////////////////
//		process_header 		
////////////////////////////////////////////////////////////////////////////////////////
void XMLProcessor::process_header(const string& xml_file_in, const string& node_in)
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream msg; msg << exDebugShortSignatureFile << "Method called.  xml_file_in: \"" << xml_file_in << "\"  node: \"" << node_in << "\"" ;
		ACS_LOG_DEBUG(msg.str());
	ACS_CLASS_END_DEBUG
		
		
	string node ; 
	{	
		const bool removeLeadingSlash (true), removeLeadingDot(true) ;
		const vector<string> p_v ( StringUtils::pathDisassemble(node_in, removeLeadingSlash, removeLeadingDot) ) ;

		// The path above will be assembled accordingly to resourceSet rules:
		// - The first root is omitted (so: /a/b/__HEADER will became b/__HEADER
		// - Dot is used as path separator (se: /a/b/__HEADER will became b.__HEADER)
		for (size_t i=1; i<p_v.size(); i++) { if (i>1) { node += "." ; } node += p_v.at(i) ; }
	}
					
					
	try 
	{				
		XMLIstream xmlIstream(xml_file_in);
		rsResourceSet rs;  
		xmlIstream >> rs;

		ACS_CLASS_BGN_DEBUG(ACS_INS_VERB)
			ostringstream msg; msg << exDebugShortSignatureFile << "DUMPING RS:" << endl << rs ;
			ACS_LOG_DEBUG(msg.str());
		ACS_CLASS_END_DEBUG


		//	Process tag: "define_lut"	
		//
		//////////////////////////////////////////////////////////////////////////////////////
		const string define_lut_tag ( node + ".define_lut" ) ; 
		if ( rs.hasKey(define_lut_tag) )
		{	
			bool	define_lut_isSingle ; 
			const	size_t define_lut_n ( rs.getArraySize(define_lut_tag, &define_lut_isSingle) ) ;

			for (size_t idl=0; idl<define_lut_n; ++idl)
			{
				ostringstream o ; o << define_lut_tag ; if (! define_lut_isSingle) { o << "[" << idl << "]" ; } 
				const string define_lut_i ( o.str() ) ; 

				string lut_id  ; const string lut_id_tag ( define_lut_i + ".id.value") ;
				rs.getValue(lut_id_tag, lut_id);

				LUT_struct tmp_lut (lut_id) ;

				const	string entry_tag ( define_lut_i + ".entry" ) ; 
				bool	entry_isSingle ; 
				const	size_t entry_n ( rs.getArraySize(entry_tag, &entry_isSingle) ) ;

				for (size_t ien=0; ien<entry_n; ++ien)
				{
					ostringstream o_ ; o_ << entry_tag ; if (! entry_isSingle) { o_ << "[" << ien << "]" ; } 
					const string entry_i ( o_.str() ) ; 

					string regexp  ; const string regexp_tag ( entry_i + ".regexp.value") ;
					rs.getValue(regexp_tag, regexp);
					string value  ; const string value_tag ( entry_i + ".value.value") ;
					rs.getValue(value_tag, value);

					tmp_lut.addElementToLUT( make_pair(regexp, value) ) ;

				}	// END: for entry_i

				ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
					ostringstream msg; msg << exDebugShortSignatureFile << "Read LUT with id: \"" << tmp_lut.getLutID() << "\"" << endl ; 
					ACS_LOG_DEBUG(msg.str());
				ACS_CLASS_END_DEBUG

				ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
					ostringstream msg; msg << tmp_lut.dump("\t") ;
					ACS_LOG_DEBUG(msg.str());
				ACS_CLASS_END_DEBUG

				_luts_M.insert( make_pair(lut_id, tmp_lut) ) ; 

			}	// END: for define_lut_i

			ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
				ostringstream msg; msg << exDebugShortSignatureFile << "Added " << _luts_M.size() << " LUT(s)" ; 
				ACS_LOG_DEBUG(msg.str());
			ACS_CLASS_END_DEBUG
		}	// END tag: "define_lut"	



		//	Process tag: "define_tag_to_add"	
		//
		//////////////////////////////////////////////////////////////////////////////////////
		const string define_entry_tag ( node + ".define_tag_to_add" ) ; 
		if ( rs.hasKey(define_entry_tag) )
		{	
			bool	define_entry_isSingle ; 
			const	size_t define_entry_n ( rs.getArraySize(define_entry_tag, &define_entry_isSingle) ) ;

			for (size_t ide=0; ide<define_entry_n; ++ide)
			{
				ostringstream o ; o << define_entry_tag ; if (! define_entry_isSingle) { o << "[" << ide << "]" ; } 
				const string define_entry_i ( o.str() ) ; 

				string entry_id  ; const string entry_id_tag ( define_entry_i + ".id.value") ;
				rs.getValue(entry_id_tag, entry_id);

				ADD_Struct tmp_struct ;

				const	string entry_tag ( define_entry_i + ".entry" ) ; 
				bool	entry_isSingle ; 
				const	size_t entry_n ( rs.getArraySize(entry_tag, &entry_isSingle) ) ;

				for (size_t ie=0; ie<entry_n; ++ie)
				{
					ostringstream o_ ; o_ << entry_tag ; if (! entry_isSingle) { o_ << "[" << ie << "]" ; } 
					const string entry_i ( o_.str() ) ; 

					string name  ; const string name_tag ( entry_i + ".tag.value") ;
					rs.getValue(name_tag, name);
					string value  ; const string value_tag ( entry_i + ".value.value") ;
					rs.getValue(value_tag, value);

					tmp_struct.addElement( make_pair(name, value) ) ; 

				}	// END: for entry_i

				ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
					ostringstream msg; msg << exDebugShortSignatureFile << "Read TAG_TO_ADD structure with id: \"" << entry_id << "\"" << endl ; 
					ACS_LOG_DEBUG(msg.str());
				ACS_CLASS_END_DEBUG
#if(0)
// FIXME DUMP UNDEFINED
				ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
					ostringstream msg; msg << tmp_struct.dump("\t") ;
					ACS_LOG_DEBUG(msg.str());
				ACS_CLASS_END_DEBUG
#endif
				_tags_to_add_M.insert( make_pair(entry_id, tmp_struct) ) ; 

			}	// END: for define_entry_i

			ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
				ostringstream msg; msg << exDebugShortSignatureFile << "Added " << _luts_M.size() << " TAG_TO_ADD structures" ; 
				ACS_LOG_DEBUG(msg.str());
			ACS_CLASS_END_DEBUG
		}	// END tag: "define_tag_to_entry"	



		//	Process tag: "define_input"	
		//
		//////////////////////////////////////////////////////////////////////////////////////
		const string define_input_tag ( node + ".define_input" ) ; 
		if ( rs.hasKey(define_input_tag) )
		{	
			bool	define_input_isSingle ; 
			const	size_t define_input_n ( rs.getArraySize(define_input_tag, &define_input_isSingle) ) ;
			string	t ; 

			for (size_t i=0; i<define_input_n; i++)
			{
				ostringstream o ; o << define_input_tag ; if (! define_input_isSingle) { o << "[" << i << "]" ; } 
				const string define_input_i ( o.str() ) ; 

				string id  ; const string id_tag ( define_input_i + ".file_id.value") ;
				rs.getValue(id_tag, id);

				string fileName  ; const string fn_tag ( define_input_i + ".file_name.value") ;
				rs.getValue(fn_tag, fileName);

				bool override (false) ; 
				t = ( define_input_i + ".override.value" ) ;
				if (rs.hasKey(t)) { rs.getValue(t, override); }

				ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
					ostringstream msg; msg << exDebugShortSignatureFile << "Read from node: " << define_input_tag << "\tid: \"" << id 
						<< "\"\tFile Name: \"" << fileName << "\"  override: " << (override? "T":"F") ;
					ACS_LOG_DEBUG(msg.str());
				ACS_CLASS_END_DEBUG

				if ( _filenames_in_M.find(id) == _filenames_in_M.end() ) 
				{
					ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
						ostringstream msg; msg << exDebugShortSignatureFile << "Defining file_id: \"" << id 
							<< "\" with value: \"" << fileName << "\"" ; 
						ACS_LOG_DEBUG(msg.str());
					ACS_CLASS_END_DEBUG

					_filenames_in_M.insert( make_pair(id, fileName) ) ;
				
				} 
				else if (override)
				{
					ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
						ostringstream msg; msg << exDebugShortSignatureFile << "Overriding file_id: \"" << id 
							<< "\" (having previous value: \"" << _filenames_in_M.at(id) << "\") with new value: \"" << fileName << "\""; 
						ACS_LOG_DEBUG(msg.str());
					ACS_CLASS_END_DEBUG

					_filenames_in_M[ id ] = fileName ;
				}
                else { /* nothing */ }



				if ( _filenames_in_M.find(id) == _filenames_in_M.end() )
				{
					ACS_THROW ( exIllegalValueException( "A file was declared but never defined: \"" + id + "\"" )) ; // PRQA S 3081

				} else {

					const string fName_ ( _filenames_in_M.at(id) ) ;

					ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
						ostringstream msg; msg << exDebugShortSignatureFile << "Input file was declared: \"" << id 
							<< "\" and defined: \"" << fName_ << "\"" ; 
						ACS_LOG_DEBUG(msg.str());
					ACS_CLASS_END_DEBUG

					ACS_COND_THROW( !File::exists(fName_), exIOException("File not found: \"" + fName_  // PRQA S 3081
						+ "\"  for key: \"" + id + "\"" ) ) ;

				}

			}	// END: for define_input_i		

			ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
				ostringstream msg; msg << exDebugShortSignatureFile << "Added " << define_input_n << " input file(s)" ; 
				ACS_LOG_DEBUG(msg.str());
			ACS_CLASS_END_DEBUG

		}	// END tag: "define_input"	


		//	Process tag: "declare_input"	
		//
		//////////////////////////////////////////////////////////////////////////////////////
		const string declare_input_tag ( node + ".declare_input" ) ; 
		if ( rs.hasKey(declare_input_tag) )
		{	
			bool	declare_input_isSingle ; 
			const	size_t declare_input_n ( rs.getArraySize(declare_input_tag, &declare_input_isSingle) ) ;

			for (size_t i=0; i<declare_input_n; i++)
			{
				ostringstream o ; o << declare_input_tag ; if (! declare_input_isSingle) { o << "[" << i << "]" ; } 
				const string declare_input_i ( o.str() ) ; 

				string id  ; const string id_tag ( declare_input_i + ".file_id.value") ;
				rs.getValue(id_tag, id);

				if ( _filenames_in_M.find(id) == _filenames_in_M.end() )
				{
					ACS_THROW ( exIllegalValueException( "A file was declared but never defined: \"" + id + "\"" )) ; // PRQA S 3081

				} 

			}	// END: for declare_input_i		

			ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
				ostringstream msg; msg << exDebugShortSignatureFile << "Added " << declare_input_n << " input file(s)" ; 
				ACS_LOG_DEBUG(msg.str());
			ACS_CLASS_END_DEBUG

		}	// END tag: "declare_input"	


		//	Process tag: "declare_function"	
		//
		//////////////////////////////////////////////////////////////////////////////////////
		const string declare_function_tag ( node + ".declare_function" ) ; 
		if ( rs.hasKey(declare_function_tag) )
		{	
			bool	declare_function_isSingle ; 
			const	size_t declare_function_n ( rs.getArraySize(declare_function_tag, &declare_function_isSingle) ) ;

			for (size_t i=0; i<declare_function_n; i++)
			{
				ostringstream o ; o << declare_function_tag ; if (! declare_function_isSingle) { o << "[" << i << "]" ; } 
				const string declare_function_i ( o.str() ) ; 

				string id  ; const string id_tag ( declare_function_i + ".funct_id.value") ;
				rs.getValue(id_tag, id);

				ACS_COND_THROW ( (_functs_M.find(id) == _functs_M.end()), exIllegalValueException( "A function was declared but never defined: \"" + id + "\"" )) ; // PRQA S 3081

			}	// END: for declare_function_i		

			ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
				ostringstream msg; msg << exDebugShortSignatureFile << "Added " << declare_function_n << " declared function(s)" ; 
				ACS_LOG_DEBUG(msg.str());
			ACS_CLASS_END_DEBUG

		}	// END tag: "declare_function"	


		//	Process tag: "declare_executable"	
		//
		//////////////////////////////////////////////////////////////////////////////////////
		const string declare_executable_tag ( node + ".declare_executable" ) ; 
		if ( rs.hasKey(declare_executable_tag) )
		{	
			bool	declare_executable_isSingle ; 
			const	size_t declare_executable_n ( rs.getArraySize(declare_executable_tag, &declare_executable_isSingle) ) ;

			for (size_t i=0; i<declare_executable_n; i++)
			{
				ostringstream o ; o << declare_executable_tag ; if (! declare_executable_isSingle) { o << "[" << i << "]" ; } 
				const string declare_executable_i ( o.str() ) ; 

				string id  ; const string id_tag ( declare_executable_i + ".cmd_id.value") ;
				rs.getValue(id_tag, id);

				ACS_COND_THROW ( (_executables_M.find(id) == _executables_M.end()), exIllegalValueException( "An executable was declared but never defined: \"" + id + "\"" )) ; // PRQA S 3081

			}	// END: for declare_executable_i		

			ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
				ostringstream msg; msg << exDebugShortSignatureFile << "Added " << declare_executable_n << " declared executable(s)" ; 
				ACS_LOG_DEBUG(msg.str());
			ACS_CLASS_END_DEBUG

		}	// END tag: "declare_executable"	


		//	Process tag: "declare_environment"	
		//
		//////////////////////////////////////////////////////////////////////////////////////
		const string declare_environment_tag ( node + ".declare_environment" ) ; 
		if ( rs.hasKey(declare_environment_tag) )
		{	
			bool	declare_environment_isSingle ; 
			const	size_t declare_environment_n ( rs.getArraySize(declare_environment_tag, &declare_environment_isSingle) ) ;

			for (size_t i=0; i<declare_environment_n; i++)
			{
				ostringstream o ; o << declare_environment_tag ; if (! declare_environment_isSingle) { o << "[" << i << "]" ; } 
				const string declare_environment_i ( o.str() ) ; 

				string id  ; const string id_tag ( declare_environment_i + ".key.value") ;
				rs.getValue(id_tag, id);

				ACS_COND_THROW ( (_env_M.find(id) == _env_M.end()), exIllegalValueException( "An environment was declared but never defined: \"" + id + "\"" )) ; // PRQA S 3081

			}	// END: for declare_environment_i		

			ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
				ostringstream msg; msg << exDebugShortSignatureFile << "Added " << declare_environment_n << " declared environment(s)" ; 
				ACS_LOG_DEBUG(msg.str());
			ACS_CLASS_END_DEBUG

		}	// END tag: "declare_environment"	


		//	Process tag: "define_environment"	
		//
		//////////////////////////////////////////////////////////////////////////////////////
		const string define_environment_tag ( node + ".define_environment" ) ; 
		if ( rs.hasKey(define_environment_tag) )
		{	
			bool	define_environment_isSingle ; 
			const	size_t define_environment_n ( rs.getArraySize(define_environment_tag, &define_environment_isSingle) ) ;
			string	t ; 

			for (size_t i=0; i<define_environment_n; i++)
			{
				ostringstream o ; o << define_environment_tag ; if (! define_environment_isSingle) { o << "[" << i << "]" ; } 
				const string define_environment_i ( o.str() ) ; 

				string id  ; const string id_tag ( define_environment_i + ".key.value") ;
				rs.getValue(id_tag, id);

				string value  ; const string fn_tag ( define_environment_i + ".value.value") ;
				rs.getValue(fn_tag, value);

				bool override (false) ; 
				t = ( define_environment_i + ".override.value" ) ;
				if (rs.hasKey(t)) { rs.getValue(t, override); }

				ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
					ostringstream msg; msg << exDebugShortSignatureFile << "Read from node: " << define_environment_tag << "\tkey: \"" << id 
						<< "\"\tvalue: \"" << value << "\"  override: " << (override? "T":"F") ;
					ACS_LOG_DEBUG(msg.str());
				ACS_CLASS_END_DEBUG

				if ( _env_M.find(id) == _env_M.end() ) 
				{
					ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
						ostringstream msg; msg << exDebugShortSignatureFile << "Defining key: \"" << id 
							<< "\" with value: \"" << value << "\"" ; 
						ACS_LOG_DEBUG(msg.str());
					ACS_CLASS_END_DEBUG

					_env_M.insert( make_pair(id, value) ) ;
				
				} 
				else if (override)
				{
					ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
						ostringstream msg; msg << exDebugShortSignatureFile << "Overriding key: \"" << id 
							<< "\" (having previous value: \"" << _env_M.at(id) << "\") with new value: \"" << value << "\""; 
						ACS_LOG_DEBUG(msg.str());
					ACS_CLASS_END_DEBUG

					_env_M[ id ] = value ;
				}
                else { /* nothing */ }

			}	// END: for define_environment_i		

			ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
				ostringstream msg; msg << exDebugShortSignatureFile << "Added " << define_environment_n << " input file(s)" ; 
				ACS_LOG_DEBUG(msg.str());
			ACS_CLASS_END_DEBUG

		}	// END tag: "define_environment"	


		//	Sanity check for all the entries in _filenames_in_M
		//
		//////////////////////////////////////////////////////////////////////////////////////
		for (map< string, string >::const_iterator i=_filenames_in_M.begin(); i!=_filenames_in_M.end(); ++i) // PRQA S 4238
		{
			const string id ( i->first ) ;
			const string fileName ( i->second ) ;

			if (fileName.empty() || !File::exists(fileName) || !File::isReadable(fileName))
			{
				ACS_THROW( exIOException("File not found or invalid : \"" + fileName  // PRQA S 3081
						+ "\"  for key: \"" + id + "\"" ) ) ;
			} else {
			
				ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
					ostringstream msg; msg << exDebugShortSignatureFile << "Final check: File with id:\t\"" << id << "\"\t file_name: \"" 
						<< fileName << "\"\tseems a regular file" ; 
					ACS_LOG_DEBUG(msg.str());
				ACS_CLASS_END_DEBUG

			}
		
		}	// END: for


	} catch (const exException& e) {
		ostringstream o ;
		o << "Error processing header from file: \"" << xml_file_in << "\"  node: \"" << node_in << "\"  rs path: \"" << node << "\"" ; 
		ACS_THROW(exIllegalValueException(e,o.str())) ; // PRQA S 3081
	}

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream msg; msg << exDebugShortSignatureFile << "Method finished.  Header file: \"" << xml_file_in << "\" processed successfully" ;
		ACS_LOG_DEBUG(msg.str());
	ACS_CLASS_END_DEBUG
}


////////////////////////////////////////////////////////////////////////////////////////
//		apply_sort 		
////////////////////////////////////////////////////////////////////////////////////////
bool XMLProcessor::apply_sort(const string& path_in, xml_tag_struct* xml_tag, const bool is_value) 
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream msg; msg << exDebugShortSignatureFile << "Method called.  path: \"" << path_in << "\" is_value: " << (is_value?"T":"F");
		ACS_LOG_DEBUG(msg.str());
	ACS_CLASS_END_DEBUG
			
	const bool removeLeadingSlash (true), removeLeadingDot(true) ;

	// The absolute_path is computed using the _base_path the engine was initialized with
	// _base_path is EMPTY in the first instance, and is filled with the local path of the call of __FROM_INPUT
	string abs_path (_base_path) ; 

	// The input path is disassembled in order to remove any "__APPLY_SUBST" subpath occurence 
	// (e.g. "/root/__APPLY_SUBST/__APPLY_SUBST/tag" is simplified to "/root/tag")
	// This is only useful to apply a LUT inside the recipe file itself, without injecting a file from outside.
	// Without this, the path to locally match the LUT should contain the "/__APPLY_SUBST" layer(s)
	{
		const vector<string> p_v ( StringUtils::pathDisassemble(path_in, removeLeadingSlash, removeLeadingDot) ) ;
		
		for (size_t i=0; i<p_v.size(); i++)
		{	if ( (p_v.at(i) != TAG_CMD__APPLY_SUBST) && (p_v.at(i) != TAG_CMD__SORT_TAGS) && (p_v.at(i) != TAG_CMD__FROM_INPUT) \
						&& (p_v.at(i) != TAG_CMD__HEADER) && (p_v.at(i) != TAG_CMD__EXECUTE_COMMAND) && (p_v.at(i) != TAG_CMD__CALL_FUNCTION))
 		{ abs_path = StringUtils::pathJoin(abs_path, p_v.at(i)) ; } }
	}

    size_t _sort_instances_V_size = _sort_instances_V.size() ;
	for (size_t i=0; i<_sort_instances_V_size ; ++i)
	{
		const pair <string, string> sort_instance (_sort_instances_V.at(i)) ; 
		const string match_element ( sort_instance.first ) ; 
		const string sort_order ( sort_instance.second ) ;


		ACS_CLASS_BGN_DEBUG(ACS_INS_VERB)
			ostringstream msg; msg << exDebugShortSignatureFile << "Dumping sort_instance #" << i << endl ;
			ACS_LOG_DEBUG(msg.str());
		ACS_CLASS_END_DEBUG

		vector<string> p_v = StringUtils::pathDisassemble(match_element, removeLeadingSlash, removeLeadingDot) ;

		string attr_to_match ("") ;

		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
			ostringstream msg; msg << exDebugShortSignatureFile << "Path to match: \"" << match_element 
																<< "\"  sort_order: \"" << sort_order << "\"" ;
			ACS_LOG_DEBUG(msg.str());
		ACS_CLASS_END_DEBUG

		
		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
			ostringstream msg; msg << exDebugShortSignatureFile << "Matching abs_path: \"" << abs_path << "\" with SORT element: " << StringUtils::pathAssemble(p_v) ;
			ACS_LOG_DEBUG(msg.str());
		ACS_CLASS_END_DEBUG

#if(0)
{
// FIXME DEBUG AREA
excerr << "FIXME MATCHING SORT abs_path: \"" << abs_path << "\"" << endl ;
excerr << "WITH:\t" ; { for (size_t i=0; i<p_v.size(); i++) { excerr << " \"" <<p_v.at(i) << "\""; } } ; excerr  << endl ;
}
#endif

		if (match_exact_path_with_node(abs_path, p_v))	/* Match p_v with the path under processing (path) */
		{
			ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
				ostringstream msg; msg << exDebugShortSignatureFile << "Comparison succesfull. Applying sort_order: \"" << sort_order
						<< "\"  to abs_path: \"" << abs_path << "\"" ;
				ACS_LOG_DEBUG(msg.str());
			ACS_CLASS_END_DEBUG
			
			if ( (is_value) && xml_tag && ((xml_tag->_containing).size()>0) )
			{

//excerr << "FIXME NEW PART MATCHING ORDER" << endl ; 
//excerr << endl << xml_tag->dump() << endl ;
				
				// STEP 1 :
				// Create a map of the ordered tags, from -MAX to -1 :
				map <string, ssize_t> to_sort_M ;
				{
					vector <string> to_sort_V ; StringUtils::Tokenize(sort_order, to_sort_V, " ");
					const ssize_t offset ( -1 * to_sort_V.size() ) ; // If 4 elemens, the offset will be -4. So from -4 to -1.
                    ssize_t t_s_V_size = ssize_t(to_sort_V.size()) ;
					for (ssize_t j=0; j<t_s_V_size; ++j) { to_sort_M.insert( make_pair ( to_sort_V.at(j), (offset+j)) ) ; }
				}
				
				/*	WHEN HERE:
					The map to_sort_M contains the token with the new index (the index will be overwritten if the token is found in the input).
					If 3 tokens are provided (e.g. TAG_A, TAG_B, TAG_C) the indexes will be: -3, -2, -1 (always: from -MAX to -1)
				*/	
					
				
				// STEP 2:
				// Since negative values will be used for values to sort, and positive values for all the others (so, the values to be sort are BEFORE the unsorted ones)
				// the indexes in xml_tag will be made positive: the lowest index is found. If it's negative (e.g. -2) all the indexes will be added with +2.
				{
					ssize_t min_index = numeric_limits<ssize_t>::max() ; // Impossibly high
					multimap <string, xml_tag_struct*>::const_iterator it_ ;
					for (it_=(xml_tag->_containing).begin() ; it_!=(xml_tag->_containing).end() ; ++it_) 
					{
						min_index = min(min_index, (it_->second)->_index) ;
					}

					if (min_index < 0) {
						const ssize_t offset (-1*min_index) ; 
						for (it_=(xml_tag->_containing).begin() ; it_!=(xml_tag->_containing).end() ; ++it_) { 
                            (it_->second)->_index += offset ; 
                        }
					}
				}	

				/*	WHEN HERE:
					The structure xml_tag contains only positive indexes
				*/	


				// STEP 3:
				// All the values of xml_tag will be matched agains the values of to_sort_M. If a value is found, the index will be replaced with the negative index.
				multimap <string, xml_tag_struct*>::const_iterator it_ ;
				for (it_=(xml_tag->_containing).begin() ; it_!=(xml_tag->_containing).end() ; ++it_) 
				{
					const string tag_name ((it_->second)->_name) ;
					map <string, ssize_t>::const_iterator to_sort_M_it = to_sort_M.find(tag_name);
					if ( to_sort_M_it !=  to_sort_M.end())
					{
						const ssize_t new_index ( to_sort_M_it->second ) ; // index contained in the to_sort_M map, negative
						(it_->second)->_index = new_index ;	// New index is negative
					}
				}

//excerr << "FIXME2  NEW PART MATCHING ORDER" << endl ; 
//excerr << endl << xml_tag->dump() << endl ;

				
			}	// END: if ( (is_value) && xml_tag && ((xml_tag->_containing).size()>0) )
				
		}	// END: if (match_exact_path_with_node(path, p_v))

	}	// END: for i
		
	const bool r ( true ) ; // FIXME CHANGE THIS

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream msg; msg << exDebugShortSignatureFile << "Method finished.  returing: " << (r?"TRUE":"FALSE") ; 
		ACS_LOG_DEBUG(msg.str());
	ACS_CLASS_END_DEBUG
	
	return r ; 
}


////////////////////////////////////////////////////////////////////////////////////////
//		 	apply_function_to_string	
////////////////////////////////////////////////////////////////////////////////////////
string XMLProcessor::apply_function_to_string(const string& funct_id, const vector<string>& funct_params_V, const string& string_in, bool& subst_is_triggered) const 
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream msg; msg << exDebugShortSignatureFile << "Method called.  funct_id: \"" << funct_id << "\"  input_string: \"" << string_in << "\"" ;
		ACS_LOG_DEBUG(msg.str());
	ACS_CLASS_END_DEBUG
	
	string string_out ;	

	FileGuard in_file(File::tempname("/tmp/file_i_", "xml")) ; 
    File::touch(in_file) ; // PRQA S 3050 
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB) ; in_file.detach() ; ACS_CLASS_END_DEBUG ;

	FileGuard processed_file(File::tempname("/tmp/file_p_", "xml")) ; 
    File::touch(processed_file) ; // PRQA S 3050 
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB) ; processed_file.detach() ; ACS_CLASS_END_DEBUG ;

	FileGuard log_file(File::tempname("/tmp/file_log_", "txt")) ; 
    File::touch(log_file) ; // PRQA S 3050 
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB) ; log_file.detach() ; ACS_CLASS_END_DEBUG ;

	// Create the input file, by placing the input string inside.
	ofstream of_c(string(in_file).c_str(), std::ios_base::binary | std::ofstream::trunc); of_c << string_in ; of_c.close() ;  // PRQA S 3050, 3081
	
	
	vector < pair <string, string> > extracted_files_V ;
	extracted_files_V.push_back( pair<string, string> ("", in_file) ) ;	// The key is irrelevant since only a file is passed // PRQA S 3050 

	int (*funct_ptr) (ACS_XMLPROCESSOR__FUNCT_SIGNATURE) = _functs_M.at(funct_id) ; // Assign the function pointer to funct_ptr
	int ret_code (-1) ; 
	string ret_message ;

	try
	{
		ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
			ostringstream msg; 
			msg << exDebugShortSignatureFile << "Calling funct_id: \"" << funct_id ;
			msg << "\"  input: " << string(in_file) ; // PRQA S 3050, 3081 3
			msg << "\"  output: " << string(processed_file) ;
			msg << "\"  log file: " << string(log_file) << "\"";
			ACS_LOG_DEBUG(msg.str());
		ACS_CLASS_END_DEBUG

		ret_code = (*funct_ptr) (extracted_files_V, funct_params_V, processed_file, _env_M, log_file) ; // PRQA S 3050 

		if ( ret_code )
		{
			ostringstream msg; 
			msg << "Status code " << ret_code << " after calling funct_id: \"" << funct_id ;
			msg << "\"  input: \"" << string(in_file) ; // PRQA S 3050, 3081 3
			msg << "\"  output: \"" << string(processed_file)  ;
			msg << "\"  log file: \"" << string(log_file) << "\"" ;
			ret_message = msg.str() ;
		}
	}
	catch (const exException& e) {
		ostringstream o ;
		o  << "Error calling funct_id: \"" << funct_id << "\"" ;
		ACS_THROW(exIOException(e,o.str())) ; // PRQA S 3081
	}
	
	subst_is_triggered = ( (ret_code==0) && File::exists(processed_file) && (File::size(processed_file) > 0) ) ; // PRQA S 3050 

	if (subst_is_triggered)
	{
		// Read the output file, reading the string from it.
		ifstream if_a(string(processed_file).c_str(), std::ios_base::binary); if (if_a.good()) { getline (if_a,string_out) ; } ; if_a.close() ;  // PRQA S 3050, 3081 2 
		if ( File::exists(log_file) ) { concat_log_file(log_file) ; } 
	}


	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream msg; msg << exDebugShortSignatureFile << "Method finished.  subst_is_triggered: " << (subst_is_triggered?"T":"F") ;
		if (subst_is_triggered) { msg << "  output_string: \"" << string_out << "\"" << endl ; }
		ACS_LOG_DEBUG(msg.str());
	ACS_CLASS_END_DEBUG
	
	return string_out ;
}
		

////////////////////////////////////////////////////////////////////////////////////////
//		 apply_subst		
////////////////////////////////////////////////////////////////////////////////////////
bool XMLProcessor::apply_subst(const string& path_in, xml_tag_struct* xml_tag, const bool is_value) 
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream msg; msg << exDebugShortSignatureFile << "Method called.  path: \"" << path_in << "\" is_value: " << (is_value?"T":"F");
		ACS_LOG_DEBUG(msg.str());
	ACS_CLASS_END_DEBUG
			
	const bool removeLeadingSlash (true), removeLeadingDot(true) ;

	// The absolute_path is computed using the _base_path the engine was initialized with
	// _base_path is EMPTY in the first instance, and is filled with the local path of the call of __FROM_INPUT
	string abs_path (_base_path) ; 

	// The input path is disassembled in order to remove any "__APPLY_SUBST" subpath occurence 
	// (e.g. "/root/__APPLY_SUBST/__APPLY_SUBST/tag" is simplified to "/root/tag")
	// This is only useful to apply a LUT inside the recipe file itself, without injecting a file from outside.
	// Without this, the path to locally match the LUT should contain the "/__APPLY_SUBST" layer(s)
	{
		const vector<string> p_v ( StringUtils::pathDisassemble(path_in, removeLeadingSlash, removeLeadingDot) ) ;
		
		for (size_t i=0; i<p_v.size(); i++)
		{	if ( (p_v.at(i) != TAG_CMD__APPLY_SUBST) && (p_v.at(i) != TAG_CMD__SORT_TAGS) && (p_v.at(i) != TAG_CMD__FROM_INPUT) \
						&& (p_v.at(i) != TAG_CMD__HEADER) && (p_v.at(i) != TAG_CMD__EXECUTE_COMMAND) && (p_v.at(i) != TAG_CMD__CALL_FUNCTION))
			{ abs_path = StringUtils::pathJoin(abs_path, p_v.at(i)) ; } 
		}	// END: for
	}
 
	SUBST_instance::Match_Element_Rule success (SUBST_instance::TAG_UNDEFINED) ;
	vector < pair<size_t, string> > attributes_to_change ;	// Pending operations: attributes to rename
	vector < pair<string, string> > new_tags_to_add ;		// Pending operations: tags to add 
	size_t s_i_V_size = _subst_instances_V.size() ;
    for (size_t i=0; i<s_i_V_size ; ++i)
	{
		const SUBST_instance& subst_instance( _subst_instances_V.at(i) ) ;
		const string id (subst_instance.getId()) ;
		const SUBST_instance::SUBST_Type subst_type (subst_instance.getType()) ;
		const SUBST_instance::Match_Element_Rule match_element_rule ( subst_instance.getMatch_Element_Rule() ) ;
		string match_element ( subst_instance.getMatch_Element() ) ;
	
		ACS_CLASS_BGN_DEBUG(ACS_INS_VERB)
			ostringstream msg; msg << exDebugShortSignatureFile << "Dumping subst_instance #" << i << endl << subst_instance.dump();
			ACS_LOG_DEBUG(msg.str());
		ACS_CLASS_END_DEBUG

		vector<string> p_v = StringUtils::pathDisassemble(match_element, removeLeadingSlash, removeLeadingDot) ;
		string attr_to_match ("") ;

		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
			ostringstream msg; msg << exDebugShortSignatureFile << "Path to match: \"" << match_element 
																<< "\"  type: " << SUBST_instance::Match_Element_Rule_to_String(match_element_rule) ;
			ACS_LOG_DEBUG(msg.str());
		ACS_CLASS_END_DEBUG


		if ( not p_v.empty() && ( (match_element_rule & SUBST_instance::TAG_ATTR_NAME) || (match_element_rule & SUBST_instance::TAG_ATTR_VALUE) ) )
		{
			string elem_to_match ( not p_v.empty() ? (p_v.at( p_v.size()-1 )) : "" ) ;

			const string needle ("###") ;
			const size_t found ( elem_to_match.find(needle) ) ;
			if (found != string::npos)
			{
				attr_to_match = elem_to_match.substr( found + needle.size() ) ;	// Do not move from here
				elem_to_match = elem_to_match.substr( 0, found ) ;				// Do not move from here
				p_v.at( p_v.size()-1 ) = elem_to_match ;
			}
		}
		
		
		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
			ostringstream msg; msg << exDebugShortSignatureFile << "Matching abs_path: \"" << abs_path << "\" with LUT element: " << StringUtils::pathAssemble(p_v) ;
			ACS_LOG_DEBUG(msg.str());
		ACS_CLASS_END_DEBUG


		if (match_exact_path_with_node(abs_path, p_v))	/* Match p_v (the SUBST trigger, without the ###Attribute) with the path under processing (path) */
		{
			ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
				ostringstream msg; msg << exDebugShortSignatureFile << "Comparison succesfull. Applying SUBST with ID: \"" << id 
						<< "\"  to abs_path: \"" << abs_path << "\"" ;
				ACS_LOG_DEBUG(msg.str());
			ACS_CLASS_END_DEBUG
			
			
			if (is_value)
			{
				// Triggered when a node contains the VALUE, i.e. the node is about to be closed. The value might be undefined
				if (match_element_rule & SUBST_instance::TAG_VALUE) 
				{
					string after_subst ("") ; 
					bool subst_is_triggered(false) ;

					if (subst_type == SUBST_instance::SUBST_LUT)
					{
						ACS_COND_THROW((_luts_M.find(id)==_luts_M.end()), exIllegalValueException("INTERNAL ERROR: LUT NOT FOUND with id: \"" + id + "\"" )) ; // PRQA S 3081
						const LUT_struct& lut( _luts_M.at(id) ) ;
						after_subst = lut.apply(xml_tag->_value, subst_is_triggered) ;
						
					} else if (subst_type == SUBST_instance::SUBST_FUNCT)
					{
						ACS_COND_THROW((_functs_M.find(id)==_functs_M.end()), exIllegalValueException("INTERNAL ERROR: FUNCTION NOT FOUND with id: \"" + id + "\"" )) ; // PRQA S 3081
						after_subst = apply_function_to_string(id, subst_instance.getOpts(), xml_tag->_value, subst_is_triggered) ;
					}
					else { /* N/A or not handled yet: Do nothing */ } 

					if (subst_is_triggered) {

						ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
							ostringstream msg; msg << exDebugShortSignatureFile << "Setting a new TAG_VALUE.  Previous value: \"" 
									<< xml_tag->_value << "\"  New value: \"" << after_subst << "\"" ;
							ACS_LOG_DEBUG(msg.str());
						ACS_CLASS_END_DEBUG

						xml_tag->_value = after_subst ; success = static_cast<SUBST_instance::Match_Element_Rule>(success | SUBST_instance::TAG_VALUE) ; 
					}

				}	// END: if (match_element_rule & SUBST_instance::TAG_VALUE) 
				
			} else {
			
				// Triggered when a node does not contain the VALUE, i.e. the node is at the beginning and contains the name and all the attributes, but not the value
				if (match_element_rule & SUBST_instance::TAG_NAME) 
				{
					if (subst_type == SUBST_instance::SUBST_LUT)
					{
						ACS_COND_THROW((_luts_M.find(id)==_luts_M.end()), exIllegalValueException("INTERNAL ERROR: LUT NOT FOUND with id: \"" + id + "\"" )) ; // PRQA S 3081

						const LUT_struct& lut( _luts_M.at(id) ) ;

						string after_subst ("") ; 
						bool subst_is_triggered(false) ;
						after_subst = lut.apply(xml_tag->_name, subst_is_triggered) ;
						if (subst_is_triggered) {

							ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
								ostringstream msg; msg << exDebugShortSignatureFile << "Setting a new TAG_NAME.  Previous value: \"" 
										<< xml_tag->_name << "\"  New value: \"" << after_subst << "\"" ;
								ACS_LOG_DEBUG(msg.str());
							ACS_CLASS_END_DEBUG

							xml_tag->_name = after_subst ; success = static_cast<SUBST_instance::Match_Element_Rule>(success | SUBST_instance::TAG_NAME) ; 

						}
					} else if (subst_type == SUBST_instance::SUBST_ADD)
					{
						ACS_COND_THROW((_tags_to_add_M.find(id)==_tags_to_add_M.end()), exIllegalValueException("INTERNAL ERROR: FUNCTION NOT FOUND with id: \"" + id + "\"" )) ; // PRQA S 3081

						const ADD_Struct& tags_to_add( _tags_to_add_M.at(id) ) ;
						size_t t_t_add_size = tags_to_add.size() ;
                        for (size_t itta=0; itta<t_t_add_size; ++itta) { new_tags_to_add.push_back( tags_to_add.at(itta) ) ; }

					} else if (subst_type == SUBST_instance::SUBST_FUNCT)
					{
						ACS_COND_THROW((_functs_M.find(id)==_functs_M.end()), exIllegalValueException("INTERNAL ERROR: FUNCTION NOT FOUND with id: \"" + id + "\"" )) ; // PRQA S 3081

						string after_subst ("") ; 
						bool subst_is_triggered(false) ;
						after_subst = apply_function_to_string(id, subst_instance.getOpts(), xml_tag->_name, subst_is_triggered) ;
						if (subst_is_triggered) {

							ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
								ostringstream msg; msg << exDebugShortSignatureFile << "Setting a new TAG_NAME.  Previous value: \"" 
										<< xml_tag->_name << "\"  New value: \"" << after_subst << "\"" ;
								ACS_LOG_DEBUG(msg.str());
							ACS_CLASS_END_DEBUG

							xml_tag->_name = after_subst ; success = static_cast<SUBST_instance::Match_Element_Rule>(success | SUBST_instance::TAG_NAME) ; 

						}

					}
					else { /* N/A or not handled yet: Do nothing */ } 

				}	// END: if (match_element_rule & SUBST_instance::TAG_NAME) 



				// Check all the attributes against the LUT Trigger
				if ( (!attr_to_match.empty()) && ( (match_element_rule & SUBST_instance::TAG_ATTR_NAME) || (match_element_rule & SUBST_instance::TAG_ATTR_VALUE) ) )
				{
					const RegExp regexp ( attr_to_match );
					size_t tag_att_size = xml_tag->_attributes.size() ;
                    for (size_t j=0; j<tag_att_size; ++j)
					{
						pair <string, string> &attribute ( xml_tag->_attributes.at(j) ) ; // Note, it's a REFERENCE 
						if ( regexp.match( attribute.first ) )
						{
							if (match_element_rule & SUBST_instance::TAG_ATTR_NAME)
							{
								string after_subst ("") ; 
								bool subst_is_triggered(false) ;
							
								if (subst_type == SUBST_instance::SUBST_LUT)
								{
									ACS_COND_THROW((_luts_M.find(id)==_luts_M.end()), exIllegalValueException("INTERNAL ERROR: LUT NOT FOUND with id: \"" + id + "\"" )) ; // PRQA S 3081
									const LUT_struct& lut( _luts_M.at(id) ) ;
									after_subst = lut.apply(attribute.first, subst_is_triggered) ;
								}
								else if (subst_type == SUBST_instance::SUBST_FUNCT)
								{
									ACS_COND_THROW((_functs_M.find(id)==_functs_M.end()), exIllegalValueException("INTERNAL ERROR: FUNCTION NOT FOUND with id: \"" + id + "\"" )) ; // PRQA S 3081
									after_subst = apply_function_to_string(id, subst_instance.getOpts(), attribute.first, subst_is_triggered) ;
								}
								else { /* N/A or not handled yet: Do nothing */ } 
								
								if (subst_is_triggered) {

									ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
										ostringstream msg; msg << exDebugShortSignatureFile << "Setting a new TAG_ATTR_NAME.  Previous value: \"" 
												<< attribute.first << "\"  New value: \"" << after_subst << "\"" ;
										ACS_LOG_DEBUG(msg.str());
									ACS_CLASS_END_DEBUG

									attributes_to_change.push_back( make_pair(j, after_subst) ) ; 
									success = static_cast<SUBST_instance::Match_Element_Rule>(success | SUBST_instance::TAG_ATTR_NAME) ; 
								}
								
							}	// END: if (match_element_rule & SUBST_instance::TAG_ATTR_NAME)


							if (match_element_rule & SUBST_instance::TAG_ATTR_VALUE)
							{
								string after_subst ("") ; 
								bool subst_is_triggered(false) ;
							
								if (subst_type == SUBST_instance::SUBST_LUT)
								{
									ACS_COND_THROW((_luts_M.find(id)==_luts_M.end()), exIllegalValueException("INTERNAL ERROR: LUT NOT FOUND with id: \"" + id + "\"" )) ; // PRQA S 3081
									const LUT_struct& lut( _luts_M.at(id) ) ;
									after_subst = lut.apply(attribute.second, subst_is_triggered) ;
								}
								else if (subst_type == SUBST_instance::SUBST_FUNCT)
								{
									ACS_COND_THROW((_functs_M.find(id)==_functs_M.end()), exIllegalValueException("INTERNAL ERROR: FUNCTION NOT FOUND with id: \"" + id + "\"" )) ; // PRQA S 3081
									after_subst = apply_function_to_string(id, subst_instance.getOpts(), attribute.second, subst_is_triggered) ;
								}
								else { /* N/A or not handled yet: Do nothing */ } 
								
								if (subst_is_triggered) {

									ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
										ostringstream msg; msg << exDebugShortSignatureFile << "Setting a new TAG_ATTR_VALUE.  Previous value: \"" 
												<< attribute.second << "\"  New value: \"" << after_subst << "\"" ;
										ACS_LOG_DEBUG(msg.str());
									ACS_CLASS_END_DEBUG

									attribute.second = after_subst ; 
									success = static_cast<SUBST_instance::Match_Element_Rule>(success | SUBST_instance::TAG_ATTR_VALUE) ; 
								}								
								
							}	// END: if (match_element_rule & SUBST_instance::TAG_ATTR_VALUE)

						}	// END: if ( regexp.match( attribute.first ) ) [ Attribute matched the LUT trigger ]

					}	// END: for j

				}	// END: if ( (!attr_to_match.empty()) && ( (match_element_rule & TAG_ATTR_NAME) || (match_element_rule & TAG_ATTR_VALUE) ) )

	
				// WHEN HERE: Tag names and Tag attributes has been renamed
	
				// Add new node
                size_t n_t_t_a_size = new_tags_to_add.size() ;
				for (size_t intta=0; intta<n_t_t_a_size; ++intta)
				{
					const string tag_name (new_tags_to_add.at(intta).first) ;
					const string tag_value (new_tags_to_add.at(intta).second) ;

					ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
						ostringstream msg; msg << exDebugShortSignatureFile << "Adding a new node: " << tag_name << " Value: " << tag_value << endl ;
						ACS_LOG_DEBUG(msg.str());
					ACS_CLASS_END_DEBUG
										
					xml_tag_struct* tmp_ptr ( xml_tag->push_tag(tag_name) ) ;
					tmp_ptr->set_value(tag_name, tag_value) ; 
	
				}	// END: Add new tags

			}	// END: if (is_value) 

		}	// END: if (match_exact_path_with_node(path, p_v))

	}	// END: for i
	
	
	//
	// The renamed attributes has NOT BEEN STORED YET.
	// Otherwise the new name should be used in the Recipe, if the value must be changed, too
	// It has been just written to a temporary vector of pending operations
	// 
    size_t att_t_c_size = attributes_to_change.size() ;
	for (size_t j=0; j<att_t_c_size; ++j)
	{
		const size_t attr_index ( attributes_to_change.at(j).first ) ; 
		const string attr_new_name ( attributes_to_change.at(j).second ) ; 
		
		xml_tag->_attributes.at( attr_index ).first = attr_new_name ;
	}
	
	const bool r ( success != SUBST_instance::TAG_UNDEFINED ) ;

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream msg; msg << exDebugShortSignatureFile << "Method finished.  returing: " << (r?"TRUE":"FALSE") ; 
		ACS_LOG_DEBUG(msg.str());
	ACS_CLASS_END_DEBUG
	
	return r ; 
}



////////////////////////////////////////////////////////////////////////////////////////
//		 check_tag_to_drop		
////////////////////////////////////////////////////////////////////////////////////////
bool XMLProcessor::check_tag_to_drop(const string& path_in) 
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream msg; msg << exDebugShortSignatureFile << "Method called.  path: \"" << path_in << "\"" ;
		ACS_LOG_DEBUG(msg.str());
	ACS_CLASS_END_DEBUG
			
	const bool removeLeadingSlash (true), removeLeadingDot(true) ;
	bool success (false) ; 

	// The absolute_path is computed using the _base_path the engine was initialized with
	// _base_path is EMPTY in the first instance, and is filled with the local path of the call of __FROM_INPUT
	string abs_path (_base_path) ; 

	// The input path is disassembled in order to remove any "__APPLY_SUBST" subpath occurence 
	// (e.g. "/root/__APPLY_SUBST/__APPLY_SUBST/tag" is simplified to "/root/tag")
	// This is only useful to apply a LUT inside the recipe file itself, without injecting a file from outside.
	// Without this, the path to locally match the LUT should contain the "/__APPLY_SUBST" layer(s)
	{
		const vector<string> p_v ( StringUtils::pathDisassemble(path_in, removeLeadingSlash, removeLeadingDot) ) ;
		
		for (size_t i=0; i<p_v.size(); i++)
		{ if ( (p_v.at(i) != TAG_CMD__APPLY_SUBST) && (p_v.at(i) != TAG_CMD__SORT_TAGS) && (p_v.at(i) != TAG_CMD__FROM_INPUT) \
						&& (p_v.at(i) != TAG_CMD__HEADER) && (p_v.at(i) != TAG_CMD__EXECUTE_COMMAND) && (p_v.at(i) != TAG_CMD__CALL_FUNCTION))
		{ abs_path = StringUtils::pathJoin(abs_path, p_v.at(i)) ; } }
	}

    size_t s_i_V_size = _subst_instances_V.size() ;
	for (size_t i=0; i<s_i_V_size ; ++i)
	{
		const SUBST_instance& subst_instance( _subst_instances_V.at(i) ) ;
		const SUBST_instance::SUBST_Type& type (subst_instance.getType()) ;

		if (type == SUBST_instance::SUBST_DROP)
		{
			ACS_CLASS_BGN_DEBUG(ACS_INS_VERB)
				ostringstream msg; msg << exDebugShortSignatureFile << "Dumping subst_instance #" << i << " of type: " << SUBST_instance::SUBST_Type_to_String(type) << endl << subst_instance.dump();
				ACS_LOG_DEBUG(msg.str());
			ACS_CLASS_END_DEBUG

			const SUBST_instance::Match_Element_Rule match_element_rule ( subst_instance.getMatch_Element_Rule() ) ;
			string match_element ( subst_instance.getMatch_Element() ) ;

			vector<string> p_v = StringUtils::pathDisassemble(match_element, removeLeadingSlash, removeLeadingDot) ;
			string elem_to_match ( not p_v.empty() ? (p_v.at( p_v.size()-1 )) : "" ) ;

			ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
				ostringstream msg; msg << exDebugShortSignatureFile << "Path to match: \"" << match_element 
																	<< "\"  type: " << SUBST_instance::Match_Element_Rule_to_String(match_element_rule) ;
				ACS_LOG_DEBUG(msg.str());
			ACS_CLASS_END_DEBUG


//  UNSUPPORTED (YET)
//
//	if ( p_v.size() && ( (match_element_rule & SUBST_instance::TAG_ATTR_NAME) )
//	{
//		const string needle ("###") ;
//		const size_t found ( elem_to_match.find(needle) ) ;
//		if (found != string::npos)
//		{
//			attr_to_match = elem_to_match.substr( found + needle.size() ) ;	// Do not move from here
//			elem_to_match = elem_to_match.substr( 0, found ) ;				// Do not move from here
//			p_v.at( p_v.size()-1 ) = elem_to_match ;
//		}
//	}
//
//
//

			ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
				ostringstream msg; msg << exDebugShortSignatureFile << "Matching abs_path: \"" << abs_path << "\" with LUT element: " << StringUtils::pathAssemble(p_v) ;
				ACS_LOG_DEBUG(msg.str());
			ACS_CLASS_END_DEBUG

			if (match_path_contained_in_node(abs_path, p_v))	/* Match p_v (the LUT trigger) is enterely contained in the path under processing) */
			{
				ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
					ostringstream msg; msg << exDebugShortSignatureFile << "Comparison succesfull. Applying SUBST of type: " << SUBST_instance::SUBST_Type_to_String(type) 
						<< " to abs_path: \"" << abs_path << "\"" ;
					ACS_LOG_DEBUG(msg.str());
				ACS_CLASS_END_DEBUG

#if(0)	
// FIXME NOT HANDLED: APPARENTLY A CHECK AGAINST THE LUT (REGEXP?) WAS DONE.	NOW NO CHECK IS DONE EXCEPT THE PATH.	
				if (match_element_rule & SUBST_instance::TAG_NAME) 
				{
					bool lut_is_triggered(false) ;
					NEWLUT_Container ctr ( lut.apply(elem_to_match, lut_is_triggered) ) ;
					success = ( (ctr.getType() == SUBST_DROP) && lut_is_triggered ) ;
				}
#else
// FIXME SUCCESS IS FORCED
success = true ;
#endif

			}	// END: if (match_exact_path_with_node(path, p_v))

		}	// END: if (type == SUBST_DROP)
	
	}	// END: for i	
	

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream msg; msg << exDebugShortSignatureFile << "Method finished.  returing: " << (success?"TRUE":"FALSE") ; 
		ACS_LOG_DEBUG(msg.str());
	ACS_CLASS_END_DEBUG
	
	return success ; 
}


////////////////////////////////////////////////////////////////////////////////////////
//		exec_cmd__FROM_INPUT 		
////////////////////////////////////////////////////////////////////////////////////////
void XMLProcessor::exec_cmd__FROM_INPUT(xml_tag_struct* xml_tag_ptr, const string& base_path, const string& entry_file_id,
										const string& path, const bool is_regexp, const bool clone_root_tag)
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream msg; 
		msg << exDebugShortSignatureFile 
			<< "Method called.  ptr: " << xml_tag_ptr << "  base_path: \"" << base_path << "\"  entry point id: \"" << entry_file_id 
			<< "\"  input path: \"" << path << "\"  is_regexp? " << (is_regexp?"T":"F") 
			<< "  dst pointer: " <<xml_tag_ptr ;
		ACS_LOG_DEBUG(msg.str());
	ACS_CLASS_END_DEBUG

	
	ACS_COND_THROW ( (_filenames_in_M.find(entry_file_id) == _filenames_in_M.end()),  // PRQA S 3081
				exIllegalValueException("FATAL: file_id undeclared: \"" + entry_file_id + "\"" )) ;

	const string file_name (_filenames_in_M.at(entry_file_id)) ;
	
	if ( (! File::exists(file_name)) || (0==File::size(file_name)) ) 
	{
		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
			ostringstream msg; 
			msg << exDebugShortSignatureFile 
				<< "Skipping processing of unexisting/empty file: \"" << file_name << "\"  with id: \"" << entry_file_id << "\"" ; 
			ACS_LOG_DEBUG(msg.str());
		ACS_CLASS_END_DEBUG
	}
	else
	{
		ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
			ostringstream msg; 
			msg << exDebugShortSignatureFile 
				<< "Creating new XMLProcessor..." ;
			ACS_LOG_DEBUG(msg.str());
		ACS_CLASS_END_DEBUG
				
		const string new_base_path ( StringUtils::pathJoin(_base_path, base_path) )	;	// This allows a CHAIN of base_paths, if a __FROM_INPUT contains another __FROM_INPUT
		XMLProcessor add_from_input(xml_tag_ptr, new_base_path) ;
		add_from_input.setCloneRootTag(clone_root_tag) ;

		// Clone the same configuration (LUTs to apply, file IDs...)
		clone_env_to(&add_from_input) ;

		ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
			ostringstream msg; 
			msg << exDebugShortSignatureFile 
				<< "Running new XMLProcessor..." ;
			ACS_LOG_DEBUG(msg.str());
		ACS_CLASS_END_DEBUG
		add_from_input.process(entry_file_id, path, is_regexp) ;
		
		ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
			ostringstream msg; 
			msg << exDebugShortSignatureFile 
				<< "Destroying new XMLProcessor..." ;
			ACS_LOG_DEBUG(msg.str());
		ACS_CLASS_END_DEBUG
	}

	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
		ostringstream msg; 
		msg << exDebugShortSignatureFile << "Dumping ptr: " << xml_tag_ptr << "  after the execution of the command" << endl << xml_tag_ptr->dump() ;
		ACS_LOG_DEBUG(msg.str());
	ACS_CLASS_END_DEBUG
	
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream msg; 
		msg << exDebugShortSignatureFile 
			<< "Method finished." ;
		ACS_LOG_DEBUG(msg.str());
	ACS_CLASS_END_DEBUG
}


////////////////////////////////////////////////////////////////////////////////////////
//		exec_cmd__EXECUTE_COMMAND 		
////////////////////////////////////////////////////////////////////////////////////////
void XMLProcessor::exec_cmd__EXECUTE_COMMAND(xml_tag_struct* xml_tag_ptr, const string& base_path, \
										const bool clone_root_tag,  \
										const string& add_new_root, \
										const string& cmd_name, \
										const vector <string>& cmd_opts_V, \
										const map < string, pair<string, bool> > &input_files_M)	/* IN: file_id, path, path_is_regexp */
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream msg; 
		msg << exDebugShortSignatureFile 
			<< "Method called.  ptr: " << xml_tag_ptr << "  base_path: \"" << base_path << "\"" << endl 
			<< "\tadd_new_root: \"" << add_new_root << "\"" << endl 
			<< "\tcmd_name: \"" << cmd_name << "\"" << endl 
			<< "\tcmd opts:" << endl ;
		for (size_t i=0; i<cmd_opts_V.size(); i++) { msg << "\t\t\"" << cmd_opts_V[i] << "\"" << endl ; }
		msg << "\tinput files:" << endl ;
		for (map < string, pair<string, bool> >::const_iterator i=input_files_M.begin() ; i!=input_files_M.end() ; i++)
		{
			msg << "\t\tid: \"" << i->first << "\"" << endl ;	
			msg << "\t\txml path: \"" << (i->second).first << "\"" << endl ;	
			msg << "\t\tpath is regexp? " << ((i->second).second? "T":"F") << endl ;	
		}
		msg << "  dst pointer: " <<xml_tag_ptr ;
		ACS_LOG_DEBUG(msg.str());
	ACS_CLASS_END_DEBUG


	FileGuard processed_file(File::tempname("/tmp/file_p_", "xml")) ;
    File::touch(processed_file) ; // PRQA S 3050
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB) ; processed_file.detach() ; ACS_CLASS_END_DEBUG ;

	FileGuard tmp_file(File::tempname("/tmp/file_t_", "xml")) ;
	File::touch(tmp_file) ; // PRQA S 3050
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB) ; tmp_file.detach() ; ACS_CLASS_END_DEBUG ;

	FileGuard log_file(File::tempname("/tmp/file_log_", "txt")) ;
	File::touch(log_file) ; // PRQA S 3050 
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB) ; log_file.detach() ; ACS_CLASS_END_DEBUG ;

	vector < pair <string, string> > extracted_files_V ;
	vector <FileGuard> fd_extracted_files_V ;
	for (map < string, pair<string, bool> >::const_iterator k=input_files_M.begin() ; k!=input_files_M.end() ; k++)
	{
		{
			fd_extracted_files_V.push_back( File::tempname("/tmp/file_e_", "xml") ) ;
			extracted_files_V.push_back( pair<string, string>( k->first, fd_extracted_files_V.back() ) ) ;  // PRQA S 3050
			ACS_CLASS_BGN_DEBUG(ACS_MID_VERB) ; (fd_extracted_files_V.back()).detach() ; ACS_CLASS_END_DEBUG ;
		}		
		const string extracted_file( (extracted_files_V.back()).second ) ; File::touch(extracted_file) ;

		{
			ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
				ostringstream msg; 
				msg << exDebugShortSignatureFile 
					<< "Creating new XMLProcessor to extract the input file..." ;
				ACS_LOG_DEBUG(msg.str());
			ACS_CLASS_END_DEBUG

			const string new_base_path ( StringUtils::pathJoin(_base_path, base_path) )	;	// This allows a CHAIN of base_paths, if a __FROM_INPUT contains another __FROM_INPUT
			XMLProcessor process_input(new_base_path) ;
			process_input.setCloneRootTag(clone_root_tag) ;

			// Clone the same configuration (LUTs to apply, file IDs...)
			clone_env_to(&process_input) ;

			ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
				ostringstream msg; 
				msg << exDebugShortSignatureFile 
					<< "Running new XMLProcessor and writing result to temporary file: \"" << extracted_file << "\"" ;
				ACS_LOG_DEBUG(msg.str());
			ACS_CLASS_END_DEBUG

			const bool add_header_to_xml (false) ; 
			process_input.process(k->first, (k->second).first, (k->second).second, extracted_file, add_header_to_xml) ; 

			ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
				ostringstream msg; 
				msg << exDebugShortSignatureFile 
					<< "Destroying new XMLProcessor..." ;
				ACS_LOG_DEBUG(msg.str());
			ACS_CLASS_END_DEBUG

		}
	}	// END: for (map < string, pair<string, bool> >::const_iterator k=input_files_M.begin() ; k!=input_files_M.end() ; k++)
	
	int ret_code (-1) ; 
	string ret_message ;

	try
	{
		vector<string> args ;
		args.push_back(processed_file) ;  // PRQA S 3050 2
		args.push_back(log_file) ; 
		args.push_back("-n") ; // Number of inputs, followed by the inputs as: key filename. Example: -n 2 main /tmp/mainfile.xml data /tmp/datafile.xml
		{ ostringstream o ; o << extracted_files_V.size() ; args.push_back(o.str()) ; }
		size_t e_f_V_size = extracted_files_V.size() ;
        for (size_t i=0; i<e_f_V_size ; ++i) { 
			args.push_back(extracted_files_V.at(i).first) ; 
			args.push_back(extracted_files_V.at(i).second) ; 
		}
        size_t c_o_V_size = cmd_opts_V.size();
		for (size_t i=0; i<c_o_V_size; ++i) { args.push_back( cmd_opts_V.at(i) ) ; }

		ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
			ostringstream msg; 
			msg << exDebugShortSignatureFile 
				<< "Running command cmd_name: \"" << cmd_name 
				<< "\"  options: " ;
				size_t args_size_ = args.size() ;
                for (size_t i=0; i<args_size_; ++i) { msg << "\"" << args.at(i) << "\"" << endl ; }
			ACS_LOG_DEBUG(msg.str());
		ACS_CLASS_END_DEBUG

    	SpawnProc proc(cmd_name) ;
        size_t args_size = args.size(); 
		for (size_t i=0; i<args_size; ++i) { proc.args()[i] = args.at(i) ; }

		const bool detached (true) ; 
    	proc.spawn(detached);
		while(proc.isRunning()) { usleep (1000) ; }
		if ( proc.exitCode(ret_code) == false ) { ret_code = -999 ; } 
		if ( 0 != ret_code )
		{
			ostringstream msg; 
			msg << "Status code " << ret_code << " running command: \"" << cmd_name 
			<< "\"  options: " << endl ;
            size_t a_size = args.size() ;
			for (size_t i=0; i<a_size; ++i) { msg << "\t\"" << args.at(i) << "\"" << endl ; }
			ret_message = msg.str() ;
		}
	}
	catch (const exException& e) {
		ostringstream o ;
		o  << "Error running command: \"" << cmd_name << "\"" ;
		ACS_THROW(exIOException(e,o.str())) ; // PRQA S 3081
	}
	
	if ( File::exists(log_file) ) { concat_log_file(log_file) ; } // PRQA S 3050

	if (0 != ret_code)
	{
		ostringstream msg; 
		msg << "Return code: " << ret_code << "  obtained" << endl << ret_message ;
		ACS_THROW( exIOException(msg.str()) ) ; // PRQA S 3081
	}

	
	// Check if a new root should be added (attribute "root" when setting the function)
	if (add_new_root.empty() == false){ encapsuleNewRoot(processed_file, processed_file, add_new_root) ; } // PRQA S 3050

	{
		ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
			ostringstream msg; 
			msg << exDebugShortSignatureFile 
				<< "Creating new XMLProcessor to import the processed file..." ;
			ACS_LOG_DEBUG(msg.str());
		ACS_CLASS_END_DEBUG
				
		XMLProcessor add_from_input(xml_tag_ptr, "") ;
		// Since this XMLProcessor should only ADD the output of the external command, without any real processing, 
		// the environment will not be cloned.

		// Add a fixed keyword to handle the input: processed_file
		add_from_input.addInputFile( "__xml_to_import__", processed_file ) ; // PRQA S 3050

		ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
			ostringstream msg; 
			msg << exDebugShortSignatureFile 
				<< "Running new XMLProcessor..." ;
			ACS_LOG_DEBUG(msg.str());
		ACS_CLASS_END_DEBUG

		add_from_input.process( "__xml_to_import__", "/", false ) ; 
		
		ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
			ostringstream msg; 
			msg << exDebugShortSignatureFile 
				<< "Destroying new XMLProcessor..." ;
			ACS_LOG_DEBUG(msg.str());
		ACS_CLASS_END_DEBUG
	}


	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
		ostringstream msg; 
		msg << exDebugShortSignatureFile << "Dumping ptr: " << xml_tag_ptr << "  after the execution of the command" << endl << xml_tag_ptr->dump() ;
		ACS_LOG_DEBUG(msg.str());
	ACS_CLASS_END_DEBUG
	
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream msg; 
		msg << exDebugShortSignatureFile 
			<< "Method finished." ;
		ACS_LOG_DEBUG(msg.str());
	ACS_CLASS_END_DEBUG
}


////////////////////////////////////////////////////////////////////////////////////////
//		exec_cmd__CALL_FUNCTION		
////////////////////////////////////////////////////////////////////////////////////////
void XMLProcessor::exec_cmd__CALL_FUNCTION(xml_tag_struct* xml_tag_ptr, const string& base_path, const bool clone_root_tag_in,  const bool clone_root_tag_out, const string& add_new_root_out, \
										const string& base_out_path, const bool base_out_path_is_regexp, \
										const string& funct_id, int (*funct_ptr) (ACS_XMLPROCESSOR__FUNCT_SIGNATURE), \
										const vector <std::string>& funct_params_V, \
										const map < string, pair<string, bool> > &input_files_M)	/* IN: file_id, path, path_is_regexp */
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream msg; 
		msg << exDebugShortSignatureFile 
			<< "Method called.  ptr: " << xml_tag_ptr << "  base_path: \"" << base_path << "\"" << endl 
			<< "\tadd_new_root_out: \"" << add_new_root_out << "\"" << endl 
			<< "\tfunct_id: \"" << funct_id << "\"" << endl 
			<< "\tcmd opts:" << endl ;
		for (size_t i=0; i<funct_params_V.size(); i++) { msg << "\t\t\"" << funct_params_V[i] << "\"" << endl ; }
		msg << "\tinput files:" << endl ;
		for (map < string, pair<string, bool> >::const_iterator i=input_files_M.begin() ; i!=input_files_M.end() ; i++)
		{
			msg << "\t\tid: \"" << i->first << "\"" << endl ;	
			msg << "\t\txml path: \"" << (i->second).first << "\"" << endl ;	
			msg << "\t\tpath is regexp? " << ((i->second).second? "T":"F") << endl ;	
		}
		msg << "  dst pointer: " <<xml_tag_ptr ;
		ACS_LOG_DEBUG(msg.str());
	ACS_CLASS_END_DEBUG


	FileGuard processed_file(File::tempname("/tmp/file_p_", "xml")) ;
	File::touch(processed_file) ; // PRQA S 3050
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB) ; processed_file.detach() ; ACS_CLASS_END_DEBUG ;
/*
	FileGuard fg_tmp_file(File::tempname("/tmp/file_t_", "xml")) ;
	File::touch(fg_tmp_file) ;
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB) ; fg_tmp_file.detach() ; ACS_CLASS_END_DEBUG ;
*/
	FileGuard log_file(File::tempname("/tmp/file_log_", "txt")) ;
	File::touch(log_file) ; // PRQA S 3050
	ACS_CLASS_BGN_DEBUG(ACS_MID_VERB) ; log_file.detach() ; ACS_CLASS_END_DEBUG ;

	vector < pair <string, string> > extracted_files_V ;
	vector <FileGuard> fd_extracted_files_V ;
	for (map < string, pair<string, bool> >::const_iterator k=input_files_M.begin() ; k!=input_files_M.end() ; k++)
	{
		{
			fd_extracted_files_V.push_back( File::tempname("/tmp/file_e_", "xml") ) ;
			extracted_files_V.push_back( pair<string, string> ( k->first, fd_extracted_files_V.back() ) ) ; // Do not use make_pair (with automatic type detection) since FileGuard would be returned by back (and not a string), creating a temporary object // PRQA S 3050
			ACS_CLASS_BGN_DEBUG(ACS_MID_VERB) ; (fd_extracted_files_V.back()).detach() ; ACS_CLASS_END_DEBUG ;
		}		
		const string extracted_file( (extracted_files_V.back()).second ) ; File::touch(extracted_file) ;

		{
			ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
				ostringstream msg; 
				msg << exDebugShortSignatureFile 
					<< "Creating new XMLProcessor to extract the input file..." ;
				ACS_LOG_DEBUG(msg.str());
			ACS_CLASS_END_DEBUG

			const string new_base_path ( StringUtils::pathJoin(_base_path, base_path) )	;	// This allows a CHAIN of base_paths, if a __FROM_INPUT contains another __FROM_INPUT
			XMLProcessor process_input(new_base_path) ;
			process_input.setCloneRootTag(clone_root_tag_in) ;

			// Clone the same configuration (LUTs to apply, file IDs...)
			clone_env_to(&process_input) ;

			ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
				ostringstream msg; 
				msg << exDebugShortSignatureFile 
					<< "Running new XMLProcessor and writing result to temporary file: \"" << extracted_file << "\"" ;
				ACS_LOG_DEBUG(msg.str());
			ACS_CLASS_END_DEBUG

			const bool add_header_to_xml (false) ; 
			process_input.process(k->first, (k->second).first, (k->second).second, extracted_file, add_header_to_xml) ;

			ACS_CLASS_BGN_DEBUG(ACS_MID_VERB) ostringstream msg;  msg << exDebugShortSignatureFile  << "Destroying new XMLProcessor..." ; ACS_LOG_DEBUG(msg.str());
			ACS_CLASS_END_DEBUG

		}
	}	// END: for (map < string, pair<string, bool> >::const_iterator k=input_files_M.begin() ; k!=input_files_M.end() ; k++)
	
	int ret_code (-1) ; 
	string ret_message ;

	try
	{
		ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
			ostringstream msg; 
			msg << exDebugShortSignatureFile 
				<< "Calling funct_id: \"" << funct_id 
				<< "\"  inputs: " ;
                size_t e_f_V_size = extracted_files_V.size(); 
				for (size_t i=0; i<e_f_V_size; ++i) { msg << "[\"" 
					<< extracted_files_V.at(i).first << "\"]:  \"" <<  extracted_files_V.at(i).second << "\"" << endl ; }
				msg << "\"  output: " << string(processed_file) ; // PRQA S 3050, 3081
				msg << "\"  params: " ;
				size_t f_p_V_size = funct_params_V.size(); 
                for (size_t i=0; i<f_p_V_size; i++) { msg << "\"" << funct_params_V.at(i) << "\"" << endl ; }
				msg << "\"  log file: " << string(log_file) ; // PRQA S 3050, 3081
			ACS_LOG_DEBUG(msg.str());
		ACS_CLASS_END_DEBUG

		ret_code = (*funct_ptr) (extracted_files_V, funct_params_V, processed_file, _env_M, log_file) ; // PRQA S 3050

		if ( 0 != ret_code )
		{
			ostringstream msg; 
			msg << "Status code " << ret_code << " after calling funct_id: \"" << funct_id << endl
				<< "\"\tthese input files of the function won't be removed:" << endl ;
				size_t e_f_V_size = extracted_files_V.size();
                for (size_t i=0; i<e_f_V_size; ++i) { msg << "\t\t[\"" 
					<< extracted_files_V.at(i).first << "\"]:  \"" <<  extracted_files_V.at(i).second << "\"" << endl ; }
				msg << "\"  output: " << string(processed_file) ; // PRQA S 3050, 3081
				msg << "\"  params: " ;
				size_t f_p_V_size = funct_params_V.size() ;
                for (size_t i=0; i<f_p_V_size; ++i) { msg << "\"" << funct_params_V.at(i) << "\"" << endl ; }
				msg << "\"  log file: " << string(log_file) ; // PRQA S 3050, 3081
			ret_message = msg.str() ;
            size_t fd_e_V_size = fd_extracted_files_V.size() ; 
			for (size_t i=0; i<fd_e_V_size; ++i) { fd_extracted_files_V.at(i).detach() ; /* Prevent removal of temporary files */ } 
		}
	}
	catch (const exException& e) {
		ostringstream o ;
		o  << "Error calling funct_id: \"" << funct_id << "\"" << endl << "\tThese input files of the function won't be removed:" << endl ;
		size_t e_f_V_size = extracted_files_V.size() ;
        for (size_t i=0; i<e_f_V_size; ++i) { o << "\t\t[\"" 
					<< extracted_files_V.at(i).first << "\"]:  \"" <<  extracted_files_V.at(i).second << "\"" << endl ; }
		size_t fd_e_f_V_size = fd_extracted_files_V.size() ;
        for (size_t i=0; i<fd_e_f_V_size; ++i) { fd_extracted_files_V.at(i).detach() ; /* Prevent removal of temporary files */ } 
		ACS_THROW(exIOException(e,o.str())) ; // PRQA S 3081
	}
	
	if ( File::exists(log_file) ) { concat_log_file(log_file) ; } // PRQA S 3050 
	
	if (0 != ret_code)
	{
		ostringstream msg; 
		msg << "Return code: " << ret_code << "  obtained" << endl << ret_message ;
		ACS_THROW( exIOException(msg.str()) ) ; // PRQA S 3081
	}


	if (add_new_root_out.empty() == false) { encapsuleNewRoot(processed_file, processed_file, add_new_root_out) ; } // PRQA S 3050 

	{
		ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
			ostringstream msg; 
			msg << exDebugShortSignatureFile 
				<< "Creating new XMLProcessor to import the processed file..." ;
			ACS_LOG_DEBUG(msg.str());
		ACS_CLASS_END_DEBUG
				
		XMLProcessor add_from_input(xml_tag_ptr, "") ;
		// Since this XMLProcessor should only ADD the output of the external command, without any real processing, 
		// the environment will not be cloned.

		// Set the clone_root_tag_out when extracting the xml processed by the function
		add_from_input.setCloneRootTag(clone_root_tag_out) ; 

		// Add a fixed keyword to handle the input: processed_file
		add_from_input.addInputFile( "__xml_to_import__", processed_file ) ; // PRQA S 3050

		ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
			ostringstream msg; 
			msg << exDebugShortSignatureFile 
				<< "Running new XMLProcessor..." ;
			ACS_LOG_DEBUG(msg.str());
		ACS_CLASS_END_DEBUG

		add_from_input.process( "__xml_to_import__", base_out_path, base_out_path_is_regexp ) ;

		ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
			ostringstream msg; 
			msg << exDebugShortSignatureFile 
				<< "Destroying new XMLProcessor..." ;
			ACS_LOG_DEBUG(msg.str());
		ACS_CLASS_END_DEBUG
	}


	ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
		ostringstream msg; 
		msg << exDebugShortSignatureFile << "Dumping ptr: " << xml_tag_ptr << "  after the execution of the command" << endl << xml_tag_ptr->dump() ;
		ACS_LOG_DEBUG(msg.str());
	ACS_CLASS_END_DEBUG
	
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream msg; 
		msg << exDebugShortSignatureFile 
			<< "Method finished." ;
		ACS_LOG_DEBUG(msg.str());
	ACS_CLASS_END_DEBUG
}


////////////////////////////////////////////////////////////////////////////////////////
//		nodeMatch 		
////////////////////////////////////////////////////////////////////////////////////////
bool XMLProcessor::nodeMatch(XMLExtractorHandlerBase &xh, const string &node_in, const XERCES_CPP_NAMESPACE::AttributeList &attr_in, const XMLCh* name) 
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream msg; 
		msg << exDebugShortSignatureFile	<< "Method called.  node_in: \"" << node_in << "\"" << "  PTR (will become the parent) : " << _xml_tag_ptr \
											<< "  *******************************************************" ; 
		ACS_LOG_DEBUG(msg.str());
	ACS_CLASS_END_DEBUG

	string node_to_store ;
	const string node (StringUtils::pathNormalize(node_in)) ; 

	const bool removeLeadingSlash (true), removeLeadingDot(true) ;

	// Look for the header (or header sub components) even if outside the node ("path") matching region ...
	{
		vector<string> p_v = StringUtils::pathDisassemble(node, removeLeadingSlash, removeLeadingDot) ;

		if ((p_v.size()>=1) && (p_v.at( p_v.size()-1 ) == TAG_CMD__HEADER) )
		{
			// Looks for the header to begin
			ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
				ostringstream msg; 
				msg << exDebugShortSignatureFile << "A new node matched the filter. Processing: \"" << node_to_store << "\"" ;
				ACS_LOG_DEBUG(msg.str());
			ACS_CLASS_END_DEBUG

			process_header(_filenames_in_M.at(_entry_point), node_in) ;
			return false;
		} else if (p_v.size()>=2)
		{
			// Check if the HEADER is found as sub path, e.g. /root1/__HEADER/tag : It will be skipped since the header has been already processed in the step above
			size_t p_v_size_1 = p_v.size()-1 ;
            for (size_t i=0; i<p_v_size_1; ++i) { 
                if (p_v.at( i ) == TAG_CMD__HEADER) {
					ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
						ostringstream msg; 
						msg << exDebugShortSignatureFile << "Skipping header component (" << i << "): \"" << node_in << "\"" ;
						ACS_LOG_DEBUG(msg.str());
					ACS_CLASS_END_DEBUG

					return false ; 
				}	// END: if
			}	// END: for
		}
        else { /* nothing */ }

	}

	// WHEN HERE: no component of header


	if ( match_node_V(node, _clone_root_tag, node_to_store) )
	{
		// This is true only if level of "node" MATCHES _nodeV: if (node_to_store==false) is GRATER than the root _node_V. Otherwise is GREATER_OR_EQUAL

		// Check invalid value
		ACS_COND_THROW( (node_to_store==""), exIllegalValueException("FATAL: Node to store seems empty: \""+node_to_store+"\"") ) ; // PRQA S 3081

		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
			ostringstream msg; 
			msg << exDebugShortSignatureFile << "A new node matched the filter. Processing: \"" << node_to_store << "\"" ;
			ACS_LOG_DEBUG(msg.str());
		ACS_CLASS_END_DEBUG

		vector<string> p_v = StringUtils::pathDisassemble(node_to_store, removeLeadingSlash, removeLeadingDot) ;

		const string node_rel ( not p_v.empty()? p_v.at( p_v.size()-1 ) : "" ) ;	// Deepest entry in the path


		if (node_rel == TAG_CMD__FROM_INPUT)
		{
			ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
				ostringstream msg; 
				msg << exDebugShortSignatureFile << "Got command: \"" << TAG_CMD__FROM_INPUT << "\"" ;
				ACS_LOG_DEBUG(msg.str());
			ACS_CLASS_END_DEBUG

			const multimap <string, string> attr_M ( xml_tag_struct::convert_attributes(attr_in) ) ;
			string attr ; 
			
			attr = "file_id" ; 
			ACS_COND_THROW ( (attr_M.find(attr)==attr_M.end()),  // PRQA S 3081
				exIllegalValueException(TAG_CMD__FROM_INPUT + ": attribute not found: \"" + attr + "\"" )) ;
			const string file_id ( attr_M.find(attr)->second ) ;
			
			// Check if the current root should be cloned (or the level skipped)
			attr = "clone_root_tag" ;
			bool clone_root_tag = true ; /* default value */
			if ( attr_M.find(attr)!=attr_M.end() )
			{
				const string c ( attr_M.find(attr)->second ) ;
				if ( (StringUtils::equalsNoCase(c, "f")) || (StringUtils::equalsNoCase(c, "false")) || (StringUtils::equalsNoCase(c, "0")) ) { clone_root_tag = false ; } 
				else if ( (StringUtils::equalsNoCase(c, "t")) || (StringUtils::equalsNoCase(c, "true")) || (StringUtils::equalsNoCase(c, "1")) ) { clone_root_tag = true ; } 
                else { /* nothing */ }
			}
			
			string path("") ; bool is_regexp(false) ; 
			attr = "path_regexp" ; 
			if (attr_M.find(attr)!=attr_M.end()) { is_regexp = true ; path = ( attr_M.find(attr)->second ) ; }
			
			if (path.empty())
			{
				attr = "path" ; 
				if (attr_M.find(attr)!=attr_M.end()) { is_regexp = false ; path = ( attr_M.find(attr)->second ) ; }
			}

			ACS_COND_THROW ( path.empty(),  // PRQA S 3081
				exIllegalValueException(TAG_CMD__FROM_INPUT + ": mandatory attribute path/path_regexp not found" )) ;

			ACS_COND_THROW ( (_filenames_in_M.find(file_id) == _filenames_in_M.end()),  // PRQA S 3081
				exIllegalValueException(TAG_CMD__FROM_INPUT + ": file_id undeclared: \"" + file_id + "\"" )) ;

			
			ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
				ostringstream msg; 
				msg << exDebugShortSignatureFile << "Running command: exec_cmd__FROM_INPUT" ; 
				ACS_LOG_DEBUG(msg.str());
			ACS_CLASS_END_DEBUG

			string base_path ;
			{
				// Remove the occurrences of any command from the path
				const vector<string> p_v_inner ( StringUtils::pathDisassemble(node_to_store, removeLeadingSlash, removeLeadingDot) ) ;
                
                size_t p_v_i_size = p_v_inner.size() ;
				for (size_t i=0; i<p_v_i_size; ++i)
				{ if ( (p_v_inner.at(i) != TAG_CMD__APPLY_SUBST) && (p_v_inner.at(i) != TAG_CMD__SORT_TAGS)  && (p_v_inner.at(i) != TAG_CMD__FROM_INPUT) && (p_v_inner.at(i) != TAG_CMD__HEADER) )
				{ base_path = StringUtils::pathJoin(base_path, p_v_inner.at(i)) ; } }
			}

			exec_cmd__FROM_INPUT(_xml_tag_ptr, base_path, file_id, path, is_regexp, clone_root_tag) ;

			ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
				ostringstream msg; 
				msg << exDebugShortSignatureFile << "Finished command: exec_cmd__FROM_INPUT" ; 
				ACS_LOG_DEBUG(msg.str());
			ACS_CLASS_END_DEBUG

			// The command is added (otherwise, the same level of the command would be ignored from the parser)
			// The point is that NO PUSH has been performed, therefore no pop should be done !
			return true ; 				
			
		} 
		else if (node_rel == TAG_CMD__EXECUTE_COMMAND)
		{
			ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
				ostringstream msg; 
				msg << exDebugShortSignatureFile << "Got command: \"" << TAG_CMD__EXECUTE_COMMAND << "\"" ;
				ACS_LOG_DEBUG(msg.str());
			ACS_CLASS_END_DEBUG

			map < string, pair<string, bool> > input_files_M ;

			const multimap <string, string> attr_M ( xml_tag_struct::convert_attributes(attr_in) ) ;
			string attr ;
			
			attr = "cmd_id" ; 
			ACS_COND_THROW ( (attr_M.find(attr)==attr_M.end()),  // PRQA S 3081
				exIllegalValueException(TAG_CMD__EXECUTE_COMMAND + ": attribute not found: \"" + attr + "\"" )) ;
			const string cmd_id ( attr_M.find(attr)->second ) ;
			
			ACS_COND_THROW ( (_executables_M.find(cmd_id) == _executables_M.end()),  // PRQA S 3081
				exIllegalValueException(TAG_CMD__EXECUTE_COMMAND + ": cmd_id undeclared: \"" + cmd_id + "\"" )) ;
			const string cmd_name ( _executables_M.at(cmd_id) ) ; // Set the executable name
						
			try {
				File::which(cmd_name) ; 		
			} catch (const exException& e) {
				ostringstream o ;
				o << TAG_CMD__EXECUTE_COMMAND << ":  Cannot find executable with id: \"" << cmd_id << "\". Error running Which on: \"" << cmd_name << "\"" ;
				ACS_THROW(exIllegalValueException(e,o.str())) ; // PRQA S 3081
			}
			
			string cmd_opts("") ; 
			attr = "cmd_opts" ; 
			if (attr_M.find(attr)!=attr_M.end()) { cmd_opts = ( attr_M.find(attr)->second ) ; }

			for (ssize_t k=-1; k<=255; k++)
			{
				ostringstream p ; if (k>=0) { p << "_" << k ; }  
				
				const string attr_file_id ( "file_id" + p.str() ) ; 
				const string attr_path( "path" + p.str() ) ; 
				const string attr_path_regexp ( "path_regexp" + p.str() ) ; 
				
				if (attr_M.find(attr_file_id)!=attr_M.end()) 
				{ 
					// Attribute "file_id" (or "file_id_NNN") was found
				
					const string file_id ( attr_M.find(attr_file_id)->second ) ;

					string path("") ; bool is_regexp(false) ; 
					if (attr_M.find(attr_path_regexp)!=attr_M.end()) { is_regexp = true ; path = ( attr_M.find(attr_path_regexp)->second ) ; }

					if (path.empty())
					{
						if (attr_M.find(attr_path)!=attr_M.end()) { is_regexp = false ; path = ( attr_M.find(attr_path)->second ) ; }
					}

					ACS_COND_THROW ( path.empty(),  // PRQA S 3081
						exIllegalValueException(TAG_CMD__EXECUTE_COMMAND + ": cmd_id: " + cmd_id +"  mandatory attribute " + attr_path + "/" + attr_path_regexp + " not found" )) ;

					ACS_COND_THROW ( (_filenames_in_M.find(file_id) == _filenames_in_M.end()),  // PRQA S 3081
						exIllegalValueException(TAG_CMD__EXECUTE_COMMAND + ": " + attr_file_id + " undeclared: \"" + file_id + "\"" )) ;

					input_files_M.insert ( make_pair (file_id, make_pair(path, is_regexp) ) ) ;
				}	// Attribute "file_id" (or "file_id_NNN") was found
		
			}	// END: for (ssize_t k=-1; k<=255; k++)

			// Check if a new root should be addded (encapsulated)
			attr = "root" ; 
			const string add_new_root ( (attr_M.find(attr)==attr_M.end())? "" : attr_M.find(attr)->second ) ;

			// Check if the current root should be cloned (or the level skipped)
			attr = "clone_root_tag" ;
			bool clone_root_tag = true ; /* default value */
			if ( attr_M.find(attr)!=attr_M.end() )
			{
				const string c ( attr_M.find(attr)->second ) ;
				if ( (StringUtils::equalsNoCase(c, "f")) || (StringUtils::equalsNoCase(c, "false")) || (StringUtils::equalsNoCase(c, "0")) ) { clone_root_tag = false ; } 
				else if ( (StringUtils::equalsNoCase(c, "t")) || (StringUtils::equalsNoCase(c, "true")) || (StringUtils::equalsNoCase(c, "1")) ) { clone_root_tag = true ; } 
                else { /* nothing */ }
			}


			ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
				ostringstream msg; 
				msg << exDebugShortSignatureFile << "Running command: " << TAG_CMD__EXECUTE_COMMAND; 
				ACS_LOG_DEBUG(msg.str());
			ACS_CLASS_END_DEBUG

			string base_path ; 
			{
				// Remove the occurrences of any command from the base_path
				const vector<string> p_v_inner ( StringUtils::pathDisassemble(node_to_store, removeLeadingSlash, removeLeadingDot) ) ;
                size_t p_v_i_size = p_v_inner.size() ;
				for (size_t i=0; i<p_v_i_size; ++i)
				{ 
					if ( (p_v_inner.at(i) != TAG_CMD__APPLY_SUBST) && (p_v_inner.at(i) != TAG_CMD__SORT_TAGS) && (p_v_inner.at(i) != TAG_CMD__FROM_INPUT) \
						&& (p_v_inner.at(i) != TAG_CMD__HEADER) && (p_v_inner.at(i) != TAG_CMD__EXECUTE_COMMAND))
					{ base_path = StringUtils::pathJoin(base_path, p_v_inner.at(i)) ; } 
				}
			}
			
			vector <string> cmd_opts_V ; 
			if (!cmd_opts.empty()) { StringUtils::Tokenize(cmd_opts, cmd_opts_V, " ") ; }
			
			exec_cmd__EXECUTE_COMMAND(_xml_tag_ptr, base_path, clone_root_tag, add_new_root, cmd_name, cmd_opts_V, input_files_M) ;

			ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
				ostringstream msg; 
				msg << exDebugShortSignatureFile << "Finished command: " << TAG_CMD__EXECUTE_COMMAND ;
				ACS_LOG_DEBUG(msg.str());
			ACS_CLASS_END_DEBUG

			// The command is added (otherwise, the same level of the command would be ignored from the parser)
			// The point is that NO PUSH has been performed, therefore no pop should be done !
			return true ; 				

		} 
		else if (node_rel == TAG_CMD__CALL_FUNCTION)
		{
			ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
				ostringstream msg; 
				msg << exDebugShortSignatureFile << "Got command: \"" << TAG_CMD__CALL_FUNCTION << "\"" ;
				ACS_LOG_DEBUG(msg.str());
			ACS_CLASS_END_DEBUG

			map < string, pair<string, bool> > input_files_M ;

			const multimap <string, string> attr_M ( xml_tag_struct::convert_attributes(attr_in) ) ;
			string attr ;
			
			attr = "funct_id" ; 
			ACS_COND_THROW ( (attr_M.find(attr)==attr_M.end()),  // PRQA S 3081
				exIllegalValueException(TAG_CMD__CALL_FUNCTION + ": attribute not found: \"" + attr + "\"" )) ;
			const string funct_id ( attr_M.find(attr)->second ) ;
						
			ACS_COND_THROW ( (_functs_M.find(funct_id) == _functs_M.end()),  // PRQA S 3081
				exIllegalValueException(TAG_CMD__CALL_FUNCTION + ": funct_id undeclared: \"" + funct_id + "\"" )) ;
			int (*funct_ptr) (ACS_XMLPROCESSOR__FUNCT_SIGNATURE) = _functs_M.at(funct_id) ; // Assign the function pointer to funct_ptr
						
			string funct_params("") ; 
			attr = "funct_params" ; 
			if (attr_M.find(attr)!=attr_M.end()) { funct_params = ( attr_M.find(attr)->second ) ; }

			for (ssize_t k=-1; k<=255; k++)
			{
				ostringstream p ; if (k>=0) { p << "_" << k ; }  
				
				const string attr_file_id ( "file_id" + p.str() ) ; 
				const string attr_path( "path" + p.str() ) ; 
				const string attr_path_regexp ( "path_regexp" + p.str() ) ; 
				
				if (attr_M.find(attr_file_id)!=attr_M.end()) 
				{ 
					// Attribute "file_id" (or "file_id_NNN") was found
				
					const string file_id ( attr_M.find(attr_file_id)->second ) ;

					string path("") ; bool is_regexp(false) ; 
					if (attr_M.find(attr_path_regexp)!=attr_M.end()) { is_regexp = true ; path = ( attr_M.find(attr_path_regexp)->second ) ; }

					if (path.empty())
					{
						if (attr_M.find(attr_path)!=attr_M.end()) { is_regexp = false ; path = ( attr_M.find(attr_path)->second ) ; }
					}

					ACS_COND_THROW ( path.empty(),  // PRQA S 3081
						exIllegalValueException(TAG_CMD__CALL_FUNCTION + ": funct_id: " + funct_id + "  mandatory attribute " + attr_path + "/" + attr_path_regexp + " not found" )) ;

					ACS_COND_THROW ( (_filenames_in_M.find(file_id) == _filenames_in_M.end()),  // PRQA S 3081
						exIllegalValueException(TAG_CMD__CALL_FUNCTION + ": " + attr_file_id + " undeclared: \"" + file_id + "\"" )) ;

					input_files_M.insert ( make_pair (file_id, make_pair(path, is_regexp) ) ) ;
				}	// Attribute "file_id" (or "file_id_NNN") was found
		
			}	// END: for (ssize_t k=-1; k<=255; k++)

			// Check if a new root should be addded (encapsulated)
			attr = "root" ; 
			const string add_new_root ( (attr_M.find(attr)==attr_M.end())? "" : attr_M.find(attr)->second ) ;

			// Check if the root from the input file should be cloned when reading the input file (or the level skipped)
			attr = "clone_root_tag_in" ;
			bool clone_root_tag_in = true ; /* default value */
			if ( attr_M.find(attr)!=attr_M.end() )
			{
				const string c ( attr_M.find(attr)->second ) ;
				if ( (StringUtils::equalsNoCase(c, "f")) || (StringUtils::equalsNoCase(c, "false")) || (StringUtils::equalsNoCase(c, "0")) ) { clone_root_tag_in = false ; } 
				else if ( (StringUtils::equalsNoCase(c, "t")) || (StringUtils::equalsNoCase(c, "true")) || (StringUtils::equalsNoCase(c, "1")) ) { clone_root_tag_in = true ; } 
                else { /* nothing */ }
			}

			// Check if the root from the output file should be cloned when reading the file created by the function (or the level skipped)
			attr = "clone_root_tag_out" ;
			bool clone_root_tag_out = true ; /* default value */
			if ( attr_M.find(attr)!=attr_M.end() )
			{
				const string c ( attr_M.find(attr)->second ) ;
				if ( (StringUtils::equalsNoCase(c, "f")) || (StringUtils::equalsNoCase(c, "false")) || (StringUtils::equalsNoCase(c, "0")) ) { clone_root_tag_out = false ; } 
				else if ( (StringUtils::equalsNoCase(c, "t")) || (StringUtils::equalsNoCase(c, "true")) || (StringUtils::equalsNoCase(c, "1")) ) { clone_root_tag_out = true ; } 
                else { /* nothing */ }
			}


			string path_out("") ; bool path_out_is_regexp(false) ; 
			attr = "path_out_regexp" ; 
			if (attr_M.find(attr)!=attr_M.end()) { path_out_is_regexp = true ; path_out = ( attr_M.find(attr)->second ) ; }
			
			if (path_out.empty())
			{
				attr = "path_out" ; 
				if (attr_M.find(attr)!=attr_M.end()) 
				{ 
					path_out_is_regexp = false ;
					path_out = ( attr_M.find(attr)->second ) ; 
				}
				else
				{ 
					// If no path_out/path_out_regexp is provided, a "catchall" is used: "/.*" as regexp
					path_out_is_regexp = true ;
					path_out = "/.*" ;	
				}
			}


			ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
				ostringstream msg; 
				msg << exDebugShortSignatureFile << "Running command: " << TAG_CMD__CALL_FUNCTION; 
				ACS_LOG_DEBUG(msg.str());
			ACS_CLASS_END_DEBUG

			string base_path ; 
			{
				// Remove the occurrences of any command from the base_path
				const vector<string> p_v_inner ( StringUtils::pathDisassemble(node_to_store, removeLeadingSlash, removeLeadingDot) ) ;
                size_t p_v_i_size = p_v_inner.size() ;
				for (size_t i=0; i<p_v_i_size; ++i)
				{ 
					if ( (p_v_inner.at(i) != TAG_CMD__APPLY_SUBST) && (p_v_inner.at(i) != TAG_CMD__SORT_TAGS) && (p_v_inner.at(i) != TAG_CMD__FROM_INPUT) \
						&& (p_v_inner.at(i) != TAG_CMD__HEADER) && (p_v_inner.at(i) != TAG_CMD__CALL_FUNCTION))
					{ base_path = StringUtils::pathJoin(base_path, p_v_inner.at(i)) ; } 
				}
			}
			
			vector <string> funct_params_V ; 
			if (!funct_params.empty()) { StringUtils::Tokenize(funct_params, funct_params_V, " ") ; }
			
			exec_cmd__CALL_FUNCTION(	_xml_tag_ptr, base_path, clone_root_tag_in, clone_root_tag_out, add_new_root, path_out, path_out_is_regexp, \
										funct_id, funct_ptr, funct_params_V, input_files_M) ;

			ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
				ostringstream msg; 
				msg << exDebugShortSignatureFile << "Finished command: " << TAG_CMD__CALL_FUNCTION ;
				ACS_LOG_DEBUG(msg.str());
			ACS_CLASS_END_DEBUG

			// The command is added (otherwise, the same level of the command would be ignored from the parser)
			// The point is that NO PUSH has been performed, therefore no pop should be done !
			return true ; 				

		} 
		else if (node_rel == TAG_CMD__APPLY_SUBST)
		{
			ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
				ostringstream msg; 
				msg << exDebugShortSignatureFile << "Begin of command: \"" << TAG_CMD__APPLY_SUBST << "\"" ;
				ACS_LOG_DEBUG(msg.str());
			ACS_CLASS_END_DEBUG
			
			SUBST_instance::SUBST_Type subst_type (SUBST_instance::SUBST_UNDEFINED) ; 
			string subst_id ;

			const multimap <string, string> attr_M ( xml_tag_struct::convert_attributes(attr_in) ) ;
			string attr ; 
			
			
			attr = "lut_id" ;
			if (attr_M.find(attr)!=attr_M.end())
			{
				subst_id = ( attr_M.find(attr)->second ) ; subst_type = SUBST_instance::SUBST_LUT ; 
				ACS_COND_THROW((_luts_M.find(subst_id)==_luts_M.end()),  // PRQA S 3081
					exIllegalValueException(TAG_CMD__APPLY_SUBST + ": LUT NOT FOUND with id: \"" + subst_id + "\"" )) ;
			}

			attr = "drop" ; // Syntax is drop="" or drop="ANY"  ; 
			if (attr_M.find(attr)!=attr_M.end())
			{
				/* Nothing to check, nor id to get */
				subst_id = "" ; //  Unused for DROP
				subst_type = SUBST_instance::SUBST_DROP ;
			}

			attr = "add_tag_id" ;
			if (attr_M.find(attr)!=attr_M.end())
			{
				subst_id = ( attr_M.find(attr)->second ) ; subst_type = SUBST_instance::SUBST_ADD ;
				ACS_COND_THROW((_tags_to_add_M.find(subst_id)==_tags_to_add_M.end()),  // PRQA S 3081
					exIllegalValueException(TAG_CMD__APPLY_SUBST + ": TAG TO ADD NOT FOUND with id: \"" + subst_id + "\"" )) ; 
			}
			
			attr = "funct_id" ;
			if (attr_M.find(attr)!=attr_M.end())
			{
				subst_id = ( attr_M.find(attr)->second ) ; subst_type = SUBST_instance::SUBST_FUNCT ;
				ACS_COND_THROW((_functs_M.find(subst_id)==_functs_M.end()),  // PRQA S 3081
					exIllegalValueException(TAG_CMD__APPLY_SUBST + ": FUNCTION NOT FOUND with id: \"" + subst_id + "\"" )) ;
			}
							
			attr = "cmd_id" ;
			if (attr_M.find(attr)!=attr_M.end())
			{		
				subst_id = ( attr_M.find(attr)->second ) ; subst_type = SUBST_instance::SUBST_CMD ;
				ACS_COND_THROW((_executables_M.find(subst_id)==_executables_M.end()),  // PRQA S 3081
					exIllegalValueException(TAG_CMD__APPLY_SUBST + ": EXECUTABLE NOT FOUND with id: \"" + subst_id + "\"" )) ; 
			}
				
			ACS_COND_THROW((subst_type==SUBST_instance::SUBST_UNDEFINED),  // PRQA S 3081
				exIllegalValueException(TAG_CMD__APPLY_SUBST + ": SUBSTITUTION TYPE UNDEFINED" )) ;


			// WHEN HERE: The type is known and the subst_id is existing (except for DROP)

	
	
			bool path_is_rel (false) ; 
			attr = "rel_path_regexp" ; 
			ACS_COND_THROW ( (attr_M.find(attr)==attr_M.end()),  // PRQA S 3081
				exIllegalValueException(TAG_CMD__APPLY_SUBST + ": attribute not found: \"" + attr + "\" for id: \"" + subst_id + "\"" )) ;
			const string rel_path_regexp ( attr_M.find(attr)->second ) ;
			path_is_rel = true ; // TBD, an absolute path might be given

			attr = "match" ; 
			ACS_COND_THROW ( (attr_M.find(attr)==attr_M.end()),  // PRQA S 3081
				exIllegalValueException(TAG_CMD__APPLY_SUBST + ": attribute not found: \"" + attr + "\" for id: \"" + subst_id + "\"" )) ;
			const string match_type ( attr_M.find(attr)->second ) ;
			const SUBST_instance::Match_Element_Rule match_type_cvt ( SUBST_instance::String_to_Match_Element_Rule(match_type) ) ;
			ACS_COND_THROW ( (match_type_cvt == SUBST_instance::TAG_UNDEFINED),  // PRQA S 3081
				exIllegalValueException(TAG_CMD__APPLY_SUBST + ": type undefined: \"" + match_type + "\"  for subst_id: \"" + subst_id + "\"" )) ;
				
				
			vector <string> params_V ; 
			attr = "funct_params" ; 
			if (attr_M.find(attr)!=attr_M.end()) { const string funct_params ( attr_M.find(attr)->second ) ; StringUtils::Tokenize(funct_params, params_V, " ") ; }


			string abs_path ; 
			if (path_is_rel)
			{
				// Remove the occurrences of: __APPLY_SUBST from the path and create the "abs_path" for the lut, starting from the rel_path
				const vector<string> p_v_inner ( StringUtils::pathDisassemble(node_to_store, removeLeadingSlash, removeLeadingDot) ) ;
                size_t p_v_i_size = p_v_inner.size() ;
				for (size_t i=0; i<p_v_i_size; ++i)
				{ 
					if ( (p_v_inner.at(i) != TAG_CMD__APPLY_SUBST) && (p_v_inner.at(i) != TAG_CMD__SORT_TAGS) && (p_v_inner.at(i) != TAG_CMD__FROM_INPUT) \
						&& (p_v_inner.at(i) != TAG_CMD__HEADER) && (p_v_inner.at(i) != TAG_CMD__EXECUTE_COMMAND) && (p_v_inner.at(i) != TAG_CMD__CALL_FUNCTION))
					{ abs_path = StringUtils::pathJoin(abs_path, p_v_inner.at(i)) ; } 
				}
				
				abs_path = StringUtils::pathJoin(abs_path, rel_path_regexp) ; 
			}

			SUBST_instance inst(subst_type, subst_id, match_type_cvt, StringUtils::pathJoin(_base_path, abs_path)) ;
			inst.setOpts(params_V) ; // Set the function_params 

			_subst_instances_V.push_back(inst) ;

			ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
				ostringstream msg; 
				msg << exDebugShortSignatureFile << "Added SUBST of type: " << SUBST_instance::SUBST_Type_to_String(subst_type) \
							<< " with id: \"" << subst_id << "\" to the list of SUBST instancies. New size: " << _subst_instances_V.size() ;
				ACS_LOG_DEBUG(msg.str());
			ACS_CLASS_END_DEBUG

			return true ; // The command has a scope, so it will be added to the flow (and it will be removed later)
		}
		else if (node_rel == TAG_CMD__SORT_TAGS)
		{
			ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
				ostringstream msg; 
				msg << exDebugShortSignatureFile << "Begin of command: \"" << TAG_CMD__SORT_TAGS << "\"" ;
				ACS_LOG_DEBUG(msg.str());
			ACS_CLASS_END_DEBUG
			
			const multimap <string, string> attr_M ( xml_tag_struct::convert_attributes(attr_in) ) ;
			string attr ; 

			attr = "to_sort" ; 
			ACS_COND_THROW ( (attr_M.find(attr)==attr_M.end()),  // PRQA S 3081
				exIllegalValueException(TAG_CMD__SORT_TAGS + ": attribute not found: \"" + attr + "\"" )) ;
			const string to_sort ( attr_M.find(attr)->second ) ;

			bool path_is_rel (false) ; 
			attr = "rel_path_regexp" ; 
			ACS_COND_THROW ( (attr_M.find(attr)==attr_M.end()),  // PRQA S 3081
				exIllegalValueException(TAG_CMD__SORT_TAGS + ": attribute not found: \"" + attr + "\"" )) ;
			const string rel_path_regexp ( attr_M.find(attr)->second ) ;
			path_is_rel = true ; // TBD, an absolute path might be given

			string abs_path ; 
			if (path_is_rel)
			{
				// Remove the occurrences of: __APPLY_SUBST from the path and create the "abs_path" for the lut, starting from the rel_path
				const vector<string> p_v_inner ( StringUtils::pathDisassemble(node_to_store, removeLeadingSlash, removeLeadingDot) ) ;
				size_t p_v_i_size = p_v_inner.size() ;
                for (size_t i=0; i<p_v_i_size; ++i)
				{ 
					if ( (p_v_inner.at(i) != TAG_CMD__APPLY_SUBST) && (p_v_inner.at(i) != TAG_CMD__SORT_TAGS) && (p_v_inner.at(i) != TAG_CMD__FROM_INPUT) \
						&& (p_v_inner.at(i) != TAG_CMD__HEADER) && (p_v_inner.at(i) != TAG_CMD__EXECUTE_COMMAND) && (p_v_inner.at(i) != TAG_CMD__CALL_FUNCTION))
					{ abs_path = StringUtils::pathJoin(abs_path, p_v_inner.at(i)) ; } 
				}
				
				abs_path = StringUtils::pathJoin(abs_path, rel_path_regexp) ; 
			}

			const pair <string, string> p ( make_pair ( StringUtils::pathJoin(_base_path, abs_path), to_sort ) ) ;
			
			_sort_instances_V.push_back( p ) ;

			ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
				ostringstream msg; 
				msg << exDebugShortSignatureFile << "Added Expression: \"" << to_sort << "\" to the list. New size: " << _sort_instances_V.size() ;
				ACS_LOG_DEBUG(msg.str());
			ACS_CLASS_END_DEBUG

			return true ; // The command has a scope, so it will be added to the flow (and it will be removed later)
		}
		else 
		{
			// Check if the node has to be DROPPED (so, it will be ignored) or it's a standard node to be added
			const bool tag_to_drop ( check_tag_to_drop(node_to_store) ) ;
			if (tag_to_drop == false)
			{
				_xml_tag_ptr = _xml_tag_ptr->push_tag(node_to_store, attr_in) ; 

				const bool is_value (false) ; 
				apply_subst(node_to_store, _xml_tag_ptr, is_value) ; 
			}
			
			return (! tag_to_drop) ;
		}
	} 
	
	return false ; 
}	


////////////////////////////////////////////////////////////////////////////////////////
//		nodeValue		
////////////////////////////////////////////////////////////////////////////////////////
bool XMLProcessor::nodeValue(XMLExtractorHandlerBase &xh, const string &node_in, const string &value, const XMLCh* name) 
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream msg; 
		msg << exDebugShortSignatureFile	<< "Method called.  node_in: \"" << node_in << "\"" << "  PTR: " << _xml_tag_ptr \
											<< "  (Pnt: " <<  ((_xml_tag_ptr!=0)? _xml_tag_ptr->_parent : 0) \
											<< ")  *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*" ; 
		ACS_LOG_DEBUG(msg.str());
	ACS_CLASS_END_DEBUG

	string node_to_store ;
	const string node (StringUtils::pathNormalize(node_in)) ; 

	if ( match_node_V(node, _clone_root_tag, node_to_store) )
	{
		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
			ostringstream msg; 
			msg << exDebugShortSignatureFile << "Setting value: \"" << value << "\"  in node: " << node_to_store << "  PTR: " << _xml_tag_ptr ;
			ACS_LOG_DEBUG(msg.str());
		ACS_CLASS_END_DEBUG

		const bool removeLeadingSlash (true), removeLeadingDot(true) ;
		vector<string> p_v = StringUtils::pathDisassemble(node_to_store, removeLeadingSlash, removeLeadingDot) ;

		const string node_rel ( not p_v.empty()? p_v.at( p_v.size()-1 ) : "" ) ;	// Deepest entry in the path

		if (node_rel == TAG_CMD__FROM_INPUT)
		{
			ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
				ostringstream msg; 
				msg << exDebugShortSignatureFile << "End of command: \"" << node_rel << "\"  (do nothing)" ;
				ACS_LOG_DEBUG(msg.str());
			ACS_CLASS_END_DEBUG

			// NOTE: Since the command was not PUSHED to the _xml_tag_ptr pointer, it won't be removed either

			// [ Do nothing ] 

			return false ; 		
		}
		else if (node_rel == TAG_CMD__EXECUTE_COMMAND)
		{
			ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
				ostringstream msg; 
				msg << exDebugShortSignatureFile << "End of command: \"" << node_rel << "\"  (do nothing)" ;
				ACS_LOG_DEBUG(msg.str());
			ACS_CLASS_END_DEBUG

			// NOTE: Since the command was not PUSHED to the _xml_tag_ptr pointer, it won't be removed either

			// [ Do nothing ] 

			return false ; 		
		}
		else if (node_rel == TAG_CMD__CALL_FUNCTION)
		{
			ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
				ostringstream msg; 
				msg << exDebugShortSignatureFile << "End of command: \"" << node_rel << "\"  (do nothing)" ;
				ACS_LOG_DEBUG(msg.str());
			ACS_CLASS_END_DEBUG

			// NOTE: Since the command was not PUSHED to the _xml_tag_ptr pointer, it won't be removed either

			// [ Do nothing ] 

			return false ; 		
		}
		else if (node_rel == TAG_CMD__APPLY_SUBST)
		{
			ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
				ostringstream msg; 
				msg << exDebugShortSignatureFile << "End of command: \"" << TAG_CMD__APPLY_SUBST << "\"" ;
				ACS_LOG_DEBUG(msg.str());
			ACS_CLASS_END_DEBUG

			if (not _subst_instances_V.empty())
			{
				const SUBST_instance::SUBST_Type type_to_remove( _subst_instances_V.at(_subst_instances_V.size()-1).getType() ) ;
				const string type_to_remove_str ( SUBST_instance::SUBST_Type_to_String(type_to_remove) ) ;
				string id_to_remove ("") ;
				if (type_to_remove != SUBST_instance::SUBST_DROP) { id_to_remove = ( _subst_instances_V.at(_subst_instances_V.size()-1).getId() ) ; }
	
				_subst_instances_V.pop_back() ;
				
				ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
					ostringstream msg; 
					msg << exDebugShortSignatureFile << "Removed SUBST of type: \"" << type_to_remove_str << "\"\tid: \"" << id_to_remove << "\" New list size: " << _subst_instances_V.size() ;
					ACS_LOG_DEBUG(msg.str());
				ACS_CLASS_END_DEBUG
			}

			return false ; 
		}
		else if (node_rel == TAG_CMD__SORT_TAGS)
		{
			ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
				ostringstream msg; 
				msg << exDebugShortSignatureFile << "End of command: \"" << TAG_CMD__SORT_TAGS << "\"" ;
				ACS_LOG_DEBUG(msg.str());
			ACS_CLASS_END_DEBUG

			if (not _sort_instances_V.empty())
			{
				const string sort_order ( _sort_instances_V.at(_sort_instances_V.size()-1).second ) ;
	
				_sort_instances_V.pop_back() ;
				
				ACS_CLASS_BGN_DEBUG(ACS_MID_VERB)
					ostringstream msg; 
					msg << exDebugShortSignatureFile << "Removed sort_order: \"" << sort_order << "\" from the list of sort_instances. New size: " << _sort_instances_V.size() ;
					ACS_LOG_DEBUG(msg.str());
				ACS_CLASS_END_DEBUG
			}

			return false ; 
		}
		else
		{
			_xml_tag_ptr->set_value(node_to_store, value) ; 
			const bool is_value (true) ; 
			apply_subst(node_to_store, _xml_tag_ptr, is_value) ; 

			apply_sort(node_to_store, _xml_tag_ptr, is_value) ; 

			_xml_tag_ptr = _xml_tag_ptr->pop_tag() ; 
		}

		return true ;
	}
	
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream msg; msg << exDebugShortSignatureFile << "Method finished" ; ACS_LOG_DEBUG(msg.str());
	ACS_CLASS_END_DEBUG

	return false ;
}	


////////////////////////////////////////////////////////////////////////////////////////
//	 encapsuleNewRoot	
////////////////////////////////////////////////////////////////////////////////////////
string XMLProcessor::encapsuleNewRoot(const string& input_file, const string& output_file, const string& root1_in, const string& root2_in) 	
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream msg; msg << exDebugShortSignatureFile << "Method called.\tinput_file: \"" <<  input_file << "\"" \
			<< "\toutput_file: \"" << output_file << "\"\troot1_in: \"" << root1_in << "\"\troot2_in: \"" << root2_in << "\"" ;
		ACS_LOG_DEBUG(msg.str());
	ACS_CLASS_END_DEBUG

	string ret ;
	ostringstream random_root1, random_root2 ; 
	random_root1 << "root1_" ; for (size_t i=0; i<8; i++) { random_root1 << string(1,('A' + (rand() % 26))) ; } 
	random_root2 << "root2_" ; for (size_t i=0; i<8; i++) { random_root2 << string(1,('A' + (rand() % 26))) ; } 
		
	const string root1( (root1_in=="__RANDOM__")? random_root1.str(): root1_in) ;
	const string root2( (root2_in=="__RANDOM__")? random_root2.str(): root2_in) ;
	
	if (root1_in == "__RANDOM__") { ret = root1 ; } else if (root2_in == "__RANDOM__") { ret = root2 ; } else { ret = "" ; }
	
	const bool layer1(root1.empty() == false); const bool layer2(root2.empty() == false);	

	{
		// Step 1: Create a random file to write. This allows input_file and output_file to be the very same file
		const FileGuard tmp_file( File::tempname("/tmp/file.") ) ;
		
		// Step 2: prepend the root-tag to a new, temporary file; copy the input_file and close the tag
		{
			ifstream if_a(input_file.c_str(), std::ios_base::binary);
			ofstream of_c(string(tmp_file).c_str(), std::ios_base::binary | std::ofstream::trunc); // PRQA S 3050, 3081 
			
			if (layer1) { of_c << "<" << root1 << ">\n" ; }	// Prepend the root-tag 1
			if (layer2) { of_c << "<" << root2 << ">\n" ; }	// Prepend the root-tag 2
			if (if_a.good()) { string line ; while ( getline (if_a,line) ) { of_c << line << "\n" ; } ; }	// Copy all the lines from the (input) input_file // PRQA S 3050 
			if (layer2) { of_c << "</" << root2 << ">\n" ; } // Close the root-tag 2
			if (layer1) { of_c << "</" << root1 << ">\n" ; } // Close the root-tag 1
		}

		// Step 3: rename the temporary file to the input_file
		// (so what happened was to embed a root-tag to the input_file)
		File::rename (tmp_file, output_file) ;  // PRQA S 3050 
	}
	
	// Even in case of failure, the FileGuard will be destroyed here

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ostringstream msg; msg << exDebugShortSignatureFile << "Method finished. Returning: \"" << ret << "\"" ; ACS_LOG_DEBUG(msg.str());
	ACS_CLASS_END_DEBUG
	
	return ret ;
}


////////////////////////////////////////////////////////////////////////////////////////
//	 	parseFunctionEnvironment
////////////////////////////////////////////////////////////////////////////////////////
bool XMLProcessor::parseFunctionEnvironment( const map<string, string>& env_M, const string& f,  const string& k, bool emit_exception) 
{
	bool status(true) ;
	
	if (env_M.find(k) == env_M.end())
	{ 
		status = false ;
		ACS_COND_THROW( emit_exception, exIllegalValueException("Function: " + f + ":  Enviroment variable with key: \"" + k + "\" not found")) ; // PRQA S 3081
	} 

	return status ;
}


////////////////////////////////////////////////////////////////////////////////////////
//		concat_log_file 		
////////////////////////////////////////////////////////////////////////////////////////
bool XMLProcessor::concat_log_file(const string& log_file)
{
	bool concat (false) ; 
	
	if ( File::exists(log_file) ) 
	{
		ifstream infile( log_file.c_str() ) ;
		string line;
		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
			ACS_LOG_DEBUG ( "Dumping log file " +  log_file + ": " ) ;    
		ACS_CLASS_END_DEBUG
    	while (std::getline(infile, line)) { ACS_LOG_INFO (line) ; } // PRQA S 3050 
		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
			ACS_LOG_DEBUG ( "End of file " +  log_file + ". " ) ;    
		ACS_CLASS_END_DEBUG
		concat = true ;    	
	}

	return concat ;
}
_ACS_END_NAMESPACE
