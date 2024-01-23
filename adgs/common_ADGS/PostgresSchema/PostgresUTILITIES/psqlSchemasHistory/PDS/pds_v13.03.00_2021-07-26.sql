
-- Version 13.03.00
-- POSTGRES








--------------------------------------------------------------------------------
-------------------------------- TABLES CREATION -------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_objsminvrelation
*/ ------------------------------
CREATE TABLE t_objsminvrelation (
	id INTEGER NOT NULL,
	http_path VARCHAR(512) NOT NULL,
	s3_path VARCHAR(512) NOT NULL,
	signature VARCHAR(64)
);
ALTER TABLE t_objsminvrelation
	ADD CONSTRAINT pk_t_objsminvrelation
		PRIMARY KEY (id) ;

ALTER TABLE t_objsminvrelation
	ADD CONSTRAINT fk_objsminvrelation01
		FOREIGN KEY (id)
			REFERENCES t_inventorysm (id)
				ON DELETE CASCADE;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'pds-13.03.00' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-13.03.00',
		'2021-07-26',
		3,
		2,
		'PDS_13-3-0',
		'Added new table t_objsminvrelation'
) ;


