
-- Version 9.7.0
-- POSTGRES








---------------------------------------------------------------------------------
-------------------------------- TABLES CREATION --------------------------------
---------------------------------------------------------------------------------

------------------------------
--- T_STYLESHEETOUTPUTTYPE ---
------------------------------
CREATE TABLE t_stylesheetoutputtypes (
	id			INT2 NOT NULL,
	outputtype	VARCHAR(8) NOT NULL
) ;

ALTER TABLE t_stylesheetoutputtypes 
	ADD CONSTRAINT pk_t_stylesheetoutputtypes
		PRIMARY KEY (id) ;

ALTER TABLE t_stylesheetoutputtypes 
	ADD CONSTRAINT uq_stylesheetoutputtypes01
		UNIQUE (outputtype) ;

INSERT INTO t_stylesheetoutputtypes (id, outputtype) VALUES (1, 'Html') ;
INSERT INTO t_stylesheetoutputtypes (id, outputtype) VALUES (2, 'Xml') ;
INSERT INTO t_stylesheetoutputtypes (id, outputtype) VALUES (3, 'Gantt') ;



-------------------------------
--- T_FILETYPES_STYLESHEETS ---
-------------------------------
CREATE TABLE t_filetypes_stylesheets (
	id				SERIAL NOT NULL,
	filetype		VARCHAR(16) NOT NULL,
	outputtype		INT2 NOT NULL,
	stylesheetname	VARCHAR(64) NOT NULL
) ;

ALTER TABLE t_filetypes_stylesheets 
	ADD CONSTRAINT pk_t_filetypes_stylesheets
		PRIMARY KEY (id) ;

ALTER TABLE t_filetypes_stylesheets 
	ADD CONSTRAINT uq_filetypes_stylesheets01
		UNIQUE (filetype, outputtype) ;

ALTER TABLE t_filetypes_stylesheets 
	ADD CONSTRAINT fk_filetypes_stylesheets01
		FOREIGN KEY (filetype)
			REFERENCES t_filetypes (filetype) 
				ON DELETE CASCADE ;

ALTER TABLE t_filetypes_stylesheets 
	ADD CONSTRAINT fk_filetypes_stylesheets02
		FOREIGN KEY (outputtype)
			REFERENCES t_stylesheetoutputtypes (id) 
				ON DELETE CASCADE  ;








---------------------------------------------------------------------------------
--------------------------------- TABLES UPDATE ---------------------------------
---------------------------------------------------------------------------------

---------------------
--- T_SCHEMATYPES ---
---------------------
UPDATE t_schematypes SET schematype = 'DEEGREEBRIM' WHERE id = 12 ;
UPDATE t_schematypes SET schematype = 'COM' WHERE id = 11 ;
UPDATE t_schematypes SET schematype = 'HARM' WHERE id = 10 ;
UPDATE t_schematypes SET schematype = 'DM' WHERE id = 7 ;
UPDATE t_schematypes SET schematype = 'INDEPENDED' WHERE id = 8 ;
UPDATE t_schematypes SET schematype = 'SPECIFIC FOR PROJECT' WHERE id = 9 ;
UPDATE t_schematypes SET schematype = 'CATALOGUE' WHERE id = 3 ;
UPDATE t_schematypes SET schematype = 'ISM' WHERE id = 4 ;
UPDATE t_schematypes SET schematype = 'USDFWA' WHERE id = 5 ;
UPDATE t_schematypes SET schematype = 'BASELINE' WHERE id = 6 ;

INSERT INTO t_schematypes (id, schematype) VALUES (13,'DEEGREEFEATURES') ;
INSERT INTO t_schematypes (id, schematype) VALUES (14,'GEOSERVER');
INSERT INTO t_schematypes (id, schematype) VALUES (15,'JOUWS');
INSERT INTO t_schematypes (id, schematype) VALUES (16,'OAS');
INSERT INTO t_schematypes (id, schematype) VALUES (17,'POR');
INSERT INTO t_schematypes (id, schematype) VALUES (18,'SIB');
INSERT INTO t_schematypes (id, schematype) VALUES (19,'WUP');








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
		'pds-9.7.0',
		'2010-10-19',
		3,
		1,
		'PDS_9-7-0',
		'Added tables t_stylesheetoutputtypes and t_filetypes_stylesheets. Updated tables t_schematypes.'
) ;
