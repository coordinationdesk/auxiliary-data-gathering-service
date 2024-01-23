
-- Version 11.6.0
-- POSTGRES








---------------------------------------------------------------------------------
-------------------------------- TABLES CREATION --------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_distributionruleslock
*/ ------------------------------
CREATE TABLE t_distributionruleslock (
    packageid   INTEGER NOT NULL,
	responsible VARCHAR(128) NOT NULL,
    tstamp   TIMESTAMP(6) DEFAULT NOW() NOT NULL
) ;
ALTER TABLE t_distributionruleslock
  ADD CONSTRAINT pk_t_distributionruleslock
  	PRIMARY KEY (packageid) ;

ALTER TABLE t_distributionruleslock
  ADD CONSTRAINT fk_t_distributionruleslock01
  	FOREIGN KEY (packageid)
	  REFERENCES t_distributionqueue (packageid)
  	  	ON DELETE CASCADE ;








---------------------------------------------------------------------------------
------------------------------ TABLES MODIFICATION ------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_smqueue
*/ ------------------------------
ALTER TABLE t_smqueue ADD asynchifparams TEXT ;
ALTER TABLE t_smqueue ADD extparams TEXT ;








---------------------------------------------------------------------------------
---------------------------------- TABLES DROP ----------------------------------
---------------------------------------------------------------------------------

DROP TABLE t_distributionpolicies CASCADE ;
DROP TABLE t_receptionpolicieslock CASCADE ;
DROP TABLE t_receptionpolicies CASCADE ;

DROP TABLE t_iorepository CASCADE ;
DROP FUNCTION tf_iorepositoryevent() ;
DROP FUNCTION p_iorepository(VARCHAR,SMALLINT,SMALLINT,SMALLINT) ;








---------------------------------------------------------------------------------
---------------------------- PROCEDURES MODIFICATION ----------------------------
---------------------------------------------------------------------------------

---------------------------
--- P_SMSTATETRANSITION ---
---------------------------
CREATE OR REPLACE FUNCTION p_smStateTransition(
	in_packageid INT, 
	pre_status VARCHAR(32), 
	post_status VARCHAR(32)) RETURNS INTEGER AS $$

DECLARE
	ex_msg VARCHAR(128);

BEGIN

-- State Transition (from -> to | or):
------------------------------------------------
-- STANDBY -> ELIGIBLE
-- ELIGIBLE -> PREPARING | SUBMITTED
-- PREPARING -> SUBMITTED | ERROR
-- SUBMITTED -> WAITINGACK | ACKNOWLEDGED | COMPLETED | ERROR
-- ACKNOWLEDGED -> UPLOADING | ERROR
-- UPLOADING -> COMPLETED | ERROR
-- WAITINGACK -> WAITINGACK | ACKNOWLEDGED | COMPLETED | ERROR
-- COMPLETED -> to nothing
-- ERROR -> ELIGIBLE | WAITINGACK
------------------------------------------------

	-- Set error message
	ex_msg := 'p_smStateTransition: id ' || in_packageid || ' - ' || pre_status || ' to ' || post_status || ' invalid sm state transition.';
	
	
	-- Empty status
	IF post_status IS NULL THEN
		RETURN 0;
	END IF ;
	
	
	-- New insert
	IF pre_status IS NULL AND post_status = 'STANDBY' THEN
		RETURN 0;
	END IF ;


	-- No changes status
	IF pre_status = post_status THEN
		RETURN 0;
	END IF ;
	

	-- Changing status
	IF post_status = 'ELIGIBLE'
		AND (pre_status = 'STANDBY' OR pre_status = 'ERROR') THEN
			RETURN 0 ;

	ELSIF post_status = 'PREPARING'
		AND pre_status = 'ELIGIBLE' THEN
			RETURN 0 ;
	
	ELSIF post_status = 'SUBMITTED'
		AND (pre_status = 'ELIGIBLE' OR pre_status = 'PREPARING') THEN
			RETURN 0 ;
	
	ELSIF post_status = 'ACKNOWLEDGED'
		AND (pre_status = 'SUBMITTED' OR pre_status = 'WAITINGACK') THEN
			RETURN 0 ;
	
	ELSIF post_status = 'UPLOADING' 
		AND pre_status = 'ACKNOWLEDGED' THEN
			RETURN 0 ;
	
	ELSIF post_status = 'COMPLETED' 
		AND (pre_status = 'UPLOADING' OR pre_status = 'SUBMITTED' OR pre_status = 'WAITINGACK') THEN
			RETURN 0 ;
	
	ELSIF post_status = 'ERROR' 
		AND (pre_status = 'PREPARING' OR pre_status = 'SUBMITTED' OR pre_status = 'ACKNOWLEDGED' OR pre_status = 'UPLOADING' OR pre_status = 'WAITINGACK') THEN
			RETURN 0 ;
	
	ELSIF post_status = 'WAITINGACK' 
		AND (pre_status = 'SUBMITTED' OR pre_status = 'WAITINGACK' OR pre_status = 'ERROR') THEN
			RETURN 0 ;
	
	ELSE
		RAISE EXCEPTION '%', ex_msg ;
	
	END IF ;	

	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
------------------------------- TRIGGER CREATION --------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_repositories
*/ ------------------------------
CREATE TRIGGER tr_repositoriesinsert AFTER INSERT
	ON t_repositories 
		FOR EACH ROW EXECUTE PROCEDURE tf_inserteventtableitem('t_repositories' ,'INS' ) ;

CREATE TRIGGER tr_repositoriesupdate AFTER UPDATE
	ON t_repositories 
		FOR EACH ROW EXECUTE PROCEDURE tf_inserteventtableitem('t_repositories' ,'UPD' ) ;

/* ------------------------------
	t_distributionrules
*/ ------------------------------
CREATE TRIGGER tr_distributionrulesinsert AFTER INSERT
	ON t_distributionrules 
		FOR EACH ROW EXECUTE PROCEDURE tf_inserteventtableitem('t_distributionrules' ,'INS' ) ;

CREATE TRIGGER tr_distributionrulesupdate AFTER UPDATE
	ON t_distributionrules 
		FOR EACH ROW EXECUTE PROCEDURE tf_inserteventtableitem('t_distributionrules' ,'UPD' ) ;








---------------------------------------------------------------------------------
------------------------------- VIEWS MODIFICATION ------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	v_distribution
*/ ------------------------------
DROP VIEW v_distribution ;
CREATE OR REPLACE VIEW v_distribution (
	packageid, 
	status, 
	distributeddate, 
	jobresponsible, 
	creationdate, 
	mediatype, 
	userid,
	distributionrule_id)
AS SELECT DISTINCT 
	dq.packageid, 
	dq.status, 
	dq.distributeddate, 
	dq.jobresponsible, 
	dq.creationdate, 
	di.mediatype, 
	di.userid,
	di.distributionrule_id
FROM 
	t_distributionqueue dq 
	INNER JOIN t_distributionitems di
		ON dq.packageid = di.packageid
;








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
		'pds-11.6.0',
		'2013-11-28',
		3,
		2,
		'PDS_11-6-0',
		'Added table t_distributionruleslock. Added new fields into t_smqueue. Removed tables t_distributionpolicies, t_receptionpolicies, t_receptionpolicieslock and t_iorepository.'
) ;


