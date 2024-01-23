
-- Version 1.24.0
-- POSTGRES








--------------------------------------------------------------------------------
-------------------------------- TABLES CREATION -------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_blockedcirculationsqueue
*/ ------------------------------
CREATE TABLE t_blockedcirculationsqueue (
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
	host_id INTEGER,
	circulationpolicy_id INTEGER,
	dest_host_id INTEGER,
	error VARCHAR(512),
	tempnamevalue VARCHAR(64),
	tempname_id SMALLINT,
	nextretrydate TIMESTAMP(6) DEFAULT NOW(),
	dest_protocol_id SMALLINT,
	media_circulation_id INTEGER,
	media_insertionmode_id SMALLINT DEFAULT 1 NOT NULL,
	localtransfermode_id SMALLINT DEFAULT 1 NOT NULL
) ;

ALTER TABLE t_blockedcirculationsqueue 
	ADD CONSTRAINT pk_t_blockedcirculationsqueue
		PRIMARY KEY (id) ;

ALTER TABLE t_blockedcirculationsqueue
	ADD CONSTRAINT ck_blockedcirculationsqueue01			
		CHECK (char_length(trim(tempnamevalue)) != 0) ;

ALTER TABLE t_blockedcirculationsqueue
	ADD CONSTRAINT ck_blockedcirculationsqueue02
		CHECK ((tempname_id != 2 AND (tempnamevalue = '..' OR tempnamevalue = '../')) != TRUE) ;

ALTER TABLE t_blockedcirculationsqueue
	ADD CONSTRAINT ck_blockedcirculationsqueue03
		CHECK ((tempname_id = 3 AND (tempnamevalue = '.' OR tempnamevalue = './')) != TRUE) ;

ALTER TABLE t_blockedcirculationsqueue 
	ADD CONSTRAINT fk_blockedcirculationsqueue01
		FOREIGN KEY (status_id) REFERENCES t_stati (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_blockedcirculationsqueue 
	ADD CONSTRAINT fk_blockedcirculationsqueue02
		FOREIGN KEY (circulationmode_id) REFERENCES t_circulationmodes (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_blockedcirculationsqueue 
	ADD CONSTRAINT fk_blockedcirculationsqueue03
		FOREIGN KEY (relay_host_id) REFERENCES t_securityzonerelays (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_blockedcirculationsqueue 
	ADD CONSTRAINT fk_blockedcirculationsqueue04
		FOREIGN KEY (host_id) REFERENCES t_knownhosts (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_blockedcirculationsqueue 
	ADD CONSTRAINT fk_blockedcirculationsqueue05
		FOREIGN KEY (circulationpolicy_id) REFERENCES t_circulationpolicies (id) 
			ON DELETE SET NULL ;

ALTER TABLE t_blockedcirculationsqueue 
	ADD CONSTRAINT fk_blockedcirculationsqueue06
		FOREIGN KEY (dest_host_id) REFERENCES t_knownhosts (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_blockedcirculationsqueue 
	ADD CONSTRAINT fk_blockedcirculationsqueue07
		FOREIGN KEY (tempname_id) REFERENCES t_tmpnamestrategy (id_tmpnamestrategy) 
			ON DELETE CASCADE ;

ALTER TABLE t_blockedcirculationsqueue 
	ADD CONSTRAINT fk_blockedcirculationsqueue08
		FOREIGN KEY (dest_protocol_id) REFERENCES t_transferprotocols (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_blockedcirculationsqueue 
	ADD CONSTRAINT fk_blockedcirculationsqueue09
		FOREIGN KEY (media_circulation_id) REFERENCES t_circulationsmedia (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_blockedcirculationsqueue 
	ADD CONSTRAINT fk_blockedcirculationsqueue10
		FOREIGN KEY (media_insertionmode_id) REFERENCES t_circulationsmedia_insmode (id) 
			ON DELETE CASCADE ;

ALTER TABLE t_blockedcirculationsqueue 
	ADD CONSTRAINT fk_blockedcirculationsqueue11
		FOREIGN KEY (localtransfermode_id) REFERENCES t_localtransfermodes (id) 
			ON DELETE CASCADE ;

CREATE INDEX ix_t_blockedcirculationsqueue01 ON t_blockedcirculationsqueue USING BTREE (status_id) ;
CREATE INDEX ix_t_blockedcirculationsqueue02 ON t_blockedcirculationsqueue USING BTREE (circulationmode_id) ;
CREATE INDEX ix_t_blockedcirculationsqueue03 ON t_blockedcirculationsqueue USING BTREE (relay_host_id) ;
CREATE INDEX ix_t_blockedcirculationsqueue04 ON t_blockedcirculationsqueue USING BTREE (host_id) ;
CREATE INDEX ix_t_blockedcirculationsqueue05 ON t_blockedcirculationsqueue USING BTREE (circulationpolicy_id) ;
CREATE INDEX ix_t_blockedcirculationsqueue06 ON t_blockedcirculationsqueue USING BTREE (dest_host_id) ;
CREATE INDEX ix_t_blockedcirculationsqueue07 ON t_blockedcirculationsqueue USING BTREE (fromurl) ;
CREATE INDEX ix_t_blockedcirculationsqueue08 ON t_blockedcirculationsqueue USING BTREE (tourl) ;
CREATE INDEX ix_t_blockedcirculationsqueue09 ON t_blockedcirculationsqueue USING BTREE (dest_protocol_id) ;
CREATE INDEX ix_t_blockedcirculationsqueue10 ON t_blockedcirculationsqueue USING BTREE (media_circulation_id) ;
CREATE INDEX ix_t_blockedcirculationsqueue11 ON t_blockedcirculationsqueue USING BTREE (media_insertionmode_id) ;
CREATE INDEX ix_t_blockedcirculationsqueue12 ON t_blockedcirculationsqueue USING BTREE (enddate) ;
CREATE INDEX ix_t_blockedcirculationsqueue13 ON t_blockedcirculationsqueue USING BTREE (localtransfermode_id) ;








--------------------------------------------------------------------------------
-------------------------------- TABLES CHANGES --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_circulationpolicies
*/ ------------------------------
ALTER TABLE t_circulationpolicies ADD status_id SMALLINT DEFAULT 43 NOT NULL ;

ALTER TABLE t_circulationpolicies 
	ADD CONSTRAINT fk_circulationpolicies07
		FOREIGN KEY (status_id) REFERENCES t_stati (id) 
			ON DELETE CASCADE ;

CREATE INDEX ix_t_circulationpolicies07 ON t_circulationpolicies USING BTREE (status_id) ;








--------------------------------------------------------------------------------
-------------------------------- TABLES UPDATES --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_dbparameters
*/ ------------------------------
DELETE FROM t_dbparameters WHERE parameter = 'circ_start_stat' ;


/* ------------------------------
	t_statetransitions_allowed
*/ ------------------------------
INSERT INTO t_statetransitions_allowed (table_id, prev_state_id, post_state_id) VALUES (
	(SELECT id FROM t_statetransitions_tables WHERE table_name = 't_circulationsqueue'),
	(SELECT id FROM t_stati WHERE status = 'BLOCKED'),
	(SELECT id FROM t_stati WHERE status = 'BLOCKING')) ;








--------------------------------------------------------------------------------
------------------------------ PROCEDURES CREATION -----------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	p_change_circ_policy_status
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_change_circ_policy_status (
	pol_id INTEGER,
	post_status_id INT2,
	change_creation_date BOOLEAN DEFAULT 't',
	new_priority INTEGER DEFAULT 0
) RETURNS INTEGER AS $$

DECLARE
	post_status_val VARCHAR(32);
	
BEGIN
	-- Get the value of the status passed using his id
	SELECT status INTO post_status_val FROM t_stati WHERE id = post_status_id ;
	
	IF post_status_val = 'BLOCKED' THEN
		UPDATE t_circulationsqueue SET status_id = (SELECT id FROM t_stati WHERE status = 'ERROR') 
		WHERE circulationpolicy_id = pol_id AND status_id = (SELECT id FROM t_stati WHERE status = 'SUSPENDED') ;
	
	ELSIF post_status_val = 'DISABLED' THEN
		UPDATE t_circulationsqueue SET status_id = (SELECT id FROM t_stati WHERE status = 'ERROR')  
		WHERE circulationpolicy_id = pol_id AND status_id IN (SELECT id FROM t_stati WHERE status IN ('SUSPENDED', 'BLOCKED')) ;
	
	ELSIF post_status_val = 'ENABLED' THEN
		IF change_creation_date = 't' THEN
			UPDATE t_circulationsqueue SET status_id = (SELECT id FROM t_stati WHERE status = 'ELIGIBLE'), priority = (priority + new_priority), creationdate = NOW() 
			WHERE circulationpolicy_id = pol_id AND status_id = (SELECT id FROM t_stati WHERE status = 'SUSPENDED') ;
		ELSE
			UPDATE t_circulationsqueue SET status_id = (SELECT id FROM t_stati WHERE status = 'ELIGIBLE'), priority = (priority + new_priority) 
			WHERE circulationpolicy_id = pol_id AND status_id = (SELECT id FROM t_stati WHERE status = 'SUSPENDED') ;
		END IF;
	ELSE -- post_status_val = 'SUSPENDED'
		RETURN 0 ;
	END IF ;
	
	RETURN 0 ;
	
END ;
$$ LANGUAGE plpgsql ;








--------------------------------------------------------------------------------
------------------------------ PROCEDURES CHANGES ------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	p_circulationCheckStartStatus
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_circulationcheckstartstatus(
	in_stat_id SMALLINT) RETURNS INTEGER AS $$

DECLARE
	in_stat VARCHAR(32);
	def_stat VARCHAR(32);

BEGIN
	in_stat := (SELECT status FROM t_stati WHERE id = in_stat_id);
	--def_stat := (SELECT value_str FROM t_dbparameters WHERE parameter = 'circ_start_stat');
	
	IF (in_stat != 'BLOCKED') AND (in_stat != 'ELIGIBLE') AND (in_stat != 'STANDBY') AND (in_stat != 'SUSPENDED') THEN
		RAISE EXCEPTION 'p_circulationcheckstartstatus: Invalid initial status. First status can be only BLOCKED, ELIGIBLE, STANDBY or SUSPENDED' ;
		RETURN 1 ;
	END IF;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;








--------------------------------------------------------------------------------
-------------------------- TRIGGER FUNCTIONS CREATION --------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	tf_circ_policies_statcheck
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_circ_policies_statcheck() RETURNS TRIGGER AS $$

DECLARE
  	errMsg VARCHAR(128) ;

BEGIN
	IF (TG_OP = 'INSERT') THEN
		IF NEW.status_id != (SELECT id FROM t_stati WHERE status = 'ENABLED') THEN
			errMsg := 'tf_circ_policies_statcheck: the first status must be ENABLED' ;
			RAISE EXCEPTION '%', errMsg ;
		END IF ;
	END IF ;

	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;








--------------------------------------------------------------------------------
--------------------------- TRIGGER FUNCTIONS CHANGES --------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	tf_circulationstatus
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_circulationstatus() RETURNS TRIGGER AS $$
BEGIN
  	PERFORM p_statetransitions_check('t_circulationsqueue', 1, OLD.status_id, NEW.status_id);
	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;








--------------------------------------------------------------------------------
------------------------------- TRIGGERS CREATION ------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_circulationpolicies
*/ ------------------------------
CREATE TRIGGER tr_circulationpolicies_ins_bfr BEFORE INSERT 
	ON t_circulationpolicies 
		FOR EACH ROW EXECUTE PROCEDURE tf_circ_policies_statcheck() ;








--------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ---------------------------------
--------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'circulation-1.24.0' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'circulation-1.24.0',
		'2018-07-27',
		6,
		20,
		'CIRCULATION_1-24-0',
		'Added t_blockedcirculationsqueue table and status_id filed on t_circulationpolicies table. Added p_change_circ_policy_status procedure and new statuses on p_circulationstatetransition procedure'
) ;


