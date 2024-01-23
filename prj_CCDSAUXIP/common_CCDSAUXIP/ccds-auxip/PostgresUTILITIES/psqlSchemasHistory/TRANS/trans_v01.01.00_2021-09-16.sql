
-- Version 01.00.00
-- POSTGRES





-------------------------------------------------------------------------------
------------------------------- TABLES CREATION -------------------------------
-------------------------------------------------------------------------------

/* ------------------------------
	t_srv_statuses
*/ ------------------------------
INSERT INTO t_srv_statuses (id, status) VALUES (30, 'COLLAPSED');








-------------------------------------------------------------------------------
--------------------------------- CONCLUSIONS ---------------------------------
-------------------------------------------------------------------------------
DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.tables WHERE table_name = 't_schema') = 1
	THEN
		DELETE FROM t_schema WHERE version = 'trans-01.00.00' ;

		INSERT INTO t_schema (
				version, 
				modificationdate,
				modificationtype,
				schematype,
				cvstag,
				description )
		VALUES	(
				'trans-01.01.00',
				'2021-09-16',
				2,
				35,
				'TRANS_1-1-0',
				'Added new status (LTA-299)'
		) ;
	END IF ;
END $$;

