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
	//	t_satrelations 
	//	satelliteid - mission - sensorid relations
	//////////////////////////////////////////////
*/
----- LANDSAT -----
-- Landsat 1
INSERT INTO t_satrelations VALUES (1, 1, 1) ; 
INSERT INTO t_satrelations VALUES (1, 1, 4) ; 
-- Landsat 2
INSERT INTO t_satrelations VALUES (1, 2, 1) ;
INSERT INTO t_satrelations VALUES (1, 2, 4) ;
-- Landsat 3
INSERT INTO t_satrelations VALUES (1, 3, 1) ; 
INSERT INTO t_satrelations VALUES (1, 3, 4) ; 
-- Landsat 4
INSERT INTO t_satrelations VALUES (1, 4, 1) ; 
INSERT INTO t_satrelations VALUES (1, 4, 2) ; 
-- Landsat 5
INSERT INTO t_satrelations VALUES (1, 5, 1) ; 
INSERT INTO t_satrelations VALUES (1, 5, 2) ; 
-- Landsat 7
INSERT INTO t_satrelations VALUES (1, 7, 3) ; 
-- Landsat 8
INSERT INTO t_satrelations VALUES (1, 8, 75) ;
INSERT INTO t_satrelations VALUES (1, 8, 76) ;


----- MOS -----
-- MOS 1
INSERT INTO t_satrelations VALUES (2, 1, 5) ; 


----- J-ERS -----
-- J-Ers 1
INSERT INTO t_satrelations VALUES (3, 1, 6) ; 
INSERT INTO t_satrelations VALUES (3, 1, 7) ; 
INSERT INTO t_satrelations VALUES (3, 1, 16) ; 


----- SPOT -----
-- Spot 1
INSERT INTO t_satrelations VALUES (4, 1, 8) ; 
INSERT INTO t_satrelations VALUES (4, 1, 9) ; 
-- Spot 2
INSERT INTO t_satrelations VALUES (4, 2, 8) ; 
INSERT INTO t_satrelations VALUES (4, 2, 9) ; 
-- Spot 3
INSERT INTO t_satrelations VALUES (4, 3, 8) ; 
INSERT INTO t_satrelations VALUES (4, 3, 9) ; 
-- Spot 4
INSERT INTO t_satrelations VALUES (4, 4, 8) ; 
INSERT INTO t_satrelations VALUES (4, 4, 9) ; 
INSERT INTO t_satrelations VALUES (4, 4, 14) ; 
INSERT INTO t_satrelations VALUES (4, 4, 15) ; 
-- Spot 5
INSERT INTO t_satrelations VALUES (4, 5, 9) ; 


----- ERS -----
-- Ers 1
INSERT INTO t_satrelations VALUES (5, 1, 10) ; 
INSERT INTO t_satrelations VALUES (5, 1, 11) ; 
INSERT INTO t_satrelations VALUES (5, 1, 18) ;
-- Ers 2
INSERT INTO t_satrelations VALUES (5, 2, 10) ; 
INSERT INTO t_satrelations VALUES (5, 2, 11) ; 
INSERT INTO t_satrelations VALUES (5, 2, 18) ; 


----- RadarSat -----
INSERT INTO t_satrelations VALUES (7, 1, 18) ;


----- NOAA -----
-- Noaa 1
INSERT INTO t_satrelations VALUES (8, 1, 12) ; 
INSERT INTO t_satrelations VALUES (8, 1, 39) ; 
INSERT INTO t_satrelations VALUES (8, 1, 51) ;
-- Noaa 2
INSERT INTO t_satrelations VALUES (8, 2, 12) ; 
INSERT INTO t_satrelations VALUES (8, 2, 39) ; 
INSERT INTO t_satrelations VALUES (8, 2, 51) ;
-- Noaa 3
INSERT INTO t_satrelations VALUES (8, 3, 12) ; 
INSERT INTO t_satrelations VALUES (8, 3, 39) ;
INSERT INTO t_satrelations VALUES (8, 3, 51) ;  
-- Noaa 4
INSERT INTO t_satrelations VALUES (8, 4, 12) ; 
INSERT INTO t_satrelations VALUES (8, 4, 39) ; 
INSERT INTO t_satrelations VALUES (8, 4, 51) ; 
-- Noaa 5
INSERT INTO t_satrelations VALUES (8, 5, 12) ; 
INSERT INTO t_satrelations VALUES (8, 5, 39) ; 
INSERT INTO t_satrelations VALUES (8, 5, 51) ;
-- Noaa 6
INSERT INTO t_satrelations VALUES (8, 6, 12) ; 
INSERT INTO t_satrelations VALUES (8, 6, 39) ; 
INSERT INTO t_satrelations VALUES (8, 6, 51) ;
-- Noaa 7
INSERT INTO t_satrelations VALUES (8, 7, 12) ; 
INSERT INTO t_satrelations VALUES (8, 7, 39) ; 
INSERT INTO t_satrelations VALUES (8, 7, 51) ;
-- Noaa 8
INSERT INTO t_satrelations VALUES (8, 8, 12) ; 
INSERT INTO t_satrelations VALUES (8, 8, 39) ; 
INSERT INTO t_satrelations VALUES (8, 8, 51) ;
-- Noaa 9
INSERT INTO t_satrelations VALUES (8, 9, 12) ;
INSERT INTO t_satrelations VALUES (8, 9, 39) ; 
INSERT INTO t_satrelations VALUES (8, 9, 51) ;  
-- Noaa 10
INSERT INTO t_satrelations VALUES (8, 10, 12) ; 
INSERT INTO t_satrelations VALUES (8, 10, 39) ; 
INSERT INTO t_satrelations VALUES (8, 10, 51) ;
-- Noaa 11
INSERT INTO t_satrelations VALUES (8, 11, 12) ; 
INSERT INTO t_satrelations VALUES (8, 11, 39) ; 
INSERT INTO t_satrelations VALUES (8, 11, 51) ;
-- Noaa 12
INSERT INTO t_satrelations VALUES (8, 12, 12) ; 
INSERT INTO t_satrelations VALUES (8, 12, 39) ; 
INSERT INTO t_satrelations VALUES (8, 12, 51) ;
-- Noaa 13
INSERT INTO t_satrelations VALUES (8, 13, 12) ; 
INSERT INTO t_satrelations VALUES (8, 13, 39) ; 
INSERT INTO t_satrelations VALUES (8, 13, 51) ;
-- Noaa 14
INSERT INTO t_satrelations VALUES (8, 14, 12) ;
INSERT INTO t_satrelations VALUES (8, 14, 39) ; 
INSERT INTO t_satrelations VALUES (8, 14, 51) ;
-- Noaa 15
INSERT INTO t_satrelations VALUES (8, 15, 12) ; 
INSERT INTO t_satrelations VALUES (8, 15, 39) ; 
INSERT INTO t_satrelations VALUES (8, 15, 51) ;
-- Noaa 16
INSERT INTO t_satrelations VALUES (8, 16, 12) ; 
INSERT INTO t_satrelations VALUES (8, 16, 39) ; 
INSERT INTO t_satrelations VALUES (8, 16, 51) ;
-- Noaa 17
INSERT INTO t_satrelations VALUES (8, 17, 12) ;
INSERT INTO t_satrelations VALUES (8, 17, 39) ;
-- Noaa 18
INSERT INTO t_satrelations VALUES (8, 18, 12) ;
INSERT INTO t_satrelations VALUES (8, 18, 39) ; 
INSERT INTO t_satrelations VALUES (8, 18, 40) ;
-- Noaa 19
INSERT INTO t_satrelations VALUES (8, 19, 12) ; 
INSERT INTO t_satrelations VALUES (8, 19, 39) ;
INSERT INTO t_satrelations VALUES (8, 19, 40) ;


----- ORBVIEW -----
INSERT INTO t_satrelations VALUES (9, 2, 13) ; 


----- EOS -----
-- eos-am
INSERT INTO t_satrelations VALUES (10, 1, 17) ; 
INSERT INTO t_satrelations VALUES (10, 1, 54) ; 
-- eos-pm
INSERT INTO t_satrelations VALUES (11, 1, 17) ; 
INSERT INTO t_satrelations VALUES (11, 1, 55) ;


----- COSMO -----
-- Cosmo 1
INSERT INTO t_satrelations VALUES (15, 1, 18) ;
-- Cosmo 2
INSERT INTO t_satrelations VALUES (15, 2, 18) ;
-- Cosmo 3
INSERT INTO t_satrelations VALUES (15, 3, 18) ;
-- Cosmo 4
INSERT INTO t_satrelations VALUES (15, 4, 18) ;


----- COSMO SG -----
-- CosmoSG 1
INSERT INTO t_satrelations VALUES (43, 1, 18) ;
-- CosmoSG 2
INSERT INTO t_satrelations VALUES (43, 2, 18) ;


----- CRYOSAT -----
INSERT INTO t_satrelations VALUES (16, 1, 19) ;
INSERT INTO t_satrelations VALUES (16, 1, 20) ;
INSERT INTO t_satrelations VALUES (16, 1, 35) ;


----- ENVISAT -----
INSERT INTO t_satrelations VALUES (17, 1, 20) ;
INSERT INTO t_satrelations VALUES (17, 1, 21) ;
INSERT INTO t_satrelations VALUES (17, 1, 29) ;
INSERT INTO t_satrelations VALUES (17, 1, 30) ;
INSERT INTO t_satrelations VALUES (17, 1, 31) ;
INSERT INTO t_satrelations VALUES (17, 1, 32) ;
INSERT INTO t_satrelations VALUES (17, 1, 33) ;
INSERT INTO t_satrelations VALUES (17, 1, 34) ;


----- GOCE -----
INSERT INTO t_satrelations VALUES (19, 1, 24) ;
INSERT INTO t_satrelations VALUES (19, 1, 25) ;
INSERT INTO t_satrelations VALUES (19, 1, 35) ;


----- IKONOS -----
INSERT INTO t_satrelations VALUES (20, 1, 26) ;


----- PREDATOR -----
INSERT INTO t_satrelations VALUES (21, 1, 27) ;


----- QUICKBIRD -----
INSERT INTO t_satrelations VALUES (22, 1, 28) ;


----- AEOLUS -----
INSERT INTO t_satrelations VALUES (23, 1, 36) ;


----- VENUS -----
INSERT INTO t_satrelations VALUES (24, 1, 37) ;


----- ALOS -----
INSERT INTO t_satrelations VALUES (25, 1, 47) ;
INSERT INTO t_satrelations VALUES (25, 1, 48) ;
INSERT INTO t_satrelations VALUES (25, 1, 49) ;


----- MetOp-A -----
INSERT INTO t_satrelations VALUES (26, 1, 12) ;
INSERT INTO t_satrelations VALUES (26, 1, 18) ;
INSERT INTO t_satrelations VALUES (26, 1, 38) ;
INSERT INTO t_satrelations VALUES (26, 1, 39) ;
INSERT INTO t_satrelations VALUES (26, 1, 40) ;
INSERT INTO t_satrelations VALUES (26, 1, 41) ;
INSERT INTO t_satrelations VALUES (26, 1, 42) ;
INSERT INTO t_satrelations VALUES (26, 1, 43) ;
INSERT INTO t_satrelations VALUES (26, 1, 44) ;
INSERT INTO t_satrelations VALUES (26, 1, 45) ;
INSERT INTO t_satrelations VALUES (26, 1, 46) ;


----- MSG -----
INSERT INTO t_satrelations VALUES (27, 2, 50) ;


----- AIRFORCE -----
-- AirForce 10
INSERT INTO t_satrelations VALUES (28, 10, 52) ;
-- AirForce 111
INSERT INTO t_satrelations VALUES (28, 11, 52) ;
-- AirForce 12
INSERT INTO t_satrelations VALUES (28, 12, 52) ;
-- AirForce 13
INSERT INTO t_satrelations VALUES (28, 13, 52) ;
-- AirForce 14
INSERT INTO t_satrelations VALUES (28, 14, 52) ;
-- AirForce 15
INSERT INTO t_satrelations VALUES (28, 15, 52) ;
-- AirForce 16
INSERT INTO t_satrelations VALUES (28, 16, 53) ;
-- AirForce 17
INSERT INTO t_satrelations VALUES (28, 17, 53) ;


----- Dummysat -----
INSERT INTO t_satrelations VALUES (29, 1, 56) ; 


----- SRTM -----
INSERT INTO t_satrelations VALUES (30, 1, 57) ;


----- Oceansat -----
INSERT INTO t_satrelations VALUES (31, 1, 58) ;
INSERT INTO t_satrelations VALUES (31, 1, 59) ;
INSERT INTO t_satrelations VALUES (31, 1, 60) ;


----- Sacd -----
INSERT INTO t_satrelations VALUES (32, 1, 60) ;


----- JASON -----
-- Jason 1
INSERT INTO t_satrelations VALUES (33, 1, 62) ;
INSERT INTO t_satrelations VALUES (33, 1, 64) ;
-- Jason 2
INSERT INTO t_satrelations VALUES (33, 2, 63) ;
INSERT INTO t_satrelations VALUES (33, 2, 65) ;


----- SENTINEL -----
-- Sentinel-1 1
INSERT INTO t_satrelations VALUES (34, 1, 18) ;
-- Sentinel-1 2
INSERT INTO t_satrelations VALUES (34, 2, 18) ;
-- Sentinel-1A 1
INSERT INTO t_satrelations VALUES (35, 1, 18) ;
-- Sentinel-1B 1
INSERT INTO t_satrelations VALUES (36, 1, 18) ;
-- Sentinel-2 1
INSERT INTO t_satrelations VALUES (37, 1, 66) ;
-- Sentinel-2 2
INSERT INTO t_satrelations VALUES (37, 2, 66) ;
-- Sentinel-2A 1
INSERT INTO t_satrelations VALUES (38, 1, 66) ;
-- Sentinel-2B 1
INSERT INTO t_satrelations VALUES (39, 1, 66) ;
-- Sentinel-3 1
INSERT INTO t_satrelations VALUES (40, 1, 2) ;
INSERT INTO t_satrelations VALUES (40, 1, 20) ;
INSERT INTO t_satrelations VALUES (40, 1, 67) ;
INSERT INTO t_satrelations VALUES (40, 1, 68) ;
INSERT INTO t_satrelations VALUES (40, 1, 69) ;
INSERT INTO t_satrelations VALUES (40, 1, 70) ;
INSERT INTO t_satrelations VALUES (40, 1, 71) ;
INSERT INTO t_satrelations VALUES (40, 1, 72) ;
INSERT INTO t_satrelations VALUES (40, 1, 73) ;
INSERT INTO t_satrelations VALUES (40, 1, 74) ;
-- Sentinel-3 2
INSERT INTO t_satrelations VALUES (40, 2, 2) ;
INSERT INTO t_satrelations VALUES (40, 2, 20) ;
INSERT INTO t_satrelations VALUES (40, 2, 67) ;
INSERT INTO t_satrelations VALUES (40, 2, 68) ;
INSERT INTO t_satrelations VALUES (40, 2, 69) ;
INSERT INTO t_satrelations VALUES (40, 2, 70) ;
INSERT INTO t_satrelations VALUES (40, 2, 71) ;
INSERT INTO t_satrelations VALUES (40, 2, 72) ;
INSERT INTO t_satrelations VALUES (40, 2, 73) ;
INSERT INTO t_satrelations VALUES (40, 2, 74) ;
-- Sentinel-3A 1
INSERT INTO t_satrelations VALUES (41, 1, 2) ;
INSERT INTO t_satrelations VALUES (41, 1, 20) ;
INSERT INTO t_satrelations VALUES (41, 1, 67) ;
INSERT INTO t_satrelations VALUES (41, 1, 68) ;
INSERT INTO t_satrelations VALUES (41, 1, 69) ;
INSERT INTO t_satrelations VALUES (41, 1, 70) ;
-- Sentinel-3B 1
INSERT INTO t_satrelations VALUES (42, 1, 2) ;
INSERT INTO t_satrelations VALUES (42, 1, 20) ;
INSERT INTO t_satrelations VALUES (42, 1, 67) ;
INSERT INTO t_satrelations VALUES (42, 1, 68) ;
INSERT INTO t_satrelations VALUES (42, 1, 69) ;
INSERT INTO t_satrelations VALUES (42, 1, 70) ;


----- Prisma -----
INSERT INTO t_satrelations VALUES (44, 1, 77) ;


----- EarthCARE -----
INSERT INTO t_satrelations VALUES (45, 1, 78) ;
INSERT INTO t_satrelations VALUES (45, 1, 79) ;
INSERT INTO t_satrelations VALUES (45, 1, 80) ;


