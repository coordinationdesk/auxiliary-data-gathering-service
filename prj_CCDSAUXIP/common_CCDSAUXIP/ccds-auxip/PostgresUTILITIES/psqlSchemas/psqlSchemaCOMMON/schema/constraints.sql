/*

 Exprivia S.p.A.
 Via Della Bufalotta, 378 - 00139 Roma - Italy
 http://www.exprivia.it

 This is UNPUBLISHED PROPRIETARY SOURCE CODE of Exprivia S.p.A.;
 the contents of this file may not be disclosed to third parties, copied
 or duplicated in any form, in whole or in part, without the prior written
 permission of Exprivia S.p.A.

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
        t_stati
*/ ------------------------------
ALTER TABLE t_stati
	ADD CONSTRAINT uq_stati01
		UNIQUE (status) ;



