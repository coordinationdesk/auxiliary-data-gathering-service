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
	Revision 1.5  2017/03/08 14:06:57  enrcar
	EC:: Added method isHDF5File (returns TRUE for NetCDF/HDF5 files)
	
	Revision 1.4  2015/03/10 12:59:35  enrcar
	EC:: Method added: getVarDimensions
	
	Revision 1.3  2014/05/14 13:50:50  ricfer
	Removing compiling warning due to new ACS logging handling.
	
	Revision 1.2  2013/11/20 16:31:04  enrcar
	EC:: Added method findVarDimensionIndex(var, dim_name) to extract from a variable the index of a dimension given its name
	
	Revision 1.1  2011/03/02 11:05:17  marant
	Usage of netcdf-4.1.1 library with netCDF-4 support enabled with C++ interface provided by cxx4 APIs
	
        
*/

#ifndef _Netcdf4Utils_H_
#define _Netcdf4Utils_H_

#include <exException.h>
#include <sstream>
#include <Netcdf4DataFile.h>


_ACS_BEGIN_NAMESPACE(acs)

/**
* \brief
* Netcdf4 Utility class
**/

class Netcdf4Utils  { 
public:

	/** Netcdf4Utils Exceptions */
	exDECLARE_EXCEPTION(exNetcdf4UtilsException, exException) ; // Base Netcdf4Utils Exception.
	exDECLARE_EXCEPTION(exNetcdf4UtilsCriticalException, exNetcdf4UtilsException) ; // Critical Exception.


	/** Default Class constructor */
	Netcdf4Utils();
	/** Destructor */
	virtual ~Netcdf4Utils();

    //dump the input variable on the stream passed as input parameter in CDL style
    static void dumpVar(std::ostream& os, const NcVar& var);

    //dump the dimensions of the input variable on the stream passed as input parameter
    static void dumpVarDims(std::ostream& os, const NcVar& var);

    //dump the input variable attribute on the stream passed as input parameter
    static void dumpAttValues(std::ostream& os, const NcVarAtt& att);

    //dump the input group attribute on the stream passed as input parameter
    static void dumpAttValues(std::ostream& os, const NcGroupAtt& att);

    /** returns the INDEX of a dimension in a variable, given the var. and the NAME of the dimension
      * If the variable doesn't contain a dimension with that name, either an exception or -1 is returned (accordingly to emit_exception) */
    static ssize_t findVarDimensionIndex(const NcVar& var, const std::string& dim_name, const bool emit_exception=true) ;
	
	static std::vector<size_t> getVarDimensions(const NcVar& var) ; 
	
	/** TRUE if file exists and is a HDF5/NetCDF file */
	static bool isHDF5File(const std::string& fileName) ;

	/** Return the value of "-1" (contained inside a long long int) with the same size of the NetCDF variable.
		E.G. if the variable is on 16 bit, return 0xffff (i.e. +65535). This allows a fast comparing with "-1" (used for e.g. empty flags) */
	static unsigned long long int getVarNegativeValue(const NcVar& var) ;

private:  // copy constructor and operator= defined but not implemented
	/** Copy Constructor */
	Netcdf4Utils(const Netcdf4Utils & );
	/** Operator = */
	Netcdf4Utils &operator=(const Netcdf4Utils &);

    //dump values on the stream passed as input parameter using the type and lenght information
    static void dumpValues(std::ostream& os, NcType type, size_t len, void* values);
	

protected:

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(Netcdf4Utils)

};

_ACS_END_NAMESPACE

#endif //_Netcdf4Utils_H_

