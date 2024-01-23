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
	Revision 1.5  2013/09/17 10:00:37  achnat
	Added virtual_filetype_id field on t_crgroups
	
	Revision 1.4  2013/09/03 14:40:55  achnat
	Added crtag field to t_crhistory and t_crtemporaries tables and changed p_cr_distribution
	
	Revision 1.3  2013/05/21 09:46:03  achnat
	Added mailaddresses field on t_crgroups
	
	Revision 1.2  2013/05/16 13:39:09  achnat
	Added t_cr_x_filetypes table
	
	Revision 1.1.1.1  2013/03/19 10:40:09  achnat
	Import psqlSchemaCR
	
*/





--------------------------------------------------------------------------------
--
--    	  	  	  	  	  	  	  TABLES
--
--------------------------------------------------------------------------------


/* ------------------------------
	t_crgroups
*/ ------------------------------
CREATE TABLE t_crgroups (
	id_crgroup SERIAL NOT NULL,
	name VARCHAR(24) NOT NULL,
	mailaddresses VARCHAR(1024),
	description VARCHAR(128),
	virtual_filetype_id INTEGER
) ;
ALTER TABLE t_crgroups 
	ADD CONSTRAINT pk_t_crgroups
		PRIMARY KEY (id_crgroup) ;



/* ------------------------------
	t_crrelations
*/ ------------------------------
CREATE TABLE t_crrelations (
	id_crrelation SERIAL NOT NULL,
	crgroup_id INT NOT NULL,
	repository_id INT NOT NULL,
	distributionrule_id INT NOT NULL
) ;
ALTER TABLE t_crrelations 
	ADD CONSTRAINT pk_t_crrelations
		PRIMARY KEY (id_crrelation) ;



/* ------------------------------
	t_crtemporaries
*/ ------------------------------
CREATE TABLE t_crtemporaries (
	inventory_id BIGINT NOT NULL,
	crgroup_id INT NOT NULL,
	tstamp TIMESTAMP(6) NOT NULL,
	author VARCHAR(24) NOT NULL,
	crtag VARCHAR(24),
	crcomment VARCHAR(128)
) ;
ALTER TABLE t_crtemporaries 
	ADD CONSTRAINT pk_t_crtemporaries
		PRIMARY KEY (inventory_id, crgroup_id, tstamp) ;



/* ------------------------------
	t_crhistory
*/ ------------------------------
CREATE TABLE t_crhistory (
	inventory_id BIGINT NOT NULL,
	crgroup_id INT NOT NULL,
	tstamp TIMESTAMP(6) NOT NULL,
	author VARCHAR(24) NOT NULL,
	crtag VARCHAR(24),
	crcomment VARCHAR(128)
) ;
ALTER TABLE t_crhistory 
	ADD CONSTRAINT pk_t_crhistory
		PRIMARY KEY (inventory_id, crgroup_id, tstamp) ;



/* ------------------------------
	t_cr_x_filetypes
*/ ------------------------------
CREATE TABLE t_cr_x_filetypes (
	id_cr_x_filetype SERIAL NOT NULL,
	crgroup_id INT NOT NULL,
	filetype_id INT NOT NULL
) ;
ALTER TABLE t_cr_x_filetypes 
	ADD CONSTRAINT pk_t_cr_x_filetypes
		PRIMARY KEY (id_cr_x_filetype) ;



