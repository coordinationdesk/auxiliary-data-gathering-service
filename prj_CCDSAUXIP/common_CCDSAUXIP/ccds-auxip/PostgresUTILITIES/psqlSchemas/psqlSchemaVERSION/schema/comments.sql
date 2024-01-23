/*

Copyright 2010-2020, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S. Databases schema comments script $

	$Id$

	$Author$

	$Log$
	Revision 1.1  2014/05/05 13:03:52  achnat
	Added comments for schema objects.
	
*/





--------------------------------------------------------------------------------
--                                                                            --
--                                  COMMENTS                                  --
--                                                                            --
--------------------------------------------------------------------------------


/* ------------------------------
	t_schema
*/ ------------------------------
COMMENT ON TABLE t_schema IS 'Contains the list of installed schemas (base and plugins) and their version';
COMMENT ON COLUMN t_schema.id IS 'Record identifier (Primary Key)';
COMMENT ON COLUMN t_schema.version IS 'Is the unique string that represents the version. It is composed by the name of schema and his version (eg: PDS_10.0.0)';
COMMENT ON COLUMN t_schema.modificationdate IS 'When the patch or upgrade has been developed';
COMMENT ON COLUMN t_schema.modificationtype IS 'Type of change (eg: UPGRADE, PATCH). It is an integer that references t_schemamodificationtypes table';
COMMENT ON COLUMN t_schema.appliedon IS 'When the patch or upgrade has been installed';
COMMENT ON COLUMN t_schema.schematype IS 'Type of schema (eg: PDS, ISM). It is an integer that references t_schematypes table';
COMMENT ON COLUMN t_schema.cvstag IS 'String used to identify the patch or the upgrade into the cvs repository';
COMMENT ON COLUMN t_schema.description IS 'Description of changes';
COMMENT ON SEQUENCE t_schema_id_seq IS 'Used to generate the primary keys of t_schema table';


/* ------------------------------
        t_schematypes
*/ ------------------------------
COMMENT ON TABLE t_schematypes IS 'Contains the list of known schema types for base schemas and plugins (eg: PDS, CIRCULATION)';
COMMENT ON COLUMN t_schematypes.id IS 'Record identifier (Primary Key)';
COMMENT ON COLUMN t_schematypes.schematype IS 'Type of schema (eg: PDS, CIRCULATION)';
COMMENT ON COLUMN t_schematypes.description IS 'Little description of the type of schema';


/* ------------------------------
	t_schemamodificationtypes
*/ ------------------------------
COMMENT ON TABLE t_schemamodificationtypes IS 'Contains the list of modification types (eg: UPGRADE, PATCH)';
COMMENT ON COLUMN t_schemamodificationtypes.id IS 'Record identifier (Primary Key)';
COMMENT ON COLUMN t_schemamodificationtypes.modificationtype IS 'Type of change (eg: UPGRADE, PATCH)';


/* ------------------------------
	v_schema_version
*/ ------------------------------
COMMENT ON VIEW v_schema_version IS 'Joins t_schema, t_schematypes and t_schemamodificationtypes. Shows only the greater version for each schema contained into t_schema table';


