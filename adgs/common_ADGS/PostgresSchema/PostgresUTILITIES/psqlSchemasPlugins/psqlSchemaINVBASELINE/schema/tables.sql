/*

Copyright 2010-2020, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S. Databases schema tables script $

	$Id$

	$Author$

	$Log$
*/





--------------------------------------------------------------------------------
--
--    	  	  	  	  	  	  	  TABLES
--
--------------------------------------------------------------------------------

/* ------------------------------
	t_inv_baselines
*/ ------------------------------
CREATE TABLE t_inv_baselines (
	id_inv_baseline SERIAL NOT NULL,
	baseline VARCHAR(24) NOT NULL
) ;
ALTER TABLE t_inv_baselines 
	ADD CONSTRAINT pk_t_inv_baselines
		PRIMARY KEY (id_inv_baseline) ;



/* ------------------------------
	t_inv_x_baselines
*/ ------------------------------
CREATE TABLE t_inv_x_baselines (
	id_inv_x_baseline BIGSERIAL NOT NULL,
	inv_id BIGINT NOT NULL,
	inv_baseline_id INTEGER NOT NULL
) ;
ALTER TABLE t_inv_x_baselines 
	ADD CONSTRAINT pk_t_inv_x_baselines
		PRIMARY KEY (id_inv_x_baseline) ;


