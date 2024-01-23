
-- Version 1.7.0
-- POSTGRES





-- #############################################################################
-- ##################                                         ##################
-- ##################                WARNING!!!               ##################
-- ##################   This patch can be installed only on   ##################
-- ##################       PDS 11.2.0 version or major       ##################
-- ##################                                         ##################
-- #############################################################################




---------------------------------------------------------------------------------
------------------------------- FUNCTION CREATION -------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_circulationusers
*/ ------------------------------
-- This function is needed only for this patch
CREATE OR REPLACE FUNCTION f_get_last_str_pos(
	in_string VARCHAR, 
	in_char VARCHAR) RETURNS INTEGER AS $$
BEGIN
	RETURN (SELECT LENGTH(in_string) - LENGTH(REGEXP_REPLACE(in_string, '.*' || in_char, ''))) ;
END ;
$$ LANGUAGE plpgsql ;








---------------------------------------------------------------------------------
-------------------------------- TABLES TRANSFER --------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_circulationusers
*/ ------------------------------
INSERT INTO t_repositories (
	symbolicname,
	transferprotocol_id,
	host_id,
	username,
	password,
	remotepath,
	tempname_id,
	tempname_value)
SELECT 
	symbolicname,
	transfermethod_id,
	host_id,
	username,
	password,
	remotepath,
	tempnamemode_id,
	tempnamevalue
FROM
	t_circulationusers ;


/* ------------------------------
	t_circulationpolicies
*/ ------------------------------
INSERT INTO t_repositories (
	symbolicname,
	transferprotocol_id,
	host_id,
	username,
	password,
	remotepath,
	tempname_id,
	tempname_value)
SELECT
	CASE WHEN (SELECT count(*) FROM t_repositories WHERE symbolicname LIKE subq.symb_name || '%') = 0
		THEN (subq.symb_name || subq.cnt)::VARCHAR
		ELSE (subq.symb_name || 
			(CAST((SELECT MAX(SUBSTRING(symbolicname FROM (f_get_last_str_pos(symbolicname, '_') +1) FOR 4)) AS max_val 
			FROM t_repositories
			WHERE symbolicname LIKE '%.%_%'
				AND (SUBSTRING(symbolicname FROM 0 FOR LENGTH(symbolicname)) = symb_name)
			GROUP BY SUBSTRING(symbolicname FROM 0 FOR LENGTH(symbolicname))) AS INTEGER) + subq.cnt))::VARCHAR
	END AS symnm,
	subq.prot_id,
	subq.hname_id,
	subq.usr,
	subq.passwd,
	subq.rpath,
	1 AS tmp_id,
	'.' AS tmp_val
FROM
	(SELECT DISTINCT
		(cp.usr || 'AT' || cp.hname || '_' || cp.prot || '_')::VARCHAR AS symb_name,
		(ROW_NUMBER() OVER (PARTITION BY cp.usr || 'AT' || cp.hname || '_' || cp.prot ORDER BY cp.usr || 'AT' || cp.hname || '_' || cp.prot)) AS cnt,
		tp.id AS prot_id,
		kh.id AS hname_id,
		cp.usr,
		cp.passwd,
		cp.rpath
	FROM
		(SELECT DISTINCT
			UPPER(SUBSTRING(dest_url FROM 0 FOR POSITION('://' IN dest_url))) AS prot,
			SUBSTRING(dest_url FROM (STRPOS(dest_url, '://') + 3) FOR 
				(SELECT LENGTH(dest_url) - LENGTH(REGEXP_REPLACE(dest_url, '.*' || ':', ''))) - LENGTH(SUBSTRING(dest_url FROM 0 FOR POSITION('://' IN dest_url))) -4) AS usr,
			SUBSTRING(dest_url FROM (SELECT LENGTH(dest_url) - LENGTH(REGEXP_REPLACE(dest_url, '.*' || ':', ''))) +1 FOR POSITION('@' IN dest_url) - 
				(SELECT LENGTH(dest_url) - LENGTH(REGEXP_REPLACE(dest_url, '.*' || ':', ''))) -1) AS passwd,
			SUBSTRING(dest_url FROM (STRPOS(dest_url, '@') + 1) FOR 
				(SELECT LENGTH(dest_url) - LENGTH(REGEXP_REPLACE(dest_url, '.*' || '//', ''))) - (STRPOS(dest_url, '@')+2)) AS hname,
			SUBSTRING(dest_url FROM (f_get_last_str_pos(dest_url, '//')) FOR LENGTH(dest_url)) AS rpath
		FROM 
			t_circulationpolicies) cp
		INNER JOIN t_transferprotocols tp
			ON cp.prot = tp.protocol
		INNER JOIN t_knownhosts kh
			ON cp.hname = kh.host_name
	ORDER BY
		symb_name
	) subq ;








---------------------------------------------------------------------------------
------------------------------ TABLES MODIFICATION ------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_circulationsqueue
*/ ------------------------------
ALTER TABLE t_circulationsqueue DROP CONSTRAINT fk_circulationsqueue07 ;
ALTER TABLE t_circulationsqueue 
	ADD CONSTRAINT fk_circulationsqueue07
		FOREIGN KEY (tempname_id) REFERENCES t_tmpnamestrategy (id_tmpnamestrategy) 
			ON DELETE CASCADE ;

ALTER TABLE t_circulationsqueue DROP CONSTRAINT ck_circulationsqueue02 ;
ALTER TABLE t_circulationsqueue
	ADD CONSTRAINT ck_circulationsqueue02
		CHECK ((tempname_id != 2 AND (tempnamevalue = '..' OR tempnamevalue = '../')) != TRUE) ;

ALTER TABLE t_circulationsqueue DROP CONSTRAINT ck_circulationsqueue03 ;
ALTER TABLE t_circulationsqueue
	ADD CONSTRAINT ck_circulationsqueue03
		CHECK ((tempname_id = 3 AND (tempnamevalue = '.' OR tempnamevalue = './')) != TRUE) ;


/* ------------------------------
	t_circulationpolicies
*/ ------------------------------
ALTER TABLE t_circulationpolicies DROP CONSTRAINT ck_circulationpolicies01 ;
ALTER TABLE t_circulationpolicies DROP CONSTRAINT ck_circulationpolicies02 ;
ALTER TABLE t_circulationpolicies DROP CONSTRAINT fk_circulationpolicies01 ;
ALTER TABLE t_circulationpolicies DROP CONSTRAINT fk_circulationpolicies02 ;
ALTER TABLE t_circulationpolicies DROP CONSTRAINT fk_circulationpolicies03 ;
ALTER TABLE t_circulationpolicies DROP CONSTRAINT fk_circulationpolicies04 ;

ALTER TABLE t_circulationpolicies ADD filetype_id2 INTEGER ;
UPDATE t_circulationpolicies SET filetype_id2 = subq.id_filetype FROM (
	SELECT id_filetype, filetype FROM t_filetypes) AS subq
		WHERE t_circulationpolicies.filetype_id = subq.filetype ;
ALTER TABLE t_circulationpolicies ALTER COLUMN filetype_id2 SET NOT NULL ;
ALTER TABLE t_circulationpolicies DROP COLUMN filetype_id ;
ALTER TABLE t_circulationpolicies RENAME COLUMN filetype_id2 TO filetype_id ;

ALTER TABLE t_circulationpolicies ADD source_id INTEGER ;
--UPDATE t_circulationpolicies SET source_id = (SELECT id_repository FROM t_repositories ORDER BY random() LIMIT 1) ;
UPDATE t_circulationpolicies SET source_id = r.id_repository FROM t_repositories r
	INNER JOIN t_circulationusers c ON r.symbolicname = c.symbolicname
		WHERE c.id = t_circulationpolicies.user_id ;
ALTER TABLE t_circulationpolicies ALTER COLUMN source_id SET NOT NULL ;

ALTER TABLE t_circulationpolicies ADD destination_id INTEGER ;
--UPDATE t_circulationpolicies SET destination_id = subq.id_repository FROM (
--	SELECT id_repository FROM t_repositories) AS subq
--		WHERE t_circulationpolicies.source_id != subq.id_repository ;
--UPDATE t_circulationpolicies SET destination_id = r.id_repository FROM t_repositories r
--	INNER JOIN t_circulationusers c ON r.symbolicname = c.symbolicname
--		WHERE c.id = t_circulationpolicies.user_id ;
UPDATE t_circulationpolicies SET destination_id = sub_q.id_repository FROM (
	SELECT 
		id_repository,
		(LOWER(tp.protocol) || '://' || username || ':' ||password ||'@' ||kh.host_name || '/' ||remotepath)::VARCHAR AS d_url
	FROM
		t_repositories rp
		INNER JOIN t_transferprotocols tp
			ON rp.transferprotocol_id = tp.id
		INNER JOIN t_knownhosts kh
			ON rp.host_id = kh.id) sub_q
	WHERE t_circulationpolicies.dest_url = sub_q.d_url ;
ALTER TABLE t_circulationpolicies ALTER COLUMN destination_id SET NOT NULL ;

ALTER TABLE t_circulationpolicies 
	ADD CONSTRAINT fk_circulationpolicies01
		FOREIGN KEY (circulator_node_id) REFERENCES t_knownhosts (id) 
			ON DELETE CASCADE ;
CREATE INDEX ix_t_circulationpolicies01 ON t_circulationpolicies USING BTREE (circulator_node_id) ;

ALTER TABLE t_circulationpolicies 
	ADD CONSTRAINT fk_circulationpolicies02
		FOREIGN KEY (filetype_id) REFERENCES t_filetypes (id_filetype) 
			ON DELETE CASCADE ;
CREATE INDEX ix_t_circulationpolicies02 ON t_circulationpolicies USING BTREE (filetype_id) ;

ALTER TABLE t_circulationpolicies 
	ADD CONSTRAINT fk_circulationpolicies03
		FOREIGN KEY (source_id) REFERENCES t_repositories (id_repository) 
			ON DELETE CASCADE ;
CREATE INDEX ix_t_circulationpolicies03 ON t_circulationpolicies USING BTREE (source_id) ;

ALTER TABLE t_circulationpolicies 
	ADD CONSTRAINT fk_circulationpolicies04
		FOREIGN KEY (destination_id) REFERENCES t_repositories (id_repository) 
			ON DELETE CASCADE ;
CREATE INDEX ix_t_circulationpolicies04 ON t_circulationpolicies USING BTREE (destination_id) ;

ALTER TABLE t_circulationpolicies
	ADD CONSTRAINT ck_circulationpolicies01			
		CHECK (source_id != destination_id) ;

ALTER TABLE t_circulationpolicies DROP COLUMN user_id ;
ALTER TABLE t_circulationpolicies DROP COLUMN dest_url ;
ALTER TABLE t_circulationpolicies DROP COLUMN source_path ;
ALTER TABLE t_circulationpolicies DROP COLUMN tempname_id ;
ALTER TABLE t_circulationpolicies DROP COLUMN tempnamevalue ;








--------------------------------------------------------------------------------
--------------------------------- TABLES DROP ----------------------------------
--------------------------------------------------------------------------------

-- t_knownhosts has been moved into PDS schema

-- t_circulationtempnames has been moved into PDS schema and renamed to t_tmpnamestrategy
DROP TABLE t_circulationtempnames CASCADE ;

DROP TABLE t_circulationusers CASCADE ;








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
		'circulation-1.7.0',
		'2013-07-10',
		6,
		20,
		'CIRCULATION_1-7-0',
		'Modified tables t_circulationsqueue and t_circulationpolicies. Dropped t_circulationusers and moved into PDS schema t_knownhosts and t_circulationtempnames (renamed to t_tmpnamestrategy).'
) ;


