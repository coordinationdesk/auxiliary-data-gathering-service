
-- Version 1.0.0
-- POSTGRES





-- ########################################################################### --
-- ##################                                        ################# --
-- ##################                WARNING!!!              ################# --
-- ##################  This schema can be installed only on  ################# --
-- ##################      PDS 11.0.0 version or major       ################# --
-- ##################                                        ################# --
-- ########################################################################### --





---------------------------------------------------------------------------------
-------------------------------- TABLES CREATION --------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_cov_rules
*/ ------------------------------
DROP TABLE IF EXISTS t_cov_rules CASCADE ;

CREATE TABLE t_cov_rules (
	id_coveragerule SERIAL NOT NULL,
	rule_type CHAR(1) NOT NULL,
	time_0 TIMESTAMP(6) NOT NULL,
	t_sec INTEGER NOT NULL,
	int_a INTEGER DEFAULT 1 NOT NULL,
	int_b INTEGER DEFAULT 0 NOT NULL,
	n_min INTEGER DEFAULT 1 NOT NULL,
	n_max INTEGER DEFAULT 1 NOT NULL,
	delta_gap INTEGER DEFAULT 0 NOT NULL,
	cover_pct FLOAT DEFAULT 0 NOT NULL,
	check_delta_gap BOOLEAN DEFAULT 'f' NOT NULL,
	check_cover_pct BOOLEAN DEFAULT 'f' NOT NULL,
	deltawait_sec INTEGER DEFAULT 0 NOT NULL,
	deltamissing_sec INTEGER,
	creation_date TIMESTAMP(6) DEFAULT NOW() NOT NULL,
	last_update TIMESTAMP(6) DEFAULT NOW() NOT NULL,
	author VARCHAR(24) NOT NULL,
	description VARCHAR(128)
) ;

ALTER TABLE t_cov_rules 
	ADD CONSTRAINT pk_t_cov_rules
		PRIMARY KEY (id_coveragerule) ;

ALTER TABLE t_cov_rules 
	ADD CONSTRAINT ck_coveragerules01
		CHECK (rule_type IN ('A', 'B', 'C')) ;

ALTER TABLE t_cov_rules 
	ADD CONSTRAINT ck_coveragerules02
		CHECK (int_a > int_b) ;

ALTER TABLE t_cov_rules 
	ADD CONSTRAINT ck_coveragerules03
		CHECK ((n_min > 0) AND (n_max > 0)) ;

ALTER TABLE t_cov_rules 
	ADD CONSTRAINT ck_coveragerules04
		CHECK ((cover_pct >= 0) AND (cover_pct <= 100)) ;

CREATE INDEX ix_t_cov_rules01 ON t_cov_rules USING BTREE (last_update) ;

GRANT SELECT, INSERT, UPDATE, DELETE ON TABLE t_cov_rules TO pdsuser ;
GRANT USAGE, SELECT, UPDATE ON SEQUENCE t_cov_rules_id_coveragerule_seq TO pdsuser ;



/* ------------------------------
	t_cov_x_filetypes
*/ ------------------------------
DROP TABLE IF EXISTS t_cov_x_filetypes CASCADE ;

CREATE TABLE t_cov_x_filetypes (
	coveragerule_id INTEGER NOT NULL,
	filetype_id INTEGER NOT NULL
) ;

ALTER TABLE t_cov_x_filetypes 
	ADD CONSTRAINT pk_t_cov_x_filetypes
		PRIMARY KEY (coveragerule_id, filetype_id) ;

ALTER TABLE t_cov_x_filetypes 
	ADD CONSTRAINT fk_coverage_x_filetypes01
		FOREIGN KEY (coveragerule_id) REFERENCES t_cov_rules (id_coveragerule) 
			ON DELETE CASCADE ;

ALTER TABLE t_cov_x_filetypes 
	ADD CONSTRAINT fk_coverage_x_filetypes02
		FOREIGN KEY (filetype_id) REFERENCES t_filetypes (id_filetype) 
			ON DELETE CASCADE ;

CREATE INDEX ix_t_cov_x_filetypes01 ON t_cov_x_filetypes USING BTREE (coveragerule_id) ;
CREATE INDEX ix_t_cov_x_filetypes02 ON t_cov_x_filetypes USING BTREE (filetype_id) ;

GRANT SELECT, INSERT, UPDATE, DELETE ON TABLE t_cov_x_filetypes TO pdsuser ;



/* ------------------------------
	t_cov_periods
*/ ------------------------------
DROP TABLE IF EXISTS t_cov_periods CASCADE ;

CREATE TABLE t_cov_periods (
	id_interval BIGSERIAL NOT NULL,
	coveragerule_id INTEGER,
	t_begin TIMESTAMP(6) NOT NULL,
	t_end TIMESTAMP(6) NOT NULL,
	exp_val_start TIMESTAMP NOT NULL,
	exp_val_stop TIMESTAMP NOT NULL,
	events INTEGER DEFAULT 0 NOT NULL,
	cover_pct FLOAT DEFAULT 0 NOT NULL,
	--expect_value SMALLINT DEFAULT 2 NOT NULL,
	creationdate TIMESTAMP(6) DEFAULT NOW() NOT NULL,
	reported BOOLEAN DEFAULT 'f' NOT NULL
) ;

ALTER TABLE t_cov_periods 
	ADD CONSTRAINT pk_t_cov_periods
		PRIMARY KEY (id_interval) ;

--ALTER TABLE t_cov_periods 
--	ADD CONSTRAINT ck_cov_periods01
--		CHECK (expect_value IN ('0', '1', '2')) ;

ALTER TABLE t_cov_periods 
	ADD CONSTRAINT fk_cov_periods01
		FOREIGN KEY (coveragerule_id) REFERENCES t_cov_rules (id_coveragerule) 
			ON DELETE CASCADE ;

CREATE INDEX ix_t_cov_periods01 ON t_cov_periods USING BTREE (coveragerule_id) ;
CREATE INDEX ix_t_cov_periods02 ON t_cov_periods USING BTREE (creationdate) ;

GRANT SELECT, INSERT, UPDATE, DELETE ON TABLE t_cov_periods TO pdsuser ;
GRANT USAGE, SELECT, UPDATE ON SEQUENCE t_cov_periods_id_interval_seq TO pdsuser ;



/* ------------------------------
	t_cov_relevancies
*/ ------------------------------
DROP TABLE IF EXISTS t_cov_relevancies CASCADE ;

CREATE TABLE t_cov_relevancies (
	id_relevancy BIGSERIAL NOT NULL,
	period_id BIGINT NOT NULL,
	inv_id BIGINT NOT NULL,
	inv_val_start TIMESTAMP NOT NULL,
	inv_val_stop TIMESTAMP NOT NULL,
	creationdate TIMESTAMP(6) DEFAULT NOW() NOT NULL
) ;

ALTER TABLE t_cov_relevancies 
	ADD CONSTRAINT pk_t_cov_relevancies
		PRIMARY KEY (id_relevancy) ;

ALTER TABLE t_cov_relevancies 
	ADD CONSTRAINT fk_cov_relevancies01
		FOREIGN KEY (period_id) REFERENCES t_cov_periods (id_interval) 
			ON DELETE CASCADE ;

ALTER TABLE t_cov_relevancies 
	ADD CONSTRAINT fk_cov_relevancies02
		FOREIGN KEY (inv_id) REFERENCES t_inventory (id_inv) 
			ON DELETE CASCADE ;

CREATE INDEX ix_t_cov_relevancies01 ON t_cov_relevancies USING BTREE (period_id) ;
CREATE INDEX ix_t_cov_relevancies02 ON t_cov_relevancies USING BTREE (inv_id) ;

GRANT SELECT, INSERT, UPDATE, DELETE ON TABLE t_cov_relevancies TO pdsuser ;
GRANT USAGE, SELECT, UPDATE ON SEQUENCE t_cov_relevancies_id_relevancy_seq TO pdsuser ;








---------------------------------------------------------------------------------
------------------------------- FUNCTIONS CREATION ------------------------------
---------------------------------------------------------------------------------
/* ------------------------------
	t_dbparameters
*/ ------------------------------
DELETE FROM t_dbparameters WHERE parameter = 'cov_ondemand_del_threshold' ;
INSERT INTO t_dbparameters (parameter, parametertype_id, value_int) VALUES ('cov_ondemand_del_threshold', 1, 86400) ;








---------------------------------------------------------------------------------
------------------------------- FUNCTIONS CREATION ------------------------------
---------------------------------------------------------------------------------

/*
CREATE OR REPLACE FUNCTION my_function (my_arg INTEGER) RETURNS text[][] AS
$$

DECLARE
   my_arr VARCHAR[][];
   r_row RECORD;
   counter INTEGER := 1;
   
BEGIN
   FOR r_row IN SELECT filename, fileversion FROM t_inventory LOOP
       my_arr[counter] := array[r_row.filename,r_row.fileversion];
       counter := counter + 1;
   END LOOP;
   
   RETURN my_arr;
END;
$$
LANGUAGE plpgsql;
*/



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
	IF (inv_val_start < period_val_start AND (inv_val_stop BETWEEN period_val_start AND period_val_stop))
		OR (inv_val_stop > period_val_stop AND (inv_val_start BETWEEN period_val_start AND period_val_stop))
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
	period_id_val BIGINT ;
	period_id_inv BIGINT ;
	delta_w INTERVAL ;
	perc_cvr INTEGER ;

BEGIN
	-- Get and cast to interval the value of deltawait_sec of the rule
	delta_w := (SELECT CAST(deltawait_sec AS VARCHAR)::INTERVAL FROM t_cov_rules WHERE id_coveragerule = in_ruleid) ;
	
	-- Get the inventorydate, validitystart and validitystop values of the file to check
	SELECT inventorydate, validitystart, validitystop INTO STRICT inv_time, inv_val_start, inv_val_stop FROM t_inventory WHERE id_inv = in_fileid ;
	
	-- ##### VALIDITY TIME CHEK #####
	SELECT id_interval INTO STRICT period_id_val FROM t_cov_periods 			
	WHERE coveragerule_id = in_ruleid
		AND (exp_val_start - delta_w ) < inv_val_start
		AND (exp_val_stop + delta_w ) > inv_val_stop
	;
	
	-- ##### INVENTORY TIME CHEK #####
	SELECT id_interval INTO STRICT period_id_inv FROM t_cov_periods
	WHERE coveragerule_id = in_ruleid
		AND inv_time BETWEEN t_begin AND t_end
	;
	
	-- ##### PERIODS UPDATE #####
	-- Update events and coverage percentage
	IF period_id_val = period_id_inv THEN
		DELETE FROM t_cov_relevancies WHERE inv_id = in_fileid ;
		INSERT INTO t_cov_relevancies (
			period_id,
			inv_id,
			inv_val_start,
			inv_val_stop)
		VALUES (
			period_id_inv,
			in_fileid,
			inv_val_start,
			inv_val_stop
		);
		
		UPDATE t_cov_periods SET events = (events + 1), cover_pct = f_cov_percentage_calculator(period_id_inv) WHERE id_interval = period_id_inv ;
		
		RETURN 1 ;
		
	END IF ;
	
	RETURN 0 ;
	
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








---------------------------------------------------------------------------------
------------------------------ PROCEDURES CREATION ------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	p_cov_periods_planner
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_cov_periods_planner(
	in_past INTEGER,
	in_future INTEGER,
	in_hold INTEGER,
	in_ruleid INTEGER
	-- If in_ruleid is not 0, will be replaced only the periods for the specific rule
	-- else, if in_ruleid is 0 will be replanned periods for all rules
) RETURNS INTEGER AS $$
	
DECLARE
	curr_tstamp TIMESTAMP ;
	first_period TIMESTAMP ;
	period_begin TIMESTAMP ;
	period_end TIMESTAMP ;
	plan_period_sec BIGINT ;
	interval_length INTEGER ;
	tot_periods_num INTEGER ;
	first_period_num INTEGER ;
	in_past_casted INTERVAL ;
	in_hold_casted INTERVAL ;
	in_future_casted INTERVAL ;
	val_interval_a INTERVAL ;
	val_interval_b INTERVAL ;
	ex_msg VARCHAR(128) ;
	r_id_rec RECORD ;
	r_id_arr INTEGER[] ;
	cnt INTEGER ;
	
BEGIN
	-- Check for in_ruleid input value (can't be a NULL value)
	IF in_ruleid IS NULL THEN
		RAISE EXCEPTION 'p_cov_periods_planner: value of in_ruleid can not be NULL.' ;
		RETURN 1 ;
	END IF ;
	
	-- Check if in_future is greater then in_hold
	IF in_future < in_hold THEN
		RAISE EXCEPTION 'p_cov_periods_planner: the value of in_future must be greater then the value of in_hold.' ;
		RETURN 1 ;
	END IF ;
	
	-- Set current timestamp
	curr_tstamp := now() ;
	
	-- Cast numbers to interval for datetime operations
	in_past_casted := CAST(in_past AS VARCHAR)::INTERVAL ;
	in_hold_casted := CAST(in_hold AS VARCHAR)::INTERVAL ;
	in_future_casted := CAST(in_future AS VARCHAR)::INTERVAL ;
	
	-- Fix the cnt value to 0
	-- cnt is the counter used to fill the array
	-- the array will contain the id of the rules
	cnt := 0 ;
	
	IF in_ruleid != 0 THEN -- If it's a specific rule
		-- Check if rule exists
		-- If there aren't rules having the specific coveragerule_id will be raised an exception
		IF (SELECT count(*) FROM t_cov_rules WHERE id_coveragerule = in_ruleid) = 0 THEN
			ex_msg := 'p_cov_periods_planner: coverage rule ' || in_ruleid || ' do not exist.' ;
			RAISE EXCEPTION '%', ex_msg ;
			RETURN 1 ;
		END IF;	
		-- Check if rule is a C type
		-- If there aren't rules having the specific coveragerule_id will be raised an exception
		IF (SELECT rule_type FROM t_cov_rules WHERE id_coveragerule = in_ruleid) = 'C' THEN
			ex_msg := 'p_cov_periods_planner: type C rules cannot be planned' ;
			RAISE EXCEPTION '%', ex_msg ;
			RETURN 1 ;
		END IF;	
		-- Fill the array with the in_ruleid value
		r_id_arr[0] := in_ruleid ;
		-- Clean old periods only for the specific rule
		DELETE FROM t_cov_periods WHERE coveragerule_id = in_ruleid AND t_end < (curr_tstamp - in_past_casted) ;
	ELSE
		-- Fill the array with all id_coveragerule found in t_cov_rules
		FOR r_id_rec IN (
			SELECT id_coveragerule FROM t_cov_rules WHERE rule_type != 'C') 
		LOOP
			r_id_arr[cnt] := r_id_rec.id_coveragerule ;
			cnt := cnt + 1 ;
		END LOOP ;
		-- Clean old periods for all rules
		DELETE FROM t_cov_periods WHERE t_end < (curr_tstamp - in_past_casted) ;
	END IF ;
	
	
	FOR i IN 0..(array_length(r_id_arr, 1) - 1) LOOP
		-- Clean periods that will be replanned
		DELETE FROM t_cov_periods WHERE coveragerule_id = r_id_arr[i] AND t_begin > (curr_tstamp + in_hold_casted) ;
		
		-- Set the first period to start
		-- If is the first plannig for a rule or if there are not records (cause delete) for the rule,
		-- the first period will be the datetime selected into time_0 of the rule
		IF (SELECT count(*) FROM t_cov_periods WHERE coveragerule_id = r_id_arr[i]) = 0 THEN
			first_period := (SELECT time_0 FROM t_cov_rules WHERE id_coveragerule = r_id_arr[i]) ;
		-- Else, if there are records into t_cov_periods, the first new period will be the last t_end
		-- found (the "holded" one) in the t_cov_periods for the rule
		ELSE
			first_period := (SELECT MAX(t_begin) FROM t_cov_periods WHERE coveragerule_id = r_id_arr[i]) ;
		END IF ;
		
		-- Select the interval length of the period from the t_sec filed in the t_cov_rules table.
		interval_length := (SELECT t_sec FROM t_cov_rules WHERE id_coveragerule = r_id_arr[i]) ;
		
		-- Calculate the period to plan in seconds
		-- The plan_period_sec will be the delta between the first_period found and the future value
		plan_period_sec := CAST(EXTRACT(epoch FROM (date_trunc('hour', curr_tstamp) + in_future_casted) - first_period) AS INTEGER);
		
		-- Calculate the number of periods to create dividing the total planning period by the length of intervals
		tot_periods_num := plan_period_sec / interval_length ;
		
		-- 
		first_period_num := trunc(EXTRACT(epoch FROM ((curr_tstamp + in_hold_casted) - first_period ) / interval_length)) ;
		
		-- 
		period_begin := first_period + ((CAST(interval_length AS VARCHAR)::INTERVAL) * (first_period_num + 1)) ;
		
		-- Select the values int_a and int_b to calculate the expected validity start and stop (for coverage) of the period
		SELECT CAST(int_a AS VARCHAR)::INTERVAL, CAST(int_b AS VARCHAR)::INTERVAL 
			INTO val_interval_a, val_interval_b 
				FROM t_cov_rules WHERE id_coveragerule = r_id_arr[i] ;
		
		-- Recreate periods 
		FOR c IN first_period_num+1..tot_periods_num LOOP
			
			period_end := period_begin + CAST(interval_length AS VARCHAR)::INTERVAL ;
			
			INSERT INTO t_cov_periods (
				coveragerule_id,
				t_begin,
				t_end,
				exp_val_start,
				exp_val_stop)
			VALUES (
				r_id_arr[i],
				period_begin,
				period_end,
				period_end - val_interval_a,
				period_end - val_interval_b
			);
			
			period_begin := period_end ;
		END LOOP ;
		
	END LOOP ;
	
	RETURN 0;
	
END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	p_cov_periods_generator
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_cov_periods_generator(
	in_replan INTEGER,
	in_wait INTEGER,
	in_past INTEGER,
	in_future INTEGER,
	in_hold INTEGER
) RETURNS INTEGER AS $$

DECLARE
	curr_tstamp TIMESTAMP ;
	replan_interval INTERVAL ;
	wait_interval INTERVAL ;
	max_creation_date TIMESTAMP ;
	max_last_update TIMESTAMP ;

BEGIN
	-- Check if in_future is greater then in_hold
	IF in_future < in_hold THEN
		RAISE EXCEPTION 'p_cov_periods_generator: the value of in_future must be greater then the value of in_hold.' ;
		RETURN 1 ;
	END IF ;
	
	-- Set current timestamp
	curr_tstamp := now() ;
	
	-- Cast numbers to intervals for datetime operations
	replan_interval := CAST(in_replan AS VARCHAR)::INTERVAL ;
	wait_interval := CAST(in_wait AS VARCHAR)::INTERVAL ;
	
	-- Select max_creation_date and max_last_update values
	max_creation_date := (SELECT max(creationdate) FROM t_cov_periods) ;
	max_last_update := (SELECT max(last_update) FROM t_cov_rules) ;
	
	IF (max_creation_date < (curr_tstamp - replan_interval)) 
		OR ((max_last_update + wait_interval) > curr_tstamp AND (max_creation_date < max_last_update))
		OR max_creation_date IS NULL -- if is the firs time
	THEN
		-- Call procedure to plan periods
		PERFORM p_cov_periods_planner(in_past, in_future, in_hold, 0) ;
	END IF ;
	
	RETURN 0 ;
	
END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	p_cov_ondemand_event
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_cov_ondemand_event(
	in_ftype_id INTEGER,
	in_exp_time TIMESTAMP,
	in_d_wait INTEGER,
	in_d_miss INTEGER,
	in_exp_vstart TIMESTAMP,
	in_exp_vstop TIMESTAMP,
	in_auth VARCHAR,
	in_desc VARCHAR
) RETURNS INTEGER AS $$

DECLARE
	on_dem_thre INTEGER ;
	ins_res INTEGER ;

BEGIN
	-- Get the cov_ondemand_del_threshold value
	SELECT value_int INTO on_dem_thre FROM t_dbparameters WHERE parameter = 'cov_ondemand_del_threshold' ;
	
	-- Check the cov_ondemand_del_threshold validity
	IF on_dem_thre IS NULL THEN
		RAISE EXCEPTION 'p_cov_ondemand_event: cov_ondemand_del_threshold parameter not setted into t_dbparameters.' ;
		RETURN 1 ;
	END IF ;
	
	-- Insert new rule into t_cov_rules and return the new id into ins_res varable.
	INSERT INTO t_cov_rules (
		rule_type,time_0,t_sec,int_a,int_b,n_min,n_max,delta_gap,cover_pct,deltawait_sec,deltamissing_sec,author,description)
	VALUES (
		'C',in_exp_time,0,0,-1,1,1,0,0,in_d_wait,in_d_miss,in_auth,in_desc) 
	RETURNING
		id_coveragerule INTO ins_res ;
	
	-- If rule was created
	IF ins_res IS NOT NULL THEN
		-- Create relation between filetype and rule
		INSERT INTO t_cov_x_filetypes VALUES (ins_res, in_ftype_id) ;
		
		-- Create relative period
		INSERT INTO t_cov_periods (
			coveragerule_id,
			t_begin,
			t_end,
			exp_val_start,
			exp_val_stop)
		VALUES (
			ins_res,
			in_exp_time,
			in_exp_time,
			in_exp_vstart,
			in_exp_vstop );
		
		-- Remove expired existing C rules
		-- The 'on delete cascade' relations will remove records on t_cov_periods and t_cov_x_filetypes
		DELETE FROM t_cov_rules WHERE rule_type = 'C' AND EXTRACT(epoch FROM (now() - time_0))::INTEGER > on_dem_thre ;
		
	ELSE
		RAISE EXCEPTION 'p_cov_ondemand_event: error during rule creation.' ;
		RETURN 1 ;
	END IF ;
	
	RETURN 0 ;
	
END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
--------------------------- TRIGGER FUNCTIONS CREATION --------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	tf_cov_rules_last_update
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_cov_rules_last_update() RETURNS TRIGGER AS $$

BEGIN
	NEW.last_update := now() ;
	
  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;


/* ------------------------------
	tf_cov_relevancies_clean
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_cov_relevancies_clean() RETURNS TRIGGER AS $$

BEGIN
	DELETE FROM t_cov_relevancies WHERE creationdate < (NOW() - INTERVAL '1 WEEKS');
	
  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
------------------------------- TRIGGERS CREATION -------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_cov_rules
*/ ------------------------------
CREATE TRIGGER tr_cov_rules_update BEFORE UPDATE
	ON t_cov_rules 
		FOR EACH ROW EXECUTE PROCEDURE tf_cov_rules_last_update() ;


/* ------------------------------
	t_cov_relevancies
*/ ------------------------------
CREATE TRIGGER tr_cov_relevancies_insert AFTER INSERT
	ON t_cov_relevancies 
		FOR EACH ROW EXECUTE PROCEDURE tf_cov_relevancies_clean() ;








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
		'coverage-1.0.0',
		'2013-04-19',
		5,
		25,
		'COVERAGE_1-0-0',
		'Created coverage schema.'
) ;


