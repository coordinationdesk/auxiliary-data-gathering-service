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
	Revision 1.2  2013/05/16 13:39:09  achnat
	Added t_cr_x_filetypes table
	
	Revision 1.1.1.1  2013/03/19 10:40:09  achnat
	Import psqlSchemaCR
	
*/





--------------------------------------------------------------------------------
--
--    	  	  	  	  	  	  	  FOREIGN KEYS
--
--------------------------------------------------------------------------------


/* ------------------------------
	t_crgroups
*/ ------------------------------
ALTER TABLE t_crgroups 
	ADD CONSTRAINT fk_crgroups01
		FOREIGN KEY (virtual_filetype_id) REFERENCES t_filetypes (id_filetype) 
			ON DELETE CASCADE ;



/* ------------------------------
	t_crrelations
*/ ------------------------------
ALTER TABLE t_crrelations 
	ADD CONSTRAINT fk_crrelations01
		FOREIGN KEY (crgroup_id) REFERENCES t_crgroups (id_crgroup) 
			ON DELETE CASCADE ;

ALTER TABLE t_crrelations 
	ADD CONSTRAINT fk_crrelations02
		FOREIGN KEY (repository_id) REFERENCES t_repositories (id_repository) 
			ON DELETE CASCADE ;

ALTER TABLE t_crrelations 
	ADD CONSTRAINT fk_crrelations03
		FOREIGN KEY (distributionrule_id) REFERENCES t_distributionrules (id_distributionrule) 
			ON DELETE CASCADE ;



/* ------------------------------
	t_crtemporaries
*/ ------------------------------
ALTER TABLE t_crtemporaries 
	ADD CONSTRAINT fk_crtemporaries01
		FOREIGN KEY (inventory_id) REFERENCES t_inventory (id_inv) 
			ON DELETE CASCADE ;

ALTER TABLE t_crtemporaries 
	ADD CONSTRAINT fk_crtemporaries02
		FOREIGN KEY (crgroup_id) REFERENCES t_crgroups (id_crgroup) 
			ON DELETE CASCADE ;



/* ------------------------------
	t_cr_x_filetypes
*/ ------------------------------
ALTER TABLE t_cr_x_filetypes 
	ADD CONSTRAINT fk_cr_x_filetypes01
		FOREIGN KEY (crgroup_id) REFERENCES t_crgroups (id_crgroup) 
			ON DELETE CASCADE ;

ALTER TABLE t_cr_x_filetypes 
	ADD CONSTRAINT fk_cr_x_filetypes02
		FOREIGN KEY (filetype_id) REFERENCES t_filetypes (id_filetype) 
			ON DELETE CASCADE ;


