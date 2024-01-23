
-- POSTGRES





-- ########################################################################### --
-- ##################                                        ################# --
-- ##################             coverage-1.0.0             ################# --
-- ##################               2013-04-19               ################# --
-- ##################               TEST CASE                ################# --
-- ##################                                        ################# --
-- ########################################################################### --




--------------------------
-- TABLES CONFIGURATION --
--------------------------
/* ------------------------------
	t_cov_periods
*/ ------------------------------
-- Clean all periods and reset serial
DELETE FROM t_cov_periods WHERE 1=1;
SELECT setval('t_cov_periods_id_interval_seq', 1);


/* ------------------------------
	t_cov_rules
*/ ------------------------------
-- Clean all rules and reset serial
DELETE FROM t_cov_rules WHERE 1=1;
SELECT setval('t_cov_rules_id_coveragerule_seq', 1);
-- Recreate test rules
INSERT INTO t_cov_rules (
	rule_type,
	time_0,
	t_sec,
	int_a,
	int_b,
	deltawait_sec,
	deltamissing_sec,
	author,
	description)
VALUES (
	'A',
	'2013-07-10 05:00:00',
	25200,
	86400,
	-86400,
	300,
	10800,
	'Achillino',
	'Regola di prova visto che non ci sto capendo un caxxo'
) ;
INSERT INTO t_cov_rules (
	rule_type,
	time_0,
	t_sec,
	int_a,
	int_b,
	deltawait_sec,
	deltamissing_sec,
	author,
	description)
VALUES (
	'A',
	'2013-07-21 12:00:00',
	43200,
	86400,
	43200,
	600,
	10800,
	'Achillino',
	'Regola prova 2'
) ;
INSERT INTO t_cov_rules (
	rule_type,
	time_0,
	t_sec,
	int_a,
	int_b,
	deltawait_sec,
	deltamissing_sec,
	author,
	description)
VALUES (
	'A',
	'2013-07-31 12:00:00',
	43200,
	86400,
	0,
	100,
	10800,
	'Achillino',
	'Regola prova 3'
) ;
INSERT INTO t_cov_rules (
	rule_type,
	time_0,
	t_sec,
	int_a,
	int_b,
	n_min,
	n_max,
	delta_gap,
	cover_pct,
	check_delta_gap,
	check_cover_pct,
	deltawait_sec,
	deltamissing_sec,
	author,
	description)
VALUES (
	'B',
	'2013-08-01 00:00:00',
	43200,
	86400,
	0,
	1,
	10,
	600,
	60,
	't',
	't',
	90,
	10800,
	'Achillino',
	'Regola prova 4'
) ;
INSERT INTO t_cov_rules (
	rule_type,
	time_0,
	t_sec,
	int_a,
	int_b,
	n_min,
	n_max,
	delta_gap,
	cover_pct,
	check_delta_gap,
	check_cover_pct,
	deltawait_sec,
	deltamissing_sec,
	author,
	description)
VALUES (
	'B',
	'2014-01-14 18:10:00',
	300,
	3600,
	600,
	3,
	5,
	0,
	80,
	't',
	't',
	10,
	100,
	'Achillino',
	'Regola prova per sovraposizione validity'
) ;


/* ------------------------------
	t_filetypes
*/ ------------------------------
-- Clean COV TEST filetypes
DELETE FROM t_filetypes WHERE filetype LIKE 'AUX_COV_%' ;
-- Rereate COV TEST filetypes
INSERT INTO t_filetypes (
	filetype,
	description,
	nameregularexpression,
	"group" )
VALUES (
	'AUX_COV_A1',
	'Filetype di prova per coverage validation',
	'^.*AUX_COV_A1.*$',
	'group0'
) ;
INSERT INTO t_filetypes (
	filetype,
	description,
	nameregularexpression,
	"group" )
VALUES (
	'AUX_COV_A2',
	'Filetype di prova per coverage validation',
	'^.*AUX_COV_A2.*$',
	'group0'
) ;
INSERT INTO t_filetypes (
	filetype,
	description,
	nameregularexpression,
	"group" )
VALUES (
	'AUX_COV_B1',
	'Filetype di prova per coverage validation',
	'^.*AUX_COV_B1.*$',
	'group0'
) ;
INSERT INTO t_filetypes (
	filetype,
	description,
	nameregularexpression,
	"group" )
VALUES (
	'AUX_COV_B2',
	'Filetype di prova per coverage validation',
	'^.*AUX_COV_B2.*$',
	'group0'
) ;
INSERT INTO t_filetypes (
	filetype,
	description,
	nameregularexpression,
	"group" )
VALUES (
	'AUX_COV_C1',
	'Filetype di prova per coverage validation',
	'^.*AUX_COV_C1.*$',
	'group0'
) ;


/* ------------------------------
	t_cov_x_filetypes
*/ ------------------------------
-- Clean all relations
DELETE FROM t_cov_x_filetypes WHERE 1=1;
-- Recreate relations
INSERT INTO t_cov_x_filetypes VALUES (2, (SELECT id_filetype FROM t_filetypes WHERE filetype = 'AUX_COV_A1')) ;
INSERT INTO t_cov_x_filetypes VALUES (3, (SELECT id_filetype FROM t_filetypes WHERE filetype = 'AUX_COV_A2')) ;
INSERT INTO t_cov_x_filetypes VALUES (4, (SELECT id_filetype FROM t_filetypes WHERE filetype = 'AUX_COV_A2')) ;
INSERT INTO t_cov_x_filetypes VALUES (5, (SELECT id_filetype FROM t_filetypes WHERE filetype = 'AUX_COV_B1')) ;
INSERT INTO t_cov_x_filetypes VALUES (6, (SELECT id_filetype FROM t_filetypes WHERE filetype = 'AUX_COV_B2')) ;





----------------------
-- POPULATE PERIODS --
----------------------
/* ------------------------------
	p_cov_periods_generator
*/ ------------------------------
SELECT p_cov_periods_generator(86400,1,2592000,2592000,604800) ;
UPDATE t_cov_periods SET 
	t_begin = (t_begin - interval '3 weeks'), 
	t_end = (t_end - interval '3 weeks'),
	exp_val_start = (exp_val_start - interval '3 weeks'),
	exp_val_stop = (exp_val_stop - interval '3 weeks');
SELECT p_cov_periods_generator(0,1,2592000,2592000,604800) ;

/* ------------------------------
	p_cov_ondemand_event
*/ ------------------------------
SELECT p_cov_ondemand_event((SELECT id_filetype FROM t_filetypes WHERE filetype = 'AUX_COV_C1'),now()::TIMESTAMP,43200,43200,now()::TIMESTAMP,(now() + interval '86400 second')::TIMESTAMP,'Achillino','Regola di prova C');



--------------------------
-- POPULATE INVENTORIES --
--------------------------
/* ------------------------------
	t_inventory
*/ ------------------------------
-- Clean COV TEST inventories
DELETE FROM t_inventory WHERE filetype LIKE 'AUX_COV_%' ;

-- Rereate COV TEST inventories
-- This inventory don't satisfy any check for rule A 
INSERT INTO t_inventory(
	filename, 
	fileversion, 
	generationtime, 
	validitystart, 
	validitystop, 
	validityflag, 
	validationdate, 
	inventorydate,
	filetype, 
	fileclass)
VALUES (
	'prova_coverage_rule_a',
	(SELECT 
		CASE 
			WHEN max(fileversion) IS NULL THEN '0001'
			ELSE trim(to_char(to_number(max(fileversion), '9999') + 1, '0000')) 
		END 
	FROM t_inventory WHERE filename = 'prova_coverage_rule_a'),
	now(),
	'1978-06-06'::TIMESTAMP,
	now(),
	'f',
	now(),
	'1978-06-06'::TIMESTAMP,
	'AUX_COV_A1',
	'TEST'
);

-- This inventory satisfy "expect" check for rule A 
INSERT INTO t_inventory(
	filename, 
	fileversion, 
	generationtime, 
	validitystart, 
	validitystop, 
	validityflag, 
	validationdate, 
	inventorydate,
	filetype, 
	fileclass)
VALUES (
	'prova_coverage_rule_a',
	(SELECT 
		CASE 
			WHEN max(fileversion) IS NULL THEN '0001'
			ELSE trim(to_char(to_number(max(fileversion), '9999') + 1, '0000')) 
		END 
	FROM t_inventory WHERE filename = 'prova_coverage_rule_a'),
	now(),
	'1978-06-06'::TIMESTAMP,
	(SELECT exp_val_stop FROM t_cov_periods WHERE coveragerule_id = 2 ORDER BY random() LIMIT 1),
	'f',
	now(),
	(SELECT t_end FROM t_cov_periods WHERE coveragerule_id = 2 ORDER BY random() LIMIT 1),
	'AUX_COV_A1',
	'TEST'
);
UPDATE t_inventory SET validitystop = (validitystop - '7200'::INTERVAL), validitystart = (validitystop - '180000'::INTERVAL) 
WHERE validitystart = '1978-06-06'::TIMESTAMP AND id_inv = (SELECT MAX(id_inv) FROM t_inventory) ;

-- This inventory satisfy "cover" check for rule A 
INSERT INTO t_inventory(
	filename, 
	fileversion, 
	generationtime, 
	validitystart, 
	validitystop, 
	validityflag, 
	validationdate, 
	inventorydate,
	filetype, 
	fileclass)
VALUES (
	'prova_coverage_rule_a',
	(SELECT 
		CASE 
			WHEN max(fileversion) IS NULL THEN '0001'
			ELSE trim(to_char(to_number(max(fileversion), '9999') + 1, '0000')) 
		END 
	FROM t_inventory WHERE filename = 'prova_coverage_rule_a'),
	now(),
	'1978-06-06'::TIMESTAMP,
	(SELECT exp_val_stop FROM t_cov_periods WHERE coveragerule_id = 2 ORDER BY random() LIMIT 1),
	'f',
	now(),
	now(),
	'AUX_COV_A1',
	'TEST'
);
UPDATE t_inventory SET validitystart = (validitystop - '172800'::INTERVAL) 
WHERE validitystart = '1978-06-06'::TIMESTAMP AND id_inv = (SELECT MAX(id_inv) FROM t_inventory) ;

-- This inventory satisfy both checks for rule A 
INSERT INTO t_inventory(
	filename, 
	fileversion, 
	generationtime, 
	validitystart, 
	validitystop, 
	validityflag, 
	validationdate, 
	inventorydate,
	filetype, 
	fileclass)
VALUES (
	'prova_coverage_rule_a',
	(SELECT 
		CASE 
			WHEN max(fileversion) IS NULL THEN '0001'
			ELSE trim(to_char(to_number(max(fileversion), '9999') + 1, '0000')) 
		END 
	FROM t_inventory WHERE filename = 'prova_coverage_rule_a'),
	now(),
	'1978-06-06'::TIMESTAMP,
	(SELECT exp_val_stop FROM t_cov_periods WHERE coveragerule_id = 2 ORDER BY random() LIMIT 1),
	'f',
	now(),
	(SELECT t_end FROM t_cov_periods WHERE coveragerule_id = 2 ORDER BY random() LIMIT 1),
	'AUX_COV_A1',
	'TEST'
);
UPDATE t_inventory SET validitystop = (validitystop - '100'::INTERVAL), validitystart = (validitystop - '172900'::INTERVAL) 
WHERE validitystart = '1978-06-06'::TIMESTAMP AND id_inv = (SELECT MAX(id_inv) FROM t_inventory) ;

-- This inventory satisfy "ontime" check for rule B 
INSERT INTO t_inventory(
	filename, 
	fileversion, 
	generationtime, 
	validitystart, 
	validitystop, 
	validityflag, 
	validationdate, 
	inventorydate,
	filetype, 
	fileclass)
(SELECT
	'prova_coverage_rule_b',
	(SELECT 
		CASE 
			WHEN max(fileversion) IS NULL THEN '0001'
			ELSE trim(to_char(to_number(max(fileversion), '9999') + 1, '0000')) 
		END 
	FROM t_inventory WHERE filename = 'prova_coverage_rule_b'),
	now(),
	exp_val_start + '3600'::INTERVAL,
	exp_val_stop - '3600'::INTERVAL,
	'f',
	now(),
	t_end - '3600'::INTERVAL,
	'AUX_COV_B1',
	'TEST'
FROM
	t_cov_periods
WHERE
	coveragerule_id = 5
ORDER BY 
	random() LIMIT 1
);

-- This inventory don't satisfy "ontime" check for rule B 
INSERT INTO t_inventory(
	filename, 
	fileversion, 
	generationtime, 
	validitystart, 
	validitystop, 
	validityflag, 
	validationdate, 
	inventorydate,
	filetype, 
	fileclass)
(SELECT
	'prova_coverage_rule_b2',
	(SELECT 
		CASE 
			WHEN max(fileversion) IS NULL THEN '0001'
			ELSE trim(to_char(to_number(max(fileversion), '9999') + 1, '0000')) 
		END 
	FROM t_inventory WHERE filename = 'prova_coverage_rule_b2'),
	now(),
	exp_val_start + '3600'::INTERVAL,
	exp_val_stop - '3600'::INTERVAL,
	'f',
	now(),
	t_end - '3600'::INTERVAL,
	'AUX_COV_B2',
	'TEST'
FROM
	t_cov_periods
WHERE
	coveragerule_id = 5
ORDER BY 
	random() LIMIT 1
);

-- This inventory don't satisfy any check for rule C 
INSERT INTO t_inventory(
	filename, 
	fileversion, 
	generationtime, 
	validitystart, 
	validitystop, 
	validityflag, 
	validationdate, 
	inventorydate,
	filetype, 
	fileclass)
VALUES (
	'prova_coverage_rule_c',
	(SELECT 
		CASE 
			WHEN max(fileversion) IS NULL THEN '0001'
			ELSE trim(to_char(to_number(max(fileversion), '9999') + 1, '0000')) 
		END 
	FROM t_inventory WHERE filename = 'prova_coverage_rule_c'),
	now(),
	'1978-06-06'::TIMESTAMP,
	now(),
	'f',
	now(),
	'1978-06-06'::TIMESTAMP,
	'AUX_COV_C1',
	'TEST'
);

-- This inventory satisfy "expect" check for rule C 
INSERT INTO t_inventory(
	filename, 
	fileversion, 
	generationtime, 
	validitystart, 
	validitystop, 
	validityflag, 
	validationdate, 
	inventorydate,
	filetype, 
	fileclass)
VALUES (
	'prova_coverage_rule_c',
	(SELECT 
		CASE 
			WHEN max(fileversion) IS NULL THEN '0001'
			ELSE trim(to_char(to_number(max(fileversion), '9999') + 1, '0000')) 
		END 
	FROM t_inventory WHERE filename = 'prova_coverage_rule_c'),
	now(),
	'1978-06-06'::TIMESTAMP,
	(SELECT exp_val_stop FROM t_cov_periods WHERE coveragerule_id = 2 ORDER BY random() LIMIT 1),
	'f',
	now(),
	(SELECT t_end FROM t_cov_periods WHERE coveragerule_id = 2 ORDER BY random() LIMIT 1),
	'AUX_COV_C1',
	'TEST'
);
UPDATE t_inventory SET validitystop = (validitystop - '7200'::INTERVAL), validitystart = (validitystop - '180000'::INTERVAL) 
WHERE validitystart = '1978-06-06'::TIMESTAMP AND id_inv = (SELECT MAX(id_inv) FROM t_inventory);






/*
SELECT t.id, MIN(t.g) FROM 
(SELECT 
	b.id AS id, LEAST(a.id, b.id) as g
	--CASE 
	--	WHEN (a.campo1, a.campo2) OVERLAPS (b.campo1, b.campo2) THEN
	--		EXTRACT(EPOCH FROM GREATEST(b.campo2, a.campo2) - LEAST(a.campo1, b.campo1))::INTEGER
	--	ELSE 
	--		(EXTRACT(EPOCH FROM a.campo2 - a.campo1)::INTEGER + EXTRACT(EPOCH FROM b.campo2 - b.campo1)::INTEGER)
--
	--END AS somma
FROM
	prova_date a
	INNER JOIN prova_date b
		ON a.periodo = b.periodo AND b.id >= a.id
WHERE
	(a.campo1, a.campo2) OVERLAPS (b.campo1, b.campo2)
ORDER BY g, b.id) t
group by t.id

--SELECT (TIMESTAMP '2013-01-01 03:00:00', TIMESTAMP '2013-01-01 05:00:00') OVERLAPS (TIMESTAMP '2013-01-01 03:01:00', TIMESTAMP '2013-01-01 03:02:00')
*/
