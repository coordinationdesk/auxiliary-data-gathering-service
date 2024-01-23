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
*/





--------------------------------------------------------------------------------
--
--    	  	  	  	  	  	  	  TRIGGERS
--
--------------------------------------------------------------------------------


/* ------------------------------
	tf_inventory_baselines
*/ ------------------------------
CREATE TRIGGER tf_inventory_baselines AFTER INSERT
	ON t_inventory 
		FOR EACH ROW EXECUTE PROCEDURE tf_invxbaselines_insert() ;


