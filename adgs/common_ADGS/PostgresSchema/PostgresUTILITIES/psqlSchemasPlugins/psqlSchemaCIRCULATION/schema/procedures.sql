/*

Copyright 2010-2015, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S. Databases schema procedures script $

	$Id$

	$Author$
	
*/





--------------------------------------------------------------------------------
--
--    	  	  	  	  	  	  	  PROCEDURES
--
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




/* ------------------------------
	p_circulationResetRetries
*/ ------------------------------
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
	p_circulationSetErrorType
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_circulationseterrortype(
	in_circ_id BIGINT) RETURNS INTEGER AS $$

DECLARE
	max_ret INT2;
	rec_err_id INT2;
	err_id INT2;
	mode VARCHAR(16);

BEGIN
	rec_err_id := (SELECT id FROM t_stati WHERE status = 'RECOVERABLEERROR');
	err_id := (SELECT id FROM t_stati WHERE status = 'ERROR');
	mode := (SELECT m.circulationmode FROM t_circulationsqueue q INNER JOIN t_circulationmodes m ON q.circulationmode_id = m.id WHERE q.id = in_circ_id);
	
	IF (mode = 'DYNAMIC') THEN
		max_ret := (SELECT value_int FROM t_dbparameters WHERE parameter = 'circ_dyn_max_ret');
	ELSEIF (mode = 'EXPLICIT') THEN
		max_ret := (SELECT value_int FROM t_dbparameters WHERE parameter = 'circ_expl_max_ret');
	ELSEIF (mode = 'IMPLICIT') THEN
		max_ret := (SELECT value_int FROM t_dbparameters WHERE parameter = 'circ_impl_max_ret');
	ELSE
		max_ret := (SELECT value_int FROM t_dbparameters WHERE parameter = 'circ_def_max_ret');
	END IF;
	
	UPDATE t_circulationsqueue SET retries = retries + 1 WHERE id = in_circ_id;
	
	IF ((SELECT retries FROM t_circulationsqueue WHERE id = in_circ_id) < max_ret) THEN
		UPDATE t_circulationsqueue SET status_id = rec_err_id WHERE id = in_circ_id;
	ELSE
		UPDATE t_circulationsqueue SET status_id = err_id WHERE id = in_circ_id;
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
	ds_id BIGINT;
	cq_stat VARCHAR(32);
	dxc_count INT;
	cq_stat_count INT;
	row RECORD ;
	archive_type_id SMALLINT ;
	lta_archive_type_id SMALLINT ;
	
BEGIN
	cq_stat := (SELECT status FROM t_stati WHERE id = in_cq_stat_id) ;
	archive_type_id := (SELECT id FROM t_datastoragetypes WHERE datastoragetype = 'ARCHIVE') ;
	lta_archive_type_id := (SELECT id FROM t_datastoragetypes WHERE datastoragetype = 'LTA_ARCHIVE') ;
	
	FOR row IN
		(SELECT datastoragequeue_id FROM t_dsqueue_x_cirqueue WHERE circulation_id = in_cq_id) LOOP
	
		ds_id := row.datastoragequeue_id ;
	
		IF cq_stat = 'ERROR' THEN
			IF (SELECT type_id FROM t_datastoragequeue WHERE id = ds_id) IN (archive_type_id, lta_archive_type_id) THEN
				UPDATE t_dsqueue_x_cirqueue SET globalstatus_id = in_cq_stat_id WHERE datastoragequeue_id = ds_id AND circulation_id = in_cq_id ; 
			ELSE
				UPDATE t_datastoragequeue SET status_id = in_cq_stat_id WHERE id = ds_id ;
			END IF ;
		END IF;
	
		IF cq_stat = 'COMPLETED' THEN
			IF (SELECT type_id FROM t_datastoragequeue WHERE id = ds_id) IN (archive_type_id, lta_archive_type_id) THEN
				UPDATE t_dsqueue_x_cirqueue SET globalstatus_id = (
					SELECT id FROM t_stati WHERE status = 'UPLOADED')
						WHERE datastoragequeue_id = ds_id AND circulation_id = in_cq_id;
			ELSE
			
				dxc_count := (SELECT count(*) FROM t_dsqueue_x_cirqueue WHERE datastoragequeue_id = ds_id);	
				cq_stat_count := (SELECT count(*) FROM t_dsqueue_x_cirqueue dxc
									INNER JOIN t_circulationsqueue cq ON dxc.circulation_id = cq.id
										WHERE dxc.datastoragequeue_id = ds_id AND cq.status_id = in_cq_stat_id);
			
				IF dxc_count = cq_stat_count THEN
					UPDATE t_datastoragequeue SET status_id = in_cq_stat_id 
					WHERE id = ds_id AND type_id = (
						SELECT id FROM t_datastoragetypes WHERE datastoragetype = 'RETRIEVE');
				
					--UPDATE t_datastoragequeue SET status_id = (
					-- 	SELECT id FROM t_stati WHERE status = 'UPLOADED')
					--WHERE id = ds_id AND type_id = (
					--	SELECT id FROM t_datastoragetypes WHERE datastoragetype = 'ARCHIVE');
				END IF;
			END IF;
		END IF;
	END LOOP ;
	
	RETURN 0 ;
	
END ;
$$ LANGUAGE plpgsql ;




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


