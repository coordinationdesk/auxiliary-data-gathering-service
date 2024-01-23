
-- SRV Version 1.5.0
-- POSTGRES





-------------------------------------------------------------------------------
------------------------------- TABLES CREATION -------------------------------
-------------------------------------------------------------------------------

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
	platrformSerialIdentifier VARCHAR(32) NOT NULL,
	archivesize BIGINT NOT NULL DEFAULT 0,
	archivecount BIGINT NOT NULL DEFAULT 0,
	retrievesize BIGINT NOT NULL DEFAULT 0,
	retrievecompletecount BIGINT NOT NULL DEFAULT 0,
	retrievefailcount BIGINT NOT NULL DEFAULT 0,
	downloadsize BIGINT NOT NULL DEFAULT 0,
	downloadcompletecount BIGINT NOT NULL DEFAULT 0,
	downloadfailcount BIGINT NOT NULL DEFAULT 0
);
ALTER TABLE t_srv_totalmetrics 
	ADD CONSTRAINT pk_t_srv_totalmetrics
		PRIMARY KEY (id) ;

ALTER TABLE t_srv_totalmetrics
	ADD CONSTRAINT uq_srv_totalmetrics01
		UNIQUE (productType, platformShortName, platrformSerialIdentifier) ;

CREATE INDEX ix_t_srv_totalmetrics01 ON t_srv_totalmetrics USING BTREE (productType) ;
CREATE INDEX ix_t_srv_totalmetrics02 ON t_srv_totalmetrics USING BTREE (platformShortName) ;
CREATE INDEX ix_t_srv_totalmetrics03 ON t_srv_totalmetrics USING BTREE (platrformSerialIdentifier) ;








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
	
	INSERT INTO t_srv_totalmetrics (productType, platformShortName, platrformSerialIdentifier) 
		VALUES (pt, psn, psi)
		ON CONFLICT (productType, platformShortName, platrformSerialIdentifier) DO NOTHING ;
	
	IF (NEW.status_id = (SELECT id FROM t_srv_statuses WHERE status = 'COMPLETED')) THEN
		UPDATE t_srv_totalmetrics SET retrievecompletecount = retrievecompletecount + 1, retrievesize = retrievesize + inv_size
			WHERE productType = pt AND platformShortName = psn AND platrformSerialIdentifier = psi ;
	END IF ;
	
	IF (NEW.status_id = (SELECT id FROM t_srv_statuses WHERE status = 'FAILED')) THEN
		UPDATE t_srv_totalmetrics SET retrievefailcount = retrievefailcount + 1
			WHERE productType = pt AND platformShortName = psn AND platrformSerialIdentifier = psi ;
	END IF ;
		
	RETURN NEW ;
	
END ;
$$ LANGUAGE plpgsql ;








-------------------------------------------------------------------------------
------------------------------ TRIGGERS CREATION ------------------------------
-------------------------------------------------------------------------------

/* ------------------------------
	t_srv_ordersqueue
*/ ------------------------------
CREATE TRIGGER tr_srv_ordersqueue_upd_tot_metrics AFTER UPDATE OF status_id
	ON t_srv_ordersqueue
		FOR EACH ROW EXECUTE PROCEDURE tf_srv_ordersqueue_upd_tot_metrics() ;








-------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS --------------------------------
-------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'srv-01.05.00' ;

INSERT INTO t_schema (
	version,
	modificationdate,
	modificationtype,
	schematype,
	cvstag,
	description )
VALUES  (
	'srv-01.05.00',
	'2021-03-11',
	6,
	35,
	'SRV_1-5-0',
	'Added tables t_applicationstatus, t_srv_metrics and t_srv_totalmetrics. Added trigger function tf_srv_ordersqueue_upd_tot_metrics. (LTA-234)'
) ;



