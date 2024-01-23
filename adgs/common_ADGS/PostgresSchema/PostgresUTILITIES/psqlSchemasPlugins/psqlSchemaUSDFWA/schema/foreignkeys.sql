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
	Revision 1.2  2013/09/10 09:27:55  achnat
	Moved (and modified) p_insert_inventoryevents procedure from USDFWA to PDS.
	
*/





--------------------------------------------------------------------------------
--
--    	  	  	  	  	  	  	FOREIGN KEYS
--
--------------------------------------------------------------------------------

/* ------------------------------
	t_usdfwa
*/ ------------------------------
ALTER TABLE t_usdfwa 
	ADD CONSTRAINT fk_fname_fver
		FOREIGN KEY (filename, fileversion)
			REFERENCES t_inventory (filename, fileversion) 
				ON DELETE CASCADE ;



/* ------------------------------
	t_usdfwa_delivery_details
*/ ------------------------------
ALTER TABLE t_usdfwa_delivery_details 
	ADD CONSTRAINT fk_usdfwa_request
		FOREIGN KEY (usdfwa_id)
			REFERENCES t_usdfwa (id_usdfwa) 
				ON DELETE CASCADE ;



/* ------------------------------
	t_inventorylinks
*/ ------------------------------
ALTER TABLE t_inventorylinks 
	ADD CONSTRAINT fk_invlinks_orig 
		FOREIGN KEY (orig_filename, orig_fileversion)
			REFERENCES t_inventory (filename, fileversion) 
				ON DELETE CASCADE ;

ALTER TABLE t_inventorylinks 
	ADD CONSTRAINT fk_invlinks_dest 
		FOREIGN KEY (dest_filename, dest_fileversion)
			REFERENCES t_inventory (filename, fileversion) 
				ON DELETE CASCADE ;



/* ------------------------------
	t_inventorylinksconf
*/ ------------------------------
ALTER TABLE t_inventorylinksconf 
	ADD CONSTRAINT fk_invlinkconf_ftype1
		FOREIGN KEY (orig_filetype)
			REFERENCES t_filetypes (filetype) 
				ON DELETE CASCADE ;

ALTER TABLE t_inventorylinksconf 
	ADD CONSTRAINT fk_invlinkconf_ftype2
		FOREIGN KEY (dest_filetype)
			REFERENCES t_filetypes (filetype) 
				ON DELETE CASCADE ;


