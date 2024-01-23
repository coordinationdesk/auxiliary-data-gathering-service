
-- Version 11.1.2
-- POSTGRES








---------------------------------------------------------------------------------
----------------------------- TABLES MODIFICATIONS ------------------------------
---------------------------------------------------------------------------------

/* --------------------------
	t_inventory
*/ --------------------------
CREATE INDEX ix_t_inventory14 ON t_inventory USING BTREE (validitystart) ;
CREATE INDEX ix_t_inventory15 ON t_inventory USING BTREE (validitystop) ;








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
		'pds-11.1.2',
		'2013-09-09',
		3,
		2,
		'PDS_11-1-2',
		'Added indexes on validitystart and validitystop.'
) ;


