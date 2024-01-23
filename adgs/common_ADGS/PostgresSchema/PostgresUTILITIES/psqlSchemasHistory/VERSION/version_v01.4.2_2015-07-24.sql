
-- Version 1.4.2
-- POSTGRES






/* ------------------------------
	f_get_version_number
*/ ------------------------------
CREATE OR REPLACE FUNCTION f_get_version_number(in_cvs_tag VARCHAR(32)) 
	RETURNS INTEGER AS $$

DECLARE
	mj_vrs CHAR(3) ;
	md_vrs CHAR(3) ;
	mn_vrs CHAR(3) ;

BEGIN
  	mj_vrs := TO_CHAR((SPLIT_PART(SUBSTRING(in_cvs_tag FROM (POSITION('_' IN in_cvs_tag)+1) FOR (CHAR_LENGTH(in_cvs_tag))), '-', 1)::INT), '00') ;
	md_vrs := TO_CHAR((SPLIT_PART(in_cvs_tag, '-', 2)::INT), '00') ;
	mn_vrs := TO_CHAR((SPLIT_PART(in_cvs_tag, '-', 3)::INT), '00') ;
	
	RETURN (TRIM(mj_vrs) || TRIM(md_vrs) || TRIM(mn_vrs))::INT ;
END ;
$$ LANGUAGE plpgsql ;





/* ------------------------------
	v_schema_version
*/ ------------------------------
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


