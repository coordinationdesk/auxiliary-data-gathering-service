/*

Copyright 2010-2020, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S. Databases schema procedures script $

	$Id$

	$Author$

	$Log$
	Revision 1.4  2014/01/09 09:35:11  achnat
	Fixed bug on p_cov_periods_generator.
	
	Revision 1.3  2013/11/29 16:38:24  achnat
	Fixed bug on p_cov_periods_planner.
	
	Revision 1.2  2013/10/10 09:29:27  achnat
	Created schema for COVERAGE_1-0-0
	
	Revision 1.1.1.1  2013/10/08 15:07:46  achnat
	psqlSchemaCOVERAGE
	
*/





--------------------------------------------------------------------------------
--
--    	  	  	  	  	  	  	  PROCEDURES
--
--------------------------------------------------------------------------------

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
	fix_val_a TIMESTAMP ;
	fix_val_b TIMESTAMP ;
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
		
		-- Select the values of int_a and int_b fields to calculate the expected validity start and stop (for coverage) of the period
		SELECT CAST(int_a AS VARCHAR)::INTERVAL, CAST(int_b AS VARCHAR)::INTERVAL 
			INTO val_interval_a, val_interval_b 
				FROM t_cov_rules WHERE id_coveragerule = r_id_arr[i] ;
		-- Select the values of fixedvaliditystart and fixedvaliditystop fields for files having fixed validity start or fixed validitystop
		SELECT fixedvaliditystart, fixedvaliditystop 
			INTO fix_val_a, fix_val_b 
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
				CASE
					WHEN fix_val_a IS NOT NULL THEN fix_val_a
					ELSE period_end - val_interval_a
				END,
				CASE
					WHEN fix_val_b IS NOT NULL THEN fix_val_b
					ELSE period_end - val_interval_b
				END
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
		OR ((max_last_update + wait_interval) < curr_tstamp AND (max_creation_date <= max_last_update))
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




