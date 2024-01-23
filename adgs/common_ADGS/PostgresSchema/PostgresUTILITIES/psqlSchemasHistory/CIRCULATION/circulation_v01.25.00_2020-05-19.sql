
-- Version 1.25.0
-- POSTGRES








--------------------------------------------------------------------------------
-------------------------------- TABLES CREATION -------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_streamers
*/ ------------------------------
CREATE TABLE t_streamers (
    id BIGSERIAL NOT NULL,
	streamtype VARCHAR(16) NOT NULL
) ;

ALTER TABLE t_streamers 
	ADD CONSTRAINT pk_t_streamers
		PRIMARY KEY (id) ;

ALTER TABLE t_streamers 
	ADD CONSTRAINT uq_streamers01
		UNIQUE (streamtype) ;

INSERT INTO t_streamers (id, streamtype) VALUES (1, 'src_tar') ;
INSERT INTO t_streamers (id, streamtype) VALUES (2, 'dst_untar') ;








--------------------------------------------------------------------------------
-------------------------------- TABLES CHANGES --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_circulationsqueue
*/ ------------------------------
ALTER TABLE t_circulationsqueue ADD streamer_id INTEGER ;

ALTER TABLE t_circulationsqueue 
	ADD CONSTRAINT fk_circulationsqueue12
		FOREIGN KEY (streamer_id) REFERENCES t_streamers (id) 
			ON DELETE CASCADE ;

CREATE INDEX ix_t_circulationsqueue14 ON t_circulationsqueue USING BTREE (streamer_id) ;



/* ------------------------------
	t_circulation_remove_queue
*/ ------------------------------
ALTER TABLE t_circulation_remove_queue ADD host_id INTEGER ;

ALTER TABLE t_circulation_remove_queue 
	ADD CONSTRAINT fk_circulation_remove_queue01
		FOREIGN KEY (host_id) REFERENCES t_knownhosts (id) 
			ON DELETE CASCADE ;

CREATE INDEX ix_circulation_remove_queue01 ON t_circulation_remove_queue USING BTREE (host_id) ;








--------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ---------------------------------
--------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'circulation-1.25.0' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'circulation-1.25.0',
		'2020-05-19',
		6,
		20,
		'CIRCULATION_1-25-0',
		'Added t_streamers table'
) ;


