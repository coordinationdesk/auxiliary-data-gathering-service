
-- Version 12.14.00
-- POSTGRES








--------------------------------------------------------------------------------
-------------------------------- TABLES CHANGES --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_inventory
*/ ------------------------------
ALTER TABLE t_inventory ADD attributes JSONB ;
CREATE INDEX ix_t_inventory_jsonb ON t_inventory USING GIN (attributes);


/* ------------------------------
	t_acsisminvrelation
*/ ------------------------------
ALTER TABLE t_acsisminvrelation ADD medialabel VARCHAR(128);
CREATE INDEX ix_t_acsisminvrelation01 ON t_acsisminvrelation USING BTREE (path);
CREATE INDEX ix_t_acsisminvrelation02 ON t_acsisminvrelation USING BTREE (medialabel);








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'pds-12.14.00' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-12.14.00',
		'2019-12-04',
		3,
		2,
		'PDS_12-14-0',
		'Added attributes field to t_inventory'
) ;


