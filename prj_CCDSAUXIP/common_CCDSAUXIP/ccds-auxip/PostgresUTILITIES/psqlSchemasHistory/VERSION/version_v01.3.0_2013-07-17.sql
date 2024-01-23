
-- Version 1.3.0
-- POSTGRES








/* ------------------------------
        t_schematypes
*/ ------------------------------
UPDATE t_schematypes SET schematype = 'DATA MINING' WHERE id = 7 ;
UPDATE t_schematypes SET schematype = 'COORDINATED RELEASE' WHERE id = 24 ;
INSERT INTO t_schematypes (id, schematype) VALUES (25,'COVERAGE') ;



/* ------------------------------
	v_schema_version
*/ ------------------------------
DROP VIEW v_schema_version ;

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
		INNER JOIN (SELECT schematype, MAX(cvstag) AS maxtag FROM t_schema GROUP BY schematype) gsc
			ON sc.schematype = gsc.schematype AND sc.cvstag = gsc.maxtag
		INNER JOIN t_schematypes AS st
			ON sc.schematype = st.id
		INNER JOIN t_schemamodificationtypes AS sm
			ON sc.modificationtype = sm.id
ORDER BY 
	sc.version
;


