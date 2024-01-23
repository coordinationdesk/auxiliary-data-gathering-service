

/*
	 This patch is needed only on postgres schemas having PDS schema version between 7.2.0 and 10.3.1 (included)
*/





---------------------------------------------------------------------------------
------------------------------------- PATCH -------------------------------------
---------------------------------------------------------------------------------

DROP TRIGGER tr_distributionqueueupdate1 ON t_distributionqueue ;

CREATE TRIGGER tr_distributionqueueupdate1 AFTER UPDATE
  ON t_distributionqueue
    FOR EACH ROW EXECUTE PROCEDURE tf_upd_p_distributionstate() ;





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
		'tr_dist_queue_upd_patch',
		'2011-06-19',
		4,
		2,
		'',
		'Changed execution time on tr_distributionqueueupdate1 trigger from BEFORE to AFTER.'
) ;
