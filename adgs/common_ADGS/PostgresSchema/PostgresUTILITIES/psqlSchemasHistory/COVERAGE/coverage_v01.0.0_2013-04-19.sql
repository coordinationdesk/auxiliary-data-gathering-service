
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
	deltamissing_sec INTEGER,
	rule_creation_date TIMESTAMP(6) NOT NULL,
	rule_last_update TIMESTAMP(6) DEFAULT NOW() NOT NULL,
	author VARCHAR(24) NOT NULL,
	description VARCHAR(128)
) ;

ALTER TABLE t_cov_rules 
	ADD CONSTRAINT pk_t_cov_rules
		PRIMARY KEY (id_coveragerule) ;

ALTER TABLE t_cov_rules 
	ADD CONSTRAINT ck_coveragerules01
		CHECK (rule_type IN ('A', 'B', 'D')) ;

ALTER TABLE t_cov_rules 
	ADD CONSTRAINT ck_coveragerules02
		CHECK (int_a > int_b) ;

ALTER TABLE t_cov_rules 
	ADD CONSTRAINT ck_coveragerules03
		CHECK ((n_min > 0) AND (n_max > 0)) ;

ALTER TABLE t_cov_rules 
	ADD CONSTRAINT ck_coveragerules04
		CHECK ((cover_pct >= 0) AND (cover_pct <= 100)) ;

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
	t_cov_queue
*/ ------------------------------
DROP TABLE IF EXISTS t_cov_queue CASCADE ;

CREATE TABLE t_cov_queue (
	id_coveragequeue BIGSERIAL NOT NULL,
	inv_id BIGINT NOT NULL,
	inv_ftype VARCHAR(16) NOT NULL,
	inv_date TIMESTAMP(6) NOT NULL,
	inv_val_start TIMESTAMP(6) NOT NULL,
    inv_val_stop TIMESTAMP(6) NOT NULL,
	validated BOOLEAN DEFAULT 'f' NOT NULL
) ;

ALTER TABLE t_cov_queue 
	ADD CONSTRAINT pk_t_cov_queue
		PRIMARY KEY (id_coveragequeue) ;

ALTER TABLE t_cov_queue 
	ADD CONSTRAINT fk_t_cov_queue01
		FOREIGN KEY (inv_id) REFERENCES t_inventory (id_inv) 
			ON DELETE CASCADE ;

CREATE INDEX ix_t_cov_queue01 ON t_cov_queue USING BTREE (inv_id) ;

GRANT SELECT, INSERT, UPDATE, DELETE ON TABLE t_cov_queue TO pdsuser ;
GRANT USAGE, SELECT, UPDATE ON SEQUENCE t_cov_queue_id_coveragequeue_seq TO pdsuser ;



/* ------------------------------
	t_cov_last_run
*/ ------------------------------
DROP TABLE IF EXISTS t_cov_last_run CASCADE ;

CREATE TABLE t_cov_last_run (
	validation TIMESTAMP(6),
	report TIMESTAMP(6)
) ;

ALTER TABLE t_cov_last_run 
	ADD CONSTRAINT pk_t_cov_last_run
		PRIMARY KEY (validation, report) ;

ALTER TABLE t_cov_last_run 
	ADD CONSTRAINT ck_cov_last_run01
		CHECK (validation < NOW() AND report < NOW()) ;

GRANT SELECT, INSERT, UPDATE, DELETE ON TABLE t_cov_last_run TO pdsuser ;



/* ------------------------------
	t_cov_logs
*/ ------------------------------
DROP TABLE IF EXISTS t_cov_logs CASCADE ;

CREATE TABLE t_cov_logs (
	id_interval BIGSERIAL NOT NULL,
	coveragerule_id INTEGER NOT NULL,
	t_begin TIMESTAMP(6) NOT NULL,
	t_end TIMESTAMP(6) NOT NULL,
	events INTEGER DEFAULT 0 NOT NULL,
	cover_pct FLOAT DEFAULT 0 NOT NULL,
	expect_value SMALLINT DEFAULT 2 NOT NULL,
	creationdate TIMESTAMP(6) DEFAULT NOW() NOT NULL
) ;

ALTER TABLE t_cov_logs 
	ADD CONSTRAINT pk_t_cov_logs
		PRIMARY KEY (id_interval) ;

ALTER TABLE t_cov_logs 
	ADD CONSTRAINT ck_coverage_logs01
		CHECK (expect_value IN ('0', '1', '2')) ;

ALTER TABLE t_cov_logs 
	ADD CONSTRAINT fk_coverageevents01
		FOREIGN KEY (coveragerule_id) REFERENCES t_cov_rules (id_coveragerule) 
			ON DELETE CASCADE ;

CREATE INDEX ix_t_cov_logs01 ON t_cov_logs USING BTREE (coveragerule_id) ;

GRANT SELECT, INSERT, UPDATE, DELETE ON TABLE t_cov_logs TO pdsuser ;
GRANT USAGE, SELECT, UPDATE ON SEQUENCE t_cov_logs_id_interval_seq TO pdsuser ;






/* ------------------------------
	p_cov_periods_planner
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_cov_periods_planner(
	in_replan INTEGER, 
	in_wait INTEGER,
	in_past INTEGER,
	in_future INTEGER,
	in_hold INTEGER,
	in_ruleid INTEGER) RETURNS INTEGER AS $$
	
DECLARE
	planning_period INTEGER ;
	periods_number INTEGER ;
	first_period TIMESTAMP ;
	interval_length INTEGER ;
	in_past_casted VARCHAR(10) ;
	in_hold_casted VARCHAR(10) ;
	ex_msg VARCHAR(128) ;
	r_id RECORD ;

BEGIN
	-- Check for in_ruleid input value (can't be a NULL value)
	IF in_ruleid IS NULL THEN
		RAISE EXCEPTION 'p_cov_periods_planner: value of in_ruleid input can not be NULL.' ;
		RETURN 1 ;
	END IF ;
	
	-- Check if in_future is greater then in_hold
	IF in_future < in_hold THEN
		RAISE EXCEPTION 'p_cov_periods_planner: the value of in_future must be greater then the value of in_hold.' ;
		RETURN 1 ;
	END IF ;
	
	
	-- Cast numbers to char for datetime operations
	in_past_casted := CAST(in_past AS VARCHAR) ;
	in_hold_casted := CAST(in_hold AS VARCHAR) ;
	
	-- Calculate the period to plan in seconds
	planning_period := in_future - in_hold ;
	
	
	-- If in_ruleid is not 0, will be replaced only the periods for the specific rule
	IF in_ruleid != 0 
	THEN		
		-- Check if rule exists
		-- If there aren't rules having the specific coveragerule_id will be raised an exception
		IF (SELECT count(*) FROM t_cov_rules WHERE id_coveragerule = in_ruleid) = 0 THEN
			ex_msg := 'p_cov_periods_planner: coverage rule ' || in_ruleid || ' do not exist.' ;
			RAISE EXCEPTION '%', ex_msg ;
			RETURN 1 ;
		END IF;
		
		-- Clean old periods only for specific rule (having specific coveragerule_id)
		DELETE FROM t_cov_periods WHERE coveragerule_id = in_ruleid AND t_end < (now() - in_past_casted::INTERVAL) ;
		DELETE FROM t_cov_periods WHERE coveragerule_id = in_ruleid AND t_begin > (now() + in_hold_casted::INTERVAL) ;
		
		-- Select the interval length of the period from the t_sec filed in the t_cov_rules table.
		interval_length := (SELECT t_sec FROM t_cov_rules WHERE id_coveragerule = in_ruleid) ;
		
		-- Calculate the number of periods to create dividing the total planning period by the length of intervals
		periods_number := planning_period / interval_length ;
		
		-- Set the first period to start
		-- If is the first plannig for a rule or if there are not records (cause delete) for the rule,
		-- the first period will be the time (only the time) of the value selected into time_0 of the rule,
		-- substituted to the time of the current date plus the "hold" value
		IF (SELECT count(*) FROM t_cov_periods WHERE coveragerule_id = in_ruleid) = 0 THEN
			first_period := (SELECT (date_trunc('day', now()) + time_0::TIME)::TIMESTAMP FROM t_cov_rules WHERE id_coveragerule = in_ruleid) + in_hold_casted::INTERVAL ;
		
		-- Else, if there are records into t_cov_periods, the first new period will be the last t_end
		-- found (the "holded" one) in the t_cov_periods for the rule
		ELSE
			first_period := (SELECT MAX(t_end) FROM t_cov_periods WHERE coveragerule_id = in_ruleid) ;
		END IF ;
		
				
		-- Recreate periods 
		FOR i IN 1..periods_number LOOP
			INSERT INTO t_cov_periods (
				coveragerule_id,
				t_begin,
				t_end)
			VALUES (
				in_ruleid,
				first_period,
				first_period + CAST(interval_length AS VARCHAR)::INTERVAL);
			
			first_period := first_period + CAST(interval_length AS VARCHAR)::INTERVAL ;
		END LOOP ;
	
	
	-- Else, if in_ruleid is 0 will be replanned periods for all rules
	ELSE
		-- Clean old periods  for all A and B type rule (having NOT NULL coveragerule_id)
		DELETE FROM t_cov_periods WHERE coveragerule_id IS NOT NULL AND t_end < (now() - in_past_casted::INTERVAL) ;
		DELETE FROM t_cov_periods WHERE coveragerule_id IS NOT NULL AND t_begin > (now() + in_hold_casted::INTERVAL) ;
		
		-- The same operations for the planning of a single role will be executed for multiple roles periods planning
		-- The only difference is the coveragerule_id, that will be calculated by a loop
		FOR r_id IN (
			SELECT id_coveragerule FROM t_cov_rules ) 
		LOOP
			interval_length := (SELECT t_sec FROM t_cov_rules WHERE id_coveragerule = r_id.id_coveragerule) ;
			periods_number := planning_period / interval_length ;
			
			IF (SELECT count(*) FROM t_cov_periods WHERE coveragerule_id = r_id.id_coveragerule) = 0 THEN
				first_period := (SELECT (date_trunc('day', now()) + time_0::TIME)::TIMESTAMP FROM t_cov_rules WHERE id_coveragerule = r_id.id_coveragerule) + in_hold_casted::INTERVAL ;
			ELSE
				first_period := (SELECT MAX(t_end) FROM t_cov_periods WHERE coveragerule_id = r_id.id_coveragerule) ;
			END IF ;
			
			FOR i IN 1..periods_number LOOP
				INSERT INTO t_cov_periods (
					coveragerule_id,
					t_begin,
					t_end)
				VALUES (
					r_id.id_coveragerule,
					first_period,
					first_period + CAST(interval_length AS VARCHAR)::INTERVAL);
				
				first_period := first_period + CAST(interval_length AS VARCHAR)::INTERVAL ;
			END LOOP ;
		END LOOP;
				
	END IF ;
	
	RETURN 0;
	
END ;
$$ LANGUAGE plpgsql ;






---------------------------------------------------------------------------------
--------------------------- TRIGGER FUNCTIONS CREATION --------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	tf_fill_coveragequeue
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_fill_coveragequeue() RETURNS TRIGGER AS $$

BEGIN	
	INSERT INTO t_cov_queue (
		inv_id, 
		inv_ftype,
		inv_date,
		inv_val_start,
		inv_val_stop) 
	VALUES (
		NEW.id_inv, 
		NEW.filetype,
		NEW.inventorydate,
		NEW.validitystart,
		NEW.validitystop
	) ;
	
	RETURN NEW ;
	
END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
------------------------------- TRIGGERS CREATION -------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_inventory
*/ ------------------------------
CREATE TRIGGER tr_inventoryinsert_cov AFTER INSERT
	ON t_inventory 
		FOR EACH ROW EXECUTE PROCEDURE tf_fill_coveragequeue() ;








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
		24,
		'COVERAGE_1-0-0',
		'Created coordinated release schema.'
) ;


