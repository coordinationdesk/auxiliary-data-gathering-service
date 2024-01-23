
-- Version 10.3.1
-- POSTGRES








---------------------------------------------------------------------------------
--------------------------------- TABLES UPDATE ---------------------------------
---------------------------------------------------------------------------------
INSERT INTO t_transferprotocols (id, protocol) VALUES (5, 'MEDIA') ;

INSERT INTO t_stati (id, status) VALUES (27, 'MEDIA_ACKNOLEDGE') ;
INSERT INTO t_stati (id, status) VALUES (28, 'MEDIA_WAIT') ;









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
		'pds-10.3.1',
		'2012-04-03',
		3,
		2,
		'PDS_10-3-1',
		'Added new records into t_transferprotocols and t_stati.'
) ;
