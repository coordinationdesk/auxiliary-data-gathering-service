/*
	Copyright 2018-2021, Advanced Computer Systems , Inc.
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
	Revision 1.1  2018/07/13 09:02:27  enrcar
	EC:: Modules added: Netcdf4xmlConverter, Netcdf4xmlHandler
	

        
*/ 

#include <Netcdf4xmlHandler.h>
#include <XMLOstream.h>
#include <XMLIstream.h>
#include <Filterables.h>
#include <rsResourceSet.h>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <StringUtils.h>
#include <set>
#include <fstream>
#include <sstream>
#include <string>
#include <numeric> 

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(Netcdf4xmlHandler) ;
	
const vector<string> Netcdf4xmlHandler::types = {"byte","ubyte","char","string","short","ushort","int","uint","int64","uint64","float","double"} ;
const vector<size_t> Netcdf4xmlHandler::typesSize = {1,sizeof(char),sizeof(char),0,sizeof(short),sizeof(unsigned short),sizeof(int),sizeof(unsigned int),sizeof(long int),sizeof(unsigned long int),sizeof(float),sizeof(double)};

/**
 * Constructor.
 * @param[in] filename The name of the netCDF XML file template
 */
Netcdf4xmlHandler::Netcdf4xmlHandler(const string& filename) :
	groups_V_()

{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method called. Filename: \"" << filename << "\"" << endl ;
	ACS_CLASS_END_DEBUG

	read(filename) ;
	
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Constructor finished" << endl ;
	ACS_CLASS_END_DEBUG
}


/**
 * Destructor.
 */
Netcdf4xmlHandler::~Netcdf4xmlHandler() throw()
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method called" << endl ;
	ACS_CLASS_END_DEBUG
}


/**
 * Copy Constructor.
 */
Netcdf4xmlHandler::Netcdf4xmlHandler(const Netcdf4xmlHandler& inp) : groups_V_(inp.groups_V_)
{
    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Copy constructor Called.") ;
   
   *this = inp;
}

/**
 * Overload of operator =.
 */
Netcdf4xmlHandler& Netcdf4xmlHandler::operator = (const Netcdf4xmlHandler& inp) 
{

    ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Operator equal Called.") ;

    if (this != &inp) {
        groups_V_ = inp.groups_V_ ;
    }

   return *this;
}

/**
 * Method to dump the handler content.
 * @param[in] t Additional tab string (empty by default)
 */
string Netcdf4xmlHandler::dump(const std::string& t) const
{
	ostringstream o ; 
	for (size_t i=0; i<groups_V_.size(); i++)
	{
		o << groups_V_.at(i).dump() ; 
	}	
		
	return o.str() ; 
}

/**
 * Method to read an XML netCDF template.
 * @param[in] xml_file_in The name of the XML template file
 */
void Netcdf4xmlHandler::read(const string& xml_file_in) 
{
	if ( (false == File::exists(xml_file_in)) || (true == File::isDir(xml_file_in)) )
	{
		ACS_THROW(exIOException("Invalid input file: \"" + xml_file_in + "\"")) ;
	}

	rsResourceSet rs;
	XMLIstream xmlIstream(xml_file_in);
	xmlIstream >> rs;
	read(rs) ;
}

/**
 * Method to read the netCDF XML template previously stored in a resource set.
 * @param[in] rs The rsResourceSet object storing the netCDF XML template
 */
void Netcdf4xmlHandler::read(rsResourceSet &rs) 
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method called" << endl ;
	ACS_CLASS_END_DEBUG
    
    
    groups_V_.clear() ;
    const string tagname = "group" ;
	const size_t val_n = rs.getArraySize(tagname) ;
	if (val_n>0)
	{
		// First of all try to read global dimensions/attributes/variables if there are
        {
            Group group ; 
		    group.read(rs) ;
		    if (false == group.empty()) { groups_V_.push_back(group) ; }
        }
        
        for (size_t i=0; i<val_n; i++)
		{
			ostringstream tag ; tag << tagname << "[" << i << "]" ; 
			if ( false == rs.hasKey(tag.str()) ) { tag.str("") ; tag << tagname ; } 
			const bool group_is_root (false) ; 
			Group group(group_is_root, "", i) ; 
			group.read(rs) ;
			groups_V_.push_back(group) ; 
		}	// END: for i 
	} else {
		// IF HERE: Either group is root, or no group at all !

		Group group ; 
		group.read(rs) ;
		if (false == group.empty()) { groups_V_.push_back(group) ; }
	}

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method finished." << endl ;
	ACS_CLASS_END_DEBUG
}

/**
 * Method to write the handler content to an XMLOstream.
 * @param[in] xst The XML output stream object
 * @param[in] schema_name The XML schema file name
 * @param[in] alter_group_tag Boolean indicating if the XML group tag should include the group name (true) or not (false)
 */
void Netcdf4xmlHandler::write(XMLOstream& xst, const string& schema_name_in, const bool& alter_group_tag) const 
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method called" << endl ;
	ACS_CLASS_END_DEBUG

	const string schema_name ( schema_name_in.empty()? "" : File::getFileName(schema_name_in) ) ;
	
	string schema ; if (false == schema_name.empty())
	{
		ostringstream o ; 
			o << "xmlns=\"http://www.example.com/" << schema_name << "\"" \
	 		<< " xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"" \
			<< " xsi:schemaLocation=\"http://www.example.com/" << schema_name << " " << schema_name << ".xsd\"" ;	
		schema = o.str() ; 
	} else {
		schema = "xmlns=\"http://www.unidata.ucar.edu/namespaces/netcdf/ncml-2.2\"";
	}

	XMLOstream::Tag t(xst, (/*nmspace_ +*/ "netcdf"), "", schema);
	for (size_t i=0; i<groups_V_.size(); i++)
	{
		groups_V_.at(i).write(xst, alter_group_tag) ; 
	}	

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method finished" << endl ;
	ACS_CLASS_END_DEBUG
}

/**
 * Method to write the handler content to an XML file.
 * @param[in] xml_file_out The name of the output XML file
 * @param[in] schema_name The XML schema file name
 * @param[in] alter_group_tag Boolean indicating if the XML group tag should include the group name (true) or not (false)
 */
void Netcdf4xmlHandler::write(const string& fileName, const string& schema_name, const bool& alter_group_tag) const
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method called" << endl ;
	ACS_CLASS_END_DEBUG

	ofstream of ; of.open(fileName.c_str(), ofstream::out | std::ofstream::trunc) ;
	of << "<?xml version =\"1.0\" encoding=\"UTF-8\"?>\n" ;
	of << "<!-- Generator: $Id$ -->\n\n" ; 
	
	XMLOstream xst(of) ;  
	write(xst, schema_name, alter_group_tag) ;

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method finished" << endl ;
	ACS_CLASS_END_DEBUG
}

/**
 * Method to get a specific group by its name.
 * @param[in] groupName The name of the group
 */
Netcdf4xmlHandler::Group Netcdf4xmlHandler::get_group(const string& groupName) const
{
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method called") ;
    
    Group output ;
	for(size_t i=0; i<groups_V_.size(); ++i)
    {
        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "i=" << i << " groupName=" << groups_V_[i].get_name() << " (searching=" << groupName << ").") ;
        if( groups_V_[i].get_name() == groupName ||
            (groupName=="/" && groups_V_[i].get_group_is_root()) )
        {
            output = groups_V_[i] ;
            break ;
        }
        
        output = groups_V_[i].get_nestedgroup(groupName) ;
        if(!output.empty())
            break ;
    }
    
    ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method finished") ;
        
    return(output) ;
}

/**
 * Method to set a specific group in the handler.
 * @param[in] grp The Group object to be added
 */
void Netcdf4xmlHandler::set_group(const Netcdf4xmlHandler::Group& grp)
{
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method called") ;
    
    const string groupName = grp.get_name() ;
	for(size_t i=0; i<groups_V_.size(); ++i)
    {
        if( groups_V_[i].get_name() == groupName ||
            (groupName=="/" && groups_V_[i].get_group_is_root()) )
        {
            groups_V_[i] = grp ;
            break ;
        }
        
        if( groups_V_[i].set_nestedgroup(grp) )
            break ;
    }
    
    ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method finished") ;
}

/**
 * Method to remove a specific group by its name.
 * @return Boolena equal to true if group(s) with requested name has(ve) been removed
 */
bool Netcdf4xmlHandler::remove_group(const string& groupName)
{
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method called") ;
    
    bool removed = false;
	for(size_t i=0; i<groups_V_.size(); ++i)
    {
        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "i=" << i << " groupName=" << groups_V_[i].get_name() << " (searching=" << groupName << ").") ;
        if( (groups_V_[i].get_name() == groupName) &&
            (groupName!="/" || !groups_V_[i].get_group_is_root()) )
        {
            groups_V_.erase (groups_V_.begin()+i);
            --i;
            removed = true;
            continue;
        }
        
        removed = groups_V_[i].remove_nestedgroup(groupName) ;
    }
    
    ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method finished") ;
        
    return(removed) ;
}

/****************************************************************************
**
**		Dimension
**
****************************************************************************/

/** 
 * Copy constructor.
 */
Netcdf4xmlHandler::Dimension::Dimension(const Netcdf4xmlHandler::Dimension & r): 		
    idx_(r.idx_),
    name_(r.name_),
    length_(r.length_)
{
}

/**
 * Overload of the Operator= 
 */
Netcdf4xmlHandler::Dimension& Netcdf4xmlHandler::Dimension::operator=(const Netcdf4xmlHandler::Dimension &r)
{
    if (&r != this) 
    {
        idx_            = r.idx_;
        name_           = r.name_;
        length_         = r.length_;
    }
	
    return *this ; 
}

/**
 * Dump the dimension content.
 * @param[in] tab Additional tab string (empty by default)
 */
string Netcdf4xmlHandler::Dimension::dump(const std::string& t) const
{
	ostringstream o ; 
	o << t << "Dimension [" << idx_ << "]:\tname: \"" << name_ << "\"\tlength: " << length_ << "\n" ; 
	return o.str() ; 
} 

/**
 * Read the Dimension from the rsResourceSet object.
 * @param[in] rs rsResourceSet object storing the XML previously read
 */
void Netcdf4xmlHandler::Dimension::read(rsResourceSet &rs) 
{
	const string tagname ( "dimension" ) ;
	ostringstream tag ; tag << tagname << "[" << idx_ << "]" ; 
	if ( false == rs.hasKey(tag.str()) ) { tag.str("") ; tag << tagname ; } 
	rsPushNSpace ns1(rs, tag.str()) ;
		// Read attribute(s) :
		const string name_key ( string("name") +".value" ) ; 
		if (true==rs.hasKey(name_key)) { rs.getValue(name_key, name_) ; } else { name_ = "" ; } 
		const string length_key ( string("length") +".value" ) ; 
		if (true==rs.hasKey(length_key)) { rs.getValue(length_key, length_) ; } else { length_ = "" ; } 
	;
}

/**
 * Write the Dimension to the XMLOstream.
 * @param[in] xst The XML output stream
 * @param[in] alter_tag Boolean indicating if the XML dimension tag should include the dimension name (true) or not (false)
 */
void Netcdf4xmlHandler::Dimension::write(XMLOstream& xst, const bool& alter_tag) const 
{
	ostringstream attr, tagname ;

	if (false == alter_tag)
	{
		tagname << "dimension" ;
		attr << "name=\"" << name_ << "\" " << "length=\"" << length_ << "\"" ;
		
	} else {
		tagname << "dimension_" << name_ ;
		attr << "length=\"" << length_ << "\"" ;
	}	
		
	XMLOstream::Tag h(xst, (/*nmspace_ +*/tagname.str()), "", attr.str(), true/*means: closeInLine*/) ;	  
}

/****************************************************************************
**
**		Attribute
**
****************************************************************************/

/** 
 * Copy constructor.
 */
Netcdf4xmlHandler::Attribute::Attribute(const Netcdf4xmlHandler::Attribute & r): 		
    idx_(r.idx_),
    name_(r.name_),
    value_(r.value_),
    type_(r.type_)
{
}

/**
 * Overload of the Operator= 
 */
Netcdf4xmlHandler::Attribute& Netcdf4xmlHandler::Attribute::operator=(const Netcdf4xmlHandler::Attribute &r)
{
    if (&r != this) 
    {
        idx_            = r.idx_;
        name_           = r.name_;
        value_          = r.value_;
        type_           = r.type_;
    }
	
    return *this ; 
}

/**
 * Dump the attribute content.
 * @param[in] tab Additional tab string (empty by default)
 */
string Netcdf4xmlHandler::Attribute::dump(const std::string& t) const
{
	ostringstream o ; 
	o << t << "Attribute [" << idx_ << "]:\tname: \"" << name_ << "\"\tvalue: \"" << value_ << "\"\ttype: \"" << type_ << "\"\n" ;
	return o.str() ; 
} 

/**
 * Read the Attribute from the rsResourceSet object.
 * @param[in] rs rsResourceSet object storing the XML previously read
 */
void Netcdf4xmlHandler::Attribute::read(rsResourceSet &rs) 
{
	const string tagname ( "attribute" ) ;
	ostringstream tag ; tag << tagname << "[" << idx_ << "]" ; 
	if ( false == rs.hasKey(tag.str()) ) { tag.str("") ; tag << tagname ; } 
	rsPushNSpace ns1(rs, tag.str()) ;
		// Read attribute(s) :
		const string name_key ( string("name") +".value" ) ; 
		if (true==rs.hasKey(name_key)) { rs.getValue(name_key, name_) ; } else { name_ = "" ; } 
		const string value_key ( string("value") +".value" ) ; 
		if (true==rs.hasKey(value_key)) { rs.getValue(value_key, value_) ; } else { value_ = "" ; } 
		const string type_key ( string("type") +".value" ) ; 
		if (true==rs.hasKey(type_key)) { rs.getValue(type_key, type_) ; } else { type_ = "" ; } 
	;
}

/**
 * Write the Attribute to the XMLOstream.
 * @param[in] xst The XML output stream
 * @param[in] alter_tag Boolean indicating if the XML attribute tag should include the attribute name (true) or not (false)
 */
void Netcdf4xmlHandler::Attribute::write(XMLOstream& xst, const bool& alter_tag) const 
{
	ostringstream attr, tagname ; 
	
	if (false == alter_tag)
	{
		tagname << "attribute" ;
		attr << "name=\"" << name_ << "\" " << "value=\"" << value_ << "\" " ;
		
	} else {
		tagname << "attribute_" << name_ ;
		attr << "value=\"" << value_ << "\" " ;
	}	
	
	if (false == type_.empty()) { attr << "type=\"" << type_ << "\"" ; }
	XMLOstream::Tag h(xst, (/*nmspace_ +*/tagname.str()), "", attr.str(), true/*means: closeInLine*/) ;	  
}

/**
 * Check that the type string is one of those allowed.
 * @param[in] str_type The string to be checked 
 */
bool Netcdf4xmlHandler::Attribute::goodType(const string& str_type) const
{
    for(size_t i=0; i<Netcdf4xmlHandler::types.size(); ++i)
    { 
	    if(str_type == Netcdf4xmlHandler::types[i])
        {
           return(true);
        }
    }
    return(false);
}

/**
 * Set the string defining the attribute type.
 * @param[in] str_typ The string identifier of the type 
 */
void Netcdf4xmlHandler::Attribute::set_type(const std::string& typ)
{ 
    ACS_COND_THROW(!goodType(typ),exNetcdf4xmlHandlerCriticalException(" Failed. Unknown type \""+typ+"\"."));
    type_ = typ ; 
}

/****************************************************************************
**
**		Variable
**
****************************************************************************/

/** 
 * Destructor.
 */
Netcdf4xmlHandler::Variable::~Variable() throw() 
{
    if(dataValues_)
    {
        // dataValues_ previously filled. Free it.
        if(dataValuesHere_)
        {
            free(dataValues_);
        }
        dataValues_ 	= 0;
        dataValuesSize_.clear();
    }
}

/** 
 * Copy constructor.
 */
Netcdf4xmlHandler::Variable::Variable(const Netcdf4xmlHandler::Variable & r): 		
    idx_(r.idx_),
    name_(r.name_),
    type_(r.type_),
    shape_(r.shape_),
    values_(r.values_),
    values_is_set_(r.values_is_set_),
    attributes_V_(r.attributes_V_),
    filename_(r.filename_),
    fileformat_(r.fileformat_),
    ascii_separator_(r.ascii_separator_),
    ascii_cols_(r.ascii_cols_),
    fillValue_(r.fillValue_),
    dataValuesSize_(),
    dataValues_(0),
    dataValuesHere_(r.dataValuesHere_)
{
    if(r.dataValues_ && r.dataValuesSize_.size()>0)
    {
        // Compute the data buffer size in bytes
        int totSize = 0;
        totSize = std::accumulate(r.dataValuesSize_.begin(), r.dataValuesSize_.end(), 1, std::multiplies<int>());
        totSize *= getTypeSize();
        
        if(r.dataValuesHere_)
        {
            // Allocate a new memory buffer
            dataValues_ = calloc(totSize, 1);
            // Fill with the provided data
            memcpy(dataValues_, r.dataValues_, totSize);
        }
        else
        {
            dataValues_ = r.dataValues_;
        }
        
        // Update the sizes
        dataValuesSize_ = r.dataValuesSize_;
        // Set to false the flag showing if the variable data are set as strings
        values_is_set_ = false;
    }
}

/**
 * Overload of the Operator= 
 */
Netcdf4xmlHandler::Variable& Netcdf4xmlHandler::Variable::operator=(const Netcdf4xmlHandler::Variable &r)
{
    if (&r != this) 
    {
        idx_             = r.idx_;
        name_            = r.name_;
        type_            = r.type_;
        shape_           = r.shape_;
        values_          = r.values_;
        values_is_set_   = r.values_is_set_;
        attributes_V_    = r.attributes_V_;
        filename_        = r.filename_;
        fileformat_      = r.fileformat_;
        ascii_separator_ = r.ascii_separator_;
        ascii_cols_      = r.ascii_cols_;
		fillValue_       = r.fillValue_;
        
        if(dataValues_ && dataValuesHere_)
        {
            // dataValues_ previously filled. Free it.
            free(dataValues_);
        }
        if(r.dataValues_ && r.dataValuesSize_.size()>0)
        {
            // Compute the data buffer size in bytes
            int totSize = 0;
            totSize = std::accumulate(r.dataValuesSize_.begin(), r.dataValuesSize_.end(), 1, std::multiplies<int>());
            totSize *= getTypeSize();
            if(r.dataValuesHere_)
            {
                // Allocate a new memory buffer
                dataValues_ = calloc(totSize, 1);
                // Fill with the provided data
                memcpy(dataValues_, r.dataValues_, totSize);
            }
            else
            {
                dataValues_ = r.dataValues_;
            }
            // Update the sizes
            dataValuesSize_ = r.dataValuesSize_;
            // Set to false the flag showing if the variable data are set as strings
            values_is_set_ = false;
        }
        else
        {
            dataValues_ 	= 0;
            dataValuesSize_.clear();
        }
        dataValuesHere_ = r.dataValuesHere_;
    }
	
    return *this ; 
}

/**
 * Dump the variable.
 * @param[in] tab Additional tab string (empty by default)
 */
string Netcdf4xmlHandler::Variable::dump(const std::string& t) const
{
	ostringstream o ; 
	o << t << "Variable [" << idx_ << "]:\tname: \"" << name_ << "\"\ttype: \"" << type_ 
		<< "\"\tshape: \"" << shape_ << "\"\n" ;
	for (size_t i=0; i<attributes_V_.size(); i++)	{ o << attributes_V_.at(i).dump(t+"\t") ; }
	if (true == values_is_set_) { o << t << "\tvalues:\t{\n" << t << "\t\t" << values_ << "\n" << t << "\t}\n" ; }
	return o.str() ; 
} 

/**
 * Set the data of a variable providing a pointer to the data buffer (it will be copied)
 * @param[in] dataSize The list of data size [dim1, dim2, ..., dimN]
 * @param[in] data The data buffer
 * @param[in] dataType The identifier of the data type
 */
void Netcdf4xmlHandler::Variable::set_dataValues(
    const vector<int>& dataSize, 
    const void* data,
    const string& dataType)
{
    if(dataValues_ && dataValuesHere_)
    {
        // dataValues_ previously filled. Free it.
        free(dataValues_);
    }
    if(data && dataSize.size()>0)
    {
        // Compute the data buffer size in bytes 
        int totSize = 0;
        totSize = std::accumulate(dataSize.begin(), dataSize.end(), 1, std::multiplies<int>());
        
        if(dataType.empty())
        {
            set_type("byte");
        }
        else
        {
            set_type(dataType);
        }
        ACS_COND_THROW(!goodType(type_),exNetcdf4xmlHandlerCriticalException("Failed. Variable=\""+name_+"\": Unknown type \""+dataType+"\"."));
        
        totSize *= getTypeSize();
        
        dataValuesHere_ = true;
        // Allocate a new memory buffer
        dataValues_ = calloc(totSize, 1);
        ACS_COND_THROW(!dataValues_,
                    exNetcdf4xmlHandlerCriticalException("Failed. Allocating memory buffer for variable=\""+name_+"\"."));
        
        // Fill with the provided data
        memcpy(dataValues_, data, totSize);
        // Update the sizes
        dataValuesSize_ = dataSize;
        // Set to false the flag showing if the variable data are set as strings
        values_is_set_ = false;
    }
    else
    {
        dataValues_ 	= 0;
        dataValuesSize_.clear();
    }
}


/**
 * Set the data of a variable providing a pointer to the data buffer.
 * @param[in] dataSize The list of data size [dim1, dim2, ..., dimN]
 * @param[in] data The data buffer
 * @param[in] dataType The identifier of the data type
 */
void Netcdf4xmlHandler::Variable::set_dataValues_pointer(
    const vector<int>& dataSize, 
    void* data,
    const string& dataType)
{
    if(dataValues_ && dataValuesHere_)
    {
        // dataValues_ previously filled. Free it.
        free(dataValues_);
    }
    if(data && dataSize.size()>0)
    {
        if(dataType.empty())
        {
            set_type("byte");
        }
        else
        {
            set_type(dataType);
            ACS_COND_THROW(!goodType(type_),exNetcdf4xmlHandlerCriticalException("Failed. Variable=\""+name_+"\": Unknown type \""+dataType+"\"."));
        }
        
        dataValuesHere_ = false;
        
        // Store the pointer to the data values
        dataValues_ = data;
        
        // Update the sizes
        dataValuesSize_ = dataSize;
        
        // Set to false the flag showing if the variable data are set as strings
        values_is_set_ = false;
    }
    else
    {
        dataValues_ 	= 0;
        dataValuesSize_.clear();
    }
}


/**
 * Udate the data (max 3D) of a variable providing a pointer to the data buffer.
 * @param[in] dataSize The list of data size [dim1, dim2, dim3]
 * @param[in] data The data buffer
 * @param[in] dataType The identifier of the data type
 */
void Netcdf4xmlHandler::Variable::update_dataValues(
    const vector<int>& dataSize, 
    const void* data,
    const string& dataType)
{
    if(!dataValues_)
    {
        // Variable date not filled yet
        set_dataValues(dataSize, data, dataType);
    }
    if(data && dataSize.size()>0)
    {
        ACS_COND_THROW((dataValuesSize_.size()>3),
                       exNetcdf4xmlHandlerCriticalException("Failed. Variable=\""+name_+"\": update_dataValues can be made only on data of at maximum 3D."));
        
        ACS_COND_THROW((dataType!=type_),
                       exNetcdf4xmlHandlerCriticalException("Failed. Variable=\""+name_+"\": Type mismatch (\""+dataType+"\" but found \""+type_+"\")."));
        
        /// Set the size of the buffer at the moment stored in memory
        const int oldSize = std::accumulate(dataValuesSize_.begin(), dataValuesSize_.end(), 1, std::multiplies<int>()) * getTypeSize();
        
        /// Check that all the "faster" dimensions have the same values
        if(dataValuesSize_.size()>1)
        {
            for(size_t i=(dataValuesSize_.size()-1); i>0; --i)
            {
                ACS_COND_THROW((dataValuesSize_[i]!=dataSize[i]),
                    exNetcdf4xmlHandlerCriticalException("Failed. Variable=\""+name_+"\": Mismatch on dimensions."));
            }
        }
        
        /// Update the size of the first dimension (the "slower")
        dataValuesSize_[0] += dataSize[0];
        
        /// Compute the new buffer size
        const int newSize = std::accumulate(dataValuesSize_.begin(), dataValuesSize_.end(), 1, std::multiplies<int>()) * getTypeSize();
        
        // Allocate a new memory buffer
        dataValues_ = realloc(dataValues_, newSize);
        ACS_COND_THROW(!dataValues_,
                    exNetcdf4xmlHandlerCriticalException("Failed. Re-allocating memory buffer for variable=\""+name_+"\"."));
        
        // Append with the provided data
        unsigned char* p = reinterpret_cast<unsigned char*>(dataValues_);
        memcpy(&p[oldSize], data, (newSize-oldSize));
        
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Var \"" << this->get_name() << "\": first size updated to " << dataValuesSize_[0]
            << "(newSize=" << newSize << ", oldSize=" << oldSize << ")."); 
    }
}

/**
 * Get the data of a variable if stored in a data buffer.
 * @param[out] dataSize The list of data size [dim1, dim2, ..., dimN]
 * @param[out] data The data buffer
 */
void Netcdf4xmlHandler::Variable::get_dataValues(vector<int>& dataSize, void** data) const
{
    if(*data)
    {
        // If provided memory buffer is not empty, free it.
        free(*data);
    }
    
    if(dataValues_ && dataValuesSize_.size()>0)
    {
        
        // Compute the data buffer size in bytes
        int totSize = 0;
        totSize = std::accumulate(dataValuesSize_.begin(), dataValuesSize_.end(), 1, std::multiplies<int>());
        totSize *= getTypeSize();
        // Allocate memory to the output buffer
        *data = calloc(totSize, 1) ;
        // Fill the output buffer
        memcpy(*data, dataValues_, totSize);
        // Set the returned data sizes
        dataSize = dataValuesSize_;
    }
    else
    {
        // Returned empty data
        *data       = 0;
        dataSize.clear();
    }
}

void Netcdf4xmlHandler::Variable::get_dataValues_pointer(vector<int>& dataSize, void** data) const
{
    if(dataValues_ && dataValuesSize_.size()>0)
    {
        *data = dataValues_;
        // Set the returned data sizes
        dataSize = dataValuesSize_;
    }
    else
    {
        // Returned empty data
        *data       = 0;
        dataSize.clear();
    }
}

/**
 * Get the size of the type of the data stored by the Variable.
 * @return The size of the type of the data stored by the Variable.
 */
size_t Netcdf4xmlHandler::Variable::getTypeSize() const
{
    const string str_type(get_type());
    const string value(get_values());
    size_t typeSize = 0 ;
    
    for(size_t i=0; i<Netcdf4xmlHandler::types.size(); ++i)
    {
        if(str_type == Netcdf4xmlHandler::types[i])
        {
            typeSize = (str_type == "string")? value.size():Netcdf4xmlHandler::typesSize[i];
        }
    }
    return(typeSize);
}

/**
 * Check that the type string is one of those allowed.
 * @param[in] str_type The string to be checked 
 */
bool Netcdf4xmlHandler::Variable::goodType(const string& str_type) const
{
    for(size_t i=0; i<Netcdf4xmlHandler::types.size(); ++i)
    { 
	    if(str_type == Netcdf4xmlHandler::types[i])
        {
           return(true);
        }
    }
    return(false);
}

/**
 * Set the string defining the attribute type.
 * @param[in] str_typ The string identifier of the type 
 */
void Netcdf4xmlHandler::Variable::set_type(const std::string& typ)
{ 
    ACS_COND_THROW(!goodType(typ),exNetcdf4xmlHandlerCriticalException(" Failed. Unknown type \""+typ+"\"."));
    type_ = typ ; 
}

/**
 * Read the Variable from the rsResourceSet object.
 * @param[in] rs rsResourceSet object storing the XML previously read
 */
void Netcdf4xmlHandler::Variable::read(rsResourceSet &rs) 
{
	const string tagname ( "variable" ) ;
	ostringstream tag ; tag << tagname << "[" << idx_ << "]" ; 
	if ( false == rs.hasKey(tag.str()) ) { tag.str("") ; tag << tagname ; } 
	
	{
		rsPushNSpace ns1(rs, tag.str()) ;
			const string name_key ( string("name") +".value" ) ; 
			if (true==rs.hasKey(name_key)) { rs.getValue(name_key, name_) ; } else { name_ = "" ; } 
			const string type_key ( string("type") +".value" ) ; 
			if (true==rs.hasKey(type_key)) { rs.getValue(type_key, type_) ; } else { type_ = "" ; } 
			const string shape_key ( string("shape") +".value" ) ; 
			if (true==rs.hasKey(shape_key)) { rs.getValue(shape_key, shape_) ; } else { shape_ = "" ; } 
			const string values_key ( string("values") +".value" ) ; 
			values_is_set_ = rs.hasKey(values_key) ;
			if (true==rs.hasKey(values_key)) { rs.getValue(values_key, values_) ; } else { values_ = "" ; } 
	        
            const string filename_key ( string("filename") +".value" ) ; 
			if (true==rs.hasKey(filename_key)) { rs.getValue(filename_key, filename_) ; } else { filename_ = "" ; }
            const string fileformat_key ( string("fileformat") +".value" ) ; 
			if (true==rs.hasKey(fileformat_key)) { rs.getValue(fileformat_key, fileformat_) ; } else { fileformat_ = "" ; }
            const string ascii_separator_key ( string("ascii_separator") +".value" ) ; 
			if (true==rs.hasKey(ascii_separator_key)) { rs.getValue(ascii_separator_key, ascii_separator_) ; } else { ascii_separator_ = "" ; }
			const string ascii_cols_key ( string("ascii_cols") +".value" ) ; 
			if (true==rs.hasKey(ascii_cols_key)) 
            { 
                string ascii_colsIN;
                rs.getValue(ascii_cols_key, ascii_colsIN);
                vector<string> ascii_colsV;
                StringUtils::Tokenize(ascii_colsIN, ascii_colsV, " ") ;
                if(ascii_colsV.empty())
                {
                    ascii_cols_.push_back(atoi(ascii_colsIN.c_str()));
                }
                else
                {
                    for(vector<string>::iterator it=ascii_colsV.begin(); it!=ascii_colsV.end(); ++it)
                    {
                        ascii_cols_.push_back(atoi((*it).c_str()));
                    }
                }
            } 
            else 
            { 
                ascii_cols_.clear();
            }
            
            attributes_V_.clear() ;
			const string tagname = "attribute" ;
			const size_t val_n = rs.getArraySize(tagname) ;
			for (size_t i=0; i<val_n; i++)
			{
				ostringstream tag ; tag << tagname << "[" << i << "]" ; 
				if ( false == rs.hasKey(tag.str()) ) { tag.str("") ; tag << tagname ; } 
				Attribute attribute(i) ; 
				attribute.read(rs) ;
				add_attribute(attribute) ; 
                if( attribute.get_name() == "_FillValue" )
                {
                    fillValue_ = attribute.get_value() ;
                }
			}	// END: for i 
		
	}	// END SCOPE

}

/**
 * Write the Variable to the XMLOstream.
 * @param[in] xst The XML output stream
 * @param[in] alter_tag Boolean indicating if the XML variable tag should include the variable name (true) or not (false)
 */
void Netcdf4xmlHandler::Variable::write(XMLOstream& xst, const bool& alter_tag) const 
{
	ostringstream attr, tagname ; 
	
	if (false == alter_tag)
	{
		tagname << "variable" ;
		attr << "name=\"" << name_ << "\" " << "type=\"" << type_ << "\" " << "shape=\"" << shape_ << "\" " ; 
	} else {
		tagname << "variable_" << name_ ;
		attr << "type=\"" << type_ << "\" " << "shape=\"" << shape_ << "\" " ; 
	}
    
    if(!filename_.empty())
	{
        attr << "filename=\"" << filename_ << "\"";
    }
    if(!fileformat_.empty())
    {
        attr << " fileformat=\"" << fileformat_ << "\"";
    }
    if(!ascii_separator_.empty())
    {
        attr << " ascii_separator=\"" << ascii_separator_ << "\"";
    }
    if(!ascii_cols_.empty())
    {
        attr << " ascii_cols=\"";
        for(size_t i=0; i<ascii_cols_.size(); ++i)
        {
            if(i>0) attr << " ";
            attr << ascii_cols_[i];
        }
        attr << "\"";
    }
    
	{	
		const bool close_tag_immediat ( (false == values_is_set_) && (true == attributes_V_.empty())  ) ;
		XMLOstream::Tag h(xst, (/*nmspace_ +*/tagname.str()), "", attr.str(), close_tag_immediat ) ;	
		if (false == close_tag_immediat)
		{
			if (false == attributes_V_.empty())
			{
				for (size_t i=0; i<attributes_V_.size(); i++) { attributes_V_.at(i).write(xst, alter_tag) ; }
			}
			if (true == values_is_set_)
			{
				XMLOstream::Tag h(xst, (/*nmspace_ +*/"values"), values_, "", true/*means: closeInLine*/) ;	  	  
			}
		}
	}	// END SCOPE
}

/**
  * Get the Attribute object by its name.
  * @param[in] attributeName The name of the attribute 
  */
Netcdf4xmlHandler::Attribute Netcdf4xmlHandler::Variable::get_attribute(const string& attributeName)
{
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method called") ;
    
    Attribute output ;
	for(size_t i=0; i<attributes_V_.size(); ++i)
    {
        if( attributes_V_[i].get_name() == attributeName )
        {
            output = attributes_V_[i] ;
            break ;
        }
    }

    ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method finished") ;
    
    return(output) ;
}

/**
 * Set a new Attribute object.
 * @param[in] attr The new attribute object
 */
int Netcdf4xmlHandler::Variable::set_attribute(const Netcdf4xmlHandler::Attribute& attr)
{
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method called") ;
    
    int found = 0 ;
	for(size_t i=0; i<attributes_V_.size(); ++i)
    {
        if( attributes_V_[i].get_name() == attr.get_name() )
        {
            attributes_V_[i] = attr;
            found = 1 ;
            break ;
        }
    }
    
    if(!found)
        add_attribute(attr) ;
    
    ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method finished") ;
    
    return(found) ;
}


/****************************************************************************
**
**		Group
**
****************************************************************************/

/** 
 * Copy constructor.
 */
Netcdf4xmlHandler::Group::Group(const Netcdf4xmlHandler::Group & r): 		
    idx_(r.idx_),
    name_(r.name_),
    group_is_root_(r.group_is_root_),
    dimensions_V_(r.dimensions_V_),
    attributes_V_(r.attributes_V_),
    variables_V_(r.variables_V_),
    nestedgroups_V_(r.nestedgroups_V_)
{
}

/**
 * Overload of the Operator= 
 */
Netcdf4xmlHandler::Group& Netcdf4xmlHandler::Group::operator=(const Netcdf4xmlHandler::Group &r)
{
    if (&r != this) 
    {
        idx_            = r.idx_;
        name_           = r.name_;
        group_is_root_  = r.group_is_root_;
        dimensions_V_   = r.dimensions_V_;
        attributes_V_   = r.attributes_V_;
        variables_V_    = r.variables_V_;
        nestedgroups_V_ = r.nestedgroups_V_;
    }
	
    return *this ; 
}

/**
 * Dump the group content.
 * @param[in] tab Additional tab string (empty by default)
 */
string Netcdf4xmlHandler::Group::dump(const std::string& t) const
{
	ostringstream o ; 
	if (false == get_group_is_root()) { o << t << "Group [" << idx_ << "]: \"" <<name_  << "\"\t{\n" ; }
	else { o << t << "{\n" ; }

	for (size_t i=0; i<dimensions_V_.size(); i++)	{ o << dimensions_V_.at(i).dump(t+"\t") ; }
	for (size_t i=0; i<attributes_V_.size(); i++)	{ o << attributes_V_.at(i).dump(t+"\t") ; }
	for (size_t i=0; i<variables_V_.size(); i++)	{ o << variables_V_.at(i).dump(t+"\t") ; }
	
	o << t << "}\n" ;	
	return o.str() ; 
} 

/**
 * Read the Group from the rsResourceSet object.
 * @param[in] rs rsResourceSet object storing the XML previously read
 */
void Netcdf4xmlHandler::Group::read(rsResourceSet &rs) 
{
	if (false == group_is_root_)
	{
		// Group not root -- enter the xml tag: <group>
		const string tagname ( "group" ) ;
		ostringstream tag ; tag << tagname << "[" << idx_ << "]" ; 
		if ( false == rs.hasKey(tag.str()) ) { tag.str("") ; tag << tagname ; } 
		{
			rsPushNSpace ns1(rs, tag.str()) ;
            const size_t val_n = rs.getArraySize(tagname) ;
            
            ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
                excerr << SimpleDebugSignatureThread << "DBG: nested groups " << val_n << std::endl ;
            ACS_CLASS_END_DEBUG
            
	        if (val_n>0)
	        {
		        // Read nested groups
                nestedgroups_V_.clear() ;
                for (size_t j=0; j<val_n; j++)
		        {
                    ostringstream ggtag ; ggtag << tagname << "[" << j << "]" ; 
			        if ( false == rs.hasKey(ggtag.str()) ) { ggtag.str("") ; ggtag << tagname ; } 
			        const bool group_is_root (false) ; 
			        Group jgroup(group_is_root, "", j) ; 
			        jgroup.read(rs) ;
			        nestedgroups_V_.push_back(jgroup) ;
                }
            }
            
            // Read attribute(s) :
			const string name_key ( string("name") +".value" ) ; 
			if (true==rs.hasKey(name_key)) { rs.getValue(name_key, name_) ; } else { name_ = "" ; } 
			// Read the payload of the group :
			read_content(rs) ;
		}
	} else {
		read_content(rs) ; 
	}
}

/**
 * Read and fill the Group from the rsResourceSet object.
 * @param[in] rs rsResourceSet object storing the XML previously read
 */
void Netcdf4xmlHandler::Group::read_content(rsResourceSet &rs) 
{
	{
		dimensions_V_.clear() ;
		const string tagname = "dimension" ;
		const size_t val_n = rs.getArraySize(tagname) ;
		for (size_t i=0; i<val_n; i++)
		{
			ostringstream tag ; tag << tagname << "[" << i << "]" ; 
			if ( false == rs.hasKey(tag.str()) ) { tag.str("") ; tag << tagname ; } 
			Dimension dimension(i) ; 
			dimension.read(rs) ;
			add_dimension(dimension) ; 
		}	// END: for i 
	}
	{
		attributes_V_.clear() ;
		const string tagname = "attribute" ;
		const size_t val_n = rs.getArraySize(tagname) ;
		for (size_t i=0; i<val_n; i++)
		{
			ostringstream tag ; tag << tagname << "[" << i << "]" ; 
			if ( false == rs.hasKey(tag.str()) ) { tag.str("") ; tag << tagname ; } 
			Attribute attribute(i) ; 
			attribute.read(rs) ;
			add_attribute(attribute) ; 
		}	// END: for i 
	}
	{
		variables_V_.clear() ;
		const string tagname = "variable" ;
		const size_t val_n = rs.getArraySize(tagname) ;
		for (size_t i=0; i<val_n; i++)
		{
			ostringstream tag ; tag << tagname << "[" << i << "]" ; 
			if ( false == rs.hasKey(tag.str()) ) { tag.str("") ; tag << tagname ; } 
			Variable variable(i) ; 
			variable.read(rs) ;
			add_variable(variable) ; 
		}	// END: for i 
	}
}

/**
 * Write the Group to the XMLOstream.
 * @param[in] xst The XML output stream
 * @param[in] alter_tag Boolean indicating if the XML variable tag should include the variable name (true) or not (false)
 */
void Netcdf4xmlHandler::Group::write(XMLOstream& xst, const bool& alter_tag) const 
{   
    if (false == group_is_root_)
	{
		ostringstream attr, tagname ;
	
		if (false == alter_tag)
		{
			tagname << "group" ; 
			attr << "name=\"" << name_ << "\"" ;
		} else {
			tagname << "group_" << name_ ;
		}

		XMLOstream::Tag h(xst, (/*nmspace_ +*/tagname.str()), "", attr.str()) ;	  
        write_content(xst, alter_tag) ;	
        
        for (size_t i=0; i<nestedgroups_V_.size(); i++)
	    {
            nestedgroups_V_.at(i).write(xst, alter_tag) ; 
	    }
	}
	else
	{
		write_content(xst, alter_tag) ;	
	}
}

/**
 * Write Group dimensions, attributes, variables to the XMLOstream.
 * @param[in] xst The XML output stream
 * @param[in] alter_tag Boolean indicating if the XML variable tag should include the variable name (true) or not (false)
 */
void Netcdf4xmlHandler::Group::write_content(XMLOstream& xst, const bool& alter_tag) const 
{
	for (size_t i=0; i<dimensions_V_.size(); i++)	{ dimensions_V_.at(i).write(xst, alter_tag) ; }
	for (size_t i=0; i<attributes_V_.size(); i++)	{ attributes_V_.at(i).write(xst, alter_tag) ; }
	for (size_t i=0; i<variables_V_.size(); i++)	{ variables_V_.at(i).write(xst, alter_tag) ; }
}

/**
 * Get the Group object corresponding to a given group name.
 * @param[in] groupName The name of the nested group
 * @return The retrieved nested Group object
 */
Netcdf4xmlHandler::Group Netcdf4xmlHandler::Group::get_nestedgroup(const string& groupName) const
{
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method called") ;
    
    Group output ;
	for(size_t i=0; i<nestedgroups_V_.size(); ++i)
    {
        if( nestedgroups_V_[i].get_name() == groupName )
        {
            output = nestedgroups_V_[i] ;
            break ;
        }
        
        output = nestedgroups_V_[i].get_nestedgroup(groupName) ;
        if(!output.empty())
            break ;
    }

    ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method finished") ;
    
    return(output) ;
}

/**
 * Set to a Group object a nested group.
 * @param[in] grp The nested Group object
 * @return 1 if ok, 0 if it was not possible to insert the Group.
 */
int Netcdf4xmlHandler::Group::set_nestedgroup(const Netcdf4xmlHandler::Group& grp)
{
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method called") ;
    
    int setOk = 0 ;
    for(size_t i=0; i<nestedgroups_V_.size(); ++i)
    {
        if( nestedgroups_V_[i].get_name() == grp.get_name() )
        {
            nestedgroups_V_[i] = grp ;
            setOk = 1 ;
            break ;
        }
        
        setOk = nestedgroups_V_[i].set_nestedgroup(grp) ;
        if(setOk)
            break ;
    }

    ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method finished") ;
    
    return(setOk) ;
}

/**
 * Remove the Group object corresponding to a given group name.
 * @param[in] groupName The name of the nested group
 * @return Boolean indicating if group(s) with configured name(s) has(ve) been removed
 */
bool Netcdf4xmlHandler::Group::remove_nestedgroup(const string& groupName)
{
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method called") ;
    
    bool removed = false;
	for(size_t i=0; i<nestedgroups_V_.size(); ++i)
    {
        if( nestedgroups_V_[i].get_name() == groupName )
        {
            nestedgroups_V_.erase(nestedgroups_V_.begin()+i);
            --i;
            removed = true;
            continue;
        }
        
        removed = nestedgroups_V_[i].remove_nestedgroup(groupName);
    }

    ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method finished") ;
    
    return(removed) ;
}

/**
 * Set an Attribute object.
 * @param[in] attr The Attribute object
 */
int Netcdf4xmlHandler::Group::set_attribute(const Netcdf4xmlHandler::Attribute& attr)
{
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method called") ;
    
    int found = 0 ;
	for(size_t i=0; i<attributes_V_.size(); ++i)
    {
        if( attributes_V_[i].get_name() == attr.get_name() )
        {
            attributes_V_[i] = attr;
            found = 1 ;
            break ;
        }
    }
    
    if(!found)
        add_attribute(attr) ;
    
    ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method finished") ;
    
    return(found) ;
}

/**
 * Get an Attribute object given its name.
 * @param[in] attributeName The name of the attribute
 * @return The retrieved Attribute object
 */
Netcdf4xmlHandler::Attribute Netcdf4xmlHandler::Group::get_attribute(const string& attributeName)
{
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method called") ;
    
    Attribute output ;
	for(size_t i=0; i<attributes_V_.size(); ++i)
    {
        if( attributes_V_[i].get_name() == attributeName )
        {
            output = attributes_V_[i] ;
            break ;
        }
    }

    ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method finished") ;
    
    return(output) ;
}

/**
 * Set a Variable object.
 * @param[in] var The Variable object
 */
int Netcdf4xmlHandler::Group::set_variable(const Netcdf4xmlHandler::Variable& var)
{
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method called") ;
    
    int found = 0 ;
	for(size_t i=0; i<variables_V_.size(); ++i)
    {
        const string varName(var.get_name());
        if( variables_V_[i].get_name() == var.get_name() )
        {
            variables_V_[i] = var ;
            found = 1 ;
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Variable " << varName << " updated.");
            break ;
        }
    }
    
    if(!found)
        add_variable(var) ;
    
    ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method finished") ;
    
    return(found) ;
}

/**
 * Remove a dimension object given its name.
 * @param[in] dim The Dimension name
 */
void Netcdf4xmlHandler::Group::rm_dimension(const string& DimName)
{
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method called") ;

	for(size_t i=0; i<dimensions_V_.size(); ++i)
    {
        if( dimensions_V_[i].get_name() == DimName )
        {
            dimensions_V_.erase(dimensions_V_.begin()+i);
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Dimension " << DimName << " found and removed.");
            break;
        }
    }

    ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method finished") ;
    
    return;
}

/**
 * Remove a variable object given its name.
 * @param[in] VarName The Variable name
 */
void Netcdf4xmlHandler::Group::rm_variable(const string& VarName)
{
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method called") ;

	for(size_t i=0; i<variables_V_.size(); ++i)
    {
        if( variables_V_[i].get_name() == VarName )
        {
            variables_V_.erase(variables_V_.begin()+i);
            ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Variable " << VarName << " found and removed.");
            break;
        }
    }

    ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method finished") ;
    
    return;
}

/**
 * Get a Variable object given its name.
 * @param[in] variableName The name of the attribute
 * @return The retrieved Variable object
 */
Netcdf4xmlHandler::Variable Netcdf4xmlHandler::Group::get_variable(const string& variableName)
{
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method called") ;
    
    Variable output ;
	for(size_t i=0; i<variables_V_.size(); ++i)
    {
        if( variables_V_[i].get_name() == variableName )
        {
            output = variables_V_[i] ;
            break ;
        }
    }

    ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method finished") ;
    
    return(output) ;
}

/**
 * Set a Dimension object.
 * @param[in] dim The Dimension object
 */
int Netcdf4xmlHandler::Group::set_dimension(const Netcdf4xmlHandler::Dimension& dim)
{
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method called") ;
    
    int found = 0 ;
	for(size_t i=0; i<dimensions_V_.size(); ++i)
    {
        if( dimensions_V_[i].get_name() == dim.get_name() )
        {
            dimensions_V_[i] = dim;
            found = 1 ;
            break ;
        }
    }
    
    if(!found)
        add_dimension(dim) ;
    
    ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method finished") ;
    
    return(found) ;
}

/**
 * Get a Dimension object given its name.
 * @param[in] dimensionName The name of the dimension
 * @return The retrieved Dimension object
 */
Netcdf4xmlHandler::Dimension Netcdf4xmlHandler::Group::get_dimension(const string& dimensionName)
{
	ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method called") ;
    
    Dimension output ;
	for(size_t i=0; i<dimensions_V_.size(); ++i)
    {
        if( dimensions_V_[i].get_name() == dimensionName )
        {
            output = dimensions_V_[i] ;
            break ;
        }
    }

    ACS_CLASS_WRITE_DEBUG(ACS_HIG_VERB, "Method finished") ;
    
    return(output) ;
}


_ACS_END_NAMESPACE

