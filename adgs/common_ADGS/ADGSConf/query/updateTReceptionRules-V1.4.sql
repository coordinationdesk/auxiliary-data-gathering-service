UPDATE t_receptionrules
	SET filetype_id = (SELECT id_filetype FROM t_filetypes where filetype = 'ECMWF_TRO_OR'),
            discoveryplugin_id = (SELECT id_discoveryplugin FROM t_discoveryplugins WHERE name = 'ftp_imp_pl_adgs_date')
	WHERE filetype_id   = (SELECT id_filetype   FROM t_filetypes    WHERE filetype = 'ECMWF_TRO') AND
	      repository_id = (SELECT id_repository FROM t_repositories WHERE symbolicname = 'ECMWF TRO Repository');
UPDATE t_receptionrules
	SET reception_tt_id = (SELECT id_tasktable from t_tasktables where name = 'auxwndReception')
	WHERE filetype_id   = (SELECT id_filetype   FROM t_filetypes    WHERE filetype = 'AUX_WND_OR');

