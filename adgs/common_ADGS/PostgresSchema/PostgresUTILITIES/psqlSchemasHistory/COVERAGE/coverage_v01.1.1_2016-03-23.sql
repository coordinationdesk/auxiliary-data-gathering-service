
-- Version 1.1.1
-- POSTGRES








--------------------------------------------------------------------------------
------------------------------- PROCEDURE CHANGES ------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	f_cov_file_expect
*/ ------------------------------
CREATE OR REPLACE FUNCTION f_cov_file_expect(
	in_fileid BIGINT,
	in_ruleid INTEGER
) RETURNS INTEGER AS $$

DECLARE
	inv_time TIMESTAMP ;
	inv_val_start TIMESTAMP ;
	inv_val_stop TIMESTAMP ;
	period_id BIGINT ;
	period_val_start TIMESTAMP ;
	period_val_stop TIMESTAMP ;
	time_res INTEGER ;
	match_res INTEGER ;

BEGIN
	time_res := 0 ;
	match_res := 0 ;
	
	-- Get the inventorydate, validitystart and validitystop values of the file to check
	SELECT inventorydate, validitystart, validitystop INTO inv_time, inv_val_start, inv_val_stop FROM t_inventory WHERE id_inv = in_fileid ;
	
	-- ##### INVENTORY TIME CHEK #####
	-- Check if exist (and get his id, validitystart and validitystop) a compatible period for the inventory time and set the result (0: not found - 1: found)
	SELECT count(*), id_interval, exp_val_start, exp_val_stop INTO STRICT time_res, period_id, period_val_start, period_val_stop FROM t_cov_periods 
		WHERE coveragerule_id = in_ruleid 
			AND (ABS(EXTRACT(epoch FROM inv_time - t_end)::INTEGER) < (SELECT deltawait_sec FROM t_cov_rules WHERE id_coveragerule = in_ruleid)) 
		GROUP BY id_interval, exp_val_start, exp_val_stop ;
	
	-- ##### VALIDITY TIME CHEK #####
	-- Check if the validitystart and validitystop of the file to check do "match" with the exp_val_start and exp_val_stop of the found period
	IF (inv_val_start < period_val_stop AND inv_val_stop > period_val_start)
	THEN
		match_res := 1 ;
	END IF ;
	
	RETURN (time_res * match_res) ;

EXCEPTION
	WHEN NO_DATA_FOUND THEN
		RETURN 0 ;
	WHEN TOO_MANY_ROWS THEN
		RAISE EXCEPTION 'f_cov_file_expected: Too many results - the deltawait_sec value in the t_cov_rules table may be configured incorrectly.' ;
	
END ;
$$ LANGUAGE plpgsql ;








--------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ---------------------------------
--------------------------------------------------------------------------------
DELETE FROM t_schema WHERE version = 'coverage-1.1.1' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'coverage-1.1.1',
		'2016-03-23',
		6,
		25,
		'COVERAGE_1-1-1',
		'Fixed bug in f_cov_file_expect on VALIDITY TIME CHEK.'
) ;


