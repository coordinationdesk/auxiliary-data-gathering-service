// PRQA S 1050 EOF
/*
	Copyright 1995-2019, Advanced Computer Systems , Inc.
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
	Release 1.24.2  2019/05/23 17:30:10  enrico.carta@acsys.it
	EC:: Added new method: isScaleFactorSet
	
	Revision 1.17  2017/08/24 13:08:40  aleber
	added family of methods copyGroup* that allow to copy recursively nested netcdf groups from a source to a destination netcdf file
	
	Revision 1.16  2017/05/23 13:31:04  enrcar
	EC:: copyVarData: filler handled
	
	Revision 1.15  2017/05/19 13:42:36  enrcar
	EC:: copyVarData i/f improved in order to optionally handle a correction factor (in progress)
	
	Revision 1.14  2016/07/11 09:24:04  enrcar
	S3PDGS-2964 EC:: It turned out that method setChunking can be called only if all the dimensions of a variable are >= 1
	
	Revision 1.13  2016/06/23 13:27:01  enrcar
	EC:: Chunking disabled if data uncompressed (better performances)
	
	Revision 1.12  2016/03/15 16:08:08  enrcar
	S3PDGS-2829 EC:: method isFillValueSet added
	
	Revision 1.11  2016/03/01 13:35:59  enrcar
	S3PDGS-2797 EC:: Added container NetCdfAttribute
	
	Revision 1.10  2015/06/25 08:39:08  damdec
	Debug deprecated macro replaced.
	
	Revision 1.9  2014/05/14 13:50:50  ricfer
	Removing compiling warning due to new ACS logging handling.
	
	Revision 1.8  2014/01/10 11:14:31  enrcar
	EC:: Added structure to contain the compression parameters:  NetcdfCompression
	Added static method:  Netcdf4DataFile::setCompressionValue
	
	Revision 1.7  2012/05/23 14:23:36  damdec
	Compilation warnings fixed.
	
	Revision 1.6  2011/06/10 08:01:28  marant
	Added new exception exEmptyVariableException used in isValid()
	
	Revision 1.5  2011/05/09 07:11:44  marant
	Added method getFillValue() used also by isValid()
	
	Revision 1.4  2011/04/19 08:34:02  marant
	isValid() fixed: exception raised when the variable doesn't contain any data
	
	Revision 1.3  2011/04/18 12:28:17  marant
	addVariable() modified: no data are written in the variable.
	
	Revision 1.2  2011/03/02 13:26:16  marant
	Versioning class added
	
	Revision 1.1  2011/03/02 11:05:17  marant
	Usage of netcdf-4.1.1 library with netCDF-4 support enabled with C++ interface provided by cxx4 APIs
	
        
*/ 

#include <Netcdf4DataFile.h>
#include <algorithm>
#include <functional>
#include <numeric>
#include <cmath>
#include <NetcdfWrapperV.h>
#include <Netcdf4Utils.h>

using namespace std;
_ACS_BEGIN_NAMESPACE(acs)

static NetcdfWrapperV version; //lib versioning

ACS_CLASS_DEFINE_DEBUG_LEVEL(Netcdf4DataFile)

string NetcdfAttribute::toAscii(const unsigned char* buffer, const size_t& buflen) 
{
	ostringstream o ;
	for (size_t i=0; i<buflen; i++) { o << hex << setw(2) << setfill ('0') << static_cast<unsigned int>(buffer[i]) ; }
	return o.str() ; 
}

void NetcdfAttribute::fromAscii(const string& ascii, unsigned char* buffer, const size_t& buflen , size_t& buf_fil)
{
	buf_fil=0 ;
	size_t lps = min(buflen, ascii.length()/2) ;
	
	for (size_t i=0; i<(2*lps); i+=2)
	{
		unsigned char c (0x00) ;
		if ( (ascii.at(i)>='0') && (ascii.at(i)<='9') )		{ c+=( (ascii.at(i)-'0') << 4) ;	}
		if ( (ascii.at(i)>='A') && (ascii.at(i)<='F') )		{ c+=( (ascii.at(i)-'A'+10) << 4) ;	}
		if ( (ascii.at(i)>='a') && (ascii.at(i)<='f') )		{ c+=( (ascii.at(i)-'a'+10) << 4) ;	}
		
		if ( (ascii.at(i+1)>='0') && (ascii.at(i+1)<='9') )	{ c+=(ascii.at(i+1)-'0') ;		}
		if ( (ascii.at(i+1)>='A') && (ascii.at(i+1)<='F') )	{ c+=(ascii.at(i+1)-'A'+10) ;	}
		if ( (ascii.at(i+1)>='a') && (ascii.at(i+1)<='f') )	{ c+=(ascii.at(i+1)-'a'+10) ;	}

		buffer[buf_fil] = c ;
		buf_fil	++ ; 	
	}
}

void NetcdfAttribute::addBinary(const NcType::ncType& type, const unsigned char* needle, const size_t& s) 
{ 
	_type = type ; 
	if ((s+_offset)<sizeof(_buffer)) 
	{ 
		if ( (type == NcType::nc_CHAR) && (_unique) )
		{
			// Type is string AND must be unique. It will be added only if is not already contained in the destination.
			
			// Step 1: to compare, remove heading and trailing spaces
			size_t needle_beg(0), needle_len(s) ;

			for (size_t i=0; i<needle_len; i++) { if (needle[i] == ' ') needle_beg++ ; else break ; }
			needle_len -= needle_beg ;

			if (needle_len>0){
				for (/*signed!*/ssize_t i=(needle_len-1); i>=0; i--) { if (needle[needle_beg+i] == ' ') needle_len-- ; else break ; }
			}

			// Step 2: if after the removal of spaces the needle_len is still >0, a memcpy will be performed on the buffer
			bool found = false ;
			if ((needle_len>0) && (needle_len<=_offset)) {
				for (size_t i=0; i<=(_offset-needle_len); i++)  
				{	if (memcmp( (_buffer+i), (needle+needle_beg), needle_len) == 0) { found = true ; break ; } }
			} else if (needle_len==0) { found = true ; }
			
			if (!found) { memcpy(_buffer+_offset, needle, s) ; _offset+=s ; }
		
		} else {
			memcpy(_buffer+_offset, needle, s) ; _offset+=s ; // Just add as trailing
		}
	}  
	_filled = true ; 
}


void NetcdfAttribute::add(const NcGroupAtt& att)
{
	const NcType::ncType type ( (att.getType()).getTypeClass() ) ;	
	switch(type)
	{
		case NcType::nc_INT:
		case NcType::nc_INT64:
		case NcType::nc_UINT: 
		case NcType::nc_UINT64:
		case NcType::nc_DOUBLE:	
		case NcType::nc_CHAR:	
		{ break ; } /* Do nothing, just check the type */ 
		default: { throwException(exCriticalException("Invalid type for NcGroupAtt. Name: \"" + getName() + "\"" )) };
	}	// END switch
	_name = att.getName() ;

	const size_t elems (att.getAttLength()) ; // Number of elements
	const size_t len_bytes ( elems * att.getType().getSize() ) ;	// Number of bytes
	unsigned char buffer[ ((len_bytes/1024)+1)*1024 ] ;
	att.getValues( reinterpret_cast<void*>(buffer) ) ; 
	addBinary(type, buffer, len_bytes) ;
}


size_t NetcdfAttribute::getAttLength() const
{
	throwConditionedException( !_filled, exCriticalException("Null NetcdfAttribute")) ;
	const size_t base_len ( getType().getSize() ) ;	// Number of bytes for each element
	const size_t elems ( (base_len>0)? (_offset/base_len) : 0) ;
	return elems ; 		
}


void* NetcdfAttribute::getDataValue(unsigned char* buffer, const size_t& buflen, size_t& elems) const 
{ 
	throwConditionedException( !_filled, exCriticalException("Null NetcdfAttribute")) ;
	if (buflen < _offset) { throwException(exCriticalException("Buffer too short for NetcdfAttribute. Name: \"" + getName() + "\"" )) };
	memset(buffer, 0, buflen) ;
	memcpy(buffer, _buffer, _offset) ; 
	elems = getAttLength() ;
	return buffer ; 
}


const netCDF::NcType NetcdfAttribute::getType() const 
{
	throwConditionedException( !_filled, exCriticalException("Null NetcdfAttribute")) ;
	switch(_type)
	{
		case NcType::nc_INT:	{ netCDF::NcInt t ;		return t ; break ; } 
		case NcType::nc_INT64:	{ netCDF::NcInt64 t ;	return t ; break ; }
		case NcType::nc_UINT: 	{ netCDF::NcUint t ;	return t ; break ; }
		case NcType::nc_UINT64:	{ netCDF::NcUint64 t ;	return t ; break ; }
		case NcType::nc_DOUBLE:	{ netCDF::NcDouble t ;	return t ; break ; }
		case NcType::nc_CHAR:	{ netCDF::NcChar t ;	return t ; break ; } 
		default: { throwException(exCriticalException("Invalid type for NetcdfAttribute. Name: \"" + getName() + "\"" )) };
	}

	netCDF::NcInt t ; return t ;	/* This line will be never reached */ 
}


NetcdfCompression::NetcdfCompression() :
	_compression_level(0),
	_shuffle(false) 
{}

NetcdfCompression::NetcdfCompression(const unsigned short& compression_level, const bool shuffle) :
	_compression_level(compression_level),
	_shuffle(shuffle) 
{}

NetcdfCompression::NetcdfCompression(const NetcdfCompression &r) :
	_compression_level(r._compression_level),
	_shuffle(r._shuffle) 
{}

NetcdfCompression &NetcdfCompression::operator=(const NetcdfCompression &r)
{   
	if (&r != this) {
		_compression_level = r._compression_level ;
		_shuffle = r._shuffle ;
	}
	return *this ; 
}

string NetcdfCompression::dump() const
{
	ostringstream o ;
	o << "deflate: " << ( (_compression_level>0)? "T":"F") << "  deflateLevel: " << _compression_level 
		<< "  shuffle: " << (_shuffle? "T":"F") ;
	return o.str() ;   
}

void NetcdfCompression::setCompression(const unsigned short& compression_level, const bool shuffle) 
{
	_compression_level = compression_level ;
	_shuffle = shuffle ;

}

void NetcdfCompression::getCompression(unsigned short& compression_level, bool& shuffle) const
{
	compression_level = _compression_level ;
	shuffle = _shuffle ;
}


Netcdf4DataFile::Netcdf4DataFile(const std::string& filename, NcFile::FileMode mode) 
try
: NcFile(filename, mode), _filename(filename)
{
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4DataFile::"<< __LINE__ <<" Id: "<< Id) ;
}
catch(exceptions::NcException& ex)
{
    ACS_CLASS_BGN_DEBUG(ACS_LOW_VERB)
		ex.what();
    ACS_CLASS_END_DEBUG
    throwException(exNetcdf4DataFileException(filename + ": I/O error." ));
}

Netcdf4DataFile::Netcdf4DataFile(const std::string& filename, NcFile::FileMode mode, NcFile::FileFormat format) : NcFile(filename, mode, format), _filename(filename) {
	ACS_CLASS_ANNOUNCE_DEBUG(ACS_VLO_VERB) ;
	static char Id[] = "@(#) ACS: $Id$";
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4DataFile::"<< __LINE__ <<" Id: "<< Id) ;
}


Netcdf4DataFile::~Netcdf4DataFile() {
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4DataFile: Destructor called.") ;
}


void Netcdf4DataFile::setCompressionValue(NcVar& var, const NetcdfCompression& c) 
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4DataFile:setCompressionValue() method called.") ;

	// The total size (as number of elements) of the variable is computed
	// The size is: 0 if no dimensions are provided or if one dimension is zero.
	// Example: [] -> 0 ; [2][4][3] -> 24 ; [1][0][2] -> 0
	
	const size_t array_dim_size = var.getDimCount() ;	// Get the number of dimensions

	if (array_dim_size >= 1)	/* At least one dimension */
	{
		// If at least one dimension, the compression can be set.
		// If the total number of items >=1 the chunking can be set.
		
		size_t dim_tot (1) ;
		for (size_t i=0; i<array_dim_size; i++) { dim_tot *= var.getDim(i).getSize() ; }	// Number of elements

		unsigned short deflateLevel ; 
		bool shuffle ; 

		c.getCompression(deflateLevel, shuffle) ;

		const bool deflate( (deflateLevel>=1) ) ;	/* Set to TRUE if compression level is >=1. Set to FALSE if compression level is zero */

		var.setCompression(shuffle, deflate, deflateLevel );
		if (deflate == false)
		{
			// If UNCOMPRESSED the chunking could be disabled.
			// It turned out that UNCOMPRESSED CONTIGUOS data (expecially on "byte" vars) perform better.
			vector< size_t > v_NOTUSED ; // Useless for nc_CONTIGUOUS, but req'd by method API
			if (dim_tot >= 1) { var.setChunking ( NcVar::nc_CONTIGUOUS , v_NOTUSED ) ; }	// Chunking can be set only if there is at least one item
		} else {

			// [ DO NOTHING ]

			// In this case, a DEFAULT CHUNCKING is automatically picked.
			// Theoretically, a tuning between the possible chunks and the actual layout of the data might be performed.
			// NOTE THAT  CONTIGUOUS MODE  IS _NOT_ ALLOWED IN CASE OF COMPRESSION

			// PLS. REFER TO the NetCDF-4 official documentation and to the 2012 Unidata NetCDF Workshop (sec. 19) :
			// https://www.unidata.ucar.edu/software/netcdf/workshops/most-recent/nc4chunking/index.html
		}
	}
} 


size_t Netcdf4DataFile::getVarSize(const NcVar& var) 
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4DataFile:getVarSize() method called.") ;

    vector<NcDim> dims = var.getDims();

    size_t size(1);
    for (vector<NcDim>::const_iterator it = dims.begin(); it!= dims.end(); ++it) {
        size *= it->getSize();
    }
    
    return size;
}

double Netcdf4DataFile::getFillValue(const NcVar& var)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4DataFile:fillValue() method called.") ;

    double fillVal;
    bool userFill(false);
    try
    {
        NcVarAtt att = var.getAtt("_FillValue"); 
        att.getValues(&fillVal);
        userFill = true;
    }
    catch(exceptions::NcException& ex)
    {
    }

    if (!userFill) { //use the library fillValue

        switch (var.getType().getTypeClass()) {
        case NcType::nc_CHAR:   { fillVal = NC_FILL_CHAR; } break;
        case NcType::nc_BYTE:   { fillVal = NC_FILL_BYTE; } break;
        case NcType::nc_SHORT:  { fillVal = NC_FILL_SHORT; } break;
        case NcType::nc_INT:    { fillVal = NC_FILL_INT; } break;
        case NcType::nc_FLOAT:  { fillVal = NC_FILL_FLOAT; } break;
        case NcType::nc_DOUBLE: { fillVal = NC_FILL_DOUBLE; } break;
        case NcType::nc_UBYTE:  { fillVal = NC_FILL_UBYTE; } break;
        case NcType::nc_USHORT: { fillVal = NC_FILL_USHORT; } break;
        case NcType::nc_UINT:   { fillVal = NC_FILL_UINT; } break;
        //case NcType::nc_STRING: 

        default: break;
        }
    }

    return fillVal;
}


bool Netcdf4DataFile::isFillValueSet(const NcVar& var)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4DataFile:isFillValueSet() method called.") ;

    
	bool isSet(false) ;
    try
    {
		const map< string, NcVarAtt > atts ( var.getAtts() ) ; 
		isSet = ( atts.find("_FillValue") != atts.end() ) ;
    }
    catch(exceptions::NcException& ex)
    { isSet = false ; }

    return isSet ;
}


bool Netcdf4DataFile::isValid(const NcVar& var, double* fill )
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4DataFile:isValid() method called.") ;

    double fillValue = getFillValue(var);
    
    size_t size = getVarSize(var);

    throwConditionedException(size==0, exEmptyVariableException("No data available in the variable: cannot assess data validity."));

    vector<double> data = vector<double> (size);
    var.getVar(&data[0]);

    
    vector<double>::iterator it = find_if(data.begin(), data.end(), bind2nd( not_equal_to<double>(), fillValue) );

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4DataFile:isValid(): Variable " << var.getName() << " - fillValue = " << fillValue) ;

    if (fill!=NULL) {
        *fill = fillValue;
    }
    
    return (it != data.end());
    
    
}

string Netcdf4DataFile::getUnits(const NcVar& var)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4DataFile:getUnits() method called.") ;

    string units("");
       
    try
    {
        NcVarAtt att = var.getAtt("units"); 
        att.getValues(units);
    }
    catch(exceptions::NcException& ex)
    {
    }

    return units;

    
}

double Netcdf4DataFile::getScaleFactor(const NcVar& var)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4DataFile:getScaleFactor() method called.") ;

    double scale_factor(1);
    try
    {
        NcVarAtt att = var.getAtt("scale_factor"); 
        att.getValues(&scale_factor);
    }
    catch(exceptions::NcException& ex)
    {
    }

    return scale_factor;
}

bool Netcdf4DataFile::isScaleFactorSet(const NcVar& var)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4DataFile::isScaleFactorSet() method called.") ;

    
	bool isSet(false) ;
    try
    {
		const map< string, NcVarAtt > atts ( var.getAtts() ) ; 
		isSet = ( atts.find("scale_factor") != atts.end() ) ;
    }
    catch(exceptions::NcException& ex)
    { isSet = false ; }

    return isSet ;
}

double Netcdf4DataFile::getOffset(const NcVar& var)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4DataFile:getOffset() method called.") ;

    double add_offset(0);
    
    try
    {
        NcVarAtt att = var.getAtt("add_offset"); 
        att.getValues(&add_offset);
    }
    catch(exceptions::NcException& ex)
    {
    }

    return add_offset;

    
}


template <typename T>
class UnpackData
{
public:
    UnpackData(const double& factor, const double& offset) : _scaleFactor(factor), _offset(offset){};
    double operator()(const T& val) {
        double res(val);
        if (_scaleFactor != 1.0)
            res *= _scaleFactor;
        if (_offset != 0) 
            res += _offset;
        return res; 
            //return val*_scaleFactor + _offset; 
    };

private:
    double _scaleFactor;
    double _offset;

};

void Netcdf4DataFile::getUnpackedData(const NcVar& var, std::vector<double>& data, const vector<size_t>& start, const vector<size_t>& count){
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4DataFile:getUnpackedData() method called.") ;

    throwConditionedException( start.size()!=0 && count.size() == 0, exNetcdf4DataFileException("Specify corner (start) and edges lengths (count)") );
    throwConditionedException( start.size()==0 && count.size() != 0, exNetcdf4DataFileException("Specify corner (start) and edges lengths (count)") );

    data.clear(); //clear vector if it contains elements

    size_t size(0);
    if (count.size() > 0) {
        size = accumulate(count.begin(), count.end(), 1, multiplies<size_t>());
    }
    else
        size = Netcdf4DataFile::getVarSize(var);

    if (size > 0) {

        char* values = new char[size * var.getType().getSize()];

        try
        {
            if (count.size() > 0) {
                var.getVar(start, count, reinterpret_cast<void*>(values));
            }
            else
                var.getVar(reinterpret_cast<void*>(values));
                
        }
        catch(exceptions::NcException& ex)
        {
            ex.what();
            throwException(exNetcdf4DataFileException("Unable to load variable data"));
        }

        double scale_factor = getScaleFactor(var);
        double add_offset = getOffset(var);
        

        switch (var.getType().getTypeClass()) {
        case NcType::nc_CHAR:   { /*LANCIA ECCEZIONE*/ } break;
        case NcType::nc_BYTE:   { char* data_p = reinterpret_cast<char*>(values); transform(data_p, data_p + size, back_inserter(data), UnpackData<char>(scale_factor, add_offset) ); } break;
        case NcType::nc_SHORT:  { short* data_p = reinterpret_cast<short*>(values); transform(data_p, data_p + size, back_inserter(data), UnpackData<short>(scale_factor, add_offset) ); } break;
        case NcType::nc_INT:    { int* data_p = reinterpret_cast<int*>(values); transform(data_p, data_p + size, back_inserter(data), UnpackData<int>(scale_factor, add_offset) ); } break;
        case NcType::nc_FLOAT:  { float* data_p = reinterpret_cast<float*>(values); transform(data_p, data_p + size, back_inserter(data), UnpackData<float>(scale_factor, add_offset) ); } break;
        case NcType::nc_DOUBLE: { double* data_p = reinterpret_cast<double*>(values); transform(data_p, data_p + size, back_inserter(data), UnpackData<double>(scale_factor, add_offset) ); } break;
        case NcType::nc_UBYTE:  { u_char* data_p = reinterpret_cast<u_char*>(values); transform(data_p, data_p + size, back_inserter(data), UnpackData<u_char>(scale_factor, add_offset) ); } break;
        case NcType::nc_USHORT: { u_short* data_p = reinterpret_cast<u_short*>(values); transform(data_p, data_p + size, back_inserter(data), UnpackData<u_short>(scale_factor, add_offset) ); } break;
        case NcType::nc_UINT:   { u_int* data_p = reinterpret_cast<u_int*>(values); transform(data_p, data_p + size, back_inserter(data), UnpackData<u_int>(scale_factor, add_offset) ); } break;
        //case NcType::nc_STRING: { string str((char*)values); os << str; } break; 

        default: break;
        }
        
        delete[] values;

    }
    
}


class PackData
{
public:
    PackData(const double& factor, const double& offset) : _scaleFactor(factor), _offset(offset){};
    double operator()(const double& val) { return round((val - _offset)/_scaleFactor); };

private:
    double _scaleFactor;
    double _offset;
    
};

void Netcdf4DataFile::putAndPackData(NcVar& var, const std::vector<double>& data){
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4DataFile:putAndPackData() method called on variable <" << var.getName() << ">") ;
        
    double scale_factor = getScaleFactor(var);
    double add_offset = getOffset(var);
    

    if ( (scale_factor!=1) || (add_offset!=0) ) {
        vector<double> packedData;
        transform(data.begin(), data.end(), back_inserter(packedData), PackData(scale_factor, add_offset) );
        
        var.putVar(&packedData[0]);

    }
    else  {
        var.putVar(&data[0]);
    }

    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4DataFile:putAndPackData() method executed.") ;
    
        
}

void Netcdf4DataFile::addAttr(const NcGroupAtt& att)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4DataFile: addAtt(NcGroupAtt) method called.") ;

    addAttribute(*this, att);
    
}

void Netcdf4DataFile::addAttr(const NetcdfAttribute& att)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4DataFile: addAtt(NetcdfAttribute) method called.") ;

    addAttribute(*this, att);
    
}

void Netcdf4DataFile::copyVarData(const NcVar& src, NcVar& dest )
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4DataFile: copyData() method called.") ;

    size_t size = getVarSize(src);

    size_t dataSize = src.getType().getSize()*size; //number of bytes

    if (dataSize > 0) {
            char* values = new char[dataSize];
            src.getVar(reinterpret_cast<void*>(values));
            dest.putVar(reinterpret_cast<void*>(values));
            delete[] values;
    }
}

void Netcdf4DataFile::copyVarData(const NcVar& src, NcVar& dest, const pair <long double, long double>& correction_factor )
{
	const long double correction_mult(correction_factor.first) ;
	const long double correction_add(correction_factor.second) ;

	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4DataFile: copyData() method called.") ;

    size_t size = getVarSize(src);

    size_t dataSize = src.getType().getSize()*size; //number of bytes

	if (dataSize > 0) {
        char* values = new char[dataSize];

		/* Fill a vector by reading the NetCDF */
		if ((correction_mult != 1.0) || (correction_add != 0.0))
		{
			const netCDF::NcType::ncType t ( src.getType().getTypeClass() ) ;

			switch(t)
			{
				case netCDF::NcType::nc_CHAR:
				{
					// No correction possibile since a "CHAR" is actually a printable character. For math use "BYTE"
					src.getVar(reinterpret_cast<void*>(values));
					dest.putVar(reinterpret_cast<void*>(values));
					break ;
				} 
				case netCDF::NcType::nc_BYTE: 
				{ 
					signed char* p = reinterpret_cast<signed char*>(values) ;
					signed char filler ; bool filler_set ;
					src.getVar(p) ; src.getFillModeParameters(filler_set, filler) ;
					for (size_t i=0; i<(size); i++) { if (!filler_set || (p[i]!=filler)) { p[i] *= correction_mult ; p[i] += correction_add ; } }
					dest.putVar(p) ;
					break ; 
				}
				case netCDF::NcType::nc_UBYTE: 
				{ 
					unsigned char* p = reinterpret_cast<unsigned char*>(values) ;
					unsigned char filler ; bool filler_set ;
					src.getVar(p) ; src.getFillModeParameters(filler_set, filler) ;
					for (size_t i=0; i<(size); i++) { if (!filler_set || (p[i]!=filler)) { p[i] *= correction_mult ; p[i] += correction_add ; } }
					dest.putVar(p) ;
					break ; 
				}
				case netCDF::NcType::nc_SHORT: 
				{ 
					int16_t* p = reinterpret_cast<int16_t*>(values) ;
					int16_t filler ; bool filler_set ;
					src.getVar(p) ; src.getFillModeParameters(filler_set, filler) ;
					for (size_t i=0; i<(size); i++) { if (!filler_set || (p[i]!=filler)) { p[i] *= correction_mult ; p[i] += correction_add ; } }
					dest.putVar(p) ;
					break ; 
				}
				case netCDF::NcType::nc_USHORT: 
				{ 
					uint16_t* p = reinterpret_cast<uint16_t*>(values) ;
					uint16_t filler ; bool filler_set ;
					src.getVar(p) ; src.getFillModeParameters(filler_set, filler) ;
					for (size_t i=0; i<(size); i++) { if (!filler_set || (p[i]!=filler)) { p[i] *= correction_mult ; p[i] += correction_add ; } }
					dest.putVar(p) ;
					break ; 
				}
				case netCDF::NcType::nc_INT: 
				{ 
					int32_t* p = reinterpret_cast<int32_t*>(values) ;
					int32_t filler ; bool filler_set ;
					src.getVar(p) ; src.getFillModeParameters(filler_set, filler) ;
					for (size_t i=0; i<(size); i++) { if (!filler_set || (p[i]!=filler)) { p[i] *= correction_mult ; p[i] += correction_add ; } }
					dest.putVar(p) ;
					break ; 
				}
				case netCDF::NcType::nc_UINT: 
				{ 
					uint32_t* p = reinterpret_cast<uint32_t*>(values) ;
					uint32_t filler ; bool filler_set ;
					src.getVar(p) ; src.getFillModeParameters(filler_set, filler) ;
					for (size_t i=0; i<(size); i++) { if (!filler_set || (p[i]!=filler)) { p[i] *= correction_mult ; p[i] += correction_add ; } }
					dest.putVar(p) ;
					break ; 
				}
				case netCDF::NcType::nc_INT64: 
				{ 
					int64_t* p = reinterpret_cast<int64_t*>(values) ;
					int64_t filler ; bool filler_set ;
					src.getVar(p) ; src.getFillModeParameters(filler_set, filler) ;
					for (size_t i=0; i<(size); i++) { if (!filler_set || (p[i]!=filler)) { p[i] *= correction_mult ; p[i] += correction_add ; } }
					dest.putVar(p) ;
					break ; 
				}
				case netCDF::NcType::nc_UINT64: 
				{ 
					uint64_t* p = reinterpret_cast<uint64_t*>(values) ;
					uint64_t filler ; bool filler_set ;
					src.getVar(p) ; src.getFillModeParameters(filler_set, filler) ;
					for (size_t i=0; i<(size); i++) { if (!filler_set || (p[i]!=filler)) { p[i] *= correction_mult ; p[i] += correction_add ; } }
					dest.putVar(p) ;
					break ; 
				}
				case netCDF::NcType::nc_FLOAT: 
				{ 
					float* p = reinterpret_cast<float*>(values) ;
					float filler ; bool filler_set ;
					src.getVar(p) ; src.getFillModeParameters(filler_set, filler) ;
					for (size_t i=0; i<(size); i++) { if (!filler_set || (p[i]!=filler)) { p[i] *= correction_mult ; p[i] += correction_add ; } }
					dest.putVar(p) ;
					break ; 
				}
				case netCDF::NcType::nc_DOUBLE: 
				{ 
					double* p = reinterpret_cast<double*>(values) ;
					double filler ; bool filler_set ;
					src.getVar(p) ; src.getFillModeParameters(filler_set, filler) ;
					for (size_t i=0; i<(size); i++) { if (!filler_set || (p[i]!=filler)) { p[i] *= correction_mult ; p[i] += correction_add ; } }
					dest.putVar(p) ;
					break ; 
				}
				default: 
				{ throwException( exCriticalException( "Netcdf4Copy::copyVars : Unsupported type: " + (src.getType().getName())) ) ; }  

			}	// END: switch

		} else {

			src.getVar(reinterpret_cast<void*>(values));
			dest.putVar(reinterpret_cast<void*>(values));

		}	// END: if ((correction_mult != 1.0) || (correction_add != 0.0))

		delete[] values;
	}	// END:	if (dataSize > 0) {

}

void Netcdf4DataFile::copyGroup(const NcGroup& father, const string& srcGroupName, const NcGroup& srcGroup, NcGroup& destGroup)
{

    // aleber - August 2017 - utility function that copies the whole content of a netcdf Group (read from the source netdcf file) into a new group (destGroup))
	// (to be written into the destination netcdf file)

    vector<NcDim> varDims;
    vector<NcDim> varDimsPrev;
	
	NcType varType;
	NcType varTypePrev;
	netCDF::NcEnumType enumtype;
	netCDF::NcEnumType::ncEnumType etype;
	etype=static_cast<netCDF::NcEnumType::ncEnumType>(NC_INT);
	
	destGroup = father.addGroup(srcGroupName) ;

	multimap<string,NcDim> dims = srcGroup.getDims();
	multimap<string,NcVar> vars = srcGroup.getVars();

    multimap<string,NcGroupAtt> srcGroupAtts = srcGroup.getAtts();

	char* dataValues;

    for (multimap<string,NcGroupAtt>::iterator it=srcGroupAtts.begin(); it!=srcGroupAtts.end(); it++) {

		NcGroupAtt att = it->second;
		NcType attrType = att.getType();
		size_t attrLength = att.getAttLength();
		size_t size = attrLength * attrType.getSize();

		dataValues = new char[size];
		att.getValues(reinterpret_cast<void*>(dataValues));						

//    	NcGroupAtt putAtt(const std::string& name, const NcType& type, size_t len, const void* dataValues) const ;
		destGroup.putAtt(it->first, attrType, attrLength, dataValues); 		
	}

    for (multimap<string,NcDim>::iterator itd1 = dims.begin(); itd1!=dims.end(); itd1++) {		
		destGroup.addDim(itd1->first,itd1->second.getSize());
	}

	multimap<string,NcType> srcGroupTypes = srcGroup.getTypes();
    for (multimap<string,NcType>::iterator itt1 = srcGroupTypes.begin(); itt1!=srcGroupTypes.end(); itt1++) {
		if (itt1->second.getTypeClass() == NC_ENUM) {		
				
			netCDF::NcEnumType enumtypeSrc = NcEnumType(itt1->second);
			size_t numOfMember = enumtypeSrc.getMemberCount();
			enumtype = destGroup.addEnumType(itt1->first, etype);
			string memberName[numOfMember];
			for (size_t memberNum = 0; memberNum < numOfMember; ++memberNum) {
				memberName[memberNum] = enumtypeSrc.getMemberNameFromIndex(memberNum);
				enumtype.addMember(memberName[memberNum],memberNum);
			} 

		}		
	}

    for (multimap<string,NcVar>::iterator itv1 = vars.begin(); itv1!=vars.end(); itv1++) {		
    	varDims = itv1->second.getDims();
		itv1->second.getName();
		NcType varType = itv1->second.getType();
   		destGroup.addVar(itv1->second.getName(), varType, varDims);		
    	NcVar newVar = destGroup.getVar(itv1->second.getName());    
    	Netcdf4DataFile::copyVarData(itv1->second, newVar);
	}
	
}

NcVar Netcdf4DataFile::addVariable(const NcVar& var)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4DataFile: addVar() method called.") ;

    vector<NcDim> dims = var.getDims();

    NcVar newVar = NcFile::addVar(var.getName(), var.getType(), dims);

    map<string,NcVarAtt> atts = var.getAtts();

    for (map<string,NcVarAtt>::iterator it = atts.begin(); it!=atts.end(); it++) {
        addAttribute(newVar, it->second);
    }

    return newVar;
       
}

void Netcdf4DataFile::setVarChunking(const NcVar& var, const std::vector<size_t>& chunkSizes)
{
	ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4DataFile: setVarChunking() method called.") ;

    int varId = var.getId();

    int status = nc_def_var_chunking(this->getId(), varId, NC_CHUNKED, &chunkSizes[0]);

    throwConditionedException(status != NC_NOERR, exNetcdf4DataFileException("Netcdf error: " + string(nc_strerror(status))) );
       
}

_ACS_END_NAMESPACE
