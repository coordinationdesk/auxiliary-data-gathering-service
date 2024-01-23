/*

Copyright 2010-2020, Advanced Computer Systems , Inc.
Via Della Bufalotta, 378 - 00139 Roma - Italy
http://www.acsys.itA.C.S. - Advanced Computer Systems S.p.A.

This is UNPUBLISHED PROPRIETARY SOURCE CODE of Advanced Computer Systems, S.p.A.;
the contents of this file may not be disclosed to third parties, copied or
duplicated in any form, in whole or in part, without the prior written
permission of Advanced Computer Systems, S.p.A.


	$Prod: A.C.S. Databases schema views script $

	$Id$

	$Author$

	$Log$
*/





--------------------------------------------------------------------------------
--
--                                  VIEWS
--
--------------------------------------------------------------------------------

/* ------------------------------
	v_filetypes_x_stylesheets
*/ ------------------------------
CREATE OR REPLACE VIEW v_filetypes_x_stylesheets (
	filetype,
	stylesheet,
	outputtype,
	fileextension )
AS SELECT
	ft.filetype,
	cs.name,
	ot.outputtype,
	fs.fileextension
FROM
	t_filetypes_x_stylesheets fs
	INNER JOIN t_filetypes ft
		ON fs.filetype_id = ft.id_filetype
	INNER JOIN t_configurationstylesheets cs
		ON fs.stylesheet_id = cs.id
	INNER JOIN t_stylesheetoutputtypes ot
		ON fs.outputtype_id = ot.id
ORDER BY
	filetype,
	stylesheet,
	outputtype,
	fileextension
;


