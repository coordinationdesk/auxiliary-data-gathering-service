/*

Copyright 2010-2020, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S.  Databases schema constraints script $

	$Id$

	$Author$

	$Log$
	Revision 1.2  2013/03/27 09:54:20  achnat
	Moved t_filetypes_stylesheets and t_stylesheetoutputtypes in REPORT schema
	
*/





--------------------------------------------------------------------------------
--
--    	  	  	  	  	  	  	 CONSTRAINTS
--
--------------------------------------------------------------------------------

-------------------------------------------------
--------------- CHECK CONSTRAINTS ---------------
-------------------------------------------------





-------------------------------------------------
--------------- UNIQUE CONSTRAINTS --------------
-------------------------------------------------

/* ------------------------------
	t_filetypes_x_stylesheets
*/ ------------------------------
ALTER TABLE t_filetypes_x_stylesheets 
	ADD CONSTRAINT uq_filetypes_x_stylesheets01
		UNIQUE (filetype_id, outputtype_id, stylesheet_id) ;


/* ------------------------------
	t_stylesheetoutputtypes
*/ ------------------------------
ALTER TABLE t_stylesheetoutputtypes 
	ADD CONSTRAINT uq_stylesheetoutputtypes01
		UNIQUE (outputtype) ;


