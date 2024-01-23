


/* @(#) A.C.S. - Spot.h %I% %E% (include) */

#ifndef __SPOT_H__
#define __SPOT_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Odb.h>

#ifdef __cplusplus
extern "C" {
#endif

/* -------------------------------------------------------------*
*  ####### WARNING:												*
*   the following defines are stated to fix the					*
*  	structure 'SpotInfo' to be not dependent from				*
* 	'Odb.H'(libOdb.a) beeing this library Object Oriented and	*
*  	so UNTOUCHABLE and  UNINFLUENCEBLE.							*
*   signed :   the pover soft developers not writing in C++		*
* --------------------------------------------------------------*/

#define SPOT_INFO_MAX_SPOT_SEGMENT_NB	50
#define SPOT_INFO_MAX_SPOT_SWATH_NB	10
#define SPOT_INFO_MAX_SPOT_EPHEMERIS_NB	50

typedef struct {
	Boolean				filled ;
	unsigned int		day ;			/* day number of year */
	unsigned int		mission ;		/* mission number */
	unsigned int		orbit ;			/* orbit number */
	olRequestType		req_type ;		/* request type */
	float				begin_lat ;		/* given in degrees (-90 to 90) */
	float				end_lat ;
} ReservRequest ; /* 01*/

typedef struct {
	Boolean				filled ;
	unsigned int		day ;			/* day number of year */
	unsigned int		mission ;		/* mission number */
	unsigned int		orbit ;			/* orbit number */
	olRespToReservDiagnostic	diagnostic ;	/* diagnostic */
	float				begin_lat ;		/* given in degrees (-90 to 90) */
	float				end_lat ;
} RespToReserv ; /* 02 */

typedef struct {
	olSpectralMode		spec_mode ;		/* spectral mode */
	float				cen_lon ;		/* degrees (0 to 360) */
	float				cen_lat ;
	olSpectralMode		spec_mode2 ;	/* last 3 fields are used */
	float				cen_lon2 ;		/* if programming type	  */
	float				cen_lat2 ;		/* is INDEP_HRVS	  */
} SpotSegment ;

typedef struct {
	Boolean				filled ;
	unsigned int		day ;			/* day number of year	*/
	unsigned int		mission ;		/* mission number	*/
	unsigned int		orbit ;			/* orbit number		*/
	olProgrammingType	prog_type ;		/* programming type	*/
	unsigned int		segments_nb ;	/* number of segments	*/
	olSpotGainType		xs_gain[3] ;
	olSpotGainType		pa_gain ;
	SpotSegment			segments[SPOT_INFO_MAX_SPOT_SEGMENT_NB] ;
} ProgRequest ; /* 03 */

typedef struct {
	olSpectralMode		spec_mode ;		/* spectral mode */
	float				begin_lon ;
	float				begin_lat ;
	float				end_lon ;
	float				end_lat ;
	float				angle ;
} SpotSwath ;

typedef struct {
	Boolean				filled ;
	unsigned int		day ;			/* day number of year	*/
	unsigned int		mission ;		/* mission number	*/
	unsigned int		orbit ;			/* orbit number		*/
	olRespToProgDiagnostic	diagnostic ;	/* diagnostic		*/
	unsigned int		swath_nb ;		/* number of swaths	*/
	SpotSwath			swaths[SPOT_INFO_MAX_SPOT_SWATH_NB] ;
} RespToProg ; /* 04 */

typedef struct {
	char				otype[4] ;    
#ifndef sgi
     /* LINUX PADDING: required, since 1st double below was aligned to byte #8 on SGI */
     char               __linux_padding[4] ;
#endif
	double				value ;
	char				unit[3] ;
	int	checksum ;
} OsculatingElem ;

typedef struct {
	unsigned int		point_id ;
#ifndef sgi
     /* LINUX PADDING: required, since 1st double below was aligned to byte #8 on SGI */
     char               __linux_padding[4] ;
#endif
	double				x_km ;
	double				y_km ;
	double				z_km ;
	double				x_km_s ;
	double				y_km_s ;
	double				z_km_s ;
} EphemerisPoint ;

typedef struct {
	Boolean				filled ;
	unsigned int		day ;			/* day number of year	*/
	unsigned int		mission ;		/* mission number	*/
	unsigned int		orbit ;			/* orbit number		*/
	unsigned int		jday ;			/* julian day date	*/
#ifndef sgi
     /* LINUX PADDING: required, since 1st double below was aligned to byte #8 on SGI */
     char               __linux_padding0[4] ;
#endif
    double				begin_sec ;		/* begin seconds (jd)	*/
	double				end_sec ;		/* end seconds (jd)	*/
	olAntennaPtDataType	data ;			/* antenna-pointing data */
#ifndef sgi
     /* LINUX PADDING: */
     char               __linux_padding1[4] ;
#endif
    OsculatingElem		oelems[6] ;		/* osculating elements	*/
	unsigned int		epoint_nb ;
	unsigned int		jd_first_pt ;
	unsigned int		sec_first_pt ;
#ifndef sgi
     /* LINUX PADDING: */
     char               __linux_padding2[4] ;
#endif	
    EphemerisPoint		epoint[SPOT_INFO_MAX_SPOT_EPHEMERIS_NB] ;
} AntennaPtData ; /* 05 */

typedef struct {
	int					chan_id ;
	unsigned int		begin_counter ;
	unsigned int		end_counter ;
	int					hrv_id ;
	char				spec_mode ;
	char				segment ;
	int					test_indicator ;
	int					pa_gain ;
	int					xs_gain[3] ;
	int					ssm_angle ;
	int					status ;
	int					validity ;
	int					ref_pos ;
} ChannelSegm ;

typedef struct {
	Boolean				filled ;
	unsigned int		day ;			/* day number of year */
	unsigned int		mission ;		/* mission number */
	unsigned int		orbit ;			/* orbit number */
	olIspDiagnostic		diagnostic ;	/* diagnostic */
	unsigned int		chan1_segm_nb ;
	unsigned int		chan2_segm_nb ;
	uint32_t	        obt ;			/* on board time of pass */
	unsigned int		To ;
	float				to ;
	float				a ;
	uint32_t	        co ;
	ChannelSegm			chan1_segm[SPOT_INFO_MAX_SPOT_SEGMENT_NB] ;
	ChannelSegm			chan2_segm[SPOT_INFO_MAX_SPOT_SEGMENT_NB] ;
} ImgSchedPlan ; /* 06 */

typedef struct {
	Boolean				filled ;
	unsigned int		day ;			/* day number of year */
	unsigned int		mission ;		/* mission number */
	unsigned int		orbit ;			/* orbit number */
	unsigned int		To ;
	float				to ;
	float				a ;
    uint32_t	        co ;
	unsigned int		epoint_nb ;
	unsigned int		jd_first_pt ;
	unsigned int		sec_first_pt ;
#ifndef sgi
     /* LINUX PADDING: */
     char               __linux_padding[4] ;
#endif
	EphemerisPoint		epoint[SPOT_INFO_MAX_SPOT_EPHEMERIS_NB] ;
} PostPassEphem ; /* 07 */

typedef struct {
	unsigned int		a;
	unsigned int		b;

	ReservRequest		data1 ;
	RespToReserv		data2 ;
	ProgRequest			data3 ;
	RespToProg			data4 ;
	AntennaPtData		data5 ;
	ImgSchedPlan		data6 ;
	PostPassEphem		data7 ;
} SpotInfo ;

SpotInfo    *FillSpotInfo(olSpotInfo *) ;
olSpotInfo *GetSpotInfoPointer(olSpotInfo *array,  int index);

#ifdef __cplusplus
}
#endif

#endif /* __SPOT_H__ */
