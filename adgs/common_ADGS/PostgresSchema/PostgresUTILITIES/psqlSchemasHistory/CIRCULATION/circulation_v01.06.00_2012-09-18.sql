
-- Version 1.6.0
-- POSTGRES








---------------------------------------------------------------------------------
-------------------------------- TABLES CREATION --------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_circulationactivehosts
*/ ------------------------------
CREATE TABLE t_circulationactivehosts (
	host_id INTEGER NOT NULL,
	tstamp TIMESTAMP(6) DEFAULT NOW() NOT NULL
) ;

ALTER TABLE t_circulationactivehosts 
	ADD CONSTRAINT pk_t_circulationactivehosts
		PRIMARY KEY (host_id) ;

ALTER TABLE t_circulationactivehosts 
	ADD CONSTRAINT fk_circulationactivehosts01
		FOREIGN KEY (host_id) REFERENCES t_knownhosts (id) 
			ON DELETE CASCADE ;








---------------------------------------------------------------------------------
--------------------------------- TABLES MODIFICATION ---------------------------------
---------------------------------------------------------------------------------
ALTER TABLE t_circulationsqueuedwh ADD error VARCHAR(512) DEFAULT '' NOT NULL;








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
		'circulation-1.6.0',
		'2012-09-18',
		6,
		20,
		'CIRCULATION_1-6-0',
		'Added new table t_circulationactivehosts and added new field on t_circulationsqueuedwh'
) ;
