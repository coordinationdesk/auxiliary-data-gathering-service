-- MP on november 2020 
-- the following query is used to discover the schema 
-- the table name to be described shall substitute the __TaBleNaMe__ string below
--
select DISTINCT att.attnum - 1 - 
	  ( SELECT COUNT(*) FROM pg_attribute att2 
  		WHERE att.attrelid = att2 .attrelid AND att2.attnum < att.attnum 
		AND   att2.attnum >0 AND att2.attisdropped ) as attnum , 
	CAST(att.attname as varchar) as fieldName,  		
	CAST(typ.typname as varchar) as proprietaryType, 		
	(CASE  		
		WHEN att.atttypid = 3802 THEN 16384 
		WHEN att.attlen = -1 AND  att.atttypmod	!= -1 THEN att.atttypmod - 4 
		WHEN att.attlen = -1 AND  att.atttypmod	= -1 THEN 0 	
		ELSE att.attlen 		
	END) as fieldSize, 		
	CAST ((CASE 		
		WHEN typ.typname = 'time' THEN 'HOUR' 		
		WHEN typ.typname = 'date' THEN 'YEAR' 		
		ELSE 'YEAR' 		
	END) as varchar) as dateTimeRangeFrom, 		
	CAST((CASE 		
		WHEN typ.typname = 'date' THEN 'DAY' 		
		ELSE  		
		(CASE 		
			WHEN att.atttypmod =0 THEN 'SECONDS' 		
			WHEN att.atttypmod =1 THEN 'FRACTION(1)' 		
			WHEN att.atttypmod =2 THEN 'FRACTION(2)' 		
			WHEN att.atttypmod =3 THEN 'FRACTION(3)' 		
			WHEN att.atttypmod =4 THEN 'FRACTION(4)' 		
			WHEN att.atttypmod =5 THEN 'FRACTION(5)' 		
			WHEN att.atttypmod =6 THEN 'FRACTION(6)' 		
			ELSE 'SECONDS'	
		END) 		
	END) as varchar) as dateTimeRangeTo, 		
	CAST((CASE  		
		WHEN conp.contype = 'p'  		
		THEN 'T' 		
		ELSE 'F' 		
	END) as varchar) as isPk, 		
	CAST((CASE  		
		WHEN att.attnotnull = 't'  		
		THEN 'T' 		
		ELSE 'F' 		
	END) as varchar) as isNotNull, 		
	CAST((CASE 		
		WHEN dep.classid IS NULL 		
		THEN 'F' 		
		ELSE 'T' 		
	END) as varchar) as isSerialString, 		
	CAST((CASE  		
		WHEN (dep.classid IS NOT NULL OR att.atthasdef = 'f' )		
		THEN 'F' 		
		ELSE 'T' 		
	END) as varchar) as hasDefaultString, 		
	CAST (cl.relname as varchar) as foreignTable, 		
	attfk.attnum -1 as foreignField,		
	CAST (attfk.attname  as varchar) as foreignFieldName,		
	0 as unused, 
    CAST((CASE 		
		WHEN conp.contype = 'u' OR  conp.contype = 'p'		
		THEN 'T'		
		ELSE 'F' 		
	END) as varchar) as isUnique		
	from pg_attribute att 		
	INNER JOIN pg_type typ ON 		
	( 		
		att.atttypid = typ.oid 		
	) 		
	INNER JOIN pg_class cl2 ON 		
	( 		
		cl2.relname = '__TaBleNaMe__'  		
		AND att.attrelid = cl2.oid 		
		AND att.attnum >= 1  		
		AND att.attisdropped = 'f' 		
	) 		
	LEFT JOIN 		
	( 		
		pg_constraint conf 		
		LEFT JOIN pg_class cl ON 		
		( 		
			conf.confrelid = cl.oid  		
		) 		
		LEFT JOIN pg_attribute attfk ON 		
		( 		
			attfk.attrelid = cl.oid  		
		) 		
	) ON ( 		
		att.attrelid = conf.conrelid 		
		AND (conf.contype = 'f' ) 		
		AND ((attfk.attnum = conf.confkey[1] AND  att.attnum = conf.conkey[1] ) OR 
		(attfk.attnum = conf.confkey[2] AND  att.attnum = conf.conkey[2])) 
	) 		
	LEFT JOIN 	pg_constraint conp 	ON	
	( 		
		att.attrelid = conp.conrelid 		
--		AND (conp.contype IN ('p', 'u') ) 
		AND ( ( (conp.contype = 'p') AND att.attnum = ANY (conp.conkey) ) OR ( (conp.contype = 'u') AND att.attnum = conp.conkey[1] AND array_length(conp.conkey, 1) = 1) ) 
--		AND att.attnum = ANY (conp.conkey) 		
	) 		
	LEFT JOIN   		
	( 		
		pg_depend dep  		
		INNER JOIN pg_class cl3 ON 		
		( 		
		dep.objid = cl3.oid 		
		AND cl3.relkind = 'S' 		
		) 		
	) ON ( 		
		dep.refobjid = cl2.oid 		
		AND dep.refobjsubid = att.attnum 		
	)  		
	order by attnum
