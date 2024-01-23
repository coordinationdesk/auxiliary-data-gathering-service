
-- Version 11.6.6
-- POSTGRES








---------------------------------------------------------------------------------
--------------------------------- TABLE CHANGES ---------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_sor_requests
*/ ------------------------------
ALTER TABLE t_sor_requests DROP inventory_id ;








---------------------------------------------------------------------------------
------------------------------- FUNCTION CREATION -------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	f_sor_generation
*/ ------------------------------
CREATE OR REPLACE FUNCTION f_sor_generation() 
	RETURNS CHAR AS $$

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
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------
DELETE FROM t_schema WHERE version = 'pds-11.6.6' ;
INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-11.6.6',
		'2014-09-22',
		3,
		2,
		'PDS_11-6-6',
		'Dropeed inventory_id field on t_sor_requests. Added function f_sor_generation.'
) ;


