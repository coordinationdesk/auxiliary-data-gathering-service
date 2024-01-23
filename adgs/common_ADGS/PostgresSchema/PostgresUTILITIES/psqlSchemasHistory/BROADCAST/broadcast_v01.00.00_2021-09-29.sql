

-- Version 01.00.00
-- POSTGRES








-------------------------------------------------------------------------------
------------------------------ FUNCTIONS CREATION -----------------------------
-------------------------------------------------------------------------------


/* ------------------------------
	f_getSubsystem
*/ ------------------------------
CREATE OR REPLACE FUNCTION f_getSubsystem(
	in_table_name VARCHAR(64)) RETURNS VARCHAR(64) AS $$

DECLARE
	subsystem VARCHAR(64);

BEGIN
	SELECT CASE 
		-- CIRCULATION
		WHEN SUBSTRING(in_table_name, 1, 6) = 't_circ' 
			OR SUBSTRING(in_table_name, 1, 10) = 't_datastor'
			OR SUBSTRING(in_table_name, 1, 10) = 't_security'
			OR in_table_name IN ('t_blockedcirculationsqueue', 't_dsqueue_x_cirqueue', 't_flowtags', 't_knownhosts_limits') THEN 'CIRCULATION'
		-- MPM
		WHEN SUBSTRING(in_table_name, 1, 5) = 't_mpm' THEN 'MPM' 
		-- ISM
		WHEN SUBSTRING(in_table_name, 1, 4) = 'ism_' THEN 'ISM' 
		-- PDS
		ELSE 'PDS' END
	INTO subsystem ;
	
	RETURN subsystem ;

END ;
$$ LANGUAGE plpgsql ;








-------------------------------------------------------------------------------
----------------------------- PROCEDURES CREATION -----------------------------
-------------------------------------------------------------------------------

/* ------------------------------
	p_events
*/ ------------------------------
CREATE OR REPLACE PROCEDURE p_events(
	payload TEXT) AS $$
	
BEGIN
	PERFORM pg_notify(current_database(), payload) ;
	
END ;
$$ LANGUAGE plpgsql ;








-------------------------------------------------------------------------------
-------------------------- TRIGGER FUNCTIONS CREATION -------------------------
-------------------------------------------------------------------------------

/* --------------------------
	tf_events
*/ --------------------------
CREATE OR REPLACE FUNCTION tf_events() RETURNS TRIGGER AS $$

DECLARE
	transition_table_name VARCHAR(64) ;
	pk_column_name VARCHAR(64) ;
	id_array BIGINT[] ;
	id_list TEXT ;
	sql_command TEXT ;
	affected_rows_count INTEGER ;
	action VARCHAR ;
	payload JSON ;
	
BEGIN
	-- Create the name of the transition table
	IF TG_OP = 'INSERT' THEN
		transition_table_name := LOWER(TG_TABLE_NAME || '_' || TG_OP || '_new') ;
	ELSE
		transition_table_name := LOWER(TG_TABLE_NAME || '_' || TG_OP || '_old') ;
	END IF;
	
	-- Retrieve the name of primary key field of the triggering table	
	sql_command := 'SELECT kcu.column_name FROM information_schema.table_constraints tco
					JOIN information_schema.key_column_usage kcu ON kcu.constraint_name = tco.constraint_name AND kcu.constraint_name = tco.constraint_name
					WHERE tco.constraint_type = ''PRIMARY KEY'' AND kcu.table_name = ''' || TG_TABLE_NAME || '''' ;
	EXECUTE sql_command INTO pk_column_name ;
	
	-- Retrieve the number of affected rows
	sql_command := 'SELECT count(*) FROM ' || transition_table_name ;
	EXECUTE sql_command INTO affected_rows_count ;
	
	-- Get the list of the affected ids
	sql_command := 'SELECT ARRAY_AGG(' || pk_column_name || ') FROM ' || transition_table_name ;
	EXECUTE sql_command INTO id_array ;
	
	-- Limit the list of the ids affected by query to 64 and cast the list to text
	IF (affected_rows_count <= 64) THEN
		id_list := REPLACE(TRIM(id_array::TEXT, '{}'), ',', ' ')::TEXT ;
	ELSE
		--id_list := REPLACE(ARRAY_TO_STRING(id_array[1:64], ','), ',', ' ') ;
		id_list := NULL ;
	END IF ;
	
	-- Set the action
	IF TG_ARGV[0] IS NOT NULL THEN
		-- If there is an argument, it will be the action ...
		action := TG_ARGV[0] ;
	ELSE
		-- else the action will be the operation that fired the trigger
		action := TG_OP ;
	END IF ;
	
	-- Create the JSON
	payload := JSON_BUILD_OBJECT(
			'serverip', inet_server_addr(),
			'clientip', inet_client_addr(),
			'user', current_user,
			'process', current_setting('application_name'),
			'pid', pg_backend_pid(),
			'database', current_database(),
			'subsystem', f_getSubsystem(TG_TABLE_NAME::VARCHAR(64)),
			'table', TG_TABLE_NAME,
			'action', action,
			'affectedrows', affected_rows_count,
			'id', id_list );
	
	-- Broadcast the event using as broadcast name the database name
	PERFORM pg_notify(current_database(), payload::TEXT) ;
		
	RETURN NULL ;
	
END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'broadcast-01.00.00' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'broadcast-01.00.00',
		'2021-09-28',
		5,
		42,
		'BROADCAST_1-0-0',
		'First development'
) ;



