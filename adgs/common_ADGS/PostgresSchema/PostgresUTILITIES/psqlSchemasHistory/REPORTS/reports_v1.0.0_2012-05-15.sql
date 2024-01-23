
-- Version 1.0.0
-- POSTGRES








---------------------------------------------------------------------------------
-------------------------------- TABLES CREATION --------------------------------
---------------------------------------------------------------------------------

------------------------------
-- t_lastacknowledge
------------------------------
CREATE TABLE t_lastacknowledge (
	id SERIAL NOT NULL,
	applicationname VARCHAR(25) NOT NULL,
	applicationdata VARCHAR(128) NOT NULL,
	tstamp TIMESTAMP(6) NOT NULL
) ;

ALTER TABLE t_lastacknowledge 
	ADD CONSTRAINT pk_t_lastacknowledge
		PRIMARY KEY (id) ;








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
		'reports-1.0.0',
		'2012-05-15',
		5,
		1,
		'REPORTS_1-0-0',
		'Created reports schema.'
) ;
