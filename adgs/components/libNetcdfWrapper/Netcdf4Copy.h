// PRQA S 1050 EOF
/*
	Copyright 1995-2016, Advanced Computer Systems , Inc.
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
	Revision 1.13  2017/05/18 12:05:51  enrcar
	EC:: copyVars i/f improved in order to optionally handle a correction factor (in progress)
	
	Revision 1.12  2016/07/15 15:06:37  enrcar
	EC:: S3PDGS-2901 copyVars: I/F added
	
	Revision 1.11  2016/03/01 13:42:07  enrcar
	S3PDGS-2797  EC:: Added method copyAtts to handle a vector of pre-filled attributes;  A specific method copyFile was added to handle this.
	
	Revision 1.10  2014/10/21 13:23:24  enrcar
	EC:: Added method: copyFile (to copy/compress a NetCDF file)
	
	Revision 1.9  2014/08/01 12:37:45  enrcar
	EC:: concatVars interface changed: row-index should be provided from outside
	
	Revision 1.8  2014/05/14 13:50:50  ricfer
	Removing compiling warning due to new ACS logging handling.
	
	Revision 1.7  2014/01/16 17:07:25  enrcar
	EC:: Added method concatenateVars(first, second, output)
	
	Revision 1.6  2014/01/10 11:16:16  enrcar
	EC:: All methods copyVars were modified in order to handle the compression
	
	Revision 1.5  2013/11/20 16:36:03  enrcar
	EC:: Interface of the method copyVars updated in order to copy an array accordingly to a pair of index and to the dimension the index belongs to.
	
	Revision 1.4  2013/05/08 10:29:16  enrcar
	EC:: Methods added: copyDimsWithExclusions, copyVarsWithExclusions
	
	Revision 1.3  2013/04/19 15:30:40  enrcar
	EC:: copyVars overload interface added
	
	Revision 1.2  2011/04/18 12:41:01  marant
	Overloaded method copyVars() added to allow variables' compression
	
	Revision 1.1  2011/03/02 11:05:17  marant
	Usage of netcdf-4.1.1 library with netCDF-4 support enabled with C++ interface provided by cxx4 APIs
	
        
*/

#ifndef _Netcdf4Copy_H_
#define _Netcdf4Copy_H_

#include <exException.h>
#include <Netcdf4DataFile.h>


_ACS_BEGIN_NAMESPACE(acs)

/**
* \brief
* <Insert Class Description> 
**/

class Netcdf4Copy  { 
public:

	/** Netcdf4Copy Exceptions */
	exDECLARE_EXCEPTION(exNetcdf4CopyException, exException) ; // Base Netcdf4Copy Exception.
	exDECLARE_EXCEPTION(exNetcdf4CopyCriticalException, exNetcdf4CopyException) ; // Critical Exception.


	/** Default Class constructor */
	Netcdf4Copy();
	/** Destructor */
	virtual ~Netcdf4Copy();

    /** COPY:  */
    /** extracts all the dimensions from file src and adds them into file dest **/
    static void copyDims(const Netcdf4DataFile& src, Netcdf4DataFile& dest);
  
    /** extracts all the attributes from file src and adds them into file dest **/
    static void copyAtts(const Netcdf4DataFile& src, Netcdf4DataFile& dest);

    /** extracts all the attributes from file src and adds them into file dest **/
    static void copyAtts(	const Netcdf4DataFile& src, Netcdf4DataFile& dest, const std::vector <NetcdfAttribute>&) ; 
							
    /** extracts all the variables from file src and adds them into file dest.
     *  Then the values of each variable are written into file dest if the flag copyData is set to true **/
	static void copyVars(const NcVar& in_var, NcVar& out_var, const std::pair<size_t, size_t>& p, const size_t& dim_to_resize=0) ;
	static void copyVars(	const NcVar& in_var, NcVar& out_var, const std::vector < std::pair<size_t, size_t> > & p, const size_t& dim_to_resize, \
							const std::pair <long double, long double>& correction_factor) ;
	static void copyVars(const NcVar& in_var, NcVar& out_var, const std::vector < std::pair<size_t, size_t> > & p, const size_t& dim_to_resize=0) ;
	static void copyVars(const Netcdf4DataFile& src, Netcdf4DataFile& dest, const NetcdfCompression& c = NetcdfCompression(), const bool& copyData = true) ;
	static void copyVars(const Netcdf4DataFile& src, Netcdf4DataFile& dest, const NetcdfCompression&, std::map<std::string, netCDF::NcVar>& newVars, const bool& copyData = true) ;
    /** As the previous, this method copies all the variables from src file into dest file and if enableShuffle is true, applies the shuffle filter
     *  while writing the variable data; if enableDeflate is true, compress the variable's data applying the deflateLevel level 
     * **/
    static void copyVars(const Netcdf4DataFile& src, Netcdf4DataFile& dest, const bool& enableShuffle, const bool& enableDeflate, const int& deflateLevel, const bool& copyData = true);


    /** aleber - August 2017 - utility function that copies the whole content of each netcdf Group (read from the source netdcf file) into a corresponding new group 
	 * (to be written into the destination netcdf file), for a single nesting level
     * **/
 	static void copyNestedGroups(const std::multimap<std::string,NcGroup>::iterator& itPrev, const NcGroup& inGroup, NcGroup& outGroup);

    /** aleber - August 2017 - utility function that copies the whole content of each netcdf Group (read from the source netdcf file) into a corresponding new group 
	 * (to be written into the destination netcdf file)
     * **/
    static void copyGroups(const Netcdf4DataFile& src, Netcdf4DataFile& dest);

    /** SELECTIVE COPY:  */
	/** extracts the dimensions matching the names in the input vector from file src and adds them into file dest*/
	static void copyDims(const Netcdf4DataFile& src, Netcdf4DataFile& dest, const std::vector<std::string>& dimnames);
	/** extracts all the dimensions from file src and adds them into file dest, excluding a list of variables **/
	static void copyDimsWithExclusions(const Netcdf4DataFile& src, Netcdf4DataFile& dest, const std::vector<std::string>& exclude_dims );
  
    /** extracts the attributes matching the names in the input vector from file src and adds them into file dest*/
    static void copyAtts(const Netcdf4DataFile& src, Netcdf4DataFile& dest, const std::vector<std::string>& match_attnames, const std::vector<std::string>& exclude_att = std::vector<std::string>() );
 
    /** extracts the variables matching the names in the input vector from file src and adds them into file dest.
      * Then the values of each variable are written into file dest **/
    static void copyVars(const Netcdf4DataFile& src, Netcdf4DataFile& dest, const NetcdfCompression&, const std::vector<std::string>& varnames, const bool& copyData = true);
    /** extracts the variables not matching the names in the exclude_vars vector from file src and adds them into file dest.
      * Then the values of each variable are written into file dest **/
	static void copyVarsWithExclusions(const Netcdf4DataFile& src, Netcdf4DataFile& dest, const NetcdfCompression&, const std::vector<std::string>& exclude_vars, const bool& copyData = true) ;

    /** CONCATENATE:  */
	/** Concatenate the first variable at the end of the second one, writing the result to the third 
		One of the dimensions will be used as "main_index" (usually the row counter)
	*/
	static void concatVars(const NcVar& first_var, const NcVar& second_var, NcVar& out_var, const size_t& main_index) ;

   	/** FILE COPY:  */
	/** Copy a whole NetCDF File
		(Useful e.g. to change the compression parameters)
	*/
	static void copyFile(const std::string& srcFile, const std::string& dstFile, const NetcdfCompression&, bool forceOverwrite) ;

	static void copyFile(	const std::string& srcFile, const std::string& dstFile, const std::vector <NetcdfAttribute>& atts_to_process, \
							const NetcdfCompression&, bool forceOverwrite) ;

private:  // copy constructor and operator= defined but not implemented
	/** Copy Constructor */
	Netcdf4Copy(const Netcdf4Copy & );
	/** Operator = */
	Netcdf4Copy &operator=(const Netcdf4Copy &);
	

protected:

private:
	ACS_CLASS_DECLARE_DEBUG_LEVEL(Netcdf4Copy)

};
_ACS_END_NAMESPACE


#endif //_Netcdf4Copy_H_

