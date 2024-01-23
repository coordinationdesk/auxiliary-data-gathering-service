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
	Revision 1.3  2013/05/16 13:39:09  achnat
	Added t_cr_x_filetypes table
	
	Revision 1.2  2013/04/19 10:34:11  achnat
	Changed procedure p_cr_distribution and added new indexes
	
	Revision 1.1.1.1  2013/03/19 10:40:09  achnat
	Import psqlSchemaCR
	
*/





--------------------------------------------------------------------------------
--
--    	  	  	  	  	  	  	  INDEXES
--
--------------------------------------------------------------------------------


/* ------------------------------
	t_crgroups
*/ ------------------------------
CREATE INDEX ix_t_crgroups01 ON t_crgroups USING BTREE (virtual_filetype_id) ;


/* ------------------------------
	t_crrelations
*/ ------------------------------
CREATE INDEX ix_t_crrelations01 ON t_crrelations USING BTREE (crgroup_id) ;
CREATE INDEX ix_t_crrelations02 ON t_crrelations USING BTREE (repository_id) ;
CREATE INDEX ix_t_crrelations03 ON t_crrelations USING BTREE (distributionrule_id) ;


/* ------------------------------
	t_crtemporaries
*/ ------------------------------
CREATE INDEX ix_t_crtemporaries01 ON t_crtemporaries USING BTREE (inventory_id) ;
CREATE INDEX ix_t_crtemporaries02 ON t_crtemporaries USING BTREE (crgroup_id) ;
CREATE INDEX ix_t_crtemporaries03 ON t_crtemporaries USING BTREE (tstamp) ;
CREATE INDEX ix_t_crtemporaries04 ON t_crtemporaries USING BTREE (author) ;


/* ------------------------------
	t_crhistory
*/ ------------------------------
CREATE INDEX ix_t_crhistory01 ON t_crhistory USING BTREE (inventory_id) ;
CREATE INDEX ix_t_crhistory02 ON t_crhistory USING BTREE (crgroup_id) ;
CREATE INDEX ix_t_crhistory03 ON t_crhistory USING BTREE (tstamp) ;
CREATE INDEX ix_t_crhistory04 ON t_crhistory USING BTREE (author) ;


/* ------------------------------
	t_cr_x_filetypes
*/ ------------------------------
CREATE INDEX ix_t_cr_x_filetypes01 ON t_cr_x_filetypes USING BTREE (crgroup_id) ;
CREATE INDEX ix_t_cr_x_filetypes02 ON t_cr_x_filetypes USING BTREE (filetype_id) ;


