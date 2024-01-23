/*

Copyright 2010-2020, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S. Databases schema views script $

	$Id$

	$Author$

*/





--------------------------------------------------------------------------------
--
--    	  	  	  	  	  	  	  VIEWS
--
--------------------------------------------------------------------------------


/* ------------------------------
	v_distribution
*/ ------------------------------
CREATE OR REPLACE VIEW v_distribution (
	packageid, 
	status, 
	distributeddate, 
	jobresponsible, 
	creationdate, 
	mediatype, 
	userid,
	distributionrule_id)
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



/* ------------------------------
	v_distributionrules
*/ ------------------------------
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



/* ------------------------------
	v_proc_ftype_generation
*/ ------------------------------
CREATE OR REPLACE VIEW v_proc_ftype_generation (
	processorname, 
	processorversion,
	filetype)
AS SELECT
	pr.processorname, 
	pr.processorversion,
	ft.filetype
FROM 
	t_proc_ftype_generation AS pf
	INNER JOIN t_processors AS pr 
		ON pf.proc_id = pr.id_processor
	INNER JOIN t_filetypes AS ft 
		ON pf.ftype_id = ft.id_filetype
;



/* ------------------------------
	v_receptionrules
*/ ------------------------------
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



/* ------------------------------
	v_satrelations
*/ ------------------------------
CREATE VIEW v_satrelations (
	satellite, 
	mission, 
	sensor)
AS SELECT 	
	sa.satellitename AS satellite, 
	sr.mission AS mission, 
	se.sensorname AS sensor
FROM 
	t_satrelations AS sr
	INNER JOIN t_satellites AS sa 
		ON sr.satelliteid = sa.satelliteid
	INNER JOIN t_sensors AS se 
		ON sr.sensorid = se.sensorid
ORDER BY 
	satellite, 
	mission, 
	sensor 
;



/* ------------------------------
	v_state_transitions
*/ ------------------------------
CREATE OR REPLACE VIEW v_state_transitions (
	table_name, 
	flow, 
	prev_status,
	post_status)
AS SELECT 
	st.table_name, 
	sf.flow_type  AS flow,
	ss1.status AS pre_s,
	ss2.status AS pst_s
FROM 
	t_statetransitions_tables AS st
	INNER JOIN t_statetransitions_flows AS sf 
		ON st.flow_id = sf.id
	INNER JOIN t_statetransitions_allowed AS sa 
		ON st.id = sa.table_id
	INNER JOIN t_stati AS ss1 
		ON sa.prev_state_id = ss1.id
	INNER JOIN t_stati AS ss2 
		ON sa.post_state_id = ss2.id
ORDER BY 
	table_name, 
	flow, 
	pre_s,
	pst_s
;


