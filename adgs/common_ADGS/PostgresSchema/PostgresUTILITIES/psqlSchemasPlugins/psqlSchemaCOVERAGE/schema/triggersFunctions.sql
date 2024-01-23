/*

Copyright 2010-2020, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S. Databases schema triggers functions script $

	$Id$

	$Author$

	$Log$
	Revision 1.1.1.1  2013/10/08 15:07:46  achnat
	psqlSchemaCOVERAGE
	
*/





--------------------------------------------------------------------------------
--
--    	  	  	  	  	   	   TRIGGERS FUNCTIONS
--
--------------------------------------------------------------------------------

/* ------------------------------
	tf_cov_relevancies_clean
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_cov_relevancies_clean() RETURNS TRIGGER AS $$

BEGIN
	DELETE FROM t_cov_relevancies WHERE creationdate < (NOW() - INTERVAL '1 WEEKS');
	
  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;



/* ------------------------------
	tf_cov_rules_last_update
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_cov_rules_last_update() RETURNS TRIGGER AS $$

BEGIN
	NEW.last_update := now() ;
	
  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;


