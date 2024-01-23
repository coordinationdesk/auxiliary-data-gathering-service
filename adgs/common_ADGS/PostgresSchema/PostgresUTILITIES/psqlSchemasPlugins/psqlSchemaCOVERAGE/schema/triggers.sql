/*

Copyright 2010-2020, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S. Databases schema triggers script $

	$Id$

	$Author$

	$Log$
	Revision 1.1.1.1  2013/10/08 15:07:46  achnat
	psqlSchemaCOVERAGE
	
*/





--------------------------------------------------------------------------------
--
--    	  	  	  	  	  	  	  TRIGGERS
--
--------------------------------------------------------------------------------

/* ------------------------------
	t_cov_relevancies
*/ ------------------------------
CREATE TRIGGER tr_cov_relevancies_insert AFTER INSERT
	ON t_cov_relevancies 
		FOR EACH ROW EXECUTE PROCEDURE tf_cov_relevancies_clean() ;


/* ------------------------------
	t_cov_rules
*/ ------------------------------
CREATE TRIGGER tr_cov_rules_update BEFORE UPDATE
	ON t_cov_rules 
		FOR EACH ROW EXECUTE PROCEDURE tf_cov_rules_last_update() ;


