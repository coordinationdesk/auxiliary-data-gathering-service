
-- Version 1.12.0
-- POSTGRES








--------------------------------------------------------------------------------
-------------------------------- TABLES CHANGES --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_circulationpolicies
*/ ------------------------------
ALTER TABLE t_circulationpolicies ADD agent_node_id INTEGER ;

ALTER TABLE t_circulationpolicies 
	ADD CONSTRAINT fk_circulationpolicies05
		FOREIGN KEY (agent_node_id) REFERENCES t_knownhosts (id) 
			ON DELETE CASCADE ;

CREATE INDEX ix_t_circulationpolicies05 ON t_circulationpolicies USING BTREE (agent_node_id) ;


/* ------------------------------
	t_circulationsqueue
*/ ------------------------------
CREATE INDEX ix_t_circulationsqueue12 ON t_circulationsqueue USING BTREE (enddate) ;


/* ------------------------------
	t_circulationsqueuedwh
*/ ------------------------------
ALTER TABLE t_circulationsqueuedwh ADD circulation_id BIGINT DEFAULT 0 NOT NULL ;
ALTER TABLE t_circulationsqueuedwh ADD source_db_host VARCHAR(256) DEFAULT 'UNKNOWN' NOT NULL ;

ALTER TABLE t_circulationsqueuedwh DROP CONSTRAINT fk_circulationsqueuedwh01 ;
ALTER TABLE t_circulationsqueuedwh DROP CONSTRAINT fk_circulationsqueuedwh02 ;

CREATE INDEX ix_t_circulationsqueuedwh03 ON t_circulationsqueuedwh USING BTREE (enddate) ;
CREATE INDEX ix_t_circulationsqueuedwh04 ON t_circulationsqueuedwh USING BTREE (creationdate) ;
CREATE INDEX ix_t_circulationsqueuedwh05 ON t_circulationsqueuedwh USING BTREE (circulation_id) ;
CREATE INDEX ix_t_circulationsqueuedwh06 ON t_circulationsqueuedwh USING BTREE (source_db_host) ;








--------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ---------------------------------
--------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'circulation-1.12.0' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'circulation-1.12.0',
		'2015-10-02',
		6,
		20,
		'CIRCULATION_1-12-0',
		'Added fields circulation_id and source_host on table t_circulationsqueuedwh and field agent_node_id on t_circulationpolicies.'
) ;


