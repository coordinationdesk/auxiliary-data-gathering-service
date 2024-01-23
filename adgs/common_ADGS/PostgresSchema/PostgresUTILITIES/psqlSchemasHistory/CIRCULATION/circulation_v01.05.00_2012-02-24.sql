
-- Version 1.5.0
-- POSTGRES





---------------------------------------------------------------------------------
-------------------------------- TABLES CREATION --------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_circulationsqueuedwh
*/ ------------------------------
CREATE TABLE t_circulationsqueuedwh (
    id BIGSERIAL NOT NULL,
    startdate TIMESTAMP(6),
	enddate TIMESTAMP(6),
	priority INTEGER DEFAULT 0 NOT NULL,
	remark VARCHAR(512),
	status_id INT2 DEFAULT 16 NOT NULL,
	creationdate TIMESTAMP(6) DEFAULT NOW(),
	modificationdate TIMESTAMP(6) DEFAULT NOW(),
	fromurl VARCHAR(512) NOT NULL,
	tourl VARCHAR(512) NOT NULL,
	relay_host_id INTEGER,
	relay_activity_id BIGINT,
	retries INT2 DEFAULT 0 NOT NULL,
	removesource BOOLEAN DEFAULT 'f' NOT NULL,
	circulationmode_id SMALLINT NOT NULL,
	host_id INTEGER,
	circulationpolicy_id INTEGER,
	dest_host_id INTEGER
) ;
ALTER TABLE t_circulationsqueuedwh 
	ADD CONSTRAINT pk_t_circulationsqueuedwh
		PRIMARY KEY (id) ;

ALTER TABLE t_circulationsqueuedwh 
	ADD CONSTRAINT fk_circulationsqueuedwh01
		FOREIGN KEY (status_id) REFERENCES t_stati (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_circulationsqueuedwh 
	ADD CONSTRAINT fk_circulationsqueuedwh02
		FOREIGN KEY (circulationmode_id) REFERENCES t_circulationmodes (id) 
			ON DELETE CASCADE ;
			
ALTER TABLE t_circulationsqueuedwh OWNER TO pdsuser ;








---------------------------------------------------------------------------------
------------------------------ TABLES MODIFICATION ------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_circulationsqueue
*/ ------------------------------
ALTER TABLE t_circulationsqueue DROP COLUMN synchronized ;








---------------------------------------------------------------------------------
--------------------------------- TABLES UPDATE ---------------------------------
---------------------------------------------------------------------------------
INSERT INTO t_circulationtempnames (id, tempname) VALUES (1, 'prefix_dot') ;
INSERT INTO t_circulationtempnames (id, tempname) VALUES (2, 'suffix_lock') ;
INSERT INTO t_circulationtempnames (id, tempname) VALUES (3, 'tmp_dir') ;








---------------------------------------------------------------------------------
------------------------------ TRIGGERS MODIFICATION ------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_circulationsqueue
*/ ------------------------------
CREATE TRIGGER tr_circulationsqueue_insert2 AFTER INSERT 
	ON t_circulationsqueue 
		EXECUTE PROCEDURE tf_inserteventtableitem('t_circulationsqueue','INS') ;

DROP TRIGGER tr_circulationsqueue_insert ON t_circulationsqueue ;








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
		'circulation-1.5.0',
		'2012-02-24',
		6,
		20,
		'CIRCULATION_1-5-0',
		'Added table t_circulationsqueuedwh. Removed synchronized field on t_circulationsqueue. fixed bug on tr_circulationsqueue_insert2 trigger.'
) ;


