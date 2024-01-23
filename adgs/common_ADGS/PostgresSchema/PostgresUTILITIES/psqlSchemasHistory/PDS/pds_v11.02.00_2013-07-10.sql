
-- Version 11.2.0
-- POSTGRES








---------------------------------------------------------------------------------
---------------------------------- VIEWS DROP -----------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	v_distributionrules
*/ ------------------------------
DROP VIEW v_distributionrules ; -- will be recreated at the end








---------------------------------------------------------------------------------
-------------------------------- TABLES CREATION --------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_knownhosts
*/ ------------------------------

-- Table t_knownhosts will be moved from CIRCULATION schema to PDS schema. 

--CREATE TABLE IF NOT EXISTS t_knownhosts (
--	id SERIAL NOT NULL,
--	ip_address VARCHAR(16),
--	host_name VARCHAR(256) NOT NULL
--) ;
--ALTER TABLE t_knownhosts 
--	ADD CONSTRAINT pk_t_knownhosts
--		PRIMARY KEY (id) ;
--ALTER TABLE t_knownhosts 
--	ADD CONSTRAINT uq_knownhosts01
--		UNIQUE (host_name) ;
--
INSERT INTO t_knownhosts (host_name)
SELECT DISTINCT hostname
FROM t_repositories WHERE hostname NOT IN (
	SELECT kh.host_name FROM t_knownhosts kh) ; 


/* ------------------------------
	t_tmpnamestrategy
*/ ------------------------------
CREATE TABLE t_tmpnamestrategy (
    id_tmpnamestrategy INT2 NOT NULL,
	tempname VARCHAR(24) NOT NULL
) ;
ALTER TABLE t_tmpnamestrategy 
	ADD CONSTRAINT pk_t_tmpnamestrategy
		PRIMARY KEY (id_tmpnamestrategy) ;

ALTER TABLE t_tmpnamestrategy 
	ADD CONSTRAINT uq_tmpnamestrategy01
		UNIQUE (tempname) ;

INSERT INTO t_tmpnamestrategy (id_tmpnamestrategy, tempname) VALUES (1, 'PREFIX') ;
INSERT INTO t_tmpnamestrategy (id_tmpnamestrategy, tempname) VALUES (2, 'SUFFIX') ;
INSERT INTO t_tmpnamestrategy (id_tmpnamestrategy, tempname) VALUES (3, 'TMPDIR') ;








---------------------------------------------------------------------------------
------------------------------ TABLES MODIFICATIONS -----------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_receptionrules
*/ ------------------------------
ALTER TABLE t_receptionrules ADD php_timeout_sec INTEGER ;
ALTER TABLE t_receptionrules
	ADD CONSTRAINT ck_receptionrules03
		CHECK (php_timeout_sec > 90) ;


/* ------------------------------
	t_repositories
*/ ------------------------------
ALTER TABLE t_repositories DROP CONSTRAINT uq_repositories01 ;
ALTER TABLE t_repositories RENAME COLUMN description TO symbolicname ;
ALTER TABLE t_repositories ALTER COLUMN symbolicname SET DATA TYPE VARCHAR(64) ;
ALTER TABLE t_repositories 
	ADD CONSTRAINT uq_repositories01
		UNIQUE (symbolicname) ;

ALTER TABLE t_repositories ADD transferprotocol_id INTEGER ;
UPDATE t_repositories SET protocol = 'fs' WHERE protocol = 'file' ;
UPDATE t_repositories SET transferprotocol_id = subq.id FROM (
	SELECT id, protocol FROM t_transferprotocols) AS subq
WHERE UPPER(t_repositories.protocol) = subq.protocol ;
ALTER TABLE t_repositories ALTER COLUMN transferprotocol_id SET NOT NULL ;
ALTER TABLE t_repositories DROP COLUMN protocol ;
ALTER TABLE t_repositories
	ADD CONSTRAINT fk_t_repositories1
		FOREIGN KEY (transferprotocol_id)
			REFERENCES t_transferprotocols (id)
				ON DELETE CASCADE ;

ALTER TABLE t_repositories ADD host_id INTEGER ;
UPDATE t_repositories SET host_id = subq.id FROM (
	SELECT id, host_name FROM t_knownhosts) AS subq
WHERE hostname = subq.host_name ;
ALTER TABLE t_repositories ALTER COLUMN host_id SET NOT NULL ;
ALTER TABLE t_repositories DROP COLUMN hostname ;
ALTER TABLE t_repositories
	ADD CONSTRAINT fk_t_repositories2
		FOREIGN KEY (host_id)
			REFERENCES t_knownhosts (id)
				ON DELETE CASCADE ;

ALTER TABLE t_repositories ADD username2 VARCHAR(32) ;
UPDATE t_repositories SET username2 = username ;
ALTER TABLE t_repositories ALTER COLUMN username2 SET NOT NULL ;
ALTER TABLE t_repositories DROP COLUMN username ;
ALTER TABLE t_repositories RENAME COLUMN username2 TO username ;

ALTER TABLE t_repositories ADD password2 VARCHAR(256) DEFAULT '' NOT NULL ;
UPDATE t_repositories SET password = '' WHERE password IS NULL ;
UPDATE t_repositories SET password2 = password ;
ALTER TABLE t_repositories DROP COLUMN password ;
ALTER TABLE t_repositories RENAME COLUMN password2 TO password ;

ALTER TABLE t_repositories ADD remotepath2 VARCHAR(1024) DEFAULT '' NOT NULL ;
UPDATE t_repositories SET remotepath2 = remotepath ;
ALTER TABLE t_repositories DROP COLUMN remotepath ;
ALTER TABLE t_repositories RENAME COLUMN remotepath2 TO remotepath ;

ALTER TABLE t_repositories ADD tempname_id INT2 DEFAULT 1 NOT NULL ;
ALTER TABLE t_repositories
	ADD CONSTRAINT fk_t_repositories3
		FOREIGN KEY (tempname_id)
			REFERENCES t_tmpnamestrategy (id_tmpnamestrategy)
				ON DELETE CASCADE ;

ALTER TABLE t_repositories ADD tempname_value VARCHAR(64) DEFAULT '.tmp' NOT NULL ;

ALTER TABLE t_repositories
	ADD CONSTRAINT ck_repositories01			
		CHECK (char_length(trim(tempname_value)) != 0) ;
ALTER TABLE t_repositories
	ADD CONSTRAINT ck_repositories02
		CHECK ((tempname_id != 2 AND (tempname_value = '..' OR tempname_value = '../')) != TRUE) ;
ALTER TABLE t_repositories
	ADD CONSTRAINT ck_repositories03
		CHECK ((tempname_id != 3 AND (tempname_value = '.' OR tempname_value = './')) != TRUE) ;

ALTER TABLE t_repositories ALTER COLUMN tempname_value DROP DEFAULT ;

ALTER TABLE t_repositories 
	ADD CONSTRAINT uq_repositories02
		UNIQUE (transferprotocol_id, host_id, username, remotepath) ;

CREATE INDEX ix_t_repositories01 ON t_repositories USING BTREE (transferprotocol_id) ;
CREATE INDEX ix_t_repositories02 ON t_repositories USING BTREE (host_id) ;
CREATE INDEX ix_t_repositories03 ON t_repositories USING BTREE (tempname_id) ;








---------------------------------------------------------------------------------
-------------------------------- VIEWS CREATION ---------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	v_distributionrules
*/ ------------------------------
CREATE OR REPLACE VIEW v_distributionrules (
	filetype, 
	username, 
	usermode,
	mediatype,
	repository_host,
	repository_prot,
	distributionalgo, 
	tasktable,
	fileclass,
	active)
AS SELECT  
	ft.filetype,
	us.userid,
	um.mode,
	mt.mediatype,
	kh.host_name,
	tp.protocol,
	da.name,
	tt.name,
	fc.fileclass,
	dr.active 
FROM 
	t_distributionrules dr 
	INNER JOIN t_filetypes ft
		ON dr.filetype_id = ft.id_filetype
	INNER JOIN t_users us
		ON dr.user_id = us.id_user
	INNER JOIN t_usermodes um
		ON dr.usermode_id = um.id
	INNER JOIN t_mediatypes mt
		ON dr.mediatype_id = mt.id_mediatype
	INNER JOIN t_repositories rp
		ON dr.repository_id = rp.id_repository
	INNER JOIN t_knownhosts kh
		ON rp.host_id = kh.id
	INNER JOIN t_transferprotocols tp
		ON rp.transferprotocol_id = tp.id
	INNER JOIN t_distributionalgo da
		ON dr.distributionalgo_id = da.id
	INNER JOIN t_tasktables tt
		ON dr.format_tt_id = tt.id_tasktable
	LEFT JOIN t_fileclasses fc
		ON dr.fileclass_id = fc.id_fileclass	
ORDER BY
	dr.filetype_id,
	dr.user_id
;








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
		'pds-11.2.0',
		'2013-07-10',
		3,
		2,
		'PDS_11-2-0',
		'Added t_tmpnamestrategy table and changed t_receptionrules and t_repositories. Table t_knownhosts will be moved from CIRCULATION schema to PDS schema.'
) ;


