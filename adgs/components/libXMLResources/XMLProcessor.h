/*

	Copyright 2002-2016, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. XMLProcessor Module -- HEADER FILE $

	$Id$

	$Author$

	$Log$
	Revision 5.3  2016/10/28 14:35:25  enrcar
	EC:: Method to handle a custom header added
	
	Revision 5.2  2016/10/14 14:29:55  enrcar
	EC:: CALL_FUNCTION modified in order to handle the clone_root_tag also on the file created by the function
	
	Revision 5.1  2016/09/27 12:27:50  enrcar
	EC:: Imported module XMLProcessor
	
	
*/

#ifndef _XMLProcessor_H_
#define _XMLProcessor_H_

#include <XMLIstream.h>
#include <XMLOstream.h>
#include <XMLDataConsumer.h>
#include <exException.h>
#include <string>

// Macro, just for convenience (it can be easily replaced with the signature itself) // PRQA S 1020, 1031 2
#define ACS_XMLPROCESSOR__FUNCT_SIGNATURE const std::vector< std::pair<std::string, std::string> >& input_files_V, const std::vector <std::string>& funct_params_V, const std::string& processed_file, const std::map<std::string, std::string> &env_M, const std::string& log_file




_ACS_BEGIN_NAMESPACE(acs)

////////////////////////////////////////////////////////////////////////////////////////
//									
//   struct xml_tag_struct 		
//									
////////////////////////////////////////////////////////////////////////////////////////
struct xml_tag_struct // PRQA S 2109, 2171, 2173, 2175
{
/**
 * This structure is used as forward-backward linked-list (aka double-pointer l.l.) and is build when the XML is read from its own reference path
 */	

public:	
	xml_tag_struct() ;
	virtual ~xml_tag_struct() throw() ;

public:	
	void set_value(const std::string& node_fp, const std::string& value) ;	
	xml_tag_struct* push_tag(const std::string& node_fp, const XERCES_CPP_NAMESPACE::AttributeList &attrs) ;
	xml_tag_struct* push_tag(const std::string& node_fp, const std::multimap < std::string, std::string >& attrs_M = std::multimap < std::string, std::string >() ) ;
	xml_tag_struct* pop_tag() ;	

	/**
	 * Transfer a container from a source to a destination: all the items in the first are added to the second and removed from the first.
	 *   the index of the added elements will start from the last index (+1) of the recipient
	 */	
	static size_t move_container (xml_tag_struct* src, xml_tag_struct* dst) ; 

	std::string dump() const { return ( dump (this, "", 0, "") ) ; }			
	static std::string dump(const xml_tag_struct* ptr, const std::string& path_so_far, const size_t& level=0, const std::string& tab_so_far="") ;
	static std::string dump_attributes(const std::vector <std::pair<std::string, std::string> >& attrs_V) ;
	static std::multimap <std::string, std::string> convert_attributes(const XERCES_CPP_NAMESPACE::AttributeList &attrs) ; 

private:
	static void remove_tag(xml_tag_struct* ptr, const std::string& parent_name="") ;

private:
	// the copy ctor and assignement operator are declared private to prevent their use.
	// They are, hence, not implemented
	xml_tag_struct(const xml_tag_struct &) ;			// Not allowed
	xml_tag_struct &operator=(const xml_tag_struct &) ;	// Not allowed

public:
	ssize_t _index ; // PRQA S 2100 6
	std::string _name ;
	std::string _value ; 
	std::vector <std::pair<std::string, std::string> > _attributes ; 
	std::multimap <std::string, xml_tag_struct*> _containing ;
	xml_tag_struct* _parent ;

private:	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(xml_tag_struct) ;
} ;


////////////////////////////////////////////////////////////////////////////////////////
//									
//   struct LUT_struct 		
//									
////////////////////////////////////////////////////////////////////////////////////////
struct LUT_struct  // PRQA S 2109, 2171, 2173, 2175
{
public:
	explicit LUT_struct(const std::string& id) : _id(id), _lut_V() {}
	LUT_struct(const LUT_struct &r) : _id(r._id), _lut_V(r._lut_V) {}
	LUT_struct &operator=(const LUT_struct &r) {
        if (this != &r) { _id = r._id ;  _lut_V = r._lut_V ; }
        return *this ;
    }
	~LUT_struct() throw() {}

	std::string apply(const std::string& key, bool& lut_is_triggered) const ;
	size_t addElementToLUT(const std::pair<std::string, std::string>& e) { _lut_V.push_back(e); return (_lut_V.size()) ; }
	std::string getLutID() const { return _id ; }
	void clearLuts() { _lut_V.clear() ; } 
	std::vector <std::pair<std::string, std::string> > getLutElements () const { return _lut_V ; }
	std::string dump(const std::string& tab="") const ;
private:
    LUT_struct() ; // not implemented
private:
	std::string _id ; 
	std::vector <std::pair<std::string, std::string> > _lut_V ;

private:	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(LUT_struct) ;
} ;


////////////////////////////////////////////////////////////////////////////////////////
//									
//   struct ADD_Struct	
//								
////////////////////////////////////////////////////////////////////////////////////////
struct ADD_Struct // PRQA S 2109, 2171, 2173, 2175
{
// Just a sketch -- Should be improved
public:		
	ADD_Struct() : _tag_to_add_V() {} 
	ADD_Struct(const std::string& tag, const std::string& value) : 
		_tag_to_add_V() { _tag_to_add_V.push_back ( make_pair(tag,value) ) ; } 
	~ADD_Struct() throw() {}
	ADD_Struct(const ADD_Struct &r) :  _tag_to_add_V(r._tag_to_add_V) {}
	ADD_Struct &operator=(const ADD_Struct &r) {
        if (this != &r) { _tag_to_add_V = r._tag_to_add_V ; }
        return *this ;
    }

	void addElement(const std::pair <std::string, std::string>& p) { _tag_to_add_V.push_back(p) ; }
	void clear() { _tag_to_add_V.clear() ; } 
	size_t size() const { return  _tag_to_add_V.size() ; }
	std::pair <std::string, std::string> at(const size_t& i) const { if (i<_tag_to_add_V.size()) { return _tag_to_add_V.at(i) ; } else { return (std::make_pair("VOID", "VOID")) ;  } } // PRQA S 4020
	std::vector < std::pair <std::string, std::string> > getADD_Struct() const { return _tag_to_add_V ; } 

private:
	std::vector < std::pair <std::string, std::string> > _tag_to_add_V ; 
} ;


////////////////////////////////////////////////////////////////////////////////////////
//									
//   struct SUBST_instance 		
//									
////////////////////////////////////////////////////////////////////////////////////////
struct SUBST_instance  // PRQA S 2109, 2171, 2173, 2175
{
enum SUBST_Type { SUBST_UNDEFINED=0x00 , SUBST_NONE=0x01, SUBST_LUT=0x02, SUBST_DROP=0x04, SUBST_ADD=0x08, SUBST_FUNCT=0x10, SUBST_CMD=0x20 } ;
enum Match_Element_Rule { TAG_UNDEFINED=0x0 , TAG_NAME=0x01, TAG_VALUE=0x02, TAG_ATTR_NAME=0x04, TAG_ATTR_VALUE=0x08 } ;

/**
 * This structure is used as a trigger for a single substitution (triggered by a specific rule) for a container of type: _type and id: _id. 
 */	
public:
	SUBST_instance(const SUBST_Type type, const std::string& id, const Match_Element_Rule& r, const std::string& match_element) : 
		_type(type), _id(id), _opts_V(), _match_element_rule(r), _match_element(match_element) {}
	~SUBST_instance() throw() {}
	SUBST_instance(const SUBST_instance &r) :
        _type(r._type), _id(r._id), _opts_V(r._opts_V), _match_element_rule(r._match_element_rule), _match_element(r._match_element) {}

	SUBST_instance& operator=(const SUBST_instance &r) {
        if (this != &r) { _type = r._type ; _id = r._id ; _opts_V = r._opts_V ;
                          _match_element_rule = r._match_element_rule ;
                          _match_element = r._match_element ; }
        return *this ;
    }

	void setMatch_Element_Rule(const Match_Element_Rule& r) { _match_element_rule = r ; }
	void setMatch_Element(const std::string& m) { _match_element = m ; }
	size_t addOpts(const std::string& m) { _opts_V.push_back(m) ; return _opts_V.size() ; }
	size_t setOpts(const std::vector <std::string>& v) { _opts_V = v ; return _opts_V.size() ; }
	size_t clearOpts() { _opts_V.clear() ; return _opts_V.size() ; /* That is: 0 */ }
	Match_Element_Rule getMatch_Element_Rule() const { return _match_element_rule ; }
	std::string getMatch_Element() const { return _match_element ; }
	SUBST_Type getType() const { return _type ; }	// Type of the substitution, i.e. of the container
	std::string getId() const { return _id ; }		// id of the container
	std::vector <std::string> getOpts() const { return _opts_V ; }		// id of the container
	std::string dump(const std::string& tab="") const ;

	static std::string Match_Element_Rule_to_String(enum Match_Element_Rule e) ;
	static Match_Element_Rule String_to_Match_Element_Rule(const std::string& s) ;
	static std::string SUBST_Type_to_String(enum SUBST_Type e) ; 
	static SUBST_Type String_to_SUBST_Type(const std::string& s) ; 
private:
    SUBST_instance() ; // not implemented
private:
	SUBST_Type _type ;
	std::string _id ;
	std::vector <std::string> _opts_V ;	// Used e.g. for functions 
	Match_Element_Rule _match_element_rule ;
	std::string _match_element ;

private:	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(SUBST_instance) ;
} ;



//////////////////////////////////
//								//
//   class  XMLProcessor 		//
//								//
//////////////////////////////////

/**
 * This class is derived by XMLDataConsumer and it's used by the XML Interpreter to process the XML files
 */	

class XMLProcessor : public XMLDataConsumer // PRQA S 2109, 2153
{
public:
    using XMLDataConsumer::nodeMatch ;
    using XMLDataConsumer::nodeValue ;

	explicit XMLProcessor(const std::string& base_path="") ;
	XMLProcessor(xml_tag_struct* xml_tag_ptr, const std::string& base_path) ;
	virtual ~XMLProcessor() throw() ;

	/**
	 * Clone the member of this processor to a new (usually, just instanced) processor, in order to share the same configuration environment
	 */	
	void clone_env_to(XMLProcessor* p) const
	{ p->_luts_M = _luts_M ; p->_subst_instances_V = _subst_instances_V ; p->_sort_instances_V = _sort_instances_V ; p->_filenames_in_M = _filenames_in_M ; \
		p->_executables_M = _executables_M ; p->_tags_to_add_M = _tags_to_add_M ; p->_functs_M = _functs_M ; p->_env_M = _env_M ;}

	void exec_cmd__FROM_INPUT(	xml_tag_struct* xml_tag_ptr, const std::string& base_path, const std::string& input_file, \
								const std::string& path_regexp, const bool is_regexp, const bool clone_root_tag) ;

	void exec_cmd__EXECUTE_COMMAND(	xml_tag_struct* xml_tag_ptr, const std::string& base_path, const bool clone_root_tag, const std::string& add_new_root, \
									const std::string& cmd_name, const std::vector <std::string>& cmd_opts_V, \
									const std::map < std::string, std::pair<std::string, bool> > &input_files_M) ;

	void exec_cmd__CALL_FUNCTION(	xml_tag_struct* xml_tag_ptr, const std::string& base_path, const bool clone_root_tag_in, const bool clone_root_tag_out, \
									const std::string& add_new_root_out, const std::string& base_out_path, const bool base_out_path_is_regexp, \
									const std::string& funct_id, int (*funct_ptr) (ACS_XMLPROCESSOR__FUNCT_SIGNATURE), \
									const std::vector <std::string>& cmd_opts_V, \
									const std::map < std::string, std::pair<std::string, bool> > &input_files_M) ;

	bool apply_subst(const std::string& path, xml_tag_struct*, const bool is_value) ;
	bool apply_sort(const std::string& path, xml_tag_struct*, const bool is_value) ;
	std::string apply_function_to_string(const std::string& funct_id, const std::vector <std::string>& funct_params_V, const std::string& key, bool& subst_is_triggered) const ; 
	bool check_tag_to_drop(const std::string& path_in) ;

	size_t setInputFiles(const std::map< std::string, std::string > &m) { _filenames_in_M = m ; return _filenames_in_M.size() ; } 
	size_t resetInputFiles() { _filenames_in_M.clear() ; return _filenames_in_M.size() /* that is: 0 */ ; }
	size_t addInputFiles(const std::vector< std::pair<std::string, std::string> > &v) 
	{ for (size_t i=0; i<v.size(); i++) { addInputFile ( v.at(i).first, v.at(i).second ) ; } return _filenames_in_M.size() ; } 
	size_t addInputFile( const std::string& s, const std::string& f) { _filenames_in_M.insert ( make_pair(s,f) ) ; return _filenames_in_M.size() ; } 

	size_t setFunctions(const std::map< std::string, int (*) (ACS_XMLPROCESSOR__FUNCT_SIGNATURE) > &m) { _functs_M = m ; return _functs_M.size() ; } 
	size_t resetFunctions() { _functs_M.clear() ; return _functs_M.size() /* that is: 0 */ ; }
	size_t addFunction( const std::string& s, int (*f) (ACS_XMLPROCESSOR__FUNCT_SIGNATURE) ) { _functs_M.insert ( make_pair (s,f) ) ; return _functs_M.size() ; } 

	size_t setExecutables(const std::map< std::string, std::string > &m) { _executables_M = m ; return _executables_M.size() ; } 
	size_t resetExecutables() { _executables_M.clear() ; return _executables_M.size() /* that is: 0 */ ; }
	size_t addExecutable( const std::string& s,  const std::string& e ) { _executables_M.insert ( make_pair (s,e) ) ; return _executables_M.size() ; } 

	size_t setTagsToAdd(const std::map< std::string, ADD_Struct > &m) { _tags_to_add_M = m ; return _tags_to_add_M.size() ; } 
	size_t resetTagsToAdd() { _tags_to_add_M.clear() ; return _tags_to_add_M.size() /* that is: 0 */ ; }
	size_t addTagToAdd( const std::string& s,  const ADD_Struct& e ) { _tags_to_add_M.insert ( make_pair (s,e) ) ; return _tags_to_add_M.size() ; } 

	size_t setEnvironments(const std::map< std::string, std::string > &m) { _env_M = m ; return _env_M.size() ; } 
	size_t resetEnvironments() { _env_M.clear() ; return _env_M.size() /* that is: 0 */ ; }
	size_t addEnvironment( const std::string& s,  const std::string& e ) { _env_M.insert ( make_pair (s,e) ) ; return _env_M.size() ; } 
	bool parseFunctionEnvironment( const std::string& f,  const std::string& k, bool emit_exception = true) // PRQA S 4211
	{ return parseFunctionEnvironment(_env_M, f, k, emit_exception ) ; }
	static bool parseFunctionEnvironment( const std::map<std::string, std::string>&, const std::string& f,  const std::string& k, bool emit_exception = true) ;

	void process(const std::string& file_id, const std::string& path, bool is_regexp, const std::string& filename_out="", bool add_header=true) ;

	void addLut(const LUT_struct &s) { _luts_M.insert( make_pair(s.getLutID(), s) ) ; } 
	void setSubstInstances(std::vector<SUBST_instance> &v) { _subst_instances_V = v ; } 
	void setCloneRootTag(const bool t) { _clone_root_tag = t; }
	void setHeaderToAdd(const std::string& s) { _header_to_add = s ; } 

public:
	static void add_to_XMLOstream(const xml_tag_struct* ptr, XMLOstream &xst) ;

	/** Used for DEBUG PURPOSES. Replace the definition of a custom processing function with this function. */
	static int DEBUG_processing_function (const std::vector< std::pair<std::string, std::string> >& input_files_V,
					const std::vector <std::string>& funct_params_V,
					const std::string& processed_file, const std::map<std::string, std::string>& env_M, const std::string& log_file) ;

	/** The output file is created by adding a new root (encapsuling) to the original file. Once or two times. 
		To prevent the second root use "" (empty string). Use "__RANDOM__" to obtain a random root.
	 */
	static std::string encapsuleNewRoot(const std::string& input_file, const std::string& output_file, const std::string& root1, const std::string& root2="") ;	


private:
	virtual bool nodeMatch(XMLExtractorHandlerBase &, const std::string &node, const XERCES_CPP_NAMESPACE::AttributeList &, const XMLCh* name) ;		
	virtual bool nodeValue(XMLExtractorHandlerBase &, const std::string &xmlnode, const std::string &value, const XMLCh* name) ;

	// the copy ctor and assignement operator are declared private to prevent their use.
	// They are, hence, not implemented
	XMLProcessor(const XMLProcessor &); 
	XMLProcessor &operator=(const XMLProcessor &); 

	bool match_node_V(const std::string&, const bool clone_root_tag, std::string& node_to_store) ;
	bool match_node_V(const std::vector <std::string>&, const bool clone_root_tag, std::string& node_to_store) ;
	void set_node_V(const std::string& node, const bool is_regexp, const bool exact_regexp) ; 
	void process_header(const std::string& entry_point_id, const std::string& node) ;

	static bool concat_log_file(const std::string& log_file) ;

	static bool match_exact_path_with_node(const std::string&, const std::vector <std::string>&) ;
	static bool match_path_contained_in_node(const std::string&, const std::vector <std::string>&) ;
	static std::string path_assemble(const std::vector <std::string>& node_V, const size_t& offset=0) ;
	static std::string dump(const std::vector <std::string>&) ;
	static std::string dump_as_path(const std::vector <std::string>&) ;

private:
	xml_tag_struct* _xml_tag_ptr ;
	bool _destroy_ptr_at_exit ;
	std::vector <std::string> _node_V ;
	bool _node_is_regexp ; 

	// Instances:
	std::map<std::string, LUT_struct> _luts_M ; 
	std::vector<SUBST_instance> _subst_instances_V ; 
	std::vector< std::pair<std::string, std::string> > _sort_instances_V ;

	std::map< std::string, std::string > _filenames_in_M ;
	std::map< std::string, std::string > _executables_M ;
	std::map< std::string, ADD_Struct > _tags_to_add_M ;
	std::map< std::string, int (*) (ACS_XMLPROCESSOR__FUNCT_SIGNATURE) > _functs_M ;
	std::map< std::string, std::string > _env_M ;

	std::string _entry_point ;		// file_id of the entry point
	std::string _filename_out ;
	std::string _base_path ;	/* Used to create a chain of "base_paths", to apply the LUT instances */
	std::string _header_to_add ; 	/* Optional, added only if the argument add_header of process() is true */
	bool		_clone_root_tag ;

	static const std::string TAG_CMD__FROM_INPUT ;   
	static const std::string TAG_CMD__APPLY_SUBST ;   
	static const std::string TAG_CMD__SORT_TAGS ;   
	static const std::string TAG_CMD__HEADER ;   
	static const std::string TAG_CMD__EXECUTE_COMMAND ;   
	static const std::string TAG_CMD__CALL_FUNCTION ;   

private:	
	ACS_CLASS_DECLARE_DEBUG_LEVEL(XMLProcessor) ;
} ;


_ACS_END_NAMESPACE



#endif //_XMLProcessor_H_

