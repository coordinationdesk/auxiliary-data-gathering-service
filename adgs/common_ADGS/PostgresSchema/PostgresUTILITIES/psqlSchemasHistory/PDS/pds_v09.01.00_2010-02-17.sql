
-- Version 9.1.0
-- POSTGRES








---------------------------------------------------------------------------------
-------------------------------- TABLES CREATION --------------------------------
---------------------------------------------------------------------------------

--------------------------------
--- T_FILETYPES_X_SATELLITES ---
--------------------------------
CREATE TABLE t_filetypes_x_satellites (
	id SERIAL NOT NULL,
	filetype VARCHAR(16) NOT NULL,
	satellite_id INT2 NOT NULL
) ;
ALTER TABLE t_filetypes_x_satellites 
	ADD CONSTRAINT pk_filetypes_x_satellites 
		PRIMARY KEY (id);

ALTER TABLE t_filetypes_x_satellites 
	ADD CONSTRAINT uq_t_filetypesxsatellites01 
		UNIQUE (filetype, satellite_id) ;

ALTER TABLE t_filetypes_x_satellites 
	ADD CONSTRAINT fk_t_filetypesxsatellites01 
		FOREIGN KEY (filetype)
			REFERENCES t_filetypes (filetype) 
				ON DELETE CASCADE ;

ALTER TABLE t_filetypes_x_satellites 
	ADD CONSTRAINT fk_t_filetypesxsatellites02 
		FOREIGN KEY (satellite_id)
			REFERENCES t_satellites (satelliteid) 
				ON DELETE CASCADE ;








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
		'pds-9.1.0',
		'2010-02-17',
		3,
		1,
		'SchemaVersion_9-1-0',
		'Added table t_filetypes_x_satellites.'
) ;
