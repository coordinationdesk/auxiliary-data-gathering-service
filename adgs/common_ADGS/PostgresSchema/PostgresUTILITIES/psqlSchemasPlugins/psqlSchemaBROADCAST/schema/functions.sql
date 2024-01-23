/*

 Exprivia S.p.A.
 Via Della Bufalotta, 378 - 00139 Roma - Italy
 http://www.exprivia.it

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia S.p.A.;
 the contents of this file may not be disclosed to third parties, copied
 or duplicated in any form, in whole or in part, without the prior written
 permission of Exprivia S.p.A.

*/





--------------------------------------------------------------------------------
--                                                                            --
--                                  FUNCTIONS                                 --
--                                                                            --
--------------------------------------------------------------------------------

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

