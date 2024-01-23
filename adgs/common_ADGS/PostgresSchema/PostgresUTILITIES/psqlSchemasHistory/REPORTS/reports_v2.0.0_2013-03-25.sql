
-- Version 2.0.0
-- POSTGRES



-- Dalla versione del PDS 11 in poi le tabelle t_filetypes_stylesheets e
-- t_stylesheetoutputtypes non saranno più installate con il PDS ma come
-- plugins con lo schema REPORTS





---------------------------------------------------------------------------------
------------------------------ TABLES MODIFICATION ------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_filetypes_stylesheets
*/ ------------------------------
ALTER TABLE t_filetypes_stylesheets ADD fileextension VARCHAR(16);








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
		'reports-2.0.0',
		'2013-03-25',
		6,
		22,
		'REPORTS_2-0-0',
		'Added fileextension to t_filetypes_stylesheets table. Moved t_filetypes_stylesheets and t_stylesheetoutputtypes into REPORTS schema.'
) ;
