
-- Version 12.05.00
-- POSTGRES








--------------------------------------------------------------------------------
-------------------------------- TABLES CREATION -------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_post_process_tt 
*/ ------------------------------
CREATE TABLE t_post_process_tt (
    id SERIAL NOT NULL,
	processor_id INTEGER NOT NULL,
	completed_tt_id INTEGER,
	failed_tt_id INTEGER,
	override_completed_exitcode BOOLEAN NOT NULL DEFAULT 'f'
) ;
ALTER TABLE t_post_process_tt
	ADD CONSTRAINT pk_t_post_process_tt
		PRIMARY KEY (id) ;

ALTER TABLE t_post_process_tt 
	ADD CONSTRAINT fk_post_process_tt0l
		FOREIGN KEY (processor_id)
			REFERENCES t_processors (id_processor)
				ON DELETE CASCADE ;

ALTER TABLE t_post_process_tt 
	ADD CONSTRAINT fk_post_process_tt02
		FOREIGN KEY (completed_tt_id)
			REFERENCES t_tasktables (id_tasktable)
				ON DELETE CASCADE ;

ALTER TABLE t_post_process_tt 
	ADD CONSTRAINT fk_post_process_tt03
		FOREIGN KEY (failed_tt_id)
			REFERENCES t_tasktables (id_tasktable)
				ON DELETE CASCADE ;

CREATE INDEX ix_t_post_process_tt01 ON t_post_process_tt USING BTREE (processor_id) ;
CREATE INDEX ix_t_post_process_tt02 ON t_post_process_tt USING BTREE (completed_tt_id) ;
CREATE INDEX ix_t_post_process_tt03 ON t_post_process_tt USING BTREE (failed_tt_id) ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'pds-12.05.00' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-12.05.00',
		'2017-10-04',
		3,
		2,
		'PDS_12-5-0',
		'Added table t_post_process_tt'
) ;
