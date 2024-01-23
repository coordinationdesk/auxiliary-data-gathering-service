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
	Revision 1.1.1.1  2013/10/08 15:07:46  achnat
	psqlSchemaCOVERAGE
	
*/





--------------------------------------------------------------------------------
--
--    	  	  	  	  	  	  	  FOREIGN KEYS
--
--------------------------------------------------------------------------------

/* ------------------------------
	t_cov_periods
*/ ------------------------------
ALTER TABLE t_cov_periods 
	ADD CONSTRAINT fk_cov_periods01
		FOREIGN KEY (coveragerule_id) REFERENCES t_cov_rules (id_coveragerule) 
			ON DELETE CASCADE ;



/* ------------------------------
	t_cov_relevancies
*/ ------------------------------
ALTER TABLE t_cov_relevancies 
	ADD CONSTRAINT fk_cov_relevancies01
		FOREIGN KEY (period_id) REFERENCES t_cov_periods (id_interval) 
			ON DELETE CASCADE ;

DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.columns WHERE table_name = 't_inventory' AND column_name = 'id') = 1
	THEN
		ALTER TABLE t_cov_relevancies 
			ADD CONSTRAINT fk_cov_relevancies02
				FOREIGN KEY (inv_id) REFERENCES t_inventory (id) 
					ON DELETE CASCADE ;
	ELSE
		ALTER TABLE t_cov_relevancies 
			ADD CONSTRAINT fk_cov_relevancies02
				FOREIGN KEY (inv_id) REFERENCES t_inventory (id_inv) 
					ON DELETE CASCADE ;
	END IF ;
END $$;


/* ------------------------------
	t_cov_x_filetypes
*/ ------------------------------
ALTER TABLE t_cov_x_filetypes 
	ADD CONSTRAINT fk_coverage_x_filetypes01
		FOREIGN KEY (coveragerule_id) REFERENCES t_cov_rules (id_coveragerule) 
			ON DELETE CASCADE ;

DO $$
BEGIN
	IF (SELECT count(*) FROM information_schema.columns WHERE table_name = 't_filetypes' AND column_name = 'id') = 1
	THEN
		ALTER TABLE t_cov_x_filetypes 
			ADD CONSTRAINT fk_coverage_x_filetypes02
				FOREIGN KEY (filetype_id) REFERENCES t_filetypes (id) 
					ON DELETE CASCADE ;
	ELSE
		ALTER TABLE t_cov_x_filetypes 
			ADD CONSTRAINT fk_coverage_x_filetypes02
				FOREIGN KEY (filetype_id) REFERENCES t_filetypes (id_filetype) 
					ON DELETE CASCADE ;
	END IF ;
END $$;




