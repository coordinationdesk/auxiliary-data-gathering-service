
-- Version 10.5.3
-- POSTGRES








---------------------------------------------------------------------------------
------------------------------ TABLES MODIFICATION ------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_inventorycomments
*/ ------------------------------
ALTER TABLE t_inventorycomments ADD id_inventorycomment BIGSERIAL NOT NULL;
ALTER TABLE t_inventorycomments DROP CONSTRAINT pk_t_inventorycomments ;
ALTER TABLE t_inventorycomments
	ADD CONSTRAINT pk_t_inventorycomments
		PRIMARY KEY (id_inventorycomment) ;
ALTER TABLE t_inventorycomments ALTER COLUMN date SET DEFAULT now() ; 
CREATE INDEX ix_t_inventorycomments01 ON t_inventorycomments USING BTREE (filename, fileversion) ;


/* ------------------------------
	t_satrelations
*/ ------------------------------
CREATE INDEX ix_t_satrelations01 ON t_satrelations USING BTREE (satelliteid) ;
CREATE INDEX ix_t_satrelations02 ON t_satrelations USING BTREE (sensorid) ;








---------------------------------------------------------------------------------
----------------------------------- TRIGGERS ------------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_completed_orders
*/ ------------------------------
CREATE TRIGGER tr_completedordersinsert AFTER INSERT
	ON t_completed_orders EXECUTE PROCEDURE tf_inserteventtableitem('t_completed_orders', 'INS') ;

CREATE TRIGGER tr_completedordersupdate AFTER UPDATE
	ON t_completed_orders EXECUTE PROCEDURE tf_inserteventtableitem('t_completed_orders', 'UPD') ;








--##############################################################################
--#####                                                                    #####
--#####   THIS PART IS NEEDED ONLY IF THE EVENTS DATABLADES IS INSTALLED   #####
--#####                                                                    #####
--##############################################################################

---------------------------------------------------------------------------------
--------------------------------- TABLES INSERT ---------------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	t_dbparameters
*/ ------------------------------
INSERT INTO t_dbparameters (parameter, parametertype_id, value_str) VALUES ('broadcast_events_path', 2, '/usr/local/acs/tmp') ;


---------------------------------------------------------------------------------
---------------------------- PROCEDURES MODIFICATION ----------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	p_inserteventtableitem
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_inserteventtableitem(
	mytable VARCHAR(64), 
	myaction VARCHAR(16)) RETURNS INTEGER AS $$

DECLARE
	event_file VARCHAR(64) ;
	ret INTEGER;

BEGIN
	IF (SELECT count(*) FROM t_dbparameters WHERE parameter = 'broadcast_events_path') > 0 THEN
		event_file := trim((SELECT value_str FROM t_dbparameters WHERE parameter = 'broadcast_events_path')) || '/PDS_events' ;
	ELSE
		event_file := '/usr/acs/tmp/PDS_events' ;
	END IF ;
	
	ret := f_acs_traceEvent(event_file, mytable, myaction) ;
	
	RETURN ret;
	
END ;
$$ LANGUAGE plpgsql ;

--##############################################################################
--#####                   END OF EVENTS DATABLADES PART                    #####
--##############################################################################








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
		'pds-10.5.3',
		'2013-02-07',
		3,
		2,
		'PDS_10-5-3',
		'Added default value for date on t_inventorycomments. Added event trigger on t_completed_orders. Changed p_inserteventtableitem to implement the broadcast_events_path customization.'
) ;
