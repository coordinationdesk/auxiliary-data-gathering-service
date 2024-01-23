
-- Version 01.00.00
-- POSTGRES





--------------------------------------------------------------------------------
------------------------------ FUNCTIONS CREATION ------------------------------
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








-------------------------------------------------------------------------------
--------------------------------- CONCLUSIONS ---------------------------------
-------------------------------------------------------------------------------
DO $$
BEGIN
	IF EXISTS (SELECT FROM information_schema.tables WHERE table_name = 't_schema')
	THEN
		DELETE FROM t_schema WHERE version = 'common-01.00.00' ;

		INSERT INTO t_schema (
				version, 
				modificationdate,
				modificationtype,
				schematype,
				cvstag,
				description )
		VALUES	(
				'common-01.00.00',
				'2021-03-03',
				2,
				40,
				'COMMON_1-0-0',
				'First development'
		) ;
	END IF ;
END $$;


