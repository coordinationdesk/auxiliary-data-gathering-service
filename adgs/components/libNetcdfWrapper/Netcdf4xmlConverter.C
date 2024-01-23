/*
	Copyright 2018-2021, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libNetcdf4xmlConverter$

	$Id$

	$Author$

	$Log$
	Release 1.24.2  2019/05/23 17:30:10  enrico.carta@acsys.it
	EC:: Compiling warning removed
	
	Revision 1.1  2018/07/13 09:02:26  enrcar
	EC:: Modules added: Netcdf4xmlConverter, Netcdf4xmlHandler
	

        
*/ 

#include <Netcdf4xmlConverter.h>
#include <Netcdf4xmlHandler.h>
#include <XMLOstream.h>
#include <XMLIstream.h>
#include <Filterables.h>
#include <rsResourceSet.h>
#include <rsPushNSpace.h>
#include <StringUtils.h>
#include <set>
#include <fstream>
#include <sstream>
#include <string>


#ifndef CATC
#define CATC(s) {try{s;} catch (...) {ACS_LOG_ERROR("Failed in " << __FILE__ << ":" << __LINE__ << ". " << # s); throw;}}
#endif

_ACS_BEGIN_NAMESPACE(acs)
using namespace std;

ACS_CLASS_DEFINE_DEBUG_LEVEL(Netcdf4xmlConverter) ;

const vector<netCDF::NcType::ncType> Netcdf4xmlConverter::ncTypes = {netCDF::NcType::nc_BYTE,netCDF::NcType::nc_UBYTE,netCDF::NcType::nc_CHAR,netCDF::NcType::nc_STRING,netCDF::NcType::nc_SHORT,netCDF::NcType::nc_USHORT,netCDF::NcType::nc_INT,netCDF::NcType::nc_UINT,netCDF::NcType::nc_INT64,netCDF::NcType::nc_UINT64,netCDF::NcType::nc_FLOAT,netCDF::NcType::nc_DOUBLE};

/**
 * Get the netCDF::NcType corresponding to a specific data type identifier (those managed by the Netcdf4xmlHandler)
 * @return The netCDF::NcType.
 */
netCDF::NcType::ncType Netcdf4xmlConverter::getNcType(const string& str_type)
{  
    netCDF::NcType::ncType type ; 
    
    for(size_t i=0; i<Netcdf4xmlHandler::types.size(); ++i)
    {
        if(str_type == Netcdf4xmlHandler::types[i])
        {
            return(Netcdf4xmlConverter::ncTypes[i]);
        }
    }
    ACS_THROW(exNetcdf4xmlConverterCriticalException(" Unknown type \"" + str_type + "\"."));     
    return(type);
}

//################################################################################
//		class NetCDF4toXml
//################################################################################


/**
 * Method to convert a netcdf file in an XML handler object.
 * @param[in] netcdf_file_in The netCDF file name
 * @param[out] handler_out The Netcdf4xmlHandler object including the info read from the netCDF  file
 * @param[in] convert_var_values Flag indicating if the variables have been converted in strings
 */
void Netcdf4xmlConverter::NetCDF4toXml::convert(const string& netcdf_file_in, 
												Netcdf4xmlHandler& handler_out, 
												const bool convert_var_values)
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method called. netcdf_file_in: \"" << netcdf_file_in << "\"  convert_var_values: " << (convert_var_values?"T":"F") << endl ;
	ACS_CLASS_END_DEBUG

	if ( (false == File::exists(netcdf_file_in)) || (true == File::isDir(netcdf_file_in)) )
	{
		ACS_THROW(exIOException("Invalid input NetCDF file: \"" + netcdf_file_in + "\"")) ;
	}

	const netCDF::NcFile file_in(netcdf_file_in, netCDF::NcFile::read);
	
	const multimap< string, netCDF::NcGroup >& groups_M ( file_in.getGroups() ) ;

	if (groups_M.size() >= 1)
	{
		// At least one named-group, i.e. non-root groups
	
		for (multimap< string, netCDF::NcGroup >::const_iterator it=groups_M.begin() ; it != groups_M.end() ; it++)
		{
			ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
				excerr << SimpleDebugSignatureThread << "Converting group: \"" << it->first << "\":" << endl ;
			ACS_CLASS_END_DEBUG

			const bool group_is_root(false) ; 
			handler_out.add_group( convert_group(it->second, group_is_root, convert_var_values) ) ; 
		}	// END: for
		
	} else {
	
		// Not even one named-group. So, either the whole file is an unnamed/root group, or the file is empty.
				
		ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
			excerr << SimpleDebugSignatureThread << "Converting file:" << endl ;
		ACS_CLASS_END_DEBUG
		
		const bool group_is_root(true) ; 
		handler_out.add_group( convert_group(file_in, group_is_root, convert_var_values) ) ; 
	}
	

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method finished" << endl ;
	ACS_CLASS_END_DEBUG
}

/**
 * Method to convert a netcdf file in an XML file template.
 * @param[in] netcdf_file_in The input netCDF file name
 * @param[in] netcdf_file_out The output netCDF file name
 * @param[in] schema_name The XML schema file name
 * @param[in] alter_group_tag Boolean indicating if the XML group tag should include the group name (true) or not (false)
 * @param[in] convert_var_values Flag indicating if the variables have been converted in strings
 */
void Netcdf4xmlConverter::NetCDF4toXml::convert(const string& netcdf_file_in, 
												const string& netcdf_file_out, 
												const string& schema_name,  
												const bool& alter_group_tag, 
												const bool convert_var_values)
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method called. netcdf_file_in: \"" << netcdf_file_in << "\" netcdf_file_out: \"" 
			<< netcdf_file_out << "\"  convert_var_values: " << (convert_var_values?"T":"F") << endl ;
	ACS_CLASS_END_DEBUG

	Netcdf4xmlHandler handler_out ;

	convert (netcdf_file_in, handler_out, convert_var_values) ;
	
	handler_out.write(netcdf_file_out, schema_name, alter_group_tag) ; 

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method finished" << endl ;
	ACS_CLASS_END_DEBUG
}

	
/**
 * Method to convert/write a netcdf group in an XML handler group object.  
 */
Netcdf4xmlHandler::Group Netcdf4xmlConverter::NetCDF4toXml::convert_group(const netCDF::NcGroup& group_in, const bool group_is_root, const bool convert_var_values)
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method called. convert_var_values: " << (convert_var_values?"T":"F") << endl ;
	ACS_CLASS_END_DEBUG

	string name ; 
	if (false == group_is_root) { name = group_in.getName() ; } 
	Netcdf4xmlHandler::Group group_out(group_is_root, name) ; 

	{
		const multimap< string,netCDF::NcDim >&	dims_M ( group_in.getDims() ) ;
		for (multimap< string, netCDF::NcDim >::const_iterator it=dims_M.begin() ; it != dims_M.end() ; it++)
		{
			ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
				excerr << SimpleDebugSignatureThread << "Converting dimension: \"" << it->first << "\"" << endl ;
			ACS_CLASS_END_DEBUG

			group_out.add_dimension( convert_dimension(it->second) ) ;  
		}
	}
		 
	{	 
 		const multimap< string, netCDF::NcGroupAtt >& atts_M ( group_in.getAtts () );
		for (multimap< string, netCDF::NcGroupAtt >::const_iterator it=atts_M.begin() ; it != atts_M.end() ; it++)
		{
			ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
				excerr << SimpleDebugSignatureThread << "Converting attribute: \"" << it->first << "\"" << endl ;
			ACS_CLASS_END_DEBUG

			group_out.add_attribute( convert_attribute(it->second) ) ;
		}
	}
	
	{
		multimap< string, netCDF::NcVar > vars_M;
        CATC(vars_M = group_in.getVars ());
		for (multimap< string, netCDF::NcVar >::const_iterator it=vars_M.begin() ; it != vars_M.end() ; it++)
		{
			ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
				excerr << SimpleDebugSignatureThread << "Converting variable: \"" << it->first << "\"" << endl ;
			ACS_CLASS_END_DEBUG

			group_out.add_variable( convert_variable(it->second, convert_var_values) ) ;
		}
	}
	
    // Check and add nested groups
    const multimap< string, netCDF::NcGroup >& nestedgroups_M ( group_in.getGroups() ) ;
	if (nestedgroups_M.size() >= 1)
	{
		// At least one named-group, i.e. non-root groups
	
		for (multimap< string, netCDF::NcGroup >::const_iterator it=nestedgroups_M.begin() ; it != nestedgroups_M.end() ; it++)
		{
			ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
				excerr << SimpleDebugSignatureThread << "Converting nested group: \"" << it->first << "\":" << endl ;
			ACS_CLASS_END_DEBUG

			group_out.add_group( convert_group(it->second, false, convert_var_values) ) ; 
		}
    }
	
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method finished" << endl ;
	ACS_CLASS_END_DEBUG

	return group_out ;
}

/**
 * Method to convert/write a netcdf dimension in an XML handler dimension object.  
 */
Netcdf4xmlHandler::Dimension Netcdf4xmlConverter::NetCDF4toXml::convert_dimension(const netCDF::NcDim& dim_in)
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method called." << endl ;
	ACS_CLASS_END_DEBUG

	const string name (dim_in.getName()) ;
	const size_t sz(dim_in.getSize()) ;
	string length ; { ostringstream o ; o << sz ; length = o.str() ; }

	Netcdf4xmlHandler::Dimension d(name, length) ; 
		
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method finished" << endl ;
	ACS_CLASS_END_DEBUG
	
	return d ; 
}

/**
 * Method to convert/write a netcdf attribute in an XML handler attribute object.  
 */
Netcdf4xmlHandler::Attribute Netcdf4xmlConverter::NetCDF4toXml::convert_attribute(const netCDF::NcAtt& attr_in)
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method called." << endl ;
	ACS_CLASS_END_DEBUG

	const string name (attr_in.getName()) ;
	string s ; 

	const netCDF::NcType::ncType type ( (attr_in.getType()).getTypeClass() ) ;	
	const string t ( (attr_in.getType()).getName() ) ; 
	const size_t elems (attr_in.getAttLength()) ; // Number of elements

	switch(type)
	{
		case netCDF::NcType::nc_BYTE:   { char v[elems] ; attr_in.getValues(v) ; s = get_string (v, elems, " ") ; break ; }
		case netCDF::NcType::nc_UBYTE:  { unsigned char v[elems] ; attr_in.getValues(v) ; s = get_string (v, elems, " ") ; break ; }
		case netCDF::NcType::nc_SHORT:	{ short v[elems] ; attr_in.getValues(v) ; s = get_string (v, elems, " ") ; break ; } 
		case netCDF::NcType::nc_USHORT:	{ unsigned short v[elems] ; attr_in.getValues(v) ; s = get_string (v, elems, " ") ; break ; }
		case netCDF::NcType::nc_INT:	{ int32_t v[elems] ; attr_in.getValues(v) ; s = get_string (v, elems, " ") ; break ; } 
		case netCDF::NcType::nc_UINT: 	{ uint32_t v[elems] ; attr_in.getValues(v) ; s = get_string (v, elems, " ") ; break ; } 
		case netCDF::NcType::nc_INT64:	{ int64_t v[elems] ; attr_in.getValues(v) ; s = get_string (v, elems, " ") ; break ; } 
		case netCDF::NcType::nc_UINT64:	{ uint64_t v[elems] ; attr_in.getValues(v) ; s = get_string (v, elems, " ") ; break ; } 
		case netCDF::NcType::nc_FLOAT:	{ float v[elems] ; attr_in.getValues(v) ; s = get_string (v, elems, " ") ; break ; } 
		case netCDF::NcType::nc_DOUBLE:	{ double v[elems] ; attr_in.getValues(v) ; s = get_string (v, elems, " ") ; break ; }
        case netCDF::NcType::nc_STRING: { char* v=0; attr_in.getValues(&v) ; s = get_string(v) ; break; }
		case netCDF::NcType::nc_CHAR:	{ char v[elems+1] ; memset(v, 0, (elems+1)) ; attr_in.getValues(&v) ; s = get_string (v, elems) ; break ; } 
		default: { throwException(exCriticalException("Invalid type (\"" + t + "\") for attribute: \"" + name + "\"" )) };
	}	// END switch


	Netcdf4xmlHandler::Attribute a(name, s, t) ; 

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method finished" << endl ;
	ACS_CLASS_END_DEBUG

	return a ; 
}

/**
 * Method to convert/write a netcdf variable in an XML handler variable object.
 */
Netcdf4xmlHandler::Variable Netcdf4xmlConverter::NetCDF4toXml::convert_variable(const netCDF::NcVar& var_in, const bool convert_var_values)
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method called. convert_var_values: " << (convert_var_values?"T":"F") << endl ;
	ACS_CLASS_END_DEBUG

	const string name (var_in.getName()) ;
	const netCDF::NcType::ncType type ( (var_in.getType()).getTypeClass() ) ;	
	const string t ( (var_in.getType()).getName() ) ; 
	string shape ; vector <size_t> elems_V ; 
	{
		vector< netCDF::NcDim > dims_V (var_in.getDims()) ;
		for (size_t i=0; i<dims_V.size(); i++)
		{ 
			shape += dims_V.at(i).getName() + ( ((i+1)!=dims_V.size())?" ":"" ) ; 
			elems_V.push_back( dims_V.at(i).getSize() ) ; 
		}	
	}
	
	// Create the new variable
	Netcdf4xmlHandler::Variable var(name, t, shape) ;
	
	
	// Add the attibutes of the variable 
	{	
		map< string, netCDF::NcVarAtt > atts_M;
        CATC( atts_M = var_in.getAtts() );

		for (map< string, netCDF::NcVarAtt >::const_iterator it=atts_M.begin() ; it!=atts_M.end(); it++)
		{
			var.add_attribute ( convert_attribute(it->second) ) ; 
		}
	}	
	
	
	// Add the values of the variable 
	if ( (true == convert_var_values) && (elems_V.size() > 0) )
	{
		const size_t dims (elems_V.size()) ; 
		vector <size_t> index_V	(dims, 0) ;

		ssize_t to_increment (dims-1) ; 

		ostringstream s ; 
		while (to_increment != -1)
		{
			switch(type)
			{
				case netCDF::NcType::nc_BYTE:   { char v ; var_in.getVar(index_V, &v) ; s << v ; break ; }
				case netCDF::NcType::nc_UBYTE:  { unsigned char v ; var_in.getVar(index_V, &v) ; s << v ; break ; }
				case netCDF::NcType::nc_SHORT:	{ short v ; var_in.getVar(index_V, &v) ; s << v ; break ; } 
				case netCDF::NcType::nc_USHORT:	{ unsigned short v ; var_in.getVar(index_V, &v) ; s << v ; break ; }
				case netCDF::NcType::nc_INT:	{ int32_t v ; var_in.getVar(index_V, &v) ; s << v ; break ; } 
				case netCDF::NcType::nc_UINT: 	{ uint32_t v ; var_in.getVar(index_V, &v) ; s << v ; break ; } 
				case netCDF::NcType::nc_INT64:	{ int64_t v ; var_in.getVar(index_V, &v) ; s << v ; break ; } 
				case netCDF::NcType::nc_UINT64:	{ uint64_t v ; var_in.getVar(index_V, &v) ; s << v ; break ; } 
				case netCDF::NcType::nc_FLOAT:	{ float v ; var_in.getVar(index_V, &v) ; s << v ; break ; } 
				case netCDF::NcType::nc_DOUBLE:	{ double v ; var_in.getVar(index_V, &v) ; s << v ; break ; } 
				case netCDF::NcType::nc_CHAR:	{ char v ; var_in.getVar(index_V, &v) ; s << v ; break ; } 
				case netCDF::NcType::nc_STRING:	{ char *p=NULL ; var_in.getVar(index_V, &p) ; if (p != NULL) { s << string(p); } ; break ; } 
				default: { throwException(exCriticalException("Invalid type for variable: \"" + name + "\"" )) };
			}	// END switch


			for (ssize_t jj=dims; jj>=1; jj--)
			{ 
				const ssize_t j ( (jj>0)?(jj-1):0) ; 
				if ( j == to_increment )
				{	
					index_V[j] ++ ; 

					if (index_V[j] >= elems_V[j])
					{
						index_V[j]=0 ; to_increment = (j-1) ;
					} else {
						to_increment = (dims-1) ; 
					}

				}
			}

			if (to_increment != -1) { s << " " ; }

		}	; // END: while (to_increment != -1)
	
		var.set_values( s.str() ) ; 
	
	} // end: if ( (true == convert_var_values) && (elems_V.size() > 0) )


	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method finished" << endl ;
	ACS_CLASS_END_DEBUG

	return var ; 
}


//################################################################################
//		class XmlToNetCDF4
//################################################################################

const std::string Netcdf4xmlConverter::XmlToNetCDF4::ascii_separator_default_=" ";

/**
 * Method to convert an XML handler object in a netcdf file.
 * @param[in] handler_in The Netcdf4xmlHandler object
 * @param[out] netcdf_file_out The output netCDF file name
 * @param[in] convert_var_values Flag indicating if the variables have been converted in strings
 */
void Netcdf4xmlConverter::XmlToNetCDF4::convert(
    const Netcdf4xmlHandler& handler_in, 
    const std::string& netcdf_file_out, 
    const bool convert_var_values)
{      
    ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method called. netcdf_file_out: \"" << netcdf_file_out << "\"  convert_var_values: " << (convert_var_values?"T":"F") << endl ;
	ACS_CLASS_END_DEBUG
	
	netCDF::NcFile file_out(netcdf_file_out, netCDF::NcFile::replace);	/* replace means that the file is created empty, even if existing */

	
	const vector <Netcdf4xmlHandler::Group> groups_V ( handler_in.get_groups() ) ;
	
	for (size_t i=0; i<groups_V.size(); i++)
	{
		convert_group( file_out, groups_V.at(i), convert_var_values) ; 
	}

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method finished" << endl ;
	ACS_CLASS_END_DEBUG
}

/**
 * Method to convert an XML handler object in a netcdf file.
 * @param[in] xml_file_in The XML template file name
 * @param[out] netcdf_file_out The output netCDF file name
 * @param[in] convert_var_values Flag indicating if the variables have been converted in strings
 * @param[in] convert_var_values Flag indicating if the variables have been converted in strings
 */
void Netcdf4xmlConverter::XmlToNetCDF4::convert(
    const std::string& xml_file_in,
    const std::string& netcdf_file_out,
    const bool convert_var_values)
{
	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method called. xml_file_in: \"" << xml_file_in << "\" netcdf_file_out: \"" 
			<< netcdf_file_out << "\"  convert_var_values: " << (convert_var_values?"T":"F") << endl ;
	ACS_CLASS_END_DEBUG


	if ( (false == File::exists(xml_file_in)) || (true == File::isDir(xml_file_in)) )
	{
		ACS_THROW(exIOException("Invalid input XML file: \"" + xml_file_in + "\"")) ;
	}


	Netcdf4xmlHandler handler_in (xml_file_in) ;

	convert (handler_in, netcdf_file_out, convert_var_values) ;

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB) 
		excerr << SimpleDebugSignatureThread << "Method finished" << endl ;
	ACS_CLASS_END_DEBUG
}

/**
 * Method to convert/write an XML handler group object in a netcdf group. 
 */
void Netcdf4xmlConverter::XmlToNetCDF4::convert_group(
    netCDF::NcFile& file_out,
    const Netcdf4xmlHandler::Group& group,
    const bool convert_var_values)
{
    const bool group_is_root( group.get_group_is_root() ) ; 
	
	if (true == group_is_root)
	{
        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Converting root group.");
        {
			// Copy Attributes:
			const vector <Netcdf4xmlHandler::Attribute>	attributes_V ( group.get_attributes() ) ;
			for (size_t i=0; i<attributes_V.size(); i++) 
            { 
                ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Copying Attribute \""<< attributes_V[i].get_name() <<"\".");
                convert_attribute(file_out, attributes_V.at(i)) ; 
            }
		}	// Copy Attributes: END SCOPE

		{
			// Copy Dimensions :
			const vector <Netcdf4xmlHandler::Dimension>	dimensions_V ( group.get_dimensions() ) ;
			for (size_t i=0; i<dimensions_V.size(); i++) 
            { 
                ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Copying Dimension \""<< dimensions_V[i].get_name() <<"\".");
                convert_dimension(file_out, dimensions_V.at(i)) ; 
            }
		}	// Copy Dimensions : END SCOPE
		
		{
			// Copy Variables (assuming that the dimensions were already copied above)
			const vector <Netcdf4xmlHandler::Variable>	variables_V ( group.get_variables() ) ;
			for (size_t i=0; i<variables_V.size(); i++) 
            { 
                ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Copying Variable \""<< variables_V[i].get_name() <<"\".");
                convert_variable(file_out, variables_V.at(i), convert_var_values) ; 
            }
		}	// Copy Variables : END SCOPE
	}
	else
	{
		const string name ( group.get_name() ) ;
		netCDF::NcGroup group_out;
        CATC( group_out = file_out.addGroup(name) ) ; 
		
        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Converting group \""<< group_out.getName() << "\".");
		{
			// Copy Attributes:
			const vector <Netcdf4xmlHandler::Attribute>	attributes_V ( group.get_attributes() ) ;
			for (size_t i=0; i<attributes_V.size(); i++) 
            { 
                ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Copying Attribute \""<< attributes_V[i].get_name() <<"\".");
                convert_attribute(group_out, attributes_V.at(i)) ; 
            }
		}	// Copy Attributes: END SCOPE

		{
			// Copy Dimensions :
			const vector <Netcdf4xmlHandler::Dimension>	dimensions_V ( group.get_dimensions() ) ;
			for (size_t i=0; i<dimensions_V.size(); i++) 
            { 
                ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Copying Dimension \""<< dimensions_V[i].get_name() <<"\".");
                convert_dimension(group_out, dimensions_V.at(i)) ; 
            }
		}	// Copy Dimensions : END SCOPE

		{
			// Copy Variables (assuming that the dimensions were already copied above)
			const vector <Netcdf4xmlHandler::Variable>	variables_V ( group.get_variables() ) ;
			for (size_t i=0; i<variables_V.size(); i++) 
            { 
                ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Copying Variable \""<< variables_V[i].get_name() <<"\".");
                convert_variable(group_out, variables_V.at(i), convert_var_values) ; 
            }
		}	// Copy Variables : END SCOPE
        
        // Check and convert for nested groups
        const vector <Netcdf4xmlHandler::Group> nestedgroups_V ( group.get_nestedgroups() ) ;   
	    for (size_t i=0; i<nestedgroups_V.size(); i++)
	    {
		    convert_nestedgroup( group_out, nestedgroups_V.at(i), convert_var_values ) ; 
	    }        
	}
}

/**
 * Method to convert/write XML handler nested group objects in the correspondent netcdf groups. 
 */
void Netcdf4xmlConverter::XmlToNetCDF4::convert_nestedgroup(
    netCDF::NcGroup& rgroup_out,
    const Netcdf4xmlHandler::Group& group,
    const bool convert_var_values)
{
    const bool group_is_root( group.get_group_is_root() ) ; 
	
	if (true == group_is_root)
    {
        ACS_THROW(exNetcdf4xmlConverterException("Group can not be root. Something wrong.")) ;   
    }
	else
	{
        const string name ( group.get_name() ) ;
        ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
            excerr << SimpleDebugSignatureThread << "DBG: Nested Group <" << name << ">" << std::endl ;        
        ACS_CLASS_END_DEBUG
        
		netCDF::NcGroup group_out;
        CATC( group_out = rgroup_out.addGroup(name) ) ; 
        ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Converting group \""<< group_out.getName() << "\".");     
		{
			// Copy Attributes:
			const vector <Netcdf4xmlHandler::Attribute>	attributes_V ( group.get_attributes() ) ;
			for (size_t i=0; i<attributes_V.size(); i++) 
            { 
                ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Copying Attribute \""<< attributes_V[i].get_name() <<"\".");
                convert_attribute(group_out, attributes_V.at(i)) ; 
            }
		}	// Copy Attributes: END SCOPE

		{
			// Copy Dimensions :
			const vector <Netcdf4xmlHandler::Dimension>	dimensions_V ( group.get_dimensions() ) ;
			for (size_t i=0; i<dimensions_V.size(); i++) 
            { 
                ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Copying Dimension \""<< dimensions_V[i].get_name() <<"\".");
                convert_dimension(group_out, dimensions_V.at(i)) ; 
            }
		}	// Copy Dimensions : END SCOPE
		
		{
			// Copy Variables (assuming that the dimensions were already copied above)
			const vector <Netcdf4xmlHandler::Variable>	variables_V ( group.get_variables() ) ;
			for (size_t i=0; i<variables_V.size(); i++) 
            { 
                ACS_CLASS_WRITE_DEBUG(ACS_MID_VERB, "Copying Variable \""<< variables_V[i].get_name() <<"\".");
                convert_variable(group_out, variables_V.at(i), convert_var_values) ; 
            }
		}	// Copy Variables : END SCOPE
        
        // Check and convert for nested groups
        const vector <Netcdf4xmlHandler::Group> nestedgroups_V ( group.get_nestedgroups() ) ;		
		
        for (size_t i=0; i<nestedgroups_V.size(); i++)
	    {
		    convert_nestedgroup( group_out, nestedgroups_V.at(i), convert_var_values ) ; 
	    }
	}
}

/**
 * Method to convert/write an XML handler attribute object in a netcdf group's attribute. 
 */
void Netcdf4xmlConverter::XmlToNetCDF4::convert_attribute (netCDF::NcGroup& group_out, const Netcdf4xmlHandler::Attribute& attribute)
{
	const string name (attribute.get_name()) ;
	const string value (attribute.get_value()) ;
	const string str_type (attribute.get_type()) ;

	if ( (false == name.empty()) || (false == value.empty()) ) 
	{
		netCDF::NcType::ncType type = Netcdf4xmlConverter::getNcType(str_type);

		vector <string> str_value_V ; 
		size_t elems(0) ;
		
		if ( (type != netCDF::NcType::nc_CHAR) && (false == value.empty()) )
		{
   			StringUtils::Tokenize(value, str_value_V, " ") ; 
			elems = str_value_V.size() ;
		}
		  
  		     if (type == netCDF::NcType::nc_BYTE) 	{ char v[elems] ; get_vector(v, elems, str_value_V) ; CATC(group_out.putAtt(name, type, elems, v)) ; }
		else if (type == netCDF::NcType::nc_UBYTE)	{ unsigned char v[elems] ; get_vector(v, elems, str_value_V) ; CATC(group_out.putAtt(name, type, elems, v)) ; }
		else if (type == netCDF::NcType::nc_CHAR)	{ CATC(group_out.putAtt(name, value)) ; }
		else if (type == netCDF::NcType::nc_SHORT)	{ int16_t v[elems] ; get_vector(v, elems, str_value_V) ; CATC(group_out.putAtt(name, type, elems, v)) ; }
		else if (type == netCDF::NcType::nc_USHORT)	{ uint16_t v[elems] ; get_vector(v, elems, str_value_V) ; CATC(group_out.putAtt(name, type, elems, v)) ; }
		else if (type == netCDF::NcType::nc_INT)	{ int32_t v[elems] ; get_vector(v, elems, str_value_V) ; CATC(group_out.putAtt(name, type, elems, v)) ; }
		else if (type == netCDF::NcType::nc_UINT)	{ uint32_t v[elems] ; get_vector(v, elems, str_value_V) ; CATC(group_out.putAtt(name, type, elems, v)) ; }
		else if (type == netCDF::NcType::nc_INT64)	{ int64_t v[elems] ; get_vector(v, elems, str_value_V) ; CATC(group_out.putAtt(name, type, elems, v)) ; }
		else if (type == netCDF::NcType::nc_UINT64)	{ uint64_t v[elems] ; get_vector(v, elems, str_value_V) ; CATC(group_out.putAtt(name, type, elems, v)) ; }
		else if (type == netCDF::NcType::nc_FLOAT)	{ float v[elems] ; get_vector(v, elems, str_value_V) ; CATC(group_out.putAtt(name, type, elems, v)) ; }
		else if (type == netCDF::NcType::nc_DOUBLE)	{ double v[elems] ; get_vector(v, elems, str_value_V) ; CATC(group_out.putAtt(name, type, elems, v)) ; }
		else { ACS_THROW(exCriticalException("Invalid type: " + str_type)) ; }
	}
}

/**
 * Method to convert/write an XML handler attribute object in a netcdf variable's attribute. 
 */
void Netcdf4xmlConverter::XmlToNetCDF4::convert_var_attribute (netCDF::NcVar& group_out, const Netcdf4xmlHandler::Attribute& attribute)
{
	const string name (attribute.get_name()) ;
	const string value (attribute.get_value()) ;
	const string str_type (attribute.get_type()) ;

	if ( (false == name.empty()) || (false == value.empty()) ) 
	{
		netCDF::NcType::ncType type = Netcdf4xmlConverter::getNcType(str_type);
        
		vector <string> str_value_V ; 
		size_t elems(0) ;
		
		if ( (type != netCDF::NcType::nc_CHAR) && (false == value.empty()) )
		{
   			StringUtils::Tokenize(value, str_value_V, " ") ; 
			elems = str_value_V.size() ;
		}
		  
  		     if (type == netCDF::NcType::nc_BYTE) 	{ char v[elems] ; get_vector(v, elems, str_value_V) ; CATC(group_out.putAtt(name, type, elems, v)) ; }
		else if (type == netCDF::NcType::nc_UBYTE)	{ unsigned char v[elems] ; get_vector(v, elems, str_value_V) ; CATC(group_out.putAtt(name, type, elems, v)) ; }
		else if (type == netCDF::NcType::nc_CHAR)	{ CATC(group_out.putAtt(name, value)) ; }
		else if (type == netCDF::NcType::nc_SHORT)	{ int16_t v[elems] ; get_vector(v, elems, str_value_V) ; CATC(group_out.putAtt(name, type, elems, v)) ; }
		else if (type == netCDF::NcType::nc_USHORT)	{ uint16_t v[elems] ; get_vector(v, elems, str_value_V) ; CATC(group_out.putAtt(name, type, elems, v)) ; }
		else if (type == netCDF::NcType::nc_INT)	{ int32_t v[elems] ; get_vector(v, elems, str_value_V) ; CATC(group_out.putAtt(name, type, elems, v)) ; }
		else if (type == netCDF::NcType::nc_UINT)	{ uint32_t v[elems] ; get_vector(v, elems, str_value_V) ; CATC(group_out.putAtt(name, type, elems, v)) ; }
		else if (type == netCDF::NcType::nc_INT64)	{ int64_t v[elems] ; get_vector(v, elems, str_value_V) ; CATC(group_out.putAtt(name, type, elems, v)) ; }
		else if (type == netCDF::NcType::nc_UINT64)	{ uint64_t v[elems] ; get_vector(v, elems, str_value_V) ; CATC(group_out.putAtt(name, type, elems, v)) ; }
		else if (type == netCDF::NcType::nc_FLOAT)	{ float v[elems] ; get_vector(v, elems, str_value_V) ; CATC(group_out.putAtt(name, type, elems, v)) ; }
		else if (type == netCDF::NcType::nc_DOUBLE)	{ double v[elems] ; get_vector(v, elems, str_value_V) ; CATC(group_out.putAtt(name, type, elems, v)) ; }
		else { ACS_THROW(exCriticalException("Invalid type: " + str_type)) ; }
	}
}

/**
 * Method to convert/write an XML handler dimension object in a netcdf group's dimension.
 */
void Netcdf4xmlConverter::XmlToNetCDF4::convert_dimension (netCDF::NcGroup& group_out, const Netcdf4xmlHandler::Dimension& dimension)
{
	const string name (dimension.get_name()) ;
    const string lenS = dimension.get_length();
    ACS_CLASS_WRITE_DEBUG(ACS_VLO_VERB, "name=" << name << " length=" << lenS);
    if(lenS == "unlimited")
    {
        CATC(group_out.addDim(name));
    }
    else
    {
	    size_t length ; stringstream(lenS) >> length ;
        CATC(group_out.addDim(name, length));
    } 	
}

/**
 * Method to convert/write an XML handler variable object in a netcdf group's variable. 
 */
void Netcdf4xmlConverter::XmlToNetCDF4::convert_variable (
    netCDF::NcGroup& group_out, 
    const Netcdf4xmlHandler::Variable& variable, 
    const bool convert_var_values)
{
    const string fileFormat = variable.get_fileformat();
    string separator(variable.get_ascii_separator());
    if(separator.empty())
    {
        // Set the separator (empty space for txt, comma for csv)
        separator = ascii_separator_default_;
    }
    
    const string name (variable.get_name()) ;
	const string str_type (variable.get_type()) ;
	const string shape (variable.get_shape()) ;
	const string value ( ((true==convert_var_values)&&(true==variable.get_values_is_set()))? (variable.get_values()) : "") ;

	multimap< string, netCDF::NcDim > dims_M;
    CATC( dims_M = group_out.getDims(netCDF::NcGroup::ParentsAndCurrent)) ;  

	netCDF::NcType::ncType type = Netcdf4xmlConverter::getNcType(str_type); 
    size_t typeSize = variable.getTypeSize() ;
    
	vector <string> shape_V ; StringUtils::Tokenize(shape, shape_V, " ") ; 

	vector <netCDF::NcDim> dims_to_store_V ;
	vector <size_t> elems_V ;
	
	for (size_t i=0;i<shape_V.size(); i++) 
	{
		if ( dims_M.find(shape_V.at(i)) == dims_M.end() )
		{
			ACS_THROW(exCriticalException("Invalid dimension: " + shape_V.at(i) + " for variable: " + (group_out.getName()+"/"+name) )) ; 
		}
	
		const string cur_dim (shape_V.at(i)) ;
		dims_to_store_V.push_back ( dims_M.find(cur_dim)->second ) ;
		elems_V.push_back ( (dims_M.find(cur_dim)->second).getSize() )   ;
	}
	
	// Create and add the variable to the group:
	if (false == dims_to_store_V.empty()) 
	{ 
		netCDF::NcVar out_var;
        CATC(out_var = group_out.addVar (name, type, dims_to_store_V)) ; 
	
		const vector<Netcdf4xmlHandler::Attribute> attributes_V (variable.get_attributes()) ; 
		for (size_t i=0; i<attributes_V.size(); i++) 
        { 
            if( attributes_V.at(i).get_name() == "_FillValue" )
            {
                // Enable and set the fill value
                const string xmk_value_s(attributes_V.at(i).get_value()) ; 
		        stringstream xml_value ( xmk_value_s ) ; 

                switch(type)
		        {
			        case netCDF::NcType::nc_BYTE:   { char v ; xml_value >> v ; CATC(out_var.setFill(true, v)) ; break ; }
			        case netCDF::NcType::nc_UBYTE:  { unsigned short v ; xml_value >> v ; v &= 0x00ff ; CATC(out_var.setFill(true, v)) ; break ; }
			        case netCDF::NcType::nc_SHORT:	{ short v ; xml_value >> v ; CATC(out_var.setFill(true, v)) ; break ; } 
			        case netCDF::NcType::nc_USHORT:	{ unsigned short v ; xml_value >> v ; CATC(out_var.setFill(true, v)) ; break ; }
			        case netCDF::NcType::nc_INT:	{ int32_t v ; xml_value >> v ; CATC(out_var.setFill(true, v)) ; break ; } 
			        case netCDF::NcType::nc_UINT: 	{ uint32_t v ; xml_value >> v ; CATC(out_var.setFill(true, v)) ; break ; } 
			        case netCDF::NcType::nc_INT64:	{ long long int v ; xml_value >> v ; CATC(out_var.setFill(true, v)) ; break ; } 
			        case netCDF::NcType::nc_UINT64:	{ unsigned long long int v ; xml_value >> v ; CATC(out_var.setFill(true, v)) ; break ; } 
			        case netCDF::NcType::nc_FLOAT:	{ float v ; xml_value >> v ; CATC(out_var.setFill(true, v)) ; break ; } 
			        case netCDF::NcType::nc_DOUBLE:	{ double v ; xml_value >> v ; CATC(out_var.setFill(true, v)) ; break ; } 
			        case netCDF::NcType::nc_CHAR:	{ CATC(out_var.setFill(true, xmk_value_s.c_str())) ; break ; } /* Note, just the VERY FIRST character of the string will be saved !*/
			        case netCDF::NcType::nc_STRING:	{ CATC(out_var.setFill(true, xmk_value_s)) ; break ; } 
			        default: { throwException(exCriticalException("Setting fillVaue found invalid type for variable: \"" + name + "\"" )) };
		        }	// END switch
            }
            
            // Convert the variable attributes (if any) 
            convert_var_attribute(out_var,	attributes_V.at(i)) ; 
        }
        
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Converting Variable \""<< variable.get_name() 
                                         << "\" convert_var_values=" << convert_var_values 
                                         << " get_values_is_set=" << variable.get_values_is_set()
                                         << " get_dataValuesSize=" << variable.get_dataValuesSize().size()
                                         << " get_filename=" << variable.get_filename());
        
		// Convert the variable values
		if ( (true==convert_var_values)&&(true==variable.get_values_is_set()) )
		{
			vector <string> xml_values_V ; StringUtils::Tokenize(value, xml_values_V, " ") ;
            putStringsInVariable(name, type, xml_values_V, elems_V, out_var) ;

		}	// END: if ( (true==convert_var_values)&&(true==variable.get_values_is_set()) )
        else if( !variable.get_dataValuesSize().empty() )
        {
            void* data = 0;
            vector<int> dataSize;
            variable.get_dataValues_pointer(dataSize, &data);
            CATC(out_var.putVar(data));
        }
        else if( !variable.get_filename().empty() )
        {
            const string filename(variable.get_filename());
            
            ACS_COND_THROW(!File::exists(filename), exCriticalException("Failed. File \""+filename+"\" does not exist (variable: \"" + name + "\")" ));
            
            if( fileFormat=="binary" )
            {
                size_t elems = 1 ;
                for(size_t i=0; i<dims_to_store_V.size(); ++i)
                    elems *= dims_to_store_V[i].getSize();
                const size_t varSize = elems*typeSize;

                size_t fileSize = 0;
                {
                    std::ifstream in(filename.c_str(), std::ifstream::ate | std::ifstream::binary);
                    fileSize = in.tellg();
                }

                ACS_COND_THROW((varSize!=fileSize), exCriticalException("Invalid file size ( filename \""+filename+"\" )for variable: \"" + name + "\"" ));

                char* buffer = new char [fileSize];
                ifstream inpFile(filename.c_str(), ios::in | ios::binary);
                ACS_COND_THROW(!inpFile.is_open(), exCriticalException("Failed. Can not open file \""+filename+"\" for variable: \"" + name + "\"" ));
                
                inpFile.read(buffer, fileSize);
                CATC(out_var.putVar(reinterpret_cast<void*>(buffer)));
                delete[] buffer;
                inpFile.close();
                
                ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Converting Variable \""<< variable.get_name() <<"\" Read binary file \"" << filename << "\".");
            }
            else if( fileFormat=="ascii" || fileFormat=="csv")
            {
                string line;
                ifstream inpFile(filename.c_str());
                ACS_COND_THROW(!inpFile.is_open(), exCriticalException("Failed. Can not open file \""+filename+"\" for variable: \"" + name + "\"" )) ;

                /// Compute the size of the input ascii file in order to pre-allocate memory
                inpFile.seekg (0, inpFile.end);
                int length = inpFile.tellg();
                inpFile.seekg (0, inpFile.beg);
                
                vector <string> ascii_values_V ;
                ascii_values_V.reserve(length);
                
                const vector<int> ascii_cols = variable.get_ascii_cols();
                if(ascii_cols.empty())
                {
                    while( getline(inpFile,line) )
                    {
                        vector<string> tmpV ;
                        StringUtils::Tokenize(line, tmpV, separator) ;
                        //ascii_values_V.reserve( ascii_values_V.size() + tmpV.size() ); // preallocate memory
                        ascii_values_V.insert( ascii_values_V.end(), tmpV.begin(), tmpV.end() );
                    }
                }
                else
                {
                    while( getline(inpFile,line) )
                    {
                        vector<string> tmpV ;
                        StringUtils::Tokenize(line, tmpV, separator) ;
                        for(size_t i=0; i<ascii_cols.size(); ++i)
                        {
                            ascii_values_V.push_back(tmpV[ascii_cols[i]]);
                        }
                    }
                }
                inpFile.close();
                
                /// Put the ascii data in the nc variable 
                putStringsInVariable(name, type, ascii_values_V, elems_V, out_var) ;
            }
        }
        
	}	// END: if (false == dims_to_store_V.empty()) 

}

/**
 * Method to convert/write a matrix of values represented as vector of strings in a netcdf variable. 
 */
void Netcdf4xmlConverter::XmlToNetCDF4::putStringsInVariable(
    const string& name,                 /*!< IN: the name of the variable to be updated */
    const netCDF::NcType::ncType& type, /*!< IN: variable type as netCDF::NcType::ncType */
    const vector<string>& xml_values_V, /*!< IN: the values to put in the variable */
    vector<size_t> elems_V,             /*!< IN: vector of size, the last shall be the fastest */
    netCDF::NcVar& out_var              /*!< OUT: netCDF::NcVar variable */
    )
{
    const size_t dims (elems_V.size()) ; 
    vector <size_t> index_V	(dims, 0) ;

    vector <size_t> len_V(dims);
    len_V[dims-1] = 1;
    for(int k=(dims-2); k>=0; --k)
    {
        len_V[k] = elems_V[k+1]*len_V[k+1];
        ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "len_V[" << k << "]=" << len_V[k]);
    }
    
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "xml_values_V.size()=" << xml_values_V.size());
    ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
    for(size_t k=0; k<dims; ++k)
    {
        ACS_LOG_DEBUG("elems_V[" << k << "]=" << elems_V[k]);
    }
    ACS_CLASS_END_DEBUG
    
    //ACS_COND_THROW(xml_values_V.size()>elems_V[0]*len_V[0],
    //    exCriticalException() << "Invalid sizes: " << xml_values_V.size() << ">"
    //    << elems_V[0] << "*" << len_V[0] << "=" << elems_V[0]*len_V[0]);
    if (xml_values_V.size()>elems_V[0]*len_V[0]){
        ostringstream os;
        os << "Invalid sizes: " << xml_values_V.size() << ">"
            << elems_V[0] << "*" << len_V[0] << "=" << elems_V[0]*len_V[0];
        throwException(exCriticalException(os.str()));
    }
    
    for(size_t i=0; i<xml_values_V.size(); ++i)
    {
        /// Set the fast index
        index_V[dims-1] = i%elems_V[dims-1];
        
        /// Set the other indices
        for(size_t k=0; k<(dims-1); ++k) {
            index_V[k] = (i/len_V[k])%elems_V[k];
        }
        
        ACS_CLASS_BGN_DEBUG(ACS_HIG_VERB)
	    for(size_t k=0; k<dims; ++k)
        {
            ACS_LOG_DEBUG("index_V[" << k << "]=" << index_V[k] << " ,,, i=" << i << " len_V[" << k << "]=" << len_V[k]);
        }
        ACS_CLASS_END_DEBUG
        
        /// Convert the string to the configured type
        stringstream xml_value ( xml_values_V[i] ); 
		switch(type)
		{
			case netCDF::NcType::nc_BYTE:   { char v ; xml_value >> v ; CATC(out_var.putVar(index_V, v)); break ; }
			case netCDF::NcType::nc_UBYTE:  { unsigned short v ; xml_value >> v ; v &= 0x00ff ; CATC(out_var.putVar(index_V, v)) ; break ; }
			case netCDF::NcType::nc_SHORT:	{ short v ; xml_value >> v ; CATC(out_var.putVar(index_V, v)) ; break ; } 
			case netCDF::NcType::nc_USHORT:	{ unsigned short v ; xml_value >> v ; CATC(out_var.putVar(index_V, v)) ; break ; }
			case netCDF::NcType::nc_INT:	{ int32_t v ; xml_value >> v ; CATC(out_var.putVar(index_V, v)) ; break ; } 
			case netCDF::NcType::nc_UINT: 	{ uint32_t v ; xml_value >> v ; CATC(out_var.putVar(index_V, v)) ; break ; } 
			case netCDF::NcType::nc_INT64:	{ long long int v ; xml_value >> v ; CATC(out_var.putVar(index_V, v)) ; break ; } 
			case netCDF::NcType::nc_UINT64:	{ unsigned long long int v ; xml_value >> v ; CATC(out_var.putVar(index_V, v)) ; break ; } 
			case netCDF::NcType::nc_FLOAT:	{ float v ; xml_value >> v ; CATC(out_var.putVar(index_V, v)) ; break ; } 
			case netCDF::NcType::nc_DOUBLE:	{ double v ; xml_value >> v ; CATC(out_var.putVar(index_V, v)) ; break ; } 
			case netCDF::NcType::nc_CHAR:	{ CATC(out_var.putVar(index_V, xml_values_V[i].c_str())) ; break ; } /* Note, just the VERY FIRST character of the string will be saved !*/
			case netCDF::NcType::nc_STRING:	{ CATC(out_var.putVar(index_V, xml_values_V[i])) ; break ; } 
			default: { throwException(exCriticalException("Invalid type for variable: \"" + name + "\"" )) };
		}    
    }
}

_ACS_END_NAMESPACE

