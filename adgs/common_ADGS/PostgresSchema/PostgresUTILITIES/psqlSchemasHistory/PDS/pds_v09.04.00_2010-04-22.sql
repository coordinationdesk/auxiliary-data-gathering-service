
-- Version 9.4.0
-- POSTGRES








---------------------------------------------------------------------------------
------------------------------ TABLES MODIFICATION ------------------------------
---------------------------------------------------------------------------------

-------------------
--- T_INVENTORY ---
-------------------
ALTER TABLE t_inventory ADD cloud_pctg DOUBLE PRECISION;

ALTER TABLE t_inventory ADD prodspec VARCHAR(128);

ALTER TABLE t_inventory 
	ADD CONSTRAINT ck_inventory02
		CHECK ((cloud_pctg >= 0 ) AND (cloud_pctg <= 100 )) ;








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
		'pds-9.4.0',
		'2010-04-22',
		3,
		1,
		'SchemaVersion_9-4-0',
		'Modified table t_inventory.'
) ;
