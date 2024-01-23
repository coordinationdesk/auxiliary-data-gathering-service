
-- Version 10.1.0
-- POSTGRES








---------------------------------------------------------------------------------
---------------------------- PROCEDURES MODIFICATION ----------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	P_INSERTEVENTTABLEITEM
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_inserteventtableitem(mytable VARCHAR(64), myaction VARCHAR(16)) RETURNS INTEGER AS $$
DECLARE
	ret INTEGER;
BEGIN
	ret := f_acs_traceEvent('/usr/local/acs/tmp/PDS_events', mytable, myaction) ;
	RETURN ret;
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
		'pds-10.1.0',
		'2011-10-11',
		3,
		2,
		'PDS_10-1-0',
		'Changed procedure inserteventtableitem.'
) ;
