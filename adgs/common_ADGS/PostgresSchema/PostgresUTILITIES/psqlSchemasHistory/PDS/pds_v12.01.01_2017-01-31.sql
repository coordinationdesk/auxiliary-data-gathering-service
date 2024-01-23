
-- Version 12.01.01
-- POSTGRES








--------------------------------------------------------------------------------
--------------------------------- TABLES INSERT --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_satellites
*/ ------------------------------
INSERT INTO t_satellites (satelliteid, satellitename, satelliteacronym) VALUES (44, 'Prisma', 'PRS') ;


/* ------------------------------
	t_sensors
*/ ------------------------------
INSERT INTO t_sensors VALUES (77, 'HYP_PAN', 'Advanced hyperspectral instrument including a panchromatic camera at medium resolution');


/* ------------------------------
	t_satrelations
*/ ------------------------------
----- Prisma -----
INSERT INTO t_satrelations VALUES (44, 1, 77) ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'pds-12.01.01' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-12.01.01',
		'2017-01-31',
		3,
		2,
		'PDS_12-1-1',
		'Added Prisma satellite, HYP_PAN sensor and sat/sen relations'
) ;


