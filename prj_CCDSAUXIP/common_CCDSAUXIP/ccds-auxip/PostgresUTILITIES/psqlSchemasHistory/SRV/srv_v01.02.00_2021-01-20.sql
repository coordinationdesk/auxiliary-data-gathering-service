
-- SRV Version 1.2.0
-- POSTGRES





BEGIN ;



--------------------------------------------------------------------------------
-------------------------------- TABLES CHANGES --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_srv_receptionruleshist 
*/ ------------------------------
ALTER TABLE t_srv_receptionruleshist ADD remoteuuid VARCHAR(160) ;
ALTER TABLE t_srv_receptionruleshist ADD evictiondate TIMESTAMP(3) ;


/* ------------------------------
	t_srv_repositories 
*/ ------------------------------
ALTER TABLE t_srv_repositories ADD attributes JSONB ;








--------------------------------------------------------------------------------
-------------------------------- TABLES UPDATES --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_srv_statuses 
*/ ------------------------------
INSERT INTO t_srv_statuses (id, status) VALUES (29, 'LOST'); 








--------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ---------------------------------
--------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'srv-01.02.00' ;

INSERT INTO t_schema (
	version,
	modificationdate,
	modificationtype,
	schematype,
	cvstag,
	description )
VALUES  (
	'srv-01.02.00',
	'2021-01-20',
	6,
	35,
	'SRV_1-2-0',
	'New implementations'
) ;


COMMIT ;


