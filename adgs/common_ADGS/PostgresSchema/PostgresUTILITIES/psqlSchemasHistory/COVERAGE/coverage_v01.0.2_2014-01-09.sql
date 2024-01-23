
-- Version 1.0.2
-- POSTGRES








---------------------------------------------------------------------------------
---------------------------- PROCEDURES MODIFICATION ----------------------------
---------------------------------------------------------------------------------

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
		'coverage-1.0.2',
		'2014-01-09',
		6,
		25,
		'COVERAGE_1-0-2',
		'Fixed bug on p_cov_periods_generator.'
) ;


