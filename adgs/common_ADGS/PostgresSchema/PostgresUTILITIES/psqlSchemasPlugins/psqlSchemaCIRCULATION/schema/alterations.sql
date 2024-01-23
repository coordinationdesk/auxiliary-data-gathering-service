/*

Copyright 2010-2015, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S. Databases schema alteration script $

	$Id$

	$Author$

	$Log$
*/





--------------------------------------------------------------------------------
--
--    	  	  	  	  	  	  	  ALTERATIONS
--
--------------------------------------------------------------------------------

/* ------------------------------
	t_distributionqueue
*/ ------------------------------
ALTER TABLE t_distributionqueue ADD circulationqueue_id BIGINT ;

ALTER TABLE t_distributionqueue
	ADD CONSTRAINT fk_distribution_to_circulation
		FOREIGN KEY (circulationqueue_id)
			REFERENCES t_circulationsqueue (id)
				ON DELETE CASCADE ;

CREATE INDEX ix_distribution_to_circulation ON t_distributionqueue USING BTREE (circulationqueue_id) ;
