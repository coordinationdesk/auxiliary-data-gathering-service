
-- Version 11.1.0
-- POSTGRES








---------------------------------------------------------------------------------
-------------------------------- TABLES CREATION --------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_distruleoriginator
*/ ------------------------------
CREATE TABLE t_distruleoriginator (
    distributionrule_id INTEGER NOT NULL,
    originator_id INTEGER NOT NULL
) ;
ALTER TABLE t_distruleoriginator
	ADD CONSTRAINT pk_t_distruleoriginator
		PRIMARY KEY (distributionrule_id, originator_id) ;

ALTER TABLE t_distruleoriginator
	ADD CONSTRAINT fk_t_distruleoriginator1
		FOREIGN KEY (distributionrule_id)
			REFERENCES t_distributionrules (id_distributionrule)
				ON DELETE CASCADE ;

ALTER TABLE t_distruleoriginator
	ADD CONSTRAINT fk_t_distruleoriginator2
		FOREIGN KEY (originator_id)
			REFERENCES t_originators (id)
				ON DELETE CASCADE ;

CREATE INDEX ix_t_distruleoriginator01 ON t_distruleoriginator USING BTREE (distributionrule_id) ;
CREATE INDEX ix_t_distruleoriginator02 ON t_distruleoriginator USING BTREE (originator_id) ;



/* ------------------------------
	t_operationtypes
*/ ------------------------------
CREATE TABLE t_operationtypes (
	id_operationtype INT2 NOT NULL,
	type VARCHAR(32) NOT NULL
) ;

ALTER TABLE t_operationtypes
	ADD CONSTRAINT pk_t_operationtypes
		PRIMARY KEY (id_operationtype) ;

ALTER TABLE t_operationtypes 
	ADD CONSTRAINT uq_operationtypes01
		UNIQUE (type) ;

INSERT INTO t_operationtypes (id_operationtype, type) VALUES (1, 'UNKNOWN') ;
INSERT INTO t_operationtypes (id_operationtype, type) VALUES (2, 'IMPORT') ;
INSERT INTO t_operationtypes (id_operationtype, type) VALUES (3, 'EXPORT') ;



/* ------------------------------
	t_migrationalgos
*/ ------------------------------
CREATE TABLE t_migrationalgos (
	id_migrationalgo SERIAL NOT NULL,
	name VARCHAR(32) NOT NULL
) ;

ALTER TABLE t_migrationalgos
	ADD CONSTRAINT pk_t_migrationalgos
		PRIMARY KEY (id_migrationalgo) ;

ALTER TABLE t_migrationalgos 
	ADD CONSTRAINT uq_migrationalgos01
		UNIQUE (name) ;



/* ------------------------------
	t_filetypes_x_migrations
*/ ------------------------------
CREATE TABLE t_filetypes_x_migrations (
	id_filetypemigration SERIAL NOT NULL,
	sm_id INTEGER NOT NULL,
	filetype_id INTEGER NOT NULL,
	migrationalgo_id INTEGER NOT NULL,
	parameters TEXT
) ;

ALTER TABLE t_filetypes_x_migrations
	ADD CONSTRAINT pk_t_filetypes_x_migrations
		PRIMARY KEY (id_filetypemigration) ;

ALTER TABLE t_filetypes_x_migrations 
	ADD CONSTRAINT uq_filetypesxmigrations01
		UNIQUE (sm_id, filetype_id) ;

ALTER TABLE t_filetypes_x_migrations 
	ADD CONSTRAINT fk_filetypesxmigrations1
		FOREIGN KEY (sm_id)
			REFERENCES t_storagemanager (id) 
				ON DELETE CASCADE ;

ALTER TABLE t_filetypes_x_migrations 
	ADD CONSTRAINT fk_filetypesxmigrations2
		FOREIGN KEY (filetype_id)
			REFERENCES t_filetypes (id_filetype) 
				ON DELETE CASCADE ;

ALTER TABLE t_filetypes_x_migrations 
	ADD CONSTRAINT fk_filetypesxmigrations3
		FOREIGN KEY (migrationalgo_id)
			REFERENCES t_migrationalgos (id_migrationalgo) 
				ON DELETE CASCADE ;

CREATE INDEX ix_t_filetypesxmigrations01 ON t_filetypes_x_migrations USING BTREE (sm_id) ;
CREATE INDEX ix_t_filetypesxmigrations02 ON t_filetypes_x_migrations USING BTREE (filetype_id) ;
CREATE INDEX ix_t_filetypesxmigrations03 ON t_filetypes_x_migrations USING BTREE (migrationalgo_id) ;








---------------------------------------------------------------------------------
------------------------------ TABLES MODIFICATIONS -----------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_storagemanager
*/ ------------------------------
ALTER TABLE t_storagemanager ADD migrationalgo_id INTEGER ;
ALTER TABLE t_storagemanager ADD migrationalgo_par TEXT ;

ALTER TABLE t_storagemanager DROP CONSTRAINT fk_t_storagemanager ;
ALTER TABLE t_storagemanager
	ADD CONSTRAINT fk_t_storagemanager1
		FOREIGN KEY (type)
			REFERENCES t_smtype (type)
				ON DELETE CASCADE ;

ALTER TABLE t_storagemanager
	ADD CONSTRAINT fk_t_storagemanager2
		FOREIGN KEY (migrationalgo_id)
			REFERENCES t_migrationalgos (id_migrationalgo)
				ON DELETE CASCADE ;

CREATE INDEX ix_t_storagemanager01 ON t_storagemanager USING BTREE (type) ;
CREATE INDEX ix_t_storagemanager02 ON t_storagemanager USING BTREE (migrationalgo_id) ;



/* ------------------------------
	t_tasktables
*/ ------------------------------
ALTER TABLE t_tasktables ADD type_id INT2 ;
ALTER TABLE t_tasktables 
	ADD CONSTRAINT fk_tasktables1
		FOREIGN KEY (type_id)
			REFERENCES t_operationtypes (id_operationtype) 
				ON DELETE CASCADE ;
CREATE INDEX ix_t_tasktables01 ON t_tasktables USING BTREE (type_id) ;








---------------------------------------------------------------------------------
---------------------------------- TABLES DROP ----------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_distpoloriginator
*/ ------------------------------
DROP TABLE t_distpoloriginator CASCADE ;








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
		'pds-11.1.0',
		'2013-06-17',
		3,
		2,
		'PDS_11-1-0',
		'Added tables t_distruleoriginator, t_operationtypes, t_migrationalgos and t_filetypes_x_migrations. Added new fields on t_storagemanager and t_tasktables.'
) ;


