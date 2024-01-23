
-- Version 12.11.00
-- POSTGRES








--------------------------------------------------------------------------------
-------------------------------- TABLES CREATION -------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_distributionwaitingitems
*/ ------------------------------
CREATE TABLE t_distributionwaitingitems (
	id SERIAL NOT NULL,
	distributionitem_id INTEGER NOT NULL,
	package_id INTEGER NOT NULL,
	inventory_id BIGINT NOT NULL,
	CONSTRAINT pk_t_distributionwaitingitems PRIMARY KEY (id)
) ;

ALTER TABLE t_distributionwaitingitems
	ADD CONSTRAINT fk_distributionwaitingitems01
		FOREIGN KEY (distributionitem_id)
			REFERENCES t_distributionitems (id)
				ON DELETE CASCADE ;

ALTER TABLE t_distributionwaitingitems
	ADD CONSTRAINT fk_distributionwaitingitems02
		FOREIGN KEY (package_id)
			REFERENCES t_distributionqueue (packageid)
				ON DELETE CASCADE ;

ALTER TABLE t_distributionwaitingitems
	ADD CONSTRAINT fk_distributionwaitingitems03
		FOREIGN KEY (inventory_id)
			REFERENCES t_inventory (id_inv)
				ON DELETE CASCADE ;

CREATE INDEX ix_t_distributionwaitingitems1 ON t_distributionwaitingitems USING BTREE (distributionitem_id) ;
CREATE INDEX ix_t_distributionwaitingitems2 ON t_distributionwaitingitems USING BTREE (package_id) ;
CREATE INDEX ix_t_distributionwaitingitems3 ON t_distributionwaitingitems USING BTREE (inventory_id) ;








--------------------------------------------------------------------------------
-------------------------------- TABLES CHANGES --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_ordersgeneration
*/ ------------------------------
ALTER TABLE t_ordersgeneration DROP CONSTRAINT uq_ordersgeneration; 

CREATE UNIQUE index uq_ordersgeneration
	ON t_ordersgeneration (processorname, processorversion, ordertype, algorithm, filetype, COALESCE(fileclass, 'DUMMY')) ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'pds-12.11.00' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-12.11.00',
		'2019-05-09',
		3,
		2,
		'PDS_12-11-0',
		'Added table t_distributionwaitingitems'
) ;


