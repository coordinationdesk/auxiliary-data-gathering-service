
-- Version 13.05.00
-- POSTGRES








--------------------------------------------------------------------------------
-------------------------------- TABLES CREATION -------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_cdp_frame_grid
*/ ------------------------------
CREATE TABLE t_cdp_frame_grid (
    id SERIAL NOT NULL,
    satelliteid INTEGER NOT NULL,
	filetype VARCHAR(16) NOT NULL,
	ordertype VARCHAR(8) NOT NULL,
	orbitNumber INTEGER NOT NULL,
	frameStart TIMESTAMP(6) NOT NULL,
	frameNumber INTEGER NOT NULL,
	orderEmitted BOOLEAN NOT NULL DEFAULT FALSE,
	creationTime TIMESTAMP(6) NOT NULL DEFAULT NOW()
);
ALTER TABLE t_cdp_frame_grid 
	ADD CONSTRAINT pk_t_cdp_frame_grid
		PRIMARY KEY (id) ;

ALTER TABLE t_cdp_frame_grid 
	ADD CONSTRAINT uq_cdp_frame_grid01 
		UNIQUE (satelliteid, filetype, ordertype, orbitnumber, frameNumber) ;

ALTER TABLE t_cdp_frame_grid 
	ADD CONSTRAINT fk_cdp_frame_grid01
		FOREIGN KEY (filetype) 
			REFERENCES t_filetypes (filetype) 
				ON DELETE CASCADE ;

ALTER TABLE t_cdp_frame_grid 
	ADD CONSTRAINT fk_cdp_frame_grid02
		FOREIGN KEY (ordertype) 
			REFERENCES t_ordertypes (ordertype) 
				ON DELETE CASCADE ;

CREATE INDEX ix_t_cdp_frame_grid01 ON t_cdp_frame_grid USING BTREE (satelliteid) ;
CREATE INDEX ix_t_cdp_frame_grid02 ON t_cdp_frame_grid USING BTREE (filetype) ;
CREATE INDEX ix_t_cdp_frame_grid03 ON t_cdp_frame_grid USING BTREE (ordertype) ;
CREATE INDEX ix_t_cdp_frame_grid04 ON t_cdp_frame_grid USING BTREE (orbitNumber) ;
CREATE INDEX ix_t_cdp_frame_grid05 ON t_cdp_frame_grid USING BTREE (frameStart) ;
CREATE INDEX ix_t_cdp_frame_grid06 ON t_cdp_frame_grid USING BTREE (creationTime) ;




 



--------------------------------------------------------------------------------
-------------------------------- TABLES CHANGES --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_inv_additional_params 
*/ ------------------------------
ALTER TABLE t_inv_additional_params ADD nextequatorxtime TIMESTAMP(6) ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'pds-13.05.00' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-13.05.00',
		'2021-09-23',
		3,
		2,
		'PDS_13-5-0',
		'Added new table t_cdp_frame_grid. Added new filed nextequatorxtime on t_inv_additional_params table (CPS-38, CPS-44)'
) ;


