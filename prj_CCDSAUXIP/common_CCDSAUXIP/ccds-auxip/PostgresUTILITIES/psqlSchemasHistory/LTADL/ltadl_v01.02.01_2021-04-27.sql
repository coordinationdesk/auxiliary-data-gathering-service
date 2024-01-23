
-- Version 01.02.01
-- POSTGRES





-------------------------------------------------------------------------------
------------------------------- TABLES CHANGES --------------------------------
-------------------------------------------------------------------------------

/* ------------------------------
	t_ltadl_retrievequeue
*/ ------------------------------
ALTER TABLE t_ltadl_retrievequeue DROP CONSTRAINT fk_ltadl_retrievequeue04 ;
ALTER TABLE t_ltadl_retrievequeue DROP CONSTRAINT fk_ltadl_retrievequeue05 ;
ALTER TABLE t_ltadl_retrievequeue ALTER COLUMN jobresponsible DROP NOT NULL ;
ALTER TABLE t_ltadl_retrievequeue ALTER COLUMN lta_id DROP NOT NULL ;
ALTER TABLE t_ltadl_retrievequeue RENAME COLUMN batchqueue_id TO bulk_id ;
ALTER TABLE t_ltadl_retrievequeue ALTER COLUMN filter_query SET DATA TYPE VARCHAR(4096) ;
ALTER TABLE t_ltadl_retrievequeue ADD extract_metadata BOOLEAN NOT NULL DEFAULT FALSE ;
ALTER TABLE t_ltadl_retrievequeue DROP CONSTRAINT ck_ltadl_retrievequeue01 ;
ALTER TABLE t_ltadl_retrievequeue 
	ADD CONSTRAINT ck_ltadl_retrievequeue01 
		CHECK ((((filename IS NOT NULL) AND (filter_query IS NULL)) OR ((filename IS NULL) AND (filter_query IS NOT NULL)))) ;








--------------------------------------------------------------------------------
--------------------------- TRIGGER FUNCTIONS CHANGE ---------------------------
--------------------------------------------------------------------------------

/* --------------------------
	tf_ltadl_retrievequeue_reset_fields
*/ --------------------------
CREATE OR REPLACE FUNCTION tf_ltadl_retrievequeue_reset_fields() RETURNS TRIGGER AS $$

BEGIN
	IF (OLD.status_id = (SELECT id FROM t_srv_statuses WHERE status = 'ERROR') AND NEW.status_id = (SELECT id FROM t_srv_statuses WHERE status = 'ELIGIBLE')) THEN
		UPDATE t_ltadl_retrievequeue SET 
			jobresponsible = NULL,
			startdate = NULL,
			modificationdate = now(),
			completiondate = NULL,
			remote_retriever_url = NULL,
			filequeue_id = NULL,
			bulk_id = NULL 
		WHERE id = OLD.id ;
	END IF ;
	
	RETURN NEW ;
	
END ;
$$ LANGUAGE plpgsql ;








-------------------------------------------------------------------------------
---------------------------------- CONCLUSIONS --------------------------------
-------------------------------------------------------------------------------

DELETE FROM t_schema WHERE version = 'ltadl-01.02.01' ;

INSERT INTO t_schema (
	version,
	modificationdate,
	modificationtype,
	schematype,
	cvstag,
	description )
VALUES  (
	'ltadl-01.02.01',
	'2021-04-27',
	6,
	37,
	'LTADL_1-2-1',
	'Changed fields and constraints on t_ltadl_retrievequeue (LTA-271)'
) ;


