
-- Version 1.15.0
-- POSTGRES





-- #############################################################################
-- ##################                                         ##################
-- ##################                WARNING!!!               ##################
-- ##################   This patch can be installed only on   ##################
-- ##################       PDS 12.0.0 version or major       ##################
-- ##################                                         ##################
-- #############################################################################




--------------------------------------------------------------------------------
-------------------------------- TABLES CHANGES --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_circulationsqueue
*/ ------------------------------
ALTER TABLE t_circulationsqueue ADD localtransfermode_id SMALLINT DEFAULT 1 NOT NULL ;

ALTER TABLE t_circulationsqueue 
	ADD CONSTRAINT fk_circulationsqueue11
		FOREIGN KEY (localtransfermode_id) REFERENCES t_localtransfermodes (id) 
			ON DELETE CASCADE ;

CREATE INDEX ix_t_circulationsqueue13 ON t_circulationsqueue USING BTREE (localtransfermode_id) ;


/* ------------------------------
	t_dsqueue_x_cirqueue
*/ ------------------------------
ALTER TABLE t_dsqueue_x_cirqueue ADD localtransfermode_id SMALLINT DEFAULT 1 NOT NULL ;

ALTER TABLE t_dsqueue_x_cirqueue 
	ADD CONSTRAINT fk_dsqueue_x_cirqueue05
		FOREIGN KEY (localtransfermode_id) REFERENCES t_localtransfermodes (id) 
			ON DELETE CASCADE ;

CREATE INDEX ix_t_dsqueuexcirqueue05 ON t_dsqueue_x_cirqueue USING BTREE (localtransfermode_id) ;


/* ------------------------------
	t_circulationpolicies
*/ ------------------------------
ALTER TABLE t_circulationpolicies ADD localtransfermode_id SMALLINT ;

ALTER TABLE t_circulationpolicies 
	ADD CONSTRAINT fk_circulationpolicies06
		FOREIGN KEY (localtransfermode_id) REFERENCES t_localtransfermodes (id) 
			ON DELETE CASCADE ;

CREATE INDEX ix_t_circulationpolicies06 ON t_circulationpolicies USING BTREE (localtransfermode_id) ;

 






--------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ---------------------------------
--------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'circulation-1.14.2' ;
DELETE FROM t_schema WHERE version = 'circulation-1.15.0' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'circulation-1.15.0',
		'2016-07-15',
		6,
		20,
		'CIRCULATION_1-15-0',
		'Added field localtransfermode_id on tables t_circulationsqueue, t_circulationpolicies and t_dsqueue_x_cirqueue'
) ;


