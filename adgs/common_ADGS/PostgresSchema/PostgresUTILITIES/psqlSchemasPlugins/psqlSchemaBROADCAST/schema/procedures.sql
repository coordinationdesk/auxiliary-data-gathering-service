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
--                                                                            --
--                                 PROCEDURES                                 --
--                                                                            --
--------------------------------------------------------------------------------

/* ------------------------------
	p_events
*/ ------------------------------
CREATE OR REPLACE PROCEDURE p_events(
	payload TEXT) AS $$
	
BEGIN
	PERFORM pg_notify(current_database(), payload) ;
	
END ;
$$ LANGUAGE plpgsql ;


