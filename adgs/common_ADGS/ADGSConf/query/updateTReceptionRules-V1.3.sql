UPDATE t_receptionrules
	SET repository_id = (SELECT id_repository FROM t_repositories WHERE symbolicname = 'S1BPRIP DLR Rep'),
	    filetype_id   = (SELECT id_filetype   FROM t_filetypes    WHERE filetype     = 'S1BPRIP_AUX')
	WHERE filetype_id   = (SELECT id_filetype   FROM t_filetypes    WHERE filetype = 'S1PRIP_AUX') AND
	      repository_id = (SELECT id_repository FROM t_repositories WHERE symbolicname = 'S1PRIP Repository');

UPDATE t_receptionrules
	SET reception_tt_id = (SELECT id_tasktable from t_tasktables where name = 'auxtecReception')
	WHERE filetype_id   = (SELECT id_filetype   FROM t_filetypes    WHERE filetype = 'AUX_TEC_OR') AND
	      repository_id = (SELECT id_repository FROM t_repositories WHERE symbolicname = 'AGS TEC Repository');
