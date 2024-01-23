
-- Version 1.5.0
-- POSTGRES








/* ------------------------------
        t_schematypes
*/ ------------------------------
INSERT INTO t_schematypes (id, schematype, description) VALUES (29, 'SNAV', 'SpaceNav schema') ;


/* ------------------------------
	t_schema
*/ ------------------------------
-- drop view to change the field:
DROP VIEW v_schema_version ;

ALTER TABLE t_schema ALTER COLUMN description TYPE TEXT ;

-- recreate view
CREATE OR REPLACE VIEW v_schema_version (
	version, 
	schema_type,
	modification_type,
	modification_date,
	applied_on,  
	cvs_tag,
	description)
AS SELECT 
	sc.version, 
	st.schematype, 
	sm.modificationtype,
	sc.modificationdate,
	sc.appliedon, 
	sc.cvstag,
	sc.description
FROM
	t_schema AS sc 
		INNER JOIN (SELECT schematype, MAX(f_get_version_number(cvstag)) AS maxtag FROM t_schema GROUP BY schematype) gsc
			ON sc.schematype = gsc.schematype AND f_get_version_number(sc.cvstag) = gsc.maxtag
		INNER JOIN t_schematypes AS st
			ON sc.schematype = st.id
		INNER JOIN t_schemamodificationtypes AS sm
			ON sc.modificationtype = sm.id
ORDER BY 
	sc.version
;


