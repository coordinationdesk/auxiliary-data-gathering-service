
-- Version 11.14.00
-- POSTGRES








--------------------------------------------------------------------------------
-------------------------------- TABLES CHANGES --------------------------------
--------------------------------------------------------------------------------

/* ------------------------------
	t_receptionrules
*/ ------------------------------
ALTER TABLE t_receptionrules ADD satellite_id INT2 ;
ALTER TABLE t_receptionrules ADD mission INT2 ;
ALTER TABLE t_receptionrules ADD sensor_id INT2 ;

ALTER TABLE t_receptionrules 
	ADD CONSTRAINT fk_receptionrules7
		FOREIGN KEY (satellite_id)
			REFERENCES t_satellites (satelliteid) 
				ON DELETE CASCADE ;

ALTER TABLE t_receptionrules 
	ADD CONSTRAINT fk_receptionrules8
		FOREIGN KEY (sensor_id)
			REFERENCES t_sensors (sensorid) 
				ON DELETE CASCADE ;

CREATE INDEX ix_t_receptionrules07 ON t_receptionrules USING BTREE (satellite_id) ;
CREATE INDEX ix_t_receptionrules08 ON t_receptionrules USING BTREE (sensor_id) ;

ALTER TABLE t_receptionrules
	ADD CONSTRAINT ck_receptionrules04
		CHECK ((satellite_id IS NULL AND (mission IS NOT NULL OR sensor_id IS NOT NULL)) != TRUE) ;

ALTER TABLE t_receptionrules
	ADD CONSTRAINT ck_receptionrules05
		CHECK ((mission IS NULL AND sensor_id IS NOT NULL) != TRUE) ;

DROP INDEX uq_receptionrules02 ;
CREATE UNIQUE INDEX uq_receptionrules02 
	ON t_receptionrules (filetype_id, localpath, COALESCE(satellite_id, 0), COALESCE(mission, 0), COALESCE(sensor_id, 0))
		WHERE active = 't' ;


/* --------------------------
	t_distributionqueue
*/ --------------------------
CREATE INDEX ix_t_distributionqueue03 ON t_distributionqueue USING BTREE (creationdate) ;
CREATE INDEX ix_t_distributionqueue04 ON t_distributionqueue USING BTREE (distributeddate) ;


/* ------------------------------
	t_orderstatetransitions
*/ ------------------------------
CREATE INDEX ix_t_orderstatetransitions01 ON t_orderstatetransitions USING BTREE (orderid) ;


/* ------------------------------
	t_ordersqueue
*/ ------------------------------
CREATE INDEX ix_t_ordersqueue10 ON t_ordersqueue USING BTREE (productionstoptime) ;








---------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS ----------------------------------
---------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'pds-11.14.00' ;

INSERT INTO t_schema (
		version, 
		modificationdate,
		modificationtype,
		schematype,
		cvstag,
		description )
VALUES	(
		'pds-11.14.00',
		'2016-03-04',
		3,
		2,
		'PDS_11-14-0',
		'Added sat/sen/mis fileds on t_receptionrules. Added indexes for reports'
) ;


