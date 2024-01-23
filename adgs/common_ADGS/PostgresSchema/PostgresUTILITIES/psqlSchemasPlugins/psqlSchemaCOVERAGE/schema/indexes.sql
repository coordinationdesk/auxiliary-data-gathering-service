/*

Copyright 2010-2020, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S. Databases schema indexes script $

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
--    	  	  	  	  	  	  	  INDEXES
--
--------------------------------------------------------------------------------

/* ------------------------------
	t_cov_periods
*/ ------------------------------
CREATE INDEX ix_t_cov_periods01 ON t_cov_periods USING BTREE (coveragerule_id) ;
CREATE INDEX ix_t_cov_periods02 ON t_cov_periods USING BTREE (t_begin) ;
CREATE INDEX ix_t_cov_periods03 ON t_cov_periods USING BTREE (t_end) ;
CREATE INDEX ix_t_cov_periods04 ON t_cov_periods USING BTREE (exp_val_start) ;
CREATE INDEX ix_t_cov_periods05 ON t_cov_periods USING BTREE (exp_val_stop) ;
CREATE INDEX ix_t_cov_periods06 ON t_cov_periods USING BTREE (creationdate) ;


/* ------------------------------
	t_cov_relevancies
*/ ------------------------------
CREATE INDEX ix_t_cov_relevancies01 ON t_cov_relevancies USING BTREE (period_id) ;
CREATE INDEX ix_t_cov_relevancies02 ON t_cov_relevancies USING BTREE (inv_id) ;


/* ------------------------------
	t_cov_rules
*/ ------------------------------
CREATE INDEX ix_t_cov_rules01 ON t_cov_rules USING BTREE (last_update) ;


/* ------------------------------
	t_cov_x_filetypes
*/ ------------------------------
CREATE INDEX ix_t_cov_x_filetypes01 ON t_cov_x_filetypes USING BTREE (coveragerule_id) ;
CREATE INDEX ix_t_cov_x_filetypes02 ON t_cov_x_filetypes USING BTREE (filetype_id) ;


