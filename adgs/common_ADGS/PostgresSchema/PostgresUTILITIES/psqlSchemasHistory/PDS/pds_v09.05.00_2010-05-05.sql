
-- Version 9.5.0
-- POSTGRES








---------------------------------------------------------------------------------
------------------------------ TABLES MODIFICATIONS -----------------------------
---------------------------------------------------------------------------------

-----------------
--- T_SENSORS ---
-----------------
ALTER TABLE t_sensors ADD description VARCHAR(128);








---------------------------------------------------------------------------------
--------------------------------- TABLES UPDATE ---------------------------------
---------------------------------------------------------------------------------

---------------------
--- T_SCHEMATYPES ---
---------------------
INSERT INTO t_schematypes VALUES (12, 'DM');

---------------------------------
--- T_SCHEMAMODIFICATIONTYPES ---
----------------------------------
INSERT INTO t_schemamodificationtypes VALUES (5, 'PLUGIN INSTALLATION');
INSERT INTO t_schemamodificationtypes VALUES (6, 'PLUGIN UPGRADE');

--------------------
--- T_SATELLITES ---
--------------------
INSERT INTO t_satellites VALUES(33, 'Jason', 'JSN') ;

-----------------
--- T_SENSORS ---
-----------------
INSERT INTO t_sensors VALUES (61, 'POSEIDON-1', 'Poseidon-1 Radar Altimeter');
INSERT INTO t_sensors VALUES (62, 'POSEIDON-2', 'Poseidon-2 Radar Altimeter');
INSERT INTO t_sensors VALUES (63, 'POSEIDON-3', 'Poseidon-3 Radar Altimeter');
INSERT INTO t_sensors VALUES (64, 'JMR', 'Jason-1 Microwave Radiometer');
INSERT INTO t_sensors VALUES (65, 'AMR', 'Advanced Microwave Radiometer');
UPDATE t_sensors SET description = 'fake satellite for test' WHERE sensorname = 'DUMMY';
UPDATE t_sensors SET description = 'Doppler Orbitography and Radiopositioning Integrated by Satellite' WHERE sensorname = 'DORIS';
UPDATE t_sensors SET description = 'Radio Occultation Sounder for Atmospheric Studies' WHERE sensorname = 'ROSA';
UPDATE t_sensors SET description = 'Scanning Scatterometer' WHERE sensorname = 'SCAT';
UPDATE t_sensors SET description = 'Ocean Colour Monitor' WHERE sensorname = 'OCM';
UPDATE t_sensors SET description = 'X-band Synthetic Aperture Radar' WHERE sensorname = 'X-SAR';
UPDATE t_sensors SET description = 'Advanced Microwave Scanning Radiometer - Earth Observing System' WHERE sensorname = 'AMSR-E';
UPDATE t_sensors SET description = 'Advanced Spaceborne Thermal Emission and Reflection Radiometer' WHERE sensorname = 'ASTER';
UPDATE t_sensors SET description = 'Special Sensor Microwave Imager / Sounder' WHERE sensorname = 'SSMIS';
UPDATE t_sensors SET description = 'Special Sensor Microwave Imager' WHERE sensorname = 'SSMI';
UPDATE t_sensors SET description = 'Advanced Microwave Sounding Unit-A' WHERE sensorname = 'AMSU-A';
UPDATE t_sensors SET description = 'Advanced Microwave Sounding Unit-B' WHERE sensorname = 'AMSU-B';
UPDATE t_sensors SET description = 'Spinning Enhanced Visible and Infrared Imager' WHERE sensorname = 'SEVIRI';
UPDATE t_sensors SET description = 'Phased Array type L-band Synthetic Aperture Radar' WHERE sensorname = 'PALSAR';
UPDATE t_sensors SET description = 'Advanced Visible and Near Infrared Radiometer type 2' WHERE sensorname = 'AVNIR-2';
UPDATE t_sensors SET description = 'Panchromatic Remote-sensing Instrument for Stereo Mapping' WHERE sensorname = 'PRISM';
UPDATE t_sensors SET description = 'Global Ozone Monitoring Experiment' WHERE sensorname = 'GOME';
UPDATE t_sensors SET description = 'Global Navigation Satellite System Receiver for Atmospheric Sounding' WHERE sensorname = 'GRAS';
UPDATE t_sensors SET description = 'Advanced SCATterometer' WHERE sensorname = 'ASCAT';
UPDATE t_sensors SET description = 'Infrared Atmospheric Sounding Interferometer' WHERE sensorname = 'IASI';
UPDATE t_sensors SET description = 'Space Environmental Monitor' WHERE sensorname = 'SEM';
UPDATE t_sensors SET description = 'Argos Advanced Data Collection System' WHERE sensorname = 'A-DCS';
UPDATE t_sensors SET description = 'Microwave Humidity Sounder ' WHERE sensorname = 'MHS';
UPDATE t_sensors SET description = 'High-resolution Infrared Radiation Sounder' WHERE sensorname = 'HIRS';
UPDATE t_sensors SET description = 'VENµS Super Spectral Camera' WHERE sensorname = 'VSSC';
UPDATE t_sensors SET description = 'Atmospheric LAser Doppler INstrument' WHERE sensorname = 'ALADIN';
UPDATE t_sensors SET description = 'Star Tracker' WHERE sensorname = 'STARTRK';
UPDATE t_sensors SET description = 'Radar Altimeter 2 - Microwave Radiometer' WHERE sensorname = 'RA2-MWR';
UPDATE t_sensors SET description = 'Advanced Along-Track Scanning Radiometer' WHERE sensorname = 'AATSR';
UPDATE t_sensors SET description = 'SCanning Imaging Absorption SpectroMeter for Atmospheric CartograpHY' WHERE sensorname = 'SCIAMACHY';
UPDATE t_sensors SET description = 'Global Ozone Monitoring by Occultation of Stars' WHERE sensorname = 'GOMOS';
UPDATE t_sensors SET description = 'Michelson Interferometer for Passive Atmospheric Sounding' WHERE sensorname = 'MIPAS';
UPDATE t_sensors SET description = 'Medium Resolution Imaging Spectrometer' WHERE sensorname = 'MERIS';
UPDATE t_sensors SET description = 'Ball Global Imaging System 2000' WHERE sensorname = 'BGIS-2000';
UPDATE t_sensors SET description = 'MQ-1 Predator unmanned aircraft' WHERE sensorname = 'predator';
UPDATE t_sensors SET description = 'high-resolution satellite' WHERE sensorname = 'ikonos';
UPDATE t_sensors SET description = 'Moderate Resolution Imaging Spectroradiometer' WHERE sensorname = 'MODIS';
UPDATE t_sensors SET description = 'Advanced Very High Resolution Radiometer' WHERE sensorname = 'AVHRR';
UPDATE t_sensors SET description = 'Satellite-to-Satellite Tracking' WHERE sensorname = 'SST';
UPDATE t_sensors SET description = 'Electrostatic Gravity Gradiometer' WHERE sensorname = 'EGG';
UPDATE t_sensors SET description = 'Advanced Synthetic Aperture Radar' WHERE sensorname = 'ASAR';
UPDATE t_sensors SET description = 'Synthetic Aperture Interferometric Radar Altimeter' WHERE sensorname = 'SIRAL';
UPDATE t_sensors SET description = 'Synthetic Aperture Radar' WHERE sensorname = 'SAR';
UPDATE t_sensors SET description = 'Sea-viewing Wide Field-of-view Sensor' WHERE sensorname = 'SeaWiFS';
UPDATE t_sensors SET description = 'Along Track Scanning Radiometer' WHERE sensorname = 'ATSR';
UPDATE t_sensors SET description = 'Active Microwave Instrument - SAR mode' WHERE sensorname = 'AMI-SAR';

----------------------
--- T_SATRELATIONS ---
----------------------
----- JASON -----
-- Jason 1
INSERT INTO t_satrelations VALUES (33, 1, 62) ;
INSERT INTO t_satrelations VALUES (33, 1, 64) ;
-- Jason 2
INSERT INTO t_satrelations VALUES (33, 2, 63) ;
INSERT INTO t_satrelations VALUES (33, 2, 65) ;








---------------------------------------------------------------------------------
------------------------------ PROCEDURES CREATION ------------------------------
---------------------------------------------------------------------------------

-------------------------------
--- P_DISTRIBUTE_ITEM_WDATE ---
-------------------------------
CREATE FUNCTION p_distribute_item_wdate(
	in_fname VARCHAR(128), 
	in_fversion VARCHAR(8), 
	in_filetype VARCHAR(16), 
	in_userid VARCHAR(32), 
	in_ordertype VARCHAR(8), 
	in_format VARCHAR(16), 
	in_orderid INTEGER,
	in_duedays INT2) RETURNS INTEGER AS $$

DECLARE
	newdate    	TIMESTAMP(6);
	tmpdate    	DATE ;
	tmptime    	TIME(6) ;
	row        	RECORD ;
	p_extordid 	VARCHAR(128);
	p_cartid   	INT;
	p_subreq   	INT;
	p_totsubreq	INT;
	
BEGIN

	p_extordid := (SELECT externalorderid FROM t_ordersqueue WHERE orderid = in_orderid);
	p_subreq := (SELECT subreq FROM t_ordersqueue WHERE orderid = in_orderid);
	
	IF p_extordid IS NOT NULL AND p_subreq IS NOT NULL 
	THEN
		p_cartid := (SELECT id FROM t_cartsref AS cr
				INNER JOIN t_ordersqueue AS oq
				ON oq.externalorderid = cr.extref
				WHERE oq.orderid = in_orderid);

		p_totsubreq := (SELECT max(c.distsubreq) +1 
				FROM t_cartsinv c 
				WHERE c.cart = p_cartid);
		
		UPDATE t_cartsinv SET distsubreq = p_totsubreq WHERE cart = p_cartid AND ordsubreq = p_subreq ;
	ELSE
		p_cartid := NULL;
		p_totsubreq := NULL;
	END IF;


--	the input inventory file is distributed to each user who declared an interest on that file-type.
--	the distribution is done according to the active field in the distribution policy record
	IF in_format IS NULL THEN
		IF in_ordertype IS NULL THEN
			FOR row IN (SELECT DISTINCT dp.id, dp.mediatype, dp.format, dp.algo
			  	  	  	  FROM t_distributionpolicies dp
						  	WHERE dp.userid = in_userid 
								AND (dp.filetype = in_filetype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = in_filetype)) 
								AND dp.active = 't' 
								AND dp.ordertype IS NULL 
								AND dp.usermode = (SELECT mode FROM t_users WHERE userid = in_userid)) LOOP

					tmpdate := NOW() ;
					tmptime := NOW() ;
					newdate := (tmpdate + in_duedays) + tmptime ;

					INSERT INTO t_temporarydistribution 
					  ("duedate", "filename", "fileversion", "mediatype", "userid", "format", "algo", "policy", "orderid", "cartid", "subreq")
					VALUES
					  (newdate, in_fname, in_fversion, row.mediatype, in_userid, row.format, row.algo, row.id, in_orderid, p_cartid, p_totsubreq) ;
			END LOOP ;
		ELSE
			-- No format but order type
			FOR row IN (SELECT DISTINCT dp.id, dp.mediatype, dp.format, dp.algo
				  	  	  FROM t_distributionpolicies dp
				  	  	  	WHERE dp.userid = in_userid 
								AND (dp.filetype = in_filetype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = in_filetype)) 
								AND dp.active = 't' 
								AND (dp.ordertype = in_ordertype OR dp.ordertype IS NULL) 
								AND dp.usermode = (SELECT mode FROM t_users WHERE userid = in_userid)) LOOP

					tmpdate := NOW() ;
					tmptime := NOW() ;
					newdate := (tmpdate + in_duedays) + tmptime ;

					INSERT INTO t_temporarydistribution
					  ("duedate", "filename", "fileversion", "mediatype", "userid", "format", "algo", "policy", "orderid", "cartid", "subreq")
					VALUES 
					  (newdate, in_fname, in_fversion, row.mediatype, in_userid, row.format, row.algo, row.id, in_orderid, p_cartid, p_totsubreq) ;
			END LOOP ;
		END IF ;
	ELSE
		IF in_ordertype IS NULL THEN

			-- Format defined in input
			FOR row IN (SELECT DISTINCT dp.id, dp.mediatype, dp.algo
  	  	  	  	  	  	  FROM t_distributionpolicies dp
  	  	  	  	  	  	  	WHERE dp.userid=in_userid 
								AND (dp.filetype = in_filetype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = in_filetype)) 
								AND dp.active = 't' 
								AND dp.format = in_format 
								AND dp.ordertype IS NULL 
								AND dp.usermode = (SELECT mode FROM t_users WHERE userid = in_userid)) LOOP

					tmpdate := NOW() ;
					tmptime := NOW() ;
					newdate := (tmpdate + in_duedays) + tmptime ;

					INSERT INTO t_temporarydistribution
					  ("duedate", "filename", "fileversion", "mediatype", "userid", "format", "algo", "policy", "orderid", "cartid", "subreq")
						VALUES
  	  	  	  	  	  (newdate, in_fname, in_fversion, row.mediatype, in_userid, in_format, row.algo, row.id, in_orderid, p_cartid, p_totsubreq) ;
			END LOOP ;
		ELSE
			-- Format and order type defined in input
			FOR row IN (SELECT DISTINCT dp.id, dp.mediatype, dp.algo
				  	  	  FROM t_distributionpolicies dp
				  	  	  	WHERE dp.userid=in_userid 
								AND (dp.filetype = in_filetype OR dp.filetype IN (SELECT pfiletype FROM t_filetypescomp WHERE cfiletype = in_filetype)) 
								AND dp.active = 't' 
								AND dp.format = in_format 
								AND (dp.ordertype = in_ordertype OR dp.ordertype IS NULL) 
								AND dp.usermode = (SELECT mode FROM t_users WHERE userid = in_userid)) LOOP

					tmpdate := NOW() ;
					tmptime := NOW() ;
					newdate := (tmpdate + in_duedays) + tmptime ;

					INSERT INTO t_temporarydistribution
					  ("duedate", "filename", "fileversion", "mediatype", "userid", "format", "algo", "policy", "orderid", "cartid", "subreq")
					VALUES
					  (newdate, in_fname, in_fversion, row.mediatype, in_userid, in_format, row.algo, row.id, in_orderid, p_cartid, p_totsubreq) ;
			END LOOP ;
		END IF ;
	END IF ;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;








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
		'pds-9.5.0',
		'2010-05-05',
		3,
		1,
		'PDS_9-5-0',
		'Modified table t_sensors. Updated tables t_satellites, t_sensors, t_satrelations, t_schematypes and schemamodificationtypes. Created procedure p_distribute_item_wdate.'
) ;
