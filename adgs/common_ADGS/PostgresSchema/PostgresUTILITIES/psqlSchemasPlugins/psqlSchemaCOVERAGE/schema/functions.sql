/*

Copyright 2010-2020, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S. Databases schema functions script $

	$Id$

	$Author$

	$Log$
	Revision 1.4  2016/03/24 13:58:59  achnat
	Fixed bug in f_cov_file_expect on VALIDITY TIME CHEK
	
	Revision 1.3  2014/01/20 16:32:41  achnat
	Changed f_cov_file_ontime.
	
	Revision 1.2  2013/10/10 09:29:27  achnat
	Created schema for COVERAGE_1-0-0
	
	Revision 1.1.1.1  2013/10/08 15:07:46  achnat
	psqlSchemaCOVERAGE
	
*/





--------------------------------------------------------------------------------
--
--    	  	  	  	  	  	  	 FUNCTIONS
--
--------------------------------------------------------------------------------

/* ------------------------------
	f_cov_percentage_calculator
*/ ------------------------------
CREATE OR REPLACE FUNCTION f_cov_percentage_calculator(
	in_period_id BIGINT
) RETURNS INTEGER AS $$

DECLARE
	cov_sum_interval INTEGER ;
	cov_ref_interval INTEGER ;
	
BEGIN
	SELECT EXTRACT(epoch FROM (exp_val_stop - exp_val_start)) INTO cov_ref_interval FROM t_cov_periods WHERE id_interval = in_period_id ;
	
	CREATE TEMP TABLE tmp_cov_val_to_calculate (
		rec_id BIGINT,
		group_id BIGINT,
		val_start TIMESTAMP, 
		val_stop TIMESTAMP) 
	;
		
	INSERT INTO tmp_cov_val_to_calculate (
		rec_id,
		group_id,
		val_start, 
		val_stop)
	SELECT 
		sub.r_id, 
		MIN(sub.g_id) AS grp,
		sub.inv_val_start,
		sub.inv_val_stop
	FROM (
		SELECT 
			b.id_relevancy AS r_id, 
			LEAST(a.id_relevancy, b.id_relevancy) AS g_id,
			b.inv_val_start,
			b.inv_val_stop
		FROM
			t_cov_relevancies a
			INNER JOIN t_cov_relevancies b
				ON a.period_id = b.period_id 
					AND b.id_relevancy >= a.id_relevancy
		WHERE
			(a.inv_val_start, a.inv_val_stop) OVERLAPS (b.inv_val_start, b.inv_val_stop)
			AND a.period_id = in_period_id
		) sub
	GROUP BY 
		sub.r_id,
		sub.inv_val_start,
		sub.inv_val_stop
	;
	
	SELECT 
		SUM(t.int_list) 
	INTO 
		cov_sum_interval 
	FROM (
		SELECT
			EXTRACT( epoch FROM MAX(val_stop) - MIN(val_start))::INTEGER AS int_list
		FROM
			tmp_cov_val_to_calculate
		GROUP BY
			group_id
		) t ;
	
	DROP TABLE tmp_cov_val_to_calculate ;
	
	RETURN cov_sum_interval * 100 / cov_ref_interval ;
	
END ;
$$ LANGUAGE plpgsql ;



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



/* ------------------------------
	f_cov_file_cover
*/ ------------------------------
CREATE OR REPLACE FUNCTION f_cov_file_cover(
	in_fileid BIGINT,
	in_ruleid INTEGER
) RETURNS INTEGER AS $$

DECLARE
	period_id BIGINT ;
	delta_w INTERVAL ;

BEGIN
	-- Get and cast to interval the value of deltawait_sec of the rule
	delta_w := (SELECT CAST(deltawait_sec AS VARCHAR)::INTERVAL FROM t_cov_rules WHERE id_coveragerule = in_ruleid) ;
	
	SELECT id_interval INTO STRICT period_id
	FROM
		t_cov_periods cp
		INNER JOIN t_inventory iv
			ON iv.validitystart BETWEEN (cp.exp_val_start - delta_w ) AND (cp.exp_val_start + delta_w)
			AND iv.validitystop BETWEEN (cp.exp_val_stop - delta_w ) AND (cp.exp_val_stop + delta_w)
	WHERE
		iv.id_inv = in_fileid
		AND cp.coveragerule_id = in_ruleid
	;
	
	IF period_id IS NOT NULL THEN
		UPDATE t_cov_periods SET events = (events + 1), cover_pct = 100  WHERE id_interval = period_id ;
	END IF ;
	
	RETURN 1 ;
	
EXCEPTION
	WHEN NO_DATA_FOUND THEN
		RETURN 0 ;
	WHEN TOO_MANY_ROWS THEN
		RAISE EXCEPTION 'f_cov_file_cover: Too many results - the deltawait_sec value in the t_cov_rules table may be configured incorrectly.' ;

END ;
$$ LANGUAGE plpgsql ;



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
		UPDATE t_cov_periods SET events = (events + 1), cover_pct = f_cov_percentage_calculator(period_id_inv) WHERE id_interval = period_to_upd ;
	END IF ;
	
	RETURN res ;
	
EXCEPTION
	WHEN NO_DATA_FOUND THEN
		RETURN 0 ;
	WHEN TOO_MANY_ROWS THEN
		RAISE EXCEPTION 'f_cov_file_ontime: Too many rows.' ;

END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	f_cov_file_gapsatisfied
*/ ------------------------------
CREATE OR REPLACE FUNCTION f_cov_file_gapsatisfied(
	in_fileid BIGINT,
	in_ruleid INTEGER
) RETURNS INTEGER AS $$

DECLARE
	r_check_flag BOOLEAN ;
	r_delta_gap INTEGER ;
	inv_ftype VARCHAR(16) ;
	inv_valstart TIMESTAMP ;
	
BEGIN
	-- Get check_delta_gap and delta_gap values from corresponding rule
	SELECT check_delta_gap, delta_gap INTO r_check_flag, r_delta_gap FROM t_cov_rules WHERE id_coveragerule = in_ruleid ; 
	
	-- First contition: If check_delta_gap is false return 1
	IF r_check_flag = 'f' THEN
		RETURN 1 ;
	END IF ;
	
	-- Else start check for delta gap
	-- Get filetype and validitystart values from corresponding inventory
	SELECT filetype, validitystart INTO inv_ftype, inv_valstart FROM t_inventory WHERE id_inv = in_fileid ; 
	
	-- Select the validitystop of the first record having the validitystart smaller then the analyzing file
	-- and check if the difference between the end and the start is smaller then r_delta_gap
	IF (SELECT EXTRACT(epoch FROM (inv_valstart - MAX(validitystop)))::INTEGER
		FROM t_inventory
		WHERE filetype = inv_ftype AND validitystart < inv_valstart) < r_delta_gap
	THEN
		RETURN 1 ;
	END IF ;
	
	-- Everything else... 
	RETURN 0 ;
	
END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	f_cov_check_file
*/ ------------------------------
CREATE OR REPLACE FUNCTION f_cov_check_file(
	in_fileid BIGINT
) RETURNS TABLE(w_exp_res INT, w_cov_res INT, w_ont_res INT, w_gap_res INT) AS $$

DECLARE
	ftype_id INTEGER ;
	rules RECORD ;
	rel_found INTEGER ;
	exp_res INTEGER ;
	cov_res INTEGER ;
	ont_res INTEGER ;
	gap_res INTEGER ;
	rtype CHAR ;
	
BEGIN	
	-- Initialize values
	rel_found := 0 ;
	exp_res := 0 ;
	cov_res := 0 ;
	ont_res := 0 ;
	gap_res := 0 ;
	
	-- Create and initialize the temporary table will contains the results
	CREATE TEMP TABLE temp_results (t_exp_res INT, t_cov_res INT, t_ont_res INT, t_gap_res INT) ;
	INSERT INTO temp_results VALUES (0, 0, 0, 0) ;
	
	-- Get the filetype id of the file
	SELECT ft.id_filetype INTO ftype_id FROM t_inventory iv INNER JOIN t_filetypes ft ON iv.filetype = ft.filetype WHERE iv.id_inv = in_fileid ;
	
	-- For each rule associated at the filetype
	FOR rules IN (
		SELECT coveragerule_id FROM t_cov_x_filetypes WHERE filetype_id = ftype_id)
	LOOP
		-- Get the rule type
		SELECT rule_type INTO rtype FROM t_cov_rules WHERE id_coveragerule = rules.coveragerule_id ;
		
		-- If is an 'A' type rule or if is a 'on demand' rule (C)
		IF rtype = 'A' OR rtype = 'C' THEN
			-- Set to NULL results for B type rules
			ont_res := NULL ;
			gap_res := NULL ;
			-- Call functions to chek files associated to A type rules
			exp_res := exp_res + f_cov_file_expect(in_fileid, rules.coveragerule_id) ;
			cov_res := cov_res + f_cov_file_cover(in_fileid,rules.coveragerule_id) ;
		END IF ;
		
		-- If is an 'B' type rule
		IF rtype = 'B' THEN
			-- Set to NULL results for A type rules
			exp_res := NULL ;
			cov_res := NULL ;
			-- Call functions to chek files associated to A type rules
			ont_res := ont_res + f_cov_file_ontime(in_fileid, rules.coveragerule_id) ;
			gap_res := gap_res + f_cov_file_gapsatisfied(in_fileid,rules.coveragerule_id) ;
		END IF ;
		
		rel_found := 1 ;
	END LOOP ;
	
	-- If no relations was found set all to NULL
	IF rel_found = 0 THEN
		exp_res := NULL ;
		cov_res := NULL ;
		ont_res := NULL ;
		gap_res := NULL ;
	END IF ;
	
	-- Insert results into results table
	UPDATE temp_results SET
		t_exp_res = exp_res,
		t_cov_res = cov_res,
		t_ont_res = ont_res,
		t_gap_res = gap_res
	;
	
	-- Returning table
	RETURN QUERY SELECT * FROM temp_results ;
	DROP TABLE temp_results ;
	
END ;
$$ LANGUAGE plpgsql ;




