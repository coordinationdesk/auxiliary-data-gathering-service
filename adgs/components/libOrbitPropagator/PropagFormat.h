// PRQA S 1050 EOF
/*

	Copyright 1995-2013, Advanced Computer Systems , Inc.
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
	Revision 2.17  2016/06/01 07:30:35  giucas
	Added CSG satellites
	
	Revision 2.16  2015/11/30 08:57:10  giucas
	SatelliteID updated
	
	Revision 2.15  2014/03/27 12:46:46  marpas
	coding best practices applied
	interface rationalization
	warnings fixed (qa & g++)
	
	Revision 2.14  2014/03/20 11:50:54  marpas
	stream inserter operators added
	
	Revision 2.13  2013/11/14 14:42:58  marpas
	coding best practices applied
	
	Revision 2.12  2011/09/08 07:07:06  giucas
	Enums updated
	
	Revision 2.11  2011/04/15 12:40:56  giucas
	Typo fixed
	
	Revision 2.10  2011/03/30 10:17:06  giucas
	Typo fixed
	
	Revision 2.9  2011/03/30 10:15:17  giucas
	New satellites added
	
	Revision 2.8  2009/11/19 16:18:09  giucas
	Added OS2 satellite.
	
	Revision 2.7  2009/10/14 15:22:34  giucas
	SAC-D added.
	
	Revision 2.6  2009/02/25 15:16:09  giucas
	Added RSAT satellite
	
	Revision 2.5  2008/12/09 17:50:21  giucas
	ALOS added
	
	Revision 2.4  2006/11/06 14:36:03  giucas
	Added OP_TERRESTRIAL in RefCoordinateFrames enum
	
	Revision 2.3  2006/10/24 15:27:27  giucas
	Added EOSAM and EOSPM Satellite IDs
	
	Revision 2.2  2006/09/13 13:54:58  crivig
	forced number to enum structure
	
	Revision 2.1  2006/05/09 17:46:03  giucas
	Added OP_TIMEMOD_STDM in TimeModel enum
	
	
*/


#ifndef _PropagFormat_H_
#define _PropagFormat_H_ 


#include <acs_c++config.hpp>
#include <exStream.h>

_ACS_BEGIN_NAMESPACE(acs)
	
class PropagFormat 
{
public:

	// Kepler state vector mode
	enum KeplerStateVectorMode
	{

	  OP_KEPLER_MEAN = 1,
	  OP_KEPLER_OSC

	} ;



	// AOCS mode
	enum AocsMode
	{

	  OP_AOCS_DEFAULT = 0,
	  OP_AOCS_USER,
	  OP_AOCS_GPM,
	  OP_AOCS_LNP,
	  OP_AOCS_YSM

	} ;



	// Calculation mode
	enum CalculationMode
	{

	  OP_CALC_POS = 1,
	  OP_CALC_POS_VEL,
	  OP_CALC_POS_VEL_ACC

	} ;


	// Time Initialization mode
	enum TimeInitMode
	{

	  OP_SEL_FILE = 0,
	  OP_SEL_TIME = 1,
	  OP_SEL_ORBIT

	} ;

	// Time reference ID
	enum TimeReference
	{

	  OP_TIME_UNDEF = -1,
	  OP_TIME_TAI,
	  OP_TIME_UTC,
	  OP_TIME_UT1,
	  OP_TIME_GPS

	} ;

	enum NoSunSychronousMode{

	  OP_NOSUNSYNC_DRIFT = 0,
	  OP_NOSUNSYNC_INCLINATION

	};


	//Propagation models
	enum PropagationModel{

	  OP_PROPAG_MODEL_MEAN_KEPL  = 0, 	// Propagation using Mean Keplerian OSV model
	  OP_PROPAG_MODEL_SPOT		 = 1, 	// Propagation using Spot model
	  OP_PROPAG_MODEL_AUTO       = 10,  // Propagation using Auto initialization model
	  OP_PROPAG_MODEL_DOUBLE     = 100  // Propagation using double initialization model
	};

	//extra choice modes
	enum PropagExtraChoiceModes
	{

	  // model independant
	  OP_PROPAG_EXTRA_NO_RESULTS = 0,            // To calculate no extra results  
	  OP_PROPAG_EXTRA_GEOLOCATION = 1,           // To calculate the geolocation data set  
	  OP_PROPAG_EXTRA_GEOLOCATION_D = 2,         // To calculate the geolocation rate data set  
	  OP_PROPAG_EXTRA_GEOLOCATION_2D = 4,        // To calculate the geolocation rate rate data  set  
	  OP_PROPAG_EXTRA_GEOLOCATION_EXTRA = 8,     // To calculate the geolocation extra data set  
	  OP_PROPAG_EXTRA_EARTH_FIXED_D = 16,        // To calculate the earth-fixed velocity data set  
	  OP_PROPAG_EXTRA_EARTH_FIXED_2D = 32,       // To calculate the earth-fixed acceleration data set  
	  OP_PROPAG_EXTRA_SUN = 64,                  // To calculate the sun data set  
	  OP_PROPAG_EXTRA_MOON = 128,                // To calculate the moon data set  
	  OP_PROPAG_EXTRA_OSCULATING_KEPLER = 256,   // To calculate the osculating Kepler data set  
	  OP_PROPAG_EXTRA_INERTIAL_AUX = 512,        // To calculate the inertial auxiliary data set  
	  // model dependant  
	  OP_PROPAG_EXTRA_DEP_ANX_TIMING = 1024,	 // To calculate ANX timing data set  
	  OP_PROPAG_EXTRA_DEP_MEAN_KEPLER = 2048,	 // To calculate the mean keplerian elements  
	  OP_PROPAG_EXTRA_ALL_RESULTS = 4095         // To calculate all extra results  

	};


	/* xo_propag_extra MKO model dependant results vector */
	enum PropagExtraInfoResults
	{

	  OP_ORBIT_INFO_EXTRA_REPEAT_CYCLE = 0,
	  OP_ORBIT_INFO_EXTRA_CYCLE_LENGTH,
	  OP_ORBIT_INFO_EXTRA_MLST_DRIFT,
	  OP_ORBIT_INFO_EXTRA_MLST,
	  OP_ORBIT_INFO_EXTRA_ANX_LONG,
	  OP_ORBIT_INFO_EXTRA_UTC_ANX,
	  OP_ORBIT_INFO_EXTRA_POS_X,
	  OP_ORBIT_INFO_EXTRA_POS_Y,
	  OP_ORBIT_INFO_EXTRA_POS_Z,
	  OP_ORBIT_INFO_EXTRA_VEL_X,
	  OP_ORBIT_INFO_EXTRA_VEL_Y,
	  OP_ORBIT_INFO_EXTRA_VEL_Z,
	  OP_ORBIT_INFO_EXTRA_MEAN_KEPL_A,
	  OP_ORBIT_INFO_EXTRA_MEAN_KEPL_E,
	  OP_ORBIT_INFO_EXTRA_MEAN_KEPL_I,
	  OP_ORBIT_INFO_EXTRA_MEAN_KEPL_RA,
	  OP_ORBIT_INFO_EXTRA_MEAN_KEPL_W,
	  OP_ORBIT_INFO_EXTRA_MEAN_KEPL_M,
	  OP_ORBIT_INFO_EXTRA_OSC_KEPL_A,
	  OP_ORBIT_INFO_EXTRA_OSC_KEPL_E,
	  OP_ORBIT_INFO_EXTRA_OSC_KEPL_I,
	  OP_ORBIT_INFO_EXTRA_OSC_KEPL_RA,
	  OP_ORBIT_INFO_EXTRA_OSC_KEPL_W,
	  OP_ORBIT_INFO_EXTRA_OSC_KEPL_M,
	  OP_ORBIT_INFO_EXTRA_NODAL_PERIOD,
	  OP_ORBIT_INFO_EXTRA_NUM_ELEMENTS

	};


	//MKO model dependant results vector 
	enum PropagExtraMKOModelDependant
	{

	  OP_PROPAG_EXTRA_DEP_NODAL_PERIOD = 0,
	  OP_PROPAG_EXTRA_DEP_UTC_CURRENT_ANX,
	  OP_PROPAG_EXTRA_DEP_ORBIT_NUMBER,		//added for CFI release 2.3.1
	  OP_PROPAG_EXTRA_DEP_SEC_SINCE_ANX,	//added for CFI release 2.3.1
	  OP_PROPAG_EXTRA_DEP_MEAN_KEPL_A,
	  OP_PROPAG_EXTRA_DEP_MEAN_KEPL_E,
	  OP_PROPAG_EXTRA_DEP_MEAN_KEPL_I,
	  OP_PROPAG_EXTRA_DEP_MEAN_KEPL_RA,
	  OP_PROPAG_EXTRA_DEP_MEAN_KEPL_W,
	  OP_PROPAG_EXTRA_DEP_MEAN_KEPL_M,
	  OP_PROPAG_EXTRA_NUM_DEP_ELEMENTS

	};

	//model independant results vector
	enum PropagExtraModelIndependant
	{

	  OP_PROPAG_EXTRA_GEOC_LONG = 0,
	  OP_PROPAG_EXTRA_GEOD_LAT,
	  OP_PROPAG_EXTRA_GEOD_ALT,
	  OP_PROPAG_EXTRA_GEOC_LONG_D,
	  OP_PROPAG_EXTRA_GEOD_LAT_D,
	  OP_PROPAG_EXTRA_GEOD_ALT_D,
	  OP_PROPAG_EXTRA_GEOC_LONG_2D,
	  OP_PROPAG_EXTRA_GEOD_LAT_2D,
	  OP_PROPAG_EXTRA_GEOD_ALT_2D,
	  OP_PROPAG_EXTRA_RAD_CUR_PARALLEL_MERIDIAN,
	  OP_PROPAG_EXTRA_RAD_CUR_ORTHO_MERIDIAN,
	  OP_PROPAG_EXTRA_RAD_CUR_ALONG_GROUNDTRACK,
	  OP_PROPAG_EXTRA_NORTH_VEL,
	  OP_PROPAG_EXTRA_EAST_VEL,
	  OP_PROPAG_EXTRA_MAG_VEL,
	  OP_PROPAG_EXTRA_AZ_VEL,
	  OP_PROPAG_EXTRA_NORTH_ACC,
	  OP_PROPAG_EXTRA_EAST_ACC,
	  OP_PROPAG_EXTRA_GROUNDTRACK_ACC,
	  OP_PROPAG_EXTRA_AZ_ACC,
	  OP_PROPAG_EXTRA_SAT_ECLIPSE_FLAG,
	  OP_PROPAG_EXTRA_SZA,
	  OP_PROPAG_EXTRA_MLST,
	  OP_PROPAG_EXTRA_TLST,
	  OP_PROPAG_EXTRA_TRUE_SUN_RA,
	  OP_PROPAG_EXTRA_TRUE_SUN_DEC,
	  OP_PROPAG_EXTRA_TRUE_SUN_SEMIDIAM,
	  OP_PROPAG_EXTRA_MOON_RA,
	  OP_PROPAG_EXTRA_MOON_DEC,
	  OP_PROPAG_EXTRA_MOON_SEMI_DIAM,
	  OP_PROPAG_EXTRA_MOON_AREA_LIT,
	  OP_PROPAG_EXTRA_OSC_KEPL_A,
	  OP_PROPAG_EXTRA_OSC_KEPL_E,
	  OP_PROPAG_EXTRA_OSC_KEPL_I,
	  OP_PROPAG_EXTRA_OSC_KEPL_RA,
	  OP_PROPAG_EXTRA_OSC_KEPL_W,
	  OP_PROPAG_EXTRA_OSC_KEPL_M,
	  OP_PROPAG_EXTRA_ORBIT_RAD,
	  OP_PROPAG_EXTRA_RADIAL_ORB_VEL,
	  OP_PROPAG_EXTRA_TRANS_ORB_VEL,
	  OP_PROPAG_EXTRA_ORB_VEL_MAG,
	  OP_PROPAG_EXTRA_RA_SAT,
	  OP_PROPAG_EXTRA_DEC_SAT,
	  OP_PROPAG_EXTRA_EARTH_ROTATION_ANGLE,
	  OP_PROPAG_EXTRA_RA_SAT_D,
	  OP_PROPAG_EXTRA_RA_SAT_2D,
	  OP_PROPAG_EXTRA_OSC_TRUE_LAT,
	  OP_PROPAG_EXTRA_OSC_TRUE_LAT_D,
	  OP_PROPAG_EXTRA_OSC_TRUE_LAT_2D,
	  OP_PROPAG_EXTRA_NUM_INDEP_ELEMENTS

	};



	// Interpolation model
	enum InterpolationModel{

	  OP_INTERPOL_MODEL_DEFAULT = 0

	} ;

	// xo_interpol_extra extra_choice modes  
	enum InterpolExtraChoiceModes
	{
	  // model independant 
	  OP_INTERPOL_EXTRA_NO_RESULTS = 0,            // To calculate no extra results  
	  OP_INTERPOL_EXTRA_GEOLOCATION = 1,           // To calculate the geolocation data set  
	  OP_INTERPOL_EXTRA_GEOLOCATION_D = 2,         // To calculate the geolocation rate data set  
	  OP_INTERPOL_EXTRA_GEOLOCATION_2D = 4,        // To calculate the geolocation rate rate data  set  
	  OP_INTERPOL_EXTRA_GEOLOCATION_EXTRA = 8,     // To calculate the geolocation extra data set  
	  OP_INTERPOL_EXTRA_EARTH_FIXED_D = 16,        // To calculate the earth-fixed velocity data set  
	  OP_INTERPOL_EXTRA_EARTH_FIXED_2D = 32,       // To calculate the earth-fixed acceleration data set  
	  OP_INTERPOL_EXTRA_SUN = 64,                  // To calculate the sun data set  
	  OP_INTERPOL_EXTRA_MOON = 128,                // To calculate the moon data set  
	  OP_INTERPOL_EXTRA_OSCULATING_KEPLER = 256,   // To calculate the osculating Kepler data set  
	  OP_INTERPOL_EXTRA_INERTIAL_AUX = 512,        // To calculate the inertial auxiliary data set  
	  //model dependant  
	  OP_INTERPOL_EXTRA_DEP_ANX_TIMING = 1024,	   // To calculate ANX timing data set  
	  OP_INTERPOL_EXTRA_ALL_RESULTS = 2047         // To calculate all extra results  

	};

	//Interpolation extra MKO model dependant results vector  
	enum InterpolExtraMKOModelDependant
	{
	  OP_INTERPOL_EXTRA_DEP_NODAL_PERIOD = 0,
	  OP_INTERPOL_EXTRA_DEP_UOP_CURRENT_ANX,
	  OP_INTERPOL_EXTRA_DEP_ORBIT_NUMBER,			//added for CFI release 2.3.1
	  OP_INTERPOL_EXTRA_DEP_SEC_SINCE_ANX,			//added for CFI release 2.3.1
	  OP_INTERPOL_EXTRA_NUM_DEP_ELEMENTS

	};


	//Interpolation extra model independant results vector  
	enum InterpolExtraModelIndependant
	{
	  OP_INTERPOL_EXTRA_GEOC_LONG = 0,
	  OP_INTERPOL_EXTRA_GEOD_LAT,
	  OP_INTERPOL_EXTRA_GEOD_ALT,
	  OP_INTERPOL_EXTRA_GEOC_LONG_D,
	  OP_INTERPOL_EXTRA_GEOD_LAT_D,
	  OP_INTERPOL_EXTRA_GEOD_ALT_D,
	  OP_INTERPOL_EXTRA_GEOC_LONG_2D,
	  OP_INTERPOL_EXTRA_GEOD_LAT_2D,
	  OP_INTERPOL_EXTRA_GEOD_ALT_2D,
	  OP_INTERPOL_EXTRA_RAD_CUR_PARALLEL_MERIDIAN,
	  OP_INTERPOL_EXTRA_RAD_CUR_ORTHO_MERIDIAN,
	  OP_INTERPOL_EXTRA_RAD_CUR_ALONG_GROUNDTRACK,
	  OP_INTERPOL_EXTRA_NORTH_VEL,
	  OP_INTERPOL_EXTRA_EAST_VEL,
	  OP_INTERPOL_EXTRA_MAG_VEL,
	  OP_INTERPOL_EXTRA_AZ_VEL,
	  OP_INTERPOL_EXTRA_NORTH_ACC,
	  OP_INTERPOL_EXTRA_EAST_ACC,
	  OP_INTERPOL_EXTRA_GROUNDTRACK_ACC,
	  OP_INTERPOL_EXTRA_AZ_ACC,
	  OP_INTERPOL_EXTRA_SAT_ECLIPSE_FLAG,
	  OP_INTERPOL_EXTRA_SZA,
	  OP_INTERPOL_EXTRA_MLST,
	  OP_INTERPOL_EXTRA_TLST,
	  OP_INTERPOL_EXTRA_TRUE_SUN_RA,
	  OP_INTERPOL_EXTRA_TRUE_SUN_DEC,
	  OP_INTERPOL_EXTRA_TRUE_SUN_SEMIDIAM,
	  OP_INTERPOL_EXTRA_MOON_RA,
	  OP_INTERPOL_EXTRA_MOON_DEC,
	  OP_INTERPOL_EXTRA_MOON_SEMI_DIAM,
	  OP_INTERPOL_EXTRA_MOON_AREA_LIT,
	  OP_INTERPOL_EXTRA_OSC_KEPL_A,
	  OP_INTERPOL_EXTRA_OSC_KEPL_E,
	  OP_INTERPOL_EXTRA_OSC_KEPL_I,
	  OP_INTERPOL_EXTRA_OSC_KEPL_RA,
	  OP_INTERPOL_EXTRA_OSC_KEPL_W,
	  OP_INTERPOL_EXTRA_OSC_KEPL_M,
	  OP_INTERPOL_EXTRA_ORBIT_RAD,
	  OP_INTERPOL_EXTRA_RADIAL_ORB_VEL,
	  OP_INTERPOL_EXTRA_TRANS_ORB_VEL,
	  OP_INTERPOL_EXTRA_ORB_VEL_MAG,
	  OP_INTERPOL_EXTRA_RA_SAT,
	  OP_INTERPOL_EXTRA_DEC_SAT,
	  OP_INTERPOL_EXTRA_EARTH_ROTATION_ANGLE,
	  OP_INTERPOL_EXTRA_RA_SAT_D,
	  OP_INTERPOL_EXTRA_RA_SAT_2D,
	  OP_INTERPOL_EXTRA_OSC_TRUE_LAT,
	  OP_INTERPOL_EXTRA_OSC_TRUE_LAT_D,
	  OP_INTERPOL_EXTRA_OSC_TRUE_LAT_2D,
	  OP_INTERPOL_EXTRA_NUM_INDEP_ELEMENTS

	};


	// Time Model
	enum TimeModel
	{
	  OP_TIMEMOD_STDM = -4, 			//ACS: NOT CFI EXPLORER
	  OP_TIMEMOD_TLE  = -3, 			//ACS: NOT CFI EXPLORER
	  OP_TIMEMOD_AUTO = -2,
	  OP_TIMEMOD_USER = -1,
	  OP_TIMEMOD_NONE = 0,
	  OP_TIMEMOD_IERS_B_PREDICTED = 1,
	  OP_TIMEMOD_IERS_B_RESTITUTED,
	  OP_TIMEMOD_FOS_PREDICTED,
	  OP_TIMEMOD_FOS_RESTITUTED,
	  OP_TIMEMOD_DORIS_PRELIMINARY,
	  OP_TIMEMOD_DORIS_PRECISE,
	  OP_TIMEMOD_DORIS_NAVIGATOR,
	  OP_TIMEMOD_OSF				    // Orbit scenario file

	} ;


	// Reference coordinate frames
	enum RefCoordinateFrames
	{
	  OP_GCRS = -2,
	  OP_TERRESTRIAL	= -1,		//SPOT Family coordinate system
	  OP_REFCOORD_UNDEF = 0,			//Not defined in Explorer Orbit Library
	  OP_BM2000 = 1,                // Barycentric Mean of 2000.0 coordinate system  
	  OP_HM2000 = 2,                   // Heliocentric Mean of 2000.0 coordinate system  
	  OP_GM2000 = 3,                    // Geocentric Mean of 2000.0 coordinate system  
	  OP_MOD = 4,                       // Mean of Date coordinate system  
	  OP_TOD = 5,                       // True of Date coordinate system  
	  OP_EF = 6                         // Earth Fixed coordinate system  

	} ;


	// Planets
	enum Planets
	{

	  OP_MERCURY = 1,               // Mercury  
	  OP_VENUS,                     // Venus  
	  OP_EM_BAR,                    // Earth-Moon barycenter  
	  OP_MARS,                      // Mars  
	  OP_JUPITER,                   // Jupiter  
	  OP_SATURN,                    // Saturn  
	  OP_URANUS,                    // Uranus  
	  OP_NEPTUNE                    // Neptune  

	} ;


	//Derivatives
	//Indicate whether the 1st and 2nd derivatives of data
	//structure are defined or control whether a function
	//calculates the 1st and 2nd derivatives 
	enum  Derivatives				
	{
	  OP_NO_DER = 0,    // No derivative is defined or calculated  
	  OP_DER_1ST,       // Only first derivative is defined or calculated  
	  OP_DER_2ND        // Both 1st and 2nd derivatives are defined or calculated  

	} ;


	//Satellite ID
	enum SatelliteID
	{
	   OP_SAT_CSGS2	  = -16,
	   OP_SAT_CSGS1	  = -15,
	   OP_SAT_OS2	  = -14,
	   OP_SAT_SACD	  = -13,
	   OP_SAT_RSAT	  = -12,
	   OP_SAT_ALOS	  = -11,
	   OP_SAT_EOSPM	  = -10,
	   OP_SAT_EOSAM	  = -9,
	   OP_SAT_CSKS8	  = -8,
	   OP_SAT_CSKS7	  = -7,
	   OP_SAT_CSKS6	  = -6,		   
	   OP_SAT_CSKS5	  = -5,
	   OP_SAT_CSKS4	  = -4,
	   OP_SAT_CSKS3	  = -3,
	   OP_SAT_CSKS2	  = -2,
	   OP_SAT_CSKS1	  = -1,
	   OP_SAT_DEFAULT = 0,
	   OP_SAT_ERS1    = 11,
	   OP_SAT_ERS2    = 12,
	   OP_SAT_ENVISAT = 21,
	   OP_SAT_METOP1  = 31,
	   OP_SAT_METOP2  = 32,
	   OP_SAT_METOP3  = 33,
	   OP_SAT_CRYOSAT = 41,
	   OP_SAT_ADM     = 51,
	   OP_SAT_GOCE    = 61,
	   OP_SAT_SMOS    = 71,
	   OP_SAT_TERRASAR    = 81,
	   OP_SAT_EARTHCARE   = 91,
	   OP_SAT_SWARM_A     = 101,
	   OP_SAT_SWARM_B     = 102,
	   OP_SAT_SWARM_C     = 103,
	   OP_SAT_SENTINEL_1A = 110,
	   OP_SAT_SENTINEL_1B = 111,
	   OP_SAT_SENTINEL_2  = 112,
	   OP_SAT_SENTINEL_3  = 113,
	   OP_SAT_SEOSAT      = 120,
	   OP_SAT_SENTINEL_1C = 125,
	   OP_SAT_SENTINEL_2A = 126,
	   OP_SAT_SENTINEL_2B = 127,
	   OP_SAT_SENTINEL_2C = 128,
	   OP_SAT_SENTINEL_3A = 129,
	   OP_SAT_SENTINEL_3B = 130,
	   OP_SAT_SENTINEL_3C = 131,
	   OP_SAT_JASON_CSA	  = 132, // AN-531 
	   OP_SAT_JASON_CSB   = 133, // AN-531  
	   OP_SAT_METOP_SG_A1 = 134, // AN-530 
	   OP_SAT_METOP_SG_A2 = 135, // AN-530 
	   OP_SAT_METOP_SG_A3 = 136, // AN-530 
	   OP_SAT_METOP_SG_B1 = 137, // AN-530 
	   OP_SAT_METOP_SG_B2 = 138, // AN-530 
	   OP_SAT_METOP_SG_B3 = 139, // AN-530 
	   OP_SAT_SENTINEL_5P = 140, // AN-502   
	   OP_SAT_BIOMASS     = 141, // AN-601   
	   OP_SAT_SENTINEL_5  = 142, // AN-639   
	   OP_SAT_SAOCOM_CS   = 143, // AN-640   
	   OP_SAT_FLEX        = 144, /* AN-683 */
	   OP_SAT_GENERIC     = 200,
	   OP_SAT_GENERIC_GEO = 300, /* ANR-353 */
	   OP_SAT_MTG         = 301, /* ANR-353 */
	   OP_SAT_GENERIC_MEO = 400  /* ANR-550 */
	} ;


};


std::ostream &operator<< (std::ostream &, PropagFormat::SatelliteID) ;
exostream &operator<< (exostream &, PropagFormat::SatelliteID) ; // PRQA S 2072

std::ostream &operator<< (std::ostream &, PropagFormat::TimeModel) ;
exostream &operator<< (exostream &, PropagFormat::TimeModel) ; // PRQA S 2072

std::ostream &operator<< (std::ostream &, PropagFormat::TimeReference) ;
exostream &operator<< (exostream &, PropagFormat::TimeReference) ; // PRQA S 2072

std::ostream &operator<< (std::ostream &, PropagFormat::TimeInitMode) ;
exostream &operator<< (exostream &, PropagFormat::TimeInitMode) ; // PRQA S 2072

std::ostream &operator<< (std::ostream &, PropagFormat::RefCoordinateFrames) ;
exostream &operator<< (exostream &, PropagFormat::RefCoordinateFrames) ; // PRQA S 2072

_ACS_END_NAMESPACE

#endif // _PropagFormat_H_
