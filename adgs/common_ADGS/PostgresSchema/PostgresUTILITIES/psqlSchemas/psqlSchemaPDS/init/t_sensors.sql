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
	//	t_sensors 
	//	sensorid, sensorname
	//////////////////////////////////////////////
*/
INSERT INTO t_sensors VALUES (1, 'MSS', 'MultiSpectral Scanner'); 
INSERT INTO t_sensors VALUES (2, 'TM', 'Thematic Mapper'); 
INSERT INTO t_sensors VALUES (3, 'ETM', 'Enhanced Thematic Mapper'); 
INSERT INTO t_sensors VALUES (4, 'RBV', 'Return Beam Vidicon'); 
INSERT INTO t_sensors VALUES (5, 'MESSR', 'Multispectral Electronic Self-Scanning Radiometer'); 
INSERT INTO t_sensors VALUES (6, 'VNIR', 'Very Near Infrared Radiometer'); 
INSERT INTO t_sensors VALUES (7, 'SWIR', 'Short Wave Infrared Radiometer'); 
INSERT INTO t_sensors VALUES (8, 'Pan', ''); 
INSERT INTO t_sensors VALUES (9, 'Xs', ''); 
INSERT INTO t_sensors VALUES (10,'AMI-SAR', 'Active Microwave Instrument - SAR mode'); 
INSERT INTO t_sensors VALUES (11,'ATSR', 'Along Track Scanning Radiometer'); 
INSERT INTO t_sensors VALUES (12,'AVHRR', 'Advanced Very High Resolution Radiometer'); 
INSERT INTO t_sensors VALUES (13,'SeaWiFS', 'Sea-viewing Wide Field-of-view Sensor'); 
INSERT INTO t_sensors VALUES (14,'M', ''); 
INSERT INTO t_sensors VALUES (15,'Xi', ''); 
INSERT INTO t_sensors VALUES (16,'JERS-SAR', '');
INSERT INTO t_sensors VALUES (17,'MODIS', 'Moderate Resolution Imaging Spectroradiometer');
INSERT INTO t_sensors VALUES (18,'SAR', 'Synthetic Aperture Radar');
INSERT INTO t_sensors VALUES (19,'SIRAL', 'Synthetic Aperture Interferometric Radar Altimeter');
INSERT INTO t_sensors VALUES (20,'DORIS', 'Doppler Orbitography and Radiopositioning Integrated by Satellite');
INSERT INTO t_sensors VALUES (21,'ASAR', 'Advanced Synthetic Aperture Radar');
INSERT INTO t_sensors VALUES (22,'H1A', '');
INSERT INTO t_sensors VALUES (23,'H2A', '');
INSERT INTO t_sensors VALUES (24,'EGG', 'Electrostatic Gravity Gradiometer');
INSERT INTO t_sensors VALUES (25,'SST', 'Satellite-to-Satellite Tracking');
INSERT INTO t_sensors VALUES (26,'ikonos', 'high-resolution satellite');
INSERT INTO t_sensors VALUES (27,'predator', 'MQ-1 Predator unmanned aircraft');
INSERT INTO t_sensors VALUES (28,'BGIS-2000', 'Ball Global Imaging System 2000');
INSERT INTO t_sensors VALUES (29,'MERIS', 'Medium Resolution Imaging Spectrometer');
INSERT INTO t_sensors VALUES (30,'MIPAS', 'Michelson Interferometer for Passive Atmospheric Sounding');
INSERT INTO t_sensors VALUES (31,'GOMOS', 'Global Ozone Monitoring by Occultation of Stars');
INSERT INTO t_sensors VALUES (32,'SCIAMACHY', 'SCanning Imaging Absorption SpectroMeter for Atmospheric CartograpHY');
INSERT INTO t_sensors VALUES (33,'AATSR', 'Advanced Along-Track Scanning Radiometer'); 
INSERT INTO t_sensors VALUES (34,'RA2-MWR', 'Radar Altimeter 2 - Microwave Radiometer'); 
INSERT INTO t_sensors VALUES (35,'STARTRK', 'Star Tracker');
INSERT INTO t_sensors VALUES (36,'ALADIN', 'Atmospheric LAser Doppler INstrument');
INSERT INTO t_sensors VALUES (37,'VSSC', 'VENuS Super Spectral Camera');
INSERT INTO t_sensors VALUES (38,'HIRS', 'High-resolution Infrared Radiation Sounder');
INSERT INTO t_sensors VALUES (39,'AMSU-A', 'Advanced Microwave Sounding Unit-A');
INSERT INTO t_sensors VALUES (40,'MHS', 'Microwave Humidity Sounder');
INSERT INTO t_sensors VALUES (41,'A-DCS', 'Argos Advanced Data Collection System');
INSERT INTO t_sensors VALUES (42,'SEM', 'Space Environmental Monitor');
INSERT INTO t_sensors VALUES (43,'IASI', 'Infrared Atmospheric Sounding Interferometer');
INSERT INTO t_sensors VALUES (44,'ASCAT', 'Advanced SCATterometer');
INSERT INTO t_sensors VALUES (45,'GRAS', 'Global Navigation Satellite System Receiver for Atmospheric Sounding');
INSERT INTO t_sensors VALUES (46,'GOME', 'Global Ozone Monitoring Experiment');
INSERT INTO t_sensors VALUES (47,'PRISM', 'Panchromatic Remote-sensing Instrument for Stereo Mapping');
INSERT INTO t_sensors VALUES (48,'AVNIR-2', 'Advanced Visible and Near Infrared Radiometer type 2');
INSERT INTO t_sensors VALUES (49,'PALSAR', 'Phased Array type L-band Synthetic Aperture Radar');
INSERT INTO t_sensors VALUES (50,'SEVIRI', 'Spinning Enhanced Visible and Infrared Imager');
INSERT INTO t_sensors VALUES (51,'AMSU-B', 'Advanced Microwave Sounding Unit-B');
INSERT INTO t_sensors VALUES (52,'SSMI', 'Special Sensor Microwave Imager');
INSERT INTO t_sensors VALUES (53,'SSMIS', 'Special Sensor Microwave Imager / Sounder');
INSERT INTO t_sensors VALUES (54,'ASTER', 'Advanced Spaceborne Thermal Emission and Reflection Radiometer');
INSERT INTO t_sensors VALUES (55,'AMSR-E', 'Advanced Microwave Scanning Radiometer - Earth Observing System');
INSERT INTO t_sensors VALUES (56,'DUMMY', 'fake satellite for test');
INSERT INTO t_sensors VALUES (57,'X-SAR', 'X-band Synthetic Aperture Radar');
INSERT INTO t_sensors VALUES (58,'OCM', 'Ocean Colour Monitor');
INSERT INTO t_sensors VALUES (59,'SCAT', 'Scanning Scatterometer');
INSERT INTO t_sensors VALUES (60,'ROSA', 'Radio Occultation Sounder for Atmospheric Studies');
INSERT INTO t_sensors VALUES (61,'POSEIDON-1', 'Poseidon-1 Radar Altimeter');
INSERT INTO t_sensors VALUES (62,'POSEIDON-2', 'Poseidon-2 Radar Altimeter');
INSERT INTO t_sensors VALUES (63,'POSEIDON-3', 'Poseidon-3 Radar Altimeter');
INSERT INTO t_sensors VALUES (64,'JMR', 'Jason-1 Microwave Radiometer');
INSERT INTO t_sensors VALUES (65,'AMR', 'Advanced Microwave Radiometer');
INSERT INTO t_sensors VALUES (66,'MSI', 'Multi-Spectral Instrument');
INSERT INTO t_sensors VALUES (67,'OLCI', 'Ocean & Land Color Instrument');
INSERT INTO t_sensors VALUES (68,'SLSTR', 'Sea and Land Surface Temperature Radiometer');
INSERT INTO t_sensors VALUES (69,'SRAL', 'SAR Radar Altimeter');
INSERT INTO t_sensors VALUES (70,'MWR', 'MicroWave Radiometer');
INSERT INTO t_sensors VALUES (71,'SYNERGY', 'Synergy Products');
INSERT INTO t_sensors VALUES (72,'GNSS', 'Global Navigation Satellite System');
INSERT INTO t_sensors VALUES (73,'HKTM', 'House Keeping Telemetry');
INSERT INTO t_sensors VALUES (74,'NAVATT', 'Navigation and Attitude data');
INSERT INTO t_sensors VALUES (75, 'OLI', 'Operational Land Imager');
INSERT INTO t_sensors VALUES (76, 'TIRS', 'Thermal InfraRed Sensor');
INSERT INTO t_sensors VALUES (77, 'HYP_PAN', 'Advanced hyperspectral instrument including a panchromatic camera at medium resolution');
INSERT INTO t_sensors VALUES (78, 'ATLID', 'Atmospheric Lidar' );
INSERT INTO t_sensors VALUES (79, 'BBR', 'Broad Band Radiometer' );
INSERT INTO t_sensors VALUES (80, 'CPR', 'Cloud Profiling Radar' );

