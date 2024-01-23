
-- Version 1.1.3
-- POSTGRES








-------------------------------------------------------------------------------
------------------------------ PROCEDURE CHANGES ------------------------------
-------------------------------------------------------------------------------

/* ------------------------------
	f_cov_file_ontime
*/ ------------------------------
CREATE OR REPLACE FUNCTION f_cov_file_ontime(
	in_fileid BIGINT,
	in_ruleid INTEGER
) RETURNS INTEGER AS $$

DECLARE
	inv_time TIMESTAMP ;
	inv_val_start TIMESTAMP ;
	inv_val_stop TIMESTAMP ;
	period_id_inv BIGINT ;
	period_to_upd BIGINT ;
	delta_w INTERVAL ;
	perc_cvr INTEGER ;
	res INTEGER ;

BEGIN
	-- Get and cast to interval the value of deltawait_sec of the rule
	delta_w := (SELECT CAST(deltawait_sec AS VARCHAR)::INTERVAL FROM t_cov_rules WHERE id_coveragerule = in_ruleid) ;
	
	-- Get the inventorydate, validitystart and validitystop values of the file to check
	SELECT inventorydate, validitystart, validitystop INTO STRICT inv_time, inv_val_start, inv_val_stop FROM t_inventory WHERE id_inv = in_fileid ;
	
	
	-- ##### INVENTORY AND VALIDITY TIME CHEK #####
	-- Find the period id using inventorytime 
	SELECT id_interval INTO STRICT period_id_inv FROM t_cov_periods
	WHERE coveragerule_id = in_ruleid
		AND t_begin <= inv_time
		AND inv_time < t_end ;
	
	-- If a period was found...
	IF period_id_inv IS NOT NULL THEN
		-- ...check if the inventory validity times satisfy the validity times of the found period
		IF (SELECT count(*) FROM t_cov_periods WHERE 
				id_interval = period_id_inv
				AND (exp_val_start - delta_w ) <= inv_val_start
				AND (exp_val_stop + delta_w ) >= inv_val_stop
			) > 0 
		THEN
			-- the period to update is the same
			period_to_upd := period_id_inv ;
			-- the result of function is TRUE
			res := 1 ;
		ELSE
			-- find a period having expected validity times nearly to the inventory validity times
			SELECT sub.id_interval INTO period_to_upd FROM (
				SELECT id_interval, (inv_time - t_end) AS diff
				FROM t_cov_periods 			
				WHERE coveragerule_id = in_ruleid
					AND (exp_val_start - delta_w ) < inv_val_start
					AND (exp_val_stop + delta_w ) > inv_val_stop
				ORDER BY diff
				LIMIT 1) sub ;
			-- the result of function is FALSE
			res := 0 ;
		END IF ;
	ELSE
		-- find a period having expected validity times nearly to the inventory validity times
		SELECT sub.id_interval INTO period_to_upd FROM (
			SELECT id_interval, (inv_time - t_end) AS diff
			FROM t_cov_periods 			
			WHERE coveragerule_id = in_ruleid
				AND (exp_val_start - delta_w ) < inv_val_start
				AND (exp_val_stop + delta_w ) > inv_val_stop
			ORDER BY diff
			LIMIT 1) sub ;
		-- the result of function is FALSE
		res := 0 ;
	END IF ;
	
	
	-- ##### PERIODS UPDATE #####
	IF period_to_upd IS NOT NULL THEN
		-- Clean t_cov_relevancies
		DELETE FROM t_cov_relevancies WHERE inv_id = in_fileid ;
		-- Insert new value into t_cov_relevancies
		INSERT INTO t_cov_relevancies (
			period_id,
			inv_id,
			inv_val_start,
			inv_val_stop)
		VALUES (
			period_to_upd,
			in_fileid,
			inv_val_start,
			inv_val_stop );
		
		-- Update events and coverage percentage
		UPDATE t_cov_periods SET events = (events + 1), cover_pct = f_cov_percentage_calculator(period_to_upd) WHERE id_interval = period_to_upd ;
	END IF ;
	
	RETURN res ;
	
EXCEPTION
	WHEN NO_DATA_FOUND THEN
		RETURN 0 ;
	WHEN TOO_MANY_ROWS THEN
		RAISE EXCEPTION 'f_cov_file_ontime: Too many rows.' ;

END ;
$$ LANGUAGE plpgsql ;








-------------------------------------------------------------------------------
--------------------------------- CONCLUSIONS ---------------------------------
-------------------------------------------------------------------------------
DELETE FROM t_schema WHERE version = 'coverage-1.1.3' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'coverage-1.1.3',
		'2022-08-03',
		6,
		25,
		'COVERAGE_1-1-3',
		'Fixed bug on f_cov_file_ontime function (S3MARINE-636)'
) ;


