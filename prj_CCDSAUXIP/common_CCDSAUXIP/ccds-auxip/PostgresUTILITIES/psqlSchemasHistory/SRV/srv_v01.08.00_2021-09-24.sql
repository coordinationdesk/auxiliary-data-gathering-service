
-- SRV Version 1.8.0
-- POSTGRES





-------------------------------------------------------------------------------
------------------------------- TABLES CREATION -------------------------------
-------------------------------------------------------------------------------

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








-------------------------------------------------------------------------------
-------------------------------- TABLES CHANGES -------------------------------
-------------------------------------------------------------------------------

/* ------------------------------
	t_srv_inventory
*/ ------------------------------
CREATE INDEX ix_t_srv_inventory14 ON t_srv_inventory USING BTREE (mission) ;








-------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS --------------------------------
-------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'srv-01.08.00' ;

INSERT INTO t_schema (
	version,
	modificationdate,
	modificationtype,
	schematype,
	cvstag,
	description )
VALUES  (
	'srv-01.08.00',
	'2021-09-24',
	6,
	35,
	'SRV_1-8-0',
	'Added new table t_srv_inventory_x_sources and new index on t_srv_inventory (LTA-369, LTA-377)'
) ;



