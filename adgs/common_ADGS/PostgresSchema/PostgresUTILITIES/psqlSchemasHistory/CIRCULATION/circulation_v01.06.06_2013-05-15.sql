
-- Version 1.6.6
-- POSTGRES








---------------------------------------------------------------------------------
------------------------- TRIGGER FUNCTION MODIFICATION -------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	tf_datastoragequeueupdate
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_datastoragequeueupdate() RETURNS TRIGGER AS $$
DECLARE
	proc_exists INTEGER ;
  	status INTEGER ;
BEGIN
	proc_exists := (SELECT count(*) FROM pg_proc WHERE proname = 'p_events') ;
  	IF proc_exists > 0 THEN
		PERFORM p_events('t_circulationsqueue', 'status:' || NEW.status_id) ;
	ELSE 
		PERFORM p_inserteventtableitem('t_circulationsqueue', 'status:' || NEW.status_id) ;
	END IF ;

	status := p_datastoragequeue_update(NEW.id, NEW.status_id) ;

	RETURN NEW ;
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
		'circulation-1.6.6',
		'2013-04-15',
		6,
		20,
		'CIRCULATION_1-6-6',
		'Changed tf_datastoragequeueupdate trigger function.'
) ;


