
-- Version 7.2.1
-- POSTGRES








---------------------------------------------------------------------------------
------------------------------- TABLES ALTERATION -------------------------------
---------------------------------------------------------------------------------

--------------------
--- T_SATELLITES ---
--------------------
INSERT INTO "t_satellites" VALUES (29,'Dummysat', 'DMY') ;
INSERT INTO "t_satellites" VALUES (30,'SRTM', 'SRT') ;
INSERT INTO "t_satellites" VALUES (31,'Oceansat2', 'OS2') ;
INSERT INTO "t_satellites" VALUES (32,'Sacd', 'SCD') ;

-----------------
--- T_SENSORS ---
-----------------
INSERT INTO "t_sensors" VALUES (56,'DUMMY');
INSERT INTO "t_sensors" VALUES (57,'X-SAR');
INSERT INTO "t_sensors" VALUES (58,'OCM');
INSERT INTO "t_sensors" VALUES (59,'SCAT');
INSERT INTO "t_sensors" VALUES (60,'ROSA');

----------------------
--- T_SATRELATIONS ---
----------------------
INSERT INTO "t_satrelations" VALUES (8, 19, 12) ; 
INSERT INTO "t_satrelations" VALUES (8, 19, 39) ;
INSERT INTO "t_satrelations" VALUES (8, 19, 40) ;
INSERT INTO "t_satrelations" VALUES (29, 1, 56) ; 
INSERT INTO "t_satrelations" VALUES (30, 1, 57) ;
INSERT INTO "t_satrelations" VALUES (31, 1, 58) ;
INSERT INTO "t_satrelations" VALUES (31, 1, 59) ;
INSERT INTO "t_satrelations" VALUES (31, 1, 60) ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------
INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'7.2.1',
		'2009-04-22',
		3,
		1,
		'SchemaVersion_7-2-1',
		'Added new satellites, sensors and relations.'
) ;
