
-- Version 1.10.0
-- POSTGRES








--------------------------------------------------------------------------------
-------------------------------- TABLES CREATION --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_circulationsmedia
*/ ------------------------------
CREATE TABLE t_circulationsmedia (
	id SERIAL NOT NULL,
	media_tag VARCHAR(128) NOT NULL,
	status_id SMALLINT NOT NULL,
	tstamp TIMESTAMP(6) DEFAULT now() NOT NULL,
	mount_point VARCHAR(1024) NOT NULL,
	perc_copied_files INTEGER DEFAULT 0 NOT NULL
) ;
ALTER TABLE t_circulationsmedia 
	ADD CONSTRAINT pk_t_circulationsmedia
		PRIMARY KEY (id) ;

ALTER TABLE t_circulationsmedia 
	ADD CONSTRAINT uq_circulationsmedia01
		UNIQUE (media_tag) ;

ALTER TABLE t_circulationsmedia 
	ADD CONSTRAINT fk_circulationsmedia01
		FOREIGN KEY (status_id) REFERENCES t_stati (id) 
			ON DELETE CASCADE ;

CREATE INDEX ix_t_circulationsmedia01 ON t_circulationsmedia USING BTREE (status_id) ;








--------------------------------------------------------------------------------
-------------------------------- TABLES CHANGES --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_circulationsqueue
*/ ------------------------------
ALTER TABLE t_circulationsqueue ADD dest_protocol_id SMALLINT ;
ALTER TABLE t_circulationsqueue ADD media_circulation_id INTEGER ;
ALTER TABLE t_circulationsqueue ADD media_insertable BOOLEAN ;

UPDATE t_circulationsqueue cq SET dest_protocol_id = (
	SELECT id FROM t_transferprotocols WHERE protocol = ( 
	CASE POSITION(':' IN cq.tourl) 
		WHEN 0 THEN 'FILE'::VARCHAR
		ELSE UPPER(SUBSTRING(cq.tourl FROM 0 FOR (POSITION(':' IN tourl))))
	END)
) ;

ALTER TABLE t_circulationsqueue ALTER COLUMN dest_protocol_id SET NOT NULL ;

ALTER TABLE t_circulationsqueue 
	ADD CONSTRAINT fk_circulationsqueue08
		FOREIGN KEY (dest_protocol_id) REFERENCES t_transferprotocols (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_circulationsqueue 
	ADD CONSTRAINT fk_circulationsqueue09
		FOREIGN KEY (media_circulation_id) REFERENCES t_circulationsmedia (id) 
			ON DELETE CASCADE ;

CREATE INDEX ix_t_circulationsqueue09 ON t_circulationsqueue USING BTREE (dest_protocol_id) ;
CREATE INDEX ix_t_circulationsqueue10 ON t_circulationsqueue USING BTREE (media_circulation_id) ;








--------------------------------------------------------------------------------
-------------------------- TRIGGER FUNCTIONS CREATION --------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	tf_circulationsetprotocol
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_circulationsetprotocol() 
	RETURNS TRIGGER AS $$

DECLARE
	transf_prot VARCHAR ;

BEGIN
  	IF NEW.dest_protocol_id IS NULL THEN
		transf_prot := UPPER(SUBSTRING(NEW.tourl FROM 0 FOR (POSITION(':' IN NEW.tourl)))) ;
		IF transf_prot IS NULL OR transf_prot = '' THEN
			transf_prot := 'FS' ;
		END IF ;
		
		NEW.dest_protocol_id := (SELECT id FROM t_transferprotocols WHERE protocol = transf_prot) ;
	END IF ;
	
	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;








--------------------------------------------------------------------------------
------------------------------- TRIGGER CREATION -------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_circulationsqueue
*/ ------------------------------
CREATE TRIGGER tr_circulationsqueue_ins_bfr BEFORE INSERT 
	ON t_circulationsqueue 
		FOR EACH ROW EXECUTE PROCEDURE tf_circulationsetprotocol() ;








--------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ---------------------------------
--------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'circulation-1.10.0' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'circulation-1.10.0',
		'2015-07-23',
		6,
		20,
		'CIRCULATION_1-10-0',
		'Added table t_circulationsmedia and new fields on t_circulationsqueue.'
) ;


