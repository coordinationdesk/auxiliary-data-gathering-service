
-- Version 10.3.2
-- POSTGRES








---------------------------------------------------------------------------------
------------------------------ TABLES MODIFICATION ------------------------------
---------------------------------------------------------------------------------
ALTER TABLE t_inventory ADD logical_id VARCHAR(256) ;
CREATE INDEX ix_t_inventory04 ON t_inventory USING btree ("logical_id") ;








---------------------------------------------------------------------------------
----------------------------- TRIGGERS MODIFICATION -----------------------------
---------------------------------------------------------------------------------

-- !!! THIS CHANGE IS NEEDED ONLY IN POSTGRES !!!

DROP TRIGGER tr_distributionqueueupdate1 ON t_distributionqueue ;
CREATE TRIGGER tr_distributionqueueupdate1 AFTER UPDATE
  ON t_distributionqueue
    FOR EACH ROW EXECUTE PROCEDURE tf_upd_p_distributionstate() ;
	
-- END OF ONLY POSTGRES








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
		'pds-10.3.2',
		'2012-05-11',
		3,
		2,
		'PDS_10-3-2',
		'Added new field to t_tinventory and changed tr_distributionqueueupdate1 trigger.'
) ;
