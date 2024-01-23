
-- Version 1.0.0
-- POSTGRES





-- ########################################################################### --
-- ##################                                        ################# --
-- ##################                WARNING!!!              ################# --
-- ##################  This schema can be installed only on  ################# --
-- ##################      PDS 11.0.0 version or major       ################# --
-- ##################                                        ################# --
-- ########################################################################### --





---------------------------------------------------------------------------------
-------------------------------- TABLES CREATION --------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_crgroups
*/ ------------------------------
CREATE TABLE t_crgroups (
	id_crgroup SERIAL NOT NULL,
	name VARCHAR(24) NOT NULL,
	mailaddresses TEXT,
	description VARCHAR(128),
	virtual_filetype_id INTEGER
) ;

ALTER TABLE t_crgroups 
	ADD CONSTRAINT pk_t_crgroups
		PRIMARY KEY (id_crgroup) ;

ALTER TABLE t_crgroups 
	ADD CONSTRAINT uq_crgroups01
		UNIQUE (name) ;

ALTER TABLE t_crgroups 
	ADD CONSTRAINT fk_crgroups01
		FOREIGN KEY (virtual_filetype_id) REFERENCES t_filetypes (id_filetype) 
			ON DELETE CASCADE ;

CREATE INDEX ix_t_crgroups01 ON t_crgroups USING BTREE (virtual_filetype_id) ;



/* ------------------------------
	t_crrelations
*/ ------------------------------
CREATE TABLE t_crrelations (
	id_crrelation SERIAL NOT NULL,
	crgroup_id INT NOT NULL,
	repository_id INT NOT NULL,
	distributionrule_id INT NOT NULL
) ;

ALTER TABLE t_crrelations 
	ADD CONSTRAINT pk_t_crrelations
		PRIMARY KEY (id_crrelation) ;

ALTER TABLE t_crrelations 
	ADD CONSTRAINT uq_crrelations01
		UNIQUE (crgroup_id, repository_id, distributionrule_id) ;

ALTER TABLE t_crrelations 
	ADD CONSTRAINT fk_crrelations01
		FOREIGN KEY (crgroup_id) REFERENCES t_crgroups (id_crgroup) 
			ON DELETE CASCADE ;

ALTER TABLE t_crrelations 
	ADD CONSTRAINT fk_crrelations02
		FOREIGN KEY (repository_id) REFERENCES t_repositories (id_repository) 
			ON DELETE CASCADE ;

ALTER TABLE t_crrelations 
	ADD CONSTRAINT fk_crrelations03
		FOREIGN KEY (distributionrule_id) REFERENCES t_distributionrules (id_distributionrule) 
			ON DELETE CASCADE ;

CREATE INDEX ix_t_crrelations01 ON t_crrelations USING BTREE (crgroup_id) ;
CREATE INDEX ix_t_crrelations02 ON t_crrelations USING BTREE (repository_id) ;
CREATE INDEX ix_t_crrelations03 ON t_crrelations USING BTREE (distributionrule_id) ;



/* ------------------------------
	t_crtemporaries
*/ ------------------------------
CREATE TABLE t_crtemporaries (
	inventory_id BIGINT NOT NULL,
	crgroup_id INT NOT NULL,
	tstamp TIMESTAMP(6) NOT NULL,
	author VARCHAR(24) NOT NULL,
	crtag VARCHAR(24),
	crcomment VARCHAR(128)
) ;

ALTER TABLE t_crtemporaries 
	ADD CONSTRAINT pk_t_crtemporaries
		PRIMARY KEY (inventory_id, crgroup_id, tstamp) ;

ALTER TABLE t_crtemporaries 
	ADD CONSTRAINT fk_crtemporaries01
		FOREIGN KEY (inventory_id) REFERENCES t_inventory (id_inv) 
			ON DELETE CASCADE ;

ALTER TABLE t_crtemporaries 
	ADD CONSTRAINT fk_crtemporaries02
		FOREIGN KEY (crgroup_id) REFERENCES t_crgroups (id_crgroup) 
			ON DELETE CASCADE ;

CREATE INDEX ix_t_crtemporaries01 ON t_crtemporaries USING BTREE (inventory_id) ;
CREATE INDEX ix_t_crtemporaries02 ON t_crtemporaries USING BTREE (crgroup_id) ;
CREATE INDEX ix_t_crtemporaries03 ON t_crtemporaries USING BTREE (tstamp) ;
CREATE INDEX ix_t_crtemporaries04 ON t_crtemporaries USING BTREE (author) ;



/* ------------------------------
	t_crhistory
*/ ------------------------------
CREATE TABLE t_crhistory (
	inventory_id BIGINT NOT NULL,
	crgroup_id INT NOT NULL,
	tstamp TIMESTAMP(6) NOT NULL,
	author VARCHAR(24) NOT NULL,
	crtag VARCHAR(24),
	crcomment VARCHAR(128)
) ;

ALTER TABLE t_crhistory 
	ADD CONSTRAINT pk_t_crhistory
		PRIMARY KEY (inventory_id, crgroup_id, tstamp) ;

CREATE INDEX ix_t_crhistory01 ON t_crhistory USING BTREE (inventory_id) ;
CREATE INDEX ix_t_crhistory02 ON t_crhistory USING BTREE (crgroup_id) ;
CREATE INDEX ix_t_crhistory03 ON t_crhistory USING BTREE (tstamp) ;
CREATE INDEX ix_t_crhistory04 ON t_crhistory USING BTREE (author) ;



/* ------------------------------
	t_cr_x_filetypes
*/ ------------------------------
CREATE TABLE t_cr_x_filetypes (
	id_cr_x_filetype SERIAL NOT NULL,
	crgroup_id INT NOT NULL,
	filetype_id INT NOT NULL
) ;

ALTER TABLE t_cr_x_filetypes 
	ADD CONSTRAINT pk_t_cr_x_filetypes
		PRIMARY KEY (id_cr_x_filetype) ;

ALTER TABLE t_cr_x_filetypes 
	ADD CONSTRAINT uq_cr_x_filetypes01
		UNIQUE (crgroup_id, filetype_id) ;

ALTER TABLE t_cr_x_filetypes 
	ADD CONSTRAINT fk_cr_x_filetypes01
		FOREIGN KEY (crgroup_id) REFERENCES t_crgroups (id_crgroup) 
			ON DELETE CASCADE ;

ALTER TABLE t_cr_x_filetypes 
	ADD CONSTRAINT fk_cr_x_filetypes02
		FOREIGN KEY (filetype_id) REFERENCES t_filetypes (id_filetype) 
			ON DELETE CASCADE ;

CREATE INDEX ix_t_cr_x_filetypes01 ON t_cr_x_filetypes USING BTREE (crgroup_id) ;
CREATE INDEX ix_t_cr_x_filetypes02 ON t_cr_x_filetypes USING BTREE (filetype_id) ;








---------------------------------------------------------------------------------
------------------------------ PROCEDURES CREATION ------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	p_cr_distribution
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_cr_distribution(
	in_auth VARCHAR(24),
	in_tstamp TIMESTAMP(6)) RETURNS INTEGER AS $$

BEGIN
	
	PERFORM p_distribute_item_on_demand(
		inv.filename,
		inv.fileversion,
		drl.filetype_id,
		drl.user_id,
		drl.fileclass_id,
		drl.format_tt_id,
		NULL)
	FROM
		t_crtemporaries crt
		INNER JOIN t_inventory inv
			ON crt.inventory_id = inv.id_inv
		INNER JOIN t_crrelations crr
			ON crt.crgroup_id = crr.crgroup_id
		INNER JOIN t_distributionrules drl
			ON crr.distributionrule_id = drl.id_distributionrule
	WHERE 
		crt.author = in_auth 
		AND crt.tstamp = in_tstamp 
	;
	
	INSERT INTO t_crhistory SELECT * FROM t_crtemporaries crt
		WHERE crt.author = in_auth AND crt.tstamp = in_tstamp ;
		
	DELETE FROM t_crtemporaries ct 
		WHERE author = in_auth AND tstamp = in_tstamp ;
	
	RETURN 0 ;

END ;
$$ LANGUAGE plpgsql ;


/* ------------------------------
	p_cr_distribution OLD
 ------------------------------
CREATE OR REPLACE FUNCTION p_cr_distribution(
	in_auth VARCHAR(24),
	in_tstamp TIMESTAMP(6)) RETURNS INTEGER AS $$

DECLARE
	inserted_rows INT;
	
BEGIN
	inserted_rows := 0 ;
	
	INSERT INTO t_distributionitems (
		duedate,
		creationdate,
		filename,
		fileversion,
		mediatype,
		userid,
		distributionrule_id,
		format,
		disseminationpackingalgo_id )
	SELECT DISTINCT
		CASE 
			WHEN drl.maxdaytodeliver = 0 THEN in_tstamp::TIMESTAMP(6)
			ELSE (in_tstamp::DATE + drl.maxdaytodeliver)::TIMESTAMP(6) 
		END AS duedate,
		in_tstamp::TIMESTAMP(6) AS creationdate,
		inv.filename,
		inv.fileversion,
		mdt.mediatype,
		usr.userid,
		crr.distributionrule_id,
		drl.format_tt_id AS format,
		drl.disseminationpackingalgo_id
	FROM
		t_crtemporaries crt
		INNER JOIN t_inventory inv
			ON crt.inventory_id = inv.id_inv
		INNER JOIN t_crrelations crr
			ON crt.crgroup_id = crr.crgroup_id
		INNER JOIN t_distributionrules drl
			ON crr.distributionrule_id = drl.id_distributionrule
		INNER JOIN t_mediatypes mdt
			ON drl.mediatype_id = mdt.id_mediatype
		INNER JOIN t_users usr
			ON drl.user_id = usr.id_user
	WHERE 
		crt.author = in_auth 
		AND crt.tstamp = in_tstamp 
	;
	
	GET DIAGNOSTICS inserted_rows = ROW_COUNT ;
	
	IF inserted_rows > 0 THEN
		INSERT INTO t_crhistory SELECT * FROM t_crtemporaries crt
			WHERE crt.author = in_auth AND crt.tstamp = in_tstamp ;
		
		DELETE FROM t_crtemporaries ct 
			WHERE author = in_auth AND tstamp = in_tstamp ;
	ELSE
		RAISE EXCEPTION 'p_cr_distribution: No items found to distribute.' ;
	END IF ;
	
	RETURN inserted_rows ;

END ;
$$ LANGUAGE plpgsql ;
*/ 








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
		'cr-1.0.0',
		'2013-03-15',
		5,
		24,
		'CR_1-0-0',
		'Created coordinated release schema.'
) ;


