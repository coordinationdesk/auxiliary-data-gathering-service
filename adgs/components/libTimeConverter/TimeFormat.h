// PRQA S 1050 EOF
/*

	Copyright 1995-2014, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod:  $

	$Id$

	$Author$

    $Log$
    Revision 2.13  2015/11/30 08:59:47  giucas
    SatelliteID updated

    Revision 2.12  2014/04/08 13:05:49  marpas
    coding best practices applied
    interface rationalization in progress.

    Revision 2.11  2014/03/18 12:32:04  marpas
    insertion operators added

    Revision 2.10  2014/03/18 12:08:34  marpas
    Coding best practices applied
    warnings removed (g++ & qa)
    interface rationalization


*/


#ifndef _TimeFormat_H_
#define _TimeFormat_H_ 

#include <acs_c++config.hpp>
#include <exStream.h>

_ACS_BEGIN_NAMESPACE(acs)
	
class TimeFormat 
{
public:
	// Processing time format ID 
	enum ProcessingTimeFormat
	{

	  TC_PROC_UNDEF		 = -1, //not defined in Explorer Lib
	  TC_PROC 			 = 0

	} ;


	// Transport time format ID 
	enum TransportTimeFormat
	{

	  TC_TRANS_UNDEF		 = -1,	//not defined in Explorer Lib
	  TC_TRANS_STD           = 0,	//Days, Seconds, Microseconds
	  TC_TRANS_ENVI_GS       = 11,	//Days, Seconds, Microseconds
	  TC_TRANS_CRYO_GS       = 21,	//Days, Seconds, Microseconds
	  TC_TRANS_CRYO_TM       = 22,	//Days, MilliSeconds, Microseconds
	  TC_TRANS_CRYO_TM_SIRAL = 23,	//Days, MilliSeconds, Microseconds, SIRAL extra counter
  	  TC_TRANS_SMOS_TM          = 31,
	  TC_TRANS_GENERIC_GPS_SEC  = 41,
	  TC_TRANS_GENERIC_GPS_WEEK = 42

	} ;

	// ASCII time format ID
	enum AsciiTimeFormat
	{
	  TC_ASCII_ERS_MILLISEC		       = -7,	//dd-mmm-yyyy_hh:nn:ss.uuu	
	  TC_ASCII_COSMO_COMPACT	       = -6,    //yyyymmddhhnnss
	  TC_ASCII_COSMO_NANOSEC	       = -5,    //YYYY-MM-DD hh:mm:ss.nnnnnnnnn
	  TC_ASCII_COSMO_MICROSEC	       = -4,    //YYYY-MM-DD hh:mm:ss:nnnnnn
	  TC_ASCII_COSMO_MILLISEC	       = -3,    //YYYY-MM-DD hh:mm:ss:nnn
	  TC_ASCII_COSMO_SEC		       = -2,    //YYYY-MM-DD hh:mm:ss
	  TC_ASCII_UNDEF                       = -1,
	  TC_ASCII_STD                         = 11,	//yyyy-mm-dd_hh:nn:ss
	  TC_ASCII_STD_REF                     = 12,	//RRR=yyyy-mm-dd_hh:nn:ss
	  TC_ASCII_STD_MICROSEC                = 13,	//yyyy-mm-dd_hh:nn:ss.uuuuuu
	  TC_ASCII_STD_REF_MICROSEC            = 14,	//RRR=yyyy-mm-dd_hh:nn:ss.uuuuuu
	  TC_ASCII_COMPACT                     = 21,	//yyyymmdd_hhnnss
	  TC_ASCII_COMPACT_REF                 = 22,	//RRR=yyyymmdd_hhnnss
	  TC_ASCII_COMPACT_MICROSEC            = 23,	//yyyymmdd_hhnnssuuuuuu
	  TC_ASCII_COMPACT_REF_MICROSEC        = 24,	//RRR=yyyymmdd_hhnnssuuuuuu
	  TC_ASCII_ENVI                        = 31,	//dd-mmm-yyyy hh:nn:ss
	  TC_ASCII_ENVI_REF                    = 32,	//RRR=dd-mmm-yyyy hh:nn:ss
	  TC_ASCII_ENVI_MICROSEC               = 33,	//dd-mmm-yyyy hh:nn:ss.uuuuuu
	  TC_ASCII_ENVI_REF_MICROSEC           = 34,	//RRR=dd-mmm-yyyy hh:nn:ss.uuuuuu
	  TC_ASCII_CCSDSA                      = 41,	//yyyy-mm-ddThh:nn:ss
	  TC_ASCII_CCSDSA_REF                  = 42,	//RRR=yyyy-mm-ddThh:nn:ss
	  TC_ASCII_CCSDSA_MICROSEC             = 43,	//yyyy-mm-ddThh:nn:ss.uuuuuu
	  TC_ASCII_CCSDSA_REF_MICROSEC         = 44,	//RRR=yyyy-mm-ddThh:nn:ss.uuuuuu
	  TC_ASCII_CCSDSA_COMPACT              = 51,	//“yyyymmddThhnss
	  TC_ASCII_CCSDSA_COMPACT_REF          = 52,	//“RRR=yyyymmddThhnnss
	  TC_ASCII_CCSDSA_COMPACT_MICROSEC     = 53,	//yyyymmddThhnnssuuuuuu
	  TC_ASCII_CCSDSA_COMPACT_REF_MICROSEC = 54	//RRR=yyyymmddThhnnssuuuuuu

	} ;


	// Time reference ID
	enum TimeReference
	{

	  TC_TIME_UNDEF = -1,
	  TC_TIME_TAI,
	  TC_TIME_UTC,
	  TC_TIME_UT1,
	  TC_TIME_GPS

	} ;


	// Time Initialization mode
	enum TimeInitMode
	{

	  TC_SEL_TIME = 1,
	  TC_SEL_ORBIT

	} ;

	// Time Model
	enum TimeModel
	{
	  TC_TIMEMOD_AUTO = -2,
	  TC_TIMEMOD_USER = -1,
	  TC_TIMEMOD_NONE = 0,
	  TC_TIMEMOD_IERS_B_PREDICTED = 1,	
	  TC_TIMEMOD_IERS_B_RESTITUTED,		
	  TC_TIMEMOD_FOS_PREDICTED,			
	  TC_TIMEMOD_FOS_RESTITUTED,		
	  TC_TIMEMOD_DORIS_PRELIMINARY,		
	  TC_TIMEMOD_DORIS_PRECISE,			
	  TC_TIMEMOD_DORIS_NAVIGATOR,		
	  TC_TIMEMOD_OSF,				    
	  TC_TIMEMOD_IERS_A_ONLY_PREDICTION,
	  TC_TIMEMOD_IERS_A_PREDICTION_AND_FORMULA,
	  TC_TIMEMOD_IERS_B_AND_A_ONLY_PREDICTION
	  
	} ;


	//Satellite ID

	enum SatelliteID
	{
	   TC_SAT_CSGS2	  	  = -16,
	   TC_SAT_CSGS1	  	  = -15,
	   TC_SAT_OS2	  	  = -14,
	   TC_SAT_SACD	  	  = -13,
	   TC_SAT_RSAT	  	  = -12,
	   TC_SAT_ALOS	  	  = -11,
	   TC_SAT_EOSPM	  	  = -10,
	   TC_SAT_EOSAM	  	  = -9,
	   TC_SAT_CSKS8	  	  = -8,
	   TC_SAT_CSKS7	  	  = -7,
	   TC_SAT_CSKS6	      = -6,    
	   TC_SAT_CSKS5	  	  = -5,
	   TC_SAT_CSKS4	  	  = -4,
	   TC_SAT_CSKS3	  	  = -3,
	   TC_SAT_CSKS2	  	  = -2,
	   TC_SAT_CSKS1	  	  = -1,
	   TC_SAT_DEFAULT 	  = 0,
	   TC_SAT_ERS1    	  = 11,
	   TC_SAT_ERS2    	  = 12,
	   TC_SAT_ENVISAT 	  = 21,
	   TC_SAT_METOP1  	  = 31,
	   TC_SAT_METOP2  	  = 32,
	   TC_SAT_METOP3  	  = 33,
	   TC_SAT_CRYOSAT 	  = 41,
	   TC_SAT_ADM     	  = 51,
	   TC_SAT_GOCE    	  = 61,
	   TC_SAT_SMOS    	  = 71,
	   TC_SAT_TERRASAR    = 81,
	   TC_SAT_EARTHCARE   = 91,
	   TC_SAT_SWARM_A     = 101,
	   TC_SAT_SWARM_B     = 102,
	   TC_SAT_SWARM_C     = 103,
	   TC_SAT_SENTINEL_1A = 110,
	   TC_SAT_SENTINEL_1B = 111,
	   TC_SAT_SENTINEL_2  = 112,
	   TC_SAT_SENTINEL_3  = 113,
	   TC_SAT_SEOSAT      = 120,
	   TC_SAT_SENTINEL_1C = 125,
	   TC_SAT_SENTINEL_2A = 126,
	   TC_SAT_SENTINEL_2B = 127,
	   TC_SAT_SENTINEL_2C = 128,
	   TC_SAT_SENTINEL_3A = 129,
	   TC_SAT_SENTINEL_3B = 130,
	   TC_SAT_SENTINEL_3C = 131,
	   TC_SAT_JASON_CSA	  = 132, // AN-531 
	   TC_SAT_JASON_CSB   = 133, // AN-531  
	   TC_SAT_METOP_SG_A1 = 134, // AN-530 
	   TC_SAT_METOP_SG_A2 = 135, // AN-530 
	   TC_SAT_METOP_SG_A3 = 136, // AN-530 
	   TC_SAT_METOP_SG_B1 = 137, // AN-530 
	   TC_SAT_METOP_SG_B2 = 138, // AN-530 
	   TC_SAT_METOP_SG_B3 = 139, // AN-530 
	   TC_SAT_SENTINEL_5P = 140, // AN-502   
	   TC_SAT_BIOMASS     = 141, // AN-601   
	   TC_SAT_SENTINEL_5  = 142, // AN-639   
	   TC_SAT_SAOCOM_CS   = 143, // AN-640   
	   TC_SAT_FLEX        = 144, /* AN-683 */
	   TC_SAT_GENERIC     = 200,
	   TC_SAT_GENERIC_GEO = 300, /* ANR-353 */
	   TC_SAT_MTG         = 301, /* ANR-353 */
	   TC_SAT_GENERIC_MEO = 400  /* ANR-550 */

	} ;
};

std::ostream &operator<< (std::ostream &, TimeFormat::SatelliteID) ;
exostream &operator<< (exostream &, TimeFormat::SatelliteID) ; // PRQA S 2072

std::ostream &operator<< (std::ostream &, TimeFormat::TimeModel) ;
exostream &operator<< (exostream &, TimeFormat::TimeModel) ; // PRQA S 2072

std::ostream &operator<< (std::ostream &, TimeFormat::TimeReference) ;
exostream &operator<< (exostream &, TimeFormat::TimeReference) ; // PRQA S 2072

_ACS_END_NAMESPACE

#endif // _TimeFormat_H_
