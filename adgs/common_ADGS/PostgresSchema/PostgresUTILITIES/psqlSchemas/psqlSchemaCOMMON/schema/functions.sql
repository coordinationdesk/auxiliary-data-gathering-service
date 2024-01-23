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
--
--    	  	  	  	  	  	  	  FUNCTIONS
--
--------------------------------------------------------------------------------

/* ------------------------------
	f_get_inv_id_from_name 
*/ ------------------------------
CREATE OR REPLACE FUNCTION f_get_inv_id_from_name (in_file_name VARCHAR) RETURNS BIGINT AS $$

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
$$ LANGUAGE plpgsql ;


