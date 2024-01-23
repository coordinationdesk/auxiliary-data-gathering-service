
-- Version 10.4.0
-- POSTGRES








---------------------------------------------------------------------------------
-------------------------------- TABLES CREATION --------------------------------
---------------------------------------------------------------------------------

/* --------------------------
	t_inv_relation_types
*/ --------------------------
CREATE TABLE t_inv_relation_types (
	id SMALLINT NOT NULL,
	relation_type VARCHAR(24) NOT NULL
) ;

ALTER TABLE t_inv_relation_types 
	ADD CONSTRAINT pk_t_inv_relation_types
		PRIMARY KEY (id) ;

ALTER TABLE t_inv_relation_types 
	ADD CONSTRAINT uq_inv_relation_types01
		UNIQUE (relation_type) ;

INSERT INTO t_inv_relation_types (id, relation_type) VALUES (1, 'PARENT_TO_CHILD') ;
INSERT INTO t_inv_relation_types (id, relation_type) VALUES (2, 'BROTHER_TO_BROTHER') ;


/* --------------------------
	t_inv_relations
*/ --------------------------
CREATE TABLE t_inv_relations (
	id BIGSERIAL NOT NULL,
	inv_id1 BIGINT NOT NULL,
	inv_id2 BIGINT NOT NULL,
	rel_type_id SMALLINT DEFAULT 1 NOT NULL,
	relation VARCHAR(256)
) ;

ALTER TABLE t_inv_relations 
	ADD CONSTRAINT pk_t_inv_relations
		PRIMARY KEY (id) ;

ALTER TABLE t_inv_relations 
	ADD CONSTRAINT uq_inv_relations01
		UNIQUE (inv_id1, inv_id2) ;

CREATE INDEX ix_t_inv_relations01 ON t_inv_relations USING btree (rel_type_id) ;

ALTER TABLE t_inv_relations 
	ADD CONSTRAINT fk_inv_relations01 
		FOREIGN KEY (inv_id1) 
			REFERENCES t_inventory (id_inv) 
				ON DELETE CASCADE ;

ALTER TABLE t_inv_relations 
	ADD CONSTRAINT fk_inv_relations02 
		FOREIGN KEY (inv_id2) 
			REFERENCES t_inventory (id_inv) 
				ON DELETE CASCADE ;

ALTER TABLE t_inv_relations 
	ADD CONSTRAINT fk_inv_relations03 
		FOREIGN KEY (rel_type_id) 
			REFERENCES t_inv_relation_types (id) 
				ON DELETE CASCADE ;




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
		'pds-10.4.0',
		'2012-07-31',
		3,
		2,
		'PDS_10-4-0',
		'Added new tables t_inv_relation_types and t_inv_relations.'
) ;
