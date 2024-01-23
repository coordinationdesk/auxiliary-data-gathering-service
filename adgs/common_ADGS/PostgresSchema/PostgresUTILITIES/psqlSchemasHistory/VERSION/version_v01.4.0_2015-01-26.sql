
-- Version 1.4.0
-- POSTGRES








/* ------------------------------
        t_schematypes
*/ ------------------------------
ALTER TABLE t_schematypes ADD description VARCHAR(128) ;

COMMENT ON COLUMN t_schematypes.description IS 'Little description of the type of schema';

INSERT INTO t_schematypes (id, schematype, description) VALUES (26, 'MPM', 'Mission Performance Monitoring') ;


