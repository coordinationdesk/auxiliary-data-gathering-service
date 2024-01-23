
-- Version 12.17.00
-- POSTGRES








--------------------------------------------------------------------------------
-------------------------------- TABLES CREATION -------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_downlink_session_tracks
*/ ------------------------------
CREATE TABLE t_downlink_session_tracks (
	id SERIAL NOT NULL,
	session_id VARCHAR(64) NOT NULL,
	downlik_stop_time TIMESTAMP(6)
);
ALTER TABLE t_downlink_session_tracks
	ADD CONSTRAINT pk_t_downlink_session_tracks
		PRIMARY KEY (id) ;

ALTER TABLE t_downlink_session_tracks 
	ADD CONSTRAINT uq_downlink_session_tracks01
		UNIQUE (session_id) ;



/* ------------------------------
	t_inv_additional_params
*/ ------------------------------
CREATE TABLE t_inv_additional_params (
	id BIGSERIAL NOT NULL,
	inv_id BIGINT NOT NULL,
	timeliness_key_id INTEGER,
	granule_number INTEGER,
	granule_position VARCHAR(24),
	dump_start TIMESTAMP(6)
);
ALTER TABLE t_inv_additional_params
	ADD CONSTRAINT pk_t_inv_additional_params
		PRIMARY KEY (id) ;

ALTER TABLE t_inv_additional_params 
	ADD CONSTRAINT fk_inv_additional_params01
		FOREIGN KEY (inv_id)
			REFERENCES t_inventory (id_inv)
				ON DELETE CASCADE ;

ALTER TABLE t_inv_additional_params 
	ADD CONSTRAINT fk_inv_additional_params02
		FOREIGN KEY (timeliness_key_id)
			REFERENCES t_downlink_session_tracks (id)
				ON DELETE CASCADE ;

CREATE INDEX ix_t_inv_additional_params01 ON t_inv_additional_params USING BTREE (inv_id) ;
CREATE INDEX ix_t_inv_additional_params02 ON t_inv_additional_params USING BTREE (timeliness_key_id) ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'pds-12.17.00' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-12.17.00',
		'2021-05-27',
		3,
		2,
		'PDS_12-17-0',
		'Added new tables t_downlink_session_tracks and t_inv_additional_params (CI-85)'
) ;


