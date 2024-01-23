UPDATE t_receptionrules
	SET discoveryplugin_id = (SELECT id_discoveryplugin FROM t_discoveryplugins WHERE name = 'ftp_imp_pl_adgs')
	WHERE filetype_id = (SELECT id_filetype FROM t_filetypes WHERE filetype = 'S1FOS_AUX_EOF') AND
		  repository_id = (SELECT id_repository FROM t_repositories WHERE symbolicname = 'S1 FOS Repository');
UPDATE t_receptionrules
	SET discoveryplugin_id = (SELECT id_discoveryplugin FROM t_discoveryplugins WHERE name = 'ftp_imp_pl_adgs')
	WHERE filetype_id = (SELECT id_filetype FROM t_filetypes WHERE filetype = 'S1FOS_AUX_TGZ') AND
		  repository_id = (SELECT id_repository FROM t_repositories WHERE symbolicname = 'S1 FOS Repository');
UPDATE t_receptionrules
	SET discoveryplugin_id = (SELECT id_discoveryplugin FROM t_discoveryplugins WHERE name = 'ftp_imp_pl_adgs')
	WHERE filetype_id = (SELECT id_filetype FROM t_filetypes WHERE filetype = 'S1FOS_AUX_DBL') AND
		  repository_id = (SELECT id_repository FROM t_repositories WHERE symbolicname = 'S1 FOS Repository');
UPDATE t_receptionrules
	SET discoveryplugin_id = (SELECT id_discoveryplugin FROM t_discoveryplugins WHERE name = 'ftp_imp_pl_adgs')
	WHERE filetype_id = (SELECT id_filetype FROM t_filetypes WHERE filetype = 'AUX_OBMEMC') AND
		  repository_id = (SELECT id_repository FROM t_repositories WHERE symbolicname = 'S1 FOS Repository');
UPDATE t_receptionrules
	SET discoveryplugin_id = (SELECT id_discoveryplugin FROM t_discoveryplugins WHERE name = 'ftp_imp_pl_adgs')
	WHERE filetype_id = (SELECT id_filetype FROM t_filetypes WHERE filetype = 'S2FOS_AUX_EOF') AND
		  repository_id = (SELECT id_repository FROM t_repositories WHERE symbolicname = 'S2 FOS Repository');
UPDATE t_receptionrules
	SET discoveryplugin_id = (SELECT id_discoveryplugin FROM t_discoveryplugins WHERE name = 'ftp_imp_pl_adgs')
	WHERE filetype_id = (SELECT id_filetype FROM t_filetypes WHERE filetype = 'S2FOS_AUX_TGZ') AND
		  repository_id = (SELECT id_repository FROM t_repositories WHERE symbolicname = 'S2 FOS Repository');
UPDATE t_receptionrules
	SET discoveryplugin_id = (SELECT id_discoveryplugin FROM t_discoveryplugins WHERE name = 'ftp_imp_pl_adgs')
	WHERE filetype_id = (SELECT id_filetype FROM t_filetypes WHERE filetype = 'S2FOS_AUX_DBL') AND
		  repository_id = (SELECT id_repository FROM t_repositories WHERE symbolicname = 'S2 FOS Repository');
UPDATE t_receptionrules
	SET (hostname, localpath, reception_tt_id) = ('adgs-dns', '/data/fromS2MCP', (SELECT id_tasktable from t_tasktables where name = 'defaultReception'))
    WHERE filetype_id = (SELECT id_filetype FROM t_filetypes WHERE filetype = 'S2GIPP_FILE') AND
		  repository_id = (SELECT id_repository FROM t_repositories WHERE symbolicname = 'S2GIPP Repository');