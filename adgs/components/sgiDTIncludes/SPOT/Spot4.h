/*

	Copyright 1995-2006, Advanced Computer Systems , Inc.
	Via Della Bufalotta, 378 - 00139 Roma - Italy
	http://www.acsys.it

	All Rights Reserved.

	This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems;
	the contents of this file may not be disclosed to third parties, copied or
	duplicated in any form, in whole or in part, without the prior written
	permission of Advanced Computer Systems, Inc.

	$Prod: A.C.S. Spot4.h $

	$Id$

	$Author$

	$Log$
	Revision 1.3  2006/08/29 17:09:12  enrcar
	Include added
	
	Revision 1.2  2006/08/29 12:31:22  enrcar
	header guard renamed
	
	Revision 1.1  2006/08/28 16:04:04  enrcar
	Spot4.h file added
	

*/


#ifndef __SPOT4_H__
#define __SPOT4_H__

#include <SPOT/Spot.h>


typedef struct {
	unsigned int	day;			/* day number of year */
	char			mission[3];		/* mission number */
	unsigned int	orbit;			/* orbit number */
	float			begin_lat;		/* latitude of beginning */
	float			end_lat;		/* latitude of end */
} Sequence4PlanSegment;

typedef struct {
 	Boolean					filled;				/* reserved */
	unsigned int			segments_nb;		/* number of segments */
	Sequence4PlanSegment	segments[50];		/* The structure Sequence4PlanSegment
													is described after */
} Sequence4Plan;								/* 00 */ /* 01 */

typedef struct {
	unsigned int	day;			/* day number of year */
	char			mission[3];		/* mission number */
	unsigned int	orbit;			/* orbit number */
	char			encoding;		/*	D = default encoding
										C = no encoded (clear)
										T = test encoding */
	float			begin_lat;		/* latitude of beginning */
	float			end_lat;		/* latitude of end */
	double			begin_sec;		/* seconds of the beginning */
	double			end_sec;		/* seconds of the end */
} Resp4ToReservSlot;

typedef struct {
	Boolean				filled;		/* reserved */
	unsigned int		nb_slots;	/* number of slots filled */
	Resp4ToReservSlot	slots[50];	/* The structure Resp4ToReservSlot is described after */
} Resp4ToReserv; 					/* 02 */

typedef enum {
	COUPLING_INDEPENDENT = 10, 	/* BI */
	COUPLING_DOUBLE = 20,		/* DO */
	COUPLING_TWIN = 30			/* TW */
} Coupling_Value ; 

typedef enum {
	HRVIR_H0 = 10,				/* H0 */
	HRVIR_H1 = 20,				/* H1 */
	HRVIR_H2 = 30				/* H2 */
} HRVIR_Value ; 

typedef enum {
	SPECTRAL_M = 10,			/* M  */
	SPECTRAL_MI = 20,			/* MI */
	SPECTRAL_XI = 30			/* XI */
} Spectral_Value ; 
	
typedef struct {
	Coupling_Value	coupling_mode;		/*	Coupling mode of segments:
											10 = BI (Independent)
											20 = DO (Double)
											30 = TW (Twin)	*/
	HRVIR_Value		HRVIR_instrument; 	/*	Instrument used:
											10 = H0 (when coupling TW) 
											20 = H1	
											30 = H2					*/
	Spectral_Value	spectral_mode;		/*	Instrument spectral mode:
											10 = M
											20 = MI	 (when coupling DO)
											30 = XI					*/
	float			center_lon;			/* longitude of center */
 	float			center_lat;			/* latitude of center */
	char			Xi_gains[4];		/* gains for multi-spectral case:
											H	= high
											M	= medium
											L	= low (default)
											A value from 1 to 6 		*/
	char			M_gain[4];			/* gain for panchromatic case */
	char			Xi_gains_2[4];		/* gains for multi-spectral case (when coupling TW) */
	char			M_gain_2[4];		/* gain for panchromatic case (when coupling TW) */
} Spot4Segment; 							

typedef struct {
	Boolean			filled;				/* reserved */
	unsigned int	day;				/* day number of year */
	char			mission[3];			/* mission number */
	unsigned int	orbit;				/* orbit number */
	unsigned int	segments_nb;		/* number of segments */
	Spot4Segment	segments[50];		/* The structure Spot4Segment is described after */
} Prog4Description;						/* 03 */

typedef struct {
	Coupling_Value	coupling_mode;	/*	Coupling mode of segments:
										10 = BI (Independent)
										20 = DO (Double)
										30 = TW (Twin)	*/
	HRVIR_Value		HRVIR_instrument; 	/*	Instrument used:
											10 = H0 (when coupling is TW)
											20 = H1	
											30 = H2					*/
	Spectral_Value	spectral_mode;		/*	Instrument spectral mode:
											10 = M
											20 = MI	 (when coupling is DO)
											30 = XI					*/
	float	begin_lon;					/* longitude of the beginning */
	float	begin_lat;					/* latitude of the beginning */
	float	end_lon;					/* longitude of the end */
	float	end_lat;					/* latitude of the end */
	float	angle;						/* viewing angle */
} Spot4Swath;

typedef enum {
	DIAG_ACCEPTED = 10,
	DIAG_CENTERING_POINT_UNACCETTABLE = 20,
	DIAG_TOO_MANY_SEGMENT = 30,
	DIAG_SPECULAR_REFLECTION = 40, 
	DIAG_CENTERING_POINT_OUTSIDE_RESERVED_SLOT = 50, 
	DIAG_SEGMENT_TOO_SHORT = 60, 
	DIAG_WRONG_PAYLOAD = 70
} Diagnostic_Values ; 

typedef struct {
	Boolean			filled;			/* reserved */
 	unsigned int	day;			/* day number of year */
 	char			mission[3];		/* mission number */
 	unsigned int	orbit;			/* orbit number */
 	Diagnostic_Values	diagnostic;		/* diagnostic:
										10=Accepted
										20=Centering point unacceptable
										30=Too many segments
										40=Specular reflection
										50=Centering point outside reserved slot
										60=Segment too short (<9)
										70=Wrong payload configuration */
 unsigned int		swath_nb;		/* number of swaths accepted or rejected */
 Spot4Swath			swaths[10];		/* The structure Spot4Swath is described after */
} Resp4ToProg;									/* 04 */

/*		...... defined in spot.h .......
typedef struct {
 char		otype[4];		
 double		value;			
 char 		unit[3];		
 int 		checksum;		
} OsculatingElem;
*/

/*   ..... defined in spot.h .......
typedef struct {
	unsigned int point_id;	
	double		x_km;		
	double		y_km;		
	double 		z_km;		
	double 		x_km_s;		
	double 		y_km_s;		
	double 		z_km_s;		
} EphemerisPoint;
*/


typedef struct {
	Boolean			filled;			/* reserved */
	unsigned int	day;			/* day number of year */
	char			mission[3];		/* mission number */
	unsigned int	orbit;			/* orbit number */
	unsigned int	jday;			/* julian day date */
#ifndef sgi
    /* LINUX PADDING: required before double */
    char            __linux_padding1[4] ;
#endif
    double			begin_sec;		/* begin seconds (jd) */
	double			end_sec;		/* end seconds (jd) */
	int				data;			/* antenna-pointing data:
											0=osculating case:	oelem[6] is filled, epoint[50] is all zero
											1=ephemeris case:	epoint[50] is filled, oelem[6] is zero, see after	*/
#ifndef sgi
    /* LINUX PADDING: required before structure multiple by 8 */
    char            __linux_padding2[4] ;
#endif
    OsculatingElem	oelems[6];			/* The structure OsculatingElem is described after*/
 	unsigned int	epoint_nb;			/* ephemeris point number */
 	unsigned int	jd_first_pt;		/* julian date of the first line of the segment */
 	unsigned int	sec_first_pt;		/* seconds of the first line of the segment */
#ifndef sgi
    /* LINUX PADDING: required before structure multiple by 8 */
    char            __linux_padding3[4] ;
#endif
 	EphemerisPoint	epoint[50];			/* The structure EphemerisPoint is described after */
} Antenna4PtData;						/* 05 */

typedef struct {
 	char			chan_id[3];			/* channel identifier: C1 or C2 */
 	unsigned int	segment_counter;	/* format counter of the beginning of segment */
	HRVIR_Value		HRVIR_instrument; 	/*	Instrument used:
											10 = H1	
											20 = H2					*/
	Spectral_Value	spectral_mode;		/*	Instrument spectral mode:
											10 = M
											20 = XI					*/
 	float	begin_lon;					/* longitude of the beginning */
 	float	begin_lat;					/* latitude of the beginning */
 	float	end_lon;					/* longitude of the end */
 	float	end_lat;						/* latitude of the end */
 	float	angle;						/* viewing angle */
#ifndef sgi
    /* LINUX PADDING: required before double */
    char            __linux_padding1[4] ;
#endif
	double	begin_sec;					/* seconds of the beginning */
	double	end_sec;					/* seconds of the end */
} Channel4Segm;

typedef enum {
	IMG4SCHEDPLAN_DIAG_ACCEPTED = 10,
	IMG4SCHEDPLAN_DIAG_APPLIED = 20,
	IMG4SCHEDPLAN_DIAG_NO_PROGRAMMING = 30
} Img4SchedPlanDiag ; 

typedef struct {
 	Boolean				filled;			/* reserved */
 	unsigned int		day;			/* day number of year */
 	char				mission[3];		/* mission number */
 	unsigned int		orbit;			/* orbit number */
 	Img4SchedPlanDiag	diagnostic;		/* station programming:
											10 = programming accepted
											20 = default programming applied
											30 = no programming */
	unsigned int		chan1_segm_nb;	/* channel 1 segment number */
	unsigned int		chan2_segm_nb;	/* channel 2 segment number */
#ifndef sgi
    /* LINUX PADDING: required before structure multiple by 8 */
    char                __linux_padding[4] ;
#endif
    Channel4Segm		chan1_segm[50];	/* the structure Channel4Segm is described after */
	Channel4Segm		chan2_segm[50];	/* the structure Channel4Segm is described after */
} Img4SchedPlan;						/* 06 */


typedef struct {
 	Boolean				filled;			/* reserved */
 	unsigned int		day;			/* day number of year */
 	char				mission[3];		/* mission number */
 	unsigned int		orbit;			/* orbit number */

} Img4DRSSchedPlan;	/* 08, to be defined */



typedef struct {
	Boolean				Filled;			/*	If FALSE the structure is empty */
	Boolean				Madras_Flag;	/* reserved */
	Boolean				Diode_Flag;		/* reserved */
	Sequence4Plan		data0;			/* SPOT 4 card 0 */
	Sequence4Plan		data1;			/* SPOT 4 card 1 */
#ifndef sgi
    /* LINUX PADDING: required before structure multiple by 8 */
    char                __linux_padding1[4] ;
#endif
    Resp4ToReserv		data2;			/* SPOT 4 card 2 */
	Prog4Description	data3;			/* SPOT 4 card 3 */
	Resp4ToProg			data4;			/* SPOT 4 card 4 */
#ifndef sgi
    /* LINUX PADDING: required before structure multiple by 8 */
    char                __linux_padding2[4] ;
#endif
    Antenna4PtData		data5;			/* SPOT 4 card 5 */
    Img4SchedPlan		data6;			/* SPOT 4 card 6 */
	Img4DRSSchedPlan	data8;			/* SPOT 4 card 8 */
} Spot_4_Info;

/* 
			SPOT 4 Card 8

 .....to be defined.....

*/
typedef struct {
		SpotInfo
				Data1;
		Spot_4_Info
				Data2;
				} Generic_Spot_Info;
#endif
