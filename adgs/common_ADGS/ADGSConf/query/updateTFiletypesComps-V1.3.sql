UPDATE t_filetypescomps
	SET pfiletype = (SELECT id_filetype FROM t_filetypes where filetype = 'S1FOS_AUX_TGZ')
	WHERE pfiletype = (SELECT id_filetype FROM t_filetypes where filetype = 'S2FOS_AUX_TGZ') and cfiletype = (SELECT id_filetype FROM t_filetypes where filetype = 'S1REP__MACP_');

UPDATE t_filetypescomps
	SET pfiletype = (SELECT id_filetype FROM t_filetypes where filetype = 'DBLTOOBJST_TBZ')
	WHERE pfiletype = (SELECT id_filetype FROM t_filetypes where filetype = 'AUXTOOBJST_TBZ') and cfiletype = (SELECT id_filetype FROM t_filetypes where filetype = 'S1MPL__RGTEF');

UPDATE t_filetypescomps
	SET pfiletype = (SELECT id_filetype FROM t_filetypes where filetype = 'DBLTOOBJST_TBZ')
	WHERE pfiletype = (SELECT id_filetype FROM t_filetypes where filetype = 'AUXTOOBJST_TBZ') and cfiletype = (SELECT id_filetype FROM t_filetypes where filetype = 'S1MPL_MANPRE');

UPDATE t_filetypescomps
	SET pfiletype = (SELECT id_filetype FROM t_filetypes where filetype = 'DBLTOOBJST_TBZ')
	WHERE pfiletype = (SELECT id_filetype FROM t_filetypes where filetype = 'AUXTOOBJST_TBZ') and cfiletype = (SELECT id_filetype FROM t_filetypes where filetype = 'S2MPL_MANPRE');

UPDATE t_filetypescomps
	SET pfiletype = (SELECT id_filetype FROM t_filetypes where filetype = 'DBLTOOBJST_TBZ')
	WHERE pfiletype = (SELECT id_filetype FROM t_filetypes where filetype = 'AUXTOOBJST_TBZ') and cfiletype = (SELECT id_filetype FROM t_filetypes where filetype = 'S2MPL__GTDEV');
