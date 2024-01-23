--
-- PostgreSQL database dump
--

-- Dumped from database version 13.4
-- Dumped by pg_dump version 13.4

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

--
-- Name: adgsdb; Type: DATABASE; Schema: -; Owner: pdsuser
--

--CREATE DATABASE adgsdb WITH TEMPLATE = template0 ENCODING = 'UTF8' LOCALE = 'en_US.UTF-8';
CREATE DATABASE adgsdb WITH TEMPLATE = template0 ENCODING = 'UTF8' LC_COLLATE = 'en_US.UTF-8' LC_CTYPE = 'en_US.UTF-8';


ALTER DATABASE adgsdb OWNER TO pdsuser;

\connect adgsdb

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

--
-- Name: lo; Type: EXTENSION; Schema: -; Owner: -
--

CREATE EXTENSION IF NOT EXISTS lo WITH SCHEMA public;


--
-- Name: EXTENSION lo; Type: COMMENT; Schema: -; Owner: 
--

COMMENT ON EXTENSION lo IS 'Large Object maintenance';


--
-- Name: postgis; Type: EXTENSION; Schema: -; Owner: -
--

CREATE EXTENSION IF NOT EXISTS postgis WITH SCHEMA public;


--
-- Name: EXTENSION postgis; Type: COMMENT; Schema: -; Owner: 
--

COMMENT ON EXTENSION postgis IS 'PostGIS geometry and geography spatial types and functions';


--
-- Name: uuid-ossp; Type: EXTENSION; Schema: -; Owner: -
--

CREATE EXTENSION IF NOT EXISTS "uuid-ossp" WITH SCHEMA public;


--
-- Name: EXTENSION "uuid-ossp"; Type: COMMENT; Schema: -; Owner: 
--

COMMENT ON EXTENSION "uuid-ossp" IS 'generate universally unique identifiers (UUIDs)';


--
-- Name: f_compare_int_values(integer, integer); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.f_compare_int_values(val1 integer, val2 integer) RETURNS boolean
    LANGUAGE plpgsql
    AS $$
BEGIN
	IF (val1 IS NULL AND val2 IS NULL) THEN
		RETURN TRUE ;
	ELSIF (val1 = val2) THEN
		RETURN TRUE ;
	ELSE
		RETURN FALSE ;
	END IF ;
END ;
$$;


ALTER FUNCTION public.f_compare_int_values(val1 integer, val2 integer) OWNER TO acsdba;

--
-- Name: f_cov_check_file(bigint); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.f_cov_check_file(in_fileid bigint) RETURNS TABLE(w_exp_res integer, w_cov_res integer, w_ont_res integer, w_gap_res integer)
    LANGUAGE plpgsql
    AS $$

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
$$;


ALTER FUNCTION public.f_cov_check_file(in_fileid bigint) OWNER TO acsdba;

--
-- Name: f_cov_file_cover(bigint, integer); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.f_cov_file_cover(in_fileid bigint, in_ruleid integer) RETURNS integer
    LANGUAGE plpgsql
    AS $$

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
$$;


ALTER FUNCTION public.f_cov_file_cover(in_fileid bigint, in_ruleid integer) OWNER TO acsdba;

--
-- Name: f_cov_file_expect(bigint, integer); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.f_cov_file_expect(in_fileid bigint, in_ruleid integer) RETURNS integer
    LANGUAGE plpgsql
    AS $$

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
$$;


ALTER FUNCTION public.f_cov_file_expect(in_fileid bigint, in_ruleid integer) OWNER TO acsdba;

--
-- Name: f_cov_file_gapsatisfied(bigint, integer); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.f_cov_file_gapsatisfied(in_fileid bigint, in_ruleid integer) RETURNS integer
    LANGUAGE plpgsql
    AS $$

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
$$;


ALTER FUNCTION public.f_cov_file_gapsatisfied(in_fileid bigint, in_ruleid integer) OWNER TO acsdba;

--
-- Name: f_cov_file_ontime(bigint, integer); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.f_cov_file_ontime(in_fileid bigint, in_ruleid integer) RETURNS integer
    LANGUAGE plpgsql
    AS $$

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
$$;


ALTER FUNCTION public.f_cov_file_ontime(in_fileid bigint, in_ruleid integer) OWNER TO acsdba;

--
-- Name: f_cov_percentage_calculator(bigint); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.f_cov_percentage_calculator(in_period_id bigint) RETURNS integer
    LANGUAGE plpgsql
    AS $$

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
$$;


ALTER FUNCTION public.f_cov_percentage_calculator(in_period_id bigint) OWNER TO acsdba;

--
-- Name: f_delete_inventory_tree(character varying, character varying); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.f_delete_inventory_tree(fname character varying, fversion character varying) RETURNS integer
    LANGUAGE plpgsql
    AS $$

DECLARE
	res INT ;
	p_dirid INT8;
	
BEGIN
	res := 0 ;
	p_dirid := (SELECT dir_id FROM t_inventory WHERE filename = fname AND fileversion = fversion);
	
	IF ( p_dirid IS NULL)
	THEN
		DELETE FROM t_inventory WHERE filename = fname AND fileversion = fversion;
		GET DIAGNOSTICS res = ROW_COUNT;
	ELSE
		DELETE FROM t_inventory WHERE ref_dir_id = p_dirid OR dir_id = p_dirid;
		GET DIAGNOSTICS res = ROW_COUNT;
	END IF;

	RETURN res ;

END ;
$$;


ALTER FUNCTION public.f_delete_inventory_tree(fname character varying, fversion character varying) OWNER TO acsdba;

--
-- Name: f_get_inv_id_from_name(character varying); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.f_get_inv_id_from_name(in_file_name character varying) RETURNS bigint
    LANGUAGE plpgsql
    AS $$

BEGIN
	IF EXISTS (SELECT FROM information_schema.tables WHERE table_name = 't_inventory')
	THEN
		RETURN (SELECT id_inv FROM t_inventory WHERE filename = in_file_name) ;
	END IF ;
	
	IF EXISTS (SELECT FROM information_schema.tables WHERE table_name = 't_srv_inventory')
	THEN
		RETURN (SELECT id FROM t_srv_inventory WHERE name = in_file_name) ;
	END IF ;
	
	EXCEPTION WHEN OTHERS THEN
	   RETURN -1 ;

END ;
$$;


ALTER FUNCTION public.f_get_inv_id_from_name(in_file_name character varying) OWNER TO acsdba;

--
-- Name: f_get_tasktable_id(character varying); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.f_get_tasktable_id(tt_name character varying) RETURNS integer
    LANGUAGE plpgsql
    AS $$
DECLARE
	res INT ;
BEGIN

  	res := (SELECT id_tasktable FROM t_tasktables WHERE name = tt_name) ;
  	RETURN res ;

	EXCEPTION WHEN OTHERS THEN
	   RETURN -1 ;

END ;
$$;


ALTER FUNCTION public.f_get_tasktable_id(tt_name character varying) OWNER TO acsdba;

--
-- Name: f_get_version_number(character varying); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.f_get_version_number(in_cvs_tag character varying) RETURNS integer
    LANGUAGE plpgsql
    AS $$

DECLARE
	mj_vrs CHAR(3) ;
	md_vrs CHAR(3) ;
	mn_vrs CHAR(3) ;

BEGIN
  	mj_vrs := TO_CHAR((SPLIT_PART(SUBSTRING(in_cvs_tag FROM (POSITION('_' IN in_cvs_tag)+1) FOR (CHAR_LENGTH(in_cvs_tag))), '-', 1)::INT), '00') ;
	md_vrs := TO_CHAR((SPLIT_PART(in_cvs_tag, '-', 2)::INT), '00') ;
	mn_vrs := TO_CHAR((SPLIT_PART(in_cvs_tag, '-', 3)::INT), '00') ;
	
	RETURN (TRIM(mj_vrs) || TRIM(md_vrs) || TRIM(mn_vrs))::INT ;
END ;
$$;


ALTER FUNCTION public.f_get_version_number(in_cvs_tag character varying) OWNER TO acsdba;

--
-- Name: f_interval_to_seconds(interval); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.f_interval_to_seconds(in_intrv interval) RETURNS double precision
    LANGUAGE plpgsql
    AS $$

DECLARE
	p_sec DOUBLE PRECISION ;

BEGIN
	p_sec := EXTRACT(EPOCH  FROM in_intrv) ;
	
	RETURN p_sec ;
END ;
$$;


ALTER FUNCTION public.f_interval_to_seconds(in_intrv interval) OWNER TO acsdba;

--
-- Name: f_interval_to_seconds_diff(timestamp without time zone, timestamp without time zone); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.f_interval_to_seconds_diff(beg_ts timestamp without time zone, end_ts timestamp without time zone) RETURNS double precision
    LANGUAGE plpgsql
    AS $$

DECLARE
	p_sec DOUBLE PRECISION ;

BEGIN
	p_sec := EXTRACT(EPOCH  FROM (end_ts - beg_ts)) ;
	
	RETURN p_sec ;
END ;
$$;


ALTER FUNCTION public.f_interval_to_seconds_diff(beg_ts timestamp without time zone, end_ts timestamp without time zone) OWNER TO acsdba;

--
-- Name: f_sor_generation(); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.f_sor_generation() RETURNS character
    LANGUAGE plpgsql
    AS $$

DECLARE
	currYear TIMESTAMP ;
	currResId CHAR(3) ;
	nextResId CHAR(3) ;

BEGIN
	-- Find and set the current year
	currYear := (SELECT TO_TIMESTAMP(TO_CHAR(extract(year FROM current_date), '9999'), 'YYYY')) ;
	
	-- Find and set the max requestid for the current year
	currResId := (SELECT SUBSTRING((SELECT MAX(requestid) FROM t_sor_requests WHERE raiseddate BETWEEN currYear AND (currYear + interval '1' year)) FROM 6 FOR 3)) ;
	-- If there are not records found, the first currResId will be 000
	IF currResId IS NULL THEN
		currResId := '000' ;
	END IF ;
	
	-- Set the next requestid
	nextResId := (SELECT TRIM(TO_CHAR((TO_NUMBER(currResId, '999') +1), '000'))) ;
	
	-- Insert the new record into t_sor_requests table
	INSERT INTO t_sor_requests (
		requestid,
		status,
		emailtitle)
	VALUES (
		TO_CHAR(currYear, 'YYYY-') || nextResId,
		'<in_progress>',
		'<in_progress>'
	);
		
	RETURN TO_CHAR(currYear, 'YYYY-') || nextResId ;
	
END ;
$$;


ALTER FUNCTION public.f_sor_generation() OWNER TO acsdba;

--
-- Name: p_application_events(character varying, character varying, character varying); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_application_events(in_appname character varying, in_evn_type character varying, in_evn_desc character varying) RETURNS integer
    LANGUAGE plpgsql
    AS $$
	
DECLARE
	p_app_id INTEGER := NULL ;
	p_exp_days INTEGER := NULL ;
	
BEGIN
	-- ##### APPLICATION SECTION ##### --
	-- Get the id of the application
	SELECT id_application INTO p_app_id FROM t_applications WHERE app_name = in_appname ;
	
	-- If the application does not exist
	IF p_app_id IS NULL THEN
		-- insert new application into t_applications and get is id
		INSERT INTO t_applications (app_name) VALUES (in_appname) RETURNING id_application INTO p_app_id ;
	END IF ;
	
	-- ##### EVENT SECTION ##### --
	-- Insert new event into t_applicationevents
	INSERT INTO t_applicationevents (
		application_id,
		tstamp,
		event_type,
		event_description)
	VALUES (
		p_app_id,
		now(),
		in_evn_type,
		in_evn_desc) ;
	
	-- ##### CLEANING SECTION ##### --
	-- Clean t_applicationevents removing events olther than app_even_expir_days (t_dbparameters)
	
	-- get the value of app_even_expir_days
	SELECT value_int INTO p_exp_days FROM t_dbparameters WHERE parameter = 'app_even_expir_days' ;
	
	-- if the value is not setted
	IF p_exp_days IS NULL THEN
		-- raise error message
		RAISE EXCEPTION 'p_application_events: Cannot retrieve value for app_even_expir_days db_parameter.' ;
	END IF ;
	-- else... delete
	DELETE FROM t_applicationevents WHERE
		application_id = p_app_id
		AND tstamp < (now() - CAST(p_exp_days || 'days' AS interval)) ;
		
	RETURN 0;
	
END ;
$$;


ALTER FUNCTION public.p_application_events(in_appname character varying, in_evn_type character varying, in_evn_desc character varying) OWNER TO acsdba;

--
-- Name: p_carts_update_status(integer); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_carts_update_status(in_cartid integer) RETURNS integer
    LANGUAGE plpgsql
    AS $$

DECLARE
	p_extorderid VARCHAR(32);
	ord_total INT;
	dis_total INT;
	mid_status VARCHAR(32) ;

BEGIN
	
	p_extorderid := (SELECT extref FROM t_cartsref WHERE id = in_cartid ) ;
	
	ord_total := (SELECT count(*) FROM t_ordersqueue WHERE externalorderid = p_extorderid AND subreq IS NOT NULL);
	dis_total := (SELECT count(*) FROM t_distributionitems WHERE cartid = in_cartid);


	-- ORDERS ERROR
	IF (SELECT count(status) FROM t_ordersqueue WHERE externalorderid = p_extorderid AND subreq IS NOT NULL AND status = 'ERROR') > 0 
	THEN
		UPDATE t_cartsref SET status = 'ERROR' WHERE id = in_cartid ;
		RETURN 0;
	END IF;
	
	-- ORDERS ABORTED
	IF (SELECT count(status) FROM t_ordersqueue WHERE externalorderid = p_extorderid AND subreq IS NOT NULL AND status = 'ABORTED') > 0 
	THEN
		UPDATE t_cartsref SET status = 'ABORTED' WHERE id = in_cartid ;
		RETURN 0;
	END IF;
	
	IF (ord_total = 0) THEN
	-- No orders, so act as every order has completed
		mid_status := 'COMPLETED' ;

	-- ORDERS ELIGIBLE
	ELSIF (SELECT count(status) FROM t_ordersqueue WHERE externalorderid = p_extorderid AND subreq IS NOT NULL AND status = 'ELIGIBLE') < ord_total 
	THEN
		-- ORDERS COMPLETED
		IF (SELECT count(status) FROM t_ordersqueue WHERE externalorderid = p_extorderid AND status = 'COMPLETED') < ord_total 
		THEN
			UPDATE t_cartsref SET status = 'PROGRESS' WHERE id = in_cartid ;
			RETURN 0;
		ELSE	
			mid_status := 'COMPLETED' ;
		END IF ;
	ELSE 
		mid_status := 'ELIGIBLE' ;
	END IF ;



	-- DISTRIBUTION ERROR
	IF (SELECT count(di.id) FROM t_distributionitems AS di
		INNER JOIN t_distributionqueue AS dq ON di.packageid = dq.packageid
		WHERE di.cartid = in_cartid AND dq.status = 'ERROR') > 0 
	THEN
		UPDATE t_cartsref SET status = 'ERROR' WHERE id = in_cartid ;
		RETURN 0;
	END IF;

	-- DISTRIBUTION ELIGIBLE
	IF (SELECT count(di.id) FROM t_distributionitems AS di
		INNER JOIN t_distributionqueue AS dq ON di.packageid = dq.packageid
		WHERE di.cartid = in_cartid AND dq.status = 'ELIGIBLE') < dis_total 
	THEN
		-- DISTRIBUTION COMPLETED
		IF (SELECT count(di.id) FROM t_distributionitems AS di
			INNER JOIN t_distributionqueue AS dq ON di.packageid = dq.packageid
			WHERE di.cartid = in_cartid AND dq.status = 'COMPLETED') < dis_total
		THEN
			UPDATE t_cartsref SET status = 'PROGRESS' WHERE id = in_cartid ;
			RETURN 0;
		ELSE
			UPDATE t_cartsref SET status = mid_status WHERE id = in_cartid ;
			RETURN 0;
		END IF;
	ELSE 
		UPDATE t_cartsref SET status = 'ELIGIBLE' WHERE id = in_cartid ;
		RETURN 0;
	END IF ;

	RETURN 0;

END ;
$$;


ALTER FUNCTION public.p_carts_update_status(in_cartid integer) OWNER TO acsdba;

--
-- Name: p_cartsinv_update(integer, character varying); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_cartsinv_update(in_packageid integer, in_status character varying) RETURNS integer
    LANGUAGE plpgsql
    AS $$

DECLARE
	p_cartid INTEGER;
	ret INTEGER;

BEGIN
	ret := 0 ;
	p_cartid := (SELECT DISTINCT cartid FROM t_distributionitems WHERE packageid = in_packageid);
	
	IF p_cartid IS NOT NULL THEN		
		UPDATE t_cartsinv SET status = in_status WHERE cart = p_cartid AND distsubreq IN (SELECT subreq FROM t_distributionitems WHERE packageid = in_packageid);
		ret := p_carts_update_status(p_cartid);
	END IF ;
	
	RETURN ret;
	
END ;
$$;


ALTER FUNCTION public.p_cartsinv_update(in_packageid integer, in_status character varying) OWNER TO acsdba;

--
-- Name: p_cartsinvcheck(integer, integer, integer, integer, integer, integer); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_cartsinvcheck(pre_id integer, pre_cart integer, pre_subreq integer, pre_distsubreq integer, pre_procsubreq integer, pre_stndsubreq integer) RETURNS integer
    LANGUAGE plpgsql
    AS $$

BEGIN

	IF (pre_distsubreq != 0) THEN 
		IF ( ( SELECT count(*) FROM t_cartsinv WHERE id != pre_id  AND cart = pre_cart AND pre_distsubreq = distsubreq ) > 0 ) THEN 
			RAISE EXCEPTION 'distribution subrequest already present for cart in t_cartsinv ' ;
			RETURN 1;
		END IF ;
	
	END IF ;

	IF (pre_procsubreq != 0) THEN 
		IF ( ( SELECT count(*) FROM t_cartsinv WHERE id != pre_id  AND cart = pre_cart AND pre_procsubreq = ordsubreq ) > 0 ) THEN 
			RAISE EXCEPTION 'processing subrequest already present for cart in t_cartsinv' ;
			RETURN 1;
		END IF ;
	
	END IF ;

	IF (pre_stndsubreq != 0) THEN 
		IF ( ( SELECT count(*) FROM t_cartsinv WHERE id != pre_id  AND cart = pre_cart AND pre_stndsubreq = stndsubreq ) > 0 ) THEN 
			RAISE EXCEPTION 'standing subrequest already present for cart in t_cartsinv ' ;
			RETURN 1;
		END IF ;
	
	END IF ;
	
	RETURN 0;

END ;
$$;


ALTER FUNCTION public.p_cartsinvcheck(pre_id integer, pre_cart integer, pre_subreq integer, pre_distsubreq integer, pre_procsubreq integer, pre_stndsubreq integer) OWNER TO acsdba;

--
-- Name: p_change_circ_policy_status(integer, smallint, boolean, integer); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_change_circ_policy_status(pol_id integer, post_status_id smallint, change_creation_date boolean DEFAULT true, new_priority integer DEFAULT 0) RETURNS integer
    LANGUAGE plpgsql
    AS $$

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
$$;


ALTER FUNCTION public.p_change_circ_policy_status(pol_id integer, post_status_id smallint, change_creation_date boolean, new_priority integer) OWNER TO acsdba;

--
-- Name: p_checksatsensmiss(smallint, smallint, smallint); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_checksatsensmiss(in_sat smallint, in_sen smallint, in_mis smallint) RETURNS integer
    LANGUAGE plpgsql
    AS $$
	
DECLARE
	cnt INTEGER ;
	
BEGIN
	cnt := 0 ;
	
	IF in_sat IS NULL AND in_sen IS NULL AND in_mis IS NULL THEN
		RETURN 0 ;
	END IF ;
		
	IF in_sen IS NULL AND in_mis IS NULL THEN
		SELECT INTO cnt COUNT(*) FROM t_satrelations WHERE satelliteid = in_sat ;
	ELSIF in_sat IS NULL AND in_mis IS NULL THEN
		SELECT INTO cnt COUNT(*) FROM t_satrelations WHERE sensorid = in_sen ;
	ELSIF in_sat IS NULL AND in_sen IS NULL THEN
		SELECT INTO cnt COUNT(*) FROM t_satrelations WHERE mission = in_mis ;
	ELSIF in_mis IS NULL THEN
		SELECT INTO cnt COUNT(*) FROM t_satrelations WHERE satelliteid = in_sat AND sensorid = in_sen ;
	ELSIF in_sat IS NULL THEN
		SELECT INTO cnt COUNT(*) FROM t_satrelations WHERE sensorid = in_sen AND mission = in_mis ;
	ELSIF in_sen IS NULL THEN
		SELECT INTO cnt COUNT(*) FROM t_satrelations WHERE satelliteid = in_sat AND mission = in_mis ;
	ELSE
		SELECT INTO cnt COUNT(*) FROM t_satrelations WHERE satelliteid = in_sat AND sensorid = in_sen AND mission = in_mis ;
	END IF ;
	
	IF (cnt = 0) THEN
		RAISE EXCEPTION 'p_checksatsensmiss: Inconsistent Satellite/Sensor/Mission' ;
	END IF ;
	
	RETURN 0;

END ;
$$;


ALTER FUNCTION public.p_checksatsensmiss(in_sat smallint, in_sen smallint, in_mis smallint) OWNER TO acsdba;

--
-- Name: p_circulationcheckstartstatus(smallint); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_circulationcheckstartstatus(in_stat_id smallint) RETURNS integer
    LANGUAGE plpgsql
    AS $$

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
$$;


ALTER FUNCTION public.p_circulationcheckstartstatus(in_stat_id smallint) OWNER TO acsdba;

--
-- Name: p_circulationresetretries(bigint, smallint, smallint); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_circulationresetretries(in_circ_id bigint, in_pre_stat_id smallint, in_post_stat_id smallint) RETURNS integer
    LANGUAGE plpgsql
    AS $$

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
$$;


ALTER FUNCTION public.p_circulationresetretries(in_circ_id bigint, in_pre_stat_id smallint, in_post_stat_id smallint) OWNER TO acsdba;

--
-- Name: p_circulationseterrortype(bigint); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_circulationseterrortype(in_circ_id bigint) RETURNS integer
    LANGUAGE plpgsql
    AS $$

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
$$;


ALTER FUNCTION public.p_circulationseterrortype(in_circ_id bigint) OWNER TO acsdba;

--
-- Name: p_completed_order(integer); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_completed_order(in_orderid integer) RETURNS integer
    LANGUAGE plpgsql
    AS $$
BEGIN
	INSERT INTO t_completed_orders VALUES (in_orderid);
	RETURN 0;
END ;
$$;


ALTER FUNCTION public.p_completed_order(in_orderid integer) OWNER TO acsdba;

--
-- Name: p_completed_order_distribution(integer); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_completed_order_distribution(in_orderid integer) RETURNS integer
    LANGUAGE plpgsql
    AS $$

DECLARE
  	row_odd RECORD ;
  	
	-- the procedure distributes the output from a completed order
	
BEGIN
	--
	--	check if the users are already assigned to that order looking into t_ondemanddistribution
	--
	IF ((SELECT COUNT(*) FROM t_ondemanddistribution WHERE orderid = in_orderid) = 0) THEN
	--
	--	no predefined user : proceed with the configured ones
	--
		PERFORM p_distribute_order_inventory(in_orderid, NULL) ;
		RETURN 0 ;
	END IF ;
	
	--
	--	a set of users was found: distribute items to that users according to their distribution policies
	--
	FOR row_odd IN (
		SELECT 
			us.id_user AS user_id, 
			odd.format AS format_id
		FROM 
			t_ondemanddistribution odd 
			INNER JOIN t_users us
				ON odd.userid = us.userid
		WHERE 
			odd.orderid = in_orderid) LOOP

  	  	PERFORM p_distribute_item_on_demand(
			inv.filename, 
			inv.fileversion, 
			ft.id_filetype, 
			row_odd.user_id, 
			(SELECT id_fileclass FROM t_fileclasses WHERE t_fileclasses.fileclass = inv.fileclass),
			row_odd.format_id, 
			in_orderid)
		FROM 
			t_inventory inv 
			INNER JOIN t_filetypes ft
				ON inv.filetype = ft.filetype
		WHERE 
			inv.orderid = in_orderid
		;
		
  	END LOOP ;
	
	RETURN 0 ;
END ;
$$;


ALTER FUNCTION public.p_completed_order_distribution(in_orderid integer) OWNER TO acsdba;

--
-- Name: p_cov_ondemand_event(integer, timestamp without time zone, integer, integer, timestamp without time zone, timestamp without time zone, character varying, character varying); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_cov_ondemand_event(in_ftype_id integer, in_exp_time timestamp without time zone, in_d_wait integer, in_d_miss integer, in_exp_vstart timestamp without time zone, in_exp_vstop timestamp without time zone, in_auth character varying, in_desc character varying) RETURNS integer
    LANGUAGE plpgsql
    AS $$

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
$$;


ALTER FUNCTION public.p_cov_ondemand_event(in_ftype_id integer, in_exp_time timestamp without time zone, in_d_wait integer, in_d_miss integer, in_exp_vstart timestamp without time zone, in_exp_vstop timestamp without time zone, in_auth character varying, in_desc character varying) OWNER TO acsdba;

--
-- Name: p_cov_periods_generator(integer, integer, integer, integer, integer); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_cov_periods_generator(in_replan integer, in_wait integer, in_past integer, in_future integer, in_hold integer) RETURNS integer
    LANGUAGE plpgsql
    AS $$

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
$$;


ALTER FUNCTION public.p_cov_periods_generator(in_replan integer, in_wait integer, in_past integer, in_future integer, in_hold integer) OWNER TO acsdba;

--
-- Name: p_cov_periods_planner(integer, integer, integer, integer); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_cov_periods_planner(in_past integer, in_future integer, in_hold integer, in_ruleid integer) RETURNS integer
    LANGUAGE plpgsql
    AS $$
	
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
$$;


ALTER FUNCTION public.p_cov_periods_planner(in_past integer, in_future integer, in_hold integer, in_ruleid integer) OWNER TO acsdba;

--
-- Name: p_cr_distribution(character varying, timestamp without time zone); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_cr_distribution(in_auth character varying, in_tstamp timestamp without time zone) RETURNS integer
    LANGUAGE plpgsql
    AS $$

BEGIN

	INSERT INTO t_temporarydistribution (
		duedate, 
		filename, 
		fileversion, 
		mediatype, 
		userid, 
		algo, 
		policy,
		format,
		disseminationpackingalgo_id )
	SELECT DISTINCT
		(NOW()::DATE + drl.maxdaytodeliver) + NOW()::TIME(6),
		inv.filename,
		inv.fileversion,
		mdt.mediatype,
		usr.userid,
		drl.distributionalgo_id,
		drl.id_distributionrule,
		drl.format_tt_id,
		drl.disseminationpackingalgo_id
	FROM
		t_crtemporaries crt
		INNER JOIN t_inventory inv
			ON crt.inventory_id = inv.id_inv
		INNER JOIN t_crrelations crr
			ON crt.crgroup_id = crr.crgroup_id
		INNER JOIN t_distributionrules drl
			ON crr.distributionrule_id = drl.id_distributionrule
			AND crr.repository_id = drl.repository_id
		INNER JOIN t_mediatypes mdt
			ON drl.mediatype_id = mdt.id_mediatype
		INNER JOIN t_users usr
			ON drl.user_id = usr.id_user
	WHERE 
		crt.author = in_auth 
		AND crt.tstamp = in_tstamp 
		AND drl.filetype_id = (
			SELECT id_filetype FROM t_filetypes WHERE filetype = inv.filetype)
	;
	
	INSERT INTO t_crhistory SELECT * FROM t_crtemporaries crt
		WHERE crt.author = in_auth AND crt.tstamp = in_tstamp ;
		
	DELETE FROM t_crtemporaries ct 
		WHERE author = in_auth AND tstamp = in_tstamp ;
	
	RETURN 0 ;

END ;
$$;


ALTER FUNCTION public.p_cr_distribution(in_auth character varying, in_tstamp timestamp without time zone) OWNER TO acsdba;

--
-- Name: p_datastoragequeue_update(bigint, smallint); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_datastoragequeue_update(in_cq_id bigint, in_cq_stat_id smallint) RETURNS integer
    LANGUAGE plpgsql
    AS $$

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
$$;


ALTER FUNCTION public.p_datastoragequeue_update(in_cq_id bigint, in_cq_stat_id smallint) OWNER TO acsdba;

--
-- Name: p_deleteinventory(character varying, character varying); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_deleteinventory(filename character varying, fileversion character varying) RETURNS integer
    LANGUAGE plpgsql
    AS $$

BEGIN

  	RETURN 0;

END ;
$$;


ALTER FUNCTION public.p_deleteinventory(filename character varying, fileversion character varying) OWNER TO acsdba;

--
-- Name: p_distribute_item(character varying, character varying, integer, integer, integer, integer, integer, integer); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_distribute_item(in_fname character varying, in_fversion character varying, in_ftypeid integer, in_userid integer, in_fileclassid integer, in_format integer, in_orderid integer, in_duedays integer) RETURNS integer
    LANGUAGE plpgsql
    AS $$
	
DECLARE
	p_extordid 	VARCHAR(128);
	p_cartid   	INT;
	p_subreq   	INT;
	p_totsubreq	INT;
	p_chng_add	INT;
	
BEGIN
	p_chng_add := NULL ;
	
	p_extordid := (SELECT externalorderid FROM t_ordersqueue WHERE orderid = in_orderid);
	p_subreq := (SELECT subreq FROM t_ordersqueue WHERE orderid = in_orderid);
	
	IF p_extordid IS NOT NULL AND p_subreq IS NOT NULL 
	THEN
		p_cartid := (SELECT id FROM t_cartsref AS cr
				INNER JOIN t_ordersqueue AS oq
				ON oq.externalorderid = cr.extref
				WHERE oq.orderid = in_orderid);

		p_totsubreq := (SELECT max(c.distsubreq) +1 
				FROM t_cartsinv c 
				WHERE c.cart = p_cartid);
	ELSE
		p_cartid := NULL;
		p_totsubreq := NULL;
	END IF;


--	the input inventory file is distributed to each user who declared an interest on that file-type.
--	the distribution is done according to the active field in the distribution policy record
	
	INSERT INTO t_temporarydistribution (
		duedate, 
		filename, 
		fileversion, 
		mediatype, 
		userid, 
		format, 
		algo, 
		policy, 
		orderid, 
		cartid, 
		subreq,
		disseminationpackingalgo_id )
	SELECT DISTINCT
		CASE
			WHEN in_duedays IS NULL THEN (NOW()::DATE + dr.maxdaytodeliver) + NOW()::TIME(6)
			ELSE (NOW()::DATE + in_duedays) + NOW()::TIME(6)
		END,
		in_fname, 
		in_fversion,
		mt.mediatype,
		us.userid,
		dr.format_tt_id, 
		dr.distributionalgo_id, 
		dr.id_distributionrule, 
		in_orderid, 
		p_cartid, 
		p_totsubreq,
		dr.disseminationpackingalgo_id
	FROM t_distributionrules dr
		INNER JOIN t_users us
			ON dr.user_id = us.id_user
		INNER JOIN t_mediatypes mt
			ON dr.mediatype_id = mt.id_mediatype
	WHERE
		dr.user_id = in_userid
		AND (dr.filetype_id = in_ftypeid OR dr.filetype_id IN (SELECT pfiletype FROM t_filetypescomps WHERE cfiletype = in_ftypeid)) 
		AND dr.active = 't'
		AND dr.ondemand = 'f'
		AND (CASE WHEN in_format IS NOT NULL 
				THEN dr.format_tt_id = in_format 
				ELSE dr.format_tt_id = dr.format_tt_id 
			END)
		AND (f_compare_int_values(dr.fileclass_id, in_fileclassid) OR dr.fileclass_id IS NULL)
		AND dr.usermode_id = (SELECT mode FROM t_users WHERE id_user = in_userid)
	;
	
	GET DIAGNOSTICS p_chng_add = ROW_COUNT ;
		
	IF p_chng_add > 0 THEN
		UPDATE t_cartsinv SET distsubreq = p_totsubreq WHERE cart = p_cartid AND ordsubreq = p_subreq ;
	END IF ;
	
	RETURN 0;

END ;
$$;


ALTER FUNCTION public.p_distribute_item(in_fname character varying, in_fversion character varying, in_ftypeid integer, in_userid integer, in_fileclassid integer, in_format integer, in_orderid integer, in_duedays integer) OWNER TO acsdba;

--
-- Name: p_distribute_item_on_demand(character varying, character varying, integer, integer, integer, integer, integer); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_distribute_item_on_demand(in_fname character varying, in_fversion character varying, in_ftypeid integer, in_userid integer, in_fileclassid integer, in_format integer, in_orderid integer) RETURNS integer
    LANGUAGE plpgsql
    AS $$
	
DECLARE
	p_extordid		VARCHAR(128);
	p_cartid		INT;
	p_subreq		INT;
	p_totsubreq		INT;
	p_ondem_media	VARCHAR(16);
	p_chng_add		INT;
	
BEGIN
	p_chng_add := NULL ;

	p_extordid := (SELECT externalorderid FROM t_ordersqueue WHERE orderid = in_orderid);
	p_subreq := (SELECT subreq FROM t_ordersqueue WHERE orderid = in_orderid);
	p_ondem_media := (SELECT od.mediatype FROM t_ondemanddistribution od WHERE od.orderid = in_orderid AND od.userid = (SELECT userid FROM t_users WHERE id_user = in_userid)) ;
	
	
	IF p_extordid IS NOT NULL AND p_subreq IS NOT NULL 
	THEN
		p_cartid := (SELECT id FROM t_cartsref AS cr
					INNER JOIN t_ordersqueue AS oq
						ON oq.externalorderid = cr.extref
					WHERE oq.orderid = in_orderid);

		p_totsubreq := (SELECT max(c.distsubreq) +1 
					FROM t_cartsinv c 
					WHERE c.cart = p_cartid);
	ELSE
		p_cartid := NULL;
		p_totsubreq := NULL;
	END IF;


--	the input inventory file is distributed to each user who declared an interest on that file-type.
--	the distribution is done according to the active field in the distribution policy record
	
	INSERT INTO t_temporarydistribution (
		duedate, 
		filename, 
		fileversion, 
		mediatype, 
		userid, 
		format, 
		algo, 
		policy, 
		orderid, 
		cartid, 
		subreq,
		disseminationpackingalgo_id )
	SELECT DISTINCT
		(NOW()::DATE + dr.maxdaytodeliver) + NOW()::TIME(6),
		in_fname, 
		in_fversion,
		CASE 
			WHEN p_ondem_media IS NULL THEN mt.mediatype
			ELSE p_ondem_media
		END,
		us.userid,
		dr.format_tt_id, 
		dr.distributionalgo_id, 
		dr.id_distributionrule, 
		in_orderid, 
		p_cartid, 
		p_totsubreq,
		dr.disseminationpackingalgo_id
	FROM t_distributionrules dr
		INNER JOIN t_users us
			ON dr.user_id = us.id_user
		INNER JOIN t_mediatypes mt
			ON dr.mediatype_id = mt.id_mediatype
	WHERE
		dr.user_id = in_userid
		AND (dr.filetype_id = in_ftypeid OR dr.filetype_id IN (SELECT pfiletype FROM t_filetypescomps WHERE cfiletype = in_ftypeid)) 
		AND dr.active = 't'
		AND dr.ondemand = 't'
		AND (CASE WHEN in_format IS NOT NULL 
				THEN dr.format_tt_id = in_format 
				ELSE dr.format_tt_id = dr.format_tt_id 
			END)
		AND (f_compare_int_values(dr.fileclass_id, in_fileclassid) OR dr.fileclass_id IS NULL)
		AND dr.usermode_id = (SELECT mode FROM t_users WHERE id_user = in_userid)
		AND (p_ondem_media IS NULL OR dr.distributionalgo_id IN (SELECT id FROM t_distributionalgo WHERE on_demand = 't'))
	;
	
	GET DIAGNOSTICS p_chng_add = ROW_COUNT ;
		
	IF p_chng_add > 0 THEN
		UPDATE t_cartsinv SET distsubreq = p_totsubreq WHERE cart = p_cartid AND ordsubreq = p_subreq ;
	END IF ;
	
	RETURN 0;

END ;
$$;


ALTER FUNCTION public.p_distribute_item_on_demand(in_fname character varying, in_fversion character varying, in_ftypeid integer, in_userid integer, in_fileclassid integer, in_format integer, in_orderid integer) OWNER TO acsdba;

--
-- Name: p_distribute_order_inventory(integer, integer); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_distribute_order_inventory(in_orderid integer, in_fileclassid integer) RETURNS integer
    LANGUAGE plpgsql
    AS $$
	
BEGIN
--	this is the standard way to distribute the output of an order:
--	each inventory is distributed to users who defined a valid distribution rule for it

	IF in_orderid IS NOT NULL THEN
  	  	
		PERFORM p_distribute_item(
			iv.filename,
			iv.fileversion,
			dr.filetype_id,
			dr.user_id,
			(SELECT id_fileclass FROM t_fileclasses WHERE t_fileclasses.fileclass = iv.fileclass),
			dr.format_tt_id,
			iv.orderid,
			NULL)
		FROM 
			t_inventory AS iv
			INNER JOIN (SELECT id_filetype, filetype FROM t_filetypes) AS ft
				ON ft.filetype = iv.filetype
			INNER JOIN t_distributionrules AS dr
				ON (dr.filetype_id = ft.id_filetype 
					OR dr.filetype_id IN (SELECT pfiletype FROM t_filetypescomps WHERE cfiletype = ft.id_filetype))
		WHERE
			iv.orderid = in_orderid
			AND dr.active = 't'
			AND dr.ondemand = 'f'
			AND ((f_compare_int_values(dr.fileclass_id, (SELECT id_fileclass FROM t_fileclasses WHERE t_fileclasses.fileclass = iv.fileclass)) OR dr.fileclass_id IS NULL))
		;
  	END IF ;
	
	RETURN 0;

END ;
$$;


ALTER FUNCTION public.p_distribute_order_inventory(in_orderid integer, in_fileclassid integer) OWNER TO acsdba;

--
-- Name: p_events(character varying, character varying); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_events(in_table character varying, in_action character varying) RETURNS integer
    LANGUAGE plpgsql
    AS $$
	
DECLARE
	event_file VARCHAR(64) ;
	ret INTEGER;
	
BEGIN

--  NOT IMPLEMENTED IN THIS SCHEMA
--  To implement this procedure you have to install the Events DataBlade.

	RETURN 0;
	
END ;
$$;


ALTER FUNCTION public.p_events(in_table character varying, in_action character varying) OWNER TO acsdba;

--
-- Name: p_fake(); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_fake() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
BEGIN
	RETURN NULL;
END ;
$$;


ALTER FUNCTION public.p_fake() OWNER TO acsdba;

--
-- Name: p_firtsdayoftheweek(timestamp without time zone); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_firtsdayoftheweek(day timestamp without time zone) RETURNS timestamp without time zone
    LANGUAGE plpgsql
    AS $$
DECLARE
	res TIMESTAMP(6) ;
	tmpdate DATE ;
	tmptime TIME(6) ;
BEGIN

  	tmpdate := day ;
	tmptime := day ;
	res := tmpdate - p_WeekDay(day);
  	res := res + tmptime ;

	RETURN res ;

END ;
$$;


ALTER FUNCTION public.p_firtsdayoftheweek(day timestamp without time zone) OWNER TO acsdba;

--
-- Name: p_generatetmporder(boolean, character varying, character varying, character varying, character varying, smallint); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_generatetmporder(in_active boolean, fname character varying, fversion character varying, ftype character varying, fclass character varying, in_satellite smallint) RETURNS integer
    LANGUAGE plpgsql
    AS $$
	
BEGIN
	
	INSERT INTO t_temporaryorders (
		filename, 
		fileversion, 
		filetype, 
		ordertype, 
		processorname,
		processorversion, 
		algorithm, 
		processorconfiguration, 
		priority,
		troubleshooting, 
		processingtimewindow, 
		ordersat)
  	SELECT
		fname, 
		fversion, 
		ftype, 
		og.orderType, 
		og.processorName,
		og.processorVersion, 
		og.algorithm, 
		og.processorconfiguration, 
		og.priority,
		og.troubleshooting, 
		og.processingtimewindow, 
		in_satellite
	FROM 
		t_ordersgeneration og
	WHERE 
		og.filetype = ftype 
		AND og.active = in_active 
		AND (og.fileclass IS NULL OR og.fileclass = fclass)
	;
	
	RETURN 0;

END ;
$$;


ALTER FUNCTION public.p_generatetmporder(in_active boolean, fname character varying, fversion character varying, ftype character varying, fclass character varying, in_satellite smallint) OWNER TO acsdba;

--
-- Name: p_generatetmporderpname(boolean, character varying, character varying, character varying, character varying, smallint); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_generatetmporderpname(in_active boolean, fname character varying, fversion character varying, ftype character varying, fclass character varying, in_satellite smallint) RETURNS integer
    LANGUAGE plpgsql
    AS $$
	
BEGIN
	
	INSERT INTO t_temporaryorders (
		filename, 
		fileversion, 
		filetype, 
		ordertype, 
		processorname,
		processorversion, 
		algorithm, 
		processorconfiguration, 
		priority,
		troubleshooting, 
		processingtimewindow, 
		ordersat)
  	SELECT
		fname, 
		fversion, 
		ftype, 
		og.orderType, 
		og.processorName,
		og.processorVersion, 
		og.algorithm, 
		og.processorconfiguration, 
		og.priority,
		og.troubleshooting, 
		og.processingtimewindow, 
		in_satellite
	FROM 
		t_ordersgeneration og
	WHERE 
		og.filetype = ftype 
		AND og.active = in_active 
		AND (og.fileclass IS NULL OR og.fileclass = fclass)
		AND og.processorname IN (SELECT pname FROM tempPnames)
	;
	
	RETURN 0;

END ;
$$;


ALTER FUNCTION public.p_generatetmporderpname(in_active boolean, fname character varying, fversion character varying, ftype character varying, fclass character varying, in_satellite smallint) OWNER TO acsdba;

--
-- Name: p_generatetmporderpname2(boolean, character varying, character varying, character varying, character varying, smallint, character varying, character varying); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_generatetmporderpname2(in_active boolean, fname character varying, fversion character varying, ftype character varying, fclass character varying, in_satellite smallint, tab_name character varying, fld_name character varying) RETURNS integer
    LANGUAGE plpgsql
    AS $$

DECLARE
	sql_cmd VARCHAR(4000) ;
	
BEGIN
	sql_cmd := '' ;
	sql_cmd := sql_cmd || 'INSERT INTO t_temporaryorders (' ;
	sql_cmd := sql_cmd || 	'filename, ' ;
	sql_cmd := sql_cmd || 	'fileversion, ' ;
	sql_cmd := sql_cmd || 	'filetype, ' ;
	sql_cmd := sql_cmd || 	'ordertype, ' ;
	sql_cmd := sql_cmd || 	'processorname, ' ;
	sql_cmd := sql_cmd || 	'processorversion, '  ;
	sql_cmd := sql_cmd || 	'algorithm, ' ;
	sql_cmd := sql_cmd || 	'processorconfiguration, ' ;
	sql_cmd := sql_cmd || 	'priority, ' ;
	sql_cmd := sql_cmd || 	'troubleshooting, ' ;
	sql_cmd := sql_cmd || 	'processingtimewindow, ' ;
	sql_cmd := sql_cmd || 	'ordersat) ' ;
	sql_cmd := sql_cmd || 'SELECT ' ;
	sql_cmd := sql_cmd ||	'''' || fname || ''', ' ;
	sql_cmd := sql_cmd ||	'''' || fversion || ''', ' ;
	sql_cmd := sql_cmd ||	'''' || ftype || ''', ' ;
	sql_cmd := sql_cmd ||	'og.orderType, ' ;
	sql_cmd := sql_cmd ||	'og.processorName, ' ;
	sql_cmd := sql_cmd ||	'og.processorVersion, ' ;
	sql_cmd := sql_cmd ||	'og.algorithm, ' ;
	sql_cmd := sql_cmd ||	'og.processorconfiguration, ' ;
	sql_cmd := sql_cmd ||	'og.priority, ' ;
	sql_cmd := sql_cmd ||	'og.troubleshooting, ' ;
	sql_cmd := sql_cmd ||	'og.processingtimewindow, ' ;
	sql_cmd := sql_cmd ||	in_satellite || ' ' ;
	sql_cmd := sql_cmd || 'FROM ' ;
	sql_cmd := sql_cmd ||	't_ordersgeneration og ' ;
	sql_cmd := sql_cmd || 'WHERE ' ;
	sql_cmd := sql_cmd ||	'og.filetype = ' || '''' || ftype || ''' ' ;
	sql_cmd := sql_cmd ||	'AND og.active = BOOL(' || '''' || in_active || ''') ' ;
	sql_cmd := sql_cmd ||	'AND (og.fileclass IS NULL OR og.fileclass = ' || '''' || fclass || ''') ' ;
	sql_cmd := sql_cmd ||	'AND og.processorname IN (SELECT ' || fld_name || ' FROM ' || tab_name || ') ' ;
	
	EXECUTE sql_cmd ;
	
	RETURN 0;

END ;
$$;


ALTER FUNCTION public.p_generatetmporderpname2(in_active boolean, fname character varying, fversion character varying, ftype character varying, fclass character varying, in_satellite smallint, tab_name character varying, fld_name character varying) OWNER TO acsdba;

--
-- Name: p_insert_inv_data_restored(integer, bigint, integer); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_insert_inv_data_restored(ismid integer, invid bigint, smid integer) RETURNS integer
    LANGUAGE plpgsql
    AS $$

DECLARE
	isSync BOOLEAN ;
	asynchCount INTEGER ;
	parentsAsynchCount INTEGER ;
	insertedRows INTEGER := 0 ;
	
BEGIN
	-- Get the syncupload type of the SM for the new record to
	-- check if the new record has been inserted into a SYNCH repository
	SELECT s.syncupload INTO isSync 
	FROM t_storagemanager s INNER JOIN t_inventorysm i ON i.storagemanid = s.id
	WHERE i.id = ismId ;
	
	IF isSync IS TRUE THEN
		
		-- Check if there are other records having the same inv_id into asynch repositories
		SELECT count(*) INTO asynchCount FROM t_inventorysm i INNER JOIN t_storagemanager s ON i.storagemanid = s.id 
		WHERE i.inv_id = invId AND s.syncupload IS FALSE ;
		
		-- Check if there are other "parents" records having the same inv_id into asynch repositories
		SELECT count(*) INTO parentsAsynchCount FROM t_inventorysm i INNER JOIN t_storagemanager s ON i.storagemanid = s.id 
		WHERE i.inv_id = (SELECT inv_id2 FROM t_inv_relations WHERE inv_id1 = invId) AND s.syncupload IS FALSE ;
		
		-- If there is another record (or his parent) into an asynch repository insert a record into t_inv_data_restored
		IF (asynchCount > 0 OR parentsAsynchCount > 0) THEN
			INSERT INTO t_inv_data_restored (
				id,
				sm_id,
				inv_id,
				parent_inv_id,
				filetype_id,
				size)
			SELECT 
				ismId,
				smId,
				invId,
				r.inv_id2,
				f.id_filetype,
				CASE WHEN i.datasize IS NULL THEN 0 ELSE i.datasize END
			FROM
				t_inventory i
				LEFT JOIN t_inv_relations r ON i.id_inv = r.inv_id1
				INNER JOIN t_filetypes f ON i.filetype = f.filetype
			WHERE
				i.id_inv = invId ;
			
			GET DIAGNOSTICS insertedRows = ROW_COUNT ;
			
		END IF ;
	END IF ;
	
	RETURN insertedRows ;
	
END ;
$$;


ALTER FUNCTION public.p_insert_inv_data_restored(ismid integer, invid bigint, smid integer) OWNER TO acsdba;

--
-- Name: p_insert_inventoryevents(bigint, integer); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_insert_inventoryevents(in_invid bigint, in_storageid integer) RETURNS integer
    LANGUAGE plpgsql
    AS $$
	
DECLARE 
	p_fname VARCHAR(128);
	p_fversion VARCHAR(32);
	p_valflag BOOLEAN;
	p_event VARCHAR(16);
	sync_cnt INTEGER ;
	async_cnt INTEGER ;

BEGIN
	-- If there aren't configured filetypes to check... return
	IF (SELECT count(*) FROM t_inventoryeventsconf WHERE filetype = (SELECT filetype FROM t_inventory WHERE id_inv = in_invid)) = 0 THEN
		RETURN 0 ;
	END IF ;
	
	-- Get inventoru filename and fileversion
	SELECT filename, fileversion, validityflag INTO p_fname, p_fversion, p_valflag FROM t_inventory WHERE id_inv = in_invid ;
	
	-- If the validity flag is FALSE... return
	IF p_valflag = 'f' THEN
		RETURN 0 ;
	END IF ;
	
	-- Count if there are synchronous storages linked to this inventory
	SELECT count(*) INTO sync_cnt FROM t_inventorysm im INNER JOIN t_storagemanager sm ON im.storagemanid = sm.id
		WHERE im.inv_id = in_invid AND sm.syncdownload = 't' ;
	-- Count if there are asynchronous storages linked to this inventory
	SELECT count(*) INTO async_cnt FROM t_inventorysm im INNER JOIN t_storagemanager sm ON im.storagemanid = sm.id
		WHERE im.inv_id = in_invid AND sm.syncdownload = 'f' ;
	
	IF (async_cnt > 0 AND sync_cnt = 0) THEN
		p_event := 'OFFLINE' ;
	ELSIF sync_cnt > 0 THEN
		p_event := 'ONLINE' ;
	ELSE
		p_event := 'NOT AVAILABLE';
	END IF ;
	
	INSERT INTO t_inventoryevents (
		filename,
		fileversion,
		event ) 
	VALUES (
		p_fname, 
		p_fversion,
		p_event );

	RETURN 0;

END ;
$$;


ALTER FUNCTION public.p_insert_inventoryevents(in_invid bigint, in_storageid integer) OWNER TO acsdba;

--
-- Name: p_insert_inventoryevents(bigint, character varying, character varying, character varying, boolean, boolean, character); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_insert_inventoryevents(in_invid bigint, fname character varying, fversion character varying, ftype character varying, pre_flag boolean, post_flag boolean, operation character) RETURNS integer
    LANGUAGE plpgsql
    AS $$
	
DECLARE 
	p_fname VARCHAR(128);
	p_fversion VARCHAR(32);
	p_event VARCHAR(16);
	sync_cnt INTEGER ;
	async_cnt INTEGER ;

BEGIN
	-- Check the last input value validity
	IF operation NOT IN ('I','U','D') THEN
		RAISE EXCEPTION 'p_insert_inventoryevents (inventory): Invalid operation input value.' ;
		RETURN 1 ;
	END IF ;
	
	-- If there aren't configured filetypes to check... do nothing
	IF (SELECT count(*) FROM t_inventoryeventsconf WHERE filetype = ftype) = 0 THEN
		RETURN 0 ;
	END IF ;
	
	-- If is an insert operation write 'INSERT' and exit
	IF operation = 'I' THEN
		INSERT INTO t_inventoryevents (filename, fileversion, event ) VALUES (fname, fversion, 'INSERT');	
		RETURN 0;
	END IF ;
	
	-- If is a delete operation write 'DELETE' and exit
	IF operation = 'D' THEN
		INSERT INTO t_inventoryevents (filename, fileversion, event ) VALUES (fname, fversion, 'DELETE');	
		RETURN 0;
	END IF ;
	
	-- Get inventoru filename and fileversion
	SELECT filename, fileversion INTO p_fname, p_fversion FROM t_inventory WHERE id_inv = in_invid ;
	
	-- Flags Table:
	--/-----------------------------------------------------------/
	--| pre_flag | post_flag | event                              |
	--|----------|-----------|------------------------------------|
	--|   false  |   true    | NOT AVAILABLE or OFFLINE or ONLINE |
	--|   false  |   false   | return  (do nothing)               |
	--|   true   |   true    | UPDATE                             |
	--|   true   |   false   | INVALIDATED                        |
	--|----------|-----------|------------------------------------|

	IF (pre_flag = 'f' AND post_flag = 't') THEN
		-- Count if there are synchronous storages linked to this inventory
		SELECT count(*) INTO sync_cnt FROM t_inventorysm im INNER JOIN t_storagemanager sm ON im.storagemanid = sm.id
			WHERE im.inv_id = in_invid AND sm.syncdownload = 't' ;
		-- Count if there are asynchronous storages linked to this inventory
		SELECT count(*) INTO async_cnt FROM t_inventorysm im INNER JOIN t_storagemanager sm ON im.storagemanid = sm.id
			WHERE im.inv_id = in_invid AND sm.syncdownload = 'f' ;
		
		IF (async_cnt > 0 AND sync_cnt = 0) THEN
			p_event := 'OFFLINE' ;
		ELSIF sync_cnt > 0 THEN
			p_event := 'ONLINE' ;
		ELSE
			p_event := 'NOT AVAILABLE';
		END IF ;
	END IF ;

	IF (pre_flag = 'f' AND post_flag = 'f') THEN
		RETURN 0 ;
	END IF ;

	IF (pre_flag = 't' AND post_flag = 't') THEN
		p_event := 'UPDATE';
	END IF ;
	
	IF (pre_flag = 't' AND post_flag = 'f') THEN
		p_event := 'INVALIDATED';
	END IF ;
	

	INSERT INTO t_inventoryevents (
		filename,
		fileversion,
		event ) 
	VALUES (
		p_fname, 
		p_fversion,
		p_event );

	RETURN 0;

END ;
$$;


ALTER FUNCTION public.p_insert_inventoryevents(in_invid bigint, fname character varying, fversion character varying, ftype character varying, pre_flag boolean, post_flag boolean, operation character) OWNER TO acsdba;

--
-- Name: p_insert_temporary_distribution(timestamp without time zone, character varying, character varying, character varying, integer, integer, integer, integer, integer, integer); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_insert_temporary_distribution(in_duedate timestamp without time zone, in_fname character varying, in_fversion character varying, in_mtype character varying, in_format integer, in_algo integer, in_ordid integer, in_cartid integer, in_subreq integer, in_disspackalgo integer) RETURNS integer
    LANGUAGE plpgsql
    AS $$

BEGIN

	INSERT INTO t_temporarydistribution (
		duedate,
		filename,
		fileversion,
		mediatype,
		userid,
		format,
		algo,
		policy,
		orderid,
		cartid,
		subreq,
		disseminationpackingalgo_id )
	VALUES (
		in_duedate,
		in_fname,
		in_fversion,
		in_mtype,
		'ANONYMOUS',
		in_format,
		in_algo,
		(SELECT max(id_distributionrule) FROM t_distributionrules WHERE user_id = (
			SELECT id_user FROM t_users WHERE userid = 'ANONYMOUS')),
		in_ordid,
		in_cartid,
		in_subreq,
		in_disspackalgo
	) ;

	RETURN 0;

END ;
$$;


ALTER FUNCTION public.p_insert_temporary_distribution(in_duedate timestamp without time zone, in_fname character varying, in_fversion character varying, in_mtype character varying, in_format integer, in_algo integer, in_ordid integer, in_cartid integer, in_subreq integer, in_disspackalgo integer) OWNER TO acsdba;

--
-- Name: p_insert_test_inventories(character varying, character varying, integer); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_insert_test_inventories(in_fname character varying, in_ftype character varying, in_files_number integer) RETURNS integer
    LANGUAGE plpgsql
    AS $$

DECLARE 
	p_ftype VARCHAR(16);

BEGIN	
	--INPUT CHECK
	-- Check the name value
	IF in_fname IS NULL THEN
		RAISE EXCEPTION 'p_insert_test_inventories: in_fname input value cannot be NULL.' ;
		RETURN 1 ;
	END IF ;
	-- Check the files number value
	IF in_files_number <= 0 THEN
		RAISE EXCEPTION 'p_insert_test_inventories: in_files_number input value must be greater than 0.' ;
		RETURN 1 ;
	END IF ;
	
	-- INSERT OPERATIONS
	FOR counter IN 1..in_files_number LOOP
		-- Set filetype
		IF in_ftype IS NOT NULL THEN
			p_ftype := in_ftype ;
		ELSE
			-- If is not setted will be selected a random value
			SELECT filetype INTO p_ftype FROM t_filetypes ORDER BY random() LIMIT 1 ;
		END IF;
		
		-- Insert into t_inventory
		INSERT INTO t_inventory(
			filename, 
			fileversion, 
			generationtime, 
			validitystart, 
			validitystop, 
			validityflag, 
			validationdate, 
			filetype, 
			fileclass)
		VALUES (
			in_fname,
			(SELECT -- select the version of file and increase it
				CASE 
					WHEN max(fileversion) IS NULL THEN '0001'
					ELSE trim(to_char(to_number(max(fileversion), '9999') + 1, '0000')) 
				END 
			FROM t_inventory WHERE filename = in_fname),
			now(),
			now(),
			now() + interval '1' day,
			'f',
			now(),
			p_ftype,
			'TEST' );
	END LOOP ;
	
	RETURN 0;

END ;
$$;


ALTER FUNCTION public.p_insert_test_inventories(in_fname character varying, in_ftype character varying, in_files_number integer) OWNER TO acsdba;

--
-- Name: p_insert_test_ordersqueue(integer); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_insert_test_ordersqueue(in_files_number integer) RETURNS integer
    LANGUAGE plpgsql
    AS $$

DECLARE 
	p_procname VARCHAR(32);
	p_procvers VARCHAR(8);

BEGIN
	--INPUT CHECK
	-- Check the files number value
	IF in_files_number <= 0 THEN
		RAISE EXCEPTION 'p_insert_test_ordersqueue: in_files_number input value must be greater than 0.' ;
		RETURN 1 ;
	END IF ;
	
	-- INSERT OPERATIONS
	FOR counter IN 1..in_files_number LOOP
		-- Set processor
		SELECT processorname, processorversion INTO p_procname, p_procvers FROM t_processors ORDER BY random() LIMIT 1 ;
		
		-- Insert into t_ordersqueue
		INSERT INTO t_ordersqueue(
			creationdate, 
			priority, 
			status,
			processorname,
			processorversion,
			troubleshooting,
			starttime,
			stoptime,
			processingtimewindow,
			ordertype)
		VALUES (
			now(),
			1,
			'ABORTED',
			p_procname,
			p_procvers,
			'FALSE',
			now(),
			now() + interval '1' day,
			1,
			(SELECT ordertype FROM t_ordertypes ORDER BY random() LIMIT 1) );
	END LOOP ;
	
	RETURN 0;

END ;
$$;


ALTER FUNCTION public.p_insert_test_ordersqueue(in_files_number integer) OWNER TO acsdba;

--
-- Name: p_intervalminutes(timestamp without time zone, timestamp without time zone); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_intervalminutes(start timestamp without time zone, stop timestamp without time zone) RETURNS numeric
    LANGUAGE plpgsql
    AS $$
DECLARE
	res DECIMAL(8,2) ;
BEGIN

	res := EXTRACT(EPOCH FROM AGE(stop, start)) / 60.0;
    RETURN res;

	EXCEPTION WHEN OTHERS THEN
	   RETURN 0 ;

END ;
$$;


ALTER FUNCTION public.p_intervalminutes(start timestamp without time zone, stop timestamp without time zone) OWNER TO acsdba;

--
-- Name: p_intervalseconds(timestamp without time zone, timestamp without time zone); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_intervalseconds(start timestamp without time zone, stop timestamp without time zone) RETURNS bigint
    LANGUAGE plpgsql
    AS $$
DECLARE
	res BIGINT ;
BEGIN

	res := EXTRACT(EPOCH FROM AGE(stop, start)) ;
    RETURN res;

	EXCEPTION WHEN OTHERS THEN
	   RETURN -99999999 ;

END ;
$$;


ALTER FUNCTION public.p_intervalseconds(start timestamp without time zone, stop timestamp without time zone) OWNER TO acsdba;

--
-- Name: p_inv_dist_counter_by_items(character varying, character varying, character); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_inv_dist_counter_by_items(in_fname character varying, in_fversion character varying, operation character) RETURNS integer
    LANGUAGE plpgsql
    AS $$
	
DECLARE
	p_invid BIGINT ;
	
BEGIN
	-- Check operation input value validity
	IF operation NOT IN ('INS','DEL') THEN
		RAISE EXCEPTION 'p_inv_distributions_by_items: Invalid operation input value (INS/DEL).' ;
		RETURN 1 ;
	END IF ;
	
	-- Get the inventory ID
	SELECT id_inv INTO p_invid FROM t_inventory WHERE filename = in_fname AND fileversion = in_fversion ;
	
	IF operation = 'DEL' THEN
		UPDATE t_inv_distributions SET retain_counter = (retain_counter - 1) WHERE inv_id = p_invid AND retain_counter > 0 ;
	END IF ;
	
	IF operation = 'INS' THEN
		IF (SELECT count(*) FROM t_inv_distributions WHERE inv_id = p_invid) > 0 THEN
			UPDATE t_inv_distributions SET retain_counter = (retain_counter + 1) WHERE inv_id = p_invid ;
		ELSE
			INSERT INTO t_inv_distributions (inv_id, retain_counter) VALUES (p_invid, 1) ;
		END IF ;
	END IF ;
	
	RETURN 0;

END ;
$$;


ALTER FUNCTION public.p_inv_dist_counter_by_items(in_fname character varying, in_fversion character varying, operation character) OWNER TO acsdba;

--
-- Name: p_inv_dist_counter_by_queue(integer, character varying, character varying, character); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_inv_dist_counter_by_queue(in_packid integer, in_pre_status character varying, in_post_status character varying, operation character) RETURNS integer
    LANGUAGE plpgsql
    AS $$
	
BEGIN
	-- Check operation input value validity
	IF operation NOT IN ('UPD','DEL') THEN
		RAISE EXCEPTION 'p_inv_distributions_by_queue: Invalid operation input value (UPD/DEL).' ;
		RETURN 1 ;
	END IF ;
	
	IF operation = 'DEL' OR (operation = 'UPD' AND (in_pre_status != in_post_status AND in_post_status = 'COMPLETED')) THEN
		UPDATE 
			t_inv_distributions 
		SET 
			retain_counter = (retain_counter - 1) 
		WHERE 
			retain_counter > 0
			AND inv_id IN (
		SELECT
			iv.id_inv
		FROM
			t_inventory iv
			INNER JOIN t_distributionitems di
				ON iv.filename = di.filename AND iv.fileversion = di.fileversion
			INNER JOIN t_distributionqueue dq
				ON dq.packageid = di.packageid
		WHERE
			dq.packageid = in_packid );
	END IF ;
	
	RETURN 0;

END ;
$$;


ALTER FUNCTION public.p_inv_dist_counter_by_queue(in_packid integer, in_pre_status character varying, in_post_status character varying, operation character) OWNER TO acsdba;

--
-- Name: p_inv_x_baselines_insert(bigint); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_inv_x_baselines_insert(in_invid bigint) RETURNS integer
    LANGUAGE plpgsql
    AS $$

DECLARE
	bsl_curr_val_id INTEGER ;
	excp_msg VARCHAR(128) ;
	
BEGIN
	-- Check if the baseline variable is setted
	IF ((SELECT count(*) FROM t_dbparameters WHERE parameter = 'baseline_curr_val') = 0) THEN
		RAISE EXCEPTION 'p_inv_x_baselines_insert: no value setted for baseline_curr_val into t_dbparameters' ;
	END IF;
	
	-- Get the id of the baseline setted
	bsl_curr_val_id := (SELECT value_int FROM t_dbparameters WHERE parameter = 'baseline_curr_val') ;
	
	-- Check if the id is setted a valid id
	IF ((SELECT count(*) FROM t_inv_baselines WHERE id_inv_baseline = bsl_curr_val_id) = 0) THEN
		excp_msg := 'p_inv_x_baselines_insert: ' || bsl_curr_val_id || ' - invalid baseline_curr_val value into t_dbparameters' ;
		RAISE EXCEPTION '%', excp_msg ;
	END IF ;
	
	-- Insert the relation into t_inv_x_baselines
	INSERT INTO t_inv_x_baselines (
		inv_id,
		inv_baseline_id )
	VALUES (
		in_invid,
		bsl_curr_val_id
	) ;
	
	RETURN 0 ;
END ;
$$;


ALTER FUNCTION public.p_inv_x_baselines_insert(in_invid bigint) OWNER TO acsdba;

--
-- Name: p_inventory_manager(character varying, character varying, boolean, boolean, character varying, character varying, integer, smallint, smallint, smallint, timestamp without time zone, timestamp without time zone); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_inventory_manager(fname character varying, fversion character varying, prevalflag boolean, postvalflag boolean, ftype character varying, fclass character varying, orderid integer, in_satellite smallint, in_sensor smallint, in_mission smallint, valstart timestamp without time zone, valstop timestamp without time zone) RETURNS integer
    LANGUAGE plpgsql
    AS $$
	
DECLARE
  evnt VARCHAR(3) ;
  evnttype VARCHAR(8) ;
  
BEGIN
	-- Check for satellite-sensor-mission relation
  	PERFORM p_checksatsensmiss(in_satellite, in_sensor, in_mission) ;
	
  	-- If is called after an insert
  	IF prevalflag IS NULL THEN
		evnt := 'INS' ;
		evnttype := 'INSFTYPE' ;
		INSERT INTO t_pendinginventory (filename, fileversion) VALUES (fname, fversion) ;
	-- If is called after an update
	ELSE
		evnt := 'UPD' ;
		evnttype := 'UPDFTYPE' ;
	END IF ;
	
	-- If validity flag is, or became, TRUE
  	IF ((prevalflag IS NULL AND postvalflag = 't') OR (prevalflag <> postvalflag AND postvalflag = 't')) THEN
  	  	PERFORM p_processinventory(fname, fversion, postvalflag, ftype, fclass, orderid, in_satellite, valstart, valstop) ;
		UPDATE t_filetypes_last_inv SET last_valid = NOW() WHERE filetype = ftype ;
  	END IF ;

	----------------------------------------------------------------------------
	-- BROADCAST SECTION
	----------------------------------------------------------------------------
  	PERFORM p_events('t_inventory', evnt) ;
	PERFORM p_events(ftype, evnttype) ;
	
	RETURN 0;

END ;
$$;


ALTER FUNCTION public.p_inventory_manager(fname character varying, fversion character varying, prevalflag boolean, postvalflag boolean, ftype character varying, fclass character varying, orderid integer, in_satellite smallint, in_sensor smallint, in_mission smallint, valstart timestamp without time zone, valstop timestamp without time zone) OWNER TO acsdba;

--
-- Name: p_inventorylinks_check(character varying, character varying, character varying, integer); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_inventorylinks_check(fname character varying, fversion character varying, ftype character varying, ordid integer) RETURNS integer
    LANGUAGE plpgsql
    AS $$
	
DECLARE
	p_oftype VARCHAR(16) ;
	p_dftype VARCHAR(16) ;
	p_algoty VARCHAR(24) ;
	row RECORD ;
	err INT ;
	
BEGIN

	FOR row IN (
		SELECT orig_filetype, dest_filetype, algotype
		FROM t_inventorylinksconf
		WHERE orig_filetype = ftype ) LOOP
			
			p_oftype := row.orig_filetype ;
			p_dftype := row.dest_filetype ;
			p_algoty := row.algotype ;
			
			IF (p_algoty = 'LINKORDER') THEN
				err := p_inventorylinks_insert(fname, fversion, p_dftype, ordid, p_algoty) ;
			END IF;
			
			IF (p_algoty = 'LINKTEMPORIG') THEN
				err := p_inventorylinks_insert(fname, fversion, p_dftype, p_algoty) ;
			END IF;
			
			IF (p_algoty = 'LINKTEMPDEST') THEN
				err := p_inventorylinks_insert(fname, fversion, p_dftype, p_algoty) ;
			END IF;
	END LOOP ;
	
	RETURN err;

END ;
$$;


ALTER FUNCTION public.p_inventorylinks_check(fname character varying, fversion character varying, ftype character varying, ordid integer) OWNER TO acsdba;

--
-- Name: p_inventorylinks_insert(character varying, character varying, character varying, character varying); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_inventorylinks_insert(fname character varying, fversion character varying, destftype character varying, algo character varying) RETURNS integer
    LANGUAGE plpgsql
    AS $$

DECLARE
	p_destfname VARCHAR(128);
	p_destfvers VARCHAR(8);
	p_start TIMESTAMP(6);
	p_stop TIMESTAMP(6);
	row RECORD ;

BEGIN

	p_start := (SELECT validitystart FROM t_inventory WHERE filename = fname AND fileversion = fversion) ;
	p_stop := (SELECT validitystop FROM t_inventory WHERE filename = fname AND fileversion = fversion) ;
	
	IF algo = 'LINKTEMPORIG' THEN
		FOR row IN (
			SELECT filename, fileversion
			FROM t_inventory
			WHERE validitystart >= p_start AND validitystop <= p_stop AND validityflag = 't' AND filetype = destftype
			GROUP BY filename, fileversion ) LOOP

				p_destfname := row.filename ;
				p_destfvers := row.fileversion ;
					
				INSERT INTO t_inventorylinks (orig_filename, orig_fileversion, dest_filename, dest_fileversion, description)
				VALUES ( fname, fversion, p_destfname, p_destfvers, 'linked by ' || algo || 'algorithm');

		END LOOP ;
	END IF ;
	
	IF algo = 'LINKTEMPDEST' THEN
		FOR row IN (
			SELECT filename, fileversion
			FROM t_inventory
			WHERE validitystart <= p_start AND validitystop >= p_stop AND validityflag = 't' AND filetype = destftype
			GROUP BY filename, fileversion ) LOOP
				
				p_destfname := row.filename ;
				p_destfvers := row.fileversion ;
			
				INSERT INTO t_inventorylinks (orig_filename, orig_fileversion, dest_filename, dest_fileversion, description)
				VALUES ( p_destfname, p_destfvers, fname, fversion, 'linked by ' || algo || 'algorithm');
				
		END LOOP ;
	END IF ;
	
	RETURN 0;

END ;
$$;


ALTER FUNCTION public.p_inventorylinks_insert(fname character varying, fversion character varying, destftype character varying, algo character varying) OWNER TO acsdba;

--
-- Name: p_inventorylinks_insert(character varying, character varying, character varying, integer, character varying); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_inventorylinks_insert(fname character varying, fversion character varying, origftype character varying, ordid integer, algo character varying) RETURNS integer
    LANGUAGE plpgsql
    AS $$
	
DECLARE 
	p_destfname VARCHAR(128);
	p_destfvers VARCHAR(8);
	row RECORD ;
	
BEGIN

	FOR row IN (
		SELECT o.filename, o.fileversion
		FROM t_ordersinput AS o
		INNER JOIN t_inventory AS i ON i.orderid = o.orderid
		WHERE o.orderid = ordid AND i.filetype = origftype ) LOOP
		
			p_destfname := row.filename;
			p_destfvers := row.fileversion;

			INSERT INTO t_inventorylinks (orig_filename, orig_fileversion, dest_filename, dest_fileversion, description)
			VALUES ( fname, fversion, p_destfname, p_destfvers, 'linked by ' || algo || 'algorithm');
	END LOOP ;
	
	RETURN 0;

END ;
$$;


ALTER FUNCTION public.p_inventorylinks_insert(fname character varying, fversion character varying, origftype character varying, ordid integer, algo character varying) OWNER TO acsdba;

--
-- Name: p_monthoftheyear(timestamp without time zone); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_monthoftheyear(value timestamp without time zone) RETURNS integer
    LANGUAGE plpgsql
    AS $$
DECLARE
	res INTEGER ;
BEGIN

  	res := EXTRACT(MONTH FROM value) ;

	RETURN res;

END ;
$$;


ALTER FUNCTION public.p_monthoftheyear(value timestamp without time zone) OWNER TO acsdba;

--
-- Name: p_move_seq_to_max(character varying); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_move_seq_to_max(tb_name character varying) RETURNS integer
    LANGUAGE plpgsql
    AS $$

DECLARE
	fld_name VARCHAR(64) ;
	row_cnt INTEGER ;
	
BEGIN
	-- Get, if exists, the name of serial column
	SELECT column_name INTO fld_name FROM information_schema.columns
		WHERE table_name = tb_name AND column_default SIMILAR TO '%(nexval|_seq|regclass)%';

	-- Check if the table contains some rows 
	EXECUTE 'SELECT count(*) FROM ' || tb_name INTO row_cnt ;

	-- If the table has a serial column and has some records, ...
	IF (fld_name IS NOT NULL) AND (row_cnt > 0) THEN
		-- update the start value of the sequence to the max value contained into the table 
		EXECUTE 'SELECT pg_catalog.setval(' || '''' || tb_name || '_' || fld_name || '_seq' || '''' || ', (SELECT MAX(' || fld_name || ') FROM ' || tb_name || '), true)' ;
	END IF ;
	
	RETURN 0;
	
END ;
$$;


ALTER FUNCTION public.p_move_seq_to_max(tb_name character varying) OWNER TO acsdba;

--
-- Name: p_orderqueuecheckchanges(integer, character varying, character varying, character varying, character varying, character varying, character varying, character varying, character varying, timestamp without time zone, timestamp without time zone, timestamp without time zone, timestamp without time zone, character varying, character varying, smallint, smallint, smallint, smallint); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_orderqueuecheckchanges(pre_orderid integer, pre_status character varying, post_status character varying, pre_processorname character varying, post_processorname character varying, pre_processorversion character varying, post_processorversion character varying, pre_processorconfiguration character varying, post_processorconfiguration character varying, pre_starttime timestamp without time zone, post_starttime timestamp without time zone, pre_stoptime timestamp without time zone, post_stoptime timestamp without time zone, pre_ordertype character varying, post_ordertype character varying, pre_satelliteid smallint, post_satelliteid smallint, pre_mission smallint, post_mission smallint) RETURNS integer
    LANGUAGE plpgsql
    AS $$
	
BEGIN

	IF (pre_status != post_status) THEN
		RETURN 0;
	END IF;
	
	IF (pre_processorname != post_processorname OR
	    pre_processorversion != post_processorversion OR
	    pre_processorconfiguration != post_processorconfiguration OR
	    pre_starttime != post_starttime OR
	    pre_stoptime != post_stoptime OR
	    pre_ordertype != post_ordertype OR
	    pre_satelliteid != post_satelliteid OR
	    pre_mission != post_mission) 
	THEN
		DELETE FROM t_waitingorders WHERE orderid = pre_orderid;
	END IF;
	
	RETURN 0 ;
		
END ;
$$;


ALTER FUNCTION public.p_orderqueuecheckchanges(pre_orderid integer, pre_status character varying, post_status character varying, pre_processorname character varying, post_processorname character varying, pre_processorversion character varying, post_processorversion character varying, pre_processorconfiguration character varying, post_processorconfiguration character varying, pre_starttime timestamp without time zone, post_starttime timestamp without time zone, pre_stoptime timestamp without time zone, post_stoptime timestamp without time zone, pre_ordertype character varying, post_ordertype character varying, pre_satelliteid smallint, post_satelliteid smallint, pre_mission smallint, post_mission smallint) OWNER TO acsdba;

--
-- Name: p_orders_manager(integer, character varying, character varying); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_orders_manager(in_orderid integer, pre_status character varying, post_status character varying) RETURNS integer
    LANGUAGE plpgsql
    AS $$
	
DECLARE
  	p_extordid VARCHAR(128);
	p_subreq INT;
	p_cartid INT;
	cartsinv_status VARCHAR(32);
	
BEGIN
	p_extordid := (SELECT externalorderid FROM t_ordersqueue WHERE orderid = in_orderid);
	p_subreq := (SELECT subreq FROM t_ordersqueue WHERE orderid = in_orderid);
	
	IF p_extordid IS NOT NULL AND p_subreq IS NOT NULL THEN
		p_cartid := (SELECT id FROM t_cartsref AS cr INNER JOIN t_ordersqueue AS oq ON oq.externalorderid = cr.extref WHERE oq.orderid = in_orderid);
		
		IF post_status = 'COMPLETED' THEN
			cartsinv_status := 'PROCESSED';
		ELSE
			cartsinv_status := post_status ;
		END IF ;
		
		UPDATE t_cartsinv SET status = cartsinv_status WHERE cart = p_cartid AND ordsubreq = p_subreq ;	
		PERFORM p_carts_update_status(p_cartid);
	END IF ;
	
	RETURN 0 ;

END ;
$$;


ALTER FUNCTION public.p_orders_manager(in_orderid integer, pre_status character varying, post_status character varying) OWNER TO acsdba;

--
-- Name: p_orderstatetransitions_mgr(integer, character varying, character varying); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_orderstatetransitions_mgr(in_ordid integer, in_pre_stat character varying, in_post_stat character varying) RETURNS integer
    LANGUAGE plpgsql
    AS $$
	
DECLARE
  	prefix VARCHAR(4);
	comp_stat VARCHAR(8);
	
BEGIN
	IF in_pre_stat IS NULL THEN
		prefix = 'NUL' ;
	ELSIF in_pre_stat IS NOT NULL AND in_post_stat = 'ABORTED'  THEN
		prefix = '' ;
	ELSE
		prefix = SUBSTRING(in_pre_stat FROM 0 FOR 4) ;
	END IF ;
	
	comp_stat := prefix || '2' || SUBSTRING(in_post_stat FROM 0 FOR 4) ;
		
	IF comp_stat = 'ERR2COM' OR comp_stat = 'PRO2COM' THEN
		PERFORM p_completed_order(in_ordid) ;
	END IF ;
	
	-- The following transition is not logged to avoid too much negligible transitions ELIGIBLE->PRECONDCHEDCK->ELIGIBLE
	IF comp_stat = 'PRE2ELI' OR comp_stat = 'ELI2PRE' THEN
		RETURN 0 ;
	END IF ;
	
	-- The following transition is logged only the first time to avoid too much negligible transitions ELIGIBLE->PRECONDCHEDCK->ELIGIBLE
	IF (comp_stat = 'ELI2ELI') THEN 
		IF ((SELECT COUNT(*) FROM t_orderstatetransitions WHERE orderid = in_ordid) = 0) THEN
			INSERT INTO t_orderstatetransitions (orderid, status) VALUES (in_ordid, in_post_stat) ;
		END IF ;
		RETURN 0 ;
	END IF ;
	
	-- Otherwise
	INSERT INTO t_orderstatetransitions (orderid, status) VALUES (in_ordid, in_post_stat) ;
	PERFORM p_events('t_orderstatetransitions', comp_stat) ;
	
	RETURN 0;
	
END ;
$$;


ALTER FUNCTION public.p_orderstatetransitions_mgr(in_ordid integer, in_pre_stat character varying, in_post_stat character varying) OWNER TO acsdba;

--
-- Name: p_postinvqueue_insert(character varying, character varying); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_postinvqueue_insert(in_fname character varying, in_fversion character varying) RETURNS integer
    LANGUAGE plpgsql
    AS $$
	
BEGIN
	
	INSERT INTO t_postinvqueue (
		filename,
		fileversion,
		filetype,
		validitystart,
		validitystop)
	SELECT 
		i.filename,
		i.fileversion,
		i.filetype,
		i.validitystart,
		i.validitystop
	FROM 
		t_inventory i
	WHERE 
		filename = in_fname 
		AND fileversion = in_fversion ;
	
	RETURN 0;
	
END ;
$$;


ALTER FUNCTION public.p_postinvqueue_insert(in_fname character varying, in_fversion character varying) OWNER TO acsdba;

--
-- Name: p_postinvqueue_manager(integer); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_postinvqueue_manager(in_secs integer) RETURNS integer
    LANGUAGE plpgsql
    AS $$

DECLARE
	p_secs INTERVAL;
	p_date TIMESTAMP(6);

BEGIN
	p_secs := (SELECT in_secs * '1 second'::interval);
	p_date := (SELECT now() - p_secs) ;
	
	PERFORM p_purgewaitingorders(
		pq.filetype,
		pq.validitystart,
		pq.validitystop)
	FROM 
		t_postinvqueue pq
	WHERE 
		pq.tstamp < p_date ;
	
	DELETE FROM t_postinvqueue WHERE tstamp < p_date;
		
	RETURN 0;
	
END ;
$$;


ALTER FUNCTION public.p_postinvqueue_manager(in_secs integer) OWNER TO acsdba;

--
-- Name: p_processinventory(character varying, character varying, boolean, character varying, character varying, integer, smallint, timestamp without time zone, timestamp without time zone); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_processinventory(fname character varying, fversion character varying, valflag boolean, ftype character varying, fclass character varying, in_orderid integer, in_satellite smallint, valstart timestamp without time zone, valstop timestamp without time zone) RETURNS integer
    LANGUAGE plpgsql
    AS $$

DECLARE
  	err INTEGER ;
	ftype_id INTEGER ;
	fclass_id INTEGER ;
	row RECORD;
	
BEGIN
	---------------------------------------------------------------------------------
	-- The procedure assumes the triplet satellite/sensor/mission is already verified
	---------------------------------------------------------------------------------
	
	-- return without processing if the inventory was not pending
  	IF ((SELECT COUNT(*) FROM t_pendinginventory WHERE filename = fname AND fileversion = fversion) = 0) THEN
  	  	RETURN 0 ;
  	END IF ;
	
  	-- remove pending so nobody will try the same
  	DELETE FROM t_pendinginventory WHERE filename = fname AND fileversion = fversion ;
	
	-----------------------------------------------------------------------------
	-- TEMPORARY ORDER GENERATION SECTION
	-----------------------------------------------------------------------------
	err := p_generateTmpOrder('t', fname, fversion, ftype, fclass, in_satellite) ;
	
	-----------------------------------------------------------------------------
	-- DISTRIBUTION SECTION
	-----------------------------------------------------------------------------
	IF in_orderid IS NULL THEN
		ftype_id := (SELECT id_filetype FROM t_filetypes WHERE filetype = ftype) ;
		fclass_id := (SELECT id_fileclass FROM t_fileclasses WHERE fileclass = fclass) ;
				
		FOR row IN (
			SELECT DISTINCT
				dr.user_id,
				dr.format_tt_id
			FROM 
				t_distributionrules AS dr
				INNER JOIN t_users AS us
					ON dr.user_id = us.id_user AND dr.usermode_id = us.mode
			WHERE (dr.filetype_id = ftype_id OR dr.filetype_id IN (SELECT pfiletype FROM t_filetypescomps WHERE cfiletype = ftype_id)) 
				AND dr.active = 't' 
				AND dr.ondemand = 'f'
				AND (dr.fileclass_id = fclass_id OR dr.fileclass_id IS NULL)) LOOP
				
			PERFORM p_distribute_item(fname, fversion, ftype_id, row.user_id, fclass_id, row.format_tt_id, in_orderid, NULL) ;
				
		END LOOP ;
	END IF ;
	
	-----------------------------------------------------------------------------
	-- ORDER'S BLACK LIST SECTION
	-----------------------------------------------------------------------------
	err :=  p_purgewaitingorders(ftype, valstart, valstop) ;

	-----------------------------------------------------------------------------
	-- INVENTORY LINKS SECTION
	-----------------------------------------------------------------------------
	err := p_inventorylinks_check(fname, fversion, ftype, in_orderid) ;

	RETURN 0;

END ;
$$;


ALTER FUNCTION public.p_processinventory(fname character varying, fversion character varying, valflag boolean, ftype character varying, fclass character varying, in_orderid integer, in_satellite smallint, valstart timestamp without time zone, valstop timestamp without time zone) OWNER TO acsdba;

--
-- Name: p_purgedistributionwaitingitems(bigint, character varying); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_purgedistributionwaitingitems(id_to_remove bigint, field_to_considerate character varying) RETURNS integer
    LANGUAGE plpgsql
    AS $$

DECLARE
	sqlCmd VARCHAR(256);
	
BEGIN
	sqlCmd = 'DELETE FROM t_distributionwaitingitems WHERE ' || field_to_considerate || ' = ' || id_to_remove ;
	RAISE NOTICE '%', sqlCmd;
	EXECUTE sqlCmd ;
	
	RETURN 0;
	
END ;
$$;


ALTER FUNCTION public.p_purgedistributionwaitingitems(id_to_remove bigint, field_to_considerate character varying) OWNER TO acsdba;

--
-- Name: p_purgeexpiredwaitingorders(); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_purgeexpiredwaitingorders() RETURNS integer
    LANGUAGE plpgsql
    AS $$

BEGIN
        DELETE FROM t_waitingorders WHERE expirationdate < NOW() AND expirationdate IS NOT NULL;

        RETURN 0;

END ;
$$;


ALTER FUNCTION public.p_purgeexpiredwaitingorders() OWNER TO acsdba;

--
-- Name: p_purgewaitingorders(character varying, timestamp without time zone, timestamp without time zone); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_purgewaitingorders(ftype character varying, ckstart timestamp without time zone, ckstop timestamp without time zone) RETURNS integer
    LANGUAGE plpgsql
    AS $$

DECLARE
	row RECORD ;

BEGIN
	FOR row IN (
		SELECT DISTINCT orderid
		FROM t_waitingorders WHERE 
			-- orders no waiting any longer due the presence of the new ineventory ftype
			(triggertype = 'FILETYPEMATCH' AND filetype = ftype)
			-- orders no waiting any longer due the covering by the new inventoy
			OR ((triggertype = 'INTERSECT' OR triggertype = 'COVER') AND filetype = ftype AND ckstart <= starttime AND ckstop >= stoptime)
			-- and the intersections
			OR (triggertype = 'INTERSECT' AND filetype = ftype AND ( NOT ( ckstart >= stoptime OR starttime >= ckstop))) 
	) LOOP
		
		-- delete records from table
		DELETE FROM t_waitingorders WHERE orderid = row.orderid ;
		-- write event
		PERFORM p_events(row.orderid::VARCHAR, 'ORDNOWAI');	
		
	END LOOP ;
	
	RETURN 0;
END ;
$$;


ALTER FUNCTION public.p_purgewaitingorders(ftype character varying, ckstart timestamp without time zone, ckstop timestamp without time zone) OWNER TO acsdba;

--
-- Name: p_remove_obsolete_actions(character varying, character varying); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_remove_obsolete_actions(fname character varying, fversion character varying) RETURNS integer
    LANGUAGE plpgsql
    AS $$

DECLARE
	row RECORD;
	
BEGIN
	FOR row IN (
		SELECT 
			sma.packageid AS pkgid,
			smq.status AS stat, 
			smq.operation AS oper
		FROM 
			t_smactions sma
			INNER JOIN t_smqueue smq
				ON smq.packageid = sma.packageid
		WHERE 
			sma.filename = fname 
			AND sma.fileversion = fversion 
			AND sma.packageid IS NOT NULL 
			AND sma.operation = 'UPLOAD') LOOP
	
		IF (row.stat = 'ELIGIBLE' AND row.oper = 'UPLOAD') OR (row.stat = 'ERROR' AND row.oper = 'UPLOAD') THEN
			UPDATE t_smactions SET packageid = NULL WHERE packageid = row.pkgid ;
			DELETE FROM t_smqueue WHERE packageid = row.pkgid ;
		END IF ;
		
	END LOOP ;
	
	DELETE FROM t_smactions WHERE filename = fname AND fileversion = fversion AND operation = 'UPLOAD' ;
	
	DELETE FROM t_smqueue WHERE packageid IN (
		SELECT sq.packageid FROM t_smqueue sq
		LEFT JOIN t_smactions sa ON sq.packageid = sa.packageid
		WHERE sq.status = 'COMPLETED' AND sa.packageid IS NULL) ;

	RETURN 0 ;
END ;
$$;


ALTER FUNCTION public.p_remove_obsolete_actions(fname character varying, fversion character varying) OWNER TO acsdba;

--
-- Name: p_smactionsupdate(character varying, character varying, integer); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_smactionsupdate(fname character varying, fversion character varying, pkid integer) RETURNS integer
    LANGUAGE plpgsql
    AS $$
	
BEGIN

	IF (SELECT status FROM t_smqueue WHERE packageid = pkid) = 'COMPLETED' THEN
		
		PERFORM
			p_purgewaitingorders(i.filetype, i.validitystart, i.validitystop)
		FROM 
			t_inventory i
		WHERE 
			filename = fname 
			AND fileversion = fversion ;
		
	END IF;

	RETURN 0;

END ;
$$;


ALTER FUNCTION public.p_smactionsupdate(fname character varying, fversion character varying, pkid integer) OWNER TO acsdba;

--
-- Name: p_statetransitions_check(character varying, integer, integer, integer); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_statetransitions_check(in_tname character varying, in_flw integer, in_pre_st integer, in_pst_st integer) RETURNS integer
    LANGUAGE plpgsql
    AS $$

DECLARE
	tbl_id INTEGER;
	pre_st_name VARCHAR(32);
	pst_st_name VARCHAR(32);
	no_table_ex_msg TEXT;
	no_trans_ex_msg TEXT;

BEGIN
	-- Retrieve the id of the table using the table name and the flow
	SELECT id INTO tbl_id FROM t_statetransitions_tables WHERE table_name = in_tname AND flow_id = in_flw ;
	
	-- Retrieve status name by id (just for error message)
	SELECT status INTO pre_st_name FROM t_stati WHERE id = in_pre_st ;
	SELECT status INTO pst_st_name FROM t_stati WHERE id = in_pst_st ;
	
	
	-- Set no table error messages
	IF in_flw = 1 THEN
		no_table_ex_msg := 'p_StateTransitions_Check: table ' || in_tname || ' not regestired into t_statetransitions_tables table.' ; 
	ELSE
		no_table_ex_msg := 'p_StateTransitions_Check: table ' || in_tname || ' in flow ' || in_flw || ' not regestired into t_statetransitions_tables table.' ; 
	END IF ;
	
	-- Set no transition error messages
	IF in_flw = 1 THEN
		no_trans_ex_msg := 'p_StateTransitions_Check: table ' || in_tname || ' - ' || pre_st_name || ' to ' || pst_st_name || ' invalid state transition.' ; 
	ELSE
		no_trans_ex_msg := 'p_StateTransitions_Check: table ' || in_tname || ' in flow ' || in_flw || ' - ' || pre_st_name || ' to ' || pst_st_name || ' invalid state transition.' ; 
	END IF ;
	
	
	-- Check for registered table
	IF tbl_id IS NULL THEN
		RAISE EXCEPTION '%', no_table_ex_msg ;
	END IF ;
	
	-- Check for allowed transition
	IF (SELECT count(*) FROM t_statetransitions_allowed WHERE table_id = tbl_id AND prev_state_id = in_pre_st AND post_state_id = in_pst_st) = 0 THEN
		RAISE EXCEPTION '%', no_trans_ex_msg ;
	END IF ;	
	
	
	RETURN 0;

END ;
$$;


ALTER FUNCTION public.p_statetransitions_check(in_tname character varying, in_flw integer, in_pre_st integer, in_pst_st integer) OWNER TO acsdba;

--
-- Name: p_weekday(timestamp without time zone); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_weekday(day timestamp without time zone) RETURNS integer
    LANGUAGE plpgsql
    AS $$
DECLARE
	res INTEGER ;
BEGIN

	res := EXTRACT(DOW FROM day) ;

  	IF res = 0 THEN
  	  	res := 6 ;
	ELSE
  	  	res := res - 1 ;
	END IF ;

	RETURN res ;

END ;
$$;


ALTER FUNCTION public.p_weekday(day timestamp without time zone) OWNER TO acsdba;

--
-- Name: p_weekoftheyear(timestamp without time zone); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.p_weekoftheyear(datevalue timestamp without time zone) RETURNS integer
    LANGUAGE plpgsql
    AS $$
DECLARE
	res INT ;
BEGIN

  	res := EXTRACT (WEEK FROM dateValue);
  	RETURN res ;

	EXCEPTION WHEN OTHERS THEN
	   RETURN -1 ;

END ;
$$;


ALTER FUNCTION public.p_weekoftheyear(datevalue timestamp without time zone) OWNER TO acsdba;

--
-- Name: tf_cartsinvtrmgr(); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.tf_cartsinvtrmgr() RETURNS trigger
    LANGUAGE plpgsql
    AS $$

DECLARE
  	status INTEGER ;

BEGIN
  	status := p_cartsinvcheck(NEW.id, NEW.cart, NEW.subreq, NEW.distsubreq, NEW.ordsubreq, NEW.stndsubreq ) ;
	
	IF (TG_ARGV[0] = 'INS') THEN
		status := p_events('t_cartsinv' ,'INS') ;
	END IF ;
	
	IF (TG_ARGV[0] = 'UPD') THEN
		status := p_events('t_cartsinv' ,'UPD') ;
  	END IF ;

  	RETURN NEW ;
END ;
$$;


ALTER FUNCTION public.tf_cartsinvtrmgr() OWNER TO acsdba;

--
-- Name: tf_circ_policies_statcheck(); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.tf_circ_policies_statcheck() RETURNS trigger
    LANGUAGE plpgsql
    AS $$

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
$$;


ALTER FUNCTION public.tf_circ_policies_statcheck() OWNER TO acsdba;

--
-- Name: tf_circulationretries(); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.tf_circulationretries() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
DECLARE
  	status INTEGER ;
BEGIN
  	status := p_circulationresetretries(OLD.id, OLD.status_id, NEW.status_id) ;
	RETURN NEW ;
END ;
$$;


ALTER FUNCTION public.tf_circulationretries() OWNER TO acsdba;

--
-- Name: tf_circulationsetprotocol(); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.tf_circulationsetprotocol() RETURNS trigger
    LANGUAGE plpgsql
    AS $$

DECLARE
	transf_prot VARCHAR ;
	errMsg VARCHAR ;

BEGIN
  	IF NEW.dest_protocol_id IS NULL THEN
		
		transf_prot := UPPER(SUBSTRING(NEW.tourl FROM 0 FOR (POSITION(':' IN NEW.tourl)))) ;
		
		IF transf_prot IS NULL OR transf_prot = '' THEN
			transf_prot := 'FILE' ;
		END IF ;
		
		SELECT id INTO NEW.dest_protocol_id FROM t_transferprotocols WHERE protocol = transf_prot ;
		IF NEW.dest_protocol_id IS NULL THEN
			errMsg := 'tf_circulationsetprotocol(): dest_protocol_id "' || transf_prot || '" not found' ;
			RAISE EXCEPTION '%', errMsg ;
		END IF ;
	END IF ;
	
	RETURN NEW ;	
END ;
$$;


ALTER FUNCTION public.tf_circulationsetprotocol() OWNER TO acsdba;

--
-- Name: tf_circulationsinsert(); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.tf_circulationsinsert() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
DECLARE
  	status INTEGER ;
BEGIN
  	status := p_circulationcheckstartstatus(NEW.status_id) ;
	RETURN NEW ;
END ;
$$;


ALTER FUNCTION public.tf_circulationsinsert() OWNER TO acsdba;

--
-- Name: tf_circulationstatus(); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.tf_circulationstatus() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
BEGIN
  	PERFORM p_statetransitions_check('t_circulationsqueue', 1, OLD.status_id, NEW.status_id);
	RETURN NEW ;
END ;
$$;


ALTER FUNCTION public.tf_circulationstatus() OWNER TO acsdba;

--
-- Name: tf_cov_relevancies_clean(); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.tf_cov_relevancies_clean() RETURNS trigger
    LANGUAGE plpgsql
    AS $$

BEGIN
	DELETE FROM t_cov_relevancies WHERE creationdate < (NOW() - INTERVAL '1 WEEKS');
	
  	RETURN NEW ;
END ;
$$;


ALTER FUNCTION public.tf_cov_relevancies_clean() OWNER TO acsdba;

--
-- Name: tf_cov_rules_last_update(); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.tf_cov_rules_last_update() RETURNS trigger
    LANGUAGE plpgsql
    AS $$

BEGIN
	NEW.last_update := now() ;
	
  	RETURN NEW ;
END ;
$$;


ALTER FUNCTION public.tf_cov_rules_last_update() OWNER TO acsdba;

--
-- Name: tf_datastoragequeuetrans(); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.tf_datastoragequeuetrans() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
BEGIN
	IF (SELECT datastoragetype FROM t_datastoragetypes WHERE id = NEW.type_id) = 'ARCHIVE'THEN
  		PERFORM p_statetransitions_check('t_datastoragequeue', (SELECT id FROM t_statetransitions_flows WHERE flow_type = 'ARCHIVE'), OLD.status_id, NEW.status_id);
	END IF ;
	
	IF (SELECT datastoragetype FROM t_datastoragetypes WHERE id = NEW.type_id) = 'DELETE' THEN
  		PERFORM p_statetransitions_check('t_datastoragequeue', (SELECT id FROM t_statetransitions_flows WHERE flow_type = 'DELETE'), OLD.status_id, NEW.status_id);
	END IF ;
	
	IF (SELECT datastoragetype FROM t_datastoragetypes WHERE id = NEW.type_id) = 'LTA_ARCHIVE' THEN
  		PERFORM p_statetransitions_check('t_datastoragequeue', (SELECT id FROM t_statetransitions_flows WHERE flow_type = 'LTA_ARCHIVE'), OLD.status_id, NEW.status_id);
	END IF ;
	
	IF (SELECT datastoragetype FROM t_datastoragetypes WHERE id = NEW.type_id) = 'RETRIEVE' THEN
  		PERFORM p_statetransitions_check('t_datastoragequeue', (SELECT id FROM t_statetransitions_flows WHERE flow_type = 'RETRIEVE'), OLD.status_id, NEW.status_id);
	END IF ;
	
	RETURN NEW ;
END ;
$$;


ALTER FUNCTION public.tf_datastoragequeuetrans() OWNER TO acsdba;

--
-- Name: tf_datastoragequeueupdate(); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.tf_datastoragequeueupdate() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
DECLARE
	proc_exists INTEGER ;
  	status INTEGER ;
BEGIN
	proc_exists := (SELECT count(*) FROM pg_proc WHERE proname = 'p_events') ;
  	IF proc_exists > 0 THEN
		PERFORM p_events('t_circulationsqueue', 'status:' || NEW.status_id) ;
	ELSE 
		PERFORM p_inserteventtableitem('t_circulationsqueue', 'status:' || NEW.status_id) ;
	END IF ;

	status := p_datastoragequeue_update(NEW.id, NEW.status_id) ;

	RETURN NEW ;
END ;
$$;


ALTER FUNCTION public.tf_datastoragequeueupdate() OWNER TO acsdba;

--
-- Name: tf_distributionitems_manager(); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.tf_distributionitems_manager() RETURNS trigger
    LANGUAGE plpgsql
    AS $$

BEGIN
	IF (TG_ARGV[0] = 'INS') THEN
		PERFORM p_inv_dist_counter_by_items(NEW.filename, NEW.fileversion, 'INS') ;
		PERFORM p_events('t_distributionitems', 'INS') ;
		RETURN NEW ;
	END IF ;
	
	IF (TG_ARGV[0] = 'UPD') THEN
		PERFORM p_events('t_distributionitems', 'UPD') ;
		PERFORM p_PurgeDistributionWaitingItems(NEW.id, 'distributionitem_id') ;
		RETURN NEW ;
	END IF ;
	
	IF (TG_ARGV[0] = 'DEL') THEN
		PERFORM p_inv_dist_counter_by_items(OLD.filename, OLD.fileversion, 'DEL') ;
		PERFORM p_events('t_distributionitems', 'DEL') ;
		RETURN OLD ;
	END IF ;
	
END ;
$$;


ALTER FUNCTION public.tf_distributionitems_manager() OWNER TO acsdba;

--
-- Name: tf_distributionitemsevent(); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.tf_distributionitemsevent() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
DECLARE
  	status INTEGER ;
	evttype VARCHAR[10] ;
BEGIN
  	status := p_events(TG_ARGV[0], TG_ARGV[1]) ;
	IF (TG_ARGV[1] = 'INS') THEN
	  evttype := 'INSERT' ;
	ELSIF (TG_ARGV[1] = 'UPD') THEN
	  evttype := 'UPDATE' ;
	ELSE
	  evttype := TG_ARGV[1] ;
	END IF ;

  	--status := p_insert_inventoryevents(NEW.filename, NEW.fileversion, evttype, NEW.validityflag) ;

  	RETURN NEW ;
END ;
$$;


ALTER FUNCTION public.tf_distributionitemsevent() OWNER TO acsdba;

--
-- Name: tf_distributionqueue_manager(); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.tf_distributionqueue_manager() RETURNS trigger
    LANGUAGE plpgsql
    AS $$

BEGIN
	IF (TG_ARGV[0] = 'INS') THEN
		PERFORM p_events('t_distributionqueue', 'INS') ;
		PERFORM p_cartsinv_update(NEW.packageid, NEW.status);
		RETURN NEW ;
	END IF ;
	
	IF (TG_ARGV[0] = 'UPD') THEN
		PERFORM p_events('t_distributionqueue', 'UPD') ;
		PERFORM p_cartsinv_update(NEW.packageid, NEW.status);
		PERFORM p_inv_dist_counter_by_queue(NEW.packageid, OLD.status, NEW.status, 'UPD') ;
		PERFORM p_PurgeDistributionWaitingItems(NEW.packageid, 'package_id') ;
		RETURN NEW ;
	END IF ;
	
	IF (TG_ARGV[0] = 'DEL') THEN
		PERFORM p_inv_dist_counter_by_queue(OLD.packageid, OLD.status, NULL, 'DEL') ;
		PERFORM p_events('t_distributionqueue', 'DEL') ;
		RETURN OLD ;
	END IF ;
	
END ;
$$;


ALTER FUNCTION public.tf_distributionqueue_manager() OWNER TO acsdba;

--
-- Name: tf_distributionqueue_statcheck(); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.tf_distributionqueue_statcheck() RETURNS trigger
    LANGUAGE plpgsql
    AS $$

DECLARE
	preStatId INT ;
	postStatId INT ;
	errMsg VARCHAR(128) ;
	
BEGIN
	IF (TG_ARGV[0] = 'UPD') THEN
		preStatId := (SELECT id FROM t_stati WHERE status = OLD.status) ;
		postStatId := (SELECT id FROM t_stati WHERE status = NEW.status) ;
		
		IF postStatId IS NULL THEN
			errMsg := 'tf_distributionqueue_statcheck: ' || NEW.status || ' status does not exist.' ;
			RAISE EXCEPTION '%', errMsg ;
		END IF ;
		
		PERFORM p_statetransitions_check('t_distributionqueue', 1, preStatId, postStatId);
	END IF ;
	
	IF (NEW.status = 'ELIGIBLE') THEN
		NEW.retrycounter := 0 ;
	END IF ;
	
	RETURN NEW ;
	
END ;
$$;


ALTER FUNCTION public.tf_distributionqueue_statcheck() OWNER TO acsdba;

--
-- Name: tf_dsqueuexcirqueuetrans(); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.tf_dsqueuexcirqueuetrans() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
BEGIN
  	PERFORM p_statetransitions_check('t_dsqueue_x_cirqueue', 1, OLD.globalstatus_id, NEW.globalstatus_id);
	RETURN NEW ;
END ;
$$;


ALTER FUNCTION public.tf_dsqueuexcirqueuetrans() OWNER TO acsdba;

--
-- Name: tf_filetype_insert(); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.tf_filetype_insert() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
DECLARE
  	status INTEGER ;
BEGIN
	INSERT INTO t_filetypes_last_inv (filetype) VALUES (NEW.filetype) ;
	status := p_events('t_filetypes', 'INS') ;
  	RETURN NEW ;
END ;
$$;


ALTER FUNCTION public.tf_filetype_insert() OWNER TO acsdba;

--
-- Name: tf_inserteventtableitem(); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.tf_inserteventtableitem() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
DECLARE
  	status INTEGER ;
BEGIN
  	status := p_events(TG_ARGV[0], TG_ARGV[1]) ;
  	RETURN NEW ;
END ;
$$;


ALTER FUNCTION public.tf_inserteventtableitem() OWNER TO acsdba;

--
-- Name: tf_invdistributionsupdate(); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.tf_invdistributionsupdate() RETURNS trigger
    LANGUAGE plpgsql
    AS $$

BEGIN
	NEW.last_update := now()::TIMESTAMP ;
	
  	RETURN NEW ;
END ;
$$;


ALTER FUNCTION public.tf_invdistributionsupdate() OWNER TO acsdba;

--
-- Name: tf_inventory_ins_bfr(); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.tf_inventory_ins_bfr() RETURNS trigger
    LANGUAGE plpgsql
    AS $$

BEGIN
	IF NEW.unique_id IS NULL  THEN
		NEW.unique_id := NEW.filename || NEW.fileversion ;
	END IF ;
	
	RETURN NEW ;
END ;
$$;


ALTER FUNCTION public.tf_inventory_ins_bfr() OWNER TO acsdba;

--
-- Name: tf_inventorydelete(); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.tf_inventorydelete() RETURNS trigger
    LANGUAGE plpgsql
    AS $$

BEGIN
	PERFORM p_insert_inventoryevents(NULL, OLD.filename, OLD.fileversion, OLD.filetype, 'f', 'f', 'D') ;
	
  	RETURN NEW ;
END ;
$$;


ALTER FUNCTION public.tf_inventorydelete() OWNER TO acsdba;

--
-- Name: tf_inventoryinsevent(); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.tf_inventoryinsevent() RETURNS trigger
    LANGUAGE plpgsql
    AS $$

BEGIN
	PERFORM p_inventory_manager(NEW.filename, NEW.fileversion, NULL, NEW.validityflag, NEW.filetype, NEW.fileclass, NEW.orderid, NEW.satelliteid, NEW.sensorid, NEW.mission, NEW.validitystart, NEW.validitystop) ;
	PERFORM p_insert_inventoryevents(NEW.id_inv, NEW.filename, NEW.fileversion, NEW.filetype, NULL, NEW.validityflag, 'I') ;
	
	RETURN NEW ;
END ;
$$;


ALTER FUNCTION public.tf_inventoryinsevent() OWNER TO acsdba;

--
-- Name: tf_inventorysm_check_invid(); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.tf_inventorysm_check_invid() RETURNS trigger
    LANGUAGE plpgsql
    AS $$

BEGIN	
	IF NEW.inv_id IS NULL THEN 
		RAISE EXCEPTION 'tf_inventorysm_check_invid: Cannot insert a new record having null inv_id';
	END IF ;
	
	RETURN NEW ;
END ;
$$;


ALTER FUNCTION public.tf_inventorysm_check_invid() OWNER TO acsdba;

--
-- Name: tf_inventorysmmanager(); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.tf_inventorysmmanager() RETURNS trigger
    LANGUAGE plpgsql
    AS $$

BEGIN
	PERFORM p_events('t_inventorysm', TG_ARGV[0]) ;
	
	IF (TG_ARGV[0] = 'INS') 
	THEN
		PERFORM p_insert_inventoryevents(NEW.inv_id, NEW.storagemanid) ;
		PERFORM p_insert_inv_data_restored(NEW.id, NEW.inv_id, NEW.storagemanid) ;
		RETURN NEW ;
	END IF ;
	
	IF (TG_ARGV[0] = 'UPD') 
	THEN
		PERFORM p_insert_inventoryevents(NEW.inv_id, NEW.storagemanid) ;
		RETURN NEW ;
	END IF ;
	
	IF (TG_ARGV[0] = 'DEL') 
	THEN 
		PERFORM p_insert_inventoryevents(OLD.inv_id, OLD.storagemanid) ;
		
		IF (SELECT count(*) FROM t_inventorysm ism
			INNER JOIN t_storagemanager stm ON ism.storagemanid = stm.id
			WHERE ism.inv_id = OLD.inv_id AND stm.syncdownload = 't') = 0 
		THEN
			DELETE FROM t_sm_sync_missingactions WHERE inv_id = OLD.inv_id AND operation = 'UPLOAD' ;
		END IF ;
		
		RETURN OLD ;
	END IF ;
	
	RETURN NEW ;
END ;
$$;


ALTER FUNCTION public.tf_inventorysmmanager() OWNER TO acsdba;

--
-- Name: tf_inventoryupdevent(); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.tf_inventoryupdevent() RETURNS trigger
    LANGUAGE plpgsql
    AS $$

BEGIN
	PERFORM p_inventory_manager(NEW.filename, NEW.fileversion, OLD.validityflag, NEW.validityflag, NEW.filetype, NEW.fileclass, NEW.orderid, NEW.satelliteid, NEW.sensorid, NEW.mission, NEW.validitystart, NEW.validitystop) ;
	PERFORM p_insert_inventoryevents(NEW.id_inv, NULL, NULL, NEW.filetype, OLD.validityflag, NEW.validityflag, 'U') ;
	
	IF (NEW.validityflag != OLD.validityflag) THEN
		IF (NEW.validityflag = 't') THEN
			PERFORM p_purgewaitingorders(NEW.filetype, NEW.validitystart, NEW.validitystop) ;
		ELSE
			PERFORM p_PurgeDistributionWaitingItems(NEW.id_inv, 'inventory_id') ;
		END IF;
	END IF;
	
	IF OLD.qualityinfo != NEW.qualityinfo THEN
		PERFORM p_purgewaitingorders(NEW.filetype, NEW.validitystart, NEW.validitystop) ;
	END IF ;
	
	IF (OLD.qualityinfo = 'DUP' AND (NEW.qualityinfo = '' OR NEW.qualityinfo IS NULL) AND NEW.validityflag = 't') THEN
		PERFORM p_generatetmporder('t', OLD.filename, OLD.fileversion, OLD.filetype, OLD.fileclass, OLD.satelliteid) ;
	END IF ;
	
	IF OLD.qualified != NEW.qualified THEN
		PERFORM p_purgewaitingorders(NEW.filetype, NEW.validitystart, NEW.validitystop) ;
	END IF ;
	
  	RETURN NEW ;
END ;
$$;


ALTER FUNCTION public.tf_inventoryupdevent() OWNER TO acsdba;

--
-- Name: tf_invxbaselines_insert(); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.tf_invxbaselines_insert() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
DECLARE
  	status INTEGER ;
BEGIN
	status := p_inv_x_baselines_insert(NEW.id_inv) ;
  	RETURN NEW ;
END ;
$$;


ALTER FUNCTION public.tf_invxbaselines_insert() OWNER TO acsdba;

--
-- Name: tf_mpm_products_populate(); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.tf_mpm_products_populate() RETURNS trigger
    LANGUAGE plpgsql
    AS $$

BEGIN
	IF (TG_ARGV[0] = 'INS') THEN
		-- If the file is not a SEN3 format ...
		IF (SUBSTRING(NEW.filename FROM STRPOS(NEW.filename, '.')+1 FOR (LENGTH(NEW.filename) - STRPOS(NEW.filename, '.'))+1)) != 'SEN3' THEN
			-- ... do nothing and exit
			RETURN NEW ;
		END IF ;
		
		INSERT INTO t_mpm_products (
			id_product,
			productname,
			productversion,
			filetype_id,
			timeliness_id,
			productsize,
			insertiontime,
			creationtime,
			starttime,
			stoptime,
			footprint,
			orbit_num,
			relative_orbit_num,
			cycle_num,
			processor_id,
			processing_level,
			qualityflag_id )
		VALUES (
			NEW.id_inv,
			NEW.filename,
			NEW.fileversion,
			(SELECT id_filetype FROM t_filetypes WHERE filetype = NEW.filetype),
			(SELECT id_timeliness FROM t_mpm_timeliness WHERE timeliness_code = SUBSTRING(NEW.filename FROM 89 FOR 2)),			
			NEW.datasize,
			NEW.inventorydate,
			NEW.generationtime,
			NEW.validitystart,
			NEW.validitystop,
			NEW.geographiclocalization,
			CASE WHEN NEW.startorbitnumber IS NULL
				THEN -1
				ELSE NEW.startorbitnumber
			END,
			CASE WHEN NEW.startorbitnumber IS NOT NULL
				THEN TO_NUMBER(SUBSTRING(NEW.filename FROM 74 FOR 3), '999')
				ELSE NULL
			END,
			CASE WHEN NEW.startorbitnumber IS NOT NULL
				THEN TO_NUMBER(SUBSTRING(NEW.filename FROM 70 FOR 3), '999')
				ELSE NULL
			END,
			(SELECT id_processor FROM t_ordersqueue o 
				INNER JOIN t_processors p ON o.processorname = p.processorname AND o.processorversion = p.processorversion
				WHERE orderid = NEW.orderid),
			CASE WHEN SUBSTRING(NEW.filename FROM 8 FOR 1) NOT IN ('0','1','2')
				THEN -1
				ELSE TO_NUMBER(SUBSTRING(NEW.filename FROM 8 FOR 1), '9')
			END,
			NEW.qualified
		) ;
	
	ELSIF (TG_ARGV[0] = 'UPD') THEN
		UPDATE t_mpm_products SET insertiontime = NEW.inventorydate WHERE id_product = NEW.id_inv ;
	
	ELSIF (TG_ARGV[0] = 'DEL') THEN
		DELETE FROM t_mpm_products WHERE id_product = OLD.id_inv ;
	
	ELSE
	  RETURN NEW ;
	END IF ;
	
	RETURN NEW ;
END;
$$;


ALTER FUNCTION public.tf_mpm_products_populate() OWNER TO acsdba;

--
-- Name: tf_mpm_values_delete_master(); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.tf_mpm_values_delete_master() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
DECLARE
    rec t_mpm_values%rowtype ;
BEGIN
    DELETE FROM ONLY t_mpm_values WHERE id_value = new.id_value RETURNING * INTO rec ;
    
	RETURN rec ;
END;
$$;


ALTER FUNCTION public.tf_mpm_values_delete_master() OWNER TO acsdba;

--
-- Name: tf_mpm_values_manager(); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.tf_mpm_values_manager() RETURNS trigger
    LANGUAGE plpgsql
    AS $$

BEGIN
	IF (TG_ARGV[0] = 'INS') THEN
		-- Do nothing
	
	ELSIF (TG_ARGV[0] = 'UPD') THEN
		-- Do nothing
	
	ELSIF (TG_ARGV[0] = 'DEL') THEN
		DELETE FROM t_mpm_values WHERE id_value IN (
			SELECT value_id FROM t_mpm_cross WHERE product_id = OLD.id_product) ;
		RETURN OLD ;
	ELSE
	  RETURN NEW ;
	END IF ;
	
	RETURN NEW ;
END;
$$;


ALTER FUNCTION public.tf_mpm_values_manager() OWNER TO acsdba;

--
-- Name: tf_mpm_values_partition_mngr(); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.tf_mpm_values_partition_mngr() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
DECLARE
	rec t_mpm_values%rowtype ;
BEGIN
    IF ( NEW.level = 1 ) THEN
        INSERT INTO t_mpm_values_part1 VALUES (NEW.*) RETURNING * INTO rec ;
    ELSIF ( NEW.level = 2 ) THEN
        INSERT INTO t_mpm_values_part2 VALUES (NEW.*) RETURNING * INTO rec ;
    ELSE
        RAISE EXCEPTION 'tf_mpm_values_partition_mngr(): Level out of range';
    END IF;
    
	RETURN rec ;
END;
$$;


ALTER FUNCTION public.tf_mpm_values_partition_mngr() OWNER TO acsdba;

--
-- Name: tf_ordersqueue_manager(); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.tf_ordersqueue_manager() RETURNS trigger
    LANGUAGE plpgsql
    AS $$

BEGIN
	IF (TG_ARGV[0] = 'INS') THEN
		PERFORM p_orderstatetransitions_mgr(NEW.orderid, NULL, NEW.status) ;
		PERFORM p_orders_manager(NEW.orderid, NEW.status, NEW.status) ;
		PERFORM p_events('t_ordersqueue', 'INS') ;
		RETURN NEW ;
	END IF ;
	
	IF (TG_ARGV[0] = 'UPD') THEN
		PERFORM p_orderstatetransitions_mgr(NEW.orderid, OLD.status, NEW.status) ;
		PERFORM p_orders_manager(OLD.orderid, OLD.status, NEW.status) ;
		PERFORM p_orderqueuecheckchanges(OLD.orderid, OLD.status, NEW.status, OLD.processorname, NEW.processorname, OLD.processorversion, NEW.processorversion, OLD.processorconfiguration, NEW.processorconfiguration, OLD.starttime, NEW.starttime, OLD.stoptime, NEW.stoptime, OLD.ordertype, NEW.ordertype, OLD.satelliteid, NEW.satelliteid, OLD.mission, NEW.mission) ;
		PERFORM p_events('t_ordersqueue', 'UPD') ;
		RETURN NEW ;
	END IF ;
	
	IF (TG_ARGV[0] = 'DEL') THEN
		PERFORM p_events('t_ordersqueue', 'DEL') ;
		RETURN OLD ;
	END IF ;
END ;
$$;


ALTER FUNCTION public.tf_ordersqueue_manager() OWNER TO acsdba;

--
-- Name: tf_ordersqueue_statcheck(); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.tf_ordersqueue_statcheck() RETURNS trigger
    LANGUAGE plpgsql
    AS $$

DECLARE
  	preStatId INT ;
	postStatId INT ;
	errMsg VARCHAR(128) ;

BEGIN
  	IF (TG_ARGV[0] = 'INS') THEN
		IF (NEW.status != 'ABORTED') AND (NEW.status != 'ELIGIBLE') AND (NEW.status != 'STANDBY') THEN
			errMsg := 'tf_ordersqueue_statcheck: the first status can be only ABORTED or ELIGIBLE or STANDBY' ;
			RAISE EXCEPTION '%', errMsg ;
		END IF ;
	END IF ;
	
	IF (TG_ARGV[0] = 'UPD') THEN
		preStatId := (SELECT id FROM t_stati WHERE status = OLD.status) ;
		postStatId := (SELECT id FROM t_stati WHERE status = NEW.status) ;
		
		IF postStatId IS NULL THEN
			errMsg := 'tf_ordersqueue_statcheck: ' || NEW.status || ' status does not exist.' ;
			RAISE EXCEPTION '%', errMsg ;
		END IF ;
		
		PERFORM p_statetransitions_check('t_ordersqueue', 1, preStatId, postStatId);
	END IF ;
	
  	RETURN NEW ;
END ;
$$;


ALTER FUNCTION public.tf_ordersqueue_statcheck() OWNER TO acsdba;

--
-- Name: tf_pendinginvinsert(); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.tf_pendinginvinsert() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
DECLARE
  	status INTEGER ;
BEGIN
	status := p_postinvqueue_insert(NEW.filename, NEW.fileversion) ;
  	RETURN NEW ;
END ;
$$;


ALTER FUNCTION public.tf_pendinginvinsert() OWNER TO acsdba;

--
-- Name: tf_smactions_upd_aft(); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.tf_smactions_upd_aft() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
BEGIN
	PERFORM p_smactionsupdate(NEW.filename, NEW.fileversion, NEW.packageid) ;
	PERFORM p_events('t_smactions', 'UPD') ;
	
  	RETURN NEW ;
END ;
$$;


ALTER FUNCTION public.tf_smactions_upd_aft() OWNER TO acsdba;

--
-- Name: tf_smactions_upd_bfr(); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.tf_smactions_upd_bfr() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
BEGIN
	IF (OLD.packageid IS NOT NULL AND NEW.packageid IS NOT NULL) AND (OLD.packageid != NEW.packageid) THEN
		RAISE EXCEPTION 'tf_smactions_upd_bfr: Cannot assign a new packageid in a record having an old packageid';
	END IF;
	
	RETURN NEW ;
END ;
$$;


ALTER FUNCTION public.tf_smactions_upd_bfr() OWNER TO acsdba;

--
-- Name: tf_smactionsinsert(); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.tf_smactionsinsert() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
BEGIN
	IF NEW.packageid IS NOT NULL THEN
		RAISE EXCEPTION 'tf_smactionsInsert: Cannot insert a new record having a packageid';
	END IF;
	
	PERFORM p_events('t_smactions', 'INS') ;
	
  	RETURN NEW ;
END ;
$$;


ALTER FUNCTION public.tf_smactionsinsert() OWNER TO acsdba;

--
-- Name: tf_smqueue_checkchanges(); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.tf_smqueue_checkchanges() RETURNS trigger
    LANGUAGE plpgsql
    AS $$

DECLARE
  	preStatId INT ;
	postStatId INT ;
	errMsg VARCHAR(128) ;

BEGIN
  	IF (TG_ARGV[0] = 'INS') THEN
		IF NEW.status != 'STANDBY' THEN
			errMsg := 'tf_smqueue_checkchanges: the first status must be STANDBY' ;
			RAISE EXCEPTION '%', errMsg ;
		END IF ;
	END IF ;
	
	IF (TG_ARGV[0] = 'UPD') THEN
		preStatId := (SELECT id FROM t_stati WHERE status = OLD.status) ;
		postStatId := (SELECT id FROM t_stati WHERE status = NEW.status) ;
		
		IF postStatId IS NULL THEN
			errMsg := 'tf_smqueue_checkchanges: ' || NEW.status || ' status does not exist.' ;
			RAISE EXCEPTION '%', errMsg ;
		END IF ;
		
		PERFORM p_statetransitions_check('t_smqueue', 1, preStatId, postStatId);
	END IF ;
	
  	RETURN NEW ;
END ;
$$;


ALTER FUNCTION public.tf_smqueue_checkchanges() OWNER TO acsdba;

--
-- Name: tf_status_transiction_check(); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.tf_status_transiction_check() RETURNS trigger
    LANGUAGE plpgsql
    AS $$

BEGIN
	PERFORM p_statetransitions_check(TG_TABLE_NAME::VARCHAR, 1, OLD.status_id, NEW.status_id) ;
	
	RETURN NEW ;
	
END ;
$$;


ALTER FUNCTION public.tf_status_transiction_check() OWNER TO acsdba;

--
-- Name: tf_waitingordersinsert(); Type: FUNCTION; Schema: public; Owner: acsdba
--

CREATE FUNCTION public.tf_waitingordersinsert() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
DECLARE
  	status INTEGER ;
BEGIN
  	status := p_events(NEW.orderid::VARCHAR(64), TG_ARGV[0]) ;
  	RETURN NEW ;
END ;
$$;


ALTER FUNCTION public.tf_waitingordersinsert() OWNER TO acsdba;

SET default_tablespace = '';

--SET default_table_access_method = heap;

--
-- Name: t_filetypes; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_filetypes (
    id_filetype integer NOT NULL,
    filetype character varying(16) NOT NULL,
    description character varying(64),
    nameregularexpression character varying(128),
    headerseparation boolean DEFAULT false NOT NULL,
    "group" character varying(32) NOT NULL,
    invperiod integer,
    mngsngext boolean DEFAULT true NOT NULL,
    sngext character varying(8),
    hdrext character varying(8),
    dblext character varying(8),
    CONSTRAINT ck_filetypes01 CHECK ((((sngext IS NULL) AND (hdrext IS NULL) AND (dblext IS NULL)) OR ((sngext IS NULL) AND (hdrext IS NOT NULL) AND (dblext IS NOT NULL) AND ((hdrext)::text <> (dblext)::text)) OR ((sngext IS NOT NULL) AND (((hdrext IS NULL) AND (dblext IS NULL)) OR ((hdrext)::text <> (dblext)::text)))))
);


ALTER TABLE public.t_filetypes OWNER TO acsdba;

--
-- Name: t_mpm_cross; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_mpm_cross (
    id_cross bigint NOT NULL,
    value_id bigint NOT NULL,
    parameter_id integer NOT NULL,
    routine_id integer NOT NULL,
    product_id integer NOT NULL
);


ALTER TABLE public.t_mpm_cross OWNER TO acsdba;

--
-- Name: t_mpm_parameters; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_mpm_parameters (
    id_parameter integer NOT NULL,
    parameter_name character varying(100) NOT NULL,
    paramtype_id smallint NOT NULL,
    output_level smallint NOT NULL,
    tableddname character varying(100) NOT NULL,
    CONSTRAINT ck_mpm_parameters01 CHECK ((output_level = ANY (ARRAY[1, 2])))
);


ALTER TABLE public.t_mpm_parameters OWNER TO acsdba;

--
-- Name: t_mpm_products; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_mpm_products (
    id_product bigint NOT NULL,
    productname character varying(128) NOT NULL,
    productversion character varying(32) NOT NULL,
    filetype_id integer NOT NULL,
    timeliness_id smallint NOT NULL,
    productsize bigint,
    insertiontime timestamp(6) without time zone DEFAULT now() NOT NULL,
    creationtime timestamp(6) without time zone NOT NULL,
    starttime timestamp(6) without time zone NOT NULL,
    stoptime timestamp(6) without time zone NOT NULL,
    footprint public.geography(Geometry,4326),
    orbit_num integer NOT NULL,
    relative_orbit_num integer,
    cycle_num integer,
    processor_id integer,
    processing_level smallint NOT NULL,
    qualityflag_id integer NOT NULL,
    baseline character varying(24),
    CONSTRAINT ck_mpm_products01 CHECK ((processing_level = ANY (ARRAY['-1'::integer, 0, 1, 2])))
);


ALTER TABLE public.t_mpm_products OWNER TO acsdba;

--
-- Name: TABLE t_mpm_products; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON TABLE public.t_mpm_products IS 'Contains the list of products and its structure is similar to the t_inventory table. This table is populated by a trigger for each insert or update on the t_inventory.';


--
-- Name: COLUMN t_mpm_products.id_product; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_mpm_products.id_product IS 'Record identifier (Primary Key)';


--
-- Name: COLUMN t_mpm_products.productname; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_mpm_products.productname IS 'The name of the product. Like the filename on the t_inventory';


--
-- Name: COLUMN t_mpm_products.productversion; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_mpm_products.productversion IS 'The version of the product. Like the fileversion on the t_inventory';


--
-- Name: COLUMN t_mpm_products.baseline; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_mpm_products.baseline IS 'Baseline collection';


--
-- Name: mv_tableddname; Type: MATERIALIZED VIEW; Schema: public; Owner: acsdba
--

CREATE MATERIALIZED VIEW public.mv_tableddname AS
 SELECT DISTINCT sq.tableddname,
    sq.output_level,
    ft.filetype
   FROM (((( SELECT max(pm.id_parameter) AS m_idp,
            pm.tableddname,
            pm.output_level
           FROM public.t_mpm_parameters pm
          GROUP BY pm.tableddname, pm.output_level) sq
     JOIN public.t_mpm_cross cx ON ((sq.m_idp = cx.parameter_id)))
     JOIN public.t_mpm_products pr ON ((cx.product_id = pr.id_product)))
     JOIN public.t_filetypes ft ON ((pr.filetype_id = ft.id_filetype)))
  ORDER BY sq.tableddname
  WITH NO DATA;


ALTER TABLE public.mv_tableddname OWNER TO acsdba;

--
-- Name: t_acshsminvrelation; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_acshsminvrelation (
    id integer NOT NULL,
    externalreference character varying(64) NOT NULL,
    signature character varying(64)
);


ALTER TABLE public.t_acshsminvrelation OWNER TO acsdba;

--
-- Name: t_acsisminvrelation; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_acsisminvrelation (
    id integer NOT NULL,
    path character varying(512) NOT NULL,
    signature character varying(64),
    medialabel character varying(128)
);


ALTER TABLE public.t_acsisminvrelation OWNER TO acsdba;

--
-- Name: t_acsltainvrelation; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_acsltainvrelation (
    id integer NOT NULL,
    productid character varying(512) NOT NULL,
    signature character varying(64)
);


ALTER TABLE public.t_acsltainvrelation OWNER TO acsdba;

--
-- Name: t_algorithm; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_algorithm (
    algorithm character varying(32) NOT NULL,
    description character varying(64)
);


ALTER TABLE public.t_algorithm OWNER TO acsdba;

--
-- Name: t_anomaly_types; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_anomaly_types (
    id smallint NOT NULL,
    anomaly_type character varying(24) NOT NULL
);


ALTER TABLE public.t_anomaly_types OWNER TO acsdba;

--
-- Name: t_applicationevents; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_applicationevents (
    id_applicationevent integer NOT NULL,
    application_id integer NOT NULL,
    tstamp timestamp(6) without time zone DEFAULT now() NOT NULL,
    event_type character varying(64) NOT NULL,
    event_description character varying(256)
);


ALTER TABLE public.t_applicationevents OWNER TO acsdba;

--
-- Name: t_applicationevents_id_applicationevent_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_applicationevents_id_applicationevent_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_applicationevents_id_applicationevent_seq OWNER TO acsdba;

--
-- Name: t_applicationevents_id_applicationevent_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_applicationevents_id_applicationevent_seq OWNED BY public.t_applicationevents.id_applicationevent;


--
-- Name: t_applications; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_applications (
    id_application integer NOT NULL,
    app_name character varying(64) NOT NULL,
    app_description text
);


ALTER TABLE public.t_applications OWNER TO acsdba;

--
-- Name: t_applications_id_application_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_applications_id_application_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_applications_id_application_seq OWNER TO acsdba;

--
-- Name: t_applications_id_application_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_applications_id_application_seq OWNED BY public.t_applications.id_application;


--
-- Name: t_applicationstatus; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_applicationstatus (
    application character varying(64) NOT NULL,
    instance character varying(8) NOT NULL,
    status oid
);


ALTER TABLE public.t_applicationstatus OWNER TO acsdba;

--
-- Name: t_asyncsm_operations; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_asyncsm_operations (
    id_asyncsm_operation integer NOT NULL,
    filename character varying(128) NOT NULL,
    fileversion character varying(32) NOT NULL,
    unique_id character varying(160) NOT NULL,
    tstamp timestamp(6) without time zone DEFAULT now() NOT NULL,
    operation_id smallint NOT NULL,
    sm_id integer NOT NULL,
    status_id smallint NOT NULL,
    status_reason text
);


ALTER TABLE public.t_asyncsm_operations OWNER TO acsdba;

--
-- Name: TABLE t_asyncsm_operations; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON TABLE public.t_asyncsm_operations IS 'Contains the list of the ingestions statuses';


--
-- Name: COLUMN t_asyncsm_operations.id_asyncsm_operation; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_asyncsm_operations.id_asyncsm_operation IS 'Record identifier (Primary Key)';


--
-- Name: COLUMN t_asyncsm_operations.unique_id; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_asyncsm_operations.unique_id IS 'Unique ID of the LTA Product. It is the ID of the PDI at datastrip level';


--
-- Name: COLUMN t_asyncsm_operations.tstamp; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_asyncsm_operations.tstamp IS 'The timestamp of the instant when the product entered in the status specified into the field "status"';


--
-- Name: COLUMN t_asyncsm_operations.operation_id; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_asyncsm_operations.operation_id IS 'Type of operation. It is a reference to t_operationtypes';


--
-- Name: COLUMN t_asyncsm_operations.sm_id; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_asyncsm_operations.sm_id IS 'The storage in which the LTA product is archived. It is a reference to t_storagemanager';


--
-- Name: COLUMN t_asyncsm_operations.status_id; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_asyncsm_operations.status_id IS 'The status in which the LTA product is. It is a reference to t_stati';


--
-- Name: COLUMN t_asyncsm_operations.status_reason; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_asyncsm_operations.status_reason IS 'Description of the reason why the product is in the status defined into the field "status"';


--
-- Name: t_asyncsm_operations_id_asyncsm_operation_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_asyncsm_operations_id_asyncsm_operation_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_asyncsm_operations_id_asyncsm_operation_seq OWNER TO acsdba;

--
-- Name: t_asyncsm_operations_id_asyncsm_operation_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_asyncsm_operations_id_asyncsm_operation_seq OWNED BY public.t_asyncsm_operations.id_asyncsm_operation;


--
-- Name: t_bands; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_bands (
    id integer NOT NULL,
    name character varying(16) NOT NULL
);


ALTER TABLE public.t_bands OWNER TO acsdba;

--
-- Name: t_bands_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_bands_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_bands_id_seq OWNER TO acsdba;

--
-- Name: t_bands_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_bands_id_seq OWNED BY public.t_bands.id;


--
-- Name: t_blockedcirculationsqueue; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_blockedcirculationsqueue (
    id bigint NOT NULL,
    startdate timestamp(6) without time zone,
    enddate timestamp(6) without time zone,
    priority integer DEFAULT 0 NOT NULL,
    remark character varying(512),
    status_id smallint DEFAULT 16 NOT NULL,
    creationdate timestamp(6) without time zone DEFAULT now(),
    modificationdate timestamp(6) without time zone DEFAULT now(),
    fromurl character varying(512) NOT NULL,
    tourl character varying(512) NOT NULL,
    relay_host_id integer,
    relay_activity_id bigint,
    retries smallint DEFAULT 0 NOT NULL,
    removesource boolean DEFAULT false NOT NULL,
    circulationmode_id smallint NOT NULL,
    host_id integer,
    circulationpolicy_id integer,
    dest_host_id integer,
    error character varying(512),
    tempnamevalue character varying(64),
    tempname_id smallint,
    nextretrydate timestamp(6) without time zone DEFAULT now(),
    dest_protocol_id smallint,
    media_circulation_id integer,
    media_insertionmode_id smallint DEFAULT 1 NOT NULL,
    localtransfermode_id smallint DEFAULT 1 NOT NULL,
    CONSTRAINT ck_blockedcirculationsqueue01 CHECK ((char_length(btrim((tempnamevalue)::text)) <> 0)),
    CONSTRAINT ck_blockedcirculationsqueue02 CHECK ((((tempname_id <> 2) AND (((tempnamevalue)::text = '..'::text) OR ((tempnamevalue)::text = '../'::text))) <> true)),
    CONSTRAINT ck_blockedcirculationsqueue03 CHECK ((((tempname_id = 3) AND (((tempnamevalue)::text = '.'::text) OR ((tempnamevalue)::text = './'::text))) <> true))
);


ALTER TABLE public.t_blockedcirculationsqueue OWNER TO acsdba;

--
-- Name: t_blockedcirculationsqueue_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_blockedcirculationsqueue_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_blockedcirculationsqueue_id_seq OWNER TO acsdba;

--
-- Name: t_blockedcirculationsqueue_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_blockedcirculationsqueue_id_seq OWNED BY public.t_blockedcirculationsqueue.id;


--
-- Name: t_cartsinv; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_cartsinv (
    id integer NOT NULL,
    filename character varying(128),
    fileversion character varying(32),
    cart integer NOT NULL,
    subreq integer NOT NULL,
    distsubreq integer,
    ordsubreq integer,
    stndsubreq integer,
    status character varying(32) DEFAULT 'ELIGIBLE'::character varying NOT NULL,
    handled boolean DEFAULT false NOT NULL
);


ALTER TABLE public.t_cartsinv OWNER TO acsdba;

--
-- Name: t_cartsinv_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_cartsinv_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_cartsinv_id_seq OWNER TO acsdba;

--
-- Name: t_cartsinv_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_cartsinv_id_seq OWNED BY public.t_cartsinv.id;


--
-- Name: t_cartsref; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_cartsref (
    id integer NOT NULL,
    extref character varying(128) NOT NULL,
    data text,
    totreqs integer NOT NULL,
    procreqs integer DEFAULT 0 NOT NULL,
    distreqs integer DEFAULT 0 NOT NULL,
    status character varying(32) NOT NULL,
    stndreqs integer DEFAULT 0 NOT NULL,
    account character varying(32)
);


ALTER TABLE public.t_cartsref OWNER TO acsdba;

--
-- Name: t_cartsref_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_cartsref_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_cartsref_id_seq OWNER TO acsdba;

--
-- Name: t_cartsref_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_cartsref_id_seq OWNED BY public.t_cartsref.id;


--
-- Name: t_cdp_feederqueue; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_cdp_feederqueue (
    id integer NOT NULL,
    uuid character varying(160),
    checksum jsonb,
    uri character varying(512) NOT NULL,
    cache_path character varying(512),
    s3_reference text,
    inv_id bigint,
    creationdate timestamp(6) without time zone DEFAULT now() NOT NULL,
    modificationdate timestamp(6) without time zone,
    startdate timestamp(6) without time zone,
    completiondate timestamp(6) without time zone,
    retry_count integer DEFAULT 0,
    retry_time timestamp(6) without time zone,
    status_id smallint NOT NULL,
    jobresponsible character varying(64),
    destinationurl character varying(512) NOT NULL,
    tempnamestrategy_id smallint NOT NULL,
    tempnamevalue character varying(24),
    CONSTRAINT ck_cdp_feederqueue01 CHECK ((((cache_path IS NOT NULL) AND (s3_reference IS NULL) AND (inv_id IS NULL)) OR ((cache_path IS NULL) AND (s3_reference IS NOT NULL) AND (inv_id IS NULL)) OR ((cache_path IS NULL) AND (s3_reference IS NULL) AND (inv_id IS NOT NULL))))
);


ALTER TABLE public.t_cdp_feederqueue OWNER TO acsdba;

--
-- Name: t_cdp_feederqueue_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_cdp_feederqueue_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_cdp_feederqueue_id_seq OWNER TO acsdba;

--
-- Name: t_cdp_feederqueue_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_cdp_feederqueue_id_seq OWNED BY public.t_cdp_feederqueue.id;


--
-- Name: t_cdp_notification_queue; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_cdp_notification_queue (
    id integer NOT NULL,
    feederqueue_id integer NOT NULL,
    uuid character varying(160),
    checksum jsonb,
    uri character varying(512) NOT NULL,
    creationdate timestamp(6) without time zone DEFAULT now() NOT NULL,
    modificationdate timestamp(6) without time zone,
    startdate timestamp(6) without time zone,
    completiondate timestamp(6) without time zone,
    retry_count integer DEFAULT 0,
    retry_time timestamp(6) without time zone,
    status_id smallint NOT NULL,
    jobresponsible character varying(64),
    notification_endpoint character varying(64)
);


ALTER TABLE public.t_cdp_notification_queue OWNER TO acsdba;

--
-- Name: t_cdp_notification_queue_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_cdp_notification_queue_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_cdp_notification_queue_id_seq OWNER TO acsdba;

--
-- Name: t_cdp_notification_queue_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_cdp_notification_queue_id_seq OWNED BY public.t_cdp_notification_queue.id;


--
-- Name: t_circ_callbacks; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_circ_callbacks (
    id integer NOT NULL,
    circ_queue_id bigint NOT NULL,
    url character varying(512) NOT NULL,
    external_id bigint NOT NULL
);


ALTER TABLE public.t_circ_callbacks OWNER TO acsdba;

--
-- Name: t_circ_callbacks_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_circ_callbacks_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_circ_callbacks_id_seq OWNER TO acsdba;

--
-- Name: t_circ_callbacks_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_circ_callbacks_id_seq OWNED BY public.t_circ_callbacks.id;


--
-- Name: t_circulation_remove_queue; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_circulation_remove_queue (
    id bigint NOT NULL,
    url character varying(512) NOT NULL,
    host_id integer
);


ALTER TABLE public.t_circulation_remove_queue OWNER TO acsdba;

--
-- Name: t_circulation_remove_queue_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_circulation_remove_queue_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_circulation_remove_queue_id_seq OWNER TO acsdba;

--
-- Name: t_circulation_remove_queue_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_circulation_remove_queue_id_seq OWNED BY public.t_circulation_remove_queue.id;


--
-- Name: t_circulationactivehosts; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_circulationactivehosts (
    host_id integer NOT NULL,
    tstamp timestamp(6) without time zone DEFAULT now() NOT NULL
);


ALTER TABLE public.t_circulationactivehosts OWNER TO acsdba;

--
-- Name: t_circulationmodes; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_circulationmodes (
    id smallint NOT NULL,
    circulationmode character varying(16) NOT NULL
);


ALTER TABLE public.t_circulationmodes OWNER TO acsdba;

--
-- Name: t_circulationpolicies; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_circulationpolicies (
    id integer NOT NULL,
    filetype_id integer NOT NULL,
    circulator_node_id integer NOT NULL,
    priority integer DEFAULT 0 NOT NULL,
    source_id integer NOT NULL,
    destination_id integer NOT NULL,
    removesource boolean DEFAULT false NOT NULL,
    agent_node_id integer,
    localtransfermode_id smallint,
    status_id smallint DEFAULT 43 NOT NULL,
    circ_policy_name character varying(64),
    CONSTRAINT ck_circulationpolicies01 CHECK ((source_id <> destination_id))
);


ALTER TABLE public.t_circulationpolicies OWNER TO acsdba;

--
-- Name: t_circulationpolicies_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_circulationpolicies_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_circulationpolicies_id_seq OWNER TO acsdba;

--
-- Name: t_circulationpolicies_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_circulationpolicies_id_seq OWNED BY public.t_circulationpolicies.id;


--
-- Name: t_circulationpolicies_monitor; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_circulationpolicies_monitor (
    id integer NOT NULL,
    circulationpolicy_id integer NOT NULL,
    monitoringflag boolean NOT NULL
);


ALTER TABLE public.t_circulationpolicies_monitor OWNER TO acsdba;

--
-- Name: TABLE t_circulationpolicies_monitor; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON TABLE public.t_circulationpolicies_monitor IS 'Contains the list of circulation policies to be monitored';


--
-- Name: COLUMN t_circulationpolicies_monitor.id; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_circulationpolicies_monitor.id IS 'Record identifier (Primary Key)';


--
-- Name: COLUMN t_circulationpolicies_monitor.circulationpolicy_id; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_circulationpolicies_monitor.circulationpolicy_id IS 'Circulation policy identifier. References the t_circulationpolicies table';


--
-- Name: COLUMN t_circulationpolicies_monitor.monitoringflag; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_circulationpolicies_monitor.monitoringflag IS 'Used for the monitoring of the circulation policy';


--
-- Name: t_circulationpolicies_monitor_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_circulationpolicies_monitor_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_circulationpolicies_monitor_id_seq OWNER TO acsdba;

--
-- Name: t_circulationpolicies_monitor_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_circulationpolicies_monitor_id_seq OWNED BY public.t_circulationpolicies_monitor.id;


--
-- Name: t_circulationsmedia; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_circulationsmedia (
    id integer NOT NULL,
    media_tag character varying(128) NOT NULL,
    status_id smallint NOT NULL,
    tstamp timestamp(6) without time zone DEFAULT now() NOT NULL,
    mount_point character varying(1024) NOT NULL,
    perc_copied_files integer DEFAULT 0 NOT NULL,
    source boolean DEFAULT true NOT NULL
);


ALTER TABLE public.t_circulationsmedia OWNER TO acsdba;

--
-- Name: t_circulationsmedia_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_circulationsmedia_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_circulationsmedia_id_seq OWNER TO acsdba;

--
-- Name: t_circulationsmedia_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_circulationsmedia_id_seq OWNED BY public.t_circulationsmedia.id;


--
-- Name: t_circulationsmedia_insmode; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_circulationsmedia_insmode (
    id smallint NOT NULL,
    mode character varying(24) NOT NULL
);


ALTER TABLE public.t_circulationsmedia_insmode OWNER TO acsdba;

--
-- Name: t_circulationsqueue; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_circulationsqueue (
    id bigint NOT NULL,
    startdate timestamp(6) without time zone,
    enddate timestamp(6) without time zone,
    priority integer DEFAULT 0 NOT NULL,
    remark character varying(512),
    status_id smallint DEFAULT 16 NOT NULL,
    creationdate timestamp(6) without time zone DEFAULT now(),
    modificationdate timestamp(6) without time zone DEFAULT now(),
    fromurl character varying(512) NOT NULL,
    tourl character varying(512) NOT NULL,
    relay_host_id integer,
    relay_activity_id bigint,
    retries smallint DEFAULT 0 NOT NULL,
    removesource boolean DEFAULT false NOT NULL,
    circulationmode_id smallint NOT NULL,
    host_id integer,
    circulationpolicy_id integer,
    dest_host_id integer,
    error character varying(512),
    tempnamevalue character varying(64),
    tempname_id smallint,
    nextretrydate timestamp(6) without time zone DEFAULT now(),
    dest_protocol_id smallint,
    media_circulation_id integer,
    media_insertionmode_id smallint DEFAULT 1 NOT NULL,
    localtransfermode_id smallint DEFAULT 1 NOT NULL,
    streamer_id integer,
    CONSTRAINT ck_circulationsqueue01 CHECK ((char_length(btrim((tempnamevalue)::text)) <> 0)),
    CONSTRAINT ck_circulationsqueue02 CHECK ((((tempname_id <> 2) AND (((tempnamevalue)::text = '..'::text) OR ((tempnamevalue)::text = '../'::text))) <> true)),
    CONSTRAINT ck_circulationsqueue03 CHECK ((((tempname_id = 3) AND (((tempnamevalue)::text = '.'::text) OR ((tempnamevalue)::text = './'::text))) <> true))
);


ALTER TABLE public.t_circulationsqueue OWNER TO acsdba;

--
-- Name: t_circulationsqueue_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_circulationsqueue_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_circulationsqueue_id_seq OWNER TO acsdba;

--
-- Name: t_circulationsqueue_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_circulationsqueue_id_seq OWNED BY public.t_circulationsqueue.id;


--
-- Name: t_circulationsqueuedwh; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_circulationsqueuedwh (
    id bigint NOT NULL,
    startdate timestamp(6) without time zone,
    enddate timestamp(6) without time zone,
    priority integer DEFAULT 0 NOT NULL,
    remark character varying(512),
    status_id smallint DEFAULT 16 NOT NULL,
    creationdate timestamp(6) without time zone DEFAULT now(),
    modificationdate timestamp(6) without time zone DEFAULT now(),
    fromurl character varying(512) NOT NULL,
    tourl character varying(512) NOT NULL,
    relay_host_id integer,
    relay_activity_id bigint,
    retries smallint DEFAULT 0 NOT NULL,
    removesource boolean DEFAULT false NOT NULL,
    circulationmode_id smallint NOT NULL,
    host_id integer,
    circulationpolicy_id integer,
    dest_host_id integer,
    error character varying(512),
    total_time double precision,
    datasize bigint,
    circulation_id bigint DEFAULT 0 NOT NULL,
    source_db_host character varying(256) DEFAULT 'UNKNOWN'::character varying NOT NULL
);


ALTER TABLE public.t_circulationsqueuedwh OWNER TO acsdba;

--
-- Name: t_circulationsqueuedwh_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_circulationsqueuedwh_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_circulationsqueuedwh_id_seq OWNER TO acsdba;

--
-- Name: t_circulationsqueuedwh_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_circulationsqueuedwh_id_seq OWNED BY public.t_circulationsqueuedwh.id;


--
-- Name: t_circulationstats; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_circulationstats (
    id bigint NOT NULL,
    creationdate timestamp(6) without time zone DEFAULT now(),
    from_host_id integer,
    to_host_id integer,
    name_lookup_time double precision,
    connection_time double precision,
    app_connection_time double precision,
    pre_transf_time double precision,
    start_transf_time double precision,
    total_time double precision,
    redirect_time double precision,
    circulationsqueue_id bigint,
    datasize bigint,
    unkn_from_host character varying(256),
    unkn_to_host character varying(256),
    CONSTRAINT ck_circulationstats01 CHECK ((((from_host_id IS NOT NULL) AND (unkn_from_host IS NULL)) OR ((from_host_id IS NULL) AND (unkn_from_host IS NOT NULL)))),
    CONSTRAINT ck_circulationstats02 CHECK ((((to_host_id IS NOT NULL) AND (unkn_to_host IS NULL)) OR ((to_host_id IS NULL) AND (unkn_to_host IS NOT NULL))))
);


ALTER TABLE public.t_circulationstats OWNER TO acsdba;

--
-- Name: t_circulationstats_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_circulationstats_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_circulationstats_id_seq OWNER TO acsdba;

--
-- Name: t_circulationstats_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_circulationstats_id_seq OWNED BY public.t_circulationstats.id;


--
-- Name: t_classifiedfiletypes; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_classifiedfiletypes (
    id integer NOT NULL,
    filetype character varying(16) NOT NULL
);


ALTER TABLE public.t_classifiedfiletypes OWNER TO acsdba;

--
-- Name: t_classifiedfiletypes_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_classifiedfiletypes_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_classifiedfiletypes_id_seq OWNER TO acsdba;

--
-- Name: t_classifiedfiletypes_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_classifiedfiletypes_id_seq OWNED BY public.t_classifiedfiletypes.id;


--
-- Name: t_cleanupalgos; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_cleanupalgos (
    id_cleanupalgo smallint NOT NULL,
    name character varying(24) NOT NULL,
    description character varying(128)
);


ALTER TABLE public.t_cleanupalgos OWNER TO acsdba;

--
-- Name: t_cleanuprules; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_cleanuprules (
    id_cleanuprule integer NOT NULL,
    repository_id integer NOT NULL,
    cleanupalgo_id smallint NOT NULL,
    hiquota_mb bigint NOT NULL,
    lowquota_mb bigint NOT NULL,
    age_h integer,
    CONSTRAINT ck_cleanuprules01 CHECK ((hiquota_mb >= lowquota_mb)),
    CONSTRAINT ck_cleanuprules02 CHECK ((lowquota_mb >= 0)),
    CONSTRAINT ck_cleanuprules03 CHECK ((age_h > 0))
);


ALTER TABLE public.t_cleanuprules OWNER TO acsdba;

--
-- Name: t_cleanuprules_id_cleanuprule_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_cleanuprules_id_cleanuprule_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_cleanuprules_id_cleanuprule_seq OWNER TO acsdba;

--
-- Name: t_cleanuprules_id_cleanuprule_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_cleanuprules_id_cleanuprule_seq OWNED BY public.t_cleanuprules.id_cleanuprule;


--
-- Name: t_completed_orders; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_completed_orders (
    order_id integer NOT NULL
);


ALTER TABLE public.t_completed_orders OWNER TO acsdba;

--
-- Name: t_configurationschemas; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_configurationschemas (
    namespace character varying(64) NOT NULL,
    version character varying(8) NOT NULL,
    schema text
);


ALTER TABLE public.t_configurationschemas OWNER TO acsdba;

--
-- Name: t_configurationspaces; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_configurationspaces (
    namespace character varying(64) NOT NULL,
    version character varying(8) NOT NULL,
    space text
);


ALTER TABLE public.t_configurationspaces OWNER TO acsdba;

--
-- Name: t_configurationstylesheets; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_configurationstylesheets (
    id integer NOT NULL,
    name character varying(64) NOT NULL,
    version character varying(8) NOT NULL,
    stylesheet text
);


ALTER TABLE public.t_configurationstylesheets OWNER TO acsdba;

--
-- Name: t_configurationstylesheets_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_configurationstylesheets_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_configurationstylesheets_id_seq OWNER TO acsdba;

--
-- Name: t_configurationstylesheets_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_configurationstylesheets_id_seq OWNED BY public.t_configurationstylesheets.id;


--
-- Name: t_cov_periods; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_cov_periods (
    id_interval bigint NOT NULL,
    coveragerule_id integer,
    t_begin timestamp(6) without time zone NOT NULL,
    t_end timestamp(6) without time zone NOT NULL,
    exp_val_start timestamp without time zone NOT NULL,
    exp_val_stop timestamp without time zone NOT NULL,
    events integer DEFAULT 0 NOT NULL,
    cover_pct double precision DEFAULT 0 NOT NULL,
    creationdate timestamp(6) without time zone DEFAULT now() NOT NULL,
    reported boolean DEFAULT false NOT NULL
);


ALTER TABLE public.t_cov_periods OWNER TO acsdba;

--
-- Name: TABLE t_cov_periods; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON TABLE public.t_cov_periods IS 'Contains the list of planned periods, shows the events that occurred in a period and the rule to which the period is associated';


--
-- Name: COLUMN t_cov_periods.id_interval; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_cov_periods.id_interval IS 'Record identifier (Primary Key autoincrement)';


--
-- Name: COLUMN t_cov_periods.coveragerule_id; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_cov_periods.coveragerule_id IS 'Id of the referenced rule. References id_coveragerule on t_cov_rules table.';


--
-- Name: COLUMN t_cov_periods.t_begin; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_cov_periods.t_begin IS 'Begin time of the period';


--
-- Name: COLUMN t_cov_periods.t_end; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_cov_periods.t_end IS 'End time of the period';


--
-- Name: COLUMN t_cov_periods.exp_val_start; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_cov_periods.exp_val_start IS 'Expected validity start';


--
-- Name: COLUMN t_cov_periods.exp_val_stop; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_cov_periods.exp_val_stop IS 'Expected validity stop';


--
-- Name: COLUMN t_cov_periods.events; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_cov_periods.events IS 'Counter of the occurred events';


--
-- Name: COLUMN t_cov_periods.cover_pct; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_cov_periods.cover_pct IS 'Percentage of coverage of the validity interval';


--
-- Name: COLUMN t_cov_periods.creationdate; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_cov_periods.creationdate IS 'Datetime that represents when the record was inserted. Is automatically generated by a default to "now"';


--
-- Name: COLUMN t_cov_periods.reported; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_cov_periods.reported IS 'Flag that declares if the period has been reported';


--
-- Name: t_cov_periods_id_interval_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_cov_periods_id_interval_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_cov_periods_id_interval_seq OWNER TO acsdba;

--
-- Name: t_cov_periods_id_interval_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_cov_periods_id_interval_seq OWNED BY public.t_cov_periods.id_interval;


--
-- Name: SEQUENCE t_cov_periods_id_interval_seq; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON SEQUENCE public.t_cov_periods_id_interval_seq IS 'Used to generate the primary keys of the t_cov_periods table';


--
-- Name: t_cov_relevancies; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_cov_relevancies (
    id_relevancy bigint NOT NULL,
    period_id bigint NOT NULL,
    inv_id bigint NOT NULL,
    inv_val_start timestamp without time zone NOT NULL,
    inv_val_stop timestamp without time zone NOT NULL,
    creationdate timestamp(6) without time zone DEFAULT now() NOT NULL
);


ALTER TABLE public.t_cov_relevancies OWNER TO acsdba;

--
-- Name: TABLE t_cov_relevancies; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON TABLE public.t_cov_relevancies IS 'Contains the list of files per period. This table is used only by stored procedures to calculate the coverage percentage';


--
-- Name: COLUMN t_cov_relevancies.id_relevancy; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_cov_relevancies.id_relevancy IS 'Record identifier (Primary Key autoincrement)';


--
-- Name: COLUMN t_cov_relevancies.period_id; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_cov_relevancies.period_id IS 'Id of the referenced period. References id_interval on t_cov_periods table.';


--
-- Name: COLUMN t_cov_relevancies.inv_id; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_cov_relevancies.inv_id IS 'Id of the referenced file to analyze. References id_inv on t_inventory table.';


--
-- Name: COLUMN t_cov_relevancies.inv_val_start; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_cov_relevancies.inv_val_start IS 'ValidityStart of the file to analyze';


--
-- Name: COLUMN t_cov_relevancies.inv_val_stop; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_cov_relevancies.inv_val_stop IS 'ValidityStop of the file to analyze';


--
-- Name: COLUMN t_cov_relevancies.creationdate; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_cov_relevancies.creationdate IS 'Datetime that represents when the record was inserted. Is automatically generated by a default to "now"';


--
-- Name: t_cov_relevancies_id_relevancy_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_cov_relevancies_id_relevancy_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_cov_relevancies_id_relevancy_seq OWNER TO acsdba;

--
-- Name: t_cov_relevancies_id_relevancy_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_cov_relevancies_id_relevancy_seq OWNED BY public.t_cov_relevancies.id_relevancy;


--
-- Name: SEQUENCE t_cov_relevancies_id_relevancy_seq; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON SEQUENCE public.t_cov_relevancies_id_relevancy_seq IS 'Used to generate the primary keys of the t_cov_relevancies table';


--
-- Name: t_cov_rules; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_cov_rules (
    id_coveragerule integer NOT NULL,
    rule_type character(1) NOT NULL,
    time_0 timestamp(6) without time zone NOT NULL,
    t_sec integer NOT NULL,
    int_a integer DEFAULT 1 NOT NULL,
    int_b integer DEFAULT 0 NOT NULL,
    n_min integer DEFAULT 1 NOT NULL,
    n_max integer DEFAULT 1 NOT NULL,
    delta_gap integer DEFAULT 0 NOT NULL,
    cover_pct double precision DEFAULT 0 NOT NULL,
    check_delta_gap boolean DEFAULT false NOT NULL,
    check_cover_pct boolean DEFAULT false NOT NULL,
    deltawait_sec integer DEFAULT 0 NOT NULL,
    deltamissing_sec integer,
    creation_date timestamp(6) without time zone DEFAULT now() NOT NULL,
    last_update timestamp(6) without time zone DEFAULT now() NOT NULL,
    author character varying(24) NOT NULL,
    description character varying(128),
    fixedvaliditystart timestamp(6) without time zone,
    fixedvaliditystop timestamp(6) without time zone,
    CONSTRAINT ck_coveragerules01 CHECK ((rule_type = ANY (ARRAY['A'::bpchar, 'B'::bpchar, 'C'::bpchar]))),
    CONSTRAINT ck_coveragerules02 CHECK ((int_a > int_b)),
    CONSTRAINT ck_coveragerules03 CHECK (((n_min > 0) AND (n_max > 0))),
    CONSTRAINT ck_coveragerules04 CHECK (((cover_pct >= (0)::double precision) AND (cover_pct <= (100)::double precision))),
    CONSTRAINT ck_coveragerules05 CHECK ((NOT ((fixedvaliditystart IS NOT NULL) AND (fixedvaliditystop IS NOT NULL))))
);


ALTER TABLE public.t_cov_rules OWNER TO acsdba;

--
-- Name: TABLE t_cov_rules; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON TABLE public.t_cov_rules IS 'Contains the list of rules and their configurations';


--
-- Name: COLUMN t_cov_rules.id_coveragerule; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_cov_rules.id_coveragerule IS 'Record identifier (Primary Key autoincrement)';


--
-- Name: COLUMN t_cov_rules.rule_type; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_cov_rules.rule_type IS 'Type of rule. Can be only A, B or C';


--
-- Name: COLUMN t_cov_rules.time_0; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_cov_rules.time_0 IS 'Datetime that represents the beginning of the first period';


--
-- Name: COLUMN t_cov_rules.t_sec; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_cov_rules.t_sec IS 'Width of each period. The values is expressed in seconds. For C type rules this value is 0';


--
-- Name: COLUMN t_cov_rules.int_a; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_cov_rules.int_a IS 'Interval A. Is the number that will be used to generate the period validity start. The value is expressed in seconds. For C type rules this value is 0';


--
-- Name: COLUMN t_cov_rules.int_b; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_cov_rules.int_b IS 'Interval B. Is the number that will be used to generate the period validity stop. The value is expressed in seconds and must be less than int_a. For C type rules this value is -1';


--
-- Name: COLUMN t_cov_rules.n_min; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_cov_rules.n_min IS 'Minimum number of expected file for a period. The value must be greater than 0. For A and C type rules this value is 1';


--
-- Name: COLUMN t_cov_rules.n_max; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_cov_rules.n_max IS 'Maximum number of expected file for a period. The value must be greater than n_min. For A and C type rules this value is 1';


--
-- Name: COLUMN t_cov_rules.delta_gap; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_cov_rules.delta_gap IS 'Allowable range for the gap between consecutive files. The value is expressed in seconds. For A and C type rules this value is 0';


--
-- Name: COLUMN t_cov_rules.cover_pct; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_cov_rules.cover_pct IS 'Expected percentage of coverage for a period. The default value is zero';


--
-- Name: COLUMN t_cov_rules.check_delta_gap; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_cov_rules.check_delta_gap IS 'Flag that indicate if the check for delta_gap is needed. The default value is false';


--
-- Name: COLUMN t_cov_rules.check_cover_pct; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_cov_rules.check_cover_pct IS 'Flag that indicate if the check for delta_pct is needed. The default value is false';


--
-- Name: COLUMN t_cov_rules.deltawait_sec; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_cov_rules.deltawait_sec IS 'Tolerance interval. The values is expressed in seconds';


--
-- Name: COLUMN t_cov_rules.deltamissing_sec; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_cov_rules.deltamissing_sec IS 'Indicates how long, after the end of an interval, you can wait before declare an item lost. The value is expressed in seconds';


--
-- Name: COLUMN t_cov_rules.creation_date; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_cov_rules.creation_date IS 'Datetime that represents when the rule was created. Is automatically generated by a default to "now"';


--
-- Name: COLUMN t_cov_rules.last_update; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_cov_rules.last_update IS 'Datetime that represents the last change of the rule. Is automatically generated by a default to "now"';


--
-- Name: COLUMN t_cov_rules.author; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_cov_rules.author IS 'Name of the user who created the rule';


--
-- Name: COLUMN t_cov_rules.description; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_cov_rules.description IS 'Description of the rule. Can be empty';


--
-- Name: COLUMN t_cov_rules.fixedvaliditystart; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_cov_rules.fixedvaliditystart IS 'For files having validity from the beginning of the mission.';


--
-- Name: COLUMN t_cov_rules.fixedvaliditystop; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_cov_rules.fixedvaliditystop IS 'For files having validity until the end of the mission.';


--
-- Name: t_cov_rules_id_coveragerule_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_cov_rules_id_coveragerule_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_cov_rules_id_coveragerule_seq OWNER TO acsdba;

--
-- Name: t_cov_rules_id_coveragerule_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_cov_rules_id_coveragerule_seq OWNED BY public.t_cov_rules.id_coveragerule;


--
-- Name: SEQUENCE t_cov_rules_id_coveragerule_seq; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON SEQUENCE public.t_cov_rules_id_coveragerule_seq IS 'Used to generate the primary keys of the t_cov_rules table';


--
-- Name: t_cov_x_filetypes; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_cov_x_filetypes (
    coveragerule_id integer NOT NULL,
    filetype_id integer NOT NULL
);


ALTER TABLE public.t_cov_x_filetypes OWNER TO acsdba;

--
-- Name: TABLE t_cov_x_filetypes; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON TABLE public.t_cov_x_filetypes IS 'Contains the relations between filetpes and rules. Only the filetypes listed into this table will be analyzed by functions.';


--
-- Name: COLUMN t_cov_x_filetypes.coveragerule_id; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_cov_x_filetypes.coveragerule_id IS 'Id of referenced rule. References id_coveragerule on t_cov_rules table.';


--
-- Name: COLUMN t_cov_x_filetypes.filetype_id; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_cov_x_filetypes.filetype_id IS 'Id of referenced file type. References id_filetype on t_filetypes table.';


--
-- Name: t_cr_x_filetypes; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_cr_x_filetypes (
    id_cr_x_filetype integer NOT NULL,
    crgroup_id integer NOT NULL,
    filetype_id integer NOT NULL
);


ALTER TABLE public.t_cr_x_filetypes OWNER TO acsdba;

--
-- Name: t_cr_x_filetypes_id_cr_x_filetype_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_cr_x_filetypes_id_cr_x_filetype_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_cr_x_filetypes_id_cr_x_filetype_seq OWNER TO acsdba;

--
-- Name: t_cr_x_filetypes_id_cr_x_filetype_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_cr_x_filetypes_id_cr_x_filetype_seq OWNED BY public.t_cr_x_filetypes.id_cr_x_filetype;


--
-- Name: t_crgroups; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_crgroups (
    id_crgroup integer NOT NULL,
    name character varying(24) NOT NULL,
    mailaddresses character varying(1024),
    description character varying(128),
    virtual_filetype_id integer
);


ALTER TABLE public.t_crgroups OWNER TO acsdba;

--
-- Name: t_crgroups_id_crgroup_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_crgroups_id_crgroup_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_crgroups_id_crgroup_seq OWNER TO acsdba;

--
-- Name: t_crgroups_id_crgroup_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_crgroups_id_crgroup_seq OWNED BY public.t_crgroups.id_crgroup;


--
-- Name: t_crhistory; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_crhistory (
    inventory_id bigint NOT NULL,
    crgroup_id integer NOT NULL,
    tstamp timestamp(6) without time zone NOT NULL,
    author character varying(24) NOT NULL,
    crtag character varying(24),
    crcomment character varying(128)
);


ALTER TABLE public.t_crhistory OWNER TO acsdba;

--
-- Name: t_crrelations; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_crrelations (
    id_crrelation integer NOT NULL,
    crgroup_id integer NOT NULL,
    repository_id integer NOT NULL,
    distributionrule_id integer NOT NULL
);


ALTER TABLE public.t_crrelations OWNER TO acsdba;

--
-- Name: t_crrelations_id_crrelation_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_crrelations_id_crrelation_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_crrelations_id_crrelation_seq OWNER TO acsdba;

--
-- Name: t_crrelations_id_crrelation_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_crrelations_id_crrelation_seq OWNED BY public.t_crrelations.id_crrelation;


--
-- Name: t_crtemporaries; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_crtemporaries (
    inventory_id bigint NOT NULL,
    crgroup_id integer NOT NULL,
    tstamp timestamp(6) without time zone NOT NULL,
    author character varying(24) NOT NULL,
    crtag character varying(24),
    crcomment character varying(128)
);


ALTER TABLE public.t_crtemporaries OWNER TO acsdba;

--
-- Name: t_datastoragequeue; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_datastoragequeue (
    id bigint NOT NULL,
    status_id smallint NOT NULL,
    type_id smallint NOT NULL,
    client_name character varying(256),
    md5 character(32),
    startdate timestamp(6) without time zone DEFAULT now(),
    enddate timestamp(6) without time zone,
    check_date timestamp(6) without time zone DEFAULT now() NOT NULL,
    flowtag_id integer DEFAULT 1 NOT NULL
);


ALTER TABLE public.t_datastoragequeue OWNER TO acsdba;

--
-- Name: t_datastoragequeue_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_datastoragequeue_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_datastoragequeue_id_seq OWNER TO acsdba;

--
-- Name: t_datastoragequeue_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_datastoragequeue_id_seq OWNED BY public.t_datastoragequeue.id;


--
-- Name: t_datastoragetypes; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_datastoragetypes (
    id smallint NOT NULL,
    datastoragetype character varying(16) NOT NULL
);


ALTER TABLE public.t_datastoragetypes OWNER TO acsdba;

--
-- Name: t_dbparameters; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_dbparameters (
    id integer NOT NULL,
    parameter character varying(32) NOT NULL,
    parametertype_id smallint NOT NULL,
    value_int integer,
    value_str character varying(32),
    value_dttm timestamp(6) without time zone,
    value_bool boolean,
    param_description text
);


ALTER TABLE public.t_dbparameters OWNER TO acsdba;

--
-- Name: t_dbparameters_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_dbparameters_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_dbparameters_id_seq OWNER TO acsdba;

--
-- Name: t_dbparameters_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_dbparameters_id_seq OWNED BY public.t_dbparameters.id;


--
-- Name: t_dbparametertypes; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_dbparametertypes (
    id smallint NOT NULL,
    parametertype character varying(16) NOT NULL
);


ALTER TABLE public.t_dbparametertypes OWNER TO acsdba;

--
-- Name: t_dbstatus; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_dbstatus (
    master boolean NOT NULL,
    modifycationdate timestamp(6) without time zone DEFAULT now()
);


ALTER TABLE public.t_dbstatus OWNER TO acsdba;

--
-- Name: t_discacheinvrelation; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_discacheinvrelation (
    id integer NOT NULL,
    path character varying(512) NOT NULL
);


ALTER TABLE public.t_discacheinvrelation OWNER TO acsdba;

--
-- Name: t_discoveryplugins; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_discoveryplugins (
    id_discoveryplugin integer NOT NULL,
    name character varying(24) NOT NULL
);


ALTER TABLE public.t_discoveryplugins OWNER TO acsdba;

--
-- Name: t_discoveryplugins_id_discoveryplugin_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_discoveryplugins_id_discoveryplugin_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_discoveryplugins_id_discoveryplugin_seq OWNER TO acsdba;

--
-- Name: t_discoveryplugins_id_discoveryplugin_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_discoveryplugins_id_discoveryplugin_seq OWNED BY public.t_discoveryplugins.id_discoveryplugin;


--
-- Name: t_disseminationpackingalgos; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_disseminationpackingalgos (
    id_disseminationpackingalgo integer NOT NULL,
    factorykey character varying(64) NOT NULL,
    description character varying(128)
);


ALTER TABLE public.t_disseminationpackingalgos OWNER TO acsdba;

--
-- Name: t_disseminationpackingalgos_id_disseminationpackingalgo_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_disseminationpackingalgos_id_disseminationpackingalgo_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_disseminationpackingalgos_id_disseminationpackingalgo_seq OWNER TO acsdba;

--
-- Name: t_disseminationpackingalgos_id_disseminationpackingalgo_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_disseminationpackingalgos_id_disseminationpackingalgo_seq OWNED BY public.t_disseminationpackingalgos.id_disseminationpackingalgo;


--
-- Name: t_distpoldataset; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_distpoldataset (
    policy integer NOT NULL,
    dataset integer NOT NULL,
    num integer NOT NULL,
    CONSTRAINT ck_distpoldataset CHECK ((num > 0))
);


ALTER TABLE public.t_distpoldataset OWNER TO acsdba;

--
-- Name: t_distributionalgo; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_distributionalgo (
    id smallint NOT NULL,
    name character varying(64) NOT NULL,
    on_demand boolean DEFAULT true NOT NULL
);


ALTER TABLE public.t_distributionalgo OWNER TO acsdba;

--
-- Name: t_distributionformat; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_distributionformat (
    format character varying(16) NOT NULL,
    description character varying(64)
);


ALTER TABLE public.t_distributionformat OWNER TO acsdba;

--
-- Name: t_distributionitems; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_distributionitems (
    id integer NOT NULL,
    duedate timestamp(6) without time zone NOT NULL,
    distributeddate timestamp(6) without time zone,
    creationdate timestamp(6) without time zone DEFAULT now(),
    packageid integer,
    filename character varying(128) NOT NULL,
    fileversion character varying(32) NOT NULL,
    mediatype character varying(16) NOT NULL,
    userid character varying(32) NOT NULL,
    format integer,
    cartid integer,
    subreq integer,
    distributionrule_id integer NOT NULL,
    disseminationpackingalgo_id integer NOT NULL
);


ALTER TABLE public.t_distributionitems OWNER TO acsdba;

--
-- Name: t_distributionitems_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_distributionitems_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_distributionitems_id_seq OWNER TO acsdba;

--
-- Name: t_distributionitems_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_distributionitems_id_seq OWNED BY public.t_distributionitems.id;


--
-- Name: t_distributionqueue; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_distributionqueue (
    packageid integer NOT NULL,
    status character varying(32) NOT NULL,
    distributeddate timestamp(6) without time zone,
    jobresponsible character varying(64),
    creationdate timestamp(6) without time zone DEFAULT now() NOT NULL,
    format integer,
    retrycounter integer DEFAULT 0 NOT NULL,
    delserviceparam text,
    lastattempt timestamp(6) without time zone,
    circulationqueue_id bigint
);


ALTER TABLE public.t_distributionqueue OWNER TO acsdba;

--
-- Name: t_distributionqueue_packageid_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_distributionqueue_packageid_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_distributionqueue_packageid_seq OWNER TO acsdba;

--
-- Name: t_distributionqueue_packageid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_distributionqueue_packageid_seq OWNED BY public.t_distributionqueue.packageid;


--
-- Name: t_distributionrules; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_distributionrules (
    id_distributionrule integer NOT NULL,
    active boolean DEFAULT true NOT NULL,
    filetype_id integer NOT NULL,
    mediatype_id smallint NOT NULL,
    maxdaytodeliver integer DEFAULT 1 NOT NULL,
    failureaddresses character varying(1024),
    successaddresses character varying(1024),
    connretries integer DEFAULT 1 NOT NULL,
    retryperiod_sec integer NOT NULL,
    repository_id integer NOT NULL,
    user_id integer NOT NULL,
    usermode_id smallint DEFAULT 1 NOT NULL,
    distributionalgo_id smallint DEFAULT 1 NOT NULL,
    format_tt_id integer DEFAULT public.f_get_tasktable_id('defaultDistribFormat'::character varying) NOT NULL,
    delivery_tt_id integer DEFAULT public.f_get_tasktable_id('defaultDelivery'::character varying) NOT NULL,
    delcheck_tt_id integer DEFAULT public.f_get_tasktable_id('nopDeliveryCheck'::character varying) NOT NULL,
    fileclass_id integer,
    disseminationpackingalgo_id integer NOT NULL,
    ackcheckperiod_sec integer DEFAULT 60 NOT NULL,
    parameters text,
    priority integer DEFAULT 1 NOT NULL,
    ondemand boolean DEFAULT false NOT NULL,
    CONSTRAINT ck_distributionrules01 CHECK ((retryperiod_sec > 0))
);


ALTER TABLE public.t_distributionrules OWNER TO acsdba;

--
-- Name: t_distributionrules_id_distributionrule_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_distributionrules_id_distributionrule_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_distributionrules_id_distributionrule_seq OWNER TO acsdba;

--
-- Name: t_distributionrules_id_distributionrule_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_distributionrules_id_distributionrule_seq OWNED BY public.t_distributionrules.id_distributionrule;


--
-- Name: t_distributionruleslock; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_distributionruleslock (
    packageid integer NOT NULL,
    responsible character varying(128) NOT NULL,
    tstamp timestamp(6) without time zone DEFAULT now() NOT NULL
);


ALTER TABLE public.t_distributionruleslock OWNER TO acsdba;

--
-- Name: t_distributionwaitingitems; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_distributionwaitingitems (
    id integer NOT NULL,
    distributionitem_id integer NOT NULL,
    package_id integer NOT NULL,
    inventory_id bigint NOT NULL
);


ALTER TABLE public.t_distributionwaitingitems OWNER TO acsdba;

--
-- Name: t_distributionwaitingitems_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_distributionwaitingitems_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_distributionwaitingitems_id_seq OWNER TO acsdba;

--
-- Name: t_distributionwaitingitems_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_distributionwaitingitems_id_seq OWNED BY public.t_distributionwaitingitems.id;


--
-- Name: t_distruleoriginator; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_distruleoriginator (
    distributionrule_id integer NOT NULL,
    originator_id integer NOT NULL
);


ALTER TABLE public.t_distruleoriginator OWNER TO acsdba;

--
-- Name: t_downlink_session_tracks; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_downlink_session_tracks (
    id integer NOT NULL,
    session_id character varying(64) NOT NULL,
    downlink_stop_time timestamp(6) without time zone
);


ALTER TABLE public.t_downlink_session_tracks OWNER TO acsdba;

--
-- Name: t_downlink_session_tracks_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_downlink_session_tracks_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_downlink_session_tracks_id_seq OWNER TO acsdba;

--
-- Name: t_downlink_session_tracks_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_downlink_session_tracks_id_seq OWNED BY public.t_downlink_session_tracks.id;


--
-- Name: t_dsqueue_x_cirqueue; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_dsqueue_x_cirqueue (
    id bigint NOT NULL,
    datastoragequeue_id bigint NOT NULL,
    circulation_id bigint,
    inv_filename character varying(128) NOT NULL,
    error character varying(512),
    parent character varying(128),
    globalstatus_id smallint DEFAULT 7 NOT NULL,
    priority integer DEFAULT 0 NOT NULL,
    tourl character varying(512),
    fromurl character varying(512),
    circ_status_id smallint,
    circ_status_mod_date timestamp(6) without time zone DEFAULT now() NOT NULL,
    localtransfermode_id smallint DEFAULT 1 NOT NULL,
    filequeue_id bigint NOT NULL
);


ALTER TABLE public.t_dsqueue_x_cirqueue OWNER TO acsdba;

--
-- Name: t_dsqueue_x_cirqueue_filequeue_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_dsqueue_x_cirqueue_filequeue_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_dsqueue_x_cirqueue_filequeue_id_seq OWNER TO acsdba;

--
-- Name: t_dsqueue_x_cirqueue_filequeue_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_dsqueue_x_cirqueue_filequeue_id_seq OWNED BY public.t_dsqueue_x_cirqueue.filequeue_id;


--
-- Name: t_dsqueue_x_cirqueue_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_dsqueue_x_cirqueue_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_dsqueue_x_cirqueue_id_seq OWNER TO acsdba;

--
-- Name: t_dsqueue_x_cirqueue_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_dsqueue_x_cirqueue_id_seq OWNED BY public.t_dsqueue_x_cirqueue.id;


--
-- Name: t_emitted_order_events; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_emitted_order_events (
    id integer NOT NULL,
    event_time timestamp without time zone NOT NULL,
    event_tag character varying(64) NOT NULL,
    time_driven_start timestamp without time zone NOT NULL,
    time_driven_stop timestamp without time zone NOT NULL,
    processor_id integer NOT NULL,
    order_type character varying(8) NOT NULL,
    creation_date timestamp without time zone DEFAULT now()
);


ALTER TABLE public.t_emitted_order_events OWNER TO acsdba;

--
-- Name: t_emitted_order_events_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_emitted_order_events_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_emitted_order_events_id_seq OWNER TO acsdba;

--
-- Name: t_emitted_order_events_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_emitted_order_events_id_seq OWNED BY public.t_emitted_order_events.id;


--
-- Name: t_fileclasses; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_fileclasses (
    id_fileclass integer NOT NULL,
    fileclass character varying(8) NOT NULL,
    description character varying(64)
);


ALTER TABLE public.t_fileclasses OWNER TO acsdba;

--
-- Name: t_fileclasses_id_fileclass_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_fileclasses_id_fileclass_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_fileclasses_id_fileclass_seq OWNER TO acsdba;

--
-- Name: t_fileclasses_id_fileclass_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_fileclasses_id_fileclass_seq OWNED BY public.t_fileclasses.id_fileclass;


--
-- Name: t_filetypes_id_filetype_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_filetypes_id_filetype_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_filetypes_id_filetype_seq OWNER TO acsdba;

--
-- Name: t_filetypes_id_filetype_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_filetypes_id_filetype_seq OWNED BY public.t_filetypes.id_filetype;


--
-- Name: t_filetypes_last_inv; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_filetypes_last_inv (
    id integer NOT NULL,
    filetype character varying(16) NOT NULL,
    last_valid timestamp(6) without time zone DEFAULT now() NOT NULL
);


ALTER TABLE public.t_filetypes_last_inv OWNER TO acsdba;

--
-- Name: t_filetypes_last_inv_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_filetypes_last_inv_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_filetypes_last_inv_id_seq OWNER TO acsdba;

--
-- Name: t_filetypes_last_inv_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_filetypes_last_inv_id_seq OWNED BY public.t_filetypes_last_inv.id;


--
-- Name: t_filetypes_x_migrations; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_filetypes_x_migrations (
    id_filetypemigration integer NOT NULL,
    sm_id integer NOT NULL,
    filetype_id integer NOT NULL,
    migrationalgo_id integer NOT NULL,
    parameters text
);


ALTER TABLE public.t_filetypes_x_migrations OWNER TO acsdba;

--
-- Name: t_filetypes_x_migrations_id_filetypemigration_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_filetypes_x_migrations_id_filetypemigration_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_filetypes_x_migrations_id_filetypemigration_seq OWNER TO acsdba;

--
-- Name: t_filetypes_x_migrations_id_filetypemigration_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_filetypes_x_migrations_id_filetypemigration_seq OWNED BY public.t_filetypes_x_migrations.id_filetypemigration;


--
-- Name: t_filetypes_x_satellites; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_filetypes_x_satellites (
    id integer NOT NULL,
    filetype character varying(16) NOT NULL,
    satellite_id smallint NOT NULL
);


ALTER TABLE public.t_filetypes_x_satellites OWNER TO acsdba;

--
-- Name: t_filetypes_x_satellites_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_filetypes_x_satellites_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_filetypes_x_satellites_id_seq OWNER TO acsdba;

--
-- Name: t_filetypes_x_satellites_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_filetypes_x_satellites_id_seq OWNED BY public.t_filetypes_x_satellites.id;


--
-- Name: t_filetypes_x_stylesheets; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_filetypes_x_stylesheets (
    id integer NOT NULL,
    filetype_id integer NOT NULL,
    stylesheet_id integer NOT NULL,
    outputtype_id smallint NOT NULL,
    fileextension character varying(16)
);


ALTER TABLE public.t_filetypes_x_stylesheets OWNER TO acsdba;

--
-- Name: TABLE t_filetypes_x_stylesheets; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON TABLE public.t_filetypes_x_stylesheets IS 'Contains the list of relations between a filetype, the stylesheet and the output type, for the statistical reprts visualization';


--
-- Name: COLUMN t_filetypes_x_stylesheets.id; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_filetypes_x_stylesheets.id IS 'Record identifier (Primary Key)';


--
-- Name: COLUMN t_filetypes_x_stylesheets.filetype_id; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_filetypes_x_stylesheets.filetype_id IS 'Id of referenced filetype (t_filetypes)';


--
-- Name: COLUMN t_filetypes_x_stylesheets.stylesheet_id; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_filetypes_x_stylesheets.stylesheet_id IS 'Id of referenced stylesheet (t_configurationstylesheets)';


--
-- Name: COLUMN t_filetypes_x_stylesheets.outputtype_id; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_filetypes_x_stylesheets.outputtype_id IS 'Id of referenced output type (t_stylesheetoutputtypes)';


--
-- Name: COLUMN t_filetypes_x_stylesheets.fileextension; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_filetypes_x_stylesheets.fileextension IS 'File extension for generated report (eg. xml or EOF)';


--
-- Name: t_filetypes_x_stylesheets_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_filetypes_x_stylesheets_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_filetypes_x_stylesheets_id_seq OWNER TO acsdba;

--
-- Name: t_filetypes_x_stylesheets_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_filetypes_x_stylesheets_id_seq OWNED BY public.t_filetypes_x_stylesheets.id;


--
-- Name: SEQUENCE t_filetypes_x_stylesheets_id_seq; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON SEQUENCE public.t_filetypes_x_stylesheets_id_seq IS 'Used to generate the primary keys of the t_filetypes_x_stylesheets table';


--
-- Name: t_filetypescomps; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_filetypescomps (
    id integer NOT NULL,
    pfiletype integer NOT NULL,
    cfiletype integer NOT NULL,
    CONSTRAINT ck_filetypescomps01 CHECK ((pfiletype <> cfiletype))
);


ALTER TABLE public.t_filetypescomps OWNER TO acsdba;

--
-- Name: t_filetypescomps_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_filetypescomps_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_filetypescomps_id_seq OWNER TO acsdba;

--
-- Name: t_filetypescomps_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_filetypescomps_id_seq OWNED BY public.t_filetypescomps.id;


--
-- Name: t_flowtags; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_flowtags (
    id integer NOT NULL,
    flowtagname character varying(32) NOT NULL
);


ALTER TABLE public.t_flowtags OWNER TO acsdba;

--
-- Name: t_flowtags_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_flowtags_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_flowtags_id_seq OWNER TO acsdba;

--
-- Name: t_flowtags_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_flowtags_id_seq OWNED BY public.t_flowtags.id;


--
-- Name: t_inv_additional_params; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_inv_additional_params (
    id bigint NOT NULL,
    inv_id bigint NOT NULL,
    timeliness_key_id integer,
    granule_number integer,
    granule_position character varying(24),
    dump_start timestamp(6) without time zone,
    uuid character varying(160) DEFAULT public.uuid_generate_v1() NOT NULL,
    data_take_id integer
);


ALTER TABLE public.t_inv_additional_params OWNER TO acsdba;

--
-- Name: t_inv_additional_params_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_inv_additional_params_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_inv_additional_params_id_seq OWNER TO acsdba;

--
-- Name: t_inv_additional_params_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_inv_additional_params_id_seq OWNED BY public.t_inv_additional_params.id;


--
-- Name: t_inv_baselines; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_inv_baselines (
    id_inv_baseline integer NOT NULL,
    baseline character varying(24) NOT NULL
);


ALTER TABLE public.t_inv_baselines OWNER TO acsdba;

--
-- Name: t_inv_baselines_id_inv_baseline_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_inv_baselines_id_inv_baseline_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_inv_baselines_id_inv_baseline_seq OWNER TO acsdba;

--
-- Name: t_inv_baselines_id_inv_baseline_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_inv_baselines_id_inv_baseline_seq OWNED BY public.t_inv_baselines.id_inv_baseline;


--
-- Name: t_inv_client; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_inv_client (
    id_inv_client integer NOT NULL,
    name character varying(128) NOT NULL
);


ALTER TABLE public.t_inv_client OWNER TO acsdba;

--
-- Name: t_inv_client_id_inv_client_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_inv_client_id_inv_client_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_inv_client_id_inv_client_seq OWNER TO acsdba;

--
-- Name: t_inv_client_id_inv_client_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_inv_client_id_inv_client_seq OWNED BY public.t_inv_client.id_inv_client;


--
-- Name: t_inv_data_restored; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_inv_data_restored (
    id integer NOT NULL,
    sm_id integer NOT NULL,
    inv_id bigint NOT NULL,
    parent_inv_id bigint,
    filetype_id integer NOT NULL,
    size bigint NOT NULL,
    tstamp timestamp without time zone DEFAULT now()
);


ALTER TABLE public.t_inv_data_restored OWNER TO acsdba;

--
-- Name: t_inv_distributions; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_inv_distributions (
    inv_id bigint NOT NULL,
    retain_counter integer NOT NULL,
    last_update timestamp(6) without time zone DEFAULT now() NOT NULL
);


ALTER TABLE public.t_inv_distributions OWNER TO acsdba;

--
-- Name: t_inv_downloads; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_inv_downloads (
    inv_id bigint NOT NULL,
    client_id integer NOT NULL,
    downloadtime timestamp(6) without time zone DEFAULT now() NOT NULL
);


ALTER TABLE public.t_inv_downloads OWNER TO acsdba;

--
-- Name: t_inv_relation_types; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_inv_relation_types (
    id smallint NOT NULL,
    relation_type character varying(24) NOT NULL
);


ALTER TABLE public.t_inv_relation_types OWNER TO acsdba;

--
-- Name: t_inv_relations; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_inv_relations (
    id bigint NOT NULL,
    inv_id1 bigint NOT NULL,
    inv_id2 bigint,
    rel_type_id smallint NOT NULL,
    relation character varying(256)
);


ALTER TABLE public.t_inv_relations OWNER TO acsdba;

--
-- Name: t_inv_relations_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_inv_relations_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_inv_relations_id_seq OWNER TO acsdba;

--
-- Name: t_inv_relations_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_inv_relations_id_seq OWNED BY public.t_inv_relations.id;


--
-- Name: t_inv_x_baselines; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_inv_x_baselines (
    id_inv_x_baseline bigint NOT NULL,
    inv_id bigint NOT NULL,
    inv_baseline_id integer NOT NULL
);


ALTER TABLE public.t_inv_x_baselines OWNER TO acsdba;

--
-- Name: t_inv_x_baselines_id_inv_x_baseline_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_inv_x_baselines_id_inv_x_baseline_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_inv_x_baselines_id_inv_x_baseline_seq OWNER TO acsdba;

--
-- Name: t_inv_x_baselines_id_inv_x_baseline_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_inv_x_baselines_id_inv_x_baseline_seq OWNED BY public.t_inv_x_baselines.id_inv_x_baseline;


--
-- Name: t_inventory; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_inventory (
    id_inv bigint NOT NULL,
    filename character varying(128) NOT NULL,
    fileversion character varying(32) NOT NULL,
    unique_id character varying(160) DEFAULT public.uuid_generate_v1() NOT NULL,
    source character varying(128),
    sourceswversion character varying(8),
    generationtime timestamp(6) without time zone NOT NULL,
    validitystart timestamp(6) without time zone NOT NULL,
    validitystop timestamp(6) without time zone NOT NULL,
    startorbitnumber integer,
    stoporbitnumber integer,
    qualityinfo character varying(72),
    validityflag boolean NOT NULL,
    validationdate timestamp(6) without time zone NOT NULL,
    headersize bigint,
    datasize bigint,
    inventorydate timestamp(6) without time zone DEFAULT now() NOT NULL,
    modificationdate timestamp(6) without time zone DEFAULT now() NOT NULL,
    filetype character varying(16) NOT NULL,
    fileclass character varying(8) NOT NULL,
    orderid integer,
    sensorid smallint,
    acquisitionstationid smallint,
    processingstationid smallint,
    sensormode character varying(16),
    phase character varying(8),
    satelliteid smallint,
    ascendingflag boolean,
    equatorxlongitude double precision,
    equatorxtime timestamp(6) without time zone,
    mission smallint,
    qualified integer DEFAULT 1 NOT NULL,
    originator integer,
    qualifierfile character varying(128),
    band integer,
    tdi smallint,
    cloud_pctg double precision,
    prodspec character varying(128),
    dir_id bigint,
    ref_dir_id bigint,
    logical_id character varying(256),
    qualityvalue double precision,
    md5 character varying(32),
    attributes jsonb,
    geographiclocalization public.geography(Geometry,4326),
    CONSTRAINT ck_inventory CHECK ((validitystart <= validitystop)),
    CONSTRAINT ck_inventory02 CHECK (((cloud_pctg >= (0)::double precision) AND (cloud_pctg <= (100)::double precision)))
);


ALTER TABLE public.t_inventory OWNER TO acsdba;

--
-- Name: t_inventory_id_inv_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_inventory_id_inv_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_inventory_id_inv_seq OWNER TO acsdba;

--
-- Name: t_inventory_id_inv_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_inventory_id_inv_seq OWNED BY public.t_inventory.id_inv;


--
-- Name: t_inventoryanomalies; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_inventoryanomalies (
    id integer NOT NULL,
    filename character varying(128) NOT NULL,
    fileversion character varying(32) NOT NULL,
    tstamp timestamp(6) without time zone DEFAULT now() NOT NULL,
    message character varying(256) NOT NULL,
    anomaly_type_id smallint DEFAULT 1 NOT NULL
);


ALTER TABLE public.t_inventoryanomalies OWNER TO acsdba;

--
-- Name: t_inventoryanomalies_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_inventoryanomalies_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_inventoryanomalies_id_seq OWNER TO acsdba;

--
-- Name: t_inventoryanomalies_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_inventoryanomalies_id_seq OWNED BY public.t_inventoryanomalies.id;


--
-- Name: t_inventorycomments; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_inventorycomments (
    id_inventorycomment bigint NOT NULL,
    comment character varying(128) NOT NULL,
    date timestamp(6) without time zone DEFAULT now() NOT NULL,
    author character varying(32) NOT NULL,
    filename character varying(128) NOT NULL,
    fileversion character varying(32) NOT NULL
);


ALTER TABLE public.t_inventorycomments OWNER TO acsdba;

--
-- Name: t_inventorycomments_id_inventorycomment_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_inventorycomments_id_inventorycomment_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_inventorycomments_id_inventorycomment_seq OWNER TO acsdba;

--
-- Name: t_inventorycomments_id_inventorycomment_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_inventorycomments_id_inventorycomment_seq OWNED BY public.t_inventorycomments.id_inventorycomment;


--
-- Name: t_inventorydataset; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_inventorydataset (
    filename character varying(128) NOT NULL,
    fileversion character varying(32) NOT NULL,
    measdatasetid integer NOT NULL,
    count integer NOT NULL,
    CONSTRAINT ck_invdataset_count CHECK ((count > 0))
);


ALTER TABLE public.t_inventorydataset OWNER TO acsdba;

--
-- Name: t_inventoryevents; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_inventoryevents (
    id integer NOT NULL,
    filename character varying(128) NOT NULL,
    fileversion character varying(32) NOT NULL,
    modificationdate timestamp(6) without time zone DEFAULT now() NOT NULL,
    event character varying(16) NOT NULL
);


ALTER TABLE public.t_inventoryevents OWNER TO acsdba;

--
-- Name: t_inventoryevents_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_inventoryevents_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_inventoryevents_id_seq OWNER TO acsdba;

--
-- Name: t_inventoryevents_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_inventoryevents_id_seq OWNED BY public.t_inventoryevents.id;


--
-- Name: t_inventoryeventsconf; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_inventoryeventsconf (
    id integer NOT NULL,
    filetype character varying(16) NOT NULL,
    conf text
);


ALTER TABLE public.t_inventoryeventsconf OWNER TO acsdba;

--
-- Name: t_inventoryeventsconf_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_inventoryeventsconf_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_inventoryeventsconf_id_seq OWNER TO acsdba;

--
-- Name: t_inventoryeventsconf_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_inventoryeventsconf_id_seq OWNED BY public.t_inventoryeventsconf.id;


--
-- Name: t_inventoryextra; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_inventoryextra (
    inv_id bigint NOT NULL,
    extra text
);


ALTER TABLE public.t_inventoryextra OWNER TO acsdba;

--
-- Name: t_inventorygaps; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_inventorygaps (
    inventorygapid integer NOT NULL,
    filename character varying(128) NOT NULL,
    fileversion character varying(32) NOT NULL,
    begin timestamp(6) without time zone NOT NULL,
    "end" timestamp(6) without time zone NOT NULL
);


ALTER TABLE public.t_inventorygaps OWNER TO acsdba;

--
-- Name: t_inventorygaps_inventorygapid_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_inventorygaps_inventorygapid_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_inventorygaps_inventorygapid_seq OWNER TO acsdba;

--
-- Name: t_inventorygaps_inventorygapid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_inventorygaps_inventorygapid_seq OWNED BY public.t_inventorygaps.inventorygapid;


--
-- Name: t_inventorylinks; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_inventorylinks (
    id integer NOT NULL,
    orig_filename character varying(128) NOT NULL,
    orig_fileversion character varying(32) NOT NULL,
    dest_filename character varying(128) NOT NULL,
    dest_fileversion character varying(32) NOT NULL,
    linkdate timestamp(6) without time zone DEFAULT now() NOT NULL,
    description character varying(64),
    CONSTRAINT ck_invlinks_orig_dest CHECK ((((orig_filename)::text <> (dest_filename)::text) OR ((orig_fileversion)::text <> (dest_fileversion)::text)))
);


ALTER TABLE public.t_inventorylinks OWNER TO acsdba;

--
-- Name: COLUMN t_inventorylinks.id; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_inventorylinks.id IS 'Record identifier (Primary Key)';


--
-- Name: COLUMN t_inventorylinks.orig_filename; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_inventorylinks.orig_filename IS 'References filename on t_inventory table.';


--
-- Name: COLUMN t_inventorylinks.orig_fileversion; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_inventorylinks.orig_fileversion IS 'References fileversion on t_inventory table.';


--
-- Name: COLUMN t_inventorylinks.dest_filename; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_inventorylinks.dest_filename IS 'References filename on t_inventory table.';


--
-- Name: COLUMN t_inventorylinks.dest_fileversion; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_inventorylinks.dest_fileversion IS 'References fileversion on t_inventory table.';


--
-- Name: t_inventorylinks_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_inventorylinks_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_inventorylinks_id_seq OWNER TO acsdba;

--
-- Name: t_inventorylinks_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_inventorylinks_id_seq OWNED BY public.t_inventorylinks.id;


--
-- Name: SEQUENCE t_inventorylinks_id_seq; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON SEQUENCE public.t_inventorylinks_id_seq IS 'Used to generate the primary keys of the t_inventorylinks table';


--
-- Name: t_inventorylinksconf; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_inventorylinksconf (
    id integer NOT NULL,
    orig_filetype character varying(16) NOT NULL,
    dest_filetype character varying(16) NOT NULL,
    algotype character varying(24) NOT NULL,
    description character varying(64)
);


ALTER TABLE public.t_inventorylinksconf OWNER TO acsdba;

--
-- Name: COLUMN t_inventorylinksconf.id; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_inventorylinksconf.id IS 'Record identifier (Primary Key)';


--
-- Name: COLUMN t_inventorylinksconf.orig_filetype; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_inventorylinksconf.orig_filetype IS 'References filetype on t_filetypes table.';


--
-- Name: COLUMN t_inventorylinksconf.dest_filetype; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_inventorylinksconf.dest_filetype IS 'References filetype on t_filetypes table.';


--
-- Name: t_inventorylinksconf_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_inventorylinksconf_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_inventorylinksconf_id_seq OWNER TO acsdba;

--
-- Name: t_inventorylinksconf_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_inventorylinksconf_id_seq OWNED BY public.t_inventorylinksconf.id;


--
-- Name: SEQUENCE t_inventorylinksconf_id_seq; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON SEQUENCE public.t_inventorylinksconf_id_seq IS 'Used to generate the primary keys of the t_inventorylinksconf table';


--
-- Name: t_inventorysm; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_inventorysm (
    id integer NOT NULL,
    inv_id bigint,
    storagemanid integer NOT NULL,
    packageid integer,
    stored_date timestamp(6) without time zone DEFAULT now() NOT NULL,
    migration_report text
);


ALTER TABLE public.t_inventorysm OWNER TO acsdba;

--
-- Name: t_inventorysm_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_inventorysm_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_inventorysm_id_seq OWNER TO acsdba;

--
-- Name: t_inventorysm_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_inventorysm_id_seq OWNED BY public.t_inventorysm.id;


--
-- Name: t_invrollingpolicies; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_invrollingpolicies (
    id integer NOT NULL,
    filetype character varying(16) NOT NULL,
    latencyhours integer NOT NULL,
    removeevendistribution boolean NOT NULL,
    removeevenused boolean NOT NULL,
    action integer NOT NULL,
    algorithm integer NOT NULL,
    parameters text,
    priority integer NOT NULL,
    fileclass character varying(8),
    storageid integer,
    fetchrowslimit integer,
    idlenumitems integer,
    idlehours double precision,
    triggertime timestamp(6) without time zone DEFAULT now() NOT NULL,
    status text
);


ALTER TABLE public.t_invrollingpolicies OWNER TO acsdba;

--
-- Name: t_invrollingpolicies_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_invrollingpolicies_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_invrollingpolicies_id_seq OWNER TO acsdba;

--
-- Name: t_invrollingpolicies_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_invrollingpolicies_id_seq OWNED BY public.t_invrollingpolicies.id;


--
-- Name: t_invsiterelation; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_invsiterelation (
    id integer NOT NULL,
    filename character varying(128) NOT NULL,
    fileversion character varying(32) NOT NULL,
    site integer NOT NULL
);


ALTER TABLE public.t_invsiterelation OWNER TO acsdba;

--
-- Name: t_invsiterelation_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_invsiterelation_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_invsiterelation_id_seq OWNER TO acsdba;

--
-- Name: t_invsiterelation_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_invsiterelation_id_seq OWNED BY public.t_invsiterelation.id;


--
-- Name: t_invspecialrollingpolicies; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_invspecialrollingpolicies (
    id integer NOT NULL,
    inventoryname character varying(128) NOT NULL,
    inventoryversion character varying(32) NOT NULL,
    expirationdate timestamp(6) without time zone,
    action integer NOT NULL,
    storageid integer,
    consumable boolean DEFAULT false NOT NULL
);


ALTER TABLE public.t_invspecialrollingpolicies OWNER TO acsdba;

--
-- Name: t_invspecialrollingpolicies_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_invspecialrollingpolicies_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_invspecialrollingpolicies_id_seq OWNER TO acsdba;

--
-- Name: t_invspecialrollingpolicies_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_invspecialrollingpolicies_id_seq OWNED BY public.t_invspecialrollingpolicies.id;


--
-- Name: t_knownhosts; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_knownhosts (
    id integer NOT NULL,
    ip_address character varying(16),
    host_name character varying(256)
);


ALTER TABLE public.t_knownhosts OWNER TO acsdba;

--
-- Name: t_knownhosts_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_knownhosts_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_knownhosts_id_seq OWNER TO acsdba;

--
-- Name: t_knownhosts_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_knownhosts_id_seq OWNED BY public.t_knownhosts.id;


--
-- Name: t_knownhosts_limits; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_knownhosts_limits (
    id integer NOT NULL,
    circulator_node_id integer NOT NULL,
    dest_host_id integer NOT NULL,
    max_upload_slots integer NOT NULL,
    CONSTRAINT ck_knownhosts_limits01 CHECK ((max_upload_slots > 0))
);


ALTER TABLE public.t_knownhosts_limits OWNER TO acsdba;

--
-- Name: t_knownhosts_limits_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_knownhosts_limits_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_knownhosts_limits_id_seq OWNER TO acsdba;

--
-- Name: t_knownhosts_limits_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_knownhosts_limits_id_seq OWNED BY public.t_knownhosts_limits.id;


--
-- Name: t_lastacknowledge; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_lastacknowledge (
    id integer NOT NULL,
    applicationname character varying(25) NOT NULL,
    applicationdata character varying(128) NOT NULL,
    tstamp timestamp(6) without time zone NOT NULL
);


ALTER TABLE public.t_lastacknowledge OWNER TO acsdba;

--
-- Name: COLUMN t_lastacknowledge.id; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_lastacknowledge.id IS 'Record identifier (Primary Key)';


--
-- Name: t_lastacknowledge_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_lastacknowledge_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_lastacknowledge_id_seq OWNER TO acsdba;

--
-- Name: t_lastacknowledge_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_lastacknowledge_id_seq OWNED BY public.t_lastacknowledge.id;


--
-- Name: SEQUENCE t_lastacknowledge_id_seq; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON SEQUENCE public.t_lastacknowledge_id_seq IS 'Used to generate the primary keys of t_lastacknowledge table';


--
-- Name: t_localtransfermodes; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_localtransfermodes (
    id smallint NOT NULL,
    localtransfermode character varying(30) NOT NULL
);


ALTER TABLE public.t_localtransfermodes OWNER TO acsdba;

--
-- Name: TABLE t_localtransfermodes; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON TABLE public.t_localtransfermodes IS 'Contains the list of possible local data transfer methods';


--
-- Name: COLUMN t_localtransfermodes.id; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_localtransfermodes.id IS 'Record identifier (Primary Key)';


--
-- Name: COLUMN t_localtransfermodes.localtransfermode; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_localtransfermodes.localtransfermode IS 'The type of transfer mode (eg: COPY, HARDLINK...)';


--
-- Name: t_measdataset; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_measdataset (
    id integer NOT NULL,
    name character varying(64) NOT NULL
);


ALTER TABLE public.t_measdataset OWNER TO acsdba;

--
-- Name: t_measdataset_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_measdataset_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_measdataset_id_seq OWNER TO acsdba;

--
-- Name: t_measdataset_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_measdataset_id_seq OWNED BY public.t_measdataset.id;


--
-- Name: t_mediacapformat; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_mediacapformat (
    format character varying(16) NOT NULL,
    mediatype character varying(16) NOT NULL,
    multiplier real DEFAULT 1.00000000 NOT NULL
);


ALTER TABLE public.t_mediacapformat OWNER TO acsdba;

--
-- Name: t_mediatypes; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_mediatypes (
    id_mediatype smallint NOT NULL,
    mediatype character varying(16) NOT NULL,
    uncompressedcapacity integer NOT NULL
);


ALTER TABLE public.t_mediatypes OWNER TO acsdba;

--
-- Name: t_migrationalgos; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_migrationalgos (
    id_migrationalgo integer NOT NULL,
    name character varying(32) NOT NULL
);


ALTER TABLE public.t_migrationalgos OWNER TO acsdba;

--
-- Name: t_migrationalgos_id_migrationalgo_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_migrationalgos_id_migrationalgo_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_migrationalgos_id_migrationalgo_seq OWNER TO acsdba;

--
-- Name: t_migrationalgos_id_migrationalgo_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_migrationalgos_id_migrationalgo_seq OWNED BY public.t_migrationalgos.id_migrationalgo;


--
-- Name: t_mpm_cross_id_cross_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_mpm_cross_id_cross_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_mpm_cross_id_cross_seq OWNER TO acsdba;

--
-- Name: t_mpm_cross_id_cross_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_mpm_cross_id_cross_seq OWNED BY public.t_mpm_cross.id_cross;


--
-- Name: t_mpm_dip; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_mpm_dip (
    id_dip integer NOT NULL,
    inventory_id bigint NOT NULL,
    eeprom integer DEFAULT 1 NOT NULL,
    ram integer NOT NULL,
    delta_dip integer,
    dip_status_id smallint DEFAULT 1 NOT NULL
);


ALTER TABLE public.t_mpm_dip OWNER TO acsdba;

--
-- Name: TABLE t_mpm_dip; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON TABLE public.t_mpm_dip IS 'Contains the eeprom and ram additional info of an inventory record.';


--
-- Name: COLUMN t_mpm_dip.id_dip; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_mpm_dip.id_dip IS 'Record identifier (Primary Key)';


--
-- Name: COLUMN t_mpm_dip.inventory_id; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_mpm_dip.inventory_id IS 'Inventory record identifier. Reference the t_inventory table';


--
-- Name: COLUMN t_mpm_dip.eeprom; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_mpm_dip.eeprom IS 'EEPROM version (default 1)';


--
-- Name: COLUMN t_mpm_dip.ram; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_mpm_dip.ram IS 'RAM version';


--
-- Name: COLUMN t_mpm_dip.delta_dip; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_mpm_dip.delta_dip IS 'Delta DIP version';


--
-- Name: t_mpm_dip_id_dip_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_mpm_dip_id_dip_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_mpm_dip_id_dip_seq OWNER TO acsdba;

--
-- Name: t_mpm_dip_id_dip_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_mpm_dip_id_dip_seq OWNED BY public.t_mpm_dip.id_dip;


--
-- Name: t_mpm_ipf_adf_check; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_mpm_ipf_adf_check (
    id bigint NOT NULL,
    product_id bigint NOT NULL,
    adf_ipf_expected character varying(256) NOT NULL,
    adf_ipf_found character varying(256),
    filename character varying(128)
);


ALTER TABLE public.t_mpm_ipf_adf_check OWNER TO acsdba;

--
-- Name: t_mpm_ipf_adf_check_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_mpm_ipf_adf_check_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_mpm_ipf_adf_check_id_seq OWNER TO acsdba;

--
-- Name: t_mpm_ipf_adf_check_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_mpm_ipf_adf_check_id_seq OWNED BY public.t_mpm_ipf_adf_check.id;


--
-- Name: t_mpm_parameter_types; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_mpm_parameter_types (
    id_paramtype smallint NOT NULL,
    type_name character varying(32) NOT NULL
);


ALTER TABLE public.t_mpm_parameter_types OWNER TO acsdba;

--
-- Name: COLUMN t_mpm_parameter_types.id_paramtype; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_mpm_parameter_types.id_paramtype IS 'Record identifier (Primary Key)';


--
-- Name: t_mpm_parameters_id_parameter_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_mpm_parameters_id_parameter_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_mpm_parameters_id_parameter_seq OWNER TO acsdba;

--
-- Name: t_mpm_parameters_id_parameter_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_mpm_parameters_id_parameter_seq OWNED BY public.t_mpm_parameters.id_parameter;


--
-- Name: t_mpm_prb; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_mpm_prb (
    id_prb integer NOT NULL,
    inventory_id bigint NOT NULL,
    status_id integer NOT NULL
);


ALTER TABLE public.t_mpm_prb OWNER TO acsdba;

--
-- Name: COLUMN t_mpm_prb.id_prb; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_mpm_prb.id_prb IS 'Record identifier (Primary Key)';


--
-- Name: COLUMN t_mpm_prb.inventory_id; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_mpm_prb.inventory_id IS 'Inventory identifier. Reference the t_inventory table';


--
-- Name: COLUMN t_mpm_prb.status_id; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_mpm_prb.status_id IS 'Status identifier. Reference the t_mpm_statuses table';


--
-- Name: t_mpm_prb_id_prb_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_mpm_prb_id_prb_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_mpm_prb_id_prb_seq OWNER TO acsdba;

--
-- Name: t_mpm_prb_id_prb_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_mpm_prb_id_prb_seq OWNED BY public.t_mpm_prb.id_prb;


--
-- Name: t_mpm_routines; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_mpm_routines (
    id_routine integer NOT NULL,
    routine_name character varying(100) NOT NULL,
    description text
);


ALTER TABLE public.t_mpm_routines OWNER TO acsdba;

--
-- Name: COLUMN t_mpm_routines.id_routine; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_mpm_routines.id_routine IS 'Record identifier (Primary Key)';


--
-- Name: t_mpm_routines_id_routine_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_mpm_routines_id_routine_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_mpm_routines_id_routine_seq OWNER TO acsdba;

--
-- Name: t_mpm_routines_id_routine_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_mpm_routines_id_routine_seq OWNED BY public.t_mpm_routines.id_routine;


--
-- Name: t_mpm_statuses; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_mpm_statuses (
    id_status smallint NOT NULL,
    status character varying(32) NOT NULL
);


ALTER TABLE public.t_mpm_statuses OWNER TO acsdba;

--
-- Name: TABLE t_mpm_statuses; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON TABLE public.t_mpm_statuses IS 'Contains the list of allowed statuses for DIP.';


--
-- Name: COLUMN t_mpm_statuses.id_status; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_mpm_statuses.id_status IS 'Record identifier (Primary Key)';


--
-- Name: COLUMN t_mpm_statuses.status; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_mpm_statuses.status IS 'The status name';


--
-- Name: t_mpm_timeliness; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_mpm_timeliness (
    id_timeliness smallint NOT NULL,
    timeliness_code character(2) NOT NULL,
    description text
);


ALTER TABLE public.t_mpm_timeliness OWNER TO acsdba;

--
-- Name: TABLE t_mpm_timeliness; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON TABLE public.t_mpm_timeliness IS 'Contains the list of timeliness codes used in file naming convention';


--
-- Name: COLUMN t_mpm_timeliness.id_timeliness; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_mpm_timeliness.id_timeliness IS 'Record identifier (Primary Key)';


--
-- Name: COLUMN t_mpm_timeliness.timeliness_code; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_mpm_timeliness.timeliness_code IS 'Timeliness code';


--
-- Name: COLUMN t_mpm_timeliness.description; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_mpm_timeliness.description IS 'Description of timeliness code';


--
-- Name: t_mpm_values; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_mpm_values (
    id_value bigint NOT NULL,
    value_name character varying(100) NOT NULL,
    level smallint NOT NULL,
    value_data oid NOT NULL,
    creationtime timestamp(6) without time zone DEFAULT now() NOT NULL
);


ALTER TABLE public.t_mpm_values OWNER TO acsdba;

--
-- Name: t_mpm_values_id_value_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_mpm_values_id_value_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_mpm_values_id_value_seq OWNER TO acsdba;

--
-- Name: t_mpm_values_id_value_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_mpm_values_id_value_seq OWNED BY public.t_mpm_values.id_value;


--
-- Name: t_mpm_values_part1; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_mpm_values_part1 (
    CONSTRAINT t_mpm_values_part1_level_check CHECK ((level = 1))
)
INHERITS (public.t_mpm_values);


ALTER TABLE public.t_mpm_values_part1 OWNER TO acsdba;

--
-- Name: t_mpm_values_part2; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_mpm_values_part2 (
    CONSTRAINT t_mpm_values_part2_level_check CHECK ((level = 2))
)
INHERITS (public.t_mpm_values);


ALTER TABLE public.t_mpm_values_part2 OWNER TO acsdba;

--
-- Name: t_objsminvrelation; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_objsminvrelation (
    id integer NOT NULL,
    http_path character varying(512) NOT NULL,
    s3_path character varying(512) NOT NULL,
    signature character varying(64)
);


ALTER TABLE public.t_objsminvrelation OWNER TO acsdba;

--
-- Name: t_ondemanddistribution; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_ondemanddistribution (
    orderid integer NOT NULL,
    userid character varying(32) NOT NULL,
    format integer,
    mediatype character varying(16),
    disseminationpackingalgo_id integer NOT NULL
);


ALTER TABLE public.t_ondemanddistribution OWNER TO acsdba;

--
-- Name: t_operation_requests; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_operation_requests (
    id integer NOT NULL,
    object_id bigint NOT NULL,
    status character varying(32) NOT NULL,
    type character varying(32) NOT NULL,
    start_time timestamp(6) without time zone DEFAULT now() NOT NULL,
    stop_time timestamp(6) without time zone,
    error_message text,
    content_type character varying(32) NOT NULL
);


ALTER TABLE public.t_operation_requests OWNER TO acsdba;

--
-- Name: t_operation_requests_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_operation_requests_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_operation_requests_id_seq OWNER TO acsdba;

--
-- Name: t_operation_requests_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_operation_requests_id_seq OWNED BY public.t_operation_requests.id;


--
-- Name: t_operationtypes; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_operationtypes (
    id_operationtype smallint NOT NULL,
    type character varying(32) NOT NULL
);


ALTER TABLE public.t_operationtypes OWNER TO acsdba;

--
-- Name: t_ordersattachments; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_ordersattachments (
    id integer NOT NULL,
    orderid integer NOT NULL,
    attachtype integer NOT NULL,
    attachname character varying(128),
    attachment oid
);


ALTER TABLE public.t_ordersattachments OWNER TO acsdba;

--
-- Name: t_ordersattachments_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_ordersattachments_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_ordersattachments_id_seq OWNER TO acsdba;

--
-- Name: t_ordersattachments_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_ordersattachments_id_seq OWNED BY public.t_ordersattachments.id;


--
-- Name: t_ordersattachmenttypes; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_ordersattachmenttypes (
    id integer NOT NULL,
    name character varying(16) NOT NULL
);


ALTER TABLE public.t_ordersattachmenttypes OWNER TO acsdba;

--
-- Name: t_ordersattachmenttypes_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_ordersattachmenttypes_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_ordersattachmenttypes_id_seq OWNER TO acsdba;

--
-- Name: t_ordersattachmenttypes_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_ordersattachmenttypes_id_seq OWNED BY public.t_ordersattachmenttypes.id;


--
-- Name: t_ordersgeneration; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_ordersgeneration (
    id integer NOT NULL,
    processorname character varying(32) NOT NULL,
    processorversion character varying(8) NOT NULL,
    processorconfiguration character varying(8) NOT NULL,
    ordertype character varying(8) NOT NULL,
    priority smallint NOT NULL,
    troubleshooting boolean NOT NULL,
    processingtimewindow smallint NOT NULL,
    algorithm character varying(32) NOT NULL,
    filetype character varying(16) NOT NULL,
    active boolean DEFAULT true NOT NULL,
    fileclass character varying(8)
);


ALTER TABLE public.t_ordersgeneration OWNER TO acsdba;

--
-- Name: t_ordersgeneration_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_ordersgeneration_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_ordersgeneration_id_seq OWNER TO acsdba;

--
-- Name: t_ordersgeneration_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_ordersgeneration_id_seq OWNED BY public.t_ordersgeneration.id;


--
-- Name: t_ordersinput; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_ordersinput (
    id integer NOT NULL,
    filename character varying(128) NOT NULL,
    fileversion character varying(32) NOT NULL,
    orderid integer NOT NULL
);


ALTER TABLE public.t_ordersinput OWNER TO acsdba;

--
-- Name: t_ordersinput_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_ordersinput_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_ordersinput_id_seq OWNER TO acsdba;

--
-- Name: t_ordersinput_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_ordersinput_id_seq OWNED BY public.t_ordersinput.id;


--
-- Name: t_ordersqueue; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_ordersqueue (
    orderid integer NOT NULL,
    creationdate timestamp(6) without time zone NOT NULL,
    priority smallint NOT NULL,
    status character varying(32) NOT NULL,
    processorname character varying(32) NOT NULL,
    processorversion character varying(8) NOT NULL,
    processorconfiguration character varying(8),
    troubleshooting boolean NOT NULL,
    starttime timestamp(6) without time zone NOT NULL,
    stoptime timestamp(6) without time zone NOT NULL,
    jobresponsible character varying(64),
    productionstarttime timestamp(6) without time zone,
    productionstoptime timestamp(6) without time zone,
    processingtimewindow smallint NOT NULL,
    processorid character varying(16),
    ordertype character varying(8) NOT NULL,
    externalorderid character varying(128),
    satelliteid smallint,
    parameters text,
    mission smallint,
    subreq integer,
    CONSTRAINT ck_ordersqueue CHECK ((starttime <= stoptime))
);


ALTER TABLE public.t_ordersqueue OWNER TO acsdba;

--
-- Name: t_ordersqueue_orderid_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_ordersqueue_orderid_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_ordersqueue_orderid_seq OWNER TO acsdba;

--
-- Name: t_ordersqueue_orderid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_ordersqueue_orderid_seq OWNED BY public.t_ordersqueue.orderid;


--
-- Name: t_ordersqueueblacklist; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_ordersqueueblacklist (
    id integer NOT NULL,
    orderid integer NOT NULL,
    jobresponsible character varying(64) NOT NULL
);


ALTER TABLE public.t_ordersqueueblacklist OWNER TO acsdba;

--
-- Name: t_ordersqueueblacklist_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_ordersqueueblacklist_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_ordersqueueblacklist_id_seq OWNER TO acsdba;

--
-- Name: t_ordersqueueblacklist_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_ordersqueueblacklist_id_seq OWNED BY public.t_ordersqueueblacklist.id;


--
-- Name: t_orderstatetransitions; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_orderstatetransitions (
    id bigint NOT NULL,
    orderid integer NOT NULL,
    tstamp timestamp(6) without time zone DEFAULT now(),
    status character varying(32) NOT NULL
);


ALTER TABLE public.t_orderstatetransitions OWNER TO acsdba;

--
-- Name: t_orderstatetransitions_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_orderstatetransitions_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_orderstatetransitions_id_seq OWNER TO acsdba;

--
-- Name: t_orderstatetransitions_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_orderstatetransitions_id_seq OWNED BY public.t_orderstatetransitions.id;


--
-- Name: t_ordertypes; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_ordertypes (
    ordertype character varying(8) NOT NULL,
    ordertypedescription character varying(64) NOT NULL
);


ALTER TABLE public.t_ordertypes OWNER TO acsdba;

--
-- Name: t_orderwaittrigger; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_orderwaittrigger (
    triggertype character varying(16) NOT NULL
);


ALTER TABLE public.t_orderwaittrigger OWNER TO acsdba;

--
-- Name: t_ordtriggeringfiles; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_ordtriggeringfiles (
    id integer NOT NULL,
    filename character varying(128) NOT NULL,
    fileversion character varying(32) NOT NULL,
    orderid integer NOT NULL
);


ALTER TABLE public.t_ordtriggeringfiles OWNER TO acsdba;

--
-- Name: t_ordtriggeringfiles_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_ordtriggeringfiles_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_ordtriggeringfiles_id_seq OWNER TO acsdba;

--
-- Name: t_ordtriggeringfiles_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_ordtriggeringfiles_id_seq OWNED BY public.t_ordtriggeringfiles.id;


--
-- Name: t_originators; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_originators (
    id smallint NOT NULL,
    name character varying(128) NOT NULL
);


ALTER TABLE public.t_originators OWNER TO acsdba;

--
-- Name: t_pdsmodes; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_pdsmodes (
    id integer NOT NULL,
    application character varying(64) NOT NULL,
    subsystem character varying(16) NOT NULL,
    mode character varying(16) NOT NULL
);


ALTER TABLE public.t_pdsmodes OWNER TO acsdba;

--
-- Name: t_pdsmodes_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_pdsmodes_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_pdsmodes_id_seq OWNER TO acsdba;

--
-- Name: t_pdsmodes_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_pdsmodes_id_seq OWNED BY public.t_pdsmodes.id;


--
-- Name: t_pendinginventory; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_pendinginventory (
    filename character varying(128) NOT NULL,
    fileversion character varying(32) NOT NULL
);


ALTER TABLE public.t_pendinginventory OWNER TO acsdba;

--
-- Name: t_postinvqueue; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_postinvqueue (
    id integer NOT NULL,
    filename character varying(128) NOT NULL,
    fileversion character varying(32) NOT NULL,
    filetype character varying(16) NOT NULL,
    validitystart timestamp(6) without time zone NOT NULL,
    validitystop timestamp(6) without time zone NOT NULL,
    tstamp timestamp(6) without time zone DEFAULT now() NOT NULL
);


ALTER TABLE public.t_postinvqueue OWNER TO acsdba;

--
-- Name: t_postinvqueue_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_postinvqueue_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_postinvqueue_id_seq OWNER TO acsdba;

--
-- Name: t_postinvqueue_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_postinvqueue_id_seq OWNED BY public.t_postinvqueue.id;


--
-- Name: t_proc_ftype_generation; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_proc_ftype_generation (
    id integer NOT NULL,
    proc_id integer NOT NULL,
    ftype_id integer NOT NULL
);


ALTER TABLE public.t_proc_ftype_generation OWNER TO acsdba;

--
-- Name: TABLE t_proc_ftype_generation; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON TABLE public.t_proc_ftype_generation IS 'Cross table that links processors and filetypes that can be generated by processors';


--
-- Name: COLUMN t_proc_ftype_generation.id; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_proc_ftype_generation.id IS 'Record identifier (Primary Key)';


--
-- Name: COLUMN t_proc_ftype_generation.proc_id; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_proc_ftype_generation.proc_id IS 'Processor identifier. References the t_processors table';


--
-- Name: COLUMN t_proc_ftype_generation.ftype_id; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_proc_ftype_generation.ftype_id IS 'FileType identifier. References the t_filetypes table';


--
-- Name: t_processors; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_processors (
    id_processor integer NOT NULL,
    processorname character varying(32) NOT NULL,
    processorversion character varying(8) NOT NULL,
    description character varying(64),
    baseline character varying(16)
);


ALTER TABLE public.t_processors OWNER TO acsdba;

--
-- Name: t_processors_id_processor_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_processors_id_processor_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_processors_id_processor_seq OWNER TO acsdba;

--
-- Name: t_processors_id_processor_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_processors_id_processor_seq OWNED BY public.t_processors.id_processor;


--
-- Name: t_qualifiedflag; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_qualifiedflag (
    id integer NOT NULL,
    name character varying(32) NOT NULL
);


ALTER TABLE public.t_qualifiedflag OWNER TO acsdba;

--
-- Name: t_qualifiedflag_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_qualifiedflag_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_qualifiedflag_id_seq OWNER TO acsdba;

--
-- Name: t_qualifiedflag_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_qualifiedflag_id_seq OWNED BY public.t_qualifiedflag.id;


--
-- Name: t_receptionrules; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_receptionrules (
    id_receptionrule integer NOT NULL,
    filetype_id integer NOT NULL,
    repository_id integer NOT NULL,
    user_id integer NOT NULL,
    usermode_id smallint NOT NULL,
    priority integer DEFAULT 1 NOT NULL,
    active boolean DEFAULT true NOT NULL,
    delete_on_remote boolean DEFAULT false NOT NULL,
    generateheader boolean DEFAULT false NOT NULL,
    reception_tt_id integer DEFAULT public.f_get_tasktable_id('defaultReception'::character varying) NOT NULL,
    tt_parameters text,
    hostname character varying(256),
    localpath character varying(1024) NOT NULL,
    connretries smallint DEFAULT 1 NOT NULL,
    pollingperiod_sec integer NOT NULL,
    retryperiod_sec integer NOT NULL,
    connfailureaddress character varying(1024),
    procfailureaddress character varying(1024),
    lastvisit timestamp without time zone DEFAULT now() NOT NULL,
    lastgood timestamp without time zone DEFAULT now() NOT NULL,
    retrycounter smallint DEFAULT 0 NOT NULL,
    discoveryplugin_id integer NOT NULL,
    successnotificationaddress character varying(1024),
    php_timeout_sec integer,
    satellite_id smallint,
    mission smallint,
    sensor_id smallint,
    timewindow_sec integer,
    CONSTRAINT ck_receptionrules01 CHECK ((pollingperiod_sec > 0)),
    CONSTRAINT ck_receptionrules02 CHECK (((retryperiod_sec > 0) AND ((retryperiod_sec)::double precision < floor(((pollingperiod_sec / connretries))::double precision)))),
    CONSTRAINT ck_receptionrules03 CHECK ((php_timeout_sec > 90)),
    CONSTRAINT ck_receptionrules04 CHECK ((((satellite_id IS NULL) AND ((mission IS NOT NULL) OR (sensor_id IS NOT NULL))) <> true)),
    CONSTRAINT ck_receptionrules05 CHECK ((((mission IS NULL) AND (sensor_id IS NOT NULL)) <> true))
);


ALTER TABLE public.t_receptionrules OWNER TO acsdba;

--
-- Name: t_receptionrules_id_receptionrule_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_receptionrules_id_receptionrule_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_receptionrules_id_receptionrule_seq OWNER TO acsdba;

--
-- Name: t_receptionrules_id_receptionrule_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_receptionrules_id_receptionrule_seq OWNED BY public.t_receptionrules.id_receptionrule;


--
-- Name: t_receptionruleshist; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_receptionruleshist (
    id_receptionrulehist integer NOT NULL,
    receptionrule_id integer NOT NULL,
    filename character varying(1024) NOT NULL,
    tstamp timestamp without time zone NOT NULL,
    status_id smallint NOT NULL,
    filecreationtstamp timestamp(6) without time zone,
    remoteuuid character varying(160),
    evictiondate timestamp(6) without time zone
);


ALTER TABLE public.t_receptionruleshist OWNER TO acsdba;

--
-- Name: t_receptionruleshist_id_receptionrulehist_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_receptionruleshist_id_receptionrulehist_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_receptionruleshist_id_receptionrulehist_seq OWNER TO acsdba;

--
-- Name: t_receptionruleshist_id_receptionrulehist_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_receptionruleshist_id_receptionrulehist_seq OWNED BY public.t_receptionruleshist.id_receptionrulehist;


--
-- Name: t_receptionruleslock; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_receptionruleslock (
    receptionrule_id integer NOT NULL,
    responsible character varying(128) NOT NULL,
    tstamp timestamp(6) without time zone DEFAULT now() NOT NULL,
    processidentifier character varying(128)
);


ALTER TABLE public.t_receptionruleslock OWNER TO acsdba;

--
-- Name: t_repositories; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_repositories (
    id_repository integer NOT NULL,
    symbolicname character varying(64) NOT NULL,
    transferprotocol_id integer NOT NULL,
    host_id integer NOT NULL,
    username character varying(32) NOT NULL,
    password character varying(256),
    remotepath character varying(1024) NOT NULL,
    tempname_id smallint DEFAULT 1 NOT NULL,
    tempname_value character varying(64) NOT NULL,
    allowedciphers character varying(256),
    port smallint,
    attributes jsonb,
    CONSTRAINT ck_repositories01 CHECK ((char_length(btrim((tempname_value)::text)) <> 0)),
    CONSTRAINT ck_repositories02 CHECK ((((tempname_id <> 2) AND (((tempname_value)::text = '..'::text) OR ((tempname_value)::text = '../'::text))) <> true)),
    CONSTRAINT ck_repositories03 CHECK ((((tempname_id = 3) AND (((tempname_value)::text = '.'::text) OR ((tempname_value)::text = './'::text))) <> true))
);


ALTER TABLE public.t_repositories OWNER TO acsdba;

--
-- Name: t_repositories_id_repository_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_repositories_id_repository_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_repositories_id_repository_seq OWNER TO acsdba;

--
-- Name: t_repositories_id_repository_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_repositories_id_repository_seq OWNED BY public.t_repositories.id_repository;


--
-- Name: t_rollingpoliciesalgo; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_rollingpoliciesalgo (
    id integer NOT NULL,
    name character varying(60) NOT NULL
);


ALTER TABLE public.t_rollingpoliciesalgo OWNER TO acsdba;

--
-- Name: t_rollingpoliciesalgo_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_rollingpoliciesalgo_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_rollingpoliciesalgo_id_seq OWNER TO acsdba;

--
-- Name: t_rollingpoliciesalgo_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_rollingpoliciesalgo_id_seq OWNED BY public.t_rollingpoliciesalgo.id;


--
-- Name: t_rollingpolicyactions; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_rollingpolicyactions (
    id integer NOT NULL,
    action character varying(60) NOT NULL
);


ALTER TABLE public.t_rollingpolicyactions OWNER TO acsdba;

--
-- Name: t_rollingpolicyactions_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_rollingpolicyactions_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_rollingpolicyactions_id_seq OWNER TO acsdba;

--
-- Name: t_rollingpolicyactions_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_rollingpolicyactions_id_seq OWNED BY public.t_rollingpolicyactions.id;


--
-- Name: t_satellites; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_satellites (
    satelliteid smallint NOT NULL,
    satelliteacronym character varying(4) NOT NULL,
    satellitename character varying(64) NOT NULL
);


ALTER TABLE public.t_satellites OWNER TO acsdba;

--
-- Name: t_satrelations; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_satrelations (
    satelliteid smallint NOT NULL,
    mission smallint NOT NULL,
    sensorid smallint NOT NULL
);


ALTER TABLE public.t_satrelations OWNER TO acsdba;

--
-- Name: t_schema; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_schema (
    id integer NOT NULL,
    version character varying(24) NOT NULL,
    modificationdate date NOT NULL,
    modificationtype integer DEFAULT 1 NOT NULL,
    appliedon date DEFAULT now() NOT NULL,
    schematype integer DEFAULT 1 NOT NULL,
    cvstag character varying(64),
    description text
);


ALTER TABLE public.t_schema OWNER TO acsdba;

--
-- Name: TABLE t_schema; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON TABLE public.t_schema IS 'Contains the list of installed schemas (base and plugins) and their version';


--
-- Name: COLUMN t_schema.id; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_schema.id IS 'Record identifier (Primary Key)';


--
-- Name: COLUMN t_schema.version; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_schema.version IS 'Is the unique string that represents the version. It is composed by the name of schema and his version (eg: PDS_10.0.0)';


--
-- Name: COLUMN t_schema.modificationdate; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_schema.modificationdate IS 'When the patch or upgrade has been developed';


--
-- Name: COLUMN t_schema.modificationtype; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_schema.modificationtype IS 'Type of change (eg: UPGRADE, PATCH). It is an integer that references t_schemamodificationtypes table';


--
-- Name: COLUMN t_schema.appliedon; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_schema.appliedon IS 'When the patch or upgrade has been installed';


--
-- Name: COLUMN t_schema.schematype; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_schema.schematype IS 'Type of schema (eg: PDS, ISM). It is an integer that references t_schematypes table';


--
-- Name: COLUMN t_schema.cvstag; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_schema.cvstag IS 'String used to identify the patch or the upgrade into the cvs repository';


--
-- Name: COLUMN t_schema.description; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_schema.description IS 'Description of changes';


--
-- Name: t_schema_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_schema_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_schema_id_seq OWNER TO acsdba;

--
-- Name: t_schema_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_schema_id_seq OWNED BY public.t_schema.id;


--
-- Name: SEQUENCE t_schema_id_seq; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON SEQUENCE public.t_schema_id_seq IS 'Used to generate the primary keys of t_schema table';


--
-- Name: t_schemamodificationtypes; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_schemamodificationtypes (
    id smallint NOT NULL,
    modificationtype character varying(32) NOT NULL
);


ALTER TABLE public.t_schemamodificationtypes OWNER TO acsdba;

--
-- Name: TABLE t_schemamodificationtypes; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON TABLE public.t_schemamodificationtypes IS 'Contains the list of modification types (eg: UPGRADE, PATCH)';


--
-- Name: COLUMN t_schemamodificationtypes.id; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_schemamodificationtypes.id IS 'Record identifier (Primary Key)';


--
-- Name: COLUMN t_schemamodificationtypes.modificationtype; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_schemamodificationtypes.modificationtype IS 'Type of change (eg: UPGRADE, PATCH)';


--
-- Name: t_schematypes; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_schematypes (
    id smallint NOT NULL,
    schematype character varying(32) NOT NULL,
    description character varying(128)
);


ALTER TABLE public.t_schematypes OWNER TO acsdba;

--
-- Name: TABLE t_schematypes; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON TABLE public.t_schematypes IS 'Contains the list of known schema types for base schemas and plugins (eg: PDS, CIRCULATION)';


--
-- Name: COLUMN t_schematypes.id; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_schematypes.id IS 'Record identifier (Primary Key)';


--
-- Name: COLUMN t_schematypes.schematype; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_schematypes.schematype IS 'Type of schema (eg: PDS, CIRCULATION)';


--
-- Name: COLUMN t_schematypes.description; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_schematypes.description IS 'Little description of the type of schema';


--
-- Name: t_securityzonerelays; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_securityzonerelays (
    id integer NOT NULL,
    host_id integer NOT NULL,
    seczone_id integer NOT NULL
);


ALTER TABLE public.t_securityzonerelays OWNER TO acsdba;

--
-- Name: t_securityzonerelays_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_securityzonerelays_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_securityzonerelays_id_seq OWNER TO acsdba;

--
-- Name: t_securityzonerelays_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_securityzonerelays_id_seq OWNED BY public.t_securityzonerelays.id;


--
-- Name: t_securityzones; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_securityzones (
    id integer NOT NULL,
    ip_cidr character varying(32) NOT NULL,
    level integer NOT NULL,
    securityzone character varying(32)
);


ALTER TABLE public.t_securityzones OWNER TO acsdba;

--
-- Name: t_securityzones_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_securityzones_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_securityzones_id_seq OWNER TO acsdba;

--
-- Name: t_securityzones_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_securityzones_id_seq OWNED BY public.t_securityzones.id;


--
-- Name: t_sensors; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_sensors (
    sensorid smallint NOT NULL,
    sensorname character varying(64) NOT NULL,
    description character varying(128)
);


ALTER TABLE public.t_sensors OWNER TO acsdba;

--
-- Name: t_serials; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_serials (
    table_name character varying(30) NOT NULL,
    field_name character varying(30) NOT NULL
);


ALTER TABLE public.t_serials OWNER TO acsdba;

--
-- Name: t_sites; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_sites (
    id integer NOT NULL,
    name character varying(32) NOT NULL,
    parent integer,
    active boolean NOT NULL,
    type integer NOT NULL,
    details text
);


ALTER TABLE public.t_sites OWNER TO acsdba;

--
-- Name: t_sites_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_sites_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_sites_id_seq OWNER TO acsdba;

--
-- Name: t_sites_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_sites_id_seq OWNED BY public.t_sites.id;


--
-- Name: t_sitetype; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_sitetype (
    id integer NOT NULL,
    name character varying(32) NOT NULL
);


ALTER TABLE public.t_sitetype OWNER TO acsdba;

--
-- Name: t_sitetype_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_sitetype_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_sitetype_id_seq OWNER TO acsdba;

--
-- Name: t_sitetype_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_sitetype_id_seq OWNED BY public.t_sitetype.id;


--
-- Name: t_sm_sync_missingactions; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_sm_sync_missingactions (
    id integer NOT NULL,
    operation character varying(16) NOT NULL,
    sm_id integer NOT NULL,
    inv_id bigint,
    invsm_id integer,
    tstamp timestamp(6) without time zone DEFAULT now() NOT NULL,
    CONSTRAINT ck_sm_sync_missingactions01 CHECK (((operation)::text = ANY ((ARRAY['UPLOAD'::character varying, 'ERASE'::character varying])::text[]))),
    CONSTRAINT ck_sm_sync_missingactions02 CHECK ((((inv_id IS NULL) AND (invsm_id IS NOT NULL)) OR ((inv_id IS NOT NULL) AND (invsm_id IS NULL))))
);


ALTER TABLE public.t_sm_sync_missingactions OWNER TO acsdba;

--
-- Name: TABLE t_sm_sync_missingactions; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON TABLE public.t_sm_sync_missingactions IS 'Contains the list of the failed and still pending operations performed on synch storage managers';


--
-- Name: COLUMN t_sm_sync_missingactions.id; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_sm_sync_missingactions.id IS 'Record identifier (Primary Key)';


--
-- Name: COLUMN t_sm_sync_missingactions.operation; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_sm_sync_missingactions.operation IS 'Identifies the type of the operation. Can be UPLOAD or ERASE';


--
-- Name: COLUMN t_sm_sync_missingactions.sm_id; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_sm_sync_missingactions.sm_id IS 'The ID of the storage manager on which the missing operation shall be performed. References the t_storagemanager table';


--
-- Name: COLUMN t_sm_sync_missingactions.inv_id; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_sm_sync_missingactions.inv_id IS 'Identifies the inventory id of the file to be UPLOADED into the storage manager. References the t_inventory table';


--
-- Name: COLUMN t_sm_sync_missingactions.invsm_id; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_sm_sync_missingactions.invsm_id IS 'Identifies the file to be ERASED from the identified storage manager. References the t_inventorysm table';


--
-- Name: COLUMN t_sm_sync_missingactions.tstamp; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_sm_sync_missingactions.tstamp IS 'When the record has been inserted (default value NOW)';


--
-- Name: t_sm_sync_missingactions_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_sm_sync_missingactions_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_sm_sync_missingactions_id_seq OWNER TO acsdba;

--
-- Name: t_sm_sync_missingactions_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_sm_sync_missingactions_id_seq OWNED BY public.t_sm_sync_missingactions.id;


--
-- Name: t_smactions; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_smactions (
    id integer NOT NULL,
    inv_id bigint NOT NULL,
    filename character varying(128) NOT NULL,
    fileversion character varying(32) NOT NULL,
    unique_id character varying(160) NOT NULL,
    smid integer NOT NULL,
    operation character varying(16) NOT NULL,
    parameter oid,
    creationdate timestamp(6) without time zone DEFAULT now() NOT NULL,
    packageid integer,
    enddate timestamp(6) without time zone DEFAULT '3000-01-01 00:00:00'::timestamp(6) without time zone
);


ALTER TABLE public.t_smactions OWNER TO acsdba;

--
-- Name: t_smactions_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_smactions_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_smactions_id_seq OWNER TO acsdba;

--
-- Name: t_smactions_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_smactions_id_seq OWNED BY public.t_smactions.id;


--
-- Name: t_smqueue; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_smqueue (
    packageid integer NOT NULL,
    status character varying(16) NOT NULL,
    operation character varying(16) NOT NULL,
    smid integer NOT NULL,
    creationdate timestamp(6) without time zone DEFAULT now() NOT NULL,
    parameter oid,
    startdate timestamp(6) without time zone,
    stopdate timestamp(6) without time zone,
    packagefilename character varying(128),
    synchronous boolean,
    asynchifparams text,
    extparams text
);


ALTER TABLE public.t_smqueue OWNER TO acsdba;

--
-- Name: t_smqueue_packageid_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_smqueue_packageid_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_smqueue_packageid_seq OWNER TO acsdba;

--
-- Name: t_smqueue_packageid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_smqueue_packageid_seq OWNED BY public.t_smqueue.packageid;


--
-- Name: t_smtype; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_smtype (
    id smallint NOT NULL,
    type character varying(16) NOT NULL
);


ALTER TABLE public.t_smtype OWNER TO acsdba;

--
-- Name: t_sor_requests; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_sor_requests (
    id_sor_request integer NOT NULL,
    spacecraftid character varying(24),
    requestid character(8) NOT NULL,
    raiseddate timestamp(6) without time zone DEFAULT now() NOT NULL,
    originator character varying(24),
    status character varying(24) NOT NULL,
    statusupdated timestamp(6) without time zone DEFAULT now() NOT NULL,
    emailtitle character varying(128) NOT NULL,
    emailbody text
);


ALTER TABLE public.t_sor_requests OWNER TO acsdba;

--
-- Name: TABLE t_sor_requests; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON TABLE public.t_sor_requests IS 'Contains the list of SOR (Special Operation Request)';


--
-- Name: COLUMN t_sor_requests.id_sor_request; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_sor_requests.id_sor_request IS 'Record identifier (Primary Key)';


--
-- Name: COLUMN t_sor_requests.spacecraftid; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_sor_requests.spacecraftid IS 'Spacecraft Identifier eg: Sentinel-1A';


--
-- Name: COLUMN t_sor_requests.requestid; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_sor_requests.requestid IS 'The unique originator request identifier. It is composed by year and a 3 value progressive number. eg: 2012-001';


--
-- Name: COLUMN t_sor_requests.raiseddate; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_sor_requests.raiseddate IS 'When SOR was generated';


--
-- Name: COLUMN t_sor_requests.originator; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_sor_requests.originator IS 'The name of the person raising the SOR';


--
-- Name: COLUMN t_sor_requests.status; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_sor_requests.status IS 'Status of SOR. Can be: Requested, Planned or Re-Planned, Executed or Abandoned';


--
-- Name: COLUMN t_sor_requests.statusupdated; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_sor_requests.statusupdated IS 'When the SOR status has been changed';


--
-- Name: COLUMN t_sor_requests.emailtitle; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_sor_requests.emailtitle IS 'The email subject. Is a naming convention composed by: <SpacecraftID> SOR <RequestID> <Title> - <Status>';


--
-- Name: COLUMN t_sor_requests.emailbody; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_sor_requests.emailbody IS 'Contain the body of the email';


--
-- Name: t_sor_requests_id_sor_request_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_sor_requests_id_sor_request_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_sor_requests_id_sor_request_seq OWNER TO acsdba;

--
-- Name: t_sor_requests_id_sor_request_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_sor_requests_id_sor_request_seq OWNED BY public.t_sor_requests.id_sor_request;


--
-- Name: SEQUENCE t_sor_requests_id_sor_request_seq; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON SEQUENCE public.t_sor_requests_id_sor_request_seq IS 'Used to generate the primary keys of the t_sor_requests table';


--
-- Name: t_stafinvrelation; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_stafinvrelation (
    id integer NOT NULL,
    remotepath character varying(128) NOT NULL
);


ALTER TABLE public.t_stafinvrelation OWNER TO acsdba;

--
-- Name: t_standingdistribution; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_standingdistribution (
    id integer NOT NULL,
    distpoliciesid integer NOT NULL,
    validitystart timestamp(6) without time zone NOT NULL,
    validitystop timestamp(6) without time zone NOT NULL,
    geographiclocalization character varying(32000) NOT NULL,
    cartsid integer,
    data text,
    subrequest integer NOT NULL,
    CONSTRAINT ck_stendingdistribution01 CHECK ((validitystart <= validitystop))
);


ALTER TABLE public.t_standingdistribution OWNER TO acsdba;

--
-- Name: t_standingdistribution_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_standingdistribution_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_standingdistribution_id_seq OWNER TO acsdba;

--
-- Name: t_standingdistribution_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_standingdistribution_id_seq OWNED BY public.t_standingdistribution.id;


--
-- Name: t_statetransitions_allowed; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_statetransitions_allowed (
    id integer NOT NULL,
    table_id integer NOT NULL,
    prev_state_id smallint NOT NULL,
    post_state_id smallint NOT NULL
);


ALTER TABLE public.t_statetransitions_allowed OWNER TO acsdba;

--
-- Name: TABLE t_statetransitions_allowed; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON TABLE public.t_statetransitions_allowed IS 'Contains the list of admitted transitions for each table';


--
-- Name: COLUMN t_statetransitions_allowed.id; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_statetransitions_allowed.id IS 'Record identifier (Primary Key)';


--
-- Name: COLUMN t_statetransitions_allowed.table_id; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_statetransitions_allowed.table_id IS 'Is the identifier of the table to which the status change is associated. References the t_statetransitions_tables table';


--
-- Name: COLUMN t_statetransitions_allowed.prev_state_id; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_statetransitions_allowed.prev_state_id IS 'Is the identifier to the previous value of the changing status. References the t_stati table';


--
-- Name: COLUMN t_statetransitions_allowed.post_state_id; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_statetransitions_allowed.post_state_id IS 'Is the identifier to the new value of the changing status. References the t_stati table';


--
-- Name: t_statetransitions_allowed_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_statetransitions_allowed_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_statetransitions_allowed_id_seq OWNER TO acsdba;

--
-- Name: t_statetransitions_allowed_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_statetransitions_allowed_id_seq OWNED BY public.t_statetransitions_allowed.id;


--
-- Name: t_statetransitions_flows; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_statetransitions_flows (
    id smallint NOT NULL,
    flow_type character varying(30) NOT NULL
);


ALTER TABLE public.t_statetransitions_flows OWNER TO acsdba;

--
-- Name: TABLE t_statetransitions_flows; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON TABLE public.t_statetransitions_flows IS 'Contains the list of possible data flows for tables where the status change is checked';


--
-- Name: COLUMN t_statetransitions_flows.id; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_statetransitions_flows.id IS 'Record identifier (Primary Key)';


--
-- Name: COLUMN t_statetransitions_flows.flow_type; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_statetransitions_flows.flow_type IS 'The type of flow (eg: RETRIEVE, ARCHIVE...). "none" where there are no different flows';


--
-- Name: t_statetransitions_tables; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_statetransitions_tables (
    id integer NOT NULL,
    table_name character varying(30) NOT NULL,
    flow_id smallint DEFAULT 1 NOT NULL
);


ALTER TABLE public.t_statetransitions_tables OWNER TO acsdba;

--
-- Name: TABLE t_statetransitions_tables; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON TABLE public.t_statetransitions_tables IS 'Contains the list of tables having the "status" field to analyze';


--
-- Name: COLUMN t_statetransitions_tables.id; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_statetransitions_tables.id IS 'Record identifier (Primary Key)';


--
-- Name: COLUMN t_statetransitions_tables.table_name; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_statetransitions_tables.table_name IS 'The name of the table';


--
-- Name: COLUMN t_statetransitions_tables.flow_id; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_statetransitions_tables.flow_id IS 'This field is used when in a table there may be the same status transitions, but in different flows. The default is 1 that mean "none". References the t_statetransitions_flows table';


--
-- Name: t_statetransitions_tables_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_statetransitions_tables_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_statetransitions_tables_id_seq OWNER TO acsdba;

--
-- Name: t_statetransitions_tables_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_statetransitions_tables_id_seq OWNED BY public.t_statetransitions_tables.id;


--
-- Name: t_stati; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_stati (
    id smallint NOT NULL,
    status character varying(32) NOT NULL
);


ALTER TABLE public.t_stati OWNER TO acsdba;

--
-- Name: t_stations; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_stations (
    stationid smallint NOT NULL,
    stationacronym character varying(4) NOT NULL,
    stationname character varying(64) NOT NULL,
    "position" character varying(60)
);


ALTER TABLE public.t_stations OWNER TO acsdba;

--
-- Name: t_storagegroup; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_storagegroup (
    id integer NOT NULL,
    filetype character varying(16) NOT NULL,
    storagemanager integer NOT NULL,
    groupname character varying(64) NOT NULL
);


ALTER TABLE public.t_storagegroup OWNER TO acsdba;

--
-- Name: t_storagegroup_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_storagegroup_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_storagegroup_id_seq OWNER TO acsdba;

--
-- Name: t_storagegroup_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_storagegroup_id_seq OWNED BY public.t_storagegroup.id;


--
-- Name: t_storagemanager; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_storagemanager (
    id integer NOT NULL,
    type character varying(16) NOT NULL,
    name character varying(24) NOT NULL,
    description character varying(64) NOT NULL,
    invrelationtable character varying(32) NOT NULL,
    parameters text,
    syncupload boolean DEFAULT true NOT NULL,
    syncdownload boolean DEFAULT true NOT NULL,
    syncerase boolean DEFAULT true NOT NULL,
    allowdefaultcost boolean DEFAULT true NOT NULL,
    migrationalgo_id integer,
    migrationalgo_par text,
    error_recovery_period integer DEFAULT 60
);


ALTER TABLE public.t_storagemanager OWNER TO acsdba;

--
-- Name: t_storagemanager_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_storagemanager_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_storagemanager_id_seq OWNER TO acsdba;

--
-- Name: t_storagemanager_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_storagemanager_id_seq OWNED BY public.t_storagemanager.id;


--
-- Name: t_storagepriority; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_storagepriority (
    id integer NOT NULL,
    filetype character varying(16) NOT NULL,
    storecost integer NOT NULL,
    retrievecost integer NOT NULL,
    erasecost integer NOT NULL,
    parameters text
);


ALTER TABLE public.t_storagepriority OWNER TO acsdba;

--
-- Name: t_storagerollingthresholds; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_storagerollingthresholds (
    storagemanager_id integer NOT NULL,
    threshold_mb bigint NOT NULL,
    action_id integer NOT NULL
);


ALTER TABLE public.t_storagerollingthresholds OWNER TO acsdba;

--
-- Name: t_streamers; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_streamers (
    id bigint NOT NULL,
    streamtype character varying(16) NOT NULL
);


ALTER TABLE public.t_streamers OWNER TO acsdba;

--
-- Name: t_streamers_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_streamers_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_streamers_id_seq OWNER TO acsdba;

--
-- Name: t_streamers_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_streamers_id_seq OWNED BY public.t_streamers.id;


--
-- Name: t_stylesheetoutputtypes; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_stylesheetoutputtypes (
    id smallint NOT NULL,
    outputtype character varying(8) NOT NULL
);


ALTER TABLE public.t_stylesheetoutputtypes OWNER TO acsdba;

--
-- Name: TABLE t_stylesheetoutputtypes; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON TABLE public.t_stylesheetoutputtypes IS 'Contains the list of output types for reports generated by a stylesheet (eg: XML, HTML)';


--
-- Name: COLUMN t_stylesheetoutputtypes.id; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_stylesheetoutputtypes.id IS 'Record identifier (Primary Key)';


--
-- Name: COLUMN t_stylesheetoutputtypes.outputtype; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_stylesheetoutputtypes.outputtype IS 'Type of output (eg: XML, HTML)';


--
-- Name: t_tasktables; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_tasktables (
    id_tasktable integer NOT NULL,
    name character varying(64) NOT NULL,
    tasktable text,
    type_id smallint,
    description character varying(256)
);


ALTER TABLE public.t_tasktables OWNER TO acsdba;

--
-- Name: t_tasktables_id_tasktable_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_tasktables_id_tasktable_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_tasktables_id_tasktable_seq OWNER TO acsdba;

--
-- Name: t_tasktables_id_tasktable_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_tasktables_id_tasktable_seq OWNED BY public.t_tasktables.id_tasktable;


--
-- Name: t_temporarydistribution; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_temporarydistribution (
    id integer NOT NULL,
    duedate timestamp(6) without time zone NOT NULL,
    distributeddate timestamp(6) without time zone,
    creationdate timestamp(6) without time zone DEFAULT now(),
    filename character varying(128) NOT NULL,
    fileversion character varying(32) NOT NULL,
    mediatype character varying(16) NOT NULL,
    userid character varying(32) NOT NULL,
    format integer,
    algo integer NOT NULL,
    tstamp timestamp(6) without time zone DEFAULT now(),
    policy integer NOT NULL,
    orderid integer,
    preselection boolean DEFAULT false NOT NULL,
    cartid integer,
    subreq integer,
    disseminationpackingalgo_id integer NOT NULL
);


ALTER TABLE public.t_temporarydistribution OWNER TO acsdba;

--
-- Name: t_temporarydistribution_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_temporarydistribution_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_temporarydistribution_id_seq OWNER TO acsdba;

--
-- Name: t_temporarydistribution_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_temporarydistribution_id_seq OWNED BY public.t_temporarydistribution.id;


--
-- Name: t_temporaryorders; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_temporaryorders (
    id integer NOT NULL,
    filename character varying(128) NOT NULL,
    fileversion character varying(32) NOT NULL,
    algorithm character varying(32) NOT NULL,
    filetype character varying(16) NOT NULL,
    ordertype character varying(8) NOT NULL,
    processorname character varying(32) NOT NULL,
    processorversion character varying(8) NOT NULL,
    processorconfiguration character varying(8) NOT NULL,
    priority smallint NOT NULL,
    troubleshooting boolean NOT NULL,
    processingtimewindow smallint NOT NULL,
    ordersat smallint,
    tstamp timestamp(6) without time zone DEFAULT now() NOT NULL,
    emittime timestamp(6) without time zone,
    time_driven_start timestamp without time zone,
    time_driven_stop timestamp without time zone
);


ALTER TABLE public.t_temporaryorders OWNER TO acsdba;

--
-- Name: t_temporaryorders_id_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_temporaryorders_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_temporaryorders_id_seq OWNER TO acsdba;

--
-- Name: t_temporaryorders_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_temporaryorders_id_seq OWNED BY public.t_temporaryorders.id;


--
-- Name: t_tmpnamestrategy; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_tmpnamestrategy (
    id_tmpnamestrategy smallint NOT NULL,
    tempname character varying(24) NOT NULL
);


ALTER TABLE public.t_tmpnamestrategy OWNER TO acsdba;

--
-- Name: t_transferprotocols; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_transferprotocols (
    id smallint NOT NULL,
    protocol character varying(16) NOT NULL
);


ALTER TABLE public.t_transferprotocols OWNER TO acsdba;

--
-- Name: t_usdfwa; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_usdfwa (
    id_usdfwa integer NOT NULL,
    filename character varying(128) NOT NULL,
    fileversion character varying(32) NOT NULL,
    mmmcupdatefile text,
    modificationdate timestamp(6) without time zone DEFAULT now()
);


ALTER TABLE public.t_usdfwa OWNER TO acsdba;

--
-- Name: COLUMN t_usdfwa.id_usdfwa; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_usdfwa.id_usdfwa IS 'Record identifier (Primary Key)';


--
-- Name: COLUMN t_usdfwa.filename; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_usdfwa.filename IS 'References filename on t_inventory table.';


--
-- Name: COLUMN t_usdfwa.fileversion; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_usdfwa.fileversion IS 'References fileversion on t_inventory table.';


--
-- Name: t_usdfwa_delivery_details; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_usdfwa_delivery_details (
    id_delivery_details integer NOT NULL,
    usdfwa_id integer NOT NULL,
    url character varying(100),
    deliverydate timestamp(6) without time zone DEFAULT now(),
    itemstatus character varying(16),
    statusdetails character varying(100)
);


ALTER TABLE public.t_usdfwa_delivery_details OWNER TO acsdba;

--
-- Name: COLUMN t_usdfwa_delivery_details.id_delivery_details; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON COLUMN public.t_usdfwa_delivery_details.id_delivery_details IS 'Record identifier (Primary Key)';


--
-- Name: t_usdfwa_delivery_details_id_delivery_details_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_usdfwa_delivery_details_id_delivery_details_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_usdfwa_delivery_details_id_delivery_details_seq OWNER TO acsdba;

--
-- Name: t_usdfwa_delivery_details_id_delivery_details_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_usdfwa_delivery_details_id_delivery_details_seq OWNED BY public.t_usdfwa_delivery_details.id_delivery_details;


--
-- Name: SEQUENCE t_usdfwa_delivery_details_id_delivery_details_seq; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON SEQUENCE public.t_usdfwa_delivery_details_id_delivery_details_seq IS 'Used to generate the primary keys of the t_usdfwa_delivery_details table';


--
-- Name: t_usdfwa_id_usdfwa_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_usdfwa_id_usdfwa_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_usdfwa_id_usdfwa_seq OWNER TO acsdba;

--
-- Name: t_usdfwa_id_usdfwa_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_usdfwa_id_usdfwa_seq OWNED BY public.t_usdfwa.id_usdfwa;


--
-- Name: SEQUENCE t_usdfwa_id_usdfwa_seq; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON SEQUENCE public.t_usdfwa_id_usdfwa_seq IS 'Used to generate the primary keys of the t_usdfwa table';


--
-- Name: t_usermodes; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_usermodes (
    id smallint NOT NULL,
    mode character varying(16) NOT NULL
);


ALTER TABLE public.t_usermodes OWNER TO acsdba;

--
-- Name: t_users; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_users (
    id_user integer NOT NULL,
    userid character varying(32) NOT NULL,
    firstname character varying(32),
    lastname character varying(32),
    title character varying(8),
    organization character varying(32),
    department character varying(32),
    fax character varying(24),
    phone character varying(24),
    postaladdress character varying(128),
    projectandprocessing character varying(128),
    packagerequired boolean NOT NULL,
    compresspackage boolean,
    compressionmode character varying(16),
    usertype character varying(12) NOT NULL,
    packagemaxsize integer,
    progmedianumber integer NOT NULL,
    emailaddress character varying(64),
    allqualified boolean DEFAULT false NOT NULL,
    mode smallint DEFAULT 1 NOT NULL
);


ALTER TABLE public.t_users OWNER TO acsdba;

--
-- Name: t_users_id_user_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_users_id_user_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_users_id_user_seq OWNER TO acsdba;

--
-- Name: t_users_id_user_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_users_id_user_seq OWNED BY public.t_users.id_user;


--
-- Name: t_usertypes; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_usertypes (
    usertype character varying(12) NOT NULL,
    description character varying(64) NOT NULL
);


ALTER TABLE public.t_usertypes OWNER TO acsdba;

--
-- Name: t_waitingorders; Type: TABLE; Schema: public; Owner: acsdba
--

CREATE TABLE public.t_waitingorders (
    waitorderid integer NOT NULL,
    orderid integer NOT NULL,
    filetype character varying(16) NOT NULL,
    starttime timestamp(6) without time zone NOT NULL,
    stoptime timestamp(6) without time zone NOT NULL,
    triggertype character varying(16) NOT NULL,
    generationdate timestamp(6) without time zone DEFAULT now() NOT NULL,
    expirationdate timestamp(6) without time zone,
    CONSTRAINT ck_waitingorders CHECK ((stoptime >= starttime)),
    CONSTRAINT ck_waitingorders02 CHECK (((((triggertype)::text = 'DELAY'::text) AND (expirationdate IS NULL)) <> true))
);


ALTER TABLE public.t_waitingorders OWNER TO acsdba;

--
-- Name: t_waitingorders_waitorderid_seq; Type: SEQUENCE; Schema: public; Owner: acsdba
--

CREATE SEQUENCE public.t_waitingorders_waitorderid_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER TABLE public.t_waitingorders_waitorderid_seq OWNER TO acsdba;

--
-- Name: t_waitingorders_waitorderid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: acsdba
--

ALTER SEQUENCE public.t_waitingorders_waitorderid_seq OWNED BY public.t_waitingorders.waitorderid;


--
-- Name: v_distribution; Type: VIEW; Schema: public; Owner: acsdba
--

CREATE VIEW public.v_distribution AS
 SELECT DISTINCT dq.packageid,
    dq.status,
    dq.distributeddate,
    dq.jobresponsible,
    dq.creationdate,
    di.mediatype,
    di.userid,
    di.distributionrule_id
   FROM (public.t_distributionqueue dq
     JOIN public.t_distributionitems di ON ((dq.packageid = di.packageid)));


ALTER TABLE public.v_distribution OWNER TO acsdba;

--
-- Name: v_distributionrules; Type: VIEW; Schema: public; Owner: acsdba
--

CREATE VIEW public.v_distributionrules AS
 SELECT ft.filetype,
    us.userid AS username,
    um.mode AS usermode,
    mt.mediatype,
    kh.host_name AS repository_host,
    tp.protocol AS repository_prot,
    da.name AS distributionalgo,
    tt.name AS tasktable,
    fc.fileclass,
    dp.factorykey AS disseminationpackingalgo,
    dr.active,
    dr.ondemand
   FROM (((((((((((public.t_distributionrules dr
     JOIN public.t_filetypes ft ON ((dr.filetype_id = ft.id_filetype)))
     JOIN public.t_users us ON ((dr.user_id = us.id_user)))
     JOIN public.t_usermodes um ON ((dr.usermode_id = um.id)))
     JOIN public.t_mediatypes mt ON ((dr.mediatype_id = mt.id_mediatype)))
     JOIN public.t_repositories rp ON ((dr.repository_id = rp.id_repository)))
     JOIN public.t_knownhosts kh ON ((rp.host_id = kh.id)))
     JOIN public.t_transferprotocols tp ON ((rp.transferprotocol_id = tp.id)))
     JOIN public.t_distributionalgo da ON ((dr.distributionalgo_id = da.id)))
     JOIN public.t_tasktables tt ON ((dr.format_tt_id = tt.id_tasktable)))
     JOIN public.t_disseminationpackingalgos dp ON ((dr.disseminationpackingalgo_id = dp.id_disseminationpackingalgo)))
     LEFT JOIN public.t_fileclasses fc ON ((dr.fileclass_id = fc.id_fileclass)))
  ORDER BY dr.filetype_id, dr.user_id;


ALTER TABLE public.v_distributionrules OWNER TO acsdba;

--
-- Name: v_filetypes_x_stylesheets; Type: VIEW; Schema: public; Owner: acsdba
--

CREATE VIEW public.v_filetypes_x_stylesheets AS
 SELECT ft.filetype,
    cs.name AS stylesheet,
    ot.outputtype,
    fs.fileextension
   FROM (((public.t_filetypes_x_stylesheets fs
     JOIN public.t_filetypes ft ON ((fs.filetype_id = ft.id_filetype)))
     JOIN public.t_configurationstylesheets cs ON ((fs.stylesheet_id = cs.id)))
     JOIN public.t_stylesheetoutputtypes ot ON ((fs.outputtype_id = ot.id)))
  ORDER BY ft.filetype, cs.stylesheet, ot.outputtype, fs.fileextension;


ALTER TABLE public.v_filetypes_x_stylesheets OWNER TO acsdba;

--
-- Name: VIEW v_filetypes_x_stylesheets; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON VIEW public.v_filetypes_x_stylesheets IS 'Joins t_filetypes_x_stylesheets, t_filetypes, t_configurationstylesheets and t_stylesheetoutputtypes tables.';


--
-- Name: v_mpm_cross; Type: VIEW; Schema: public; Owner: acsdba
--

CREATE VIEW public.v_mpm_cross AS
 SELECT vl.value_name AS value,
    pm.parameter_name AS parameter,
    rt.routine_name AS routine,
    pr.productname,
    pr.productversion
   FROM ((((public.t_mpm_cross cr
     JOIN public.t_mpm_values vl ON ((cr.value_id = vl.id_value)))
     JOIN public.t_mpm_parameters pm ON ((cr.parameter_id = pm.id_parameter)))
     JOIN public.t_mpm_routines rt ON ((cr.routine_id = rt.id_routine)))
     JOIN public.t_mpm_products pr ON ((cr.product_id = pr.id_product)));


ALTER TABLE public.v_mpm_cross OWNER TO acsdba;

--
-- Name: v_proc_ftype_generation; Type: VIEW; Schema: public; Owner: acsdba
--

CREATE VIEW public.v_proc_ftype_generation AS
 SELECT pr.processorname,
    pr.processorversion,
    ft.filetype
   FROM ((public.t_proc_ftype_generation pf
     JOIN public.t_processors pr ON ((pf.proc_id = pr.id_processor)))
     JOIN public.t_filetypes ft ON ((pf.ftype_id = ft.id_filetype)));


ALTER TABLE public.v_proc_ftype_generation OWNER TO acsdba;

--
-- Name: v_receptionrules; Type: VIEW; Schema: public; Owner: acsdba
--

CREATE VIEW public.v_receptionrules AS
 SELECT ft.filetype,
    us.userid AS username,
    um.mode AS usermode,
    rp.symbolicname AS repository,
    rr.localpath,
    tt.name AS tasktable,
    rr.delete_on_remote,
    rr.active
   FROM (((((public.t_receptionrules rr
     JOIN public.t_filetypes ft ON ((rr.filetype_id = ft.id_filetype)))
     JOIN public.t_users us ON ((rr.user_id = us.id_user)))
     JOIN public.t_usermodes um ON ((rr.usermode_id = um.id)))
     JOIN public.t_repositories rp ON ((rr.repository_id = rp.id_repository)))
     JOIN public.t_tasktables tt ON ((rr.reception_tt_id = tt.id_tasktable)))
  ORDER BY rr.filetype_id, rr.user_id;


ALTER TABLE public.v_receptionrules OWNER TO acsdba;

--
-- Name: v_satrelations; Type: VIEW; Schema: public; Owner: acsdba
--

CREATE VIEW public.v_satrelations AS
 SELECT sa.satellitename AS satellite,
    sr.mission,
    se.sensorname AS sensor
   FROM ((public.t_satrelations sr
     JOIN public.t_satellites sa ON ((sr.satelliteid = sa.satelliteid)))
     JOIN public.t_sensors se ON ((sr.sensorid = se.sensorid)))
  ORDER BY sa.satellitename, sr.mission, se.sensorname;


ALTER TABLE public.v_satrelations OWNER TO acsdba;

--
-- Name: v_schema_version; Type: VIEW; Schema: public; Owner: acsdba
--

CREATE VIEW public.v_schema_version AS
 SELECT sc.version,
    st.schematype AS schema_type,
    sm.modificationtype AS modification_type,
    sc.modificationdate AS modification_date,
    sc.appliedon AS applied_on,
    sc.cvstag AS cvs_tag,
    sc.description
   FROM (((public.t_schema sc
     JOIN ( SELECT t_schema.schematype,
            max(public.f_get_version_number(t_schema.cvstag)) AS maxtag
           FROM public.t_schema
          GROUP BY t_schema.schematype) gsc ON (((sc.schematype = gsc.schematype) AND (public.f_get_version_number(sc.cvstag) = gsc.maxtag))))
     JOIN public.t_schematypes st ON ((sc.schematype = st.id)))
     JOIN public.t_schemamodificationtypes sm ON ((sc.modificationtype = sm.id)))
  ORDER BY sc.version;


ALTER TABLE public.v_schema_version OWNER TO acsdba;

--
-- Name: VIEW v_schema_version; Type: COMMENT; Schema: public; Owner: acsdba
--

COMMENT ON VIEW public.v_schema_version IS 'Joins t_schema, t_schematypes and t_schemamodificationtypes. Shows only the greater version for each schema contained into t_schema table';


--
-- Name: v_state_transitions; Type: VIEW; Schema: public; Owner: acsdba
--

CREATE VIEW public.v_state_transitions AS
 SELECT st.table_name,
    sf.flow_type AS flow,
    ss1.status AS prev_status,
    ss2.status AS post_status
   FROM ((((public.t_statetransitions_tables st
     JOIN public.t_statetransitions_flows sf ON ((st.flow_id = sf.id)))
     JOIN public.t_statetransitions_allowed sa ON ((st.id = sa.table_id)))
     JOIN public.t_stati ss1 ON ((sa.prev_state_id = ss1.id)))
     JOIN public.t_stati ss2 ON ((sa.post_state_id = ss2.id)))
  ORDER BY st.table_name, sf.flow_type, ss1.status, ss2.status;


ALTER TABLE public.v_state_transitions OWNER TO acsdba;

--
-- Name: t_applicationevents id_applicationevent; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_applicationevents ALTER COLUMN id_applicationevent SET DEFAULT nextval('public.t_applicationevents_id_applicationevent_seq'::regclass);


--
-- Name: t_applications id_application; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_applications ALTER COLUMN id_application SET DEFAULT nextval('public.t_applications_id_application_seq'::regclass);


--
-- Name: t_asyncsm_operations id_asyncsm_operation; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_asyncsm_operations ALTER COLUMN id_asyncsm_operation SET DEFAULT nextval('public.t_asyncsm_operations_id_asyncsm_operation_seq'::regclass);


--
-- Name: t_bands id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_bands ALTER COLUMN id SET DEFAULT nextval('public.t_bands_id_seq'::regclass);


--
-- Name: t_blockedcirculationsqueue id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_blockedcirculationsqueue ALTER COLUMN id SET DEFAULT nextval('public.t_blockedcirculationsqueue_id_seq'::regclass);


--
-- Name: t_cartsinv id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_cartsinv ALTER COLUMN id SET DEFAULT nextval('public.t_cartsinv_id_seq'::regclass);


--
-- Name: t_cartsref id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_cartsref ALTER COLUMN id SET DEFAULT nextval('public.t_cartsref_id_seq'::regclass);


--
-- Name: t_cdp_feederqueue id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_cdp_feederqueue ALTER COLUMN id SET DEFAULT nextval('public.t_cdp_feederqueue_id_seq'::regclass);


--
-- Name: t_cdp_notification_queue id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_cdp_notification_queue ALTER COLUMN id SET DEFAULT nextval('public.t_cdp_notification_queue_id_seq'::regclass);


--
-- Name: t_circ_callbacks id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circ_callbacks ALTER COLUMN id SET DEFAULT nextval('public.t_circ_callbacks_id_seq'::regclass);


--
-- Name: t_circulation_remove_queue id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulation_remove_queue ALTER COLUMN id SET DEFAULT nextval('public.t_circulation_remove_queue_id_seq'::regclass);


--
-- Name: t_circulationpolicies id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationpolicies ALTER COLUMN id SET DEFAULT nextval('public.t_circulationpolicies_id_seq'::regclass);


--
-- Name: t_circulationpolicies_monitor id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationpolicies_monitor ALTER COLUMN id SET DEFAULT nextval('public.t_circulationpolicies_monitor_id_seq'::regclass);


--
-- Name: t_circulationsmedia id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationsmedia ALTER COLUMN id SET DEFAULT nextval('public.t_circulationsmedia_id_seq'::regclass);


--
-- Name: t_circulationsqueue id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationsqueue ALTER COLUMN id SET DEFAULT nextval('public.t_circulationsqueue_id_seq'::regclass);


--
-- Name: t_circulationsqueuedwh id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationsqueuedwh ALTER COLUMN id SET DEFAULT nextval('public.t_circulationsqueuedwh_id_seq'::regclass);


--
-- Name: t_circulationstats id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationstats ALTER COLUMN id SET DEFAULT nextval('public.t_circulationstats_id_seq'::regclass);


--
-- Name: t_classifiedfiletypes id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_classifiedfiletypes ALTER COLUMN id SET DEFAULT nextval('public.t_classifiedfiletypes_id_seq'::regclass);


--
-- Name: t_cleanuprules id_cleanuprule; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_cleanuprules ALTER COLUMN id_cleanuprule SET DEFAULT nextval('public.t_cleanuprules_id_cleanuprule_seq'::regclass);


--
-- Name: t_configurationstylesheets id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_configurationstylesheets ALTER COLUMN id SET DEFAULT nextval('public.t_configurationstylesheets_id_seq'::regclass);


--
-- Name: t_cov_periods id_interval; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_cov_periods ALTER COLUMN id_interval SET DEFAULT nextval('public.t_cov_periods_id_interval_seq'::regclass);


--
-- Name: t_cov_relevancies id_relevancy; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_cov_relevancies ALTER COLUMN id_relevancy SET DEFAULT nextval('public.t_cov_relevancies_id_relevancy_seq'::regclass);


--
-- Name: t_cov_rules id_coveragerule; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_cov_rules ALTER COLUMN id_coveragerule SET DEFAULT nextval('public.t_cov_rules_id_coveragerule_seq'::regclass);


--
-- Name: t_cr_x_filetypes id_cr_x_filetype; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_cr_x_filetypes ALTER COLUMN id_cr_x_filetype SET DEFAULT nextval('public.t_cr_x_filetypes_id_cr_x_filetype_seq'::regclass);


--
-- Name: t_crgroups id_crgroup; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_crgroups ALTER COLUMN id_crgroup SET DEFAULT nextval('public.t_crgroups_id_crgroup_seq'::regclass);


--
-- Name: t_crrelations id_crrelation; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_crrelations ALTER COLUMN id_crrelation SET DEFAULT nextval('public.t_crrelations_id_crrelation_seq'::regclass);


--
-- Name: t_datastoragequeue id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_datastoragequeue ALTER COLUMN id SET DEFAULT nextval('public.t_datastoragequeue_id_seq'::regclass);


--
-- Name: t_dbparameters id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_dbparameters ALTER COLUMN id SET DEFAULT nextval('public.t_dbparameters_id_seq'::regclass);


--
-- Name: t_discoveryplugins id_discoveryplugin; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_discoveryplugins ALTER COLUMN id_discoveryplugin SET DEFAULT nextval('public.t_discoveryplugins_id_discoveryplugin_seq'::regclass);


--
-- Name: t_disseminationpackingalgos id_disseminationpackingalgo; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_disseminationpackingalgos ALTER COLUMN id_disseminationpackingalgo SET DEFAULT nextval('public.t_disseminationpackingalgos_id_disseminationpackingalgo_seq'::regclass);


--
-- Name: t_distributionitems id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_distributionitems ALTER COLUMN id SET DEFAULT nextval('public.t_distributionitems_id_seq'::regclass);


--
-- Name: t_distributionqueue packageid; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_distributionqueue ALTER COLUMN packageid SET DEFAULT nextval('public.t_distributionqueue_packageid_seq'::regclass);


--
-- Name: t_distributionrules id_distributionrule; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_distributionrules ALTER COLUMN id_distributionrule SET DEFAULT nextval('public.t_distributionrules_id_distributionrule_seq'::regclass);


--
-- Name: t_distributionwaitingitems id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_distributionwaitingitems ALTER COLUMN id SET DEFAULT nextval('public.t_distributionwaitingitems_id_seq'::regclass);


--
-- Name: t_downlink_session_tracks id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_downlink_session_tracks ALTER COLUMN id SET DEFAULT nextval('public.t_downlink_session_tracks_id_seq'::regclass);


--
-- Name: t_dsqueue_x_cirqueue id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_dsqueue_x_cirqueue ALTER COLUMN id SET DEFAULT nextval('public.t_dsqueue_x_cirqueue_id_seq'::regclass);


--
-- Name: t_dsqueue_x_cirqueue filequeue_id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_dsqueue_x_cirqueue ALTER COLUMN filequeue_id SET DEFAULT nextval('public.t_dsqueue_x_cirqueue_filequeue_id_seq'::regclass);


--
-- Name: t_emitted_order_events id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_emitted_order_events ALTER COLUMN id SET DEFAULT nextval('public.t_emitted_order_events_id_seq'::regclass);


--
-- Name: t_fileclasses id_fileclass; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_fileclasses ALTER COLUMN id_fileclass SET DEFAULT nextval('public.t_fileclasses_id_fileclass_seq'::regclass);


--
-- Name: t_filetypes id_filetype; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_filetypes ALTER COLUMN id_filetype SET DEFAULT nextval('public.t_filetypes_id_filetype_seq'::regclass);


--
-- Name: t_filetypes_last_inv id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_filetypes_last_inv ALTER COLUMN id SET DEFAULT nextval('public.t_filetypes_last_inv_id_seq'::regclass);


--
-- Name: t_filetypes_x_migrations id_filetypemigration; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_filetypes_x_migrations ALTER COLUMN id_filetypemigration SET DEFAULT nextval('public.t_filetypes_x_migrations_id_filetypemigration_seq'::regclass);


--
-- Name: t_filetypes_x_satellites id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_filetypes_x_satellites ALTER COLUMN id SET DEFAULT nextval('public.t_filetypes_x_satellites_id_seq'::regclass);


--
-- Name: t_filetypes_x_stylesheets id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_filetypes_x_stylesheets ALTER COLUMN id SET DEFAULT nextval('public.t_filetypes_x_stylesheets_id_seq'::regclass);


--
-- Name: t_filetypescomps id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_filetypescomps ALTER COLUMN id SET DEFAULT nextval('public.t_filetypescomps_id_seq'::regclass);


--
-- Name: t_flowtags id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_flowtags ALTER COLUMN id SET DEFAULT nextval('public.t_flowtags_id_seq'::regclass);


--
-- Name: t_inv_additional_params id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inv_additional_params ALTER COLUMN id SET DEFAULT nextval('public.t_inv_additional_params_id_seq'::regclass);


--
-- Name: t_inv_baselines id_inv_baseline; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inv_baselines ALTER COLUMN id_inv_baseline SET DEFAULT nextval('public.t_inv_baselines_id_inv_baseline_seq'::regclass);


--
-- Name: t_inv_client id_inv_client; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inv_client ALTER COLUMN id_inv_client SET DEFAULT nextval('public.t_inv_client_id_inv_client_seq'::regclass);


--
-- Name: t_inv_relations id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inv_relations ALTER COLUMN id SET DEFAULT nextval('public.t_inv_relations_id_seq'::regclass);


--
-- Name: t_inv_x_baselines id_inv_x_baseline; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inv_x_baselines ALTER COLUMN id_inv_x_baseline SET DEFAULT nextval('public.t_inv_x_baselines_id_inv_x_baseline_seq'::regclass);


--
-- Name: t_inventory id_inv; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventory ALTER COLUMN id_inv SET DEFAULT nextval('public.t_inventory_id_inv_seq'::regclass);


--
-- Name: t_inventoryanomalies id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventoryanomalies ALTER COLUMN id SET DEFAULT nextval('public.t_inventoryanomalies_id_seq'::regclass);


--
-- Name: t_inventorycomments id_inventorycomment; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventorycomments ALTER COLUMN id_inventorycomment SET DEFAULT nextval('public.t_inventorycomments_id_inventorycomment_seq'::regclass);


--
-- Name: t_inventoryevents id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventoryevents ALTER COLUMN id SET DEFAULT nextval('public.t_inventoryevents_id_seq'::regclass);


--
-- Name: t_inventoryeventsconf id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventoryeventsconf ALTER COLUMN id SET DEFAULT nextval('public.t_inventoryeventsconf_id_seq'::regclass);


--
-- Name: t_inventorygaps inventorygapid; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventorygaps ALTER COLUMN inventorygapid SET DEFAULT nextval('public.t_inventorygaps_inventorygapid_seq'::regclass);


--
-- Name: t_inventorylinks id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventorylinks ALTER COLUMN id SET DEFAULT nextval('public.t_inventorylinks_id_seq'::regclass);


--
-- Name: t_inventorylinksconf id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventorylinksconf ALTER COLUMN id SET DEFAULT nextval('public.t_inventorylinksconf_id_seq'::regclass);


--
-- Name: t_inventorysm id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventorysm ALTER COLUMN id SET DEFAULT nextval('public.t_inventorysm_id_seq'::regclass);


--
-- Name: t_invrollingpolicies id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_invrollingpolicies ALTER COLUMN id SET DEFAULT nextval('public.t_invrollingpolicies_id_seq'::regclass);


--
-- Name: t_invsiterelation id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_invsiterelation ALTER COLUMN id SET DEFAULT nextval('public.t_invsiterelation_id_seq'::regclass);


--
-- Name: t_invspecialrollingpolicies id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_invspecialrollingpolicies ALTER COLUMN id SET DEFAULT nextval('public.t_invspecialrollingpolicies_id_seq'::regclass);


--
-- Name: t_knownhosts id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_knownhosts ALTER COLUMN id SET DEFAULT nextval('public.t_knownhosts_id_seq'::regclass);


--
-- Name: t_knownhosts_limits id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_knownhosts_limits ALTER COLUMN id SET DEFAULT nextval('public.t_knownhosts_limits_id_seq'::regclass);


--
-- Name: t_lastacknowledge id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_lastacknowledge ALTER COLUMN id SET DEFAULT nextval('public.t_lastacknowledge_id_seq'::regclass);


--
-- Name: t_measdataset id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_measdataset ALTER COLUMN id SET DEFAULT nextval('public.t_measdataset_id_seq'::regclass);


--
-- Name: t_migrationalgos id_migrationalgo; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_migrationalgos ALTER COLUMN id_migrationalgo SET DEFAULT nextval('public.t_migrationalgos_id_migrationalgo_seq'::regclass);


--
-- Name: t_mpm_cross id_cross; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mpm_cross ALTER COLUMN id_cross SET DEFAULT nextval('public.t_mpm_cross_id_cross_seq'::regclass);


--
-- Name: t_mpm_dip id_dip; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mpm_dip ALTER COLUMN id_dip SET DEFAULT nextval('public.t_mpm_dip_id_dip_seq'::regclass);


--
-- Name: t_mpm_ipf_adf_check id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mpm_ipf_adf_check ALTER COLUMN id SET DEFAULT nextval('public.t_mpm_ipf_adf_check_id_seq'::regclass);


--
-- Name: t_mpm_parameters id_parameter; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mpm_parameters ALTER COLUMN id_parameter SET DEFAULT nextval('public.t_mpm_parameters_id_parameter_seq'::regclass);


--
-- Name: t_mpm_prb id_prb; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mpm_prb ALTER COLUMN id_prb SET DEFAULT nextval('public.t_mpm_prb_id_prb_seq'::regclass);


--
-- Name: t_mpm_routines id_routine; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mpm_routines ALTER COLUMN id_routine SET DEFAULT nextval('public.t_mpm_routines_id_routine_seq'::regclass);


--
-- Name: t_mpm_values id_value; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mpm_values ALTER COLUMN id_value SET DEFAULT nextval('public.t_mpm_values_id_value_seq'::regclass);


--
-- Name: t_mpm_values_part1 id_value; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mpm_values_part1 ALTER COLUMN id_value SET DEFAULT nextval('public.t_mpm_values_id_value_seq'::regclass);


--
-- Name: t_mpm_values_part1 creationtime; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mpm_values_part1 ALTER COLUMN creationtime SET DEFAULT now();


--
-- Name: t_mpm_values_part2 id_value; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mpm_values_part2 ALTER COLUMN id_value SET DEFAULT nextval('public.t_mpm_values_id_value_seq'::regclass);


--
-- Name: t_mpm_values_part2 creationtime; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mpm_values_part2 ALTER COLUMN creationtime SET DEFAULT now();


--
-- Name: t_operation_requests id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_operation_requests ALTER COLUMN id SET DEFAULT nextval('public.t_operation_requests_id_seq'::regclass);


--
-- Name: t_ordersattachments id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_ordersattachments ALTER COLUMN id SET DEFAULT nextval('public.t_ordersattachments_id_seq'::regclass);


--
-- Name: t_ordersattachmenttypes id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_ordersattachmenttypes ALTER COLUMN id SET DEFAULT nextval('public.t_ordersattachmenttypes_id_seq'::regclass);


--
-- Name: t_ordersgeneration id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_ordersgeneration ALTER COLUMN id SET DEFAULT nextval('public.t_ordersgeneration_id_seq'::regclass);


--
-- Name: t_ordersinput id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_ordersinput ALTER COLUMN id SET DEFAULT nextval('public.t_ordersinput_id_seq'::regclass);


--
-- Name: t_ordersqueue orderid; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_ordersqueue ALTER COLUMN orderid SET DEFAULT nextval('public.t_ordersqueue_orderid_seq'::regclass);


--
-- Name: t_ordersqueueblacklist id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_ordersqueueblacklist ALTER COLUMN id SET DEFAULT nextval('public.t_ordersqueueblacklist_id_seq'::regclass);


--
-- Name: t_orderstatetransitions id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_orderstatetransitions ALTER COLUMN id SET DEFAULT nextval('public.t_orderstatetransitions_id_seq'::regclass);


--
-- Name: t_ordtriggeringfiles id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_ordtriggeringfiles ALTER COLUMN id SET DEFAULT nextval('public.t_ordtriggeringfiles_id_seq'::regclass);


--
-- Name: t_pdsmodes id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_pdsmodes ALTER COLUMN id SET DEFAULT nextval('public.t_pdsmodes_id_seq'::regclass);


--
-- Name: t_postinvqueue id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_postinvqueue ALTER COLUMN id SET DEFAULT nextval('public.t_postinvqueue_id_seq'::regclass);


--
-- Name: t_processors id_processor; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_processors ALTER COLUMN id_processor SET DEFAULT nextval('public.t_processors_id_processor_seq'::regclass);


--
-- Name: t_qualifiedflag id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_qualifiedflag ALTER COLUMN id SET DEFAULT nextval('public.t_qualifiedflag_id_seq'::regclass);


--
-- Name: t_receptionrules id_receptionrule; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_receptionrules ALTER COLUMN id_receptionrule SET DEFAULT nextval('public.t_receptionrules_id_receptionrule_seq'::regclass);


--
-- Name: t_receptionruleshist id_receptionrulehist; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_receptionruleshist ALTER COLUMN id_receptionrulehist SET DEFAULT nextval('public.t_receptionruleshist_id_receptionrulehist_seq'::regclass);


--
-- Name: t_repositories id_repository; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_repositories ALTER COLUMN id_repository SET DEFAULT nextval('public.t_repositories_id_repository_seq'::regclass);


--
-- Name: t_rollingpoliciesalgo id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_rollingpoliciesalgo ALTER COLUMN id SET DEFAULT nextval('public.t_rollingpoliciesalgo_id_seq'::regclass);


--
-- Name: t_rollingpolicyactions id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_rollingpolicyactions ALTER COLUMN id SET DEFAULT nextval('public.t_rollingpolicyactions_id_seq'::regclass);


--
-- Name: t_schema id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_schema ALTER COLUMN id SET DEFAULT nextval('public.t_schema_id_seq'::regclass);


--
-- Name: t_securityzonerelays id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_securityzonerelays ALTER COLUMN id SET DEFAULT nextval('public.t_securityzonerelays_id_seq'::regclass);


--
-- Name: t_securityzones id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_securityzones ALTER COLUMN id SET DEFAULT nextval('public.t_securityzones_id_seq'::regclass);


--
-- Name: t_sites id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_sites ALTER COLUMN id SET DEFAULT nextval('public.t_sites_id_seq'::regclass);


--
-- Name: t_sitetype id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_sitetype ALTER COLUMN id SET DEFAULT nextval('public.t_sitetype_id_seq'::regclass);


--
-- Name: t_sm_sync_missingactions id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_sm_sync_missingactions ALTER COLUMN id SET DEFAULT nextval('public.t_sm_sync_missingactions_id_seq'::regclass);


--
-- Name: t_smactions id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_smactions ALTER COLUMN id SET DEFAULT nextval('public.t_smactions_id_seq'::regclass);


--
-- Name: t_smqueue packageid; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_smqueue ALTER COLUMN packageid SET DEFAULT nextval('public.t_smqueue_packageid_seq'::regclass);


--
-- Name: t_sor_requests id_sor_request; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_sor_requests ALTER COLUMN id_sor_request SET DEFAULT nextval('public.t_sor_requests_id_sor_request_seq'::regclass);


--
-- Name: t_standingdistribution id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_standingdistribution ALTER COLUMN id SET DEFAULT nextval('public.t_standingdistribution_id_seq'::regclass);


--
-- Name: t_statetransitions_allowed id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_statetransitions_allowed ALTER COLUMN id SET DEFAULT nextval('public.t_statetransitions_allowed_id_seq'::regclass);


--
-- Name: t_statetransitions_tables id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_statetransitions_tables ALTER COLUMN id SET DEFAULT nextval('public.t_statetransitions_tables_id_seq'::regclass);


--
-- Name: t_storagegroup id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_storagegroup ALTER COLUMN id SET DEFAULT nextval('public.t_storagegroup_id_seq'::regclass);


--
-- Name: t_storagemanager id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_storagemanager ALTER COLUMN id SET DEFAULT nextval('public.t_storagemanager_id_seq'::regclass);


--
-- Name: t_streamers id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_streamers ALTER COLUMN id SET DEFAULT nextval('public.t_streamers_id_seq'::regclass);


--
-- Name: t_tasktables id_tasktable; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_tasktables ALTER COLUMN id_tasktable SET DEFAULT nextval('public.t_tasktables_id_tasktable_seq'::regclass);


--
-- Name: t_temporarydistribution id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_temporarydistribution ALTER COLUMN id SET DEFAULT nextval('public.t_temporarydistribution_id_seq'::regclass);


--
-- Name: t_temporaryorders id; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_temporaryorders ALTER COLUMN id SET DEFAULT nextval('public.t_temporaryorders_id_seq'::regclass);


--
-- Name: t_usdfwa id_usdfwa; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_usdfwa ALTER COLUMN id_usdfwa SET DEFAULT nextval('public.t_usdfwa_id_usdfwa_seq'::regclass);


--
-- Name: t_usdfwa_delivery_details id_delivery_details; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_usdfwa_delivery_details ALTER COLUMN id_delivery_details SET DEFAULT nextval('public.t_usdfwa_delivery_details_id_delivery_details_seq'::regclass);


--
-- Name: t_users id_user; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_users ALTER COLUMN id_user SET DEFAULT nextval('public.t_users_id_user_seq'::regclass);


--
-- Name: t_waitingorders waitorderid; Type: DEFAULT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_waitingorders ALTER COLUMN waitorderid SET DEFAULT nextval('public.t_waitingorders_waitorderid_seq'::regclass);


--
-- Name: t_filetypes_x_satellites pk_filetypes_x_satellites; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_filetypes_x_satellites
    ADD CONSTRAINT pk_filetypes_x_satellites PRIMARY KEY (id);


--
-- Name: t_standingdistribution pk_standingdistribution; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_standingdistribution
    ADD CONSTRAINT pk_standingdistribution PRIMARY KEY (id);


--
-- Name: t_acshsminvrelation pk_t_acshsminvrelation; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_acshsminvrelation
    ADD CONSTRAINT pk_t_acshsminvrelation PRIMARY KEY (id);


--
-- Name: t_acsisminvrelation pk_t_acsisminvrel; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_acsisminvrelation
    ADD CONSTRAINT pk_t_acsisminvrel PRIMARY KEY (id);


--
-- Name: t_acsltainvrelation pk_t_acsltainvrelation; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_acsltainvrelation
    ADD CONSTRAINT pk_t_acsltainvrelation PRIMARY KEY (id);


--
-- Name: t_algorithm pk_t_algorithm; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_algorithm
    ADD CONSTRAINT pk_t_algorithm PRIMARY KEY (algorithm);


--
-- Name: t_anomaly_types pk_t_anomaly_types; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_anomaly_types
    ADD CONSTRAINT pk_t_anomaly_types PRIMARY KEY (id);


--
-- Name: t_applicationevents pk_t_applicationevents; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_applicationevents
    ADD CONSTRAINT pk_t_applicationevents PRIMARY KEY (id_applicationevent);


--
-- Name: t_applications pk_t_applications; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_applications
    ADD CONSTRAINT pk_t_applications PRIMARY KEY (id_application);


--
-- Name: t_applicationstatus pk_t_applicationstatus; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_applicationstatus
    ADD CONSTRAINT pk_t_applicationstatus PRIMARY KEY (application, instance);


--
-- Name: t_asyncsm_operations pk_t_asyncsm_operations; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_asyncsm_operations
    ADD CONSTRAINT pk_t_asyncsm_operations PRIMARY KEY (id_asyncsm_operation);


--
-- Name: t_bands pk_t_bands1; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_bands
    ADD CONSTRAINT pk_t_bands1 PRIMARY KEY (id);


--
-- Name: t_blockedcirculationsqueue pk_t_blockedcirculationsqueue; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_blockedcirculationsqueue
    ADD CONSTRAINT pk_t_blockedcirculationsqueue PRIMARY KEY (id);


--
-- Name: t_cartsinv pk_t_cartsinv; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_cartsinv
    ADD CONSTRAINT pk_t_cartsinv PRIMARY KEY (id);


--
-- Name: t_cartsref pk_t_cartsref; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_cartsref
    ADD CONSTRAINT pk_t_cartsref PRIMARY KEY (id);


--
-- Name: t_cdp_feederqueue pk_t_cdp_feederqueue; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_cdp_feederqueue
    ADD CONSTRAINT pk_t_cdp_feederqueue PRIMARY KEY (id);


--
-- Name: t_cdp_notification_queue pk_t_cdp_notification_queue; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_cdp_notification_queue
    ADD CONSTRAINT pk_t_cdp_notification_queue PRIMARY KEY (id);


--
-- Name: t_circ_callbacks pk_t_circ_callbacks; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circ_callbacks
    ADD CONSTRAINT pk_t_circ_callbacks PRIMARY KEY (id);


--
-- Name: t_circulation_remove_queue pk_t_circulation_remove_queue; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulation_remove_queue
    ADD CONSTRAINT pk_t_circulation_remove_queue PRIMARY KEY (id);


--
-- Name: t_circulationactivehosts pk_t_circulationactivehosts; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationactivehosts
    ADD CONSTRAINT pk_t_circulationactivehosts PRIMARY KEY (host_id);


--
-- Name: t_circulationmodes pk_t_circulationmodes; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationmodes
    ADD CONSTRAINT pk_t_circulationmodes PRIMARY KEY (id);


--
-- Name: t_circulationpolicies pk_t_circulationpolicies; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationpolicies
    ADD CONSTRAINT pk_t_circulationpolicies PRIMARY KEY (id);


--
-- Name: t_circulationpolicies_monitor pk_t_circulationpolicies_monitor; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationpolicies_monitor
    ADD CONSTRAINT pk_t_circulationpolicies_monitor PRIMARY KEY (id);


--
-- Name: t_circulationsmedia pk_t_circulationsmedia; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationsmedia
    ADD CONSTRAINT pk_t_circulationsmedia PRIMARY KEY (id);


--
-- Name: t_circulationsmedia_insmode pk_t_circulationsmedia_insmode; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationsmedia_insmode
    ADD CONSTRAINT pk_t_circulationsmedia_insmode PRIMARY KEY (id);


--
-- Name: t_circulationsqueue pk_t_circulationsqueue; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationsqueue
    ADD CONSTRAINT pk_t_circulationsqueue PRIMARY KEY (id);


--
-- Name: t_circulationsqueuedwh pk_t_circulationsqueuedwh; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationsqueuedwh
    ADD CONSTRAINT pk_t_circulationsqueuedwh PRIMARY KEY (id);


--
-- Name: t_circulationstats pk_t_circulationstats; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationstats
    ADD CONSTRAINT pk_t_circulationstats PRIMARY KEY (id);


--
-- Name: t_classifiedfiletypes pk_t_classifiedfiletypes; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_classifiedfiletypes
    ADD CONSTRAINT pk_t_classifiedfiletypes PRIMARY KEY (id);


--
-- Name: t_cleanupalgos pk_t_cleanupalgos; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_cleanupalgos
    ADD CONSTRAINT pk_t_cleanupalgos PRIMARY KEY (id_cleanupalgo);


--
-- Name: t_cleanuprules pk_t_cleanuprules; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_cleanuprules
    ADD CONSTRAINT pk_t_cleanuprules PRIMARY KEY (id_cleanuprule);


--
-- Name: t_completed_orders pk_t_completed_425; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_completed_orders
    ADD CONSTRAINT pk_t_completed_425 PRIMARY KEY (order_id);


--
-- Name: t_configurationschemas pk_t_configurationschemas; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_configurationschemas
    ADD CONSTRAINT pk_t_configurationschemas PRIMARY KEY (namespace, version);


--
-- Name: t_configurationspaces pk_t_configurationspaces; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_configurationspaces
    ADD CONSTRAINT pk_t_configurationspaces PRIMARY KEY (namespace, version);


--
-- Name: t_configurationstylesheets pk_t_configurationstylesheets; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_configurationstylesheets
    ADD CONSTRAINT pk_t_configurationstylesheets PRIMARY KEY (id);


--
-- Name: t_cov_periods pk_t_cov_periods; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_cov_periods
    ADD CONSTRAINT pk_t_cov_periods PRIMARY KEY (id_interval);


--
-- Name: t_cov_relevancies pk_t_cov_relevancies; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_cov_relevancies
    ADD CONSTRAINT pk_t_cov_relevancies PRIMARY KEY (id_relevancy);


--
-- Name: t_cov_rules pk_t_cov_rules; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_cov_rules
    ADD CONSTRAINT pk_t_cov_rules PRIMARY KEY (id_coveragerule);


--
-- Name: t_cov_x_filetypes pk_t_cov_x_filetypes; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_cov_x_filetypes
    ADD CONSTRAINT pk_t_cov_x_filetypes PRIMARY KEY (coveragerule_id, filetype_id);


--
-- Name: t_cr_x_filetypes pk_t_cr_x_filetypes; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_cr_x_filetypes
    ADD CONSTRAINT pk_t_cr_x_filetypes PRIMARY KEY (id_cr_x_filetype);


--
-- Name: t_crgroups pk_t_crgroups; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_crgroups
    ADD CONSTRAINT pk_t_crgroups PRIMARY KEY (id_crgroup);


--
-- Name: t_crhistory pk_t_crhistory; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_crhistory
    ADD CONSTRAINT pk_t_crhistory PRIMARY KEY (inventory_id, crgroup_id, tstamp);


--
-- Name: t_crrelations pk_t_crrelations; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_crrelations
    ADD CONSTRAINT pk_t_crrelations PRIMARY KEY (id_crrelation);


--
-- Name: t_crtemporaries pk_t_crtemporaries; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_crtemporaries
    ADD CONSTRAINT pk_t_crtemporaries PRIMARY KEY (inventory_id, crgroup_id, tstamp);


--
-- Name: t_datastoragequeue pk_t_datastoragequeue; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_datastoragequeue
    ADD CONSTRAINT pk_t_datastoragequeue PRIMARY KEY (id);


--
-- Name: t_datastoragetypes pk_t_datastoragetypes; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_datastoragetypes
    ADD CONSTRAINT pk_t_datastoragetypes PRIMARY KEY (id);


--
-- Name: t_dbparameters pk_t_dbparameters; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_dbparameters
    ADD CONSTRAINT pk_t_dbparameters PRIMARY KEY (id);


--
-- Name: t_dbparametertypes pk_t_dbparametertypes; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_dbparametertypes
    ADD CONSTRAINT pk_t_dbparametertypes PRIMARY KEY (id);


--
-- Name: t_discacheinvrelation pk_t_discacheinvrelation; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_discacheinvrelation
    ADD CONSTRAINT pk_t_discacheinvrelation PRIMARY KEY (id);


--
-- Name: t_discoveryplugins pk_t_discoveryplugins; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_discoveryplugins
    ADD CONSTRAINT pk_t_discoveryplugins PRIMARY KEY (id_discoveryplugin);


--
-- Name: t_disseminationpackingalgos pk_t_disseminationpackingalgos; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_disseminationpackingalgos
    ADD CONSTRAINT pk_t_disseminationpackingalgos PRIMARY KEY (id_disseminationpackingalgo);


--
-- Name: t_distpoldataset pk_t_distpoldataset; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_distpoldataset
    ADD CONSTRAINT pk_t_distpoldataset PRIMARY KEY (policy, dataset);


--
-- Name: t_distributionalgo pk_t_distributionalgo; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_distributionalgo
    ADD CONSTRAINT pk_t_distributionalgo PRIMARY KEY (id);


--
-- Name: t_distributionformat pk_t_distributionformat; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_distributionformat
    ADD CONSTRAINT pk_t_distributionformat PRIMARY KEY (format);


--
-- Name: t_distributionitems pk_t_distributionitems; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_distributionitems
    ADD CONSTRAINT pk_t_distributionitems PRIMARY KEY (id);


--
-- Name: t_distributionqueue pk_t_distributionqueue; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_distributionqueue
    ADD CONSTRAINT pk_t_distributionqueue PRIMARY KEY (packageid);


--
-- Name: t_distributionrules pk_t_distributionrules; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_distributionrules
    ADD CONSTRAINT pk_t_distributionrules PRIMARY KEY (id_distributionrule);


--
-- Name: t_distributionruleslock pk_t_distributionruleslock; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_distributionruleslock
    ADD CONSTRAINT pk_t_distributionruleslock PRIMARY KEY (packageid);


--
-- Name: t_distributionwaitingitems pk_t_distributionwaitingitems; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_distributionwaitingitems
    ADD CONSTRAINT pk_t_distributionwaitingitems PRIMARY KEY (id);


--
-- Name: t_distruleoriginator pk_t_distruleoriginator; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_distruleoriginator
    ADD CONSTRAINT pk_t_distruleoriginator PRIMARY KEY (distributionrule_id, originator_id);


--
-- Name: t_downlink_session_tracks pk_t_downlink_session_tracks; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_downlink_session_tracks
    ADD CONSTRAINT pk_t_downlink_session_tracks PRIMARY KEY (id);


--
-- Name: t_dsqueue_x_cirqueue pk_t_dsqueue_x_cirqueue; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_dsqueue_x_cirqueue
    ADD CONSTRAINT pk_t_dsqueue_x_cirqueue PRIMARY KEY (id);


--
-- Name: t_emitted_order_events pk_t_emitted_order_events; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_emitted_order_events
    ADD CONSTRAINT pk_t_emitted_order_events PRIMARY KEY (id);


--
-- Name: t_fileclasses pk_t_fileclasses; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_fileclasses
    ADD CONSTRAINT pk_t_fileclasses PRIMARY KEY (id_fileclass);


--
-- Name: t_filetypes pk_t_filetypes; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_filetypes
    ADD CONSTRAINT pk_t_filetypes PRIMARY KEY (id_filetype);


--
-- Name: t_filetypes_last_inv pk_t_filetypes_last_inv; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_filetypes_last_inv
    ADD CONSTRAINT pk_t_filetypes_last_inv PRIMARY KEY (id);


--
-- Name: t_filetypes_x_migrations pk_t_filetypes_x_migrations; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_filetypes_x_migrations
    ADD CONSTRAINT pk_t_filetypes_x_migrations PRIMARY KEY (id_filetypemigration);


--
-- Name: t_filetypes_x_stylesheets pk_t_filetypes_x_stylesheets; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_filetypes_x_stylesheets
    ADD CONSTRAINT pk_t_filetypes_x_stylesheets PRIMARY KEY (id);


--
-- Name: t_filetypescomps pk_t_filetypescomps; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_filetypescomps
    ADD CONSTRAINT pk_t_filetypescomps PRIMARY KEY (id);


--
-- Name: t_flowtags pk_t_flowtags; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_flowtags
    ADD CONSTRAINT pk_t_flowtags PRIMARY KEY (id);


--
-- Name: t_inv_additional_params pk_t_inv_additional_params; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inv_additional_params
    ADD CONSTRAINT pk_t_inv_additional_params PRIMARY KEY (id);


--
-- Name: t_inv_baselines pk_t_inv_baselines; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inv_baselines
    ADD CONSTRAINT pk_t_inv_baselines PRIMARY KEY (id_inv_baseline);


--
-- Name: t_inv_client pk_t_inv_client; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inv_client
    ADD CONSTRAINT pk_t_inv_client PRIMARY KEY (id_inv_client);


--
-- Name: t_inv_data_restored pk_t_inv_data_restored; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inv_data_restored
    ADD CONSTRAINT pk_t_inv_data_restored PRIMARY KEY (id);


--
-- Name: t_inv_distributions pk_t_inv_distributions; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inv_distributions
    ADD CONSTRAINT pk_t_inv_distributions PRIMARY KEY (inv_id);


--
-- Name: t_inv_downloads pk_t_inv_downloads; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inv_downloads
    ADD CONSTRAINT pk_t_inv_downloads PRIMARY KEY (inv_id, client_id);


--
-- Name: t_inv_relation_types pk_t_inv_relation_types; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inv_relation_types
    ADD CONSTRAINT pk_t_inv_relation_types PRIMARY KEY (id);


--
-- Name: t_inv_relations pk_t_inv_relations; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inv_relations
    ADD CONSTRAINT pk_t_inv_relations PRIMARY KEY (id);


--
-- Name: t_inv_x_baselines pk_t_inv_x_baselines; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inv_x_baselines
    ADD CONSTRAINT pk_t_inv_x_baselines PRIMARY KEY (id_inv_x_baseline);


--
-- Name: t_inventory pk_t_inventory; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventory
    ADD CONSTRAINT pk_t_inventory PRIMARY KEY (id_inv);


--
-- Name: t_inventoryanomalies pk_t_inventorya266; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventoryanomalies
    ADD CONSTRAINT pk_t_inventorya266 PRIMARY KEY (id);


--
-- Name: t_inventorycomments pk_t_inventorycomments; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventorycomments
    ADD CONSTRAINT pk_t_inventorycomments PRIMARY KEY (id_inventorycomment);


--
-- Name: t_inventorydataset pk_t_inventorydataset; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventorydataset
    ADD CONSTRAINT pk_t_inventorydataset PRIMARY KEY (filename, fileversion, measdatasetid);


--
-- Name: t_inventoryeventsconf pk_t_inventorye258; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventoryeventsconf
    ADD CONSTRAINT pk_t_inventorye258 PRIMARY KEY (id);


--
-- Name: t_inventoryevents pk_t_inventoryevents; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventoryevents
    ADD CONSTRAINT pk_t_inventoryevents PRIMARY KEY (id);


--
-- Name: t_inventoryextra pk_t_inventoryextra; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventoryextra
    ADD CONSTRAINT pk_t_inventoryextra PRIMARY KEY (inv_id);


--
-- Name: t_inventorygaps pk_t_inventorygaps; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventorygaps
    ADD CONSTRAINT pk_t_inventorygaps PRIMARY KEY (inventorygapid);


--
-- Name: t_inventorylinks pk_t_inventorylinks; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventorylinks
    ADD CONSTRAINT pk_t_inventorylinks PRIMARY KEY (id);


--
-- Name: t_inventorylinksconf pk_t_inventorylinksconf; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventorylinksconf
    ADD CONSTRAINT pk_t_inventorylinksconf PRIMARY KEY (id);


--
-- Name: t_inventorysm pk_t_inventorysm; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventorysm
    ADD CONSTRAINT pk_t_inventorysm PRIMARY KEY (id);


--
-- Name: t_invrollingpolicies pk_t_invrollingpolicies; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_invrollingpolicies
    ADD CONSTRAINT pk_t_invrollingpolicies PRIMARY KEY (id);


--
-- Name: t_invsiterelation pk_t_invsiterelation; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_invsiterelation
    ADD CONSTRAINT pk_t_invsiterelation PRIMARY KEY (id);


--
-- Name: t_invspecialrollingpolicies pk_t_invspecialrollingpolicies; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_invspecialrollingpolicies
    ADD CONSTRAINT pk_t_invspecialrollingpolicies PRIMARY KEY (id);


--
-- Name: t_knownhosts pk_t_knownhosts; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_knownhosts
    ADD CONSTRAINT pk_t_knownhosts PRIMARY KEY (id);


--
-- Name: t_knownhosts_limits pk_t_knownhosts_limits; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_knownhosts_limits
    ADD CONSTRAINT pk_t_knownhosts_limits PRIMARY KEY (id);


--
-- Name: t_lastacknowledge pk_t_lastacknowledge; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_lastacknowledge
    ADD CONSTRAINT pk_t_lastacknowledge PRIMARY KEY (id);


--
-- Name: t_localtransfermodes pk_t_localtransfermodes; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_localtransfermodes
    ADD CONSTRAINT pk_t_localtransfermodes PRIMARY KEY (id);


--
-- Name: t_measdataset pk_t_measdataset; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_measdataset
    ADD CONSTRAINT pk_t_measdataset PRIMARY KEY (id);


--
-- Name: t_mediacapformat pk_t_mediacapformat; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mediacapformat
    ADD CONSTRAINT pk_t_mediacapformat PRIMARY KEY (mediatype, format);


--
-- Name: t_mediatypes pk_t_mediatypes; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mediatypes
    ADD CONSTRAINT pk_t_mediatypes PRIMARY KEY (id_mediatype);


--
-- Name: t_migrationalgos pk_t_migrationalgos; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_migrationalgos
    ADD CONSTRAINT pk_t_migrationalgos PRIMARY KEY (id_migrationalgo);


--
-- Name: t_mpm_cross pk_t_mpm_cross; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mpm_cross
    ADD CONSTRAINT pk_t_mpm_cross PRIMARY KEY (id_cross);


--
-- Name: t_mpm_dip pk_t_mpm_dip; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mpm_dip
    ADD CONSTRAINT pk_t_mpm_dip PRIMARY KEY (id_dip);


--
-- Name: t_mpm_ipf_adf_check pk_t_mpm_ipf_adf_check; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mpm_ipf_adf_check
    ADD CONSTRAINT pk_t_mpm_ipf_adf_check PRIMARY KEY (id);


--
-- Name: t_mpm_parameter_types pk_t_mpm_parameter_types; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mpm_parameter_types
    ADD CONSTRAINT pk_t_mpm_parameter_types PRIMARY KEY (id_paramtype);


--
-- Name: t_mpm_parameters pk_t_mpm_parameters; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mpm_parameters
    ADD CONSTRAINT pk_t_mpm_parameters PRIMARY KEY (id_parameter);


--
-- Name: t_mpm_prb pk_t_mpm_prb; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mpm_prb
    ADD CONSTRAINT pk_t_mpm_prb PRIMARY KEY (id_prb);


--
-- Name: t_mpm_products pk_t_mpm_products; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mpm_products
    ADD CONSTRAINT pk_t_mpm_products PRIMARY KEY (id_product);


--
-- Name: t_mpm_routines pk_t_mpm_routines; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mpm_routines
    ADD CONSTRAINT pk_t_mpm_routines PRIMARY KEY (id_routine);


--
-- Name: t_mpm_statuses pk_t_mpm_statuses; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mpm_statuses
    ADD CONSTRAINT pk_t_mpm_statuses PRIMARY KEY (id_status);


--
-- Name: t_mpm_timeliness pk_t_mpm_timeliness; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mpm_timeliness
    ADD CONSTRAINT pk_t_mpm_timeliness PRIMARY KEY (id_timeliness);


--
-- Name: t_mpm_values_part1 pk_t_mpm_values_part1; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mpm_values_part1
    ADD CONSTRAINT pk_t_mpm_values_part1 PRIMARY KEY (id_value);


--
-- Name: t_mpm_values_part2 pk_t_mpm_values_part2; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mpm_values_part2
    ADD CONSTRAINT pk_t_mpm_values_part2 PRIMARY KEY (id_value);


--
-- Name: t_objsminvrelation pk_t_objsminvrelation; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_objsminvrelation
    ADD CONSTRAINT pk_t_objsminvrelation PRIMARY KEY (id);


--
-- Name: t_ondemanddistribution pk_t_ondemanddistribution; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_ondemanddistribution
    ADD CONSTRAINT pk_t_ondemanddistribution PRIMARY KEY (orderid, userid);


--
-- Name: t_operation_requests pk_t_operation_requests; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_operation_requests
    ADD CONSTRAINT pk_t_operation_requests PRIMARY KEY (id);


--
-- Name: t_operationtypes pk_t_operationtypes; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_operationtypes
    ADD CONSTRAINT pk_t_operationtypes PRIMARY KEY (id_operationtype);


--
-- Name: t_ordersattachments pk_t_ordersattachments; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_ordersattachments
    ADD CONSTRAINT pk_t_ordersattachments PRIMARY KEY (id);


--
-- Name: t_ordersattachmenttypes pk_t_ordersattachmenttypes; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_ordersattachmenttypes
    ADD CONSTRAINT pk_t_ordersattachmenttypes PRIMARY KEY (id);


--
-- Name: t_ordersgeneration pk_t_ordersgeneration; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_ordersgeneration
    ADD CONSTRAINT pk_t_ordersgeneration PRIMARY KEY (id);


--
-- Name: t_ordersinput pk_t_ordersinput; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_ordersinput
    ADD CONSTRAINT pk_t_ordersinput PRIMARY KEY (id);


--
-- Name: t_ordersqueue pk_t_ordersqueue; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_ordersqueue
    ADD CONSTRAINT pk_t_ordersqueue PRIMARY KEY (orderid);


--
-- Name: t_ordersqueueblacklist pk_t_ordersqueueblacklist; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_ordersqueueblacklist
    ADD CONSTRAINT pk_t_ordersqueueblacklist PRIMARY KEY (id);


--
-- Name: t_orderstatetransitions pk_t_orderstatetransitions; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_orderstatetransitions
    ADD CONSTRAINT pk_t_orderstatetransitions PRIMARY KEY (id);


--
-- Name: t_ordertypes pk_t_ordertypes; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_ordertypes
    ADD CONSTRAINT pk_t_ordertypes PRIMARY KEY (ordertype);


--
-- Name: t_orderwaittrigger pk_t_orderwaittrigger; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_orderwaittrigger
    ADD CONSTRAINT pk_t_orderwaittrigger PRIMARY KEY (triggertype);


--
-- Name: t_ordtriggeringfiles pk_t_ordtriggeringfiles; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_ordtriggeringfiles
    ADD CONSTRAINT pk_t_ordtriggeringfiles PRIMARY KEY (id);


--
-- Name: t_originators pk_t_originators; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_originators
    ADD CONSTRAINT pk_t_originators PRIMARY KEY (id);


--
-- Name: t_pdsmodes pk_t_pdsmodes; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_pdsmodes
    ADD CONSTRAINT pk_t_pdsmodes PRIMARY KEY (id);


--
-- Name: t_pendinginventory pk_t_pendinginventory; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_pendinginventory
    ADD CONSTRAINT pk_t_pendinginventory PRIMARY KEY (filename, fileversion);


--
-- Name: t_postinvqueue pk_t_postinvqueue; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_postinvqueue
    ADD CONSTRAINT pk_t_postinvqueue PRIMARY KEY (id);


--
-- Name: t_proc_ftype_generation pk_t_proc_ftype_generation; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_proc_ftype_generation
    ADD CONSTRAINT pk_t_proc_ftype_generation PRIMARY KEY (id);


--
-- Name: t_processors pk_t_processors; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_processors
    ADD CONSTRAINT pk_t_processors PRIMARY KEY (id_processor);


--
-- Name: t_qualifiedflag pk_t_qualifiedflag; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_qualifiedflag
    ADD CONSTRAINT pk_t_qualifiedflag PRIMARY KEY (id);


--
-- Name: t_receptionrules pk_t_receptionrules; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_receptionrules
    ADD CONSTRAINT pk_t_receptionrules PRIMARY KEY (id_receptionrule);


--
-- Name: t_receptionruleshist pk_t_receptionruleshist; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_receptionruleshist
    ADD CONSTRAINT pk_t_receptionruleshist PRIMARY KEY (id_receptionrulehist);


--
-- Name: t_receptionruleslock pk_t_receptionruleslock; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_receptionruleslock
    ADD CONSTRAINT pk_t_receptionruleslock PRIMARY KEY (receptionrule_id);


--
-- Name: t_repositories pk_t_repositories; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_repositories
    ADD CONSTRAINT pk_t_repositories PRIMARY KEY (id_repository);


--
-- Name: t_rollingpoliciesalgo pk_t_rollingpoliciesalgo; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_rollingpoliciesalgo
    ADD CONSTRAINT pk_t_rollingpoliciesalgo PRIMARY KEY (id);


--
-- Name: t_rollingpolicyactions pk_t_rollingpolicyactions; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_rollingpolicyactions
    ADD CONSTRAINT pk_t_rollingpolicyactions PRIMARY KEY (id);


--
-- Name: t_satellites pk_t_satellites; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_satellites
    ADD CONSTRAINT pk_t_satellites PRIMARY KEY (satelliteid);


--
-- Name: t_satrelations pk_t_satrelations; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_satrelations
    ADD CONSTRAINT pk_t_satrelations PRIMARY KEY (satelliteid, mission, sensorid);


--
-- Name: t_schema pk_t_schema; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_schema
    ADD CONSTRAINT pk_t_schema PRIMARY KEY (id);


--
-- Name: t_schemamodificationtypes pk_t_schemamodificationtype; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_schemamodificationtypes
    ADD CONSTRAINT pk_t_schemamodificationtype PRIMARY KEY (id);


--
-- Name: t_schematypes pk_t_schematypes; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_schematypes
    ADD CONSTRAINT pk_t_schematypes PRIMARY KEY (id);


--
-- Name: t_securityzonerelays pk_t_securityzonerelays; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_securityzonerelays
    ADD CONSTRAINT pk_t_securityzonerelays PRIMARY KEY (id);


--
-- Name: t_securityzones pk_t_securityzones; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_securityzones
    ADD CONSTRAINT pk_t_securityzones PRIMARY KEY (id);


--
-- Name: t_sensors pk_t_sensors; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_sensors
    ADD CONSTRAINT pk_t_sensors PRIMARY KEY (sensorid);


--
-- Name: t_serials pk_t_serials; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_serials
    ADD CONSTRAINT pk_t_serials PRIMARY KEY (table_name, field_name);


--
-- Name: t_sites pk_t_sites; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_sites
    ADD CONSTRAINT pk_t_sites PRIMARY KEY (id);


--
-- Name: t_sitetype pk_t_sitetype; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_sitetype
    ADD CONSTRAINT pk_t_sitetype PRIMARY KEY (id);


--
-- Name: t_sm_sync_missingactions pk_t_sm_sync_missingactions; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_sm_sync_missingactions
    ADD CONSTRAINT pk_t_sm_sync_missingactions PRIMARY KEY (id);


--
-- Name: t_smactions pk_t_smactions1; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_smactions
    ADD CONSTRAINT pk_t_smactions1 PRIMARY KEY (id);


--
-- Name: t_smqueue pk_t_smqueue1; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_smqueue
    ADD CONSTRAINT pk_t_smqueue1 PRIMARY KEY (packageid);


--
-- Name: t_smtype pk_t_smtype; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_smtype
    ADD CONSTRAINT pk_t_smtype PRIMARY KEY (id);


--
-- Name: t_sor_requests pk_t_sor_requests; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_sor_requests
    ADD CONSTRAINT pk_t_sor_requests PRIMARY KEY (id_sor_request);


--
-- Name: t_stafinvrelation pk_t_stafinvrelation; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_stafinvrelation
    ADD CONSTRAINT pk_t_stafinvrelation PRIMARY KEY (id);


--
-- Name: t_statetransitions_allowed pk_t_statetransitions_allowed; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_statetransitions_allowed
    ADD CONSTRAINT pk_t_statetransitions_allowed PRIMARY KEY (id);


--
-- Name: t_statetransitions_flows pk_t_statetransitions_flows; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_statetransitions_flows
    ADD CONSTRAINT pk_t_statetransitions_flows PRIMARY KEY (id);


--
-- Name: t_statetransitions_tables pk_t_statetransitions_tables; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_statetransitions_tables
    ADD CONSTRAINT pk_t_statetransitions_tables PRIMARY KEY (id);


--
-- Name: t_stati pk_t_stati; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_stati
    ADD CONSTRAINT pk_t_stati PRIMARY KEY (id);


--
-- Name: t_stations pk_t_stations; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_stations
    ADD CONSTRAINT pk_t_stations PRIMARY KEY (stationid);


--
-- Name: t_storagegroup pk_t_storagegroup; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_storagegroup
    ADD CONSTRAINT pk_t_storagegroup PRIMARY KEY (id);


--
-- Name: t_storagemanager pk_t_storagemanager; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_storagemanager
    ADD CONSTRAINT pk_t_storagemanager PRIMARY KEY (id);


--
-- Name: t_storagepriority pk_t_storagepriority; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_storagepriority
    ADD CONSTRAINT pk_t_storagepriority PRIMARY KEY (id, filetype);


--
-- Name: t_storagerollingthresholds pk_t_storagerollingthresholds; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_storagerollingthresholds
    ADD CONSTRAINT pk_t_storagerollingthresholds PRIMARY KEY (storagemanager_id);


--
-- Name: t_streamers pk_t_streamers; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_streamers
    ADD CONSTRAINT pk_t_streamers PRIMARY KEY (id);


--
-- Name: t_stylesheetoutputtypes pk_t_stylesheetoutputtypes; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_stylesheetoutputtypes
    ADD CONSTRAINT pk_t_stylesheetoutputtypes PRIMARY KEY (id);


--
-- Name: t_tasktables pk_t_tasktables; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_tasktables
    ADD CONSTRAINT pk_t_tasktables PRIMARY KEY (id_tasktable);


--
-- Name: t_temporarydistribution pk_t_temporarydistribution; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_temporarydistribution
    ADD CONSTRAINT pk_t_temporarydistribution PRIMARY KEY (id);


--
-- Name: t_temporaryorders pk_t_temporaryorders; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_temporaryorders
    ADD CONSTRAINT pk_t_temporaryorders PRIMARY KEY (id);


--
-- Name: t_tmpnamestrategy pk_t_tmpnamestrategy; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_tmpnamestrategy
    ADD CONSTRAINT pk_t_tmpnamestrategy PRIMARY KEY (id_tmpnamestrategy);


--
-- Name: t_transferprotocols pk_t_transferprotocols; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_transferprotocols
    ADD CONSTRAINT pk_t_transferprotocols PRIMARY KEY (id);


--
-- Name: t_usdfwa pk_t_usdfwa; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_usdfwa
    ADD CONSTRAINT pk_t_usdfwa PRIMARY KEY (id_usdfwa);


--
-- Name: t_usdfwa_delivery_details pk_t_usdfwa_dd; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_usdfwa_delivery_details
    ADD CONSTRAINT pk_t_usdfwa_dd PRIMARY KEY (id_delivery_details);


--
-- Name: t_usermodes pk_t_usermodes; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_usermodes
    ADD CONSTRAINT pk_t_usermodes PRIMARY KEY (id);


--
-- Name: t_users pk_t_users; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_users
    ADD CONSTRAINT pk_t_users PRIMARY KEY (id_user);


--
-- Name: t_usertypes pk_t_usertypes; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_usertypes
    ADD CONSTRAINT pk_t_usertypes PRIMARY KEY (usertype);


--
-- Name: t_waitingorders pk_t_waitingorders; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_waitingorders
    ADD CONSTRAINT pk_t_waitingorders PRIMARY KEY (waitorderid);


--
-- Name: t_cartsinv q_cartsubreq; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_cartsinv
    ADD CONSTRAINT q_cartsubreq UNIQUE (cart, subreq);


--
-- Name: t_anomaly_types uq_anomaly_types01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_anomaly_types
    ADD CONSTRAINT uq_anomaly_types01 UNIQUE (anomaly_type);


--
-- Name: t_applicationevents uq_applicationevents01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_applicationevents
    ADD CONSTRAINT uq_applicationevents01 UNIQUE (application_id, tstamp);


--
-- Name: t_applications uq_applications01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_applications
    ADD CONSTRAINT uq_applications01 UNIQUE (app_name);


--
-- Name: t_cartsref uq_cartsref_extref; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_cartsref
    ADD CONSTRAINT uq_cartsref_extref UNIQUE (extref);


--
-- Name: t_circulationmodes uq_circulationmodes01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationmodes
    ADD CONSTRAINT uq_circulationmodes01 UNIQUE (circulationmode);


--
-- Name: t_circulationpolicies uq_circulationpolicies01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationpolicies
    ADD CONSTRAINT uq_circulationpolicies01 UNIQUE (filetype_id, circulator_node_id, source_id, destination_id);


--
-- Name: t_circulationsmedia uq_circulationsmedia01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationsmedia
    ADD CONSTRAINT uq_circulationsmedia01 UNIQUE (media_tag, source);


--
-- Name: t_circulationsmedia_insmode uq_circulationsmedia_insmode01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationsmedia_insmode
    ADD CONSTRAINT uq_circulationsmedia_insmode01 UNIQUE (mode);


--
-- Name: t_cleanupalgos uq_cleanupalgos01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_cleanupalgos
    ADD CONSTRAINT uq_cleanupalgos01 UNIQUE (name);


--
-- Name: t_configurationstylesheets uq_configurationstylesheets01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_configurationstylesheets
    ADD CONSTRAINT uq_configurationstylesheets01 UNIQUE (name, version);


--
-- Name: t_cr_x_filetypes uq_cr_x_filetypes01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_cr_x_filetypes
    ADD CONSTRAINT uq_cr_x_filetypes01 UNIQUE (crgroup_id, filetype_id);


--
-- Name: t_crgroups uq_crgroups01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_crgroups
    ADD CONSTRAINT uq_crgroups01 UNIQUE (name);


--
-- Name: t_crrelations uq_crrelations01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_crrelations
    ADD CONSTRAINT uq_crrelations01 UNIQUE (crgroup_id, repository_id, distributionrule_id);


--
-- Name: t_datastoragetypes uq_datastoragetypes01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_datastoragetypes
    ADD CONSTRAINT uq_datastoragetypes01 UNIQUE (datastoragetype);


--
-- Name: t_dbparameters uq_dbparameters01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_dbparameters
    ADD CONSTRAINT uq_dbparameters01 UNIQUE (parameter);


--
-- Name: t_dbparametertypes uq_dbparametertypes01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_dbparametertypes
    ADD CONSTRAINT uq_dbparametertypes01 UNIQUE (parametertype);


--
-- Name: t_dbstatus uq_dbstatus1; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_dbstatus
    ADD CONSTRAINT uq_dbstatus1 UNIQUE (master);


--
-- Name: t_discoveryplugins uq_discoveryplugins01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_discoveryplugins
    ADD CONSTRAINT uq_discoveryplugins01 UNIQUE (name);


--
-- Name: t_disseminationpackingalgos uq_disseminationpackingalgos01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_disseminationpackingalgos
    ADD CONSTRAINT uq_disseminationpackingalgos01 UNIQUE (factorykey);


--
-- Name: t_distributionalgo uq_distalgo_name; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_distributionalgo
    ADD CONSTRAINT uq_distalgo_name UNIQUE (name);


--
-- Name: t_downlink_session_tracks uq_downlink_session_tracks01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_downlink_session_tracks
    ADD CONSTRAINT uq_downlink_session_tracks01 UNIQUE (session_id);


--
-- Name: t_dsqueue_x_cirqueue uq_dsqueuexcirqueue01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_dsqueue_x_cirqueue
    ADD CONSTRAINT uq_dsqueuexcirqueue01 UNIQUE (datastoragequeue_id, circulation_id);


--
-- Name: t_emitted_order_events uq_emitted_order_events01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_emitted_order_events
    ADD CONSTRAINT uq_emitted_order_events01 UNIQUE (event_time, event_tag, time_driven_start, time_driven_stop);


--
-- Name: t_fileclasses uq_fileclasses01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_fileclasses
    ADD CONSTRAINT uq_fileclasses01 UNIQUE (fileclass);


--
-- Name: t_filetypes uq_filetypes01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_filetypes
    ADD CONSTRAINT uq_filetypes01 UNIQUE (filetype);


--
-- Name: t_filetypes_last_inv uq_filetypes_last_inv01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_filetypes_last_inv
    ADD CONSTRAINT uq_filetypes_last_inv01 UNIQUE (filetype);


--
-- Name: t_filetypes_x_stylesheets uq_filetypes_x_stylesheets01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_filetypes_x_stylesheets
    ADD CONSTRAINT uq_filetypes_x_stylesheets01 UNIQUE (filetype_id, outputtype_id, stylesheet_id);


--
-- Name: t_filetypescomps uq_filetypescomps01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_filetypescomps
    ADD CONSTRAINT uq_filetypescomps01 UNIQUE (pfiletype, cfiletype);


--
-- Name: t_filetypes_x_migrations uq_filetypesxmigrations01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_filetypes_x_migrations
    ADD CONSTRAINT uq_filetypesxmigrations01 UNIQUE (sm_id, filetype_id);


--
-- Name: t_inv_client uq_inv_client01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inv_client
    ADD CONSTRAINT uq_inv_client01 UNIQUE (name);


--
-- Name: t_inv_relation_types uq_inv_relation_types01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inv_relation_types
    ADD CONSTRAINT uq_inv_relation_types01 UNIQUE (relation_type);


--
-- Name: t_inv_relations uq_inv_relations01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inv_relations
    ADD CONSTRAINT uq_inv_relations01 UNIQUE (inv_id1, inv_id2);


--
-- Name: t_inv_x_baselines uq_inv_x_baselines01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inv_x_baselines
    ADD CONSTRAINT uq_inv_x_baselines01 UNIQUE (inv_id, inv_baseline_id);


--
-- Name: t_inventory uq_inventory01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventory
    ADD CONSTRAINT uq_inventory01 UNIQUE (filename, fileversion);


--
-- Name: t_inventory uq_inventory02; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventory
    ADD CONSTRAINT uq_inventory02 UNIQUE (unique_id);


--
-- Name: t_inv_baselines uq_inventorybaselines01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inv_baselines
    ADD CONSTRAINT uq_inventorybaselines01 UNIQUE (baseline);


--
-- Name: t_inventorysm uq_inventorysm01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventorysm
    ADD CONSTRAINT uq_inventorysm01 UNIQUE (inv_id, storagemanid);


--
-- Name: t_inventoryeventsconf uq_inveventsconf; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventoryeventsconf
    ADD CONSTRAINT uq_inveventsconf UNIQUE (filetype);


--
-- Name: t_invsiterelation uq_invsiterelation; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_invsiterelation
    ADD CONSTRAINT uq_invsiterelation UNIQUE (filename, fileversion, site);


--
-- Name: t_invspecialrollingpolicies uq_invspecialrollingpolicies; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_invspecialrollingpolicies
    ADD CONSTRAINT uq_invspecialrollingpolicies UNIQUE (inventoryname, inventoryversion, action, storageid);


--
-- Name: t_knownhosts uq_knownhosts01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_knownhosts
    ADD CONSTRAINT uq_knownhosts01 UNIQUE (host_name);


--
-- Name: t_knownhosts_limits uq_knownhosts_limits01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_knownhosts_limits
    ADD CONSTRAINT uq_knownhosts_limits01 UNIQUE (circulator_node_id, dest_host_id);


--
-- Name: t_localtransfermodes uq_localtransfermodes01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_localtransfermodes
    ADD CONSTRAINT uq_localtransfermodes01 UNIQUE (localtransfermode);


--
-- Name: t_measdataset uq_measdata_name; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_measdataset
    ADD CONSTRAINT uq_measdata_name UNIQUE (name);


--
-- Name: t_mediatypes uq_mediatypes01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mediatypes
    ADD CONSTRAINT uq_mediatypes01 UNIQUE (mediatype);


--
-- Name: t_migrationalgos uq_migrationalgos01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_migrationalgos
    ADD CONSTRAINT uq_migrationalgos01 UNIQUE (name);


--
-- Name: t_mpm_cross uq_mpm_cross01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mpm_cross
    ADD CONSTRAINT uq_mpm_cross01 UNIQUE (value_id, parameter_id, routine_id, product_id);


--
-- Name: t_mpm_statuses uq_mpm_dip_statuses01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mpm_statuses
    ADD CONSTRAINT uq_mpm_dip_statuses01 UNIQUE (status);


--
-- Name: t_mpm_parameter_types uq_mpm_parameter_types01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mpm_parameter_types
    ADD CONSTRAINT uq_mpm_parameter_types01 UNIQUE (type_name);


--
-- Name: t_mpm_parameters uq_mpm_parameters01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mpm_parameters
    ADD CONSTRAINT uq_mpm_parameters01 UNIQUE (parameter_name, tableddname);


--
-- Name: t_mpm_products uq_mpm_products01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mpm_products
    ADD CONSTRAINT uq_mpm_products01 UNIQUE (productname, productversion);


--
-- Name: t_mpm_routines uq_mpm_routines01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mpm_routines
    ADD CONSTRAINT uq_mpm_routines01 UNIQUE (routine_name);


--
-- Name: t_mpm_timeliness uq_mpm_timeliness01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mpm_timeliness
    ADD CONSTRAINT uq_mpm_timeliness01 UNIQUE (timeliness_code);


--
-- Name: t_operationtypes uq_operationtypes01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_operationtypes
    ADD CONSTRAINT uq_operationtypes01 UNIQUE (type);


--
-- Name: t_ordersattachmenttypes uq_ordersattachmenttypes; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_ordersattachmenttypes
    ADD CONSTRAINT uq_ordersattachmenttypes UNIQUE (name);


--
-- Name: t_ordersqueueblacklist uq_ordersqueueblacklist01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_ordersqueueblacklist
    ADD CONSTRAINT uq_ordersqueueblacklist01 UNIQUE (orderid, jobresponsible);


--
-- Name: t_originators uq_originators; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_originators
    ADD CONSTRAINT uq_originators UNIQUE (name);


--
-- Name: t_pdsmodes uq_pdsmodes_appsub; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_pdsmodes
    ADD CONSTRAINT uq_pdsmodes_appsub UNIQUE (application, subsystem);


--
-- Name: t_proc_ftype_generation uq_proc_ftype_generation01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_proc_ftype_generation
    ADD CONSTRAINT uq_proc_ftype_generation01 UNIQUE (proc_id, ftype_id);


--
-- Name: t_processors uq_processors01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_processors
    ADD CONSTRAINT uq_processors01 UNIQUE (processorname, processorversion);


--
-- Name: t_qualifiedflag uq_qualifiedflag; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_qualifiedflag
    ADD CONSTRAINT uq_qualifiedflag UNIQUE (name);


--
-- Name: t_receptionruleshist uq_receptionruleshist01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_receptionruleshist
    ADD CONSTRAINT uq_receptionruleshist01 UNIQUE (receptionrule_id, filename);


--
-- Name: t_repositories uq_repositories01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_repositories
    ADD CONSTRAINT uq_repositories01 UNIQUE (symbolicname);


--
-- Name: t_repositories uq_repositories02; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_repositories
    ADD CONSTRAINT uq_repositories02 UNIQUE (transferprotocol_id, host_id, username, remotepath);


--
-- Name: t_rollingpoliciesalgo uq_rollingpoliciesalgo; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_rollingpoliciesalgo
    ADD CONSTRAINT uq_rollingpoliciesalgo UNIQUE (name);


--
-- Name: t_rollingpolicyactions uq_rollingpolicyact; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_rollingpolicyactions
    ADD CONSTRAINT uq_rollingpolicyact UNIQUE (action);


--
-- Name: t_schematypes uq_schematypes01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_schematypes
    ADD CONSTRAINT uq_schematypes01 UNIQUE (schematype);


--
-- Name: t_securityzonerelays uq_securityzonerelays01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_securityzonerelays
    ADD CONSTRAINT uq_securityzonerelays01 UNIQUE (host_id, seczone_id);


--
-- Name: t_securityzones uq_securityzones01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_securityzones
    ADD CONSTRAINT uq_securityzones01 UNIQUE (securityzone);


--
-- Name: t_sites uq_sites_name; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_sites
    ADD CONSTRAINT uq_sites_name UNIQUE (name);


--
-- Name: t_sitetype uq_sitetype_name; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_sitetype
    ADD CONSTRAINT uq_sitetype_name UNIQUE (name);


--
-- Name: t_sm_sync_missingactions uq_sm_sync_missingactions1; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_sm_sync_missingactions
    ADD CONSTRAINT uq_sm_sync_missingactions1 UNIQUE (operation, sm_id, inv_id);


--
-- Name: t_sm_sync_missingactions uq_sm_sync_missingactions2; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_sm_sync_missingactions
    ADD CONSTRAINT uq_sm_sync_missingactions2 UNIQUE (operation, sm_id, invsm_id);


--
-- Name: t_smactions uq_smactions1; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_smactions
    ADD CONSTRAINT uq_smactions1 UNIQUE (filename, fileversion, smid, operation, enddate);


--
-- Name: t_smtype uq_smtype1; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_smtype
    ADD CONSTRAINT uq_smtype1 UNIQUE (type);


--
-- Name: t_sor_requests uq_sor_requests01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_sor_requests
    ADD CONSTRAINT uq_sor_requests01 UNIQUE (requestid);


--
-- Name: t_statetransitions_allowed uq_statetransitions_allowed01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_statetransitions_allowed
    ADD CONSTRAINT uq_statetransitions_allowed01 UNIQUE (table_id, prev_state_id, post_state_id);


--
-- Name: t_statetransitions_flows uq_statetransitions_flows01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_statetransitions_flows
    ADD CONSTRAINT uq_statetransitions_flows01 UNIQUE (flow_type);


--
-- Name: t_statetransitions_tables uq_statetransitions_tables01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_statetransitions_tables
    ADD CONSTRAINT uq_statetransitions_tables01 UNIQUE (table_name, flow_id);


--
-- Name: t_stati uq_stati01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_stati
    ADD CONSTRAINT uq_stati01 UNIQUE (status);


--
-- Name: t_standingdistribution uq_stendingdistribution01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_standingdistribution
    ADD CONSTRAINT uq_stendingdistribution01 UNIQUE (distpoliciesid, validitystart, validitystop, cartsid, subrequest);


--
-- Name: t_storagegroup uq_storagegroup01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_storagegroup
    ADD CONSTRAINT uq_storagegroup01 UNIQUE (filetype, storagemanager);


--
-- Name: t_storagemanager uq_storagemanager; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_storagemanager
    ADD CONSTRAINT uq_storagemanager UNIQUE (name);


--
-- Name: t_streamers uq_streamers01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_streamers
    ADD CONSTRAINT uq_streamers01 UNIQUE (streamtype);


--
-- Name: t_stylesheetoutputtypes uq_stylesheetoutputtypes01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_stylesheetoutputtypes
    ADD CONSTRAINT uq_stylesheetoutputtypes01 UNIQUE (outputtype);


--
-- Name: t_bands uq_t_bands; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_bands
    ADD CONSTRAINT uq_t_bands UNIQUE (name);


--
-- Name: t_circ_callbacks uq_t_circ_callbacks01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circ_callbacks
    ADD CONSTRAINT uq_t_circ_callbacks01 UNIQUE (circ_queue_id);


--
-- Name: t_circulationpolicies_monitor uq_t_circulationpolicies_monitor01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationpolicies_monitor
    ADD CONSTRAINT uq_t_circulationpolicies_monitor01 UNIQUE (circulationpolicy_id);


--
-- Name: t_filetypes_x_satellites uq_t_filetypesxsatellites01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_filetypes_x_satellites
    ADD CONSTRAINT uq_t_filetypesxsatellites01 UNIQUE (filetype, satellite_id);


--
-- Name: t_flowtags uq_t_flowtags; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_flowtags
    ADD CONSTRAINT uq_t_flowtags UNIQUE (flowtagname);


--
-- Name: t_inventorylinksconf uq_t_invlinksconf; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventorylinksconf
    ADD CONSTRAINT uq_t_invlinksconf UNIQUE (orig_filetype, dest_filetype, algotype);


--
-- Name: t_satellites uq_t_satellites1; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_satellites
    ADD CONSTRAINT uq_t_satellites1 UNIQUE (satellitename);


--
-- Name: t_satellites uq_t_satellites2; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_satellites
    ADD CONSTRAINT uq_t_satellites2 UNIQUE (satelliteacronym);


--
-- Name: t_schema uq_t_schema_version01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_schema
    ADD CONSTRAINT uq_t_schema_version01 UNIQUE (version);


--
-- Name: t_schemamodificationtypes uq_t_schemamodificationtypes01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_schemamodificationtypes
    ADD CONSTRAINT uq_t_schemamodificationtypes01 UNIQUE (modificationtype);


--
-- Name: t_sensors uq_t_sensors1; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_sensors
    ADD CONSTRAINT uq_t_sensors1 UNIQUE (sensorname);


--
-- Name: t_stations uq_t_stations01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_stations
    ADD CONSTRAINT uq_t_stations01 UNIQUE (stationname);


--
-- Name: t_stations uq_t_stations02; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_stations
    ADD CONSTRAINT uq_t_stations02 UNIQUE (stationacronym);


--
-- Name: t_tasktables uq_tasktables01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_tasktables
    ADD CONSTRAINT uq_tasktables01 UNIQUE (name);


--
-- Name: t_tmpnamestrategy uq_tmpnamestrategy01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_tmpnamestrategy
    ADD CONSTRAINT uq_tmpnamestrategy01 UNIQUE (tempname);


--
-- Name: t_transferprotocols uq_transferprotocols01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_transferprotocols
    ADD CONSTRAINT uq_transferprotocols01 UNIQUE (protocol);


--
-- Name: t_usermodes uq_usermodes01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_usermodes
    ADD CONSTRAINT uq_usermodes01 UNIQUE (mode);


--
-- Name: t_users uq_users01; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_users
    ADD CONSTRAINT uq_users01 UNIQUE (userid);


--
-- Name: t_waitingorders uq_waitingorders; Type: CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_waitingorders
    ADD CONSTRAINT uq_waitingorders UNIQUE (orderid, filetype, starttime, stoptime, triggertype);


--
-- Name: ix_circulation_remove_queue01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_circulation_remove_queue01 ON public.t_circulation_remove_queue USING btree (host_id);


--
-- Name: ix_distribution_to_circulation; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_distribution_to_circulation ON public.t_distributionqueue USING btree (circulationqueue_id);


--
-- Name: ix_mv_tableddname; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_mv_tableddname ON public.mv_tableddname USING btree (filetype);


--
-- Name: ix_t_acsisminvrelation01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_acsisminvrelation01 ON public.t_acsisminvrelation USING btree (path);


--
-- Name: ix_t_acsisminvrelation02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_acsisminvrelation02 ON public.t_acsisminvrelation USING btree (medialabel);


--
-- Name: ix_t_applicationevents01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_applicationevents01 ON public.t_applicationevents USING btree (application_id);


--
-- Name: ix_t_applicationevents02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_applicationevents02 ON public.t_applicationevents USING btree (tstamp);


--
-- Name: ix_t_asyncsm_operations01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_asyncsm_operations01 ON public.t_asyncsm_operations USING btree (operation_id);


--
-- Name: ix_t_asyncsm_operations02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_asyncsm_operations02 ON public.t_asyncsm_operations USING btree (sm_id);


--
-- Name: ix_t_asyncsm_operations03; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_asyncsm_operations03 ON public.t_asyncsm_operations USING btree (status_id);


--
-- Name: ix_t_blockedcirculationsqueue01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_blockedcirculationsqueue01 ON public.t_blockedcirculationsqueue USING btree (status_id);


--
-- Name: ix_t_blockedcirculationsqueue02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_blockedcirculationsqueue02 ON public.t_blockedcirculationsqueue USING btree (circulationmode_id);


--
-- Name: ix_t_blockedcirculationsqueue03; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_blockedcirculationsqueue03 ON public.t_blockedcirculationsqueue USING btree (relay_host_id);


--
-- Name: ix_t_blockedcirculationsqueue04; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_blockedcirculationsqueue04 ON public.t_blockedcirculationsqueue USING btree (host_id);


--
-- Name: ix_t_blockedcirculationsqueue05; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_blockedcirculationsqueue05 ON public.t_blockedcirculationsqueue USING btree (circulationpolicy_id);


--
-- Name: ix_t_blockedcirculationsqueue06; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_blockedcirculationsqueue06 ON public.t_blockedcirculationsqueue USING btree (dest_host_id);


--
-- Name: ix_t_blockedcirculationsqueue07; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_blockedcirculationsqueue07 ON public.t_blockedcirculationsqueue USING btree (fromurl);


--
-- Name: ix_t_blockedcirculationsqueue08; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_blockedcirculationsqueue08 ON public.t_blockedcirculationsqueue USING btree (tourl);


--
-- Name: ix_t_blockedcirculationsqueue09; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_blockedcirculationsqueue09 ON public.t_blockedcirculationsqueue USING btree (dest_protocol_id);


--
-- Name: ix_t_blockedcirculationsqueue10; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_blockedcirculationsqueue10 ON public.t_blockedcirculationsqueue USING btree (media_circulation_id);


--
-- Name: ix_t_blockedcirculationsqueue11; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_blockedcirculationsqueue11 ON public.t_blockedcirculationsqueue USING btree (media_insertionmode_id);


--
-- Name: ix_t_blockedcirculationsqueue12; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_blockedcirculationsqueue12 ON public.t_blockedcirculationsqueue USING btree (enddate);


--
-- Name: ix_t_blockedcirculationsqueue13; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_blockedcirculationsqueue13 ON public.t_blockedcirculationsqueue USING btree (localtransfermode_id);


--
-- Name: ix_t_cartsinv01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_cartsinv01 ON public.t_cartsinv USING btree (filename, fileversion);


--
-- Name: ix_t_cartsinv02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_cartsinv02 ON public.t_cartsinv USING btree (cart);


--
-- Name: ix_t_cdp_feederqueue01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_cdp_feederqueue01 ON public.t_cdp_feederqueue USING btree (creationdate);


--
-- Name: ix_t_cdp_feederqueue02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_cdp_feederqueue02 ON public.t_cdp_feederqueue USING btree (retry_count);


--
-- Name: ix_t_cdp_feederqueue03; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_cdp_feederqueue03 ON public.t_cdp_feederqueue USING btree (retry_time);


--
-- Name: ix_t_cdp_feederqueue04; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_cdp_feederqueue04 ON public.t_cdp_feederqueue USING btree (status_id);


--
-- Name: ix_t_cdp_feederqueue05; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_cdp_feederqueue05 ON public.t_cdp_feederqueue USING btree (jobresponsible);


--
-- Name: ix_t_cdp_feederqueue06; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_cdp_feederqueue06 ON public.t_cdp_feederqueue USING btree (tempnamestrategy_id);


--
-- Name: ix_t_cdp_notification_queue01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_cdp_notification_queue01 ON public.t_cdp_notification_queue USING btree (feederqueue_id);


--
-- Name: ix_t_cdp_notification_queue02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_cdp_notification_queue02 ON public.t_cdp_notification_queue USING btree (creationdate);


--
-- Name: ix_t_cdp_notification_queue03; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_cdp_notification_queue03 ON public.t_cdp_notification_queue USING btree (retry_count);


--
-- Name: ix_t_cdp_notification_queue04; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_cdp_notification_queue04 ON public.t_cdp_notification_queue USING btree (retry_time);


--
-- Name: ix_t_cdp_notification_queue05; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_cdp_notification_queue05 ON public.t_cdp_notification_queue USING btree (status_id);


--
-- Name: ix_t_cdp_notification_queue06; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_cdp_notification_queue06 ON public.t_cdp_notification_queue USING btree (jobresponsible);


--
-- Name: ix_t_circ_callbacks01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_circ_callbacks01 ON public.t_circ_callbacks USING btree (circ_queue_id);


--
-- Name: ix_t_circ_callbacks02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_circ_callbacks02 ON public.t_circ_callbacks USING btree (external_id);


--
-- Name: ix_t_circulationpolicies01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_circulationpolicies01 ON public.t_circulationpolicies USING btree (circulator_node_id);


--
-- Name: ix_t_circulationpolicies02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_circulationpolicies02 ON public.t_circulationpolicies USING btree (filetype_id);


--
-- Name: ix_t_circulationpolicies03; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_circulationpolicies03 ON public.t_circulationpolicies USING btree (source_id);


--
-- Name: ix_t_circulationpolicies04; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_circulationpolicies04 ON public.t_circulationpolicies USING btree (destination_id);


--
-- Name: ix_t_circulationpolicies05; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_circulationpolicies05 ON public.t_circulationpolicies USING btree (agent_node_id);


--
-- Name: ix_t_circulationpolicies06; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_circulationpolicies06 ON public.t_circulationpolicies USING btree (localtransfermode_id);


--
-- Name: ix_t_circulationpolicies07; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_circulationpolicies07 ON public.t_circulationpolicies USING btree (status_id);


--
-- Name: ix_t_circulationsmedia01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_circulationsmedia01 ON public.t_circulationsmedia USING btree (status_id);


--
-- Name: ix_t_circulationsqueue01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_circulationsqueue01 ON public.t_circulationsqueue USING btree (status_id);


--
-- Name: ix_t_circulationsqueue02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_circulationsqueue02 ON public.t_circulationsqueue USING btree (circulationmode_id);


--
-- Name: ix_t_circulationsqueue03; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_circulationsqueue03 ON public.t_circulationsqueue USING btree (relay_host_id);


--
-- Name: ix_t_circulationsqueue04; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_circulationsqueue04 ON public.t_circulationsqueue USING btree (host_id);


--
-- Name: ix_t_circulationsqueue05; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_circulationsqueue05 ON public.t_circulationsqueue USING btree (circulationpolicy_id);


--
-- Name: ix_t_circulationsqueue06; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_circulationsqueue06 ON public.t_circulationsqueue USING btree (dest_host_id);


--
-- Name: ix_t_circulationsqueue07; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_circulationsqueue07 ON public.t_circulationsqueue USING btree (fromurl);


--
-- Name: ix_t_circulationsqueue08; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_circulationsqueue08 ON public.t_circulationsqueue USING btree (tourl);


--
-- Name: ix_t_circulationsqueue09; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_circulationsqueue09 ON public.t_circulationsqueue USING btree (dest_protocol_id);


--
-- Name: ix_t_circulationsqueue10; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_circulationsqueue10 ON public.t_circulationsqueue USING btree (media_circulation_id);


--
-- Name: ix_t_circulationsqueue11; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_circulationsqueue11 ON public.t_circulationsqueue USING btree (media_insertionmode_id);


--
-- Name: ix_t_circulationsqueue12; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_circulationsqueue12 ON public.t_circulationsqueue USING btree (enddate);


--
-- Name: ix_t_circulationsqueue13; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_circulationsqueue13 ON public.t_circulationsqueue USING btree (localtransfermode_id);


--
-- Name: ix_t_circulationsqueue14; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_circulationsqueue14 ON public.t_circulationsqueue USING btree (streamer_id);


--
-- Name: ix_t_circulationsqueuedwh01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_circulationsqueuedwh01 ON public.t_circulationsqueuedwh USING btree (status_id);


--
-- Name: ix_t_circulationsqueuedwh02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_circulationsqueuedwh02 ON public.t_circulationsqueuedwh USING btree (circulationmode_id);


--
-- Name: ix_t_circulationsqueuedwh03; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_circulationsqueuedwh03 ON public.t_circulationsqueuedwh USING btree (enddate);


--
-- Name: ix_t_circulationsqueuedwh04; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_circulationsqueuedwh04 ON public.t_circulationsqueuedwh USING btree (creationdate);


--
-- Name: ix_t_circulationsqueuedwh05; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_circulationsqueuedwh05 ON public.t_circulationsqueuedwh USING btree (circulation_id);


--
-- Name: ix_t_circulationsqueuedwh06; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_circulationsqueuedwh06 ON public.t_circulationsqueuedwh USING btree (source_db_host);


--
-- Name: ix_t_circulationsqueuedwh07; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_circulationsqueuedwh07 ON public.t_circulationsqueuedwh USING btree (modificationdate);


--
-- Name: ix_t_circulationsqueuedwh08; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_circulationsqueuedwh08 ON public.t_circulationsqueuedwh USING btree (host_id);


--
-- Name: ix_t_circulationstats01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_circulationstats01 ON public.t_circulationstats USING btree (circulationsqueue_id);


--
-- Name: ix_t_circulationstats02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_circulationstats02 ON public.t_circulationstats USING btree (from_host_id);


--
-- Name: ix_t_circulationstats03; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_circulationstats03 ON public.t_circulationstats USING btree (to_host_id);


--
-- Name: ix_t_classifiedfiletypes; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_classifiedfiletypes ON public.t_classifiedfiletypes USING btree (filetype);


--
-- Name: ix_t_cleanuprules01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_cleanuprules01 ON public.t_cleanuprules USING btree (repository_id);


--
-- Name: ix_t_cleanuprules02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_cleanuprules02 ON public.t_cleanuprules USING btree (cleanupalgo_id);


--
-- Name: ix_t_cov_periods01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_cov_periods01 ON public.t_cov_periods USING btree (coveragerule_id);


--
-- Name: ix_t_cov_periods02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_cov_periods02 ON public.t_cov_periods USING btree (t_begin);


--
-- Name: ix_t_cov_periods03; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_cov_periods03 ON public.t_cov_periods USING btree (t_end);


--
-- Name: ix_t_cov_periods04; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_cov_periods04 ON public.t_cov_periods USING btree (exp_val_start);


--
-- Name: ix_t_cov_periods05; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_cov_periods05 ON public.t_cov_periods USING btree (exp_val_stop);


--
-- Name: ix_t_cov_periods06; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_cov_periods06 ON public.t_cov_periods USING btree (creationdate);


--
-- Name: ix_t_cov_relevancies01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_cov_relevancies01 ON public.t_cov_relevancies USING btree (period_id);


--
-- Name: ix_t_cov_relevancies02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_cov_relevancies02 ON public.t_cov_relevancies USING btree (inv_id);


--
-- Name: ix_t_cov_rules01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_cov_rules01 ON public.t_cov_rules USING btree (last_update);


--
-- Name: ix_t_cov_x_filetypes01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_cov_x_filetypes01 ON public.t_cov_x_filetypes USING btree (coveragerule_id);


--
-- Name: ix_t_cov_x_filetypes02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_cov_x_filetypes02 ON public.t_cov_x_filetypes USING btree (filetype_id);


--
-- Name: ix_t_cr_x_filetypes01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_cr_x_filetypes01 ON public.t_cr_x_filetypes USING btree (crgroup_id);


--
-- Name: ix_t_cr_x_filetypes02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_cr_x_filetypes02 ON public.t_cr_x_filetypes USING btree (filetype_id);


--
-- Name: ix_t_crgroups01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_crgroups01 ON public.t_crgroups USING btree (virtual_filetype_id);


--
-- Name: ix_t_crhistory01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_crhistory01 ON public.t_crhistory USING btree (inventory_id);


--
-- Name: ix_t_crhistory02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_crhistory02 ON public.t_crhistory USING btree (crgroup_id);


--
-- Name: ix_t_crhistory03; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_crhistory03 ON public.t_crhistory USING btree (tstamp);


--
-- Name: ix_t_crhistory04; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_crhistory04 ON public.t_crhistory USING btree (author);


--
-- Name: ix_t_crrelations01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_crrelations01 ON public.t_crrelations USING btree (crgroup_id);


--
-- Name: ix_t_crrelations02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_crrelations02 ON public.t_crrelations USING btree (repository_id);


--
-- Name: ix_t_crrelations03; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_crrelations03 ON public.t_crrelations USING btree (distributionrule_id);


--
-- Name: ix_t_crtemporaries01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_crtemporaries01 ON public.t_crtemporaries USING btree (inventory_id);


--
-- Name: ix_t_crtemporaries02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_crtemporaries02 ON public.t_crtemporaries USING btree (crgroup_id);


--
-- Name: ix_t_crtemporaries03; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_crtemporaries03 ON public.t_crtemporaries USING btree (tstamp);


--
-- Name: ix_t_crtemporaries04; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_crtemporaries04 ON public.t_crtemporaries USING btree (author);


--
-- Name: ix_t_datastoragequeue01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_datastoragequeue01 ON public.t_datastoragequeue USING btree (status_id);


--
-- Name: ix_t_datastoragequeue02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_datastoragequeue02 ON public.t_datastoragequeue USING btree (type_id);


--
-- Name: ix_t_datastoragequeue03; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_datastoragequeue03 ON public.t_datastoragequeue USING btree (md5);


--
-- Name: ix_t_datastoragequeue04; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_datastoragequeue04 ON public.t_datastoragequeue USING btree (flowtag_id);


--
-- Name: ix_t_dbparameters01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_dbparameters01 ON public.t_dbparameters USING btree (parametertype_id);


--
-- Name: ix_t_distpoldataset01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_distpoldataset01 ON public.t_distpoldataset USING btree (policy);


--
-- Name: ix_t_distpoldataset02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_distpoldataset02 ON public.t_distpoldataset USING btree (dataset);


--
-- Name: ix_t_distributionitems01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_distributionitems01 ON public.t_distributionitems USING btree (packageid);


--
-- Name: ix_t_distributionitems02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_distributionitems02 ON public.t_distributionitems USING btree (filename, fileversion);


--
-- Name: ix_t_distributionitems03; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_distributionitems03 ON public.t_distributionitems USING btree (mediatype);


--
-- Name: ix_t_distributionitems04; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_distributionitems04 ON public.t_distributionitems USING btree (userid);


--
-- Name: ix_t_distributionitems05; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_distributionitems05 ON public.t_distributionitems USING btree (format);


--
-- Name: ix_t_distributionitems06; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_distributionitems06 ON public.t_distributionitems USING btree (cartid);


--
-- Name: ix_t_distributionitems07; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_distributionitems07 ON public.t_distributionitems USING btree (distributionrule_id);


--
-- Name: ix_t_distributionitems08; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_distributionitems08 ON public.t_distributionitems USING btree (disseminationpackingalgo_id);


--
-- Name: ix_t_distributionitems09; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_distributionitems09 ON public.t_distributionitems USING btree (distributeddate);


--
-- Name: ix_t_distributionitems10; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_distributionitems10 ON public.t_distributionitems USING btree (creationdate);


--
-- Name: ix_t_distributionqueue01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_distributionqueue01 ON public.t_distributionqueue USING btree (status);


--
-- Name: ix_t_distributionqueue02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_distributionqueue02 ON public.t_distributionqueue USING btree (format);


--
-- Name: ix_t_distributionqueue03; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_distributionqueue03 ON public.t_distributionqueue USING btree (creationdate);


--
-- Name: ix_t_distributionqueue04; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_distributionqueue04 ON public.t_distributionqueue USING btree (distributeddate);


--
-- Name: ix_t_distributionrules01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_distributionrules01 ON public.t_distributionrules USING btree (filetype_id);


--
-- Name: ix_t_distributionrules02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_distributionrules02 ON public.t_distributionrules USING btree (mediatype_id);


--
-- Name: ix_t_distributionrules03; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_distributionrules03 ON public.t_distributionrules USING btree (repository_id);


--
-- Name: ix_t_distributionrules04; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_distributionrules04 ON public.t_distributionrules USING btree (user_id);


--
-- Name: ix_t_distributionrules05; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_distributionrules05 ON public.t_distributionrules USING btree (usermode_id);


--
-- Name: ix_t_distributionrules06; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_distributionrules06 ON public.t_distributionrules USING btree (distributionalgo_id);


--
-- Name: ix_t_distributionrules07; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_distributionrules07 ON public.t_distributionrules USING btree (format_tt_id);


--
-- Name: ix_t_distributionrules08; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_distributionrules08 ON public.t_distributionrules USING btree (delivery_tt_id);


--
-- Name: ix_t_distributionrules09; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_distributionrules09 ON public.t_distributionrules USING btree (delcheck_tt_id);


--
-- Name: ix_t_distributionrules10; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_distributionrules10 ON public.t_distributionrules USING btree (fileclass_id);


--
-- Name: ix_t_distributionrules11; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_distributionrules11 ON public.t_distributionrules USING btree (disseminationpackingalgo_id);


--
-- Name: ix_t_distributionwaitingitems1; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_distributionwaitingitems1 ON public.t_distributionwaitingitems USING btree (distributionitem_id);


--
-- Name: ix_t_distributionwaitingitems2; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_distributionwaitingitems2 ON public.t_distributionwaitingitems USING btree (package_id);


--
-- Name: ix_t_distributionwaitingitems3; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_distributionwaitingitems3 ON public.t_distributionwaitingitems USING btree (inventory_id);


--
-- Name: ix_t_distruleoriginator01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_distruleoriginator01 ON public.t_distruleoriginator USING btree (distributionrule_id);


--
-- Name: ix_t_distruleoriginator02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_distruleoriginator02 ON public.t_distruleoriginator USING btree (originator_id);


--
-- Name: ix_t_dsqueuexcirqueue01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_dsqueuexcirqueue01 ON public.t_dsqueue_x_cirqueue USING btree (datastoragequeue_id);


--
-- Name: ix_t_dsqueuexcirqueue02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_dsqueuexcirqueue02 ON public.t_dsqueue_x_cirqueue USING btree (circulation_id);


--
-- Name: ix_t_dsqueuexcirqueue03; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_dsqueuexcirqueue03 ON public.t_dsqueue_x_cirqueue USING btree (globalstatus_id);


--
-- Name: ix_t_dsqueuexcirqueue04; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_dsqueuexcirqueue04 ON public.t_dsqueue_x_cirqueue USING btree (circ_status_id);


--
-- Name: ix_t_dsqueuexcirqueue05; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_dsqueuexcirqueue05 ON public.t_dsqueue_x_cirqueue USING btree (localtransfermode_id);


--
-- Name: ix_t_emitted_order_events01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_emitted_order_events01 ON public.t_emitted_order_events USING btree (event_time);


--
-- Name: ix_t_emitted_order_events02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_emitted_order_events02 ON public.t_emitted_order_events USING btree (processor_id, order_type);


--
-- Name: ix_t_filetypes_last_inv01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_filetypes_last_inv01 ON public.t_filetypes_last_inv USING btree (filetype);


--
-- Name: ix_t_filetypes_x_satellites01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_filetypes_x_satellites01 ON public.t_filetypes_x_satellites USING btree (filetype);


--
-- Name: ix_t_filetypes_x_satellites02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_filetypes_x_satellites02 ON public.t_filetypes_x_satellites USING btree (satellite_id);


--
-- Name: ix_t_filetypes_x_stylesheets01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_filetypes_x_stylesheets01 ON public.t_filetypes_x_stylesheets USING btree (filetype_id);


--
-- Name: ix_t_filetypes_x_stylesheets02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_filetypes_x_stylesheets02 ON public.t_filetypes_x_stylesheets USING btree (stylesheet_id);


--
-- Name: ix_t_filetypes_x_stylesheets03; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_filetypes_x_stylesheets03 ON public.t_filetypes_x_stylesheets USING btree (outputtype_id);


--
-- Name: ix_t_filetypescomps01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_filetypescomps01 ON public.t_filetypescomps USING btree (pfiletype);


--
-- Name: ix_t_filetypescomps02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_filetypescomps02 ON public.t_filetypescomps USING btree (cfiletype);


--
-- Name: ix_t_filetypesxmigrations01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_filetypesxmigrations01 ON public.t_filetypes_x_migrations USING btree (sm_id);


--
-- Name: ix_t_filetypesxmigrations02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_filetypesxmigrations02 ON public.t_filetypes_x_migrations USING btree (filetype_id);


--
-- Name: ix_t_filetypesxmigrations03; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_filetypesxmigrations03 ON public.t_filetypes_x_migrations USING btree (migrationalgo_id);


--
-- Name: ix_t_inv_additional_params01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inv_additional_params01 ON public.t_inv_additional_params USING btree (inv_id);


--
-- Name: ix_t_inv_additional_params02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inv_additional_params02 ON public.t_inv_additional_params USING btree (timeliness_key_id);


--
-- Name: ix_t_inv_additional_params03; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inv_additional_params03 ON public.t_inv_additional_params USING btree (data_take_id);


--
-- Name: ix_t_inv_data_restored01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inv_data_restored01 ON public.t_inv_data_restored USING btree (sm_id);


--
-- Name: ix_t_inv_data_restored02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inv_data_restored02 ON public.t_inv_data_restored USING btree (inv_id);


--
-- Name: ix_t_inv_data_restored03; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inv_data_restored03 ON public.t_inv_data_restored USING btree (parent_inv_id);


--
-- Name: ix_t_inv_data_restored04; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inv_data_restored04 ON public.t_inv_data_restored USING btree (filetype_id);


--
-- Name: ix_t_inv_data_restored05; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inv_data_restored05 ON public.t_inv_data_restored USING btree (tstamp);


--
-- Name: ix_t_inv_downloads01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inv_downloads01 ON public.t_inv_downloads USING btree (inv_id);


--
-- Name: ix_t_inv_downloads02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inv_downloads02 ON public.t_inv_downloads USING btree (client_id);


--
-- Name: ix_t_inv_relations01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inv_relations01 ON public.t_inv_relations USING btree (rel_type_id);


--
-- Name: ix_t_inv_relations02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inv_relations02 ON public.t_inv_relations USING btree (inv_id1);


--
-- Name: ix_t_inv_relations03; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inv_relations03 ON public.t_inv_relations USING btree (inv_id2);


--
-- Name: ix_t_inv_relations04; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inv_relations04 ON public.t_inv_relations USING btree (relation);


--
-- Name: ix_t_inv_x_baselines01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inv_x_baselines01 ON public.t_inv_x_baselines USING btree (inv_id);


--
-- Name: ix_t_inv_x_baselines02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inv_x_baselines02 ON public.t_inv_x_baselines USING btree (inv_baseline_id);


--
-- Name: ix_t_invanomalies01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_invanomalies01 ON public.t_inventoryanomalies USING btree (anomaly_type_id);


--
-- Name: ix_t_inventory01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inventory01 ON public.t_inventory USING btree (dir_id);


--
-- Name: ix_t_inventory02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inventory02 ON public.t_inventory USING btree (ref_dir_id);


--
-- Name: ix_t_inventory03; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inventory03 ON public.t_inventory USING btree (orderid);


--
-- Name: ix_t_inventory04; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inventory04 ON public.t_inventory USING btree (logical_id);


--
-- Name: ix_t_inventory05; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inventory05 ON public.t_inventory USING btree (fileclass);


--
-- Name: ix_t_inventory06; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inventory06 ON public.t_inventory USING btree (sensorid);


--
-- Name: ix_t_inventory07; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inventory07 ON public.t_inventory USING btree (satelliteid);


--
-- Name: ix_t_inventory08; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inventory08 ON public.t_inventory USING btree (acquisitionstationid);


--
-- Name: ix_t_inventory09; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inventory09 ON public.t_inventory USING btree (filetype);


--
-- Name: ix_t_inventory10; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inventory10 ON public.t_inventory USING btree (processingstationid);


--
-- Name: ix_t_inventory11; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inventory11 ON public.t_inventory USING btree (qualified);


--
-- Name: ix_t_inventory12; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inventory12 ON public.t_inventory USING btree (band);


--
-- Name: ix_t_inventory13; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inventory13 ON public.t_inventory USING btree (filename);


--
-- Name: ix_t_inventory14; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inventory14 ON public.t_inventory USING btree (validitystart);


--
-- Name: ix_t_inventory15; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inventory15 ON public.t_inventory USING btree (validitystop);


--
-- Name: ix_t_inventory16; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inventory16 ON public.t_inventory USING btree (inventorydate);


--
-- Name: ix_t_inventory17; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inventory17 ON public.t_inventory USING btree (modificationdate);


--
-- Name: ix_t_inventory18; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inventory18 ON public.t_inventory USING btree (generationtime);


--
-- Name: ix_t_inventory19; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inventory19 ON public.t_inventory USING btree (startorbitnumber);


--
-- Name: ix_t_inventory20; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inventory20 ON public.t_inventory USING btree (originator);


--
-- Name: ix_t_inventory_jsonb; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inventory_jsonb ON public.t_inventory USING gin (attributes);


--
-- Name: ix_t_inventory_spatial; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inventory_spatial ON public.t_inventory USING gist (geographiclocalization);


--
-- Name: ix_t_inventorycomments01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inventorycomments01 ON public.t_inventorycomments USING btree (filename, fileversion);


--
-- Name: ix_t_inventorydataset01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inventorydataset01 ON public.t_inventorydataset USING btree (filename, fileversion);


--
-- Name: ix_t_inventorydataset02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inventorydataset02 ON public.t_inventorydataset USING btree (measdatasetid);


--
-- Name: ix_t_inventoryeventsconf01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inventoryeventsconf01 ON public.t_inventoryeventsconf USING btree (filetype);


--
-- Name: ix_t_inventorygaps01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inventorygaps01 ON public.t_inventorygaps USING btree (filename, fileversion);


--
-- Name: ix_t_inventorygaps02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inventorygaps02 ON public.t_inventorygaps USING btree (begin);


--
-- Name: ix_t_inventorygaps03; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inventorygaps03 ON public.t_inventorygaps USING btree ("end");


--
-- Name: ix_t_inventorylinks01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inventorylinks01 ON public.t_inventorylinks USING btree (orig_filename, orig_fileversion);


--
-- Name: ix_t_inventorylinks02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inventorylinks02 ON public.t_inventorylinks USING btree (dest_filename, dest_fileversion);


--
-- Name: ix_t_inventorylinksconf01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inventorylinksconf01 ON public.t_inventorylinksconf USING btree (orig_filetype);


--
-- Name: ix_t_inventorylinksconf02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inventorylinksconf02 ON public.t_inventorylinksconf USING btree (dest_filetype);


--
-- Name: ix_t_inventorysm01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inventorysm01 ON public.t_inventorysm USING btree (inv_id);


--
-- Name: ix_t_inventorysm02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inventorysm02 ON public.t_inventorysm USING btree (storagemanid);


--
-- Name: ix_t_inventorysm03; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_inventorysm03 ON public.t_inventorysm USING btree (packageid);


--
-- Name: ix_t_invrollingpolicies01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_invrollingpolicies01 ON public.t_invrollingpolicies USING btree (filetype);


--
-- Name: ix_t_invrollingpolicies02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_invrollingpolicies02 ON public.t_invrollingpolicies USING btree (algorithm);


--
-- Name: ix_t_invrollingpolicies03; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_invrollingpolicies03 ON public.t_invrollingpolicies USING btree (action);


--
-- Name: ix_t_invrollingpolicies04; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_invrollingpolicies04 ON public.t_invrollingpolicies USING btree (fileclass);


--
-- Name: ix_t_invrollingpolicies05; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_invrollingpolicies05 ON public.t_invrollingpolicies USING btree (storageid);


--
-- Name: ix_t_invsiterelation01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_invsiterelation01 ON public.t_invsiterelation USING btree (filename, fileversion);


--
-- Name: ix_t_invsiterelation02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_invsiterelation02 ON public.t_invsiterelation USING btree (site);


--
-- Name: ix_t_invspecialrollingpol01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_invspecialrollingpol01 ON public.t_invspecialrollingpolicies USING btree (inventoryname, inventoryversion);


--
-- Name: ix_t_invspecialrollingpol02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_invspecialrollingpol02 ON public.t_invspecialrollingpolicies USING btree (action);


--
-- Name: ix_t_invspecialrollingpol03; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_invspecialrollingpol03 ON public.t_invspecialrollingpolicies USING btree (storageid);


--
-- Name: ix_t_mediacapformat01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_mediacapformat01 ON public.t_mediacapformat USING btree (mediatype);


--
-- Name: ix_t_mpm_cross01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_mpm_cross01 ON public.t_mpm_cross USING btree (value_id);


--
-- Name: ix_t_mpm_cross02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_mpm_cross02 ON public.t_mpm_cross USING btree (parameter_id);


--
-- Name: ix_t_mpm_cross03; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_mpm_cross03 ON public.t_mpm_cross USING btree (routine_id);


--
-- Name: ix_t_mpm_cross04; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_mpm_cross04 ON public.t_mpm_cross USING btree (product_id);


--
-- Name: ix_t_mpm_dip01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_mpm_dip01 ON public.t_mpm_dip USING btree (inventory_id);


--
-- Name: ix_t_mpm_dip02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_mpm_dip02 ON public.t_mpm_dip USING btree (eeprom);


--
-- Name: ix_t_mpm_dip03; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_mpm_dip03 ON public.t_mpm_dip USING btree (ram);


--
-- Name: ix_t_mpm_dip04; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_mpm_dip04 ON public.t_mpm_dip USING btree (dip_status_id);


--
-- Name: ix_t_mpm_parameters01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_mpm_parameters01 ON public.t_mpm_parameters USING btree (paramtype_id);


--
-- Name: ix_t_mpm_parameters02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_mpm_parameters02 ON public.t_mpm_parameters USING btree (parameter_name);


--
-- Name: ix_t_mpm_prb01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_mpm_prb01 ON public.t_mpm_prb USING btree (inventory_id);


--
-- Name: ix_t_mpm_prb02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_mpm_prb02 ON public.t_mpm_prb USING btree (status_id);


--
-- Name: ix_t_mpm_products01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_mpm_products01 ON public.t_mpm_products USING btree (productname);


--
-- Name: ix_t_mpm_products02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_mpm_products02 ON public.t_mpm_products USING btree (filetype_id);


--
-- Name: ix_t_mpm_products03; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_mpm_products03 ON public.t_mpm_products USING btree (timeliness_id);


--
-- Name: ix_t_mpm_products04; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_mpm_products04 ON public.t_mpm_products USING btree (insertiontime);


--
-- Name: ix_t_mpm_products05; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_mpm_products05 ON public.t_mpm_products USING btree (creationtime);


--
-- Name: ix_t_mpm_products06; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_mpm_products06 ON public.t_mpm_products USING btree (starttime);


--
-- Name: ix_t_mpm_products07; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_mpm_products07 ON public.t_mpm_products USING btree (stoptime);


--
-- Name: ix_t_mpm_products08; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_mpm_products08 ON public.t_mpm_products USING btree (orbit_num);


--
-- Name: ix_t_mpm_products09; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_mpm_products09 ON public.t_mpm_products USING btree (processor_id);


--
-- Name: ix_t_mpm_products10; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_mpm_products10 ON public.t_mpm_products USING btree (qualityflag_id);


--
-- Name: ix_t_mpm_products_spatial; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_mpm_products_spatial ON public.t_mpm_products USING gist (footprint);


--
-- Name: ix_t_mpm_values_id; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_mpm_values_id ON public.t_mpm_values USING btree (id_value);


--
-- Name: ix_t_mpm_values_part1; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_mpm_values_part1 ON public.t_mpm_values_part1 USING btree (creationtime);


--
-- Name: ix_t_mpm_values_part2; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_mpm_values_part2 ON public.t_mpm_values_part2 USING btree (creationtime);


--
-- Name: ix_t_ondemanddistribution01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_ondemanddistribution01 ON public.t_ondemanddistribution USING btree (orderid);


--
-- Name: ix_t_ondemanddistribution02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_ondemanddistribution02 ON public.t_ondemanddistribution USING btree (userid);


--
-- Name: ix_t_ondemanddistribution03; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_ondemanddistribution03 ON public.t_ondemanddistribution USING btree (format);


--
-- Name: ix_t_ondemanddistribution04; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_ondemanddistribution04 ON public.t_ondemanddistribution USING btree (mediatype);


--
-- Name: ix_t_ondemanddistribution05; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_ondemanddistribution05 ON public.t_ondemanddistribution USING btree (disseminationpackingalgo_id);


--
-- Name: ix_t_ordersattachments01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_ordersattachments01 ON public.t_ordersattachments USING btree (orderid);


--
-- Name: ix_t_ordersattachments02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_ordersattachments02 ON public.t_ordersattachments USING btree (attachtype);


--
-- Name: ix_t_ordersgeneration01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_ordersgeneration01 ON public.t_ordersgeneration USING btree (processorname, processorversion);


--
-- Name: ix_t_ordersgeneration02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_ordersgeneration02 ON public.t_ordersgeneration USING btree (ordertype);


--
-- Name: ix_t_ordersgeneration03; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_ordersgeneration03 ON public.t_ordersgeneration USING btree (algorithm);


--
-- Name: ix_t_ordersgeneration04; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_ordersgeneration04 ON public.t_ordersgeneration USING btree (filetype);


--
-- Name: ix_t_ordersgeneration05; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_ordersgeneration05 ON public.t_ordersgeneration USING btree (fileclass);


--
-- Name: ix_t_ordersinput01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_ordersinput01 ON public.t_ordersinput USING btree (orderid);


--
-- Name: ix_t_ordersinput02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_ordersinput02 ON public.t_ordersinput USING btree (filename, fileversion);


--
-- Name: ix_t_ordersqueue01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_ordersqueue01 ON public.t_ordersqueue USING btree (status);


--
-- Name: ix_t_ordersqueue02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_ordersqueue02 ON public.t_ordersqueue USING btree (ordertype);


--
-- Name: ix_t_ordersqueue03; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_ordersqueue03 ON public.t_ordersqueue USING btree (satelliteid);


--
-- Name: ix_t_ordersqueue04; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_ordersqueue04 ON public.t_ordersqueue USING btree (mission);


--
-- Name: ix_t_ordersqueue05; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_ordersqueue05 ON public.t_ordersqueue USING btree (processorname, processorversion);


--
-- Name: ix_t_ordersqueue06; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_ordersqueue06 ON public.t_ordersqueue USING btree (creationdate);


--
-- Name: ix_t_ordersqueue07; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_ordersqueue07 ON public.t_ordersqueue USING btree (starttime);


--
-- Name: ix_t_ordersqueue08; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_ordersqueue08 ON public.t_ordersqueue USING btree (stoptime);


--
-- Name: ix_t_ordersqueue09; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_ordersqueue09 ON public.t_ordersqueue USING btree (jobresponsible);


--
-- Name: ix_t_ordersqueue10; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_ordersqueue10 ON public.t_ordersqueue USING btree (productionstoptime);


--
-- Name: ix_t_ordersqueueblacklist01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_ordersqueueblacklist01 ON public.t_ordersqueueblacklist USING btree (orderid);


--
-- Name: ix_t_ordersqueueblacklist02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_ordersqueueblacklist02 ON public.t_ordersqueueblacklist USING btree (jobresponsible);


--
-- Name: ix_t_orderstatetransitions01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_orderstatetransitions01 ON public.t_orderstatetransitions USING btree (orderid);


--
-- Name: ix_t_ordtriggeringfiles01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_ordtriggeringfiles01 ON public.t_ordtriggeringfiles USING btree (filename, fileversion);


--
-- Name: ix_t_ordtriggeringfiles02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_ordtriggeringfiles02 ON public.t_ordtriggeringfiles USING btree (orderid);


--
-- Name: ix_t_proc_ftype_generation01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_proc_ftype_generation01 ON public.t_proc_ftype_generation USING btree (proc_id);


--
-- Name: ix_t_proc_ftype_generation02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_proc_ftype_generation02 ON public.t_proc_ftype_generation USING btree (ftype_id);


--
-- Name: ix_t_receptionrules01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_receptionrules01 ON public.t_receptionrules USING btree (filetype_id);


--
-- Name: ix_t_receptionrules02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_receptionrules02 ON public.t_receptionrules USING btree (repository_id);


--
-- Name: ix_t_receptionrules03; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_receptionrules03 ON public.t_receptionrules USING btree (user_id);


--
-- Name: ix_t_receptionrules04; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_receptionrules04 ON public.t_receptionrules USING btree (usermode_id);


--
-- Name: ix_t_receptionrules05; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_receptionrules05 ON public.t_receptionrules USING btree (reception_tt_id);


--
-- Name: ix_t_receptionrules06; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_receptionrules06 ON public.t_receptionrules USING btree (discoveryplugin_id);


--
-- Name: ix_t_receptionrules07; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_receptionrules07 ON public.t_receptionrules USING btree (satellite_id);


--
-- Name: ix_t_receptionrules08; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_receptionrules08 ON public.t_receptionrules USING btree (sensor_id);


--
-- Name: ix_t_receptionruleshist01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_receptionruleshist01 ON public.t_receptionruleshist USING btree (receptionrule_id);


--
-- Name: ix_t_receptionruleshist02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_receptionruleshist02 ON public.t_receptionruleshist USING btree (filename);


--
-- Name: ix_t_receptionruleshist03; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_receptionruleshist03 ON public.t_receptionruleshist USING btree (status_id);


--
-- Name: ix_t_receptionruleshist04; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_receptionruleshist04 ON public.t_receptionruleshist USING btree (filecreationtstamp);


--
-- Name: ix_t_receptionruleslock01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_receptionruleslock01 ON public.t_receptionruleslock USING btree (receptionrule_id);


--
-- Name: ix_t_repositories01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_repositories01 ON public.t_repositories USING btree (transferprotocol_id);


--
-- Name: ix_t_repositories02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_repositories02 ON public.t_repositories USING btree (host_id);


--
-- Name: ix_t_repositories03; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_repositories03 ON public.t_repositories USING btree (tempname_id);


--
-- Name: ix_t_satrelations01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_satrelations01 ON public.t_satrelations USING btree (satelliteid);


--
-- Name: ix_t_satrelations02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_satrelations02 ON public.t_satrelations USING btree (sensorid);


--
-- Name: ix_t_sites01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_sites01 ON public.t_sites USING btree (parent);


--
-- Name: ix_t_sites02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_sites02 ON public.t_sites USING btree (type);


--
-- Name: ix_t_sm_sync_missingactions01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_sm_sync_missingactions01 ON public.t_sm_sync_missingactions USING btree (sm_id);


--
-- Name: ix_t_sm_sync_missingactions02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_sm_sync_missingactions02 ON public.t_sm_sync_missingactions USING btree (inv_id);


--
-- Name: ix_t_sm_sync_missingactions03; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_sm_sync_missingactions03 ON public.t_sm_sync_missingactions USING btree (invsm_id);


--
-- Name: ix_t_smactions01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_smactions01 ON public.t_smactions USING btree (inv_id);


--
-- Name: ix_t_smactions02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_smactions02 ON public.t_smactions USING btree (smid);


--
-- Name: ix_t_smactions03; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_smactions03 ON public.t_smactions USING btree (packageid);


--
-- Name: ix_t_smactions04; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_smactions04 ON public.t_smactions USING btree (filename);


--
-- Name: ix_t_smactions05; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_smactions05 ON public.t_smactions USING btree (fileversion);


--
-- Name: ix_t_smactions06; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_smactions06 ON public.t_smactions USING btree (operation);


--
-- Name: ix_t_smactions07; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_smactions07 ON public.t_smactions USING btree (enddate);


--
-- Name: ix_t_smqueue01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_smqueue01 ON public.t_smqueue USING btree (smid);


--
-- Name: ix_t_smqueue02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_smqueue02 ON public.t_smqueue USING btree (status);


--
-- Name: ix_t_standingdistribution01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_standingdistribution01 ON public.t_standingdistribution USING btree (distpoliciesid);


--
-- Name: ix_t_standingdistribution02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_standingdistribution02 ON public.t_standingdistribution USING btree (cartsid);


--
-- Name: ix_t_statetrans_allowed01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_statetrans_allowed01 ON public.t_statetransitions_allowed USING btree (table_id);


--
-- Name: ix_t_statetrans_allowed02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_statetrans_allowed02 ON public.t_statetransitions_allowed USING btree (prev_state_id);


--
-- Name: ix_t_statetrans_allowed03; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_statetrans_allowed03 ON public.t_statetransitions_allowed USING btree (post_state_id);


--
-- Name: ix_t_statetransitions_tables01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_statetransitions_tables01 ON public.t_statetransitions_tables USING btree (flow_id);


--
-- Name: ix_t_storagegroup01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_storagegroup01 ON public.t_storagegroup USING btree (filetype);


--
-- Name: ix_t_storagegroup02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_storagegroup02 ON public.t_storagegroup USING btree (storagemanager);


--
-- Name: ix_t_storagemanager01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_storagemanager01 ON public.t_storagemanager USING btree (type);


--
-- Name: ix_t_storagemanager02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_storagemanager02 ON public.t_storagemanager USING btree (migrationalgo_id);


--
-- Name: ix_t_storagerollingthreshold01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_storagerollingthreshold01 ON public.t_storagerollingthresholds USING btree (action_id);


--
-- Name: ix_t_tasktables01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_tasktables01 ON public.t_tasktables USING btree (type_id);


--
-- Name: ix_t_temporarydistribution01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_temporarydistribution01 ON public.t_temporarydistribution USING btree (filename, fileversion);


--
-- Name: ix_t_temporarydistribution02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_temporarydistribution02 ON public.t_temporarydistribution USING btree (mediatype);


--
-- Name: ix_t_temporarydistribution03; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_temporarydistribution03 ON public.t_temporarydistribution USING btree (userid);


--
-- Name: ix_t_temporarydistribution04; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_temporarydistribution04 ON public.t_temporarydistribution USING btree (format);


--
-- Name: ix_t_temporarydistribution05; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_temporarydistribution05 ON public.t_temporarydistribution USING btree (algo);


--
-- Name: ix_t_temporarydistribution06; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_temporarydistribution06 ON public.t_temporarydistribution USING btree (policy);


--
-- Name: ix_t_temporarydistribution07; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_temporarydistribution07 ON public.t_temporarydistribution USING btree (cartid);


--
-- Name: ix_t_temporarydistribution08; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_temporarydistribution08 ON public.t_temporarydistribution USING btree (disseminationpackingalgo_id);


--
-- Name: ix_t_temporaryorders01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_temporaryorders01 ON public.t_temporaryorders USING btree (filename, fileversion);


--
-- Name: ix_t_temporaryorders02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_temporaryorders02 ON public.t_temporaryorders USING btree (ordersat);


--
-- Name: ix_t_usdfwa01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_usdfwa01 ON public.t_usdfwa USING btree (filename, fileversion);


--
-- Name: ix_t_usdfwa_del_det01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_usdfwa_del_det01 ON public.t_usdfwa_delivery_details USING btree (usdfwa_id);


--
-- Name: ix_t_users01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_users01 ON public.t_users USING btree (usertype);


--
-- Name: ix_t_users02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_users02 ON public.t_users USING btree (mode);


--
-- Name: ix_t_waitingorders01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_waitingorders01 ON public.t_waitingorders USING btree (orderid);


--
-- Name: ix_t_waitingorders02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_waitingorders02 ON public.t_waitingorders USING btree (filetype);


--
-- Name: ix_t_waitingorders03; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE INDEX ix_t_waitingorders03 ON public.t_waitingorders USING btree (triggertype);


--
-- Name: uq_datastoragequeue01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE UNIQUE INDEX uq_datastoragequeue01 ON public.t_datastoragequeue USING btree (COALESCE(md5, 'NULL'::bpchar), type_id, status_id) WHERE (status_id <> 3);


--
-- Name: uq_distributionrules01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE UNIQUE INDEX uq_distributionrules01 ON public.t_distributionrules USING btree (ondemand, filetype_id, mediatype_id, user_id, usermode_id, repository_id, format_tt_id) WHERE ((active = true) AND (fileclass_id IS NULL));


--
-- Name: uq_distributionrules02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE UNIQUE INDEX uq_distributionrules02 ON public.t_distributionrules USING btree (ondemand, filetype_id, mediatype_id, user_id, usermode_id, repository_id, format_tt_id, fileclass_id) WHERE ((active = true) AND (fileclass_id IS NOT NULL));


--
-- Name: uq_invrollingpolicies; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE UNIQUE INDEX uq_invrollingpolicies ON public.t_invrollingpolicies USING btree (filetype, action, algorithm, COALESCE(fileclass, ''::character varying), COALESCE(storageid, 0));


--
-- Name: uq_mpm_ipf_adf_check01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE UNIQUE INDEX uq_mpm_ipf_adf_check01 ON public.t_mpm_ipf_adf_check USING btree (product_id, adf_ipf_expected, COALESCE(adf_ipf_found, 'NULL'::character varying), COALESCE(filename, 'NULL'::character varying));


--
-- Name: uq_ordersgeneration; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE UNIQUE INDEX uq_ordersgeneration ON public.t_ordersgeneration USING btree (processorname, processorversion, ordertype, algorithm, filetype, COALESCE(fileclass, 'DUMMY'::character varying));


--
-- Name: uq_receptionrules01; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE UNIQUE INDEX uq_receptionrules01 ON public.t_receptionrules USING btree (filetype_id, repository_id, user_id, usermode_id) WHERE (active = true);


--
-- Name: uq_receptionrules02; Type: INDEX; Schema: public; Owner: acsdba
--

CREATE UNIQUE INDEX uq_receptionrules02 ON public.t_receptionrules USING btree (filetype_id, localpath, COALESCE(hostname, 'DUMMY_VALUE_USED_FOR_UNIQUE_CONSTRAINT'::character varying), COALESCE((satellite_id)::integer, 0), COALESCE((mission)::integer, 0), COALESCE((sensor_id)::integer, 0)) WHERE (active = true);


--
-- Name: t_inventory tf_inventory_baselines; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tf_inventory_baselines AFTER INSERT ON public.t_inventory FOR EACH ROW EXECUTE FUNCTION public.tf_invxbaselines_insert();


--
-- Name: t_acshsminvrelation tr_acshsminvrelationinsert; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_acshsminvrelationinsert AFTER INSERT ON public.t_acshsminvrelation FOR EACH STATEMENT EXECUTE FUNCTION public.tf_inserteventtableitem('t_acshsminvrelation', 'INS');


--
-- Name: t_acshsminvrelation tr_acshsminvrelationupdate; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_acshsminvrelationupdate AFTER UPDATE ON public.t_acshsminvrelation FOR EACH STATEMENT EXECUTE FUNCTION public.tf_inserteventtableitem('t_acshsminvrelation', 'UPD');


--
-- Name: t_acsltainvrelation tr_acsltainvrelationinsert; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_acsltainvrelationinsert AFTER INSERT ON public.t_acsltainvrelation FOR EACH STATEMENT EXECUTE FUNCTION public.tf_inserteventtableitem('t_acsltainvrelation', 'INS');


--
-- Name: t_acsltainvrelation tr_acsltainvrelationupdate; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_acsltainvrelationupdate AFTER UPDATE ON public.t_acsltainvrelation FOR EACH STATEMENT EXECUTE FUNCTION public.tf_inserteventtableitem('t_acsltainvrelation', 'UPD');


--
-- Name: t_applicationstatus tr_applicationstatusinsert; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_applicationstatusinsert AFTER INSERT ON public.t_applicationstatus FOR EACH STATEMENT EXECUTE FUNCTION public.tf_inserteventtableitem('t_applicationstatus', 'INS');


--
-- Name: t_applicationstatus tr_applicationstatusupdate; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_applicationstatusupdate AFTER UPDATE ON public.t_applicationstatus FOR EACH STATEMENT EXECUTE FUNCTION public.tf_inserteventtableitem('t_applicationstatus', 'UPD');


--
-- Name: t_cartsinv tr_cartsinvinsert; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_cartsinvinsert AFTER INSERT ON public.t_cartsinv FOR EACH ROW EXECUTE FUNCTION public.tf_cartsinvtrmgr('INS');


--
-- Name: t_cartsinv tr_cartsinvupdate; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_cartsinvupdate AFTER UPDATE ON public.t_cartsinv FOR EACH ROW EXECUTE FUNCTION public.tf_cartsinvtrmgr('UPD');


--
-- Name: t_cdp_feederqueue tr_cdp_feederqueue_upd_bfr; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_cdp_feederqueue_upd_bfr BEFORE UPDATE OF status_id ON public.t_cdp_feederqueue FOR EACH ROW EXECUTE FUNCTION public.tf_status_transiction_check();


--
-- Name: t_cdp_notification_queue tr_cdp_notification_queue_upd_bfr; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_cdp_notification_queue_upd_bfr BEFORE UPDATE OF status_id ON public.t_cdp_notification_queue FOR EACH ROW EXECUTE FUNCTION public.tf_status_transiction_check();


--
-- Name: t_circulationpolicies tr_circulationpolicies_ins_bfr; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_circulationpolicies_ins_bfr BEFORE INSERT ON public.t_circulationpolicies FOR EACH ROW EXECUTE FUNCTION public.tf_circ_policies_statcheck();


--
-- Name: t_circulationsqueue tr_circulationsqueue_ins_bfr; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_circulationsqueue_ins_bfr BEFORE INSERT ON public.t_circulationsqueue FOR EACH ROW EXECUTE FUNCTION public.tf_circulationsetprotocol();


--
-- Name: t_circulationsqueue tr_circulationsqueue_insert2; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_circulationsqueue_insert2 AFTER INSERT ON public.t_circulationsqueue FOR EACH STATEMENT EXECUTE FUNCTION public.tf_inserteventtableitem('t_circulationsqueue', 'INS');


--
-- Name: t_circulationsqueue tr_circulationsqueue_update2; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_circulationsqueue_update2 AFTER UPDATE ON public.t_circulationsqueue FOR EACH ROW EXECUTE FUNCTION public.tf_circulationretries();


--
-- Name: t_circulationsqueue tr_circulationsqueue_update3; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_circulationsqueue_update3 AFTER UPDATE OF status_id ON public.t_circulationsqueue FOR EACH ROW EXECUTE FUNCTION public.tf_datastoragequeueupdate();


--
-- Name: t_circulationsqueue tr_circulationsqueue_update4; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_circulationsqueue_update4 AFTER UPDATE ON public.t_circulationsqueue FOR EACH STATEMENT EXECUTE FUNCTION public.tf_inserteventtableitem('t_circulationsqueue', 'UPD');


--
-- Name: t_completed_orders tr_completedordersinsert; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_completedordersinsert AFTER INSERT ON public.t_completed_orders FOR EACH STATEMENT EXECUTE FUNCTION public.tf_inserteventtableitem('t_completed_orders', 'INS');


--
-- Name: t_completed_orders tr_completedordersupdate; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_completedordersupdate AFTER UPDATE ON public.t_completed_orders FOR EACH STATEMENT EXECUTE FUNCTION public.tf_inserteventtableitem('t_completed_orders', 'UPD');


--
-- Name: t_configurationschemas tr_configurationschemasinsert; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_configurationschemasinsert AFTER INSERT ON public.t_configurationschemas FOR EACH STATEMENT EXECUTE FUNCTION public.tf_inserteventtableitem('t_configurationschemas', 'INS');


--
-- Name: t_configurationschemas tr_configurationschemasupdate; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_configurationschemasupdate AFTER UPDATE ON public.t_configurationschemas FOR EACH STATEMENT EXECUTE FUNCTION public.tf_inserteventtableitem('t_configurationschemas', 'UPD');


--
-- Name: t_configurationspaces tr_configurationspacesinsert; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_configurationspacesinsert AFTER INSERT ON public.t_configurationspaces FOR EACH STATEMENT EXECUTE FUNCTION public.tf_inserteventtableitem('t_configurationspaces', 'INS');


--
-- Name: t_configurationspaces tr_configurationspacesupdate; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_configurationspacesupdate AFTER UPDATE ON public.t_configurationspaces FOR EACH STATEMENT EXECUTE FUNCTION public.tf_inserteventtableitem('t_configurationspaces', 'UPD');


--
-- Name: t_configurationstylesheets tr_configurationstylesheetsinsert; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_configurationstylesheetsinsert AFTER INSERT ON public.t_configurationstylesheets FOR EACH STATEMENT EXECUTE FUNCTION public.tf_inserteventtableitem('t_configurationstylesheets', 'INS');


--
-- Name: t_configurationstylesheets tr_configurationstylesheetsupdate; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_configurationstylesheetsupdate AFTER UPDATE ON public.t_configurationstylesheets FOR EACH STATEMENT EXECUTE FUNCTION public.tf_inserteventtableitem('t_configurationstylesheets', 'UPD');


--
-- Name: t_cov_relevancies tr_cov_relevancies_insert; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_cov_relevancies_insert AFTER INSERT ON public.t_cov_relevancies FOR EACH ROW EXECUTE FUNCTION public.tf_cov_relevancies_clean();


--
-- Name: t_cov_rules tr_cov_rules_update; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_cov_rules_update BEFORE UPDATE ON public.t_cov_rules FOR EACH ROW EXECUTE FUNCTION public.tf_cov_rules_last_update();


--
-- Name: t_datastoragequeue tr_datastoragequeue_insert; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_datastoragequeue_insert AFTER INSERT ON public.t_datastoragequeue FOR EACH STATEMENT EXECUTE FUNCTION public.tf_inserteventtableitem('t_datastoragequeue', 'INS');


--
-- Name: t_datastoragequeue tr_datastoragequeue_update1; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_datastoragequeue_update1 BEFORE UPDATE OF status_id ON public.t_datastoragequeue FOR EACH ROW EXECUTE FUNCTION public.tf_datastoragequeuetrans();


--
-- Name: t_datastoragequeue tr_datastoragequeue_update2; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_datastoragequeue_update2 BEFORE UPDATE ON public.t_datastoragequeue FOR EACH STATEMENT EXECUTE FUNCTION public.tf_inserteventtableitem('t_datastoragequeue', 'UPD');


--
-- Name: t_distributionitems tr_distributionitemsdelete; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_distributionitemsdelete AFTER DELETE ON public.t_distributionitems FOR EACH ROW EXECUTE FUNCTION public.tf_distributionitems_manager('DEL');


--
-- Name: t_distributionitems tr_distributionitemsinsert; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_distributionitemsinsert AFTER INSERT ON public.t_distributionitems FOR EACH ROW EXECUTE FUNCTION public.tf_distributionitems_manager('INS');


--
-- Name: t_distributionitems tr_distributionitemsupdate; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_distributionitemsupdate AFTER UPDATE ON public.t_distributionitems FOR EACH ROW EXECUTE FUNCTION public.tf_distributionitems_manager('UPD');


--
-- Name: t_distributionqueue tr_distributionqueue_del_aft; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_distributionqueue_del_aft AFTER DELETE ON public.t_distributionqueue FOR EACH ROW EXECUTE FUNCTION public.tf_distributionqueue_manager('DEL');


--
-- Name: t_distributionqueue tr_distributionqueue_ins_aft; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_distributionqueue_ins_aft AFTER INSERT ON public.t_distributionqueue FOR EACH ROW EXECUTE FUNCTION public.tf_distributionqueue_manager('INS');


--
-- Name: t_distributionqueue tr_distributionqueue_ins_bfr; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_distributionqueue_ins_bfr BEFORE INSERT ON public.t_distributionqueue FOR EACH ROW EXECUTE FUNCTION public.tf_distributionqueue_statcheck('INS');


--
-- Name: t_distributionqueue tr_distributionqueue_upd_aft; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_distributionqueue_upd_aft AFTER UPDATE ON public.t_distributionqueue FOR EACH ROW EXECUTE FUNCTION public.tf_distributionqueue_manager('UPD');


--
-- Name: t_distributionqueue tr_distributionqueue_upd_bfr; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_distributionqueue_upd_bfr BEFORE UPDATE OF status ON public.t_distributionqueue FOR EACH ROW EXECUTE FUNCTION public.tf_distributionqueue_statcheck('UPD');


--
-- Name: t_distributionrules tr_distributionrulesinsert; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_distributionrulesinsert AFTER INSERT ON public.t_distributionrules FOR EACH ROW EXECUTE FUNCTION public.tf_inserteventtableitem('t_distributionrules', 'INS');


--
-- Name: t_distributionrules tr_distributionrulesupdate; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_distributionrulesupdate AFTER UPDATE ON public.t_distributionrules FOR EACH ROW EXECUTE FUNCTION public.tf_inserteventtableitem('t_distributionrules', 'UPD');


--
-- Name: t_dsqueue_x_cirqueue tr_dsqueuexcirqueue_update; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_dsqueuexcirqueue_update BEFORE UPDATE OF globalstatus_id ON public.t_dsqueue_x_cirqueue FOR EACH ROW EXECUTE FUNCTION public.tf_dsqueuexcirqueuetrans();


--
-- Name: t_filetypes tr_filetypesinsert; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_filetypesinsert AFTER INSERT ON public.t_filetypes FOR EACH ROW EXECUTE FUNCTION public.tf_filetype_insert();


--
-- Name: t_filetypes tr_filetypesupdate; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_filetypesupdate AFTER UPDATE ON public.t_filetypes FOR EACH STATEMENT EXECUTE FUNCTION public.tf_inserteventtableitem('t_filetypes', 'UPD');


--
-- Name: t_inv_distributions tr_invdistributionsupdate; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_invdistributionsupdate BEFORE UPDATE ON public.t_inv_distributions FOR EACH ROW EXECUTE FUNCTION public.tf_invdistributionsupdate();


--
-- Name: t_inventory tr_inventory_del_aft; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_inventory_del_aft AFTER DELETE ON public.t_inventory FOR EACH ROW EXECUTE FUNCTION public.tf_inventorydelete();


--
-- Name: t_inventory tr_inventory_ins_aft; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_inventory_ins_aft AFTER INSERT ON public.t_inventory FOR EACH ROW EXECUTE FUNCTION public.tf_inventoryinsevent();


--
-- Name: t_inventory tr_inventory_upd_aft; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_inventory_upd_aft AFTER UPDATE ON public.t_inventory FOR EACH ROW EXECUTE FUNCTION public.tf_inventoryupdevent();


--
-- Name: t_inventorycomments tr_inventorycommentsinsert; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_inventorycommentsinsert AFTER INSERT ON public.t_inventorycomments FOR EACH STATEMENT EXECUTE FUNCTION public.tf_inserteventtableitem('t_inventorycomments', 'INS');


--
-- Name: t_inventorycomments tr_inventorycommentsupdate; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_inventorycommentsupdate AFTER UPDATE ON public.t_inventorycomments FOR EACH STATEMENT EXECUTE FUNCTION public.tf_inserteventtableitem('t_inventorycomments', 'UPD');


--
-- Name: t_inventorygaps tr_inventorygapsinsert; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_inventorygapsinsert AFTER INSERT ON public.t_inventorygaps FOR EACH STATEMENT EXECUTE FUNCTION public.tf_inserteventtableitem('t_inventorygaps', 'INS');


--
-- Name: t_inventorygaps tr_inventorygapsupdate; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_inventorygapsupdate AFTER UPDATE ON public.t_inventorygaps FOR EACH STATEMENT EXECUTE FUNCTION public.tf_inserteventtableitem('t_inventorygaps', 'UPD');


--
-- Name: t_inventorysm tr_inventorysm_ins_bfr; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_inventorysm_ins_bfr BEFORE INSERT ON public.t_inventorysm FOR EACH ROW EXECUTE FUNCTION public.tf_inventorysm_check_invid('INS');


--
-- Name: t_inventorysm tr_inventorysmdelete; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_inventorysmdelete AFTER DELETE ON public.t_inventorysm FOR EACH ROW EXECUTE FUNCTION public.tf_inventorysmmanager('DEL');


--
-- Name: t_inventorysm tr_inventorysminsert; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_inventorysminsert AFTER INSERT ON public.t_inventorysm FOR EACH ROW EXECUTE FUNCTION public.tf_inventorysmmanager('INS');


--
-- Name: t_inventorysm tr_inventorysmupdate; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_inventorysmupdate AFTER UPDATE ON public.t_inventorysm FOR EACH ROW EXECUTE FUNCTION public.tf_inventorysmmanager('UPD');


--
-- Name: t_inventory tr_mpm_products_populate_del; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_mpm_products_populate_del AFTER DELETE ON public.t_inventory FOR EACH ROW EXECUTE FUNCTION public.tf_mpm_products_populate('DEL');


--
-- Name: t_inventory tr_mpm_products_populate_ins; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_mpm_products_populate_ins AFTER INSERT ON public.t_inventory FOR EACH ROW EXECUTE FUNCTION public.tf_mpm_products_populate('INS');

ALTER TABLE public.t_inventory DISABLE TRIGGER tr_mpm_products_populate_ins;


--
-- Name: t_inventory tr_mpm_products_populate_upd; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_mpm_products_populate_upd AFTER UPDATE OF inventorydate ON public.t_inventory FOR EACH ROW EXECUTE FUNCTION public.tf_mpm_products_populate('UPD');


--
-- Name: t_mpm_products tr_mpm_values_del; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_mpm_values_del BEFORE DELETE ON public.t_mpm_products FOR EACH ROW EXECUTE FUNCTION public.tf_mpm_values_manager('DEL');


--
-- Name: t_mpm_values tr_mpm_values_delete; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_mpm_values_delete AFTER INSERT ON public.t_mpm_values FOR EACH ROW EXECUTE FUNCTION public.tf_mpm_values_delete_master();


--
-- Name: t_mpm_values tr_mpm_values_insert; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_mpm_values_insert BEFORE INSERT ON public.t_mpm_values FOR EACH ROW EXECUTE FUNCTION public.tf_mpm_values_partition_mngr();


--
-- Name: t_applicationstatus tr_oid_applicationstatus; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_oid_applicationstatus BEFORE DELETE OR UPDATE ON public.t_applicationstatus FOR EACH ROW EXECUTE FUNCTION public.lo_manage('status');


--
-- Name: t_mpm_values_part1 tr_oid_mpm_values_part1; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_oid_mpm_values_part1 BEFORE DELETE OR UPDATE ON public.t_mpm_values_part1 FOR EACH ROW EXECUTE FUNCTION public.lo_manage('value_data');


--
-- Name: t_mpm_values_part2 tr_oid_mpm_values_part2; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_oid_mpm_values_part2 BEFORE DELETE OR UPDATE ON public.t_mpm_values_part2 FOR EACH ROW EXECUTE FUNCTION public.lo_manage('value_data');


--
-- Name: t_ordersattachments tr_oid_ordersattachments; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_oid_ordersattachments BEFORE DELETE OR UPDATE ON public.t_ordersattachments FOR EACH ROW EXECUTE FUNCTION public.lo_manage('attachment');


--
-- Name: t_smactions tr_oid_smactions; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_oid_smactions BEFORE DELETE OR UPDATE ON public.t_smactions FOR EACH ROW EXECUTE FUNCTION public.lo_manage('parameter');


--
-- Name: t_smqueue tr_oid_smqueue; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_oid_smqueue BEFORE DELETE OR UPDATE ON public.t_smqueue FOR EACH ROW EXECUTE FUNCTION public.lo_manage('parameter');


--
-- Name: t_ordersinput tr_ordersinputinsert; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_ordersinputinsert AFTER INSERT ON public.t_ordersinput FOR EACH STATEMENT EXECUTE FUNCTION public.tf_inserteventtableitem('t_ordersinput', 'INS');


--
-- Name: t_ordersinput tr_ordersinputupdate; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_ordersinputupdate AFTER UPDATE ON public.t_ordersinput FOR EACH STATEMENT EXECUTE FUNCTION public.tf_inserteventtableitem('t_ordersinput', 'UPD');


--
-- Name: t_ordersqueue tr_ordersqueue_del_aft; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_ordersqueue_del_aft AFTER DELETE ON public.t_ordersqueue FOR EACH ROW EXECUTE FUNCTION public.tf_ordersqueue_manager('DEL');


--
-- Name: t_ordersqueue tr_ordersqueue_ins_aft; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_ordersqueue_ins_aft AFTER INSERT ON public.t_ordersqueue FOR EACH ROW EXECUTE FUNCTION public.tf_ordersqueue_manager('INS');


--
-- Name: t_ordersqueue tr_ordersqueue_ins_bfr; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_ordersqueue_ins_bfr BEFORE INSERT ON public.t_ordersqueue FOR EACH ROW EXECUTE FUNCTION public.tf_ordersqueue_statcheck('INS');


--
-- Name: t_ordersqueue tr_ordersqueue_upd_aft; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_ordersqueue_upd_aft AFTER UPDATE ON public.t_ordersqueue FOR EACH ROW EXECUTE FUNCTION public.tf_ordersqueue_manager('UPD');


--
-- Name: t_ordersqueue tr_ordersqueue_upd_bfr; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_ordersqueue_upd_bfr BEFORE UPDATE OF status ON public.t_ordersqueue FOR EACH ROW EXECUTE FUNCTION public.tf_ordersqueue_statcheck('UPD');


--
-- Name: t_orderstatetransitions tr_ordertransinsert; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_ordertransinsert AFTER INSERT ON public.t_orderstatetransitions FOR EACH STATEMENT EXECUTE FUNCTION public.tf_inserteventtableitem('t_orderstatetransitions', 'INS');


--
-- Name: t_orderstatetransitions tr_ordertransupdate; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_ordertransupdate AFTER UPDATE ON public.t_orderstatetransitions FOR EACH STATEMENT EXECUTE FUNCTION public.tf_inserteventtableitem('t_orderstatetransitions', 'UPD');


--
-- Name: t_pendinginventory tr_pendinginvinsert; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_pendinginvinsert AFTER INSERT ON public.t_pendinginventory FOR EACH ROW EXECUTE FUNCTION public.tf_pendinginvinsert();


--
-- Name: t_receptionrules tr_receptionrulesinsert; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_receptionrulesinsert AFTER INSERT ON public.t_receptionrules FOR EACH ROW EXECUTE FUNCTION public.tf_inserteventtableitem('t_receptionrules', 'INS');


--
-- Name: t_receptionrules tr_receptionrulesupdate; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_receptionrulesupdate AFTER UPDATE ON public.t_receptionrules FOR EACH ROW EXECUTE FUNCTION public.tf_inserteventtableitem('t_receptionrules', 'UPD');


--
-- Name: t_repositories tr_repositoriesinsert; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_repositoriesinsert AFTER INSERT ON public.t_repositories FOR EACH ROW EXECUTE FUNCTION public.tf_inserteventtableitem('t_repositories', 'INS');


--
-- Name: t_repositories tr_repositoriesupdate; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_repositoriesupdate AFTER UPDATE ON public.t_repositories FOR EACH ROW EXECUTE FUNCTION public.tf_inserteventtableitem('t_repositories', 'UPD');


--
-- Name: t_smactions tr_smactions_upd_aft; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_smactions_upd_aft AFTER UPDATE ON public.t_smactions FOR EACH ROW EXECUTE FUNCTION public.tf_smactions_upd_aft();


--
-- Name: t_smactions tr_smactions_upd_bfr; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_smactions_upd_bfr BEFORE UPDATE ON public.t_smactions FOR EACH ROW EXECUTE FUNCTION public.tf_smactions_upd_bfr();


--
-- Name: t_smactions tr_smactionsinsert; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_smactionsinsert AFTER INSERT ON public.t_smactions FOR EACH ROW EXECUTE FUNCTION public.tf_smactionsinsert();


--
-- Name: t_smqueue tr_smqueue_del_aft; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_smqueue_del_aft AFTER DELETE ON public.t_smqueue FOR EACH ROW EXECUTE FUNCTION public.tf_inserteventtableitem('t_smqueue', 'DEL');


--
-- Name: t_smqueue tr_smqueue_ins_aft; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_smqueue_ins_aft AFTER INSERT ON public.t_smqueue FOR EACH ROW EXECUTE FUNCTION public.tf_inserteventtableitem('t_smqueue', 'INS');


--
-- Name: t_smqueue tr_smqueue_ins_bfr; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_smqueue_ins_bfr BEFORE INSERT ON public.t_smqueue FOR EACH ROW EXECUTE FUNCTION public.tf_smqueue_checkchanges('INS');


--
-- Name: t_smqueue tr_smqueue_upd_aft; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_smqueue_upd_aft AFTER UPDATE ON public.t_smqueue FOR EACH ROW EXECUTE FUNCTION public.tf_inserteventtableitem('t_smqueue', 'UPD');


--
-- Name: t_smqueue tr_smqueue_upd_bfr; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_smqueue_upd_bfr BEFORE UPDATE OF status ON public.t_smqueue FOR EACH ROW EXECUTE FUNCTION public.tf_smqueue_checkchanges('UPD');


--
-- Name: t_temporarydistribution tr_temporarydistributioninsert; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_temporarydistributioninsert AFTER INSERT ON public.t_temporarydistribution FOR EACH STATEMENT EXECUTE FUNCTION public.tf_inserteventtableitem('t_temporarydistribution', 'INS');


--
-- Name: t_temporarydistribution tr_temporarydistributionupdate; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_temporarydistributionupdate AFTER UPDATE ON public.t_temporarydistribution FOR EACH STATEMENT EXECUTE FUNCTION public.tf_inserteventtableitem('t_temporarydistribution', 'UPD');


--
-- Name: t_temporaryorders tr_temporaryordersinsert; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_temporaryordersinsert AFTER INSERT ON public.t_temporaryorders FOR EACH STATEMENT EXECUTE FUNCTION public.tf_inserteventtableitem('t_temporaryorders', 'INS');


--
-- Name: t_temporaryorders tr_temporaryordersupdate; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_temporaryordersupdate AFTER UPDATE ON public.t_temporaryorders FOR EACH STATEMENT EXECUTE FUNCTION public.tf_inserteventtableitem('t_temporaryorders', 'UPD');


--
-- Name: t_users tr_usersinsert; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_usersinsert AFTER INSERT ON public.t_users FOR EACH STATEMENT EXECUTE FUNCTION public.tf_inserteventtableitem('t_users', 'INS');


--
-- Name: t_users tr_usersupdate; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_usersupdate AFTER UPDATE ON public.t_users FOR EACH STATEMENT EXECUTE FUNCTION public.tf_inserteventtableitem('t_users', 'UPD');


--
-- Name: t_waitingorders tr_waitingordersinsert; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_waitingordersinsert AFTER INSERT ON public.t_waitingorders FOR EACH ROW EXECUTE FUNCTION public.tf_waitingordersinsert('ORDWAIT');


--
-- Name: t_waitingorders tr_waitingordersupdate; Type: TRIGGER; Schema: public; Owner: acsdba
--

CREATE TRIGGER tr_waitingordersupdate AFTER UPDATE ON public.t_waitingorders FOR EACH ROW EXECUTE FUNCTION public.tf_inserteventtableitem('t_waitingorders', 'UPD');


--
-- Name: t_acsltainvrelation fk_acsltainvrelation1; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_acsltainvrelation
    ADD CONSTRAINT fk_acsltainvrelation1 FOREIGN KEY (id) REFERENCES public.t_inventorysm(id) ON DELETE CASCADE;


--
-- Name: t_applicationevents fk_applicationevents0l; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_applicationevents
    ADD CONSTRAINT fk_applicationevents0l FOREIGN KEY (application_id) REFERENCES public.t_applications(id_application) ON DELETE CASCADE;


--
-- Name: t_blockedcirculationsqueue fk_blockedcirculationsqueue01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_blockedcirculationsqueue
    ADD CONSTRAINT fk_blockedcirculationsqueue01 FOREIGN KEY (status_id) REFERENCES public.t_stati(id) ON DELETE CASCADE;


--
-- Name: t_blockedcirculationsqueue fk_blockedcirculationsqueue02; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_blockedcirculationsqueue
    ADD CONSTRAINT fk_blockedcirculationsqueue02 FOREIGN KEY (circulationmode_id) REFERENCES public.t_circulationmodes(id) ON DELETE CASCADE;


--
-- Name: t_blockedcirculationsqueue fk_blockedcirculationsqueue03; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_blockedcirculationsqueue
    ADD CONSTRAINT fk_blockedcirculationsqueue03 FOREIGN KEY (relay_host_id) REFERENCES public.t_securityzonerelays(id) ON DELETE CASCADE;


--
-- Name: t_blockedcirculationsqueue fk_blockedcirculationsqueue04; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_blockedcirculationsqueue
    ADD CONSTRAINT fk_blockedcirculationsqueue04 FOREIGN KEY (host_id) REFERENCES public.t_knownhosts(id) ON DELETE CASCADE;


--
-- Name: t_blockedcirculationsqueue fk_blockedcirculationsqueue05; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_blockedcirculationsqueue
    ADD CONSTRAINT fk_blockedcirculationsqueue05 FOREIGN KEY (circulationpolicy_id) REFERENCES public.t_circulationpolicies(id) ON DELETE SET NULL;


--
-- Name: t_blockedcirculationsqueue fk_blockedcirculationsqueue06; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_blockedcirculationsqueue
    ADD CONSTRAINT fk_blockedcirculationsqueue06 FOREIGN KEY (dest_host_id) REFERENCES public.t_knownhosts(id) ON DELETE CASCADE;


--
-- Name: t_blockedcirculationsqueue fk_blockedcirculationsqueue07; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_blockedcirculationsqueue
    ADD CONSTRAINT fk_blockedcirculationsqueue07 FOREIGN KEY (tempname_id) REFERENCES public.t_tmpnamestrategy(id_tmpnamestrategy) ON DELETE CASCADE;


--
-- Name: t_blockedcirculationsqueue fk_blockedcirculationsqueue08; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_blockedcirculationsqueue
    ADD CONSTRAINT fk_blockedcirculationsqueue08 FOREIGN KEY (dest_protocol_id) REFERENCES public.t_transferprotocols(id) ON DELETE CASCADE;


--
-- Name: t_blockedcirculationsqueue fk_blockedcirculationsqueue09; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_blockedcirculationsqueue
    ADD CONSTRAINT fk_blockedcirculationsqueue09 FOREIGN KEY (media_circulation_id) REFERENCES public.t_circulationsmedia(id) ON DELETE CASCADE;


--
-- Name: t_blockedcirculationsqueue fk_blockedcirculationsqueue10; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_blockedcirculationsqueue
    ADD CONSTRAINT fk_blockedcirculationsqueue10 FOREIGN KEY (media_insertionmode_id) REFERENCES public.t_circulationsmedia_insmode(id) ON DELETE CASCADE;


--
-- Name: t_blockedcirculationsqueue fk_blockedcirculationsqueue11; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_blockedcirculationsqueue
    ADD CONSTRAINT fk_blockedcirculationsqueue11 FOREIGN KEY (localtransfermode_id) REFERENCES public.t_localtransfermodes(id) ON DELETE CASCADE;


--
-- Name: t_cartsinv fk_cartsinv_cart; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_cartsinv
    ADD CONSTRAINT fk_cartsinv_cart FOREIGN KEY (cart) REFERENCES public.t_cartsref(id) ON DELETE CASCADE;


--
-- Name: t_cartsinv fk_cartsinv_fnamefver; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_cartsinv
    ADD CONSTRAINT fk_cartsinv_fnamefver FOREIGN KEY (filename, fileversion) REFERENCES public.t_inventory(filename, fileversion) ON DELETE CASCADE;


--
-- Name: t_cdp_feederqueue fk_cdp_feederqueue01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_cdp_feederqueue
    ADD CONSTRAINT fk_cdp_feederqueue01 FOREIGN KEY (status_id) REFERENCES public.t_stati(id) ON DELETE CASCADE;


--
-- Name: t_cdp_feederqueue fk_cdp_feederqueue02; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_cdp_feederqueue
    ADD CONSTRAINT fk_cdp_feederqueue02 FOREIGN KEY (tempnamestrategy_id) REFERENCES public.t_tmpnamestrategy(id_tmpnamestrategy) ON DELETE CASCADE;


--
-- Name: t_cdp_notification_queue fk_cdp_notification_queue01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_cdp_notification_queue
    ADD CONSTRAINT fk_cdp_notification_queue01 FOREIGN KEY (feederqueue_id) REFERENCES public.t_cdp_feederqueue(id) ON DELETE CASCADE;


--
-- Name: t_cdp_notification_queue fk_cdp_notification_queue02; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_cdp_notification_queue
    ADD CONSTRAINT fk_cdp_notification_queue02 FOREIGN KEY (status_id) REFERENCES public.t_stati(id) ON DELETE CASCADE;


--
-- Name: t_circ_callbacks fk_circ_callbacks01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circ_callbacks
    ADD CONSTRAINT fk_circ_callbacks01 FOREIGN KEY (circ_queue_id) REFERENCES public.t_circulationsqueue(id) ON DELETE CASCADE;


--
-- Name: t_circulation_remove_queue fk_circulation_remove_queue01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulation_remove_queue
    ADD CONSTRAINT fk_circulation_remove_queue01 FOREIGN KEY (host_id) REFERENCES public.t_knownhosts(id) ON DELETE CASCADE;


--
-- Name: t_circulationactivehosts fk_circulationactivehosts01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationactivehosts
    ADD CONSTRAINT fk_circulationactivehosts01 FOREIGN KEY (host_id) REFERENCES public.t_knownhosts(id) ON DELETE CASCADE;


--
-- Name: t_circulationpolicies fk_circulationpolicies01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationpolicies
    ADD CONSTRAINT fk_circulationpolicies01 FOREIGN KEY (circulator_node_id) REFERENCES public.t_knownhosts(id) ON DELETE CASCADE;


--
-- Name: t_circulationpolicies fk_circulationpolicies02; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationpolicies
    ADD CONSTRAINT fk_circulationpolicies02 FOREIGN KEY (filetype_id) REFERENCES public.t_filetypes(id_filetype) ON DELETE CASCADE;


--
-- Name: t_circulationpolicies fk_circulationpolicies03; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationpolicies
    ADD CONSTRAINT fk_circulationpolicies03 FOREIGN KEY (source_id) REFERENCES public.t_repositories(id_repository) ON DELETE CASCADE;


--
-- Name: t_circulationpolicies fk_circulationpolicies04; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationpolicies
    ADD CONSTRAINT fk_circulationpolicies04 FOREIGN KEY (destination_id) REFERENCES public.t_repositories(id_repository) ON DELETE CASCADE;


--
-- Name: t_circulationpolicies fk_circulationpolicies05; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationpolicies
    ADD CONSTRAINT fk_circulationpolicies05 FOREIGN KEY (agent_node_id) REFERENCES public.t_knownhosts(id) ON DELETE CASCADE;


--
-- Name: t_circulationpolicies fk_circulationpolicies06; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationpolicies
    ADD CONSTRAINT fk_circulationpolicies06 FOREIGN KEY (localtransfermode_id) REFERENCES public.t_localtransfermodes(id) ON DELETE CASCADE;


--
-- Name: t_circulationpolicies fk_circulationpolicies07; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationpolicies
    ADD CONSTRAINT fk_circulationpolicies07 FOREIGN KEY (status_id) REFERENCES public.t_stati(id) ON DELETE CASCADE;


--
-- Name: t_circulationsmedia fk_circulationsmedia01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationsmedia
    ADD CONSTRAINT fk_circulationsmedia01 FOREIGN KEY (status_id) REFERENCES public.t_stati(id) ON DELETE CASCADE;


--
-- Name: t_circulationsqueue fk_circulationsqueue01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationsqueue
    ADD CONSTRAINT fk_circulationsqueue01 FOREIGN KEY (status_id) REFERENCES public.t_stati(id) ON DELETE CASCADE;


--
-- Name: t_circulationsqueue fk_circulationsqueue02; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationsqueue
    ADD CONSTRAINT fk_circulationsqueue02 FOREIGN KEY (circulationmode_id) REFERENCES public.t_circulationmodes(id) ON DELETE CASCADE;


--
-- Name: t_circulationsqueue fk_circulationsqueue03; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationsqueue
    ADD CONSTRAINT fk_circulationsqueue03 FOREIGN KEY (relay_host_id) REFERENCES public.t_securityzonerelays(id) ON DELETE CASCADE;


--
-- Name: t_circulationsqueue fk_circulationsqueue04; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationsqueue
    ADD CONSTRAINT fk_circulationsqueue04 FOREIGN KEY (host_id) REFERENCES public.t_knownhosts(id) ON DELETE CASCADE;


--
-- Name: t_circulationsqueue fk_circulationsqueue05; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationsqueue
    ADD CONSTRAINT fk_circulationsqueue05 FOREIGN KEY (circulationpolicy_id) REFERENCES public.t_circulationpolicies(id) ON DELETE SET NULL;


--
-- Name: t_circulationsqueue fk_circulationsqueue06; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationsqueue
    ADD CONSTRAINT fk_circulationsqueue06 FOREIGN KEY (dest_host_id) REFERENCES public.t_knownhosts(id) ON DELETE CASCADE;


--
-- Name: t_circulationsqueue fk_circulationsqueue07; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationsqueue
    ADD CONSTRAINT fk_circulationsqueue07 FOREIGN KEY (tempname_id) REFERENCES public.t_tmpnamestrategy(id_tmpnamestrategy) ON DELETE CASCADE;


--
-- Name: t_circulationsqueue fk_circulationsqueue08; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationsqueue
    ADD CONSTRAINT fk_circulationsqueue08 FOREIGN KEY (dest_protocol_id) REFERENCES public.t_transferprotocols(id) ON DELETE CASCADE;


--
-- Name: t_circulationsqueue fk_circulationsqueue09; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationsqueue
    ADD CONSTRAINT fk_circulationsqueue09 FOREIGN KEY (media_circulation_id) REFERENCES public.t_circulationsmedia(id) ON DELETE CASCADE;


--
-- Name: t_circulationsqueue fk_circulationsqueue10; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationsqueue
    ADD CONSTRAINT fk_circulationsqueue10 FOREIGN KEY (media_insertionmode_id) REFERENCES public.t_circulationsmedia_insmode(id) ON DELETE CASCADE;


--
-- Name: t_circulationsqueue fk_circulationsqueue11; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationsqueue
    ADD CONSTRAINT fk_circulationsqueue11 FOREIGN KEY (localtransfermode_id) REFERENCES public.t_localtransfermodes(id) ON DELETE CASCADE;


--
-- Name: t_circulationsqueue fk_circulationsqueue12; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationsqueue
    ADD CONSTRAINT fk_circulationsqueue12 FOREIGN KEY (streamer_id) REFERENCES public.t_streamers(id) ON DELETE CASCADE;


--
-- Name: t_circulationstats fk_circulationstats01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationstats
    ADD CONSTRAINT fk_circulationstats01 FOREIGN KEY (circulationsqueue_id) REFERENCES public.t_circulationsqueue(id) ON DELETE CASCADE;


--
-- Name: t_circulationstats fk_circulationstats02; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationstats
    ADD CONSTRAINT fk_circulationstats02 FOREIGN KEY (from_host_id) REFERENCES public.t_knownhosts(id) ON DELETE CASCADE;


--
-- Name: t_circulationstats fk_circulationstats03; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationstats
    ADD CONSTRAINT fk_circulationstats03 FOREIGN KEY (to_host_id) REFERENCES public.t_knownhosts(id) ON DELETE CASCADE;


--
-- Name: t_cleanuprules fk_cleanuprules1; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_cleanuprules
    ADD CONSTRAINT fk_cleanuprules1 FOREIGN KEY (repository_id) REFERENCES public.t_repositories(id_repository) ON DELETE CASCADE;


--
-- Name: t_cleanuprules fk_cleanuprules2; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_cleanuprules
    ADD CONSTRAINT fk_cleanuprules2 FOREIGN KEY (cleanupalgo_id) REFERENCES public.t_cleanupalgos(id_cleanupalgo) ON DELETE CASCADE;


--
-- Name: t_cov_periods fk_cov_periods01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_cov_periods
    ADD CONSTRAINT fk_cov_periods01 FOREIGN KEY (coveragerule_id) REFERENCES public.t_cov_rules(id_coveragerule) ON DELETE CASCADE;


--
-- Name: t_cov_relevancies fk_cov_relevancies01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_cov_relevancies
    ADD CONSTRAINT fk_cov_relevancies01 FOREIGN KEY (period_id) REFERENCES public.t_cov_periods(id_interval) ON DELETE CASCADE;


--
-- Name: t_cov_relevancies fk_cov_relevancies02; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_cov_relevancies
    ADD CONSTRAINT fk_cov_relevancies02 FOREIGN KEY (inv_id) REFERENCES public.t_inventory(id_inv) ON DELETE CASCADE;


--
-- Name: t_cov_x_filetypes fk_coverage_x_filetypes01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_cov_x_filetypes
    ADD CONSTRAINT fk_coverage_x_filetypes01 FOREIGN KEY (coveragerule_id) REFERENCES public.t_cov_rules(id_coveragerule) ON DELETE CASCADE;


--
-- Name: t_cov_x_filetypes fk_coverage_x_filetypes02; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_cov_x_filetypes
    ADD CONSTRAINT fk_coverage_x_filetypes02 FOREIGN KEY (filetype_id) REFERENCES public.t_filetypes(id_filetype) ON DELETE CASCADE;


--
-- Name: t_cr_x_filetypes fk_cr_x_filetypes01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_cr_x_filetypes
    ADD CONSTRAINT fk_cr_x_filetypes01 FOREIGN KEY (crgroup_id) REFERENCES public.t_crgroups(id_crgroup) ON DELETE CASCADE;


--
-- Name: t_cr_x_filetypes fk_cr_x_filetypes02; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_cr_x_filetypes
    ADD CONSTRAINT fk_cr_x_filetypes02 FOREIGN KEY (filetype_id) REFERENCES public.t_filetypes(id_filetype) ON DELETE CASCADE;


--
-- Name: t_crgroups fk_crgroups01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_crgroups
    ADD CONSTRAINT fk_crgroups01 FOREIGN KEY (virtual_filetype_id) REFERENCES public.t_filetypes(id_filetype) ON DELETE CASCADE;


--
-- Name: t_crrelations fk_crrelations01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_crrelations
    ADD CONSTRAINT fk_crrelations01 FOREIGN KEY (crgroup_id) REFERENCES public.t_crgroups(id_crgroup) ON DELETE CASCADE;


--
-- Name: t_crrelations fk_crrelations02; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_crrelations
    ADD CONSTRAINT fk_crrelations02 FOREIGN KEY (repository_id) REFERENCES public.t_repositories(id_repository) ON DELETE CASCADE;


--
-- Name: t_crrelations fk_crrelations03; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_crrelations
    ADD CONSTRAINT fk_crrelations03 FOREIGN KEY (distributionrule_id) REFERENCES public.t_distributionrules(id_distributionrule) ON DELETE CASCADE;


--
-- Name: t_crtemporaries fk_crtemporaries01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_crtemporaries
    ADD CONSTRAINT fk_crtemporaries01 FOREIGN KEY (inventory_id) REFERENCES public.t_inventory(id_inv) ON DELETE CASCADE;


--
-- Name: t_crtemporaries fk_crtemporaries02; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_crtemporaries
    ADD CONSTRAINT fk_crtemporaries02 FOREIGN KEY (crgroup_id) REFERENCES public.t_crgroups(id_crgroup) ON DELETE CASCADE;


--
-- Name: t_datastoragequeue fk_datastoragequeue02; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_datastoragequeue
    ADD CONSTRAINT fk_datastoragequeue02 FOREIGN KEY (status_id) REFERENCES public.t_stati(id) ON DELETE CASCADE;


--
-- Name: t_datastoragequeue fk_datastoragequeue03; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_datastoragequeue
    ADD CONSTRAINT fk_datastoragequeue03 FOREIGN KEY (type_id) REFERENCES public.t_datastoragetypes(id) ON DELETE CASCADE;


--
-- Name: t_dbparameters fk_dbparameters01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_dbparameters
    ADD CONSTRAINT fk_dbparameters01 FOREIGN KEY (parametertype_id) REFERENCES public.t_dbparametertypes(id) ON DELETE CASCADE;


--
-- Name: t_distributionqueue fk_distribution_to_circulation; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_distributionqueue
    ADD CONSTRAINT fk_distribution_to_circulation FOREIGN KEY (circulationqueue_id) REFERENCES public.t_circulationsqueue(id) ON DELETE CASCADE;


--
-- Name: t_distributionrules fk_distributionrules01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_distributionrules
    ADD CONSTRAINT fk_distributionrules01 FOREIGN KEY (filetype_id) REFERENCES public.t_filetypes(id_filetype) ON DELETE CASCADE;


--
-- Name: t_distributionrules fk_distributionrules02; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_distributionrules
    ADD CONSTRAINT fk_distributionrules02 FOREIGN KEY (mediatype_id) REFERENCES public.t_mediatypes(id_mediatype) ON DELETE CASCADE;


--
-- Name: t_distributionrules fk_distributionrules03; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_distributionrules
    ADD CONSTRAINT fk_distributionrules03 FOREIGN KEY (repository_id) REFERENCES public.t_repositories(id_repository) ON DELETE CASCADE;


--
-- Name: t_distributionrules fk_distributionrules04; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_distributionrules
    ADD CONSTRAINT fk_distributionrules04 FOREIGN KEY (user_id) REFERENCES public.t_users(id_user) ON DELETE CASCADE;


--
-- Name: t_distributionrules fk_distributionrules05; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_distributionrules
    ADD CONSTRAINT fk_distributionrules05 FOREIGN KEY (usermode_id) REFERENCES public.t_usermodes(id) ON DELETE CASCADE;


--
-- Name: t_distributionrules fk_distributionrules06; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_distributionrules
    ADD CONSTRAINT fk_distributionrules06 FOREIGN KEY (distributionalgo_id) REFERENCES public.t_distributionalgo(id) ON DELETE CASCADE;


--
-- Name: t_distributionrules fk_distributionrules07; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_distributionrules
    ADD CONSTRAINT fk_distributionrules07 FOREIGN KEY (format_tt_id) REFERENCES public.t_tasktables(id_tasktable) ON DELETE CASCADE;


--
-- Name: t_distributionrules fk_distributionrules08; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_distributionrules
    ADD CONSTRAINT fk_distributionrules08 FOREIGN KEY (delivery_tt_id) REFERENCES public.t_tasktables(id_tasktable) ON DELETE CASCADE;


--
-- Name: t_distributionrules fk_distributionrules09; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_distributionrules
    ADD CONSTRAINT fk_distributionrules09 FOREIGN KEY (delcheck_tt_id) REFERENCES public.t_tasktables(id_tasktable) ON DELETE CASCADE;


--
-- Name: t_distributionrules fk_distributionrules10; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_distributionrules
    ADD CONSTRAINT fk_distributionrules10 FOREIGN KEY (fileclass_id) REFERENCES public.t_fileclasses(id_fileclass) ON DELETE CASCADE;


--
-- Name: t_distributionrules fk_distributionrules11; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_distributionrules
    ADD CONSTRAINT fk_distributionrules11 FOREIGN KEY (disseminationpackingalgo_id) REFERENCES public.t_disseminationpackingalgos(id_disseminationpackingalgo) ON DELETE CASCADE;


--
-- Name: t_distributionwaitingitems fk_distributionwaitingitems01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_distributionwaitingitems
    ADD CONSTRAINT fk_distributionwaitingitems01 FOREIGN KEY (distributionitem_id) REFERENCES public.t_distributionitems(id) ON DELETE CASCADE;


--
-- Name: t_distributionwaitingitems fk_distributionwaitingitems02; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_distributionwaitingitems
    ADD CONSTRAINT fk_distributionwaitingitems02 FOREIGN KEY (package_id) REFERENCES public.t_distributionqueue(packageid) ON DELETE CASCADE;


--
-- Name: t_distributionwaitingitems fk_distributionwaitingitems03; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_distributionwaitingitems
    ADD CONSTRAINT fk_distributionwaitingitems03 FOREIGN KEY (inventory_id) REFERENCES public.t_inventory(id_inv) ON DELETE CASCADE;


--
-- Name: t_dsqueue_x_cirqueue fk_dsqueue_x_cirqueue01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_dsqueue_x_cirqueue
    ADD CONSTRAINT fk_dsqueue_x_cirqueue01 FOREIGN KEY (datastoragequeue_id) REFERENCES public.t_datastoragequeue(id) ON DELETE CASCADE;


--
-- Name: t_dsqueue_x_cirqueue fk_dsqueue_x_cirqueue03; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_dsqueue_x_cirqueue
    ADD CONSTRAINT fk_dsqueue_x_cirqueue03 FOREIGN KEY (globalstatus_id) REFERENCES public.t_stati(id) ON DELETE CASCADE;


--
-- Name: t_dsqueue_x_cirqueue fk_dsqueue_x_cirqueue04; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_dsqueue_x_cirqueue
    ADD CONSTRAINT fk_dsqueue_x_cirqueue04 FOREIGN KEY (circ_status_id) REFERENCES public.t_stati(id) ON DELETE CASCADE;


--
-- Name: t_dsqueue_x_cirqueue fk_dsqueue_x_cirqueue05; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_dsqueue_x_cirqueue
    ADD CONSTRAINT fk_dsqueue_x_cirqueue05 FOREIGN KEY (localtransfermode_id) REFERENCES public.t_localtransfermodes(id) ON DELETE CASCADE;


--
-- Name: t_emitted_order_events fk_emitted_order_events01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_emitted_order_events
    ADD CONSTRAINT fk_emitted_order_events01 FOREIGN KEY (processor_id) REFERENCES public.t_processors(id_processor) ON DELETE CASCADE;


--
-- Name: t_emitted_order_events fk_emitted_order_events02; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_emitted_order_events
    ADD CONSTRAINT fk_emitted_order_events02 FOREIGN KEY (order_type) REFERENCES public.t_ordertypes(ordertype) ON DELETE CASCADE;


--
-- Name: t_filetypes_last_inv fk_filetypes_last_inv01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_filetypes_last_inv
    ADD CONSTRAINT fk_filetypes_last_inv01 FOREIGN KEY (filetype) REFERENCES public.t_filetypes(filetype) ON DELETE CASCADE;


--
-- Name: t_filetypes_x_stylesheets fk_filetypes_x_stylesheets01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_filetypes_x_stylesheets
    ADD CONSTRAINT fk_filetypes_x_stylesheets01 FOREIGN KEY (filetype_id) REFERENCES public.t_filetypes(id_filetype) ON DELETE CASCADE;


--
-- Name: t_filetypes_x_stylesheets fk_filetypes_x_stylesheets02; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_filetypes_x_stylesheets
    ADD CONSTRAINT fk_filetypes_x_stylesheets02 FOREIGN KEY (stylesheet_id) REFERENCES public.t_configurationstylesheets(id) ON DELETE CASCADE;


--
-- Name: t_filetypes_x_stylesheets fk_filetypes_x_stylesheets03; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_filetypes_x_stylesheets
    ADD CONSTRAINT fk_filetypes_x_stylesheets03 FOREIGN KEY (outputtype_id) REFERENCES public.t_stylesheetoutputtypes(id) ON DELETE CASCADE;


--
-- Name: t_filetypescomps fk_filetypescomps01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_filetypescomps
    ADD CONSTRAINT fk_filetypescomps01 FOREIGN KEY (pfiletype) REFERENCES public.t_filetypes(id_filetype) ON DELETE CASCADE;


--
-- Name: t_filetypescomps fk_filetypescomps02; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_filetypescomps
    ADD CONSTRAINT fk_filetypescomps02 FOREIGN KEY (cfiletype) REFERENCES public.t_filetypes(id_filetype) ON DELETE CASCADE;


--
-- Name: t_filetypes_x_migrations fk_filetypesxmigrations1; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_filetypes_x_migrations
    ADD CONSTRAINT fk_filetypesxmigrations1 FOREIGN KEY (sm_id) REFERENCES public.t_storagemanager(id) ON DELETE CASCADE;


--
-- Name: t_filetypes_x_migrations fk_filetypesxmigrations2; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_filetypes_x_migrations
    ADD CONSTRAINT fk_filetypesxmigrations2 FOREIGN KEY (filetype_id) REFERENCES public.t_filetypes(id_filetype) ON DELETE CASCADE;


--
-- Name: t_filetypes_x_migrations fk_filetypesxmigrations3; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_filetypes_x_migrations
    ADD CONSTRAINT fk_filetypesxmigrations3 FOREIGN KEY (migrationalgo_id) REFERENCES public.t_migrationalgos(id_migrationalgo) ON DELETE CASCADE;


--
-- Name: t_usdfwa fk_fname_fver; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_usdfwa
    ADD CONSTRAINT fk_fname_fver FOREIGN KEY (filename, fileversion) REFERENCES public.t_inventory(filename, fileversion) ON DELETE CASCADE;


--
-- Name: t_inv_additional_params fk_inv_additional_params01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inv_additional_params
    ADD CONSTRAINT fk_inv_additional_params01 FOREIGN KEY (inv_id) REFERENCES public.t_inventory(id_inv) ON DELETE CASCADE;


--
-- Name: t_inv_additional_params fk_inv_additional_params02; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inv_additional_params
    ADD CONSTRAINT fk_inv_additional_params02 FOREIGN KEY (timeliness_key_id) REFERENCES public.t_downlink_session_tracks(id) ON DELETE CASCADE;


--
-- Name: t_inv_data_restored fk_inv_data_restored01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inv_data_restored
    ADD CONSTRAINT fk_inv_data_restored01 FOREIGN KEY (id) REFERENCES public.t_inventorysm(id) ON DELETE CASCADE;


--
-- Name: t_inv_data_restored fk_inv_data_restored02; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inv_data_restored
    ADD CONSTRAINT fk_inv_data_restored02 FOREIGN KEY (sm_id) REFERENCES public.t_storagemanager(id) ON DELETE CASCADE;


--
-- Name: t_inv_data_restored fk_inv_data_restored03; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inv_data_restored
    ADD CONSTRAINT fk_inv_data_restored03 FOREIGN KEY (inv_id) REFERENCES public.t_inventory(id_inv) ON DELETE CASCADE;


--
-- Name: t_inv_data_restored fk_inv_data_restored04; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inv_data_restored
    ADD CONSTRAINT fk_inv_data_restored04 FOREIGN KEY (parent_inv_id) REFERENCES public.t_inventory(id_inv) ON DELETE CASCADE;


--
-- Name: t_inv_data_restored fk_inv_data_restored05; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inv_data_restored
    ADD CONSTRAINT fk_inv_data_restored05 FOREIGN KEY (filetype_id) REFERENCES public.t_filetypes(id_filetype) ON DELETE CASCADE;


--
-- Name: t_inv_distributions fk_inv_distributions01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inv_distributions
    ADD CONSTRAINT fk_inv_distributions01 FOREIGN KEY (inv_id) REFERENCES public.t_inventory(id_inv) ON DELETE CASCADE;


--
-- Name: t_inv_downloads fk_inv_downloads01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inv_downloads
    ADD CONSTRAINT fk_inv_downloads01 FOREIGN KEY (inv_id) REFERENCES public.t_inventory(id_inv) ON DELETE CASCADE;


--
-- Name: t_inv_downloads fk_inv_downloads02; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inv_downloads
    ADD CONSTRAINT fk_inv_downloads02 FOREIGN KEY (client_id) REFERENCES public.t_inv_client(id_inv_client) ON DELETE CASCADE;


--
-- Name: t_inv_relations fk_inv_relations01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inv_relations
    ADD CONSTRAINT fk_inv_relations01 FOREIGN KEY (inv_id1) REFERENCES public.t_inventory(id_inv) ON DELETE CASCADE;


--
-- Name: t_inv_relations fk_inv_relations02; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inv_relations
    ADD CONSTRAINT fk_inv_relations02 FOREIGN KEY (inv_id2) REFERENCES public.t_inventory(id_inv) ON DELETE SET NULL;


--
-- Name: t_inv_relations fk_inv_relations03; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inv_relations
    ADD CONSTRAINT fk_inv_relations03 FOREIGN KEY (rel_type_id) REFERENCES public.t_inv_relation_types(id) ON DELETE CASCADE;


--
-- Name: t_inv_x_baselines fk_inv_x_baselines01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inv_x_baselines
    ADD CONSTRAINT fk_inv_x_baselines01 FOREIGN KEY (inv_id) REFERENCES public.t_inventory(id_inv) ON DELETE CASCADE;


--
-- Name: t_inv_x_baselines fk_inv_x_baselines02; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inv_x_baselines
    ADD CONSTRAINT fk_inv_x_baselines02 FOREIGN KEY (inv_baseline_id) REFERENCES public.t_inv_baselines(id_inv_baseline) ON DELETE CASCADE;


--
-- Name: t_inventory fk_inventory7; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventory
    ADD CONSTRAINT fk_inventory7 FOREIGN KEY (qualified) REFERENCES public.t_qualifiedflag(id) ON DELETE CASCADE;


--
-- Name: t_inventory fk_inventory8; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventory
    ADD CONSTRAINT fk_inventory8 FOREIGN KEY (band) REFERENCES public.t_bands(id) ON DELETE CASCADE;


--
-- Name: t_inventory fk_inventory9; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventory
    ADD CONSTRAINT fk_inventory9 FOREIGN KEY (originator) REFERENCES public.t_originators(id);


--
-- Name: t_inventoryanomalies fk_inventoryanomalies01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventoryanomalies
    ADD CONSTRAINT fk_inventoryanomalies01 FOREIGN KEY (anomaly_type_id) REFERENCES public.t_anomaly_types(id) ON DELETE CASCADE;


--
-- Name: t_inventorylinksconf fk_invlinkconf_ftype1; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventorylinksconf
    ADD CONSTRAINT fk_invlinkconf_ftype1 FOREIGN KEY (orig_filetype) REFERENCES public.t_filetypes(filetype) ON DELETE CASCADE;


--
-- Name: t_inventorylinksconf fk_invlinkconf_ftype2; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventorylinksconf
    ADD CONSTRAINT fk_invlinkconf_ftype2 FOREIGN KEY (dest_filetype) REFERENCES public.t_filetypes(filetype) ON DELETE CASCADE;


--
-- Name: t_inventorylinks fk_invlinks_dest; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventorylinks
    ADD CONSTRAINT fk_invlinks_dest FOREIGN KEY (dest_filename, dest_fileversion) REFERENCES public.t_inventory(filename, fileversion) ON DELETE CASCADE;


--
-- Name: t_inventorylinks fk_invlinks_orig; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventorylinks
    ADD CONSTRAINT fk_invlinks_orig FOREIGN KEY (orig_filename, orig_fileversion) REFERENCES public.t_inventory(filename, fileversion) ON DELETE CASCADE;


--
-- Name: t_invrollingpolicies fk_invrollpol_storage; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_invrollingpolicies
    ADD CONSTRAINT fk_invrollpol_storage FOREIGN KEY (storageid) REFERENCES public.t_storagemanager(id) ON DELETE CASCADE;


--
-- Name: t_invsiterelation fk_invsiterel_inventory; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_invsiterelation
    ADD CONSTRAINT fk_invsiterel_inventory FOREIGN KEY (filename, fileversion) REFERENCES public.t_inventory(filename, fileversion) ON DELETE CASCADE;


--
-- Name: t_invsiterelation fk_invsiterel_sites; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_invsiterelation
    ADD CONSTRAINT fk_invsiterel_sites FOREIGN KEY (site) REFERENCES public.t_sites(id) ON DELETE CASCADE;


--
-- Name: t_invspecialrollingpolicies fk_invspecrollpol_storage; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_invspecialrollingpolicies
    ADD CONSTRAINT fk_invspecrollpol_storage FOREIGN KEY (storageid) REFERENCES public.t_storagemanager(id) ON DELETE CASCADE;


--
-- Name: t_knownhosts_limits fk_knownhosts_limits01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_knownhosts_limits
    ADD CONSTRAINT fk_knownhosts_limits01 FOREIGN KEY (circulator_node_id) REFERENCES public.t_knownhosts(id) ON DELETE CASCADE;


--
-- Name: t_knownhosts_limits fk_knownhosts_limits02; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_knownhosts_limits
    ADD CONSTRAINT fk_knownhosts_limits02 FOREIGN KEY (dest_host_id) REFERENCES public.t_knownhosts(id) ON DELETE CASCADE;


--
-- Name: t_mpm_dip fk_mpm_dip01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mpm_dip
    ADD CONSTRAINT fk_mpm_dip01 FOREIGN KEY (inventory_id) REFERENCES public.t_inventory(id_inv) ON DELETE CASCADE;


--
-- Name: t_mpm_dip fk_mpm_dip02; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mpm_dip
    ADD CONSTRAINT fk_mpm_dip02 FOREIGN KEY (dip_status_id) REFERENCES public.t_mpm_statuses(id_status) ON DELETE CASCADE;


--
-- Name: t_mpm_ipf_adf_check fk_mpm_ipf_adf_check01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mpm_ipf_adf_check
    ADD CONSTRAINT fk_mpm_ipf_adf_check01 FOREIGN KEY (product_id) REFERENCES public.t_mpm_products(id_product) ON DELETE CASCADE;


--
-- Name: t_mpm_parameters fk_mpm_parameters01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mpm_parameters
    ADD CONSTRAINT fk_mpm_parameters01 FOREIGN KEY (paramtype_id) REFERENCES public.t_mpm_parameter_types(id_paramtype) ON DELETE CASCADE;


--
-- Name: t_mpm_prb fk_mpm_prb01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mpm_prb
    ADD CONSTRAINT fk_mpm_prb01 FOREIGN KEY (inventory_id) REFERENCES public.t_inventory(id_inv) ON DELETE CASCADE;


--
-- Name: t_mpm_prb fk_mpm_prb02; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mpm_prb
    ADD CONSTRAINT fk_mpm_prb02 FOREIGN KEY (status_id) REFERENCES public.t_mpm_statuses(id_status) ON DELETE CASCADE;


--
-- Name: t_objsminvrelation fk_objsminvrelation01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_objsminvrelation
    ADD CONSTRAINT fk_objsminvrelation01 FOREIGN KEY (id) REFERENCES public.t_inventorysm(id) ON DELETE CASCADE;


--
-- Name: t_ordersattachments fk_ordattach_attachtype; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_ordersattachments
    ADD CONSTRAINT fk_ordattach_attachtype FOREIGN KEY (attachtype) REFERENCES public.t_ordersattachmenttypes(id) ON DELETE CASCADE;


--
-- Name: t_ordersattachments fk_ordattach_ordqueue; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_ordersattachments
    ADD CONSTRAINT fk_ordattach_ordqueue FOREIGN KEY (orderid) REFERENCES public.t_ordersqueue(orderid) ON DELETE CASCADE;


--
-- Name: t_ordtriggeringfiles fk_ordtrigger_inventory; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_ordtriggeringfiles
    ADD CONSTRAINT fk_ordtrigger_inventory FOREIGN KEY (filename, fileversion) REFERENCES public.t_inventory(filename, fileversion) ON DELETE CASCADE;


--
-- Name: t_ordtriggeringfiles fk_ordtrigger_ordqueue; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_ordtriggeringfiles
    ADD CONSTRAINT fk_ordtrigger_ordqueue FOREIGN KEY (orderid) REFERENCES public.t_ordersqueue(orderid) ON DELETE CASCADE;


--
-- Name: t_proc_ftype_generation fk_proc_ftype_generation01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_proc_ftype_generation
    ADD CONSTRAINT fk_proc_ftype_generation01 FOREIGN KEY (proc_id) REFERENCES public.t_processors(id_processor) ON DELETE CASCADE;


--
-- Name: t_proc_ftype_generation fk_proc_ftype_generation02; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_proc_ftype_generation
    ADD CONSTRAINT fk_proc_ftype_generation02 FOREIGN KEY (ftype_id) REFERENCES public.t_filetypes(id_filetype) ON DELETE CASCADE;


--
-- Name: t_receptionrules fk_receptionrules1; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_receptionrules
    ADD CONSTRAINT fk_receptionrules1 FOREIGN KEY (filetype_id) REFERENCES public.t_filetypes(id_filetype) ON DELETE CASCADE;


--
-- Name: t_receptionrules fk_receptionrules2; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_receptionrules
    ADD CONSTRAINT fk_receptionrules2 FOREIGN KEY (repository_id) REFERENCES public.t_repositories(id_repository) ON DELETE CASCADE;


--
-- Name: t_receptionrules fk_receptionrules3; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_receptionrules
    ADD CONSTRAINT fk_receptionrules3 FOREIGN KEY (user_id) REFERENCES public.t_users(id_user) ON DELETE CASCADE;


--
-- Name: t_receptionrules fk_receptionrules4; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_receptionrules
    ADD CONSTRAINT fk_receptionrules4 FOREIGN KEY (usermode_id) REFERENCES public.t_usermodes(id) ON DELETE CASCADE;


--
-- Name: t_receptionrules fk_receptionrules5; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_receptionrules
    ADD CONSTRAINT fk_receptionrules5 FOREIGN KEY (reception_tt_id) REFERENCES public.t_tasktables(id_tasktable) ON DELETE CASCADE;


--
-- Name: t_receptionrules fk_receptionrules6; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_receptionrules
    ADD CONSTRAINT fk_receptionrules6 FOREIGN KEY (discoveryplugin_id) REFERENCES public.t_discoveryplugins(id_discoveryplugin) ON DELETE CASCADE;


--
-- Name: t_receptionrules fk_receptionrules7; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_receptionrules
    ADD CONSTRAINT fk_receptionrules7 FOREIGN KEY (satellite_id) REFERENCES public.t_satellites(satelliteid) ON DELETE CASCADE;


--
-- Name: t_receptionrules fk_receptionrules8; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_receptionrules
    ADD CONSTRAINT fk_receptionrules8 FOREIGN KEY (sensor_id) REFERENCES public.t_sensors(sensorid) ON DELETE CASCADE;


--
-- Name: t_receptionruleshist fk_receptionruleshist1; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_receptionruleshist
    ADD CONSTRAINT fk_receptionruleshist1 FOREIGN KEY (receptionrule_id) REFERENCES public.t_receptionrules(id_receptionrule) ON DELETE CASCADE;


--
-- Name: t_receptionruleshist fk_receptionruleshist2; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_receptionruleshist
    ADD CONSTRAINT fk_receptionruleshist2 FOREIGN KEY (status_id) REFERENCES public.t_stati(id) ON DELETE CASCADE;


--
-- Name: t_receptionruleslock fk_receptionruleslock01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_receptionruleslock
    ADD CONSTRAINT fk_receptionruleslock01 FOREIGN KEY (receptionrule_id) REFERENCES public.t_receptionrules(id_receptionrule) ON DELETE CASCADE;


--
-- Name: t_securityzonerelays fk_securityzonerelays01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_securityzonerelays
    ADD CONSTRAINT fk_securityzonerelays01 FOREIGN KEY (host_id) REFERENCES public.t_knownhosts(id) ON DELETE CASCADE;


--
-- Name: t_securityzonerelays fk_securityzonerelays02; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_securityzonerelays
    ADD CONSTRAINT fk_securityzonerelays02 FOREIGN KEY (seczone_id) REFERENCES public.t_securityzones(id) ON DELETE CASCADE;


--
-- Name: t_sites fk_sites_parent; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_sites
    ADD CONSTRAINT fk_sites_parent FOREIGN KEY (parent) REFERENCES public.t_sites(id) ON DELETE CASCADE;


--
-- Name: t_sites fk_sites_sitetype; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_sites
    ADD CONSTRAINT fk_sites_sitetype FOREIGN KEY (type) REFERENCES public.t_sitetype(id) ON DELETE CASCADE;


--
-- Name: t_sm_sync_missingactions fk_sm_sync_missingactions01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_sm_sync_missingactions
    ADD CONSTRAINT fk_sm_sync_missingactions01 FOREIGN KEY (sm_id) REFERENCES public.t_storagemanager(id) ON DELETE CASCADE;


--
-- Name: t_sm_sync_missingactions fk_sm_sync_missingactions02; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_sm_sync_missingactions
    ADD CONSTRAINT fk_sm_sync_missingactions02 FOREIGN KEY (inv_id) REFERENCES public.t_inventory(id_inv) ON DELETE CASCADE;


--
-- Name: t_sm_sync_missingactions fk_sm_sync_missingactions03; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_sm_sync_missingactions
    ADD CONSTRAINT fk_sm_sync_missingactions03 FOREIGN KEY (invsm_id) REFERENCES public.t_inventorysm(id) ON DELETE CASCADE;


--
-- Name: t_smactions fk_smactions_smqueue; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_smactions
    ADD CONSTRAINT fk_smactions_smqueue FOREIGN KEY (packageid) REFERENCES public.t_smqueue(packageid) ON DELETE CASCADE;


--
-- Name: t_smactions fk_smactions_storagemanager; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_smactions
    ADD CONSTRAINT fk_smactions_storagemanager FOREIGN KEY (smid) REFERENCES public.t_storagemanager(id) ON DELETE CASCADE;


--
-- Name: t_smqueue fk_smqueue_storagemanager; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_smqueue
    ADD CONSTRAINT fk_smqueue_storagemanager FOREIGN KEY (smid) REFERENCES public.t_storagemanager(id) ON DELETE CASCADE;


--
-- Name: t_stafinvrelation fk_stafinvrel_invsm; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_stafinvrelation
    ADD CONSTRAINT fk_stafinvrel_invsm FOREIGN KEY (id) REFERENCES public.t_inventorysm(id) ON DELETE CASCADE;


--
-- Name: t_statetransitions_allowed fk_statetransitions_allowed01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_statetransitions_allowed
    ADD CONSTRAINT fk_statetransitions_allowed01 FOREIGN KEY (table_id) REFERENCES public.t_statetransitions_tables(id) ON DELETE CASCADE;


--
-- Name: t_statetransitions_allowed fk_statetransitions_allowed02; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_statetransitions_allowed
    ADD CONSTRAINT fk_statetransitions_allowed02 FOREIGN KEY (prev_state_id) REFERENCES public.t_stati(id) ON DELETE CASCADE;


--
-- Name: t_statetransitions_allowed fk_statetransitions_allowed03; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_statetransitions_allowed
    ADD CONSTRAINT fk_statetransitions_allowed03 FOREIGN KEY (post_state_id) REFERENCES public.t_stati(id) ON DELETE CASCADE;


--
-- Name: t_statetransitions_tables fk_statetransitions_tables01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_statetransitions_tables
    ADD CONSTRAINT fk_statetransitions_tables01 FOREIGN KEY (flow_id) REFERENCES public.t_statetransitions_flows(id) ON DELETE CASCADE;


--
-- Name: t_standingdistribution fk_stendingdistribution01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_standingdistribution
    ADD CONSTRAINT fk_stendingdistribution01 FOREIGN KEY (distpoliciesid) REFERENCES public.t_distributionrules(id_distributionrule) ON DELETE CASCADE;


--
-- Name: t_standingdistribution fk_stendingdistribution02; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_standingdistribution
    ADD CONSTRAINT fk_stendingdistribution02 FOREIGN KEY (cartsid) REFERENCES public.t_cartsref(id) ON DELETE CASCADE;


--
-- Name: t_storagegroup fk_storagegroup_ftype; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_storagegroup
    ADD CONSTRAINT fk_storagegroup_ftype FOREIGN KEY (filetype) REFERENCES public.t_filetypes(filetype) ON DELETE CASCADE;


--
-- Name: t_storagegroup fk_storagegroup_storage; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_storagegroup
    ADD CONSTRAINT fk_storagegroup_storage FOREIGN KEY (storagemanager) REFERENCES public.t_storagemanager(id) ON DELETE CASCADE;


--
-- Name: t_storagerollingthresholds fk_storagerollingthresholds01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_storagerollingthresholds
    ADD CONSTRAINT fk_storagerollingthresholds01 FOREIGN KEY (storagemanager_id) REFERENCES public.t_storagemanager(id) ON DELETE CASCADE;


--
-- Name: t_storagerollingthresholds fk_storagerollingthresholds02; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_storagerollingthresholds
    ADD CONSTRAINT fk_storagerollingthresholds02 FOREIGN KEY (action_id) REFERENCES public.t_rollingpolicyactions(id) ON DELETE CASCADE;


--
-- Name: t_acshsminvrelation fk_t_acshsminvrelation; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_acshsminvrelation
    ADD CONSTRAINT fk_t_acshsminvrelation FOREIGN KEY (id) REFERENCES public.t_inventorysm(id) ON DELETE CASCADE;


--
-- Name: t_acsisminvrelation fk_t_acsisminvrel; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_acsisminvrelation
    ADD CONSTRAINT fk_t_acsisminvrel FOREIGN KEY (id) REFERENCES public.t_inventorysm(id) ON DELETE CASCADE;


--
-- Name: t_asyncsm_operations fk_t_asyncsm_operations1; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_asyncsm_operations
    ADD CONSTRAINT fk_t_asyncsm_operations1 FOREIGN KEY (operation_id) REFERENCES public.t_operationtypes(id_operationtype);


--
-- Name: t_asyncsm_operations fk_t_asyncsm_operations2; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_asyncsm_operations
    ADD CONSTRAINT fk_t_asyncsm_operations2 FOREIGN KEY (sm_id) REFERENCES public.t_storagemanager(id);


--
-- Name: t_asyncsm_operations fk_t_asyncsm_operations3; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_asyncsm_operations
    ADD CONSTRAINT fk_t_asyncsm_operations3 FOREIGN KEY (status_id) REFERENCES public.t_stati(id);


--
-- Name: t_circulationpolicies_monitor fk_t_circulationpolicies_monitor01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_circulationpolicies_monitor
    ADD CONSTRAINT fk_t_circulationpolicies_monitor01 FOREIGN KEY (circulationpolicy_id) REFERENCES public.t_circulationpolicies(id) ON DELETE CASCADE;


--
-- Name: t_classifiedfiletypes fk_t_classifiedfiletypes; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_classifiedfiletypes
    ADD CONSTRAINT fk_t_classifiedfiletypes FOREIGN KEY (filetype) REFERENCES public.t_filetypes(filetype) ON DELETE CASCADE;


--
-- Name: t_completed_orders fk_t_completed_orders_id; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_completed_orders
    ADD CONSTRAINT fk_t_completed_orders_id FOREIGN KEY (order_id) REFERENCES public.t_ordersqueue(orderid) ON DELETE CASCADE;


--
-- Name: t_datastoragequeue fk_t_datastoragequeue; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_datastoragequeue
    ADD CONSTRAINT fk_t_datastoragequeue FOREIGN KEY (flowtag_id) REFERENCES public.t_flowtags(id) ON DELETE CASCADE;


--
-- Name: t_discacheinvrelation fk_t_discacheinvrelation01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_discacheinvrelation
    ADD CONSTRAINT fk_t_discacheinvrelation01 FOREIGN KEY (id) REFERENCES public.t_inventorysm(id) ON DELETE CASCADE;


--
-- Name: t_distpoldataset fk_t_distpoldataset1; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_distpoldataset
    ADD CONSTRAINT fk_t_distpoldataset1 FOREIGN KEY (policy) REFERENCES public.t_distributionrules(id_distributionrule) ON DELETE CASCADE;


--
-- Name: t_distpoldataset fk_t_distpoldataset2; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_distpoldataset
    ADD CONSTRAINT fk_t_distpoldataset2 FOREIGN KEY (dataset) REFERENCES public.t_measdataset(id) ON DELETE CASCADE;


--
-- Name: t_distributionitems fk_t_distributionitems1; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_distributionitems
    ADD CONSTRAINT fk_t_distributionitems1 FOREIGN KEY (packageid) REFERENCES public.t_distributionqueue(packageid) ON DELETE CASCADE;


--
-- Name: t_distributionitems fk_t_distributionitems2; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_distributionitems
    ADD CONSTRAINT fk_t_distributionitems2 FOREIGN KEY (mediatype) REFERENCES public.t_mediatypes(mediatype) ON DELETE CASCADE;


--
-- Name: t_distributionitems fk_t_distributionitems3; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_distributionitems
    ADD CONSTRAINT fk_t_distributionitems3 FOREIGN KEY (userid) REFERENCES public.t_users(userid) ON DELETE CASCADE;


--
-- Name: t_distributionitems fk_t_distributionitems4; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_distributionitems
    ADD CONSTRAINT fk_t_distributionitems4 FOREIGN KEY (filename, fileversion) REFERENCES public.t_inventory(filename, fileversion) ON DELETE CASCADE;


--
-- Name: t_distributionitems fk_t_distributionitems5; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_distributionitems
    ADD CONSTRAINT fk_t_distributionitems5 FOREIGN KEY (format) REFERENCES public.t_tasktables(id_tasktable) ON DELETE CASCADE;


--
-- Name: t_distributionitems fk_t_distributionitems6; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_distributionitems
    ADD CONSTRAINT fk_t_distributionitems6 FOREIGN KEY (cartid) REFERENCES public.t_cartsref(id) ON DELETE CASCADE;


--
-- Name: t_distributionitems fk_t_distributionitems7; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_distributionitems
    ADD CONSTRAINT fk_t_distributionitems7 FOREIGN KEY (distributionrule_id) REFERENCES public.t_distributionrules(id_distributionrule) ON DELETE CASCADE;


--
-- Name: t_distributionitems fk_t_distributionitems8; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_distributionitems
    ADD CONSTRAINT fk_t_distributionitems8 FOREIGN KEY (disseminationpackingalgo_id) REFERENCES public.t_disseminationpackingalgos(id_disseminationpackingalgo) ON DELETE CASCADE;


--
-- Name: t_distributionqueue fk_t_distributionqueue1; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_distributionqueue
    ADD CONSTRAINT fk_t_distributionqueue1 FOREIGN KEY (format) REFERENCES public.t_tasktables(id_tasktable) ON DELETE CASCADE;


--
-- Name: t_distributionruleslock fk_t_distributionruleslock01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_distributionruleslock
    ADD CONSTRAINT fk_t_distributionruleslock01 FOREIGN KEY (packageid) REFERENCES public.t_distributionqueue(packageid) ON DELETE CASCADE;


--
-- Name: t_distruleoriginator fk_t_distruleoriginator1; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_distruleoriginator
    ADD CONSTRAINT fk_t_distruleoriginator1 FOREIGN KEY (distributionrule_id) REFERENCES public.t_distributionrules(id_distributionrule) ON DELETE CASCADE;


--
-- Name: t_distruleoriginator fk_t_distruleoriginator2; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_distruleoriginator
    ADD CONSTRAINT fk_t_distruleoriginator2 FOREIGN KEY (originator_id) REFERENCES public.t_originators(id) ON DELETE CASCADE;


--
-- Name: t_filetypes_x_satellites fk_t_filetypesxsatellites01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_filetypes_x_satellites
    ADD CONSTRAINT fk_t_filetypesxsatellites01 FOREIGN KEY (filetype) REFERENCES public.t_filetypes(filetype) ON DELETE CASCADE;


--
-- Name: t_filetypes_x_satellites fk_t_filetypesxsatellites02; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_filetypes_x_satellites
    ADD CONSTRAINT fk_t_filetypesxsatellites02 FOREIGN KEY (satellite_id) REFERENCES public.t_satellites(satelliteid) ON DELETE CASCADE;


--
-- Name: t_inventory fk_t_inventory1; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventory
    ADD CONSTRAINT fk_t_inventory1 FOREIGN KEY (fileclass) REFERENCES public.t_fileclasses(fileclass) ON DELETE CASCADE;


--
-- Name: t_inventory fk_t_inventory2; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventory
    ADD CONSTRAINT fk_t_inventory2 FOREIGN KEY (sensorid) REFERENCES public.t_sensors(sensorid) ON DELETE CASCADE;


--
-- Name: t_inventory fk_t_inventory3; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventory
    ADD CONSTRAINT fk_t_inventory3 FOREIGN KEY (satelliteid) REFERENCES public.t_satellites(satelliteid) ON DELETE CASCADE;


--
-- Name: t_inventory fk_t_inventory4; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventory
    ADD CONSTRAINT fk_t_inventory4 FOREIGN KEY (acquisitionstationid) REFERENCES public.t_stations(stationid) ON DELETE CASCADE;


--
-- Name: t_inventory fk_t_inventory5; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventory
    ADD CONSTRAINT fk_t_inventory5 FOREIGN KEY (filetype) REFERENCES public.t_filetypes(filetype) ON DELETE CASCADE;


--
-- Name: t_inventory fk_t_inventory6; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventory
    ADD CONSTRAINT fk_t_inventory6 FOREIGN KEY (processingstationid) REFERENCES public.t_stations(stationid) ON DELETE CASCADE;


--
-- Name: t_inventorycomments fk_t_inventorycomments; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventorycomments
    ADD CONSTRAINT fk_t_inventorycomments FOREIGN KEY (filename, fileversion) REFERENCES public.t_inventory(filename, fileversion) ON DELETE CASCADE;


--
-- Name: t_inventorydataset fk_t_inventorydataset1; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventorydataset
    ADD CONSTRAINT fk_t_inventorydataset1 FOREIGN KEY (filename, fileversion) REFERENCES public.t_inventory(filename, fileversion) ON DELETE CASCADE;


--
-- Name: t_inventorydataset fk_t_inventorydataset2; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventorydataset
    ADD CONSTRAINT fk_t_inventorydataset2 FOREIGN KEY (measdatasetid) REFERENCES public.t_measdataset(id) ON DELETE CASCADE;


--
-- Name: t_inventoryextra fk_t_inventoryextra; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventoryextra
    ADD CONSTRAINT fk_t_inventoryextra FOREIGN KEY (inv_id) REFERENCES public.t_inventory(id_inv) ON DELETE CASCADE;


--
-- Name: t_inventorygaps fk_t_inventorygaps; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventorygaps
    ADD CONSTRAINT fk_t_inventorygaps FOREIGN KEY (filename, fileversion) REFERENCES public.t_inventory(filename, fileversion) ON DELETE CASCADE;


--
-- Name: t_inventorysm fk_t_inventorysm1; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventorysm
    ADD CONSTRAINT fk_t_inventorysm1 FOREIGN KEY (inv_id) REFERENCES public.t_inventory(id_inv) ON DELETE CASCADE;


--
-- Name: t_inventorysm fk_t_inventorysm2; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventorysm
    ADD CONSTRAINT fk_t_inventorysm2 FOREIGN KEY (storagemanid) REFERENCES public.t_storagemanager(id) ON DELETE CASCADE;


--
-- Name: t_inventoryeventsconf fk_t_inventsconf_ftype; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_inventoryeventsconf
    ADD CONSTRAINT fk_t_inventsconf_ftype FOREIGN KEY (filetype) REFERENCES public.t_filetypes(filetype) ON DELETE CASCADE;


--
-- Name: t_invrollingpolicies fk_t_invrollingpolicies1; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_invrollingpolicies
    ADD CONSTRAINT fk_t_invrollingpolicies1 FOREIGN KEY (filetype) REFERENCES public.t_filetypes(filetype) ON DELETE CASCADE;


--
-- Name: t_invrollingpolicies fk_t_invrollingpolicies2; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_invrollingpolicies
    ADD CONSTRAINT fk_t_invrollingpolicies2 FOREIGN KEY (algorithm) REFERENCES public.t_rollingpoliciesalgo(id) ON DELETE CASCADE;


--
-- Name: t_invrollingpolicies fk_t_invrollingpolicies3; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_invrollingpolicies
    ADD CONSTRAINT fk_t_invrollingpolicies3 FOREIGN KEY (action) REFERENCES public.t_rollingpolicyactions(id) ON DELETE CASCADE;


--
-- Name: t_invrollingpolicies fk_t_invrollingpolicies4; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_invrollingpolicies
    ADD CONSTRAINT fk_t_invrollingpolicies4 FOREIGN KEY (fileclass) REFERENCES public.t_fileclasses(fileclass) ON DELETE CASCADE;


--
-- Name: t_invspecialrollingpolicies fk_t_invspecialrollingpolicies1; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_invspecialrollingpolicies
    ADD CONSTRAINT fk_t_invspecialrollingpolicies1 FOREIGN KEY (inventoryname, inventoryversion) REFERENCES public.t_inventory(filename, fileversion) ON DELETE CASCADE;


--
-- Name: t_invspecialrollingpolicies fk_t_invspecialrollingpolicies2; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_invspecialrollingpolicies
    ADD CONSTRAINT fk_t_invspecialrollingpolicies2 FOREIGN KEY (action) REFERENCES public.t_rollingpolicyactions(id) ON DELETE CASCADE;


--
-- Name: t_mediacapformat fk_t_mediacapformat1; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mediacapformat
    ADD CONSTRAINT fk_t_mediacapformat1 FOREIGN KEY (mediatype) REFERENCES public.t_mediatypes(mediatype) ON DELETE CASCADE;


--
-- Name: t_mpm_cross fk_t_mpm_cross02; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mpm_cross
    ADD CONSTRAINT fk_t_mpm_cross02 FOREIGN KEY (parameter_id) REFERENCES public.t_mpm_parameters(id_parameter) ON DELETE CASCADE;


--
-- Name: t_mpm_cross fk_t_mpm_cross03; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mpm_cross
    ADD CONSTRAINT fk_t_mpm_cross03 FOREIGN KEY (routine_id) REFERENCES public.t_mpm_routines(id_routine) ON DELETE CASCADE;


--
-- Name: t_mpm_cross fk_t_mpm_cross04; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mpm_cross
    ADD CONSTRAINT fk_t_mpm_cross04 FOREIGN KEY (product_id) REFERENCES public.t_mpm_products(id_product) ON DELETE CASCADE;


--
-- Name: t_mpm_products fk_t_mpm_products01; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mpm_products
    ADD CONSTRAINT fk_t_mpm_products01 FOREIGN KEY (filetype_id) REFERENCES public.t_filetypes(id_filetype) ON DELETE CASCADE;


--
-- Name: t_mpm_products fk_t_mpm_products02; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mpm_products
    ADD CONSTRAINT fk_t_mpm_products02 FOREIGN KEY (timeliness_id) REFERENCES public.t_mpm_timeliness(id_timeliness) ON DELETE CASCADE;


--
-- Name: t_mpm_products fk_t_mpm_products03; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mpm_products
    ADD CONSTRAINT fk_t_mpm_products03 FOREIGN KEY (processor_id) REFERENCES public.t_processors(id_processor) ON DELETE CASCADE;


--
-- Name: t_mpm_products fk_t_mpm_products04; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_mpm_products
    ADD CONSTRAINT fk_t_mpm_products04 FOREIGN KEY (qualityflag_id) REFERENCES public.t_qualifiedflag(id) ON DELETE CASCADE;


--
-- Name: t_ondemanddistribution fk_t_ondemanddistribution1; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_ondemanddistribution
    ADD CONSTRAINT fk_t_ondemanddistribution1 FOREIGN KEY (orderid) REFERENCES public.t_ordersqueue(orderid) ON DELETE CASCADE;


--
-- Name: t_ondemanddistribution fk_t_ondemanddistribution2; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_ondemanddistribution
    ADD CONSTRAINT fk_t_ondemanddistribution2 FOREIGN KEY (userid) REFERENCES public.t_users(userid) ON DELETE CASCADE;


--
-- Name: t_ondemanddistribution fk_t_ondemanddistribution3; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_ondemanddistribution
    ADD CONSTRAINT fk_t_ondemanddistribution3 FOREIGN KEY (format) REFERENCES public.t_tasktables(id_tasktable) ON DELETE CASCADE;


--
-- Name: t_ondemanddistribution fk_t_ondemanddistribution4; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_ondemanddistribution
    ADD CONSTRAINT fk_t_ondemanddistribution4 FOREIGN KEY (mediatype) REFERENCES public.t_mediatypes(mediatype) ON DELETE CASCADE;


--
-- Name: t_ondemanddistribution fk_t_ondemanddistribution5; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_ondemanddistribution
    ADD CONSTRAINT fk_t_ondemanddistribution5 FOREIGN KEY (disseminationpackingalgo_id) REFERENCES public.t_disseminationpackingalgos(id_disseminationpackingalgo) ON DELETE CASCADE;


--
-- Name: t_ordersgeneration fk_t_ordersgeneration1; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_ordersgeneration
    ADD CONSTRAINT fk_t_ordersgeneration1 FOREIGN KEY (processorname, processorversion) REFERENCES public.t_processors(processorname, processorversion) ON DELETE CASCADE;


--
-- Name: t_ordersgeneration fk_t_ordersgeneration2; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_ordersgeneration
    ADD CONSTRAINT fk_t_ordersgeneration2 FOREIGN KEY (filetype) REFERENCES public.t_filetypes(filetype) ON DELETE CASCADE;


--
-- Name: t_ordersgeneration fk_t_ordersgeneration3; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_ordersgeneration
    ADD CONSTRAINT fk_t_ordersgeneration3 FOREIGN KEY (ordertype) REFERENCES public.t_ordertypes(ordertype) ON DELETE CASCADE;


--
-- Name: t_ordersgeneration fk_t_ordersgeneration4; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_ordersgeneration
    ADD CONSTRAINT fk_t_ordersgeneration4 FOREIGN KEY (algorithm) REFERENCES public.t_algorithm(algorithm) ON DELETE CASCADE;


--
-- Name: t_ordersgeneration fk_t_ordersgeneration5; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_ordersgeneration
    ADD CONSTRAINT fk_t_ordersgeneration5 FOREIGN KEY (fileclass) REFERENCES public.t_fileclasses(fileclass) ON DELETE CASCADE;


--
-- Name: t_ordersinput fk_t_ordersinput1; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_ordersinput
    ADD CONSTRAINT fk_t_ordersinput1 FOREIGN KEY (filename, fileversion) REFERENCES public.t_inventory(filename, fileversion) ON DELETE CASCADE;


--
-- Name: t_ordersinput fk_t_ordersinput2; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_ordersinput
    ADD CONSTRAINT fk_t_ordersinput2 FOREIGN KEY (orderid) REFERENCES public.t_ordersqueue(orderid) ON DELETE CASCADE;


--
-- Name: t_ordersqueue fk_t_ordersqueue1; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_ordersqueue
    ADD CONSTRAINT fk_t_ordersqueue1 FOREIGN KEY (ordertype) REFERENCES public.t_ordertypes(ordertype) ON DELETE CASCADE;


--
-- Name: t_ordersqueue fk_t_ordersqueue2; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_ordersqueue
    ADD CONSTRAINT fk_t_ordersqueue2 FOREIGN KEY (processorname, processorversion) REFERENCES public.t_processors(processorname, processorversion) ON DELETE CASCADE;


--
-- Name: t_ordersqueue fk_t_ordersqueue3; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_ordersqueue
    ADD CONSTRAINT fk_t_ordersqueue3 FOREIGN KEY (satelliteid) REFERENCES public.t_satellites(satelliteid) ON DELETE CASCADE;


--
-- Name: t_ordersqueueblacklist fk_t_ordersqueueblacklist1; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_ordersqueueblacklist
    ADD CONSTRAINT fk_t_ordersqueueblacklist1 FOREIGN KEY (orderid) REFERENCES public.t_ordersqueue(orderid) ON DELETE CASCADE;


--
-- Name: t_orderstatetransitions fk_t_orderstatetransitions; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_orderstatetransitions
    ADD CONSTRAINT fk_t_orderstatetransitions FOREIGN KEY (orderid) REFERENCES public.t_ordersqueue(orderid) ON DELETE CASCADE;


--
-- Name: t_pendinginventory fk_t_pendinginventory; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_pendinginventory
    ADD CONSTRAINT fk_t_pendinginventory FOREIGN KEY (filename, fileversion) REFERENCES public.t_inventory(filename, fileversion) ON DELETE CASCADE;


--
-- Name: t_repositories fk_t_repositories1; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_repositories
    ADD CONSTRAINT fk_t_repositories1 FOREIGN KEY (transferprotocol_id) REFERENCES public.t_transferprotocols(id) ON DELETE CASCADE;


--
-- Name: t_repositories fk_t_repositories2; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_repositories
    ADD CONSTRAINT fk_t_repositories2 FOREIGN KEY (host_id) REFERENCES public.t_knownhosts(id) ON DELETE CASCADE;


--
-- Name: t_repositories fk_t_repositories3; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_repositories
    ADD CONSTRAINT fk_t_repositories3 FOREIGN KEY (tempname_id) REFERENCES public.t_tmpnamestrategy(id_tmpnamestrategy) ON DELETE CASCADE;


--
-- Name: t_satrelations fk_t_satrelations1; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_satrelations
    ADD CONSTRAINT fk_t_satrelations1 FOREIGN KEY (satelliteid) REFERENCES public.t_satellites(satelliteid) ON DELETE CASCADE;


--
-- Name: t_satrelations fk_t_satrelations2; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_satrelations
    ADD CONSTRAINT fk_t_satrelations2 FOREIGN KEY (sensorid) REFERENCES public.t_sensors(sensorid) ON DELETE CASCADE;


--
-- Name: t_schema fk_t_schema; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_schema
    ADD CONSTRAINT fk_t_schema FOREIGN KEY (schematype) REFERENCES public.t_schematypes(id);


--
-- Name: t_schema fk_t_schema2; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_schema
    ADD CONSTRAINT fk_t_schema2 FOREIGN KEY (modificationtype) REFERENCES public.t_schemamodificationtypes(id);


--
-- Name: t_storagemanager fk_t_storagemanager1; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_storagemanager
    ADD CONSTRAINT fk_t_storagemanager1 FOREIGN KEY (type) REFERENCES public.t_smtype(type) ON DELETE CASCADE;


--
-- Name: t_storagemanager fk_t_storagemanager2; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_storagemanager
    ADD CONSTRAINT fk_t_storagemanager2 FOREIGN KEY (migrationalgo_id) REFERENCES public.t_migrationalgos(id_migrationalgo) ON DELETE CASCADE;


--
-- Name: t_storagepriority fk_t_storagepriority1; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_storagepriority
    ADD CONSTRAINT fk_t_storagepriority1 FOREIGN KEY (id) REFERENCES public.t_storagemanager(id) ON DELETE CASCADE;


--
-- Name: t_storagepriority fk_t_storagepriority2; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_storagepriority
    ADD CONSTRAINT fk_t_storagepriority2 FOREIGN KEY (filetype) REFERENCES public.t_filetypes(filetype) ON DELETE CASCADE;


--
-- Name: t_temporarydistribution fk_t_temporarydistribution1; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_temporarydistribution
    ADD CONSTRAINT fk_t_temporarydistribution1 FOREIGN KEY (policy) REFERENCES public.t_distributionrules(id_distributionrule) ON DELETE CASCADE;


--
-- Name: t_temporarydistribution fk_t_temporarydistribution2; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_temporarydistribution
    ADD CONSTRAINT fk_t_temporarydistribution2 FOREIGN KEY (mediatype) REFERENCES public.t_mediatypes(mediatype) ON DELETE CASCADE;


--
-- Name: t_temporarydistribution fk_t_temporarydistribution3; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_temporarydistribution
    ADD CONSTRAINT fk_t_temporarydistribution3 FOREIGN KEY (userid) REFERENCES public.t_users(userid) ON DELETE CASCADE;


--
-- Name: t_temporarydistribution fk_t_temporarydistribution4; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_temporarydistribution
    ADD CONSTRAINT fk_t_temporarydistribution4 FOREIGN KEY (filename, fileversion) REFERENCES public.t_inventory(filename, fileversion) ON DELETE CASCADE;


--
-- Name: t_temporarydistribution fk_t_temporarydistribution5; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_temporarydistribution
    ADD CONSTRAINT fk_t_temporarydistribution5 FOREIGN KEY (format) REFERENCES public.t_tasktables(id_tasktable) ON DELETE CASCADE;


--
-- Name: t_temporarydistribution fk_t_temporarydistribution6; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_temporarydistribution
    ADD CONSTRAINT fk_t_temporarydistribution6 FOREIGN KEY (algo) REFERENCES public.t_distributionalgo(id) ON DELETE CASCADE;


--
-- Name: t_temporarydistribution fk_t_temporarydistribution7; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_temporarydistribution
    ADD CONSTRAINT fk_t_temporarydistribution7 FOREIGN KEY (cartid) REFERENCES public.t_cartsref(id) ON DELETE CASCADE;


--
-- Name: t_temporarydistribution fk_t_temporarydistribution8; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_temporarydistribution
    ADD CONSTRAINT fk_t_temporarydistribution8 FOREIGN KEY (disseminationpackingalgo_id) REFERENCES public.t_disseminationpackingalgos(id_disseminationpackingalgo) ON DELETE CASCADE;


--
-- Name: t_temporaryorders fk_t_temporaryorders1; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_temporaryorders
    ADD CONSTRAINT fk_t_temporaryorders1 FOREIGN KEY (ordersat) REFERENCES public.t_satellites(satelliteid) ON DELETE CASCADE;


--
-- Name: t_temporaryorders fk_t_temporaryorders2; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_temporaryorders
    ADD CONSTRAINT fk_t_temporaryorders2 FOREIGN KEY (filename, fileversion) REFERENCES public.t_inventory(filename, fileversion) ON DELETE CASCADE;


--
-- Name: t_users fk_t_users; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_users
    ADD CONSTRAINT fk_t_users FOREIGN KEY (usertype) REFERENCES public.t_usertypes(usertype) ON DELETE CASCADE;


--
-- Name: t_waitingorders fk_t_waitingorders1; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_waitingorders
    ADD CONSTRAINT fk_t_waitingorders1 FOREIGN KEY (orderid) REFERENCES public.t_ordersqueue(orderid) ON DELETE CASCADE;


--
-- Name: t_waitingorders fk_t_waitingorders2; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_waitingorders
    ADD CONSTRAINT fk_t_waitingorders2 FOREIGN KEY (filetype) REFERENCES public.t_filetypes(filetype) ON DELETE CASCADE;


--
-- Name: t_waitingorders fk_t_waitingorders3; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_waitingorders
    ADD CONSTRAINT fk_t_waitingorders3 FOREIGN KEY (triggertype) REFERENCES public.t_orderwaittrigger(triggertype) ON DELETE CASCADE;


--
-- Name: t_tasktables fk_tasktables1; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_tasktables
    ADD CONSTRAINT fk_tasktables1 FOREIGN KEY (type_id) REFERENCES public.t_operationtypes(id_operationtype) ON DELETE CASCADE;


--
-- Name: t_usdfwa_delivery_details fk_usdfwa_request; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_usdfwa_delivery_details
    ADD CONSTRAINT fk_usdfwa_request FOREIGN KEY (usdfwa_id) REFERENCES public.t_usdfwa(id_usdfwa) ON DELETE CASCADE;


--
-- Name: t_users fk_users_modes; Type: FK CONSTRAINT; Schema: public; Owner: acsdba
--

ALTER TABLE ONLY public.t_users
    ADD CONSTRAINT fk_users_modes FOREIGN KEY (mode) REFERENCES public.t_usermodes(id) ON DELETE CASCADE;


--
-- Name: TABLE geography_columns; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT ON TABLE public.geography_columns TO pdsuser;


--
-- Name: TABLE geometry_columns; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT ON TABLE public.geometry_columns TO pdsuser;


--
-- Name: TABLE t_filetypes; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_filetypes TO pdsuser;


--
-- Name: TABLE t_mpm_cross; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_mpm_cross TO pdsuser;


--
-- Name: TABLE t_mpm_parameters; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_mpm_parameters TO pdsuser;


--
-- Name: TABLE t_mpm_products; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_mpm_products TO pdsuser;


--
-- Name: TABLE spatial_ref_sys; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.spatial_ref_sys TO pdsuser;


--
-- Name: TABLE t_acshsminvrelation; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_acshsminvrelation TO pdsuser;


--
-- Name: TABLE t_acsisminvrelation; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_acsisminvrelation TO pdsuser;


--
-- Name: TABLE t_acsltainvrelation; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_acsltainvrelation TO pdsuser;


--
-- Name: TABLE t_algorithm; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_algorithm TO pdsuser;


--
-- Name: TABLE t_anomaly_types; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_anomaly_types TO pdsuser;


--
-- Name: TABLE t_applicationevents; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_applicationevents TO pdsuser;


--
-- Name: SEQUENCE t_applicationevents_id_applicationevent_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_applicationevents_id_applicationevent_seq TO pdsuser;


--
-- Name: TABLE t_applications; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_applications TO pdsuser;


--
-- Name: SEQUENCE t_applications_id_application_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_applications_id_application_seq TO pdsuser;


--
-- Name: TABLE t_applicationstatus; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_applicationstatus TO pdsuser;


--
-- Name: TABLE t_asyncsm_operations; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_asyncsm_operations TO pdsuser;


--
-- Name: SEQUENCE t_asyncsm_operations_id_asyncsm_operation_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_asyncsm_operations_id_asyncsm_operation_seq TO pdsuser;


--
-- Name: TABLE t_bands; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_bands TO pdsuser;


--
-- Name: SEQUENCE t_bands_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_bands_id_seq TO pdsuser;


--
-- Name: TABLE t_blockedcirculationsqueue; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_blockedcirculationsqueue TO pdsuser;


--
-- Name: SEQUENCE t_blockedcirculationsqueue_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_blockedcirculationsqueue_id_seq TO pdsuser;


--
-- Name: TABLE t_cartsinv; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_cartsinv TO pdsuser;


--
-- Name: SEQUENCE t_cartsinv_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_cartsinv_id_seq TO pdsuser;


--
-- Name: TABLE t_cartsref; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_cartsref TO pdsuser;


--
-- Name: SEQUENCE t_cartsref_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_cartsref_id_seq TO pdsuser;


--
-- Name: TABLE t_cdp_feederqueue; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_cdp_feederqueue TO pdsuser;


--
-- Name: SEQUENCE t_cdp_feederqueue_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_cdp_feederqueue_id_seq TO pdsuser;


--
-- Name: TABLE t_cdp_notification_queue; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_cdp_notification_queue TO pdsuser;


--
-- Name: SEQUENCE t_cdp_notification_queue_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_cdp_notification_queue_id_seq TO pdsuser;


--
-- Name: TABLE t_circ_callbacks; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_circ_callbacks TO pdsuser;


--
-- Name: SEQUENCE t_circ_callbacks_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_circ_callbacks_id_seq TO pdsuser;


--
-- Name: TABLE t_circulation_remove_queue; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_circulation_remove_queue TO pdsuser;


--
-- Name: SEQUENCE t_circulation_remove_queue_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_circulation_remove_queue_id_seq TO pdsuser;


--
-- Name: TABLE t_circulationactivehosts; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_circulationactivehosts TO pdsuser;


--
-- Name: TABLE t_circulationmodes; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_circulationmodes TO pdsuser;


--
-- Name: TABLE t_circulationpolicies; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_circulationpolicies TO pdsuser;


--
-- Name: SEQUENCE t_circulationpolicies_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_circulationpolicies_id_seq TO pdsuser;


--
-- Name: TABLE t_circulationpolicies_monitor; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_circulationpolicies_monitor TO pdsuser;


--
-- Name: SEQUENCE t_circulationpolicies_monitor_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_circulationpolicies_monitor_id_seq TO pdsuser;


--
-- Name: TABLE t_circulationsmedia; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_circulationsmedia TO pdsuser;


--
-- Name: SEQUENCE t_circulationsmedia_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_circulationsmedia_id_seq TO pdsuser;


--
-- Name: TABLE t_circulationsmedia_insmode; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_circulationsmedia_insmode TO pdsuser;


--
-- Name: TABLE t_circulationsqueue; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_circulationsqueue TO pdsuser;


--
-- Name: SEQUENCE t_circulationsqueue_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_circulationsqueue_id_seq TO pdsuser;


--
-- Name: TABLE t_circulationsqueuedwh; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_circulationsqueuedwh TO pdsuser;


--
-- Name: SEQUENCE t_circulationsqueuedwh_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_circulationsqueuedwh_id_seq TO pdsuser;


--
-- Name: TABLE t_circulationstats; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_circulationstats TO pdsuser;


--
-- Name: SEQUENCE t_circulationstats_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_circulationstats_id_seq TO pdsuser;


--
-- Name: TABLE t_classifiedfiletypes; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_classifiedfiletypes TO pdsuser;


--
-- Name: SEQUENCE t_classifiedfiletypes_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_classifiedfiletypes_id_seq TO pdsuser;


--
-- Name: TABLE t_cleanupalgos; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_cleanupalgos TO pdsuser;


--
-- Name: TABLE t_cleanuprules; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_cleanuprules TO pdsuser;


--
-- Name: SEQUENCE t_cleanuprules_id_cleanuprule_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_cleanuprules_id_cleanuprule_seq TO pdsuser;


--
-- Name: TABLE t_completed_orders; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_completed_orders TO pdsuser;


--
-- Name: TABLE t_configurationschemas; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_configurationschemas TO pdsuser;


--
-- Name: TABLE t_configurationspaces; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_configurationspaces TO pdsuser;


--
-- Name: TABLE t_configurationstylesheets; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_configurationstylesheets TO pdsuser;


--
-- Name: SEQUENCE t_configurationstylesheets_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_configurationstylesheets_id_seq TO pdsuser;


--
-- Name: TABLE t_cov_periods; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_cov_periods TO pdsuser;


--
-- Name: SEQUENCE t_cov_periods_id_interval_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_cov_periods_id_interval_seq TO pdsuser;


--
-- Name: TABLE t_cov_relevancies; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_cov_relevancies TO pdsuser;


--
-- Name: SEQUENCE t_cov_relevancies_id_relevancy_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_cov_relevancies_id_relevancy_seq TO pdsuser;


--
-- Name: TABLE t_cov_rules; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_cov_rules TO pdsuser;


--
-- Name: SEQUENCE t_cov_rules_id_coveragerule_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_cov_rules_id_coveragerule_seq TO pdsuser;


--
-- Name: TABLE t_cov_x_filetypes; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_cov_x_filetypes TO pdsuser;


--
-- Name: TABLE t_cr_x_filetypes; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_cr_x_filetypes TO pdsuser;


--
-- Name: SEQUENCE t_cr_x_filetypes_id_cr_x_filetype_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_cr_x_filetypes_id_cr_x_filetype_seq TO pdsuser;


--
-- Name: TABLE t_crgroups; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_crgroups TO pdsuser;


--
-- Name: SEQUENCE t_crgroups_id_crgroup_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_crgroups_id_crgroup_seq TO pdsuser;


--
-- Name: TABLE t_crhistory; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_crhistory TO pdsuser;


--
-- Name: TABLE t_crrelations; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_crrelations TO pdsuser;


--
-- Name: SEQUENCE t_crrelations_id_crrelation_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_crrelations_id_crrelation_seq TO pdsuser;


--
-- Name: TABLE t_crtemporaries; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_crtemporaries TO pdsuser;


--
-- Name: TABLE t_datastoragequeue; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_datastoragequeue TO pdsuser;


--
-- Name: SEQUENCE t_datastoragequeue_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_datastoragequeue_id_seq TO pdsuser;


--
-- Name: TABLE t_datastoragetypes; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_datastoragetypes TO pdsuser;


--
-- Name: TABLE t_dbparameters; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_dbparameters TO pdsuser;


--
-- Name: SEQUENCE t_dbparameters_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_dbparameters_id_seq TO pdsuser;


--
-- Name: TABLE t_dbparametertypes; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_dbparametertypes TO pdsuser;


--
-- Name: TABLE t_dbstatus; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_dbstatus TO pdsuser;


--
-- Name: TABLE t_discacheinvrelation; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_discacheinvrelation TO pdsuser;


--
-- Name: TABLE t_discoveryplugins; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_discoveryplugins TO pdsuser;


--
-- Name: SEQUENCE t_discoveryplugins_id_discoveryplugin_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_discoveryplugins_id_discoveryplugin_seq TO pdsuser;


--
-- Name: TABLE t_disseminationpackingalgos; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_disseminationpackingalgos TO pdsuser;


--
-- Name: SEQUENCE t_disseminationpackingalgos_id_disseminationpackingalgo_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_disseminationpackingalgos_id_disseminationpackingalgo_seq TO pdsuser;


--
-- Name: TABLE t_distpoldataset; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_distpoldataset TO pdsuser;


--
-- Name: TABLE t_distributionalgo; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_distributionalgo TO pdsuser;


--
-- Name: TABLE t_distributionformat; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_distributionformat TO pdsuser;


--
-- Name: TABLE t_distributionitems; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_distributionitems TO pdsuser;


--
-- Name: SEQUENCE t_distributionitems_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_distributionitems_id_seq TO pdsuser;


--
-- Name: TABLE t_distributionqueue; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_distributionqueue TO pdsuser;


--
-- Name: SEQUENCE t_distributionqueue_packageid_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_distributionqueue_packageid_seq TO pdsuser;


--
-- Name: TABLE t_distributionrules; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_distributionrules TO pdsuser;


--
-- Name: SEQUENCE t_distributionrules_id_distributionrule_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_distributionrules_id_distributionrule_seq TO pdsuser;


--
-- Name: TABLE t_distributionruleslock; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_distributionruleslock TO pdsuser;


--
-- Name: TABLE t_distributionwaitingitems; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_distributionwaitingitems TO pdsuser;


--
-- Name: SEQUENCE t_distributionwaitingitems_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_distributionwaitingitems_id_seq TO pdsuser;


--
-- Name: TABLE t_distruleoriginator; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_distruleoriginator TO pdsuser;


--
-- Name: TABLE t_downlink_session_tracks; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_downlink_session_tracks TO pdsuser;


--
-- Name: SEQUENCE t_downlink_session_tracks_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_downlink_session_tracks_id_seq TO pdsuser;


--
-- Name: TABLE t_dsqueue_x_cirqueue; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_dsqueue_x_cirqueue TO pdsuser;


--
-- Name: SEQUENCE t_dsqueue_x_cirqueue_filequeue_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_dsqueue_x_cirqueue_filequeue_id_seq TO pdsuser;


--
-- Name: SEQUENCE t_dsqueue_x_cirqueue_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_dsqueue_x_cirqueue_id_seq TO pdsuser;


--
-- Name: TABLE t_emitted_order_events; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_emitted_order_events TO pdsuser;


--
-- Name: SEQUENCE t_emitted_order_events_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_emitted_order_events_id_seq TO pdsuser;


--
-- Name: TABLE t_fileclasses; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_fileclasses TO pdsuser;


--
-- Name: SEQUENCE t_fileclasses_id_fileclass_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_fileclasses_id_fileclass_seq TO pdsuser;


--
-- Name: SEQUENCE t_filetypes_id_filetype_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_filetypes_id_filetype_seq TO pdsuser;


--
-- Name: TABLE t_filetypes_last_inv; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_filetypes_last_inv TO pdsuser;


--
-- Name: SEQUENCE t_filetypes_last_inv_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_filetypes_last_inv_id_seq TO pdsuser;


--
-- Name: TABLE t_filetypes_x_migrations; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_filetypes_x_migrations TO pdsuser;


--
-- Name: SEQUENCE t_filetypes_x_migrations_id_filetypemigration_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_filetypes_x_migrations_id_filetypemigration_seq TO pdsuser;


--
-- Name: TABLE t_filetypes_x_satellites; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_filetypes_x_satellites TO pdsuser;


--
-- Name: SEQUENCE t_filetypes_x_satellites_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_filetypes_x_satellites_id_seq TO pdsuser;


--
-- Name: TABLE t_filetypes_x_stylesheets; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_filetypes_x_stylesheets TO pdsuser;


--
-- Name: SEQUENCE t_filetypes_x_stylesheets_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_filetypes_x_stylesheets_id_seq TO pdsuser;


--
-- Name: TABLE t_filetypescomps; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_filetypescomps TO pdsuser;


--
-- Name: SEQUENCE t_filetypescomps_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_filetypescomps_id_seq TO pdsuser;


--
-- Name: TABLE t_flowtags; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_flowtags TO pdsuser;


--
-- Name: SEQUENCE t_flowtags_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_flowtags_id_seq TO pdsuser;


--
-- Name: TABLE t_inv_additional_params; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_inv_additional_params TO pdsuser;


--
-- Name: SEQUENCE t_inv_additional_params_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_inv_additional_params_id_seq TO pdsuser;


--
-- Name: TABLE t_inv_baselines; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_inv_baselines TO pdsuser;


--
-- Name: SEQUENCE t_inv_baselines_id_inv_baseline_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_inv_baselines_id_inv_baseline_seq TO pdsuser;


--
-- Name: TABLE t_inv_client; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_inv_client TO pdsuser;


--
-- Name: SEQUENCE t_inv_client_id_inv_client_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_inv_client_id_inv_client_seq TO pdsuser;


--
-- Name: TABLE t_inv_data_restored; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_inv_data_restored TO pdsuser;


--
-- Name: TABLE t_inv_distributions; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_inv_distributions TO pdsuser;


--
-- Name: TABLE t_inv_downloads; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_inv_downloads TO pdsuser;


--
-- Name: TABLE t_inv_relation_types; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_inv_relation_types TO pdsuser;


--
-- Name: TABLE t_inv_relations; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_inv_relations TO pdsuser;


--
-- Name: SEQUENCE t_inv_relations_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_inv_relations_id_seq TO pdsuser;


--
-- Name: TABLE t_inv_x_baselines; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_inv_x_baselines TO pdsuser;


--
-- Name: SEQUENCE t_inv_x_baselines_id_inv_x_baseline_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_inv_x_baselines_id_inv_x_baseline_seq TO pdsuser;


--
-- Name: TABLE t_inventory; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_inventory TO pdsuser;


--
-- Name: SEQUENCE t_inventory_id_inv_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_inventory_id_inv_seq TO pdsuser;


--
-- Name: TABLE t_inventoryanomalies; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_inventoryanomalies TO pdsuser;


--
-- Name: SEQUENCE t_inventoryanomalies_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_inventoryanomalies_id_seq TO pdsuser;


--
-- Name: TABLE t_inventorycomments; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_inventorycomments TO pdsuser;


--
-- Name: SEQUENCE t_inventorycomments_id_inventorycomment_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_inventorycomments_id_inventorycomment_seq TO pdsuser;


--
-- Name: TABLE t_inventorydataset; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_inventorydataset TO pdsuser;


--
-- Name: TABLE t_inventoryevents; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_inventoryevents TO pdsuser;


--
-- Name: SEQUENCE t_inventoryevents_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_inventoryevents_id_seq TO pdsuser;


--
-- Name: TABLE t_inventoryeventsconf; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_inventoryeventsconf TO pdsuser;


--
-- Name: SEQUENCE t_inventoryeventsconf_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_inventoryeventsconf_id_seq TO pdsuser;


--
-- Name: TABLE t_inventoryextra; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_inventoryextra TO pdsuser;


--
-- Name: TABLE t_inventorygaps; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_inventorygaps TO pdsuser;


--
-- Name: SEQUENCE t_inventorygaps_inventorygapid_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_inventorygaps_inventorygapid_seq TO pdsuser;


--
-- Name: TABLE t_inventorylinks; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_inventorylinks TO pdsuser;


--
-- Name: SEQUENCE t_inventorylinks_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_inventorylinks_id_seq TO pdsuser;


--
-- Name: TABLE t_inventorylinksconf; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_inventorylinksconf TO pdsuser;


--
-- Name: SEQUENCE t_inventorylinksconf_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_inventorylinksconf_id_seq TO pdsuser;


--
-- Name: TABLE t_inventorysm; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_inventorysm TO pdsuser;


--
-- Name: SEQUENCE t_inventorysm_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_inventorysm_id_seq TO pdsuser;


--
-- Name: TABLE t_invrollingpolicies; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_invrollingpolicies TO pdsuser;


--
-- Name: SEQUENCE t_invrollingpolicies_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_invrollingpolicies_id_seq TO pdsuser;


--
-- Name: TABLE t_invsiterelation; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_invsiterelation TO pdsuser;


--
-- Name: SEQUENCE t_invsiterelation_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_invsiterelation_id_seq TO pdsuser;


--
-- Name: TABLE t_invspecialrollingpolicies; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_invspecialrollingpolicies TO pdsuser;


--
-- Name: SEQUENCE t_invspecialrollingpolicies_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_invspecialrollingpolicies_id_seq TO pdsuser;


--
-- Name: TABLE t_knownhosts; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_knownhosts TO pdsuser;


--
-- Name: SEQUENCE t_knownhosts_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_knownhosts_id_seq TO pdsuser;


--
-- Name: TABLE t_knownhosts_limits; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_knownhosts_limits TO pdsuser;


--
-- Name: SEQUENCE t_knownhosts_limits_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_knownhosts_limits_id_seq TO pdsuser;


--
-- Name: TABLE t_lastacknowledge; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_lastacknowledge TO pdsuser;


--
-- Name: SEQUENCE t_lastacknowledge_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_lastacknowledge_id_seq TO pdsuser;


--
-- Name: TABLE t_localtransfermodes; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_localtransfermodes TO pdsuser;


--
-- Name: TABLE t_measdataset; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_measdataset TO pdsuser;


--
-- Name: SEQUENCE t_measdataset_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_measdataset_id_seq TO pdsuser;


--
-- Name: TABLE t_mediacapformat; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_mediacapformat TO pdsuser;


--
-- Name: TABLE t_mediatypes; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_mediatypes TO pdsuser;


--
-- Name: TABLE t_migrationalgos; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_migrationalgos TO pdsuser;


--
-- Name: SEQUENCE t_migrationalgos_id_migrationalgo_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_migrationalgos_id_migrationalgo_seq TO pdsuser;


--
-- Name: SEQUENCE t_mpm_cross_id_cross_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_mpm_cross_id_cross_seq TO pdsuser;


--
-- Name: TABLE t_mpm_dip; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_mpm_dip TO pdsuser;


--
-- Name: SEQUENCE t_mpm_dip_id_dip_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_mpm_dip_id_dip_seq TO pdsuser;


--
-- Name: TABLE t_mpm_ipf_adf_check; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_mpm_ipf_adf_check TO pdsuser;


--
-- Name: SEQUENCE t_mpm_ipf_adf_check_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_mpm_ipf_adf_check_id_seq TO pdsuser;


--
-- Name: TABLE t_mpm_parameter_types; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_mpm_parameter_types TO pdsuser;


--
-- Name: SEQUENCE t_mpm_parameters_id_parameter_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_mpm_parameters_id_parameter_seq TO pdsuser;


--
-- Name: TABLE t_mpm_prb; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_mpm_prb TO pdsuser;


--
-- Name: SEQUENCE t_mpm_prb_id_prb_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_mpm_prb_id_prb_seq TO pdsuser;


--
-- Name: TABLE t_mpm_routines; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_mpm_routines TO pdsuser;


--
-- Name: SEQUENCE t_mpm_routines_id_routine_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_mpm_routines_id_routine_seq TO pdsuser;


--
-- Name: TABLE t_mpm_statuses; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_mpm_statuses TO pdsuser;


--
-- Name: TABLE t_mpm_timeliness; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_mpm_timeliness TO pdsuser;


--
-- Name: TABLE t_mpm_values; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_mpm_values TO pdsuser;


--
-- Name: SEQUENCE t_mpm_values_id_value_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_mpm_values_id_value_seq TO pdsuser;


--
-- Name: TABLE t_mpm_values_part1; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_mpm_values_part1 TO pdsuser;


--
-- Name: TABLE t_mpm_values_part2; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_mpm_values_part2 TO pdsuser;


--
-- Name: TABLE t_objsminvrelation; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_objsminvrelation TO pdsuser;


--
-- Name: TABLE t_ondemanddistribution; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_ondemanddistribution TO pdsuser;


--
-- Name: TABLE t_operation_requests; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_operation_requests TO pdsuser;


--
-- Name: SEQUENCE t_operation_requests_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_operation_requests_id_seq TO pdsuser;


--
-- Name: TABLE t_operationtypes; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_operationtypes TO pdsuser;


--
-- Name: TABLE t_ordersattachments; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_ordersattachments TO pdsuser;


--
-- Name: SEQUENCE t_ordersattachments_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_ordersattachments_id_seq TO pdsuser;


--
-- Name: TABLE t_ordersattachmenttypes; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_ordersattachmenttypes TO pdsuser;


--
-- Name: SEQUENCE t_ordersattachmenttypes_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_ordersattachmenttypes_id_seq TO pdsuser;


--
-- Name: TABLE t_ordersgeneration; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_ordersgeneration TO pdsuser;


--
-- Name: SEQUENCE t_ordersgeneration_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_ordersgeneration_id_seq TO pdsuser;


--
-- Name: TABLE t_ordersinput; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_ordersinput TO pdsuser;


--
-- Name: SEQUENCE t_ordersinput_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_ordersinput_id_seq TO pdsuser;


--
-- Name: TABLE t_ordersqueue; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_ordersqueue TO pdsuser;


--
-- Name: SEQUENCE t_ordersqueue_orderid_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_ordersqueue_orderid_seq TO pdsuser;


--
-- Name: TABLE t_ordersqueueblacklist; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_ordersqueueblacklist TO pdsuser;


--
-- Name: SEQUENCE t_ordersqueueblacklist_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_ordersqueueblacklist_id_seq TO pdsuser;


--
-- Name: TABLE t_orderstatetransitions; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_orderstatetransitions TO pdsuser;


--
-- Name: SEQUENCE t_orderstatetransitions_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_orderstatetransitions_id_seq TO pdsuser;


--
-- Name: TABLE t_ordertypes; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_ordertypes TO pdsuser;


--
-- Name: TABLE t_orderwaittrigger; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_orderwaittrigger TO pdsuser;


--
-- Name: TABLE t_ordtriggeringfiles; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_ordtriggeringfiles TO pdsuser;


--
-- Name: SEQUENCE t_ordtriggeringfiles_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_ordtriggeringfiles_id_seq TO pdsuser;


--
-- Name: TABLE t_originators; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_originators TO pdsuser;


--
-- Name: TABLE t_pdsmodes; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_pdsmodes TO pdsuser;


--
-- Name: SEQUENCE t_pdsmodes_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_pdsmodes_id_seq TO pdsuser;


--
-- Name: TABLE t_pendinginventory; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_pendinginventory TO pdsuser;


--
-- Name: TABLE t_postinvqueue; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_postinvqueue TO pdsuser;


--
-- Name: SEQUENCE t_postinvqueue_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_postinvqueue_id_seq TO pdsuser;


--
-- Name: TABLE t_proc_ftype_generation; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_proc_ftype_generation TO pdsuser;


--
-- Name: TABLE t_processors; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_processors TO pdsuser;


--
-- Name: SEQUENCE t_processors_id_processor_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_processors_id_processor_seq TO pdsuser;


--
-- Name: TABLE t_qualifiedflag; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_qualifiedflag TO pdsuser;


--
-- Name: SEQUENCE t_qualifiedflag_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_qualifiedflag_id_seq TO pdsuser;


--
-- Name: TABLE t_receptionrules; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_receptionrules TO pdsuser;


--
-- Name: SEQUENCE t_receptionrules_id_receptionrule_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_receptionrules_id_receptionrule_seq TO pdsuser;


--
-- Name: TABLE t_receptionruleshist; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_receptionruleshist TO pdsuser;


--
-- Name: SEQUENCE t_receptionruleshist_id_receptionrulehist_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_receptionruleshist_id_receptionrulehist_seq TO pdsuser;


--
-- Name: TABLE t_receptionruleslock; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_receptionruleslock TO pdsuser;


--
-- Name: TABLE t_repositories; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_repositories TO pdsuser;


--
-- Name: SEQUENCE t_repositories_id_repository_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_repositories_id_repository_seq TO pdsuser;


--
-- Name: TABLE t_rollingpoliciesalgo; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_rollingpoliciesalgo TO pdsuser;


--
-- Name: SEQUENCE t_rollingpoliciesalgo_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_rollingpoliciesalgo_id_seq TO pdsuser;


--
-- Name: TABLE t_rollingpolicyactions; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_rollingpolicyactions TO pdsuser;


--
-- Name: SEQUENCE t_rollingpolicyactions_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_rollingpolicyactions_id_seq TO pdsuser;


--
-- Name: TABLE t_satellites; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_satellites TO pdsuser;


--
-- Name: TABLE t_satrelations; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_satrelations TO pdsuser;


--
-- Name: TABLE t_schema; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_schema TO pdsuser;


--
-- Name: SEQUENCE t_schema_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_schema_id_seq TO pdsuser;


--
-- Name: TABLE t_schemamodificationtypes; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_schemamodificationtypes TO pdsuser;


--
-- Name: TABLE t_schematypes; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_schematypes TO pdsuser;


--
-- Name: TABLE t_securityzonerelays; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_securityzonerelays TO pdsuser;


--
-- Name: SEQUENCE t_securityzonerelays_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_securityzonerelays_id_seq TO pdsuser;


--
-- Name: TABLE t_securityzones; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_securityzones TO pdsuser;


--
-- Name: SEQUENCE t_securityzones_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_securityzones_id_seq TO pdsuser;


--
-- Name: TABLE t_sensors; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_sensors TO pdsuser;


--
-- Name: TABLE t_serials; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_serials TO pdsuser;


--
-- Name: TABLE t_sites; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_sites TO pdsuser;


--
-- Name: SEQUENCE t_sites_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_sites_id_seq TO pdsuser;


--
-- Name: TABLE t_sitetype; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_sitetype TO pdsuser;


--
-- Name: SEQUENCE t_sitetype_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_sitetype_id_seq TO pdsuser;


--
-- Name: TABLE t_sm_sync_missingactions; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_sm_sync_missingactions TO pdsuser;


--
-- Name: SEQUENCE t_sm_sync_missingactions_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_sm_sync_missingactions_id_seq TO pdsuser;


--
-- Name: TABLE t_smactions; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_smactions TO pdsuser;


--
-- Name: SEQUENCE t_smactions_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_smactions_id_seq TO pdsuser;


--
-- Name: TABLE t_smqueue; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_smqueue TO pdsuser;


--
-- Name: SEQUENCE t_smqueue_packageid_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_smqueue_packageid_seq TO pdsuser;


--
-- Name: TABLE t_smtype; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_smtype TO pdsuser;


--
-- Name: TABLE t_sor_requests; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_sor_requests TO pdsuser;


--
-- Name: SEQUENCE t_sor_requests_id_sor_request_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_sor_requests_id_sor_request_seq TO pdsuser;


--
-- Name: TABLE t_stafinvrelation; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_stafinvrelation TO pdsuser;


--
-- Name: TABLE t_standingdistribution; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_standingdistribution TO pdsuser;


--
-- Name: SEQUENCE t_standingdistribution_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_standingdistribution_id_seq TO pdsuser;


--
-- Name: TABLE t_statetransitions_allowed; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_statetransitions_allowed TO pdsuser;


--
-- Name: SEQUENCE t_statetransitions_allowed_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_statetransitions_allowed_id_seq TO pdsuser;


--
-- Name: TABLE t_statetransitions_flows; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_statetransitions_flows TO pdsuser;


--
-- Name: TABLE t_statetransitions_tables; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_statetransitions_tables TO pdsuser;


--
-- Name: SEQUENCE t_statetransitions_tables_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_statetransitions_tables_id_seq TO pdsuser;


--
-- Name: TABLE t_stati; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_stati TO pdsuser;


--
-- Name: TABLE t_stations; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_stations TO pdsuser;


--
-- Name: TABLE t_storagegroup; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_storagegroup TO pdsuser;


--
-- Name: SEQUENCE t_storagegroup_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_storagegroup_id_seq TO pdsuser;


--
-- Name: TABLE t_storagemanager; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_storagemanager TO pdsuser;


--
-- Name: SEQUENCE t_storagemanager_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_storagemanager_id_seq TO pdsuser;


--
-- Name: TABLE t_storagepriority; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_storagepriority TO pdsuser;


--
-- Name: TABLE t_storagerollingthresholds; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_storagerollingthresholds TO pdsuser;


--
-- Name: TABLE t_streamers; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_streamers TO pdsuser;


--
-- Name: SEQUENCE t_streamers_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_streamers_id_seq TO pdsuser;


--
-- Name: TABLE t_stylesheetoutputtypes; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_stylesheetoutputtypes TO pdsuser;


--
-- Name: TABLE t_tasktables; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_tasktables TO pdsuser;


--
-- Name: SEQUENCE t_tasktables_id_tasktable_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_tasktables_id_tasktable_seq TO pdsuser;


--
-- Name: TABLE t_temporarydistribution; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_temporarydistribution TO pdsuser;


--
-- Name: SEQUENCE t_temporarydistribution_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_temporarydistribution_id_seq TO pdsuser;


--
-- Name: TABLE t_temporaryorders; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_temporaryorders TO pdsuser;


--
-- Name: SEQUENCE t_temporaryorders_id_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_temporaryorders_id_seq TO pdsuser;


--
-- Name: TABLE t_tmpnamestrategy; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_tmpnamestrategy TO pdsuser;


--
-- Name: TABLE t_transferprotocols; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_transferprotocols TO pdsuser;


--
-- Name: TABLE t_usdfwa; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_usdfwa TO pdsuser;


--
-- Name: TABLE t_usdfwa_delivery_details; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_usdfwa_delivery_details TO pdsuser;


--
-- Name: SEQUENCE t_usdfwa_delivery_details_id_delivery_details_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_usdfwa_delivery_details_id_delivery_details_seq TO pdsuser;


--
-- Name: SEQUENCE t_usdfwa_id_usdfwa_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_usdfwa_id_usdfwa_seq TO pdsuser;


--
-- Name: TABLE t_usermodes; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_usermodes TO pdsuser;


--
-- Name: TABLE t_users; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_users TO pdsuser;


--
-- Name: SEQUENCE t_users_id_user_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_users_id_user_seq TO pdsuser;


--
-- Name: TABLE t_usertypes; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_usertypes TO pdsuser;


--
-- Name: TABLE t_waitingorders; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT,INSERT,DELETE,UPDATE ON TABLE public.t_waitingorders TO pdsuser;


--
-- Name: SEQUENCE t_waitingorders_waitorderid_seq; Type: ACL; Schema: public; Owner: acsdba
--

GRANT ALL ON SEQUENCE public.t_waitingorders_waitorderid_seq TO pdsuser;


--
-- Name: TABLE v_distribution; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT ON TABLE public.v_distribution TO pdsuser;


--
-- Name: TABLE v_distributionrules; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT ON TABLE public.v_distributionrules TO pdsuser;


--
-- Name: TABLE v_filetypes_x_stylesheets; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT ON TABLE public.v_filetypes_x_stylesheets TO pdsuser;


--
-- Name: TABLE v_mpm_cross; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT ON TABLE public.v_mpm_cross TO pdsuser;


--
-- Name: TABLE v_proc_ftype_generation; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT ON TABLE public.v_proc_ftype_generation TO pdsuser;


--
-- Name: TABLE v_receptionrules; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT ON TABLE public.v_receptionrules TO pdsuser;


--
-- Name: TABLE v_satrelations; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT ON TABLE public.v_satrelations TO pdsuser;


--
-- Name: TABLE v_schema_version; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT ON TABLE public.v_schema_version TO pdsuser;


--
-- Name: TABLE v_state_transitions; Type: ACL; Schema: public; Owner: acsdba
--

GRANT SELECT ON TABLE public.v_state_transitions TO pdsuser;


--
-- PostgreSQL database dump complete
--

