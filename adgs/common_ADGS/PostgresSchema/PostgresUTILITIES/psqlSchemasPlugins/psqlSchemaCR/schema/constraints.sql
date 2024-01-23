/*

Copyright 2010-2020, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S. Databases schema constraints script $

	$Id$

	$Author$

	$Log$
	Revision 1.1.1.1  2013/03/19 10:40:09  achnat
	Import psqlSchemaCR
	
*/





--------------------------------------------------------------------------------
--
--    	  	  	  	  	  	  	  CONSTRAINTS
--
--------------------------------------------------------------------------------


-------------------------------------------------
--------------- CHECK CONSTRAINTS ---------------
-------------------------------------------------








-------------------------------------------------
--------------- UNIQUE CONSTRAINTS --------------
-------------------------------------------------

/* ------------------------------
	t_crgroups
*/ ------------------------------
ALTER TABLE t_crgroups 
	ADD CONSTRAINT uq_crgroups01
		UNIQUE (name) ;


/* ------------------------------
	t_crrelations
*/ ------------------------------
ALTER TABLE t_crrelations 
	ADD CONSTRAINT uq_crrelations01
		UNIQUE (crgroup_id, repository_id, distributionrule_id) ;


/* ------------------------------
	t_cr_x_filetypes
*/ ------------------------------
ALTER TABLE t_cr_x_filetypes 
	ADD CONSTRAINT uq_cr_x_filetypes01
		UNIQUE (crgroup_id, filetype_id) ;


