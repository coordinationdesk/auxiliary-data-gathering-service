/*

Copyright 2010-2020, Advanced Computer Systems , Inc.
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
	Revision 1.3  2013/06/04 12:49:40  achnat
	Added new table t_filetypes_x_stylesheets and removed t_filetypes_stylesheets. Added view v_filetypes_x_stylesheets.
	
	Revision 1.2  2013/03/27 09:54:21  achnat
	Moved t_filetypes_stylesheets and t_stylesheetoutputtypes in REPORT schema
	
	Revision 1.1.1.1  2012/05/16 10:31:45  achnat
	Import psqlSchemaREPORTS
	
*/





--------------------------------------------------------------------------------
--
--    	  	  	  	  	  	  	  TABLES
--
--------------------------------------------------------------------------------

/* ------------------------------
	t_filetypes_x_stylesheets
*/ ------------------------------
CREATE TABLE t_filetypes_x_stylesheets (
	id SERIAL NOT NULL,
	filetype_id INTEGER NOT NULL,
	stylesheet_id INTEGER NOT NULL,
	outputtype_id INT2 NOT NULL,
	fileextension VARCHAR(16)
) ;
ALTER TABLE t_filetypes_x_stylesheets 
	ADD CONSTRAINT pk_t_filetypes_x_stylesheets
		PRIMARY KEY (id) ;



/* ------------------------------
	t_lastacknowledge
*/ ------------------------------
CREATE TABLE t_lastacknowledge (
	id SERIAL NOT NULL,
	applicationname VARCHAR(25) NOT NULL,
	applicationdata VARCHAR(128) NOT NULL,
	tstamp TIMESTAMP(6) NOT NULL
) ;
ALTER TABLE t_lastacknowledge 
	ADD CONSTRAINT pk_t_lastacknowledge
		PRIMARY KEY (id) ;



/* ------------------------------
	t_stylesheetoutputtypes
*/ ------------------------------
CREATE TABLE t_stylesheetoutputtypes (
	id INT2 NOT NULL,
	outputtype VARCHAR(8) NOT NULL
) ;
ALTER TABLE t_stylesheetoutputtypes 
	ADD CONSTRAINT pk_t_stylesheetoutputtypes
		PRIMARY KEY (id) ;


