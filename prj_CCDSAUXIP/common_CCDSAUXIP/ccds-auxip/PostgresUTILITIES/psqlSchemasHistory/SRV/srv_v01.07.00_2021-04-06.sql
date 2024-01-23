
-- SRV Version 1.7.0
-- POSTGRES





-------------------------------------------------------------------------------
------------------------------- TABLES CREATION -------------------------------
-------------------------------------------------------------------------------

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
------------------------------ TRIGGER FUNCTIONS ------------------------------
-------------------------------------------------------------------------------

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
	
	IF (NEW.status_id = (SELECT id FROM t_srv_statuses WHERE status = 'FAILED')) THEN
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








-------------------------------------------------------------------------------
------------------------------ TRIGGERS CREATION ------------------------------
-------------------------------------------------------------------------------

/* --------------------------
	t_srv_metrics
*/ --------------------------
CREATE TRIGGER tr_srv_update_lastmetrics AFTER INSERT OR UPDATE
	ON t_srv_metrics
		FOR EACH ROW EXECUTE PROCEDURE tf_srv_update_lastmetrics() ;








-------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS --------------------------------
-------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'srv-01.07.00' ;

INSERT INTO t_schema (
	version,
	modificationdate,
	modificationtype,
	schematype,
	cvstag,
	description )
VALUES  (
	'srv-01.07.00',
	'2021-04-06',
	6,
	35,
	'SRV_1-7-0',
	'Added new tables and trigger functions (LTA-259, LTA-266)'
) ;



