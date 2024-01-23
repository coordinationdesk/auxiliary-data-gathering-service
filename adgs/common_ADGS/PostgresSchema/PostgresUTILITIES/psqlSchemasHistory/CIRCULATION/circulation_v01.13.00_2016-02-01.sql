
-- Version 1.13.0
-- POSTGRES








--------------------------------------------------------------------------------
-------------------------------- TABLES CHANGES --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_datastoragequeue
*/ ------------------------------
ALTER TABLE t_datastoragequeue ADD check_date TIMESTAMP(6) DEFAULT now() NOT NULL;


/* ------------------------------
	t_dsqueue_x_cirqueue
*/ ------------------------------
ALTER TABLE t_dsqueue_x_cirqueue ADD fromurl VARCHAR(512) ;
ALTER TABLE t_dsqueue_x_cirqueue ADD circ_status_id INT2 ;

ALTER TABLE t_dsqueue_x_cirqueue 
	ADD CONSTRAINT fk_dsqueue_x_cirqueue04
		FOREIGN KEY (circ_status_id) REFERENCES t_stati (id) 
			ON DELETE CASCADE ;

CREATE INDEX ix_t_dsqueuexcirqueue04 ON t_dsqueue_x_cirqueue USING BTREE (circ_status_id) ;








--------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ---------------------------------
--------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'circulation-1.13.0' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'circulation-1.13.0',
		'2016-02-01',
		6,
		20,
		'CIRCULATION_1-13-0',
		'Added check_date field on t_datastoragequeue. Added fromurl and circ_status_id fields on t_dsqueue_x_cirqueue.'
) ;


