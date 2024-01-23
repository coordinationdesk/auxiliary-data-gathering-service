#include<sys/types.h>
#include<stdlib.h>
#include<stdio.h>

#ifndef _STATION_CONF_H_
#define _STATION_CONF_H_

#ifdef __cplusplus
extern "C" {
#endif


struct station_conf
{
/*1*/	char		ReceivingStation[25],		/* receiving station 	    */
/*2*/			ProcessingStation[25],		/* processing station 	    */
/*3*/			StorageStation[25],		/* storage   station 	    */
/*4*/			StationCode[25],		/* station code		    */
/*5*/			StationName[25];		/* station name		    */
/*6*/	float		AntennaLocLat;			/* Antenna Localization	    */
/*7*/	float		AntennaLocLon;			/* Antenna Localization	    */
/*8*/	char		TimeCodeType[25];		/* time code type 	    */
/*9*/	char		HddrType[25];			/* hddr  type		    */
/*10*/	char		FrameSyncType[25];		/* fs  type		    */
/*11*/	char		MediumLoc[25];			/* def Medium Localization  */
/*12*/	char		ReceivingStationShort[3];	/* rec station Short	    */
/*13*/	int			ReceivingStationId,		/* receiving station ident. */
/*14*/				ProcessingStationId,		/* processing station ident.*/
/*15*/				StorageStationId,		/* storage   station ident. */
/*15*/				HddrTypeId,			/* hddr type	 ident.	    */
/*16*/				FrameSyncTypeId,		/* frame synch ident.	    */
/*17*/				TimeCodeTypeId;			/* time code type ident.    */
/*18*/	char		AgencyName[25];			/* Agency name		    */
/*19*/	char		CountryName[25];		/* Country name		    */
/*20*/	float		StationAltitude;		/* Station Altitude [kmetres]*/
};

struct Availables {
		int			index[STATIONS_AVAILABLES];
		int			nfound;
};

/*
* PROTOTYPES
*/
int 	 stationDumpConf				(struct station_conf *conf);
int      stationReadConfFromFile		(char *filename, struct station_conf *conf);
int 	 stationReadConf				(struct station_conf *conf);
int 	 stationWriteConf				(char *filename, struct station_conf *conf);

int	 	  stationGetRecInt				(int *statid);
int 	 stationGetRecString			(char *station);

int 	 stationGetProcInt				(int *statid);
int 	 stationGetProcString			(char *station);

int 	 stationGetCode					(char *code);

int  	 stationReadAvailable			(char *StationTypeKey, struct Availables *AvStat);
int  	 stationReadAvailableFromFile	(char *Filename, char *StationTypeKey, struct Availables *AvStat);
int  	 stationWriteAvailable			(char *StationTypeKey, struct Availables *AvStat);
int  	 stationIsAmongAvail			(int index, struct Availables *AvStat);

char	 *stationLong2ShortStr			(char *station);
int	 	 stationStr2Int	    			(char *station);
int	 	 stationStr2CloserInt	    	(char *station);

int 	 stationReplaceInTable			(char * Filename, char * Key, int ncmd, char ** command_table);
int 	 stationAppendInTable 			(char * Filename, char * Key, int ncmd, char ** command_table);
int 	 stationGetFromTable  			(char * Filename, char * Key, int * ncmd, char ** command_table);

int      stationGetDebugLevel			(char *debug_what);
int 	 stationSkipRest				(FILE * fp);
char 	 **stationAllChMatrix			(int nrl, int nrh, int ncl, int nch);
void 	 stationFreeChMatrix			(char **m, int nrl, int nrh, int ncl, int nch);


/* for back compatibility */

#define ENVGetStationCode(a) stationGetCode(a)
#define ENVGetProcessingStation(a) stationGetProcString(a)
#define ENVGetProcessingStationId(a) stationGetProcInt(a)
#define ENVGetReceivingStationId(a) stationGetRecInt(a)
#define Write_station_conf(a,b) stationWriteConf(a,b)
#define Read_station_conf(a) stationReadConf(a)
#define Read_not_default_station_conf(a,b) stationReadConfFromFile(a,b)
#define ENVDumpStationConf(a) stationDumpConf(a)
#define GetStationId(a) stationStr2Int(a)
#define GetCloserStationId(a) stationStr2CloserInt(a)

#ifdef __cplusplus
}
#endif

#endif
