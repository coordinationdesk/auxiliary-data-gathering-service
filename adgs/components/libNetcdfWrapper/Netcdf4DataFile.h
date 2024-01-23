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
	
	Revision 1.12  2017/05/19 13:42:36  enrcar
	EC:: copyVarData i/f improved in order to optionally handle a correction factor (in progress)
	
	Revision 1.11  2016/12/06 12:32:20  enrcar
	EC:: Updated in order to handle string attributes of length zero
	
	Revision 1.10  2016/03/15 16:08:08  enrcar
	S3PDGS-2829 EC:: method isFillValueSet added
	
	Revision 1.9  2016/03/08 15:46:08  enrcar
	EC:: missing "const" attribute
	
	Revision 1.8  2016/03/01 13:35:59  enrcar
	S3PDGS-2797 EC:: Added container NetCdfAttribute
	
	Revision 1.7  2014/05/14 13:50:50  ricfer
	Removing compiling warning due to new ACS logging handling.
	
	Revision 1.6  2014/01/10 11:14:31  enrcar
	EC:: Added structure to contain the compression parameters:  NetcdfCompression
	Added static method:  Netcdf4DataFile::setCompressionValue
	
	Revision 1.5  2012/05/23 14:23:36  damdec
	Compilation warnings fixed.
	
	Revision 1.4  2011/06/10 08:00:51  marant
	Added new exception exEmptyVariableException
	
	Revision 1.3  2011/05/09 07:11:44  marant
	Added method getFillValue() used also by isValid()
	
	Revision 1.2  2011/04/18 12:28:17  marant
	addVariable() modified: no data are written in the variable.
	
	Revision 1.1  2011/03/02 11:05:17  marant
	Usage of netcdf-4.1.1 library with netCDF-4 support enabled with C++ interface provided by cxx4 APIs
	
        
*/

#ifndef _Netcdf4DataFile_H_
#define _Netcdf4DataFile_H_

#include <exException.h>
#include <ncFile.h>
#include <ncDim.h>
#include <ncException.h>
#include <ncVar.h>

#include <Filterables.h>
#include <stdint.h>

_ACS_BEGIN_NAMESPACE(acs)

 using namespace netCDF;

/**
* \brief
* Compact container to store a NetcdfAttribute (the NcGroupAtt can be used only to retrive an attribute, attributes CANNOT be created from scratch)
**/

struct NetcdfAttribute {
	public:
		NetcdfAttribute(const std::string &name = "") : _filled (false), _name(name), _offset(0), _buffer(), _type(), _unique(false), _merge(false), _force(false) {}
        NetcdfAttribute(NetcdfAttribute const &r) :
			_filled(r._filled),
			_name(r._name),
			_offset(r._offset),
			_type(r._type),
			_unique(r._unique),
			_merge(r._merge),
			_force(r._force)
        { memcpy(_buffer, r._buffer, r._offset) ;  }

		NetcdfAttribute &operator=(NetcdfAttribute const &r) 
        {
            if (this != &r) {
				_filled = r._filled ;
				_name = r._name ;
				_offset = r._offset ;
				memcpy(_buffer, r._buffer, r._offset) ;  
				_type = r._type ;
				_unique = r._unique ;
 				_merge = r._merge ;
				_force = r._force ;
          }
           return *this ;
        }
		virtual ~NetcdfAttribute() throw() { _filled = false ; }
		void setName(const std::string &name) { _name = name ; }
		void setUnique(const bool b) { _unique = b ; }
		void setMerge(const bool b) { _merge = b ; }
		void setForce(const bool b) { _force = b ; }
		void set(const NcGroupAtt& a, const bool unique)		{ _offset = 0 ; _unique = unique; _name = a.getName() ; add(a) ; }
		void set(const NetcdfAttribute& a)	{ _offset = 0 ; _name = a.getName() ; add(a) ; }
		void setBinary(const NcType::ncType& type, const unsigned char* buffer, const size_t& buflen) { _offset = 0 ; addBinary(type, buffer, buflen) ; }
		void setInt(const int32_t& v)		{ _offset = 0 ; addInt(v) ; }
		void setInt64(const int64_t& v) 	{ _offset = 0 ; addInt64(v) ; } 
		void setUInt(const uint32_t& v) 	{ _offset = 0 ; addUInt(v) ; }
		void setUInt64(const uint64_t& v)	{ _offset = 0 ; addUInt64(v) ; }
		void setDouble(const double& v) 	{ _offset = 0 ; addDouble(v) ; }
		void setString(const std::string& v){ _offset = 0 ; addString(v) ; }
		void add(const NcGroupAtt& att) ;
		void add(const NetcdfAttribute& a) { _name = a.getName() ; addBinary(a._type, a._buffer, a._offset) ; }
		void addBinary(const NcType::ncType& type, const unsigned char* v, const size_t& s) ;
		void addInt(const int32_t& v)		{ addBinary ( NcType::nc_INT, reinterpret_cast<const unsigned char*>(&v), sizeof(v) ) ; }	 
		void addInt64(const int64_t& v) 	{ addBinary ( NcType::nc_INT64, reinterpret_cast<const unsigned char*>(&v), sizeof(v) ) ; }	
		void addUInt(const uint32_t& v) 	{ addBinary ( NcType::nc_UINT, reinterpret_cast<const unsigned char*>(&v), sizeof(v) ) ; }	
		void addUInt64(const uint64_t& v)	{ addBinary ( NcType::nc_UINT64, reinterpret_cast<const unsigned char*>(&v), sizeof(v) ) ; }	
		void addDouble(const double& v) 	{ addBinary ( NcType::nc_DOUBLE, reinterpret_cast<const unsigned char*>(&v), sizeof(v) ) ; }	
		void addString(const std::string& v){ addBinary ( NcType::nc_CHAR, reinterpret_cast<const unsigned char*>(v.c_str()), v.length()) ; }
		std::string getName() const { return _name ; } 
		bool getUnique() const { return _unique ; } 
		bool getMerge() const { return _merge ; } 
		bool getForce() const { return _force ; } 
		const netCDF::NcType getType() const ; 
		size_t getAttLength() const ; 
		void* getDataValue(unsigned char* buffer, const size_t& buflen) const { size_t fake ; return getDataValue(buffer, buflen, fake) ; }
		void* getDataValue(unsigned char* buffer, const size_t& buflen, size_t& elems) const ;
		std::string getAscii() const { return toAscii(_buffer, _offset) ; }	// Return the BINARY_to_ASCII of the buffer. Like e.g. "0100FF4A" ; 
		std::string dump() const
		{ std::ostringstream o ; if (!_filled) { o << "Not filled" ; } else { o << "\tName: \"" << _name << "\"\n\tType: " << getType().getTypeClassName () \
			<< "\n\tForce: " << (_force?"T":"F") << "\n\tMerge: " << (_merge?"T":"F")<< "\n\tUnique: " << (_unique?"T":"F") \
			<< "\n\tBuffer len: " << _offset << "\tBuffer: " ; for (size_t i=0; i<_offset; i++) 
			{ unsigned char c (_buffer[i]) ; o << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(c) << std::dec; } 
			} ; o << "\n" ; return o.str() ; 
		}
			
	public:
		static std::string toAscii(const unsigned char* buffer, const size_t& buflen) ; 
		static void fromAscii(const std::string& ascii, unsigned char* buffer, const size_t& buflen , size_t& buf_fil) ; 
			
	private:
		bool _filled ;
		std::string _name ;
		size_t _offset ; /* bytes */
		unsigned char _buffer[64*1024] ;
		NcType::ncType _type ;
		bool _unique ;
		bool _merge ;
		bool _force ;
} ;


/**
* \brief
* Compact container for compression parameters
**/

struct NetcdfCompression {
	public:
		NetcdfCompression() ;
		NetcdfCompression(const unsigned short& compression_level, const bool shuffle) ;
		virtual ~NetcdfCompression() {}
		NetcdfCompression(const NetcdfCompression & );
		NetcdfCompression &operator=(const NetcdfCompression &r) ;

		std::string dump() const ; 
		void setCompression(const unsigned short& compression_level, const bool shuffle) ;
		void getCompression(unsigned short& compression_level, bool& shuffle) const ;
	private:
		unsigned short	_compression_level ;
		bool			_shuffle ; 
} ;


/**
* \brief
* Netcdf4DataFile extends NcFile adding useful functionalities 
**/

class Netcdf4DataFile : public NcFile  { 
public:

	/** Netcdf4DataFile Exceptions */
    exDECLARE_EXCEPTION(exNetcdf4DataFileException, exException) ; // Base Netcdf4DataFile Exception.
    exDECLARE_EXCEPTION(exEmptyVariableException, exNetcdf4DataFileException) ; // Variable with no data
	exDECLARE_EXCEPTION(exNetcdf4DataFileCriticalException, exNetcdf4DataFileException) ; // Critical Exception.


	/** Default Class constructor */
	Netcdf4DataFile(const std::string& filename, NcFile::FileMode mode);
    Netcdf4DataFile(const std::string& filename, NcFile::FileMode mode, NcFile::FileFormat format);
	/** Destructor */
	virtual ~Netcdf4DataFile();

    //return the filename of the open netcdf resource
    std::string getFilename() const;

    //return the variable size (product of dimensions) 
    static size_t getVarSize(const NcVar& var) ;

    //return the value of the attribute _FillValue or the default value used by the library
    static double getFillValue(const NcVar& var);

    //return true is FillValue is set
    static bool isFillValueSet(const NcVar& var) ;

    //return true if the variable has not been written (all values set to fillValue)
    static bool isValid(const NcVar& var, double* fillValue = 0);

    //returns the variable data unpacking them if needed
    static void getUnpackedData(const NcVar& var, std::vector<double>& data, const std::vector<size_t>& start = std::vector<size_t>(0), const std::vector<size_t>& count  = std::vector<size_t>(0) );

    //write data into variable var and pack them according to the variable 'scale_factor' and 'add_offset' attributes
    static void putAndPackData(NcVar& var, const std::vector<double>& data);

	//set the requested compress value to the variable. THIS SHOULD BE DONE WHEN THE VARIABLE IS CREATED.
	static void setCompressionValue(NcVar& var, const NetcdfCompression& c) ;

    //return the value of the attribute 'units'
    static std::string getUnits(const NcVar& var);

    //return the value of the attribute 'scale_factor'
    static double getScaleFactor(const NcVar& var);
 
    //return true is ScaleFactor is set
    static bool isScaleFactorSet(const NcVar& var) ;

    //return the value of the attribute 'add_offset'
    static double getOffset(const NcVar& var);

    //add a global attribute to the file (root group)
    virtual void addAttr(const NcGroupAtt& att);
    virtual void addAttr(const NetcdfAttribute& att);


    /** add the variable passed as input parameter to the file (root group); the variable definition (dimensions, attributes) is copied
     *  whereas the data are neglected. Use copyVarData() to copy the data.
     */
    virtual NcVar addVariable(const NcVar& var);

    /** set the chunking for the input variable */
    void setVarChunking(const NcVar& var, const std::vector<size_t>& chunkSizes);

    // utility function used by addVariable: copies the data from one variable (src) into another one (dest)
    static void copyVarData(const NcVar& src, NcVar& dest);
    static void copyVarData(const NcVar& src, NcVar& dest, const std::pair <long double, long double>& correction_factor);

    // aleber - August 2017 - utility function that copies the whole content of a netcdf Group (read from the source netdcf file) into a new group (destGroup))
	// (to be written into the destination netcdf file)
    static void copyGroup(const NcGroup& father, const std::string& srcGroupName, const NcGroup& srcGroup, NcGroup& destGroup);

    /* writes a record of data (vals) into the input variable (var) at record position numRec */
    template <typename T>
    static void writeRecord(NcVar* var, const T* vals, const long& numRec );

    
private:  // copy constructor and operator= defined but not implemented
	/** Copy Constructor */
	Netcdf4DataFile(const Netcdf4DataFile & );
	/** Operator = */
	Netcdf4DataFile &operator=(const Netcdf4DataFile &);
	

protected:

    std::string _filename;

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(Netcdf4DataFile)

};

inline std::string Netcdf4DataFile::getFilename() const { return this->_filename; }

template <typename T>
void Netcdf4DataFile::writeRecord(NcVar* var, const T* vals, const long& numRec )
{   
    ACS_CLASS_WRITE_DEBUG(ACS_LOW_VERB, "Netcdf4DataFile::writeRecord(NcVar& var, const T* vals, const long& numRec ) method called.") ;

    try
    {
        std::vector<NcDim> dims = var->getDims();
    
        std::vector<size_t> startp;
        std::vector<size_t> countp;
    
        startp.push_back(numRec);
        countp.push_back(1);
    
        //the first dimension is considered as the record dim
        for (std::vector<NcDim>::iterator it = dims.begin() + 1; it != dims.end() ; ++it) {
            startp.push_back(0);
            countp.push_back(it->getSize());
        }
   
        var->putVar(startp, countp, vals);
    }
    catch(exceptions::NcException& ex)
    {
        ACS_CLASS_BGN_DEBUG(10)
		    ex.what();
        ACS_CLASS_END_DEBUG
        throwException(exNetcdf4DataFileException("Error while writing record to variable <" + var->getName() + ">"));
    }

   
}


template <class Component>
void addAttribute(Component& c, const NcAtt& att)
{
    const size_t attLen ( att.getAttLength() ) ;	// Number of elements 

    const size_t n ( att.getType().getSize() * attLen ) ;	// Number of bytes 

    if (n > 0) {
		const size_t bufsize(((n/1024)+1)*1024) ;
        char* values = new char[ bufsize ];

        att.getValues(reinterpret_cast<void*>(values));

        c.putAtt(att.getName(), att.getType(), attLen, reinterpret_cast<void*>(values));

        delete[] values;
    } else if (att.getType() == NcType::nc_CHAR)
	{    
		// If the length is zero, but is a string, the attribute will be copied (zero-length string)
		char values[16] ;
		memset(values, 0, sizeof(values));
		c.putAtt(att.getName(), att.getType(), 0, reinterpret_cast<void*>(values)); 
	} else {
		// Zero-length attribute and not a string: invalid
	}
}


template <class Component>
void addAttribute(Component& c, const NetcdfAttribute& att)
{
    const size_t attLen ( att.getAttLength() ) ;	// Number of elements 

    const size_t n ( att.getType().getSize() * attLen ) ;  // Number of bytes 

   if (n > 0) {
		const size_t bufsize(((n/1024)+1)*1024) ;
        unsigned char* values = new unsigned char[ bufsize ];

        att.getDataValue(values, bufsize);

        c.putAtt(att.getName(), att.getType(), attLen, reinterpret_cast<void*>(values));

        delete[] values;
    } else if (att.getType() == NcType::nc_CHAR)
	{    
		// If the length is zero, but is a string, the attribute will be copied (zero-length string)
		char values[16] ;
		memset(values, 0, sizeof(values));
		c.putAtt(att.getName(), att.getType(), 0, reinterpret_cast<void*>(values)); 
	} else {
		// Zero-length attribute and not a string: invalid
	}
        
    
}
_ACS_END_NAMESPACE

#endif //_Netcdf4DataFile_H_

