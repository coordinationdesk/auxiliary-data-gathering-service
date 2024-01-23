UPDATE t_receptionrules
	SET hostname = 'pds-imp1-1'
WHERE filetype_id   = (SELECT id_filetype   FROM t_filetypes    WHERE filetype = 'EUM_AUX') AND
	  repository_id = (SELECT id_repository FROM t_repositories WHERE symbolicname = 'EUM Repository');

UPDATE t_receptionrules
	SET hostname = 'pds-imp1-1'
WHERE filetype_id   = (SELECT id_filetype   FROM t_filetypes    WHERE filetype = 'NPPF_AUX') AND
	  repository_id = (SELECT id_repository FROM t_repositories WHERE symbolicname = 'EUM Repository');

UPDATE t_receptionrules
	SET (hostname, localpath) = ('pds-imp1-2', '/data/FromExt')
WHERE filetype_id   = (SELECT id_filetype   FROM t_filetypes    WHERE filetype = 'NOAA_1KM_IMSDLY') AND
	  repository_id = (SELECT id_repository FROM t_repositories WHERE symbolicname = 'NOAA Public Datasets');

UPDATE t_receptionrules
	SET (hostname, localpath) = ('pds-imp1-2', '/data/FromExt')
WHERE filetype_id   = (SELECT id_filetype   FROM t_filetypes    WHERE filetype = 'OSISAF_ICE') AND
	  repository_id = (SELECT id_repository FROM t_repositories WHERE symbolicname = 'OSI SAF Repository');

UPDATE t_receptionrules
	SET (hostname, localpath) = ('pds-imp1-2', '/data/FromExt')
WHERE filetype_id   = (SELECT id_filetype   FROM t_filetypes    WHERE filetype = 'OSIICE_SYSTEM_N') AND
	  repository_id = (SELECT id_repository FROM t_repositories WHERE symbolicname = 'S3 OSI Repository');

UPDATE t_receptionrules
	SET (hostname, localpath) = ('pds-imp1-2', '/data/FromExt')
WHERE filetype_id   = (SELECT id_filetype   FROM t_filetypes    WHERE filetype = 'OSIICE_SYSTEM_S') AND
	  repository_id = (SELECT id_repository FROM t_repositories WHERE symbolicname = 'S3 OSI Repository');

UPDATE t_receptionrules
	SET (hostname, localpath) = ('pds-imp1-2', '/data/FromExt')
WHERE filetype_id   = (SELECT id_filetype   FROM t_filetypes    WHERE filetype = 'OSIICE_RPRO_21N') AND
	  repository_id = (SELECT id_repository FROM t_repositories WHERE symbolicname = 'S3 OSI ReproRep');

UPDATE t_receptionrules
	SET (hostname, localpath) = ('pds-imp1-2', '/data/FromExt')
WHERE filetype_id   = (SELECT id_filetype   FROM t_filetypes    WHERE filetype = 'OSIICE_RPRO_21S') AND
	  repository_id = (SELECT id_repository FROM t_repositories WHERE symbolicname = 'S3 OSI ReproRep');

UPDATE t_receptionrules
	SET (hostname, localpath) = ('pds-imp1-3', '/data/FromExt')
WHERE filetype_id   = (SELECT id_filetype   FROM t_filetypes    WHERE filetype = 'AUX_WND_OR') AND
	  repository_id = (SELECT id_repository FROM t_repositories WHERE symbolicname = 'ECMWF EXT Repository');

UPDATE t_receptionrules
	SET (hostname, localpath) = ('pds-imp1-4', '/data/FromExt')
WHERE filetype_id   = (SELECT id_filetype   FROM t_filetypes    WHERE filetype = 'AUX_WAV_OR') AND
	  repository_id = (SELECT id_repository FROM t_repositories WHERE symbolicname = 'IFREMER Repository');

UPDATE t_receptionrules
        SET (hostname, reception_tt_id) = ('pds-imp2-1', (SELECT id_tasktable from t_tasktables where name = 'defaultReception'))
WHERE filetype_id   = (SELECT id_filetype   FROM t_filetypes    WHERE filetype = 'CAMSFC_VIR') AND
          repository_id = (SELECT id_repository FROM t_repositories WHERE symbolicname = 'ECMWF CAMSFO Rep00');

UPDATE t_receptionrules
        SET (hostname, reception_tt_id) = ('pds-imp2-1', (SELECT id_tasktable from t_tasktables where name = 'defaultReception'))
WHERE filetype_id   = (SELECT id_filetype   FROM t_filetypes    WHERE filetype = 'CAMSFC_VIR') AND
          repository_id = (SELECT id_repository FROM t_repositories WHERE symbolicname = 'ECMWF CAMSFO Rep12');

UPDATE t_receptionrules
	SET (hostname, localpath) = ('pds-imp2-2', '/data/S2TRO')
WHERE filetype_id   = (SELECT id_filetype   FROM t_filetypes    WHERE filetype = 'ECMWF_TRO_OR') AND
	  repository_id = (SELECT id_repository FROM t_repositories WHERE symbolicname = 'ECMWF TRO Repository');

UPDATE t_receptionrules
	SET hostname = 'pds-imp1-5'
WHERE filetype_id   = (SELECT id_filetype   FROM t_filetypes    WHERE filetype = 'AUX_ECMWFD_OR') AND
	  repository_id = (SELECT id_repository FROM t_repositories WHERE symbolicname = 'ECMWF EXT Repository');
