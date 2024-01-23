// PRQA S 1050 EOF
/*

	Copyright 2017-2022, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Processors $

	$Id$

	$Author$

	$Log$
	Revision 1.1  2017/12/19 09:43:04  ricfer
	Added this script to add ACS header to all source/header/makefile files.
	

*/

#ifndef _PolarMotion_H_
#define _PolarMotion_H_ 


#include <acs_c++config.hpp>
#include <exMacros.h>
#include <DateTime.h>

_ACS_BEGIN_NAMESPACE(acs)

	
class PolarMotion 
{
public:
	/*! class PolarMotionException declaration */
	exDECLARE_EXCEPTION(PolarMotionException, exException) ; 

	/**
	 * Default Class constructor.  
	 */	
	PolarMotion(const std::string& fileNameIERSbulletin="") ;

	/**
	 * Class destructor.
	 */
	virtual ~PolarMotion() throw();

	/**
	 * Class copy constructor.   
	 */ 
	PolarMotion(const PolarMotion & ); 

    /**
     * Overload of the assign operator =.
     */
	PolarMotion &operator=(const PolarMotion &);

    /**
      * This method sets the EOP UTC time as dateTime object.
      * @param[in] The EOP UTC time as dateTime object to set.
      */
	void utcTime(const DateTime& value) throw() { utcTime_ = value; } ;
	
    /**
     * This method sets the Earth Orientation Parameter x in arcsec.
     * @param[in] The pole x component in arcsec to set.
     */
	void X(const double& value) throw() { X_ = value; } ;
	
	/**
	 * This method sets the Earth Orientation Parameter y component in arcsec.
	 * @param[in] The pole y component in arcsec to set.
	 */
	void Y(const double& value) throw() { Y_ = value; } ;
	
    /**
     * This method sets the EOPs UT1-UTC delta time in seconds.
     * @param[in] The pole coordinates EOPs UT1-UTC delta time in seconds to set.
     */
	void UT1minusUTC(const double& value) throw() { UT1minusUTC_ = value; } ;
	
    /**
     * This method sets the EOPs  TAI-UTC delta time in seconds.
     * @param[in] The EOPs  TAI-UTC delta time in seconds to set.
     */
	void TAIminusUTC(const double& value) throw() { TAIminusUTC_ = value; } ;
	
    /**
      * This method returns the IERS Bulletin file name (it can be bulletin A or B).
      * @return The IERS Bulletin file name (it can be bulletin A or B).
      */
	void fileNameIERSbulletin(const std::string& value) throw() { fileNameIERSbull_ = value; } ;
	
    /**
      * This method returns the EOP UTC time as dateTime object.
      * @return The EOP UTC time as dateTime object.
      */
	const DateTime& utcTime() const throw() { return utcTime_ ; };
	
    /**
     * This method returns the Earth Orientation Parameter x in arcsec.
     * @return The pole x component in arcsec.
     */
	const double& X() const throw() { return X_ ; };

	/**
	 * This method returns the Earth Orientation Parameter y component in arcsec.
	 * @return The pole y component in arcsec.
	 */
	const double& Y() const throw() { return Y_ ; };
	
    /**
     * This method returns the EOPs UT1-UTC delta time in seconds.
     * @return The pole coordinates EOPs UT1-UTC delta time in seconds.
     */
	const double& UT1minusUTC() const throw() { return UT1minusUTC_ ; };
	
    /**
     * This method returns the EOPs  TAI-UTC delta time in seconds.
     * @return The EOPs  TAI-UTC delta time in seconds.
     */
	const double& TAIminusUTC() const throw() { return TAIminusUTC_ ; };
	
	/**
     * This method return the IERS Bulletin file name (it shall be bulletin A or B).
     * @return The IERS Bulletin file name.
     */
	const std::string& fileNameIERSbulletin() const throw() { return fileNameIERSbull_ ; };

    /**
     * Read EOPs and times from IERS bulletin A or B
     * @param[in] bul_file_name IERS Bulletin file name
     * @param[in] time2search UTC time to search (provided as DateTime object)
     */
    void setFromIERSbulletin(const std::string& bul_file_name, const DateTime& time2search);

    /**
     * Read EOPs and times from IERS bulletin A or B
     * @param[in] time2search UTC time to search (provided as DateTime object)
     */
    void setFromIERSbulletin(const DateTime& time2search);
    	
    /**
     * Dump PolarMotion object attributes
     */
	std::string dump() const;

public:
    /**
     * Check if the provided file is an IERS bulletin A or B
     * @param[in] fileName IERS Bulletin file name
     * @return "A" if the file is a IERS bulletin A, "B" if bulletin B, otherwise empty
     */
    static std::string checkIERSbullType(const std::string& fileName);

private:
    /**
     * Read EOPs and times from IERS bulletin A
     * @param[in] bula_file_name IERS Bulletin A file name
     * @param[in] time2search UTC time to search (provided as DateTime object)
     */
    void setFromIERSA(const std::string& bula_file_name, const DateTime& time2search);
        
    /**
     * Read EOPs and times from IERS bulletin B
     * @param[in] bulb_file_name IERS Bulletin B file name
     * @param[in] time2search UTC time to search (provided as DateTime object)
     */
	void setFromIERSB(const std::string& bulb_file_name, const DateTime& time2search);

    /**
     * This method read and register EOPs and times from IERS bulletin A.
     * @param[in] bula_file_name IERS Bulletin A file name.
     */
    void readIERSA(const std::string& bula_file_name);

    /**
     * This method read and register EOPs and times from IERS bulletin B.
     * @param[in] bula_file_name IERS Bulletin B file name.
     */
    void readIERSB(const std::string& bulb_file_name);

    /**
     * This method interpolates EOP and relative UT1-UTC delta time at a given time.
     * @param[in] time2search UTC time to search (provided as DateTime object)
     * @param[in] scFact A vector of 3 scale factors, respectively for EOP x, EOP y and UT1-UTC delta time.
     */
    void interpolateEOP(const DateTime& time2search, const std::vector<double>& scFact);

    /**
     * This method read EOPs and times from IERS bulletin A in XML format.
     * @param[in] bula_file_name IERS Bulletin A file name.
     */
    void readIERSABinXML(const std::string& bula_file_name);
      	
private:

	DateTime	utcTime_ ;          /**< pole coordinate UTC time */
	double		X_ ;                /**< x coordinate of the pole in arcsec */
	double		Y_ ;                /**< y coordinate of the pole in arcsec */
	double		UT1minusUTC_ ;      /**< UT1-UTC in seconds */
	double		TAIminusUTC_ ;      /**< TAI-UTC in seconds */
    std::string fileNameIERSbull_ ; /**< IERS bulletin file name (A or B) */
	
    std::vector<double> bull_mjddate_;  /**< IERS bulletin's EOPs MJD date times */
    std::vector<double> bull_x_;        /**< IERS bulletin's EOPs x component */
    std::vector<double> bull_y_;        /**< IERS bulletin's EOPs y component */
    std::vector<double> bull_ut1_utc_;  /**< IERS bulletin's EOPs UT1-UTC delta times */
	
    ACS_CLASS_DECLARE_DEBUG_LEVEL(PolarMotion) ;

} ;

_ACS_END_NAMESPACE

#endif //_PolarMotion_H_
