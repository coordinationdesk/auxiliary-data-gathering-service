// PRQA S 1050 EOF
/*
	Copyright 1995-2018, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. libNetcdfWrapper$

	$Id$

	$Author$

	$Log$
	Revision 1.6  2017/03/08 14:06:57  enrcar
	EC:: Added method isHDF5File (returns TRUE for NetCDF/HDF5 files)
	
	Revision 1.5  2015/03/10 12:59:35  enrcar
	EC:: Method added: getVarDimensions
	
	Revision 1.4  2014/05/14 13:50:50  ricfer
	Removing compiling warning due to new ACS logging handling.
	
	Revision 1.3  2013/11/20 16:31:04  enrcar
	EC:: Added method findVarDimensionIndex(var, dim_name) to extract from a variable the index of a dimension given its name
	
	Revision 1.2  2012/05/23 14:22:47  damdec
	Compilation error and warnings fixed.
	
	Revision 1.1  2011/03/02 11:05:17  marant
	Usage of netcdf-4.1.1 library with netCDF-4 support enabled with C++ interface provided by cxx4 APIs
	
        
*/ 

#include <Netcdf4Utils.h>
#include <StringUtils.h>
#include <Filterables.h>
#include <File.h>
#include <iterator>
#include <algorithm>
#include <limits>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

ACS_CLASS_DEFINE_DEBUG_LEVEL(Netcdf4Utils)


Netcdf4Utils::Netcdf4Utils() {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4Utils::"<< __LINE__ <<" Id: "<< Id) ;
}

Netcdf4Utils::~Netcdf4Utils() {
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4Utils: Destructor called.") ;
}

bool Netcdf4Utils::isHDF5File(const std::string& fileName) 
{

	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		excerr << "Method called. Filename: \"" << fileName << "\"" << endl ;
	ACS_CLASS_END_DEBUG

	bool success (false) ;
	
	if (File::exists(fileName) && !File::isDir(fileName)) 
	{
		unsigned char magic[] = { 0x89, 0x48, 0x44, 0x46, 0x0d, 0x0a, 0x1a, 0x0a } ; // Magic number for HDF5/NetCDF files
		const ssize_t magic_len ( sizeof (magic) ) ;	// Bytes to compare in the magic number
		unsigned char buffer[1024] ;
		ssize_t r(-1) ;
		int fd = open (fileName.c_str(), O_RDONLY) ;
		if (fd >= 0)
		{
			r = read(fd, buffer, magic_len) ;
			close (fd) ;
		}

		success = ( (r == magic_len) && (0 == memcmp(buffer, magic, magic_len)) ) ;
	} 


	ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		excerr << "Method finished. Returning: " << (success?"T":"F") << endl ;
	ACS_CLASS_END_DEBUG

	return success ;
}


template <typename T>
void printArray(ostream& os, size_t len, T* array, string delim = ", ")
{
    os << setprecision(numeric_limits<T>::digits10);//set precision
    //copy values into the stream os
    ostream_iterator<T> oi(os, delim.c_str());
    copy(&array[0], &array[len-1], oi);
    os << array[len-1];
}


void printByteArray(ostream& os, size_t len, char* array, string delim = ", ")
{
    ostream_iterator<int> oi(os, delim.c_str());
    copy(&array[0], &array[len-1], oi);
    os << static_cast<int>(array[len-1]);
}

void Netcdf4Utils::dumpValues(ostream& os, NcType type, size_t len, void* values)
{
    switch (type.getTypeClass()) {
    case NcType::nc_CHAR:   { char* data_p = reinterpret_cast<char*>(values); os << "\""; printArray(os,len,data_p,""); os << "\""; } break;
    case NcType::nc_BYTE:   { char* data_p = reinterpret_cast<char*>(values); printByteArray(os,len,data_p); } break;
    case NcType::nc_SHORT:  { short* data_p = reinterpret_cast<short*>(values); printArray(os,len,data_p); } break;
    case NcType::nc_INT:    { int* data_p = reinterpret_cast<int*>(values); printArray(os,len,data_p); } break;
    case NcType::nc_FLOAT:  { float* data_p = reinterpret_cast<float*>(values); printArray(os,len,data_p); } break;
    case NcType::nc_DOUBLE: { double* data_p = reinterpret_cast<double*>(values); printArray(os,len,data_p); } break;
    case NcType::nc_UBYTE:  { u_char* data_p = reinterpret_cast<u_char*>(values); printArray(os,len,data_p); } break;
    case NcType::nc_USHORT: { u_short* data_p = reinterpret_cast<u_short*>(values); printArray(os,len,data_p); } break;
    case NcType::nc_UINT:   { u_int* data_p = reinterpret_cast<u_int*>(values); printArray(os,len,data_p); } break;
    case NcType::nc_STRING: { string str(reinterpret_cast<char*>(values)); os << str; } break;
    default: break;
    }
    

}

void Netcdf4Utils::dumpVarDims(std::ostream& os, const NcVar& var)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4Utils::dumpVarDims().") ;

    if (var.getDimCount() > 0) {

        vector<NcDim> dims = var.getDims();

        os << "(";
        for (vector<NcDim>::const_iterator it = dims.begin(); it!= dims.end(); ++it) {
            os << it->getName() ;
            if (it!= dims.end() -1) {
                os << ", ";
            }
        }
        os << ");" << endl;
    }

}

void Netcdf4Utils::dumpVar(ostream& os, const NcVar& var)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4Utils::dumpVar().") ;

    os << var.getType().getName() << " " << var.getName()  ;
    dumpVarDims(os, var); //dump dimensions

    //and now dump the variable atrtibutes
    typedef map<string, NcVarAtt> att_map;
    att_map atts = var.getAtts();
    
    for (att_map::const_iterator it = atts.begin(); it!= atts.end(); ++it) {

        size_t size = it->second.getAttLength() * it->second.getType().getSize();

        char* values = new char[size];
        it->second.getValues(reinterpret_cast<void*>(values));

        os << "\t" << var.getName() << ":" << it->second.getName() << " = " ;
        dumpValues(os, it->second.getType(), it->second.getAttLength(), values);
        os << endl;

        delete[] values;
    }

   
}


void Netcdf4Utils::dumpAttValues(std::ostream& os, const NcVarAtt& att)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4Utils::dumpAttValues(NcVarAtt) called.") ;

    size_t size = att.getAttLength() * att.getType().getSize();

    char* values = new char[size];
    att.getValues(reinterpret_cast<void*>(values));

    dumpValues(os, att.getType(), att.getAttLength(), values);
    
    delete[] values;
 

}


void Netcdf4Utils::dumpAttValues(std::ostream& os, const NcGroupAtt& att)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4Utils::dumpAttValues(NcGroupAtt) called.") ;

    size_t size = att.getAttLength() * att.getType().getSize();

    char* values = new char[size];
    att.getValues(reinterpret_cast<void*>(values));

    dumpValues(os, att.getType(), att.getAttLength(), values);
    
    delete[] values;
 

}


/** returns the INDEX of a dimension in a variable, given the var. and the NAME of the dimension
  * If the variable doesn't contain a dimension with that name, either an exception or -1 is returned (accordingly to emit_exception) */
ssize_t Netcdf4Utils::findVarDimensionIndex(const NcVar& var, const string& dim_name, const bool emit_exception)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4Utils::findVarDimensionIndex called. dim_name: \"" << dim_name << "\"") ;

	ssize_t ret = -1 ;	/* Meaning not found */ 

	const size_t dim_size = var.getDimCount() ;

	if (dim_size<1) 
	{
    	throwConditionedException( emit_exception, exNetcdf4UtilsException("At least one dimension was expected!")) ;
	}
	else 
	{
		for (size_t dim=0; dim<dim_size; dim++)
		{
			if ( StringUtils::equalsNoCase(var.getDim(dim).getName(), dim_name) )
			{
				ret = dim; /* Dimension was found */
				break ;
			}
		}

    	throwConditionedException( (emit_exception && (ret<0)), exNetcdf4UtilsException("Dimension not found: " + dim_name)) ;
	}

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4Utils::findVarDimensionIndex finished. dim_name: \"" << dim_name << "\"  returning: " << ret) ;

	return ret ;
}
	

/** returns a vector with all the dimensions of a variable
  * It can be used e.g. to evaluate the minimum and the maximum dimension (since the dimensions are name-less, just an array, other uses are deprecated) */
vector <size_t> Netcdf4Utils::getVarDimensions(const NcVar& var)
{
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4Utils::getVarDimensions called.") ;

	vector <size_t> ret ; 
	
	const vector < NcDim > v ( var.getDims() ) ;

	for (size_t i=0; i<v.size(); i++) { ret.push_back( v.at(i).getSize() ) ; }

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4Utils::getVarDimensions finished. Vector size: " << ret.size() 
		<< "  MIN: " << *min_element(ret.begin(), ret.end()) << "  MAX: " << *max_element(ret.begin(), ret.end()) );

	return ret ;
}


/** Return the value of "-1" (contained inside a long long int) with the same size of the NetCDF variable.
	E.G. if the variable is on 16 bit, return 0xffff (i.e. +65535). This allows a fast comparing with "-1" (used for e.g. empty flags) */
unsigned long long int Netcdf4Utils::getVarNegativeValue(const NcVar& var) 
{
	const size_t sz (var.getType().getSize()) ; 
	unsigned long long int value ;
	
	switch(sz)
	{
		case 1: value = 0xff ;					break ; // -1 on 1 byte
		case 2: value = 0xffff ;				break ; // -1 on 2 bytes
		case 4: value = 0xffffffff ; 			break ; // -1 on 4 bytes
		case 8: value = 0xffffffffffffffffLL ;	break ; // -1 on 8 bytes
		default: { throwException(exCriticalException("Invalid dimension for variable" )) };
	}

	return value ; 
}

	
_ACS_END_NAMESPACE
