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
	Revision 1.2  2013/10/10 09:29:27  achnat
	Created schema for COVERAGE_1-0-0
	
	Revision 1.1.1.1  2013/10/08 15:07:46  achnat
	psqlSchemaCOVERAGE
	
*/





--------------------------------------------------------------------------------
--
--    	  	  	  	  	  	  	  CONSTRAINTS
--
--------------------------------------------------------------------------------


-------------------------------------------------
--------------- CHECK CONSTRAINTS ---------------
-------------------------------------------------

/* ------------------------------
	t_cov_rules
*/ ------------------------------
ALTER TABLE t_cov_rules 
	ADD CONSTRAINT ck_coveragerules01
		CHECK (rule_type IN ('A', 'B', 'C')) ;

ALTER TABLE t_cov_rules 
	ADD CONSTRAINT ck_coveragerules02
		CHECK (int_a > int_b) ;

ALTER TABLE t_cov_rules 
	ADD CONSTRAINT ck_coveragerules03
		CHECK ((n_min > 0) AND (n_max > 0)) ;

ALTER TABLE t_cov_rules 
	ADD CONSTRAINT ck_coveragerules04
		CHECK ((cover_pct >= 0) AND (cover_pct <= 100)) ;

ALTER TABLE t_cov_rules 
	ADD CONSTRAINT ck_coveragerules05
		CHECK (NOT (FixedValidityStart IS NOT NULL AND FixedValidityStop IS NOT NULL)) ;








-------------------------------------------------
--------------- UNIQUE CONSTRAINTS --------------
-------------------------------------------------






