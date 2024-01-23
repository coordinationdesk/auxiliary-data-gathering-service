/*

Copyright 2010-2020, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S.  T_Schema table population script $

	$Id$

	$Author$

*/

/* 
	//////////////////////////////////////////////
	//	t_stations (stationid, stationname, stationacronym) 
	//	Stations Name
	//////////////////////////////////////////////
*/
INSERT INTO t_stations (stationid, stationname, stationacronym, position) VALUES (  1, 'Fucino', 'FUC', 'SRID=4326; POINT(13.601927 41.978887)') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym, position) VALUES (  2, 'Kiruna', 'KIS', 'SRID=4326; POINT(20.964325 67.857128)') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym, position) VALUES (  3, 'Maspalomas', 'MAS', 'SRID=4326; POINT(-15.633809 27.762889)') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES (  4, 'Trompso', 'TRP') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES (  5, 'Tel_Aviv', 'TLV') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES (  6, 'Bangkok', 'BGK') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES (  7, 'Fairbanks', 'FRB') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES (  8, 'Cotopaxi', 'CTX') ;
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES (  9, 'Gatineau', 'GTN') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 10, 'AliceSpring', 'ASP') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 11, 'Hyderabad', 'HYD') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 12, 'Kumamoto', 'KUM') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 13, 'PrinceAlbert', 'PAB') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 14, 'WestFreugh', 'WFR') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 15, 'OHiggins', 'OHG') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 16, 'Hatoyama', 'HTY') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 17, 'Syowa', 'SYW') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 18, 'PariPari', 'PRP') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 19, 'HarteBeesHoek', 'HBH') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 20, 'Aussaguel', 'ASS') ;
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 21, 'Islamabad', 'ISL') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 22, 'Hobart', 'HBT') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 23, 'Cuiaba', 'CIB') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 24, 'Farnborough', 'FBR') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 25, 'Pretoria', 'PRT') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 26, 'Scanzano', 'SCZ') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 27, 'Oberpfaff', 'OBP') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 28, 'Budapest', 'BDP') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 29, 'Bremenhaven', 'BRH') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 30, 'Berlin', 'BRL') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 31, 'Berne', 'BRN') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 32, 'Copenhagen', 'CPH') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 33, 'DeBilt', 'DBL') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 34, 'Dundee', 'DND') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 35, 'Frascati', 'FRS') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 36, 'Hamburg', 'HBG') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 37, 'Helsinki', 'HSK') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 38, 'Krakow', 'KKW') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 39, 'Lannion', 'LNN') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 40, 'Lasham', 'LSH') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 41, 'Madrid', 'MDR') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 42, 'Norrkoping', 'NRK') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 43, 'Offenbach', 'OFB') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 44, 'Oslo', 'OSL') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 45, 'Prague', 'PRG') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 46, 'Rome', 'RM') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 47, 'Spitzenbergen', 'SPT') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 48, 'Traben-Trar', 'TBT') ;
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 49, 'Bedford', 'BDF') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 50, 'Downsview', 'DWN') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 51, 'Edmonton', 'EDM') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 52, 'BantonRouge', 'BTN') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 53, 'Miami', 'MIM') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 54, 'GilmoreCreek', 'GLC') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 55, 'Nesdis', 'NSD') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 56, 'WallopsIsl', 'WLI') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 57, 'RedwoodCity', 'RWC') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 58, 'ScippsInstitute', 'SIN') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 59, 'SiouxFalls', 'SFL') ;
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 60, 'StennisSpace', 'SNS') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 61, 'UnivOfAlaska', 'UNA') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 62, 'UnivOfTexas', 'UNT') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 63, 'UnivOfRhodeIsl', 'UNR') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 64, 'Cachoeira', 'CPA') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 65, 'Funceme', 'FNC') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 66, 'Santiago', 'SNT') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 67, 'Adelaide', 'ADL') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 68, 'Aspendale', 'APD') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 69, 'Honolulu', 'HNL') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 70, 'Perth', 'PTH') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 71, 'Sydney', 'SYD') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 72, 'Townsville', 'TSV') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 73, 'Wellington', 'WLL') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 74, 'Beijing', 'BJG') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 75, 'Dhaka', 'DHK') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 76, 'Da-Xing', 'DAX') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 77, 'Keelung', 'KLG') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 78, 'Kiyose', 'KYS') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 79, 'Lapan', 'LPN') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 80, 'Manila', 'MNL') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 81, 'NewDelhi', 'NDY') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 82, 'Riyadh', 'RYD') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 83, 'Singapore', 'SNG') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 84, 'Selangor', 'SLG') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 85, 'Seoul', 'SU') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 86, 'SeoulUniv', 'SUU') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 87, 'Sendai', 'SND') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 88, 'TokaiUniv', 'TAU') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 89, 'TokyoUniv', 'TKU') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 90, 'Taipei', 'TIP') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 91, 'KualaLumpur', 'KLL') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 92, 'Casey', 'CSY') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 93, 'McMurdo', 'MMR') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 94, 'Palmer', 'PLM') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 95, 'TerranovaBay', 'TNV') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 96, 'Cairo', 'CRO') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 97, 'Agrhymet', 'AGY') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 98, 'Nairobi', 'NRB') ;
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES ( 99, 'LaReunion', 'LRN') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES (100, 'Tunis', 'TNS') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES (101, 'RRSC_Nairobi', 'RSC') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES (102, 'Norman', 'NRM') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES (103, 'Cordoba', 'CDB') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym, position) VALUES (104, 'Matera', 'MAT', 'SRID=4326; POINT(16.704017 40.649513)') ;
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES (105, 'Atlanta', 'ATL') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES (106, 'Libreville', 'LBV') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES (108, 'Neustrelitz', 'NST') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES (109, 'Malindi', 'MLD') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES (110, 'Moscow', 'MSC') ;
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES (111, 'Kourou', 'KUR') ; 
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES (112, 'Bishkek', 'BSK') ;	   
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES (113, 'Kitab', 'KTB') ;
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES (114, 'UlanBator', 'ULB') ;
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES (115,'Itu', 'ITU') ;
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES (116,'KhantyMansiysk', 'KMK') ;
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES (117,'Tripoli', 'TRI') ;
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES (118,'Morzuk', 'MZK') ;
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES (119,'Athens', 'ATH') ;
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES (120,'Gander', 'GAN') ;
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES (121,'Kangerlussuaq', 'KAN') ;
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES (122,'Monterey', 'MON') ;
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES (123,'Svalbard', 'SVA') ;
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES (124,'Milano', 'EPS') ;
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES (125,'Greenbelt', 'GRN') ;
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES (126,'Darmstadt', 'DRM') ;
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES (127,'Toulouse', 'TLS') ;
INSERT INTO t_stations (stationid, stationname, stationacronym) VALUES (128,'London', 'LND') ;
