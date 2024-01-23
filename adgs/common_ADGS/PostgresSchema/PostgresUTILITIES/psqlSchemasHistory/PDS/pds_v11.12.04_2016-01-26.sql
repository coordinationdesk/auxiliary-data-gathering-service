
-- Version 11.12.04
-- POSTGRES








--------------------------------------------------------------------------------
--------------------------------- TABLES INSERT --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_sensors
*/ ------------------------------
INSERT INTO t_sensors VALUES (75, 'OLI', 'Operational Land Imager');
INSERT INTO t_sensors VALUES (76, 'TIRS', 'Thermal InfraRed Sensor');

/* ------------------------------
	t_satrelations
*/ ------------------------------
-- Landsat 8
INSERT INTO t_satrelations VALUES (1, 8, 75) ;
INSERT INTO t_satrelations VALUES (1, 8, 76) ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'pds-11.12.04' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-11.12.04',
		'2016-01-26',
		3,
		2,
		'PDS_11-12-4',
		'Added the Landsat 8 satellite/mission/sensors'
) ;


