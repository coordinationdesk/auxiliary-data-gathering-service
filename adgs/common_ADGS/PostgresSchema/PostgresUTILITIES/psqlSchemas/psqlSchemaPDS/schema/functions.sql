/*

Copyright 2010-2015, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S. Databases schema functions script $

	$Id$

	$Author$
	
*/





--------------------------------------------------------------------------------
--
--    	  	  	  	  	  	  	  FUNCTIONS
--
--------------------------------------------------------------------------------


/* ------------------------------
	f_get_text_from_file
*/ ------------------------------
/*CREATE OR REPLACE FUNCTION f_get_text_from_file(fname TEXT)
	RETURNS TEXT AS $$

try:
	f = open(fname, "r")
except IOError:
	print 'ERROR!!! Cannot open file:', fname
	raise

dataTxt = f.read()
f.close()

return dataTxt

$$ LANGUAGE plpythonu;
*/



/* ------------------------------
	f_get_tasktable_id
*/ ------------------------------
CREATE OR REPLACE FUNCTION f_get_tasktable_id(tt_name VARCHAR(64)) RETURNS INT AS $$
DECLARE
	res INT ;
BEGIN

  	res := (SELECT id_tasktable FROM t_tasktables WHERE name = tt_name) ;
  	RETURN res ;

	EXCEPTION WHEN OTHERS THEN
	   RETURN -1 ;

END ;
$$ LANGUAGE plpgsql ;





/* ------------------------------
	f_compare_int_values
*/ ------------------------------
CREATE OR REPLACE FUNCTION f_compare_int_values(val1 INT, val2 INT) RETURNS BOOLEAN AS $$
BEGIN
	IF (val1 IS NULL AND val2 IS NULL) THEN
		RETURN TRUE ;
	ELSIF (val1 = val2) THEN
		RETURN TRUE ;
	ELSE
		RETURN FALSE ;
	END IF ;
END ;
$$ LANGUAGE plpgsql ;





/* ------------------------------
	f_interval_to_seconds
*/ ------------------------------
CREATE OR REPLACE FUNCTION f_interval_to_seconds(
	in_intrv INTERVAL
) RETURNS DOUBLE PRECISION AS $$

DECLARE
	p_sec DOUBLE PRECISION ;

BEGIN
	p_sec := EXTRACT(EPOCH  FROM in_intrv) ;
	
	RETURN p_sec ;
END ;
$$ LANGUAGE plpgsql ;





/* ------------------------------
	f_interval_to_seconds_diff
*/ ------------------------------
CREATE OR REPLACE FUNCTION f_interval_to_seconds_diff(
	beg_ts TIMESTAMP(6),
	end_ts TIMESTAMP(6)
) RETURNS DOUBLE PRECISION AS $$

DECLARE
	p_sec DOUBLE PRECISION ;

BEGIN
	p_sec := EXTRACT(EPOCH  FROM (end_ts - beg_ts)) ;
	
	RETURN p_sec ;
END ;
$$ LANGUAGE plpgsql ;





/* ------------------------------
	p_intervalminutes
*/ ------------------------------
CREATE FUNCTION p_intervalminutes(start TIMESTAMP(6), stop TIMESTAMP(6)) RETURNS DECIMAL(8,2) AS $$
DECLARE
	res DECIMAL(8,2) ;
BEGIN

	res := EXTRACT(EPOCH FROM AGE(stop, start)) / 60.0;
    RETURN res;

	EXCEPTION WHEN OTHERS THEN
	   RETURN 0 ;

END ;
$$ LANGUAGE plpgsql ;





/* ------------------------------
	p_weekoftheyear
*/ ------------------------------
CREATE FUNCTION p_weekoftheyear(dateValue TIMESTAMP(6))	RETURNS INT AS $$
DECLARE
	res INT ;
BEGIN

  	res := EXTRACT (WEEK FROM dateValue);
  	RETURN res ;

	EXCEPTION WHEN OTHERS THEN
	   RETURN -1 ;

END ;
$$ LANGUAGE plpgsql ;





/* ------------------------------
	p_intervalseconds
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_intervalseconds(start TIMESTAMP(6), stop TIMESTAMP(6)) RETURNS BIGINT AS $$
DECLARE
	res BIGINT ;
BEGIN

	res := EXTRACT(EPOCH FROM AGE(stop, start)) ;
    RETURN res;

	EXCEPTION WHEN OTHERS THEN
	   RETURN -99999999 ;

END ;
$$ LANGUAGE plpgsql ;





/* ------------------------------
	p_weekday
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_weekday(day TIMESTAMP(6)) RETURNS INTEGER AS $$
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
$$ LANGUAGE plpgsql ;





/* ------------------------------
	p_firtsdayoftheweek
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_firtsdayoftheweek(day TIMESTAMP(6)) RETURNS TIMESTAMP(6) AS $$
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
$$ LANGUAGE plpgsql ;





/* ------------------------------
	p_monthoftheyear
*/ ------------------------------
CREATE FUNCTION p_monthoftheyear(value TIMESTAMP(6)) RETURNS INTEGER AS $$
DECLARE
	res INTEGER ;
BEGIN

  	res := EXTRACT(MONTH FROM value) ;

	RETURN res;

END ;
$$ LANGUAGE plpgsql ;





/* ------------------------------
	p_deleteinventory
*/ ------------------------------
CREATE FUNCTION p_deleteinventory(
	filename VARCHAR(128), 
	fileversion VARCHAR(32)) RETURNS INTEGER AS $$

BEGIN

  	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;




------------------------------
-- f_delete_inventory_tree
------------------------------
CREATE OR REPLACE FUNCTION f_delete_inventory_tree(
	fname VARCHAR(128), 
	fversion VARCHAR(32))
RETURNS INTEGER AS $$

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
$$ LANGUAGE plpgsql ;





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


