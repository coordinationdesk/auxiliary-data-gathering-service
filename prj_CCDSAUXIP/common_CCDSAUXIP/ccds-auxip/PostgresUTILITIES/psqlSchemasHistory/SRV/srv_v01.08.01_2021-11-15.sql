
-- SRV Version 1.8.1
-- POSTGRES





-------------------------------------------------------------------------------
--------------------------- TRIGGER FUNCTIONS CHANGE --------------------------
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

	IF (NEW.status_id = (SELECT id FROM t_srv_statuses WHERE status = 'ERROR')) THEN
		UPDATE t_srv_totalmetrics SET retrievefailcount = retrievefailcount + 1
			WHERE productType = pt AND platformShortName = psn AND platformSerialIdentifier = psi AND servicealias IS NULL ;
	END IF ;

	RETURN NEW ;

END ;
$$ LANGUAGE plpgsql ;








-------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS --------------------------------
-------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'srv-01.08.01' ;

INSERT INTO t_schema (
	version,
	modificationdate,
	modificationtype,
	schematype,
	cvstag,
	description )
VALUES  (
	'srv-01.08.01',
	'2021-11-15',
	6,
	35,
	'SRV_1-8-1',
	'Fixed bug on tf_srv_ordersqueue_upd_tot_metrics trigger function'
) ;



