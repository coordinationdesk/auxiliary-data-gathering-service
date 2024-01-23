/*

Copyright 2010-2020, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S. Databases schema indexes script $

	$Id$

	$Author$

	$Log$
*/





--------------------------------------------------------------------------------
--
--    	  	  	  	  	  	  	  INDEXES
--
--------------------------------------------------------------------------------

/* ------------------------------
	t_inv_x_baselines
*/ ------------------------------
CREATE INDEX ix_t_inv_x_baselines01 ON t_inv_x_baselines USING BTREE (inv_id) ;
CREATE INDEX ix_t_inv_x_baselines02 ON t_inv_x_baselines USING BTREE (inv_baseline_id) ;


