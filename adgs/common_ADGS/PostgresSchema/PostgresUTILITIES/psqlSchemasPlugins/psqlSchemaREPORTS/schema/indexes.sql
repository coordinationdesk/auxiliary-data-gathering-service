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
	Revision 1.2  2013/03/27 09:54:21  achnat
	Moved t_filetypes_stylesheets and t_stylesheetoutputtypes in REPORT schema
	
	
*/





--------------------------------------------------------------------------------
--
--                                  INDEXES
--
--------------------------------------------------------------------------------

/* --------------------------
	t_filetypes_x_stylesheets
*/ --------------------------
CREATE INDEX ix_t_filetypes_x_stylesheets01 ON t_filetypes_x_stylesheets USING BTREE (filetype_id) ;
CREATE INDEX ix_t_filetypes_x_stylesheets02 ON t_filetypes_x_stylesheets USING BTREE (stylesheet_id) ;
CREATE INDEX ix_t_filetypes_x_stylesheets03 ON t_filetypes_x_stylesheets USING BTREE (outputtype_id) ;


