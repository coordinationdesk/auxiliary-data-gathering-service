
-- SRV Version 1.6.0
-- POSTGRES





-------------------------------------------------------------------------------
------------------------------- TABLES CHANGES --------------------------------
-------------------------------------------------------------------------------

/* ------------------------------
	t_srv_bulk_orders
*/ ------------------------------
ALTER TABLE t_srv_bulk_orders ALTER COLUMN statusmessage DROP NOT NULL ;


/* ------------------------------
	t_srv_bulkbatches
*/ ------------------------------
ALTER TABLE t_srv_bulkbatches ALTER COLUMN statusmessage DROP NOT NULL ;


/* ------------------------------
	t_srv_totalmetrics
*/ ------------------------------
ALTER TABLE t_srv_totalmetrics RENAME COLUMN platrformSerialIdentifier TO platformSerialIdentifier ;
ALTER TABLE t_srv_totalmetrics ADD servicealias VARCHAR(64) ;
ALTER TABLE t_srv_totalmetrics DROP CONSTRAINT uq_srv_totalmetrics01 ;
CREATE UNIQUE INDEX uq_idx_totalmetrics01 
	ON t_srv_totalmetrics (producttype, platformshortname, platformserialidentifier, COALESCE(servicealias, 'DUMMY')) ;








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
		ON CONFLICT (productType, platformShortName, platformSerialIdentifier, servicealias) DO NOTHING ;
	
	IF (NEW.status_id = (SELECT id FROM t_srv_statuses WHERE status = 'COMPLETED')) THEN
		UPDATE t_srv_totalmetrics SET retrievecompletecount = retrievecompletecount + 1, retrievesize = retrievesize + inv_size
			WHERE productType = pt AND platformShortName = psn AND platformSerialIdentifier = psi ;
	END IF ;
	
	IF (NEW.status_id = (SELECT id FROM t_srv_statuses WHERE status = 'FAILED')) THEN
		UPDATE t_srv_totalmetrics SET retrievefailcount = retrievefailcount + 1
			WHERE productType = pt AND platformShortName = psn AND platformSerialIdentifier = psi ;
	END IF ;
		
	RETURN NEW ;
	
END ;
$$ LANGUAGE plpgsql ;








-------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS --------------------------------
-------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'srv-01.06.00' ;

INSERT INTO t_schema (
	version,
	modificationdate,
	modificationtype,
	schematype,
	cvstag,
	description )
VALUES  (
	'srv-01.06.00',
	'2021-03-30',
	6,
	35,
	'SRV_1-6-0',
	'Added servicealias field on t_srv_totalmetrics and changed unique index (LTA-258)'
) ;



