
-- Version 12.02.00
-- POSTGRES








--------------------------------------------------------------------------------
------------------------------- CHECK CONSTRAINTS -----------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_waitingorders
*/ ------------------------------
ALTER TABLE t_waitingorders
	ADD CONSTRAINT ck_waitingorders02
		CHECK ((triggertype = 'DELAY' AND expirationdate IS NULL) != TRUE) ;








--------------------------------------------------------------------------------
------------------------------- PROCEDURE CHANGES ------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	P_PURGEEXPIREDWAITINGORDERS
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_purgeexpiredwaitingorders() RETURNS INTEGER AS $$

BEGIN
	DELETE FROM t_waitingorders WHERE expirationdate < NOW() AND expirationdate IS NOT NULL;

	RETURN 0;
	
END ;
$$ LANGUAGE plpgsql ;








--------------------------------------------------------------------------------
-------------------------------- VIEWS CREATION --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	v_receptionrules
*/ ------------------------------
DROP VIEW IF EXISTS v_receptionrules ;
CREATE OR REPLACE VIEW v_receptionrules (
	filetype, 
	username, 
	usermode,
	repository,
	localpath,
	tasktable,
	delete_on_remote,
	active)
AS SELECT  
	ft.filetype,
	us.userid,
	um.mode,
	rp.symbolicname,
	rr.localpath,
	tt.name,
	rr.delete_on_remote,
	rr.active
FROM 
	t_receptionrules rr 
	INNER JOIN t_filetypes ft
		ON rr.filetype_id = ft.id_filetype
	INNER JOIN t_users us
		ON rr.user_id = us.id_user
	INNER JOIN t_usermodes um
		ON rr.usermode_id = um.id
	INNER JOIN t_repositories rp
		ON rr.repository_id = rp.id_repository
	INNER JOIN t_tasktables tt
		ON rr.reception_tt_id = tt.id_tasktable
ORDER BY
	rr.filetype_id,
	rr.user_id
;








--------------------------------------------------------------------------------
-------------------------------- VIEWS CHANGES ---------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	v_distributionrules
*/ ------------------------------
DROP VIEW IF EXISTS v_distributionrules ;
CREATE OR REPLACE VIEW v_distributionrules (
	filetype, 
	username, 
	usermode,
	mediatype,
	repository_host,
	repository_prot,
	distributionalgo, 
	tasktable,
	fileclass,
	disseminationpackingalgo,
	active,
	ondemand)
AS SELECT  
	ft.filetype,
	us.userid,
	um.mode,
	mt.mediatype,
	kh.host_name,
	tp.protocol,
	da.name,
	tt.name,
	fc.fileclass,
	dp.factorykey,
	dr.active,
	dr.ondemand 
FROM 
	t_distributionrules dr 
	INNER JOIN t_filetypes ft
		ON dr.filetype_id = ft.id_filetype
	INNER JOIN t_users us
		ON dr.user_id = us.id_user
	INNER JOIN t_usermodes um
		ON dr.usermode_id = um.id
	INNER JOIN t_mediatypes mt
		ON dr.mediatype_id = mt.id_mediatype
	INNER JOIN t_repositories rp
		ON dr.repository_id = rp.id_repository
	INNER JOIN t_knownhosts kh
		ON rp.host_id = kh.id
	INNER JOIN t_transferprotocols tp
		ON rp.transferprotocol_id = tp.id
	INNER JOIN t_distributionalgo da
		ON dr.distributionalgo_id = da.id
	INNER JOIN t_tasktables tt
		ON dr.format_tt_id = tt.id_tasktable
	INNER JOIN t_disseminationpackingalgos dp
		ON dr.disseminationpackingalgo_id = dp.id_disseminationpackingalgo
	LEFT JOIN t_fileclasses fc
		ON dr.fileclass_id = fc.id_fileclass	
ORDER BY
	dr.filetype_id,
	dr.user_id
;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'pds-12.02.00' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-12.02.00',
		'2017-01-31',
		3,
		2,
		'PDS_12-2-0',
		'Added indexes on t_waitingorders. Change procedure p_purgeexpiredwaitingorders. Added view v_receptionrules and changed view v_distributionrules'
) ;


