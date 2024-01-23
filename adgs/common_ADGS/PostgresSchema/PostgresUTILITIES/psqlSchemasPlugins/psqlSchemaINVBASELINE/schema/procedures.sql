/*

Copyright 2010-2020, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S. Databases schema procedures script $

	$Id$

	$Author$

	$Log$
*/





--------------------------------------------------------------------------------
--
--    	  	  	  	  	  	  	  PROCEDURES
--
--------------------------------------------------------------------------------


/* ------------------------------
	p_inv_x_baselines_insert
*/ ------------------------------
CREATE OR REPLACE FUNCTION p_inv_x_baselines_insert(
	in_invid BIGINT) RETURNS INTEGER AS $$

DECLARE
	bsl_curr_val_id INTEGER ;
	excp_msg VARCHAR(128) ;
	
BEGIN
	-- Check if the baseline variable is setted
	IF ((SELECT count(*) FROM t_dbparameters WHERE parameter = 'baseline_curr_val') = 0) THEN
		RAISE EXCEPTION 'p_inv_x_baselines_insert: no value setted for baseline_curr_val into t_dbparameters' ;
	END IF;
	
	-- Get the id of the baseline setted
	bsl_curr_val_id := (SELECT value_int FROM t_dbparameters WHERE parameter = 'baseline_curr_val') ;
	
	-- Check if the id is setted a valid id
	IF ((SELECT count(*) FROM t_inv_baselines WHERE id_inv_baseline = bsl_curr_val_id) = 0) THEN
		excp_msg := 'p_inv_x_baselines_insert: ' || bsl_curr_val_id || ' - invalid baseline_curr_val value into t_dbparameters' ;
		RAISE EXCEPTION '%', excp_msg ;
	END IF ;
	
	-- Insert the relation into t_inv_x_baselines
	INSERT INTO t_inv_x_baselines (
		inv_id,
		inv_baseline_id )
	VALUES (
		in_invid,
		bsl_curr_val_id
	) ;
	
	RETURN 0 ;
END ;
$$ LANGUAGE plpgsql ;


