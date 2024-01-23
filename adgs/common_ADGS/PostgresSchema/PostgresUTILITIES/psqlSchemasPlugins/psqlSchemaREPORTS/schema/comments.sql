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
	Revision 1.1  2014/05/05 14:48:51  achnat
	Added comments for schema objects
	
*/





--------------------------------------------------------------------------------
--                                                                            --
--                                  COMMENTS                                  --
--                                                                            --
--------------------------------------------------------------------------------


/* ------------------------------
	t_filetypes_x_stylesheets
*/ ------------------------------
COMMENT ON TABLE t_filetypes_x_stylesheets IS 'Contains the list of relations between a filetype, the stylesheet and the output type, for the statistical reprts visualization';
COMMENT ON COLUMN t_filetypes_x_stylesheets.id IS 'Record identifier (Primary Key)';
COMMENT ON COLUMN t_filetypes_x_stylesheets.filetype_id IS 'Id of referenced filetype (t_filetypes)';
COMMENT ON COLUMN t_filetypes_x_stylesheets.stylesheet_id IS 'Id of referenced stylesheet (t_configurationstylesheets)';
COMMENT ON COLUMN t_filetypes_x_stylesheets.outputtype_id IS 'Id of referenced output type (t_stylesheetoutputtypes)';
COMMENT ON COLUMN t_filetypes_x_stylesheets.fileextension IS 'File extension for generated report (eg. xml or EOF)';
COMMENT ON SEQUENCE t_filetypes_x_stylesheets_id_seq IS 'Used to generate the primary keys of the t_filetypes_x_stylesheets table';


/* ------------------------------
	t_lastacknowledge
*/ ------------------------------
COMMENT ON TABLE t_lastacknowledge IS '';
COMMENT ON COLUMN t_lastacknowledge.id IS 'Record identifier (Primary Key)';
COMMENT ON COLUMN t_lastacknowledge.applicationname IS '';
COMMENT ON COLUMN t_lastacknowledge.applicationdata IS '';
COMMENT ON COLUMN t_lastacknowledge.tstamp IS '';
COMMENT ON SEQUENCE t_lastacknowledge_id_seq IS 'Used to generate the primary keys of t_lastacknowledge table';


/* ------------------------------
	t_stylesheetoutputtypes
*/ ------------------------------
COMMENT ON TABLE t_stylesheetoutputtypes IS 'Contains the list of output types for reports generated by a stylesheet (eg: XML, HTML)';
COMMENT ON COLUMN t_stylesheetoutputtypes.id IS 'Record identifier (Primary Key)';
COMMENT ON COLUMN t_stylesheetoutputtypes.outputtype IS 'Type of output (eg: XML, HTML)';


/* ------------------------------
	v_filetypes_x_stylesheets
*/ ------------------------------
COMMENT ON VIEW v_filetypes_x_stylesheets IS 'Joins t_filetypes_x_stylesheets, t_filetypes, t_configurationstylesheets and t_stylesheetoutputtypes tables.';

