
/*

	Copyright 2013-2018, Advanced Computer Systems , Inc.
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
	Revision 1.2  2013/02/26 17:02:45  ricfer
	mssConstants.h included. MSS_CALWEDGE_PARAMETERS, MSS_CROSS_CAL and MSS_CAL_PARAMETERS structures added.
	
	Revision 1.1  2013/01/16 15:07:28  ricfer
	First add.
	
	Revision 1.1  2013/01/16 16:04:41  ricfer
	Added this script to add ACS header to all source/header/makefile files.
	

*/


/* +----------------------------------------------------------------------------------------------------+ */	 
/* !   This controls avoids the redefinition of the following code.					! */
/* +----------------------------------------------------------------------------------------------------+ */
#ifndef _mssCPFTypes_h_
#define _mssCPFTypes_h_

/* +--------------------------------------------------------------------------------------------------------+ */	 
/* !   STRUCT DEFINITION																				    ! */
/* +--------------------------------------------------------------------------------------------------------+ */

#include <mssConstants.h>

# define MSS_NUM_BANDS		MSS_NUM_OF_BANDS
# define MSS_NUM_DETECTORS	MSS_NUM_OF_DET

typedef struct {
	int Number_Mirr_Coef; // = 3
	double Time_Mid_Scan; // 0.016035
	double Mirr_First_Half_Coef[3]; // (-76.976,224.763,-141.956)
	double Mirr_Second_Half_Coef[3]; // (-76.976,224.763,-141.956)
} MSS_MIRROR_PARAMETERS;

typedef struct { 
  int		WRS_Cycle_Days; // = 16
  int		WRS_Cycle_Orbits; // = 233
  int		Scenes_Per_Orbit; // = 248
  double	Orbital_Period; // = 5933.0472
  double	Angular_Momentum; // = 53.136250E9
  double	Orbit_Radius; // = 7083.4457
  double	Orbit_Semimajor_Axis; // = 7083.4457
  double	Orbit_Semiminor_Axis; // = 7083.4408
  double	Orbit_Eccentricity; // = 0.00117604
  double	Inclination_Angle; // = 98.2096
  double	Argument_Of_Perigee; // = 90.0
  int		Descending_Node_Row; // = 60
  double	Long_Path1_Row60; // = -64.6
  double	Descending_Node_Time_Min; // = "09:45"
  double	Descending_Node_Time_Max; // = "10:15"
  double	Nodal_Regression_Rate; // = 0.985647366
} MSS_ORBIT_PARAMETERS;

//scanner parameters
typedef struct {
	int	Lines_Per_Scan_60; // = 6
  	int	Scans_Per_Scene; //  = 390
	double	Swath_Angle; //  = 0.2007
	double	Scan_Rate; //  = 9.958e-6
	double	Dwell_Time_60; //  = 9.958e-6
	int	IC_Line_Length_60; //  = 100
	int	Scan_Line_Length_60; //  = 3240
	double	Filter_Frequency_60; //  = 0.0
	double	IFOV_B1234; //  = 86.0e-6
	double	IFOV_B1234_ALONG; //  = 86.0e-6
	double	IFOV_B1234_ACROSS; //  = 86.0e-6
	double	Scan_Period; //  = 0.07342
	double	Scan_Frequency; //  = 13.62
	double	Active_Scan_Time; //  = 0.032130
	double	Turn_Around_Time;
} MSS_SCANNER_PARAMETERS;

typedef struct {
	int Image_Center_Line; // 1170.0  						
    int Image_Center_Sample; // 1620.0						
    int Image_Center_Scan; // 195.0							
	int Number_Channels; // 25								
    double Image_Line_Rate; // 81.72160174						
    double Image_Yaw; // -0.000584133							
    double Image_Sample_Slope; // 16039.2641185  				
    int Image_Frame_Size; // 268000.0 						
    double Xcorrect_Delays[MSS_NUM_DETECTORS]; // (0.20,0.12,0.04,-0.04,-0.12,-0.20)
} MSS_PROC_CONSTANTS;

typedef struct {
	double Along_Scan_DO[MSS_NUM_BANDS][MSS_NUM_DETECTORS];	//(IFOV) detector offset
    double Across_Scan_DO[MSS_NUM_BANDS][MSS_NUM_DETECTORS];  //(IFOV)
	double Along_Scan_Band_Offsets [MSS_NUM_BANDS];							//(microrad) band offset
    double Across_Scan_Band_Offsets[MSS_NUM_BANDS];							//(microrad)
    double Focal_Plane_Offsets[2][MSS_NUM_BANDS];				//(IFOV)
} MSS_FOCAL_PLANE_PARAMETERS;

typedef struct {
 	double Gyro_To_Attitude_Matrix[9];
  	double Attitude_To_MSS_Matrix[9];
  	double Spacecraft_Roll_Bias;
  	double Spacecraft_Pitch_Bias;
  	double Spacecraft_Yaw_Bias; 
} MSS_ATTITUDE_PARAMETERS;

typedef struct {
	double			C[MSS_NUM_OF_BANDS][MSS_NUM_OF_DET][MSS_NUM_OF_CAL_WEDGE];
	double			D[MSS_NUM_OF_BANDS][MSS_NUM_OF_DET][MSS_NUM_OF_CAL_WEDGE];
	double			Bias_modifier[MSS_NUM_OF_BANDS][MSS_NUM_OF_DET];
	double			Gain_modifier[MSS_NUM_OF_BANDS][MSS_NUM_OF_DET];
	unsigned char	decompressTab[MSS_NUM_OF_BANDS][MSS_Q_COMPR_MAX+1];
} MSS_CALWEDGE_PARAMETERS;

typedef struct {
	double	G;
	double	B;
	double	TDF_A;
	double	TDF_B;
	double	TDF_C;
} MSS_CROSS_CAL;

typedef struct {
	double					launchDate;
	MSS_CALWEDGE_PARAMETERS wedge;
	MSS_CROSS_CAL			xCal[MSS_NUM_OF_BANDS];
	double					absCal[MSS_NUM_OF_BANDS];
} MSS_CAL_PARAMETERS;

#endif

