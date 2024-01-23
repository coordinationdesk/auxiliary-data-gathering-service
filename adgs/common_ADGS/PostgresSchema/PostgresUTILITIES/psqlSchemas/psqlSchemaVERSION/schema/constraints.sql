/*

Copyright 2010-2015, Advanced Computer Systems , Inc.
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

------------------------------
--	t_schemamodificationtypes
------------------------------
ALTER TABLE t_schemamodificationtypes
	ADD CONSTRAINT uq_t_schemamodificationtypes01
		UNIQUE (modificationtype) ;

------------------------------
--	t_schematypes
------------------------------
ALTER TABLE t_schematypes
	ADD CONSTRAINT uq_schematypes01
		UNIQUE (schematype) ;

-----------------------------
--	t_schema
------------------------------
ALTER TABLE t_schema 
	ADD CONSTRAINT uq_t_schema_version01
		UNIQUE (version);


