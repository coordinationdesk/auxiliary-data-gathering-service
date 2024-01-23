
-- Version 11.6.1
-- POSTGRES








---------------------------------------------------------------------------------
------------------------------ TABLES MODIFICATION ------------------------------
---------------------------------------------------------------------------------

/* --------------------------
	t_inventory
*/ --------------------------
CREATE INDEX ix_t_inventory16 ON t_inventory USING BTREE (inventorydate) ;

/* --------------------------
	t_users
*/ --------------------------
CREATE INDEX ix_t_users01 ON t_users USING BTREE (usertype) ;
CREATE INDEX ix_t_users02 ON t_users USING BTREE (mode) ;

/* --------------------------
	t_waitingorders
*/ --------------------------
CREATE INDEX ix_t_waitingorders01 ON t_waitingorders USING BTREE (orderid) ;
CREATE INDEX ix_t_waitingorders02 ON t_waitingorders USING BTREE (filetype) ;
CREATE INDEX ix_t_waitingorders03 ON t_waitingorders USING BTREE (triggertype) ;







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
		'pds-11.6.1',
		'2014-01-21',
		3,
		2,
		'PDS_11-6-1',
		'Added indexes on t_inventory, t_users and t_waitingorders.'
) ;


