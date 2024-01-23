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
	Revision 1.2  2013/10/10 09:29:27  achnat
	Created schema for COVERAGE_1-0-0
	
	Revision 1.1.1.1  2013/10/08 15:07:46  achnat
	psqlSchemaCOVERAGE
	
*/





--------------------------------------------------------------------------------
--
--    	  	  	  	  	  	  	  TABLES
--
--------------------------------------------------------------------------------

/* ------------------------------
	t_cov_periods
*/ ------------------------------
CREATE TABLE t_cov_periods (
	id_interval BIGSERIAL NOT NULL,
	coveragerule_id INTEGER,
	t_begin TIMESTAMP(6) NOT NULL,
	t_end TIMESTAMP(6) NOT NULL,
	exp_val_start TIMESTAMP NOT NULL,
	exp_val_stop TIMESTAMP NOT NULL,
	events INTEGER DEFAULT 0 NOT NULL,
	cover_pct FLOAT DEFAULT 0 NOT NULL,
	creationdate TIMESTAMP(6) DEFAULT NOW() NOT NULL,
	reported BOOLEAN DEFAULT 'f' NOT NULL
) ;
ALTER TABLE t_cov_periods 
	ADD CONSTRAINT pk_t_cov_periods
		PRIMARY KEY (id_interval) ;



/* ------------------------------
	t_cov_relevancies
*/ ------------------------------
CREATE TABLE t_cov_relevancies (
	id_relevancy BIGSERIAL NOT NULL,
	period_id BIGINT NOT NULL,
	inv_id BIGINT NOT NULL,
	inv_val_start TIMESTAMP NOT NULL,
	inv_val_stop TIMESTAMP NOT NULL,
	creationdate TIMESTAMP(6) DEFAULT NOW() NOT NULL
) ;
ALTER TABLE t_cov_relevancies 
	ADD CONSTRAINT pk_t_cov_relevancies
		PRIMARY KEY (id_relevancy) ;



/* ------------------------------
	t_cov_rules
*/ ------------------------------
CREATE TABLE t_cov_rules (
	id_coveragerule SERIAL NOT NULL,
	rule_type CHAR(1) NOT NULL,
	time_0 TIMESTAMP(6) NOT NULL,
	t_sec INTEGER NOT NULL,
	int_a INTEGER DEFAULT 1 NOT NULL,
	int_b INTEGER DEFAULT 0 NOT NULL,
	n_min INTEGER DEFAULT 1 NOT NULL,
	n_max INTEGER DEFAULT 1 NOT NULL,
	delta_gap INTEGER DEFAULT 0 NOT NULL,
	cover_pct FLOAT DEFAULT 0 NOT NULL,
	check_delta_gap BOOLEAN DEFAULT 'f' NOT NULL,
	check_cover_pct BOOLEAN DEFAULT 'f' NOT NULL,
	deltawait_sec INTEGER DEFAULT 0 NOT NULL,
	deltamissing_sec INTEGER,
	creation_date TIMESTAMP(6) DEFAULT NOW() NOT NULL,
	last_update TIMESTAMP(6) DEFAULT NOW() NOT NULL,
	author VARCHAR(24) NOT NULL,
	description VARCHAR(128),
	fixedvaliditystart TIMESTAMP(6),
	fixedvaliditystop TIMESTAMP(6)
) ;
ALTER TABLE t_cov_rules 
	ADD CONSTRAINT pk_t_cov_rules
		PRIMARY KEY (id_coveragerule) ;



/* ------------------------------
	t_cov_x_filetypes
*/ ------------------------------
CREATE TABLE t_cov_x_filetypes (
	coveragerule_id INTEGER NOT NULL,
	filetype_id INTEGER NOT NULL
) ;
ALTER TABLE t_cov_x_filetypes 
	ADD CONSTRAINT pk_t_cov_x_filetypes
		PRIMARY KEY (coveragerule_id, filetype_id) ;


