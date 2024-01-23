
-- Version 12.07.00
-- POSTGRES








--------------------------------------------------------------------------------
-------------------------------- INDEX CREATION --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_inv_relations
*/ ------------------------------
CREATE INDEX ix_t_inv_relations04 ON t_inv_relations USING BTREE (relation) ;





--------------------------------------------------------------------------------
-------------------------------- TABLES UPDATES --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_transferprotocols
*/ ------------------------------
UPDATE t_transferprotocols SET protocol = 'FILE' WHERE protocol = 'FS' ;





---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'pds-12.07.00' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-12.07.00',
		'2018-02-09',
		3,
		2,
		'PDS_12-7-0',
		'Added new index on t_inv_relations and changed FS to FILE into t_transferprotocols'
) ;


