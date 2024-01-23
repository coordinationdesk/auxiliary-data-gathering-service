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
	Revision 1.1  2018/07/13 09:02:27  enrcar
	EC:: Modules added: Netcdf4xmlConverter, Netcdf4xmlHandler
	

        
*/

#ifndef _Netcdf4xmlConverter_H_
#define _Netcdf4xmlConverter_H_
#include <Netcdf4xmlHandler.h>
#include <acs_c++config.hpp>
#include <rsResourceSet.h>
#include <XMLOstream.h>
#include <XMLIstream.h>
#include <DateTime.h>
#include <exException.h>
#include <ncFile.h>
#include <ncGroup.h>
#include <ncDim.h>
#include <ncException.h>
#include <ncVar.h>
#include <vector>
#include <string>

_ACS_BEGIN_NAMESPACE(acs)

class XMLOstream ;
class rsResourceSet ;

/**
 * \class Netcdf4xmlConverter
 * \brief Class to handle conversions from/to XML representation to/from netcdf
 */
class Netcdf4xmlConverter  { 

public:
    static const std::vector<netCDF::NcType::ncType> ncTypes;
    
	/** Netcdf4xmlConverter Exceptions */
	exDECLARE_EXCEPTION(exNetcdf4xmlConverterException, exException) ; // Base Netcdf4xmlConverter Exception.
	exDECLARE_EXCEPTION(exNetcdf4xmlConverterCriticalException, exNetcdf4xmlConverterException) ; // Critical Exception.
    
    /**
     * Class to handle conversions from netcdf to XML representation.
     */
	class NetCDF4toXml {
	
		public:
		    /**
             * Method to convert a netcdf file in an XML handler object.
             * @param[in] netcdf_file_in The netCDF file name
             * @param[out] handler_out The Netcdf4xmlHandler object including the info read from the netCDF  file
             * @param[in] convert_var_values Flag indicating if the variables have been converted in strings
             */
			static void convert(const std::string& netcdf_file_in, Netcdf4xmlHandler& handler_out, const bool convert_var_values=false) ;
			
            /**
             * Method to convert a netcdf file in an XML file template.
             * @param[in] netcdf_file_in The input netCDF file name
             * @param[in] netcdf_file_out The output netCDF file name
             * @param[in] schema_name The XML schema file name
             * @param[in] alter_group_tag Boolean indicating if the XML group tag should include the group name (true) or not (false)
             * @param[in] convert_var_values Flag indicating if the variables have been converted in strings
             */
            static void convert(const std::string& netcdf_file_in, const std::string& xml_file_out, const std::string& schema_name, const bool& alter_group_tag, const bool convert_var_values=false) ;
		
		private:
			/**
             * Method to convert/write a netcdf group in an XML handler group object.  
             */
            static Netcdf4xmlHandler::Group convert_group(const netCDF::NcGroup&, const bool is_root, const bool convert_var_values) ;
			
            /**
             * Method to convert/write a netcdf dimension in an XML handler dimension object.  
             */
            static Netcdf4xmlHandler::Dimension	convert_dimension(const netCDF::NcDim& dim_in) ;
			
            /**
             * Method to convert/write a netcdf attribute in an XML handler attribute object.  
             */
            static Netcdf4xmlHandler::Attribute	convert_attribute(const netCDF::NcAtt& attr_in) ;
			
            /**
             * Method to convert/write a netcdf variable in an XML handler variable object.
             */
            static Netcdf4xmlHandler::Variable	convert_variable (const netCDF::NcVar& var_in, const bool convert_var_values) ;

		private:
			/**
             * Default Constructor.
             */
            NetCDF4toXml() ;
			
            /**
             * Destructor.
             */
            ~NetCDF4toXml() ;
            
            /**
             * Copy Constructor.
             */
			NetCDF4toXml(const NetCDF4toXml &) ;
            
            /**
             * Overload operator=.
             */
			NetCDF4toXml &operator=(const NetCDF4toXml &) ;

			template<typename T>
			static std::string get_string (
                const T* v, 
                const size_t& n, 
                const std::string& sp="") 
			{ 
                std::ostringstream o ; 
                for (size_t i=0; i<n; i++) 
                { 
                    const std::string c(((i+1)==n)?"":sp) ; o << v[i] << c ; 
                }
                return o.str() ; 
            }
            
            static std::string get_string(
                const char *v
                )
            {
                const char *cp;
                const char *sp;
                unsigned char uc;
                
                std::ostringstream out ;
                size_t len = strlen(v);
                
                cp = v;
                /* adjust len so trailing nulls don't get printed */
                sp = cp + len - 1;
                while (len != 0 && *sp-- == '\0') {
	                len--;
                }
                for (size_t iel = 0; iel < len; iel++)
                {
	                switch (uc = *cp++ & 0377) {
	                case '\b':
	                    out << "\\b";
	                    break;
	                case '\f':
	                    out << "\\f";
	                    break;
	                case '\n':
	                    printf("\\n");
                        out << "\\n";
	                    break;
	                case '\r':
	                    out << "\\r";
	                    break;
	                case '\t':
	                    out << "\\t";
	                    break;
	                case '\v':
	                    out << "\\v";
	                    break;
	                case '\\':
	                    out << "\\\\";
	                    break;
	                case '\'':
	                    out << "\\\'";
	                    break;
	                case '\"':
	                    out << "\\\"";
	                    break;
	                default:
	                    {
                            std::string s(1, uc);
                            out << s ;
                        }
	                    break;
	                }
                }
                return(out.str());
            }      
	} ;
	
    /**
     * Class to handle conversions from XML representation to netcdf.
     */
	class XmlToNetCDF4 {
	
		public:
		    static const std::string ascii_separator_default_; /**< separator string used in ascii files */
            
            /**
             * Method to convert an XML handler object in a netcdf file.
             * @param[in] handler_in The Netcdf4xmlHandler object
             * @param[out] netcdf_file_out The output netCDF file name
             * @param[in] convert_var_values Flag indicating if the variables have been converted in strings
             */
			static void convert(
                const Netcdf4xmlHandler& handler_in, 
                const std::string& netcdf_file_out,
                const bool convert_var_values=false);
			
            /**
             * Method to convert an XML handler object in a netcdf file.
             * @param[in] xml_file_in The XML template file name
             * @param[out] netcdf_file_out The output netCDF file name
             * @param[in] convert_var_values Flag indicating if the variables have been converted in strings
             */
            static void convert(
                const std::string& xml_file_in, 
                const std::string& netcdf_file_out, 
                const bool convert_var_values=false);
		
		private:
			/**
             * Method to convert/write an XML handler group object in a netcdf group. 
             */
            static void convert_group (
                netCDF::NcFile& file_out, 
                const Netcdf4xmlHandler::Group& group, 
                const bool convert_var_values);
            
            /**
             * Method to convert/write XML handler nested group objects in the correspondent netcdf groups. 
             */
            static void convert_nestedgroup(
                netCDF::NcGroup& group_out, 
                const Netcdf4xmlHandler::Group& group, 
                const bool convert_var_values);
            
            /**
             * Method to convert/write an XML handler dimension object in a netcdf group's dimension.
             */
            static void convert_dimension (netCDF::NcGroup& group_out,	const Netcdf4xmlHandler::Dimension&) ;
            
            /**
             * Method to convert/write an XML handler attribute object in a netcdf group's attribute. 
             */
			static void convert_attribute (netCDF::NcGroup& group_out,	const Netcdf4xmlHandler::Attribute&) ;
			
            /**
             * Method to convert/write an XML handler variable object in a netcdf group's variable. 
             */
            static void convert_variable(
                netCDF::NcGroup& group_out,	
                const Netcdf4xmlHandler::Variable&, 
                const bool convert_var_values);
			
            /**
             * Method to convert/write an XML handler attribute object in a netcdf variable's attribute. 
             */
            static void convert_var_attribute (netCDF::NcVar& var_out,	const Netcdf4xmlHandler::Attribute&) ;
            
            /**
             * Method to convert/write a matrix of values represented as vector of strings in a netcdf variable. 
             */
            static void putStringsInVariable( 
                const std::string& name,                        /*!< IN: the name of the variable to be updated */
                const netCDF::NcType::ncType& type,             /*!< IN: variable type as netCDF::NcType::ncType */
                const std::vector<std::string>& xml_values_V,   /*!< IN: the values to put in the variable */
                std::vector<size_t> elems_V,                    /*!< IN: vector of size, the last shall be the fastest */
                netCDF::NcVar& out_var ) ;                      /*!< OUT: netCDF::NcVar variable */
			
            /**
             * Template method to convert/write a vector of strings in a vector of elements of a given type.
             */		
			template<typename T>
			static void get_vector (T* v_out, const size_t& n, const std::vector<std::string>& v_in) 
			{ const size_t m ((n<v_in.size())?n:v_in.size()) ; for (size_t i=0; i<m; i++) { v_out[i] = static_cast<T>(atof(v_in[i].c_str())); }  }

		private:
			XmlToNetCDF4() ;
			~XmlToNetCDF4() ;
			XmlToNetCDF4(const XmlToNetCDF4 &) ;
			XmlToNetCDF4 &operator=(const XmlToNetCDF4 &) ;
            
	} ;

public:
    /**
     * Get the netCDF::NcType corresponding to a specific data type identifier (those managed by the Netcdf4xmlHandler)
     * @return The netCDF::NcType.
     */
    static netCDF::NcType::ncType getNcType(const std::string& str_type);
        
private:
	Netcdf4xmlConverter() ;
	~Netcdf4xmlConverter() ;
	Netcdf4xmlConverter(const Netcdf4xmlConverter &) ;
	Netcdf4xmlConverter &operator=(const Netcdf4xmlConverter &) ;

private:

	ACS_CLASS_DECLARE_DEBUG_LEVEL(Netcdf4xmlConverter) ;

};

_ACS_END_NAMESPACE
	

#endif //_Netcdf4xmlConverter_H_
