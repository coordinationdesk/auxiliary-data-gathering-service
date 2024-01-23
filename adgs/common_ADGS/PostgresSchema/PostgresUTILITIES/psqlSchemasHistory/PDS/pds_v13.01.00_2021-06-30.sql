
-- Version 13.01.00
-- POSTGRES








--------------------------------------------------------------------------------
--------------------------------- TABLES CHANGE --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_inv_additional_params
*/ ------------------------------
ALTER TABLE t_inv_additional_params ADD uuid VARCHAR(160) NOT NULL DEFAULT uuid_generate_v1() ;
ALTER TABLE t_inv_additional_params ADD data_take_id INTEGER ;
CREATE INDEX ix_t_inv_additional_params03 ON t_inv_additional_params USING BTREE (data_take_id) ;


/* ------------------------------
	t_downlink_session_tracks
*/ ------------------------------
ALTER TABLE t_downlink_session_tracks RENAME COLUMN downlik_stop_time TO downlink_stop_time;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'pds-13.01.00' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-13.01.00',
		'2021-06-30',
		3,
		2,
		'PDS_13-1-0',
		'Added data_take_id field on table t_inv_additional_params'
) ;


