
-- Version 2.1.0
-- POSTGRES






---------------------------------------------------------------------------------
-------------------------------- TABLES CREATION --------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_filetypes_x_stylesheets
*/ ------------------------------
CREATE TABLE t_filetypes_x_stylesheets (
	id SERIAL NOT NULL,
	filetype_id INTEGER NOT NULL,
	stylesheet_id INTEGER NOT NULL,
	outputtype_id INT2 NOT NULL,
	fileextension VARCHAR(16)
) ;
ALTER TABLE t_filetypes_x_stylesheets 
	ADD CONSTRAINT pk_t_filetypes_x_stylesheets
		PRIMARY KEY (id) ;

ALTER TABLE t_filetypes_x_stylesheets 
	ADD CONSTRAINT uq_filetypes_x_stylesheets01
		UNIQUE (filetype_id, outputtype_id, stylesheet_id) ;

ALTER TABLE t_filetypes_x_stylesheets 
	ADD CONSTRAINT fk_filetypes_x_stylesheets01
		FOREIGN KEY (filetype_id)
			REFERENCES t_filetypes (id_filetype) 
				ON DELETE CASCADE ;

ALTER TABLE t_filetypes_x_stylesheets 
	ADD CONSTRAINT fk_filetypes_x_stylesheets02
		FOREIGN KEY (stylesheet_id)
			REFERENCES t_configurationstylesheets (id) 
				ON DELETE CASCADE ;

ALTER TABLE t_filetypes_x_stylesheets 
	ADD CONSTRAINT fk_filetypes_x_stylesheets03
		FOREIGN KEY (outputtype_id)
			REFERENCES t_stylesheetoutputtypes (id) 
				ON DELETE CASCADE  ;

CREATE INDEX ix_t_filetypes_x_stylesheets01 ON t_filetypes_x_stylesheets USING BTREE (filetype_id) ;
CREATE INDEX ix_t_filetypes_x_stylesheets02 ON t_filetypes_x_stylesheets USING BTREE (stylesheet_id) ;
CREATE INDEX ix_t_filetypes_x_stylesheets03 ON t_filetypes_x_stylesheets USING BTREE (outputtype_id) ;

INSERT INTO t_filetypes_x_stylesheets (
	filetype_id,
	stylesheet_id,
	outputtype_id,
	fileextension )
SELECT
	ft.id_filetype,
	cs.id,
	fs.outputtype,
	fs.fileextension
FROM
	t_filetypes_stylesheets fs
	INNER JOIN t_filetypes ft
		ON fs.filetype = ft.filetype
	INNER JOIN t_configurationstylesheets cs
		ON fs.stylesheetname = cs.name
;








---------------------------------------------------------------------------------
--------------------------------- VIEW CREATION ---------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	v_filetypes_x_stylesheets
*/ ------------------------------
CREATE OR REPLACE VIEW v_filetypes_x_stylesheets (
	filetype,
	stylesheet,
	outputtype,
	fileextension )
AS SELECT
	ft.filetype,
	cs.name,
	ot.outputtype,
	fs.fileextension
FROM
	t_filetypes_x_stylesheets fs
	INNER JOIN t_filetypes ft
		ON fs.filetype_id = ft.id_filetype
	INNER JOIN t_configurationstylesheets cs
		ON fs.stylesheet_id = cs.id
	INNER JOIN t_stylesheetoutputtypes ot
		ON fs.outputtype_id = ot.id
ORDER BY
	filetype,
	stylesheet,
	outputtype,
	fileextension
;








---------------------------------------------------------------------------------
------------------------------ TABLES MODIFICATION ------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_filetypes_stylesheets
*/ ------------------------------
DROP TABLE t_filetypes_stylesheets ;








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
		'reports-2.1.0',
		'2013-06-04',
		6,
		22,
		'REPORTS_2-1-0',
		'Added new table t_filetypes_x_stylesheets and removed t_filetypes_stylesheets. Added view v_filetypes_x_stylesheets.'
) ;
