
-- Version 11.5.2
-- POSTGRES








---------------------------------------------------------------------------------
------------------------------ TABLES MODIFICATION ------------------------------
---------------------------------------------------------------------------------

ALTER TABLE t_receptionrules ALTER COLUMN usermode_id TYPE INT2 ;
ALTER TABLE t_receptionpolicies ALTER COLUMN usermode TYPE INT2 ;
ALTER TABLE t_users ALTER COLUMN mode TYPE INT2 ;
ALTER TABLE t_distributionpolicies ALTER COLUMN usermode TYPE INT2 ;








---------------------------------------------------------------------------------
-------------------------------- INDEX CREATION ---------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_classifiedfiletypes
*/ ------------------------------
CREATE INDEX ix_t_classifiedfiletypes ON t_classifiedfiletypes USING BTREE (filetype) ;

/* ------------------------------
	t_dbparameters
*/ ------------------------------
CREATE INDEX ix_t_dbparameters01 ON t_dbparameters USING BTREE (parametertype_id) ;

/* --------------------------
	t_ordersgeneration
*/ --------------------------
CREATE INDEX ix_t_ordersgeneration01 ON t_ordersgeneration USING BTREE (processorname, processorversion) ;
CREATE INDEX ix_t_ordersgeneration02 ON t_ordersgeneration USING BTREE (ordertype) ;
CREATE INDEX ix_t_ordersgeneration03 ON t_ordersgeneration USING BTREE (algorithm) ;
CREATE INDEX ix_t_ordersgeneration04 ON t_ordersgeneration USING BTREE (filetype) ;
CREATE INDEX ix_t_ordersgeneration05 ON t_ordersgeneration USING BTREE (fileclass) ;

/* ------------------------------
	t_ordtriggeringfiles
*/ ------------------------------
CREATE INDEX ix_t_ordtriggeringfiles01 ON t_ordtriggeringfiles USING BTREE (filename, fileversion) ;
CREATE INDEX ix_t_ordtriggeringfiles02 ON t_ordtriggeringfiles USING BTREE (orderid) ;

/* ------------------------------
	t_temporarydistribution
*/ ------------------------------
DROP INDEX IF EXISTS ix_t_temporarydistribution01 ;
DROP INDEX IF EXISTS ix_t_temporarydistribution02 ;
DROP INDEX IF EXISTS ix_t_temporarydistribution03 ;
DROP INDEX IF EXISTS ix_t_temporarydistribution04 ;
CREATE INDEX ix_t_temporarydistribution01 ON t_temporarydistribution USING BTREE (filename, fileversion) ;
CREATE INDEX ix_t_temporarydistribution02 ON t_temporarydistribution USING BTREE (mediatype) ;
CREATE INDEX ix_t_temporarydistribution03 ON t_temporarydistribution USING BTREE (userid) ;
CREATE INDEX ix_t_temporarydistribution04 ON t_temporarydistribution USING BTREE (format) ;
CREATE INDEX ix_t_temporarydistribution05 ON t_temporarydistribution USING BTREE (algo) ;
CREATE INDEX ix_t_temporarydistribution06 ON t_temporarydistribution USING BTREE (policy) ;
CREATE INDEX ix_t_temporarydistribution07 ON t_temporarydistribution USING BTREE (cartid) ;
CREATE INDEX ix_t_temporarydistribution08 ON t_temporarydistribution USING BTREE (disseminationpackingalgo_id) ;







---------------------------------------------------------------------------------
------------------------------- VIEWS MODIFICATION ------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	v_distribution
*/ ------------------------------
DROP VIEW v_distribution ;
CREATE OR REPLACE VIEW v_distribution (
	packageid, 
	status, 
	distributeddate, 
	jobresponsible, 
	creationdate, 
	mediatype, 
	userid,
	distributionruleid)
AS SELECT DISTINCT 
	dq.packageid, 
	dq.status, 
	dq.distributeddate, 
	dq.jobresponsible, 
	dq.creationdate, 
	di.mediatype, 
	di.userid,
	di.distributionrule_id
FROM 
	t_distributionqueue dq 
	INNER JOIN t_distributionitems di
		ON dq.packageid = di.packageid
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
		'pds-11.5.2',
		'2013-11-26',
		3,
		2,
		'PDS_11-5-2',
		'Added some indexes. Fixed bug on view v_distribution.'
) ;


