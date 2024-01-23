
#ifndef ACSSTATIONS_H
#define ACSSTATIONS_H

/*----------------------------------------------------------------------------------*
*	SCCS identificator							    *
*-----------------------------------------------------------------------------------*/ 
static char const * ACSSTATIONSId="@(#) Advanced Computer Systems: acsStations.h           1.8 04/10/03 (include)";

#ifdef __cplusplus
	extern "C" {
#endif

/*----------------------------------------------------------------------------------*
*	Define the Values for the Field "Station_ID"				    *
*-----------------------------------------------------------------------------------*/ 
#define	STATIONS_AVAILABLES 	118
#define	Fucino		1		/* Fucino	(Italy)			    			*/
#define	Kiruna		2		/* Kiruna	(Sweden)		    			*/
#define	Maspalomas	3		/* Maspalomas	(Spain)			    		*/
#define	Trompso		4		/* Trompso	(Norway)		    			*/
#define	Tel_Aviv	5		/* Tel Aviv	(Israel)		    			*/
#define	Bangkok		6		/* Bangkok	(Thailland)		    			*/
#define	Fairbanks	7		/* Fairbanks	(Alaska SAR Facility)	    */ 
#define	Cotopaxi	8		/* Cotopaxi	(Ecquador)		   		 		*/
#define	Gatineau	9		/* Gatineau 	(Canada) [ERS High Rate]    */
#define	AliceSpring	10		/* AliceSpring	(Australia)		    		*/
#define	Hyderabad	11		/* Hyderabad	(India)			    		*/
#define	Kumamoto	12		/* Kumamoto	(Japan)			   		 		*/
#define	PrinceAlbert 13		/* PrinceAlbert	(Canada) [ERS High Rate]    */
#define	WestFreugh	14		/* WestFreugh	(United Kingdom)	    	*/
#define	OHiggins	15
#define	Hatoyama	16
#define	Syowa		17
#define	PariPari	18
#define	HarteBeesHoek	19
#define	Aussaguel	20
#define	Islamabad	21
#define	Hobart		22
#define	Cuiaba		23
#define	Farnborough	24
#define	Pretoria	25
#define	Scanzano	26 
#define	Oberpfaff	27
#define	Budapest	28
#define	Bremenhaven	29
#define	Berlin		30
#define	Berne		31
#define	Copenhagen	32
#define	DeBilt		33
#define	Dundee		34
#define	Frascati	35
#define	Hamburg		36
#define	Helsinki	37
#define	Krakow		38  
#define	Lannion		39
#define	Lasham		40
#define	Madrid		41
#define	Norrkoping	42
#define	Offenbach	43
#define	Oslo		44
#define	Prague		45
#define	Rome		46
#define	Spitzenbergen	47
#ifdef sgi
    /* Commented, since a warning is emitted on Linux (due to "-"). DECOMMENT IF NEEDED */
	#define Traben-Trar 48
#endif
#define	Bedford		49
#define	Downsview	50
#define	Edmonton	51
#define	BantonRouge	52
#define	Miami		53
#define	GilmoreCreek	54
#define	NESDIS		55
#define	WallopsIsl	56
#define	RedwoodCity	57
#define	ScippsInstitute	58	    /* modified 29-05-96: before "Scipps Inst" (???) */
#define	SiouxFalls	59
#define	StennisSpace	60
#define	UnivOfAlaska	61
#define	UnivOfTexas	62
#define	UnivOfRhodeIsl	63
#define	Cashoiera	64
#define	Funceme		65
#define	Santiago	66
#define	Adelaide	67
#define	Aspendale	68
#define	Honolulu	69
#define	Perth		70
#define	Sydney		71
#define	Townsville	72
#define	Wellington	73
#define	Beijing		74
#define	Dhaka		75
#ifdef sgi
/* Commented, since a warning is emitted on Linux (due to "-"). DECOMMENT IF NEEDED */
    #define	Da-Xing		76
#endif
#define	Keelung		77
#define Kiyose		78
#define	Lapan		79
#define	Manila		80
#define	NewDelhi	81
#define	Riyadh		82
#define	Singapore	83
#define	Selangor	84
#define	Seoul		85
#define	SeoulUniv	86
#define	Sendai		87
#define	TokaiUniv	88
#define	TokyoUniv	89
#define	Taipei		90
#define	KualaLumpur	91
#define	Casey		92
#define	McMurdo		93
#define	Palmer		94
#define	TerranovaBay 95
#define	Cairo		96
#define	Agrhymet	97
#define	Nairobi		98
#define	LaReunion	99
#define	Tunis		100
#define	RRSC_Nairobi 101
#define	Norman   	102		/* Oklaoma				*/
#define	Cordoba		103		/* Cordoba Ground Station (Argentina)	*/
#define	Matera		104		/* Matera (IPAF) Ground Station (Italy)	*/
#define	Atlanta		105		/* Atlanta (USA)			*/
#define	Libreville	106		/* Libreville (CONGO)			*/
/* 	??????	107	???? saltata */
#define	Neustrelitz	108		/* Neustrelitz (Germany)		*/
#define	Malindi		109		/* Malindi (Kenia)			*/
#define	Moscow		110		/* Moscow (Russia)			*/
#define	Kourou		111		/* Kourou (Guayana)			*/
/* added 04/2001 */
#define Bishkek		112		/* Kyrgysistan */
#define	Kitab		113		/* Uzbekistan  */
#define UlanBator	114		/* Mongolia */	
#define Itu			115		/* Turkey */
#define KhantyMansiysk	116  /* Russia, Siberia */
#define Tripoli		117  /* Tripoli */
#define Morzuk		118  /* Morzuk */	

	static char const
		*Station_Name[] =
		{
/* 1 */		"Fucino",
			"Kiruna",
			"Maspalomas",
			"Tromsoe",	/* 'Trompso' to 'Tromsoe': 22-01-96		*/
			"TelAviv",
			"Bangkok",
			"Fairbanks",
			"Cotopaxi",	/* Cotopaxi	(Ecquador)			*/
			"Gatineau",	/* Gatineau 	(Canada) [ERS High Rate]	*/
/* 10 */	"AliceSprings", /* 'AliceSpring' to 'AliceSprings': 22-01-96	*/
			"Hyderabad",
			"Kumamoto",
			"PrinceAlbert",
			"WestFreugh",
			"OHiggins",
			"Hatoyama",
			"Syowa",
			"ParePare",	/* 'PariPari' to 'ParePare': 22-01-96		*/
			"Hartebeeshoek",/* 'HarteBeesHoek' to 'Hartebeeshoek' 01-08-96	*/
/* 20 */	"Aussaguel",
			"Islamabad",
			"Hobart",
			"Cuiaba",
			"Farnborough", 
			"Pretoria", 
			"Scanzano", 
			"Oberpfaff.",
			"Budapest",
			"Bremenhaven",
/* 30 */	"Berlin",
			"Berne",
			"Copenhagen",
			"DeBilt",
			"Dundee",
			"Frascati",
			"Hamburg",
			"Helsinki",
			"Krakow",
			"Lannion",
/* 40 */	"Lasham",
			"Madrid",
			"Norrkoping",
			"Offenbach",
			"Oslo",
			"Prague",
			"Rome",
			"Spitzenbergen",
			"Traben-Trar.",
			"Bedford",
/* 50 */	"Downsview",
			"Edmonton",
			"BantonRouge",
			"Miami",
			"GilmoreCreek",
			"NESDIS",
			"WallopsIsl.",
			"RedwoodCity",
			"ScippsInstitute",	/* 'Scipps Inst' to 'ScippsInstitute': 29-05-96 */
			"SiouxFalls",
/* 60 */	"StennisSpace",
			"UnivOfAlaska",
			"UnivOfTexas",
			"UnivOfRhodeIsl",
			"Cashoiera",
			"Funceme",
			"Santiago",
			"Adelaide",
			"Aspendale",
			"Honolulu",
/* 70 */	"Perth",
			"Sydney",
			"Townsville",
			"Wellington",
			"Beijing",
			"Dhaka",
			"Da-Xing",
			"Keelung",
			"Kiyose",
			"Lapan",
/* 80 */	"Manila",
			"NewDelhi",
			"Rhyad",		/*'Riyadh'    tp  'Rhyad'	    : 01-06097 */
			"Singapore",
			"Selangor",
			"Seoul",
			"SeoulUniversity",	/* 'SeoulUniv' to 'SeoulUniversity' : 29-05-96 */
			"Sendai",
			"TokaiUniversity",	/* 'TokaiUniv' to 'TokaiUniversity' : 29-05-96 */
			"TokyoUniversity",	/* 'TokyoUniv' to 'TokyoUniversity' : 29-05-96 */
/* 90 */	"Taipei",
			"KualaLumpur",
			"Casey",
			"McMurdo",
			"Palmer",
			"TerranovaBay",
			"Cairo",
			"Agrhymet",
			"Nairobi",
			"LaReunion",
/* 100 */	"Tunis",
			"RRSC_Nairobi", 
			"Norman", 
			"Cordoba", 
			"Matera", 
			"Atlanta", 
			"Libreville", 
			" ", 
			"Neustrelitz", 
			"Malindi", 
/* 110 */	"Moscow", 
			"Kourou",
			"Biskek",
			"Kitab",
			"UlanBator",
			"Itu",
			"KhantyMansiysk",
			"Tripoli",
			"Morzuk"
		};

	static char const
		*Station_Name_Short[] =
		{
						"FS",	/* Fucino								*//*------- 1 -------*/	
						"KS",	/* Kiruna	(Sweeden)					*/
						"MS",	/* Maspalomas	(Canay Islands)			*/
						"TS",	/* Tromsoe	(Norway)					*/
/*'TS'->'IR':22-01-96*/	"IR",	/* Tel Aviv	(Israel)					*/
/*'BS'->'TH':22-01-96*/	"TH",	/* Bangkok	(Thailland)					*/
/*'FS'->'AF':22-01-96*/	"AF",	/* Fairbanks	(Alaska SAR Facility)	*/ 
/*'CS'->'CO':22-01-96*/	"CO",	/* Cotopaxi	(Ecquador)					*/
/*'GS'->'GH':22-01-96*/ "GH",	/* Gatineau 	(Canada) [ERS High Rate]*/
						"AS",	/* AliceSpring	(Australia)				*/ /*------ 10 ------*/
/*'HS'->'SE':22-01-96*/	"SE",	/* Hyderabad	(India)					*/
/*'KS'->'KU':22-01-96*/ "KU",	/* Kumamoto	(Japan)						*/
/*'PS'->'PH':22-01-96*/ "PH",	/* PrinceAlbert	(Canada) [ERS High Rate]*/
/*'WS'->'WF':22-01-96*/ "WF",	/* WestFreugh	(United Kingdom)		*/
/*'OS'->'TF':22-01-96*/ "TF",	/* O'Higgins/Transportable Fern.(Antartic/German)*/
/*'HS'->'HA':22-01-96*/	"HA",	/* Hatoyama	(Japan)						*/
/*'SS'->'SY':22-01-96*/	"SY",	/* Syowa	(Antartic/Japan)			*/
/*'PS'->'IN':22-01-96*/	"IN",	/* ParePare 	(Indonesia)				*/
/*'HS'->'JO':22-01-96*/	"JO",	/* HarteBeesHoek/Johannesburg(South Africa)*/
/*'AS'->'TO':22-01-96*/	"TO",	/* Aussaguel/Tolouse (France)			*/ /*------ 20 ------*/
						"IS",	/* Islamabad	(Pakistan)				*/
/*'HS'->'HO':22-01-96*/	"HO",	/* Hobart 	(Australia)					*/
/*'CS'->'CU':22-01-96*/	"CU",	/* Cuiaba 	(Brazil)					*/
						"UP",	/* Farnborough[UKPAF] (UK)				*/
						"PS",	/* PrinceAlbert (Canada)				*/
						"SS",	/* Scansano (Italy)						*/
/*"OS"->"OP"-"DP"01/02*/ "DP",	/* Oberpfaff. (Germany)					*/
						"BS",	/* Budapest (Hunmgaria)					*/
						"BS",	/* Brema (Germany)						*/
/* 30 */				"BS",	/* Berlin (Germany)						*/
						"BS",	/*		*/
						"CS",	/*		*/
						"DS",	/*		*/
						"DS",	/*		*/
						"ES",	/*Frascati(EECF)*/
						"HS",	/*		*/
						"HS",	/*		*/
						"KS",	/*		*/
						"LS",	/*		*/
/* 40 */				"LS",	/*		*/
						"MS",	/*		*/
						"NS",	/*		*/
						"OS",	/*		*/
						"OS",	/*		*/
						"PS",	/*		*/
						"RS",	/*		*/
						"SS",	/*		*/
						"TS",	/*		*/
						"BS",	/*		*/
/* 50 */				"DS",	/*		*/
						"ES",	/*		*/
						"BS",	/*		*/
						"MS",	/*		*/
						"GS",	/*		*/
						"NS",	/*		*/
						"WS",	/*		*/
						"RS",	/*		*/
						"SS",	/*		*/
						"SS",	/*		*/
/* 60 */				"SS",	/*		*/
/* AU -> AF 22-11-99*/	"AF",	/* Alaska ASF	[Alaska(USA)	*/	
						"TU",	/*		*/
						"RU",	/*		*/
						"CS",	/*		*/
						"FS",	/*		*/
						"SS",	/*		*/
						"AS",	/*		*/
						"AS",	/*		*/
/* Hs -> Hw */			"HW",	/*		*/		/* Honolulu (Haway)*/
/* 70 */				"PS",	/*		*/
						"SS",	/*		*/
						"TS",	/*		*/
						"WS",	/*		*/
/*'BS'->'BE':22-01-96*/	"BE",	/* Beijing	    (China)		*/
						"DS",	/*		*/
						"DS",	/*		*/
						"KS",	/*		*/
						"KS",	/*		*/
						"LS",	/*		*/
/* 80 */				"MS",	/*		*/
						"NS",	/*		*/
/*'RS'->'SA':22-01-96*/	"SA",	/* Rhyad	    (Saudi Arabia)	*/
/*'SS'->'SG':22-01-96*/	"SG",	/* Singapore	    (Malaysia)		*/
						"SS",	/*		*/
						"SS",	/*		*/
						"SS",	/*		*/
						"SS",	/*		*/
						"TU",	/*		*/
						"TU",	/*		*/
/*'TS'->'TW':01-06-97*/ "TW",	/* Taipei	    (Taiwan)			*/
/* 91 */				"KS",	/*									*/
						"CS",	/*									*/
/*'MS'->'MM':22-01-96*/ "MM",	/* Mac Murdo	    (Antartic/USA)	*/
						"PS",	/*									*/
						"TS",	/*									*/
						"CS",	/*									*/
						"AS",	/*									*/
						"NS",	/*									*/
						"LS",	/*									*/
/* 100 */				"TS",	/*									*/
						"NS", 	/*									*/
/* NS to NO :01-06-97*/ "NO", 	/* Norman							*/
/* AS to CA :01-06-97*/	"CA", 	/* Cordoba	    (Argentina)			*/
/* IP to MA :25-10-98*/	"MA", 	/* Matera[IPAF]	    (Italy)			*/
						"AT", 	/* Atlanta	    (USA)				*/
						"LI", 	/* Libreville	 (Congo)			*/
						"  ", 	/* Dummy							*/
						"NZ", 	/* Neustrelitz	 (Germany)			*/
						"ML", 	/* Malindi	    (Kenia)				*/
						"MW", 	/* Moscow	    (Russia)			*/
						"KR", 	/* Kourou	    (Guayana)			*/
						"BK",	/* Bishkek 		(Kyrgysistan)		*/
						"KB",	/* Kitab        (Uzbakistan)		*/
						"UB",	/* Ulan Bator   (Mongolia)			*/
						"IT",    /* Turkish station (temporary....)  */
						"KM",	/* KhantyMansiysk (Siberia)			*/
						"TR",	/* Tripoli (Libya) */
						"MZ"	/* Morzuk (Libya) */
		};

	static double
		Station_Lat[] =
		{
/*Fucino 		*/		41.9782,
/*Kiruna  		*/		67.878,
/*Maspalomas	*/		27.75,
/*Tromsoe		*/		69.96,	
/*TelAviv		*/		32.06,
/*Bangkok		*/		13.75,
/*Fairbanks		*/		64.83,
/*Cotopaxi		*/		-0.217,	
/*Gatineau		*/		45.483,	
/*AliceSprings	*/		-23.7, 
/*Hyderabad		*/		17.383,
/*Kumamoto		*/		32.800,
/*PrinceAlbert	*/		52.967,
/*WestFreugh	*/		0.0,
/*OHiggins		*/		-63.316,
/*Hatoyama		*/		43.083,
/*Syowa			*/		0.0,
/*ParePare		*/		-4.017,
/*Hartebeeshoek	*/		-25.914,
/*Aussaguel		*/		0.0,
/*Islamabad		*/		32.271,
/*Hobart		*/		-42.917,
/*Cuiaba		*/		-15.583,  /* TBC */
/*Farnborough	*/		0.0, 
/*Pretoria		*/		-25.914, 
/*Scanzano		*/		40.250, 
/*Oberpfaff.	*/		48.067,
/*Budapest		*/		47.500,
/*Bremenhaven	*/		48.033,
/* 30 Berlin	*/		52.517,
/*Berne			*/		0.0,
/*Copenhagen	*/		0.0,
/*DeBilt		*/		0.0,
/*Dundee		*/		0.0,
/*Frascati		*/		0.0,
/*Hamburg		*/		0.0,
/*Helsinki		*/		0.0,
/*Krakow		*/		0.0,
/*Lannion		*/		0.0,
/* 40 Lasham	*/		0.0,
/*Madrid		*/		0.0,
/*Norrkoping	*/		0.0,
/*Offenbach		*/		0.0,
/*Oslo			*/		0.0,
/*Prague		*/		0.0,
/*Rome			*/		0.0,
/*Spitzenbergen	*/		0.0,
/*Traben-Trar.	*/		0.0,
/*Bedford		*/		0.0,
/* 50 Downsview	*/		0.0,
/*Edmonton		*/		0.0,
/*BantonRouge	*/		0.0,
/*Miami			*/		0.0,
/*GilmoreCreek	*/		0.0,
/*NESDIS		*/		0.0,
/*WallopsIsl.	*/		0.0,
/*RedwoodCity	*/		0.0,
/*ScippsInstitute*/		0.0,
/*SiouxFalls	*/		0.0,
/* 60 StennisSpace*/	0.0,
/*UnivOfAlaska	*/		64.838,
/*UnivOfTexas	*/		0.0,
/*UnivOfRhodeIsl*/		0.0,
/*Cashoiera		*/		0.0,
/*Funceme		*/		0.0,
/*Santiago		*/		0.0,
/*Adelaide		*/		0.0,
/*Aspendale		*/		0.0,
/*Honolulu		*/		21.307,
/* 70 Perth		*/		0.0,
/*Sydney		*/		0.0,
/*Townsville	*/		0.0,
/*Wellington	*/		0.0,
/*Beijing		*/		39.900,
/*Dhaka			*/		0.0,
/*Da-Xing		*/		0.0,
/*Keelung		*/		0.0,
/*Kiyose		*/		0.0,
/*Lapan			*/		0.0,
/* 80 Manila	*/		0.0,
/*NewDelhi		*/		0.0,
/*Rhyad			*/		24.672,
/*Singapore		*/		1.293,
/*Selangor		*/		0.0,
/*Seoul			*/		0.0,
/*SeoulUniversity*/		0.0,	
/*Sendai		*/		0.0,
/*TokaiUniversity*/		0.0,	
/*TokyoUniversity*/		0.0,	
/* 90 Taipei	*/		25.017,
/*KualaLumpur	*/		0.0,
/*Casey			*/		0.0,
/*McMurdo		*/		-77.850,
/*Palmer		*/		0.0,
/* TerranovaBay	*/		0.0,
/* Cairo		*/		0.0,
/* Agrhymet		*/		0.0,
/* Nairobi		*/		0.0,
/* LaReunion	*/		0.0,
/* 100 Tunis	*/		0.0,
/* RRSC_Nairobi	*/		0.0, 
/* Norman		*/		35.223, 
/* Cordoba		*/		-31.52, 
/* Matera		*/		40.667, 
/* Atlanta		*/		45.005, /* michigan */
/* Libreville	*/		0.3000, 
/* 				*/		0.0, 
/* Neustrelitz	*/		53.32972, 
/* Malindi		*/		-2.995, 
/* Moscow		*/		55.8623, 
/* Kourou		*/		52.5144,
/* Bishkek 		*/		33.7,
/* Kitab		*/		39.124,
/* Ulan Bator   */		47.917,
/* Itu*/				0.0,
/* KhantyMansiysk */    61.0042,
/*Tripoli */		32.9666,
/* Morzuk */		25.9283
		};

	static double
		Station_Lon[] =
		{
/*Fucino 	*/		13.60356,
/*Kiruna  	*/		21.0634,
/*Maspalomas*/		-15.56,
/*Tromsoe	*/		18.967,	
/*TelAviv	*/		34.767,
/*Bangkok	*/		100.517,
/*Fairbanks	*/		-147.716,
/*Cotopaxi	*/		-78.500,	
/*Gatineau	*/		-75.650,	
/*AliceSprings*/	133.88, 
/*Hyderabad	*/		78.467,
/*Kumamoto	*/		130.717,
/*PrinceAlbert*/	-108.383,
/*WestFreugh*/		0.0,
/*OHiggins	*/		-57.900,
/*Hatoyama	*/		141.817,
/*Syowa		*/		0.0,
/*ParePare	*/		119.633,
/*Hartebeeshoek	*/	27.145,
/*Aussaguel	*/		0.0,
/*Islamabad	*/		71.920,
/*Hobart	*/		147.333,
/*Cuiaba	*/		-56.083,	/* TBC */
/*Farnborough	*/		0.0, 
/*Pretoria	*/		27.145, 
/*Scanzano	*/		16.700, 
/*Oberpfaff.	*/	11.267,
/*Budapest	*/		19.083,
/*Bremenhaven	*/	9.367,
/* 30 Berlin	*/	13.400,
/*Berne		*/		0.0,
/*Copenhagen	*/		0.0,
/*DeBilt	*/		0.0,
/*Dundee	*/		0.0,
/*Frascati	*/		0.0,
/*Hamburg	*/		0.0,
/*Helsinki	*/		0.0,
/*Krakow	*/		0.0,
/*Lannion	*/		0.0,
/* 40 Lasham	*/		0.0,
/*Madrid	*/		0.0,
/*Norrkoping	*/		0.0,
/*Offenbach	*/		0.0,
/*Oslo		*/		0.0,
/*Prague	*/		0.0,
/*Rome		*/		0.0,
/*Spitzenbergen	*/		0.0,
/*Traben-Trar.	*/		0.0,
/*Bedford	*/		0.0,
/* 50 Downsview	*/		0.0,
/*Edmonton	*/		0.0,
/*BantonRouge	*/		0.0,
/*Miami		*/		0.0,
/*GilmoreCreek	*/		0.0,
/*NESDIS	*/		0.0,
/*WallopsIsl.	*/		0.0,
/*RedwoodCity	*/		0.0,
/*ScippsInstitute*/		0.0,
/*SiouxFalls	*/		0.0,
/* 60 StennisSpace*/		0.0,
/*UnivOfAlaska	*/		-147.716,
/*UnivOfTexas	*/		0.0,
/*UnivOfRhodeIsl*/		0.0,
/*Cashoiera	*/		0.0,
/*Funceme	*/		0.0,
/*Santiago	*/		0.0,
/*Adelaide	*/		0.0,
/*Aspendale	*/		0.0,
/*Honolulu	*/		-157.858,
/* 70 Perth	*/		0.0,
/*Sydney	*/		0.0,
/*Townsville	*/		0.0,
/*Wellington	*/		0.0,
/*Beijing	*/		116.413,
/*Dhaka		*/		0.0,
/*Da-Xing	*/		0.0,
/*Keelung	*/		0.0,
/*Kiyose	*/		0.0,
/*Lapan		*/		0.0,
/* 80 Manila	*/		0.0,
/*NewDelhi	*/		0.0,
/*Rhyad		*/		46.713,
/*Singapore	*/		103.856,
/*Selangor	*/		0.0,
/*Seoul		*/		0.0,
/*SeoulUniversity*/		0.0,	
/*Sendai	*/		0.0,
/*TokaiUniversity*/		0.0,	
/*TokyoUniversity*/		0.0,	
/* 90 Taipei	*/		121.450,
/*KualaLumpur	*/		0.0,
/*Casey		*/		0.0,
/*McMurdo	*/		166.650,
/*Palmer	*/		0.0,
/*TerranovaBay	*/		0.0,
/*Cairo		*/		0.0,
/*Agrhymet	*/		0.0,
/*Nairobi	*/		0.0,
/*LaReunion	*/		0.0,
/* 100 Tunis	*/		0.0,
/*RRSC_Nairobi	*/		0.0, 
/*Norman	*/		-97.439, 
/*Cordoba	*/		-64.46, 
/*Matera	*/		16.600, 
/*Atlanta	*/		-84.144, 
/*Libreville	*/		9.2501, 
/* 	*/			0.0, 
/*Neustrelitz	*/		13.0725, 
/*Malindi	*/		40.194, 
/*Moscow	*/		37.9555, 
/*Kourou	*/		-52.8046,
/* Bishkek 		*/	74.6,
/* Kitab		*/	66.877,
/* Ulan Bator   */	106.917,
/* Itu*/				0.0,		
/* KhantyMansiysk */    69.0019,
/*Tripoli */		13.2,
/* Morzuk */		13.9425
};

	static double
		Station_Altitude[] =
		{
/* Fucino 	*/		0.6508,
/*Kiruna  	*/		0.496217,
/*Maspalomas	*/		0.0,
/*Tromsoe	*/		0.0,	
/*TelAviv	*/		0.0,
/*Bangkok	*/		0.0,
/*Fairbanks	*/		0.0,
/*Cotopaxi	*/		0.0,	
/*Gatineau	*/		0.0,	
/*AliceSprings	*/		0.0, 
/*Hyderabad	*/		0.0,
/*Kumamoto	*/		0.0,
/*PrinceAlbert	*/		0.0,
/*WestFreugh	*/		0.0,
/*OHiggins	*/		0.0,
/*Hatoyama	*/		0.0,
/*Syowa		*/		0.0,
/*ParePare	*/		0.0,
/*Hartebeeshoek	*/		1.5548,
/*Aussaguel	*/		0.0,
/*Islamabad	*/		0.0,
/*Hobart	*/		0.0,
/*Cuiaba	*/		0.0,
/*Farnborough	*/		0.0, 
/*Pretoria	*/		1.5548, 
/*Scanzano	*/		0.0, 
/*Oberpfaff.	*/		0.0,
/*Budapest	*/		0.0,
/*Bremenhaven	*/		0.0,
/* 30 Berlin	*/		0.0,
/*Berne		*/		0.0,
/*Copenhagen	*/		0.0,
/*DeBilt	*/		0.0,
/*Dundee	*/		0.0,
/*Frascati	*/		0.0,
/*Hamburg	*/		0.0,
/*Helsinki	*/		0.0,
/*Krakow	*/		0.0,
/*Lannion	*/		0.0,
/* 40 Lasham	*/		0.0,
/*Madrid	*/		0.0,
/*Norrkoping	*/		0.0,
/*Offenbach	*/		0.0,
/*Oslo		*/		0.0,
/*Prague	*/		0.0,
/*Rome		*/		0.0,
/*Spitzenbergen	*/		0.0,
/*Traben-Trar.	*/		0.0,
/*Bedford	*/		0.0,
/* 50 Downsview	*/		0.0,
/*Edmonton	*/		0.0,
/*BantonRouge	*/		0.0,
/*Miami		*/		0.0,
/*GilmoreCreek	*/		0.0,
/*NESDIS	*/		0.0,
/*WallopsIsl.	*/		0.0,
/*RedwoodCity	*/		0.0,
/*ScippsInstitute*/		0.0,
/*SiouxFalls	*/		0.0,
/* 60 StennisSpace*/		0.0,
/*UnivOfAlaska	*/		0.0,
/*UnivOfTexas	*/		0.0,
/*UnivOfRhodeIsl*/		0.0,
/*Cashoiera	*/		0.0,
/*Funceme	*/		0.0,
/*Santiago	*/		0.0,
/*Adelaide	*/		0.0,
/*Aspendale	*/		0.0,
/*Honolulu	*/		0.0,
/* 70 Perth	*/		0.0,
/*Sydney	*/		0.0,
/*Townsville	*/		0.0,
/*Wellington	*/		0.0,
/*Beijing	*/		0.0,
/*Dhaka		*/		0.0,
/*Da-Xing	*/		0.0,
/*Keelung	*/		0.0,
/*Kiyose	*/		0.0,
/*Lapan		*/		0.0,
/* 80 Manila	*/		0.0,
/*NewDelhi	*/		0.0,
/*Rhyad		*/		0.0,
/*Singapore	*/		0.0,
/*Selangor	*/		0.0,
/*Seoul		*/		0.0,
/*SeoulUniversity*/		0.0,	
/*Sendai	*/		0.0,
/*TokaiUniversity*/		0.0,	
/*TokyoUniversity*/		0.0,	
/* 90 Taipei	*/		0.0,
/*KualaLumpur	*/		0.0,
/*Casey		*/		0.0,
/*McMurdo	*/		0.0,
/*Palmer	*/		0.0,
/*TerranovaBay	*/		0.0,
/*Cairo		*/		0.0,
/*Agrhymet	*/		0.0,
/*Nairobi	*/		0.0,
/*LaReunion	*/		0.0,
/* 100 Tunis	*/		0.0,
/*RRSC_Nairobi	*/		0.0, 
/*Norman	*/		0.0, 
/*Cordoba	*/		0.752, 
/*Matera	*/		0.0, 
/*Atlanta	*/		0.0, 
/*Libreville	*/		0.1717, 
/* 	*/			0.0, 
/*Neustrelitz	*/		0.117, 
/*Malindi	*/		0.0123, 
/*Moscow	*/		0.1717 , 
/*Kourou	*/		0.014561,
					0.0,
					0.0,
					0.0,
/* Itu*/				0.0,
/* KhantyMansiysk */    0.300,
/*Tripoli */		0.0,
/* Morzuk */		0.453

		};
		
#ifdef __cplusplus
	}
#endif

#endif
