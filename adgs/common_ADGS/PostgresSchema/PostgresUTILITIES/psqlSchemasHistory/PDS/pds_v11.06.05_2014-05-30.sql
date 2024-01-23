
-- Version 11.6.5
-- POSTGRES








---------------------------------------------------------------------------------
-------------------------------- TABLES CREATION --------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_sor_requests
*/ ------------------------------
CREATE TABLE t_sor_requests (
	id_sor_request SERIAL NOT NULL,
	spacecraftid VARCHAR(24),
	requestid CHAR(8) NOT NULL,
	raiseddate TIMESTAMP(6) DEFAULT now() NOT NULL,
	originator VARCHAR(24),
	status VARCHAR(24) NOT NULL,
	statusupdated TIMESTAMP(6) DEFAULT now() NOT NULL,
	emailtitle VARCHAR(128) NOT NULL,
	emailbody TEXT,
	inventory_id BIGINT
) ;
ALTER TABLE t_sor_requests
	ADD CONSTRAINT pk_t_sor_requests
		PRIMARY KEY (id_sor_request) ;

ALTER TABLE t_sor_requests 
	ADD CONSTRAINT uq_sor_requests01
		UNIQUE (requestid) ;

ALTER TABLE t_sor_requests 
	ADD CONSTRAINT fk_sor_requests01
		FOREIGN KEY (inventory_id)
			REFERENCES t_inventory (id_inv)
				ON DELETE CASCADE ;

CREATE INDEX ix_t_sor_requests01 ON t_sor_requests USING BTREE (inventory_id) ;

COMMENT ON TABLE t_sor_requests IS 'Contains the list of SOR (Special Operation Request)';
COMMENT ON COLUMN t_sor_requests.id_sor_request IS 'Record identifier (Primary Key)';
COMMENT ON COLUMN t_sor_requests.spacecraftid IS 'Spacecraft Identifier eg: Sentinel-1A';
COMMENT ON COLUMN t_sor_requests.requestid IS 'The unique originator request identifier. It is composed by year and a 3 value progressive number. eg: 2012-001';
COMMENT ON COLUMN t_sor_requests.raiseddate IS 'When SOR was generated';
COMMENT ON COLUMN t_sor_requests.originator IS 'The name of the person raising the SOR';
COMMENT ON COLUMN t_sor_requests.status IS 'Status of SOR. Can be: Requested, Planned or Re-Planned, Executed or Abandoned';
COMMENT ON COLUMN t_sor_requests.statusupdated IS 'When the SOR status has been changed';
COMMENT ON COLUMN t_sor_requests.emailtitle IS 'The email subject. Is a naming convention composed by: <SpacecraftID> SOR <RequestID> <Title> - <Status>';
COMMENT ON COLUMN t_sor_requests.emailbody IS 'Contain the body of the email';
COMMENT ON COLUMN t_sor_requests.inventory_id IS 'Reference the file contained into t_inventory table';
COMMENT ON SEQUENCE t_sor_requests_id_sor_request_seq IS 'Used to generate the primary keys of the t_sor_requests table';








---------------------------------------------------------------------------------
------------------------------ TABLES MODIFICATION ------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_qualifiedflag
*/ ------------------------------
INSERT INTO t_qualifiedflag ("name") VALUES ('NO_SCHEMA_AVAILABLE') ;


/* ------------------------------
	t_stati
*/ ------------------------------
INSERT INTO t_stati (id, status) VALUES (33, 'REQUESTED') ;
INSERT INTO t_stati (id, status) VALUES (34, 'PLANNED') ;
INSERT INTO t_stati (id, status) VALUES (35, 'RE-PLANNED') ;
INSERT INTO t_stati (id, status) VALUES (36, 'EXECUTED') ;
INSERT INTO t_stati (id, status) VALUES (37, 'ABANDONED') ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------
DELETE FROM t_schema WHERE version = 'pds-11.6.5' ;
INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-11.6.5',
		'2014-05-30',
		3,
		2,
		'PDS_11-6-5',
		'Added table t_sor_requests. Added new stati and new qualifiedflag.'
) ;


