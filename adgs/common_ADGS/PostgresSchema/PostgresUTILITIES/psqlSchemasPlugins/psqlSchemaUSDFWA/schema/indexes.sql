/*

Copyright 2010-2020, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S. Databases schema indexes script $

	$Id$

	$Author$

	$Log$
*/





--------------------------------------------------------------------------------
--
--    	  	  	  	  	  	  	  INDEXES
--
--------------------------------------------------------------------------------

/* ------------------------------
	t_usdfwa
*/ ------------------------------
CREATE INDEX ix_t_usdfwa01 ON t_usdfwa USING BTREE (filename, fileversion) ;


/* ------------------------------
	t_usdfwa_delivery_details
*/ ------------------------------
CREATE INDEX ix_t_usdfwa_del_det01 ON t_usdfwa_delivery_details USING BTREE (usdfwa_id) ;


/* ------------------------------
	t_inventorylinks
*/ ------------------------------
CREATE INDEX ix_t_inventorylinks01 ON t_inventorylinks USING BTREE (orig_filename, orig_fileversion) ;
CREATE INDEX ix_t_inventorylinks02 ON t_inventorylinks USING BTREE (dest_filename, dest_fileversion) ;


/* ------------------------------
	t_inventorylinksconf
*/ ------------------------------
CREATE INDEX ix_t_inventorylinksconf01 ON t_inventorylinksconf USING BTREE (orig_filetype) ;
CREATE INDEX ix_t_inventorylinksconf02 ON t_inventorylinksconf USING BTREE (dest_filetype) ;


