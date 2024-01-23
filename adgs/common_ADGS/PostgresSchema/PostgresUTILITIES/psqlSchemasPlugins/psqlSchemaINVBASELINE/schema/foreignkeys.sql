/*

Copyright 2010-2020, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S. Databases schema foreign keys script $

	$Id$

	$Author$

	$Log$
*/





--------------------------------------------------------------------------------
--
--    	  	  	  	  	  	  	  FOREIGN KEYS
--
--------------------------------------------------------------------------------


/* ------------------------------
	t_inv_x_baselines
*/ ------------------------------
ALTER TABLE t_inv_x_baselines 
	ADD CONSTRAINT fk_inv_x_baselines01
		FOREIGN KEY (inv_id) REFERENCES t_inventory (id_inv) 
			ON DELETE CASCADE ;

ALTER TABLE t_inv_x_baselines 
	ADD CONSTRAINT fk_inv_x_baselines02
		FOREIGN KEY (inv_baseline_id) REFERENCES t_inv_baselines (id_inv_baseline) 
			ON DELETE CASCADE ;


