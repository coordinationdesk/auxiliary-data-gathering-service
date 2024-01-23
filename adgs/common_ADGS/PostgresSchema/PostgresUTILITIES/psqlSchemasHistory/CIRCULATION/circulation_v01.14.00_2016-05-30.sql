
-- Version 1.14.0
-- POSTGRES








--------------------------------------------------------------------------------
-------------------------------- TABLES CREATION -------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_circ_callbacks
*/ ------------------------------
CREATE TABLE t_circ_callbacks (
	id SERIAL NOT NULL,
	circ_queue_id BIGINT NOT NULL,
	url VARCHAR(512) NOT NULL,
	external_id BIGINT NOT NULL
) ;
ALTER TABLE t_circ_callbacks 
	ADD CONSTRAINT pk_t_circ_callbacks
		PRIMARY KEY (id) ;

ALTER TABLE t_circ_callbacks 
	ADD CONSTRAINT fk_circ_callbacks01
		FOREIGN KEY (circ_queue_id) REFERENCES t_circulationsqueue (id) 
			ON DELETE CASCADE ;

CREATE INDEX ix_t_circ_callbacks01 ON t_circ_callbacks USING BTREE (circ_queue_id) ;
CREATE INDEX ix_t_circ_callbacks02 ON t_circ_callbacks USING BTREE (external_id) ;








--------------------------------------------------------------------------------
-------------------------------- TABLES CHANGES --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_circulationsqueue
*/ ------------------------------
ALTER TABLE t_circulationsqueue ALTER COLUMN error DROP NOT NULL ;
ALTER TABLE t_circulationsqueue ALTER COLUMN error DROP DEFAULT ;


/* ------------------------------
	t_circulationsqueuedwh
*/ ------------------------------
ALTER TABLE t_circulationsqueuedwh ALTER COLUMN error DROP NOT NULL ;
ALTER TABLE t_circulationsqueuedwh ALTER COLUMN error DROP DEFAULT ;


/* ------------------------------
	t_datastoragequeue
*/ ------------------------------
/* 
DELETE FROM t_datastoragequeue WHERE id IN (
	SELECT id FROM t_datastoragequeue q 
	INNER JOIN (
		SELECT md5, type_id, status_id
		FROM t_datastoragequeue 
		WHERE status_id != 3
		GROUP BY md5, type_id, status_id
		HAVING count(*) > 1) s
	ON (q.md5 = s.md5 OR q.md5 IS NULL) AND q.type_id = s.type_id AND q.status_id = s.status_id 
);
*/
CREATE UNIQUE INDEX uq_datastoragequeue01 
	ON t_datastoragequeue (COALESCE(md5, 'NULL'), type_id, status_id) 
		WHERE status_id != 3 ;


/* ------------------------------
	t_dsqueue_x_cirqueue
*/ ------------------------------
ALTER TABLE t_dsqueue_x_cirqueue ADD circ_status_mod_date TIMESTAMP(6) DEFAULT NOW() NOT NULL;
ALTER TABLE t_dsqueue_x_cirqueue ALTER COLUMN error DROP NOT NULL ;
ALTER TABLE t_dsqueue_x_cirqueue ALTER COLUMN error DROP DEFAULT ;








--------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ---------------------------------
--------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'circulation-1.14.0' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'circulation-1.14.0',
		'2016-05-30',
		6,
		20,
		'CIRCULATION_1-14-0',
		'Added new table t_circ_callbacks. Added new filed on t_dsqueue_x_cirqueue and new constrain on t_datastoragequeue'
) ;


