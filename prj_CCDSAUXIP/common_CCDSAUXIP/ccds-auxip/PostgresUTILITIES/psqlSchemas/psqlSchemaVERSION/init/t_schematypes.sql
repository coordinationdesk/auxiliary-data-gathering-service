/*

Copyright 2010-2015, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S. T_SchemaTypes population script $

	$Id$

	$Author$

*/





/* 
	//////////////////////////////////////////////
	//	T_SchemaTypes 
	//	Types of schemas
	//////////////////////////////////////////////
*/
INSERT INTO t_schematypes (id, schematype) VALUES (1, 'UNKNOWN') ;
INSERT INTO t_schematypes (id, schematype) VALUES (2, 'PDS') ;
INSERT INTO t_schematypes (id, schematype) VALUES (3, 'CATALOGUE') ;
INSERT INTO t_schematypes (id, schematype) VALUES (4, 'ISM') ;
INSERT INTO t_schematypes (id, schematype) VALUES (5, 'USDFWA') ;
INSERT INTO t_schematypes (id, schematype) VALUES (6, 'BASELINE') ;
INSERT INTO t_schematypes (id, schematype) VALUES (7, 'DATA MINING') ;
INSERT INTO t_schematypes (id, schematype) VALUES (8, 'INDEPENDED') ;
INSERT INTO t_schematypes (id, schematype) VALUES (9, 'SPECIFIC FOR PROJECT') ;
INSERT INTO t_schematypes (id, schematype) VALUES (10,'HARM') ;
INSERT INTO t_schematypes (id, schematype) VALUES (11,'COM') ;
INSERT INTO t_schematypes (id, schematype) VALUES (12,'DEEGREEBRIM') ;
INSERT INTO t_schematypes (id, schematype) VALUES (13,'DEEGREEFEATURES') ;
INSERT INTO t_schematypes (id, schematype) VALUES (14,'GEOSERVER');
INSERT INTO t_schematypes (id, schematype) VALUES (15,'JOUWS');
INSERT INTO t_schematypes (id, schematype) VALUES (16,'OAS');
INSERT INTO t_schematypes (id, schematype) VALUES (17,'POR');
INSERT INTO t_schematypes (id, schematype) VALUES (18,'SIB');
INSERT INTO t_schematypes (id, schematype) VALUES (19,'WUP');
INSERT INTO t_schematypes (id, schematype) VALUES (20,'CIRCULATION') ;
INSERT INTO t_schematypes (id, schematype) VALUES (21,'PDS-2') ;
INSERT INTO t_schematypes (id, schematype) VALUES (22,'REPORTS') ;
INSERT INTO t_schematypes (id, schematype) VALUES (23,'INVBASELINE') ;
INSERT INTO t_schematypes (id, schematype) VALUES (24,'COORDINATED RELEASE') ;
INSERT INTO t_schematypes (id, schematype) VALUES (25,'COVERAGE') ;
INSERT INTO t_schematypes (id, schematype, description) VALUES (26, 'MPM', 'Mission Performance Monitoring') ;
INSERT INTO t_schematypes (id, schematype, description) VALUES (27, 'ADMBASELINE', 'ADM Aeolus APF Baseline system') ;
INSERT INTO t_schematypes (id, schematype, description) VALUES (28, 'IPF', 'Cryosat2 IPF system test schema') ;
INSERT INTO t_schematypes (id, schematype, description) VALUES (29, 'SNAV', 'SpaceNav schema') ;
INSERT INTO t_schematypes (id, schematype, description) VALUES (30, 'ECI', 'Earth Care schema') ;
INSERT INTO t_schematypes (id, schematype, description) VALUES (31, 'FSSM', 'File System Storage Manager schema') ;
INSERT INTO t_schematypes (id, schematype, description) VALUES (32, 'POSTEXETT', 'Post Execution TaskTable schema') ;
INSERT INTO t_schematypes (id, schematype, description) VALUES (33, 'PGCRON', 'Schema for pg_cron logging') ;
INSERT INTO t_schematypes (id, schematype, description) VALUES (34, 'LTAQ', 'LTA Qualification') ;
INSERT INTO t_schematypes (id, schematype, description) VALUES (35, 'SRV', NULL) ;
INSERT INTO t_schematypes (id, schematype, description) VALUES (36, 'CRS', 'Clean Remote Sources') ;
INSERT INTO t_schematypes (id, schematype, description) VALUES (37, 'LTADL', NULL) ;
INSERT INTO t_schematypes (id, schematype, description) VALUES (38, 'TRANS', 'State transition plug-in schema') ;
INSERT INTO t_schematypes (id, schematype, description) VALUES (39, 'VRSN', 'Versioning schema') ;
INSERT INTO t_schematypes (id, schematype, description) VALUES (40, 'COMMON', 'Common schema') ;
INSERT INTO t_schematypes (id, schematype, description) VALUES (41, 'MMO', 'MultiMission Ordering DB') ;
INSERT INTO t_schematypes (id, schematype, description) VALUES (42, 'BROADCAST', 'Broadcast Events Messages Schema') ;



