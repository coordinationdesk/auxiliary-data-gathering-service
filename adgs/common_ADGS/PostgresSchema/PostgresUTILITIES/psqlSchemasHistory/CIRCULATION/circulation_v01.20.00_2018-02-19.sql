
-- Version 1.20.0
-- POSTGRES





--------------------------------------------------------------------------------
-------------------------------- TABLES CHANGES --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_flowtags
*/ ------------------------------
CREATE TABLE t_flowtags (
	id serial NOT NULL, 
	flowtagname VARCHAR(32) NOT NULL
);
ALTER TABLE t_flowtags ADD CONSTRAINT pk_t_flowtags PRIMARY KEY (id);
ALTER TABLE t_flowtags ADD CONSTRAINT uq_t_flowtags UNIQUE (flowtagname);

INSERT INTO t_flowtags (flowtagname) VALUES ('DEFAULT');



/* ------------------------------
	t_datastoragequeue
*/ ------------------------------
ALTER TABLE t_datastoragequeue ADD COLUMN flowtag_id INT DEFAULT 1 NOT NULL;
ALTER TABLE t_datastoragequeue ADD CONSTRAINT fk_t_datastoragequeue FOREIGN KEY (flowtag_id) REFERENCES t_flowtags (id) on DELETE CASCADE;
CREATE INDEX ix_t_datastoragequeue04 ON t_datastoragequeue USING BTREE (flowtag_id) ;






--------------------------------------------------------------------------------
-------------------------------- TABLES UPDATES --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_transferprotocols
*/ ------------------------------
UPDATE t_transferprotocols SET protocol = 'FILE' WHERE protocol = 'FS' ;






--------------------------------------------------------------------------------
-------------------------------- TRIGGER CHANGES -------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	tf_circulationsetprotocol
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_circulationsetprotocol() 
	RETURNS TRIGGER AS $$

DECLARE
	transf_prot VARCHAR ;
	errMsg VARCHAR ;

BEGIN
  	IF NEW.dest_protocol_id IS NULL THEN
		
		transf_prot := UPPER(SUBSTRING(NEW.tourl FROM 0 FOR (POSITION(':' IN NEW.tourl)))) ;
		
		IF transf_prot IS NULL OR transf_prot = '' THEN
			transf_prot := 'FILE' ;
		END IF ;
		
		SELECT id INTO NEW.dest_protocol_id FROM t_transferprotocols WHERE protocol = transf_prot ;
		IF NEW.dest_protocol_id IS NULL THEN
			errMsg := 'tf_circulationsetprotocol(): dest_protocol_id "' || transf_prot || '" not found' ;
			RAISE EXCEPTION '%', errMsg ;
		END IF ;
	END IF ;
	
	RETURN NEW ;	
END ;
$$ LANGUAGE plpgsql ;






--------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ---------------------------------
--------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'circulation-1.20.0' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'circulation-1.20.0',
		'2018-02-19',
		6,
		20,
		'CIRCULATION_1-20-0',
		'Added new table t_flowtags and new field flowtag_id on t_datastoragequeue. Fixed tranfer protocol on tf_circulationsetprotocol'
) ;

