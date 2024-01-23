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
	t_inv_baselines
*/ ------------------------------
ALTER TABLE t_inv_baselines 
	ADD CONSTRAINT uq_inventorybaselines01
		UNIQUE (baseline) ;


/* ------------------------------
	t_inv_x_baselines
*/ ------------------------------
ALTER TABLE t_inv_x_baselines 
	ADD CONSTRAINT uq_inv_x_baselines01
		UNIQUE (inv_id, inv_baseline_id) ;


