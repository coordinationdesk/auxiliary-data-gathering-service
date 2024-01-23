/*

Copyright 2010-2015, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S. Databases schema tables script $

	$Id$

	$Author$

	$Log$
	Revision 1.4  2015/01/26 14:06:20  achnat
	Added description filed on t_schematypes and added MPM schema
	
	Revision 1.3  2014/05/05 13:06:25  achnat
	Cosmetic changes
	
	Revision 1.2  2011/06/08 14:41:33  achnat
	Increased version field on t_schema.
	
	Revision 1.1.1.1  2010/12/13 16:24:08  achnat
	Import psqlSchemaVERSION
	
*/





--------------------------------------------------------------------------------
--
--    	  	  	  	  	  	  	  TABLES
--
--------------------------------------------------------------------------------


/* ------------------------------
	t_schemamodificationtypes
*/ ------------------------------
CREATE TABLE t_schemamodificationtypes (
    id INT2 NOT NULL,
    modificationtype VARCHAR(32) NOT NULL
);
ALTER TABLE t_schemamodificationtypes
	ADD CONSTRAINT pk_t_schemamodificationtype
		PRIMARY KEY (id) ;



/* ------------------------------
	t_schematypes
*/ ------------------------------
CREATE TABLE t_schematypes (
	id INT2 NOT NULL,
	schematype VARCHAR(32) NOT NULL,
	description VARCHAR(128)
) ;
ALTER TABLE t_schematypes
	ADD CONSTRAINT pk_t_schematypes
		PRIMARY KEY (id) ;



/* ------------------------------
	t_schema
*/ ------------------------------
CREATE TABLE t_schema (
    id SERIAL NOT NULL,
    version VARCHAR(24) NOT NULL,
    modificationdate DATE NOT NULL,
    modificationtype INTEGER DEFAULT 1 NOT NULL,
	appliedon DATE DEFAULT NOW() NOT NULL,
	schematype INTEGER DEFAULT 1 NOT NULL,
	cvstag VARCHAR(64),
    description TEXT
) ;
ALTER TABLE t_schema
	ADD CONSTRAINT pk_t_schema
		PRIMARY KEY (id) ;


