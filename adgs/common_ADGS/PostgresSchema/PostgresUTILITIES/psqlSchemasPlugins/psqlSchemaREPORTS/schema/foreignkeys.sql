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
	Revision 1.2  2013/03/27 09:54:21  achnat
	Moved t_filetypes_stylesheets and t_stylesheetoutputtypes in REPORT schema
	
*/





--------------------------------------------------------------------------------
--
--    	  	  	  	  	  	  	FOREIGN KEYS
--
--------------------------------------------------------------------------------

/* ------------------------------
	t_filetypes_x_stylesheets
*/ ------------------------------
ALTER TABLE t_filetypes_x_stylesheets 
	ADD CONSTRAINT fk_filetypes_x_stylesheets01
		FOREIGN KEY (filetype_id)
			REFERENCES t_filetypes (id_filetype) 
				ON DELETE CASCADE ;

ALTER TABLE t_filetypes_x_stylesheets 
	ADD CONSTRAINT fk_filetypes_x_stylesheets02
		FOREIGN KEY (stylesheet_id)
			REFERENCES t_configurationstylesheets (id) 
				ON DELETE CASCADE ;

ALTER TABLE t_filetypes_x_stylesheets 
	ADD CONSTRAINT fk_filetypes_x_stylesheets03
		FOREIGN KEY (outputtype_id)
			REFERENCES t_stylesheetoutputtypes (id) 
				ON DELETE CASCADE  ;


