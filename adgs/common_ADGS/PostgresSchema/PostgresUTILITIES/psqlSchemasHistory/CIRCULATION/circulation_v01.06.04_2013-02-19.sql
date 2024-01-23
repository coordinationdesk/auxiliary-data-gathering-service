
-- Version 1.6.4
-- POSTGRES








---------------------------------------------------------------------------------
------------------------ TRIGGERS FUNCTIONS MODIFICATION ------------------------
---------------------------------------------------------------------------------

/* ------------------------------
	tf_datastoragequeueupdate
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_datastoragequeueupdate() RETURNS TRIGGER AS $$
DECLARE
  	status INTEGER ;
BEGIN
  	status := p_inserteventtableitem('t_circulationsqueue', 'status:' || NEW.status_id) ;
	status := p_datastoragequeue_update(NEW.id, NEW.status_id) ;
	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;








--------------------------------------------------------------------------------
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
		'circulation-1.6.4',
		'2013-02-19',
		6,
		20,
		'CIRCULATION_1-6-4',
		'Changed tf_datastoragequeueupdate trigger function to write the changes of status in the PDS_event file'
) ;
