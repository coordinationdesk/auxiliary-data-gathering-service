#ifndef ACSSATELLITES_H
#define ACSSATELLITES_H

/*----------------------------------------------------------------------------------*
*	SCCS identificator							    *
*-----------------------------------------------------------------------------------*/ 
static char const* ACSSATELLITES_HID="@(#) Advanced Computer Systems: acsSatellites.h         1.6 04/10/03 (include)";

#ifdef __cplusplus
	extern "C" {
#endif

/*----------------------------------------------------------------------------------*
*	Define the Values for the Field "Satellite_ID"				    				*
*-----------------------------------------------------------------------------------*/ 
#define	SATELLITES_AVAILABLES 	16	
#define	LANDSAT		  1			/* NASA Landsat (Landsat)									*/
#define	MOS			  2			/* NASDA Marine Observation Satellite (MOS)					*/
#define	JERS		  3			/* NASDA Japanese Earth Resources Satellite (JERS)			*/
#define	SPOT		  4			/* CNES Satellite Pour l' Observation de la Terre (SPOT)	*/
#define	ERS			  5			/* ESA Earth Resources Satellite (ERS)						*/
#define IRS			  6			/* INDIAN satellite											*/
#define RADARSAT	  7			/* CANADIAN satellite 										*/
#define NOAA		  8			/* Noaa														*/
#define ORBVIEW		  9			/* SS1														*/
#define SS1			 10
#define HELIOS		 11			/* Helios													*/
#define SPACESHUTTLE 12 		/* Space Shuttle (added for XSAR mission)					*/
#define EOSAM        13 		/* EOS Terra												*/
#define EOSPM        14 		/* EOS Aqua													*/
#define COSMO        15			/* COSMO SKYMED												*/
#define SACC        16			/* Sac C Argentinian Satellite												*/

	static char const
		*Satellite_Name[] =
		{
			"Landsat",
			"Mos",
			"Jers",
			"Spot",
			"Ers",
			"Irs", 
			"Radarsat",
			"Noaa",
			"Orbview", 
			"SS1", 
			"Helios", 
			"Shuttle",
			"EOSAM",
			"EOSPM",
			"COSMO",
			"SAC-C"
		};

/*----------------------------------------------------------------------------------*
*	Define the Values for the Field "Instrument_ID"				    				*
*-----------------------------------------------------------------------------------*/ 
#define	SENSORS_AVAILABLES 	19	
#define	Mss			1			/* Landsat Multi Spectral Scanner (MSS)					*/
#define	Tm			2			/* Landsat Thematic Mapper (TM)							*/
#define	Etm			3			/* Landsat Enhanced Thematic Mapper (ETM)				*/
#define	Rbv			4			/* Landsat Return Beam Vidicon (RBV)					*/
#define	Messr		5			/* MOS Multispectral Electronic Self Scanning Radiometer (MESSR)	*/
#define	Vnir		6			/* JERS Visible and Near Infrared Radiometer (VNIR)		*/
#define	Swir		7			/* JERS Short Wave Infrared Radiometer (SWIR)			*/
#define	Pan			8			/* SPOT Panchromatic (Pan)								*/
#define	Xs			9			/* SPOT Multispectral Scanner (XS)						*/
#define	Ers_Sar		10			/* ERS Synthetic Aperture Radar (SAR)					*/
#define	Atsr		11			/* ERS Along Track Scanning Radiometer (ATSR)			*/
#define	Hrv			12			/* SPOT Instrument Name (HRV)							*/
#define Jers_Sar 	13			/* JERS Synthetic Aperture Radar (SAR)					*/
#define Avhrr		14			/* NOAA AVHRR											*/
#define Hrvi		15			/* SPOT 4 Instrument Name (HRVI)						*/
#define XSar		16			/* Shuttle X Band SAR (HRVI)						    */
#define Modis		17			/* EOS satellites sensor								*/
#define Cosmo_Sar	18			/* COSMO SAR satellites sensor							*/
#define Mmrs	    19				/* MMRS for SAC-C										*/

	static char const
		*Sensor_Name[] =
		{
			"*WRONG*",
			"MSS",
			"TM",
			"ETM",
			"RBV",
			"MESSR",
			"VNIR",
			"SWIR",
			"Pan",
			"XS",
			"SAR",
			"ATSR",
			"HRV", 
			"SAR", 
			"AVHRR", 
			"HRVI",
			"XSAR",
			"MODIS",
			"SAR",
			"MMRS"	
		};
		
#ifdef __cplusplus
	}
#endif

#endif
