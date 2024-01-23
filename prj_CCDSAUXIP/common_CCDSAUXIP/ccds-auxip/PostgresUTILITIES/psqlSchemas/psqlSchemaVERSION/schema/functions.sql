/*

Copyright 2010-2015, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S. Databases schema functions script $

	$Id$

	$Author$

	$Log$
	Revision 1.1.1.1  2010/12/13 16:24:08  achnat
	Import psqlSchemaVERSION
	

*/





--------------------------------------------------------------------------------
--
--    	  	  	  	  	  	  	  FUNCTIONS
--
--------------------------------------------------------------------------------

/* ------------------------------
	f_get_version_number
*/ ------------------------------
CREATE OR REPLACE FUNCTION f_get_version_number(in_cvs_tag VARCHAR(32)) 
	RETURNS INTEGER AS $$

DECLARE
	mj_vrs CHAR(3) ;
	md_vrs CHAR(3) ;
	mn_vrs CHAR(3) ;

BEGIN
  	mj_vrs := TO_CHAR((SPLIT_PART(SUBSTRING(in_cvs_tag FROM (POSITION('_' IN in_cvs_tag)+1) FOR (CHAR_LENGTH(in_cvs_tag))), '-', 1)::INT), '00') ;
	md_vrs := TO_CHAR((SPLIT_PART(in_cvs_tag, '-', 2)::INT), '00') ;
	mn_vrs := TO_CHAR((SPLIT_PART(in_cvs_tag, '-', 3)::INT), '00') ;
	
	RETURN (TRIM(mj_vrs) || TRIM(md_vrs) || TRIM(mn_vrs))::INT ;
END ;
$$ LANGUAGE plpgsql ;


