
-- Version 1.5.4
-- POSTGRES








---------------------------------------------------------------------------------
--------------------------------- TABLES MODIFICATION ---------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_circulationsqueue
*/ ------------------------------
CREATE INDEX ix_t_circulationsqueue01 ON t_circulationsqueue USING BTREE (status_id) ;
CREATE INDEX ix_t_circulationsqueue02 ON t_circulationsqueue USING BTREE (circulationmode_id) ;
CREATE INDEX ix_t_circulationsqueue03 ON t_circulationsqueue USING BTREE (relay_host_id) ;
CREATE INDEX ix_t_circulationsqueue04 ON t_circulationsqueue USING BTREE (host_id) ;
CREATE INDEX ix_t_circulationsqueue05 ON t_circulationsqueue USING BTREE (circulationpolicy_id) ;
CREATE INDEX ix_t_circulationsqueue06 ON t_circulationsqueue USING BTREE (dest_host_id) ;

/* ------------------------------
	t_circulationsqueuedwh
*/ ------------------------------
CREATE INDEX ix_t_circulationsqueuedwh01 ON t_circulationsqueuedwh USING BTREE (status_id) ;
CREATE INDEX ix_t_circulationsqueuedwh02 ON t_circulationsqueuedwh USING BTREE (circulationmode_id) ;

/* ------------------------------
	t_circulationstats
*/ ------------------------------
CREATE INDEX ix_t_circulationstats01 ON t_circulationstats USING BTREE (circulationsqueue_id) ;
CREATE INDEX ix_t_circulationstats02 ON t_circulationstats USING BTREE (from_host_id) ;
CREATE INDEX ix_t_circulationstats03 ON t_circulationstats USING BTREE (to_host_id) ;

/* ------------------------------
	t_datastoragequeue
*/ ------------------------------
CREATE INDEX ix_t_datastoragequeue01 ON t_datastoragequeue USING BTREE (status_id) ;
CREATE INDEX ix_t_datastoragequeue02 ON t_datastoragequeue USING BTREE (type_id) ;

/* ------------------------------
	t_dsqueue_x_cirqueue
*/ ------------------------------
ALTER TABLE t_dsqueue_x_cirqueue ADD parent VARCHAR(128) ;








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
		'circulation-1.5.4',
		'2012-06-22',
		6,
		20,
		'CIRCULATION_1-5-4',
		'Added indexes on all queue tables and new fiwld on t_dsqueue_x_cirqueue.'
) ;
