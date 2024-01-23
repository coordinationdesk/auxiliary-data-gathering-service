
-- Version 01.00.00
-- POSTGRES








--------------------------------------------------------------------------------
------------------------------ EXTENSION CREATION ------------------------------
--------------------------------------------------------------------------------

CREATE EXTENSION IF NOT EXISTS "uuid-ossp" ;
CREATE EXTENSION IF NOT EXISTS "postgis" ;
CREATE EXTENSION IF NOT EXISTS "pg_trgm" ;








--------------------------------------------------------------------------------
------------------------------ FUNCTIONS CREATION ------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	f_get_tasktable_id
*/ ------------------------------
CREATE OR REPLACE FUNCTION f_get_tasktable_id(tt_name VARCHAR(64)) RETURNS INT AS $$
DECLARE
	res INT ;
BEGIN

  	res := (SELECT id_tasktable FROM t_tasktables WHERE name = tt_name) ;
  	RETURN res ;

	EXCEPTION WHEN OTHERS THEN
	   RETURN -1 ;

END ;
$$ LANGUAGE plpgsql ;




/* ------------------------------
	f_srv_get_status_id
*/ ------------------------------
CREATE OR REPLACE FUNCTION f_srv_get_status_id(
	statusName VARCHAR(32)
) RETURNS BIGINT AS $$

DECLARE
	res BIGINT ;
	
BEGIN
  	SELECT id INTO res FROM t_srv_statuses WHERE status = statusName ;
	
  	RETURN res ;

	EXCEPTION WHEN OTHERS THEN
	   RETURN -1 ;

END ;
$$ LANGUAGE plpgsql ;




/* ------------------------------
	f_srv_create_order
*/ ------------------------------
CREATE OR REPLACE FUNCTION f_srv_create_order(
	inProductId BIGINT,
	inAipId INTEGER,
	inStatusId BIGINT,
	inPriority INTEGER,
	inEstimatedDate TIMESTAMP(3),
	inRetentionTime INTEGER,
	inUserId INTEGER,
	inBulkbatchId INTEGER
) RETURNS BIGINT AS $$

DECLARE
	foundOrderId BIGINT ;
	returnedOrderId BIGINT ;
	sqlCmd VARCHAR ;
	
BEGIN
	IF (inUserId IS NULL AND inBulkbatchId IS NULL) OR (inUserId IS NOT NULL AND inBulkbatchId IS NOT NULL) THEN
		RAISE EXCEPTION 'f_srv_create_order: UserId and BulkbatchId must be valorized, and only one per time' ;
	END IF ;
	
	SELECT id INTO foundOrderId FROM t_srv_ordersqueue WHERE product_id = inProductId AND target_aip_id = inAipId AND status_id != f_srv_get_status_id('ERROR');
	
	IF foundOrderId IS NOT NULL THEN
		BEGIN
			IF inUserId IS NOT NULL THEN
				INSERT INTO t_srv_request_x_user (user_id, order_id) VALUES (inUserId, foundOrderId) ON CONFLICT DO NOTHING ; 
			ELSE
				INSERT INTO t_srv_bulkbatch_x_ordersqueue (bulkbatch_id, order_id) VALUES (inBulkbatchId, foundOrderId) ON CONFLICT DO NOTHING ; 
			END IF ;
			
			UPDATE t_srv_ordersqueue SET estimated_date = inEstimatedDate, priority = CASE WHEN priority < inPriority THEN inPriority ELSE priority END WHERE id = foundOrderId ;
			
			IF (SELECT s.status FROM t_srv_statuses s INNER JOIN t_srv_ordersqueue q ON q.status_id = s.id WHERE q.id = foundOrderId) = 'COMPLETED' THEN
				sqlCmd := 'UPDATE t_srv_aip_content SET eviction_time = NOW() + INTERVAL ''' || inRetentionTime || ' SECOND'' WHERE order_id = ' || foundOrderId ;
				EXECUTE sqlCmd ;
			END IF ;
		
			RETURN foundOrderId ;
			
		EXCEPTION WHEN others THEN
			INSERT INTO t_srv_ordersqueue (product_id, status_id, target_aip_id, estimated_date, priority)
				VALUES (inProductId, inStatusId, inAipId, inEstimatedDate, inPriority)
				RETURNING id INTO returnedOrderId ;
				
			IF inUserId IS NOT NULL THEN
				INSERT INTO t_srv_request_x_user (user_id, order_id) VALUES (inUserId, returnedOrderId) ON CONFLICT DO NOTHING ; 
			ELSE
				INSERT INTO t_srv_bulkbatch_x_ordersqueue (bulkbatch_id, order_id) VALUES (inBulkbatchId, returnedOrderId) ON CONFLICT DO NOTHING ; 
			END IF ;
		
			RETURN returnedOrderId ;
		END ;
		
	ELSE
		INSERT INTO t_srv_ordersqueue (product_id, status_id, target_aip_id, estimated_date, priority)
			VALUES (inProductId, inStatusId, inAipId, inEstimatedDate, inPriority)
			RETURNING id INTO returnedOrderId ;
		
		IF inUserId IS NOT NULL THEN
			INSERT INTO t_srv_request_x_user (user_id, order_id) VALUES (inUserId, returnedOrderId) ON CONFLICT DO NOTHING ; 
		ELSE
			INSERT INTO t_srv_bulkbatch_x_ordersqueue (bulkbatch_id, order_id) VALUES (inBulkbatchId, returnedOrderId) ON CONFLICT DO NOTHING ; 
		END IF ;
		
		RETURN returnedOrderId ;
		
	END IF ;
		
END ;
$$ LANGUAGE plpgsql ;








--------------------------------------------------------------------------------
-------------------------------- TABLES CREATION -------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_satellites
*/ ------------------------------
CREATE TABLE t_satellites ( 
    satelliteid INT2 NOT NULL,
	satelliteacronym VARCHAR(4) NOT NULL,
    satellitename VARCHAR(64) NOT NULL
) ;
ALTER TABLE t_satellites
	ADD CONSTRAINT pk_t_satellites
		PRIMARY KEY (satelliteid) ;

ALTER TABLE t_satellites
	ADD CONSTRAINT uq_t_satellites1
		UNIQUE (satellitename) ;

ALTER TABLE t_satellites
	ADD CONSTRAINT uq_t_satellites2
		UNIQUE (satelliteacronym) ;

INSERT INTO t_satellites (satelliteid, satellitename, satelliteacronym) VALUES (34,'Sentinel-1', 'S1_') ;
INSERT INTO t_satellites (satelliteid, satellitename, satelliteacronym) VALUES (35,'Sentinel-1A', 'S1A') ;
INSERT INTO t_satellites (satelliteid, satellitename, satelliteacronym) VALUES (36,'Sentinel-1B', 'S1B') ;
INSERT INTO t_satellites (satelliteid, satellitename, satelliteacronym) VALUES (37,'Sentinel-2', 'S2_') ;
INSERT INTO t_satellites (satelliteid, satellitename, satelliteacronym) VALUES (38,'Sentinel-2A', 'S2A') ;
INSERT INTO t_satellites (satelliteid, satellitename, satelliteacronym) VALUES (39,'Sentinel-2B', 'S2B') ;
INSERT INTO t_satellites (satelliteid, satellitename, satelliteacronym) VALUES (40, 'Sentinel-3', 'S3_') ;
INSERT INTO t_satellites (satelliteid, satellitename, satelliteacronym) VALUES (41, 'Sentinel-3A', 'S3A') ;
INSERT INTO t_satellites (satelliteid, satellitename, satelliteacronym) VALUES (42, 'Sentinel-3B', 'S3B') ;




/* ------------------------------
	t_sensors
*/ ------------------------------
CREATE TABLE t_sensors ( 
    sensorid INT2 NOT NULL,
    sensorname VARCHAR(64) NOT NULL,
	description VARCHAR(128)
) ;
ALTER TABLE t_sensors
	ADD CONSTRAINT pk_t_sensors
		PRIMARY KEY (sensorid) ;

ALTER TABLE t_sensors
	ADD CONSTRAINT uq_t_sensors1
		UNIQUE (sensorname) ;

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




/* ------------------------------
	t_satrelations
*/ ------------------------------
CREATE TABLE t_satrelations ( 
    satelliteid INT2 NOT NULL,
    mission INT2 NOT NULL,
    sensorid INT2 NOT NULL
) ;
ALTER TABLE t_satrelations
	ADD CONSTRAINT pk_t_satrelations
		PRIMARY KEY (satelliteid, mission, sensorid) ;

ALTER TABLE t_satrelations
	ADD CONSTRAINT fk_t_satrelations1
		FOREIGN KEY (satelliteid) REFERENCES t_satellites (satelliteid)
			ON DELETE CASCADE ;

ALTER TABLE t_satrelations
	ADD CONSTRAINT fk_t_satrelations2
		FOREIGN KEY (sensorid) REFERENCES t_sensors (sensorid)
			ON DELETE CASCADE ;

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




/* ------------------------------
	t_srv_awsuploadqueue
*/ ------------------------------
CREATE TABLE t_srv_awsuploadqueue (
	id SERIAL NOT NULL,
	localpath VARCHAR(4096),
	remotepath VARCHAR(4096),
	nextretrydate TIMESTAMP(3),
	size_bytes BIGINT,
	retries SMALLINT DEFAULT 0,                   
	status_id BIGINT NOT NULL DEFAULT f_srv_get_status_id('CREATED'),
	hostname VARCHAR(256) NOT NULL,
	lasterrormsg VARCHAR(4096),
	aws_s3_endpoint VARCHAR(4096),
	aws_s3_key_ak VARCHAR(256),
	aws_s3_key_sk VARCHAR(256),
	aws_s3_provider VARCHAR(32),
	aws_s3_region VARCHAR(32),
	aws_s3_remotebasepath VARCHAR(4096),
	priority SMALLINT DEFAULT 9,
	aws_s3_upload_pool_size SMALLINT,
	aws_s3_upload_buffer_size BIGINT
) ;
ALTER TABLE t_srv_awsuploadqueue
	ADD CONSTRAINT pk_t_srv_awsuploadqueue
		PRIMARY KEY (id) ;

ALTER TABLE t_srv_awsuploadqueue 
	ADD CONSTRAINT fk_srv_awsuploadqueue01
		FOREIGN KEY (status_id)
			REFERENCES t_srv_statuses (id)
				ON DELETE CASCADE ;

CREATE INDEX ix_t_srv_awsuploadqueue01 ON t_srv_awsuploadqueue USING BTREE (nextretrydate) ;
CREATE INDEX ix_t_srv_awsuploadqueue02 ON t_srv_awsuploadqueue USING BTREE (status_id) ;
CREATE INDEX ix_t_srv_awsuploadqueue03 ON t_srv_awsuploadqueue USING BTREE (hostname) ;




/* ------------------------------
	t_srv_catalogue_change_registry
*/ ------------------------------
CREATE TABLE t_srv_catalogue_change_registry (
    id BIGSERIAL NOT NULL,
    sensingStartDate TIMESTAMP(0) NOT NULL,
    modificationDate TIMESTAMP(0) NOT NULL DEFAULT NOW()::DATE,
	satellite_id SMALLINT NOT NULL
);
ALTER TABLE t_srv_catalogue_change_registry 
	ADD CONSTRAINT pk_t_srv_catalogue_change_registry
		PRIMARY KEY (id) ;

ALTER TABLE t_srv_catalogue_change_registry
	ADD CONSTRAINT uq_srv_catalogue_change_registry01
		UNIQUE (sensingStartDate, modificationDate, satellite_id) ;

ALTER TABLE t_srv_catalogue_change_registry
	ADD CONSTRAINT fk_srv_catalogue_change_registry01
		FOREIGN KEY (satellite_id) REFERENCES t_satellites (satelliteid)
			ON DELETE CASCADE ;

CREATE INDEX ix_t_srv_catalogue_change_registry01 ON t_srv_catalogue_change_registry USING BTREE (sensingStartDate) ;
CREATE INDEX ix_t_srv_catalogue_change_registry02 ON t_srv_catalogue_change_registry USING BTREE (modificationDate) ;
CREATE INDEX ix_t_srv_catalogue_change_registry03 ON t_srv_catalogue_change_registry USING BTREE (satellite_id) ;




/* ------------------------------
	t_applicationstatus
*/ ------------------------------
CREATE TABLE t_applicationstatus (
    application VARCHAR(64) NOT NULL,
    instance VARCHAR(8) NOT NULL,
    status OID
) ;
ALTER TABLE t_applicationstatus
	ADD CONSTRAINT pk_t_applicationstatus
		PRIMARY KEY (application, instance) ;




/* ------------------------------
	t_filetypes
*/ ------------------------------
CREATE TABLE t_filetypes (
    id_filetype SERIAL NOT NULL,
	filetype VARCHAR(16) NOT NULL,
    description VARCHAR(64),
    nameregularexpression VARCHAR(128),
    headerseparation BOOLEAN DEFAULT 'f' NOT NULL,
    "group" VARCHAR(32) NOT NULL,
    invperiod INT,
	mngsngext BOOLEAN DEFAULT 't' NOT NULL,
	sngext VARCHAR(8),
	hdrext VARCHAR(8),
	dblext VARCHAR(8)
) ;
ALTER TABLE t_filetypes
	ADD CONSTRAINT pk_t_filetypes
		PRIMARY KEY (id_filetype) ;

ALTER TABLE t_filetypes 
	ADD CONSTRAINT ck_filetypes01 
		CHECK ((sngext IS NULL AND hdrext IS NULL AND dblext IS NULL) 
			OR (sngext IS NULL AND hdrext IS NOT NULL AND dblext IS NOT NULL AND ( hdrext != dblext)) 
			OR (sngext IS NOT NULL AND ((hdrext IS NULL AND dblext IS NULL) OR ( hdrext != dblext)))) ;

ALTER TABLE t_filetypes 
	ADD CONSTRAINT uq_filetypes01
		UNIQUE (filetype) ;




/* ------------------------------
	t_filetypescomps
*/ ------------------------------
CREATE TABLE t_filetypescomps (
    id SERIAL NOT NULL,
    pfiletype INTEGER NOT NULL,
    cfiletype INTEGER NOT NULL
);
ALTER TABLE t_filetypescomps 
	ADD CONSTRAINT pk_t_filetypescomps
		PRIMARY KEY (id) ;

ALTER TABLE t_filetypescomps 
	ADD CONSTRAINT ck_filetypescomps01 
		CHECK (pfiletype != cfiletype ) ;

ALTER TABLE t_filetypescomps 
	ADD CONSTRAINT uq_filetypescomps01 
		UNIQUE (pfiletype, cfiletype) ;

ALTER TABLE t_filetypescomps 
	ADD CONSTRAINT fk_filetypescomps01
		FOREIGN KEY (pfiletype) 
			REFERENCES t_filetypes (id_filetype) 
				ON DELETE CASCADE ;

ALTER TABLE t_filetypescomps 
	ADD CONSTRAINT fk_filetypescomps02
		FOREIGN KEY (cfiletype) 
			REFERENCES t_filetypes (id_filetype) 
				ON DELETE CASCADE ;

CREATE INDEX ix_t_filetypescomps01 ON t_filetypescomps USING BTREE (pfiletype) ;
CREATE INDEX ix_t_filetypescomps02 ON t_filetypescomps USING BTREE (cfiletype) ;




/* ------------------------------
	t_srv_filetype_x_eviction
*/ ------------------------------
CREATE TABLE t_srv_filetype_x_eviction (
    id SERIAL NOT NULL,
    filetype_id INTEGER NOT NULL,
    deltatime INTEGER NOT NULL
);
ALTER TABLE t_srv_filetype_x_eviction 
	ADD CONSTRAINT pk_t_srv_filetype_x_eviction
		PRIMARY KEY (id) ;

ALTER TABLE t_srv_filetype_x_eviction 
	ADD CONSTRAINT uq_srv_filetype_x_eviction01 
		UNIQUE (filetype_id) ;

ALTER TABLE t_srv_filetype_x_eviction 
	ADD CONSTRAINT fk_srv_filetype_x_eviction01
		FOREIGN KEY (filetype_id) 
			REFERENCES t_filetypes (id_filetype) 
				ON DELETE CASCADE ;

CREATE INDEX ix_t_srv_filetype_x_eviction01 ON t_srv_filetype_x_eviction USING BTREE (filetype_id) ;




/* ------------------------------
	t_storagemanager
*/ ------------------------------
CREATE TABLE t_storagemanager (
	id SERIAL NOT NULL,
    type VARCHAR(16) NOT NULL,
    name VARCHAR(24) NOT NULL,
    description VARCHAR(64) NOT NULL,
    invrelationtable VARCHAR(32) NOT NULL,
    parameters TEXT,
	syncupload BOOLEAN DEFAULT 't' NOT NULL, 
	syncdownload BOOLEAN DEFAULT 't' NOT NULL,
	syncerase BOOLEAN DEFAULT 't' NOT NULL,
	allowdefaultcost BOOLEAN DEFAULT 't' NOT NULL,
	migrationalgo_id INTEGER,
	migrationalgo_par TEXT,
	error_recovery_period INTEGER DEFAULT 60
) ;
ALTER TABLE t_storagemanager
	ADD CONSTRAINT pk_t_storagemanager
		PRIMARY KEY (id) ;

ALTER TABLE t_storagemanager
	ADD CONSTRAINT uq_storagemanager
		UNIQUE (name) ;

CREATE INDEX ix_t_storagemanager01 ON t_storagemanager USING BTREE (type) ;
CREATE INDEX ix_t_storagemanager02 ON t_storagemanager USING BTREE (migrationalgo_id) ;




/* ------------------------------
	t_storagepriority
*/ ------------------------------
CREATE TABLE t_storagepriority (
    id INTEGER NOT NULL,
    filetype VARCHAR(16) NOT NULL,
    storecost INTEGER NOT NULL,
    retrievecost INTEGER NOT NULL,
    erasecost INTEGER NOT NULL,
	parameters TEXT
) ;
ALTER TABLE t_storagepriority
	ADD CONSTRAINT pk_t_storagepriority
		PRIMARY KEY (id, filetype) ;

ALTER TABLE t_storagepriority
	ADD CONSTRAINT fk_storagepriority1
		FOREIGN KEY (id)
			REFERENCES t_storagemanager (id)
				ON DELETE CASCADE ;

ALTER TABLE t_storagepriority
	ADD CONSTRAINT fk_storagepriority2
		FOREIGN KEY (filetype)
			REFERENCES t_filetypes (filetype)
				ON DELETE CASCADE ;




/* ------------------------------
	t_srv_inventory 
*/ ------------------------------
CREATE TABLE t_srv_inventory (
	id BIGSERIAL NOT NULL,
	uuid VARCHAR(160) DEFAULT uuid_generate_v1() NOT NULL,
	name VARCHAR(1024) NOT NULL,
	contenttype VARCHAR(256) NOT NULL,
	contentlength BIGINT NOT NULL,
	origindate TIMESTAMP(3) DEFAULT NOW() NOT NULL,
	creationdate TIMESTAMP(3) DEFAULT NOW() NOT NULL,
	modificationdate TIMESTAMP(3) DEFAULT NOW() NOT NULL,
	checksum JSONB NOT NULL,
	validitystart TIMESTAMP(3) NOT NULL,
	validitystop TIMESTAMP(3) NOT NULL,
	footprint GEOGRAPHY(GEOMETRY,4326),
	validity BOOLEAN NOT NULL DEFAULT FALSE,
	filetype_id INTEGER NOT NULL,
	satellite_id SMALLINT,
	sensor_id SMALLINT,
	mission SMALLINT,
	evictiondate TIMESTAMP(3) NOT NULL DEFAULT '2199-04-01 12:00:00.000'::TIMESTAMP(3),
	productiontype VARCHAR(36) NOT NULL DEFAULT 'systematic_production',
	attributes JSONB,
	trace_id VARCHAR(160)
) ;
ALTER TABLE t_srv_inventory
	ADD CONSTRAINT pk_t_srv_inventory
		PRIMARY KEY (id) ;

ALTER TABLE t_srv_inventory 
	ADD CONSTRAINT ck_srv_inventory01 
		CHECK (productiontype IN ('systematic_production','on-demand:default','on-demand:non-default')) ;

ALTER TABLE t_srv_inventory 
	ADD CONSTRAINT uq_srv_inventory01
		UNIQUE (uuid) ;

ALTER TABLE t_srv_inventory 
	ADD CONSTRAINT uq_srv_inventory02
		UNIQUE (name) ;

ALTER TABLE t_srv_inventory
	ADD CONSTRAINT fk_srv_inventory01
		FOREIGN KEY (filetype_id)
			REFERENCES t_filetypes (id_filetype)
				ON DELETE CASCADE ;

ALTER TABLE t_srv_inventory
	ADD CONSTRAINT fk_srv_inventory02
		FOREIGN KEY (satellite_id)
			REFERENCES t_satellites (satelliteid)
				ON DELETE CASCADE ;

ALTER TABLE t_srv_inventory
	ADD CONSTRAINT fk_srv_inventory03
		FOREIGN KEY (sensor_id)
			REFERENCES t_sensors (sensorid)
				ON DELETE CASCADE ;

CREATE INDEX ix_t_srv_inventory01 ON t_srv_inventory USING BTREE (uuid) ;
CREATE INDEX ix_t_srv_inventory02 ON t_srv_inventory USING BTREE (name) ;
CREATE INDEX ix_t_srv_inventory03 ON t_srv_inventory USING BTREE (origindate) ;
CREATE INDEX ix_t_srv_inventory04 ON t_srv_inventory USING BTREE (creationdate) ;
CREATE INDEX ix_t_srv_inventory05 ON t_srv_inventory USING BTREE (modificationdate) ;
CREATE INDEX ix_t_srv_inventory06 ON t_srv_inventory USING GIN (checksum) ;
CREATE INDEX ix_t_srv_inventory07 ON t_srv_inventory USING BTREE (validitystart) ;
CREATE INDEX ix_t_srv_inventory08 ON t_srv_inventory USING BTREE (validitystop) ;
CREATE INDEX ix_t_srv_inventory_spatial ON t_srv_inventory USING GIST (footprint) ;
CREATE INDEX ix_t_srv_inventory09 ON t_srv_inventory USING BTREE (filetype_id) ;
CREATE INDEX ix_t_srv_inventory10 ON t_srv_inventory USING BTREE (satellite_id) ;
CREATE INDEX ix_t_srv_inventory11 ON t_srv_inventory USING BTREE (sensor_id) ;
CREATE INDEX ix_t_srv_inventory12 ON t_srv_inventory USING BTREE (evictiondate) ;
CREATE INDEX ix_t_srv_inventory13 ON t_srv_inventory USING GIN (attributes) ;
CREATE INDEX ix_t_srv_inventory14 ON t_srv_inventory USING BTREE (mission) ;




/* ------------------------------
        t_srv_inventory_dwh
*/ ------------------------------
CREATE TABLE t_srv_inventory_dwh (
	id BIGINT,
	uuid VARCHAR(160),
	name VARCHAR(1024),
	contenttype VARCHAR(256),
	contentlength BIGINT,
	origindate TIMESTAMP(3),
	creationdate TIMESTAMP(3),
	modificationdate TIMESTAMP(3),
	checksum JSONB,
	validitystart TIMESTAMP(3),
	validitystop TIMESTAMP(3),
	footprint GEOGRAPHY(GEOMETRY,4326),
	validity BOOLEAN,
	filetype_id INTEGER,
	satellite_id SMALLINT,
	sensor_id SMALLINT,
	mission SMALLINT,
	evictiondate TIMESTAMP(3),
	productiontype VARCHAR(36),
	attributes JSONB,
	trace_id VARCHAR(160)
) ;
ALTER TABLE t_srv_inventory_dwh
	ADD CONSTRAINT pk_t_srv_inventory_dwh
		PRIMARY KEY (id) ;

CREATE INDEX ix_t_srv_inventory_dwh01 ON t_srv_inventory_dwh USING BTREE (uuid) ;
CREATE INDEX ix_t_srv_inventory_dwh02 ON t_srv_inventory_dwh USING GIN (name GIN_TRGM_OPS) ;
CREATE INDEX ix_t_srv_inventory_dwh03 ON t_srv_inventory_dwh USING BTREE (origindate) ;
CREATE INDEX ix_t_srv_inventory_dwh04 ON t_srv_inventory_dwh USING BTREE (creationdate) ;
CREATE INDEX ix_t_srv_inventory_dwh05 ON t_srv_inventory_dwh USING BTREE (modificationdate) ;
CREATE INDEX ix_t_srv_inventory_dwh06 ON t_srv_inventory_dwh USING GIN (checksum) ;
CREATE INDEX ix_t_srv_inventory_dwh07 ON t_srv_inventory_dwh USING BTREE (validitystart) ;
CREATE INDEX ix_t_srv_inventory_dwh08 ON t_srv_inventory_dwh USING BTREE (validitystop) ;
CREATE INDEX ix_t_srv_inventory_dwh_spatial ON t_srv_inventory_dwh USING GIST (footprint) ;
CREATE INDEX ix_t_srv_inventory_dwh09 ON t_srv_inventory_dwh USING BTREE (filetype_id) ;
CREATE INDEX ix_t_srv_inventory_dwh10 ON t_srv_inventory_dwh USING BTREE (satellite_id) ;
CREATE INDEX ix_t_srv_inventory_dwh11 ON t_srv_inventory_dwh USING BTREE (sensor_id) ;
CREATE INDEX ix_t_srv_inventory_dwh12 ON t_srv_inventory_dwh USING BTREE (evictiondate) ;
CREATE INDEX ix_t_srv_inventory_dwh13 ON t_srv_inventory_dwh USING GIN (attributes) ;




/* ------------------------------
	t_srv_pendinginv 
*/ ------------------------------
CREATE TABLE t_srv_pendinginv (
	id BIGSERIAL NOT NULL,
	inv_id BIGINT NOT NULL,
	uuid VARCHAR(160) NOT NULL,
	name VARCHAR(1024) NOT NULL,
	contenttype VARCHAR(256) NOT NULL,
	contentlength BIGINT NOT NULL,
	origindate TIMESTAMP(3)NOT NULL,
	creationdate TIMESTAMP(3) NOT NULL,
	modificationdate TIMESTAMP(3) NOT NULL,
	checksum JSONB NOT NULL,
	validitystart TIMESTAMP(3) NOT NULL,
	validitystop TIMESTAMP(3) NOT NULL,
	footprint GEOGRAPHY(GEOMETRY,4326),
	validity BOOLEAN NOT NULL,
	filetype_id INTEGER NOT NULL,
	satellite_id SMALLINT,
	sensor_id SMALLINT,
	mission SMALLINT,
	evictiondate TIMESTAMP(3) NOT NULL,
	productiontype VARCHAR(36) NOT NULL,
	attributes JSONB,
	trace_id VARCHAR(160),
	operation VARCHAR(8) NOT NULL
) ;
ALTER TABLE t_srv_pendinginv
	ADD CONSTRAINT pk_t_srv_pendinginv
		PRIMARY KEY (id) ;

ALTER TABLE t_srv_pendinginv 
	ADD CONSTRAINT ck_srv_pendinginv01 
		CHECK (operation IN ('INSERT','UPDATE','DELETE')) ;

CREATE INDEX ix_t_srv_pendinginv01 ON t_srv_pendinginv USING BTREE (inv_id) ;
CREATE INDEX ix_t_srv_pendinginv02 ON t_srv_pendinginv USING BTREE (uuid) ;





/* ------------------------------
	t_srv_inventorysm
*/ ------------------------------
CREATE TABLE t_srv_inventorysm (
	id SERIAL NOT NULL,
	inv_id BIGINT,
	storagemanid INTEGER NOT NULL,
	packageid INTEGER,
	stored_date TIMESTAMP(3) DEFAULT NOW() NOT NULL,
	migration_report TEXT
) ;
ALTER TABLE t_srv_inventorysm
	ADD CONSTRAINT pk_t_srv_inventorysm
		PRIMARY KEY (id) ;

ALTER TABLE t_srv_inventorysm
	ADD CONSTRAINT uq_srv_inventorysm01
		UNIQUE (inv_id, storagemanid) ;

ALTER TABLE t_srv_inventorysm
	ADD CONSTRAINT fk_srv_inventorysm1
		FOREIGN KEY (inv_id)
			REFERENCES t_srv_inventory (id)
				ON DELETE CASCADE ;

ALTER TABLE t_srv_inventorysm
	ADD CONSTRAINT fk_srv_inventorysm2
		FOREIGN KEY (storagemanid)
			REFERENCES t_storagemanager (id)
				ON DELETE CASCADE ;

CREATE INDEX ix_t_srv_inventorysm01 ON t_srv_inventorysm USING BTREE (inv_id);
CREATE INDEX ix_t_srv_inventorysm02 ON t_srv_inventorysm USING BTREE (storagemanid);
CREATE INDEX ix_t_srv_inventorysm03 ON t_srv_inventorysm USING BTREE (packageid);




/* ------------------------------
	t_srv_inventory_x_sources
*/ ------------------------------
CREATE TABLE t_srv_inventory_x_sources (
	id SERIAL NOT NULL,
	tt_label VARCHAR(32) NOT NULL,
	description VARCHAR(512) NULL,
	mission_source INTEGER NOT NULL
) ;
ALTER TABLE t_srv_inventory_x_sources
	ADD CONSTRAINT pk_t_srv_inventory_x_sources
		PRIMARY KEY (id) ;

ALTER TABLE t_srv_inventory_x_sources
	ADD CONSTRAINT uq_srv_inventory_x_sources2
		UNIQUE (tt_label) ;




/* ------------------------------
	t_acsisminvrelation
*/ ------------------------------
CREATE TABLE t_acsisminvrelation (
    id INT NOT NULL,
    path VARCHAR(512) NOT NULL,
    signature VARCHAR(64),
	medialabel VARCHAR(128)
);
ALTER TABLE t_acsisminvrelation 
	ADD CONSTRAINT pk_t_acsisminvrelation
		PRIMARY KEY (id);

ALTER TABLE t_acsisminvrelation 
	ADD CONSTRAINT fk_acshsminvrelation01
		FOREIGN KEY (id)
			REFERENCES t_srv_inventorysm (id)
				ON DELETE CASCADE ;

CREATE INDEX ix_t_acsisminvrelation01 ON t_acsisminvrelation USING BTREE (path);
CREATE INDEX ix_t_acsisminvrelation02 ON t_acsisminvrelation USING BTREE (medialabel);




/* ------------------------------
	t_srv_users 
*/ ------------------------------
CREATE TABLE t_srv_users (
    id SERIAL NOT NULL,
	name VARCHAR(32) NOT NULL,
	--aip_disk_quota BIGINT NOT NULL,
	default_priority SMALLINT NOT NULL,
	highest_priority SMALLINT NOT NULL,
	creation_tstamp TIMESTAMP(0) DEFAULT NOW(),
	service_alias VARCHAR(64)
) ;
ALTER TABLE t_srv_users
	ADD CONSTRAINT pk_t_srv_users
		PRIMARY KEY (id) ;

ALTER TABLE t_srv_users 
	ADD CONSTRAINT uq_srv_users01
		UNIQUE (name) ;

ALTER TABLE t_srv_users 
	ADD CONSTRAINT ck_srv_users01
		CHECK ((default_priority >= 1 ) AND (default_priority <= 100 )) ;

ALTER TABLE t_srv_users 
	ADD CONSTRAINT ck_srv_users02
		CHECK (((highest_priority >= 1 ) AND (highest_priority <= 100 )) AND (highest_priority >= default_priority ));




/* ------------------------------
	t_srv_userroles 
*/ ------------------------------
CREATE TABLE t_srv_userroles (
    id SMALLINT NOT NULL,
	role VARCHAR(32) NOT NULL,
	description VARCHAR(256)
) ;
ALTER TABLE t_srv_userroles
	ADD CONSTRAINT pk_t_srv_userroles
		PRIMARY KEY (id) ;

ALTER TABLE t_srv_userroles 
	ADD CONSTRAINT uq_srv_userroles01
		UNIQUE (role) ;

INSERT INTO t_srv_userroles (id, role, description) VALUES (1, 'Order', 'Allow to place orders for product retrieval and create subscriptions') ;
INSERT INTO t_srv_userroles (id, role, description) VALUES (2, 'BulkOrder', 'As for Order role, and additionally allow to place Bulk product retrieval requests') ;
INSERT INTO t_srv_userroles (id, role, description) VALUES (3, 'Reporting', 'Allow to perform reporting queries on all orders, subscriptions and bulks which have been created by all users') ;
INSERT INTO t_srv_userroles (id, role, description) VALUES (4, 'UserAdmin', 'Allow to register new users, modify credentials and define system roles for users') ;
INSERT INTO t_srv_userroles (id, role, description) VALUES (5, 'Download', 'The client may perform queries on the products available and perform download') ;




/* ------------------------------
	t_srv_users_x_roles 
*/ ------------------------------
CREATE TABLE t_srv_users_x_roles (
    id SERIAL NOT NULL,
	user_id INTEGER NOT NULL,
	role_id SMALLINT NOT NULL
) ;
ALTER TABLE t_srv_users_x_roles
	ADD CONSTRAINT pk_t_srv_users_x_roles
		PRIMARY KEY (id) ;

ALTER TABLE t_srv_users_x_roles 
	ADD CONSTRAINT fk_srv_users_x_roles01
		FOREIGN KEY (user_id)
			REFERENCES t_srv_users (id)
				ON DELETE CASCADE ;

ALTER TABLE t_srv_users_x_roles 
	ADD CONSTRAINT fk_srv_users_x_roles02
		FOREIGN KEY (role_id)
			REFERENCES t_srv_userroles (id)
				ON DELETE CASCADE ;

CREATE INDEX ix_t_srv_users_x_roles01 ON t_srv_users_x_roles USING BTREE (user_id) ;
CREATE INDEX ix_t_srv_users_x_roles02 ON t_srv_users_x_roles USING BTREE (role_id) ;




/* ------------------------------
	t_srv_op_types 
*/ ------------------------------
CREATE TABLE t_srv_op_types (
	id INTEGER NOT NULL,
	type VARCHAR(32) NOT NULL
) ;
ALTER TABLE t_srv_op_types 
	ADD CONSTRAINT pk_t_srv_op_types
		PRIMARY KEY (id) ;

ALTER TABLE t_srv_op_types 
	ADD CONSTRAINT uq_srv_op_types01
		UNIQUE (type) ;

INSERT INTO t_srv_op_types (id, type) VALUES (1, 'CREATEORDER');
INSERT INTO t_srv_op_types (id, type) VALUES (2, 'TRIGGERBATCH');
INSERT INTO t_srv_op_types (id, type) VALUES (3, 'CREATEDLTOKEN');
INSERT INTO t_srv_op_types (id, type) VALUES (4, 'USEDLTOKEN');
INSERT INTO t_srv_op_types (id, type) VALUES (5, 'DELETEDLTOKEN');
INSERT INTO t_srv_op_types (id, type) VALUES (6, 'DOWNLOADDATA');




/* ------------------------------
	t_srv_op_quota_registry 
*/ ------------------------------
CREATE TABLE t_srv_op_quota_registry (
    id SERIAL NOT NULL,
	operationuuid VARCHAR(160) DEFAULT uuid_generate_v1() NOT NULL,
	referenceuuid VARCHAR(160),
	operation_type_id INTEGER NOT NULL,
	user_id INTEGER NOT NULL,
	operationdate TIMESTAMP(3) NOT NULL DEFAULT NOW(),
	granted BOOLEAN NOT NULL DEFAULT TRUE,
	reason VARCHAR(512),
	cost BIGINT NOT NULL
) ;
ALTER TABLE t_srv_op_quota_registry
	ADD CONSTRAINT pk_t_srv_op_quota_registry
		PRIMARY KEY (id) ;

ALTER TABLE t_srv_op_quota_registry 
	ADD CONSTRAINT uq_srv_op_quota_registry
		UNIQUE (operationuuid) ;

ALTER TABLE t_srv_op_quota_registry 
	ADD CONSTRAINT fk_srv_op_quota_registry01
		FOREIGN KEY (operation_type_id)
			REFERENCES t_srv_op_types (id)
				ON DELETE CASCADE ;

ALTER TABLE t_srv_op_quota_registry 
	ADD CONSTRAINT fk_srv_op_quota_registry02
		FOREIGN KEY (user_id)
			REFERENCES t_srv_users (id)
				ON DELETE CASCADE ;

CREATE INDEX ix_t_srv_op_quota_registry01 ON t_srv_op_quota_registry USING BTREE (operationuuid) ;
CREATE INDEX ix_t_srv_op_quota_registry02 ON t_srv_op_quota_registry USING BTREE (referenceuuid) ;
CREATE INDEX ix_t_srv_op_quota_registry03 ON t_srv_op_quota_registry USING BTREE (operation_type_id) ;
CREATE INDEX ix_t_srv_op_quota_registry04 ON t_srv_op_quota_registry USING BTREE (user_id) ;
CREATE INDEX ix_t_srv_op_quota_registry05 ON t_srv_op_quota_registry USING BTREE (operationdate) ;




/* ------------------------------
	t_srv_op_dltokens 
*/ ------------------------------
CREATE TABLE t_srv_op_dltokens (
	id SERIAL NOT NULL,
	user_id INTEGER NOT NULL,
	tokenuuid VARCHAR(160) DEFAULT uuid_generate_v1() NOT NULL,
	status_id BIGINT NOT NULL,                                    -- (VALID, INUSE) verificate in t_srv_statuses
	creationtime TIMESTAMP(3) DEFAULT NOW()
) ;
ALTER TABLE t_srv_op_dltokens
	ADD CONSTRAINT pk_t_srv_op_dltokens
		PRIMARY KEY (id) ;

ALTER TABLE t_srv_op_dltokens 
	ADD CONSTRAINT fk_srv_op_dltokens01
		FOREIGN KEY (user_id)
			REFERENCES t_srv_users (id)
				ON DELETE CASCADE ;

ALTER TABLE t_srv_op_dltokens 
	ADD CONSTRAINT fk_srv_op_dltokens02
		FOREIGN KEY (status_id)
			REFERENCES t_srv_statuses (id)
				ON DELETE CASCADE ;

CREATE INDEX ix_t_srv_op_dltokens01 ON t_srv_op_dltokens USING BTREE (user_id) ;
CREATE INDEX ix_t_srv_op_dltokens02 ON t_srv_op_dltokens USING BTREE (status_id) ;




/* ------------------------------
	t_srv_users_quota 
*/ ------------------------------
CREATE TABLE t_srv_users_quota (
	id SERIAL NOT NULL,
	user_id INTEGER NOT NULL,
	modificationdate TIMESTAMP(3) NOT NULL DEFAULT NOW(),
	ordersize BIGINT NOT NULL DEFAULT 0,
	OrderInterval INTERVAL NOT NULL DEFAULT '1 second' ,
	OrderParallel INTEGER NOT NULL DEFAULT 0,
	BatchSize BIGINT NOT NULL DEFAULT 0,
	BatchInterval INTERVAL NOT NULL DEFAULT '1 second',
	BatchParallel INTEGER NOT NULL DEFAULT 0,
	DownloadSize BIGINT NOT NULL DEFAULT 0,
	DownloadInterval INTERVAL NOT NULL DEFAULT '1 second',
	DownloadParallel INTEGER NOT NULL DEFAULT 0
) ;
ALTER TABLE t_srv_users_quota
	ADD CONSTRAINT pk_t_srv_users_quota
		PRIMARY KEY (id) ;

ALTER TABLE t_srv_users_quota 
	ADD CONSTRAINT fk_srv_users_quota01
		FOREIGN KEY (user_id)
			REFERENCES t_srv_users (id)
				ON DELETE CASCADE ;

CREATE INDEX ix_t_srv_users_quota01 ON t_srv_users_quota USING BTREE (user_id) ;
CREATE INDEX ix_t_srv_users_quota02 ON t_srv_users_quota USING BTREE (modificationdate) ;




/* ------------------------------
	t_srv_aip 
*/ ------------------------------
CREATE TABLE t_srv_aip (
    id SERIAL NOT NULL,
	aip_name VARCHAR(64) NOT NULL,
	url VARCHAR(512) NOT NULL,
	username VARCHAR(32) NOT NULL,
	passwd VARCHAR(256) NOT NULL,
	disk_space BIGINT NOT NULL,
	base_redirect_url VARCHAR(1024) NOT NULL
) ;
ALTER TABLE t_srv_aip
	ADD CONSTRAINT pk_t_srv_aip
		PRIMARY KEY (id) ;

ALTER TABLE t_srv_aip 
	ADD CONSTRAINT uq_srv_aip01
		UNIQUE (aip_name) ;




/* ------------------------------
	t_srv_notificationsendpoints
*/ ------------------------------
CREATE TABLE t_srv_notificationsendpoints (
	id SERIAL NOT NULL,
	endpoint VARCHAR(512) NOT NULL,
	username VARCHAR(32),
	password VARCHAR(1024)
) ;

ALTER TABLE t_srv_notificationsendpoints
	ADD CONSTRAINT pk_t_srv_notificationsendpoints
		PRIMARY KEY (id) ;

ALTER TABLE t_srv_notificationsendpoints 
	ADD CONSTRAINT uq_srv_notificationsendpoints01
		UNIQUE (endpoint, username) ;




/* ------------------------------
	t_srv_bulk_orders
*/ ------------------------------
CREATE TABLE t_srv_bulk_orders (
	id SERIAL NOT NULL,
	uuid VARCHAR(160) DEFAULT uuid_generate_v1() NOT NULL,
	user_id INTEGER NOT NULL,
	status_id BIGINT NOT NULL,
	statusmessage VARCHAR(512),
	filterparam VARCHAR(1024) NOT NULL,
	orderparam VARCHAR(64),
	batchsizeproducts BIGINT,
	batchsizevolume BIGINT,
	submissiondate TIMESTAMP(3) DEFAULT NOW() NOT NULL,
	completiondate TIMESTAMP(3),
	modificationdate TIMESTAMP(3) DEFAULT NOW() NOT NULL,
--	notificationendpoint VARCHAR(512),
--	notificationepuser VARCHAR(32),
--	notificationeppassword VARCHAR(1024),
	notificationendpoint_id INTEGER,
	tobecancelled BOOLEAN DEFAULT FALSE NOT NULL
) ;

ALTER TABLE t_srv_bulk_orders
	ADD CONSTRAINT pk_t_srv_bulk_orders
		PRIMARY KEY (id) ;

ALTER TABLE t_srv_bulk_orders 
	ADD CONSTRAINT uq_srv_bulk_order01
		UNIQUE (uuid) ;

ALTER TABLE t_srv_bulk_orders 
	ADD CONSTRAINT fk_srv_bulk_order01
		FOREIGN KEY (user_id)
			REFERENCES t_srv_users (id)
				ON DELETE CASCADE ;

ALTER TABLE t_srv_bulk_orders 
	ADD CONSTRAINT fk_srv_bulk_order02
		FOREIGN KEY (status_id)
			REFERENCES t_srv_statuses (id)
				ON DELETE CASCADE ;

ALTER TABLE t_srv_bulk_orders 
	ADD CONSTRAINT fk_srv_bulk_order03
		FOREIGN KEY (notificationendpoint_id)
			REFERENCES t_srv_notificationsendpoints (id)
				ON DELETE CASCADE ;

CREATE INDEX ix_t_srv_bulk_orders01 ON t_srv_bulk_orders USING BTREE (uuid) ;
CREATE INDEX ix_t_srv_bulk_orders02 ON t_srv_bulk_orders USING BTREE (user_id) ;
CREATE INDEX ix_t_srv_bulk_orders03 ON t_srv_bulk_orders USING BTREE (status_id) ;
CREATE INDEX ix_t_srv_bulk_orders04 ON t_srv_bulk_orders USING BTREE (notificationendpoint_id) ;




/* ------------------------------
	t_srv_bulkbatches
*/ ------------------------------
CREATE TABLE t_srv_bulkbatches (
	id SERIAL NOT NULL,
	uuid VARCHAR(160) DEFAULT uuid_generate_v1() NOT NULL,
	bulk_id INTEGER NOT NULL,
	status_id BIGINT NOT NULL,
	statusmessage VARCHAR(512),
	ordersize BIGINT,
	submissiondate TIMESTAMP(3),
	estimateddate TIMESTAMP(3),
	modificationdate TIMESTAMP(3) DEFAULT NOW() NOT NULL,
	completeddate TIMESTAMP(3),
	priority SMALLINT,
	tobecancelled BOOLEAN DEFAULT FALSE NOT NULL
) ;

ALTER TABLE t_srv_bulkbatches
	ADD CONSTRAINT pk_t_srv_bulkbatches
		PRIMARY KEY (id) ;

ALTER TABLE t_srv_bulkbatches 
	ADD CONSTRAINT ck_srv_bulkbatches01
		CHECK ((priority >= 0 ) AND (priority <= 100 )) ;

ALTER TABLE t_srv_bulkbatches 
	ADD CONSTRAINT uq_srv_bulkbatches01
		UNIQUE (uuid) ;

ALTER TABLE t_srv_bulkbatches 
	ADD CONSTRAINT fk_srv_bulkbatches01
		FOREIGN KEY (bulk_id)
			REFERENCES t_srv_bulk_orders (id)
				ON DELETE CASCADE ;

ALTER TABLE t_srv_bulkbatches 
	ADD CONSTRAINT fk_srv_bulkbatches02
		FOREIGN KEY (status_id)
			REFERENCES t_srv_statuses (id)
				ON DELETE CASCADE ;

CREATE INDEX ix_t_srv_bulkbatches01 ON t_srv_bulkbatches USING BTREE (uuid) ;
CREATE INDEX ix_t_srv_bulkbatches02 ON t_srv_bulkbatches USING BTREE (bulk_id) ;
CREATE INDEX ix_t_srv_bulkbatches03 ON t_srv_bulkbatches USING BTREE (status_id) ;




/* ------------------------------
	t_srv_bulkbatch_x_inv
*/ ------------------------------
CREATE TABLE t_srv_bulkbatch_x_inv (
	id SERIAL NOT NULL,
	bulkbatch_id INTEGER NOT NULL,
	inv_id BIGINT NOT NULL
) ;

ALTER TABLE t_srv_bulkbatch_x_inv
	ADD CONSTRAINT pk_t_srv_bulkbatch_x_inv
		PRIMARY KEY (id) ;

ALTER TABLE t_srv_bulkbatch_x_inv 
	ADD CONSTRAINT fk_srv_bulkbatch_x_inv01
		FOREIGN KEY (bulkbatch_id)
			REFERENCES t_srv_bulkbatches (id)
				ON DELETE CASCADE ;

ALTER TABLE t_srv_bulkbatch_x_inv 
	ADD CONSTRAINT fk_srv_bulkbatch_x_inv02
		FOREIGN KEY (inv_id)
			REFERENCES t_srv_inventory (id)
				ON DELETE CASCADE ;

CREATE INDEX ix_t_srv_bulkbatch_x_inv01 ON t_srv_bulkbatch_x_inv USING BTREE (bulkbatch_id) ;
CREATE INDEX ix_t_srv_bulkbatch_x_inv02 ON t_srv_bulkbatch_x_inv USING BTREE (inv_id) ;




/* ------------------------------
	t_srv_ordersqueue
*/ ------------------------------
CREATE TABLE t_srv_ordersqueue (
    id BIGSERIAL NOT NULL,
	uuid VARCHAR(160) DEFAULT uuid_generate_v1() NOT NULL,
	product_id BIGINT NOT NULL,
	status_id BIGINT NOT NULL,
	jobresponsible VARCHAR(64),
	target_aip_id INTEGER NOT NULL,
	upload_status_id BIGINT,
	remote_aws_uploader_jobid BIGINT,
	remote_aws_uploader_url VARCHAR(512),
	last_status_update TIMESTAMP(3) DEFAULT NOW() NOT NULL,
	creation_date TIMESTAMP(3) DEFAULT NOW() NOT NULL,
	start_date TIMESTAMP(3),
	completion_date TIMESTAMP(3),
	modification_date TIMESTAMP(3) DEFAULT NOW() NOT NULL,
	estimated_date TIMESTAMP(3) NOT NULL,
	priority INTEGER NOT NULL,
	tobecancelled BOOLEAN DEFAULT FALSE NOT NULL,
	links_count INTEGER DEFAULT 0 NOT NULL
) ;
ALTER TABLE t_srv_ordersqueue
	ADD CONSTRAINT pk_t_srv_ordersqueue
		PRIMARY KEY (id) ;

ALTER TABLE t_srv_ordersqueue 
	ADD CONSTRAINT uq_srv_ordersqueue01
		UNIQUE (uuid) ;

ALTER TABLE t_srv_ordersqueue 
	ADD CONSTRAINT uq_srv_ordersqueue02
		UNIQUE (remote_aws_uploader_jobid) ;

ALTER TABLE t_srv_ordersqueue 
	ADD CONSTRAINT fk_srv_ordersqueue01
		FOREIGN KEY (product_id)
			REFERENCES t_srv_inventory (id)
				ON DELETE CASCADE ;

ALTER TABLE t_srv_ordersqueue 
	ADD CONSTRAINT fk_srv_ordersqueue02
		FOREIGN KEY (status_id)
			REFERENCES t_srv_statuses (id)
				ON DELETE CASCADE ;

ALTER TABLE t_srv_ordersqueue 
	ADD CONSTRAINT fk_srv_ordersqueue03
		FOREIGN KEY (target_aip_id)
			REFERENCES t_srv_aip (id)
				ON DELETE CASCADE ;

ALTER TABLE t_srv_ordersqueue 
	ADD CONSTRAINT fk_srv_ordersqueue04
		FOREIGN KEY (upload_status_id)
			REFERENCES t_srv_statuses (id)
				ON DELETE CASCADE ;

ALTER TABLE t_srv_ordersqueue 
	ADD CONSTRAINT fk_srv_ordersqueue05
		FOREIGN KEY (remote_aws_uploader_jobid)
			REFERENCES t_srv_awsuploadqueue (id)
				ON DELETE CASCADE ;

CREATE INDEX ix_t_srv_ordersqueue01 ON t_srv_ordersqueue USING BTREE (product_id) ;
CREATE INDEX ix_t_srv_ordersqueue02 ON t_srv_ordersqueue USING BTREE (status_id) ;
CREATE INDEX ix_t_srv_ordersqueue03 ON t_srv_ordersqueue USING BTREE (target_aip_id) ;
CREATE INDEX ix_t_srv_ordersqueue04 ON t_srv_ordersqueue USING BTREE (upload_status_id) ;
CREATE INDEX ix_t_srv_ordersqueue05 ON t_srv_ordersqueue USING BTREE (remote_aws_uploader_jobid) ;
CREATE INDEX ix_t_srv_ordersqueue06 ON t_srv_ordersqueue USING BTREE (creation_date) ;
CREATE INDEX ix_t_srv_ordersqueue07 ON t_srv_ordersqueue USING BTREE (completion_date) ;
CREATE INDEX ix_t_srv_ordersqueue08 ON t_srv_ordersqueue USING BTREE (modification_date) ;




/* ------------------------------
	t_srv_bulkbatch_x_ordersqueue
*/ ------------------------------
CREATE TABLE t_srv_bulkbatch_x_ordersqueue (
	id SERIAL NOT NULL,
	bulkbatch_id INTEGER NOT NULL, 
	order_id INTEGER NOT NULL, 
	creationdate TIMESTAMP(3) DEFAULT NOW() NOT NULL
) ;

ALTER TABLE t_srv_bulkbatch_x_ordersqueue
	ADD CONSTRAINT pk_t_srv_bulkbatch_x_ordersqueue
		PRIMARY KEY (id) ;

ALTER TABLE t_srv_bulkbatch_x_ordersqueue 
	ADD CONSTRAINT uq_srv_bulkbatch_x_ordersqueue01
		UNIQUE (bulkbatch_id, order_id) ;

ALTER TABLE t_srv_bulkbatch_x_ordersqueue 
	ADD CONSTRAINT fk_srv_bulkbatch_x_inv01
		FOREIGN KEY (bulkbatch_id)
			REFERENCES t_srv_bulkbatches (id)
				ON DELETE CASCADE ;

ALTER TABLE t_srv_bulkbatch_x_ordersqueue 
	ADD CONSTRAINT fk_srv_bulkbatch_x_inv02
		FOREIGN KEY (order_id)
			REFERENCES t_srv_ordersqueue (id)
				ON DELETE CASCADE ;

CREATE INDEX ix_t_srv_bulkbatch_x_ordersqueue01 ON t_srv_bulkbatch_x_ordersqueue USING BTREE (bulkbatch_id) ;
CREATE INDEX ix_t_srv_bulkbatch_x_ordersqueue02 ON t_srv_bulkbatch_x_ordersqueue USING BTREE (order_id) ;
CREATE INDEX ix_t_srv_bulkbatch_x_ordersqueue03 ON t_srv_bulkbatch_x_ordersqueue USING BTREE (creationdate) ;




/* ------------------------------
	t_srv_subscriptions
*/ ------------------------------
CREATE TABLE t_srv_subscriptions (
	id SERIAL NOT NULL,
	uuid VARCHAR(160) DEFAULT uuid_generate_v1() NOT NULL,
	user_id INTEGER NOT NULL,
	subscriptionevent VARCHAR(8) NOT NULL,
	status_id BIGINT NOT NULL,
	filterparam VARCHAR(1024) NOT NULL,
	submissiondate TIMESTAMP(3) DEFAULT NOW() NOT NULL,
	lastquerydate TIMESTAMP(3),
	stageorder BOOLEAN NOT NULL,
	priority INTEGER,
--	notificationendpoint VARCHAR(512),
--	notificationepuser VARCHAR(32),
--	notificationeppassword VARCHAR(1024)
	notificationendpoint_id INTEGER
) ;

ALTER TABLE t_srv_subscriptions
	ADD CONSTRAINT pk_t_srv_subscriptions
		PRIMARY KEY (id) ;

ALTER TABLE t_srv_subscriptions 
	ADD CONSTRAINT ck_srv_subscriptions01 
		CHECK (subscriptionevent IN ('CREATED','DELETED')) ;

ALTER TABLE t_srv_subscriptions 
	ADD CONSTRAINT ck_srv_subscriptions02
		CHECK ((priority >= 0 ) AND (priority <= 100 )) ;

ALTER TABLE t_srv_subscriptions 
	ADD CONSTRAINT uq_srv_subscriptions01
		UNIQUE (uuid) ;

ALTER TABLE t_srv_subscriptions 
	ADD CONSTRAINT fk_srv_subscriptions01
		FOREIGN KEY (user_id)
			REFERENCES t_srv_users (id)
				ON DELETE CASCADE ;

ALTER TABLE t_srv_subscriptions 
	ADD CONSTRAINT fk_srv_subscriptions02
		FOREIGN KEY (status_id)
			REFERENCES t_srv_statuses (id)
				ON DELETE CASCADE ;

ALTER TABLE t_srv_subscriptions 
	ADD CONSTRAINT fk_srv_subscriptions03
		FOREIGN KEY (notificationendpoint_id)
			REFERENCES t_srv_notificationsendpoints (id)
				ON DELETE CASCADE ;

CREATE INDEX ix_t_srv_subscriptions01 ON t_srv_subscriptions USING BTREE (user_id) ;
CREATE INDEX ix_t_srv_subscriptions02 ON t_srv_subscriptions USING BTREE (status_id) ;
CREATE INDEX ix_t_srv_subscriptions03 ON t_srv_subscriptions USING BTREE (notificationendpoint_id) ;




/* ------------------------------
	t_srv_subscription_x_ordersqueue
*/ ------------------------------
CREATE TABLE t_srv_subscription_x_ordersqueue (
	id SERIAL NOT NULL,
	subscription_id INTEGER NOT NULL, 
	order_id INTEGER NOT NULL, 
	creationdate TIMESTAMP(3) DEFAULT NOW() NOT NULL
) ;

ALTER TABLE t_srv_subscription_x_ordersqueue
	ADD CONSTRAINT pk_t_srv_subscription_x_ordersqueue
		PRIMARY KEY (id) ;

ALTER TABLE t_srv_subscription_x_ordersqueue 
	ADD CONSTRAINT fk_srv_subscription_x_ordersqueue01
		FOREIGN KEY (subscription_id)
			REFERENCES t_srv_subscriptions (id)
				ON DELETE CASCADE ;

ALTER TABLE t_srv_subscription_x_ordersqueue 
	ADD CONSTRAINT fk_srv_subscription_x_ordersqueue02
		FOREIGN KEY (order_id)
			REFERENCES t_srv_ordersqueue (id)
				ON DELETE CASCADE ;

CREATE INDEX ix_t_srv_subscription_x_ordersqueue01 ON t_srv_subscription_x_ordersqueue USING BTREE (subscription_id) ;
CREATE INDEX ix_t_srv_subscription_x_ordersqueue02 ON t_srv_subscription_x_ordersqueue USING BTREE (order_id) ;
CREATE INDEX ix_t_srv_subscription_x_ordersqueue03 ON t_srv_subscription_x_ordersqueue USING BTREE (creationdate) ;




/* ------------------------------
	t_srv_notificationsendpoints_x_ordersqueue
*/ ------------------------------
CREATE TABLE t_srv_notificationsendpoints_x_ordersqueue (
	id SERIAL NOT NULL,
	order_id INTEGER NOT NULL, 
	bulkorder_uuid VARCHAR(160),
	subscription_uuid VARCHAR(160), 
	endpoint_id INTEGER NOT NULL
) ;

ALTER TABLE t_srv_notificationsendpoints_x_ordersqueue
	ADD CONSTRAINT pk_t_srv_notificationsendpoints_x_ordersqueue
		PRIMARY KEY (id) ;

ALTER TABLE t_srv_notificationsendpoints_x_ordersqueue 
	ADD CONSTRAINT fk_srv_notificationsendpoints_x_ordersqueue01
		FOREIGN KEY (order_id)
			REFERENCES t_srv_ordersqueue (id)
				ON DELETE CASCADE ;

ALTER TABLE t_srv_notificationsendpoints_x_ordersqueue 
	ADD CONSTRAINT fk_srv_notificationsendpoints_x_ordersqueue02
		FOREIGN KEY (bulkorder_uuid)
			REFERENCES t_srv_bulk_orders (uuid)
				ON DELETE CASCADE ;

ALTER TABLE t_srv_notificationsendpoints_x_ordersqueue 
	ADD CONSTRAINT fk_srv_notificationsendpoints_x_ordersqueue03
		FOREIGN KEY (subscription_uuid)
			REFERENCES t_srv_subscriptions (uuid)
				ON DELETE CASCADE ;

ALTER TABLE t_srv_notificationsendpoints_x_ordersqueue 
	ADD CONSTRAINT fk_srv_notificationsendpoints_x_ordersqueue04
		FOREIGN KEY (endpoint_id)
			REFERENCES t_srv_notificationsendpoints (id)
				ON DELETE CASCADE ;

CREATE INDEX ix_t_srv_notificationsendpoints_x_ordersqueue01 ON t_srv_notificationsendpoints_x_ordersqueue USING BTREE (order_id) ;
CREATE INDEX ix_t_srv_notificationsendpoints_x_ordersqueue02 ON t_srv_notificationsendpoints_x_ordersqueue USING BTREE (bulkorder_uuid) ;
CREATE INDEX ix_t_srv_notificationsendpoints_x_ordersqueue03 ON t_srv_notificationsendpoints_x_ordersqueue USING BTREE (subscription_uuid) ;
CREATE INDEX ix_t_srv_notificationsendpoints_x_ordersqueue04 ON t_srv_notificationsendpoints_x_ordersqueue USING BTREE (endpoint_id) ;




/* ------------------------------
	t_srv_aip_content 
*/ ------------------------------
CREATE TABLE t_srv_aip_content (
    id BIGSERIAL NOT NULL,
	aip_id INTEGER NOT NULL,
	product_uuid VARCHAR(160) NOT NULL,
	order_id BIGINT NOT NULL,
	bucket_name VARCHAR(32) NOT NULL,
	product_size BIGINT NOT NULL,
	status_id BIGINT NOT NULL,
	upload_time TIMESTAMP(3) DEFAULT NOW() NOT NULL,
	eviction_time TIMESTAMP(3) DEFAULT NOW() + INTERVAL '3' DAY NOT NULL
) ;
ALTER TABLE t_srv_aip_content
	ADD CONSTRAINT pk_t_srv_aip_content
		PRIMARY KEY (id) ;

ALTER TABLE t_srv_aip_content 
	ADD CONSTRAINT uq_srv_aip_content01
		UNIQUE (order_id) ;

ALTER TABLE t_srv_aip_content 
	ADD CONSTRAINT uq_srv_aip_content02
		UNIQUE (aip_id, product_uuid) ;

ALTER TABLE t_srv_aip_content 
	ADD CONSTRAINT fk_srv_aip_content0l
		FOREIGN KEY (aip_id)
			REFERENCES t_srv_aip (id)
				ON DELETE CASCADE ;

ALTER TABLE t_srv_aip_content 
	ADD CONSTRAINT fk_srv_aip_content02
		FOREIGN KEY (product_uuid)
			REFERENCES t_srv_inventory (uuid)
				ON DELETE CASCADE ;

ALTER TABLE t_srv_aip_content 
	ADD CONSTRAINT fk_srv_aip_content03
		FOREIGN KEY (status_id)
			REFERENCES t_srv_statuses (id)
				ON DELETE CASCADE ;

ALTER TABLE t_srv_aip_content 
	ADD CONSTRAINT fk_srv_aip_content04
		FOREIGN KEY (order_id)
			REFERENCES t_srv_ordersqueue (id)
				ON DELETE CASCADE ;

CREATE INDEX ix_t_srv_aip_content01 ON t_srv_aip_content USING BTREE (aip_id) ;
CREATE INDEX ix_t_srv_aip_content02 ON t_srv_aip_content USING BTREE (product_uuid) ;
CREATE INDEX ix_t_srv_aip_content03 ON t_srv_aip_content USING BTREE (bucket_name) ;
CREATE INDEX ix_t_srv_aip_content04 ON t_srv_aip_content USING BTREE (status_id) ;
CREATE INDEX ix_t_srv_aip_content05 ON t_srv_aip_content USING BTREE (upload_time) ;
CREATE INDEX ix_t_srv_aip_content06 ON t_srv_aip_content USING BTREE (eviction_time) ;




/* ------------------------------
	t_srv_request_x_user
*/ ------------------------------
CREATE TABLE t_srv_request_x_user (
    id SERIAL NOT NULL,
	user_id INTEGER NOT NULL,
	order_id BIGINT NOT NULL,
	creation_date TIMESTAMP(3) DEFAULT NOW() NOT NULL,
	to_be_cancelled BOOLEAN DEFAULT FALSE NOT NULL
) ;
ALTER TABLE t_srv_request_x_user
	ADD CONSTRAINT pk_t_srv_request_x_user
		PRIMARY KEY (id) ;

ALTER TABLE t_srv_request_x_user 
	ADD CONSTRAINT uq_srv_request_x_user01
		UNIQUE (user_id, order_id) ;

ALTER TABLE t_srv_request_x_user 
	ADD CONSTRAINT fk_srv_request_x_user01
		FOREIGN KEY (user_id)
			REFERENCES t_srv_users (id)
				ON DELETE CASCADE ;

ALTER TABLE t_srv_request_x_user 
	ADD CONSTRAINT fk_srv_request_x_user02
		FOREIGN KEY (order_id)
			REFERENCES t_srv_ordersqueue (id)
				ON DELETE CASCADE ;

CREATE INDEX ix_t_srv_request_x_user01 ON t_srv_request_x_user USING BTREE (user_id) ;
CREATE INDEX ix_t_srv_request_x_user02 ON t_srv_request_x_user USING BTREE (order_id) ;
CREATE INDEX ix_t_srv_request_x_user03 ON t_srv_request_x_user USING BTREE (creation_date) ;




/* ------------------------------
	t_srv_ordersqueue_state_transitions
*/ ------------------------------
CREATE TABLE t_srv_ordersqueue_state_transitions (
    id SERIAL NOT NULL,
	order_id BIGINT NOT NULL,
	status_id INTEGER NOT NULL,
	tstamp TIMESTAMP(3) DEFAULT NOW() NOT NULL
) ;
ALTER TABLE t_srv_ordersqueue_state_transitions
	ADD CONSTRAINT pk_t_srv_ordersqueue_state_transitions
		PRIMARY KEY (id) ;

ALTER TABLE t_srv_ordersqueue_state_transitions 
	ADD CONSTRAINT fk_srv_ordersqueue_state_transitions01
		FOREIGN KEY (order_id)
			REFERENCES t_srv_ordersqueue (id)
				ON DELETE CASCADE ;

ALTER TABLE t_srv_ordersqueue_state_transitions 
	ADD CONSTRAINT fk_srv_ordersqueue_state_transitions02
		FOREIGN KEY (status_id)
			REFERENCES t_srv_statuses (id)
				ON DELETE CASCADE ;

CREATE INDEX ix_t_srv_ordersqueue_state_transitions01 ON t_srv_ordersqueue_state_transitions USING BTREE (order_id) ;
CREATE INDEX ix_t_srv_ordersqueue_state_transitions02 ON t_srv_ordersqueue_state_transitions USING BTREE (status_id) ;
CREATE INDEX ix_t_srv_ordersqueue_state_transitions03 ON t_srv_ordersqueue_state_transitions USING BTREE (tstamp) ;




/* ------------------------------
	t_srv_pending_notifications
*/ ------------------------------
CREATE TABLE t_srv_pending_notifications (
	id SERIAL NOT NULL,
	product_uuid VARCHAR(160) NOT NULL,
	product_name VARCHAR(1024) NOT NULL,
	subscription_uuid VARCHAR(160),
	order_uuid VARCHAR(160),
	batchorder_uuid VARCHAR(160),
	subscriptionevent VARCHAR(8),
	notificationdate TIMESTAMP(3) DEFAULT NOW() NOT NULL,
	retrytime TIMESTAMP(3) DEFAULT NOW() NOT NULL,
	retrycount INTEGER DEFAULT 0 NOT NULL,
	endpoint VARCHAR(1024),
	endpointuser VARCHAR(32),
	endpointpassword VARCHAR(1024),
	completiondate TIMESTAMP(3),
	expirationdate TIMESTAMP(3)
) ;
ALTER TABLE t_srv_pending_notifications
	ADD CONSTRAINT pk_t_srv_pending_notifications
		PRIMARY KEY (id) ;

ALTER TABLE t_srv_pending_notifications 
	ADD CONSTRAINT ck_srv_pending_notifications01 
		CHECK (subscriptionevent IN ('CREATED','DELETED')) ;

ALTER TABLE t_srv_pending_notifications 
	ADD CONSTRAINT ck_srv_pending_notifications02 
		CHECK (((batchorder_uuid IS NOT NULL) AND (subscription_uuid IS NULL) AND (subscriptionevent IS NULL) AND (order_uuid IS NULL)) 
		OR ((batchorder_uuid IS NULL) AND (subscription_uuid IS NULL) AND (order_uuid IS NOT NULL)) 
		OR ((subscription_uuid IS NOT NULL) AND (batchorder_uuid IS NULL) AND (subscriptionevent IS NOT NULL)) 
		OR ((subscription_uuid IS NOT NULL) AND (batchorder_uuid IS NULL) AND ((subscriptionevent) = 'CREATED') AND (order_uuid IS NOT NULL))) ;

CREATE INDEX ix_t_srv_pending_notifications01 ON t_srv_pending_notifications USING BTREE (product_uuid) ;
CREATE INDEX ix_t_srv_pending_notifications02 ON t_srv_pending_notifications USING BTREE (subscription_uuid) ;
CREATE INDEX ix_t_srv_pending_notifications03 ON t_srv_pending_notifications USING BTREE (order_uuid) ;
CREATE INDEX ix_t_srv_pending_notifications04 ON t_srv_pending_notifications USING BTREE (batchorder_uuid) ;
CREATE INDEX ix_t_srv_pending_notifications05 ON t_srv_pending_notifications USING BTREE (retrycount) ;
CREATE INDEX ix_t_srv_pending_notifications06 ON t_srv_pending_notifications USING BTREE (completiondate) ;
CREATE INDEX ix_t_srv_pending_notifications07 ON t_srv_pending_notifications USING BTREE (expirationdate) ;




/* ------------------------------
	t_transferprotocols
*/ ------------------------------
CREATE TABLE t_transferprotocols (
	id INT2 NOT NULL,
	protocol VARCHAR(16) NOT NULL
) ;
ALTER TABLE t_transferprotocols 
	ADD CONSTRAINT pk_t_transferprotocols
		PRIMARY KEY (id);

ALTER TABLE t_transferprotocols 
	ADD CONSTRAINT uq_transferprotocols01
		UNIQUE (protocol);

INSERT INTO t_transferprotocols (id, protocol) VALUES (1, 'FTP') ;
INSERT INTO t_transferprotocols (id, protocol) VALUES (2, 'FTPS') ;
INSERT INTO t_transferprotocols (id, protocol) VALUES (3, 'SFTP') ;
INSERT INTO t_transferprotocols (id, protocol) VALUES (4, 'FILE') ;
INSERT INTO t_transferprotocols (id, protocol) VALUES (5, 'MEDIA') ;
INSERT INTO t_transferprotocols (id, protocol) VALUES (6, 'HTTP') ;
INSERT INTO t_transferprotocols (id, protocol) VALUES (7, 'HTTPS') ;




/* ------------------------------
	t_knownhosts
*/ ------------------------------
CREATE TABLE t_knownhosts (
	id SERIAL NOT NULL,
	ip_address VARCHAR(16),
	host_name VARCHAR(256)
) ;
ALTER TABLE t_knownhosts 
	ADD CONSTRAINT pk_t_knownhosts
		PRIMARY KEY (id) ;

ALTER TABLE t_knownhosts 
	ADD CONSTRAINT uq_knownhosts01
		UNIQUE (host_name) ;




/* ------------------------------
	t_tmpnamestrategy
*/ ------------------------------
CREATE TABLE t_tmpnamestrategy (
    id_tmpnamestrategy INT2 NOT NULL,
	tempname VARCHAR(24) NOT NULL
) ;
ALTER TABLE t_tmpnamestrategy 
	ADD CONSTRAINT pk_t_tmpnamestrategy
		PRIMARY KEY (id_tmpnamestrategy) ;

ALTER TABLE t_tmpnamestrategy 
	ADD CONSTRAINT uq_tmpnamestrategy01
		UNIQUE (tempname) ;

INSERT INTO t_tmpnamestrategy (id_tmpnamestrategy, tempname) VALUES (1, 'PREFIX') ;
INSERT INTO t_tmpnamestrategy (id_tmpnamestrategy, tempname) VALUES (2, 'SUFFIX') ;
INSERT INTO t_tmpnamestrategy (id_tmpnamestrategy, tempname) VALUES (3, 'TMPDIR') ;
INSERT INTO t_tmpnamestrategy (id_tmpnamestrategy, tempname) VALUES (4, 'TSTAMP') ;




/* ------------------------------
	t_srv_repositories
*/ ------------------------------
CREATE TABLE t_srv_repositories (
    id_repository SERIAL NOT NULL,
	symbolicname VARCHAR(64) NOT NULL,
	transferprotocol_id INTEGER NOT NULL,
	host_id INTEGER NOT NULL,
	username VARCHAR(32) NOT NULL,
	password VARCHAR(256),
	remotepath VARCHAR(1024) NOT NULL,
	tempname_id INT2 DEFAULT 1 NOT NULL,
	tempname_value VARCHAR(64) NOT NULL,
	allowedciphers VARCHAR(256),
	port SMALLINT,
	attributes JSONB
) ;
ALTER TABLE t_srv_repositories
	ADD CONSTRAINT pk_t_srv_repositories
		PRIMARY KEY (id_repository) ;

ALTER TABLE t_srv_repositories
	ADD CONSTRAINT ck_srv_repositories01			
		CHECK (char_length(trim(tempname_value)) != 0) ;

ALTER TABLE t_srv_repositories
	ADD CONSTRAINT ck_srv_repositories02
		CHECK ((tempname_id != 2 AND (tempname_value = '..' OR tempname_value = '../')) != TRUE) ;

ALTER TABLE t_srv_repositories
	ADD CONSTRAINT ck_srv_repositories03
		CHECK ((tempname_id = 3 AND (tempname_value = '.' OR tempname_value = './')) != TRUE) ;

ALTER TABLE t_srv_repositories
	ADD CONSTRAINT fk_t_srv_repositories1
		FOREIGN KEY (transferprotocol_id)
			REFERENCES t_transferprotocols (id)
				ON DELETE CASCADE ;

ALTER TABLE t_srv_repositories
	ADD CONSTRAINT fk_t_srv_repositories2
		FOREIGN KEY (host_id)
			REFERENCES t_knownhosts (id)
				ON DELETE CASCADE ;

ALTER TABLE t_srv_repositories
	ADD CONSTRAINT fk_t_srv_repositories3
		FOREIGN KEY (tempname_id)
			REFERENCES t_tmpnamestrategy (id_tmpnamestrategy)
				ON DELETE CASCADE ;

CREATE INDEX ix_t_srv_repositories01 ON t_srv_repositories USING BTREE (transferprotocol_id) ;
CREATE INDEX ix_t_srv_repositories02 ON t_srv_repositories USING BTREE (host_id) ;
CREATE INDEX ix_t_srv_repositories03 ON t_srv_repositories USING BTREE (tempname_id) ;




/* ------------------------------
	t_discoveryplugins 
*/ ------------------------------
CREATE TABLE t_discoveryplugins (
	id_discoveryplugin SERIAL NOT NULL,
	name VARCHAR(24) NOT NULL
) ;
ALTER TABLE t_discoveryplugins
	ADD CONSTRAINT pk_t_discoveryplugins
		PRIMARY KEY (id_discoveryplugin) ;

ALTER TABLE t_discoveryplugins 
	ADD CONSTRAINT uq_discoveryplugins01
		UNIQUE (name) ;

INSERT INTO t_discoveryplugins (name) VALUES ('DummyPlugin') ;




/* ------------------------------
	t_operationtypes
*/ ------------------------------
CREATE TABLE t_operationtypes (
	id_operationtype INT2 NOT NULL,
	type VARCHAR(32) NOT NULL
) ;
ALTER TABLE t_operationtypes
	ADD CONSTRAINT pk_t_operationtypes
		PRIMARY KEY (id_operationtype) ;

ALTER TABLE t_operationtypes 
	ADD CONSTRAINT uq_operationtypes01
		UNIQUE (type) ;

INSERT INTO t_operationtypes (id_operationtype, type) VALUES (1, 'UNKNOWN') ;
INSERT INTO t_operationtypes (id_operationtype, type) VALUES (2, 'IMPORT') ;
INSERT INTO t_operationtypes (id_operationtype, type) VALUES (3, 'EXPORT') ;
INSERT INTO t_operationtypes (id_operationtype, type) VALUES (4, 'UPLOAD') ;
INSERT INTO t_operationtypes (id_operationtype, type) VALUES (5, 'DOWNLOAD') ;
INSERT INTO t_operationtypes (id_operationtype, type) VALUES (6, 'EREASE') ;
INSERT INTO t_operationtypes (id_operationtype, type) VALUES (7, 'POSTPROC') ;




/* ------------------------------
	t_tasktables
*/ ------------------------------
CREATE TABLE t_tasktables (
    id_tasktable SERIAL NOT NULL,
	name VARCHAR(64) NOT NULL,
	tasktable TEXT,
	type_id INT2,
	description VARCHAR(256)
) ;
ALTER TABLE t_tasktables
	ADD CONSTRAINT pk_t_tasktables
		PRIMARY KEY (id_tasktable) ;

ALTER TABLE t_tasktables 
	ADD CONSTRAINT uq_tasktables01
		UNIQUE (name) ;

ALTER TABLE t_tasktables 
	ADD CONSTRAINT fk_tasktables1
		FOREIGN KEY (type_id)
			REFERENCES t_operationtypes (id_operationtype) 
				ON DELETE CASCADE ;

CREATE INDEX ix_t_tasktables01 ON t_tasktables USING BTREE (type_id) ;




/* ------------------------------
	t_srv_receptionrules
*/ ------------------------------
CREATE TABLE t_srv_receptionrules (
    id_receptionrule SERIAL NOT NULL,
    filetype_id INTEGER NOT NULL,
	repository_id INTEGER NOT NULL,
	priority INTEGER DEFAULT 1 NOT NULL,
	active BOOLEAN DEFAULT 't' NOT NULL,
	generateheader BOOLEAN DEFAULT 'f' NOT NULL,
	reception_tt_id INTEGER DEFAULT f_get_tasktable_id('defaultReception') NOT NULL,
	tt_parameters TEXT,
	hostname VARCHAR(256),
	localpath VARCHAR(1024) NOT NULL,
	connretries INT2 DEFAULT 1 NOT NULL,
	pollingperiod_sec INTEGER NOT NULL,
	retryperiod_sec INTEGER NOT NULL,
	connfailureaddress VARCHAR(1024),
	procfailureaddress VARCHAR(1024),
	lastvisit TIMESTAMP(3) DEFAULT NOW() NOT NULL,
	lastgood TIMESTAMP(3) DEFAULT NOW() NOT NULL,
	retrycounter INT2 DEFAULT 0 NOT NULL,
	discoveryplugin_id INTEGER NOT NULL,
	successnotificationaddress VARCHAR(1024),
	php_timeout_sec INTEGER,
	timewindow_sec INTEGER
) ;
ALTER TABLE t_srv_receptionrules
	ADD CONSTRAINT pk_t_srv_receptionrules
		PRIMARY KEY (id_receptionrule) ;

ALTER TABLE t_srv_receptionrules
	ADD CONSTRAINT ck_srv_receptionrules01
		CHECK (pollingperiod_sec > 0) ;

ALTER TABLE t_srv_receptionrules
	ADD CONSTRAINT ck_srv_receptionrules02
		CHECK ((retryperiod_sec > 0) AND (retryperiod_sec < floor(pollingperiod_sec/connretries))) ;

ALTER TABLE t_srv_receptionrules
	ADD CONSTRAINT ck_srv_receptionrules03
		CHECK (php_timeout_sec > 90) ;

CREATE UNIQUE INDEX uq_srv_receptionrules01 
	ON t_srv_receptionrules (filetype_id, repository_id)
		WHERE active = 't' ;

CREATE UNIQUE INDEX uq_srv_receptionrules02 
	ON t_srv_receptionrules (filetype_id, localpath, COALESCE(hostname, 'DUMMY_VALUE_USED_FOR_UNIQUE_CONSTRAINT'))
		WHERE active = 't' ;

ALTER TABLE t_srv_receptionrules 
	ADD CONSTRAINT fk_srv_receptionrules1
		FOREIGN KEY (filetype_id)
			REFERENCES t_filetypes (id_filetype) 
				ON DELETE CASCADE ;

ALTER TABLE t_srv_receptionrules 
	ADD CONSTRAINT fk_srv_receptionrules2
		FOREIGN KEY (repository_id)
			REFERENCES t_srv_repositories (id_repository) 
				ON DELETE CASCADE ;

ALTER TABLE t_srv_receptionrules 
	ADD CONSTRAINT fk_srv_receptionrules5
		FOREIGN KEY (reception_tt_id)
			REFERENCES t_tasktables (id_tasktable) 
				ON DELETE CASCADE ;

ALTER TABLE t_srv_receptionrules 
	ADD CONSTRAINT fk_srv_receptionrules6
		FOREIGN KEY (discoveryplugin_id)
			REFERENCES t_discoveryplugins (id_discoveryplugin) 
				ON DELETE CASCADE ;

CREATE INDEX ix_t_srv_receptionrules01 ON t_srv_receptionrules USING BTREE (filetype_id) ;
CREATE INDEX ix_t_srv_receptionrules02 ON t_srv_receptionrules USING BTREE (repository_id) ;
--CREATE INDEX ix_t_srv_receptionrules05 ON t_srv_receptionrules USING BTREE (reception_tt_id) ;
CREATE INDEX ix_t_srv_receptionrules06 ON t_srv_receptionrules USING BTREE (discoveryplugin_id) ;




/* ------------------------------
	t_srv_receptionruleshist
*/ ------------------------------
CREATE TABLE t_srv_receptionruleshist (
    id_receptionrulehist SERIAL NOT NULL,
	receptionrule_id INTEGER NOT NULL,
	filename VARCHAR(1024) NOT NULL,
	tstamp TIMESTAMP(3) NOT NULL,
	status_id BIGINT NOT NULL,
	filecreationtstamp TIMESTAMP(3),
	remoteuuid VARCHAR(160),
	evictiondate TIMESTAMP(3)
) ;
ALTER TABLE t_srv_receptionruleshist
	ADD CONSTRAINT pk_t_srv_receptionruleshist
		PRIMARY KEY (id_receptionrulehist) ;

ALTER TABLE t_srv_receptionruleshist 
	ADD CONSTRAINT uq_srv_receptionruleshist01
		UNIQUE (receptionrule_id, filename) ;

ALTER TABLE t_srv_receptionruleshist 
	ADD CONSTRAINT fk_srv_receptionruleshist1
		FOREIGN KEY (receptionrule_id)
			REFERENCES t_srv_receptionrules (id_receptionrule) 
				ON DELETE CASCADE ;

ALTER TABLE t_srv_receptionruleshist 
	ADD CONSTRAINT fk_srv_receptionruleshist2
		FOREIGN KEY (status_id)
			REFERENCES t_srv_statuses (id) 
				ON DELETE CASCADE ;

CREATE INDEX ix_t_srv_receptionruleshist01 ON t_srv_receptionruleshist USING BTREE (receptionrule_id) ;
CREATE INDEX ix_t_srv_receptionruleshist02 ON t_srv_receptionruleshist USING BTREE (filename) ;
CREATE INDEX ix_t_srv_receptionruleshist03 ON t_srv_receptionruleshist USING BTREE (status_id) ;
CREATE INDEX ix_t_srv_receptionruleshist04 ON t_srv_receptionruleshist USING BTREE (filecreationtstamp) ;
CREATE INDEX ix_t_srv_receptionruleshist05 ON t_srv_receptionruleshist USING BTREE (evictiondate) ;




/* ------------------------------
	t_srv_receptionruleslock
*/ ------------------------------
CREATE TABLE t_srv_receptionruleslock (
    receptionrule_id INTEGER NOT NULL,
    responsible VARCHAR(128) NOT NULL,
    tstamp TIMESTAMP(3) DEFAULT NOW() NOT NULL,
	processidentifier VARCHAR(128)
) ;
ALTER TABLE t_srv_receptionruleslock
	ADD CONSTRAINT pk_t_srv_receptionruleslock
		PRIMARY KEY (receptionrule_id) ;

ALTER TABLE t_srv_receptionruleslock
	ADD CONSTRAINT fk_srv_receptionruleslock01
		FOREIGN KEY (receptionrule_id)
			REFERENCES t_srv_receptionrules (id_receptionrule)
				ON DELETE CASCADE ;

CREATE INDEX ix_t_srv_receptionruleslock01 ON t_srv_receptionruleslock USING BTREE (receptionrule_id) ;




/* ------------------------------
	t_srv_waiting_orders_queue
*/ ------------------------------
CREATE TABLE t_srv_waiting_orders_queue (
    id SERIAL NOT NULL,
	order_id BIGINT NOT NULL,
	file_path VARCHAR(512) NOT NULL,
	expiration_date TIMESTAMP(3) DEFAULT (NOW() + INTERVAL '3 MONTH') NOT NULL
) ;
ALTER TABLE t_srv_waiting_orders_queue
	ADD CONSTRAINT pk_t_srv_waiting_orders_queue
		PRIMARY KEY (id) ;

ALTER TABLE t_srv_waiting_orders_queue 
	ADD CONSTRAINT uq_srv_waiting_orders_queue01
		UNIQUE (order_id, file_path) ;

ALTER TABLE t_srv_waiting_orders_queue 
	ADD CONSTRAINT fk_srv_waiting_orders_queue01
		FOREIGN KEY (order_id)
			REFERENCES t_srv_ordersqueue (id)
				ON DELETE CASCADE ;

CREATE INDEX ix_t_srv_waiting_orders_queue01 ON t_srv_waiting_orders_queue USING BTREE (order_id) ;
CREATE INDEX ix_t_srv_waiting_orders_queue02 ON t_srv_waiting_orders_queue USING BTREE (file_path) ;
CREATE INDEX ix_t_srv_waiting_orders_queue03 ON t_srv_waiting_orders_queue USING BTREE (expiration_date) ;




/* ------------------------------
	t_srv_metrics
*/ ------------------------------
CREATE TABLE t_srv_metrics (
    id BIGSERIAL NOT NULL,
    name VARCHAR(256) NOT NULL,
    tstamp TIMESTAMP(3) NOT NULL DEFAULT NOW(),
	metrictype VARCHAR(24) NOT NULL DEFAULT 'Counter',
	counter BIGINT,
	gauge DOUBLE PRECISION
);
ALTER TABLE t_srv_metrics 
	ADD CONSTRAINT pk_t_srv_metrics
		PRIMARY KEY (id) ;

ALTER TABLE t_srv_metrics 
	ADD CONSTRAINT ck_srv_metrics01 
		CHECK (metrictype IN ('Counter','Gauge')) ;

ALTER TABLE t_srv_metrics 
	ADD CONSTRAINT ck_srv_metrics02 
		CHECK ((metrictype = 'Counter' AND counter IS NOT NULL) OR (metrictype = 'Gauge' AND gauge IS NOT NULL));

CREATE INDEX ix_t_srv_metrics01 ON t_srv_metrics USING BTREE (name) ;
CREATE INDEX ix_t_srv_metrics02 ON t_srv_metrics USING BTREE (tstamp) ;
CREATE INDEX ix_t_srv_metrics03 ON t_srv_metrics USING BTREE (metrictype) ;




/* ------------------------------
	t_srv_totalmetrics
*/ ------------------------------
CREATE TABLE t_srv_totalmetrics (
    id BIGSERIAL NOT NULL,
    productType VARCHAR(32) NOT NULL,
    platformShortName VARCHAR(64) NOT NULL,
	platformSerialIdentifier VARCHAR(32) NOT NULL,
	archivesize BIGINT NOT NULL DEFAULT 0,
	archivecount BIGINT NOT NULL DEFAULT 0,
	retrievesize BIGINT NOT NULL DEFAULT 0,
	retrievecompletecount BIGINT NOT NULL DEFAULT 0,
	retrievefailcount BIGINT NOT NULL DEFAULT 0,
	downloadsize BIGINT NOT NULL DEFAULT 0,
	downloadcompletecount BIGINT NOT NULL DEFAULT 0,
	downloadfailcount BIGINT NOT NULL DEFAULT 0,
	servicealias VARCHAR(64)
);
ALTER TABLE t_srv_totalmetrics 
	ADD CONSTRAINT pk_t_srv_totalmetrics
		PRIMARY KEY (id) ;

CREATE UNIQUE INDEX uq_idx_totalmetrics01 
	ON t_srv_totalmetrics (producttype, platformshortname, platformserialidentifier, COALESCE(servicealias, 'DUMMY')) ;

CREATE INDEX ix_t_srv_totalmetrics01 ON t_srv_totalmetrics USING BTREE (productType) ;
CREATE INDEX ix_t_srv_totalmetrics02 ON t_srv_totalmetrics USING BTREE (platformShortName) ;
CREATE INDEX ix_t_srv_totalmetrics03 ON t_srv_totalmetrics USING BTREE (platformserialidentifier) ;




/* ------------------------------
	t_srv_distributionscope
*/ ------------------------------
CREATE TABLE t_srv_distributionscope (
    id SERIAL NOT NULL,
	distributionscope VARCHAR(512) NOT NULL
) ;
ALTER TABLE t_srv_distributionscope
	ADD CONSTRAINT pk_t_srv_distributionscope
		PRIMARY KEY (id) ;

ALTER TABLE t_srv_distributionscope
	ADD CONSTRAINT uq_srv_distributionscope01
		UNIQUE (distributionscope) ;



/* ------------------------------
	t_srv_eventcategory
*/ ------------------------------
CREATE TABLE t_srv_eventcategory (
    id SERIAL NOT NULL,
	eventcategory VARCHAR(512) NOT NULL
) ;
ALTER TABLE t_srv_eventcategory
	ADD CONSTRAINT pk_t_srv_eventcategory
		PRIMARY KEY (id) ;

ALTER TABLE t_srv_eventcategory
	ADD CONSTRAINT uq_srv_eventcategory01
		UNIQUE (eventcategory) ;



/* ------------------------------
	t_srv_events
*/ ------------------------------
CREATE TABLE t_srv_events (
    id BIGSERIAL NOT NULL,
	uuid VARCHAR(160) DEFAULT uuid_generate_v1() NOT NULL,
	title VARCHAR (1024),
	description VARCHAR (1024),
	eventcategory_id INTEGER,
	eventdate TIMESTAMP(3) NOT NULL DEFAULT NOW(),
	publicationdate TIMESTAMP(3) NOT NULL DEFAULT NOW(),
	modificationdate TIMESTAMP(3) NOT NULL DEFAULT NOW(),
	distributionscope_id INTEGER,
	informationuri VARCHAR(1024)
) ;
ALTER TABLE t_srv_events
	ADD CONSTRAINT pk_t_srv_events
		PRIMARY KEY (id) ;

ALTER TABLE t_srv_events
	ADD CONSTRAINT fk_srv_events01
		FOREIGN KEY (eventcategory_id)
			REFERENCES t_srv_eventcategory (id)
				ON DELETE CASCADE ;

ALTER TABLE t_srv_events
	ADD CONSTRAINT fk_srv_events02
		FOREIGN KEY (distributionscope_id)
			REFERENCES t_srv_distributionscope (id)
				ON DELETE CASCADE ;

CREATE INDEX ix_t_srv_events01 ON t_srv_events USING BTREE (uuid) ;
CREATE INDEX ix_t_srv_events02 ON t_srv_events USING BTREE (eventcategory_id) ;
CREATE INDEX ix_t_srv_events03 ON t_srv_events USING BTREE (distributionscope_id) ;



/* ------------------------------
	t_srv_lastmetrics
*/ ------------------------------
CREATE TABLE t_srv_lastmetrics (
    id BIGSERIAL NOT NULL,
    name VARCHAR(256) NOT NULL,
    tstamp TIMESTAMP(3) NOT NULL DEFAULT NOW(),
	metrictype VARCHAR(24) NOT NULL DEFAULT 'Counter',
	counter BIGINT,
	gauge DOUBLE PRECISION
);
ALTER TABLE t_srv_lastmetrics 
	ADD CONSTRAINT pk_t_srv_lastmetrics
		PRIMARY KEY (id) ;

ALTER TABLE t_srv_lastmetrics 
	ADD CONSTRAINT ck_srv_lastmetrics01 
		CHECK (metrictype IN ('Counter','Gauge')) ;

ALTER TABLE t_srv_lastmetrics 
	ADD CONSTRAINT ck_srv_lastmetrics02 
		CHECK ((metrictype = 'Counter' AND counter IS NOT NULL) OR (metrictype = 'Gauge' AND gauge IS NOT NULL));

ALTER TABLE t_srv_lastmetrics 
	ADD CONSTRAINT uq_srv_lastmetrics01
		UNIQUE (name) ;

CREATE INDEX ix_t_srv_lastmetrics01 ON t_srv_lastmetrics USING BTREE (name) ;
CREATE INDEX ix_t_srv_lastmetrics02 ON t_srv_lastmetrics USING BTREE (tstamp) ;
CREATE INDEX ix_t_srv_lastmetrics03 ON t_srv_lastmetrics USING BTREE (metrictype) ;








-------------------------------------------------------------------------------
-------------------------------- TABLES INSERT --------------------------------
-------------------------------------------------------------------------------

/* ------------------------------
	t_statetransitions_tables
*/ ------------------------------
INSERT INTO t_statetransitions_tables (table_name) VALUES ('t_srv_aip_content') ;
INSERT INTO t_statetransitions_tables (table_name) VALUES ('t_srv_awsuploadqueue') ;
INSERT INTO t_statetransitions_tables (table_name) VALUES ('t_srv_bulkbatches') ;
INSERT INTO t_statetransitions_tables (table_name) VALUES ('t_srv_bulk_orders') ;
INSERT INTO t_statetransitions_tables (table_name) VALUES ('t_srv_ordersqueue') ;



/* ------------------------------
	t_statetransitions_allowed
*/ ------------------------------

-- t_srv_aip_content
----------------------
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_srv_aip_content'),
	(SELECT id FROM t_srv_statuses WHERE status = 'ONGOING'),
	(SELECT id FROM t_srv_statuses WHERE status = 'ONLINE')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_srv_aip_content'),
	(SELECT id FROM t_srv_statuses WHERE status = 'ONLINE'),
	(SELECT id FROM t_srv_statuses WHERE status = 'INEVICTION')) ;

-- t_srv_awsuploadqueue
----------------------
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_srv_awsuploadqueue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'CREATED'),
	(SELECT id FROM t_srv_statuses WHERE status = 'IN_PROGRESS')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_srv_awsuploadqueue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'IN_PROGRESS'),
	(SELECT id FROM t_srv_statuses WHERE status = 'COMPLETED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_srv_awsuploadqueue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'IN_PROGRESS'),
	(SELECT id FROM t_srv_statuses WHERE status = 'RECOVERABLEERROR')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_srv_awsuploadqueue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'IN_PROGRESS'),
	(SELECT id FROM t_srv_statuses WHERE status = 'UNRECOVERABLEERROR')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_srv_awsuploadqueue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'RECOVERABLEERROR'),
	(SELECT id FROM t_srv_statuses WHERE status = 'IN_PROGRESS')) ;

-- t_srv_bulkbatches
----------------------
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_srv_bulkbatches'),
	(SELECT id FROM t_srv_statuses WHERE status = 'QUEUED'),
	(SELECT id FROM t_srv_statuses WHERE status = 'CANCELLED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_srv_bulkbatches'),
	(SELECT id FROM t_srv_statuses WHERE status = 'QUEUED'),
	(SELECT id FROM t_srv_statuses WHERE status = 'IN_PROGRESS')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_srv_bulkbatches'),
	(SELECT id FROM t_srv_statuses WHERE status = 'CANCELLED'),
	(SELECT id FROM t_srv_statuses WHERE status = 'CANCELLED')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_srv_bulkbatches'),
	(SELECT id FROM t_srv_statuses WHERE status = 'IN_PROGRESS'),
	(SELECT id FROM t_srv_statuses WHERE status = 'CANCELLED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_srv_bulkbatches'),
	(SELECT id FROM t_srv_statuses WHERE status = 'IN_PROGRESS'),
	(SELECT id FROM t_srv_statuses WHERE status = 'COMPLETED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_srv_bulkbatches'),
	(SELECT id FROM t_srv_statuses WHERE status = 'IN_PROGRESS'),
	(SELECT id FROM t_srv_statuses WHERE status = 'ERROR')) ;

-- t_srv_bulk_orders
----------------------
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_srv_bulk_orders'),
	(SELECT id FROM t_srv_statuses WHERE status = 'CREATED'),
	(SELECT id FROM t_srv_statuses WHERE status = 'CANCELLED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_srv_bulk_orders'),
	(SELECT id FROM t_srv_statuses WHERE status = 'CREATED'),
	(SELECT id FROM t_srv_statuses WHERE status = 'IN_PROGRESS')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_srv_bulk_orders'),
	(SELECT id FROM t_srv_statuses WHERE status = 'CANCELLED'),
	(SELECT id FROM t_srv_statuses WHERE status = 'CANCELLED')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_srv_bulk_orders'),
	(SELECT id FROM t_srv_statuses WHERE status = 'IN_PROGRESS'),
	(SELECT id FROM t_srv_statuses WHERE status = 'CANCELLED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_srv_bulk_orders'),
	(SELECT id FROM t_srv_statuses WHERE status = 'IN_PROGRESS'),
	(SELECT id FROM t_srv_statuses WHERE status = 'COMPLETED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_srv_bulk_orders'),
	(SELECT id FROM t_srv_statuses WHERE status = 'IN_PROGRESS'),
	(SELECT id FROM t_srv_statuses WHERE status = 'ERROR')) ;

-- t_srv_ordersqueue
----------------------
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_srv_ordersqueue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_srv_statuses WHERE status = 'CANCELLED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_srv_ordersqueue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_srv_statuses WHERE status = 'BOOKED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_srv_ordersqueue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'ELIGIBLE'),
	(SELECT id FROM t_srv_statuses WHERE status = 'ERROR')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_srv_ordersqueue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'BOOKED'),
	(SELECT id FROM t_srv_statuses WHERE status = 'ELIGIBLE')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_srv_ordersqueue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'BOOKED'),
	(SELECT id FROM t_srv_statuses WHERE status = 'CANCELLED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_srv_ordersqueue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'BOOKED'),
	(SELECT id FROM t_srv_statuses WHERE status = 'COMPLETED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_srv_ordersqueue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'BOOKED'),
	(SELECT id FROM t_srv_statuses WHERE status = 'LTA_DOWNLOADING')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_srv_ordersqueue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'BOOKED'),
	(SELECT id FROM t_srv_statuses WHERE status = 'ERROR')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_srv_ordersqueue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'LTA_DOWNLOADING'),
	(SELECT id FROM t_srv_statuses WHERE status = 'CANCELLED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_srv_ordersqueue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'LTA_DOWNLOADING'),
	(SELECT id FROM t_srv_statuses WHERE status = 'DISTRIBUTION')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_srv_ordersqueue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'LTA_DOWNLOADING'),
	(SELECT id FROM t_srv_statuses WHERE status = 'ELIGIBLE')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_srv_ordersqueue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'LTA_DOWNLOADING'),
	(SELECT id FROM t_srv_statuses WHERE status = 'ERROR')) ;

INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_srv_ordersqueue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'DISTRIBUTION'),
	(SELECT id FROM t_srv_statuses WHERE status = 'CANCELLED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_srv_ordersqueue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'DISTRIBUTION'),
	(SELECT id FROM t_srv_statuses WHERE status = 'COMPLETED')) ;
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_srv_ordersqueue'),
	(SELECT id FROM t_srv_statuses WHERE status = 'DISTRIBUTION'),
	(SELECT id FROM t_srv_statuses WHERE status = 'ERROR')) ;








--------------------------------------------------------------------------------
------------------------------ PROCEDURES CREATION -----------------------------
--------------------------------------------------------------------------------

/* --------------------------
	p_srv_add_order_link
*/ --------------------------
CREATE OR REPLACE PROCEDURE p_srv_add_order_link(
	ordq_id BIGINT) AS $$

DECLARE
	err_msg VARCHAR ;
	
BEGIN
	IF ((SELECT links_count FROM t_srv_ordersqueue WHERE id = ordq_id) > 0) 
		OR ((SELECT status_id FROM t_srv_ordersqueue WHERE id = ordq_id) IN (SELECT id FROM t_srv_statuses WHERE status IN ('ELIGIBLE','COMPLETED')))
	THEN
		UPDATE t_srv_ordersqueue SET links_count = links_count +1 WHERE id = ordq_id ;
	ELSE
		err_msg := 'p_srv_add_order_link: order id ' || ordq_id::VARCHAR || 'must have links count > 0 or staus in ELIGIBLE or COMPLETED mode' ;
		RAISE EXCEPTION '%', err_msg ;
	END IF ;
	
END;
$$ LANGUAGE PLPGSQL ;



/* --------------------------
	p_srv_batchorder_status_update
*/ --------------------------
CREATE OR REPLACE PROCEDURE p_srv_batchorder_status_update(
	in_bat_ord_id INTEGER) AS $$

DECLARE
	compl_status_id BIGINT ;
	
BEGIN
	SELECT id INTO compl_status_id FROM t_srv_statuses WHERE status = 'COMPLETED' ;
	
	IF	(SELECT status_id FROM t_srv_bulkbatches WHERE id = in_bat_ord_id) != 
		(SELECT id FROM t_srv_statuses WHERE status = 'IN_PROGRESS') 
	THEN
		RETURN ;
	END IF ;
	
	IF	(SELECT count(*) FROM t_srv_bulkbatch_x_ordersqueue WHERE bulkbatch_id = in_bat_ord_id) = 
		(SELECT count(*) FROM t_srv_ordersqueue oq INNER JOIN t_srv_bulkbatch_x_ordersqueue bxq ON oq.id = bxq.order_id WHERE bxq.bulkbatch_id = in_bat_ord_id AND oq.status_id = compl_status_id) 
	THEN
		UPDATE t_srv_bulkbatches SET status_id = compl_status_id WHERE id = in_bat_ord_id ;
	END IF ;
	
END;
$$ LANGUAGE PLPGSQL ;



/* --------------------------
	p_srv_compute_batch_status
*/ --------------------------
CREATE OR REPLACE PROCEDURE p_srv_compute_batch_status(
	btch_ord_id INTEGER) AS $$

DECLARE
	compl_status_id BIGINT ;
	error_status_id BIGINT ;
	cancel_status_id BIGINT ;
	
BEGIN
	-- All referenced orders are COMPLETED
	SELECT id INTO compl_status_id FROM t_srv_statuses WHERE status = 'COMPLETED' ;
	IF	(SELECT count(*) FROM t_srv_bulkbatch_x_ordersqueue WHERE bulkbatch_id = btch_ord_id) = 
		(SELECT count(*) FROM t_srv_ordersqueue oq INNER JOIN t_srv_bulkbatch_x_ordersqueue bxq ON oq.id = bxq.order_id WHERE bxq.bulkbatch_id = btch_ord_id AND oq.status_id = compl_status_id) 
	THEN
		UPDATE t_srv_bulkbatches SET status_id = compl_status_id WHERE id = btch_ord_id ;
		RETURN ;
	END IF ;
	
	-- At least one order is in ERROR
	SELECT id INTO error_status_id FROM t_srv_statuses WHERE status = 'ERROR' ;
	IF (SELECT count(*) FROM t_srv_ordersqueue oq INNER JOIN t_srv_bulkbatch_x_ordersqueue bxq ON oq.id = bxq.order_id WHERE bxq.bulkbatch_id = btch_ord_id AND oq.status_id = error_status_id) > 0 THEN
		UPDATE t_srv_bulkbatches SET tobecancelled = TRUE, status_id = error_status_id WHERE id = btch_ord_id ;
		RETURN ;
	END IF ;
	
	-- At least one order is CANCELLED
	SELECT id INTO cancel_status_id FROM t_srv_statuses WHERE status = 'CANCELLED' ;
	IF (SELECT count(*) FROM t_srv_ordersqueue oq INNER JOIN t_srv_bulkbatch_x_ordersqueue bxq ON oq.id = bxq.order_id WHERE bxq.bulkbatch_id = btch_ord_id AND oq.status_id = cancel_status_id) > 0 THEN
		UPDATE t_srv_bulkbatches SET tobecancelled = TRUE, status_id = cancel_status_id WHERE id = btch_ord_id ;
		RETURN ;
	END IF ;
	
END;
$$ LANGUAGE PLPGSQL ;



/* --------------------------
	p_srv_del_order_link
*/ --------------------------
CREATE OR REPLACE PROCEDURE p_srv_del_order_link(
	ordq_id BIGINT) AS $$
BEGIN
	IF (SELECT links_count FROM t_srv_ordersqueue WHERE id = ordq_id) > 0 THEN
		UPDATE t_srv_ordersqueue SET links_count = links_count -1 WHERE id = ordq_id ;
	END IF ;
	
END;
$$ LANGUAGE PLPGSQL ;



/* --------------------------
	p_srv_successful_inventory
*/ --------------------------
CREATE OR REPLACE PROCEDURE p_srv_successful_inventory(
	in_inv_id BIGINT) AS $$
BEGIN
	INSERT INTO t_srv_pendinginv (
		inv_id, 
		uuid, 
		name, 
		contenttype, 
		contentlength, 
		origindate,
		creationdate,
		modificationdate,
		checksum,
		validitystart,
		validitystop,
		footprint,
		validity,
		filetype_id,
		satellite_id,
		sensor_id,
		mission,
		evictiondate,
		productiontype,
		attributes,
		trace_id,
		operation
	)
		SELECT *, 'INSERT' FROM t_srv_inventory WHERE id = in_inv_id ;
		
	INSERT INTO t_srv_catalogue_change_registry (sensingStartDate, satellite_id)
		SELECT validitystart::DATE, satellite_id FROM t_srv_inventory WHERE id = in_inv_id AND satellite_id IS NOT NULL
	ON CONFLICT DO NOTHING ;
	
END;
$$ LANGUAGE PLPGSQL ;








--------------------------------------------------------------------------------
-------------------------- TRIGGER FUNCTIONS CREATION --------------------------
--------------------------------------------------------------------------------

/* --------------------------
	tf_srv_aip_content_ins_bfr
*/ --------------------------
CREATE OR REPLACE FUNCTION tf_srv_aip_content_ins_bfr() RETURNS TRIGGER AS $$

BEGIN
	IF NEW.product_size IS NULL THEN
		NEW.product_size := (SELECT contentlength FROM t_srv_inventory WHERE uuid = NEW.product_uuid) ;
	END IF ;
	
	RETURN NEW ;
	
END ;
$$ LANGUAGE plpgsql ;



/* --------------------------
	tf_srv_aip_content_del_aft
*/ --------------------------
CREATE OR REPLACE FUNCTION tf_srv_aip_content_del_aft() RETURNS TRIGGER AS $$

BEGIN
	IF (OLD.status_id != (SELECT id FROM t_srv_statuses WHERE status = 'ONGOING')) THEN
		DELETE FROM t_srv_ordersqueue WHERE id = OLD.order_id ;
	END IF;
	
	RETURN NEW ;
	
END ;
$$ LANGUAGE plpgsql ;



/* --------------------------
	tf_srv_bulk_order_upd_bfr
*/ --------------------------
CREATE OR REPLACE FUNCTION tf_srv_bulk_order_upd_bfr() RETURNS TRIGGER AS $$

BEGIN
	IF NEW.modificationdate IS NULL THEN
		NEW.modificationdate := NOW()::TIMESTAMP(3) ;
	END IF ;
	
	IF OLD.tobecancelled = TRUE AND NEW.tobecancelled = FALSE THEN
		RAISE EXCEPTION 'tf_srv_bulk_order_upd_bfr: Cannot change tobecancelled flag from TRUE to FALSE';
	END IF ;
	
	RETURN NEW ;
	
END ;
$$ LANGUAGE plpgsql ;



/* --------------------------
	tf_srv_bulk_order_to_be_cancelled
*/ --------------------------
CREATE OR REPLACE FUNCTION tf_srv_bulk_order_to_be_cancelled() RETURNS TRIGGER AS $$

BEGIN
	UPDATE t_srv_bulkbatches SET tobecancelled = TRUE
		WHERE bulk_id = NEW.id AND status_id = (SELECT id FROM t_srv_statuses WHERE status = 'CREATED') ;
	
	RETURN NEW ;
	
END ;
$$ LANGUAGE plpgsql ;


/* --------------------------
	tf_srv_bulk_order_in_error
*/ --------------------------
CREATE OR REPLACE FUNCTION tf_srv_bulk_order_in_error() RETURNS TRIGGER AS $$

BEGIN
	IF NEW.status_id = (SELECT id FROM t_srv_statuses WHERE status = 'ERROR') THEN
		UPDATE t_srv_bulkbatches SET tobecancelled = TRUE
			WHERE bulk_id = NEW.id AND status_id = (SELECT id FROM t_srv_statuses WHERE status = 'CREATED') ;
	END IF;
	
	RETURN NEW ;
	
END ;
$$ LANGUAGE plpgsql ;


/* --------------------------
	tf_srv_bulkbatches_upd_bfr
*/ --------------------------
CREATE OR REPLACE FUNCTION tf_srv_bulkbatches_upd_bfr() RETURNS TRIGGER AS $$

BEGIN
	IF NEW.modificationdate IS NULL THEN
		NEW.modificationdate := NOW()::TIMESTAMP(3) ;
	END IF ;
	
	IF OLD.tobecancelled = TRUE AND NEW.tobecancelled = FALSE THEN
		RAISE EXCEPTION 'tf_srv_bulkbatches_upd_bfr: Cannot change tobecancelled flag from TRUE to FALSE';
	END IF ;
	
	RETURN NEW ;
	
END ;
$$ LANGUAGE plpgsql ;



/* --------------------------
	tf_srv_bulkbatches_upd_bulk
*/ --------------------------
CREATE OR REPLACE FUNCTION tf_srv_bulkbatches_upd_bulk() RETURNS TRIGGER AS $$

BEGIN
	IF NEW.status_id = (SELECT id FROM t_srv_statuses WHERE status = 'ERROR') THEN
		UPDATE t_srv_bulk_orders SET status_id = (SELECT id FROM t_srv_statuses WHERE status = 'ERROR') 
			WHERE id = NEW.bulk_id AND status_id IN (SELECT id FROM t_srv_statuses WHERE status IN ('CREATED','IN_PROGRESS'));
		UPDATE t_srv_bulkbatches SET tobecancelled = TRUE ;
	END IF ;
	
	IF NEW.status_id = (SELECT id FROM t_srv_statuses WHERE status = 'COMPLETED') THEN
		IF (SELECT count(*) FROM t_srv_bulkbatches WHERE bulk_id = NEW.bulk_id) = (SELECT count(*) FROM t_srv_bulkbatches WHERE bulk_id = NEW.bulk_id AND status_id = (SELECT id FROM t_srv_statuses WHERE status = 'COMPLETED')) THEN
			UPDATE t_srv_bulk_orders SET status_id = (SELECT id FROM t_srv_statuses WHERE status = 'COMPLETED')
				WHERE id = NEW.bulk_id ;
		END IF ;
	END IF ;
	
	IF NEW.status_id = (SELECT id FROM t_srv_statuses WHERE status = 'CANCELLED') THEN
		UPDATE t_srv_bulk_orders SET status_id = (SELECT id FROM t_srv_statuses WHERE status = 'CANCELLED') 
			WHERE id = NEW.bulk_id AND status_id IN (SELECT id FROM t_srv_statuses WHERE status IN ('CREATED','IN_PROGRESS'));
	END IF ;
	
	IF NEW.status_id = (SELECT id FROM t_srv_statuses WHERE status = 'IN_PROGRESS') THEN
		UPDATE t_srv_bulk_orders SET status_id = (SELECT id FROM t_srv_statuses WHERE status = 'IN_PROGRESS') 
			WHERE id = NEW.bulk_id AND status_id = (SELECT id FROM t_srv_statuses WHERE status = 'CREATED');
	END IF ;
	
	RETURN NEW ;
	
END ;
$$ LANGUAGE plpgsql ;



/* --------------------------
	tf_srv_bulkbatches_to_be_cancelled
*/ --------------------------
CREATE OR REPLACE FUNCTION tf_srv_bulkbatches_to_be_cancelled() RETURNS TRIGGER AS $$

DECLARE
	child_ord_id RECORD ;
	
BEGIN
	UPDATE t_srv_bulk_orders SET tobecancelled = TRUE
		WHERE id = NEW.bulk_id AND status_id IN (SELECT id FROM t_srv_statuses WHERE status IN ('CREATED','IN_PROGRESS')) ;
	
	IF (NEW.status_id = (SELECT id FROM t_srv_statuses WHERE status = 'QUEUED')) 
	THEN
		UPDATE t_srv_bulkbatches SET tobecancelled = TRUE WHERE id = NEW.id ;
	ELSE
		FOR child_ord_id IN (
			SELECT order_id FROM t_srv_bulkbatch_x_ordersqueue WHERE bulkbatch_id = NEW.id )
		LOOP
			CALL p_srv_del_order_link(child_ord_id.order_id) ;
		END LOOP ;
	END IF ;
	
	RETURN NEW ;
	
END ;
$$ LANGUAGE plpgsql ;



/* --------------------------
	tf_srv_bulkbatch_x_ordersqueue_add_link
*/ --------------------------
CREATE OR REPLACE FUNCTION tf_srv_bulkbatch_x_ordersqueue_add_link() RETURNS TRIGGER AS $$
	
BEGIN
	CALL p_srv_add_order_link(NEW.order_id);
	
	RETURN NEW ;
	
END ;
$$ LANGUAGE plpgsql ;


/* --------------------------
	tf_srv_bulkbatch_x_ordersqueue_del_link
*/ --------------------------
CREATE OR REPLACE FUNCTION tf_srv_bulkbatch_x_ordersqueue_del_link() RETURNS TRIGGER AS $$
	
BEGIN
	IF (SELECT tobecancelled FROM t_srv_bulkbatches WHERE id = OLD.bulkbatch_id) = FALSE THEN
		CALL p_srv_del_order_link(OLD.order_id);
	END IF ;
	
	RETURN OLD ;
	
END ;
$$ LANGUAGE plpgsql ;


/* --------------------------
	tf_srv_bulkbatch_x_ordersqueue_prevent_update
*/ --------------------------
CREATE OR REPLACE FUNCTION tf_srv_bulkbatch_x_ordersqueue_prevent_update() RETURNS TRIGGER AS $$
	
BEGIN
	RAISE EXCEPTION 'tf_srv_bulkbatch_x_ordersqueue_prevent_update: Cannot update table t_srv_bulkbatch_x_ordersqueue';
	
	RETURN OLD ;
	
END ;
$$ LANGUAGE plpgsql ;



/* --------------------------
	f_srv_computeevictiondate
*/ --------------------------
CREATE OR REPLACE FUNCTION tf_srv_computeevictiondate() RETURNS TRIGGER AS $$

DECLARE
	dt VARCHAR ;
	
BEGIN
	SELECT deltatime::VARCHAR INTO dt FROM t_srv_filetype_x_eviction WHERE filetype_id = NEW.filetype_id ;
	
	IF dt IS NOT NULL THEN
		NEW.evictiondate := NOW() + dt::INTERVAL SECOND ;
	END IF ;
	
	RETURN NEW ;
	
END ;
$$ LANGUAGE plpgsql ;



/* --------------------------
	tf_srv_ineviction_check
*/ --------------------------
CREATE OR REPLACE FUNCTION tf_srv_ineviction_check() RETURNS TRIGGER AS $$
	
BEGIN
	IF (OLD.eviction_time != NEW.eviction_time AND NEW.status_id = (SELECT id FROM t_srv_statuses WHERE status = 'INEVICTION')) THEN
		RAISE EXCEPTION 'tf_srv_ineviction_check: Cannot change eviction_time for a record in INEVICTION status';
	END IF ;
	
	IF (NEW.status_id = (SELECT id FROM t_srv_statuses WHERE status = 'INEVICTION') AND NEW.eviction_time > NOW()) THEN
		RAISE EXCEPTION 'tf_srv_ineviction_check: Cannot change status to INEVICTION when eviction_time is in the future';
	END IF ;
	
	RETURN NEW ;
	
END ;
$$ LANGUAGE plpgsql ;



/* --------------------------
	tf_srv_ordersqueue_to_be_cancelled
*/ --------------------------
/*CREATE OR REPLACE FUNCTION tf_srv_ordersqueue_to_be_cancelled() RETURNS TRIGGER AS $$

BEGIN
	UPDATE t_srv_bulkbatches SET tobecancelled = TRUE
		WHERE id IN (SELECT bulkbatch_id FROM t_srv_bulkbatch_x_ordersqueue WHERE order_id = NEW.id) 
		AND status_id = (SELECT id FROM t_srv_statuses WHERE status = 'IN_PROGRESS') ;
	
	RETURN NEW ;
	
END ;
$$ LANGUAGE plpgsql ;*/
--  |
--  |
--  V
/* --------------------------
	tf_srv_ordersqueue_upd_link_count
*/ --------------------------
CREATE OR REPLACE FUNCTION tf_srv_ordersqueue_upd_link_count() RETURNS TRIGGER AS $$

BEGIN
	IF (NEW.links_count = 0 AND (NEW.status_id NOT IN (SELECT id FROM t_srv_statuses WHERE status IN ('COMPLETED', 'ERROR')))) THEN
		UPDATE t_srv_bulkbatches SET tobecancelled = TRUE
			WHERE id IN (SELECT bulkbatch_id FROM t_srv_bulkbatch_x_ordersqueue WHERE order_id = NEW.id) 
			AND status_id = (SELECT id FROM t_srv_statuses WHERE status = 'IN_PROGRESS') ;
	END IF ;
	
	RETURN NEW ;
	
END ;
$$ LANGUAGE plpgsql ;



/* --------------------------
	tf_srv_ordersqueue_status_update
*/ --------------------------
CREATE OR REPLACE FUNCTION tf_srv_ordersqueue_status_update() RETURNS TRIGGER AS $$

DECLARE
	btch_parent RECORD ;
	
BEGIN
	IF NEW.status_id = (SELECT id FROM t_srv_statuses WHERE status = 'ERROR') THEN
		UPDATE t_srv_bulkbatches SET status_id = (SELECT id FROM t_srv_statuses WHERE status = 'ERROR') 
		WHERE id IN (SELECT bulkbatch_id FROM t_srv_bulkbatch_x_ordersqueue WHERE order_id = NEW.id) AND status_id IN (SELECT id FROM t_srv_statuses WHERE status IN ('CREATED','IN_PROGRESS'));
	END IF ;
	
	IF NEW.status_id = (SELECT id FROM t_srv_statuses WHERE status = 'COMPLETED') THEN
		FOR btch_parent IN (
			SELECT bulkbatch_id FROM t_srv_bulkbatch_x_ordersqueue WHERE order_id = NEW.id)
		LOOP
			IF	(SELECT count(*) FROM t_srv_bulkbatch_x_ordersqueue WHERE bulkbatch_id = btch_parent.bulkbatch_id) = 
				(SELECT count(*) FROM t_srv_ordersqueue oq INNER JOIN t_srv_bulkbatch_x_ordersqueue bxq ON oq.id = bxq.order_id WHERE bxq.bulkbatch_id = btch_parent.bulkbatch_id AND oq.status_id = (SELECT id FROM t_srv_statuses WHERE status = 'COMPLETED')) 
			THEN
				UPDATE t_srv_bulkbatches SET status_id = (SELECT id FROM t_srv_statuses WHERE status = 'COMPLETED') WHERE id = btch_parent.bulkbatch_id ;
			END IF ;
		END LOOP ;
	END IF ;
		
	IF NEW.status_id = (SELECT id FROM t_srv_statuses WHERE status = 'CANCELLED') THEN
		UPDATE t_srv_bulkbatches SET status_id = (SELECT id FROM t_srv_statuses WHERE status = 'CANCELLED') 
		WHERE id IN (SELECT bulkbatch_id FROM t_srv_bulkbatch_x_ordersqueue WHERE order_id = NEW.id) AND status_id = (SELECT id FROM t_srv_statuses WHERE status = 'IN_PROGRESS');
	END IF ;
	
	RETURN NEW ;
	
END ;
$$ LANGUAGE plpgsql ;



/* --------------------------
	tf_insert_ordersqueue_state_transitions
*/ --------------------------
CREATE OR REPLACE FUNCTION tf_insert_ordersqueue_state_transitions() RETURNS TRIGGER AS $$

BEGIN
	INSERT INTO t_srv_ordersqueue_state_transitions (order_id, status_id) VALUES (NEW.id, NEW.status_id) ;
	
	RETURN NEW ;
	
END ;
$$ LANGUAGE plpgsql ;



/* --------------------------
	tf_populate_inventory_dwh
*/ --------------------------
CREATE OR REPLACE FUNCTION tf_populate_inventory_dwh() RETURNS TRIGGER AS $$

BEGIN
	INSERT INTO t_srv_inventory_dwh (
		id,
		uuid,
		name,
		contenttype,
		contentlength,
		origindate,
		creationdate,
		modificationdate,
		checksum,
		validitystart,
		validitystop,
		footprint,
		validity,
		filetype_id,
		satellite_id,
		sensor_id,
		mission,
		evictiondate,
		productiontype,
		attributes,
		trace_id)
	VALUES (
		NEW.id,
		NEW.uuid,
		NEW.name,
		NEW.contenttype,
		NEW.contentlength,
		NEW.origindate,
		NEW.creationdate,
		NEW.modificationdate,
		NEW.checksum,
		NEW.validitystart,
		NEW.validitystop,
		NEW.footprint,
		NEW.validity,
		NEW.filetype_id,
		NEW.satellite_id,
		NEW.sensor_id,
		NEW.mission,
		NEW.evictiondate,
		NEW.productiontype,
		NEW.attributes,
		NEW.trace_id 
	)
	ON CONFLICT (id) 
	DO 
	UPDATE SET
		uuid = NEW.uuid,
		name = NEW.name,
		contenttype = NEW.contenttype,
		contentlength = NEW.contentlength,
		origindate = NEW.origindate,
		creationdate = NEW.creationdate,
		modificationdate = NEW.modificationdate,
		checksum = NEW.checksum,
		validitystart = NEW.validitystart,
		validitystop = NEW.validitystop,
		footprint = NEW.footprint,
		validity = NEW.validity,
		filetype_id = NEW.filetype_id,
		satellite_id = NEW.satellite_id,
		sensor_id = NEW.sensor_id,
		mission = NEW.mission,
		evictiondate = NEW.evictiondate,
		productiontype = NEW.productiontype,
		attributes = NEW.attributes,
		trace_id = NEW.trace_id
	;
	
	RETURN NEW ;
	
END ;
$$ LANGUAGE plpgsql ;



/* --------------------------
	tf_srv_users_ins_bfr
*/ --------------------------
CREATE OR REPLACE FUNCTION tf_srv_users_ins_bfr() RETURNS TRIGGER AS $$

BEGIN
	IF NEW.highest_priority IS NULL THEN
		NEW.highest_priority := NEW.default_priority ;
	END IF ;
	
	RETURN NEW ;
	
END ;
$$ LANGUAGE plpgsql ;



/* --------------------------
	tf_srv_pending_inv_operations
*/ --------------------------
CREATE OR REPLACE FUNCTION tf_srv_pending_inv_operations() RETURNS TRIGGER AS $$

BEGIN
	IF (TG_OP = 'DELETE') THEN
		INSERT INTO t_srv_pendinginv (
			inv_id, uuid, name, contenttype, contentlength, origindate, creationdate, modificationdate, checksum, validitystart, validitystop, footprint, validity, filetype_id, satellite_id, sensor_id, mission, evictiondate, productiontype, attributes, trace_id, operation) 
			SELECT OLD.*, 'DELETE' ;
		
		IF OLD.satellite_id IS NOT NULL THEN
			INSERT INTO t_srv_catalogue_change_registry (sensingStartDate, satellite_id) 
				VALUES (OLD.validitystart::DATE, OLD.satellite_id) 
				ON CONFLICT DO NOTHING ;
		END IF;
		
		RETURN OLD;
		
	ELSIF (TG_OP = 'UPDATE') THEN
		INSERT INTO t_srv_pendinginv (
			inv_id, uuid, name, contenttype, contentlength, origindate, creationdate, modificationdate, checksum, validitystart, validitystop, footprint, validity, filetype_id, satellite_id, sensor_id, mission, evictiondate, productiontype, attributes, trace_id, operation) 
			SELECT NEW.*, 'UPDATE' ;
		
		IF NEW.satellite_id IS NOT NULL THEN
			INSERT INTO t_srv_catalogue_change_registry (sensingStartDate, satellite_id) 
				VALUES (NEW.validitystart::DATE, NEW.satellite_id) 
				ON CONFLICT DO NOTHING ;
		END IF;
		
		RETURN NEW;
		
	ELSE
		RETURN NULL;
		
	END IF;
	
END ;
$$ LANGUAGE plpgsql ;



/* --------------------------
	tf_srv_request_x_user_to_be_cancelled
*/ --------------------------
CREATE OR REPLACE FUNCTION tf_srv_request_x_user_to_be_cancelled() RETURNS TRIGGER AS $$

BEGIN
	IF OLD.to_be_cancelled = TRUE AND NEW.to_be_cancelled = FALSE THEN
		RAISE EXCEPTION 'tf_srv_request_x_user_to_be_cancelled: Cannot change to_be_cancelled flag from TRUE to FALSE';	
	END IF ;
	
	IF OLD.to_be_cancelled != TRUE AND NEW.to_be_cancelled = TRUE THEN
		CALL p_srv_del_order_link(NEW.order_id) ;	
	END IF ;
	
	RETURN NEW ;
	
END ;
$$ LANGUAGE plpgsql ;



/* --------------------------
	tf_srv_request_x_user_add_order_link
*/ --------------------------
CREATE OR REPLACE FUNCTION tf_srv_request_x_user_add_order_link() RETURNS TRIGGER AS $$

BEGIN
	CALL p_srv_add_order_link(NEW.order_id) ;
	
	RETURN NEW ;
	
END ;
$$ LANGUAGE plpgsql ;



/* --------------------------
	tf_srv_request_x_user_del_order_link
*/ --------------------------
CREATE OR REPLACE FUNCTION tf_srv_request_x_user_del_order_link() RETURNS TRIGGER AS $$

BEGIN
	CALL p_srv_del_order_link(OLD.order_id) ;
	
	RETURN OLD ;
	
END ;
$$ LANGUAGE plpgsql ;



/* --------------------------
	tf_srv_ordersqueue_upd_tot_metrics
*/ --------------------------
CREATE OR REPLACE FUNCTION tf_srv_ordersqueue_upd_tot_metrics() RETURNS TRIGGER AS $$

DECLARE
	inv_name VARCHAR ;
	inv_ftype VARCHAR ;
	inv_size BIGINT ;
	pt  VARCHAR ;
	psn VARCHAR ;
	psi VARCHAR ;

BEGIN
	SELECT i.name, f.filetype, i.contentlength
		INTO inv_name, inv_ftype, inv_size
		FROM t_srv_inventory i INNER JOIN t_filetypes f ON i.filetype_id = f.id_filetype
		WHERE id = NEW.product_id ;

	pt := SUBSTRING(inv_ftype FROM 4) ;
	psn := CASE
		WHEN SUBSTRING(inv_name FOR 2) = 'S1' THEN 'SENTINEL-1'
		WHEN SUBSTRING(inv_name FOR 2) = 'S2' THEN 'SENTINEL-2'
		WHEN SUBSTRING(inv_name FOR 2) = 'S3' THEN 'SENTINEL-3'
		END ;
	psi := SUBSTRING(inv_name FROM 3 FOR 1) ;

	INSERT INTO t_srv_totalmetrics (productType, platformShortName, platformSerialIdentifier, servicealias)
		VALUES (pt, psn, psi, NULL)
		ON CONFLICT (productType, platformShortName, platformSerialIdentifier, COALESCE(servicealias, 'DUMMY')) DO NOTHING ;

	IF (NEW.status_id = (SELECT id FROM t_srv_statuses WHERE status = 'COMPLETED')) THEN
		UPDATE t_srv_totalmetrics SET retrievecompletecount = retrievecompletecount + 1, retrievesize = retrievesize + inv_size
			WHERE productType = pt AND platformShortName = psn AND platformSerialIdentifier = psi AND servicealias IS NULL ;
	END IF ;

	IF (NEW.status_id = (SELECT id FROM t_srv_statuses WHERE status = 'ERROR')) THEN
		UPDATE t_srv_totalmetrics SET retrievefailcount = retrievefailcount + 1
			WHERE productType = pt AND platformShortName = psn AND platformSerialIdentifier = psi AND servicealias IS NULL ;
	END IF ;

	RETURN NEW ;

END ;
$$ LANGUAGE plpgsql ;



/* --------------------------
	tf_srv_update_lastmetrics
*/ --------------------------
CREATE OR REPLACE FUNCTION tf_srv_update_lastmetrics() RETURNS TRIGGER AS $$

BEGIN
	INSERT INTO t_srv_lastmetrics AS lm (name, tstamp, metrictype, counter, gauge)
		VALUES (NEW.name, NEW.tstamp, NEW.metrictype, NEW.counter, NEW.gauge)
		ON CONFLICT (name) DO UPDATE SET tstamp = NEW.tstamp, metrictype = NEW.metrictype, counter = NEW.counter, gauge = NEW.gauge
		WHERE lm.name = NEW.name AND lm.tstamp < NEW.tstamp ;	
	
	RETURN NEW ;
	
END ;
$$ LANGUAGE plpgsql ;








--------------------------------------------------------------------------------
------------------------------- TRIGGER CREATION -------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_srv_aip_content 
*/ ------------------------------
CREATE TRIGGER tr_srv_aip_content_ins_bfr BEFORE INSERT
	ON t_srv_aip_content
		FOR EACH ROW EXECUTE PROCEDURE tf_srv_aip_content_ins_bfr() ;

CREATE TRIGGER tr_srv_aip_content_upd_bfr BEFORE UPDATE
	ON t_srv_aip_content
		FOR EACH ROW EXECUTE PROCEDURE tf_srv_ineviction_check() ;

CREATE TRIGGER tr_srv_aip_content_del_aft AFTER DELETE
	ON t_srv_aip_content
		FOR EACH ROW EXECUTE PROCEDURE tf_srv_aip_content_del_aft() ;


/* ------------------------------
	t_srv_bulk_orders
*/ ------------------------------
CREATE TRIGGER tr_srv_bulk_order_upd_bfr BEFORE UPDATE
	ON t_srv_bulk_orders
		FOR EACH ROW EXECUTE PROCEDURE tf_srv_bulk_order_upd_bfr() ;

CREATE TRIGGER tr_srv_bulk_order_check_status BEFORE UPDATE OF status_id
	ON t_srv_bulk_orders
		FOR EACH ROW EXECUTE PROCEDURE tf_check_status_changes() ;

CREATE TRIGGER tr_srv_bulk_order_to_be_cancelled AFTER UPDATE OF tobecancelled
	ON t_srv_bulk_orders
		FOR EACH ROW 
		WHEN (OLD.tobecancelled = FALSE AND NEW.tobecancelled = TRUE)
		EXECUTE PROCEDURE tf_srv_bulk_order_to_be_cancelled() ;

CREATE TRIGGER tr_srv_bulk_order_in_error AFTER UPDATE OF status_id
	ON t_srv_bulk_orders
		FOR EACH ROW EXECUTE PROCEDURE tf_srv_bulk_order_in_error() ;


/* ------------------------------
	t_srv_bulkbatches
*/ ------------------------------
CREATE TRIGGER tr_srv_bulkbatches_upd_bfr BEFORE UPDATE
	ON t_srv_bulkbatches
		FOR EACH ROW EXECUTE PROCEDURE tf_srv_bulkbatches_upd_bfr() ;

CREATE TRIGGER tr_srv_bulkbatches_check_status BEFORE UPDATE OF status_id
	ON t_srv_bulkbatches
		FOR EACH ROW EXECUTE PROCEDURE tf_check_status_changes() ;

CREATE TRIGGER tr_srv_bulkbatches_upd_bulk AFTER UPDATE OF status_id
	ON t_srv_bulkbatches
		FOR EACH ROW EXECUTE PROCEDURE tf_srv_bulkbatches_upd_bulk() ;

CREATE TRIGGER tr_srv_bulkbatches_to_be_cancelled AFTER UPDATE OF tobecancelled
	ON t_srv_bulkbatches
		FOR EACH ROW 
		WHEN (OLD.tobecancelled = FALSE AND NEW.tobecancelled = TRUE)
		EXECUTE PROCEDURE tf_srv_bulkbatches_to_be_cancelled() ;


/* ------------------------------
	t_srv_bulkbatch_x_ordersqueue
*/ ------------------------------
CREATE TRIGGER tr_srv_bulkbatch_x_ordersqueue_ins_bfr BEFORE INSERT
	ON t_srv_bulkbatch_x_ordersqueue
		FOR EACH ROW EXECUTE PROCEDURE tf_srv_bulkbatch_x_ordersqueue_add_link() ;

CREATE TRIGGER tr_srv_bulkbatch_x_ordersqueue_upd_bfr BEFORE UPDATE
	ON t_srv_bulkbatch_x_ordersqueue
		FOR EACH ROW EXECUTE PROCEDURE tf_srv_bulkbatch_x_ordersqueue_prevent_update() ;

CREATE TRIGGER tr_srv_bulkbatch_x_ordersqueue_del_aft AFTER DELETE
	ON t_srv_bulkbatch_x_ordersqueue
		FOR EACH ROW EXECUTE PROCEDURE tf_srv_bulkbatch_x_ordersqueue_del_link() ;


/* ------------------------------
	t_srv_inventory
*/ ------------------------------
CREATE TRIGGER tr_srv_inventory_computeevictiondate BEFORE INSERT OR UPDATE
	ON t_srv_inventory
		FOR EACH ROW EXECUTE PROCEDURE tf_srv_computeevictiondate() ;

CREATE TRIGGER tr_srv_inventory_pending_operations AFTER DELETE OR UPDATE
	ON t_srv_inventory
		FOR EACH ROW EXECUTE PROCEDURE tf_srv_pending_inv_operations() ;

CREATE TRIGGER tr_srv_inventory_populate_inventory_dwh AFTER INSERT OR UPDATE
	ON t_srv_inventory
		FOR EACH ROW EXECUTE PROCEDURE tf_populate_inventory_dwh() ;


/* ------------------------------
	t_srv_ordersqueue
*/ ------------------------------
CREATE TRIGGER tr_srv_ordersqueue_ins_state_trans AFTER INSERT
ON t_srv_ordersqueue
		FOR EACH ROW EXECUTE PROCEDURE tf_insert_ordersqueue_state_transitions() ;

CREATE TRIGGER tr_srv_ordersqueue_check_status BEFORE UPDATE OF status_id
	ON t_srv_ordersqueue
		FOR EACH ROW EXECUTE PROCEDURE tf_check_status_changes() ;

CREATE TRIGGER tr_srv_ordersqueue_upd_status AFTER UPDATE OF status_id
	ON t_srv_ordersqueue
		FOR EACH ROW EXECUTE PROCEDURE tf_srv_ordersqueue_status_update() ;

CREATE TRIGGER tr_srv_ordersqueue_upd_tot_metrics AFTER UPDATE OF status_id
	ON t_srv_ordersqueue
		FOR EACH ROW EXECUTE PROCEDURE tf_srv_ordersqueue_upd_tot_metrics() ;

--CREATE TRIGGER tr_srv_ordersqueue_to_be_cancelled AFTER UPDATE OF tobecancelled
--	ON t_srv_ordersqueue
--		FOR EACH ROW 
--		WHEN (OLD.tobecancelled = FALSE AND NEW.tobecancelled = TRUE)
--		EXECUTE PROCEDURE tf_srv_ordersqueue_to_be_cancelled() ;

CREATE TRIGGER tr_srv_ordersqueue_upd_state_trans AFTER UPDATE
	ON t_srv_ordersqueue
		FOR EACH ROW EXECUTE PROCEDURE tf_insert_ordersqueue_state_transitions() ;

CREATE TRIGGER tr_srv_ordersqueue_upd_link_count AFTER UPDATE
	ON t_srv_ordersqueue
		FOR EACH ROW EXECUTE PROCEDURE tf_srv_ordersqueue_upd_link_count() ;


/* --------------------------
	t_srv_metrics
*/ --------------------------
CREATE TRIGGER tr_srv_update_lastmetrics AFTER INSERT OR UPDATE
	ON t_srv_metrics
		FOR EACH ROW EXECUTE PROCEDURE tf_srv_update_lastmetrics() ;


/* ------------------------------
	t_srv_request_x_user 
*/ ------------------------------
CREATE TRIGGER tr_srv_request_x_user_tobecancelled_bfr BEFORE UPDATE OF to_be_cancelled
	ON t_srv_request_x_user
		FOR EACH ROW EXECUTE PROCEDURE tf_srv_request_x_user_to_be_cancelled() ;

CREATE TRIGGER tr_srv_request_x_user_tobecancelled_aft AFTER UPDATE OF to_be_cancelled
	ON t_srv_request_x_user
		FOR EACH ROW EXECUTE PROCEDURE tf_srv_request_x_user_to_be_cancelled() ;

CREATE TRIGGER tr_srv_request_x_user_ins_aft AFTER INSERT
	ON t_srv_request_x_user
		FOR EACH ROW EXECUTE PROCEDURE tf_srv_request_x_user_add_order_link() ;

CREATE TRIGGER tr_srv_request_x_user_del_bfr AFTER DELETE
	ON t_srv_request_x_user
		FOR EACH ROW 
		WHEN (OLD.to_be_cancelled = FALSE)
		EXECUTE PROCEDURE tf_srv_request_x_user_del_order_link() ;


/* ------------------------------
	t_srv_users 
*/ ------------------------------
CREATE TRIGGER tr_srv_users_ins_bfr BEFORE INSERT
	ON t_srv_users
		FOR EACH ROW EXECUTE PROCEDURE tf_srv_users_ins_bfr() ;








--------------------------------------------------------------------------------
-------------------------------- VIEWS CREATION --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	v_satrelations
*/ ------------------------------
CREATE VIEW v_satrelations (
	satellite, 
	mission, 
	sensor)
AS SELECT 	
	sa.satellitename AS satellite, 
	sr.mission AS mission, 
	se.sensorname AS sensor
FROM 
	t_satrelations AS sr
	INNER JOIN t_satellites AS sa 
		ON sr.satelliteid = sa.satelliteid
	INNER JOIN t_sensors AS se 
		ON sr.sensorid = se.sensorid
ORDER BY 
	satellite, 
	mission, 
	sensor 
;








