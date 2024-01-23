/*
	Copyright 2018-2019, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libNetcdf4xmlHandler$

	$Id$

	$Author$

	$Log$
	Release 1.24.2  2019/05/23 17:30:10  enrico.carta@acsys.it
	EC:: Compiling warning removed
	
	Revision 1.1  2018/07/13 09:02:27  enrcar
	EC:: Modules added: Netcdf4xmlConverter, Netcdf4xmlHandler
	

        
*/

#ifndef _Netcdf4xmlHandler_H_
#define _Netcdf4xmlHandler_H_
#include <acs_c++config.hpp>
#include <rsResourceSet.h>
#include <XMLOstream.h>
#include <XMLIstream.h>
#include <DateTime.h>
#include <exException.h>
#include <vector>
#include <string>

_ACS_BEGIN_NAMESPACE(acs)

class XMLOstream ;
class rsResourceSet ;

/**
 * Class to handle a netcdf file by an XML representation.
 */
class Netcdf4xmlHandler  { 

public:
    static const std::vector<std::string> types;
    static const std::vector<size_t> typesSize;
    
	/** Netcdf4xmlHandler Exceptions */
	exDECLARE_EXCEPTION(exNetcdf4xmlHandlerException, exException) ; // Base Netcdf4xmlHandler Exception.
	exDECLARE_EXCEPTION(exNetcdf4xmlHandlerCriticalException, exNetcdf4xmlHandlerException) ; // Critical Exception.

	/**
     * Default constructor.
     */
	Netcdf4xmlHandler() : groups_V_() {}
    
    /**
     * Constructor.
     * @param[in] filename The name of the netCDF XML file template
     */
	Netcdf4xmlHandler(const std::string& filename) ;
	
	/**
     * Destructor.
     */
	virtual ~Netcdf4xmlHandler() throw() ;
    
    /**
     * Copy constructor.
     */
    Netcdf4xmlHandler(const Netcdf4xmlHandler &) ;
	
    /**
     * Overload operator=.
     */
    Netcdf4xmlHandler &operator=(const Netcdf4xmlHandler &) ;
    
    /**
     * \struct Struct to handle netcdf dimensions.
     */
	struct Dimension {

		public :
			/** 
             * Constructors.
             */
            Dimension(const size_t idx=0):
				idx_(idx),
				name_(""),
				length_("0")
				{ }								
			Dimension(const std::string& name, const std::string& length, const size_t idx=0):
				idx_(idx),
				name_(name),
				length_(length)
				{ }								
			virtual ~Dimension() {} 
			
            /**
     		 * Copy constructor.
			 */
			Dimension(const Dimension & ); 
			
            /**
     		 * Overload of the Operator= .
			 */
			Dimension &operator=(const Dimension &) ;
            
            /**
             * Set the Dimension name.
             * @param[in] s The Dimension name
             */
			inline void					set_name(const std::string& s)		{ name_ = s ; }
            
            /**
             * Get the Dimension name.
             * @return The Dimension name
             */
			inline std::string			get_name() const					{ return name_ ; }
            
            /**
             * Set the value ("lenght") of the dimension.
             * @param[in] s The dimension value
             */
			inline void					set_length(const std::string& s)	{ length_ = s ; }
            
            /**
             * Get the value ("lenght") of the dimension.
             * @return The dimension value
             */
			inline std::string			get_length() const					{ return length_ ; }
            
            /**
             * Read the Dimension from the rsResourceSet object.
             * @param[in] rs rsResourceSet object storing the XML previously read
             */
			void read(rsResourceSet &) ;
            
            /**
             * Write the Dimension to the XMLOstream.
             * @param[in] xst The XML output stream
             * @param[in] alter_tag Boolean indicating if the XML dimension tag should include the dimension name (true) or not (false)
             */
			void write(XMLOstream& xst, const bool& alter_tag) const ;
            
            /**
             * Dump the dimension content.
             * @param[in] tab Additional tab string (empty by default)
             */
			std::string dump(const std::string& tab="") const ;
			
		private :
			size_t		idx_ ;
	    	std::string name_ ;
	    	std::string length_ ;
	} ;
	
	/**
     * \struct Struct to handle netcdf attributes.
     */
	struct Attribute {

		public :
			/** 
             * Constructors.
             */
            Attribute(const size_t idx=0):
				idx_(idx),
				name_(""),
				value_(""),
				type_("")
				{ }								
			Attribute(const std::string& name, const std::string& value, const std::string& type, const size_t idx=0):
				idx_(idx),
				name_(name),
				value_(value),
				type_(type)
				{ }								
			virtual ~Attribute() {} 
			
            /**
     		 * Copy constructor.
			 */
			Attribute(const Attribute & ); 
			
            /**
     		 * Overload of the Operator= .
			 */
			Attribute &operator=(const Attribute &) ;
            
            /**
             * Set the Attribute name.
             * @param[in] s The Attribute name
             */
			inline void					set_name(const std::string& s)		{ name_ = s ; }
			
            /**
             * Get the Attribute name.
             * @return The Attribute name
             */
            inline std::string			get_name() const					{ return name_ ; }
            
            /**
             * Set the Attribute value.
             * @param[in] s The Attribute value
             */
			inline void					set_value(const std::string& s)		{ value_ = s ; }
            
            /**
             * Get the Attribute value.
             * @return The Attribute value
             */
			inline std::string			get_value() const					{ return value_ ; }
            
            /**
             * Get the Attribute data type.
             * @return The string identifying the Attribute data type
             */
			inline std::string			get_type() const					{ return type_ ; }
            
            /**
             * Check if the attribute is yet not set.
             * @return Boolean indicating if variable is empty (true) or not (false)
             */
	        inline bool                 empty() const                       { return name_.empty() ; }
            
            /**
             * Read the Attribute from the rsResourceSet object.
             * @param[in] rs rsResourceSet object storing the XML previously read
             */
			void read(rsResourceSet &rs) ;
            
            /**
             * Write the Attribute to the XMLOstream.
             * @param[in] xst The XML output stream
             * @param[in] alter_tag Boolean indicating if the XML attributr tag should include the attribute name (true) or not (false)
             */
			void write(XMLOstream& xst, const bool& alter_tag) const ;
            
            /**
             * Dump the attribute content.
             * @param[in] tab Additional tab string (empty by default)
             */
			std::string dump(const std::string& tab="") const ;
			
            /**
             * Check that the type string is one of those allowed.
             * @param[in] str_type The string to be checked 
             */
            bool goodType(const std::string& str_type) const ;
            
            /**
             * Set the string defining the attribute type.
             * @param[in] str_typ The string identifier of the type 
             */
            void set_type(const std::string& typ);
            
		private :
			size_t		idx_ ;
	    	std::string name_ ;
	    	std::string value_ ;
	    	std::string type_ ;
	} ;

    /**
     * \struct Struct to handle netcdf variables.
     */
	struct Variable {

		public :
			/**
     		 * Constructors.
			 */
			Variable(const size_t idx=0):
				idx_(idx),
				name_(""),
				type_(""),
				shape_(""),
				values_(""),
				values_is_set_(false),
				attributes_V_(),
                filename_(""),
                fileformat_(""),
                ascii_separator_(""),
                ascii_cols_(),
                fillValue_(""),
				dataValuesSize_(),
                dataValues_(0),
                dataValuesHere_(false)
				{ }								
			Variable(const std::string& name, const std::string& type, const std::string& shape, const size_t idx=0):
				idx_(idx),
				name_(name),
				type_(type),
				shape_(shape),
				values_(""),
				values_is_set_(false),
				attributes_V_(),
                filename_(""),
                fileformat_(""),
                ascii_separator_(""),
                ascii_cols_(),
                fillValue_(""),
				dataValuesSize_(),
                dataValues_(0),
                dataValuesHere_(false)
				{ }
			Variable(const std::string& name, const std::string& type, const std::string& shape, const std::string& values, const size_t idx=0):
				idx_(idx),
				name_(name),
				type_(type),
				shape_(shape),
				values_(values),
				values_is_set_(true),
				attributes_V_(),
                filename_(""),
                fileformat_(""),
                ascii_separator_(""),
                ascii_cols_(),
                fillValue_(""),
				dataValuesSize_(),
                dataValues_(0),
                dataValuesHere_(false)
				{ }
			/**
     		 * Destructor.
			 */
			virtual ~Variable() throw() ;
			/**
     		 * Copy constructor.
			 */
			Variable(const Variable & ); 
			/**
     		 * Overload of the Operator= .
			 */
			Variable &operator=(const Variable &) ; 
			
            /**
             * Reset the list of attributes.
             */
			inline void reset_attributes()										{ attributes_V_.clear() ; }
			
            /**
             * Wraps reset_attributes method.
             */
            inline void reset()													{ reset_attributes() ; }
			
            /**
             * Set the Variable name.
             * @param[in] s The string name
             */
            inline void						set_name(const std::string& s)		{ name_ = s ; }
            
            /**
             * Get the Variable name.
             * @return The string name
             */
			inline std::string				get_name() const					{ return name_ ; }
            
            /**
             * Get the Variable data type.
             * @return The data type string identifier
             */
			inline std::string				get_type() const					{ return type_ ; }
            
            /**
             * Set the Variable shape [dim1_string dime2_string ...].
             * @param[in] s The shape string
             */
			inline void						set_shape(const std::string& s)		{ shape_ = s ; }
            
            /**
             * Get the Variable shape [dim1_string dime2_string ...].
             * @return s The shape string
             */
			inline std::string				get_shape() const					{ return shape_ ; }
            
            /**
             * Add one Attribute object.
             * @param[in] e The Attribute oject to be added
             */
    		inline void 					add_attribute(const Attribute& e)	{ attributes_V_.push_back(e) ; ; }
            
            /**
             * Get the list of the Attribute objects.
             * @return The list of the Attribute objects stored in the Variable.
             */
        	inline std::vector <Attribute>	get_attributes() const				{ return attributes_V_ ; }
            
            /**
             * Set the values of the Variable in the form of string separated by empty space.
             * @param[in] s The data values.
             */
			inline void						set_values(const std::string& s)	{ values_ = s ; set_values_is_set(true) ; }
			
            /**
             * Get the values of the Variable in the form of string separated by empty space.
             * @return The data values.
             */
            inline std::string				get_values() const					{ return ( (true == values_is_set_)? values_ : "" ) ; }
			
            /**
             * Set the flag indicating if the data are stored in the form of string (true) or not (false).
             * @param[in] b The flag.
             */
            inline void						set_values_is_set(const bool& b)	{ values_is_set_ = b ; }
            
            /**
             * Get the flag indicating if the data are stored in the form of string (true) or not (false).
             * @return The flag.
             */
			inline bool						get_values_is_set() const			{ return values_is_set_ ; }
            
            /**
             * Set the name of the file storing the data of the Variable.
             * @param[in] s The file name.
             */
            inline void						set_filename(const std::string& s)	{ filename_ = s ; }
            
            /**
             * Get the name of the file storing the data of the Variable.
             * @return The file name.
             */
			inline std::string				get_filename() const				{ return filename_ ; }
            
            /**
             * Set the name of the format of the file storing the data of the Variable.
             * @param[in] s The file format identifier.
             */
            inline void						set_fileformat(const std::string& s) { fileformat_ = s ; }
            
            /**
             * Set the separator character used in ascii/csv files storing the data of the Variable.
             * @param[in] s The file format identifier.
             */
            inline void						set_ascii_separator(const std::string& s) { ascii_separator_ = s ; }
            
            /**
             * Set the column indices to be read of the data provided by ascii/csv files.
             * @param[in] s The file format identifier.
             */
            inline void						set_ascii_cols(const std::vector<int>& s) { ascii_cols_ = s ; }            
            
            /**
             * Get the name of the format of the file storing the data of the Variable.
             * @return The file format identifier.
             */
			inline std::string				get_fileformat() const              { return fileformat_ ; }
            
            /**
             * Get the separator character used in ascii/csv files storing the data of the Variable.
             * @return The file format identifier.
             */
			inline std::string				get_ascii_separator() const              { return ascii_separator_ ; }            

            /**
             * Get the separator character used in ascii/csv files storing the data of the Variable.
             * @return The file format identifier.
             */
			inline std::vector<int>			get_ascii_cols() const              { return ascii_cols_ ; }             

            /**
             * Set the fill value of the Variable data.
             * @param[in] s The fill value.
             */
            inline void						set_fillValue(const std::string& s) { fillValue_ = s ; }
            
            /**
             * Get the fill value of the Variable data.
             * @return The fill value.
             */
			inline std::string				get_fillValue() const               { return fillValue_ ; }
            
            /**
             * Check if the variable is yet not set.
             * @return Boolean indicating if variable is empty (true) or not (false)
             */
            inline bool						empty() const                       { return name_.empty() ; }
            
            /**
             * Get the dimensions of the Variable data (if stored as a buffer).
             * @return The data dimensions [dim1,dim2,...,dimN]
             */
            inline std::vector<int>         get_dataValuesSize() const          { return dataValuesSize_ ; }
            
            inline bool                     dataValuesHere() const          { return dataValuesHere_ ; }
            
            /**
             * Read the Variable from the rsResourceSet object.
             * @param[in] rs rsResourceSet object storing the XML previously read
             */
            void read(rsResourceSet &rs) ;
            
            /**
             * Write the Variable to the XMLOstream.
             * @param[in] xst The XML output stream
             * @param[in] alter_tag Boolean indicating if the XML variable tag should include the variable name (true) or not (false)
             */
			void write(XMLOstream& xst, const bool& alter_tag) const ;
            
            /**
             * Dump the variable content.
             * @param[in] tab Additional tab string (empty by default)
             */
			std::string dump(const std::string& tab="") const ;
            
            /**
             * Check that the type string is one of those allowed.
             * @param[in] str_type The string to be checked 
             */
			bool goodType(const std::string& str_type) const ;
            
            /**
             * Set the string defining the attribute type.
             * @param[in] str_typ The string identifier of the type 
             */
            void set_type(const std::string& typ);
            
            /**
             * Get the Attribute object by its name.
             * @param[in] attributeName The name of the attribute 
             */
            Attribute get_attribute(const std::string& attributeName) ;
            
            /**
             * Set a new Attribute object.
             * @param[in] attr The new attribute object
             */
            int set_attribute(const Attribute& attr) ;
            
            /**
             * Set the data of a variable providing a pointer to the data buffer.
             * @param[in] dataSize The list of data size [dim1, dim2, ..., dimN]
             * @param[in] data The data buffer
             * @param[in] The identifier of the data type
             */
			void set_dataValues(const std::vector<int>& dataSize, const void* data, const std::string& dataType="byte") ;

            /**
             * Set the data of a variable providing a pointer to the data buffer.
             * @param[in] dataSize The list of data size [dim1, dim2, ..., dimN]
             * @param[in] data The data buffer
             * @param[in] The identifier of the data type
             */
			void set_dataValues_pointer(const std::vector<int>& dataSize, void* data, const std::string& dataType="byte") ;
            
            void get_dataValues_pointer(std::vector<int>& dataSize, void** data) const;		
            
            /**
             * Udate the data (max 3D) of a variable providing a pointer to the data buffer.
             * @param[in] dataSize The list of data size [dim1, dim2, dim3]
             * @param[in] data The data buffer
             * @param[in] dataType The identifier of the data type
             */
            void update_dataValues(const std::vector<int>& dataSize, const void* data, const std::string& dataType="byte");

            /**
             * Get the data of a variable if stored in a data buffer.
             * @param[out] dataSize The list of data size [dim1, dim2, ..., dimN]
             * @param[out] data The data buffer
             */
            void get_dataValues(std::vector<int>& dataSize, void** data) const;
			
            /**
             * Get the size of the type of the data stored by the Variable.
             * @return The size of the type of the data stored by the Variable.
             */
            size_t getTypeSize() const;
                        
		private :
			size_t		            idx_ ;
	    	std::string             name_ ;
	    	std::string             type_ ;
	    	std::string             shape_ ;
	    	std::string             values_ ;
			bool                    values_is_set_ ; 
        	std::vector <Attribute> attributes_V_ ;
            std::string             filename_ ;
            std::string             fileformat_ ;
            std::string             ascii_separator_ ;
            std::vector<int>        ascii_cols_;
            std::string             fillValue_ ;
			std::vector<int>        dataValuesSize_ ;
            void*	                dataValues_ ;
            bool                    dataValuesHere_;
	} ;


    /**
     * \struct Struct to handle netcdf groups.
     */
	struct Group {

		public :
			/**
     		 * Default Constructor.
			 */
            Group():
				idx_(0),
				name_(""),
				group_is_root_(true),	/* TRUE means that the group is unnamed and it's actually the root of a file */
				dimensions_V_(),
				attributes_V_(),
				variables_V_(),
                nestedgroups_V_()
				{ }								
			/**
     		 * Constructor.
             * @param[in] group_is_root Flag indicating if the group is root or not
             * @param[in] name The name of the group (empty by default)
             * @param[in] idx netCDF group index (0 by default)
			 */
            Group(const bool& group_is_root, const std::string& name="", const size_t idx=0):
				idx_(idx),
				name_(name),
				group_is_root_(group_is_root),	/* TRUE means that the group is unnamed and it's actually the root of a file */
				dimensions_V_(),
				attributes_V_(),
				variables_V_(),
                nestedgroups_V_()
				{ }								
			virtual ~Group() {} 
			
            /**
     		 * Copy constructor.
			 */
			Group(const Group & ); 
			/**
     		 * Overload of the Operator= .
			 */
			Group &operator=(const Group &) ; 
            
			inline void reset_dimensions()											{ dimensions_V_.clear() ; }
			inline void reset_attributes()											{ attributes_V_.clear() ; }
			inline void reset_variables()											{ variables_V_.clear() ; }
			inline void reset_groups()											    { nestedgroups_V_.clear() ; }
            inline void reset()														{ reset_dimensions() ; reset_attributes() ; reset_variables() ; reset_groups(); }
			inline void						set_name(const std::string& s)			{ name_ = s ; }
			inline std::string				get_name() const						{ return name_ ; }
			inline void						set_group_is_root(const bool& b)		{ group_is_root_ = b ; }
			inline bool						get_group_is_root() const				{ return group_is_root_ ; }
	        inline std::vector <Dimension>	get_dimensions() const					{ return dimensions_V_ ; }
        	inline std::vector <Attribute>	get_attributes() const					{ return attributes_V_ ; }
        	inline std::vector <Variable>	get_variables() const					{ return variables_V_ ; }
			inline std::vector <Group>      get_nestedgroups() const                { return nestedgroups_V_ ; }
            inline void add_dimension(const Dimension& e)							{ dimensions_V_.push_back(e) ; }
    		inline void add_attribute(const Attribute& e)							{ attributes_V_.push_back(e) ; ; }
    		inline void add_variable(const Variable& e)							    { variables_V_.push_back(e) ; ; }
            inline size_t add_group(const Group& g)                                 { nestedgroups_V_.push_back(g) ; return nestedgroups_V_.size() ; }
			inline bool						empty()	const							
			{	return (name_.empty() && (0 == (nestedgroups_V_.size() + dimensions_V_.size() + attributes_V_.size() + variables_V_.size())) ) ; }
            inline bool						not_filled()	const							
			{	return (!name_.empty() && (0 == (nestedgroups_V_.size() + dimensions_V_.size() + attributes_V_.size() + variables_V_.size())) ) ; }
            
            /**
             * Read the Group from the rsResourceSet object.
             * @param[in] rs rsResourceSet object storing the XML previously read
             */
			void read(rsResourceSet &rs) ;
			
            /**
             * Write the Group to the XMLOstream.
             * @param[in] xst The XML output stream
             * @param[in] alter_tag Boolean indicating if the XML variable tag should include the variable name (true) or not (false)
             */
            void write(XMLOstream& xst, const bool& alter_tag) const ;
			
            /**
             * Dump the group content.
             * @param[in] tab Additional tab string (empty by default)
             */
            std::string dump(const std::string& tab="") const ;
            
            /**
             * Get the Group object corresponding to a given group name.
             * @param[in] groupName The name of the nested group
             * @return The retrieved nested Group object
             */
            Group get_nestedgroup(const std::string& groupName) const ;
            
            /**
             * Set to a Group object a nested group.
             * @param[in] grp The nested Group object
             * @return 1 if ok, 0 if it was not possible to insert the Group.
             */
            int set_nestedgroup(const Group& grp) ;
            
            /**
             * Remove the Group object corresponding to a given group name.
             * @param[in] groupName The name of the nested group
             * @return Boolean indicating if group(s) with configured name(s) has(ve) been removed
             */
            bool remove_nestedgroup(const std::string& groupName);            
            /**
             * Get an Attribute object given its name.
             * @param[in] attributeName The name of the attribute
             * @return The retrieved Attribute object
             */
            Attribute get_attribute(const std::string& attributeName) ;
            
            /**
             * Get a Variable object given its name.
             * @param[in] variableName The name of the attribute
             * @return The retrieved Variable object
             */
            Variable get_variable(const std::string& variableName) ;
            
            /**
             * Get a Dimension object given its name.
             * @param[in] dimensionName The name of the dimension
             * @return The retrieved Dimension object
             */
            Dimension get_dimension(const std::string& dimensionName) ;
            
            /**
             * Set an Attribute object.
             * @param[in] attr The Attribute object
             */
            int set_attribute(const Attribute& attr) ;
            
            /**
             * Set a Variable object.
             * @param[in] var The Variable object
             */
            int set_variable(const Variable& var) ;
            
            /**
             * Set a Dimension object.
             * @param[in] dim The Dimension object
             */
            int set_dimension(const Dimension& dim) ;
            
            /**
             * Remove a dimension object given its name.
             * @param[in] dim The Dimension name
             */
            void rm_dimension(const std::string& DimName);
                      
            /**
             * Remove a variable object given its name.
             * @param[in] dim The Variable name
             */
            void rm_variable(const std::string& varName);
                
		private:
			void read_content(rsResourceSet &) ;
			void write_content(XMLOstream& xst, const bool& alter_tag) const ;
			
		private :
			size_t idx_ ;
	    	std::string name_ ;
			bool group_is_root_ ; 
        	std::vector <Dimension>			dimensions_V_ ;
        	std::vector <Attribute>			attributes_V_ ;
        	std::vector <Variable>			variables_V_ ;
            std::vector <struct Group>      nestedgroups_V_ ;
	} ;


public:
	/**
     * Method to dump the handler content.
     * @param[in] t Additional tab string (empty by default)
     */
    std::string dump(const std::string& t="") const ;
	
    /**
     * Method to reset all groups.
     */
    inline void reset_groups()							{ groups_V_.clear() ; }
	
    /**
     * It wraps reset_groups.
     */
    inline void reset()									{ reset_groups() ; }
	
    /**
     * Method to read the netCDF XML template previously stored in a resource set.
     * @param[in] rs The rsResourceSet object storing the netCDF XML template
     */
    void read(rsResourceSet &rs) ;
    
    /**
     * Method to read an XML netCDF template.
     * @param[in] xml_file_in The name of the XML template file
     */
	void read(const std::string& xml_file_in) ;
    
    /**
     * Method to write the handler content to an XMLOstream.
     * @param[in] xst The XML output stream object
     * @param[in] schema_name The XML schema file name
     * @param[in] alter_group_tag Boolean indicating if the XML group tag should include the group name (true) or not (false)
     */
	void write(XMLOstream& xst, const std::string& schema_name, const bool& alter_group_tag) const ;
	
    /**
     * Method to write the handler content to an XML file.
     * @param[in] xml_file_out The name of the output XML file
     * @param[in] schema_name The XML schema file name
     * @param[in] alter_group_tag Boolean indicating if the XML group tag should include the group name (true) or not (false)
     */
    void write(const std::string& xml_file_out, const std::string& schema_name, const bool& alter_group_tag) const ;
	
    /**
     * Method to add a new group.
     * @param[in] g Add a Group object
     */
    inline size_t add_group(const Group& g)				{ groups_V_.push_back(g) ; return groups_V_.size() ; }
	
    /**
     * Method to get all the groups (as a vector).
     * @return The list of the Group objects
     */
    inline std::vector <Group> get_groups() const		{ return groups_V_ ; }
    
    /**
     * Method to check if the handler is empty or not.
     * @return A boolean indicating if the group is empty (true) or not (false)
     */
    inline const bool empty() const                     { return groups_V_.empty() ; }
	
    /**
     * Method to get a specific group by its name.
     * @param[in] groupName The name of the group
     */
    Group get_group(const std::string& groupName) const ;
    
    /**
     * Method to set a specific group in the handler.
     * @param[in] grp The Group object to be added
     */
    void set_group(const Group& grp) ;
    
    /**
     * Method to remove a specific group by its name.
     * @return Boolena equal to true if group(s) with requested name has(ve) been removed
     */
    bool remove_group(const std::string& groupName);
    
private:
	std::vector <Group>	groups_V_ ;
	
private:

	ACS_CLASS_DECLARE_DEBUG_LEVEL(Netcdf4xmlHandler) ;

};	// END: class Netcdf4xmlHandler 

_ACS_END_NAMESPACE
	

#endif //_Netcdf4xmlHandler_H_
