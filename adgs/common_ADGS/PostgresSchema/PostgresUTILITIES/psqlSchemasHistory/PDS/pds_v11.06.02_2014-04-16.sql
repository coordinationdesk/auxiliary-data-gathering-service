
-- Version 11.6.2
-- POSTGRES








---------------------------------------------------------------------------------
---------------------------- PROCEDURES MODIFICATION ----------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	P_DISTRIBUTE_ITEM
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_distribute_item(
	in_fname VARCHAR(128),
	in_fversion VARCHAR(32),
	in_ftypeid INTEGER,
	in_userid INTEGER,
	in_fileclassid INTEGER,
	in_format INTEGER,
	in_orderid INTEGER,
	in_duedays INTEGER) RETURNS INTEGER AS $$
	
DECLARE
	p_extordid 	VARCHAR(128);
	p_cartid   	INT;
	p_subreq   	INT;
	p_totsubreq	INT;
	p_chng_add	INT;
	
BEGIN
	p_chng_add := NULL ;
	
	p_extordid := (SELECT externalorderid FROM t_ordersqueue WHERE orderid = in_orderid);
	p_subreq := (SELECT subreq FROM t_ordersqueue WHERE orderid = in_orderid);
	
	IF p_extordid IS NOT NULL AND p_subreq IS NOT NULL 
	THEN
		p_cartid := (SELECT id FROM t_cartsref AS cr
				INNER JOIN t_ordersqueue AS oq
				ON oq.externalorderid = cr.extref
				WHERE oq.orderid = in_orderid);

		p_totsubreq := (SELECT max(c.distsubreq) +1 
				FROM t_cartsinv c 
				WHERE c.cart = p_cartid);
	ELSE
		p_cartid := NULL;
		p_totsubreq := NULL;
	END IF;


--	the input inventory file is distributed to each user who declared an interest on that file-type.
--	the distribution is done according to the active field in the distribution policy record
	
	INSERT INTO t_temporarydistribution (
		duedate, 
		filename, 
		fileversion, 
		mediatype, 
		userid, 
		format, 
		algo, 
		policy, 
		orderid, 
		cartid, 
		subreq,
		disseminationpackingalgo_id )
	SELECT DISTINCT
		CASE
			WHEN in_duedays IS NULL THEN (NOW()::DATE + dr.maxdaytodeliver) + NOW()::TIME(6)
			ELSE (NOW()::DATE + in_duedays) + NOW()::TIME(6)
		END,
		in_fname, 
		in_fversion,
		mt.mediatype,
		us.userid,
		dr.format_tt_id, 
		dr.distributionalgo_id, 
		dr.id_distributionrule, 
		in_orderid, 
		p_cartid, 
		p_totsubreq,
		dr.disseminationpackingalgo_id
	FROM t_distributionrules dr
		INNER JOIN t_users us
			ON dr.user_id = us.id_user
		INNER JOIN t_mediatypes mt
			ON dr.mediatype_id = mt.id_mediatype
	WHERE
		dr.user_id = in_userid
		AND (dr.filetype_id = in_ftypeid OR dr.filetype_id IN (SELECT pfiletype FROM t_filetypescomps WHERE cfiletype = in_ftypeid)) 
		AND dr.active = 't'
		AND dr.ondemand = 'f'
		AND (CASE WHEN in_format IS NOT NULL 
				THEN dr.format_tt_id = in_format 
				ELSE dr.format_tt_id = dr.format_tt_id 
			END)
		AND (f_compare_int_values(dr.fileclass_id, in_fileclassid) OR dr.fileclass_id IS NULL)
		AND dr.usermode_id = (SELECT mode FROM t_users WHERE id_user = in_userid)
	;
	
	GET DIAGNOSTICS p_chng_add = ROW_COUNT ;
		
	IF p_chng_add > 0 THEN
		UPDATE t_cartsinv SET distsubreq = p_totsubreq WHERE cart = p_cartid AND ordsubreq = p_subreq ;
	END IF ;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	P_DISTRIBUTE_ITEM_ON_DEMAND
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_distribute_item_on_demand(
	in_fname VARCHAR(128),
	in_fversion VARCHAR(32),
	in_ftypeid INTEGER,
	in_userid INTEGER,
	in_fileclassid INTEGER,
	in_format INTEGER,
	in_orderid INTEGER) RETURNS INTEGER AS $$
	
DECLARE
	p_extordid		VARCHAR(128);
	p_cartid		INT;
	p_subreq		INT;
	p_totsubreq		INT;
	p_ondem_media	VARCHAR(16);
	p_chng_add		INT;
	
BEGIN
	p_chng_add := NULL ;

	p_extordid := (SELECT externalorderid FROM t_ordersqueue WHERE orderid = in_orderid);
	p_subreq := (SELECT subreq FROM t_ordersqueue WHERE orderid = in_orderid);
	p_ondem_media := (SELECT od.mediatype FROM t_ondemanddistribution od WHERE od.orderid = in_orderid AND od.userid = (SELECT userid FROM t_users WHERE id_user = in_userid)) ;
	
	
	IF p_extordid IS NOT NULL AND p_subreq IS NOT NULL 
	THEN
		p_cartid := (SELECT id FROM t_cartsref AS cr
					INNER JOIN t_ordersqueue AS oq
						ON oq.externalorderid = cr.extref
					WHERE oq.orderid = in_orderid);

		p_totsubreq := (SELECT max(c.distsubreq) +1 
					FROM t_cartsinv c 
					WHERE c.cart = p_cartid);
	ELSE
		p_cartid := NULL;
		p_totsubreq := NULL;
	END IF;


--	the input inventory file is distributed to each user who declared an interest on that file-type.
--	the distribution is done according to the active field in the distribution policy record
	
	INSERT INTO t_temporarydistribution (
		duedate, 
		filename, 
		fileversion, 
		mediatype, 
		userid, 
		format, 
		algo, 
		policy, 
		orderid, 
		cartid, 
		subreq,
		disseminationpackingalgo_id )
	SELECT DISTINCT
		(NOW()::DATE + dr.maxdaytodeliver) + NOW()::TIME(6),
		in_fname, 
		in_fversion,
		CASE 
			WHEN p_ondem_media IS NULL THEN mt.mediatype
			ELSE p_ondem_media
		END,
		us.userid,
		dr.format_tt_id, 
		dr.distributionalgo_id, 
		dr.id_distributionrule, 
		in_orderid, 
		p_cartid, 
		p_totsubreq,
		dr.disseminationpackingalgo_id
	FROM t_distributionrules dr
		INNER JOIN t_users us
			ON dr.user_id = us.id_user
		INNER JOIN t_mediatypes mt
			ON dr.mediatype_id = mt.id_mediatype
	WHERE
		dr.user_id = in_userid
		AND (dr.filetype_id = in_ftypeid OR dr.filetype_id IN (SELECT pfiletype FROM t_filetypescomps WHERE cfiletype = in_ftypeid)) 
		AND dr.active = 't'
		AND dr.ondemand = 't'
		AND (CASE WHEN in_format IS NOT NULL 
				THEN dr.format_tt_id = in_format 
				ELSE dr.format_tt_id = dr.format_tt_id 
			END)
		AND (f_compare_int_values(dr.fileclass_id, in_fileclassid) OR dr.fileclass_id IS NULL)
		AND dr.usermode_id = (SELECT mode FROM t_users WHERE id_user = in_userid)
		AND (p_ondem_media IS NULL OR dr.distributionalgo_id IN (SELECT id FROM t_distributionalgo WHERE on_demand = 't'))
	;
	
	GET DIAGNOSTICS p_chng_add = ROW_COUNT ;
		
	IF p_chng_add > 0 THEN
		UPDATE t_cartsinv SET distsubreq = p_totsubreq WHERE cart = p_cartid AND ordsubreq = p_subreq ;
	END IF ;
	
	RETURN 0;

END ;
$$ LANGUAGE plpgsql ;





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
		'pds-11.6.2',
		'2014-04-16',
		3,
		2,
		'PDS_11-6-2',
		'Fixed bug for in_format NULL value into p_distribute_item* procedures.'
) ;


