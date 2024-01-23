
-- Version 12.13.00
-- POSTGRES








--------------------------------------------------------------------------------
------------------------------ EXTENSION CREATION ------------------------------
--------------------------------------------------------------------------------

CREATE EXTENSION IF NOT EXISTS "uuid-ossp";








--------------------------------------------------------------------------------
-------------------------------- TABLES CHANGES --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_inventory
*/ ------------------------------
ALTER TABLE t_inventory ADD md5 VARCHAR(32) ;

ALTER TABLE t_inventory ALTER COLUMN unique_id SET DEFAULT uuid_generate_v1() ;

DROP TRIGGER tr_inventory_ins_bfr ON t_inventory ;








--------------------------------------------------------------------------------
-------------------------------- TABLES INSERT ---------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_inventory
*/ ------------------------------
INSERT INTO t_stati (id, status) VALUES (45, 'ONLINE') ;
INSERT INTO t_stati (id, status) VALUES (46, 'ONGOING') ;
INSERT INTO t_stati (id, status) VALUES (47, 'EVICTED') ;
INSERT INTO t_stati (id, status) VALUES (48, 'BOOKED') ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'pds-12.13.00' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-12.13.00',
		'2019-09-02',
		3,
		2,
		'PDS_12-13-0',
		'Added md5 field to t_inventory and added uuid v1 default value to unique_id field on t_inventory'
) ;


