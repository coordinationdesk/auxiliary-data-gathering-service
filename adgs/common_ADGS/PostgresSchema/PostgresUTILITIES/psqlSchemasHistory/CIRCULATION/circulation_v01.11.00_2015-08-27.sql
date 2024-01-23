
-- Version 1.11.0
-- POSTGRES








--------------------------------------------------------------------------------
-------------------------------- TABLES CREATION --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_circulationsmedia_insmode 
*/ ------------------------------
CREATE TABLE t_circulationsmedia_insmode (
	id SMALLINT NOT NULL,
	mode VARCHAR(24) NOT NULL
) ;
ALTER TABLE t_circulationsmedia_insmode 
	ADD CONSTRAINT pk_t_circulationsmedia_insmode
		PRIMARY KEY (id) ;

ALTER TABLE t_circulationsmedia_insmode 
	ADD CONSTRAINT uq_circulationsmedia_insmode01
		UNIQUE (mode) ;

INSERT INTO t_circulationsmedia_insmode (id, mode) VALUES (1, 'UNDEFINED') ;
INSERT INTO t_circulationsmedia_insmode (id, mode) VALUES (2, 'INSERTABLE') ;
INSERT INTO t_circulationsmedia_insmode (id, mode) VALUES (3, 'NOTINSERTABLE') ;








--------------------------------------------------------------------------------
-------------------------------- TABLES CHANGES --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_circulationsqueue
*/ ------------------------------
ALTER TABLE t_circulationsqueue DROP media_insertable ;
ALTER TABLE t_circulationsqueue ADD media_insertionmode_id SMALLINT DEFAULT 1 NOT NULL ;

ALTER TABLE t_circulationsqueue 
	ADD CONSTRAINT fk_circulationsqueue10
		FOREIGN KEY (media_insertionmode_id) REFERENCES t_circulationsmedia_insmode (id) 
			ON DELETE CASCADE ;

CREATE INDEX ix_t_circulationsqueue11 ON t_circulationsqueue USING BTREE (media_insertionmode_id) ;








--------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ---------------------------------
--------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'circulation-1.11.0' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'circulation-1.11.0',
		'2015-08-27',
		6,
		20,
		'CIRCULATION_1-11-0',
		'Added table t_circulationsmedia_insmode.Changed field media_insertable to media_insertionmode_id on t_circulationsqueue'
) ;


