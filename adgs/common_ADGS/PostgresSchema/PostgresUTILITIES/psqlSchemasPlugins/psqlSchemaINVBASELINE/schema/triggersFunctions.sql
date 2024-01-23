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
*/





--------------------------------------------------------------------------------
--
--    	  	  	  	  	   	   TRIGGERS FUNCTIONS
--
--------------------------------------------------------------------------------


/* ------------------------------
	tf_filetype_insert
*/ ------------------------------
CREATE OR REPLACE FUNCTION tf_invxbaselines_insert() RETURNS TRIGGER AS $$
DECLARE
  	status INTEGER ;
BEGIN
	status := p_inv_x_baselines_insert(NEW.id_inv) ;
  	RETURN NEW ;
END ;
$$ LANGUAGE plpgsql ;


