
-- Version 11.13.00
-- POSTGRES








--------------------------------------------------------------------------------
-------------------------------- TABLES CREATION -------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_serials
*/ ------------------------------
CREATE TABLE t_serials (
    table_name VARCHAR(30) NOT NULL,
	field_name VARCHAR(30) NOT NULL
) ;
ALTER TABLE t_serials
	ADD CONSTRAINT pk_t_serials
		PRIMARY KEY (table_name, field_name) ;








--------------------------------------------------------------------------------
-------------------------------- TABLES CHANGES --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_stations
*/ ------------------------------
ALTER TABLE t_stations ADD position VARCHAR(60) ;

UPDATE t_stations SET position = 'SRID=4326; POINT(13.601927 41.978887)' WHERE stationacronym = 'FUC' ;
UPDATE t_stations SET position = 'SRID=4326; POINT(20.964325 67.857128)' WHERE stationacronym = 'KIS' ;
UPDATE t_stations SET position = 'SRID=4326; POINT(-15.633809 27.762889)' WHERE stationacronym = 'MAS' ;
UPDATE t_stations SET position = 'SRID=4326; POINT(16.704017 40.649513)' WHERE stationacronym = 'MAT' ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'pds-11.13.00' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-11.13.00',
		'2016-02-01',
		3,
		2,
		'PDS_11-13-0',
		'Added table t_serials. Added field position on t_stations'
) ;


