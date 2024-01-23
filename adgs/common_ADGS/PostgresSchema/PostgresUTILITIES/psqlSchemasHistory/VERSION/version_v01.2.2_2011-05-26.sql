
-- Version 1.2.2
-- POSTGRES








---------------------------------------------------------------------------------
------------------------------- TABLES ALTERATION -------------------------------
---------------------------------------------------------------------------------

------------------------------
-- t_schema
------------------------------
DROP VIEW v_schema_version;

ALTER TABLE t_schema ALTER COLUMN version TYPE VARCHAR(24);

CREATE VIEW v_schema_version (
	"id",	
	"version", 
	"modification_date",
	"modification_type",
	"applied_on", 
	"schema_type", 
	"cvs_tag",
	"description")
AS SELECT 
	sc.id,
	sc.version, 
	sc.modificationdate,
	sm.modificationtype,
	sc.appliedon, 
	st.schematype, 
	sc.cvstag,
	sc.description
FROM
	t_schema AS sc 
		INNER JOIN t_schematypes AS st
			ON sc.schematype = st.id
		INNER JOIN t_schemamodificationtypes AS sm
			ON sc.modificationtype = sm.id
ORDER BY sc.id
;
