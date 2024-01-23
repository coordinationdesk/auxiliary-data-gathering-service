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
*/





--------------------------------------------------------------------------------
--
--    	  	  	  	  	  	  	 CONSTRAINTS
--
--------------------------------------------------------------------------------



-------------------------------------------------
--------------- CHECK CONSTRAINTS ---------------
-------------------------------------------------

ALTER TABLE t_inventorylinks 
	ADD CONSTRAINT ck_invlinks_orig_dest
		CHECK ((orig_filename != dest_filename ) OR (orig_fileversion != dest_fileversion )) ;





-------------------------------------------------
--------------- UNIQUE CONSTRAINTS --------------
-------------------------------------------------

ALTER TABLE t_inventorylinksconf 
	ADD CONSTRAINT uq_t_invlinksconf
		UNIQUE (orig_filetype, dest_filetype, algotype) ;

