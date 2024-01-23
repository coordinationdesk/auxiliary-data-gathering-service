
-- Version 13.04.00
-- POSTGRES








--------------------------------------------------------------------------------
-------------------------------- TABLES CHANGES --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_repositories
*/ ------------------------------
ALTER TABLE t_repositories ADD port SMALLINT ;
ALTER TABLE t_repositories ADD attributes JSONB ;


/* ------------------------------
	t_receptionrules
*/ ------------------------------
ALTER TABLE t_receptionrules ADD timewindow_sec INTEGER ;


/* ------------------------------
	t_receptionruleshist
*/ ------------------------------
ALTER TABLE t_receptionruleshist ADD filecreationtstamp TIMESTAMP(6) ;
ALTER TABLE t_receptionruleshist ADD remoteuuid VARCHAR(160) ;
ALTER TABLE t_receptionruleshist ADD evictiondate TIMESTAMP(6) ;

CREATE INDEX ix_t_receptionruleshist04 ON t_receptionruleshist USING BTREE (filecreationtstamp) ;


/* ------------------------------
	t_receptionruleslock
*/ ------------------------------
ALTER TABLE t_receptionruleslock ADD processidentifier VARCHAR(128) ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'pds-13.04.00' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-13.04.00',
		'2021-08-04',
		3,
		2,
		'PDS_13-4-0',
		'Added new fields for php importer on t_repositories, t_receptionrules, t_receptionruleshist and t_receptionruleslock tables'
) ;


