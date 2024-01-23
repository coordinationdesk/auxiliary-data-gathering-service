/*

Copyright 2010-2020, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S. Databases schema comments script $

	$Id$

	$Author$

	$Log$
	Revision 1.1  2014/05/07 12:45:57  achnat
	Added foreignkeys indexes. Added comments on schema objects.
	
*/





--------------------------------------------------------------------------------
--                                                                            --
--                                  COMMENTS                                  --
--                                                                            --
--------------------------------------------------------------------------------

/* ------------------------------
	t_usdfwa
*/ ------------------------------
COMMENT ON TABLE t_usdfwa IS '';
COMMENT ON COLUMN t_usdfwa.id_usdfwa IS 'Record identifier (Primary Key)';
COMMENT ON COLUMN t_usdfwa.filename IS 'References filename on t_inventory table.';
COMMENT ON COLUMN t_usdfwa.fileversion IS 'References fileversion on t_inventory table.';
COMMENT ON COLUMN t_usdfwa.mmmcupdatefile IS '';
COMMENT ON COLUMN t_usdfwa.modificationdate IS '';
COMMENT ON SEQUENCE t_usdfwa_id_usdfwa_seq IS 'Used to generate the primary keys of the t_usdfwa table';


/* ------------------------------
	t_usdfwa_delivery_details
*/ ------------------------------
COMMENT ON TABLE t_usdfwa_delivery_details IS '';
COMMENT ON COLUMN t_usdfwa_delivery_details.id_delivery_details IS 'Record identifier (Primary Key)';
COMMENT ON COLUMN t_usdfwa_delivery_details.usdfwa_id IS '';
COMMENT ON COLUMN t_usdfwa_delivery_details.url IS '';
COMMENT ON COLUMN t_usdfwa_delivery_details.deliverydate IS '';
COMMENT ON COLUMN t_usdfwa_delivery_details.itemstatus IS '';
COMMENT ON COLUMN t_usdfwa_delivery_details.statusdetails IS '';
COMMENT ON SEQUENCE t_usdfwa_delivery_details_id_delivery_details_seq IS 'Used to generate the primary keys of the t_usdfwa_delivery_details table';


/* ------------------------------
	t_inventorylinks
*/ ------------------------------
COMMENT ON TABLE t_inventorylinks IS '';
COMMENT ON COLUMN t_inventorylinks.id IS 'Record identifier (Primary Key)';
COMMENT ON COLUMN t_inventorylinks.orig_filename IS 'References filename on t_inventory table.';
COMMENT ON COLUMN t_inventorylinks.orig_fileversion IS 'References fileversion on t_inventory table.';
COMMENT ON COLUMN t_inventorylinks.dest_filename IS 'References filename on t_inventory table.';
COMMENT ON COLUMN t_inventorylinks.dest_fileversion IS 'References fileversion on t_inventory table.';
COMMENT ON COLUMN t_inventorylinks.linkdate IS '';
COMMENT ON COLUMN t_inventorylinks.description IS '';
COMMENT ON SEQUENCE t_inventorylinks_id_seq IS 'Used to generate the primary keys of the t_inventorylinks table';


/* ------------------------------
	t_inventorylinksconf
*/ ------------------------------
COMMENT ON TABLE t_inventorylinksconf IS '';
COMMENT ON COLUMN t_inventorylinksconf.id IS 'Record identifier (Primary Key)';
COMMENT ON COLUMN t_inventorylinksconf.orig_filetype IS 'References filetype on t_filetypes table.';
COMMENT ON COLUMN t_inventorylinksconf.dest_filetype IS 'References filetype on t_filetypes table.';
COMMENT ON COLUMN t_inventorylinksconf.algotype IS '';
COMMENT ON COLUMN t_inventorylinksconf.description IS '';
COMMENT ON SEQUENCE t_inventorylinksconf_id_seq IS 'Used to generate the primary keys of the t_inventorylinksconf table';


