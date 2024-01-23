
-- Version 1.0.0
-- POSTGRES





-- ########################################################################### --
-- ##################                                        ################# --
-- ##################                WARNING!!!              ################# --
-- ##################  This schema can be installed only on  ################# --
-- ##################      PDS 10.0.0 version or major       ################# --
-- ##################                                        ################# --
-- ########################################################################### --





---------------------------------------------------------------------------------
-------------------------------- TABLES CREATION --------------------------------
---------------------------------------------------------------------------------

------------------------------
-- t_knownhosts
------------------------------
CREATE TABLE t_knownhosts (
	id SERIAL NOT NULL,
	ip_address VARCHAR(16) NOT NULL,
	host_name VARCHAR(64)
) ;

ALTER TABLE t_knownhosts 
	ADD CONSTRAINT pk_t_knownhosts
		PRIMARY KEY (id) ;

ALTER TABLE t_knownhosts 
	ADD CONSTRAINT uq_knownhosts01
	UNIQUE (host_name) ;
		


------------------------------
-- t_securityzones
------------------------------
CREATE TABLE t_securityzones (
	id SERIAL NOT NULL,
	ip_cidr VARCHAR(32) NOT NULL,
	level INTEGER NOT NULL,
	securityzone VARCHAR(32)
) ;

ALTER TABLE t_securityzones 
	ADD CONSTRAINT pk_t_securityzones
		PRIMARY KEY (id) ;

ALTER TABLE t_securityzones 
	ADD CONSTRAINT uq_securityzones01
		UNIQUE (securityzone) ;



------------------------------
-- t_securityzonerelays
------------------------------
CREATE TABLE t_securityzonerelays (
	id SERIAL NOT NULL,
	host_id INTEGER NOT NULL,
	seczone_id INTEGER NOT NULL
) ;

ALTER TABLE t_securityzonerelays 
	ADD CONSTRAINT pk_t_securityzonerelays
		PRIMARY KEY (id) ;

ALTER TABLE t_securityzonerelays 
	ADD CONSTRAINT uq_securityzonerelays01
		UNIQUE (host_id, seczone_id) ;

ALTER TABLE t_securityzonerelays 
	ADD CONSTRAINT fk_securityzonerelays01
		FOREIGN KEY (host_id) REFERENCES t_knownhosts (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_securityzonerelays 
	ADD CONSTRAINT fk_securityzonerelays02
		FOREIGN KEY (seczone_id) REFERENCES t_securityzones (id) 
			ON DELETE CASCADE ;



------------------------------
-- t_circulationmodes
------------------------------
CREATE TABLE t_circulationmodes (
	id SMALLINT NOT NULL,
	circulationmode VARCHAR(16) NOT NULL
) ;

ALTER TABLE t_circulationmodes 
	ADD CONSTRAINT pk_t_circulationmodes
		PRIMARY KEY (id) ;
		
ALTER TABLE t_circulationmodes 
	ADD CONSTRAINT uq_circulationmodes01
		UNIQUE (circulationmode) ;

INSERT INTO t_circulationmodes (id, circulationmode) VALUES (1, 'DYNAMIC') ;
INSERT INTO t_circulationmodes (id, circulationmode) VALUES (2, 'EXPLICIT') ;
INSERT INTO t_circulationmodes (id, circulationmode) VALUES (3, 'IMPLICIT') ;



------------------------------
-- t_circulationusers
------------------------------
CREATE TABLE t_circulationusers (
	id SERIAL NOT NULL,
	symbolicname VARCHAR(64) NOT NULL,
	host_id INTEGER NOT NULL,
	username VARCHAR(32) NOT NULL,
	password VARCHAR(256) NOT NULL,
	transfermethod_id INT2 NOT NULL,
	remotepath VARCHAR(256)
) ;

ALTER TABLE t_circulationusers 
	ADD CONSTRAINT pk_t_circulationusers
		PRIMARY KEY (id) ;

ALTER TABLE t_circulationusers 
	ADD CONSTRAINT uq_circulationusers01
		UNIQUE (symbolicname) ;

ALTER TABLE t_circulationusers 
	ADD CONSTRAINT fk_circulationusers01
		FOREIGN KEY (host_id) REFERENCES t_knownhosts (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_circulationusers 
	ADD CONSTRAINT fk_circulationusers02
		FOREIGN KEY (transfermethod_id) REFERENCES t_transferprotocols (id) 
			ON DELETE CASCADE ;



/* ------------------------------
	t_datastoragetypes
*/ ------------------------------
CREATE TABLE t_datastoragetypes (
	id INT2 NOT NULL,
	datastoragetype VARCHAR(16) NOT NULL
) ;

ALTER TABLE t_datastoragetypes 
	ADD CONSTRAINT pk_t_datastoragetypes
		PRIMARY KEY (id) ;
		
ALTER TABLE t_datastoragetypes 
	ADD CONSTRAINT uq_datastoragetypes01
		UNIQUE (datastoragetype) ;

INSERT INTO t_datastoragetypes (id, datastoragetype) VALUES (1, 'ARCHIVE') ;
INSERT INTO t_datastoragetypes (id, datastoragetype) VALUES (2, 'DELETE') ;
INSERT INTO t_datastoragetypes (id, datastoragetype) VALUES (3, 'RETRIEVE') ;



------------------------------
-- t_circulationtempnames
------------------------------
CREATE TABLE t_circulationtempnames (
    id INT2 NOT NULL,
	tempname VARCHAR(16) NOT NULL
) ;

ALTER TABLE t_circulationtempnames 
	ADD CONSTRAINT pk_t_circulationtempnames
		PRIMARY KEY (id) ;

ALTER TABLE t_circulationtempnames 
	ADD CONSTRAINT uq_circulationtempnames01
		UNIQUE (tempname) ;



------------------------------
-- t_circulationpolicies
------------------------------
CREATE TABLE t_circulationpolicies (
    id SERIAL NOT NULL,
	filetype_id VARCHAR(16) NOT NULL,
	user_id INTEGER NOT NULL,
	dest_url VARCHAR(512) NOT NULL,
	circulator_node_id INTEGER NOT NULL,
	source_path VARCHAR(256),
	tempname_id INT2
) ;

ALTER TABLE t_circulationpolicies 
	ADD CONSTRAINT pk_t_circulationpolicies
		PRIMARY KEY (id) ;

ALTER TABLE t_circulationpolicies 
	ADD CONSTRAINT fk_circulationpolicies01
		FOREIGN KEY (filetype_id) REFERENCES t_filetypes (filetype) 
			ON DELETE CASCADE ;

ALTER TABLE t_circulationpolicies 
	ADD CONSTRAINT fk_circulationpolicies02
		FOREIGN KEY (user_id) REFERENCES t_circulationusers (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_circulationpolicies 
	ADD CONSTRAINT fk_circulationpolicies03
		FOREIGN KEY (installationnode_id) REFERENCES t_knownhosts (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_circulationpolicies 
	ADD CONSTRAINT fk_circulationpolicies04
		FOREIGN KEY (tempname_id) REFERENCES t_circulationtempnames (id) 
			ON DELETE CASCADE ;



------------------------------
-- t_circulationsqueue
------------------------------
CREATE TABLE t_circulationsqueue (
    id BIGSERIAL NOT NULL,
    startdate TIMESTAMP(6),
	enddate TIMESTAMP(6),
	priority INTEGER DEFAULT 0 NOT NULL,
	remark VARCHAR(512),
	status_id INT2 DEFAULT 16 NOT NULL,
	creationdate TIMESTAMP(6) DEFAULT NOW(),
	modificationdate TIMESTAMP(6) DEFAULT NOW(),
	fromurl VARCHAR(512) NOT NULL,
	tourl VARCHAR(512) NOT NULL,
	relay_host_id INTEGER,
	relay_activity_id BIGINT,
	retries INT2 DEFAULT 0 NOT NULL,
	removesource BOOLEAN DEFAULT 'f' NOT NULL,
	circulationmode_id SMALLINT NOT NULL,
	synchronized TIMESTAMP(6),
	host_id INTEGER,
	circulationpolicy_id INTEGER
) ;

ALTER TABLE t_circulationsqueue 
	ADD CONSTRAINT pk_t_circulationsqueue
		PRIMARY KEY (id) ;

ALTER TABLE t_circulationsqueue 
	ADD CONSTRAINT fk_circulationsqueue01
		FOREIGN KEY (status_id) REFERENCES t_stati (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_circulationsqueue 
	ADD CONSTRAINT fk_circulationsqueue02
		FOREIGN KEY (circulationmode_id) REFERENCES t_circulationmodes (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_circulationsqueue 
	ADD CONSTRAINT fk_circulationsqueue03
		FOREIGN KEY (relay_host_id) REFERENCES t_securityzonerelays (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_circulationsqueue 
	ADD CONSTRAINT fk_circulationsqueue04
		FOREIGN KEY (host_id) REFERENCES t_knownhosts (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_circulationsqueue 
	ADD CONSTRAINT fk_circulationsqueue05
		FOREIGN KEY (circulationpolicy_id) REFERENCES t_circulationpolicies (id) 
			ON DELETE SET NULL;



/* ------------------------------
	t_datastoragequeue
*/ ------------------------------
CREATE TABLE t_datastoragequeue (
	id BIGSERIAL NOT NULL,
	circulation_id BIGINT,
	status_id INT2 NOT NULL,
	filename VARCHAR(128) NOT NULL,
	prodname VARCHAR(128) NOT NULL,
	url VARCHAR(512),
	type_id INT2 NOT NULL
) ;

ALTER TABLE t_datastoragequeue 
	ADD CONSTRAINT pk_t_datastoragequeue
		PRIMARY KEY (id) ;

ALTER TABLE t_datastoragequeue 
	ADD CONSTRAINT fk_datastoragequeue01
		FOREIGN KEY (circulation_id) REFERENCES t_circulationsqueue (id) 
			ON DELETE SET NULL ;

ALTER TABLE t_datastoragequeue 
	ADD CONSTRAINT fk_datastoragequeue02
		FOREIGN KEY (status_id) REFERENCES t_stati (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_datastoragequeue 
	ADD CONSTRAINT fk_datastoragequeue03
		FOREIGN KEY (type_id) REFERENCES t_datastoragetypes (id) 
			ON DELETE CASCADE ;



/* ------------------------------
	t_circulation_remove_queue
*/ ------------------------------
CREATE TABLE t_circulation_remove_queue (
    id BIGSERIAL NOT NULL,
	url VARCHAR(512) NOT NULL
) ;

ALTER TABLE t_circulation_remove_queue 
	ADD CONSTRAINT pk_t_circulation_remove_queue
		PRIMARY KEY (id) ; 








---------------------------------------------------------------------------------
--------------------------------- TABLES INSERT ---------------------------------
---------------------------------------------------------------------------------

------------------------------
-- t_dbparameters
------------------------------
INSERT INTO t_dbparameters (parameter, parametertype_id, value_int) VALUES ('circ_max_ret', 1, 5) ;
INSERT INTO t_dbparameters (parameter, parametertype_id, value_str) VALUES ('circ_start_stat', 2, 'STANDBY') ;








---------------------------------------------------------------------------------
------------------------------ PROCEDURES CREATION ------------------------------
---------------------------------------------------------------------------------

------------------------------
-- p_circulationCheckStartStatus
------------------------------
CREATE OR REPLACE FUNCTION p_circulationcheckstartstatus(
	in_stat_id SMALLINT) RETURNS INTEGER AS $$

DECLARE
	in_stat VARCHAR(32);
	def_stat VARCHAR(32);

BEGIN
	in_stat := (SELECT status FROM t_stati WHERE id = in_stat_id);
	def_stat := (SELECT value_str FROM t_dbparameters WHERE parameter = 'circ_start_stat');
	
	IF (in_stat != def_stat) THEN
		RAISE EXCEPTION 'p_circulationcheckstartstatus: Invalid start status.' ;
		RETURN 1 ;
	END IF;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;




------------------------------
-- p_circulationStateTransition
------------------------------
CREATE OR REPLACE FUNCTION p_circulationstatetransition(
	in_circ_id BIGINT,
	in_pre_stat_id INT2,
	in_post_stat_id INT2) RETURNS INTEGER AS $$
	
	
-- State Transition (from -> to | or):
------------------------------------------------
-- STANDBY -> PROCESSING | ELIGIBLE
-- ELIGIBLE -> PROCESSING | TRANSFERRING | RECOVERABLE ERROR | ERROR
-- TRANSFERRING -> PROCESSING | COMPLETED | RECOVERABLE ERROR | ERROR
-- RECOVERABLE ERROR -> PROCESSING | ELIGIBLE | ERROR
-- ERROR -> ELIGIBLE
-- PROCESSING -> ELIGIBLE | TRANSFERRING | COMPLETED | RECOVERABLE ERROR | ERROR
------------------------------------------------

DECLARE
	ex_msg VARCHAR(128);
	pre_stat VARCHAR(32);
	post_stat VARCHAR(32);
	
BEGIN
	-- Set error message
	ex_msg := 'p_CirculationStateTransition: id ' || in_circ_id || ' - invalid state transition.';
	
	-- Set stati from id
	pre_stat := (SELECT status FROM t_stati WHERE id = in_pre_stat_id) ;
	post_stat := (SELECT status FROM t_stati WHERE id = in_post_stat_id) ;
	
	
	------------------------------
	--      Starting Chek       --
	------------------------------
	-- No status changes
	IF pre_stat = post_stat THEN
		RETURN 0 ;
	END IF ;
	
	-- Changing status
	IF pre_stat = 'STANDBY' THEN
		IF post_stat = 'PROCESSING' OR post_stat = 'ELIGIBLE' THEN
			RETURN 0 ;
		ELSE 
			RAISE EXCEPTION '%', ex_msg ;
			RETURN 1 ;
		END IF ;

	ELSIF pre_stat = 'ELIGIBLE' THEN
		IF post_stat = 'PROCESSING' OR post_stat = 'TRANSFERRING' OR post_stat = 'RECOVERABLEERROR' OR post_stat = 'ERROR'  THEN
			RETURN 0 ;
		ELSE 
			RAISE EXCEPTION '%', ex_msg ;
			RETURN 1 ;
		END IF ;
	
	ELSIF pre_stat = 'TRANSFERRING' THEN
		IF post_stat = 'PROCESSING' OR post_stat = 'COMPLETED' OR post_stat = 'RECOVERABLEERROR'OR post_stat = 'ERROR'  THEN
			RETURN 0 ;
		ELSE 
			RAISE EXCEPTION '%', ex_msg ;
			RETURN 1 ;
		END IF ;
	
	ELSIF pre_stat = 'RECOVERABLEERROR' THEN
		IF post_stat = 'PROCESSING' OR post_stat = 'ELIGIBLE' OR post_stat = 'ERROR' THEN
			RETURN 0 ;
		ELSE 
			RAISE EXCEPTION '%', ex_msg ;
			RETURN 1 ;
		END IF ;
	
	ELSIF pre_stat = 'ERROR' THEN
		IF post_stat = 'ELIGIBLE' THEN
			RETURN 0 ;
		ELSE 
			RAISE EXCEPTION '%', ex_msg ;
			RETURN 1 ;
		END IF ;
	
	ELSIF pre_stat = 'PROCESSING' THEN
		IF post_stat = 'ELIGIBLE' OR post_stat = 'TRANSFERRING' OR post_stat = 'COMPLETED' OR post_stat = 'RECOVERABLEERROR' OR post_stat = 'ERROR' THEN
			RETURN 0 ;
		ELSE 
			RAISE EXCEPTION '%', ex_msg ;
			RETURN 1 ;
		END IF ;
	
	ELSE
		RAISE EXCEPTION '%', ex_msg ;
		RETURN 1 ;
		
	END IF ;
	
	RETURN 0 ;

END ;
$$ LANGUAGE plpgsql ;




------------------------------
-- p_circulationSetErrorType
------------------------------
CREATE OR REPLACE FUNCTION p_circulationseterrortype(
	in_circ_id BIGINT) RETURNS INTEGER AS $$

DECLARE
	max_ret INT2;
	rec_err_id INT2;
	err_id INT2;

BEGIN
	max_ret := (SELECT value_int FROM t_dbparameters WHERE parameter = 'circ_max_ret');
	rec_err_id := (SELECT id FROM t_stati WHERE status = 'RECOVERABLEERROR');
	err_id := (SELECT id FROM t_stati WHERE status = 'ERROR');
	
	UPDATE t_circulationsqueue SET retries = retries + 1 WHERE id = in_circ_id;
	
	IF ((SELECT retries FROM t_circulationsqueue WHERE id = in_circ_id) < max_ret) THEN
		UPDATE t_circulationsqueue SET status_id = rec_err_id WHERE id = in_circ_id;
	ELSE
		UPDATE t_circulationsqueue SET status_id = err_id WHERE id = in_circ_id;
	END IF;
	
	RETURN 0 ;

END ;
$$ LANGUAGE plpgsql ;




------------------------------
-- p_circulationResetRetries
------------------------------
CREATE OR REPLACE FUNCTION p_circulationresetretries(
	in_circ_id BIGINT,
	in_pre_stat_id INT2,
	in_post_stat_id INT2) RETURNS INTEGER AS $$

DECLARE
	pre_stat VARCHAR(32);
	post_stat VARCHAR(32);
	
BEGIN
	pre_stat := (SELECT status FROM t_stati WHERE id = in_pre_stat_id) ;
	post_stat := (SELECT status FROM t_stati WHERE id = in_post_stat_id) ;
	
	IF pre_stat = 'ERROR' AND post_stat = 'ELIGIBLE' THEN
		UPDATE t_circulationsqueue SET retries = 0 WHERE id = in_circ_id ; 
	END IF ;
	
	RETURN 0 ;
	
END ;
$$ LANGUAGE plpgsql ;




/* ------------------------------
	p_datastoragequeue_statetrans
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_datastoragequeue_statetrans(
	in_dsq_id BIGINT,
	in_dsq_type SMALLINT,
	in_pre_stat_id SMALLINT,
	in_post_stat_id SMALLINT) RETURNS INTEGER AS $$

DECLARE
	ex_msg VARCHAR(128);
	dsq_type VARCHAR(16);
	pre_stat VARCHAR(32);
	post_stat VARCHAR(32);
	
BEGIN
	-- Set error message
	ex_msg := 'p_DatastorageQueue_StateTrans: id ' || in_dsq_id || ' - invalid state transition.';
	
	-- Set stati and type from id
	dsq_type := (SELECT datastoragetype FROM t_datastoragetypes WHERE id = in_dsq_type) ;
	pre_stat := (SELECT status FROM t_stati WHERE id = in_pre_stat_id) ;
	post_stat := (SELECT status FROM t_stati WHERE id = in_post_stat_id) ;
	
	------------------------------
	--      Starting Chek       --
	------------------------------
	-- No status changes
	IF pre_stat = post_stat THEN
		RETURN 0;
	END IF ;
	
	-- status changes to ERROR
	-- (all stati can changes to error)
	IF post_stat = 'ERROR' THEN
		RETURN 0;
	END IF ;
	
	-- IF TYPE IS RETIEVE
	IF dsq_type = 'RETRIEVE' THEN
		IF pre_stat = 'ERROR' AND post_stat = 'ELIGIBLE' THEN
			RETURN 0 ;
		ELSIF pre_stat = 'ELIGIBLE' AND post_stat = 'DOWNLOADING' THEN
			RETURN 0 ;
		ELSIF pre_stat = 'DOWNLOADING' AND (post_stat = 'DISTRIBUTION' OR post_stat = 'ELIGIBLE') THEN
			RETURN 0 ;
		ELSIF pre_stat = 'DISTRIBUTION' AND post_stat = 'COMPLETED' THEN
			RETURN 0 ;
		ELSE
			RAISE EXCEPTION '%', ex_msg ;
			RETURN 1 ;
		END IF;
	END IF;
	
	-- IF TYPE IS ARCHIVE
	IF dsq_type = 'ARCHIVE' THEN
		IF pre_stat = 'ERROR' AND post_stat = 'UPLOADING' THEN
			RETURN 0 ;
		ELSIF pre_stat = 'UPLOADING' AND post_stat = 'UPLOADED' THEN
			RETURN 0 ;
		ELSIF pre_stat = 'UPLOADED' AND post_stat = 'COMPLETED' THEN
			RETURN 0 ;
		ELSE
			RAISE EXCEPTION '%', ex_msg ;
			RETURN 1 ;
		END IF;
	END IF;
	
	-- IF TYPE IS DELETE
	IF dsq_type = 'DELETE' THEN
		IF pre_stat = 'ERROR' AND post_stat = 'ELIGIBLE' THEN
			RETURN 0 ;
		ELSIF pre_stat = 'ELIGIBLE' AND post_stat = 'DELETING' THEN
			RETURN 0 ;
		ELSIF pre_stat = 'DELETING' AND post_stat = 'COMPLETED' THEN
			RETURN 0 ;
		ELSE
			RAISE EXCEPTION '%', ex_msg ;
			RETURN 1 ;
		END IF;
	END IF;
	
	RETURN 0 ;
	
END ;
$$ LANGUAGE plpgsql ;




/* ------------------------------
	p_datastoragequeue_update
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_datastoragequeue_update(
	in_cq_id BIGINT,
	in_cq_stat_id SMALLINT) RETURNS INTEGER AS $$

DECLARE
	cq_stat VARCHAR(32);

BEGIN
	cq_stat := (SELECT status FROM t_stati WHERE id = in_cq_stat_id) ;
	
	IF cq_stat = 'ERROR' THEN
		UPDATE t_datastoragequeue SET status_id = in_cq_stat_id WHERE circulation_id = in_cq_id;
	END IF;
	
	IF cq_stat = 'COMPLETED' THEN
		UPDATE t_datastoragequeue SET status_id = in_cq_stat_id 
		WHERE circulation_id = in_cq_id AND type_id = (
			SELECT id FROM t_datastoragetypes WHERE datastoragetype = 'RETRIEVE');
			
		UPDATE t_datastoragequeue SET status_id = (
			 SELECT id FROM t_stati WHERE status = 'UPLOADED')
		WHERE circulation_id = in_cq_id AND type_id = (
			SELECT id FROM t_datastoragetypes WHERE datastoragetype = 'ARCHIVE');
	END IF;
	
	RETURN 0 ;
	
END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
-------------------------- TRIGGERS FUNCTIONS CREATION --------------------------
---------------------------------------------------------------------------------

------------------------------
-- t_circulations
------------------------------
CREATE OR REPLACE FUNCTION tf_circulationsinsert() RETURNS TRIGGER AS $$
DECLARE
  	status INTEGER ;
BEGIN
  	status := p_circulationcheckstartstatus(NEW.status_id) ;
	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;

CREATE OR REPLACE FUNCTION tf_circulationstatus() RETURNS TRIGGER AS $$
DECLARE
  	status INTEGER ;
BEGIN
  	status := p_circulationstatetransition(OLD.id, OLD.status_id, NEW.status_id) ;
	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;

CREATE OR REPLACE FUNCTION tf_circulationretries() RETURNS TRIGGER AS $$
DECLARE
  	status INTEGER ;
BEGIN
  	status := p_circulationresetretries(OLD.id, OLD.status_id, NEW.status_id) ;
	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;


/* ------------------------------
	tf_datastoragequeuetrans
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_datastoragequeuetrans() RETURNS TRIGGER AS $$
DECLARE
  	status INTEGER ;
BEGIN
  	status := p_datastoragequeue_statetrans(OLD.id, OLD.status_id, NEW.status_id, NEW.type_id) ;
	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;


/* ------------------------------
	tf_datastoragequeueupdate
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_datastoragequeueupdate() RETURNS TRIGGER AS $$
DECLARE
  	status INTEGER ;
BEGIN
  	status := p_datastoragequeue_update(NEW.id, NEW.status_id) ;
	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
------------------------------- TRIGGERS CREATION -------------------------------
---------------------------------------------------------------------------------

------------------------------
-- t_circulationsqueue
------------------------------
CREATE TRIGGER tr_circulationsqueue_insert1 BEFORE INSERT 
	ON t_circulationsqueue 
		FOR EACH ROW EXECUTE PROCEDURE tf_circulationsinsert() ;

CREATE TRIGGER tr_circulationsqueue_insert2 AFTER INSERT 
	ON t_circulationsqueue 
		EXECUTE PROCEDURE tf_inserteventtableitem('t_circulationsqueue','INS') ;

CREATE TRIGGER tr_circulationsqueue_update1 BEFORE UPDATE 
	ON t_circulationsqueue 
		FOR EACH ROW EXECUTE PROCEDURE tf_circulationstatus();

CREATE TRIGGER tr_circulationsqueue_update2 AFTER UPDATE 
	ON t_circulationsqueue 
		FOR EACH ROW EXECUTE PROCEDURE tf_circulationretries();

CREATE TRIGGER tr_circulationsqueue_update3 AFTER UPDATE 
	ON t_circulationsqueue 
		FOR EACH ROW EXECUTE PROCEDURE tf_datastoragequeueupdate();

CREATE TRIGGER tr_circulationsqueue_update4 AFTER UPDATE 
	ON t_circulationsqueue 
		EXECUTE PROCEDURE tf_inserteventtableitem('t_circulationsqueue', 'UPD') ;


/* ------------------------------
	t_datastoragequeue
*/ ------------------------------
CREATE TRIGGER tr_datastoragequeue_insert AFTER INSERT 
	ON t_datastoragequeue 
		EXECUTE PROCEDURE tf_inserteventtableitem('t_datastoragequeue','INS') ;

CREATE TRIGGER tr_datastoragequeue_update1 BEFORE UPDATE 
	ON t_datastoragequeue 
		FOR EACH ROW EXECUTE PROCEDURE tf_datastoragequeuetrans();

CREATE TRIGGER tr_datastoragequeue_update2 BEFORE UPDATE 
	ON t_datastoragequeue 
		EXECUTE PROCEDURE tf_inserteventtableitem('t_datastoragequeue', 'UPD') ;








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
		'circulation-1.0.0',
		'2011-05-26',
		5,
		20,
		'CIRCULATION_1-0-0',
		'Created circulation schema.'
) ;


